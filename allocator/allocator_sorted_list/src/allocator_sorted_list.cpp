#include <not_implemented.h>

#include "../include/allocator_sorted_list.h"
#include <sstream>

constexpr size_t ALLOCATOR_META_SIZE            = (sizeof(allocator*) + 
                                                sizeof(size_t) + 
                                                sizeof(logger*) + 
                                                sizeof(std::mutex*) + 
                                                sizeof(allocator_with_fit_mode::fit_mode) + 
                                                sizeof(allocator_sorted_list::meta_block_info*));

constexpr size_t SPACE_SIZE_BYTES_SHIFT         = (sizeof(size_t));
constexpr size_t ALLOCATOR_BYTES_SHIFT          = (SPACE_SIZE_BYTES_SHIFT + sizeof(allocator*));
constexpr size_t LOGGER_BYTES_SHIFT             = (ALLOCATOR_BYTES_SHIFT + sizeof(logger*));
constexpr size_t MUTEX_BYTES_SHIFT              = (LOGGER_BYTES_SHIFT + sizeof(std::mutex*));
constexpr size_t FITMODE_BYTES_SHIFT            = (MUTEX_BYTES_SHIFT + sizeof(allocator_with_fit_mode::fit_mode));
constexpr size_t FIRST_FREE_BLOCK_BYTES_SHIFT   = (FITMODE_BYTES_SHIFT + sizeof(allocator_sorted_list::meta_block_info*));

#pragma region Constructors, Operators

allocator_sorted_list::~allocator_sorted_list()
{
    free_memory();
    //throw not_implemented("allocator_sorted_list::~allocator_sorted_list()", "your code should be here...");
}

allocator_sorted_list::allocator_sorted_list(
    allocator_sorted_list const& other)
{
    auto space_size = other.get_space_size();
    auto full_size = space_size + ALLOCATOR_META_SIZE;
    _trusted_memory = allocate_with_guard(full_size);
    //throw not_implemented("allocator_sorted_list::allocator_sorted_list(allocator_sorted_list const &)", "your code should be here...");
}

allocator_sorted_list& allocator_sorted_list::operator=(
    allocator_sorted_list const& other)
{
    if (this != &other)
    {
        free_memory();
        auto space_size = other.get_space_size();
        auto full_size = space_size + ALLOCATOR_META_SIZE;
        _trusted_memory = allocate_with_guard(full_size);
    }

    return *this;
    //throw not_implemented("allocator_sorted_list &allocator_sorted_list::operator=(allocator_sorted_list const &)", "your code should be here...");
}

allocator_sorted_list::allocator_sorted_list(
    allocator_sorted_list&& other) noexcept :
    _trusted_memory(other._trusted_memory)
{
    //throw not_implemented("allocator_sorted_list::allocator_sorted_list(allocator_sorted_list &&) noexcept", "your code should be here...");
}

allocator_sorted_list& allocator_sorted_list::operator=(
    allocator_sorted_list&& other) noexcept
{
    if (this != &other)
    {
        free_memory();
        _trusted_memory = other._trusted_memory;
        other._trusted_memory = nullptr;
    }

    return *this;
    //throw not_implemented("allocator_sorted_list &allocator_sorted_list::operator=(allocator_sorted_list &&) noexcept", "your code should be here...");
}


#pragma endregion

#pragma region Memory operating


allocator_sorted_list::allocator_sorted_list(
    size_t space_size,
    allocator* parent_allocator,
    logger* logger,
    allocator_with_fit_mode::fit_mode allocate_fit_mode) :
    _trusted_memory(nullptr)
{
    try
    {
        auto full_size = space_size + ALLOCATOR_META_SIZE;
        _trusted_memory = allocate_with_guard(full_size);
    }
    catch (const std::exception& error)
    {
        throw error;
    }

    set_space_size(space_size);
    set_allocator(parent_allocator);
    set_logger(logger);
    set_fit_mode(allocate_fit_mode);
    set_mutex(new std::mutex());

    auto first_block_char = reinterpret_cast<unsigned char*>(_trusted_memory) + ALLOCATOR_META_SIZE;
    auto free_block = reinterpret_cast<meta_block_info*>(first_block_char);
    free_block->assotiated_ptr = nullptr;
    free_block->base_info.block_size = space_size;
    free_block->base_info.is_block_occupied = false;

    set_first_free_block(free_block);
    //throw not_implemented("allocator_sorted_list::allocator_sorted_list(size_t, allocator *, logger *, allocator_with_fit_mode::fit_mode)", "your code should be here...");
}

