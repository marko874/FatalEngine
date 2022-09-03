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
inline auto fatal_vk_assert = [](VkResult res) -> void
{
	if(res != VK_SUCCESS)
	{
		Logger::log<Logger::Level::Error>("fatal_vk_assert() failed.");
		debug_break();
	}
};

} // namespace Assert

#endif
