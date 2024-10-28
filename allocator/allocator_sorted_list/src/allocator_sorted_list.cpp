#include <not_implemented.h>

#include "../include/allocator_sorted_list.h"
#include <sstream>

#pragma region Constructors, Operators

allocator_sorted_list::~allocator_sorted_list()
{
    std::ostringstream oss;
    oss << "Destructor called for object of type: " << get_typename() << '\n';
    log_with_guard(oss.str(), logger::severity::trace);

    oss.str("");
    oss << "Destructor finished for object of type: " << get_typename() << '\n';
    log_with_guard(oss.str(), logger::severity::trace);

    free_memory();
    //throw not_implemented("allocator_sorted_list::~allocator_sorted_list()", "your code should be here...");
}

allocator_sorted_list::allocator_sorted_list(
    allocator_sorted_list&& other) noexcept :
    _trusted_memory(other._trusted_memory)
{
    std::ostringstream oss;
    oss << "Move-constructor called for object of type: " << get_typename() << '\n';
    log_with_guard(oss.str(), logger::severity::trace);

    oss.str("");
    oss << "Move-constructor finished for object of type: " << get_typename() << '\n';
    log_with_guard(oss.str(), logger::severity::trace);
    //throw not_implemented("allocator_sorted_list::allocator_sorted_list(allocator_sorted_list &&) noexcept", "your code should be here...");
}

allocator_sorted_list& allocator_sorted_list::operator=(
    allocator_sorted_list&& other) noexcept
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
    //throw not_implemented("allocator_sorted_list &allocator_sorted_list::operator=(allocator_sorted_list &&) noexcept", "your code should be here...");
}

allocator_sorted_list::allocator_sorted_list(
    size_t space_size,
    allocator* parent_allocator,
    logger* logger,
    allocator_with_fit_mode::fit_mode allocate_fit_mode) :
    _trusted_memory(nullptr)
{
    std::ostringstream oss;
    oss << "Constructor called for object of type: " << get_typename() << '\n';
    if (logger != nullptr)
        logger->log(oss.str(), logger::severity::trace);
    
    space_size += BLOCK_META_SIZE;
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
            logger->log(error.what(), logger::severity::trace);

        throw error;
    }

    set_space_size(space_size);
    set_allocator(parent_allocator);
    set_logger(logger);
    set_fit_mode(allocate_fit_mode);
    init_mutex();

    auto first_block_char = reinterpret_cast<unsigned char*>(_trusted_memory) + ALLOCATOR_META_SIZE;
    auto free_block = block_construct(first_block_char, nullptr, space_size - BLOCK_META_SIZE);
    set_first_free_block(free_block);

    oss.str("");
    oss << "Constructor finished for object of type: " << get_typename() << '\n';
    if (logger != nullptr)
        logger->log(oss.str(), logger::severity::trace);
    //throw not_implemented("allocator_sorted_list::allocator_sorted_list(size_t, allocator *, logger *, allocator_with_fit_mode::fit_mode)", "your code should be here...");
}

#pragma endregion

#pragma region Memory operating

[[nodiscard]] void* allocator_sorted_list::allocate(
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
    //throw not_implemented("[[nodiscard]] void *allocator_sorted_list::allocate(size_t, size_t)", "your code should be here...");
}

unsigned char* allocator_sorted_list::block_construct(unsigned char* block_memory, unsigned char* associated_ptr, size_t block_size)
{
    block_set_associated_ptr(block_memory, associated_ptr);
    block_set_size(block_memory, block_size);

    return block_memory;
}

unsigned char* allocator_sorted_list::block_get_associated_ptr(unsigned char* block_memory) const
{
    auto memory = block_memory + BLOCK_PTR_BYTES_SHIFT;
    auto associated_ptr = *(reinterpret_cast<unsigned char**>(memory));
    return associated_ptr;
}

unsigned char* allocator_sorted_list::block_get_next_block(unsigned char* block_memory) const
{
    auto trusted_memory_char = reinterpret_cast<unsigned char*>(_trusted_memory);
    auto trusted_memory_char_end = trusted_memory_char + ALLOCATOR_META_SIZE + get_space_size() - 1;
    auto memory = block_memory + BLOCK_META_SIZE + block_get_size(block_memory);
    if (memory >= trusted_memory_char_end)
    {
        memory = nullptr;
    }

    return memory;
}

size_t allocator_sorted_list::block_get_size(unsigned char* block_memory) const
{
    auto memory = block_memory + BLOCK_SIZE_BYTES_SHIFT;
    auto block_size = *(reinterpret_cast<size_t*>(memory));
    return block_size;
}

