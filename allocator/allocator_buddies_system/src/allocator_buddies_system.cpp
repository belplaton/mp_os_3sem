#include <not_implemented.h>

#include "../include/allocator_buddies_system.h"
#include <sstream>

#pragma region Constructors, Operators

allocator_buddies_system::~allocator_buddies_system()
{
    std::ostringstream oss;
    oss << "Destructor called for object of type: " << get_typename() << '\n';
    log_with_guard(oss.str(), logger::severity::trace);

    oss.str("");
    oss << "Destructor finished for object of type: " << get_typename() << '\n';
    log_with_guard(oss.str(), logger::severity::trace);

    free_memory();
    //throw not_implemented("allocator_buddies_system::~allocator_buddies_system()", "your code should be here...");
}

allocator_buddies_system::allocator_buddies_system(
    allocator_buddies_system &&other) noexcept :
    _trusted_memory(other._trusted_memory)
{
    std::ostringstream oss;
    oss << "Move-constructor called for object of type: " << get_typename() << '\n';
    log_with_guard(oss.str(), logger::severity::trace);

    oss.str("");
    oss << "Move-constructor finished for object of type: " << get_typename() << '\n';
    log_with_guard(oss.str(), logger::severity::trace);
    //throw not_implemented("allocator_buddies_system::allocator_buddies_system(allocator_buddies_system &&) noexcept", "your code should be here...");
}

allocator_buddies_system &allocator_buddies_system::operator=(
    allocator_buddies_system &&other) noexcept
{
    std::ostringstream oss;
    oss << "Move-operator '=' called for object of type: " << get_typename() << '\n';
    log_with_guard(oss.str(), logger::severity::trace);

    if (this != &other)
    {
        free_memory();
        _trusted_memory = other._trusted_memory;
        other._trusted_memory = nullptr;
    }

    oss.str("");
    oss << "Move-operator '=' finished for object of type: " << get_typename() << '\n';
    log_with_guard(oss.str(), logger::severity::trace);

    return *this;
    //throw not_implemented("allocator_buddies_system &allocator_buddies_system::operator=(allocator_buddies_system &&) noexcept", "your code should be here...");
}

allocator_buddies_system::allocator_buddies_system(
    size_t space_size_power_of_two,
    allocator *parent_allocator,
    logger *logger,
    allocator_with_fit_mode::fit_mode allocate_fit_mode) :
    _trusted_memory(nullptr)
{
    std::ostringstream oss;
    oss << "Constructor called for object of type: " << get_typename() << '\n';
    if (logger != nullptr)
        logger->log(oss.str(), logger::severity::trace);

    if (1 << space_size_power_of_two < FREE_BLOCK_META_SIZE)
    {
        space_size_power_of_two = nearest_power_of_two(FREE_BLOCK_META_SIZE) + 1;
    }

    try
    {
        size_t full_size = (1 << space_size_power_of_two) + ALLOCATOR_META_SIZE;
        _trusted_memory = parent_allocator == nullptr
            ? ::operator new(full_size)
            : parent_allocator->allocate(full_size, 1);

        oss.str("");
        oss << "Allocated " << (1 << space_size_power_of_two) << " bytes of usable memory and " << ALLOCATOR_META_SIZE << " bytes of allocator meta data" << '\n';
        if (logger != nullptr)
            logger->log(oss.str(), logger::severity::debug);
    }
    catch (const std::exception& error)
    {
        parent_allocator == nullptr
            ? ::operator delete(_trusted_memory)
            : parent_allocator->deallocate(_trusted_memory);

        _trusted_memory = nullptr;
        if (logger != nullptr)
            logger->log(error.what(), logger::severity::error);

        throw error;
    }

    set_space_degree(space_size_power_of_two);
    set_allocator(parent_allocator);
    set_logger(logger);
    set_fit_mode(allocate_fit_mode);
    init_mutex();

    auto block_size = space_size_power_of_two - FREE_BLOCK_META_SIZE;
    auto block_char = reinterpret_cast<unsigned char*>(_trusted_memory) + ALLOCATOR_META_SIZE;

    free_block_set_prev(block_char, nullptr);
    free_block_set_next(block_char, nullptr);
    block_set_degree(block_char, space_size_power_of_two);
    set_first_free_block(block_char);

    oss.str("");
    oss << "Constructor finished for object of type: " << get_typename() << '\n';
    if (logger != nullptr)
        logger->log(oss.str(), logger::severity::trace);
    //throw not_implemented("allocator_buddies_system::allocator_buddies_system(size_t, allocator *, logger *, allocator_with_fit_mode::fit_mode)", "your code should be here...");
}

#pragma endregion

[[nodiscard]] void *allocator_buddies_system::allocate(
    size_t value_size,
    size_t values_count)
{
    throw not_implemented("[[nodiscard]] void *allocator_buddies_system::allocate(size_t, size_t)", "your code should be here...");
}

