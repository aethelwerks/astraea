/*
 * Copyright 2020 Krayfaus
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license that can be
 * found in the LICENSE file in the root directory of this source tree.
 */
#include "include/utils/platform_string.hpp"
#include <cstring>

namespace platform {

uint32_t
utf8_cp_size(std::string_view str)
{
    int32_t ch, i = 0, q = 0;
    for (; i < 4; i += 1) {
        ch = (unsigned char)str[i];
        if (ch >= 0 && ch <= 127) {
            return 1;
        } else if ((ch & 0xE0) == 0xC0) {
            return 2;
        } else if ((ch & 0xF0) == 0xE0) {
            return 3;
        } else if ((ch & 0xF8) == 0xF0) {
            return 4;
        } else {
            return 0;  // invalid utf8
        }
    }
    return -1;
}

int32_t
utf8_string_lenght(const char *str)
{
    int32_t c, i, ix, q;
    for (q = 0, i = 0, ix = (int32_t)strlen(reinterpret_cast<const char *>(str)); i < ix;
         i++, q++) {
        c = (unsigned char)str[i];
        if (c >= 0 && c <= 127)
            i += 0;
        else if ((c & 0xE0) == 0xC0)
            i += 1;
        else if ((c & 0xF0) == 0xE0)
            i += 2;
        else if ((c & 0xF8) == 0xF0)
            i += 3;
        // else if (($c & 0xFC) == 0xF8) i+=4; // 111110bb //byte 5, unnecessary in 4 byte UTF-8
        // else if (($c & 0xFE) == 0xFC) i+=5; // 1111110b //byte 6, unnecessary in 4 byte UTF-8
        else
            return 0;  // invalid utf8
    }
    return q;
}
}  // namespace platform
