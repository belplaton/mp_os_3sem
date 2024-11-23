#ifndef MATH_PRACTICE_AND_OPERATING_SYSTEMS_BINARY_SEARCH_TREE_H
#define MATH_PRACTICE_AND_OPERATING_SYSTEMS_BINARY_SEARCH_TREE_H

#include <list>
#include <stack>
#include <vector>
#include <logger.h>
#include <logger_guardant.h>
#include <allocator.h>
#include <allocator_guardant.h>
#include <not_implemented.h>
#include <search_tree.h>
#include <exception>


template<
    typename tkey,
    typename tvalue>
class binary_search_tree:
    public search_tree<tkey, tvalue>
{

protected:
    
    struct node
    {
    
    public:
        
        tkey key;
        
        tvalue value;
        
        node *left_subtree;
        
        node *right_subtree;
    
    public:
        
        explicit node(
            tkey const &key,
            tvalue &&value);
        
    };

public:
    
    #pragma region iterators definition
    
    struct iterator_data
    {
    
    public:
        
        unsigned int depth;
        
        tkey key;
        
        tvalue value;
    
    public:
    
        explicit iterator_data(
            unsigned int depth,
            tkey const &key,
            tvalue const &value);
        
    };
    
    class prefix_iterator final
    {
    
    private:

        std::stack<node*> traversal_stack;
        iterator_data* current_data;

    public:
        
        explicit prefix_iterator(
            typename binary_search_tree<tkey, tvalue>::node *subtree_root);

        ~prefix_iterator();
    
    public:
        
        bool operator==(
            prefix_iterator const &other) const noexcept;
        
        bool operator!=(
            prefix_iterator const &other) const noexcept;
        
        prefix_iterator &operator++();
        
        prefix_iterator const operator++(
            int not_used);
        
        iterator_data *operator*() const;
        
    };
    
    class prefix_const_iterator final
    {

    private:

        std::stack<node*> traversal_stack;
        iterator_data* current_data;

    public:
        
        explicit prefix_const_iterator(
            typename binary_search_tree<tkey, tvalue>::node *subtree_root);
    
        ~prefix_const_iterator();

    public:
        
        bool operator==(
            prefix_const_iterator const &other) const noexcept;
        
        bool operator!=(
            prefix_const_iterator const &other) const noexcept;
        
        prefix_const_iterator &operator++();
        
        prefix_const_iterator const operator++(
            int not_used);
        
        iterator_data const *operator*() const;
        
    };
    
    class prefix_reverse_iterator final
    {
    
    private:

        std::stack<node*> traversal_stack;
        iterator_data* current_data;

    public:
        
        ~prefix_reverse_iterator();

        explicit prefix_reverse_iterator(
            typename binary_search_tree<tkey, tvalue>::node *subtree_root);
    
    public:
        
        bool operator==(
            prefix_reverse_iterator const &other) const noexcept;
        
        bool operator!=(
            prefix_reverse_iterator const &other) const noexcept;
        
        prefix_reverse_iterator &operator++();
        
        prefix_reverse_iterator const operator++(
            int not_used);
        
        iterator_data *operator*() const;
        
    };
    
    class prefix_const_reverse_iterator final
    {
    
    private:

        std::stack<node*> traversal_stack;
        iterator_data* current_data;

    public:

        ~prefix_const_reverse_iterator();
        
        explicit prefix_const_reverse_iterator(
            typename binary_search_tree<tkey, tvalue>::node *subtree_root);
    
    public:
        
        bool operator==(
            prefix_const_reverse_iterator const &other) const noexcept;
        
        bool operator!=(
            prefix_const_reverse_iterator const &other) const noexcept;
        
        prefix_const_reverse_iterator &operator++();
        
        prefix_const_reverse_iterator const operator++(
            int not_used);
        
        iterator_data const *operator*() const;
        
    };
    
    class infix_iterator final
    {

    private:

        std::stack<node*> traversal_stack;
        iterator_data* current_data;
    
    public:

        ~infix_iterator();
        
        explicit infix_iterator(
            typename binary_search_tree<tkey, tvalue>::node *subtree_root);
    
    public:
        
        bool operator==(
            infix_iterator const &other) const noexcept;
        
        bool operator!=(
            infix_iterator const &other) const noexcept;
        
        infix_iterator &operator++();
        
        infix_iterator const operator++(
            int not_used);
        
        iterator_data *operator*() const;
        
    };
    
    class infix_const_iterator final
    {

    private:

        std::stack<node*> traversal_stack;
        iterator_data* current_data;
    
    public:

        ~infix_const_iterator();
        
        explicit infix_const_iterator(
            typename binary_search_tree<tkey, tvalue>::node *subtree_root);
    
    public:
        
        bool operator==(
            infix_const_iterator const &other) const noexcept;
        
        bool operator!=(
            infix_const_iterator const &other) const noexcept;
        
        infix_const_iterator &operator++();
        
        infix_const_iterator const operator++(
            int not_used);
        
        iterator_data const *operator*() const;
        
    };
    
    class infix_reverse_iterator final
    {

    private:

        std::stack<node*> traversal_stack;
        iterator_data* current_data;
    
    public:

        ~infix_reverse_iterator();
        
        explicit infix_reverse_iterator(
            typename binary_search_tree<tkey, tvalue>::node *subtree_root);
    
    public:
        
        bool operator==(
            infix_reverse_iterator const &other) const noexcept;
        
        bool operator!=(
            infix_reverse_iterator const &other) const noexcept;
        
        infix_reverse_iterator &operator++();
        
        infix_reverse_iterator const operator++(
            int not_used);
        
        iterator_data *operator*() const;
        
    };
    
    class infix_const_reverse_iterator final
    {

    private:

        std::stack<node*> traversal_stack;
        iterator_data* current_data;
    
    public:

        ~infix_const_reverse_iterator();
        
        explicit infix_const_reverse_iterator(
            typename binary_search_tree<tkey, tvalue>::node *subtree_root);
    
    public:
        
        bool operator==(
            infix_const_reverse_iterator const &other) const noexcept;
        
        bool operator!=(
            infix_const_reverse_iterator const &other) const noexcept;
        
        infix_const_reverse_iterator &operator++();
        
        infix_const_reverse_iterator const operator++(
            int not_used);
        
        iterator_data const *operator*() const;
        
    };
    
    class postfix_iterator final
    {

    private:

        std::stack<node*> traversal_stack;
        std::stack<node*> post_order_stack;
        iterator_data* current_data;
    
    public:

        ~postfix_iterator();
        
        explicit postfix_iterator(
            typename binary_search_tree<tkey, tvalue>::node *subtree_root);
    
    public:
        
        bool operator==(
            postfix_iterator const &other) const noexcept;
        
        bool operator!=(
            postfix_iterator const &other) const noexcept;
        
        postfix_iterator &operator++();
        
        postfix_iterator const operator++(
            int not_used);
        
        iterator_data *operator*() const;
        
    };
    
    class postfix_const_iterator final
    {

    private:

        std::stack<node*> traversal_stack;
        std::stack<node*> post_order_stack;
        iterator_data* current_data;
    
    public:

        ~postfix_const_iterator();
        
        explicit postfix_const_iterator(
            typename binary_search_tree<tkey, tvalue>::node *subtree_root);
    
    public:
        
        bool operator==(
            postfix_const_iterator const &other) const noexcept;
        
        bool operator!=(
            postfix_const_iterator const &other) const noexcept;
        
        postfix_const_iterator &operator++();
        
        postfix_const_iterator const operator++(
            int not_used);
        
        iterator_data const *operator*() const;
        
    };
    
    class postfix_reverse_iterator final
    {

    private:

        std::stack<node*> traversal_stack;
        std::stack<node*> post_order_stack;
        iterator_data* current_data;
    
    public:

        ~postfix_reverse_iterator();
        
        explicit postfix_reverse_iterator(
            typename binary_search_tree<tkey, tvalue>::node *subtree_root);
    
    public:
        
        bool operator==(
            postfix_reverse_iterator const &other) const noexcept;
        
        bool operator!=(
            postfix_reverse_iterator const &other) const noexcept;
        
        postfix_reverse_iterator &operator++();
        
        postfix_reverse_iterator const operator++(
            int not_used);
        
        iterator_data *operator*() const;
        
    };
    
    class postfix_const_reverse_iterator final
    {
    
    private:

        std::stack<node*> traversal_stack;
        std::stack<node*> post_order_stack;
        iterator_data* current_data;

    public:

        ~postfix_const_reverse_iterator();
        
        explicit postfix_const_reverse_iterator(
            typename binary_search_tree<tkey, tvalue>::node *subtree_root);
    
    public:
        
        bool operator==(
            postfix_const_reverse_iterator const &other) const noexcept;
        
        bool operator!=(
            postfix_const_reverse_iterator const &other) const noexcept;
        
        postfix_const_reverse_iterator &operator++();
        
        postfix_const_reverse_iterator const operator++(
            int not_used);
        
        iterator_data const *operator*() const;
        
    };
    
    #pragma endregion iterators definition

protected:
    
    #pragma region target operations strategies definition
    
    enum class insertion_of_existent_key_attempt_strategy
    {
        update_value,
        throw_an_exception
    };
    
    enum class disposal_of_nonexistent_key_attempt_strategy
    {
        do_nothing,
        throw_an_exception
    };
    
    #pragma endregion target operations strategies definition
    
    #pragma region target operations associated exception types
    
    class insertion_of_existent_key_attempt_exception final:
        public std::logic_error
    {
    
    private:
        
        tkey _key;
    
    public:
        
        explicit insertion_of_existent_key_attempt_exception(
            tkey const &key);
        
    public:
        
        tkey const &get_key() const noexcept;
    
    };
    
    class obtaining_of_nonexistent_key_attempt_exception final:
        public std::logic_error
    {
    
    private:
        
        tkey _key;
        
    public:
        
        explicit obtaining_of_nonexistent_key_attempt_exception(
            tkey const &key);
        
    public:
        
        tkey const &get_key() const noexcept;
        
    };
    
    class disposal_of_nonexistent_key_attempt_exception final:
        public std::logic_error
    {
    
    private:
        
        tkey _key;
    
    public:
        
        explicit disposal_of_nonexistent_key_attempt_exception(
            tkey const &key);
        
    public:
        
        tkey const &get_key() const noexcept;
    
    };
    
    #pragma endregion target operations associated exception types
    
    #pragma region template methods definition
    
    class template_method_basics:
        public logger_guardant
    {
    
    protected:
    
        binary_search_tree<tkey, tvalue> *_tree;
        
    public:
    
        explicit template_method_basics(
            binary_search_tree<tkey, tvalue> *tree);
        
    protected:
        
        std::stack<node**> obtain_path(
            tkey const& key_to_obtain_path_to);

    protected:
    
        [[nodiscard]] logger *get_logger() const noexcept final;
        
    };
    
    class insertion_template_method:
        public template_method_basics,
        public allocator_guardant
    {
    
    private:

        typename binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy _insertion_strategy;

    public:
        
        explicit insertion_template_method(
            binary_search_tree<tkey, tvalue> *tree,
            typename binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy insertion_strategy);
        
    public:
        
        void insert(
            tkey const &key,
            tvalue &&value);
    
        void set_insertion_strategy(
            typename binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy insertion_strategy) noexcept;
    
    protected:
        
        inline virtual size_t obtain_node_size() const noexcept;

        inline virtual void construct_node(
            node* at,
            tkey const& key,
            tvalue&& value) const noexcept;
    
    private:
        
        [[nodiscard]] allocator *get_allocator() const noexcept final;
        
    };
    
    class obtaining_template_method:
        public template_method_basics
    {
    
    public:
        
        explicit obtaining_template_method(
            binary_search_tree<tkey, tvalue> *tree);
    
    public:
        
        tvalue const &obtain(
            tkey const &key);
    
    protected:
        
        // TODO: think about it!
        
    };
    
    class disposal_template_method:
        public template_method_basics,
        public allocator_guardant
    {

    private:

        typename binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy _disposal_strategy;
    
    public:
        
        explicit disposal_template_method(
            binary_search_tree<tkey, tvalue> *tree,
            typename binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy disposal_strategy);
        
    public:
        
        tvalue dispose(
            tkey const &key);
        
        void set_disposal_strategy(
            typename binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy disposal_strategy) noexcept;
    
    protected:
        
        // TODO: think about it!
    
    private:
        
        [[nodiscard]] allocator *get_allocator() const noexcept final;
        
    };
    
    #pragma endregion template methods definition

private:
    
    node *_root;
    
    insertion_template_method *_insertion_template;
    
    obtaining_template_method *_obtaining_template;
    
    disposal_template_method *_disposal_template;

protected:
    
    explicit binary_search_tree(
        typename binary_search_tree<tkey, tvalue>::insertion_template_method *insertion_template,
        typename binary_search_tree<tkey, tvalue>::obtaining_template_method *obtaining_template,
        typename binary_search_tree<tkey, tvalue>::disposal_template_method *disposal_template,
        std::function<int(tkey const &, tkey const &)>,
        allocator *allocator,
        logger *logger);

public:
    
    explicit binary_search_tree(
        std::function<int(tkey const &, tkey const &)> comparer = std::less<tkey>(),
        allocator *allocator = nullptr,
        logger *logger = nullptr,
        typename binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy insertion_strategy = binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy::throw_an_exception,
        typename binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy disposal_strategy = binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy::throw_an_exception);

public:
    
    binary_search_tree(
        binary_search_tree<tkey, tvalue> const &other);
    
    binary_search_tree(
        binary_search_tree<tkey, tvalue> &&other) noexcept;
    
    binary_search_tree<tkey, tvalue> &operator=(
        binary_search_tree<tkey, tvalue> const &other);
    
    binary_search_tree<tkey, tvalue> &operator=(
        binary_search_tree<tkey, tvalue> &&other) noexcept;
    
    ~binary_search_tree() override;

public:
        
    void insert(
        tkey const &key,
        tvalue &&value) final;
    
    tvalue const &obtain(
        tkey const &key) final;
    
    std::vector<typename associative_container<tkey, tvalue>::key_value_pair> obtain_between(
        tkey const &lower_bound,
        tkey const &upper_bound,
        bool lower_bound_inclusive,
        bool upper_bound_inclusive) final;
    
    tvalue dispose(
        tkey const &key) final;
    
public:
    
    void set_insertion_strategy(
        typename binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy insertion_strategy) noexcept;
    
    void set_removal_strategy(
        typename binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy disposal_strategy) noexcept;

public:
    
    #pragma region iterators requests definition
    
    prefix_iterator begin_prefix() const noexcept;
    
    prefix_iterator end_prefix() const noexcept;
    
    prefix_const_iterator cbegin_prefix() const noexcept;
    
    prefix_const_iterator cend_prefix() const noexcept;
    
    prefix_reverse_iterator rbegin_prefix() const noexcept;
    
    prefix_reverse_iterator rend_prefix() const noexcept;
    
    prefix_const_reverse_iterator crbegin_prefix() const noexcept;
    
    prefix_const_reverse_iterator crend_prefix() const noexcept;
    
    infix_iterator begin_infix() const noexcept;
    
    infix_iterator end_infix() const noexcept;
    
    infix_const_iterator cbegin_infix() const noexcept;
    
    infix_const_iterator cend_infix() const noexcept;
    
    infix_reverse_iterator rbegin_infix() const noexcept;
    
    infix_reverse_iterator rend_infix() const noexcept;
    
    infix_const_reverse_iterator crbegin_infix() const noexcept;
    
    infix_const_reverse_iterator crend_infix() const noexcept;
    
    postfix_iterator begin_postfix() const noexcept;
    
    postfix_iterator end_postfix() const noexcept;
    
    postfix_const_iterator cbegin_postfix() const noexcept;
    
    postfix_const_iterator cend_postfix() const noexcept;
    
    postfix_reverse_iterator rbegin_postfix() const noexcept;
    
    postfix_reverse_iterator rend_postfix() const noexcept;
    
    postfix_const_reverse_iterator crbegin_postfix() const noexcept;
    
    postfix_const_reverse_iterator crend_postfix() const noexcept;
    
    #pragma endregion iterators requests definition

protected:
    
    #pragma region subtree rotations definition
    
    void small_left_rotation(
        typename binary_search_tree<tkey, tvalue>::node *&subtree_root,
        bool validate = true) const;
    
    void small_right_rotation(
        typename binary_search_tree<tkey, tvalue>::node *&subtree_root,
        bool validate = true) const;
    
    void big_left_rotation(
        typename binary_search_tree<tkey, tvalue>::node *&subtree_root,
        bool validate = true) const;
    
    void big_right_rotation(
        typename binary_search_tree<tkey, tvalue>::node *&subtree_root,
        bool validate = true) const;
    
    void double_left_rotation(
        typename binary_search_tree<tkey, tvalue>::node *&subtree_root,
        bool at_grandparent_first,
        bool validate = true) const;
    
    void double_right_rotation(
        typename binary_search_tree<tkey, tvalue>::node *&subtree_root,
        bool at_grandparent_first,
        bool validate = true) const;
    
    #pragma endregion subtree rotations definition
    
};

