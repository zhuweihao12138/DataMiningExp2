/*
 * Copyright (c) Meta Platforms, Inc. and affiliates.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef GEAXFRONTEND_COMMON_FOLLYARENA_H_
#define GEAXFRONTEND_COMMON_FOLLYARENA_H_

#include <boost/intrusive/list.hpp>
#include <boost/intrusive/slist.hpp>
#include "geax-front-end/common/FollyMemory.h"
#include "geax-front-end/common/FollyAlign.h"
#include "geax-front-end/common/FollyCheckedMath.h"

namespace folly::clone {

template <class Alloc>
struct ArenaAllocatorTraits;
template <class Alloc>
class Arena {
 public:
  explicit Arena(
      const Alloc& alloc,
      size_t minBlockSize = kDefaultMinBlockSize,
      size_t sizeLimit = kNoSizeLimit,
      size_t maxAlign = kDefaultMaxAlign)
      : allocAndSize_(alloc, minBlockSize),
        currentBlock_(blocks_.last()),
        ptr_(nullptr),
        end_(nullptr),
        totalAllocatedSize_(0),
        bytesUsed_(0),
        sizeLimit_(sizeLimit),
        maxAlign_(maxAlign) {
    if ((maxAlign_ & (maxAlign_ - 1)) || maxAlign_ > alignof(Block)) {
      throw std::invalid_argument("Invalid maxAlign: " + std::to_string(maxAlign_));
    }
  }

  ~Arena() {
    freeBlocks();
    freeLargeBlocks();
  }

  void* allocate(size_t size) {
    size = roundUp(size);
    bytesUsed_ += size;

    assert(ptr_ <= end_);
    if (__builtin_expect(!!((size_t)(end_ - ptr_) >= size), 1)) {
      // Fast path: there's enough room in the current block
      char* r = ptr_;
      ptr_ += size;
      assert(isAligned(r));
      return r;
    }

    if (canReuseExistingBlock(size)) {
      currentBlock_++;
      char* r = currentBlock_->start();
      ptr_ = r + size;
      end_ = r + blockGoodAllocSize() - sizeof(Block);
      assert(ptr_ <= end_);
      assert(isAligned(r));
      return r;
    }

    // Not enough room in the current block
    void* r = allocateSlow(size);
    assert(isAligned(r));
    return r;
  }

  void deallocate(void* /* p */, size_t = 0) {
    // Deallocate? Never!
  }

  // Transfer ownership of all memory allocated from "other" to "this".
  void merge(Arena&& other);

  void clear() {
    bytesUsed_ = 0;
    freeLargeBlocks(); // We don't reuse large blocks
    if (blocks_.empty()) {
      return;
    }
    currentBlock_ = blocks_.begin();
    char* start = currentBlock_->start();
    ptr_ = start;
    end_ = start + blockGoodAllocSize() - sizeof(Block);
    assert(ptr_ <= end_);
  }

  // Gets the total memory used by the arena
  size_t totalSize() const { return totalAllocatedSize_ + sizeof(Arena); }

  // Gets the total number of "used" bytes, i.e. bytes that the arena users
  // allocated via the calls to `allocate`. Doesn't include fragmentation, e.g.
  // if block size is 4KB and you allocate 2 objects of 3KB in size,
  // `bytesUsed()` will be 6KB, while `totalSize()` will be 8KB+.
  size_t bytesUsed() const { return bytesUsed_; }

  // not copyable or movable
  Arena(const Arena&) = delete;
  Arena& operator=(const Arena&) = delete;
  Arena(Arena&&) = delete;
  Arena& operator=(Arena&&) = delete;

 private:
  using AllocTraits =
      typename std::allocator_traits<Alloc>::template rebind_traits<char>;
  using BlockLink = boost::intrusive::slist_member_hook<>;

  struct alignas(max_align_v) Block {
    BlockLink link;

    char* start() { return reinterpret_cast<char*>(this + 1); }

    Block() = default;
    ~Block() = default;
  };

  constexpr size_t blockGoodAllocSize() {
    return ArenaAllocatorTraits<Alloc>::goodSize(
        alloc(), sizeof(Block) + minBlockSize());
  }

  struct alignas(max_align_v) LargeBlock {
    BlockLink link;
    const size_t allocSize;

    char* start() { return reinterpret_cast<char*>(this + 1); }

    LargeBlock(size_t s) : allocSize(s) {}
    ~LargeBlock() = default;
  };

  bool canReuseExistingBlock(size_t size) {
    if (size > minBlockSize()) {
      // We don't reuse large blocks
      return false;
    }
    if (blocks_.empty() || currentBlock_ == blocks_.last()) {
      // No regular blocks to reuse
      return false;
    }
    return true;
  }

  void freeBlocks() {
    blocks_.clear_and_dispose([this](Block* b) {
      b->~Block();
      AllocTraits::deallocate(
          alloc(), reinterpret_cast<char*>(b), blockGoodAllocSize());
    });
  }

  void freeLargeBlocks() {
    largeBlocks_.clear_and_dispose([this](LargeBlock* b) {
      auto size = b->allocSize;
      totalAllocatedSize_ -= size;
      b->~LargeBlock();
      AllocTraits::deallocate(alloc(), reinterpret_cast<char*>(b), size);
    });
  }

 public:
  static constexpr size_t kDefaultMinBlockSize = 4096 - sizeof(Block);
  static constexpr size_t kNoSizeLimit = 0;
  static constexpr size_t kDefaultMaxAlign = alignof(Block);
  static constexpr size_t kBlockOverhead = sizeof(Block);

 private:
  bool isAligned(uintptr_t address) const {
    return (address & (maxAlign_ - 1)) == 0;
  }
  bool isAligned(void* p) const {
    return isAligned(reinterpret_cast<uintptr_t>(p));
  }

  // Round up size so it's properly aligned
  size_t roundUp(size_t size) const {
    auto maxAl = maxAlign_ - 1;
    size_t realSize;
    if (!checked_add<size_t>(&realSize, size, maxAl)) {
      throw std::bad_alloc();
    }
    return realSize & ~maxAl;
  }

  // cache_last<true> makes the list keep a pointer to the last element, so we
  // have push_back() and constant time splice_after()
  typedef boost::intrusive::slist<
      Block,
      boost::intrusive::member_hook<Block, BlockLink, &Block::link>,
      boost::intrusive::constant_time_size<false>,
      boost::intrusive::cache_last<true>>
      BlockList;

  typedef boost::intrusive::slist<
      LargeBlock,
      boost::intrusive::member_hook<LargeBlock, BlockLink, &LargeBlock::link>,
      boost::intrusive::constant_time_size<false>,
      boost::intrusive::cache_last<true>>
      LargeBlockList;

  void* allocateSlow(size_t size);

  // Empty member optimization: package Alloc with a non-empty member
  // in case Alloc is empty (as it is in the case of SysAllocator).
  struct AllocAndSize : public Alloc {
    explicit AllocAndSize(const Alloc& a, size_t s)
        : Alloc(a), minBlockSize(s) {}

    size_t minBlockSize;
  };

  size_t minBlockSize() const { return allocAndSize_.minBlockSize; }
  Alloc& alloc() { return allocAndSize_; }
  const Alloc& alloc() const { return allocAndSize_; }

  AllocAndSize allocAndSize_;
  BlockList blocks_;
  typename BlockList::iterator currentBlock_;
  LargeBlockList largeBlocks_;
  char* ptr_;
  char* end_;
  size_t totalAllocatedSize_;
  size_t bytesUsed_;
  const size_t sizeLimit_;
  const size_t maxAlign_;
};

/**
 * By default, don't pad the given size.
 */
template <class Alloc>
struct ArenaAllocatorTraits {
  static size_t goodSize(const Alloc& /* alloc */, size_t size) { return size; }
};

// template <>
// struct ArenaAllocatorTraits<SysAllocator<char>> {
//   static size_t goodSize(const SysAllocator<char>& /* alloc */, size_t size) {
//     return goodMallocSize(size);
//   }
// };

/**
 * Arena that uses the system allocator (malloc / free)
 */
class SysArena : public Arena<SysAllocator<char>> {
 public:
  explicit SysArena(
      size_t minBlockSize = kDefaultMinBlockSize,
      size_t sizeLimit = kNoSizeLimit,
      size_t maxAlign = kDefaultMaxAlign)
      : Arena<SysAllocator<char>>({}, minBlockSize, sizeLimit, maxAlign) {}
};

} // namespace folly::clone

#include "geax-front-end/common/FollyArenaInl.h"

#endif  // GEAXFRONTEND_COMMON_FOLLYARENA_H_
