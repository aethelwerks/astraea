/*
 * Copyright 2020 Krayfaus
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license that can be
 * found in the LICENSE file in the root directory of this source tree.
 */
#pragma once
#include "array.hpp"
#include "endian.hpp"
#include "platform_string.hpp"
#include <filesystem>
#include <fstream>

namespace binaryreader {
using File = std::fstream;
using Offset = std::streampos;
using Path = std::filesystem::path;
using Stream = std::iostream;

/*
 * Opens a file handle.
 */
inline File
open_file(Path file_path)
{
#ifdef OS_WINDOWS
    return File(platform::utf8_to_winapi(file_path.string()), File::in | File::out | File::binary);
#elif defined OS_POSIX
    return File(file_path.string(), File::in | File::out | File::binary);
#endif
}

/*
 * Closes a file handle.
 */
inline void
close_file(File& file)
{
    file.close();
}

/**
 * Get current offset of the stream.
 */
inline Offset
tell(Stream& file)
{
    return file.tellp();
}

/**
 * Seek an offset in the stream.
 */
inline void
seek(Stream& file, Offset pos)
{
    file.seekp(pos);
    if (file.bad()) {
        // std::cerr << "Error while seeking pos in file." << std::endl;
    }
}

inline uint64_t
file_size(Stream& file)
{
    auto old_pos = tell(file);
    seek(file, 0x0);
    file.ignore(std::numeric_limits<std::streamsize>::max());
    auto lenght = file.gcount();
    file.clear();
    seek(file, old_pos);
    return (uint64_t) lenght;
}

/*
 * Reads basic type from stream.
 */
template <typename Type>
constexpr auto
read(Stream& file, Endian endian = Endian::native)
{
    auto data = Type{};
    file.read(reinterpret_cast<char*>(&data), sizeof(Type));
    if (file.fail()) {
        // std::cerr << "Error while reading file." << std::endl;
        // std::cerr << "Only " << fs.gcount() << " bytes could be read." << std::endl;
    }
    maybe_endian_swap(&data, 1, endian);

    return data;
}

/*
 * Read an array of _lenght_ Types
 */
template <typename Type, uint32_t lenght>
constexpr auto
read(Stream& file, Endian endian = Endian::native)
{
    auto array = array_init<Type>(lenght);
    array.count = lenght;
    file.read(reinterpret_cast<char*>(array.data), sizeof(Type) * lenght);
    if (file.fail()) {
        // std::cerr << "Error while reading file." << std::endl;
        // std::cerr << "Only " << fs.gcount() << " bytes could be read." << std::endl;
    }
    maybe_endian_swap(array.data, lenght, endian);

    return array;
}

/*
 * Read an array of Types from _beg_ till _end_..
 */
template <typename Type>
Array<Type>
read(Stream &file, Offset begin, Offset end, Endian endian = Endian::native)
{
    assert((end - begin) % sizeof(Type) == 0); // "It's not possible to fit the requested array lenght in the offsets."
    uint32_t lenght = (uint32_t)(end - begin) / sizeof(Type);
    auto array = array_init<Type>(lenght);
    array.count = lenght;

    auto old_pos = tell(file);
    seek(file, begin);
    file.read(reinterpret_cast<char*>(array.data), sizeof(Type) * lenght);
    if (file.fail()) {
        // std::cerr << "Error while reading file." << std::endl;
        // std::cerr << "Only " << fs.gcount() << " bytes could be read." << std::endl;
    }
    seek(file, old_pos);
    maybe_endian_swap(array.data, lenght, endian);

    return array;
}

/*
 * Read an array of _lenght_ Types to &data.
 */
template <typename Type, uint32_t lenght>
void
read(Stream &file, Type(&r_data)[lenght], Endian endian = Endian::native)
{
    file.read(reinterpret_cast<char*>(r_data), sizeof(Type) * lenght);
    if (file.fail()) {
        // std::cerr << "Error while reading file." << std::endl;
        // std::cerr << "Only " << fs.gcount() << " bytes could be read." << std::endl;
    }
    maybe_endian_swap(r_data, lenght, endian);
}

} // namespace binaryreader
