/*
 * Copyright 2020 Krayfaus
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license that can be
 * found in the LICENSE file in the root directory of this source tree.
 */
#include "include/core/ast.hpp"
#include "include/utils/platform_console.hpp"
#include <algorithm>
#include <string>
#include <string_view>

namespace astraea {

AstNode *
ast_noop_init()
{
    auto *ast_noop = (AstNode *)std::calloc(1, sizeof(AstNode));
    if (!ast_noop) {
        std::exit(1);
    }

    ast_noop->node_type = AstNodeType::NO_OPERATION;

    return ast_noop;
}

AstCompound *
ast_compound_init()
{
    auto *ast_compound = (AstCompound *)std::calloc(1, sizeof(AstCompound));
    if (!ast_compound) {
        std::exit(1);
    }

    ast_compound->node_type  = AstNodeType::COMPOUND;
    ast_compound->statements = nullptr;
    ast_compound->statement_count = 0;
    ast_compound->scope = nullptr;

    return ast_compound;
}

AstCompound *
ast_compound_add_statement(AstCompound *ast_compound, AstNode *statement)
{
    auto reallocated_buffer = (AstNode **)std::realloc(
        ast_compound->statements,
        (ast_compound->statement_count + 1 /*new elem*/) * sizeof(AstNode *)
    );
    if (!reallocated_buffer) {
        std::exit(1);
    }

    ast_compound->statements = reallocated_buffer;
    ast_compound->statements[ast_compound->statement_count] = statement;
    ast_compound->statement_count += 1;

    return ast_compound;
}

AstFunction *
ast_function_init(std::string_view func_name)
{
    auto ast_function = (AstFunction *)calloc(1, sizeof(AstFunction));
    if (!ast_function) {
        std::exit(1);
    }

    ast_function->node_type = AstNodeType::FUNCTION_DEFINITION;
    ast_function->name = func_name;
    ast_function->arguments = nullptr;
    ast_function->argument_count = 0;
    ast_function->block = nullptr;
    ast_function->scope = nullptr;

    return ast_function;
}

AstString *
ast_string_init(std::string_view string_value)
{
    auto *ast_string = (AstString *)calloc(1, sizeof(AstString));
    if (!ast_string) {
        std::exit(1);
    }

    ast_string->node_type = AstNodeType::EXPRESSION_STRING;
    ast_string->literal = string_value;

    return ast_string;
}

AstTypeBasic *
ast_typedef_basic_init()
{
    auto *ast_type_basic = (AstTypeBasic *)calloc(1, sizeof(AstTypeBasic));
    if (!ast_type_basic) {
        std::exit(1);
    }

    ast_type_basic->node_type = AstNodeType::TYPE_DEFINITION;
    ast_type_basic->base_type = AstTypeInfo::UNKNOWN;

    return ast_type_basic;
}

AstTypeEnum *
ast_typedef_enum_init(std::string_view enum_name)
{
    auto ast_type_enum = (AstTypeEnum *)calloc(1, sizeof(AstTypeEnum));
    if (!ast_type_enum) {
        std::exit(1);
    }

    ast_type_enum->node_type = AstNodeType::TYPE_DEFINITION;
    ast_type_enum->base_type = AstTypeInfo::UNKNOWN;
    ast_type_enum->name = enum_name;
    ast_type_enum->elements = nullptr;

    return ast_type_enum;
}

AstTypeEnum *
ast_typedef_enum_add_element(AstTypeEnum *ast_type_enum, AstNode *element)
{
    /* This code sucks! */
    auto reallocated_buffer = (AstNode **)realloc(
        ast_type_enum->elements,
        (ast_type_enum->element_count + 1 /*new elem*/) * sizeof(AstNode *)
    );
    if (!reallocated_buffer) {
        std::exit(1);
    }

    ast_type_enum->elements = reallocated_buffer;
    ast_type_enum->elements[ast_type_enum->element_count] = element;
    ast_type_enum->element_count += 1;
    ast_type_enum->scope = nullptr;

    return ast_type_enum;
}

AstTypeString *
ast_typedef_string_init(std::string_view string_name)
{
    auto ast_type_string = (AstTypeString *)calloc(1, sizeof(AstTypeString));
    if (!ast_type_string) {
        std::exit(1);
    }

    ast_type_string->node_type = AstNodeType::TYPE_DEFINITION;
    ast_type_string->base_type = AstTypeInfo::STRING;
    ast_type_string->name = string_name;
    ast_type_string->value = std::string{""};
    ast_type_string->encoding = std::string{""};
    ast_type_string->count = 0;

    return ast_type_string;
}

AstTypeStruct *
ast_typedef_struct_init(std::string_view struct_name)
{
    auto ast_type_struct = (AstTypeStruct *)calloc(1, sizeof(AstTypeStruct));
    if (!ast_type_struct) {
        std::exit(1);
    }

    ast_type_struct->node_type = AstNodeType::TYPE_DEFINITION;
    ast_type_struct->base_type = AstTypeInfo::STRUCT;
    ast_type_struct->name = struct_name;
    ast_type_struct->block = nullptr;

    return ast_type_struct;
}

AstVariable *
ast_vardef_init(std::string_view variable_name)
{
    auto *ast_variable = (AstVariable *)calloc(1, sizeof(AstVariable));
    if (!ast_variable) {
        std::exit(1);
    }

    ast_variable->node_type = AstNodeType::VARIABLE_DEFINITION;
    ast_variable->name  = variable_name;

    return ast_variable;
}

AstTypeInfo
parse_type_info(std::string_view base_type)
{
    auto out = std::string{"PARSER [Type Base Type]: "};
    out += base_type;
    out += "\n";
    platform::print(out);

    if ("uint" == base_type) {
        return AstTypeInfo::UNSIGNED;
    } else if ("u8" == base_type) {
        return AstTypeInfo::U8;
    } else if ("u16" == base_type) {
        return AstTypeInfo::U16;
    } else if ("u32" == base_type) {
        return AstTypeInfo::U32;
    } else if ("u64" == base_type) {
        return AstTypeInfo::U64;
    } else if ("int" == base_type) {
        return AstTypeInfo::SIGNED;
    } else if ("s8" == base_type) {
        return AstTypeInfo::S8;
    } else if ("s16" == base_type) {
        return AstTypeInfo::S16;
    } else if ("s32" == base_type) {
        return AstTypeInfo::S32;
    } else if ("s64" == base_type) {
        return AstTypeInfo::S64;
    } else if ("float" == base_type) {
        return AstTypeInfo::FLOAT;
    } else if ("f32" == base_type) {
        return AstTypeInfo::F32;
    } else if ("f64" == base_type) {
        return AstTypeInfo::F64;
    }

    return AstTypeInfo::UNKNOWN;
}

std::string
ast_node_type_as_string(AstNodeType node_type)
{
    switch(node_type) {
    case AstNodeType::COMPOUND:
    {
        return "Compound";
        break;
    }
    case AstNodeType::TYPE_DEFINITION:
    {
        return "Type Definition";
        break;
    }
    case AstNodeType::VARIABLE_DEFINITION:
    {
        return "Variable Definition";
        break;
    }
    }

    return "";
}
} // namespace astraea