void allocator_buddies_system::deallocate(
    void *at)
{
    throw not_implemented("void allocator_buddies_system::deallocate(void *)", "your code should be here...");
}

#pragma region get - set block

unsigned long long allocator_buddies_system::global_to_local(unsigned char* at_char) const
{
    auto temp = reinterpret_cast<unsigned long long>(at_char);
    temp -= reinterpret_cast<unsigned long long>(_trusted_memory);
    return temp;
}

unsigned char* allocator_buddies_system::local_to_global(unsigned long long at_char) const
{
    auto temp = at_char;
    temp += reinterpret_cast<unsigned long long>(_trusted_memory);
    return reinterpret_cast<unsigned char*>(temp);
}

unsigned char* allocator_buddies_system::block_get_ptr(unsigned char* at_char) const
{
    auto current = at_char + BLOCK_PTR_BYTES_SHIFT;
    return *reinterpret_cast<unsigned char**>(current);
}

void allocator_buddies_system::block_set_ptr(unsigned char* at_char, unsigned char* ptr)
{
    auto current = at_char + BLOCK_PTR_BYTES_SHIFT;
    *reinterpret_cast<unsigned char**>(current) = ptr;
}

size_t allocator_buddies_system::block_get_degree(unsigned char* at_char) const
{
    auto current = at_char + BLOCK_PTR_BYTES_SHIFT;
    return *reinterpret_cast<size_t*>(current);
}

void allocator_buddies_system::block_set_degree(unsigned char* at_char, size_t degree)
{
    auto current = at_char + BLOCK_PTR_BYTES_SHIFT;
    *reinterpret_cast<size_t*>(current) = degree;
}

unsigned char* allocator_buddies_system::block_get_buddie(unsigned char* at_char, size_t degree) const
{
    auto at_char_local = global_to_local(at_char);
    auto buddie_local = at_char_local ^ (1 << degree);
    return local_to_global(buddie_local);
}

unsigned char* allocator_buddies_system::block_get_buddie(unsigned char* at_char) const
{
    auto degree = block_get_degree(at_char);
    return block_get_buddie(at_char, degree);
}

bool allocator_buddies_system::block_is_free(unsigned char* at_char_start) const
{
    auto ptr = block_get_ptr(at_char_start);
    return ptr != reinterpret_cast<unsigned char*>(_trusted_memory);
}

unsigned char* allocator_buddies_system::free_block_get_prev(unsigned char* at_char) const
{
    auto current = at_char + BLOCK_PTR_BYTES_SHIFT;
    return *reinterpret_cast<unsigned char**>(current);
}

unsigned char* allocator_buddies_system::free_block_get_next(unsigned char* at_char) const
{
    auto current = at_char + FREE_BLOCK_PTR_BYTES_SHIFT;
    return *reinterpret_cast<unsigned char**>(current);
}

void allocator_buddies_system::free_block_set_prev(unsigned char* at_char, unsigned char* prev)
{
    auto current = at_char + BLOCK_PTR_BYTES_SHIFT;
    *reinterpret_cast<unsigned char**>(current) = prev;
}

void allocator_buddies_system::free_block_set_next(unsigned char* at_char, unsigned char* next)
{
    auto current = at_char + FREE_BLOCK_PTR_BYTES_SHIFT;
    *reinterpret_cast<unsigned char**>(current) = next;
}

#pragma endregion

#pragma region get - set main

inline size_t allocator_buddies_system::get_space_degree() const
{
    auto memory = reinterpret_cast<unsigned char*>(_trusted_memory) + BLOCK_DEGREE_BYTES_SHIFT;
    auto size_value = *(reinterpret_cast<size_t*>(memory));
    return size_value;
}

inline void allocator_buddies_system::set_space_degree(size_t size_value)
{
    auto memory = reinterpret_cast<unsigned char*>(_trusted_memory) + BLOCK_DEGREE_BYTES_SHIFT;
    *(reinterpret_cast<size_t*>(memory)) = size_value;
}

inline allocator_with_fit_mode::fit_mode allocator_buddies_system::get_fit_mode() const
{
    auto memory = reinterpret_cast<unsigned char*>(_trusted_memory) + FITMODE_BYTES_SHIFT;
    auto fitmode_value = *(reinterpret_cast<allocator_with_fit_mode::fit_mode*>(memory));
    return fitmode_value;
}

