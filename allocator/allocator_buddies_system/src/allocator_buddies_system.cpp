#include <not_implemented.h>
#include "../include/allocator_buddies_system.h"


#pragma region Object methods

void allocator_buddies_system::deallocate_object_fields()
{

    auto* logger = get_logger();
    if (logger != nullptr) logger->trace("Called method void allocator_boundary_tags::deallocate_object_fields()");

    if (_trusted_memory != nullptr)
    {
        allocator* a = get_allocator();
        if (a != nullptr)
            a->deallocate(_trusted_memory);
        else
            ::delete a;

        _trusted_memory = nullptr; //после удаления не забывать
    }

    if (logger != nullptr) logger->trace("Successfully executed method void allocator_boundary_tags::deallocate_object_fields()");
}

allocator_buddies_system::~allocator_buddies_system()
{
    auto* logger = get_logger();
    if (logger != nullptr) logger->debug("Called method allocator_boundary_tags::~allocator_boundary_tags()");

    deallocate_object_fields();

    if (logger != nullptr) logger->debug("Successfully executed method allocator_boundary_tags::~allocator_boundary_tags()");
}

allocator_buddies_system::allocator_buddies_system(
    allocator_buddies_system &&other) noexcept
{
    _trusted_memory = other._trusted_memory;
    other._trusted_memory = nullptr;

    debug_with_guard("Called method allocator_boundary_tags::allocator_boundary_tags(allocator_boundary_tags && other) noexcept");
    debug_with_guard("Successfully executed method allocator_boundary_tags::allocator_boundary_tags(allocator_boundary_tags && other) noexcept");
}

allocator_buddies_system &allocator_buddies_system::operator=(
    allocator_buddies_system &&other) noexcept
{
    debug_with_guard("Called method allocator_boundary_tags& allocator_boundary_tags::operator=(allocator_boundary_tags && other) noexcept");

    if (&other != this)
    {
        deallocate_object_fields();                   //НЕ ССЫЛАТЬСЯ ТУТ НА КОНСТРУКТОР. Объект уже есть. Это плохо
        _trusted_memory = other._trusted_memory;      //не забывать тут чистить
        other._trusted_memory = nullptr;
    }

    debug_with_guard("Successfully executed method allocator_boundary_tags& allocator_boundary_tags::operator=(allocator_boundary_tags && other) noexcept");
    return *this;
}

#pragma endregion

#pragma region Memory methods

allocator_buddies_system::allocator_buddies_system(
    size_t space_size,
    allocator *parent_allocator,
    logger *logger,
    allocator_with_fit_mode::fit_mode allocate_fit_mode)
{
    throw not_implemented("allocator_buddies_system::allocator_buddies_system(size_t, allocator *, logger *, allocator_with_fit_mode::fit_mode)", "your code should be here...");
}

[[nodiscard]] void *allocator_buddies_system::allocate(
    size_t value_size,
    size_t values_count)
{
    throw not_implemented("[[nodiscard]] void *allocator_buddies_system::allocate(size_t, size_t)", "your code should be here...");
}

void allocator_buddies_system::deallocate(
    void *at)
{
    throw not_implemented("void allocator_buddies_system::deallocate(void *)", "your code should be here...");
}

#pragma endregion

#pragma region Metadata allocator methods

inline void allocator_buddies_system::set_fit_mode(
    allocator_with_fit_mode::fit_mode mode)
{
    throw not_implemented("inline void allocator_buddies_system::set_fit_mode(allocator_with_fit_mode::fit_mode)", "your code should be here...");
}

inline std::string allocator_buddies_system::get_typename() const noexcept
{
    throw not_implemented("inline std::string allocator_buddies_system::get_typename() const noexcept", "your code should be here...");
}

inline logger* allocator_buddies_system::get_logger() const
{
    throw not_implemented("inline logger *allocator_buddies_system::get_logger() const", "your code should be here...");
}

inline allocator* allocator_buddies_system::get_allocator() const
{
    throw not_implemented("inline allocator *allocator_buddies_system::get_allocator() const", "your code should be here...");
}

#pragma endregion

#pragma region Metadata first_occupied_block methods

#pragma endregion

#pragma region Metadata occupied_block methods

#pragma endregion

#pragma region Log

std::vector<allocator_test_utils::block_info> allocator_buddies_system::get_blocks_info() const noexcept
{
    throw not_implemented("std::vector<allocator_test_utils::block_info> allocator_buddies_system::get_blocks_info() const noexcept", "your code should be here...");
}

#pragma endregion