#pragma region binary_search_tree<tkey, tvalue>::node methods implementation

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue>::node::node(
    tkey const &key,
    tvalue &&value) :
    key(key),
    value(std::forward<tvalue &&>(value)),
    left_subtree(nullptr),
    right_subtree(nullptr)
{
     //throw not_implemented("template<typename tkey, typename tvalue> binary_search_tree<tkey, tvalue>::node::node(tkey const &, tvalue &&)", "your code should be here...");
}

#pragma endregion binary_search_tree<tkey, tvalue>::node methods implementation

#pragma region iterators implementation

#pragma region iterator data implementation

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue>::iterator_data::iterator_data(
    unsigned int depth,
    tkey const &key,
    tvalue const &value):
    depth(depth),
    key(key),
    value(value)
{
    //throw not_implemented("template<typename tkey, typename tvalue> binary_search_tree<tkey, tvalue>::iterator_data::iterator_data(unsigned int, tkey const &, tvalue const &)", "your code should be here...");
}

#pragma endregion iterator data implementation

#pragma region prefix_iterator implementation

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue>::prefix_iterator::prefix_iterator(
    typename binary_search_tree<tkey, tvalue>::node *subtree_root) : current_data(nullptr), traversal_stack()
{
    if (subtree_root)
    {
        traversal_stack.push(subtree_root);
        current_data = new iterator_data(0, subtree_root->key, subtree_root->value);
    }

    //throw not_implemented("template<typename tkey, typename tvalue> binary_search_tree<tkey, tvalue>::prefix_iterator::prefix_iterator(typename binary_search_tree<tkey, tvalue>::node *)", "your code should be here...");
}

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue>::prefix_iterator::~prefix_iterator()
{
    delete current_data;
}

template<
    typename tkey,
    typename tvalue>
bool binary_search_tree<tkey, tvalue>::prefix_iterator::operator==(
    typename binary_search_tree<tkey, tvalue>::prefix_iterator const &other) const noexcept
{
    if (traversal_stack.empty() && other.traversal_stack.empty()) {
        return true;
    }
    if (traversal_stack.empty() || other.traversal_stack.empty()) {
        return false;
    }

    return traversal_stack.top() == other.traversal_stack.top();
    //throw not_implemented("template<typename tkey, typename tvalue> bool binary_search_tree<tkey, tvalue>::prefix_iterator::operator==(typename binary_search_tree<tkey, tvalue>::prefix_iterator const &) const noexcept", "your code should be here...");
}

template<
    typename tkey,
    typename tvalue>
