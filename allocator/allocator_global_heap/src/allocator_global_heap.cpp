#include <not_implemented.h>

#include "../include/allocator_global_heap.h"
#include <sstream>

std::map<int, allocator_global_heap*> allocator_global_heap::_allocators_map{};
int allocator_global_heap::_allocators_created{0};

allocator_global_heap::allocator_global_heap(
    logger *logger):
    _logger(logger),
    _id(allocator_global_heap::_allocators_created)
{
    std::ostringstream oss;
    oss << "Constructor called for object of type: " << get_typename() << '\n';
    log_with_guard(oss.str(), logger::severity::trace);

    allocator_global_heap::_allocators_created++;
    allocator_global_heap::_allocators_map.emplace(_id, this);

    oss.str("");
    oss << "Constructor finished for object of type: " << get_typename() << '\n';
    log_with_guard(oss.str(), logger::severity::trace);
    //throw not_implemented("allocator_global_heap::allocator_global_heap(logger *)", "your code should be here...");
}

allocator_global_heap::~allocator_global_heap()
{
    std::ostringstream oss;
    oss << "Destructor called for object of type: " << get_typename() << '\n';
    log_with_guard(oss.str(), logger::severity::trace);

    _logger == nullptr;
    allocator_global_heap::_allocators_map.erase(_id);

    oss.str("");
    oss << "Destructor finished for object of type: " << get_typename() << '\n';
    log_with_guard(oss.str(), logger::severity::trace);
    //throw not_implemented("allocator_global_heap::~allocator_global_heap()", "your code should be here...");
}

allocator_global_heap::allocator_global_heap(
    allocator_global_heap &&other) noexcept:
    _logger(std::move(other._logger)),
    _id(std::move(other._id))
{
    std::ostringstream oss;
    oss << "Move-destructor called for object of type: " << get_typename() << '\n';
    log_with_guard(oss.str(), logger::severity::trace);

    other._logger = nullptr;
    allocator_global_heap::_allocators_map.erase(_id);
    allocator_global_heap::_allocators_map.emplace(_id, this);

    oss.str("");
    oss << "Move-destructor finished for object of type: " << get_typename() << '\n';
    log_with_guard(oss.str(), logger::severity::trace);
    //throw not_implemented("allocator_global_heap::allocator_global_heap(allocator_global_heap &&) noexcept", "your code should be here...");
}

allocator_global_heap &allocator_global_heap::operator=(
    allocator_global_heap &&other) noexcept
{
    std::ostringstream oss;
    oss << "Move-operator '=' called for object of type: " << get_typename() << '\n';
    log_with_guard(oss.str(), logger::severity::trace);

    if (this != &other)
    {
        _logger = std::move(other._logger);
        _id = std::move(other._id);
        other._logger == nullptr;
        other._id = 0;
    }


    oss.str("");
    oss << "Move-operator '=' finished for object of type: " << get_typename() << '\n';
    log_with_guard(oss.str(), logger::severity::trace);

    return *this;
    //throw not_implemented("allocator_global_heap &allocator_global_heap::operator=(allocator_global_heap &&) noexcept", "your code should be here...");
}

[[nodiscard]] void *allocator_global_heap::allocate(
    size_t value_size,
    size_t values_count)
{
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

    void* allocated = nullptr;
    void* used_memory = nullptr;

    try
    {
        size_t total_size = sizeof(allocation_header) + value_size * values_count;
        allocated = ::operator new(total_size);
        allocation_header* header = static_cast<allocation_header*>(allocated);
        header->allocator_id = _id;
        header->data_size = value_size * values_count;
        used_memory = reinterpret_cast<void*>(header + 1);

        oss.str("");
        oss << "Successfully allocated memory for " << values_count << " elements with " << value_size << " bytes size in " << get_typename() << '\n';
        log_with_guard(oss.str(), logger::severity::information);
    }
    catch (std::bad_alloc const &error)
    {
        oss.str("");
        oss << "Can`t allocate memory in " << get_typename() << " for " << values_count << " elements with " << value_size << " bytes size" << '\n';
        log_with_guard(oss.str(), logger::severity::warning);
    }
    catch (std::exception const& error)
    {
        log_with_guard(error.what(), logger::severity::error);
        ::operator delete(allocated);
        throw error;
    }

    oss.str("");
    oss << "Allocate memory method finish in " << get_typename() << " for " << values_count << " elements with " << value_size << " bytes size" << '\n';
    log_with_guard(oss.str(), logger::severity::debug);

    return used_memory;
    //throw not_implemented("[[nodiscard]] void *allocator_global_heap::allocate(size_t, size_t)", "your code should be here...");
}

void allocator_global_heap::deallocate(
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

    auto header = reinterpret_cast<allocation_header*>(at) - 1;
    auto allocator_temp_iter = allocator_global_heap::_allocators_map.find(header->allocator_id);
    if (allocator_temp_iter != allocator_global_heap::_allocators_map.end())
    {
        if (allocator_temp_iter->second != this)
        {
            oss.str("");
            oss << "Attempting to free a memory not belonging to this allocator in " << get_typename() << '\n';
            log_with_guard(oss.str(), logger::severity::error);

            throw std::logic_error(oss.str());
        }
    }
    else
    {
        oss.str("");
        oss << "Can`t found allocator with " << header->allocator_id << " in " << get_typename() << " static std::map<int, allocator_global_heap*> _allocators_map " << '\n';
        log_with_guard(oss.str(), logger::severity::error);

        throw std::logic_error(oss.str());
    }

    oss.str("");
    oss << "Memory block state before cleaning: " << at << '\n';
    log_with_guard(oss.str(), logger::severity::debug);
    ::operator delete(header);

    oss.str("");
    oss << "Deallocate memory method finish in " << get_typename() << " at " << at << '\n';
    log_with_guard(oss.str(), logger::severity::debug);
    //throw not_implemented("void allocator_global_heap::deallocate(void *)", "your code should be here...");
}

inline logger *allocator_global_heap::get_logger() const
{
    return _logger;
    //throw not_implemented("inline logger *allocator_global_heap::get_logger() const", "your code should be here...");
}

inline std::string allocator_global_heap::get_typename() const noexcept
{
    return "allocator_global_heap";
    //throw not_implemented("inline std::string allocator_global_heap::get_typename() const noexcept", "your code should be here...");
}