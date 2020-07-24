/*
 * Copyright 2020 Krayfaus
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license that can be
 * found in the LICENSE file in the root directory of this source tree.
 */
#pragma once
#include "include/core/ast.hpp"

namespace astraea {

// IWYU pragma: private, include "scope.hpp"
struct Scope;

struct Visitor {
    Scope *current_scope = nullptr;
};

AstNode *visitor_visit(Visitor *visitor, AstNode *node);

AstNode *visitor_visit_compound(Visitor *visitor, AstNode *node);

AstNode *visitor_visit_function_definition(Visitor *visitor, AstNode *node);

AstNode *visitor_visit_type_definition(Visitor *visitor, AstNode *node);

AstNode *visitor_visit_variable_definition(Visitor *visitor, AstNode *node);

}  // namespace astraea