bool binary_search_tree<tkey, tvalue>::prefix_iterator::operator!=(
    typename binary_search_tree<tkey, tvalue>::prefix_iterator const &other) const noexcept
{
    return !(*this == other);
    //throw not_implemented("template<typename tkey, typename tvalue> bool binary_search_tree<tkey, tvalue>::prefix_iterator::operator!=(typename binary_search_tree<tkey, tvalue>::prefix_iterator const &) const noexcept", "your code should be here...");
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::prefix_iterator &binary_search_tree<tkey, tvalue>::prefix_iterator::operator++()
{
    if (traversal_stack.empty())
    {
        delete current_data;
        current_data = nullptr;
        return *this;
    }

    auto* current = traversal_stack.top();
    traversal_stack.pop();

    if (current->right_subtree)
    {
        traversal_stack.push(current->right_subtree);
    }
    if (current->left_subtree)
    {
        traversal_stack.push(current->left_subtree);
    }

    if (!traversal_stack.empty())
    {
        auto* next_node = traversal_stack.top();
        delete current_data;
        current_data = new iterator_data(traversal_stack.size() - 1, next_node->key, next_node->value);
    }
    else
    {
        delete current_data;
        current_data = nullptr;
    }

    return *this;

    //throw not_implemented("template<typename tkey, typename tvalue> typename binary_search_tree<tkey, tvalue>::prefix_iterator &binary_search_tree<tkey, tvalue>::prefix_iterator::operator++()", "your code should be here...");
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::prefix_iterator const binary_search_tree<tkey, tvalue>::prefix_iterator::operator++(
    int not_used)
{
    prefix_iterator temp = *this;
    ++(*this);
    return temp;
    //throw not_implemented("template<typename tkey, typename tvalue> typename binary_search_tree<tkey, tvalue>::prefix_iterator const binary_search_tree<tkey, tvalue>::prefix_iterator::operator++(int)", "your code should be here...");
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::iterator_data *binary_search_tree<tkey, tvalue>::prefix_iterator::operator*() const
{
    return current_data;
    //throw not_implemented("template<typename tkey, typename tvalue> typename binary_search_tree<tkey, tvalue>::iterator_data *binary_search_tree<tkey, tvalue>::prefix_iterator::operator*() const", "your code should be here...");
}

#pragma endregion prefix_iterator implementation

#pragma region prefix_const_iterator implementation

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue>::prefix_const_iterator::prefix_const_iterator(
    typename binary_search_tree<tkey, tvalue>::node *subtree_root) : current_data(nullptr), traversal_stack()
{
    if (subtree_root) {
        traversal_stack.push(subtree_root);
        current_data = new iterator_data(traversal_stack.size() - 1, subtree_root->key, subtree_root->value);
    }

    //throw not_implemented("template<typename tkey, typename tvalue> binary_search_tree<tkey, tvalue>::prefix_const_iterator::prefix_const_iterator(typename binary_search_tree<tkey, tvalue>::node *)", "your code should be here...");
}

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue>::prefix_const_iterator::~prefix_const_iterator()
{
    delete current_data;
}

template<
    typename tkey,
    typename tvalue>
bool binary_search_tree<tkey, tvalue>::prefix_const_iterator::operator==(
    typename binary_search_tree<tkey, tvalue>::prefix_const_iterator const &other) const noexcept
{
    if (traversal_stack.empty() && other.traversal_stack.empty())
    {
        return true;
    }
    if (traversal_stack.empty() || other.traversal_stack.empty())
    {
        return false;
    }

    return traversal_stack.top() == other.traversal_stack.top();
    //throw not_implemented("template<typename tkey, typename tvalue> bool binary_search_tree<tkey, tvalue>::prefix_const_iterator::operator==(typename binary_search_tree<tkey, tvalue>::prefix_const_iterator const &) const noexcept", "your code should be here...");
}

template<
    typename tkey,
    typename tvalue>
bool binary_search_tree<tkey, tvalue>::prefix_const_iterator::operator!=(
    typename binary_search_tree<tkey, tvalue>::prefix_const_iterator const &other) const noexcept
{
    return !(*this == other);
    //throw not_implemented("template<typename tkey, typename tvalue> bool binary_search_tree<tkey, tvalue>::prefix_const_iterator::operator!=(typename binary_search_tree<tkey, tvalue>::prefix_const_iterator const &) const noexcept", "your code should be here...");
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::prefix_const_iterator &binary_search_tree<tkey, tvalue>::prefix_const_iterator::operator++()
{
    if (traversal_stack.empty())
    {
        delete current_data;
        current_data = nullptr;
        return *this;
    }

    node* current = traversal_stack.top();
    traversal_stack.pop();

    if (current->right_subtree)
    {
        traversal_stack.push(current->right_subtree);
    }
    if (current->left_subtree)
    {
        traversal_stack.push(current->left_subtree);
    }

    if (!traversal_stack.empty())
    {
        node* next_node = traversal_stack.top();
        delete current_data;
        current_data = new iterator_data(traversal_stack.size() - 1, next_node->key, next_node->value);
    }
    else {
        delete current_data;
        current_data = nullptr;
    }

    return *this;
    //throw not_implemented("template<typename tkey, typename tvalue> typename binary_search_tree<tkey, tvalue>::prefix_const_iterator &binary_search_tree<tkey, tvalue>::prefix_const_iterator::operator++()", "your code should be here...");
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::prefix_const_iterator const binary_search_tree<tkey, tvalue>::prefix_const_iterator::operator++(
    int not_used)
{
    prefix_const_iterator temp = *this;
    ++(*this);
    return temp;
    //throw not_implemented("template<typename tkey, typename tvalue> typename binary_search_tree<tkey, tvalue>::prefix_const_iterator const binary_search_tree<tkey, tvalue>::prefix_const_iterator::operator++(int)", "your code should be here...");
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::iterator_data const *binary_search_tree<tkey, tvalue>::prefix_const_iterator::operator*() const
{
    return current_data
    //throw not_implemented("template<typename tkey, typename tvalue> typename binary_search_tree<tkey, tvalue>::iterator_data const *binary_search_tree<tkey, tvalue>::prefix_const_iterator::operator*() const", "your code should be here...");
}

#pragma endregion prefix_const_iterator implementation

#pragma region prefix_reverse_iterator implementation

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue>::prefix_reverse_iterator::prefix_reverse_iterator(
    typename binary_search_tree<tkey, tvalue>::node *subtree_root) : current_data(nullptr), traversal_stack()
{
    if (subtree_root)
    {
        traversal_stack.push(subtree_root);
        current_data = new iterator_data(traversal_stack.size() - 1, subtree_root->key, subtree_root->value);
    }

    //throw not_implemented("template<typename tkey, typename tvalue> binary_search_tree<tkey, tvalue>::prefix_reverse_iterator::prefix_reverse_iterator(typename binary_search_tree<tkey, tvalue>::node *)", "your code should be here...");
}

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue>::prefix_reverse_iterator::~prefix_reverse_iterator()
{
    delete current_data;
    current_data = nullptr;
}

template<
    typename tkey,
    typename tvalue>
bool binary_search_tree<tkey, tvalue>::prefix_reverse_iterator::operator==(
    typename binary_search_tree<tkey, tvalue>::prefix_reverse_iterator const &other) const noexcept
{
    if (traversal_stack.empty() && other.traversal_stack.empty())
    {
        return true;
    }
    if (traversal_stack.empty() || other.traversal_stack.empty())
    {
        return false;
    }

    return traversal_stack.top() == other.traversal_stack.top();
    //throw not_implemented("template<typename tkey, typename tvalue> bool binary_search_tree<tkey, tvalue>::prefix_reverse_iterator::operator==(typename binary_search_tree<tkey, tvalue>::prefix_reverse_iterator const &) const noexcept", "your code should be here...");
}

template<
    typename tkey,
    typename tvalue>
bool binary_search_tree<tkey, tvalue>::prefix_reverse_iterator::operator!=(
    typename binary_search_tree<tkey, tvalue>::prefix_reverse_iterator const &other) const noexcept
{
    return !(*this == other);
    //throw not_implemented("template<typename tkey, typename tvalue> bool binary_search_tree<tkey, tvalue>::prefix_reverse_iterator::operator!=(typename binary_search_tree<tkey, tvalue>::prefix_reverse_iterator const &) const noexcept", "your code should be here...");
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::prefix_reverse_iterator &binary_search_tree<tkey, tvalue>::prefix_reverse_iterator::operator++()
{
    if (traversal_stack.empty())
    {
        delete current_data;
        current_data = nullptr;
        return *this;
    }

    node* current = traversal_stack.top();
    traversal_stack.pop();

    if (current->left_subtree)
    {
        traversal_stack.push(current->left_subtree);
    }
    if (current->right_subtree)
    {
        traversal_stack.push(current->right_subtree);
    }

    if (!traversal_stack.empty())
    {
        node* next_node = traversal_stack.top();
        delete current_data;
        current_data = new iterator_data(traversal_stack.size() - 1, next_node->key, next_node->value);
    }
    else
    {
        delete current_data;
        current_data = nullptr;
    }

    return *this;
    //throw not_implemented("template<typename tkey, typename tvalue> typename binary_search_tree<tkey, tvalue>::prefix_reverse_iterator &binary_search_tree<tkey, tvalue>::prefix_reverse_iterator::operator++()", "your code should be here...");
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::prefix_reverse_iterator const binary_search_tree<tkey, tvalue>::prefix_reverse_iterator::operator++(
    int not_used)
{
    prefix_reverse_iterator temp = *this;
    ++(*this);
    return temp;
    //throw not_implemented("template<typename tkey, typename tvalue> typename binary_search_tree<tkey, tvalue>::prefix_reverse_iterator const binary_search_tree<tkey, tvalue>::prefix_reverse_iterator::operator++(int)", "your code should be here...");
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::iterator_data *binary_search_tree<tkey, tvalue>::prefix_reverse_iterator::operator*() const
{
    return current_data;
    //throw not_implemented("template<typename tkey, typename tvalue> typename binary_search_tree<tkey, tvalue>::iterator_data *binary_search_tree<tkey, tvalue>::prefix_reverse_iterator::operator*() const", "your code should be here...");
}

#pragma endregion prefix_reverse_iterator implementation

#pragma region prefix_const_reverse_iterator implementation

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue>::prefix_const_reverse_iterator::prefix_const_reverse_iterator(
    typename binary_search_tree<tkey, tvalue>::node *subtree_root) : current_data(nullptr), traversal_stack()
{
    if (subtree_root)
    {
        traversal_stack.push(subtree_root);
        current_data = new iterator_data(traversal_stack.size() - 1, subtree_root->key, subtree_root->value);
    }
    //throw not_implemented("template<typename tkey, typename tvalue> binary_search_tree<tkey, tvalue>::prefix_const_reverse_iterator::prefix_const_reverse_iterator(typename binary_search_tree<tkey, tvalue>::node *)", "your code should be here...");
}

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue>::prefix_const_reverse_iterator::~prefix_const_reverse_iterator()
{
    delete current_data;
    current_data = nullptr;
}

template<
    typename tkey,
    typename tvalue>
bool binary_search_tree<tkey, tvalue>::prefix_const_reverse_iterator::operator==(
    typename binary_search_tree<tkey, tvalue>::prefix_const_reverse_iterator const &other) const noexcept
{
    if (traversal_stack.empty() && other.traversal_stack.empty())
    {
        return true;
    }
    if (traversal_stack.empty() || other.traversal_stack.empty())
    {
        return false;
    }

    return traversal_stack.top() == other.traversal_stack.top();
    //throw not_implemented("template<typename tkey, typename tvalue> bool binary_search_tree<tkey, tvalue>::prefix_const_reverse_iterator::operator==(typename binary_search_tree<tkey, tvalue>::prefix_const_reverse_iterator const &) const noexcept", "your code should be here...");
}

template<
    typename tkey,
    typename tvalue>
bool binary_search_tree<tkey, tvalue>::prefix_const_reverse_iterator::operator!=(
    typename binary_search_tree<tkey, tvalue>::prefix_const_reverse_iterator const &other) const noexcept
{
    return !(*this == other);
    //throw not_implemented("template<typename tkey, typename tvalue> bool binary_search_tree<tkey, tvalue>::prefix_const_reverse_iterator::operator!=(typename binary_search_tree<tkey, tvalue>::prefix_const_reverse_iterator const &) const noexcept", "your code should be here...");
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::prefix_const_reverse_iterator &binary_search_tree<tkey, tvalue>::prefix_const_reverse_iterator::operator++()
{
    if (traversal_stack.empty())
    {
        delete current_data;
        current_data = nullptr;
        return *this;
    }

    node* current = traversal_stack.top();
    traversal_stack.pop();

    if (current->left_subtree)
    {
        traversal_stack.push(current->left_subtree);
    }
    if (current->right_subtree)
    {
        traversal_stack.push(current->right_subtree);
    }

    if (!traversal_stack.empty())
    {
        node* next_node = traversal_stack.top();
        delete current_data;
        current_data = new iterator_data(traversal_stack.size() - 1, next_node->key, next_node->value);
    }
    else
    {
        delete current_data;
        current_data = nullptr;
    }

    return *this;
    //throw not_implemented("template<typename tkey, typename tvalue> typename binary_search_tree<tkey, tvalue>::prefix_const_reverse_iterator &binary_search_tree<tkey, tvalue>::prefix_const_reverse_iterator::operator++()", "your code should be here...");
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::prefix_const_reverse_iterator const binary_search_tree<tkey, tvalue>::prefix_const_reverse_iterator::operator++(
    int not_used)
{
    prefix_const_reverse_iterator temp = *this;
    ++(*this);
    return temp;
    //throw not_implemented("template<typename tkey, typename tvalue> typename binary_search_tree<tkey, tvalue>::prefix_const_reverse_iterator const binary_search_tree<tkey, tvalue>::prefix_const_reverse_iterator::operator++(int)", "your code should be here...");
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::iterator_data const *binary_search_tree<tkey, tvalue>::prefix_const_reverse_iterator::operator*() const
{
    return current_data;
    //throw not_implemented("template<typename tkey, typename tvalue> typename binary_search_tree<tkey, tvalue>::iterator_data const *binary_search_tree<tkey, tvalue>::prefix_const_reverse_iterator::operator*() const", "your code should be here...");
}

#pragma endregion prefix_const_reverse_iterator implementation

#pragma region infix_iterator implementation

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue>::infix_iterator::infix_iterator(
    typename binary_search_tree<tkey, tvalue>::node *subtree_root) : current_data(nullptr), traversal_stack()
{
    if (subtree_root)
    {
        auto current = subtree_root;
        while (current)
        {
            traversal_stack.push(current);
            if (current->left_subtree == nullptr)
            {
                current_data = new iterator_data(traversal_stack.size(), current->key, current->value);
            }

            current = current->left_subtree;
        }
    }

    //throw not_implemented("template<typename tkey, typename tvalue> binary_search_tree<tkey, tvalue>::infix_iterator::infix_iterator(typename binary_search_tree<tkey, tvalue>::node *)", "your code should be here...");
}

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue>::infix_iterator::~infix_iterator()
{
    delete current_data;
}

template<
    typename tkey,
    typename tvalue>
bool binary_search_tree<tkey, tvalue>::infix_iterator::operator==(
    typename binary_search_tree<tkey, tvalue>::infix_iterator const &other) const noexcept
{
    if (traversal_stack.empty() && other.traversal_stack.empty())
    {
        return true;
    }
    if (traversal_stack.empty() || other.traversal_stack.empty())
    {
        return false;
    }

    return traversal_stack.top() == other.traversal_stack.top();
    //throw not_implemented("template<typename tkey, typename tvalue> bool binary_search_tree<tkey, tvalue>::infix_iterator::operator==(typename binary_search_tree<tkey, tvalue>::infix_iterator const &) const noexcept", "your code should be here...");
}

template<
    typename tkey,
    typename tvalue>
bool binary_search_tree<tkey, tvalue>::infix_iterator::operator!=(
    typename binary_search_tree<tkey, tvalue>::infix_iterator const &other) const noexcept
{
    return !(*this == other);
    //throw not_implemented("template<typename tkey, typename tvalue> bool binary_search_tree<tkey, tvalue>::infix_iterator::operator!=(typename binary_search_tree<tkey, tvalue>::infix_iterator const &) const noexcept", "your code should be here...");
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::infix_iterator &binary_search_tree<tkey, tvalue>::infix_iterator::operator++()
{
    if (traversal_stack.empty())
    {
        delete current_data;
        current_data = nullptr;
        return *this;
    }

    auto* current = traversal_stack.top();
    traversal_stack.pop();

    if (current->right_subtree)
    {
        current = current->right_subtree;
        while (current)
        {
            traversal_stack.push(current);
            current = current->left_subtree;
        }
    }

    if (!traversal_stack.empty())
    {
        current = traversal_stack.top();
        traversal_stack.pop();
        delete current_data;
        current_data = new iterator_data(traversal_stack.size(), current->key, current->value);
    }
    else
    {
        current = nullptr;
        delete current_data;
        current_data = nullptr;
    }

    return *this;
    //throw not_implemented("template<typename tkey, typename tvalue> typename binary_search_tree<tkey, tvalue>::infix_iterator &binary_search_tree<tkey, tvalue>::infix_iterator::operator++()", "your code should be here...");
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::infix_iterator const binary_search_tree<tkey, tvalue>::infix_iterator::operator++(
    int not_used)
{
    infix_iterator temp = *this;
    ++(*this);
    return temp;
    //throw not_implemented("template<typename tkey, typename tvalue> typename binary_search_tree<tkey, tvalue>::infix_iterator const binary_search_tree<tkey, tvalue>::infix_iterator::operator++(int)", "your code should be here...");
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::iterator_data *binary_search_tree<tkey, tvalue>::infix_iterator::operator*() const
{
    return current_data;
    //throw not_implemented("template<typename tkey, typename tvalue> typename binary_search_tree<tkey, tvalue>::iterator_data *binary_search_tree<tkey, tvalue>::infix_iterator::operator*() const", "your code should be here...");
}

#pragma endregion infix_iterator implementation

#pragma region infix_const_iterator implementation

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue>::infix_const_iterator::infix_const_iterator(
    typename binary_search_tree<tkey, tvalue>::node *subtree_root) : current_data(nullptr), traversal_stack()
{
    if (subtree_root)
    {
        auto current = subtree_root;
        while (current)
        {
            traversal_stack.push(current);
            if (current->left_subtree == nullptr)
            {
                current_data = new iterator_data(traversal_stack.size(), current->key, current->value);
            }

            current = current->left_subtree;
        }
    }
    //throw not_implemented("template<typename tkey, typename tvalue> binary_search_tree<tkey, tvalue>::infix_const_iterator::infix_const_iterator(typename binary_search_tree<tkey, tvalue>::node *)", "your code should be here...");
}

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue>::infix_const_iterator::~infix_const_iterator()
{
    delete current_data;
}

template<
    typename tkey,
    typename tvalue>
bool binary_search_tree<tkey, tvalue>::infix_const_iterator::operator==(
    typename binary_search_tree<tkey, tvalue>::infix_const_iterator const &other) const noexcept
{
    if (traversal_stack.empty() && other.traversal_stack.empty())
    {
        return true;
    }
    if (traversal_stack.empty() || other.traversal_stack.empty())
    {
        return false;
    }

    return traversal_stack.top() == other.traversal_stack.top();
    //throw not_implemented("template<typename tkey, typename tvalue> bool binary_search_tree<tkey, tvalue>::infix_const_iterator::operator==(typename binary_search_tree<tkey, tvalue>::infix_const_iterator const &) const noexcept", "your code should be here...");
}

template<
    typename tkey,
    typename tvalue>
bool binary_search_tree<tkey, tvalue>::infix_const_iterator::operator!=(
    typename binary_search_tree<tkey, tvalue>::infix_const_iterator const &other) const noexcept
{
    return !(this == other);
    //throw not_implemented("template<typename tkey, typename tvalue> bool binary_search_tree<tkey, tvalue>::infix_const_iterator::operator!=(typename binary_search_tree<tkey, tvalue>::infix_const_iterator const &) const noexcept", "your code should be here...");
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::infix_const_iterator &binary_search_tree<tkey, tvalue>::infix_const_iterator::operator++()
{
    if (traversal_stack.empty())
    {
        delete current_data;
        current_data = nullptr;
        return *this;
    }

    auto* current = traversal_stack.top();
    traversal_stack.pop();

    if (current->right_subtree)
    {
        current = current->right_subtree;
        while (current)
        {
            traversal_stack.push(current);
            current = current->left_subtree;
        }
    }

    if (!traversal_stack.empty())
    {
        current = traversal_stack.top();
        traversal_stack.pop();
        delete current_data;
        current_data = new iterator_data(traversal_stack.size(), current->key, current->value);
    }
    else
    {
        current = nullptr;
        delete current_data;
        current_data = nullptr;
    }

    return *this;
    //throw not_implemented("template<typename tkey, typename tvalue> typename binary_search_tree<tkey, tvalue>::infix_const_iterator &binary_search_tree<tkey, tvalue>::infix_const_iterator::operator++()", "your code should be here...");
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::infix_const_iterator const binary_search_tree<tkey, tvalue>::infix_const_iterator::operator++(
    int not_used)
{
    auto temp = *this;
    ++(*this);
    return temp;
    //throw not_implemented("template<typename tkey, typename tvalue> typename binary_search_tree<tkey, tvalue>::infix_const_iterator const binary_search_tree<tkey, tvalue>::infix_const_iterator::operator++(int)", "your code should be here...");
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::iterator_data const *binary_search_tree<tkey, tvalue>::infix_const_iterator::operator*() const
{
    return current_data;
    //throw not_implemented("template<typename tkey, typename tvalue> typename binary_search_tree<tkey, tvalue>::iterator_data const *binary_search_tree<tkey, tvalue>::infix_const_iterator::operator*() const", "your code should be here...");
}

#pragma endregion infix_const_iterator implementation

#pragma region infix_reverse_iterator implementation

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue>::infix_reverse_iterator::infix_reverse_iterator(
    typename binary_search_tree<tkey, tvalue>::node *subtree_root) : current_data(nullptr), traversal_stack()
{
    if (subtree_root)
    {
        auto current = subtree_root;
        while (current)
        {
            traversal_stack.push(current);
            if (current->right_subtree == nullptr)
            {
                current_data = new iterator_data(traversal_stack.size(), current->key, current->value);
            }

            current = current->right_subtree;
        }
    }

    //throw not_implemented("template<typename tkey, typename tvalue> binary_search_tree<tkey, tvalue>::infix_reverse_iterator::infix_reverse_iterator(typename binary_search_tree<tkey, tvalue>::node *)", "your code should be here...");
}

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue>::infix_reverse_iterator::~infix_reverse_iterator()
{
    delete current_data;
}

template<
    typename tkey,
    typename tvalue>
bool binary_search_tree<tkey, tvalue>::infix_reverse_iterator::operator==(
    typename binary_search_tree<tkey, tvalue>::infix_reverse_iterator const &other) const noexcept
{
    if (traversal_stack.empty() && other.traversal_stack.empty())
    {
        return true;
    }
    if (traversal_stack.empty() || other.traversal_stack.empty())
    {
        return false;
    }

    return traversal_stack.top() == other.traversal_stack.top();
    //throw not_implemented("template<typename tkey, typename tvalue> bool binary_search_tree<tkey, tvalue>::infix_reverse_iterator::operator==(typename binary_search_tree<tkey, tvalue>::infix_reverse_iterator const &) const noexcept", "your code should be here...");
}

template<
    typename tkey,
    typename tvalue>
bool binary_search_tree<tkey, tvalue>::infix_reverse_iterator::operator!=(
    typename binary_search_tree<tkey, tvalue>::infix_reverse_iterator const &other) const noexcept
{
    return !(*this == other);
    //throw not_implemented("template<typename tkey, typename tvalue> bool binary_search_tree<tkey, tvalue>::infix_reverse_iterator::operator!=(typename binary_search_tree<tkey, tvalue>::infix_reverse_iterator const &) const noexcept", "your code should be here...");
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::infix_reverse_iterator &binary_search_tree<tkey, tvalue>::infix_reverse_iterator::operator++()
{
    if (traversal_stack.empty())
    {
        delete current_data;
        current_data = nullptr;
        return *this;
    }

    auto* current = traversal_stack.top();
    traversal_stack.pop();

    if (current->left_subtree)
    {
        current = current->left_subtree;
        while (current)
        {
            traversal_stack.push(current);
            current = current->right_subtree;
        }
    }

    if (!traversal_stack.empty())
    {
        current = traversal_stack.top();
        traversal_stack.pop();
        delete current_data;
        current_data = new iterator_data(traversal_stack.size(), current->key, current->value);
    }
    else
    {
        current = nullptr;
        delete current_data;
        current_data = nullptr;
    }

    return *this;
    //throw not_implemented("template<typename tkey, typename tvalue> typename binary_search_tree<tkey, tvalue>::infix_reverse_iterator &binary_search_tree<tkey, tvalue>::infix_reverse_iterator::operator++()", "your code should be here...");
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::infix_reverse_iterator const binary_search_tree<tkey, tvalue>::infix_reverse_iterator::operator++(
    int not_used)
{
    auto temp = *this;
    ++(*this);
    return temp;
    //throw not_implemented("template<typename tkey, typename tvalue> typename binary_search_tree<tkey, tvalue>::infix_reverse_iterator const binary_search_tree<tkey, tvalue>::infix_reverse_iterator::operator++(int)", "your code should be here...");
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::iterator_data *binary_search_tree<tkey, tvalue>::infix_reverse_iterator::operator*() const
{
    return current_data;
    //throw not_implemented("template<typename tkey, typename tvalue> typename binary_search_tree<tkey, tvalue>::iterator_data *binary_search_tree<tkey, tvalue>::infix_reverse_iterator::operator*() const", "your code should be here...");
}

#pragma endregion infix_reverse_iterator implementation

#pragma region infix_const_reverse_iterator implementation

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue>::infix_const_reverse_iterator::infix_const_reverse_iterator(
    typename binary_search_tree<tkey, tvalue>::node *subtree_root) : current_data(nullptr), traversal_stack()
{
    if (subtree_root)
    {
        auto current = subtree_root;
        while (current)
        {
            traversal_stack.push(current);
            if (current->right_subtree == nullptr)
            {
                current_data = new iterator_data(traversal_stack.size(), current->key, current->value);
            }

            current = current->right_subtree;
        }
    }
    //throw not_implemented("template<typename tkey, typename tvalue> binary_search_tree<tkey, tvalue>::infix_const_reverse_iterator::infix_const_reverse_iterator(typename binary_search_tree<tkey, tvalue>::node *)", "your code should be here...");
}

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue>::infix_const_reverse_iterator::~infix_const_reverse_iterator()
{
    delete current_data;
}

template<
    typename tkey,
    typename tvalue>
bool binary_search_tree<tkey, tvalue>::infix_const_reverse_iterator::operator==(
    typename binary_search_tree<tkey, tvalue>::infix_const_reverse_iterator const &other) const noexcept
{
    if (traversal_stack.empty() && other.traversal_stack.empty())
    {
        return true;
    }
    if (traversal_stack.empty() || other.traversal_stack.empty())
    {
        return false;
    }

    return traversal_stack.top() == other.traversal_stack.top();
    //throw not_implemented("template<typename tkey, typename tvalue> bool binary_search_tree<tkey, tvalue>::infix_const_reverse_iterator::operator==(typename binary_search_tree<tkey, tvalue>::infix_const_reverse_iterator const &) const noexcept", "your code should be here...");
}

template<
    typename tkey,
    typename tvalue>
bool binary_search_tree<tkey, tvalue>::infix_const_reverse_iterator::operator!=(
    typename binary_search_tree<tkey, tvalue>::infix_const_reverse_iterator const &other) const noexcept
{
    return !(*this == current);
    //throw not_implemented("template<typename tkey, typename tvalue> bool binary_search_tree<tkey, tvalue>::infix_const_reverse_iterator::operator!=(typename binary_search_tree<tkey, tvalue>::infix_const_reverse_iterator const &) const noexcept", "your code should be here...");
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::infix_const_reverse_iterator &binary_search_tree<tkey, tvalue>::infix_const_reverse_iterator::operator++()
{
    if (traversal_stack.empty())
    {
        delete current_data;
        current_data = nullptr;
        return *this;
    }

    auto* current = traversal_stack.top();
    traversal_stack.pop();

    if (current->left_subtree)
    {
        current = current->left_subtree;
        while (current)
        {
            traversal_stack.push(current);
            current = current->right_subtree;
        }
    }

    if (!traversal_stack.empty())
    {
        current = traversal_stack.top();
        traversal_stack.pop();
        delete current_data;
        current_data = new iterator_data(traversal_stack.size(), current->key, current->value);
    }
    else
    {
        current = nullptr;
        delete current_data;
        current_data = nullptr;
    }

    return *this;
    //throw not_implemented("template<typename tkey, typename tvalue> typename binary_search_tree<tkey, tvalue>::infix_const_reverse_iterator &binary_search_tree<tkey, tvalue>::infix_const_reverse_iterator::operator++()", "your code should be here...");
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::infix_const_reverse_iterator const binary_search_tree<tkey, tvalue>::infix_const_reverse_iterator::operator++(
    int not_used)
{
    auto temp = *this;
    ++(*this);
    return temp;
    //throw not_implemented("template<typename tkey, typename tvalue> typename binary_search_tree<tkey, tvalue>::infix_const_reverse_iterator const binary_search_tree<tkey, tvalue>::infix_const_reverse_iterator::operator++(int)", "your code should be here...");
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::iterator_data const *binary_search_tree<tkey, tvalue>::infix_const_reverse_iterator::operator*() const
{
    return current_data;
    //throw not_implemented("template<typename tkey, typename tvalue> typename binary_search_tree<tkey, tvalue>::iterator_data const *binary_search_tree<tkey, tvalue>::infix_const_reverse_iterator::operator*() const", "your code should be here...");
}

#pragma endregion infix_const_reverse_iterator implementation

#pragma region postfix_iterator implementation

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue>::postfix_iterator::postfix_iterator(
    typename binary_search_tree<tkey, tvalue>::node *subtree_root) : current_data(nullptr), traversal_stack(), post_order_stack()
{
    if (subtree_root)
    {
        traversal_stack.push(subtree_root);
        while (!traversal_stack.empty())
        {
            node* current = traversal_stack.top();
            traversal_stack.pop();
            post_order_stack.push(current);
            if (current->left_subtree)
            {
                traversal_stack.push(current->left_subtree);
            }

            if (current->right_subtree)
            {
                traversal_stack.push(current->right_subtree);
            }
        }

        if (!post_order_stack.empty())
        {
            node* next_node = post_order_stack.top();
            current_data = new iterator_data(post_order_stack.size(), next_node->key, next_node->value);
        }
    }
    //throw not_implemented("template<typename tkey, typename tvalue> binary_search_tree<tkey, tvalue>::postfix_iterator::postfix_iterator(typename binary_search_tree<tkey, tvalue>::node *)", "your code should be here...");
}

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue>::postfix_iterator::~postfix_iterator()
{
    delete current_data;
}

template<
    typename tkey,
    typename tvalue>
bool binary_search_tree<tkey, tvalue>::postfix_iterator::operator==(
    typename binary_search_tree<tkey, tvalue>::postfix_iterator const &other) const noexcept
{
    if (traversal_stack.empty() && other.traversal_stack.empty())
    {
        return true;
    }
    if (traversal_stack.empty() || other.traversal_stack.empty())
    {
        return false;
    }

    return traversal_stack.top() == other.traversal_stack.top();
    //throw not_implemented("template<typename tkey, typename tvalue> bool binary_search_tree<tkey, tvalue>::postfix_iterator::operator==(typename binary_search_tree<tkey, tvalue>::postfix_iterator const &) const noexcept", "your code should be here...");
}

template<
    typename tkey,
    typename tvalue>
bool binary_search_tree<tkey, tvalue>::postfix_iterator::operator!=(
    typename binary_search_tree<tkey, tvalue>::postfix_iterator const &other) const noexcept
{
    return !(*this == current);
    //throw not_implemented("template<typename tkey, typename tvalue> bool binary_search_tree<tkey, tvalue>::postfix_iterator::operator!=(typename binary_search_tree<tkey, tvalue>::postfix_iterator const &) const noexcept", "your code should be here...");
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::postfix_iterator &binary_search_tree<tkey, tvalue>::postfix_iterator::operator++()
{
    if (post_order_stack.empty())
    {
        delete current_data;
        current_data = nullptr;
        return *this;
    }

    post_order_stack.pop();
    if (!post_order_stack.empty())
    {
        node* next_node = post_order_stack.top();
        delete current_data;
        current_data = new iterator_data(post_order_stack.size(), next_node->key, next_node->value);
    }
    else
    {
        delete current_data;
        current_data = nullptr;
    }

    return *this;
    //throw not_implemented("template<typename tkey, typename tvalue> typename binary_search_tree<tkey, tvalue>::postfix_iterator &binary_search_tree<tkey, tvalue>::postfix_iterator::operator++()", "your code should be here...");
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::postfix_iterator const binary_search_tree<tkey, tvalue>::postfix_iterator::operator++(
    int not_used)
{
    auto temp = *this;
    ++(*this);
    return temp;
    //throw not_implemented("template<typename tkey, typename tvalue> typename binary_search_tree<tkey, tvalue>::postfix_iterator const binary_search_tree<tkey, tvalue>::postfix_iterator::operator++(int)", "your code should be here...");
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::iterator_data *binary_search_tree<tkey, tvalue>::postfix_iterator::operator*() const
{
    return current_data;
    //throw not_implemented("template<typename tkey, typename tvalue> typename binary_search_tree<tkey, tvalue>::iterator_data *binary_search_tree<tkey, tvalue>::postfix_iterator::operator*() const", "your code should be here...");
}

#pragma endregion postfix_iterator implementation

#pragma region postfix_const_iterator implementation

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue>::postfix_const_iterator::postfix_const_iterator(
    typename binary_search_tree<tkey, tvalue>::node *subtree_root) : current_data(nullptr), traversal_stack(), post_order_stack()
{
    if (subtree_root)
    {
        traversal_stack.push(subtree_root);
        while (!traversal_stack.empty())
        {
            node* current = traversal_stack.top();
            traversal_stack.pop();
            post_order_stack.push(current);
            if (current->left_subtree)
            {
                traversal_stack.push(current->left_subtree);
            }

            if (current->right_subtree)
            {
                traversal_stack.push(current->right_subtree);
            }
        }

        if (!post_order_stack.empty())
        {
            node* next_node = post_order_stack.top();
            current_data = new iterator_data(post_order_stack.size(), next_node->key, next_node->value);
        }
    }
    //throw not_implemented("template<typename tkey, typename tvalue> binary_search_tree<tkey, tvalue>::postfix_const_iterator::postfix_const_iterator(typename binary_search_tree<tkey, tvalue>::node *)", "your code should be here...");
}

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue>::postfix_const_iterator::postfix_const_iterator()
{
    delete current_data;
}

template<
    typename tkey,
    typename tvalue>
bool binary_search_tree<tkey, tvalue>::postfix_const_iterator::operator==(
    typename binary_search_tree<tkey, tvalue>::postfix_const_iterator const &other) const noexcept
{
    if (traversal_stack.empty() && other.traversal_stack.empty())
    {
        return true;
    }
    if (traversal_stack.empty() || other.traversal_stack.empty())
    {
        return false;
    }

    return traversal_stack.top() == other.traversal_stack.top();
    //throw not_implemented("template<typename tkey, typename tvalue> bool binary_search_tree<tkey, tvalue>::postfix_const_iterator::operator==(typename binary_search_tree<tkey, tvalue>::postfix_const_iterator const &) const noexcept", "your code should be here...");
}

template<
    typename tkey,
    typename tvalue>
bool binary_search_tree<tkey, tvalue>::postfix_const_iterator::operator!=(
    typename binary_search_tree<tkey, tvalue>::postfix_const_iterator const &other) const noexcept
{
    return !(*this == other);
    //throw not_implemented("template<typename tkey, typename tvalue> bool binary_search_tree<tkey, tvalue>::postfix_const_iterator::operator!=(typename binary_search_tree<tkey, tvalue>::postfix_const_iterator const &) const noexcept", "your code should be here...");
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::postfix_const_iterator &binary_search_tree<tkey, tvalue>::postfix_const_iterator::operator++()
{
    throw not_implemented("template<typename tkey, typename tvalue> typename binary_search_tree<tkey, tvalue>::postfix_const_iterator &binary_search_tree<tkey, tvalue>::postfix_const_iterator::operator++()", "your code should be here...");
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::postfix_const_iterator const binary_search_tree<tkey, tvalue>::postfix_const_iterator::operator++(
    int not_used)
{
    auto temp = *this;
    ++(*this);
    return temp;
    //throw not_implemented("template<typename tkey, typename tvalue> typename binary_search_tree<tkey, tvalue>::postfix_const_iterator const binary_search_tree<tkey, tvalue>::postfix_const_iterator::operator++(int)", "your code should be here...");
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::iterator_data const *binary_search_tree<tkey, tvalue>::postfix_const_iterator::operator*() const
{
    return current_data;
    //throw not_implemented("template<typename tkey, typename tvalue> typename binary_search_tree<tkey, tvalue>::iterator_data const *binary_search_tree<tkey, tvalue>::postfix_const_iterator::operator*() const", "your code should be here...");
}

#pragma endregion postfix_const_iterator implementation

#pragma region postfix_reverse_iterator implementation

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue>::postfix_reverse_iterator::postfix_reverse_iterator(
    typename binary_search_tree<tkey, tvalue>::node *subtree_root) : current_data(nullptr), traversal_stack(), post_order_stack()
{
    if (subtree_root)
    {
        traversal_stack.push(subtree_root);
        while (!traversal_stack.empty())
        {
            node* current = traversal_stack.top();
            traversal_stack.pop();
            post_order_stack.push(current);
            if (current->right_subtree)
            {
                traversal_stack.push(current->right_subtree);
            }

            if (current->left_subtree)
            {
                traversal_stack.push(current->left_subtree);
            }
        }

        if (!post_order_stack.empty())
        {
            node* next_node = post_order_stack.top();
            current_data = new iterator_data(post_order_stack.size(), next_node->key, next_node->value);
        }
    }
    //throw not_implemented("template<typename tkey, typename tvalue> binary_search_tree<tkey, tvalue>::postfix_reverse_iterator::postfix_reverse_iterator(typename binary_search_tree<tkey, tvalue>::node *)", "your code should be here...");
}

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue>::postfix_reverse_iterator::postfix_reverse_iterator()
{
    delete current_data;
}

template<
    typename tkey,
    typename tvalue>
bool binary_search_tree<tkey, tvalue>::postfix_reverse_iterator::operator==(
    typename binary_search_tree<tkey, tvalue>::postfix_reverse_iterator const &other) const noexcept
{
    if (traversal_stack.empty() && other.traversal_stack.empty())
    {
        return true;
    }
    if (traversal_stack.empty() || other.traversal_stack.empty())
    {
        return false;
    }

    return traversal_stack.top() == other.traversal_stack.top();
    //throw not_implemented("template<typename tkey, typename tvalue> bool binary_search_tree<tkey, tvalue>::postfix_reverse_iterator::operator==(typename binary_search_tree<tkey, tvalue>::postfix_reverse_iterator const &) const noexcept", "your code should be here...");
}

template<
    typename tkey,
    typename tvalue>
bool binary_search_tree<tkey, tvalue>::postfix_reverse_iterator::operator!=(
    typename binary_search_tree<tkey, tvalue>::postfix_reverse_iterator const &other) const noexcept
{
    return !(*this == other);
    //throw not_implemented("template<typename tkey, typename tvalue> bool binary_search_tree<tkey, tvalue>::postfix_reverse_iterator::operator!=(typename binary_search_tree<tkey, tvalue>::postfix_reverse_iterator const &) const noexcept", "your code should be here...");
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::postfix_reverse_iterator &binary_search_tree<tkey, tvalue>::postfix_reverse_iterator::operator++()
{
    throw not_implemented("template<typename tkey, typename tvalue> typename binary_search_tree<tkey, tvalue>::postfix_reverse_iterator &binary_search_tree<tkey, tvalue>::postfix_reverse_iterator::operator++()", "your code should be here...");
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::postfix_reverse_iterator const binary_search_tree<tkey, tvalue>::postfix_reverse_iterator::operator++(
    int not_used)
{
    auto temp = *this;
    ++(*this);
    return temp;
    //throw not_implemented("template<typename tkey, typename tvalue> typename binary_search_tree<tkey, tvalue>::postfix_reverse_iterator const binary_search_tree<tkey, tvalue>::postfix_reverse_iterator::operator++(int)", "your code should be here...");
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::iterator_data *binary_search_tree<tkey, tvalue>::postfix_reverse_iterator::operator*() const
{
    return current_data;
    //throw not_implemented("template<typename tkey, typename tvalue> typename binary_search_tree<tkey, tvalue>::iterator_data *binary_search_tree<tkey, tvalue>::postfix_reverse_iterator::operator*() const", "your code should be here...");
}

#pragma endregion postfix_reverse_iterator implementation

#pragma region postfix_const_reverse_iterator implementation

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue>::postfix_const_reverse_iterator::postfix_const_reverse_iterator(
    typename binary_search_tree<tkey, tvalue>::node *subtree_root) : current_data(nullptr), traversal_stack(), post_order_stack()
{
    if (subtree_root)
    {
        traversal_stack.push(subtree_root);
        while (!traversal_stack.empty())
        {
            node* current = traversal_stack.top();
            traversal_stack.pop();
            post_order_stack.push(current);
            if (current->right_subtree)
            {
                traversal_stack.push(current->right_subtree);
            }

            if (current->left_subtree)
            {
                traversal_stack.push(current->left_subtree);
            }
        }

        if (!post_order_stack.empty())
        {
            node* next_node = post_order_stack.top();
            current_data = new iterator_data(post_order_stack.size(), next_node->key, next_node->value);
        }
    }
    //throw not_implemented("template<typename tkey, typename tvalue> binary_search_tree<tkey, tvalue>::postfix_const_reverse_iterator::postfix_const_reverse_iterator(typename binary_search_tree<tkey, tvalue>::node *)", "your code should be here...");
}

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue>::postfix_const_reverse_iterator::postfix_const_reverse_iterator()
{
    delete current_data;
}

template<
    typename tkey,
    typename tvalue>
bool binary_search_tree<tkey, tvalue>::postfix_const_reverse_iterator::operator==(
    typename binary_search_tree<tkey, tvalue>::postfix_const_reverse_iterator const &other) const noexcept
{
    if (traversal_stack.empty() && other.traversal_stack.empty())
    {
        return true;
    }
    if (traversal_stack.empty() || other.traversal_stack.empty())
    {
        return false;
    }

    return traversal_stack.top() == other.traversal_stack.top();
    //throw not_implemented("template<typename tkey, typename tvalue> bool binary_search_tree<tkey, tvalue>::postfix_const_reverse_iterator::operator==(typename binary_search_tree<tkey, tvalue>::postfix_const_reverse_iterator const &) const noexcept", "your code should be here...");
}

template<
    typename tkey,
    typename tvalue>
bool binary_search_tree<tkey, tvalue>::postfix_const_reverse_iterator::operator!=(
    typename binary_search_tree<tkey, tvalue>::postfix_const_reverse_iterator const &other) const noexcept
{
    return !(*this == other);
    //throw not_implemented("template<typename tkey, typename tvalue> bool binary_search_tree<tkey, tvalue>::postfix_const_reverse_iterator::operator!=(typename binary_search_tree<tkey, tvalue>::postfix_const_reverse_iterator const &) const noexcept", "your code should be here...");
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::postfix_const_reverse_iterator &binary_search_tree<tkey, tvalue>::postfix_const_reverse_iterator::operator++()
{
    throw not_implemented("template<typename tkey, typename tvalue> typename binary_search_tree<tkey, tvalue>::postfix_const_reverse_iterator &binary_search_tree<tkey, tvalue>::postfix_const_reverse_iterator::operator++()", "your code should be here...");
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::postfix_const_reverse_iterator const binary_search_tree<tkey, tvalue>::postfix_const_reverse_iterator::operator++(
    int not_used)
{
    auto temp = *this;
    ++(*this);
    return temp;
    //throw not_implemented("template<typename tkey, typename tvalue> typename binary_search_tree<tkey, tvalue>::postfix_const_reverse_iterator const binary_search_tree<tkey, tvalue>::postfix_const_reverse_iterator::operator++(int)", "your code should be here...");
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::iterator_data const *binary_search_tree<tkey, tvalue>::postfix_const_reverse_iterator::operator*() const
{
    return current_data
    //throw not_implemented("template<typename tkey, typename tvalue> typename binary_search_tree<tkey, tvalue>::iterator_data const *binary_search_tree<tkey, tvalue>::postfix_const_reverse_iterator::operator*() const", "your code should be here...");
}

#pragma endregion postfix_const_reverse_iterator implementation

#pragma endregion iterators implementation

#pragma region target operations associated exception types implementation

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_exception::insertion_of_existent_key_attempt_exception(
    tkey const &key):
    std::logic_error("Attempt to insert already existing key inside the tree.")
{

}

template<
    typename tkey,
    typename tvalue>
tkey const &binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_exception::get_key() const noexcept
{
    return _key;
}

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue>::obtaining_of_nonexistent_key_attempt_exception::obtaining_of_nonexistent_key_attempt_exception(
    tkey const &key):
    std::logic_error("Attempt to obtain a value by non-existing key from the tree.")
{

}

template<
    typename tkey,
    typename tvalue>
tkey const &binary_search_tree<tkey, tvalue>::obtaining_of_nonexistent_key_attempt_exception::get_key() const noexcept
{
    return _key;
}

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_exception::disposal_of_nonexistent_key_attempt_exception(
    tkey const &key):
    std::logic_error("Attempt to dispose a value by non-existing key from the tree.")
{

}

template<
    typename tkey,
    typename tvalue>
tkey const &binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_exception::get_key() const noexcept
{
    return _key;
}

#pragma endregion target operations associated exception types implementation

#pragma region template methods implementation

#pragma region binary_search_tree<tkey, tvalue>::template_method_basics implementation

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue>::template_method_basics::template_method_basics(
    binary_search_tree<tkey, tvalue> *tree) :
    _tree(tree)
{
    //throw not_implemented("template<typename tkey, typename tvalue>binary_search_tree<tkey, tvalue>::insertion_template_method::insertion_template_method(binary_search_tree<tkey, tvalue> *)", "your code should be here...");
}

template<
    typename tkey,
    typename tvalue>
std::stack<typename binary_search_tree<tkey, tvalue>::node**> binary_search_tree<tkey, tvalue>::template_method_basics::obtain_path(
    tkey const& key_to_obtain_path_to)
{
    std::stack<typename binary_search_tree<tkey, tvalue>::node**> obtained_path;
    auto** iter = &_tree->_root;
    obtained_path.push(iter);
    while (*iter != nullptr)
    {
        auto cmp_res = _tree->_keys_comparer(key_to_obtain_path_to);
        if (cmp_res == 0)
        {
            break;
        }

        iter = cmp_res < 0
            ? &(*iter)->left_subtree
            : &(*iter)->right_subtree;

        obtained_path.push(iter);
    }

    return obtained_path;
}

template<
    typename tkey,
    typename tvalue>
[[nodiscard]] inline logger *binary_search_tree<tkey, tvalue>::template_method_basics::get_logger() const noexcept
{
    return _tree->get_logger();
    //throw not_implemented("template<typename tkey, typename tvalue> [[nodiscard]] inline logger *binary_search_tree<tkey, tvalue>::template_method_basics::get_logger() const noexcept", "your code should be here...");
}

#pragma endregion binary_search_tree<tkey, tvalue>::template_method_basics implementation

#pragma region search_tree<tkey, tvalue>::insertion_template_method implementation

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue>::insertion_template_method::insertion_template_method(
    binary_search_tree<tkey, tvalue> *tree,
    typename binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy insertion_strategy):
    binary_search_tree<tkey, tvalue>::template_method_basics::template_method_basics(tree),
    _insertion_strategy(insertion_strategy)
{
    //throw not_implemented("template<typename tkey, typename tvalue>binary_search_tree<tkey, tvalue>::insertion_template_method::insertion_template_method(binary_search_tree<tkey, tvalue> *, typename binary_search_tree<tkey, tvalue>::insertion_of_existent_key_strategy)", "your code should be here...");
}

template<
    typename tkey,
    typename tvalue>
void binary_search_tree<tkey, tvalue>::insertion_template_method::insert(
    tkey const &key,
    tvalue &&value)
{
    std::stack<typename binary_search_tree<tkey, tvalue>::node**> path_to_node_with_key =
        reinterpret_cast<typename binary_search_tree<tkey, tvalue>::template_method_basics>(this)
        .obtain_path(key);

    auto** top = path_to_node_with_key->top();
    if (top != nullptr)
    {
        switch (_insertion_strategy)
        {
        case binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy::update_value:
            (*top)->value = std::forward<tvalue&&>(value);
            break;
        case binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy::throw_an_exception:
            throw binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_exception(key);
        }
    }
    else
    {
        *top = reinterpret_cast<typename binary_search_tree<tkey, tvalue>::node*>(allocate_with_guard(obtain_node_size()));
        construct_node(*top, key, std::forward<tvalue &&>(tvalue));
        // TODO: launch balance operation
    }

    //throw not_implemented("template<typename tkey, typename tvalue> void binary_search_tree<tkey, tvalue>::insertion_template_method::insert(tkey const &, tvalue &&)", "your code should be here...");
}

template<
    typename tkey,
    typename tvalue>
void binary_search_tree<tkey, tvalue>::insertion_template_method::set_insertion_strategy(
    typename binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy insertion_strategy) noexcept
{
    _insertion_strategy = insertion_strategy;
    //throw not_implemented("template<typename tkey, typename tvalue> void binary_search_tree<tkey, tvalue>::insertion_template_method::set_insertion_strategy(typename binary_search_tree<tkey, tvalue>::insertion_of_existent_key_strategy)", "your code should be here...");
}

template<
    typename tkey,
    typename tvalue>
inline size_t binary_search_tree<tkey, tvalue>::insertion_template_method::obtain_node_size() const noexcept
{
    return sizeof(typename binary_search_tree<tkey, tvalue>::node);
}

template<
    typename tkey,
    typename tvalue>
inline void binary_search_tree<tkey, tvalue>::insertion_template_method::construct_node(
    typename binary_search_tree<tkey, tvalue>::node* at,
    tkey const& key,
    tvalue&& value) const noexcept
{
    allocator::construct(at, key, std::forward<tvalue&&>(value));
}

template<
    typename tkey,
    typename tvalue>
allocator *binary_search_tree<tkey, tvalue>::insertion_template_method::get_allocator() const noexcept
{
    return this->_tree->get_allocator();
    //throw not_implemented("template<typename tkey, typename tvalue> allocator *binary_search_tree<tkey, tvalue>::insertion_template_method::get_allocator() const noexcept", "your code should be here...");
}

#pragma endregion search_tree<tkey, tvalue>::insertion_template_method implementation

#pragma region search_tree<tkey, tvalue>::obtaining_template_method implementation

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue>::obtaining_template_method::obtaining_template_method(
    binary_search_tree<tkey, tvalue> *tree):
    binary_search_tree<tkey, tvalue>::template_method_basics::template_method_basics(tree)
{
    //throw not_implemented("template<typename tkey, typename tvalue> binary_search_tree<tkey, tvalue>::obtaining_template_method::obtaining_template_method(binary_search_tree<tkey, tvalue> *tree)", "your code should be here...");
}

template<
    typename tkey,
    typename tvalue>
tvalue const &binary_search_tree<tkey, tvalue>::obtaining_template_method::obtain(
    tkey const &key)
{
    std::stack<typename binary_search_tree<tkey, tvalue>::node**> path_to_node_with_key =
        reinterpret_cast<typename binary_search_tree<tkey, tvalue>::template_method_basics>(this)
        .obtain_path(key);

    auto** top = path_to_node_with_key.top();
    if (top == nullptr)
    {
        throw binary_search_tree<tkey, tvalue>::obtaining_of_nonexistent_key_attempt_exception(key);
    }
    else
    {
        return **top;
    }

    //throw not_implemented("template<typename tkey, typename tvalue> tvalue const &binary_search_tree<tkey, tvalue>::obtaining_template_method::obtain(tkey const &)", "your code should be here...");
}

#pragma endregion search_tree<tkey, tvalue>::obtaining_template_method implementation

#pragma region search_tree<tkey, tvalue>::disposal_template_method implementation

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue>::disposal_template_method::disposal_template_method(
    binary_search_tree<tkey, tvalue> *tree,
    typename binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy disposal_strategy):
    binary_search_tree<tkey, tvalue>::template_method_basics(tree),
    _disposal_strategy(disposal_strategy)
{
    //throw not_implemented("template<typename tkey, typename tvalue> binary_search_tree<tkey, tvalue>::disposal_template_method::disposal_template_method(binary_search_tree<tkey, tvalue> *, typename binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_strategy)", "your code should be here...");
}

template<
    typename tkey,
    typename tvalue>
tvalue binary_search_tree<tkey, tvalue>::disposal_template_method::dispose(
    tkey const &key)
{
    std::stack<typename binary_search_tree<tkey, tvalue>::node**> path_to_node_with_key =
        reinterpret_cast<typename binary_search_tree<tkey, tvalue>::template_method_basics>(this)
        .obtain_path(key);

    auto** top = path_to_node_with_key->top();
    if (top == nullptr)
    {
        switch (_disposal_strategy)
        {
        case binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy::do_nothing:
            return tvalue();
            break;
        case binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy::throw_an_exception:
            throw binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_exception(key);
        }
    }
    else
    {
        auto node_to_remove = *current;
        auto return_value = node_to_remove->value;
        if (node_to_remove->left_subtree == nullptr)
        {
            *current = node_to_remove->right_subtree;
        }
        else if (node_to_remove->right_subtree == nullptr)
        {
            *current = node_to_remove->left_subtree;
        }
        else
        {
            auto** smallest = &node_to_remove->right_subtree;
            node* parent_of_smallest = nullptr;

            while ((*smallest)->left_subtree != nullptr)
            {
                parent_of_smallest = *smallest;
                smallest = &(*smallest)->left_subtree;
            }

            auto* smallest_node = *smallest;
            if (parent_of_smallest != nullptr)
            {
                parent_of_smallest->left_subtree = smallest_node->right_subtree;
            }
            else
            {
                node_to_remove->right_subtree = smallest_node->right_subtree;
            }

            smallest_node->left_subtree = node_to_remove->left_subtree;
            smallest_node->right_subtree = node_to_remove->right_subtree;
            *current = smallest_node;
        }

        delete node_to_remove;
        return return_value;
        // TODO: launch balance operation
    }

    //throw not_implemented("template<typename tkey, typename tvalue> tvalue binary_search_tree<tkey, tvalue>::disposal_template_method::dispose(tkey const &)", "your code should be here...");
}

template<
    typename tkey,
    typename tvalue>
void binary_search_tree<tkey, tvalue>::disposal_template_method::set_disposal_strategy(
    typename binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy disposal_strategy) noexcept
{
    _disposal_strategy = disposal_strategy;
    //throw not_implemented("template<typename tkey, typename tvalue> void binary_search_tree<tkey, tvalue>::disposal_template_method::set_disposal_strategy(typename binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_strategy)", "your code should be here...");
}

template<
    typename tkey,
    typename tvalue>
[[nodiscard]] inline allocator *binary_search_tree<tkey, tvalue>::disposal_template_method::get_allocator() const noexcept
{
    return this->_tree->get_allocator();
    //throw not_implemented("template<typename tkey, typename tvalue> [[nodiscard]] inline allocator *binary_search_tree<tkey, tvalue>::disposal_template_method::get_allocator() const noexcept", "your code should be here...");
}

#pragma endregion search_tree<tkey, tvalue>::disposal_template_method implementation

#pragma endregion template methods

#pragma region construction, assignment, destruction implementation

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue>::binary_search_tree(
    typename binary_search_tree<tkey, tvalue>::insertion_template_method *insertion_template,
    typename binary_search_tree<tkey, tvalue>::obtaining_template_method *obtaining_template,
    typename binary_search_tree<tkey, tvalue>::disposal_template_method *disposal_template,
    std::function<int(tkey const &, tkey const &)> comparer,
    allocator *allocator,
    logger *logger):
    search_tree<tkey, tvalue>(comparer, logger, allocator),
    _insertion_template(insertion_template),
    _obtaining_template(obtaining_template),
    _disposal_template(disposal_template)
{
    throw not_implemented("template<typename tkey, typename tvalue> binary_search_tree<tkey, tvalue>::binary_search_tree(typename binary_search_tree<tkey, tvalue>::insertion_template_method *, typename binary_search_tree<tkey, tvalue>::obtaining_template_method *, typename binary_search_tree<tkey, tvalue>::disposal_template_method *, std::function<int(tkey const &, tkey const &)>, allocator *, logger *)", "your code should be here...");
}

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue>::binary_search_tree(
    std::function<int(tkey const &, tkey const &)> keys_comparer,
    allocator *allocator,
    logger *logger,
    typename binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy insertion_strategy,
    typename binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy disposal_strategy):
    binary_search_tree(
        new binary_search_tree<tkey, tvalue>::insertion_template_method(this, insertion_strategy),
        new binary_search_tree<tkey, tvalue>::obtaining_template_method(this),
        new binary_search_tree<tkey, tvalue>::disposal_template_method(this, disposal_strategy),
        keys_comparer,
        allocator,
        logger)
{
    throw not_implemented("template<typename tkey, typename tvalue> binary_search_tree<tkey, tvalue>::binary_search_tree(std::function<int(tkey const &, tkey const &)>, allocator *, logger *, typename binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy, typename binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy)", "your code should be here...");
}

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue>::binary_search_tree(
    binary_search_tree<tkey, tvalue> const &other)
{
    throw not_implemented("template<typename tkey, typename tvalue> binary_search_tree<tkey, tvalue>::binary_search_tree(binary_search_tree<tkey, tvalue> const &)", "your code should be here...");
}

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue>::binary_search_tree(
    binary_search_tree<tkey, tvalue> &&other) noexcept
{
    throw not_implemented("template<typename tkey, typename tvalue> binary_search_tree<tkey, tvalue>::binary_search_tree(binary_search_tree<tkey, tvalue> &&) noexcept", "your code should be here...");
}

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue> &binary_search_tree<tkey, tvalue>::operator=(
    binary_search_tree<tkey, tvalue> const &other)
{
    throw not_implemented("template<typename tkey, typename tvalue> binary_search_tree<tkey, tvalue> &binary_search_tree<tkey, tvalue>::operator=(binary_search_tree<tkey, tvalue> const &)", "your code should be here...");
}

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue> &binary_search_tree<tkey, tvalue>::operator=(
    binary_search_tree<tkey, tvalue> &&other) noexcept
{
    throw not_implemented("template<typename tkey, typename tvalue> binary_search_tree<tkey, tvalue> &binary_search_tree<tkey, tvalue>::operator=(binary_search_tree<tkey, tvalue> &&) noexcept", "your code should be here...");
}

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue>::~binary_search_tree()
{
    throw not_implemented("template<typename tkey, typename tvalue> binary_search_tree<tkey, tvalue>::~binary_search_tree()", "your code should be here...");
}

#pragma endregion construction, assignment, destruction implementation

#pragma region associative_container<tkey, tvalue> contract implementation

template<
    typename tkey,
    typename tvalue>
void binary_search_tree<tkey, tvalue>::insert(
    tkey const &key,
    tvalue &&value)
{
    _insertion_template->insert(key, std::forward(value));
}

template<
    typename tkey,
    typename tvalue>
tvalue const &binary_search_tree<tkey, tvalue>::obtain(
    tkey const &key)
{
    return _obtaining_template->obtain(key);
}

template<
    typename tkey,
    typename tvalue>
std::vector<typename associative_container<tkey, tvalue>::key_value_pair> binary_search_tree<tkey, tvalue>::obtain_between(
    tkey const &lower_bound,
    tkey const &upper_bound,
    bool lower_bound_inclusive,
    bool upper_bound_inclusive)
{
    throw not_implemented("template<typename tkey, typename tvalue> std::vector<typename associative_container<tkey, tvalue>::key_value_pair> binary_search_tree<tkey, tvalue>::obtain_between(tkey const &, tkey const &, bool, bool)", "your code should be here...");
}

template<
    typename tkey,
    typename tvalue>
tvalue binary_search_tree<tkey, tvalue>::dispose(
    tkey const &key)
{
    return _disposal_template->dispose(key);
}

#pragma endregion associative_containers contract implementations

template<
    typename tkey,
    typename tvalue>
void binary_search_tree<tkey, tvalue>::set_insertion_strategy(
    typename binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy insertion_strategy) noexcept
{
    _insertion_template->set_insertion_strategy(insertion_strategy);
    //throw not_implemented("template<typename tkey, typename tvalue> void binary_search_tree<tkey, tvalue>::set_insertion_strategy(typename binary_search_tree<tkey, tvalue>::insertion_of_existent_key_strategy) noexcept", "your code should be here...");
}

template<
    typename tkey,
    typename tvalue>
void binary_search_tree<tkey, tvalue>::set_removal_strategy(
    typename binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy disposal_strategy) noexcept
{
    _disposal_template->set_disposal_strategy(disposal_strategy);
    //throw not_implemented("template<typename tkey, typename tvalue> void binary_search_tree<tkey, tvalue>::set_removal_strategy(typename binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_strategy) noexcept", "your code should be here...");
}

#pragma region iterators requesting implementation

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::prefix_iterator binary_search_tree<tkey, tvalue>::begin_prefix() const noexcept
{
    return binary_search_tree<tkey, tvalue>::prefix_iterator(dynamic_cast<typename binary_search_tree<tkey, tvalue>::node *>(_root));
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::prefix_iterator binary_search_tree<tkey, tvalue>::end_prefix() const noexcept
{
    return binary_search_tree<tkey, tvalue>::prefix_iterator(nullptr);
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::prefix_const_iterator binary_search_tree<tkey, tvalue>::cbegin_prefix() const noexcept
{
    return binary_search_tree<tkey, tvalue>::prefix_const_iterator(dynamic_cast<typename binary_search_tree<tkey, tvalue>::node *>(_root));
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::prefix_const_iterator binary_search_tree<tkey, tvalue>::cend_prefix() const noexcept
{
    return binary_search_tree<tkey, tvalue>::prefix_const_iterator(nullptr);
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::prefix_reverse_iterator binary_search_tree<tkey, tvalue>::rbegin_prefix() const noexcept
{
    return binary_search_tree<tkey, tvalue>::prefix_reverse_iterator(dynamic_cast<typename binary_search_tree<tkey, tvalue>::node *>(_root));
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::prefix_reverse_iterator binary_search_tree<tkey, tvalue>::rend_prefix() const noexcept
{
    return binary_search_tree<tkey, tvalue>::prefix_reverse_iterator(nullptr);
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::prefix_const_reverse_iterator binary_search_tree<tkey, tvalue>::crbegin_prefix() const noexcept
{
    return binary_search_tree<tkey, tvalue>::prefix_const_reverse_iterator(dynamic_cast<typename binary_search_tree<tkey, tvalue>::node *>(_root));
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::prefix_const_reverse_iterator binary_search_tree<tkey, tvalue>::crend_prefix() const noexcept
{
    return binary_search_tree<tkey, tvalue>::prefix_const_reverse_iterator(nullptr);
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::infix_iterator binary_search_tree<tkey, tvalue>::begin_infix() const noexcept
{
    return binary_search_tree<tkey, tvalue>::infix_iterator(dynamic_cast<typename binary_search_tree<tkey, tvalue>::node *>(_root));
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::infix_iterator binary_search_tree<tkey, tvalue>::end_infix() const noexcept
{
    return binary_search_tree<tkey, tvalue>::infix_iterator(nullptr);
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::infix_const_iterator binary_search_tree<tkey, tvalue>::cbegin_infix() const noexcept
{
    return binary_search_tree<tkey, tvalue>::infix_const_iterator(dynamic_cast<typename binary_search_tree<tkey, tvalue>::node *>(_root));
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::infix_const_iterator binary_search_tree<tkey, tvalue>::cend_infix() const noexcept
{
    return binary_search_tree<tkey, tvalue>::infix_const_iterator(nullptr);
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::infix_reverse_iterator binary_search_tree<tkey, tvalue>::rbegin_infix() const noexcept
{
    return binary_search_tree<tkey, tvalue>::infix_reverse_iterator(dynamic_cast<typename binary_search_tree<tkey, tvalue>::node *>(_root));
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::infix_reverse_iterator binary_search_tree<tkey, tvalue>::rend_infix() const noexcept
{
    return binary_search_tree<tkey, tvalue>::infix_reverse_iterator(nullptr);
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::infix_const_reverse_iterator binary_search_tree<tkey, tvalue>::crbegin_infix() const noexcept
{
    return binary_search_tree<tkey, tvalue>::infix_const_reverse_iterator(dynamic_cast<typename binary_search_tree<tkey, tvalue>::node *>(_root));
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::infix_const_reverse_iterator binary_search_tree<tkey, tvalue>::crend_infix() const noexcept
{
    return binary_search_tree<tkey, tvalue>::infix_const_reverse_iterator(nullptr);
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::postfix_iterator binary_search_tree<tkey, tvalue>::begin_postfix() const noexcept
{
    return binary_search_tree<tkey, tvalue>::postfix_iterator(dynamic_cast<typename binary_search_tree<tkey, tvalue>::node *>(_root));
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::postfix_iterator binary_search_tree<tkey, tvalue>::end_postfix() const noexcept
{
    return binary_search_tree<tkey, tvalue>::postfix_iterator(nullptr);
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::postfix_const_iterator binary_search_tree<tkey, tvalue>::cbegin_postfix() const noexcept
{
    return binary_search_tree<tkey, tvalue>::postfix_const_iterator(dynamic_cast<typename binary_search_tree<tkey, tvalue>::node *>(_root));
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::postfix_const_iterator binary_search_tree<tkey, tvalue>::cend_postfix() const noexcept
{
    return binary_search_tree<tkey, tvalue>::postfix_const_iterator(nullptr);
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::postfix_reverse_iterator binary_search_tree<tkey, tvalue>::rbegin_postfix() const noexcept
{
    return binary_search_tree<tkey, tvalue>::postfix_reverse_iterator(dynamic_cast<typename binary_search_tree<tkey, tvalue>::node *>(_root));
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::postfix_reverse_iterator binary_search_tree<tkey, tvalue>::rend_postfix() const noexcept
{
    return binary_search_tree<tkey, tvalue>::postfix_reverse_iterator(nullptr);
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::postfix_const_reverse_iterator binary_search_tree<tkey, tvalue>::crbegin_postfix() const noexcept
{
    return binary_search_tree<tkey, tvalue>::postfix_const_reverse_iterator(dynamic_cast<typename binary_search_tree<tkey, tvalue>::node *>(_root));
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::postfix_const_reverse_iterator binary_search_tree<tkey, tvalue>::crend_postfix() const noexcept
{
    return binary_search_tree<tkey, tvalue>::postfix_const_reverse_iterator(nullptr);
}

#pragma endregion iterators request implementation

#pragma region subtree rotations implementation

template<
    typename tkey,
    typename tvalue>
void binary_search_tree<tkey, tvalue>::small_left_rotation(
    binary_search_tree<tkey, tvalue>::node *&subtree_root,
    bool validate) const
{
    if (subtree_root == nullptr)
    {
        throw std::logic_error("Trying to perform SLR for nullptr subtree_root.");
    }

    auto new_root = subtree_root->right_subtree;
    if (new_root == nullptr)
    {
        throw std::logic_error("Trying to perform SLR for right branch-less subtree root.");
    }

    subtree_root->right_subtree = new_root->left_subtree;
    new_root->left_subtree = subtree_root;
    subtree_root = new_root;
    //throw not_implemented("template<typename tkey, typename tvalue> void binary_search_tree<tkey, tvalue>::small_left_rotation(binary_search_tree<tkey, tvalue>::node *&, bool) const", "your code should be here...");
}

template<
    typename tkey,
    typename tvalue>
void binary_search_tree<tkey, tvalue>::small_right_rotation(
    binary_search_tree<tkey, tvalue>::node *&subtree_root,
    bool validate) const
{
    if (subtree_root == nullptr)
    {
        throw std::logic_error("Trying to perform SRR for nullptr subtree_root.");
    }

    auto new_root = subtree_root->left_subtree;
    if (new_root == nullptr)
    {
        throw std::logic_error("Trying to perform SRR for left branch-less subtree root.");
    }

    subtree_root->left_subtree = new_root->right_subtree;
    new_root->right_subtree = subtree_root;
    subtree_root = new_root;
    //throw not_implemented("template<typename tkey, typename tvalue> void binary_search_tree<tkey, tvalue>::small_right_rotation(binary_search_tree<tkey, tvalue>::node *&, bool) const", "your code should be here...");
}

template<
    typename tkey,
    typename tvalue>
void binary_search_tree<tkey, tvalue>::big_left_rotation(
    binary_search_tree<tkey, tvalue>::node *&subtree_root,
    bool validate) const
{
    small_right_rotation(subtree_root->right_subtree, validate);
    small_left_rotation(subtree_root, validate);
    //throw not_implemented("template<typename tkey, typename tvalue> void binary_search_tree<tkey, tvalue>::big_left_rotation(binary_search_tree<tkey, tvalue>::node *&, bool) const", "your code should be here...");
}

template<
    typename tkey,
    typename tvalue>
void binary_search_tree<tkey, tvalue>::big_right_rotation(
    binary_search_tree<tkey, tvalue>::node *&subtree_root,
    bool validate) const
{
    small_left_rotation(subtree_root->left_subtree, validate);
    small_right_rotation(subtree_root, validate);
    //throw not_implemented("template<typename tkey, typename tvalue> void binary_search_tree<tkey, tvalue>::big_right_rotation(binary_search_tree<tkey, tvalue>::node *&, bool) const", "your code should be here...");
}

template<
    typename tkey,
    typename tvalue>
void binary_search_tree<tkey, tvalue>::double_left_rotation(
    binary_search_tree<tkey, tvalue>::node *&subtree_root,
    bool at_grandparent_first,
    bool validate) const
{
    if (subtree_root == nullptr)
    {
        throw std::logic_error("Trying to perform DLR on nullptr subtree_root.");
    }

    if (subtree_root->right_subtree == nullptr)
    {
        throw std::logic_error("Trying to perform DLR on a node without a right subtree.");
    }

    if (at_grandparent_first)
    {
        small_left_rotation(subtree_root, validate);
        small_right_rotation(subtree_root->right_subtree, validate);
    }
    else
    {
        small_right_rotation(subtree_root->right_subtree, validate);
        small_left_rotation(subtree_root, validate);
    }

    //throw not_implemented("template<typename tkey, typename tvalue> void binary_search_tree<tkey, tvalue>::double_left_rotation(binary_search_tree<tkey, tvalue>::node *&, bool, bool) const", "your code should be here...");
}

template<
    typename tkey,
    typename tvalue>
void binary_search_tree<tkey, tvalue>::double_right_rotation(
    binary_search_tree<tkey, tvalue>::node *&subtree_root,
    bool at_grandparent_first,
    bool validate) const
{
    if (subtree_root == nullptr)
    {
        throw std::logic_error("Trying to perform DRR on nullptr subtree_root.");
    }

    if (subtree_root->left_subtree == nullptr)
    {
        throw std::logic_error("Trying to perform DRR on a node without a left subtree.");
    }

    if (at_grandparent_first)
    {
        small_right_rotation(subtree_root, validate);
        small_left_rotation(subtree_root->left_subtree, validate);
    }
    else
    {
        small_left_rotation(subtree_root->left_subtree, validate);
        small_right_rotation(subtree_root, validate);
    }

    //throw not_implemented("template<typename tkey, typename tvalue> void binary_search_tree<tkey, tvalue>::double_right_rotation(binary_search_tree<tkey, tvalue>::node *&, bool, bool) const", "your code should be here...");
}

#pragma endregion subtree rotations implementation

#endif //MATH_PRACTICE_AND_OPERATING_SYSTEMS_BINARY_SEARCH_TREE_H