#pragma once

#include "../Defines.h"
#include "Logger.h"
#include <cstdint>

#define USE_ASSERT

#ifdef USE_ASSERT
#if _MSC_VER
#include <intrin.h>
#define debug_break() __debugbreak()
#else
#define debug_break() __builtin_trap()
#endif

namespace Assert
{
constexpr inline void fatal_assert(bool expr, const char *msg = "")
{
    if (!expr)
    {
        Logger::log(Logger::Level::Fatal,
                    {reinterpret_cast<const char *>(expr), msg, __FILE__, reinterpret_cast<const char *>(__LINE__)});
    }
}
} // namespace Assert

#endif
