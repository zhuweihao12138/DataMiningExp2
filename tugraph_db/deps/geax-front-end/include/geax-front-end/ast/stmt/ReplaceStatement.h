/**
 * Copyright 2023 AntGroup CO., Ltd.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *
 *  Author:
 *         lili <liangjingru.ljr@antgroup.com>
 */

#ifndef GEAXFRONTEND_AST_STMT_REPLACESTATEMENT_H_
#define GEAXFRONTEND_AST_STMT_REPLACESTATEMENT_H_

#include "geax-front-end/ast/clause/PathChain.h"
#include "geax-front-end/ast/stmt/PrimitiveDataModifyStatement.h"

namespace geax {
namespace frontend {

class ReplaceStatement : public PrimitiveDataModifyStatement {
public:
    ReplaceStatement() : PrimitiveDataModifyStatement(AstNodeType::kReplaceStatement) {}
    ~ReplaceStatement() = default;

    void appendPath(PathChain* path) { paths_.emplace_back(path); }
    void setPaths(std::vector<PathChain*>&& paths) { paths_ = std::move(paths); }
    const std::vector<PathChain*>& paths() const { return paths_; }

    std::any accept(AstNodeVisitor& visitor) override { return visitor.visit(this); }

private:
    std::vector<PathChain*> paths_;
};  // class ReplaceStatement

}  // namespace frontend
}  // namespace geax

#endif  // GEAXFRONTEND_AST_STMT_REPLACESTATEMENT_H_
