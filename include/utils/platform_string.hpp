/*
 * Copyright 2020 Krayfaus
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license that can be
 * found in the LICENSE file in the root directory of this source tree.
 */
#pragma once
#include "types.hpp"
#include <string>       // IWYU pragma: export
#include <string_view>  // IWYU pragma: export

namespace platform {

/*
 * The size of an UTF-8 code point.
 */
uint32_t utf8_cp_size(std::string_view);

/*
 * The lenght of an UTF-8 encoded string.
 */
int32_t utf8_string_lenght(const char *);

/*
 * Convert from Windows API string to UTF-8 string.
 */
std::string winapi_to_utf8(const wchar_t *, const wchar_t *);

/*
 * Convert from Windows API string to UTF-8 string.
 */
std::string winapi_to_utf8(const std::wstring_view &);

/*
 * Convert from UTF-8 string to Windows API string.
 */
std::wstring utf8_to_winapi(const std::string_view &);

/*
 * Convert from UTF-8 string to UTF-16 string.
 */
// std::u16string utf8_to_utf16(const char *str);

/*
 * Convert from UTF-16 string to UTF-8 string.
 */
// std::string utf16_to_utf8(const char16_t *str);

/*
 * Convert from UTF-8 string to UTF-32 string.
 */
// std::u32string utf8_to_utf32(const char *str);

/*
 * Convert from UTF-32 string to UTF-8 string.
 */
// std::string utf32_to_utf8(const char32_t *str);

}  // namespace platform
