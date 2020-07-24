/*
 * Copyright 2020 Krayfaus
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license that can be
 * found in the LICENSE file in the root directory of this source tree.
 */
#include "include/core/ast.hpp"
#include "include/core/parser.hpp"
#include "include/utils/platform_console.hpp"
#include <algorithm>

namespace astraea {

Parser::Parser(Lexer &lexer) :
    lexer(lexer)
{
    current_token = lexer.get_next_token();
    previous_token = current_token;
}

void
Parser::eat(TokenType token_type)
{
    if (current_token.type != token_type) {
        auto print_str = std::string{"PARSER [Unexpected Token]: \""};
        print_str += current_token.literal + "\" with type (";
        print_str += std::string(std::to_string((int32_t)current_token.type));
        print_str += "\n";
        platform::print(print_str);
        exit(1);
    }

    previous_token = current_token;
    current_token = lexer.get_next_token();
}

AstNode *
Parser::parse()
{
    return parse_statements();
}

AstNode *
Parser::parse_expression()
{   /*
    printf(
        "PARSER [Expression]: %s %d\n",
        current_token.literal,
        current_token.type
    );*/
    switch (current_token.type) {
    case TokenType::STRING:
    {
        return parse_string();
        break;
    }
    }

    return nullptr;
}

AstNode *
Parser::parse_factor()
{
    return nullptr;
}

AstNode *
Parser::parse_function_call()
{
    return nullptr;
}

AstNode *
Parser::parse_function_definition(std::string_view func_name)
{
    auto ast_funcdef = ast_function_init(func_name);
    eat(TokenType::LEFT_PAREN);
    while (TokenType::RIGHT_PAREN != current_token.type) {
        auto arg_name = current_token.literal;
        eat(TokenType::COLON);
        // auto arg_type = parse_argument_type();
        if (TokenType::COMMA == current_token.type) {
            eat(TokenType::COMMA);
        }
    }
    eat(TokenType::RIGHT_PAREN);

    eat(TokenType::LEFT_BRACE);
    eat(TokenType::RIGHT_BRACE);

    return (AstNode *)ast_funcdef;
}

AstNode *
Parser::parse_identifier()
{
    // auto out = std::string{"PARSER [Identifier]: "};
    // out += current_token.literal;
    // out += "\n";
    // platform::print(out, 6);
    eat(TokenType::IDENTIFIER);
    // printf("OP %s\n", current_token.literal);

    if (TokenType::EQUAL == current_token.type) {
        // return parse_binary_operation();
    } else if (TokenType::COLON_COLON == current_token.type) {
        return parse_const_definition();
    } else if (TokenType::COLON == current_token.type) {
        return parse_variable_definition();
    } else if (TokenType::COLON_EQUAL == current_token.type) {
        return parse_variable_definition();
    }

    return nullptr;
}

AstNode *
Parser::parse_statement()
{
    switch (current_token.type) {
        case TokenType::IDENTIFIER:
            return parse_identifier();
            break;
    }
    return nullptr;
}

AstNode *
Parser::parse_statements()
{
    auto ast_compound = ast_compound_init();

    while (
        TokenType::EOF_ != current_token.type &&
        TokenType::ILLEGAL != current_token.type
    ) {
        if (TokenType::IDENTIFIER != current_token.type) {
            break;
        }
        // std::printf("PARSER [Statement]: %d\n", current_token.type);
        auto ast_statement = parse_statement();
        ast_compound_add_statement(ast_compound, ast_statement);
        eat(TokenType::SEMICOLON);
    };

    return (AstNode *)ast_compound;
}


AstNode *
Parser::parse_string()
{
    auto ast_string = ast_string_init(current_token.literal);
    eat(TokenType::STRING);

    return (AstNode *)ast_string;
}

AstNode *
Parser::parse_term()
{
    return nullptr;
}

AstNode *
Parser::parse_type_enum(std::string_view enum_name)
{
    auto out = std::string{"PARSER [Type Definition Enum]: "};
    out += enum_name;
    out += "\n";
    platform::print(out);
    auto ast_type_enum = ast_typedef_enum_init(enum_name);

    if (TokenType::LEFT_BRACE != current_token.type) {
        auto enum_base_type = parse_type_info(current_token.literal);
        // if (enum_base_type != AstTypeInfo::UNKNOWN) {
            eat(TokenType::IDENTIFIER);  // eat base_type
        // }
        ast_type_enum->base_type = enum_base_type;
    }

    eat(TokenType::LEFT_BRACE);
    do {
        auto enum_elem = ast_typedef_basic_init();
        enum_elem->name = current_token.literal;
        eat(TokenType::IDENTIFIER);
        //printf("PARSER [Enum Element]: { name := %s", enum_elem->name);

        auto is_comma = TokenType::COMMA == current_token.type;
        auto is_right_brace = TokenType::RIGHT_BRACE == current_token.type;

        if (!is_comma && !is_right_brace) {
            eat(TokenType::EQUAL);
            enum_elem->value = current_token.literal;
            // printf(", value := %s", enum_elem->value);
            eat(current_token.type);
        }
        printf(" }\n");

        /*
         * @TODO: type check enum element values
         */
        ast_typedef_enum_add_element(ast_type_enum, (AstNode *)enum_elem);

        if (is_comma) {
            eat(TokenType::COMMA);
        }
        /*
         * this is proposital the programmer may by his wish
         * leave a trailling comma after the last element in
         * the enum declaration.
         */
        if (is_right_brace) {
            eat(TokenType::RIGHT_BRACE);
        }
    } while (TokenType::SEMICOLON != current_token.type);

    return (AstNode *)ast_type_enum;
}

AstNode *
Parser::parse_type_string(std::string_view string_name)
{
    //printf("PARSER [Type Definition String]: %s\n", string_name);
    auto ast_type_string = ast_typedef_string_init(string_name);

    if (TokenType::LEFT_ANGLE == current_token.type) {
        eat(TokenType::LEFT_ANGLE);
        auto string_encoding = current_token.literal;
        ast_type_string->encoding = string_encoding;
        //printf("PARSER [Type String]: encoding := %s\n", string_encoding);
        eat(TokenType::STRING);
        eat(TokenType::RIGHT_ANGLE);
    }

    if (TokenType::SEMICOLON != current_token.type) {
        eat(TokenType::COLON);
        auto string_value = current_token.literal;
        ast_type_string->value = string_value;
        auto output = std::string{"PARSER [Type String]: value := "} + string_value;
        output += "\n";
        platform::print(output);
    }

    return (AstNode *)ast_type_string;
}

AstNode *
Parser::parse_type_struct(std::string_view struct_name)
{
    //printf("PARSER [Type Definition Struct]: %s\n", struct_name);
    auto ast_type_struct = ast_typedef_struct_init(struct_name);

    eat(TokenType::LEFT_BRACE);
    ast_type_struct->block = parse_statements();
    eat(TokenType::RIGHT_BRACE);

    return (AstNode *)ast_type_struct;
}

AstNode *
Parser::parse_const_definition()
{
    auto const_name = previous_token.literal;
    eat(TokenType::COLON_COLON);

    auto const_type = current_token.literal;
    eat(TokenType::IDENTIFIER);

    if ("alias" == const_type) {
        // return parse_type_alias(const_name);
    } else if ("enum" == const_type) {
        return parse_type_enum(const_name);
    } else if ("string" == const_type) {
        return parse_type_string(const_name);
    } else if ("struct" == const_type) {
        return parse_type_struct(const_name);
    } else if ("function" == const_type) {
        return parse_function_definition(const_name);
    }

    return nullptr;
}

AstNode *
Parser::parse_variable_definition()
{
    auto variable_name = previous_token.literal;
    auto var_def = ast_vardef_init(variable_name);
    // std::printf("PARSER [Variable Definition]: %s\n", var_def->name.c_str());

    if (TokenType::COLON == current_token.type) {
        eat(TokenType::COLON);
        auto variable_type = current_token.literal;
        eat(TokenType::IDENTIFIER);
        var_def->type = variable_type;
        // std::printf("\twith type %s\n", var_def->type.c_str());
        if (TokenType::SEMICOLON == current_token.type) {
            return (AstNode *)var_def;
        } else {
            eat(TokenType::EQUAL);
        }
    } else {
        eat(TokenType::COLON_EQUAL);
    }

    auto variable_value = current_token.literal;
    eat(current_token.type);
    // This is not always true we may need to parse expressions.
    var_def->value = variable_value;
    // std::printf("\tvalue %s\n", var_def->value.c_str());

    return (AstNode *)var_def;
}

} // namespace astraea