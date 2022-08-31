#include "Memory.h"
#include "Logger.h"
#include "PlatformLayer.h"

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
    std::cout << "Amount of non-freed bytes: " << get_current_allocations() << "\n";
}

void *falloc(std::size_t size)
{
    Memory::alloc_stats.m_TotallyAllocatedBytes += size;
    return Platform::platform_allocate(size, false);
}

void ffree(void *block, std::size_t size)
{
    Memory::alloc_stats.m_TotalFreedBytes += size;
    Platform::platform_free(block, false);
}

FATAL_API void *fzero_memory(void *block, uint64_t size)
{
    return Platform::platform_zero_memory(block, size);
}

FATAL_API void *fcopy_memory(void *destination, const void *source, uint64_t size)
{
    return Platform::platform_copy_memory(destination, source, size);
}

FATAL_API void *fset_memory(void *destination, int32_t value, uint64_t size)
{
    return Platform::platform_set_memory(destination, value, size);
}
} // namespace Memory
