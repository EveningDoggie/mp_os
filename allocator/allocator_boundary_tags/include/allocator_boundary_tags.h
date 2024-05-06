#ifndef MATH_PRACTICE_AND_OPERATING_SYSTEMS_ALLOCATOR_ALLOCATOR_BOUNDARY_TAGS_H
#define MATH_PRACTICE_AND_OPERATING_SYSTEMS_ALLOCATOR_ALLOCATOR_BOUNDARY_TAGS_H

#include <allocator_guardant.h>
#include <allocator_test_utils.h>
#include <allocator_with_fit_mode.h>
#include <logger_guardant.h>
#include <typename_holder.h>
#include <mutex>

class allocator_boundary_tags final:
    private allocator_guardant,
    public allocator_test_utils,
    public allocator_with_fit_mode,
    private logger_guardant,
    private typename_holder
{

private:
    
    void *_trusted_memory;

public:
    
    ~allocator_boundary_tags() override;
    
    allocator_boundary_tags(
        allocator_boundary_tags const &other) = delete;
    
    allocator_boundary_tags &operator=(
        allocator_boundary_tags const &other) = delete;
    
    allocator_boundary_tags(
        allocator_boundary_tags &&other) noexcept;
    
    allocator_boundary_tags &operator=(
        allocator_boundary_tags &&other) noexcept;

private:

    void deallocate_object_fields();

public:
    
    explicit allocator_boundary_tags(
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

private:

    void * add_finded_block_to_occupied_list(void* previous_block, void* next_block, size_t size_optimal);

public:

    inline allocator_with_fit_mode::fit_mode get_fit_mode() const;

    inline void set_fit_mode(
        allocator_with_fit_mode::fit_mode mode) override;

    inline size_t& get_space_size() const;

    inline size_t& get_avalaible_size() const;

    void increase_avalaible_size(int value) const; 

private:

    inline std::string get_typename() const noexcept override;

    inline logger* get_logger() const override;

    inline allocator* get_allocator() const override;

    inline void set_allocator(allocator* a);

    inline std::mutex& get_sync_object() const;

    inline size_t get_allocator_metadata_size() const;

    void* get_memory_start() const;

    void* get_memory_end() const;

private:

    inline void* get_first_occupied_block_address() const;

    inline void set_first_occupied_block_address(void* pointer);


private:

    inline size_t get_occupied_block_metadata_size() const;

    size_t get_occupied_block_minimum_size() const;

    inline size_t& get_occupied_block_size(void* occupied_block)  const;

    inline void set_occupied_block_size(void* occupied_block, size_t size);

    inline void* get_occupied_block_next_block_ptr(void* occupied_block) const;

    inline void set_occupied_block_next_block_ptr(void* occupied_block, void* ptr);

    inline void* get_occupied_block_previous_block_ptr(void* occupied_block) const;

    inline void set_occupied_block_previous_block_ptr(void* occupied_block, void* ptr);

    inline void* get_occupied_block_trusted_memory(void* occupied_block) const;

    inline void set_occupied_block_trusted_memory(void* fre);


public:

    std::vector<allocator_test_utils::block_info> get_blocks_info() const noexcept override;

private:

    void log_blocks_info() const;

    void log_avalaible_size_info() const;

    void log_current_block_reference_info(void* current) const;
};

#endif //MATH_PRACTICE_AND_OPERATING_SYSTEMS_ALLOCATOR_ALLOCATOR_BOUNDARY_TAGS_H