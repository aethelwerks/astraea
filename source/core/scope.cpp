/*
 * Copyright 2020 Krayfaus
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license that can be
 * found in the LICENSE file in the root directory of this source tree.
 */
#include "include/core/scope.hpp"
#include "include/utils/platform_string.hpp"
#include "include/utils/types.hpp"
#include <algorithm>
#include <string_view>

namespace astraea {

Scope *
scope_init()
{
    auto scope = (Scope *)std::calloc(1, sizeof(Scope));

    scope->function_definitions = nullptr;
    scope->num_function_definitions = 0;
    scope->type_definitions = nullptr;
    scope->num_type_definitions = 0;
    scope->variable_definitions = nullptr;
    scope->num_variable_definitions = 0;

    return scope;
}

AstFunction *
scope_add_function_definition(Scope *scope, AstFunction *func_def)
{
    if (scope->function_definitions == nullptr) {
        scope->function_definitions = (AstFunction **)std::calloc(1, sizeof(AstFunction *));
    } else {
        scope->function_definitions = (AstFunction **)std::realloc(
            scope->function_definitions,
            (scope->num_function_definitions + 1 /*new element*/) * sizeof(AstFunction **));
    }

    scope->function_definitions[scope->num_function_definitions] = func_def;
    scope->num_function_definitions += 1;

    return func_def;
}

AstFunction *
scope_get_function_definition(Scope *scope, std::string_view func_name)
{
    for (int i = 0; i < scope->num_function_definitions; i++) {
        AstFunction *func_def = scope->function_definitions[i];
        if (func_def->name == func_name) {
            return func_def;
        }
    }

    return nullptr;
}

AstType *
scope_add_typedef(Scope *scope, AstType *type_def)
{
    if (scope->type_definitions == nullptr) {
        scope->type_definitions = (AstType **)std::calloc(1, sizeof(AstType *));
    } else {
        scope->type_definitions = (AstType **)std::realloc(
            scope->type_definitions,
            (scope->num_type_definitions + 1 /*new element*/) * sizeof(AstType **));
    }

    scope->type_definitions[scope->num_type_definitions] = type_def;
    scope->num_type_definitions += 1;

    return type_def;
}

AstType *
scope_get_typedef(Scope *scope, std::string_view type_name)
{
    for (int i = 0; i < scope->num_type_definitions; i++) {
        AstType *type_def = scope->type_definitions[i];
        if (type_def->name == type_name) {
            return type_def;
        }
    }

    return nullptr;
}

AstVariable *
scope_add_variable_definition(Scope *scope, AstVariable *var_def)
{
    if (scope->variable_definitions == nullptr) {
        scope->variable_definitions = (AstVariable **)std::calloc(1, sizeof(AstVariable *));
        scope->variable_definitions[0] = var_def;
        scope->num_variable_definitions += 1;
    } else {
        scope->variable_definitions = (AstVariable **)std::realloc(
            scope->variable_definitions,
            (scope->num_variable_definitions + 1 /*new element*/) * sizeof(AstVariable *));
        scope->variable_definitions[scope->num_variable_definitions] = var_def;
        scope->num_variable_definitions += 1;
    }

    return var_def;
}

AstVariable *
scope_get_variable_definition(Scope *scope, std::string_view var_name)
{
    for (int i = 0; i < scope->num_variable_definitions; i++) {
        AstVariable *var_def = scope->variable_definitions[i];
        if (var_def->name == var_name) {
            return var_def;
        }
    }

    return nullptr;
}

}  // namespace astraea