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

template <typename T>
static constexpr bool
to_bool(const T& x)
{
    return 0 != x;
}

template <typename T>
constexpr inline bool
is_pow2(T value)
{
    return (value & (value - 1)) == 0;
}
