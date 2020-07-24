/*
 * Copyright 2020 Krayfaus
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license that can be
 * found in the LICENSE file in the root directory of this source tree.
 */
#pragma once
#include "include/core/token.hpp"
#include "include/utils/types.hpp"

struct Lexer {
public:
    std::string path;
    uint32_t col;
    uint32_t row;
    uint32_t pos;
    std::string contents;
    enum class Status {
        OK, ERROR, EOF_
    } status;

public:
    Lexer(std::string_view source_path);

    void reset();
    bool validate();
    void advance_cursor();
    char current_character();
    char next_character();
    std::string current_character_as_u8string();

    Token get_next_token();
    Token collect_character();
    Token collect_identifier();
    Token collect_keyword();
    Token collect_number();
    Token collect_number_binary();
    Token collect_number_octal();
    Token collect_number_hex();
    Token collect_operator();
    Token collect_string();

    void skip_whitespace();
    void skip_comment();
    void skip_line_comment();
    void clean_illegal_characters();
};
