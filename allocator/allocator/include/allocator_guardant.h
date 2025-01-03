#ifndef MATH_PRACTICE_AND_OPERATING_SYSTEMS_ALLOCATOR_GUARDANT_H
#define MATH_PRACTICE_AND_OPERATING_SYSTEMS_ALLOCATOR_GUARDANT_H

#include "allocator.h"

class allocator_guardant
{

public:
    
    virtual ~allocator_guardant() noexcept = default;

public:
    
    [[nodiscard]] void *allocate_with_guard(
        size_t value_size,
        size_t values_count = 1) const;

    [[nodiscard]] void *reallocate_with_guard(
        void* at,
        size_t old_value_size,
        size_t old_values_count,
        size_t value_size,
        size_t values_count = 1) const;
    
    void deallocate_with_guard(
        void *at) const;

    [[nodiscard]] static void *allocate_with_guard_static(allocator* allocator,
        size_t value_size,
        size_t values_count = 1);

public:
    
    [[nodiscard]] inline virtual allocator *get_allocator() const = 0;
    
};

#endif //MATH_PRACTICE_AND_OPERATING_SYSTEMS_ALLOCATOR_GUARDANT_H
