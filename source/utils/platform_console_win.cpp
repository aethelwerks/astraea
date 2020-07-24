#include "include/utils/platform.hpp"
#ifdef OS_WINDOWS
#include "include/utils/platform_console.hpp"
#include "include/utils/platform_string.hpp"
#include <windows.h>

namespace platform {

uint64_t print(std::string_view str, uint32_t color)
{
    uint64_t count;
    std::wstring output = platform::utf8_to_winapi(str);
    void* console = GetStdHandle(STD_OUTPUT_HANDLE);
    auto csbi = CONSOLE_SCREEN_BUFFER_INFO{};
    GetConsoleScreenBufferInfo(console, &csbi);

    SetConsoleTextAttribute(console, color);
    WriteConsoleW(console, output.c_str(), output.size(), (LPDWORD) &count, 0);
    SetConsoleTextAttribute(console, csbi.wAttributes);

    return count;
}

uint64_t print(std::string_view str)
{
    return print(str, 3);
}

uint64_t print_rainbow(std::string_view str)
{
    uint64_t count;
    void* console = GetStdHandle(STD_OUTPUT_HANDLE);
    // you can loop k till 255 to see more color choices
    std::wstring output = L" I want to be nice today!";
    for (int color_attr = 1; color_attr < 15; color_attr += 1) {
        SetConsoleTextAttribute(console, color_attr);
        WriteConsoleW(console, output.c_str(), output.size(), (LPDWORD) &count, 0);
        WriteConsoleW(console, "\n", 1, (LPDWORD) &count, 0);
    }
    return count;
}
} // namespace platform
#endif
