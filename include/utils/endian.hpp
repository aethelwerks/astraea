/*
 * Copyright 2020 Krayfaus
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license that can be
 * found in the LICENSE file in the root directory of this source tree.
 */
#pragma once
#include "platform.hpp"
#include "types.hpp"
#include <algorithm>

#if _MSVC_LANG > 201703L
#include <bit>
using Endian = std::endian;
#else
enum class Endian {
#ifdef OS_WINDOWS
    little = 0,
    big = 1,
    native = little
#elif defined OS_POSIX
    little = __ORDER_LITTLE_ENDIAN__,
    big = __ORDER_BIG_ENDIAN__,
    native = __BYTE_ORDER__
#else
#error "Unsupported platform. Please define its endianess."
#endif
};
#endif

template <typename Type> void
endian_swap(Type &data)
{
    uint8_t* mem = reinterpret_cast<uint8_t*>(&data);
    std::reverse(mem, mem + sizeof(Type));
}

template <typename Type>
inline constexpr void
maybe_endian_swap(Type* data, uint32_t lenght, Endian endian)
{
    if (Endian::native != endian && sizeof(Type) > sizeof(uint8_t)) {
        for (uint32_t i = 0; i < lenght; i += 1) {
            endian_swap<Type>(data[i]);
        }
    }
}
