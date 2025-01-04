#ifndef MATH_PRACTICE_AND_OPERATING_SYSTEMS_AVL_TREE_H
#define MATH_PRACTICE_AND_OPERATING_SYSTEMS_AVL_TREE_H

#include <binary_search_tree.h>
#include <cmath>

template<
    typename tkey,
    typename tvalue>
class AVL_tree final:
    public binary_search_tree<tkey, tvalue>
{

private:
    
    struct node final:
        binary_search_tree<tkey, tvalue>::node
    {

    public:

        explicit node(tkey const &key, tvalue &&value, size_t const &height);

        size_t height;

        int get_balance() const;

        static void update_height(node* other);

        typename binary_search_tree<tkey, tvalue>::iterator_data* construct_iterator_data(int depth) override;

    };

public:
    
    struct iterator_data final:
        public binary_search_tree<tkey, tvalue>::iterator_data
    {
    
    public:
        
        size_t subtree_height;
    
    public:
        
        explicit iterator_data(
            unsigned int depth,
            tkey const &key,
            tvalue const &value,
            size_t subtree_height);

    };

protected:

    void small_left_rotation(
        typename binary_search_tree<tkey, tvalue>::node *&subtree_root,
        bool validate = true) const override;

    void small_right_rotation(
        typename binary_search_tree<tkey, tvalue>::node *&subtree_root,
        bool validate = true) const override;

private:
    
    class insertion_template_method final:
        public binary_search_tree<tkey, tvalue>::insertion_template_method
    {
    
    public:
        
        explicit insertion_template_method(
            AVL_tree<tkey, tvalue> *tree,
            typename binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy insertion_strategy);
    
    protected:
        
        [[nodiscard]] inline size_t obtain_node_size() const noexcept override;

        inline void construct_node(
            typename binary_search_tree<tkey, tvalue>::node* at,
            tkey const& key,
            tvalue&& value) const noexcept override;

        void balance(
            std::stack<typename binary_search_tree<tkey, tvalue>::node **> &path) override;
        
    };
    
    class obtaining_template_method final:
        public binary_search_tree<tkey, tvalue>::obtaining_template_method
    {
    
    public:
        
        explicit obtaining_template_method(
            AVL_tree<tkey, tvalue> *tree);
        
        // TODO: think about it!
        
    };
    
    class disposal_template_method final:
        public binary_search_tree<tkey, tvalue>::disposal_template_method
    {
    
    public:
        
        explicit disposal_template_method(
            AVL_tree<tkey, tvalue> *tree,
            typename binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy disposal_strategy);

    protected:

        void balance(
            std::stack<typename binary_search_tree<tkey, tvalue>::node **> &path) override;
        
    };

public:
    
    explicit AVL_tree(
        std::function<int(tkey const &, tkey const &)> comparer = std::less<tkey>(),
        allocator *allocator = nullptr,
        logger *logger = nullptr,
        typename binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy insertion_strategy =
            binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy::throw_an_exception,
        typename binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy disposal_strategy =
            binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy::throw_an_exception);

    explicit AVL_tree(
    allocator *allocator = nullptr,
    logger *logger = nullptr,
    typename binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy insertion_strategy =
        binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy::throw_an_exception,
    typename binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy disposal_strategy =
        binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy::throw_an_exception);

public:
    
    ~AVL_tree() noexcept final;
    
    AVL_tree(
        AVL_tree<tkey, tvalue> const &other);
    
    AVL_tree<tkey, tvalue> &operator=(
        AVL_tree<tkey, tvalue> const &other);
    
    AVL_tree(
        AVL_tree<tkey, tvalue> &&other) noexcept;
    
    AVL_tree<tkey, tvalue> &operator=(
        AVL_tree<tkey, tvalue> &&other) noexcept;
    
};

template<
    typename tkey,
    typename tvalue>
AVL_tree<tkey, tvalue>::node::node(
    tkey const &key,
    tvalue &&value,
    size_t const &height):
    binary_search_tree<tkey, tvalue>::node(key, std::forward<tvalue&&>(value)),
    height(height)
{

}

