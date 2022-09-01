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

} // namespace Assert

#endif