void allocator_sorted_list::block_set_associated_ptr(unsigned char* block_memory, unsigned char* associated_ptr)
{
    auto memory = block_memory + BLOCK_PTR_BYTES_SHIFT;
    *(reinterpret_cast<unsigned char**>(memory)) = associated_ptr;
}

void allocator_sorted_list::block_set_size(unsigned char* block_memory, size_t block_size)
{
    auto memory = block_memory + BLOCK_SIZE_BYTES_SHIFT;
    *(reinterpret_cast<size_t*>(memory)) = block_size;
}

[[nodiscard]] unsigned char* allocator_sorted_list::allocate_block(unsigned char* prev, unsigned char* current, size_t size)
{
    if (current == nullptr)
    {
        throw std::runtime_error("Can`t allocate memory in null block pointer");
    }

    auto total_size = block_get_size(current);
    auto next = block_get_associated_ptr(current);
    if (block_get_size(current) >= BLOCK_META_SIZE + size)
    {
        total_size = size;
        next = block_construct(current + BLOCK_META_SIZE + size, block_get_associated_ptr(current), block_get_size(current) - BLOCK_META_SIZE - size);
    }

    auto trusted_memory_char = reinterpret_cast<unsigned char*>(_trusted_memory);
    block_construct(current, trusted_memory_char, total_size);
    if (prev != nullptr)
    {
        block_set_associated_ptr(prev, next);
    }
    else
    {
        auto next_void = reinterpret_cast<void*>(next);
        set_first_free_block(next_void);
    }

    return current + BLOCK_META_SIZE;
}

[[nodiscard]] unsigned char* allocator_sorted_list::allocate_first_fit(size_t size)
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
            if (block_get_size(current) < size)
            {
                prev = current;
                current = block_get_associated_ptr(current);
                continue;
            }

            oss.str("");
            oss << "Allocate with first fit finish in " << get_typename() << " for " << size << " bytes size" << '\n';
            log_with_guard(oss.str(), logger::severity::trace);

            return allocate_block(prev, current, size);
        } while (current != nullptr);
    }

    oss.str("");
    oss << "Can`t found free space to allocate " << size << " bytes" << '\n';
    throw std::runtime_error(oss.str());
}

[[nodiscard]] unsigned char* allocator_sorted_list::allocate_best_fit(size_t size)
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
            if (block_get_size(current) >= size)
            {
                if (best == nullptr)
                {
                    best = current;
                }
                else if (block_get_size(best) > block_get_size(current))
                {
                    best_prev = prev;
                    best = current;
                }
            }

            prev = current;
            current = block_get_associated_ptr(current);
        } while (current != nullptr);

        oss.str("");
        oss << "Allocate with best fit finish in " << get_typename() << " for " << size << " bytes size" << '\n';
        log_with_guard(oss.str(), logger::severity::trace);

        return allocate_block(best_prev, best, size);
    }

    oss.str("");
    oss << "Can`t found free space to allocate " << size << " bytes" << '\n';
    throw std::runtime_error(oss.str());    
}

[[nodiscard]] unsigned char* allocator_sorted_list::allocate_worst_fit(size_t size)
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
            if (block_get_size(current) >= size)
            {
                if (worst == nullptr)
                {
                    worst = current;
                }
                else if (block_get_size(worst) < block_get_size(current))
                {
                    worst_prev = prev;
                    worst = current;
                }
            }

            prev = current;
            current = block_get_associated_ptr(current);;
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

