#ifndef MATH_PRACTICE_AND_OPERATING_SYSTEMS_SPLAY_TREE_H
#define MATH_PRACTICE_AND_OPERATING_SYSTEMS_SPLAY_TREE_H

#include <binary_search_tree.h>

template<
    typename tkey,
    typename tvalue>
class splay_tree final:
    public binary_search_tree<tkey, tvalue>
{

private:

    class splay
    {

    private:

        splay_tree<tkey, tvalue> *_tree;

    protected:

        explicit splay(
            splay_tree<tkey, tvalue> *tree);

    public:

        virtual ~splay() noexcept = default;

    protected:

        void make(
            std::stack<typename binary_search_tree<tkey, tvalue>::node **> &path);

    };

private:
    
    class insertion_template_method final:
        public binary_search_tree<tkey, tvalue>::insertion_template_method,
        public splay
    {
    
    public:
        
        explicit insertion_template_method(
            splay_tree<tkey, tvalue> *tree,
            typename binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy insertion_strategy);
    
    private:
        
        void balance(
            std::stack<typename binary_search_tree<tkey, tvalue>::node **> &path) override;
        
    };
    
    class obtaining_template_method final:
        public binary_search_tree<tkey, tvalue>::obtaining_template_method,
        public splay
    {
    
    public:
        
        explicit obtaining_template_method(
            splay_tree<tkey, tvalue> *tree);

    private:

        void balance(
            std::stack<typename binary_search_tree<tkey, tvalue>::node **> &path) override;
        
    };
    
    class disposal_template_method final:
        public binary_search_tree<tkey, tvalue>::disposal_template_method,
        public splay
    {
    
    public:
        
        explicit disposal_template_method(
            splay_tree<tkey, tvalue> *tree,
            typename binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy disposal_strategy);
        
    private:

        void balance(
            std::stack<typename binary_search_tree<tkey, tvalue>::node **> &path) override;
        
    };

public:

    explicit splay_tree(
        allocator *allocator = nullptr,
        logger *logger = nullptr);

    explicit splay_tree(
        std::function<int(tkey const &, tkey const &)> comparer = std::less<tkey>(),

        allocator *allocator = nullptr,
        logger *logger = nullptr,

        typename binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy insertion_strategy =
            binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy::throw_an_exception,

        typename binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy disposal_strategy =
            binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy::throw_an_exception

);

public:
    
    ~splay_tree() noexcept override;
    
    splay_tree(
        splay_tree<tkey, tvalue> const &other);
    
    splay_tree<tkey, tvalue> &operator=(
        splay_tree<tkey, tvalue> const &other);
    
    splay_tree(
        splay_tree<tkey, tvalue> &&other) noexcept;
    
    splay_tree<tkey, tvalue> &operator=(
        splay_tree<tkey, tvalue> &&other) noexcept;
    
};

#pragma region splay implementation

template<
    typename tkey,
    typename tvalue>
splay_tree<tkey, tvalue>::splay::splay(
    splay_tree<tkey, tvalue> *tree):
        _tree(tree)
{

}

template<
    typename tkey,
    typename tvalue>
void splay_tree<tkey, tvalue>::splay::make(
    std::stack<typename binary_search_tree<tkey, tvalue>::node **> &path)
{
    if (path.empty())
    {
        throw std::invalid_argument("Path to balance is empty!");
    }

    typename binary_search_tree<tkey, tvalue>::node **to_splay = path.top();
    path.pop();

    while (!path.empty())
    {
        auto **parent = path.top();
        path.pop();

        if (path.empty())
        {
            (*parent)->left_subtree == *to_splay
                ? _tree->small_right_rotation(*parent)
                : _tree->small_left_rotation(*parent);

            to_splay = parent;
        }
        else
        {
            auto **grandparent = path.top();
            path.pop();
            (*grandparent)->left_subtree == parent
                ? ((*parent)->left_subtree == *to_splay
                    ? _tree->double_right_rotation(*grandparent, true)
                    : _tree->big_right_rotation(*grandparent))
                : (*parent->left_subtree == *to_splay
                    ? _tree->big_left_rotation(*grandparent)
                    : _tree->double_left_rotation(*grandparent, true));
            to_splay = grandparent;
        }
    }
}

#pragma endregion

template<
    typename tkey,
    typename tvalue>
splay_tree<tkey, tvalue>::insertion_template_method::insertion_template_method(
    splay_tree<tkey, tvalue> *tree,
    typename binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy insertion_strategy):
    binary_search_tree<tkey, tvalue>::insertion_template_method(dynamic_cast<binary_search_tree<tkey, tvalue> *>(tree)),
    splay(tree)
{

}

template<
    typename tkey,
    typename tvalue>
