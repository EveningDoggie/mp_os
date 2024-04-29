#include <not_implemented.h>

#include "../include/allocator_global_heap.h"

allocator_global_heap::allocator_global_heap(
    logger *logger):
    _logger(logger)
{
    
}

allocator_global_heap::~allocator_global_heap()
{
    
}


[[nodiscard]] void * allocator_global_heap::allocate(
    size_t value_size,
    size_t values_count)
{
    debug_with_guard("Called method [[nodiscard]] void *allocator_global_heap::allocate(size_t value_size, size_t values_count)");

    try
    {
        size_t size = values_count * value_size;
        auto result = ::operator new (sizeof(allocator*) + sizeof(size_t) + size);

        allocator ** allocator_object_ptr = reinterpret_cast<allocator**>(result);
        *allocator_object_ptr = this;

        size_t * block_size_ptr = reinterpret_cast<size_t*>(allocator_object_ptr+1);
        *block_size_ptr = size;
        
        debug_with_guard("Successfully executed method [[nodiscard]] void *allocator_global_heap::allocate(size_t value_size, size_t values_count)");
        
        return reinterpret_cast<void *>(block_size_ptr + 1);

    }
    catch (std::bad_alloc const &ex)
    {
        error_with_guard(std::string("Failed to perfom method [[nodiscard]] void *allocator_global_heap::allocate(size_t value_size, size_t values_count): exception of type std::badalloc with an error: ")  + ex.what());
        debug_with_guard(std::string("Cancel execute method [[nodiscard]] void *allocator_global_heap::allocate(size_t value_size, size_t values_count) with exception of type std::badalloc with an error: ") + ex.what());
    }
}

void allocator_global_heap::deallocate(
    void *at)
{
    debug_with_guard("Called method void allocator_global_heap::deallocate(void* at)");

    size_t * block_size_ptr = reinterpret_cast<size_t*>(at) - 1;
    allocator ** allocator_object_ptr = reinterpret_cast<allocator**>(block_size_ptr)-1;
    

    std::string bytes_str;
    auto char_ptr = reinterpret_cast<unsigned char*>(at);
    for (int i = 0; i < *block_size_ptr; ++i)
    {
        bytes_str += std::to_string(*char_ptr);
        ++char_ptr;
    }
    debug_with_guard(std::string("Block state before deallocation: ")+bytes_str);


    if (*allocator_object_ptr != this)
    {
        error_with_guard(std::string("Failed to perfom method void allocator_global_heap::deallocate: exception of type std::badalloc with an error: block can't be deallocated: invalid pointer"));
        debug_with_guard(std::string("Cancel execute method void allocator_global_heap::deallocate with exception of type std::badalloc with an error: block can't be deallocated: invalid pointer"));
        throw std::logic_error("Block can't be deallocated: invalid pointer");
    }

    ::operator delete(allocator_object_ptr);

    debug_with_guard("Successfully executed method void allocator_global_heap::deallocate(void* at) ");

}

inline logger *allocator_global_heap::get_logger() const
{
    return _logger;
}

inline std::string allocator_global_heap::get_typename() const noexcept
{
    return "allocator_global_heap";
}