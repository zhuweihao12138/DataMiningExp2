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
 *         Yaochi <boyao.zby@alibaba-inc.com>
 */

#ifndef GEAXFRONTEND_AST_STMT_SCHEMAFROMPATH_H_
#define GEAXFRONTEND_AST_STMT_SCHEMAFROMPATH_H_

#include "geax-front-end/ast/stmt/SchemaRef.h"

namespace geax {
namespace frontend {

class SchemaFromPath : public SchemaRef {
public:
    SchemaFromPath() : SchemaRef(AstNodeType::kSchemaFromPath) {}
    ~SchemaFromPath() = default;

    void setPath(std::string&& path) { path_ = std::move(path); }
    const std::string& path() const { return path_; }

    std::any accept(AstNodeVisitor& visitor) override { return visitor.visit(this); }

private:
    std::string path_;
};  // class SchemaFromPath

}  // namespace frontend
}  // namespace geax

#endif  // GEAXFRONTEND_AST_STMT_SCHEMAFROMPATH_H_