void allocator_sorted_list::deallocate(
    void* at)
{
    try
    {
        auto mutex = get_mutex();
        std::lock_guard<std::mutex> lock(*mutex);

        std::ostringstream oss;
        oss << "Deallocate memory method init in " << get_typename() << " at " << at << '\n';
        log_with_guard(oss.str(), logger::severity::debug);

        if (!at)
        {
            oss.str("");
            oss << "Attempting to free a null pointer in " << get_typename() << '\n';
            throw std::runtime_error(oss.str());
        }

        auto at_char = reinterpret_cast<unsigned char*>(at) - BLOCK_META_SIZE;
        auto trusted_memory_char = reinterpret_cast<unsigned char*>(_trusted_memory);
        if (block_get_associated_ptr(at_char) != trusted_memory_char)
        {
            oss.str("");
            oss << "Can`t deallocate memory out of allocator working space." << '\n';
            throw std::runtime_error(oss.str());
        }

        auto current = reinterpret_cast<unsigned char*>(get_first_free_block());
        unsigned char* prev = nullptr;
        if (current != nullptr)
        {
            do
            {
                if (current > at_char)
                {
                    block_set_associated_ptr(at_char, current);
                    if (block_get_next_block(at_char) == current)
                    {
                        auto temp_size = block_get_size(at_char) + block_get_size(current) + BLOCK_META_SIZE;
                        block_construct(at_char, block_get_associated_ptr(current), temp_size);
                    }

                    if (prev != nullptr)
                    {
                        block_set_associated_ptr(prev, at_char);
                        if (block_get_next_block(prev) == at_char)
                        {
                            auto temp_size = block_get_size(prev) + block_get_size(at_char) + BLOCK_META_SIZE;
                            block_construct(prev, block_get_associated_ptr(at_char), temp_size);
                        }
                    }
                    else
                    {
                        auto at_char_void = reinterpret_cast<void*>(at_char);
                        set_first_free_block(at_char_void);
                    }

                    break;
                }

                prev = current;
                current = block_get_associated_ptr(current);
            } while (current != nullptr);
        }
        else
        {
            block_set_associated_ptr(at_char, nullptr);
            set_first_free_block(at_char);
        }

        oss.str("");
        oss << "Memory after deallocate: " << get_blocks_info_str() << "\n";
        log_with_guard(oss.str(), logger::severity::debug);

        oss.str("");
        oss << "Free memory left after deallocate: " << get_blocks_info_str(block_info_type::avail) << "\n";
        log_with_guard(oss.str(), logger::severity::information);

        oss.str("");
        oss << "Deallocate memory method finish in " << get_typename() << '\n';
        log_with_guard(oss.str(), logger::severity::debug);
    }
    catch (const std::exception& error)
    {
        log_with_guard(error.what(), logger::severity::error);
        throw std::bad_alloc();
    }

    //throw not_implemented("void allocator_sorted_list::deallocate(void *)", "your code should be here...\n");
}

void allocator_sorted_list::free_memory()
{
    get_mutex()->~mutex();
    deallocate_with_guard(_trusted_memory);
    _trusted_memory = nullptr;
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
    std::ostringstream oss;
    oss << "Set fit mode method init in " << get_typename() << " with mode: " << (int)mode << '\n';
    log_with_guard(oss.str(), logger::severity::debug);

    auto memory = reinterpret_cast<unsigned char*>(_trusted_memory) + FITMODE_BYTES_SHIFT;
    *(reinterpret_cast<allocator_with_fit_mode::fit_mode*>(memory)) = mode;

    oss.str("");
    oss << "Set fit mode method finish in " << get_typename() << " with mode: " << (int)mode << '\n';
    log_with_guard(oss.str(), logger::severity::debug);
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

    auto current = reinterpret_cast<unsigned char*>(_trusted_memory) + ALLOCATOR_META_SIZE;
    auto trusted_memory_char = reinterpret_cast<unsigned char*>(_trusted_memory);
    auto trusted_memory_char_end = trusted_memory_char + ALLOCATOR_META_SIZE + get_space_size() - 1;

    if (current != nullptr)
    {
        do
        {
            auto info = block_info();
            info.block_size = block_get_size(current) + BLOCK_META_SIZE;
            info.is_block_occupied = block_get_associated_ptr(current) == trusted_memory_char;
            result.push_back(info);
            current = block_get_next_block(current);
        } while (current != nullptr);
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
    auto mutex_value = reinterpret_cast<std::mutex*>(memory);
    return mutex_value;
}

inline void allocator_sorted_list::init_mutex()
{
    auto memory = reinterpret_cast<unsigned char*>(_trusted_memory) + MUTEX_BYTES_SHIFT;
    new (reinterpret_cast<std::mutex*>(memory)) std::mutex();
}

inline void* allocator_sorted_list::get_first_free_block() const
{
    auto memory = reinterpret_cast<unsigned char*>(_trusted_memory) + FIRST_FREE_BLOCK_BYTES_SHIFT;
    auto free_block_ptr = *(reinterpret_cast<void**>(memory));
    return free_block_ptr;
}

inline void allocator_sorted_list::set_first_free_block(void* free_block_ptr)
{
    auto memory = reinterpret_cast<unsigned char*>(_trusted_memory) + FIRST_FREE_BLOCK_BYTES_SHIFT;
    *(reinterpret_cast<void**>(memory)) = free_block_ptr;
}

inline std::string allocator_sorted_list::get_typename() const noexcept
{
    return ("allocator_sorted_list");
    //throw not_implemented("inline std::string allocator_sorted_list::get_typename() const noexcept", "your code should be here...");
}

#pragma endregion