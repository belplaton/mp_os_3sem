#ifndef MATH_PRACTICE_AND_OPERATING_SYSTEMS_ALLOCATOR_ALLOCATOR_BUDDIES_SYSTEM_H
#define MATH_PRACTICE_AND_OPERATING_SYSTEMS_ALLOCATOR_ALLOCATOR_BUDDIES_SYSTEM_H

#include <allocator_guardant.h>
#include <allocator_test_utils.h>
#include <allocator_with_fit_mode.h>
#include <logger_guardant.h>
#include <typename_holder.h>
#include <mutex>

class allocator_buddies_system final:
    private allocator_guardant,
    public allocator_test_utils,
    public allocator_with_fit_mode,
    private logger_guardant,
    private typename_holder
{

private:
    
    static constexpr size_t ALLOCATOR_META_SIZE =
        (sizeof(size_t) + // degree
        sizeof(allocator*) +
        sizeof(logger*) +
        sizeof(std::mutex) +
        sizeof(allocator_with_fit_mode::fit_mode) +
        sizeof(void*));

    static constexpr size_t SIZE_DEGREE_BYTES_SHIFT = 0;
    static constexpr size_t ALLOCATOR_BYTES_SHIFT = (SIZE_DEGREE_BYTES_SHIFT + sizeof(size_t));
    static constexpr size_t LOGGER_BYTES_SHIFT = (ALLOCATOR_BYTES_SHIFT + sizeof(allocator*));
    static constexpr size_t MUTEX_BYTES_SHIFT = (LOGGER_BYTES_SHIFT + sizeof(logger*));
    static constexpr size_t FITMODE_BYTES_SHIFT = (MUTEX_BYTES_SHIFT + sizeof(std::mutex));
    static constexpr size_t FIRST_FREE_BLOCK_BYTES_SHIFT = (FITMODE_BYTES_SHIFT + sizeof(allocator_with_fit_mode::fit_mode));

    static constexpr size_t BLOCK_DEGREE_BYTES_SHIFT = 0;
    static constexpr size_t BLOCK_PTR_BYTES_SHIFT = (BLOCK_DEGREE_BYTES_SHIFT + sizeof(unsigned char));

    static constexpr size_t FREE_BLOCK_PTR_BYTES_SHIFT = (BLOCK_PTR_BYTES_SHIFT + sizeof(void*));

    static constexpr size_t FREE_BLOCK_META_SIZE = (sizeof(unsigned char) + sizeof(void*) * 2);
    static constexpr size_t CAPTURED_BLOCK_META_SIZE = (sizeof(unsigned char) + sizeof(void*));

    static constexpr size_t BLOCKS_META_SIZE_DIFF = FREE_BLOCK_META_SIZE - CAPTURED_BLOCK_META_SIZE;

    void *_trusted_memory;

public:
    
    ~allocator_buddies_system() override;
    
    allocator_buddies_system(
        allocator_buddies_system const &other) = delete;
    
    allocator_buddies_system &operator=(
        allocator_buddies_system const &other) = delete;
    
    allocator_buddies_system(
        allocator_buddies_system &&other) noexcept;
    
    allocator_buddies_system &operator=(
        allocator_buddies_system &&other) noexcept;

public:
    
    explicit allocator_buddies_system(
        size_t space_size_power_of_two,
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

    [[nodiscard]] unsigned char* allocate_block(unsigned char* next, unsigned char degree);

    [[nodiscard]] unsigned char* allocate_first_fit(size_t size);

    [[nodiscard]] unsigned char* allocate_best_fit(size_t size);

    [[nodiscard]] unsigned char* allocate_worst_fit(size_t size);

    void free_memory();

private:

    unsigned long long global_to_local(unsigned char* at_char) const;

    unsigned char* local_to_global(unsigned long long at_char) const;

    unsigned char* block_get_ptr(unsigned char* at_char) const;

    void block_set_ptr(unsigned char* at_char, unsigned char* ptr);

    size_t block_get_size(unsigned char* at_char) const;

    unsigned char block_get_degree(unsigned char* at_char) const;

    void block_set_degree(unsigned char* at_char, unsigned char degree);

    unsigned char* block_get_buddie(unsigned char* at_char, unsigned char degree) const;

    unsigned char* block_get_buddie(unsigned char* at_char) const;

    bool block_is_free(unsigned char* at_char) const;

    unsigned char* free_block_get_prev(unsigned char* at_char) const;

    unsigned char* free_block_get_next(unsigned char* at_char) const;

    void free_block_set_prev(unsigned char* at_char, unsigned char* prev);

    void free_block_set_next(unsigned char* at_char, unsigned char* next);

    bool try_free_block_split(unsigned char* at_char);

    void block_merge_to_free(unsigned char* at_char, unsigned char* prev, unsigned char* next);

private:

    inline size_t get_space_size() const;

    inline unsigned char get_space_degree() const;

    inline void set_space_degree(unsigned char);

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
    
private:

    unsigned int static constexpr nearest_power_of_two(unsigned int N);

    unsigned char static constexpr get_degree_bottom_limit();

};

#endif //MATH_PRACTICE_AND_OPERATING_SYSTEMS_ALLOCATOR_ALLOCATOR_BUDDIES_SYSTEM_H
