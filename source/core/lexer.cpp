/*
 * Copyright 2020 Krayfaus
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license that can be
 * found in the LICENSE file in the root directory of this source tree.
 */
#include "include/core/lexer.hpp"
#include "include/utils/array.hpp"
#include "include/utils/binaryreader.hpp"
#include "include/utils/platform_console.hpp"
#include <cstdio>

#include "include/core/lexer.inl"

Lexer::Lexer(std::string_view source_path) :
    path(source_path),
    col(1),
    row(1),
    pos(0),
    contents("")
{
    namespace br = binaryreader;
    auto source_file = br::open_file(source_path);
    auto source_file_size = br::file_size(source_file);
    auto source_contents = br::read<char>(source_file, 0x0, source_file_size);
    br::close_file(source_file);

    if (source_contents.count > 0) {
        contents = std::string{source_contents.data, source_contents.count};
        status = Status::OK;
    } else {
        status = Status::ERROR;
    }
    /*{
        platform::print(contents, 3);
        std::printf("\n%I64u == %d\n", contents.length(), source_contents.count);

        auto len1 = platform::utf8_cp_size(std::string_view{contents.data(), 4});
        platform::print(std::string{contents.data(), len1}, 2);
        std::printf(" lenght is %d byte(s)\n", len1);

        auto len2 = platform::utf8_cp_size(std::string_view{contents.data() + len1, 4});
        platform::print(std::string{contents.data() + len1, len2}, 2);
        std::printf(" lenght is %d byte(s)\n", len2);
    }*/
}

void
Lexer::reset()
{
    col = 1;
    row = 1;
    pos = 0;
    status = Status::OK;
}

bool
Lexer::validate()
{
    if (pos >= contents.length()) {
        // std::printf("LEXER: advance stopped.\n");
        status = Status::EOF_;
        return false;
    }
    auto ch = contents.at(pos);
    if (ch == '\0') {
        return false;
    }
    if (status != Status::OK) {
        return false;
    }

    return true;
}

void
Lexer::advance_cursor()
{
    if (validate()) {
        auto ch = contents.at(pos);
        if (ch == '\n') {
            row += 1;
            col = 1;
        } else if (ch == '\t') {
            col += 4;
        } else if (ch != 13) {
            col += 1;
        }
        pos += platform::utf8_cp_size(contents.data() + pos);
    }
}

char
Lexer::current_character()
{
    if (validate()) {
        return contents.at(pos);
    } else {
        // std::printf("LEXER OVERFLOW: You're being greedy stop that!\n");
        return '\0';
    }
}

char
Lexer::next_character()
{
    auto sz = platform::utf8_cp_size(contents.data() + pos);
    return contents.at(pos + sz);
}

std::string
Lexer::current_character_as_u8string()
{
    auto sz = platform::utf8_cp_size(contents.data() + pos);
    auto str = std::string{contents.data() + pos, sz};

    return str;
}

Token
Lexer::get_next_token()
{
    while (validate()) {
        auto ch = current_character();
        if (ch == '/' || is_character_skipable(ch)) {
            clean_illegal_characters();
        }
        if (!validate()) {
            break;
        }

        ch = current_character();
        if (ch == 39) {                                         // [ CHARACTER ]
            return collect_character();
        } else if (ch == '"') {                                 // [ STRING ]
            return collect_string();
        } else if (is_character_number(ch)) {                   // [ INTEGER, ..., HEX ]
            return collect_number();
        } else if (is_character_reserved(ch)) {                 // [ OPERATORS ]
            return collect_operator();
        } else if (is_character_valid_identifier(ch)) {         // [ IDENTIFIER ]
            return collect_identifier();
        } else {
            return Token(TokenType::ILLEGAL, "", path, row, col);
        }
    }
    printf("LEXER: End of file.\n");
    return Token(TokenType::EOF_, "", path, row, col);
}

Token
Lexer::collect_character()
{
    printf("LEXER: collect_character.\n");
    uint32_t init_row = row;
    uint32_t init_col = col;
    advance_cursor();  // eat APOSTROPHE

    if (next_character() == 39) {
        auto token = Token(
            TokenType::CHARACTER,
            current_character_as_u8string(),
            path, init_row, init_col
        );
        advance_cursor();  // eat CHARACTER
        advance_cursor();  // eat APOSTROPHE
        return token;
    }

    return Token(TokenType::ILLEGAL, "", path, init_row, init_col);
}

Token
Lexer::collect_identifier()
{
    // std::printf("LEXER: Collect Identifier.\n");
    uint32_t init_row = row;
    uint32_t init_col = col;
    auto identifier = std::string{""};

    while (status == Status::OK && is_character_valid_identifier(current_character())) {
        identifier += current_character_as_u8string();
        advance_cursor();
    }

    return Token(TokenType::IDENTIFIER, identifier, path, init_row, init_col);
}

