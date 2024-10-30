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

    auto space_size = 1 << space_size_power_of_two;
    if (space_size < FREE_BLOCK_META_SIZE)
    {
        space_size_power_of_two = nearest_power_of_two(FREE_BLOCK_META_SIZE);
        space_size = 1 << space_size_power_of_two;
    }

    try
    {
        size_t full_size = space_size + ALLOCATOR_META_SIZE;
        _trusted_memory = parent_allocator == nullptr
            ? ::operator new(full_size)
            : parent_allocator->allocate(full_size, 1);

        oss.str("");
        oss << "Allocated " << space_size << " bytes of usable memory and " << ALLOCATOR_META_SIZE << " bytes of allocator meta data" << '\n';
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

    auto block_char = reinterpret_cast<unsigned char*>(_trusted_memory) + ALLOCATOR_META_SIZE;
    free_block_set_prev(block_char, nullptr);
    free_block_set_next(block_char, nullptr);
    block_set_degree(block_char, space_size_power_of_two);
    set_first_free_block(block_char);

    std::cout << reinterpret_cast<unsigned>(free_block_get_prev(block_char)) << " AND " << reinterpret_cast<unsigned>(free_block_get_next(block_char)) << std::endl;

    oss.str("");
    oss << "Memory in constructed allocator: " << get_blocks_info_str() << "\n";
    log_with_guard(oss.str(), logger::severity::debug);

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
    try
    {
        void* result = nullptr;
        auto mutex = get_mutex();
        std::lock_guard<std::mutex> lock(*mutex);

        std::ostringstream oss;
        oss << "Allocate memory method init in " << get_typename() << " for " << values_count << " elements with " << value_size << " bytes size" << '\n';
        log_with_guard(oss.str(), logger::severity::debug);

        if (values_count <= 0)
        {
            oss.str("");
            oss << "Allocate request redifined from " << values_count << " elements to 1 element in " << get_typename() << '\n';
            log_with_guard(oss.str(), logger::severity::warning);

            values_count = 1;
        }

        if (value_size * values_count > get_space_size())
        {
            oss.str("");
            oss << "Can`t allocate " << value_size * values_count << " bytes, because allocator limit is " << get_space_size() << " bytes." << '\n';
            throw std::runtime_error(oss.str());
        }

        auto fit_mode = get_fit_mode();
        auto block_size = value_size * values_count;

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
            oss.str("");
            oss << "Can`t found allocate method for " << static_cast<int>(fit_mode) << " fit mode" << '\n';
            throw std::runtime_error(oss.str());
        }

        oss.str("");
        oss << "Successfully allocated memory for " << values_count << " elements with " << value_size << " bytes size in " << get_typename() << '\n';
        log_with_guard(oss.str(), logger::severity::information);

        oss.str("");
        oss << "Memory after allocate: " << get_blocks_info_str() << "\n";
        log_with_guard(oss.str(), logger::severity::debug);

        oss.str("");
        oss << "Free memory left after allocate: " << get_blocks_info_str(block_info_type::avail) << "\n";
        log_with_guard(oss.str(), logger::severity::information);

        oss.str("");
        oss << "Allocate memory method finish in " << get_typename() << " for " << values_count << " elements with " << value_size << " bytes size" << '\n';
        log_with_guard(oss.str(), logger::severity::debug);

        return result;
    }
    catch (const std::exception& error)
    {
        log_with_guard(error.what(), logger::severity::error);
        throw std::bad_alloc();
    }
    //throw not_implemented("[[nodiscard]] void *allocator_buddies_system::allocate(size_t, size_t)", "your code should be here...");
}

void allocator_buddies_system::deallocate(
    void *at)
{
    throw not_implemented("void allocator_buddies_system::deallocate(void *)", "your code should be here...");
}

#pragma region Memory Operating

[[nodiscard]] unsigned char* allocator_buddies_system::allocate_block(unsigned char* current, unsigned char degree)
{
    if (current == nullptr)
    {
        throw std::runtime_error("Can`t allocate memory in null block pointer");
    }

    auto trusted_memory_char = reinterpret_cast<unsigned char*>(_trusted_memory);
    auto current_degree = block_get_degree(current);
    auto prev = free_block_get_prev(current);
    auto next = free_block_get_next(current);

    for (auto i = 0; i < current_degree - degree; i++)
    {
        if (!try_free_block_split(current))
        {
            break;
        }

        prev = free_block_get_prev(current);
        next = free_block_get_next(current);
    }

    auto buddie = block_get_buddie(current);
    block_set_ptr(current, trusted_memory_char);
    if (prev != nullptr)
    {
        free_block_set_next(prev, next);
    }
    else
    {
        set_first_free_block(current);
    }
    
    if (next != nullptr)
    {
        free_block_set_prev(next, prev);
    }

    return current + CAPTURED_BLOCK_META_SIZE;
}

