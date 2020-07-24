/*
 * Copyright 2020 Krayfaus
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license that can be
 * found in the LICENSE file in the root directory of this source tree.
 */
#pragma once
#include "include/core/ast.hpp"
#include "include/utils/types.hpp"

namespace astraea {

struct Scope {
    AstFunction **function_definitions;
    uint32_t num_function_definitions;

    AstType **type_definitions;
    uint32_t num_type_definitions;

    AstVariable **variable_definitions;
    uint32_t num_variable_definitions;
};

Scope *scope_init();

AstFunction *scope_add_function_definition(Scope *scope, AstFunction *fdef);

AstFunction *scope_get_function_definition(Scope *scope, const char *fname);

AstType *scope_add_typedef(Scope *scope, AstType *type_def);

AstType *scope_get_typedef(Scope *scope, std::string_view type_name);

AstVariable *scope_add_variable_definition(Scope *scope, AstVariable *vdef);

AstVariable *scope_get_variable_definition(Scope *scope, const char *name);

}  // namespace astraea