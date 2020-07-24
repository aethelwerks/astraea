/*
 * Copyright 2020 Krayfaus
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license that can be
 * found in the LICENSE file in the root directory of this source tree.
 */
#include "include/core/ast.hpp"
#include "include/core/lexer.hpp"
#include "include/core/parser.hpp"
#include "include/core/visitor.hpp"
#include "include/utils/platform_console.hpp"
#include <cstdio>

#ifdef OS_POSIX
#include <unistd.h>
#endif

using namespace astraea;

void test_tokenizer(Lexer &lexer);


int
main(int argc, char **argv)
{
    printf("Running!\n");
#ifdef OS_POSIX
    {
        char cwd[1024];
        chdir("/path/to/change/directory/to");
        getcwd(cwd, sizeof(cwd));
        printf("Current working dir: %s\n", cwd);
    }
    auto source_path = std::string_view{"./resources/teste1.ast"};
#else 
    auto source_path = std::string_view{"D:/scripts/teste1.ast"};
#endif
    if (argc == 2) {
        source_path = std::string_view{argv[1]};
    }
    platform::print(std::string("Script: ") + std::string(source_path) + "\n");


    auto lexer = Lexer{source_path};
    platform::print(lexer.contents + "\n");
    test_tokenizer(lexer);

    auto parser = Parser{lexer};
    auto root = parser.parse();

    Visitor visitor;
    visitor_visit(&visitor, root);

    return 0;
}

void test_tokenizer(Lexer &lexer)
{
    Token token = lexer.get_next_token();
    while (token.type != TokenType::EOF_ && token.type != TokenType::ILLEGAL) {
        auto output = std::string{"TOKEN "}
            + std::to_string((int32_t)token.type)
            + ": [ " + std::string(token.literal) + " ]"
            + " (row " + std::to_string(token.row)
            + ", col " + std::to_string(token.col) + ")\n";
        platform::print(output);
        token = lexer.get_next_token();
    }
    lexer.reset();
}