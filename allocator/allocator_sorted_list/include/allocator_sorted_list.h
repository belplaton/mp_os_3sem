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

    static constexpr size_t ALLOCATOR_META_SIZE = 
        (sizeof(size_t) + 
        sizeof(allocator*) +
        sizeof(logger*) +
        sizeof(std::mutex) +
        sizeof(allocator_with_fit_mode::fit_mode) +
        sizeof(void*));

    static constexpr size_t SPACE_SIZE_BYTES_SHIFT = 0;
    static constexpr size_t ALLOCATOR_BYTES_SHIFT = (SPACE_SIZE_BYTES_SHIFT + sizeof(size_t));
    static constexpr size_t LOGGER_BYTES_SHIFT = (ALLOCATOR_BYTES_SHIFT + sizeof(allocator*));
    static constexpr size_t MUTEX_BYTES_SHIFT = (LOGGER_BYTES_SHIFT + sizeof(logger*));
    static constexpr size_t FITMODE_BYTES_SHIFT = (MUTEX_BYTES_SHIFT + sizeof(std::mutex));
    static constexpr size_t FIRST_FREE_BLOCK_BYTES_SHIFT = (FITMODE_BYTES_SHIFT + sizeof(allocator_with_fit_mode::fit_mode));
    
    static constexpr size_t BLOCK_META_SIZE = (sizeof(size_t) + sizeof(void*));
    static constexpr size_t BLOCK_SIZE_BYTES_SHIFT = 0;
    static constexpr size_t BLOCK_PTR_BYTES_SHIFT = (BLOCK_SIZE_BYTES_SHIFT + sizeof(size_t));

    void *_trusted_memory;

public:
    
    ~allocator_sorted_list() override;
    
    allocator_sorted_list(
        allocator_sorted_list const &other) = delete;
    
    allocator_sorted_list &operator=(
        allocator_sorted_list const &other) = delete;
    
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

    void deallocate(
        void* at) override;

private:

    unsigned char* block_construct(unsigned char*, unsigned char*, size_t);

    void block_set_associated_ptr(unsigned char*, unsigned char*);

    void block_set_size(unsigned char*, size_t);

    unsigned char* block_get_associated_ptr(unsigned char*) const;

    unsigned char* block_get_next_block(unsigned char*) const;

    size_t block_get_size(unsigned char*) const;

    [[nodiscard]] unsigned char* allocate_block(unsigned char* prev, unsigned char* next, size_t size);

private:

    [[nodiscard]] unsigned char* allocate_first_fit(size_t size);

    [[nodiscard]] unsigned char* allocate_best_fit(size_t size);

    [[nodiscard]] unsigned char* allocate_worst_fit(size_t size);

    void free_memory();

public:

    inline allocator_with_fit_mode::fit_mode get_fit_mode() const override;

    inline void set_fit_mode(allocator_with_fit_mode::fit_mode mode) override;

private:

    inline size_t get_space_size() const;

    inline void set_space_size(size_t);

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

    inline void init_mutex();

private:

    inline void* get_first_free_block() const;

    inline void set_first_free_block(void*);

private:
    
    inline std::string get_typename() const noexcept override;

};

#endif //MATH_PRACTICE_AND_OPERATING_SYSTEMS_ALLOCATOR_ALLOCATOR_SORTED_LIST_H