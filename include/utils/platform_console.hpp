/*
 * Copyright 2020 Krayfaus
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license that can be
 * found in the LICENSE file in the root directory of this source tree.
 */
#pragma once
#include "platform.hpp"
#include "platform_string.hpp"
#include "types.hpp"
#include <string_view>

namespace platform {
/*
 * Prints an UTF-8 string to the platform native console.
 */
uint64_t print(std::string_view str);

uint64_t print(std::string_view str, uint32_t color);
} // namespace platform
