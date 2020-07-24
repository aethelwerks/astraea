/*
 * Copyright 2020 Krayfaus
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license that can be
 * found in the LICENSE file in the root directory of this source tree.
 */
#pragma once

#if __APPLE__
#define OS_APPLE
#define OS_POSIX
#elif __linux__
#define OS_LINUX
#define OS_POSIX
#elif defined(_MSC_VER) || defined(WIN32)|| defined(_WIN32)
#define OS_WINDOWS
#else
#error "Unsuported platform."
#endif