template<
    typename tkey,
typename tvalue>
int AVL_tree<tkey, tvalue>::node::get_balance() const
{
    return (this->right_subtree != nullptr ? dynamic_cast<node *>(this->right_subtree)->height : 0)
    - (this->left_subtree != nullptr ? dynamic_cast<node *>(this->left_subtree)->height : 0);
}

template<
    typename tkey,
    typename tvalue>
void AVL_tree<tkey, tvalue>::node::update_height(node *other)
{
    if (other != nullptr)
    {
        other->height = std::max(
            other->left_subtree != nullptr ? dynamic_cast<node *>(other->left_subtree)->height : 0,
            other->right_subtree != nullptr ? dynamic_cast<node *>(other->right_subtree)->height : 0)
            + 1;
    }
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::iterator_data*
    AVL_tree<tkey, tvalue>::node::construct_iterator_data(
    int depth)
{
    iterator_data* data = new iterator_data(depth, this->key, this->value, height);
    return data;
}

template<
    typename tkey,
    typename tvalue>
AVL_tree<tkey, tvalue>::iterator_data::iterator_data(
    unsigned int depth,
    tkey const &key,
    tvalue const &value,
    size_t subtree_height):
    binary_search_tree<tkey, tvalue>::iterator_data(depth, key, value),
    subtree_height(subtree_height)
{
    //throw not_implemented("template<typename tkey, typename tvalue> AVL_tree<tkey, tvalue>::iterator_data::iterator_data(unsigned int, tkey const &, tvalue const &, size_t)", "your code should be here...");
}

template<
    typename tkey,
    typename tvalue>
void AVL_tree<tkey, tvalue>::small_left_rotation(
    typename binary_search_tree<tkey, tvalue>::node *&subtree_root, bool validate) const
{
    binary_search_tree<tkey, tvalue>::small_left_rotation(subtree_root, validate);
}

template<
    typename tkey,
    typename tvalue>
void AVL_tree<tkey, tvalue>::small_right_rotation(
    typename binary_search_tree<tkey, tvalue>::node *&subtree_root, bool validate) const
{
    binary_search_tree<tkey, tvalue>::small_right_rotation(subtree_root, validate);
}

template<
    typename tkey,
    typename tvalue>
AVL_tree<tkey, tvalue>::insertion_template_method::insertion_template_method(
    AVL_tree<tkey, tvalue> *tree,
    typename binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy insertion_strategy):
    binary_search_tree<tkey, tvalue>::insertion_template_method(tree, insertion_strategy)
{
    //throw not_implemented("template<typename tkey, typename tvalue> AVL_tree<tkey, tvalue>::insertion_template_method::insertion_template_method(AVL_tree<tkey, tvalue> *, typename binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy)", "your code should be here...");
}

template<
    typename tkey,
    typename tvalue>
size_t AVL_tree<tkey, tvalue>::insertion_template_method::obtain_node_size() const noexcept
{
    return sizeof(AVL_tree<tkey, tvalue>::node);
}

template<
    typename tkey,
    typename tvalue>
void AVL_tree<tkey, tvalue>::insertion_template_method::construct_node(
    typename binary_search_tree<tkey, tvalue>::node *at, tkey const &key, tvalue &&value) const noexcept
{
    allocator::construct(reinterpret_cast<AVL_tree<tkey, tvalue>::node *>(at), key, std::forward<tvalue>(value), 1);
}

template<
    typename tkey,
typename tvalue>
void AVL_tree<tkey, tvalue>::insertion_template_method::balance(
    std::stack<typename binary_search_tree<tkey, tvalue>::node **> &path)
{
    if (path.empty())
    {
        throw std::invalid_argument("Path to balance is empty!");
    }

    path.pop();

    while (!path.empty())
    {
        auto **parent = path.top();
        path.pop();

        auto balance_factor = dynamic_cast<node *>(*parent)->get_balance();
        if (abs(balance_factor) > 1)
        {
            if (balance_factor < 0)
            {
                auto left_balance_factor = dynamic_cast<node *>((*parent)->left_subtree)->get_balance();
                if (left_balance_factor > 0)
                {
                    dynamic_cast<AVL_tree<tkey, tvalue>*>(this->_tree)->small_right_rotation(*parent);
                }
                else
                {
                    dynamic_cast<AVL_tree<tkey, tvalue>*>(this->_tree)->big_right_rotation(*parent);
                }
            }
            else
            {
                auto right_balance_factor = dynamic_cast<node *>((*parent)->right_subtree)->get_balance();
                if (right_balance_factor > 0)
                {
                    dynamic_cast<AVL_tree<tkey, tvalue>*>(this->_tree)->small_left_rotation(*parent);
                }
                else
                {
                    dynamic_cast<AVL_tree<tkey, tvalue>*>(this->_tree)->big_left_rotation(*parent);
                }
            }
        }

        auto avl_node = dynamic_cast<node*>(*parent);
        node::update_height(dynamic_cast<node*>(avl_node->left_subtree));
        node::update_height(dynamic_cast<node*>(avl_node->right_subtree));
        node::update_height(avl_node);
    }
}

template<
    typename tkey,
    typename tvalue>
AVL_tree<tkey, tvalue>::obtaining_template_method::obtaining_template_method(
    AVL_tree<tkey, tvalue> *tree):
    binary_search_tree<tkey, tvalue>::obtaining_template_method(tree)
{
    //throw not_implemented("template<typename tkey, typename tvalue> AVL_tree<tkey, tvalue>::obtaining_template_method::obtaining_template_method(AVL_tree<tkey, tvalue> *)", "your code should be here...");
}

template<
    typename tkey,
    typename tvalue>
AVL_tree<tkey, tvalue>::disposal_template_method::disposal_template_method(
    AVL_tree<tkey, tvalue> *tree,
    typename binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy disposal_strategy):
    binary_search_tree<tkey, tvalue>::disposal_template_method(tree, disposal_strategy)
{
    //throw not_implemented("template<typename tkey, typename tvalue> AVL_tree<tkey, tvalue>::disposal_template_method::disposal_template_method(AVL_tree<tkey, tvalue> *, typename binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy)", "your code should be here...");
}

template<
    typename tkey,
    typename tvalue>
void AVL_tree<tkey, tvalue>::disposal_template_method::balance(
    std::stack<typename binary_search_tree<tkey, tvalue>::node **> &path)
{
    if (path.empty())
    {
        throw std::invalid_argument("Path to balance is empty!");
    }

    path.pop();

    while (!path.empty())
    {
        auto **parent = path.top();
        path.pop();

        auto balance_factor = dynamic_cast<node *>(*parent)->get_balance();
        if (abs(balance_factor) > 1)
        {
            if (balance_factor < 0)
            {
                auto left_balance_factor = dynamic_cast<node *>((*parent)->left_subtree)->get_balance();
                if (left_balance_factor > 0)
                {
                    dynamic_cast<AVL_tree<tkey, tvalue>*>(this->_tree)->small_right_rotation(*parent);
                }
                else
                {
                    dynamic_cast<AVL_tree<tkey, tvalue>*>(this->_tree)->big_right_rotation(*parent);
                }
            }
            else
            {
                auto right_balance_factor = dynamic_cast<node *>((*parent)->right_subtree)->get_balance();
                if (right_balance_factor > 0)
                {
                    dynamic_cast<AVL_tree<tkey, tvalue>*>(this->_tree)->small_left_rotation(*parent);
                }
                else
                {
                    dynamic_cast<AVL_tree<tkey, tvalue>*>(this->_tree)->big_left_rotation(*parent);
                }
            }
        }

        auto avl_node = dynamic_cast<node*>(*parent);
        node::update_height(dynamic_cast<node*>(avl_node->left_subtree));
        node::update_height(dynamic_cast<node*>(avl_node->right_subtree));
        node::update_height(avl_node);
    }
}

template<
    typename tkey,
    typename tvalue>
AVL_tree<tkey, tvalue>::AVL_tree(
    std::function<int(tkey const &, tkey const &)> comparer,
    allocator *allocator,
    logger *logger,
    typename binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy insertion_strategy,
    typename binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy disposal_strategy):
    binary_search_tree<tkey, tvalue>(
        new (allocator_guardant::allocate_with_guard_static(allocator,
            sizeof(AVL_tree<tkey, tvalue>::insertion_template_method)))
            AVL_tree<tkey, tvalue>::insertion_template_method(this, insertion_strategy),
        new (allocator_guardant::allocate_with_guard_static(allocator,
            sizeof(AVL_tree<tkey, tvalue>::obtaining_template_method)))
            AVL_tree<tkey, tvalue>::obtaining_template_method(this),
        new (allocator_guardant::allocate_with_guard_static(allocator,
            sizeof(AVL_tree<tkey, tvalue>::disposal_template_method)))
            AVL_tree<tkey, tvalue>::disposal_template_method(this, disposal_strategy),
        comparer,
        allocator,
        logger)
{

}

template<typename tkey, typename tvalue>
AVL_tree<tkey, tvalue>::AVL_tree(
    allocator *allocator, logger *logger,
    typename binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy insertion_strategy,
    typename binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy disposal_strategy):
    AVL_tree(std::less<tkey>(), allocator, logger, insertion_strategy, disposal_strategy)
{
    //throw not_implemented("template<typename tkey, typename tvalue> AVL_tree<tkey, tvalue>::AVL_tree(allocator *, logger *, typename binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy, typename binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy)", "your code should be here...");
}

template<
    typename tkey,
    typename tvalue>
AVL_tree<tkey, tvalue>::~AVL_tree() noexcept
{
    //throw not_implemented("template<typename tkey, typename tvalue> AVL_tree<tkey, tvalue>::~AVL_tree() noexcept", "your code should be here...");
}

template<
    typename tkey,
    typename tvalue>
AVL_tree<tkey, tvalue>::AVL_tree(
    AVL_tree<tkey, tvalue> const &other):
    binary_search_tree<tkey, tvalue>(other)
{
    //throw not_implemented("template<typename tkey, typename tvalue> AVL_tree<tkey, tvalue>::AVL_tree(AVL_tree<tkey, tvalue> const &)", "your code should be here...");
}

template<
    typename tkey,
    typename tvalue>
AVL_tree<tkey, tvalue> &AVL_tree<tkey, tvalue>::operator=(
    AVL_tree<tkey, tvalue> const &other)
{
    if (this != &other)
    {
        binary_search_tree<tkey, tvalue>::operator=(other);
    }

    return *this;
    //throw not_implemented("template<typename tkey, typename tvalue> AVL_tree<tkey, tvalue> &AVL_tree<tkey, tvalue>::operator=(AVL_tree<tkey, tvalue> const &)", "your code should be here...");
}

template<
    typename tkey,
    typename tvalue>
AVL_tree<tkey, tvalue>::AVL_tree(
    AVL_tree<tkey, tvalue> &&other) noexcept:
    binary_search_tree<tkey, tvalue>(std::forward<AVL_tree<tkey, tvalue>>(other))
{
    //throw not_implemented("template<typename tkey, typename tvalue> AVL_tree<tkey, tvalue>::AVL_tree(AVL_tree<tkey, tvalue> &&) noexcept", "your code should be here...");
}

template<
    typename tkey,
    typename tvalue>
AVL_tree<tkey, tvalue> &AVL_tree<tkey, tvalue>::operator=(
    AVL_tree<tkey, tvalue> &&other) noexcept
{
    if (this != &other)
    {
        binary_search_tree<tkey, tvalue>::operator=(std::forward<AVL_tree<tkey, tvalue>>(other));
    }

    return *this;
    //throw not_implemented("template<typename tkey, typename tvalue> AVL_tree<tkey, tvalue> &AVL_tree<tkey, tvalue>::operator=(AVL_tree<tkey, tvalue> &&) noexcept", "your code should be here...");
}

#endif //MATH_PRACTICE_AND_OPERATING_SYSTEMS_AVL_TREE_H