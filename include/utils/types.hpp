/*
 * Copyright 2020 Krayfaus
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license that can be
 * found in the LICENSE file.
 */
#pragma once
#include <cstddef>
#include <cstdint>
#include <cstring>

template <typename Type>
static constexpr bool
to_bool(const Type &value)
{
    return 0 != value;
}

template <typename Type>
constexpr inline bool
is_pow2(Type value)
{
    return (value & (value - 1)) == 0;
}
