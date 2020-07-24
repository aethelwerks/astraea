/*
 * Copyright 2020 Krayfaus
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license that can be
 * found in the LICENSE file in the root directory of this source tree.
 */
#pragma once
#include "include/utils/platform_string.hpp"
#include "include/utils/types.hpp"

enum class TokenType : uint32_t {
    // Special tokens
    ILLEGAL,
    EOF_,

    // Literals
    IDENTIFIER,
    CHARACTER,
    STRING,
    INTEGER,
    FLOAT,
    DOUBLE,
    BINARY,
    OCTAL,
    HEX,

    // Operators
    PLUS,
    MINUS,
    STAR,
    SLASH,
    PERCENT,
    BANG,
    EQUAL,
    COLON,
    LESSER,
    LEFT_ANGLE = LESSER,
    GREATER,
    RIGHT_ANGLE = GREATER,
    COMMA,
    DOT,
    DOT_DOT,
    SEMICOLON,  
    PLUS_EQUAL,
    MINUS_EQUAL,
    STAR_EQUAL,
    SLASH_EQUAL,
    PERCENT_EQUAL,
    BANG_EQUAL,
    GREATER_EQUAL,
    LESSER_EQUAL,
    EQUAL_EQUAL,
    COLON_EQUAL,
    COLON_COLON,

    // Brackets
    LEFT_PAREN,
    RIGHT_PAREN,
    LEFT_BRACE,
    RIGHT_BRACE,
    LEFT_BRACKET,
    RIGHT_BRACKET,

    // KEYWORDS
    NULL_,
    TRUE_,
    FALSE_,
    IF,
    ELSE,
    WHILE,
    FOR,
    IN_,
    AND,
    OR,
    NOT,
    BREAK,
    CONTINUE,
    FUNCTION,
    RETURN,
    IMPORT
};

struct Token {
    TokenType type;       // type of lexer token, e.g., INTEGER
    std::string literal;  // value of lexer token, e.g., "5"
    std::string filename; // source file of token
    uint32_t row;        // line number in source file
    uint32_t col;      // cursor number in line of source file

    Token(
        TokenType type,
        std::string_view literal,
        std::string_view filename,
        uint32_t row,
        uint32_t col) :
        type(type),
        literal(literal),
        filename(filename),
        row(row),
        col(col)
    {
    }

    Token()
    {
    }
};
