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

#define MIN_ARRAY_LEN 8

template <typename Type>
struct Array {
    Type *data;          // pointer to the elements in the array.
    uint32_t count;      // number of elements stored in the array.
    uint32_t capacity;   // number of allocated slots in the array.
    bool is_unique : 1;  // whether the array owns its memory or not.

    Array(uint32_t reserve_count = 0)
    {
        this->count = 0;
        if (!reserve_count) {
            this->capacity = 0;
            this->data = nullptr;
            this->is_unique = true;
            return;
        }
        this->capacity = reserve_count > MIN_ARRAY_LEN ? reserve_count : MIN_ARRAY_LEN;
        this->data = (Type *)std::calloc((size_t)this->capacity, sizeof(Type));
        this->is_unique = true;
    }

    ~Array()
    {
        if (data != nullptr && is_unique) {
            std::free(data);
        }
    }

    auto
    begin()
    {
        return this->data;
    }

    auto
    end()
    {
        return this->data ? this->data + this->count : nullptr;
    }

    void
    push(Type element)
    {
        maybe_grow(array);
        this->data[this->count] = element;
        this->count += 1;
    }

    void
    pop(Type element)
    {
        for (uint32_t i = 0; i < this->count; i += 1) {
            if (std::memcmp(&this->data[i], &element, sizeof(Type)) == 0) {
                uint32_t next_at = i + 1;
                if (next_at < this->count) {
                    int32_t n_after = this->count - next_at;
                    std::memmove(
                        &this->data[i],
                        &this->data[next_at],
                        n_after * sizeof(Type));
                    uint32_t reset_at = i + n_after;
                    assert(reset_at < this->count);
                    std::memset(&this->data[reset_at], 0, sizeof(Type));
                } else {
                    std::memset(&this->data[i], 0, sizeof(Type));
                }
                this->count -= 1;
                break;
            }
        }
    }

    static void
    resize(Array<Type> &array, uint32_t new_capacity)
    {
        array.capacity = is_pow2(new_capacity) ? new_capacity : new_capacity + 1;
        assert(array.capacity >= new_capacity);
        array.count = std::min(array.count, array.capacity);

        Type *new_data = (Type *)std::calloc((size_t)array.capacity, sizeof(Type));
        if (new_data && array.count > 0) {
            std::memcpy(new_data, array.data, sizeof(Type) * array.count);
        }
        if (array.data && array.is_unique) {
            std::free(array.data);
        }

        array.data = new_data;
        array.is_unique = true;
    }

    static void
    maybe_grow(Array<Type> &array)
    {
        if (array.count + 1 < array.capacity) {
            return;
        }
        array_resize<Type>(array, array.count + MIN_ARRAY_LEN);
    }
};
