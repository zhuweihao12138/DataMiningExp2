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

#ifndef GEAXFRONTEND_AST_STMT_BINDTABLEINNEREXPR_H_
#define GEAXFRONTEND_AST_STMT_BINDTABLEINNEREXPR_H_

#include "geax-front-end/ast/expr/Expr.h"
#include "geax-front-end/ast/stmt/BindingTableExpr.h"

namespace geax {
namespace frontend {

class BindingTableInnerExpr : public BindingTableExpr {
public:
    BindingTableInnerExpr() : BindingTableExpr(AstNodeType::kBindingTableInnerExpr) {}
    ~BindingTableInnerExpr() = default;

    void setExpr(Expr* expr) { expr_ = expr; }
    Expr* expr() { return expr_; }

    std::any accept(AstNodeVisitor& visitor) override { return visitor.visit(this); }

private:
    Expr* expr_;
};  // class BindingTableInnerExpr

}  // namespace frontend
}  // namespace geax

#endif  // GEAXFRONTEND_AST_STMT_BINDTABLEINNEREXPR_H_
