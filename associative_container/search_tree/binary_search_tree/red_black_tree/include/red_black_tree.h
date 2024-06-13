#ifndef MATH_PRACTICE_AND_OPERATING_SYSTEMS_RED_BLACK_TREE_H
#define MATH_PRACTICE_AND_OPERATING_SYSTEMS_RED_BLACK_TREE_H

#include <binary_search_tree.h>

template<
    typename tkey,
    typename tvalue>
class red_black_tree final:
    public binary_search_tree<tkey, tvalue>
{

public:
    
    enum class node_color
    {
        RED,
        BLACK
    };

private:
    
    struct node final:
        binary_search_tree<tkey, tvalue>::node
    {
    public:

        node_color color;

        explicit node(
            tkey const &key,
            tvalue const &value);

        explicit node(
            tkey const &key,
            tvalue &&value);
    };

public:
    
    struct iterator_data final:
        public binary_search_tree<tkey, tvalue>::iterator_data
    {

    public:

        node_color color;
    
    public:
        
        explicit iterator_data(
            unsigned int depth,
            tkey const &key,
            tvalue const &value,
            node_color color);

        explicit iterator_data(
            unsigned int depth,
            node const * node);
    };



private:

    class insertion_template_method final:
        public binary_search_tree<tkey, tvalue>::insertion_template_method
    {
    
    public:

        explicit insertion_template_method(
            red_black_tree<tkey, tvalue> *tree,
            typename binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy insertion_strategy);
    
    private:

         void balance_execute() override;
        
    };
    
    class obtaining_template_method final:
        public binary_search_tree<tkey, tvalue>::obtaining_template_method
    {
    
    public:
        
        explicit obtaining_template_method(
            red_black_tree<tkey, tvalue> *tree);
    };
    
    class disposal_template_method final:
        public binary_search_tree<tkey, tvalue>::disposal_template_method
    {
    
    public:
        
        explicit disposal_template_method(
            red_black_tree<tkey, tvalue> *tree,
            typename binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy disposal_strategy);
        
        void balance_execute() override;
        
    };

public:
    
    explicit red_black_tree(
        allocator *allocator = nullptr,
        logger *logger = nullptr,
        typename binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy insertion_strategy = binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy::throw_an_exception,
        typename binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy disposal_strategy = binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy::throw_an_exception);


private:

    static int key_comparer(tkey const &first_key, tkey const &second_key);

public:

    ~red_black_tree() noexcept final;
    
    red_black_tree(
        red_black_tree<tkey, tvalue> const &other);
    
    red_black_tree<tkey, tvalue> &operator=(
        red_black_tree<tkey, tvalue> const &other);
    
    red_black_tree(
        red_black_tree<tkey, tvalue> &&other) noexcept;
    
    red_black_tree<tkey, tvalue> &operator=(
        red_black_tree<tkey, tvalue> &&other) noexcept;
    
};

template<typename tkey, typename tvalue>
red_black_tree<tkey, tvalue>::node::node(tkey const &key, tvalue const &value):
binary_search_tree<tkey, tvalue>(key, value),
color(node_color::RED)
{

}

template<typename tkey, typename tvalue>
red_black_tree<tkey, tvalue>::node::node(tkey const &key, tvalue &&value):
binary_search_tree<tkey, tvalue>(key, value),
color(node_color::RED)
{

}

template<
    typename tkey,
    typename tvalue>
red_black_tree<tkey, tvalue>::iterator_data::iterator_data(
    unsigned int depth,
    tkey const &key,
    tvalue const &value,
    typename red_black_tree<tkey, tvalue>::node_color color):
    binary_search_tree<tkey, tvalue>::iterator_data(depth, key, value),
    color(color)
{

}

template<typename tkey, typename tvalue>
red_black_tree<tkey, tvalue>::iterator_data::iterator_data(unsigned int depth, node const *node):
iterator_data(depth, node->key, node->value, node->color)
{

}

template<
    typename tkey,
    typename tvalue>
red_black_tree<tkey, tvalue>::insertion_template_method::insertion_template_method(
    red_black_tree<tkey, tvalue> *tree,
    typename binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy insertion_strategy):
    binary_search_tree<tkey, tvalue>::insertion_template_method(tree, insertion_strategy)
{

}