[[nodiscard]] unsigned char* allocator_buddies_system::allocate_first_fit(size_t size)
{
    std::ostringstream oss;
    oss << "Allocate with first fit init in " << get_typename() << " for " << size << " bytes size" << '\n';
    log_with_guard(oss.str(), logger::severity::trace);

    auto first_free_block = get_first_free_block();
    auto current = reinterpret_cast<unsigned char*>(first_free_block);
    if (current != nullptr)
    {
        do
        {
            auto requested_full_size = CAPTURED_BLOCK_META_SIZE + size;
            auto requested_degree = nearest_power_of_two(requested_full_size);
            if (block_get_degree(current) < requested_degree)
            {
                current = free_block_get_next(current);
                continue;
            }

            oss.str("");
            oss << "Allocate with first fit finish in " << get_typename() << " for " << size << " bytes size" << '\n';
            log_with_guard(oss.str(), logger::severity::debug);

            return allocate_block(current, requested_degree);
        } while (current != nullptr);
    }

    oss.str("");
    oss << "Can`t found free space to allocate " << size << " bytes" << '\n';
    throw std::runtime_error(oss.str());
}

[[nodiscard]] unsigned char* allocator_buddies_system::allocate_best_fit(size_t size)
{
    std::ostringstream oss;
    oss << "Allocate with best fit init in " << get_typename() << " for " << size << " bytes size" << '\n';
    log_with_guard(oss.str(), logger::severity::trace);

    auto first_free_block = get_first_free_block();
    auto current = reinterpret_cast<unsigned char*>(first_free_block);
    if (current != nullptr)
    {
        unsigned char* best = nullptr;
        auto requested_full_size = CAPTURED_BLOCK_META_SIZE + size;
        auto requested_degree = nearest_power_of_two(requested_full_size);

        do
        {
            if (block_get_degree(current) >= requested_degree)
            {
                if (best == nullptr)
                {
                    best = current;
                }
                else if (block_get_degree(best) > block_get_degree(current))
                {
                    best = current;
                }
            }

            current = free_block_get_next(current);
        } while (current != nullptr);

        oss.str("");
        oss << "Allocate with best fit finish in " << get_typename() << " for " << size << " bytes size" << '\n';
        log_with_guard(oss.str(), logger::severity::trace);

        return allocate_block(best, requested_degree);
    }
}

[[nodiscard]] unsigned char* allocator_buddies_system::allocate_worst_fit(size_t size)
{
    std::ostringstream oss;
    oss << "Allocate with worst fit init in " << get_typename() << " for " << size << " bytes size" << '\n';
    log_with_guard(oss.str(), logger::severity::trace);

    auto first_free_block = get_first_free_block();
    auto current = reinterpret_cast<unsigned char*>(first_free_block);
    if (current != nullptr)
    {
        unsigned char* worst = nullptr;
        auto requested_full_size = CAPTURED_BLOCK_META_SIZE + size;
        auto requested_degree = nearest_power_of_two(requested_full_size);

        do
        {
            if (block_get_degree(current) >= requested_degree)
            {
                if (worst == nullptr)
                {
                    worst = current;
                }
                else if (block_get_degree(worst) < block_get_degree(current))
                {
                    worst = current;
                }
            }

            current = free_block_get_next(current);
        } while (current != nullptr);

        oss.str("");
        oss << "Allocate with worst fit finish in " << get_typename() << " for " << size << " bytes size" << '\n';
        log_with_guard(oss.str(), logger::severity::trace);

        return allocate_block(worst, requested_degree);
    }

    oss.str("");
    oss << "Can`t found free space to allocate " << size << " bytes" << '\n';
    throw std::runtime_error(oss.str());
}

void allocator_buddies_system::free_memory()
{
    if (_trusted_memory != nullptr)
    {
        get_mutex()->~mutex();
        deallocate_with_guard(_trusted_memory);
        _trusted_memory = nullptr;
    }
}

