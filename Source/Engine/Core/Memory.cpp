#include "Memory.h"

#include "Logger.h"
#include "PlatformLayer.h"
#include "WindowsLayer.h"

#include <string>

namespace Memory
{
struct AllocationStatistics
{
	uint64_t m_TotallyAllocatedBytes;
	uint64_t m_TotalFreedBytes;
};

static AllocationStatistics alloc_stats;

uint64_t get_current_allocations()
{
	return alloc_stats.m_TotallyAllocatedBytes - alloc_stats.m_TotalFreedBytes;
}

void print_usage()
{
	auto current_allocs = std::to_string(get_current_allocations());
	Logger::log<Logger::Level::Info>("Amount of non-freed bytes: ", current_allocs.data());
}

void* falloc(std::size_t size)
{
	alloc_stats.m_TotallyAllocatedBytes += size;
	return Platform::platform_allocate(size, false);
}

void ffree(void* block, std::size_t size)
{
	alloc_stats.m_TotalFreedBytes += size;
	Platform::platform_free(block, false);
}

FATAL_API void* fzero_memory(void* block, uint64_t size)
{
	return Platform::platform_zero_memory(block, size);
}

FATAL_API void* fcopy_memory(void* destination, void const* source, uint64_t size)
{
	return Platform::platform_copy_memory(destination, source, size);
}

FATAL_API void* fset_memory(void* destination, int32_t value, uint64_t size)
{
	return Platform::platform_set_memory(destination, value, size);
}
} // namespace Memory