template<typename tkey, typename tvalue>
void red_black_tree<tkey, tvalue>::insertion_template_method::balance_execute()
{
   // if(insertion_template_method::template_method_basics::_order_stack.size()==0)
//   auto var = static_cast<node*>(red_black_tree::insertion_template_method::_order_stack.top())->color;
   // binary_search_tree<tkey, tvalue>::stack_kek.top()
   // binary_search_tree<tkey,tvalue>
   /// insertion_template_method::template_method_basics::get_logger()->trace("KEK");
  ///   (*static_cast<node**>(&insertion_template_method::template_method_basics::_order_stack.top()))->color=node_color::RED;

  //  insertion_template_method::template_method_basics::get_logger()->trace("KEK");

   // auto var =  insertion_template_method::template_method_basics::_order_stack.top();
   // auto v = dynamic_cast<red_black_tree<tkey, tvalue>::node*>(var);

  // auto var = _order_stack.size();
  //  insertion_template_method::template_method_basics::get_logger()->trace(std::to_string(var));

    auto var = dynamic_cast<red_black_tree::node*>(insertion_template_method::template_method_basics::_order_stack.top());
}

template<
    typename tkey,
    typename tvalue>
red_black_tree<tkey, tvalue>::obtaining_template_method::obtaining_template_method(red_black_tree<tkey, tvalue> *tree):
binary_search_tree<tkey, tvalue>::obtaining_template_method(tree)
{

}

template<
    typename tkey,
    typename tvalue>
red_black_tree<tkey, tvalue>::disposal_template_method::disposal_template_method(
    red_black_tree<tkey, tvalue> *tree,
    typename binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy disposal_strategy):
    binary_search_tree<tkey, tvalue>::disposal_template_method(tree, disposal_strategy)
{

}

template<typename tkey, typename tvalue>
void red_black_tree<tkey, tvalue>::disposal_template_method::balance_execute()
{

}

template<
    typename tkey,
    typename tvalue>
red_black_tree<tkey, tvalue>::red_black_tree(
    allocator *allocator,
    logger *logger,
    typename binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy insertion_strategy,
    typename binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy disposal_strategy):
binary_search_tree<tkey, tvalue>::binary_search_tree(
        new red_black_tree<tkey, tvalue>::insertion_template_method(this, insertion_strategy),
        new red_black_tree<tkey, tvalue>::obtaining_template_method(this),
        new red_black_tree<tkey, tvalue>::disposal_template_method(this, disposal_strategy),
        key_comparer,
        allocator,
        logger)
{

}

template<typename tkey, typename tvalue>
int red_black_tree<tkey, tvalue>::key_comparer(tkey const &first_key, tkey const &second_key)
{
    if(first_key==second_key)
        return 0;

    if(first_key>second_key)
        return 1;

    return -1;
}

template<
    typename tkey,
    typename tvalue>
red_black_tree<tkey, tvalue>::~red_black_tree() noexcept
{
    throw not_implemented("template<typename tkey, typename tvalue> red_black_tree<tkey, tvalue>::~red_black_tree() noexcept", "your code should be here...");
}

template<
    typename tkey,
    typename tvalue>
red_black_tree<tkey, tvalue>::red_black_tree(
    red_black_tree<tkey, tvalue> const &other)
{
    throw not_implemented("template<typename tkey, typename tvalue> red_black_tree<tkey, tvalue>::red_black_tree(red_black_tree<tkey, tvalue> const &)", "your code should be here...");
}

template<
    typename tkey,
    typename tvalue>
red_black_tree<tkey, tvalue> &red_black_tree<tkey, tvalue>::operator=(
    red_black_tree<tkey, tvalue> const &other)
{
    throw not_implemented("template<typename tkey, typename tvalue> red_black_tree<tkey, tvalue> &red_black_tree<tkey, tvalue>::operator=(red_black_tree<tkey, tvalue> const &)", "your code should be here...");
}

template<
    typename tkey,
    typename tvalue>
red_black_tree<tkey, tvalue>::red_black_tree(
    red_black_tree<tkey, tvalue> &&other) noexcept
{
    throw not_implemented("template<typename tkey, typename tvalue> red_black_tree<tkey, tvalue>::red_black_tree(red_black_tree<tkey, tvalue> &&) noexcept", "your code should be here...");
}

template<
    typename tkey,
    typename tvalue>
red_black_tree<tkey, tvalue> &red_black_tree<tkey, tvalue>::operator=(
    red_black_tree<tkey, tvalue> &&other) noexcept
{
    throw not_implemented("template<typename tkey, typename tvalue> red_black_tree<tkey, tvalue> &red_black_tree<tkey, tvalue>::operator=(red_black_tree<tkey, tvalue> &&) noexcept", "your code should be here...");
}

#endif //MATH_PRACTICE_AND_OPERATING_SYSTEMS_RED_BLACK_TREE_H