inline void allocator_buddies_system::set_fit_mode(
    allocator_with_fit_mode::fit_mode mode)
{
    std::ostringstream oss;
    oss << "Set fit mode method init in " << get_typename() << " with mode: " << (int)mode << '\n';
    log_with_guard(oss.str(), logger::severity::debug);

    auto memory = reinterpret_cast<unsigned char*>(_trusted_memory) + FITMODE_BYTES_SHIFT;
    *(reinterpret_cast<allocator_with_fit_mode::fit_mode*>(memory)) = mode;

    oss.str("");
    oss << "Set fit mode method finish in " << get_typename() << " with mode: " << (int)mode << '\n';
    log_with_guard(oss.str(), logger::severity::debug);
    //throw not_implemented("inline void allocator_boundary_tags::set_fit_mode(allocator_with_fit_mode::fit_mode)", "your code should be here...");
}

inline allocator* allocator_buddies_system::get_allocator() const
{
    auto memory = reinterpret_cast<unsigned char*>(_trusted_memory) + ALLOCATOR_BYTES_SHIFT;
    auto allocator_value = *reinterpret_cast<allocator**>(memory);
    return allocator_value;
    //throw not_implemented("inline allocator *allocator_boundary_tags::get_allocator() const", "your code should be here...");
}

inline void allocator_buddies_system::set_allocator(allocator* allocator_value)
{
    auto memory = reinterpret_cast<unsigned char*>(_trusted_memory) + ALLOCATOR_BYTES_SHIFT;
    *(reinterpret_cast<allocator**>(memory)) = allocator_value;
}

std::vector<allocator_test_utils::block_info> allocator_buddies_system::get_blocks_info() const noexcept
{
    std::vector<allocator_test_utils::block_info> result{};

    auto current = reinterpret_cast<unsigned char*>(_trusted_memory) + ALLOCATOR_META_SIZE;
    auto trusted_memory_char = reinterpret_cast<unsigned char*>(_trusted_memory);
    auto trusted_memory_char_end = trusted_memory_char + ALLOCATOR_META_SIZE + (1 << get_space_degree());

    if (current != nullptr)
    {
        do
        {
            auto info = block_info();
            info.is_block_occupied = !block_is_free(current);
            if (info.is_block_occupied)
            {
                info.block_size = block_get_degree(current) + CAPTURED_BLOCK_META_SIZE;
                current += info.block_size;
            }
            else
            {
                info.block_size = block_get_degree(current) + FREE_BLOCK_META_SIZE;
                current += info.block_size;
            }

            result.push_back(info);
        } while (current != nullptr && current < trusted_memory_char_end);
    }

    return result;
    //throw not_implemented("std::vector<allocator_test_utils::block_info> allocator_boundary_tags::get_blocks_info() const noexcept", "your code should be here...");
}

inline logger* allocator_buddies_system::get_logger() const
{
    auto memory = reinterpret_cast<unsigned char*>(_trusted_memory) + LOGGER_BYTES_SHIFT;
    auto logger_value = *(reinterpret_cast<logger**>(memory));
    return logger_value;
    //throw not_implemented("inline logger *allocator_boundary_tags::get_logger() const", "your code should be here...");
}

inline void allocator_buddies_system::set_logger(logger* logger_value)
{
    auto memory = reinterpret_cast<unsigned char*>(_trusted_memory) + LOGGER_BYTES_SHIFT;
    *(reinterpret_cast<logger**>(memory)) = logger_value;
}

inline std::mutex* allocator_buddies_system::get_mutex() const
{
    auto memory = reinterpret_cast<unsigned char*>(_trusted_memory) + MUTEX_BYTES_SHIFT;
    auto mutex_value = reinterpret_cast<std::mutex*>(memory);
    return mutex_value;
}

inline void allocator_buddies_system::init_mutex()
{
    auto memory = reinterpret_cast<unsigned char*>(_trusted_memory) + MUTEX_BYTES_SHIFT;
    new (reinterpret_cast<std::mutex*>(memory)) std::mutex();
}

inline void* allocator_buddies_system::get_first_free_block() const
{
    auto memory = reinterpret_cast<unsigned char*>(_trusted_memory) + FIRST_FREE_BLOCK_BYTES_SHIFT;
    auto free_block_ptr = *(reinterpret_cast<void**>(memory));
    return free_block_ptr;
}

inline void allocator_buddies_system::set_first_free_block(void* free_block_ptr)
{
    auto memory = reinterpret_cast<unsigned char*>(_trusted_memory) + FIRST_FREE_BLOCK_BYTES_SHIFT;
    *(reinterpret_cast<void**>(memory)) = free_block_ptr;
}

inline std::string allocator_buddies_system::get_typename() const noexcept
{
    return ("allocator_buddies_system");
    //throw not_implemented("inline std::string allocator_boundary_tags::get_typename() const noexcept", "your code should be here...");
}

unsigned int allocator_buddies_system::nearest_power_of_two(unsigned int N)
{
    if (N < 1) return 0;

    int exponent = static_cast<int>(std::floor(std::log2(N)));
    return static_cast<unsigned int>(std::pow(2, exponent));
}

#pragma endregion