#include "../include/allocator_guardant.h"

#include <algorithm>
#include <new>
#include <string.h>

void *allocator_guardant::allocate_with_guard(
    size_t value_size,
    size_t values_count) const
{
    allocator *target_allocator = get_allocator();
    return target_allocator == nullptr
        ? ::operator new(value_size * values_count)
        : target_allocator->allocate(value_size, values_count);
}

void *allocator_guardant::reallocate_with_guard(
    void* at,
    size_t old_value_size,
    size_t old_values_count,
    size_t value_size,
    size_t values_count) const
{
    if (at == nullptr)
    {
        return allocate_with_guard(value_size, values_count);
    }

    auto old_size = old_value_size * old_values_count;
    auto new_size = value_size + values_count;

    if (old_size == new_size)
    {
        return at;
    }

    void *new_block = allocate_with_guard(value_size, values_count);
    if (new_block == nullptr)
    {
        throw std::bad_alloc();
    }

    memcpy(new_block, at, std::min(old_size, new_size));
    deallocate_with_guard(at);

    return new_block;
}

void *allocator_guardant::allocate_with_guard_static(
    allocator *allocator,
    size_t value_size,
    size_t values_count)
{
    return allocator == nullptr
        ? ::operator new(value_size * values_count)
        : allocator->allocate(value_size, values_count);
}


void allocator_guardant::deallocate_with_guard(
    void *at) const
{
    allocator *target_allocator = get_allocator();
    return target_allocator == nullptr
        ? ::operator delete(at)
        : target_allocator->deallocate(at);
}