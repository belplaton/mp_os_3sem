#include <not_implemented.h>

#include "../include/allocator_boundary_tags.h"
#include <sstream>

#pragma region Constructors, Operators

allocator_boundary_tags::~allocator_boundary_tags()
{
    std::ostringstream oss;
    oss << "Destructor called for object of type: " << get_typename() << '\n';
    log_with_guard(oss.str(), logger::severity::trace);

    oss.str("");
    oss << "Destructor finished for object of type: " << get_typename() << '\n';
    log_with_guard(oss.str(), logger::severity::trace);

    free_memory();
    //throw not_implemented("allocator_boundary_tags::~allocator_boundary_tags()", "your code should be here...");
}

allocator_boundary_tags::allocator_boundary_tags(
    allocator_boundary_tags &&other) noexcept :
    _trusted_memory(other._trusted_memory)
{
    std::ostringstream oss;
    oss << "Move-constructor called for object of type: " << get_typename() << '\n';
    log_with_guard(oss.str(), logger::severity::trace);

    oss.str("");
    oss << "Move-constructor finished for object of type: " << get_typename() << '\n';
    log_with_guard(oss.str(), logger::severity::trace);
    //throw not_implemented("allocator_boundary_tags::allocator_boundary_tags(allocator_boundary_tags &&) noexcept", "your code should be here...");
}

allocator_boundary_tags &allocator_boundary_tags::operator=(
    allocator_boundary_tags &&other) noexcept
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
    //throw not_implemented("allocator_boundary_tags &allocator_boundary_tags::operator=(allocator_boundary_tags &&) noexcept", "your code should be here...");
}

