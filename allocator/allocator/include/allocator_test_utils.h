#ifndef MATH_PRACTICE_AND_OPERATING_SYSTEMS_ALLOCATOR_TEST_UTILS_H
#define MATH_PRACTICE_AND_OPERATING_SYSTEMS_ALLOCATOR_TEST_UTILS_H

#include <cstddef>
#include <vector>
#include <iostream>

class allocator_test_utils
{

public:
    
    enum class block_info_type
    {
        avail = 0,
        occipied = 1,
        both = 2
    };

    struct block_info final
    {
        
        size_t block_size;
        
        bool is_block_occupied;
        
        bool operator==(
            block_info const &other) const noexcept;
        
        bool operator!=(
            block_info const &other) const noexcept;
        
        friend std::ostream& operator<<(std::ostream& os, const block_info& block);
    };

public:
    
    virtual ~allocator_test_utils() noexcept = default;

public:
    
    virtual std::vector<block_info> get_blocks_info() const noexcept = 0;

    std::string get_blocks_info_str(block_info_type block_info_type = block_info_type::both) const noexcept;

    std::string get_captured_block_data_str(void* at, size_t size) const noexcept;
    
};

#endif //MATH_PRACTICE_AND_OPERATING_SYSTEMS_ALLOCATOR_TEST_UTILS_H