#pragma endregion

#pragma region get - set block

unsigned long long allocator_buddies_system::global_to_local(unsigned char* at_char) const
{
    auto temp = reinterpret_cast<unsigned long long>(at_char);
    auto trusted_char = reinterpret_cast<unsigned char*>(_trusted_memory);
    temp -= reinterpret_cast<unsigned long long>(trusted_char + ALLOCATOR_META_SIZE);
    return temp;
}

unsigned char* allocator_buddies_system::local_to_global(unsigned long long at_char) const
{
    auto temp = at_char;
    auto trusted_char = reinterpret_cast<unsigned char*>(_trusted_memory);
    temp += reinterpret_cast<unsigned long long>(trusted_char + ALLOCATOR_META_SIZE);
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

size_t allocator_buddies_system::block_get_size(unsigned char* at_char) const
{
    auto degree = block_get_degree(at_char);
    auto is_free = block_is_free(at_char);
    auto size = (1ull << degree) - (is_free ? FREE_BLOCK_META_SIZE : CAPTURED_BLOCK_META_SIZE);
    return size;
}

unsigned char allocator_buddies_system::block_get_degree(unsigned char* at_char) const
{
    auto current = at_char + BLOCK_DEGREE_BYTES_SHIFT;
    return *reinterpret_cast<unsigned char*>(current);
}

void allocator_buddies_system::block_set_degree(unsigned char* at_char, unsigned char degree)
{
    auto current = at_char + BLOCK_DEGREE_BYTES_SHIFT;
    *reinterpret_cast<unsigned char*>(current) = degree;
}

unsigned char* allocator_buddies_system::block_get_buddie(unsigned char* at_char, unsigned char degree) const
{
    auto at_char_local = global_to_local(at_char);
    auto block_size = 1ull << degree;
    auto buddie_local = at_char_local ^ block_size;
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

bool allocator_buddies_system::try_free_block_split(unsigned char* at_char)
{
    auto degree = block_get_degree(at_char);
    auto size = block_get_size(at_char);
    auto next = free_block_get_next(at_char);
    if (size >= FREE_BLOCK_META_SIZE)
    {
        auto buddie = block_get_buddie(at_char, degree - 1);
        block_set_degree(at_char, degree - 1);
        block_set_degree(buddie, degree - 1);
        free_block_set_next(at_char, buddie);
        free_block_set_prev(buddie, at_char);
        free_block_set_next(buddie, next);
        return true;
    }

    return false;
}

void allocator_buddies_system::block_merge_to_free(unsigned char* at_char, unsigned char* prev, unsigned char* next)
{
    auto buddie = block_get_buddie(at_char);
    auto current = buddie < at_char ? buddie : at_char;
    auto degree = block_get_degree(current);
    block_set_degree(current, degree + 1);
    free_block_set_prev(current, prev);
    free_block_set_next(current, next);
}

#pragma endregion

#pragma region get - set main

inline size_t allocator_buddies_system::get_space_size() const
{
    auto degree = get_space_degree();
    size_t space_size = 1ull << degree;
    return space_size;
}

inline unsigned char allocator_buddies_system::get_space_degree() const
{
    auto memory = reinterpret_cast<unsigned char*>(_trusted_memory) + BLOCK_DEGREE_BYTES_SHIFT;
    auto size_value = *(reinterpret_cast<unsigned char*>(memory));
    return size_value;
}

inline void allocator_buddies_system::set_space_degree(unsigned char size_value)
{
    auto memory = reinterpret_cast<unsigned char*>(_trusted_memory) + BLOCK_DEGREE_BYTES_SHIFT;
    *(reinterpret_cast<unsigned char*>(memory)) = size_value;
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
    auto trusted_memory_char_end = trusted_memory_char + ALLOCATOR_META_SIZE + get_space_size();

    if (current != nullptr)
    {
        do
        {
            auto info = block_info();
            info.is_block_occupied = !block_is_free(current);
            if (info.is_block_occupied)
            {
                info.block_size = block_get_size(current) + CAPTURED_BLOCK_META_SIZE;
                current += info.block_size;
            }
            else
            {
                info.block_size = block_get_size(current) + FREE_BLOCK_META_SIZE;
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

    return static_cast<unsigned int>(std::ceil(std::log2(N)));
}

#pragma endregion