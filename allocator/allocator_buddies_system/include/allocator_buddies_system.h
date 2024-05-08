#ifndef MATH_PRACTICE_AND_OPERATING_SYSTEMS_ALLOCATOR_ALLOCATOR_BUDDIES_SYSTEM_H
#define MATH_PRACTICE_AND_OPERATING_SYSTEMS_ALLOCATOR_ALLOCATOR_BUDDIES_SYSTEM_H

#include <allocator_guardant.h>
#include <allocator_test_utils.h>
#include <allocator_with_fit_mode.h>
#include <logger_guardant.h>
#include <typename_holder.h>
#include <mutex>

class allocator_buddies_system final:
    private allocator_guardant,
    public allocator_test_utils,
    public allocator_with_fit_mode,
    private logger_guardant,
    private typename_holder
{

#pragma region Object fields

private:
    
    void *_trusted_memory;

#pragma endregion


#pragma region Object methods

private:

    void deallocate_object_fields();

public:
    
    ~allocator_buddies_system() override;
    
    allocator_buddies_system(
        allocator_buddies_system const &other) = delete;
    
    allocator_buddies_system &operator=(
        allocator_buddies_system const &other) = delete;
    
    allocator_buddies_system(
        allocator_buddies_system &&other) noexcept;
    
    allocator_buddies_system &operator=(
        allocator_buddies_system &&other) noexcept;

#pragma endregion


#pragma region Memory methods

public:
    
    explicit allocator_buddies_system(
        size_t space_size_power_of_two,
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

    void* get_buddy(void* block) const;

#pragma endregion


#pragma region Metadata allocator methods

public:

    inline size_t get_space_size() const;

    inline size_t& get_space_in_pow() const;

    inline size_t& get_avalaible_size() const;

    inline allocator_with_fit_mode::fit_mode get_fit_mode() const;

    inline void set_fit_mode(
        allocator_with_fit_mode::fit_mode mode) override;

private:
    
    inline std::string get_typename() const noexcept override;

    inline logger* get_logger() const override;

    inline allocator *get_allocator() const override;

    inline void set_allocator(allocator* a);

    void increase_avalaible_size(int value) const;

    inline std::mutex& get_sync_object() const;

    inline size_t get_allocator_metadata_size() const;

    void* get_memory_start() const;

    void* get_memory_end() const;


#pragma endregion


#pragma region Metadata first_free_block methods

private:

    inline void* get_first_free_block_address() const;

    inline void set_first_free_block_address(void* pointer);

#pragma endregion


#pragma region Metadata free_block methods

private:

    inline size_t get_free_block_metadata_size() const;

    inline size_t get_occupied_block_metadata_size() const;

    inline void* get_block_trusted_memory(void* block) const;

    inline void set_block_trusted_memory(void* block);

    inline size_t get_block_size(void* block)  const;

    inline void set_block_pow(void* block, size_t size);

    inline size_t& get_block_pow(void* block) const;

    inline bool get_block_flag(void* block)  const;

    inline void set_block_flag(void* block, bool isOccupied);

    inline void* get_block_next_block_ptr(void* block) const;

    inline void set_block_next_block_ptr(void* block, void* ptr);

    inline void* get_block_previous_block_ptr(void* occupied_block) const;

    inline void set_block_previous_block_ptr(void* occupied_block, void* ptr);

#pragma endregion


#pragma region Log methods

public:
    
    std::vector<allocator_test_utils::block_info> get_blocks_info() const noexcept override;

private:

    void log_blocks_info() const;

#pragma endregion


#pragma region Helpers

    inline size_t get_pow_from_value(size_t value) const;

    inline size_t get_value_from_pow(size_t pow) const;

    inline bool get_bit_from_byte(void* block, size_t shift_mask) const;

    inline void set_bit_in_byte(void* block, size_t shift_mask, bool byte);

    std::string byte_to_string(void* block) const;

#pragma endregion

};

#endif //MATH_PRACTICE_AND_OPERATING_SYSTEMS_ALLOCATOR_ALLOCATOR_BUDDIES_SYSTEM_H
