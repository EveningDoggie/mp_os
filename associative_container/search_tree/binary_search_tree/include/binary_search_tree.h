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
            tvalue const &value);
        
        explicit node(
            tkey const &key,
            tvalue &&value);
        
    };

public:
    
    // region iterators definition
    
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

        explicit iterator_data(
        unsigned int depth,
        node const * node);
    };

protected:

    class iterator_basics
    {

    protected:

        std::stack<node*> _stack;

        explicit iterator_basics(typename binary_search_tree<tkey, tvalue>::node *subtree_root);

        bool operator==(
            iterator_basics const &other) const noexcept;

        bool operator!=(
            iterator_basics const &other) const noexcept;

        iterator_data *operator*() const;

        void fall_left();

        void fall_right();

        void fall_left_bottom();

        void fall_right_bottom();
    };

public:

    class prefix_iterator final:iterator_basics
    {

    public:
        
        explicit prefix_iterator(
            typename binary_search_tree<tkey, tvalue>::node *subtree_root);
    
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
    
    class prefix_const_iterator final:iterator_basics
    {
    
    public:
        
        explicit prefix_const_iterator(
            typename binary_search_tree<tkey, tvalue>::node *subtree_root);
    
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
    
    class prefix_reverse_iterator final:iterator_basics
    {
    
    public:
        
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
    
    class prefix_const_reverse_iterator final:iterator_basics
    {
    
    public:
        
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
    
    class infix_iterator final:iterator_basics
    {
    
    public:
        
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
    
    class infix_const_iterator final:iterator_basics
    {
    
    public:
        
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
    
    class infix_reverse_iterator final:iterator_basics
    {
    
    public:
        
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
    
    class infix_const_reverse_iterator final:iterator_basics
    {
    
    public:
        
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
    
    class postfix_iterator final:iterator_basics
    {
    
    public:
        
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
    
    class postfix_const_iterator final:iterator_basics
    {
    
    public:
        
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
    
    class postfix_reverse_iterator final:iterator_basics
    {
    
    public:
        
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
    
    class postfix_const_reverse_iterator final:iterator_basics
    {
    
    public:
        
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
    
    // endregion iterators definition

protected:
    
    // region target operations strategies definition
    
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
    
    // endregion target operations strategies definition
    
    // region target operations associated exception types
    
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
    
    // endregion target operations associated exception types
    
    // region template methods definition
    
    class template_method_basics:
        public logger_guardant
    {
    
    protected:
    
        binary_search_tree<tkey, tvalue> *_tree;

        std::stack<node*> _order_stack;

    public:
    
        explicit template_method_basics(
            binary_search_tree<tkey, tvalue> *tree);
        
    protected:

        node * get_node_from_key(tkey key);

        node * get_minimum_node(node * current_node);

        node * get_maximum_node(node * current_node);

        node * get_successor_node(node * current_node);

        node * get_descessor_node(node * current_node);

    protected:
    
        [[nodiscard]] logger *get_logger() const noexcept final;
        
    };
    
    class insertion_template_method:
        public template_method_basics,
        public allocator_guardant
    {

    private:

        insertion_of_existent_key_attempt_strategy _insertion_strategy;
    
    public:
        
        explicit insertion_template_method(
            binary_search_tree<tkey, tvalue> *tree,
            typename binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy insertion_strategy);
        
    public:
        
        void insert(
            tkey const &key,
            tvalue const &value);
        
        void insert(
            tkey const &key,
            tvalue &&value);

        void set_insertion_strategy(
            typename binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy insertion_strategy) noexcept;

    private:

        void insert_execute(
          tkey const &key,
          tvalue const &value);

    protected:

        virtual void balance_execute();
    
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

    };
    
    class disposal_template_method:
        public template_method_basics,
        public allocator_guardant
    {
    
    private:

        disposal_of_nonexistent_key_attempt_strategy _disposal_strategy;

    public:
        
        explicit disposal_template_method(
            binary_search_tree<tkey, tvalue> *tree,
            typename binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy disposal_strategy);
        
    public:
        
        tvalue dispose(
            tkey const &key);
        
        void set_disposal_strategy(
            typename binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy disposal_strategy) noexcept;

    private:

        tvalue dispose_execute(
            tkey const &key);

    protected:
        
        virtual void balance_execute();
    
    private:
        
        [[nodiscard]] allocator *get_allocator() const noexcept final;
        
    };
    
    // endregion template methods definition

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
        binary_search_tree<tkey, tvalue> const &other) = delete;
    
    binary_search_tree(
        binary_search_tree<tkey, tvalue> &&other) noexcept;
    
    binary_search_tree<tkey, tvalue> &operator=(
        binary_search_tree<tkey, tvalue> const &other) = delete;
    
    binary_search_tree<tkey, tvalue> &operator=(
        binary_search_tree<tkey, tvalue> &&other) noexcept;
    
    ~binary_search_tree() override;

    void dispose();

    void init_from_other( binary_search_tree<tkey, tvalue> &&other);

public:
    
    void insert(
        tkey const &key,
        tvalue const &value) final;
    
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

    void check(
    tkey const &key);
    
public:
    
    void set_insertion_strategy(
        typename binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy insertion_strategy) noexcept;
    
    void set_removal_strategy(
        typename binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy disposal_strategy) noexcept;

public:

    // region iterators requests definition

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
    
    // endregion iterators requests definition

protected:
    
    // region subtree rotations definition
    
    void small_left_rotation(
        node *&subtree_root,
        bool validate = true) const;
    
    void small_right_rotation(
        node *&subtree_root,
        bool validate = true) const;
    
    void big_left_rotation(
        node *&subtree_root,
        bool validate = true) const;
    
    void big_right_rotation(
        node *&subtree_root,
        bool validate = true) const;
    
    void double_left_rotation(
        node *&subtree_root,
        bool at_grandparent_first,
        bool validate = true) const;
    
    void double_right_rotation(
       node *&subtree_root,
        bool at_grandparent_first,
        bool validate = true) const;
    
    // endregion subtree rotations definition
    
};

// region binary_search_tree<tkey, tvalue>::node methods implementation

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue>::node::node(
    tkey const &key,
    tvalue const &value):
    key(key),
    value(value),
    left_subtree(nullptr),
    right_subtree(nullptr)
{

}

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue>::node::node(
    tkey const &key,
    tvalue &&value):
    key(*key),
    value(*value),
    left_subtree(nullptr),
    right_subtree(nullptr)
{

}

// endregion binary_search_tree<tkey, tvalue>::node methods implementation

// region iterators implementation

// region iterator data implementation

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

}

template<typename tkey, typename tvalue>
binary_search_tree<tkey, tvalue>::iterator_data::iterator_data(unsigned int depth, node const *node):
iterator_data(depth, node->key, node->value)
{

}

// endregion iterator data implementation

// region prefix_iterator implementation

template<typename tkey, typename tvalue>
binary_search_tree<tkey, tvalue>::iterator_basics::iterator_basics(
    node *subtree_root)
{
     _stack.push(subtree_root);
}

template<typename tkey, typename tvalue>
    bool binary_search_tree<tkey, tvalue>::iterator_basics::operator==(iterator_basics const &other) const noexcept
{
    return other._stack.top() == this->_stack.top();
}

template<typename tkey, typename tvalue>
bool binary_search_tree<tkey, tvalue>::iterator_basics::operator!=(iterator_basics const &other) const noexcept
{
    return other._stack.top() != this->_stack.top();
}

template<typename tkey, typename tvalue>
typename binary_search_tree<tkey, tvalue>::iterator_data * binary_search_tree<tkey, tvalue>::iterator_basics::operator*() const
{
    return new iterator_data(iterator_basics::_stack.size()-1, iterator_basics::_stack.top());
}

template<typename tkey, typename tvalue>
void binary_search_tree<tkey, tvalue>::iterator_basics::fall_left()
{
    node * current_node = iterator_basics::_stack.top();
    if(current_node==nullptr) return;
    while(current_node->left_subtree!=nullptr)
    {
        iterator_basics::_stack.push(current_node->left_subtree);
        current_node = current_node->left_subtree;
    }
}

template<typename tkey, typename tvalue>
void binary_search_tree<tkey, tvalue>::iterator_basics::fall_right()
{
    node * current_node = iterator_basics::_stack.top();
    if(current_node==nullptr) return;
    while(current_node->right_subtree!=nullptr)
    {
        iterator_basics::_stack.push(current_node->right_subtree);
        current_node = current_node->right_subtree;
    }
}

template<typename tkey, typename tvalue>
void binary_search_tree<tkey, tvalue>::iterator_basics::fall_left_bottom()
{
    node * current_node = iterator_basics::_stack.top();
    if(current_node==nullptr) return;

    while(current_node->left_subtree!=nullptr || current_node->right_subtree!=nullptr)
    {
        if(current_node->left_subtree!=nullptr)
        {
            fall_left();
        }

        current_node = iterator_basics::_stack.top();

        if(current_node->right_subtree!=nullptr)
        {
            iterator_basics::_stack.push(current_node->right_subtree);
            current_node = current_node->right_subtree;
        }
    }
}

template<typename tkey, typename tvalue>
void binary_search_tree<tkey, tvalue>::iterator_basics::fall_right_bottom()
{
    node * current_node = iterator_basics::_stack.top();
    if(current_node==nullptr) return;

    while(current_node->left_subtree!=nullptr || current_node->right_subtree!=nullptr)
    {
        if(current_node->right_subtree!=nullptr)
        {
            fall_right();
        }

        current_node = iterator_basics::_stack.top();

        if(current_node->left_subtree!=nullptr)
        {
            iterator_basics::_stack.push(current_node->left_subtree);
            current_node = current_node->left_subtree;
        }
    }
}


template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue>::prefix_iterator::prefix_iterator(
    typename binary_search_tree<tkey, tvalue>::node *subtree_root):
    iterator_basics(subtree_root)
{

}

template<
    typename tkey,
    typename tvalue>
bool binary_search_tree<tkey, tvalue>::prefix_iterator::operator==(
    typename binary_search_tree<tkey, tvalue>::prefix_iterator const &other) const noexcept
{
    return iterator_basics::operator==(other);
}

template<
    typename tkey,
    typename tvalue>
bool binary_search_tree<tkey, tvalue>::prefix_iterator::operator!=(
    typename binary_search_tree<tkey, tvalue>::prefix_iterator const &other) const noexcept
{

    return iterator_basics::operator!=(other);
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::prefix_iterator &binary_search_tree<tkey, tvalue>::prefix_iterator::operator++()
{
    node * current_node = iterator_basics::_stack.top();
    if(current_node == nullptr)  return *this;

    if(current_node->left_subtree!=nullptr)
    {
        iterator_basics::_stack.push(current_node->left_subtree);
        return *this;
    }

    if(current_node->right_subtree!=nullptr)
    {
        iterator_basics::_stack.push(current_node->right_subtree);
        return *this;
    }

    iterator_basics::_stack.pop();
    while(iterator_basics::_stack.size()!=0)
    {
        node * last_node = current_node;
        current_node = iterator_basics::_stack.top();

        if(current_node->right_subtree!=nullptr && current_node->right_subtree != last_node)
        {
            iterator_basics::_stack.push(current_node->right_subtree);
            return *this;
        }

        iterator_basics::_stack.pop();
    }

    iterator_basics::_stack.push(nullptr);
    return *this;
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::prefix_iterator const binary_search_tree<tkey, tvalue>::prefix_iterator::operator++(
    int not_used)
{
    prefix_iterator temp {*this};
    ++(*this);
    return temp;
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::iterator_data *binary_search_tree<tkey, tvalue>::prefix_iterator::operator*() const
{
    return iterator_basics::operator*();
}

// endregion prefix_iterator implementation

// region prefix_const_iterator implementation

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue>::prefix_const_iterator::prefix_const_iterator(
    typename binary_search_tree<tkey, tvalue>::node *subtree_root):
    iterator_basics(subtree_root)
{

}

template<
    typename tkey,
    typename tvalue>
bool binary_search_tree<tkey, tvalue>::prefix_const_iterator::operator==(
    typename binary_search_tree<tkey, tvalue>::prefix_const_iterator const &other) const noexcept
{
    return iterator_basics::operator==(other);
}

template<
    typename tkey,
    typename tvalue>
bool binary_search_tree<tkey, tvalue>::prefix_const_iterator::operator!=(
    typename binary_search_tree<tkey, tvalue>::prefix_const_iterator const &other) const noexcept
{
    return iterator_basics::operator!=(other);
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::prefix_const_iterator &binary_search_tree<tkey, tvalue>::prefix_const_iterator::operator++()
{
    node * current_node = iterator_basics::_stack.top();
    if(current_node == nullptr)  return *this;

    if(current_node->left_subtree!=nullptr)
    {
        iterator_basics::_stack.push(current_node->left_subtree);
        return *this;
    }

    if(current_node->right_subtree!=nullptr)
    {
        iterator_basics::_stack.push(current_node->right_subtree);
        return *this;
    }

    iterator_basics::_stack.pop();
    while(iterator_basics::_stack.size()!=0)
    {
        node * last_node = current_node;
        current_node = iterator_basics::_stack.top();

        if(current_node->right_subtree!=nullptr && current_node->right_subtree != last_node)
        {
            iterator_basics::_stack.push(current_node->right_subtree);
            return *this;
        }

        iterator_basics::_stack.pop();
    }

    iterator_basics::_stack.push(nullptr);
    return *this;
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::prefix_const_iterator const binary_search_tree<tkey, tvalue>::prefix_const_iterator::operator++(
    int not_used)
{
    prefix_const_iterator temp {*this};
    ++(*this);
    return temp;
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::iterator_data const *binary_search_tree<tkey, tvalue>::prefix_const_iterator::operator*() const
{
    return iterator_basics::operator*();
}

// endregion prefix_const_iterator implementation

// region prefix_reverse_iterator implementation

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue>::prefix_reverse_iterator::prefix_reverse_iterator(
typename binary_search_tree<tkey, tvalue>::node *subtree_root):
iterator_basics(subtree_root)
{

}

template<
    typename tkey,
    typename tvalue>
bool binary_search_tree<tkey, tvalue>::prefix_reverse_iterator::operator==(
    typename binary_search_tree<tkey, tvalue>::prefix_reverse_iterator const &other) const noexcept
{
    return iterator_basics::operator==(other);
}

template<
    typename tkey,
    typename tvalue>
bool binary_search_tree<tkey, tvalue>::prefix_reverse_iterator::operator!=(
    typename binary_search_tree<tkey, tvalue>::prefix_reverse_iterator const &other) const noexcept
{
    return iterator_basics::operator!=(other);
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::prefix_reverse_iterator &binary_search_tree<tkey, tvalue>::prefix_reverse_iterator::operator++()
{
    node * current_node = iterator_basics::_stack.top();
    if(current_node == nullptr)  return *this;

    if(current_node->right_subtree!=nullptr)
    {
        iterator_basics::_stack.push(current_node->right_subtree);
        return *this;
    }

    if(current_node->left_subtree!=nullptr)
    {
        iterator_basics::_stack.push(current_node->left_subtree);
        return *this;
    }

    iterator_basics::_stack.pop();
    while(iterator_basics::_stack.size()!=0)
    {
        node * last_node = current_node;
        current_node = iterator_basics::_stack.top();

        if(current_node->left_subtree!=nullptr && current_node->left_subtree != last_node)
        {
            iterator_basics::_stack.push(current_node->left_subtree);
            return *this;
        }

        iterator_basics::_stack.pop();
    }

    iterator_basics::_stack.push(nullptr);
    return *this;
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::prefix_reverse_iterator const binary_search_tree<tkey, tvalue>::prefix_reverse_iterator::operator++(
    int not_used)
{
    prefix_reverse_iterator temp {*this};
    ++(*this);
    return temp;
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::iterator_data *binary_search_tree<tkey, tvalue>::prefix_reverse_iterator::operator*() const
{
    return iterator_basics::operator*();
}

// endregion prefix_reverse_iterator implementation

// region prefix_const_reverse_iterator implementation

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue>::prefix_const_reverse_iterator::prefix_const_reverse_iterator(
typename binary_search_tree<tkey, tvalue>::node *subtree_root):
iterator_basics(subtree_root)
{

}

template<
    typename tkey,
    typename tvalue>
bool binary_search_tree<tkey, tvalue>::prefix_const_reverse_iterator::operator==(
    typename binary_search_tree<tkey, tvalue>::prefix_const_reverse_iterator const &other) const noexcept
{
    return iterator_basics::operator==(other);
}

template<
    typename tkey,
    typename tvalue>
bool binary_search_tree<tkey, tvalue>::prefix_const_reverse_iterator::operator!=(
    typename binary_search_tree<tkey, tvalue>::prefix_const_reverse_iterator const &other) const noexcept
{
    return iterator_basics::operator!=(other);
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::prefix_const_reverse_iterator &binary_search_tree<tkey, tvalue>::prefix_const_reverse_iterator::operator++()
{
    node * current_node = iterator_basics::_stack.top();
    if(current_node == nullptr)  return *this;

    if(current_node->right_subtree!=nullptr)
    {
        iterator_basics::_stack.push(current_node->right_subtree);
        return *this;
    }

    if(current_node->left_subtree!=nullptr)
    {
        iterator_basics::_stack.push(current_node->left_subtree);
        return *this;
    }

    iterator_basics::_stack.pop();
    while(iterator_basics::_stack.size()!=0)
    {
        node * last_node = current_node;
        current_node = iterator_basics::_stack.top();

        if(current_node->left_subtree!=nullptr && current_node->left_subtree != last_node)
        {
            iterator_basics::_stack.push(current_node->left_subtree);
            return *this;
        }

        iterator_basics::_stack.pop();
    }

    iterator_basics::_stack.push(nullptr);
    return *this;
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::prefix_const_reverse_iterator const binary_search_tree<tkey, tvalue>::prefix_const_reverse_iterator::operator++(
    int not_used)
{
    prefix_const_reverse_iterator temp {*this};
    ++(*this);
    return temp;
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::iterator_data const *binary_search_tree<tkey, tvalue>::prefix_const_reverse_iterator::operator*() const
{
    return iterator_basics::operator*();
}

// endregion prefix_const_reverse_iterator implementation

// region infix_iterator implementation

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue>::infix_iterator::infix_iterator(
typename binary_search_tree<tkey, tvalue>::node *subtree_root):
iterator_basics(subtree_root)
{
    iterator_basics::fall_left();
}

template<
    typename tkey,
    typename tvalue>
bool binary_search_tree<tkey, tvalue>::infix_iterator::operator==(
    typename binary_search_tree<tkey, tvalue>::infix_iterator const &other) const noexcept
{
    return iterator_basics::operator==(other);
}

template<
    typename tkey,
    typename tvalue>
bool binary_search_tree<tkey, tvalue>::infix_iterator::operator!=(
    typename binary_search_tree<tkey, tvalue>::infix_iterator const &other) const noexcept
{
    return iterator_basics::operator!=(other);
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::infix_iterator &binary_search_tree<tkey, tvalue>::infix_iterator::operator++()
{
    node * current_node = iterator_basics::_stack.top();
    if(current_node == nullptr) return *this;

    if(current_node->right_subtree!=nullptr)
    {
        iterator_basics::_stack.push(current_node->right_subtree);
        iterator_basics::fall_left();
        return *this;
    }

    while(iterator_basics::_stack.size()!=0)
    {
        node * last_node = current_node;
        iterator_basics::_stack.pop();
        current_node = iterator_basics::_stack.top();

        if(current_node->left_subtree == last_node)
        {
            return *this;
        }
        else if(iterator_basics::_stack.size()==1)
        {
            break;
        }
    }

    iterator_basics::_stack.push(nullptr);
    return *this;
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::infix_iterator const binary_search_tree<tkey, tvalue>::infix_iterator::operator++(
    int not_used)
{
    infix_iterator temp {*this};
    ++(*this);
    return temp;
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::iterator_data *binary_search_tree<tkey, tvalue>::infix_iterator::operator*() const
{
    return iterator_basics::operator*();
}

// endregion infix_iterator implementation

// region infix_const_iterator implementation

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue>::infix_const_iterator::infix_const_iterator(
typename binary_search_tree<tkey, tvalue>::node *subtree_root):
iterator_basics(subtree_root)
{

}

template<
    typename tkey,
    typename tvalue>
bool binary_search_tree<tkey, tvalue>::infix_const_iterator::operator==(
    typename binary_search_tree<tkey, tvalue>::infix_const_iterator const &other) const noexcept
{
    return iterator_basics::operator==(other);
}

template<
    typename tkey,
    typename tvalue>
bool binary_search_tree<tkey, tvalue>::infix_const_iterator::operator!=(
    typename binary_search_tree<tkey, tvalue>::infix_const_iterator const &other) const noexcept
{
    return iterator_basics::operator!=(other);
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::infix_const_iterator &binary_search_tree<tkey, tvalue>::infix_const_iterator::operator++()
{
    node * current_node = iterator_basics::_stack.top();
    if(current_node == nullptr) return *this;

    if(current_node->right_subtree!=nullptr)
    {
        iterator_basics::_stack.push(current_node->right_subtree);
        iterator_basics::fall_left();
        return *this;
    }

    while(iterator_basics::_stack.size()!=0)
    {
        node * last_node = current_node;
        iterator_basics::_stack.pop();
        current_node = iterator_basics::_stack.top();

        if(current_node->left_subtree == last_node)
        {
            return *this;
        }
        else if(iterator_basics::_stack.size()==1)
        {
            break;
        }
    }

    iterator_basics::_stack.push(nullptr);
    return *this;
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::infix_const_iterator const binary_search_tree<tkey, tvalue>::infix_const_iterator::operator++(
    int not_used)
{
    infix_const_iterator temp {*this};
    ++(*this);
    return temp;
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::iterator_data const *binary_search_tree<tkey, tvalue>::infix_const_iterator::operator*() const
{
    return iterator_basics::operator*();
}

// endregion infix_const_iterator implementation

// region infix_reverse_iterator implementation

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue>::infix_reverse_iterator::infix_reverse_iterator(
typename binary_search_tree<tkey, tvalue>::node *subtree_root):
iterator_basics(subtree_root)
{
    iterator_basics::fall_right();
}

template<
    typename tkey,
    typename tvalue>
bool binary_search_tree<tkey, tvalue>::infix_reverse_iterator::operator==(
    typename binary_search_tree<tkey, tvalue>::infix_reverse_iterator const &other) const noexcept
{
    return iterator_basics::operator==(other);
}

template<
    typename tkey,
    typename tvalue>
bool binary_search_tree<tkey, tvalue>::infix_reverse_iterator::operator!=(
    typename binary_search_tree<tkey, tvalue>::infix_reverse_iterator const &other) const noexcept
{
    return iterator_basics::operator!=(other);
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::infix_reverse_iterator &binary_search_tree<tkey, tvalue>::infix_reverse_iterator::operator++()
{
    node * current_node = iterator_basics::_stack.top();
    if(current_node == nullptr) return *this;

    if(current_node->left_subtree!=nullptr)
    {
        iterator_basics::_stack.push(current_node->left_subtree);
        iterator_basics::fall_right();
        return *this;
    }

    while(iterator_basics::_stack.size()!=0)
    {
        node * last_node = current_node;
        iterator_basics::_stack.pop();
        current_node = iterator_basics::_stack.top();

        if(current_node->right_subtree == last_node)
        {
            return *this;
        }
        else if(iterator_basics::_stack.size()==1)
        {
            break;
        }
    }

    iterator_basics::_stack.push(nullptr);
    return *this;
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::infix_reverse_iterator const binary_search_tree<tkey, tvalue>::infix_reverse_iterator::operator++(
    int not_used)
{
    infix_reverse_iterator temp {*this};
    ++(*this);
    return temp;
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::iterator_data *binary_search_tree<tkey, tvalue>::infix_reverse_iterator::operator*() const
{
   return iterator_basics::operator*();
}

// endregion infix_reverse_iterator implementation

// region infix_const_reverse_iterator implementation

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue>::infix_const_reverse_iterator::infix_const_reverse_iterator(
typename binary_search_tree<tkey, tvalue>::node *subtree_root):
iterator_basics(subtree_root)
{

}

template<
    typename tkey,
    typename tvalue>
bool binary_search_tree<tkey, tvalue>::infix_const_reverse_iterator::operator==(
    typename binary_search_tree<tkey, tvalue>::infix_const_reverse_iterator const &other) const noexcept
{
    return iterator_basics::operator==(other);
}

template<
    typename tkey,
    typename tvalue>
bool binary_search_tree<tkey, tvalue>::infix_const_reverse_iterator::operator!=(
    typename binary_search_tree<tkey, tvalue>::infix_const_reverse_iterator const &other) const noexcept
{
    return iterator_basics::operator!=(other);
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::infix_const_reverse_iterator &binary_search_tree<tkey, tvalue>::infix_const_reverse_iterator::operator++()
{
    node * current_node = iterator_basics::_stack.top();
    if(current_node == nullptr) return *this;

    if(current_node->left_subtree!=nullptr)
    {
        iterator_basics::_stack.push(current_node->left_subtree);
        iterator_basics::fall_right();
        return *this;
    }

    while(iterator_basics::_stack.size()!=0)
    {
        node * last_node = current_node;
        iterator_basics::_stack.pop();
        current_node = iterator_basics::_stack.top();

        if(current_node->right_subtree == last_node)
        {
            return *this;
        }
        else if(iterator_basics::_stack.size()==1)
        {
            break;
        }
    }

    iterator_basics::_stack.push(nullptr);
    return *this;
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::infix_const_reverse_iterator const binary_search_tree<tkey, tvalue>::infix_const_reverse_iterator::operator++(
    int not_used)
{
    infix_const_reverse_iterator temp {*this};
    ++(*this);
    return temp;
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::iterator_data const *binary_search_tree<tkey, tvalue>::infix_const_reverse_iterator::operator*() const
{
    return iterator_basics::operator*();
}

// endregion infix_const_reverse_iterator implementation

// region postfix_iterator implementation

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue>::postfix_iterator::postfix_iterator(
typename binary_search_tree<tkey, tvalue>::node *subtree_root):
iterator_basics(subtree_root)
{
    iterator_basics::fall_left_bottom();
}

template<
    typename tkey,
    typename tvalue>
bool binary_search_tree<tkey, tvalue>::postfix_iterator::operator==(
    typename binary_search_tree<tkey, tvalue>::postfix_iterator const &other) const noexcept
{
    return iterator_basics::operator==(other);
}

template<
    typename tkey,
    typename tvalue>
bool binary_search_tree<tkey, tvalue>::postfix_iterator::operator!=(
    typename binary_search_tree<tkey, tvalue>::postfix_iterator const &other) const noexcept
{
    return iterator_basics::operator!=(other);
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::postfix_iterator &binary_search_tree<tkey, tvalue>::postfix_iterator::operator++()
{
    if(iterator_basics::_stack.top() == nullptr) return *this;
    if(iterator_basics::_stack.size()==1)
    {
        iterator_basics::_stack.push(nullptr);
        return *this;
    }

    node * last_node = iterator_basics::_stack.top();
    iterator_basics::_stack.pop();
    node * current_node = iterator_basics::_stack.top();

    if(current_node->left_subtree==last_node
        && current_node->right_subtree!=nullptr)
    {
        iterator_basics::_stack.push(current_node->right_subtree);
        iterator_basics::fall_left_bottom();
        return *this;
    }

    return *this;
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::postfix_iterator const binary_search_tree<tkey, tvalue>::postfix_iterator::operator++(
    int not_used)
{
    postfix_iterator temp {*this};
    ++(*this);
    return temp;
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::iterator_data *binary_search_tree<tkey, tvalue>::postfix_iterator::operator*() const
{
    return iterator_basics::operator*();
}

// endregion postfix_iterator implementation

// region postfix_const_iterator implementation

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue>::postfix_const_iterator::postfix_const_iterator(
typename binary_search_tree<tkey, tvalue>::node *subtree_root):
iterator_basics(subtree_root)
{

}

template<
    typename tkey,
    typename tvalue>
bool binary_search_tree<tkey, tvalue>::postfix_const_iterator::operator==(
    typename binary_search_tree<tkey, tvalue>::postfix_const_iterator const &other) const noexcept
{
    return iterator_basics::operator==(other);
}

template<
    typename tkey,
    typename tvalue>
bool binary_search_tree<tkey, tvalue>::postfix_const_iterator::operator!=(
    typename binary_search_tree<tkey, tvalue>::postfix_const_iterator const &other) const noexcept
{
    return iterator_basics::operator!=(other);
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::postfix_const_iterator &binary_search_tree<tkey, tvalue>::postfix_const_iterator::operator++()
{
    if(iterator_basics::_stack.top() == nullptr) return *this;
    if(iterator_basics::_stack.size()==1)
    {
        iterator_basics::_stack.push(nullptr);
        return *this;
    }

    node * last_node = iterator_basics::_stack.top();
    iterator_basics::_stack.pop();
    node * current_node = iterator_basics::_stack.top();

    if(current_node->left_subtree==last_node
        && current_node->right_subtree!=nullptr)
    {
        iterator_basics::_stack.push(current_node->right_subtree);
        iterator_basics::fall_left_bottom();
        return *this;
    }

    return *this;
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::postfix_const_iterator const binary_search_tree<tkey, tvalue>::postfix_const_iterator::operator++(
    int not_used)
{
    postfix_const_iterator temp {*this};
    ++(*this);
    return temp;
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::iterator_data const *binary_search_tree<tkey, tvalue>::postfix_const_iterator::operator*() const
{
    return iterator_basics::operator*();
}

// endregion postfix_const_iterator implementation

// region postfix_reverse_iterator implementation

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue>::postfix_reverse_iterator::postfix_reverse_iterator(
typename binary_search_tree<tkey, tvalue>::node *subtree_root):
iterator_basics(subtree_root)
{
    iterator_basics::fall_right_bottom();
}

template<
    typename tkey,
    typename tvalue>
bool binary_search_tree<tkey, tvalue>::postfix_reverse_iterator::operator==(
    typename binary_search_tree<tkey, tvalue>::postfix_reverse_iterator const &other) const noexcept
{
    return iterator_basics::operator==(other);
}

template<
    typename tkey,
    typename tvalue>
bool binary_search_tree<tkey, tvalue>::postfix_reverse_iterator::operator!=(
    typename binary_search_tree<tkey, tvalue>::postfix_reverse_iterator const &other) const noexcept
{
    return iterator_basics::operator!=(other);
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::postfix_reverse_iterator &binary_search_tree<tkey, tvalue>::postfix_reverse_iterator::operator++()
{
    if(iterator_basics::_stack.top() == nullptr) return *this;
    if(iterator_basics::_stack.size()==1)
    {
        iterator_basics::_stack.push(nullptr);
        return *this;
    }

    node * last_node = iterator_basics::_stack.top();
    iterator_basics::_stack.pop();
    node * current_node = iterator_basics::_stack.top();

    if(current_node->right_subtree==last_node
        && current_node->left_subtree!=nullptr)
    {
        iterator_basics::_stack.push(current_node->left_subtree);
        iterator_basics::fall_right_bottom();
        return *this;
    }

    return *this;

}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::postfix_reverse_iterator const binary_search_tree<tkey, tvalue>::postfix_reverse_iterator::operator++(
    int not_used)
{
    postfix_reverse_iterator temp {*this};
    ++(*this);
    return temp;
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::iterator_data *binary_search_tree<tkey, tvalue>::postfix_reverse_iterator::operator*() const
{
    return iterator_basics::operator*();
}

// endregion postfix_reverse_iterator implementation

// region postfix_const_reverse_iterator implementation

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue>::postfix_const_reverse_iterator::postfix_const_reverse_iterator(
typename binary_search_tree<tkey, tvalue>::node *subtree_root):
iterator_basics(subtree_root)
{
}

template<
    typename tkey,
    typename tvalue>
bool binary_search_tree<tkey, tvalue>::postfix_const_reverse_iterator::operator==(
    typename binary_search_tree<tkey, tvalue>::postfix_const_reverse_iterator const &other) const noexcept
{
    return iterator_basics::operator==(other);
}

template<
    typename tkey,
    typename tvalue>
bool binary_search_tree<tkey, tvalue>::postfix_const_reverse_iterator::operator!=(
    typename binary_search_tree<tkey, tvalue>::postfix_const_reverse_iterator const &other) const noexcept
{
    return iterator_basics::operator!=(other);
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::postfix_const_reverse_iterator &binary_search_tree<tkey, tvalue>::postfix_const_reverse_iterator::operator++()
{
    if(iterator_basics::_stack.top() == nullptr) return *this;
    if(iterator_basics::_stack.size()==1)
    {
        iterator_basics::_stack.push(nullptr);
        return *this;
    }

    node * last_node = iterator_basics::_stack.top();
    iterator_basics::_stack.pop();
    node * current_node = iterator_basics::_stack.top();

    if(current_node->right_subtree==last_node
        && current_node->left_subtree!=nullptr)
    {
        iterator_basics::_stack.push(current_node->left_subtree);
        iterator_basics::fall_right_bottom();
        return *this;
    }

    return *this;
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::postfix_const_reverse_iterator const binary_search_tree<tkey, tvalue>::postfix_const_reverse_iterator::operator++(
    int not_used)
{
    postfix_const_reverse_iterator temp {*this};
    ++(*this);
    return temp;
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::iterator_data const *binary_search_tree<tkey, tvalue>::postfix_const_reverse_iterator::operator*() const
{
   return iterator_basics::operator*();
}

// endregion postfix_const_reverse_iterator implementation

// endregion iterators implementation

// region target operations associated exception types implementation

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

// endregion target operations associated exception types implementation

// region template methods implementation

// region binary_search_tree<tkey, tvalue>::template_method_basics implementation

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue>::template_method_basics::template_method_basics(
    binary_search_tree<tkey, tvalue> *tree):
    _tree(tree)
{

}


template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::node * binary_search_tree<tkey, tvalue>::template_method_basics::get_node_from_key(tkey key)
{
    _order_stack = std::stack<nodeQ*>();
    node * current_node = _tree->_root;

    while(current_node!=nullptr)
    {
        _order_stack.push(current_node);

        int result = _tree->_keys_comparer(key, current_node->key);
        if(result==0)
            return current_node;
        if (result>0)
            current_node = current_node->right_subtree;
        else
            current_node = current_node->left_subtree;
    }

    return current_node;
}

template<typename tkey, typename tvalue>
typename binary_search_tree<tkey, tvalue>::node * binary_search_tree<tkey, tvalue>::template_method_basics::get_minimum_node(node * current_node)
{
    if(current_node==nullptr)
        return nullptr;

    node * searched_node = current_node;
    while(searched_node->left_subtree!=nullptr)
    {
        _order_stack.push(searched_node->left_subtree);
        searched_node=searched_node->left_subtree;
    }

    return searched_node;
}

template<typename tkey, typename tvalue>
typename binary_search_tree<tkey, tvalue>::node * binary_search_tree<tkey, tvalue>::template_method_basics::get_maximum_node(node * current_node)
{
    if(current_node==nullptr)
        return nullptr;

    node * searched_node = current_node;
    while(searched_node->right_subtree!=nullptr)
    {
        _order_stack.push(searched_node->right_subtree);
        searched_node=searched_node->right_subtree;
    }

    return searched_node;
}

template<typename tkey, typename tvalue>
typename binary_search_tree<tkey, tvalue>::node * binary_search_tree<tkey, tvalue>::template_method_basics::get_successor_node(node *current_node)
{
    if(current_node==nullptr) return nullptr;
    if(current_node->right_subtree!=nullptr)
    {
        _order_stack.push(current_node->right_subtree);
        return get_minimum_node(current_node->right_subtree);
    }

    _order_stack.pop();
    node * parent_node = _order_stack.top();

    while(!_order_stack.empty() && parent_node->right_subtree == current_node)
    {
        current_node=parent_node;
        _order_stack.pop();
        parent_node = _order_stack.top();
    }

    return parent_node;
}


template<typename tkey, typename tvalue>
typename binary_search_tree<tkey, tvalue>::node * binary_search_tree<tkey, tvalue>::template_method_basics::get_descessor_node(node *current_node)
{
    if(current_node==nullptr) return nullptr;
    if(current_node->left_subtree!=nullptr)
    {
        _order_stack.push(current_node->left_subtree);
        return get_maximum_node(current_node->left_subtree);
    }

    _order_stack.pop();
    node * parent_node = _order_stack.top();

    while(!_order_stack.empty() && parent_node->left_subtree == current_node)
    {
        current_node=parent_node;
        _order_stack.pop();
        parent_node = _order_stack.top();
    }

    return parent_node;
}






template<
    typename tkey,
    typename tvalue>
[[nodiscard]] inline logger *binary_search_tree<tkey, tvalue>::template_method_basics::get_logger() const noexcept
{
    return _tree->get_logger();
}

// endregion binary_search_tree<tkey, tvalue>::template_method_basics implementation

// region search_tree<tkey, tvalue>::insertion_template_method implementation

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue>::insertion_template_method::insertion_template_method(
    binary_search_tree<tkey, tvalue> *tree,
    typename binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy insertion_strategy):
    binary_search_tree<tkey, tvalue>::template_method_basics::template_method_basics(tree),
    _insertion_strategy(insertion_strategy)
{

}

template<
    typename tkey,
    typename tvalue>
void binary_search_tree<tkey, tvalue>::insertion_template_method::insert(
    tkey const &key,
    tvalue const &value)
{
    insert_execute(key, value);
    balance_execute();
}



template<
    typename tkey,
    typename tvalue>
void binary_search_tree<tkey, tvalue>::insertion_template_method::insert(
    tkey const &key,
    tvalue &&value)
{
    binary_search_tree<tkey, tvalue>::insertion_template_method::insert(key, value);
}

template<typename tkey, typename tvalue>
void binary_search_tree<tkey, tvalue>::insertion_template_method::insert_execute(tkey const &key, tvalue const &value)
{
    node * searched_node = template_method_basics::get_node_from_key(key);

    if(searched_node!=nullptr)
    {
        if(_insertion_strategy==insertion_of_existent_key_attempt_strategy::throw_an_exception)
        {
            throw disposal_of_nonexistent_key_attempt_exception(key);
        }
        else
        {
            searched_node->value = value;
            return;
        }
    }

    try
    {
        node* insertion_node =  reinterpret_cast<node*>(allocate_with_guard(sizeof(node)));
        new (insertion_node) node(key, value);

        if(template_method_basics::_tree->_root==nullptr)
        {
            template_method_basics::_tree->_root = insertion_node;
            return;
        }

        node * insert_place = template_method_basics::_order_stack.top();
        int result = template_method_basics::_tree->_keys_comparer(key, insert_place->key);
        if(result > 0)
        {
            insert_place->right_subtree = insertion_node;
        }
        else
        {
            insert_place->left_subtree = insertion_node;
        }
    }
    catch(std::bad_alloc const &ex)
    {
        //log
    }
    //последний элемент стека указывает на родителя вставленного узла
}

template<typename tkey, typename tvalue>
void binary_search_tree<tkey, tvalue>::insertion_template_method::balance_execute()
{

}

template<
    typename tkey,
    typename tvalue>
void binary_search_tree<tkey, tvalue>::insertion_template_method::set_insertion_strategy(
    typename binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy insertion_strategy) noexcept
{
    _insertion_strategy = insertion_strategy;
}

template<
    typename tkey,
    typename tvalue>
allocator *binary_search_tree<tkey, tvalue>::insertion_template_method::get_allocator() const noexcept
{
    return template_method_basics::_tree->get_allocator();
}

// endregion search_tree<tkey, tvalue>::insertion_template_method implementation

// region search_tree<tkey, tvalue>::obtaining_template_method implementation

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue>::obtaining_template_method::obtaining_template_method(
    binary_search_tree<tkey, tvalue> *tree):
    binary_search_tree<tkey, tvalue>::template_method_basics::template_method_basics(tree)
{

}

template<
    typename tkey,
    typename tvalue>
tvalue const &binary_search_tree<tkey, tvalue>::obtaining_template_method::obtain(
    tkey const &key)
{
    node * searched_node = template_method_basics::get_node_from_key(key);
    if(searched_node==nullptr)
    {
        throw new obtaining_of_nonexistent_key_attempt_exception(key);
    }

    return searched_node->value;
}

// endregion search_tree<tkey, tvalue>::obtaining_template_method implementation

// region search_tree<tkey, tvalue>::disposal_template_method implementation

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue>::disposal_template_method::disposal_template_method(
    binary_search_tree<tkey, tvalue> *tree,
    typename binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy disposal_strategy):
    binary_search_tree<tkey, tvalue>::template_method_basics(tree),
    _disposal_strategy(disposal_strategy)
{

}

template<
    typename tkey,
    typename tvalue>
tvalue binary_search_tree<tkey, tvalue>::disposal_template_method::dispose(
    tkey const &key)
{
   return dispose_execute(key);
}

template<
    typename tkey,
    typename tvalue>
void binary_search_tree<tkey, tvalue>::disposal_template_method::set_disposal_strategy(
    typename binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy disposal_strategy) noexcept
{
    _disposal_strategy=disposal_strategy;
}

template<typename tkey, typename tvalue>
tvalue binary_search_tree<tkey, tvalue>::disposal_template_method::dispose_execute(tkey const &key)
{
    node * searched_node = template_method_basics::get_node_from_key(key);
    if(searched_node==nullptr)
    {
        if(_disposal_strategy==disposal_of_nonexistent_key_attempt_strategy::throw_an_exception)
            throw disposal_of_nonexistent_key_attempt_exception(key);
        else
            return 0;
    }

    node * remove_node;
    node * orphan_node;

    if(searched_node->left_subtree==nullptr
        || searched_node->right_subtree==nullptr)
    {
        remove_node=searched_node;
    }
    else
    {
        remove_node = template_method_basics::get_successor_node(searched_node);
    }

    if(remove_node->left_subtree!=nullptr)
        orphan_node=remove_node->left_subtree;
    else
        orphan_node=remove_node->right_subtree;

    template_method_basics::_order_stack.pop();
    if(template_method_basics::_order_stack.empty())
    {
        template_method_basics::_tree->_root = orphan_node;
    }
    else
    {
        node * parent = template_method_basics::_order_stack.top();
        if(parent->left_subtree==remove_node)
            parent->left_subtree = orphan_node;
        else
            parent->right_subtree = orphan_node;
    }


    tvalue value = searched_node->value;
    if(searched_node!=remove_node)
    {
        searched_node->key = remove_node->key;
        searched_node->value = remove_node->value;
    }

    remove_node->~node();
    deallocate_with_guard(remove_node);
    return value;

    //во всех случаях в конце стека лежит указатель на родителя удаленного элемента
    //если был узел-замена, то на его родителя, тк удаляли мы именно его
}

template<typename tkey, typename tvalue>
void binary_search_tree<tkey, tvalue>::disposal_template_method::balance_execute()
{

}

template<
    typename tkey,
    typename tvalue>
[[nodiscard]] inline allocator *binary_search_tree<tkey, tvalue>::disposal_template_method::get_allocator() const noexcept
{
    return template_method_basics::_tree->get_allocator();
}

// endregion search_tree<tkey, tvalue>::disposal_template_method implementation

// endregion template methods

// region construction, assignment, destruction implementation

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
    _root=nullptr;
}

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue>::binary_search_tree(
    std::function<int(tkey const &, tkey const &)> comparer,
    allocator *allocator,
    logger *logger,
    typename binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy insertion_strategy,
    typename binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy disposal_strategy):
    binary_search_tree(
        new binary_search_tree<tkey, tvalue>::insertion_template_method(this, insertion_strategy),
        new binary_search_tree<tkey, tvalue>::obtaining_template_method(this),
        new binary_search_tree<tkey, tvalue>::disposal_template_method(this, disposal_strategy),
        comparer,
        allocator,
        logger)
{

}

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue>::binary_search_tree(
    binary_search_tree<tkey, tvalue> &&other) noexcept
{
    init_from_other(std::move(other));
}

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue> &binary_search_tree<tkey, tvalue>::operator=(
    binary_search_tree<tkey, tvalue> &&other) noexcept
{
    dispose();
    init_from_other(std::move(other));
    return *this;
}

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue>::~binary_search_tree()
{
    dispose();
}

template<typename tkey, typename tvalue>
void binary_search_tree<tkey, tvalue>::dispose()
{
    for(auto it = begin_postfix(); it!=end_postfix(); ++it)
    {
        dispose((*it)->key);
    }

   if(_insertion_template!=nullptr)
   {
       delete _insertion_template;
       _insertion_template=nullptr;
   }

    if(_obtaining_template!=nullptr)
    {
        delete _obtaining_template;
        _obtaining_template=nullptr;
    }

    if(_disposal_template!=nullptr)
    {
        delete _disposal_template;
        _disposal_template=nullptr;
    }
}

template<typename tkey, typename tvalue>
void binary_search_tree<tkey, tvalue>::init_from_other(binary_search_tree<tkey, tvalue> &&other)
{
    _insertion_template = other._insertion_template;
    _disposal_template = other._disposal_template;
    _obtaining_template = other._obtaining_template;
    _root = other._root;
}

// endregion construction, assignment, destruction implementation

// region associative_container<tkey, tvalue> contract implementation

template<
    typename tkey,
    typename tvalue>
void binary_search_tree<tkey, tvalue>::insert(
    tkey const &key,
    tvalue const &value)
{
    _insertion_template->insert(key, value);
}

template<
    typename tkey,
    typename tvalue>
void binary_search_tree<tkey, tvalue>::insert(
    tkey const &key,
    tvalue &&value)
{
    _insertion_template->insert(key, std::move(value));
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
    //оптимизация: пока левая граница > текущего элемента или правая граница < текущего элемента
    //делать цикл: если левая граница > ... то идем вправо. если правая <... то идем влево

    std::vector<typename associative_container<tkey, tvalue>::key_value_pair> pairs;

    for(auto it = begin_infix(); it!=end_infix(); ++it)
    {
        int lower_current_compare = search_tree<tkey,tvalue>::_keys_comparer(lower_bound, (*it)->key);
        bool lower_within_the_borders = lower_current_compare<0 || (lower_bound_inclusive && lower_current_compare==0);
        int upper_current_compare = search_tree<tkey,tvalue>::_keys_comparer(upper_bound, (*it)->key);
        bool upper_within_the_borders = upper_current_compare>0 || (upper_bound_inclusive && upper_current_compare==0);

        if(lower_within_the_borders && upper_within_the_borders)
        {
            auto pair = typename associative_container<tkey, tvalue>::key_value_pair((*it)->key, (*it)->value);
            pairs.push_back(pair);
        }
    }

    return pairs;

}

template<
    typename tkey,
    typename tvalue>
tvalue binary_search_tree<tkey, tvalue>::dispose(
    tkey const &key)
{
    return _disposal_template->dispose(key);
}

template<typename tkey, typename tvalue>
void binary_search_tree<tkey, tvalue>::check(tkey const &key)
{
    _insertion_template->check(key);
}

// endregion associative_containers contract implementations

template<
    typename tkey,
    typename tvalue>
void binary_search_tree<tkey, tvalue>::set_insertion_strategy(
    typename binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy insertion_strategy) noexcept
{
    _insertion_template->set_insertion_strategy(insertion_strategy);
}

template<
    typename tkey,
    typename tvalue>
void binary_search_tree<tkey, tvalue>::set_removal_strategy(
    typename binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy disposal_strategy) noexcept
{
    _disposal_template->set_disposal_strategy(disposal_strategy);
}

// region iterators requesting implementation

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


// endregion iterators request implementation

// region subtree rotations implementation

template<
    typename tkey,
    typename tvalue>
void binary_search_tree<tkey, tvalue>::small_left_rotation(
    node *&subtree_root,
    bool validate) const
{
    node * parent = subtree_root;
    node * child = subtree_root->right_subtree;

    parent->right_subtree = child->left_subtree;
    child->left_subtree = parent;
    subtree_root = child;
}

template<
    typename tkey,
    typename tvalue>
void binary_search_tree<tkey, tvalue>::small_right_rotation(
   node *&subtree_root,
    bool validate) const
{
    node * parent = subtree_root;//A
    node * child = subtree_root->left_subtree;//B

    parent->left_subtree = child->right_subtree;
    child->right_subtree = parent;
    subtree_root = child;
}

template<
    typename tkey,
    typename tvalue>
void binary_search_tree<tkey, tvalue>::big_left_rotation(
    node *&subtree_root,
    bool validate) const
{
    small_right_rotation(subtree_root->right_subtree);
    small_left_rotation(subtree_root);
}

template<
    typename tkey,
    typename tvalue>
void binary_search_tree<tkey, tvalue>::big_right_rotation(
   node *&subtree_root,
    bool validate) const
{
    small_left_rotation(subtree_root->left_subtree);
    small_right_rotation(subtree_root);
}

template<
    typename tkey,
    typename tvalue>
void binary_search_tree<tkey, tvalue>::double_left_rotation(
    node *&subtree_root,
    bool at_grandparent_first,
    bool validate) const
{
    small_left_rotation(subtree_root->left_subtree);
    small_left_rotation(subtree_root);
}

template<
    typename tkey,
    typename tvalue>
void binary_search_tree<tkey, tvalue>::double_right_rotation(
    binary_search_tree<tkey, tvalue>::node *&subtree_root,
    bool at_grandparent_first,
    bool validate) const
{
    small_right_rotation(subtree_root->right_subtree);
    small_right_rotation(subtree_root);
}

// endregion subtree rotations implementation

#endif //MATH_PRACTICE_AND_OPERATING_SYSTEMS_BINARY_SEARCH_TREE_H