#ifndef MATH_PRACTICE_AND_OPERATING_SYSTEMS_ALLOCATOR_ALLOCATOR_SORTED_LIST_H
#define MATH_PRACTICE_AND_OPERATING_SYSTEMS_ALLOCATOR_ALLOCATOR_SORTED_LIST_H

#include <allocator_guardant.h>
#include <allocator_test_utils.h>
#include <allocator_with_fit_mode.h>
#include <logger_guardant.h>
#include <typename_holder.h>

class allocator_sorted_list final:
    private allocator_guardant,
    public allocator_test_utils,
    public allocator_with_fit_mode,
    private logger_guardant,
    private typename_holder
{

private:
    
    void *_trusted_memory;

public:
    
    ~allocator_sorted_list() override;
    
    allocator_sorted_list(
        allocator_sorted_list const &other) = delete;
    
    allocator_sorted_list &operator=(
        allocator_sorted_list const &other) = delete;
    
    allocator_sorted_list(
        allocator_sorted_list &&other) noexcept;
    
    allocator_sorted_list &operator=(
        allocator_sorted_list &&other) noexcept;

public:
    
    explicit allocator_sorted_list(
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
    
    inline size_t get_allocator_metadata_size() const;

    inline size_t get_free_block_metadata_size() const;

    inline std::mutex& get_sync_object() const;

    inline void* get_first_free_block_address() const;

    inline void allocator_sorted_list::set_first_free_block_address(void* pointer);

    inline allocator_with_fit_mode::fit_mode get_fit_mode() const;

    inline size_t& get_space_size() const;

    inline size_t& allocator_sorted_list::get_free_block_size(void* free_block)  const;

    inline void* allocator_sorted_list::get_free_block_next_block_ptr(void* free_block) const;

    inline void* allocator_sorted_list::get_free_block_trusted_memory(void* free_block) const;

    size_t allocator_sorted_list::get_free_block_minimum_size() const;

    void get_target_blocks_pointers_firstfit(size_t target_size, void* &current_target, void* &previous_target);

    void get_target_blocks_pointers_bestfit(size_t target_size, void* &current_target, void* &previous_target);

    void get_target_blocks_pointers_worstfit(size_t target_size, void* &current_target, void* &previous_target);

    inline void allocator_sorted_list::set_free_block_next_block_ptr(void* free_block, void* ptr) const;

    inline void allocator_sorted_list::set_free_block_size(void* free_block, size_t size) const;

    inline void allocator_sorted_list::set_free_block_trusted_memory(void* free_block) const;

    void allocator_sorted_list::log_blocks_info() const;

    inline void allocator_sorted_list::set_allocator(allocator* a) const;
};

#endif //MATH_PRACTICE_AND_OPERATING_SYSTEMS_ALLOCATOR_ALLOCATOR_SORTED_LIST_H