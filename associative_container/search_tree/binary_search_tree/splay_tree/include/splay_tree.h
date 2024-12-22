#ifndef MATH_PRACTICE_AND_OPERATING_SYSTEMS_SPLAY_TREE_H
#define MATH_PRACTICE_AND_OPERATING_SYSTEMS_SPLAY_TREE_H

#include <binary_search_tree.h>

// TODO: переделать удаление, чтобы удаляя ноду мы перемещали удаляемую ноду в рут, сплитили дерево
// А затем брали любое из двух поддеревьев. допустим правое. а его самый левый элемент ставим в рут.
// Тогда слева у него нет детей и левое поддерево ставим его влево

template<
    typename tkey,
    typename tvalue>
class splay_tree final:
    public binary_search_tree<tkey, tvalue>
{

private:

    class tree_splayer
    {

    private:

        splay_tree<tkey, tvalue> *_tree;

    protected:

        explicit tree_splayer(
            splay_tree<tkey, tvalue> *tree);

    public:

        virtual ~tree_splayer() noexcept = default;

    protected:

        void make_splay(
            std::stack<typename binary_search_tree<tkey, tvalue>::node **> &path);

    };

private:
    
    class insertion_template_method final:
        public binary_search_tree<tkey, tvalue>::insertion_template_method,
        public tree_splayer
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
        public tree_splayer
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
        public tree_splayer
    {
    
    public:
        
        explicit disposal_template_method(
            splay_tree<tkey, tvalue> *tree,
            typename binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy disposal_strategy);

    public:

        tvalue dispose(
            tkey const &key) override;

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
splay_tree<tkey, tvalue>::tree_splayer::tree_splayer(
    splay_tree<tkey, tvalue> *tree):
        _tree(tree)
{

}

template<
    typename tkey,
    typename tvalue>
void splay_tree<tkey, tvalue>::tree_splayer::make_splay(
    std::stack<typename binary_search_tree<tkey, tvalue>::node **> &path)
{
    _tree->trace_with_guard("make_splay for splay_tree\n");
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
            (*grandparent)->left_subtree == *parent
                ? ((*parent)->left_subtree == *to_splay
                    ? _tree->double_right_rotation(*grandparent, true)
                    : _tree->big_right_rotation(*grandparent))
                : ((*parent)->right_subtree == *to_splay
                    ? _tree->double_left_rotation(*grandparent, true)
                    : _tree->big_left_rotation(*grandparent));
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
    binary_search_tree<tkey, tvalue>::insertion_template_method(
        dynamic_cast<binary_search_tree<tkey, tvalue> *>(tree),
        insertion_strategy),
    splay_tree<tkey, tvalue>::tree_splayer(tree)
{

}

template<
    typename tkey,
    typename tvalue>
void splay_tree<tkey, tvalue>::insertion_template_method::balance(
    std::stack<typename binary_search_tree<tkey, tvalue>::node **> &path)
{
    tree_splayer::make_splay(path);
}

template<
    typename tkey,
    typename tvalue>
splay_tree<tkey, tvalue>::obtaining_template_method::obtaining_template_method(
    splay_tree<tkey, tvalue> *tree):
    binary_search_tree<tkey, tvalue>::obtaining_template_method(dynamic_cast<binary_search_tree<tkey, tvalue> *>(tree)),
    tree_splayer(tree)
{

}

template<
    typename tkey,
    typename tvalue>
void splay_tree<tkey, tvalue>::obtaining_template_method::balance(
    std::stack<typename binary_search_tree<tkey, tvalue>::node **> &path)
{
    tree_splayer::make_splay(path);
}

template<
    typename tkey,
    typename tvalue>
splay_tree<tkey, tvalue>::disposal_template_method::disposal_template_method(
    splay_tree<tkey, tvalue> *tree,
    typename binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy disposal_strategy):
    binary_search_tree<tkey, tvalue>::disposal_template_method(dynamic_cast<binary_search_tree<tkey, tvalue> *>(tree), disposal_strategy),
    tree_splayer(tree)
{
    //throw not_implemented("template<typename tkey, typename tvalue> splay_tree<tkey, tvalue>::disposal_template_method::disposal_template_method(splay_tree<tkey, tvalue> *)", "your code should be here...");
}

template<
    typename tkey,
    typename tvalue>
tvalue splay_tree<tkey, tvalue>::disposal_template_method::dispose(tkey const &key)
{
    std::stack<typename binary_search_tree<tkey, tvalue>::node**> path_to_node_with_key = this->obtain_path(key);
    auto** current = path_to_node_with_key.top();

    if (current == nullptr)
    {
        switch (this->disposal_strategy)
        {
        case binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy::do_nothing:
            return tvalue();
            break;
        case binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy::throw_an_exception:
            throw typename binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_exception(key);
        default:
            throw not_implemented(
"tvalue splay_tree<tkey, tvalue>::disposal_template_method::dispose(tkey const &key)",
    "Not implemented disposal strategy");
        }
    }
    else
    {
        balance(path_to_node_with_key);

        auto node_to_remove = binary_search_tree<tkey, tvalue>::disposal_template_method::get_root();
        auto return_value = node_to_remove->value;

        if (node_to_remove->left_subtree == nullptr)
        {
            binary_search_tree<tkey, tvalue>::disposal_template_method::set_root(node_to_remove->right_subtree);
        }
        else if (node_to_remove->right_subtree == nullptr)
        {
            binary_search_tree<tkey, tvalue>::disposal_template_method::set_root(node_to_remove->left_subtree);
        }
        else
        {
            std::stack<typename binary_search_tree<tkey, tvalue>::node**> path_to_smallest_node {};
            auto* biggest = node_to_remove->left_subtree;
            binary_search_tree<tkey, tvalue>::disposal_template_method::set_root(biggest);

            path_to_smallest_node.push(&biggest);
            while (biggest->right_subtree != nullptr)
            {
                biggest = biggest->right_subtree;
                path_to_smallest_node.push(&biggest);
            }

            balance(path_to_smallest_node);
            binary_search_tree<tkey, tvalue>::disposal_template_method::get_root()->right_subtree = node_to_remove->right_subtree;
        }

        this->deallocate_with_guard(node_to_remove);
        return return_value;
    }
}

template<
    typename tkey,
    typename tvalue>
void splay_tree<tkey, tvalue>::disposal_template_method::balance(
    std::stack<typename binary_search_tree<tkey, tvalue>::node **> &path)
{
    tree_splayer::make_splay(path);
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
    binary_search_tree<tkey, tvalue>(
        new (allocator_guardant::allocate_with_guard_static(allocator,
            sizeof(splay_tree<tkey, tvalue>::insertion_template_method)))
            splay_tree<tkey, tvalue>::insertion_template_method(this, insertion_strategy),
        new (allocator_guardant::allocate_with_guard_static(allocator,
            sizeof(splay_tree<tkey, tvalue>::obtaining_template_method)))
            splay_tree<tkey, tvalue>::obtaining_template_method(this),
        new (allocator_guardant::allocate_with_guard_static(allocator,
            sizeof(splay_tree<tkey, tvalue>::disposal_template_method)))
            splay_tree<tkey, tvalue>::disposal_template_method(this, disposal_strategy),
        comparer,
        allocator,
        logger)
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