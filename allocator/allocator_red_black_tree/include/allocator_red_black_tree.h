#ifndef MATH_PRACTICE_AND_OPERATING_SYSTEMS_ALLOCATOR_ALLOCATOR_RED_BLACK_TREE_H
#define MATH_PRACTICE_AND_OPERATING_SYSTEMS_ALLOCATOR_ALLOCATOR_RED_BLACK_TREE_H

#include <allocator_guardant.h>
#include <allocator_test_utils.h>
#include <allocator_with_fit_mode.h>
#include <logger_guardant.h>
#include <typename_holder.h>
#include <mutex>
#include <functional>

class allocator_red_black_tree final:
    private allocator_guardant,
    public allocator_test_utils,
    public allocator_with_fit_mode,
    private logger_guardant,
    private typename_holder
{

#pragma region Object fields

private:
    
    void *_trusted_memory;

    enum Color : bool
    {
        Black = false,
        Red = true
    };

#pragma endregion


#pragma region Object methods

private:

    void deallocate_object_fields();

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

#pragma endregion


#pragma region Memory methods

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

#pragma endregion


#pragma region Metadata allocator methods

public:

    inline size_t& get_space_size() const;

    inline size_t& get_avalaible_size() const;

    inline allocator_with_fit_mode::fit_mode get_fit_mode() const;

    inline void set_fit_mode(
        allocator_with_fit_mode::fit_mode mode) override;

private:
    
    inline std::string get_typename() const noexcept override;

    inline logger* get_logger() const override;

    inline allocator* get_allocator() const override;

    inline void set_allocator(allocator* a);

    void increase_avalaible_size(int value) const;

    inline std::mutex& get_sync_object() const;

    inline size_t get_allocator_metadata_size() const;

    void* get_memory_start() const;

    void* get_memory_end() const;

#pragma endregion


#pragma region Metadata first block methods

private:

    inline void* get_nullptr_parent() const;

    inline void* set_nullptr_parent(void * parent) const;

    inline void* get_root() const;

    inline void set_root(void* pointer);

#pragma endregion


#pragma region Metadata block methods

private:

    inline size_t get_block_minimum_size() const;

    inline size_t get_free_block_metadata_size() const;

    inline size_t get_occupied_block_metadata_size() const;

    inline void* get_block_next(void* block) const;

    inline void set_block_next(void* block, void* ptr);

    inline void* get_block_previous(void* block) const;

    inline void set_block_previous(void* block, void* ptr);

    inline void* get_block_trusted_memory(void* block) const;

    inline void set_block_trusted_memory(void* free);

    inline Color get_block_color(void* block)  const;

    inline void set_block_color(void* block, Color color);

    inline size_t get_block_size(void* block)  const;

    inline void set_block_size(void* block, size_t size);

    inline void* get_block_parent(void* block) const;

    inline void set_block_parent(void* block, void* ptr);

    inline void* get_block_left_child(void* block) const;

    inline void set_block_left_child(void* block, void* ptr);

    inline void* get_block_right_child(void* block) const;

    inline void set_block_right_child(void* block, void* ptr);



#pragma endregion


#pragma region Log methods

public:
    
    std::vector<allocator_test_utils::block_info> get_blocks_info() const noexcept override;

private:

        void log_blocks_info() const;

#pragma endregion

#pragma region Tree methods


    void* tree_search(void* node, size_t size, std::function<bool(int, int)>compare);

    void* tree_minimum(void* node);

    void* tree_maximum(void* node);

    void* tree_successor(void* node);

    void* tree_predecessor(void* node);

    void tree_left_rotate(void* node);

    void tree_right_rotate(void* node);

    void tree_insert(void* node, std::function<int(int, int)>compare);

    void tree_insert_fixup(void* node, std::function<int(int, int)>compare);

    void* tree_delete(void* node, std::function<int(int, int)>compare);

    void tree_delete_fixup(void* node, void* saved_parent, std::function<int(int, int)>compare);

#pragma endregion

};

#endif //MATH_PRACTICE_AND_OPERATING_SYSTEMS_ALLOCATOR_ALLOCATOR_RED_BLACK_TREE_H