/*
 * Copyright 2020 Krayfaus
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license that can be
 * found in the LICENSE file in the root directory of this source tree.
 */
#pragma once
#include "include/core/ast_types.hpp"  // IWYU pragma: export
#include "include/utils/platform_string.hpp"
#include "include/utils/types.hpp"

namespace astraea {

// IWYU pragma: private, include "scope.hpp"
struct Scope;

struct AstNode {
    AstNodeType node_type;
};

struct AstCompound {
    AstNodeType node_type;
    AstNode **statements;
    uint32_t statement_count;
    Scope *scope;
};

struct AstExpression {
    AstNodeType node_type;
};

struct AstString {
    AstNodeType node_type;
    std::string literal;
};

struct AstOperation {
    AstNodeType node_type;
    std::string left;
    std::string right;
    std::string operation;
};

struct AstFunctionCall {
    AstNodeType node_type;
    std::string name;
    AstNode **arguments;
    uint32_t argument_count;
};

struct AstFunction {
    AstNodeType node_type;
    std::string name;
    AstNode **arguments;
    uint32_t argument_count;
    AstNode *block;
    Scope *scope;
};

struct AstVariable {
    AstNodeType node_type;
    std::string name;
    std::string type;
    std::string value;
};

struct AstType {
    AstNodeType node_type;
    AstTypeInfo base_type;
    std::string name;
};

struct AstTypeBasic {
    AstNodeType node_type;
    AstTypeInfo base_type;
    std::string name;
    std::string type;
    std::string value;
};

struct AstTypeEnum {
    AstNodeType node_type;
    AstTypeInfo base_type;
    std::string name;
    AstNode **elements;
    uint32_t element_count;
    Scope *scope;
};

struct AstTypeString {
    AstNodeType node_type;
    AstTypeInfo base_type;
    std::string name;
    std::string value;
    std::string encoding;
    uint32_t count;
};

struct AstTypeStruct {
    AstNodeType node_type;
    AstTypeInfo base_type;
    std::string name;
    AstNode *block;
    Scope *scope;
};

AstTypeInfo parse_type_info(std::string_view base_type);
std::string ast_node_type_as_string(AstNodeType node_type);

AstNode *ast_noop_init();
AstCompound *ast_compound_init();
AstCompound *ast_compound_add_statement(AstCompound *ast_compound, AstNode *statement);

AstFunction *ast_function_init(std::string_view func_name);
AstString *ast_string_init(std::string_view string_value);

AstTypeBasic *ast_typedef_basic_init();
AstTypeEnum *ast_typedef_enum_init(std::string_view enum_name);
AstTypeEnum *ast_typedef_enum_add_element(AstTypeEnum *ast_type_enum, AstNode *element);
AstTypeString *ast_typedef_string_init(std::string_view string_name);
AstTypeStruct *ast_typedef_struct_init(std::string_view struct_name);
AstVariable *ast_vardef_init(std::string_view variable_name);

}  // namespace astraea
