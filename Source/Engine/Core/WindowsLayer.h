#pragma once

#ifdef _WIN32

#include "PlatformLayer.h"

#include <Windows.h>
#include <iostream>
#include <utility>
#include <windowsx.h>

namespace Platform
{
template <uint8_t Color, typename... T> inline void write_to_console(DWORD handle, T &&...args)
{
    static_assert(Color <= 5);

    HANDLE console = GetStdHandle(handle);
    SetConsoleTextAttribute(console, LEVELS[Color]);

    auto write_console = [=](const char *msg) {
        uint64_t len = strlen(msg);
        WriteConsoleA(GetStdHandle(handle), msg, static_cast<DWORD>(len), 0, 0);
    };

    (write_console(args), ...);
}
} // namespace Platform
#endif