void splay_tree<tkey, tvalue>::insertion_template_method::balance(
    std::stack<typename binary_search_tree<tkey, tvalue>::node **> &path)
{
    splay::make(path);
}

template<
    typename tkey,
    typename tvalue>
splay_tree<tkey, tvalue>::obtaining_template_method::obtaining_template_method(
    splay_tree<tkey, tvalue> *tree):
    binary_search_tree<tkey, tvalue>::obtaining_template_method(dynamic_cast<binary_search_tree<tkey, tvalue> *>(tree)),
    splay(tree)
{

}

template<
    typename tkey,
    typename tvalue>
void splay_tree<tkey, tvalue>::obtaining_template_method::balance(
    std::stack<typename binary_search_tree<tkey, tvalue>::node **> &path)
{
    splay::make(path);
}

template<
    typename tkey,
    typename tvalue>
splay_tree<tkey, tvalue>::disposal_template_method::disposal_template_method(
    splay_tree<tkey, tvalue> *tree,
    typename binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy disposal_strategy):
    binary_search_tree<tkey, tvalue>::disposal_template_method(dynamic_cast<binary_search_tree<tkey, tvalue> *>(tree), disposal_strategy),
    splay(tree)
{
    //throw not_implemented("template<typename tkey, typename tvalue> splay_tree<tkey, tvalue>::disposal_template_method::disposal_template_method(splay_tree<tkey, tvalue> *)", "your code should be here...");
}

template<typename tkey, typename tvalue>
void splay_tree<tkey, tvalue>::disposal_template_method::balance(
    std::stack<typename binary_search_tree<tkey, tvalue>::node **> &path)
{
    splay::make(path);
}

#pragma region splay_tree consturctors

template<
    typename tkey,
    typename tvalue>
splay_tree<tkey, tvalue>::splay_tree(allocator *allocator, logger *logger):
    splay_tree(
        std::less<tkey>(),
        allocator,
        logger)
{

}

template<
    typename tkey,
    typename tvalue>
splay_tree<tkey, tvalue>::splay_tree(
    std::function<int(tkey const &, tkey const &)> comparer,
    allocator *allocator,
    logger *logger,
    typename binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy insertion_strategy,
    typename binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy disposal_strategy):
    binary_search_tree<tkey, tvalue>(comparer, allocator, logger, insertion_strategy, disposal_strategy)
{

}

template<
    typename tkey,
    typename tvalue>
splay_tree<tkey, tvalue>::~splay_tree() noexcept
{
    //throw not_implemented("template<typename tkey, typename tvalue> splay_tree<tkey, tvalue>::~splay_tree() noexcept", "your code should be here...");
}

template<
    typename tkey,
    typename tvalue>
splay_tree<tkey, tvalue>::splay_tree(
    splay_tree<tkey, tvalue> const &other):
    binary_search_tree<tkey, tvalue>(other)
{
    //throw not_implemented("template<typename tkey, typename tvalue> splay_tree<tkey, tvalue>::splay_tree(splay_tree<tkey, tvalue> const &)", "your code should be here...");
}

template<
    typename tkey,
    typename tvalue>
splay_tree<tkey, tvalue> &splay_tree<tkey, tvalue>::operator=(
    splay_tree<tkey, tvalue> const &other)
{
    if (this != &other)
    {
        binary_search_tree<tkey, tvalue>::operator=(other);
    }

    return *this;
    //throw not_implemented("template<typename tkey, typename tvalue> splay_tree<tkey, tvalue> &splay_tree<tkey, tvalue>::operator=(splay_tree<tkey, tvalue> const &)", "your code should be here...");
}

template<
    typename tkey,
    typename tvalue>
splay_tree<tkey, tvalue>::splay_tree(
    splay_tree<tkey, tvalue> &&other) noexcept:
    binary_search_tree<tkey, tvalue>(std::forward<splay_tree<tkey, tvalue>>(other))
{
    //throw not_implemented("template<typename tkey, typename tvalue> splay_tree<tkey, tvalue>::splay_tree(splay_tree<tkey, tvalue> &&) noexcept", "your code should be here...");
}

template<
    typename tkey,
    typename tvalue>
splay_tree<tkey, tvalue> &splay_tree<tkey, tvalue>::operator=(
    splay_tree<tkey, tvalue> &&other) noexcept
{
    if (this != &other)
    {
        binary_search_tree<tkey, tvalue>::operator=(std::forward<splay_tree<tkey, tvalue>>(other));
    }

    return *this;
    //throw not_implemented("template<typename tkey, typename tvalue> splay_tree<tkey, tvalue> &splay_tree<tkey, tvalue>::operator=(splay_tree<tkey, tvalue> &&) noexcept", "your code should be here...");
}

#pragma endregion

#endif //MATH_PRACTICE_AND_OPERATING_SYSTEMS_SPLAY_TREE_H