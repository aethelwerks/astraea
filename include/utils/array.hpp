/*
 * Copyright 2020 Krayfaus
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license that can be
 * found in the LICENSE file in the root directory of this source tree.
 */
#pragma once
#include "types.hpp"
#include <algorithm>
#include <cassert>

template <typename Type>
struct Array {
    Type* data;           // pointer to the elements in the array.
    uint32_t count;       // number of elements stored in the array.
    uint32_t capacity;    // number of allocated slots in the array.
    bool is_unique : 1;   // whether the array owns its memory or not.

    ~Array() {
        if (data != nullptr && is_unique) {
            std::free(data);
        }
    }
};

#define MIN_ARRAY_LEN 8

template <typename Type>
Array<Type>
array_init(uint32_t reserve_count = 0)
{
    Array<Type> array;
    array.count = 0;
    if (!reserve_count) {
        array.capacity = 0;
        array.data = nullptr;
        array.is_unique = true;
        return array;
    }
    array.capacity = reserve_count > MIN_ARRAY_LEN ? reserve_count : MIN_ARRAY_LEN;
    array.data = (Type*) std::calloc((size_t) array.capacity, sizeof(Type));
    array.is_unique = true;

    return array;
}

template <typename Type>
void
array_resize(Array<Type>& array, uint32_t new_capacity)
{
    array.capacity = is_pow2(new_capacity) ? new_capacity : new_capacity + 1;
    assert(array.capacity >= new_capacity);
    array.count = std::min(array.count, array.capacity);

    Type* new_data = (Type*) std::calloc((size_t) array.capacity, sizeof(Type));
    if (new_data && array.count > 0) {
        std::memcpy(new_data, array.data, sizeof(Type) * array.count);
    }
    if (array.data && array.is_unique) {
        std::free(array.data);
    }

    array.data = new_data;
    array.is_unique = true;
}

template <typename Type>
void
array_maybe_grow(Array<Type>& array)
{
    if (array.count + 1 < array.capacity) {
        return;
    }
    array_resize<Type>(array, array.count + MIN_ARRAY_LEN);
}

template <typename Type>
void
array_add(Array<Type>& array, Type element)
{
    array_maybe_grow(array);
    array.data[array.count] = element;
    array.count += 1;
}

template <typename Type>
void
array_remove(Array<Type>& array, Type element)
{
    for (uint32_t i = 0; i < array.count; i += 1) {
        if (std::memcmp(&array.data[i], &element, sizeof(Type)) == 0) {
            uint32_t next_at = i + 1;
            if (next_at < array.count) {
                int32_t n_after = array.count - next_at;
                std::memmove(
                    &array.data[i],
                    &array.data[next_at],
                    n_after * sizeof(Type)
                );
                uint32_t reset_at = i + n_after;
                assert(reset_at < array.count);
                std::memset(&array.data[reset_at], 0, sizeof(Type));
            } else {
                std::memset(&array.data[i], 0, sizeof(Type));
            }
            array.count -= 1;
            break;
        }
    }
}

template <typename Type>
Type*
array_first(Array<Type>& array)
{
    return array.data;
}

template <typename Type>
Type*
array_last(Array<Type>& array)
{
    return array.data ? array.data + array.count : nullptr;
}
