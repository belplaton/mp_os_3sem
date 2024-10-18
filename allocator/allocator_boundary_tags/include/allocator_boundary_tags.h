#ifndef MATH_PRACTICE_AND_OPERATING_SYSTEMS_ALLOCATOR_ALLOCATOR_BOUNDARY_TAGS_H
#define MATH_PRACTICE_AND_OPERATING_SYSTEMS_ALLOCATOR_ALLOCATOR_BOUNDARY_TAGS_H

#include <allocator_guardant.h>
#include <allocator_test_utils.h>
#include <allocator_with_fit_mode.h>
#include <logger_guardant.h>
#include <typename_holder.h>
#include <mutex>

class allocator_boundary_tags final:
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

    static constexpr size_t BLOCK_START_META_SIZE = (sizeof(size_t) + sizeof(void*));
    static constexpr size_t BLOCK_START_PTR_BYTES_SHIFT = 0;
    static constexpr size_t BLOCK_START_SIZE_BYTES_SHIFT = (BLOCK_START_PTR_BYTES_SHIFT + sizeof(void*));

    static constexpr size_t BLOCK_END_PTR_BYTES_SHIFT = 0;

    static constexpr size_t FREE_BLOCK_END_META_SIZE = (sizeof(size_t) + sizeof(void*));
    static constexpr size_t FREE_BLOCK_END_SIZE_BYTES_SHIFT = (BLOCK_END_PTR_BYTES_SHIFT + sizeof(size_t));
    static constexpr size_t FREE_BLOCK_META_SIZE = BLOCK_START_META_SIZE + FREE_BLOCK_END_META_SIZE;

    static constexpr size_t CAPTURED_BLOCK_END_META_SIZE = sizeof(size_t);
    static constexpr size_t CAPTURED_BLOCK_META_SIZE = BLOCK_START_META_SIZE + CAPTURED_BLOCK_END_META_SIZE;

    static constexpr size_t BLOCKS_META_SIZE_DIFF = FREE_BLOCK_META_SIZE - CAPTURED_BLOCK_META_SIZE;

    void *_trusted_memory;

public:
    
    ~allocator_boundary_tags() override;
    
    allocator_boundary_tags(
        allocator_boundary_tags const& other) = delete;
    
    allocator_boundary_tags &operator=(
        allocator_boundary_tags const &other) = delete;
    
    allocator_boundary_tags(
        allocator_boundary_tags &&other) noexcept;
    
    allocator_boundary_tags &operator=(
        allocator_boundary_tags &&other) noexcept;

public:
    
    explicit allocator_boundary_tags(
        size_t space_size,
        allocator *parent_allocator = nullptr,
        logger *logger = nullptr,
        allocator_with_fit_mode::fit_mode allocate_fit_mode = allocator_with_fit_mode::fit_mode::first_fit);

public:
    
    [[nodiscard]] void *allocate(
        size_t value_size,
        size_t values_count) override;
    
    void deallocate(
        void *at) override;

private:

    unsigned char* block_get_ptr(unsigned char* at_char, bool is_start) const;

    void block_set_ptr(unsigned char* at_char, unsigned char* ptr, bool is_start);

    unsigned char* free_block_get_prev(unsigned char* at_char, bool is_start) const;

    unsigned char* free_block_get_next(unsigned char* at_char, bool is_start) const;

    void free_block_set_prev(unsigned char* at_char, unsigned char* prev, bool is_start);

    void free_block_set_next(unsigned char* at_char, unsigned char* next, bool is_start);

    size_t free_block_get_size(unsigned char* at_char, bool is_start) const;

    void free_block_set_size(unsigned char* at_char, size_t size, bool is_start);

    size_t captured_block_get_size(unsigned char* at_start_char) const;

    void captured_block_set_size(unsigned char* at_start_char, size_t size);

    unsigned char* captured_block_get_end(unsigned char* at_start_char) const;

    unsigned char* free_block_get_end(unsigned char* at_start_char) const;

    unsigned char* free_block_get_start(unsigned char* at_end_char) const;

    bool block_is_free(unsigned char* at_char_start) const;

    [[nodiscard]] unsigned char* allocate_block(unsigned char* prev, unsigned char* next, size_t size);

private:

    [[nodiscard]] unsigned char* allocate_first_fit(size_t size);

    [[nodiscard]] unsigned char* allocate_best_fit(size_t size);

    [[nodiscard]] unsigned char* allocate_worst_fit(size_t size);

    void free_memory();

private:

    inline size_t get_space_size() const;

    inline void set_space_size(size_t);

public:

    inline allocator_with_fit_mode::fit_mode get_fit_mode() const override;

    inline void set_fit_mode(allocator_with_fit_mode::fit_mode mode) override;

private:

    inline allocator* get_allocator() const override;

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

#endif //MATH_PRACTICE_AND_OPERATING_SYSTEMS_ALLOCATOR_ALLOCATOR_BOUNDARY_TAGS_H