/*
 * Copyright 2020 Krayfaus
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license that can be
 * found in the LICENSE file in the root directory of this source tree.
 */
#include "include/utils/platform.hpp"
#ifdef OS_WINDOWS
#include "include/utils/unicode.hpp"

namespace platform {
static_assert(sizeof(wchar_t) == 2, "wchar_t must be 2 bytes");

std::string
winapi_to_utf8(const wchar_t *strb, const wchar_t *stre)
{
    std::size_t needed_size = 0;
    for (const wchar_t *strtarget = strb; strtarget < stre;) {
        auto dr = unicode::utf16_to_code_point(strtarget, stre);
        auto er = unicode::code_point_to_utf8(dr.codepoint);
        needed_size += er.code_units_size;
        strtarget = dr.next;
    }

    auto u8str = std::string{"", 0};
    u8str.resize(needed_size);
    char *target = &u8str[0];

    for (const wchar_t *strtarget = strb; strtarget < stre;) {
        auto dr = unicode::utf16_to_code_point(strtarget, stre);
        auto er = unicode::code_point_to_utf8(dr.codepoint);
        const char *utf8data = er.code_units.data();
        std::memcpy(target, utf8data, er.code_units_size * sizeof(char));
        target += er.code_units_size;
        strtarget = dr.next;
    }

    return u8str;
}

std::string
winapi_to_utf8(const std::wstring_view &u16str)
{
    const wchar_t *strb = &u16str[0];
    const wchar_t *stre = strb + std::char_traits<wchar_t>::length(strb);

    return winapi_to_utf8(strb, stre);
}

std::wstring
utf8_to_winapi(const std::string_view &u8str)
{
    size_t len = std::char_traits<char>::length(u8str.data());
    std::size_t needed_size = 0;
    const char *utf8p = u8str.data();
    const char *strb = utf8p;
    const char *stre = utf8p + len;

    for (const char *strtarget = strb; strtarget < stre;) {
        auto dr = unicode::utf8_to_code_point(strtarget, stre);
        auto er = unicode::code_point_to_utf16(dr.codepoint);
        needed_size += er.code_units_size;
        strtarget = dr.next;
    }

    std::wstring r(needed_size, 0);
    r.resize(needed_size);
    wchar_t *target = &r[0];

    for (const char *strtarget = strb; strtarget < stre;) {
        auto dr = unicode::utf8_to_code_point(strtarget, stre);
        auto er = unicode::code_point_to_utf16(dr.codepoint);
        std::memcpy(target, er.code_units.data(), er.code_units_size * sizeof(wchar_t));
        strtarget = dr.next;
        target += er.code_units_size;
    }

    return r;
}
}  // namespace platform
#endif
