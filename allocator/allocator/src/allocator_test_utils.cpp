#include "../include/allocator_test_utils.h"
#include <sstream>

bool allocator_test_utils::block_info::operator==(
    allocator_test_utils::block_info const &other) const noexcept
{
    return block_size == other.block_size
           && is_block_occupied == other.is_block_occupied;
}

bool allocator_test_utils::block_info::operator!=(
    allocator_test_utils::block_info const &other) const noexcept
{
    return !(*this == other);
}

std::ostream& operator<<(std::ostream& os, const allocator_test_utils::block_info& block)
{
    os << "<" << (block.is_block_occupied ? "+" : "-") << ">"
        << "<" << block.block_size << ">";
    return os;
}

std::string allocator_test_utils::get_blocks_info_str(allocator_test_utils::block_info_type block_info_type) const noexcept
{
    auto blocks_info = get_blocks_info();
    std::ostringstream oss;
    oss << "[";
    for (auto i = 0; i < blocks_info.size(); i++)
    {
        if (block_info_type == allocator_test_utils::block_info_type::both ||
            blocks_info[i].is_block_occupied && block_info_type == allocator_test_utils::block_info_type::occipied ||
            !blocks_info[i].is_block_occupied && block_info_type == allocator_test_utils::block_info_type::avail)
        {
            oss << blocks_info[i];
            if (i + 1 < blocks_info.size())
            {
                oss << "|";
            }
        }
    }

    oss << "]";

    return oss.str();
}