[[nodiscard]] void* allocator_sorted_list::allocate(
    size_t value_size,
    size_t values_count)
{
    auto memory = reinterpret_cast<unsigned char*>(_trusted_memory);
    auto mutex = get_mutex();
    auto fit_mode = get_fit_mode();
    auto block_size = value_size * values_count;
    void* result = nullptr;
    try
    {
        mutex->lock();
        switch (fit_mode)
        {
        case allocator_with_fit_mode::fit_mode::first_fit:
            result = allocate_first_fit(block_size);
            break;
        case allocator_with_fit_mode::fit_mode::the_best_fit:
            result = allocate_best_fit(block_size);
            break;
        case allocator_with_fit_mode::fit_mode::the_worst_fit:
            result = allocate_worst_fit(block_size);
            break;
        default:
            std::ostringstream oss;
            oss << "Can`t found allocate method for " << static_cast<int>(fit_mode) << " fit mode" << '\n';
            log_with_guard(oss.str(), logger::severity::error);
            throw std::runtime_error(oss.str());
        }
    }
    catch (const std::exception& error)
    {
        mutex->unlock();
        log_with_guard(error.what(), logger::severity::error);
        throw error;
    }

    mutex->unlock();
    return result;
    //throw not_implemented("[[nodiscard]] void *allocator_sorted_list::allocate(size_t, size_t)", "your code should be here...");
}

[[nodiscard]] allocator_sorted_list::meta_block_info* allocator_sorted_list::allocate_block(meta_block_info* prev, meta_block_info* next, size_t size)
{
    if (prev == nullptr)
    {
        throw std::runtime_error("Can`t allocate memory in null block pointer");
    }

    auto total_size = size;
    if (prev->base_info.block_size - size > sizeof(allocator_sorted_list::meta_block_info))
    {
        total_size = prev->base_info.block_size;
    }

    auto result = prev + 1;
    result->assotiated_ptr = next;
    result->base_info.block_size = total_size;
    prev->assotiated_ptr = result;

    return result;
}

[[nodiscard]] allocator_sorted_list::meta_block_info* allocator_sorted_list::allocate_first_fit(size_t size)
{
    try
    {
        auto memory = get_first_free_block();
        if (memory != nullptr)
        {
            while (memory->assotiated_ptr != nullptr)
            {
                auto temp = *(reinterpret_cast<allocator_sorted_list::meta_block_info**>(memory->assotiated_ptr));
                if (memory->base_info.block_size < size)
                {
                    memory = temp;
                    continue;
                }

                return allocate_block(memory, temp, size);
            }
        }
    }
    catch (const std::bad_alloc& error)
    {
        std::ostringstream oss;
        oss << "Can`t found free space to allocate " << size << " bytes" << '\n';
        throw std::runtime_error(oss.str());
    }
    catch (const std::exception& error)
    {
        throw std::runtime_error(error.what());
    }
}

[[nodiscard]] allocator_sorted_list::meta_block_info* allocator_sorted_list::allocate_best_fit(size_t size)
{

}

[[nodiscard]] allocator_sorted_list::meta_block_info* allocator_sorted_list::allocate_worst_fit(size_t size)
{

}

void allocator_sorted_list::deallocate(
    void* at)
{
    throw not_implemented("void allocator_sorted_list::deallocate(void *)", "your code should be here...\n");
}

void allocator_sorted_list::free_memory()
{
    auto free_block = get_first_free_block();
    //make here clear

    deallocate_with_guard(_trusted_memory);
}

#pragma endregion

#pragma region get - set

inline size_t allocator_sorted_list::get_space_size() const
{
    auto memory = reinterpret_cast<unsigned char*>(_trusted_memory) + SPACE_SIZE_BYTES_SHIFT;
    auto size_value = *(reinterpret_cast<size_t*>(memory));
    return size_value;
}

inline void allocator_sorted_list::set_space_size(size_t size_value)
{
    auto memory = reinterpret_cast<unsigned char*>(_trusted_memory) + SPACE_SIZE_BYTES_SHIFT;
    *(reinterpret_cast<size_t*>(memory)) = size_value;
}

