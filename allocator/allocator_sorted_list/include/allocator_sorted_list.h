#ifndef MATH_PRACTICE_AND_OPERATING_SYSTEMS_ALLOCATOR_ALLOCATOR_SORTED_LIST_H
#define MATH_PRACTICE_AND_OPERATING_SYSTEMS_ALLOCATOR_ALLOCATOR_SORTED_LIST_H

#include <allocator_guardant.h>
#include <allocator_test_utils.h>
#include <allocator_with_fit_mode.h>
#include <logger_guardant.h>
#include <typename_holder.h>
#include <mutex>

class allocator_sorted_list final:
    private allocator_guardant,
    public allocator_test_utils,
    public allocator_with_fit_mode,
    private logger_guardant,
    private typename_holder
{

private:
    
    void *_trusted_memory;

public:

    struct meta_block_info
    {
        void* assotiated_ptr;
        block_info base_info;
    };

public:
    
    ~allocator_sorted_list() override;
    
    allocator_sorted_list(
        allocator_sorted_list const &other);
    
    allocator_sorted_list &operator=(
        allocator_sorted_list const &other);
    
    allocator_sorted_list(
        allocator_sorted_list &&other) noexcept;
    
    allocator_sorted_list &operator=(
        allocator_sorted_list &&other) noexcept;

public:
    
    explicit allocator_sorted_list(
        size_t space_size,
        allocator *parent_allocator = nullptr,
        logger *logger = nullptr,
        allocator_with_fit_mode::fit_mode allocate_fit_mode = allocator_with_fit_mode::fit_mode::first_fit);

public:
    
    [[nodiscard]] void *allocate(
        size_t value_size,
        size_t values_count) override;

private:

    [[nodiscard]] meta_block_info* allocate_block(meta_block_info* prev, meta_block_info* next, size_t size);

    [[nodiscard]] meta_block_info* allocate_first_fit(size_t size);

    [[nodiscard]] meta_block_info* allocate_best_fit(size_t size);

    [[nodiscard]] meta_block_info* allocate_worst_fit(size_t size);

public:

    void deallocate(
        void *at) override;

private:

    void free_memory();

public:

    inline size_t get_space_size() const;

    inline void set_space_size(size_t);

public:

    inline allocator_with_fit_mode::fit_mode get_fit_mode() const override;

    inline void set_fit_mode(allocator_with_fit_mode::fit_mode mode) override;

private:
    
    inline allocator *get_allocator() const override;

    inline void set_allocator(allocator*);

public:
    
    std::vector<allocator_test_utils::block_info> get_blocks_info() const noexcept override;

private:
    
    inline logger* get_logger() const override;

    inline void set_logger(logger*);

private:

    inline std::mutex* get_mutex() const;

    inline void set_mutex(std::mutex*);

private:

    inline meta_block_info* get_first_free_block() const;

    inline void set_first_free_block(meta_block_info*);

private:
    
    inline std::string get_typename() const noexcept override;

};

#endif //MATH_PRACTICE_AND_OPERATING_SYSTEMS_ALLOCATOR_ALLOCATOR_SORTED_LIST_H