Token
Lexer::collect_keyword()
{
    printf("LEXER: Collect Keyword.\n");
    uint32_t init_row = row;
    uint32_t init_col = col;

    return Token(TokenType::ILLEGAL, "", path, init_row, init_col);
}

Token
Lexer::collect_number_binary()
{
    uint32_t init_row = row;
    uint32_t init_col = col;
    auto literal = std::string{""};

    auto ch = current_character();
    while (ch == '0' || ch == '1') {
        literal += current_character_as_u8string();
        advance_cursor();
        ch = current_character();
    }
    if (is_character_number(ch)) {
        status = Status::ERROR;
        printf("LEXER [ERROR]: %c is to big to base 2.\n", ch);
        return Token(TokenType::ILLEGAL, literal, path, init_row, init_col);
    }

    return Token(TokenType::BINARY, literal, path, init_row, init_col);
}

Token Lexer::collect_number_octal()
{
    uint32_t init_row = row;
    uint32_t init_col = col;
    auto literal = std::string{""};

    auto ch = current_character();
    while (is_character_octal(ch)) {
        literal += current_character_as_u8string();
        advance_cursor();
        ch = current_character();
    }

    return Token(TokenType::OCTAL, literal, path, init_row, init_col);
}

Token Lexer::collect_number_hex()
{
    uint32_t init_row = row;
    uint32_t init_col = col;
    auto literal = std::string{""};

    auto ch = current_character();
    while (is_character_hex(ch)) {
        literal += current_character_as_u8string();
        advance_cursor();
        ch = current_character();
    }

    return Token(TokenType::HEX, literal, path, init_row, init_col);
}

Token
Lexer::collect_number()
{
    // std::printf("LEXER: Collect Number.\n");
    uint32_t init_row = row;
    uint32_t init_col = col;
    auto literal = std::string{""};

    auto ch = current_character();
    if (ch == '0') {
        advance_cursor();
        ch = current_character();
        if (ch == 'b') {  // [ BINARY ]
            advance_cursor();
            return collect_number_binary();
        } else if (ch == 'o') {  // [ OCTAL ]
            advance_cursor();
            return collect_number_octal();
        } else if (ch == 'x') {  // [ HEXA ]
            advance_cursor();
            return collect_number_hex();
        }
        literal += '0';
    }

    // INTEGER
    ch = current_character();
    auto token_type = TokenType::INTEGER;
    while (Status::OK == status && is_character_number(ch)) {
        literal += current_character_as_u8string();
        advance_cursor();
        ch = current_character();
    }

    // FLOATING
    if (ch == '.') {
        token_type = TokenType::FLOAT;
        literal += current_character_as_u8string();
        advance_cursor();
        ch = current_character();
        if (!is_character_number(ch)) {
            status = Status::ERROR;
            printf("LEXER [ERROR]: %c is not a valid value for a floating point number.\n", ch);
            return Token(TokenType::ILLEGAL, literal, path, init_row, init_col);
        }
        while (Status::OK == status && is_character_number(ch)) {
            literal += current_character_as_u8string();
            advance_cursor();
            ch = current_character();
        }
    }

    return Token(TokenType::INTEGER, literal, path, init_row, init_col);
}