inline allocator_with_fit_mode::fit_mode allocator_sorted_list::get_fit_mode() const
{
    auto memory = reinterpret_cast<unsigned char*>(_trusted_memory) + FITMODE_BYTES_SHIFT;
    auto fitmode_value = *(reinterpret_cast<allocator_with_fit_mode::fit_mode*>(memory));
    return fitmode_value;
}

inline void allocator_sorted_list::set_fit_mode(allocator_with_fit_mode::fit_mode mode)
{
    auto memory = reinterpret_cast<unsigned char*>(_trusted_memory) + FITMODE_BYTES_SHIFT;
    auto fitmode_value = *(reinterpret_cast<allocator_with_fit_mode::fit_mode*>(memory));
    fitmode_value = mode;
    //throw not_implemented("inline void allocator_sorted_list::set_fit_mode(allocator_with_fit_mode::fit_mode)", "your code should be here...");
}

inline allocator* allocator_sorted_list::get_allocator() const
{
    auto memory = reinterpret_cast<unsigned char*>(_trusted_memory) + ALLOCATOR_BYTES_SHIFT;
    auto allocator_value = *(reinterpret_cast<allocator**>(memory));
    return allocator_value;
    //throw not_implemented("inline allocator *allocator_sorted_list::get_allocator() const", "your code should be here...");
}

inline void allocator_sorted_list::set_allocator(allocator* allocator_value)
{
    auto memory = reinterpret_cast<unsigned char*>(_trusted_memory) + ALLOCATOR_BYTES_SHIFT;
    *(reinterpret_cast<allocator**>(memory)) = allocator_value;
}

std::vector<allocator_test_utils::block_info> allocator_sorted_list::get_blocks_info() const noexcept
{
    std::vector<allocator_test_utils::block_info> result{};
    auto free_block = get_first_free_block();
    if (free_block != nullptr)
    {
        do
        {
            result.push_back(free_block->base_info);
            free_block = reinterpret_cast<allocator_sorted_list::meta_block_info*>(free_block->assotiated_ptr);
        } while (free_block->assotiated_ptr != nullptr);
    }

    return result;
    //throw not_implemented("std::vector<allocator_test_utils::block_info> allocator_sorted_list::get_blocks_info() const noexcept", "your code should be here...");
}

inline logger* allocator_sorted_list::get_logger() const
{
    auto memory = reinterpret_cast<unsigned char*>(_trusted_memory) + LOGGER_BYTES_SHIFT;
    auto logger_value = *(reinterpret_cast<logger**>(memory));
    return logger_value;
    //throw not_implemented("inline logger *allocator_sorted_list::get_logger() const", "your code should be here...");
}

inline void allocator_sorted_list::set_logger(logger* logger_value)
{
    auto memory = reinterpret_cast<unsigned char*>(_trusted_memory) + LOGGER_BYTES_SHIFT;
    *(reinterpret_cast<logger**>(memory)) = logger_value;
}

inline std::mutex* allocator_sorted_list::get_mutex() const
{
    auto memory = reinterpret_cast<unsigned char*>(_trusted_memory) + MUTEX_BYTES_SHIFT;
    auto mutex_value = *(reinterpret_cast<std::mutex**>(memory));
    return mutex_value;
}

inline void allocator_sorted_list::set_mutex(std::mutex* mutex_value)
{
    auto memory = reinterpret_cast<unsigned char*>(_trusted_memory) + MUTEX_BYTES_SHIFT;
    *(reinterpret_cast<std::mutex**>(memory)) = mutex_value;
}

inline allocator_sorted_list::meta_block_info* allocator_sorted_list::get_first_free_block() const
{
    auto memory = reinterpret_cast<unsigned char*>(_trusted_memory) + FIRST_FREE_BLOCK_BYTES_SHIFT;
    auto free_block_ptr = *(reinterpret_cast<allocator_sorted_list::meta_block_info**>(memory));
    return free_block_ptr;
}

inline void allocator_sorted_list::set_first_free_block(allocator_sorted_list::meta_block_info* free_block_ptr)
{
    auto memory = reinterpret_cast<unsigned char*>(_trusted_memory) + FIRST_FREE_BLOCK_BYTES_SHIFT;
    *(reinterpret_cast<allocator_sorted_list::meta_block_info**>(memory)) = free_block_ptr;
}

inline std::string allocator_sorted_list::get_typename() const noexcept
{
    return "allocator_sorted_list";
    //throw not_implemented("inline std::string allocator_sorted_list::get_typename() const noexcept", "your code should be here...");
}

#pragma endregion