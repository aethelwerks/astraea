/*
 * Copyright 2020 Krayfaus
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license that can be
 * found in the LICENSE file in the root directory of this source tree.
 */
#include "include/core/scope.hpp"
#include "include/core/visitor.hpp"
#include "include/utils/types.hpp"
#include <cstdio>

namespace astraea {

AstNode *
visitor_visit(Visitor *visitor, AstNode *node)
{
    if (!node) return ast_noop_init();

    switch (node->node_type) {
        case AstNodeType::COMPOUND:
            return visitor_visit_compound(visitor, node);
            break;
        case AstNodeType::FUNCTION_DEFINITION:
            return visitor_visit_function_definition(visitor, node);
            break;
        case AstNodeType::TYPE_DEFINITION:
            return visitor_visit_type_definition(visitor, node);
            break;
        case AstNodeType::VARIABLE_DEFINITION:
            return visitor_visit_variable_definition(visitor, node);
            break;
        case AstNodeType::NO_OPERATION:
            return node;
            break;
    }

    std::printf("VISITOR: Uncaught statement of type `%d`\n", node->node_type);
    exit(1);
}

AstNode *
visitor_visit_compound(Visitor *visitor, AstNode *node)
{
    auto compound = (AstCompound *)node;
    std::printf("VISITOR [Compound] Statement Count %u\n", compound->statement_count);

    if (compound->scope == nullptr) {
        compound->scope = scope_init();
    }

    for (uint32_t i = 0; i < compound->statement_count; i += 1) {
        auto &statement = compound->statements[i];
        std::printf(
            "VISITOR [Compound: Statement %d] %s\n", i,
            ast_node_type_as_string(statement->node_type).c_str()
        );

        visitor->current_scope = compound->scope;
        visitor_visit(visitor, statement);
    }

    return ast_noop_init();
}

AstNode *
visitor_visit_function_definition(Visitor *visitor, AstNode *node)
{
    std::printf("VISITOR [Function Definition]\n");

    auto func_def = (AstFunction *)node;
    scope_add_function_definition(visitor->current_scope, func_def);

    return node;
}

AstNode *
visitor_visit_type_definition(Visitor *visitor, AstNode *node)
{
    auto type_def = (AstType *)node;
    scope_add_typedef(visitor->current_scope, type_def);

    if (AstTypeInfo::STRUCT == type_def->base_type) {
        auto struct_def = (AstTypeStruct *)node;
        visitor_visit(visitor, struct_def->block);  // COMPOUND BLOCK
    }

    return node;
}

AstNode *
visitor_visit_variable_definition(Visitor *visitor, AstNode *node)
{
    auto var_def = (AstVariable *)node;
    scope_add_variable_definition(visitor->current_scope, var_def);

    return node;
}

}  // namespace astraea