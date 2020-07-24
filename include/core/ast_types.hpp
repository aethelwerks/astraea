/*
 * Copyright 2020 Krayfaus
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license that can be
 * found in the LICENSE file in the root directory of this source tree.
 */
#pragma once
#include "include/utils/types.hpp"

enum class AstNodeType : uint32_t {
    COMPOUND,             // Container block for other nodes.
    EXPRESSION,           // Anything that must be evaluated.
    EXPRESSION_STRING,    // Compile time string expressions.
    OPERATION,            // Binary operation: lvalue operation rvalue.
    FUNCTION_DEFINITION,  // Defines a callable function.
    TYPE_DEFINITION,      // Defines a data type.
    VARIABLE_DEFINITION,  // Defines a variable.
    NO_OPERATION          // ... kinda like a return command, stops the visitor.
};

enum class AstTypeInfo : uint32_t {
    UNSIGNED, U8, U16, U32, U64,
    SIGNED, S8, S16, S32, S64,
    FLOAT, F32, F64,
    STRING,
    STRUCT,
    ENUM,
    BOOL,
    // POINTER,
    // PROCEDURE,
    // VOID,
    // ARRAY,
    // ANY,
    // TYPE,
    UNKNOWN
};