allocator_boundary_tags::allocator_boundary_tags(
    size_t space_size,
    allocator *parent_allocator,
    logger *logger,
    allocator_with_fit_mode::fit_mode allocate_fit_mode) :
    _trusted_memory(nullptr)
{
    std::ostringstream oss;
    oss << "Constructor called for object of type: " << get_typename() << '\n';
    if (logger != nullptr)
        logger->log(oss.str(), logger::severity::trace);

    space_size += FREE_BLOCK_META_SIZE;
    try
    {
        size_t full_size = space_size + ALLOCATOR_META_SIZE;
        _trusted_memory = parent_allocator == nullptr
            ? ::operator new(full_size)
            : parent_allocator->allocate(full_size, 1);
        std::cout << _trusted_memory << std::endl;
        std::cout << reinterpret_cast<void*>(reinterpret_cast<unsigned char*>(_trusted_memory) + full_size) << std::endl;
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

    set_space_size(space_size);
    set_allocator(parent_allocator);
    set_logger(logger);
    set_fit_mode(allocate_fit_mode);
    init_mutex();

    auto block_size = space_size - FREE_BLOCK_META_SIZE;
    auto block_char_start = reinterpret_cast<unsigned char*>(_trusted_memory) + ALLOCATOR_META_SIZE;
    auto block_char_end = block_char_start + space_size - 1;
    free_block_set_size(block_char_start, block_size, true);
    free_block_set_size(block_char_end, block_size, false);
    free_block_set_prev(block_char_start, nullptr, true);
    free_block_set_next(block_char_end, nullptr, false);
    set_first_free_block(block_char_start);

    oss.str("");
    oss << "Constructor finished for object of type: " << get_typename() << '\n';
    if (logger != nullptr)
        logger->log(oss.str(), logger::severity::trace);
    //throw not_implemented("allocator_boundary_tags::allocator_boundary_tags(size_t, allocator *, logger *, allocator_with_fit_mode::fit_mode)", "your code should be here...");
}

#pragma endregion

#pragma region Memory blocks

unsigned char* allocator_boundary_tags::block_get_ptr(unsigned char* at_char, bool is_start) const
{
    auto current = at_char;
    if (is_start)
    {
        current += BLOCK_START_PTR_BYTES_SHIFT;
    }
    else
    {
        current -= BLOCK_END_PTR_BYTES_SHIFT;
    }

    return *reinterpret_cast<unsigned char**>(current);
}

void allocator_boundary_tags::block_set_ptr(unsigned char* at_char, unsigned char* ptr, bool is_start)
{
    auto current = at_char;
    if (is_start)
    {
        *reinterpret_cast<unsigned char**>(current + BLOCK_START_PTR_BYTES_SHIFT) = ptr;
    }
    else
    {
        *reinterpret_cast<unsigned char**>(current - BLOCK_END_PTR_BYTES_SHIFT) = ptr;
    }
}

unsigned char* allocator_boundary_tags::free_block_get_prev(unsigned char* at_char, bool is_start) const
{
    if (!is_start)
    {
        at_char = free_block_get_start(at_char);
    }

    return *reinterpret_cast<unsigned char**>(at_char + BLOCK_START_PTR_BYTES_SHIFT);
}

unsigned char* allocator_boundary_tags::free_block_get_next(unsigned char* at_char, bool is_start) const
{
    if (is_start)
    {
        at_char = free_block_get_end(at_char);
    }

    return *reinterpret_cast<unsigned char**>(at_char - BLOCK_END_PTR_BYTES_SHIFT);
}

void allocator_boundary_tags::free_block_set_prev(unsigned char* at_char, unsigned char* prev, bool is_start)
{
    if (!is_start)
    {
        at_char = free_block_get_start(at_char);
    }

    *reinterpret_cast<unsigned char**>(at_char + BLOCK_START_PTR_BYTES_SHIFT) = prev;
}

void allocator_boundary_tags::free_block_set_next(unsigned char* at_char, unsigned char* next, bool is_start)
{
    if (is_start)
    {
        at_char = free_block_get_end(at_char);
    }

    *reinterpret_cast<unsigned char**>(at_char - BLOCK_END_PTR_BYTES_SHIFT) = next;
}

size_t allocator_boundary_tags::free_block_get_size(unsigned char* at_char, bool is_start) const
{
    auto current = at_char;
    if (is_start)
    {
        current += BLOCK_START_SIZE_BYTES_SHIFT;
    }
    else
    {
        current -= FREE_BLOCK_END_SIZE_BYTES_SHIFT;
    }

    return *reinterpret_cast<size_t*>(current);
}

void allocator_boundary_tags::free_block_set_size(unsigned char* at_char, size_t size, bool is_start)
{
    if (is_start)
    {
        *reinterpret_cast<size_t*>(at_char + BLOCK_START_SIZE_BYTES_SHIFT) = size;
    }
    else
    {
        *reinterpret_cast<size_t*>(at_char - FREE_BLOCK_END_SIZE_BYTES_SHIFT) = size;
    }
}

size_t allocator_boundary_tags::captured_block_get_size(unsigned char* at_start_char) const
{
    return *reinterpret_cast<size_t*>(at_start_char + BLOCK_START_SIZE_BYTES_SHIFT);
}

void allocator_boundary_tags::captured_block_set_size(unsigned char* at_start_char, size_t size)
{
    *reinterpret_cast<size_t*>(at_start_char + BLOCK_START_SIZE_BYTES_SHIFT) = size;
}

unsigned char* allocator_boundary_tags::captured_block_get_end(unsigned char* at_start_char) const
{
    auto size = captured_block_get_size(at_start_char);
    return at_start_char + size + CAPTURED_BLOCK_META_SIZE - 1;
}

unsigned char* allocator_boundary_tags::free_block_get_end(unsigned char* at_start_char) const
{
    auto size = free_block_get_size(at_start_char, true);
    return at_start_char + size + FREE_BLOCK_META_SIZE - 1;
}

unsigned char* allocator_boundary_tags::free_block_get_start(unsigned char* at_end_char) const
{
    auto size = free_block_get_size(at_end_char, false);
    return at_end_char - size - FREE_BLOCK_META_SIZE + 1;
}

bool allocator_boundary_tags::block_is_free(unsigned char* at_char_start) const
{
    return *reinterpret_cast<unsigned char**>(at_char_start + BLOCK_START_PTR_BYTES_SHIFT) != reinterpret_cast<unsigned char*>(_trusted_memory);
}

[[nodiscard]] unsigned char* allocator_boundary_tags::allocate_block(unsigned char* prev, unsigned char* current, size_t size)
{
    if (current == nullptr)
    {
        throw std::runtime_error("Can`t allocate memory in null block pointer");
    }

    auto total_size = free_block_get_size(current, true);
    auto next = free_block_get_next(current, true);
    if (free_block_get_size(current, true) >= CAPTURED_BLOCK_META_SIZE + size)
    {
        total_size = size;
        auto temp_next = next;
        auto next_size = free_block_get_size(current, true) - CAPTURED_BLOCK_META_SIZE - size;
        auto next_end = next + FREE_BLOCK_META_SIZE + next_size - 1;
        next = current + CAPTURED_BLOCK_META_SIZE + size;

        block_set_ptr(next, temp_next, true);
        block_set_ptr(next_end, prev, false);
        free_block_set_size(next, next_size, true);
        free_block_set_size(next_end, next_size, false);
    }

    auto trusted_memory_char = reinterpret_cast<unsigned char*>(_trusted_memory);
    block_set_ptr(current, trusted_memory_char, true);
    block_set_ptr(current + CAPTURED_BLOCK_META_SIZE + total_size - 1, trusted_memory_char, false);
    captured_block_set_size(current, total_size);
    if (prev != nullptr)
    {
        free_block_set_next(prev, next, true);
    }
    else
    {
        auto current_void = reinterpret_cast<void*>(current);
        set_first_free_block(current_void);
    }

    return current + BLOCK_START_META_SIZE;
}


#pragma endregion

#pragma region Memory operating

[[nodiscard]] void *allocator_boundary_tags::allocate(
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
    //throw not_implemented("[[nodiscard]] void *allocator_boundary_tags::allocate(size_t, size_t)", "your code should be here...");
}

[[nodiscard]] unsigned char* allocator_boundary_tags::allocate_first_fit(size_t size)
{
    std::ostringstream oss;
    oss << "Allocate with first fit init in " << get_typename() << " for " << size << " bytes size" << '\n';
    log_with_guard(oss.str(), logger::severity::trace);

    auto first_free_block = get_first_free_block();
    auto current = reinterpret_cast<unsigned char*>(first_free_block);
    if (current != nullptr)
    {
        unsigned char* prev = nullptr;
        do
        {
            if (free_block_get_size(current, true) + BLOCKS_META_SIZE_DIFF < size)
            {
                prev = current;
                current = free_block_get_next(current, true);
                continue;
            }

            oss.str("");
            oss << "Allocate with first fit finish in " << get_typename() << " for " << size << " bytes size" << '\n';
            log_with_guard(oss.str(), logger::severity::debug);

            return allocate_block(prev, current, size);
        } while (current != nullptr);
    }

    oss.str("");
    oss << "Can`t found free space to allocate " << size << " bytes" << '\n';
    throw std::runtime_error(oss.str());
}

[[nodiscard]] unsigned char* allocator_boundary_tags::allocate_best_fit(size_t size)
{
    std::ostringstream oss;
    oss << "Allocate with best fit init in " << get_typename() << " for " << size << " bytes size" << '\n';
    log_with_guard(oss.str(), logger::severity::trace);

    auto first_free_block = get_first_free_block();
    auto current = reinterpret_cast<unsigned char*>(first_free_block);
    if (current != nullptr)
    {
        unsigned char* best_prev = nullptr;
        unsigned char* best = nullptr;
        unsigned char* prev = nullptr;
        do
        {
            if (free_block_get_size(current, true) + BLOCKS_META_SIZE_DIFF >= size)
            {
                if (best == nullptr)
                {
                    best = current;
                }
                else if (free_block_get_size(best, true) > free_block_get_size(current, true))
                {
                    best_prev = prev;
                    best = current;
                }
            }

            prev = current;
            current = free_block_get_next(current, true);
        } while (current != nullptr);

        oss.str("");
        oss << "Allocate with best fit finish in " << get_typename() << " for " << size << " bytes size" << '\n';
        log_with_guard(oss.str(), logger::severity::trace);

        return allocate_block(best_prev, best, size);
    }
}

[[nodiscard]] unsigned char* allocator_boundary_tags::allocate_worst_fit(size_t size)
{
    std::ostringstream oss;
    oss << "Allocate with worst fit init in " << get_typename() << " for " << size << " bytes size" << '\n';
    log_with_guard(oss.str(), logger::severity::trace);

    auto first_free_block = get_first_free_block();
    auto current = reinterpret_cast<unsigned char*>(first_free_block);
    if (current != nullptr)
    {
        unsigned char* worst_prev = nullptr;
        unsigned char* worst = nullptr;
        unsigned char* prev = nullptr;
        do
        {
            if (free_block_get_size(current, true) + BLOCKS_META_SIZE_DIFF >= size)
            {
                if (worst == nullptr)
                {
                    worst = current;
                }
                else if (free_block_get_size(worst, true) < free_block_get_size(current, true))
                {
                    worst_prev = prev;
                    worst = current;
                }
            }

            prev = current;
            current = free_block_get_next(current, true);
        } while (current != nullptr);

        oss.str("");
        oss << "Allocate with worst fit finish in " << get_typename() << " for " << size << " bytes size" << '\n';
        log_with_guard(oss.str(), logger::severity::trace);

        return allocate_block(worst_prev, worst, size);
    }

    oss.str("");
    oss << "Can`t found free space to allocate " << size << " bytes" << '\n';
    throw std::runtime_error(oss.str());
}

void allocator_boundary_tags::deallocate(
    void *at)
{
    std::ostringstream oss;
    oss << "Deallocate memory method init in " << get_typename() << " at " << at << '\n';
    log_with_guard(oss.str(), logger::severity::debug);

    if (!at)
    {
        oss.str("");
        oss << "Attempting to free a null pointer in " << get_typename() << '\n';
        log_with_guard(oss.str(), logger::severity::warning);

        oss.str("");
        oss << "Deallocate memory method finish in " << get_typename() << " at " << at << '\n';
        log_with_guard(oss.str(), logger::severity::debug);
        return;
    }

    auto mutex = get_mutex();
    mutex->lock();

    auto at_char_start = reinterpret_cast<unsigned char*>(at) - BLOCK_START_META_SIZE;
    auto trusted_memory_char = reinterpret_cast<unsigned char*>(_trusted_memory);
    if (block_get_ptr(at_char_start, true) != trusted_memory_char)
    {
        std::ostringstream oss;
        oss << "Can`t deallocate memory out of allocator working space." << '\n';
        mutex->unlock();
        throw std::runtime_error(oss.str());
    }

    oss.str("");
    oss << "Memory block state before cleaning: " << at << '\n';
    log_with_guard(oss.str(), logger::severity::debug);

    auto memory_space_char_start = reinterpret_cast<unsigned char*>(_trusted_memory) + ALLOCATOR_META_SIZE;
    auto memory_space_char_end = memory_space_char_start + get_space_size() - 1;

    auto at_char_end = captured_block_get_end(at_char_start);

    if (at_char_end + 1 <= memory_space_char_end && block_get_ptr(at_char_end + 1, true) != trusted_memory_char)
    {
        auto right_start = at_char_end + 1;
        auto right_end = free_block_get_end(right_start);
        auto right_prev = free_block_get_prev(right_start, true);
        auto new_size = free_block_get_size(right_start, true) + captured_block_get_size(at_char_start) + CAPTURED_BLOCK_META_SIZE;

        free_block_set_size(at_char_start, new_size, true);
        free_block_set_size(right_end, new_size, false);
        free_block_set_prev(at_char_start, right_prev, true);
        if (right_prev != nullptr)
        {
            auto right_prev_end = free_block_get_end(right_prev);
            free_block_set_next(right_prev, at_char_start, true);
            if (right_prev_end == at_char_start - 1)
            {
                new_size += free_block_get_size(right_prev, true) + FREE_BLOCK_META_SIZE;
                free_block_set_size(right_prev, new_size, true);
                free_block_set_size(right_end, new_size, false);
            }
        }
        else
        {
            set_first_free_block(at_char_start);
        }
    }
    else if (at_char_start - 1 >= memory_space_char_start && block_get_ptr(at_char_start - 1, false) != trusted_memory_char)
    {
        auto left_end = at_char_start - 1;
        auto left_start = free_block_get_start(left_end);
        auto left_next = free_block_get_next(left_start, true);
        auto new_size = free_block_get_size(left_start, true) + captured_block_get_size(at_char_start) + CAPTURED_BLOCK_META_SIZE;

        free_block_set_size(left_start, new_size, true);
        free_block_set_size(left_end, new_size, false);
        free_block_set_next(at_char_end, left_next, false);
        if (left_next != nullptr)
        {
            auto left_next_end = free_block_get_end(left_next);
            if (left_next = at_char_end + 1)
            {
                new_size += free_block_get_size(left_next, true) + FREE_BLOCK_META_SIZE;
                free_block_set_size(left_next, new_size, true);
                free_block_set_size(left_next_end, new_size, false);
            }
        }
    }
    else
    {
        auto current = reinterpret_cast<unsigned char*>(get_first_free_block());
        unsigned char* prev = nullptr;

        if (current != nullptr)
        {
            do
            {
                if (current > at_char_start)
                {
                    auto at_char_is_free = false;
                    auto current_end = free_block_get_end(current);
                    auto temp_size = captured_block_get_size(at_char_start);
                    auto temp_end = at_char_end;
                    free_block_set_prev(at_char_start, prev, true);
                    free_block_set_next(at_char_end, current, false);
                    if (at_char_end + 1 == current)
                    {
                        at_char_is_free = true;
                        temp_size += free_block_get_size(current, true) + CAPTURED_BLOCK_META_SIZE;
                        free_block_set_size(at_char_start, temp_size, true);
                        free_block_set_size(current_end, temp_size, false);
                        temp_end = current_end;
                    }

                    if (prev != nullptr && free_block_get_end(prev) + 1 == at_char_start)
                    {
                        temp_size += free_block_get_size(prev, true) + CAPTURED_BLOCK_META_SIZE;
                        if (at_char_is_free)
                        {
                            temp_size += BLOCKS_META_SIZE_DIFF;
                        }

                        free_block_set_size(prev, temp_size, true);
                        free_block_set_size(temp_end, temp_size, false);
                    }

                    break;
                }

                prev = current;
                current = free_block_get_next(current, true);
            } while (current != nullptr);
        }
        else
        {
            (at_char_start, nullptr, true);
            set_first_free_block(at_char_start);
        }
    }

    mutex->unlock();
    oss.str("");
    oss << "Deallocate memory method finish in " << get_typename() << " at " << at << '\n';
    log_with_guard(oss.str(), logger::severity::debug);

    //throw not_implemented("void allocator_boundary_tags::deallocate(void *)", "your code should be here...");
}

void allocator_boundary_tags::free_memory()
{
    if (_trusted_memory != nullptr)
    {
        get_mutex()->~mutex();
        deallocate_with_guard(_trusted_memory);
        _trusted_memory = nullptr;
    }
}

#pragma endregion

#pragma region get - set

inline size_t allocator_boundary_tags::get_space_size() const
{
    auto memory = reinterpret_cast<unsigned char*>(_trusted_memory) + SPACE_SIZE_BYTES_SHIFT;
    auto size_value = *(reinterpret_cast<size_t*>(memory));
    return size_value;
}

inline void allocator_boundary_tags::set_space_size(size_t size_value)
{
    auto memory = reinterpret_cast<unsigned char*>(_trusted_memory) + SPACE_SIZE_BYTES_SHIFT;
    *(reinterpret_cast<size_t*>(memory)) = size_value;
}

inline allocator_with_fit_mode::fit_mode allocator_boundary_tags::get_fit_mode() const
{
    auto memory = reinterpret_cast<unsigned char*>(_trusted_memory) + FITMODE_BYTES_SHIFT;
    auto fitmode_value = *(reinterpret_cast<allocator_with_fit_mode::fit_mode*>(memory));
    return fitmode_value;
}

inline void allocator_boundary_tags::set_fit_mode(
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

inline allocator *allocator_boundary_tags::get_allocator() const
{
    auto memory = reinterpret_cast<unsigned char*>(_trusted_memory) + ALLOCATOR_BYTES_SHIFT;
    auto allocator_value = *reinterpret_cast<allocator**>(memory);
    return allocator_value;
    //throw not_implemented("inline allocator *allocator_boundary_tags::get_allocator() const", "your code should be here...");
}

inline void allocator_boundary_tags::set_allocator(allocator* allocator_value)
{
    auto memory = reinterpret_cast<unsigned char*>(_trusted_memory) + ALLOCATOR_BYTES_SHIFT;
    *(reinterpret_cast<allocator**>(memory)) = allocator_value;
}

std::vector<allocator_test_utils::block_info> allocator_boundary_tags::get_blocks_info() const noexcept
{
    std::vector<allocator_test_utils::block_info> result{};

    auto current = reinterpret_cast<unsigned char*>(_trusted_memory) + ALLOCATOR_META_SIZE;
    auto trusted_memory_char = reinterpret_cast<unsigned char*>(_trusted_memory);
    auto trusted_memory_char_end = trusted_memory_char + ALLOCATOR_META_SIZE + get_space_size() - 1;

    if (current != nullptr)
    {
        do
        {
            auto info = block_info();
            info.is_block_occupied = !block_is_free(current);
            if (info.is_block_occupied)
            {
                info.block_size = captured_block_get_size(current);
                current += info.block_size + CAPTURED_BLOCK_META_SIZE;
            }
            else
            {
                info.block_size = free_block_get_size(current, true);
                current += info.block_size + FREE_BLOCK_META_SIZE;
            }

            result.push_back(info);
        } while (current != nullptr && current <= trusted_memory_char_end);
    }

    return result;
    //throw not_implemented("std::vector<allocator_test_utils::block_info> allocator_boundary_tags::get_blocks_info() const noexcept", "your code should be here...");
}

inline logger *allocator_boundary_tags::get_logger() const
{
    auto memory = reinterpret_cast<unsigned char*>(_trusted_memory) + LOGGER_BYTES_SHIFT;
    auto logger_value = *(reinterpret_cast<logger**>(memory));
    return logger_value;
    //throw not_implemented("inline logger *allocator_boundary_tags::get_logger() const", "your code should be here...");
}

inline void allocator_boundary_tags::set_logger(logger* logger_value)
{
    auto memory = reinterpret_cast<unsigned char*>(_trusted_memory) + LOGGER_BYTES_SHIFT;
    *(reinterpret_cast<logger**>(memory)) = logger_value;
}

inline std::mutex* allocator_boundary_tags::get_mutex() const
{
    auto memory = reinterpret_cast<unsigned char*>(_trusted_memory) + MUTEX_BYTES_SHIFT;
    auto mutex_value = reinterpret_cast<std::mutex*>(memory);
    return mutex_value;
}

inline void allocator_boundary_tags::init_mutex()
{
    auto memory = reinterpret_cast<unsigned char*>(_trusted_memory) + MUTEX_BYTES_SHIFT;
    new (reinterpret_cast<std::mutex*>(memory)) std::mutex();
}

inline void* allocator_boundary_tags::get_first_free_block() const
{
    auto memory = reinterpret_cast<unsigned char*>(_trusted_memory) + FIRST_FREE_BLOCK_BYTES_SHIFT;
    auto free_block_ptr = *(reinterpret_cast<void**>(memory));
    return free_block_ptr;
}

inline void allocator_boundary_tags::set_first_free_block(void* free_block_ptr)
{
    auto memory = reinterpret_cast<unsigned char*>(_trusted_memory) + FIRST_FREE_BLOCK_BYTES_SHIFT;
    *(reinterpret_cast<void**>(memory)) = free_block_ptr;
}

inline std::string allocator_boundary_tags::get_typename() const noexcept
{
    return ("allocator_boundary_tags");
    //throw not_implemented("inline std::string allocator_boundary_tags::get_typename() const noexcept", "your code should be here...");
}
#pragma endregion