Token
Lexer::collect_operator()
{
    // std::printf("LEXER: Collect Operator.\n");
    uint32_t init_row = row;
    uint32_t init_col = col;
    TokenType token_type = TokenType::ILLEGAL;
    auto token_literal = std::string{""};

    switch (current_character()) {
    case '+':
    {
        token_type = TokenType::PLUS;
        token_literal += current_character_as_u8string();
        break;
    }
    case '-':
    {
        token_type = TokenType::MINUS;
        token_literal += current_character_as_u8string();
        break;
    }
    case '*':
    {
        token_type = TokenType::STAR;
        token_literal += current_character_as_u8string();
        break;
    }
    case '/':
    {
        token_type = TokenType::SLASH;
        token_literal += current_character_as_u8string();
        break;
    }
    case '%':
    {
        token_type = TokenType::PERCENT;
        token_literal += current_character_as_u8string();
        break;
    }
    case '!':
    {
        token_type = TokenType::BANG;
        token_literal += current_character_as_u8string();
        break;
    }
    case '>':
    {
        token_type = TokenType::GREATER;
        token_literal += current_character_as_u8string();
        break;
    }
    case '<':
    {
        token_type = TokenType::LESSER;
        token_literal += current_character_as_u8string();
        break;
    }
    case '=':
    {
        token_type = TokenType::EQUAL;
        token_literal += current_character_as_u8string();
        break;
    }
    case ':':
    {
        token_type = TokenType::COLON;
        token_literal += current_character_as_u8string();
        break;
    }
    case ',':
    {
        token_type = TokenType::COMMA;
        token_literal += current_character_as_u8string();
        break;
    }
    case '.':
    {
        token_type = TokenType::DOT;
        token_literal += current_character_as_u8string();
        break;
    }
    case ';':
    {
        token_type = TokenType::SEMICOLON;
        token_literal += current_character_as_u8string();
        break;
    }
    case '(':
    {
        token_type = TokenType::LEFT_PAREN;
        token_literal += current_character_as_u8string();
        break;
    }
    case ')':
    {
        token_type = TokenType::RIGHT_PAREN;
        token_literal += current_character_as_u8string();
        break;
    }
    case '{':
    {
        token_type = TokenType::LEFT_BRACE;
        token_literal += current_character_as_u8string();
        break;
    }
    case '}':
    {
        token_type = TokenType::RIGHT_BRACE;
        token_literal += current_character_as_u8string();
        break;
    }
    case '[':
    {
        token_type = TokenType::LEFT_BRACKET;
        token_literal += current_character_as_u8string();
        break;
    }
    case ']':
    {
        token_type = TokenType::RIGHT_BRACKET;
        token_literal += current_character_as_u8string();
        break;
    }
    default:
        /*
         * Error: Illegal. Should never happen.
         */
        break;
    }

    advance_cursor();
    if (Status::OK != status) {
        return Token(token_type, token_literal, path, init_row, init_col);
    }

    // EQUAL variants
    if (current_character() == '=') {
        switch (token_type) {
        case TokenType::PLUS:
        {
            token_type = TokenType::PLUS_EQUAL;
            token_literal += current_character_as_u8string();
            break;
        }
        case TokenType::MINUS:
        {
            token_type = TokenType::MINUS_EQUAL;
            token_literal += current_character_as_u8string();
            break;
        }
        case TokenType::STAR:
        {
            token_type = TokenType::STAR_EQUAL;
            token_literal += current_character_as_u8string();
            break;
        }
        case TokenType::SLASH:
        {
            token_type = TokenType::SLASH_EQUAL;
            token_literal += current_character_as_u8string();
            break;
        }
        case TokenType::PERCENT:
        {
            token_type = TokenType::PERCENT_EQUAL;
            token_literal += current_character_as_u8string();
            break;
        }
        case TokenType::BANG:
        {
            token_type = TokenType::BANG_EQUAL;
            token_literal += current_character_as_u8string();
            break;
        }
        case TokenType::EQUAL:
        {
            token_type = TokenType::EQUAL_EQUAL;
            token_literal += current_character_as_u8string();
            break;
        }
        case TokenType::GREATER:
        {
            token_type = TokenType::GREATER_EQUAL;
            token_literal += current_character_as_u8string();
            break;
        }
        case TokenType::LESSER:
        {
            token_type = TokenType::LESSER_EQUAL;
            token_literal += current_character_as_u8string();
            break;
        }
        case TokenType::COLON:
        {
            token_type = TokenType::COLON_EQUAL;
            token_literal += current_character_as_u8string();
            break;
        }
        default:
        {
            /*
            * Error: Illegal. Should never happen.
            */
            break;
        }
        }
        advance_cursor();
    } else if (current_character() == ':') {
        switch (token_type) {
        case TokenType::COLON:
        {
            token_type = TokenType::COLON_COLON;  // [ COLON_COLON ]
            token_literal += current_character_as_u8string();
            break;
        }
        default:
        {
            /*
            * Error: Illegal. Should never happen.
            */
            break;
        }
        }
        advance_cursor();
    }

    return Token(token_type, token_literal, path, init_row, init_col);
}

Token
Lexer::collect_string()
{
    // std::printf("LEXER: Collect String.\n");
    uint32_t init_row = row;
    uint32_t init_col = col;
    auto token_literal = std::string{""};

    advance_cursor(); // Eat ["] character.
    while (current_character() != '"') {
        token_literal += current_character_as_u8string();
        advance_cursor();
    }
    advance_cursor(); // Eat ["] character.

    return Token(TokenType::STRING, token_literal, path, init_row, init_col);
}

void
Lexer::skip_whitespace()
{
    while (is_character_skipable(current_character())) {
        advance_cursor();
    }
}

void
Lexer::skip_comment()
{
    advance_cursor();  // eat SLASH
    bool is_comment = true;
    do {
        advance_cursor();  // each ch (also eats first ASTERISK)
        if (current_character() == '*' && next_character() == '/') {
            advance_cursor();  // eat ASTERISK
            advance_cursor();  // eat SLASH
            is_comment = false;
        }
    } while (is_comment);
    printf("LEXER: Skiped multi-line comment.\n");
}

void
Lexer::skip_line_comment()
{
    while (current_character() != 10) {
        advance_cursor();
    }
    printf("LEXER: Skiped line comment.\n");
}

inline void
Lexer::clean_illegal_characters()
{
    skip_whitespace();

    // @TODO review this code as there may not parse all comment styles.
    if (current_character() == '/') {
        if (next_character() == '*') {
            skip_comment();
            clean_illegal_characters();
        } else if (next_character() == '/') {
            skip_line_comment();
            clean_illegal_characters();
        }  // else it is an operator.
    }
}
