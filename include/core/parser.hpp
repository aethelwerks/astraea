/*
 * Copyright 2020 Krayfaus
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license that can be
 * found in the LICENSE file in the root directory of this source tree.
 */
#pragma once
#include "include/core/lexer.hpp"  // IWYU pragma: export
#include "include/core/token.hpp"  // IWYU pragma: export
#include "include/utils/types.hpp"

namespace astraea {

// IWYU pragma: private, include "ast.hpp"
struct AstNode;

struct Parser {
public:
    Lexer &lexer;
    Token current_token;
    Token previous_token;

public:
    Parser(Lexer &lexer);

    void eat(TokenType token_type);

    AstNode *parse();

    AstNode *parse_expression();

    AstNode *parse_factor();

    AstNode *parse_function_call();

    AstNode *parse_identifier();

    AstNode *parse_statement();

    AstNode *parse_statements();

    AstNode *parse_string();

    AstNode *parse_term();

    AstNode *parse_variable();

    AstNode *parse_type_enum(std::string_view enum_name);
    AstNode *parse_type_string(std::string_view string_name);
    AstNode *parse_type_struct(std::string_view struct_name);
    AstNode *parse_function_definition(std::string_view func_name);
    AstNode *parse_const_definition();
    AstNode *parse_variable_definition();
};
}  // namespace astraea