#ifndef MATH_PRACTICE_AND_OPERATING_SYSTEMS_ALLOCATOR_ALLOCATOR_RED_BLACK_TREE_H
#define MATH_PRACTICE_AND_OPERATING_SYSTEMS_ALLOCATOR_ALLOCATOR_RED_BLACK_TREE_H

#include <allocator_guardant.h>
#include <allocator_test_utils.h>
#include <allocator_with_fit_mode.h>
#include <logger_guardant.h>
#include <typename_holder.h>

// 5 принципов построения красно-черного дерева
// 1. у каждого  узла дерева либо красный цвет либо черный
// 2. sentinel node - нода которой не существует, является пустым поддеревом. всегда черная.
//    либо делаем вид что нода существует, но на деле там nullptr
//    либо подставляем вместо nullptr в пустоту, создавая в виде статика или синглтона и знаем о том что он сентинел
// 3. I. у красного узла не может быть другого красного дочернего узла
//    II у черного узла дочерний узел может быть любого цвета
// 4. корень дерева всегда черного цвета
// 5. свойство черной высоты. любой тривиальный путь (напрямую) от корня до терминального узла
//    занимает одинаковое число черных узлов

class allocator_red_black_tree final:
    private allocator_guardant,
    public allocator_test_utils,
    public allocator_with_fit_mode,
    private logger_guardant,
    private typename_holder
{

private:
    
    void *_trusted_memory;

public:
    
    ~allocator_red_black_tree() override;
    
    allocator_red_black_tree(
        allocator_red_black_tree const &other);
    
    allocator_red_black_tree &operator=(
        allocator_red_black_tree const &other);
    
    allocator_red_black_tree(
        allocator_red_black_tree &&other) noexcept;
    
    allocator_red_black_tree &operator=(
        allocator_red_black_tree &&other) noexcept;

public:
    
    explicit allocator_red_black_tree(
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

public:
    
    inline void set_fit_mode(
        allocator_with_fit_mode::fit_mode mode) override;

private:
    
    inline allocator *get_allocator() const override;

public:
    
    std::vector<allocator_test_utils::block_info> get_blocks_info() const noexcept override;

private:
    
    inline logger *get_logger() const override;

private:
    
    inline std::string get_typename() const noexcept override;
    
};

#endif //MATH_PRACTICE_AND_OPERATING_SYSTEMS_ALLOCATOR_ALLOCATOR_RED_BLACK_TREE_H