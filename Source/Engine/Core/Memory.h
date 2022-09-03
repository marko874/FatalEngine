#pragma once

#include <FatalPCH.h>
#include <cstring>

namespace Memory
{
FATAL_API uint64_t get_current_allocations();

FATAL_API void print_usage();

// TODO: replace allocation functions with operator overloading
FATAL_API void* falloc(std::size_t size);

FATAL_API void ffree(void* block, std::size_t size);

FATAL_API void* fzero_memory(void* block, uint64_t size);

FATAL_API void* fcopy_memory(void* destination, const void* source, uint64_t size);

FATAL_API void* fset_memory(void* destination, int32_t value, uint64_t size);
} // namespace Memory
