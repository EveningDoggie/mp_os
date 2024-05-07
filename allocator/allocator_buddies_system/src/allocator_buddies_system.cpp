#include <not_implemented.h>
#include "../include/allocator_buddies_system.h"

//ВАЖНО. ПЕРЕПРОВЕРИТЬ ВСЕ.
//В БЛОКЕ ХРАНИТСЯ СТЕПЕНЬ В 1 БИТЕ
//В АЛЛОКАТОРЕ ХОРАНИТСЯ ТОЖЕ СТЕПЕНЬ НО В SIZE_T


//+
#pragma region Object methods

void allocator_buddies_system::deallocate_object_fields()
{

    auto* logger = get_logger();
    if (logger != nullptr) logger->trace("Called method void allocator_buddies_system::deallocate_object_fields()");

    if (_trusted_memory != nullptr)
    {
        allocator* a = get_allocator();
        if (a != nullptr)
            a->deallocate(_trusted_memory);
        else
            ::delete a;

        _trusted_memory = nullptr; 
    }

    if (logger != nullptr) logger->trace("Successfully executed method void allocator_buddies_system::deallocate_object_fields()");
}

allocator_buddies_system::~allocator_buddies_system()
{
    auto* logger = get_logger();
    if (logger != nullptr) logger->debug("Called method allocator_buddies_system::~allocator_buddies_system()");

    deallocate_object_fields();

    if (logger != nullptr) logger->debug("Successfully executed method allocator_buddies_system::~allocator_buddies_system()");
}

allocator_buddies_system::allocator_buddies_system(
    allocator_buddies_system &&other) noexcept
{
    _trusted_memory = other._trusted_memory;
    other._trusted_memory = nullptr;

    debug_with_guard("Called method allocator_buddies_system::allocator_buddies_system(allocator_buddies_system && other) noexcept");
    debug_with_guard("Successfully executed method allocator_buddies_system::allocator_buddies_system(allocator_buddies_system && other) noexcept");
}

allocator_buddies_system &allocator_buddies_system::operator=(
    allocator_buddies_system &&other) noexcept
{
    debug_with_guard("Called method allocator_buddies_system& allocator_buddies_system::operator=(allocator_buddies_system && other) noexcept");

    if (&other != this)
    {
        deallocate_object_fields();                   
        _trusted_memory = other._trusted_memory;    
        other._trusted_memory = nullptr;
    }

    debug_with_guard("Successfully executed method allocator_buddies_system& allocator_buddies_system::operator=(allocator_buddies_system && other) noexcept");
    return *this;
}

#pragma endregion

//-
#pragma region Memory methods

allocator_buddies_system::allocator_buddies_system(
    size_t space_size,
    allocator *parent_allocator,
    logger *logger,
    allocator_with_fit_mode::fit_mode allocate_fit_mode)
{
    if (logger != nullptr) logger->debug("Called method allocator_buddies_system::allocator_buddies_system(size_t space_size, allocator * parent_allocator,logger * logger,allocator_with_fit_mode::fit_mode allocate_fit_mode) with request: memory:" + std::to_string(space_size) + ", metadata:" + std::to_string(get_allocator_metadata_size()));

    //!!!!
    size_t space_size_with_metadata = space_size + get_allocator_metadata_size();

    try
    {
        if (logger != nullptr) logger->warning("The amount of allocated memory has been overridden in method allocator_buddies_system::allocator_buddies_system(size_t space_size, allocator * parent_allocator,logger * logger,allocator_with_fit_mode::fit_mode allocate_fit_mode): allocator's metadata and free block metadata added");
        _trusted_memory =
            parent_allocator != nullptr ?
            parent_allocator->allocate(space_size_with_metadata, 1) :
            ::operator new(space_size_with_metadata);
    }
    catch (std::bad_alloc const& ex)
    {
        if (logger != nullptr) logger->error(std::string("Failed to perfom method allocator_buddies_system::allocator_buddies_system(size_t space_size, allocator * parent_allocator,logger * logger,allocator_with_fit_mode::fit_mode allocate_fit_mode): exception of type std::badalloc with an error: std::bad_alloc: ") + ex.what());
        if (logger != nullptr) logger->debug(std::string("Cancel with error execute method allocator_buddies_system::allocator_buddies_system(size_t space_size, allocator * parent_allocator,logger * logger,allocator_with_fit_mode::fit_mode allocate_fit_mode) with exception of type std::badalloc with an error: std::bad_alloc: ") + ex.what());
        throw;
    }


    allocator** parent_allocator_ptr = reinterpret_cast<allocator**>(_trusted_memory);
    *parent_allocator_ptr = parent_allocator;

    class logger** logger_ptr = reinterpret_cast<class logger**>(parent_allocator_ptr + 1);
    *logger_ptr = logger;

    size_t* space_size_ptr = reinterpret_cast<size_t*>(logger_ptr + 1);
    //!!!!!!!!! хранить степень. в size_t
    *space_size_ptr = space_size;

    std::mutex* sync_object_ptr = reinterpret_cast<std::mutex*>(space_size_ptr + 1);
    allocator::construct(sync_object_ptr);

    allocator_with_fit_mode::fit_mode* fit_mode_ptr = reinterpret_cast<allocator_with_fit_mode::fit_mode*>(sync_object_ptr + 1);
    *fit_mode_ptr = allocate_fit_mode;

    void** first_block_ptr = reinterpret_cast<void**>(fit_mode_ptr + 1);
    size_t* avalaible_size = reinterpret_cast<size_t*>(first_block_ptr + 1);

    *first_block_ptr = reinterpret_cast<void*>(avalaible_size + 1); 
    //!!!!!!!!!
    *avalaible_size = space_size - get_block_metadata_size(); 


    /*
    size_t* firstblock_size_ptr = reinterpret_cast<size_t*>(*first_block_ptr);
    *firstblock_size_ptr = space_size - get_block_metadata_size();

    void** firstblock_trusted_memory_ptr = reinterpret_cast<void**>(firstblock_size_ptr + 1);
    *firstblock_trusted_memory_ptr = _trusted_memory;

    void** firstblock_next_block_ptr = reinterpret_cast<void**>(firstblock_trusted_memory_ptr + 1);
    *firstblock_next_block_ptr = nullptr;
    */
    log_blocks_info();
    if (logger != nullptr) logger->debug("Succesfully executed method allocator_buddies_system::allocator_buddies_system(size_t space_size, allocator * parent_allocator,logger * logger,allocator_with_fit_mode::fit_mode allocate_fit_mode)");
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

void* allocator_buddies_system::get_buddy(void* block) const
{
    size_t b = reinterpret_cast<char*>(block) - reinterpret_cast<char*>(get_memory_start());
    //применить [+]

    return reinterpret_cast<void*>(reinterpret_cast<unsigned char*>(get_memory_start()) + b);
}


#pragma endregion

//+
#pragma region Metadata allocator methods

inline size_t& allocator_buddies_system::get_space_pow() const
{
    trace_with_guard("Called method inline size_t& allocator_buddies_system::get_space_pow() const");
    trace_with_guard("Successfully executed method inline size_t& allocator_buddies_system::get_space_pow() const");

    return *reinterpret_cast<size_t*>(
        reinterpret_cast<unsigned char*>(_trusted_memory)
        + sizeof(allocator*)
        + sizeof(logger*));
}

inline size_t allocator_buddies_system::get_space_size() const
{
    trace_with_guard("Called method inline size_t& allocator_buddies_system::get_space_size() const");
    trace_with_guard("Successfully executed method inline size_t& allocator_buddies_system::get_space_size() const");

    return get_value_from_pow(get_space_pow());
}

inline size_t& allocator_buddies_system::get_avalaible_size() const
{
    trace_with_guard("Called method inline size_t& allocator_buddies_system::get_avalaible_size() const");
    trace_with_guard("Successfully executed method inline size_t& allocator_buddies_system::get_avalaible_size() const");

    return *reinterpret_cast<size_t*>(
        reinterpret_cast<unsigned char*>(_trusted_memory)
        + sizeof(allocator*)
        + sizeof(logger*)
        + sizeof(size_t)
        + sizeof(std::mutex)
        + sizeof(allocator_with_fit_mode::fit_mode)
        + sizeof(size_t));
}

inline allocator_with_fit_mode::fit_mode allocator_buddies_system::get_fit_mode() const
{
    trace_with_guard("Called method inline allocator_with_fit_mode::fit_mode allocator_buddies_system::get_fit_mode() const");
    trace_with_guard("Successfully executed method inline allocator_with_fit_mode::fit_mode allocator_buddies_system::get_fit_mode() const");
    return *reinterpret_cast<fit_mode*>(
        reinterpret_cast<unsigned char*>(_trusted_memory)
        + sizeof(allocator*)
        + sizeof(logger*)
        + sizeof(size_t)
        + sizeof(std::mutex));
}

inline void allocator_buddies_system::set_fit_mode(
    allocator_with_fit_mode::fit_mode mode)
{
    debug_with_guard("Called method inline void allocator_buddies_system::set_fit_mode(allocator_with_fit_mode::fit_mode mode)");
    *reinterpret_cast<fit_mode*>(
        reinterpret_cast<unsigned char*>(_trusted_memory)
        + sizeof(allocator*)
        + sizeof(logger*)
        + sizeof(size_t)
        + sizeof(std::mutex)) = mode;
    debug_with_guard("Successfully executed method inline void allocator_buddies_system::set_fit_mode(allocator_with_fit_mode::fit_mode mode)");
}

inline std::string allocator_buddies_system::get_typename() const noexcept
{
    debug_with_guard("Called method inline std::string allocator_buddies_system::get_typename() const noexcept");
    debug_with_guard("Successfully executed method inline std::string allocator_buddies_system::get_typename() const noexcept");
    return "string allocator_buddies_system";
}

inline logger* allocator_buddies_system::get_logger() const
{
    return *reinterpret_cast<logger**>(
        reinterpret_cast<unsigned char*>(_trusted_memory)
        + sizeof(allocator*));
}

inline allocator* allocator_buddies_system::get_allocator() const
{
    debug_with_guard("Called method inline allocator* allocator_buddies_system::get_allocator() const");
    debug_with_guard("Successfully executed method inline allocator* allocator_buddies_system::get_allocator() const");
    return *reinterpret_cast<allocator**>(_trusted_memory);
}

inline void allocator_buddies_system::set_allocator(allocator* a)
{
    trace_with_guard("Called method inline void allocator_buddies_system::set_allocator(allocator * a) const");
    *reinterpret_cast<allocator**>(_trusted_memory) = a;
    debug_with_guard("Successfully executed method inline void allocator_buddies_system::set_allocator(allocator * a) const");
}

inline void allocator_buddies_system::increase_avalaible_size(int value) const
{
    trace_with_guard("Called method inline void allocator_buddies_system::increase_avalaible_size(int value) const");
    trace_with_guard("Successfully executed method inline void allocator_buddies_system::increase_avalaible_size(int value) const");

    *reinterpret_cast<size_t*>(
        reinterpret_cast<unsigned char*>(_trusted_memory)
        + sizeof(allocator*)
        + sizeof(logger*)
        + sizeof(size_t)
        + sizeof(std::mutex)
        + sizeof(allocator_with_fit_mode::fit_mode)
        + sizeof(size_t)) += value;
}

inline std::mutex& allocator_buddies_system::get_sync_object() const
{
    trace_with_guard("Called method inline std::mutex& allocator_buddies_system::get_sync_object() const");
    trace_with_guard("Successfully executed method inline std::mutex& allocator_buddies_system::get_sync_object() const");
    return *reinterpret_cast<std::mutex*>(
        reinterpret_cast<unsigned char*>(_trusted_memory)
        + sizeof(allocator*)
        + sizeof(logger*)
        + sizeof(size_t));
}

size_t allocator_buddies_system::get_allocator_metadata_size() const
{
    //аллокатор, логгер, размер кучи, мьютекс, фитмод, указатель на 1 элемент, размер свободной памяти
    //allocator*, logger*, size_t, mutex, allocator_with_fit_mode::fit_mode, void*
    //size_ptr, void* trusted, void* next
    return sizeof(allocator*) + sizeof(logger*) + sizeof(size_t) + sizeof(std::mutex) + sizeof(allocator_with_fit_mode::fit_mode) + sizeof(void*) + sizeof(size_t);
}

void* allocator_buddies_system::get_memory_start() const
{
    return reinterpret_cast<unsigned char*>(_trusted_memory) + get_allocator_metadata_size();
}


void* allocator_buddies_system::get_memory_end() const
{
    return reinterpret_cast<unsigned char*>(get_memory_start()) + get_space_size();
}

#pragma endregion

//+
#pragma region Metadata first_free_block methods

inline void* allocator_buddies_system::get_first_free_block_address() const
{
    trace_with_guard("Called method inline void* allocator_buddies_system::get_first_free_block_address() const");
    trace_with_guard("Successfully executed method inline void* allocator_buddies_system::get_first_free_block_address() const");
    return *reinterpret_cast<void**>(
        reinterpret_cast<unsigned char*>(_trusted_memory)
        + sizeof(allocator*)
        + sizeof(logger*)
        + sizeof(size_t)
        + sizeof(std::mutex)
        + sizeof(allocator_with_fit_mode::fit_mode));
}

inline void allocator_buddies_system::set_first_free_block_address(void* pointer)
{
    trace_with_guard("Called method inline void allocator_buddies_system::set_first_free_block_address(void * pointer)");
    trace_with_guard("Successfully executed method inline void allocator_buddies_system::set_first_free_block_address(void * pointer)");
    *reinterpret_cast<void**>(
        reinterpret_cast<unsigned char*>(_trusted_memory)
        + sizeof(allocator*)
        + sizeof(logger*)
        + sizeof(size_t)
        + sizeof(std::mutex)
        + sizeof(allocator_with_fit_mode::fit_mode)) = pointer;
}

#pragma endregion

//+
#pragma region Metadata occupied_block methods

inline size_t allocator_buddies_system::get_free_block_metadata_size() const
{
    return 1 + sizeof(void*) * 3; //1 байт для хранения размера и флага занятости, указатель на начало памяти, на след свободный и на пред свободный
}

inline size_t allocator_buddies_system::get_occupied_block_metadata_size() const
{
    return 1 + sizeof(void*); //1 байт для хранения размера и флага занятости, указатель на начало памяти
}

inline void* allocator_buddies_system::get_block_trusted_memory(void* block) const
{
    trace_with_guard("Called method inline void* allocator_buddies_system::get_block_trusted_memory(void* block) const");
    trace_with_guard("Successfully executed method inline void* allocator_buddies_system::get_block_trusted_memory(void* block) const");
    return *reinterpret_cast<void**>(
        reinterpret_cast<unsigned char*>(block)
        + 1);
}

inline void allocator_buddies_system::set_block_trusted_memory(void* block)
{
    trace_with_guard("Called method inline void allocator_buddies_system::set_block_trusted_memory(void* block) const");
    trace_with_guard("Successfully executed method inline void allocator_buddies_system::set_block_trusted_memory(void* block) const");
   *reinterpret_cast<void**>(
        reinterpret_cast<unsigned char*>(block)
        + 1) = _trusted_memory;
        
}

inline size_t allocator_buddies_system::get_block_size(void* block) const
{
    return get_value_from_pow(get_block_pow(block));
}

inline void allocator_buddies_system::set_block_pow(void* block, size_t size)
{
    trace_with_guard("Called method inline void allocator_buddies_system::set_block_size(void* block, size_t size) const");
    trace_with_guard("Successfully executed method inline void allocator_buddies_system::set_block_size(void* block, size_t size) const");
    
    int shift = 1;
    for (int i = 0; i < 7; i++)
    {
        set_bit_in_byte(block, shift, shift & size);
        shift <<= 1;
    }

}

inline size_t& allocator_buddies_system::get_block_pow(void* block) const
{
    trace_with_guard("Called method inline size_t& allocator_buddies_system::get_block_size(void * block) const");
    trace_with_guard("Successfully executed method inline size_t& allocator_buddies_system::get_block_size(void * block) const");

    size_t size = 0;
    for (int i = 0; i < 7; i++) {
        int value = 1 << i;
        size += value * get_bit_from_byte(block, value); //по сути двойка в степени умноженная на бит (1 или 0)
    }
    return size;
}


inline bool allocator_buddies_system::get_block_flag(void* block) const
{
    trace_with_guard("Called method inline size_t& allocator_buddies_system::get_block_flag(void * block) const");
    trace_with_guard("Successfully executed method inline size_t& allocator_buddies_system::get_block_flag(void * block) const");

    return get_bit_from_byte(block, 1 << 7);
}

inline void allocator_buddies_system::set_block_flag(void* block, bool flag)
{
    trace_with_guard("Called method inline void allocator_buddies_system::set_block_size(void* block, size_t size) const");
    trace_with_guard("Successfully executed method inline void allocator_buddies_system::set_block_size(void* block, size_t size) const");
   
    set_bit_in_byte(block, 1 << 7, flag);
}

inline void* allocator_buddies_system::get_block_next_block_ptr(void* block) const
{
    trace_with_guard("Called method inline void * allocator_buddies_system::get_block_next_block_ptr(void * block) const");
    trace_with_guard("Successfully executed method inline void * allocator_buddies_system::get_block_next_block_ptr(void * block) const");
    
    return *reinterpret_cast<void**>(
        reinterpret_cast<unsigned char*>(block)
        + 1
        + sizeof(void*));
        
}

inline void allocator_buddies_system::set_block_next_block_ptr(void* block, void* ptr)
{
    trace_with_guard("Called method inline void allocator_buddies_system::set_block_next_block_ptr(void* block, void * ptr) const");
    trace_with_guard("Successfully executed method inline void allocator_buddies_system::set_block_next_block_ptr(void* block, void * ptr) const");
   
   *reinterpret_cast<void**>(
        reinterpret_cast<unsigned char*>(block)
        + 1
        + sizeof(void*)) = ptr;
        
}

inline void* allocator_buddies_system::get_block_previous_block_ptr(void* occupied_block) const
{
    trace_with_guard("Called method inline void * allocator_buddies_system::get_occupied_block_next_block_ptr(void * occupied_block) const");
    trace_with_guard("Successfully executed method inline void * allocator_buddies_system::get_occupied_block_next_block_ptr(void * occupied_block) const");
    
    return *reinterpret_cast<void**>(
        reinterpret_cast<unsigned char*>(occupied_block)
        + 1
        + sizeof(void*)
        + sizeof(void*));
        
}

inline void allocator_buddies_system::set_block_previous_block_ptr(void* occupied_block, void* ptr)
{
    trace_with_guard("Called method inline void allocator_buddies_system::set_occupied_block_next_block_ptr(void* occupied_block, void * ptr) const");
    trace_with_guard("Successfully executed method inline void allocator_buddies_system::set_occupied_block_next_block_ptr(void* occupied_block, void * ptr) const");
   
   *reinterpret_cast<void**>(
        reinterpret_cast<unsigned char*>(occupied_block)
        + 1
        + sizeof(void*)
        + sizeof(void*)) = ptr;
}

#pragma endregion

//-
#pragma region Log

std::vector<allocator_test_utils::block_info> allocator_buddies_system::get_blocks_info() const noexcept
{
   
    debug_with_guard("Called method std::vector<allocator_test_utils::block_info> allocator_buddies_system::get_blocks_info() const noexcept");
    std::vector<allocator_test_utils::block_info> state;
    /*
    void* current_block = get_first_occupied_block_address();
    void* previous_block = nullptr;

    if (current_block == nullptr) //если нет занятых
    {
        allocator_test_utils::block_info current_info{ get_space_size(), false };
        state.push_back(current_info);
        return state;
    }

    void* memory_start = get_memory_start();
    if (current_block != memory_start)
    {
        size_t size = reinterpret_cast<unsigned char*>(current_block)
            - reinterpret_cast<unsigned char*>(memory_start);
        allocator_test_utils::block_info current_info{ size, false };
        state.push_back(current_info);
    }


    while (current_block != nullptr)
    {
        if (previous_block != nullptr)
        {
            size_t size =
                reinterpret_cast<unsigned char*>(current_block) -
                reinterpret_cast<unsigned char*>(previous_block) -
                get_occupied_block_metadata_size() -
                get_occupied_block_size(previous_block);

            if (size != 0)
            {
                allocator_test_utils::block_info current_info2{ size, false };
                state.push_back(current_info2);
            }
        }

        allocator_test_utils::block_info current_info{ get_occupied_block_size(current_block), true };
        state.push_back(current_info);

        previous_block = current_block;
        current_block = get_occupied_block_next_block_ptr(current_block);
    }

    void* memory_end = get_memory_end();
    auto* last_occupied_block_end =
        reinterpret_cast<unsigned char*>(previous_block) +
        get_occupied_block_metadata_size() + get_occupied_block_size(previous_block);
    if (memory_end != last_occupied_block_end)
    {
        size_t size = reinterpret_cast<unsigned char*>(memory_end) - last_occupied_block_end;
        allocator_test_utils::block_info current_info{ size, false };
        state.push_back(current_info);
    }
    */
    debug_with_guard("Successfully executed method std::vector<allocator_test_utils::block_info> allocator_buddies_system::get_blocks_info() const noexcept");

    return state;
}

void allocator_buddies_system::log_blocks_info() const
{
    debug_with_guard("Called method void allocator_buddies_system::log_blocks_info() const");

    auto state = get_blocks_info();
    std::string state_str = "Blocks info:\n";

    for (auto row : state)
    {
        state_str += std::string(row.is_block_occupied ? "1" : "0") + "_" + std::to_string(row.block_size) + "\t";
    }

    state_str += "\t";
    get_logger()->debug(state_str);

    debug_with_guard("Successfully executed method void allocator_buddies_system::log_blocks_info() const");
}

#pragma endregion

//+
#pragma region Helpers

inline size_t allocator_buddies_system::get_pow_from_value(size_t value) const
{
    int pow = 1;
    while (value >>= 1) ++pow;
    return pow;
}

inline size_t allocator_buddies_system::get_value_from_pow(size_t pow) const
{
    int value = 2;
    while (--pow) value <<= 1;
    return value;
}

inline void allocator_buddies_system::set_bit_in_byte(void* block, size_t shift_mask, bool byte)
{
    if (byte)
        *reinterpret_cast<char*>(block) |= shift_mask;
    else
        *reinterpret_cast<char*>(block) &= ~shift_mask;
}

inline bool allocator_buddies_system::get_bit_from_byte(void* block, size_t shift_mask) const
{
    return (*reinterpret_cast<char*>(block) & shift_mask)>0;
}

std::string allocator_buddies_system::byte_to_string(void* block) const
{
    std::string str = "";
    int charBytesCount = 8;
    for (int i = 0; i < 8; i++) {
        int mask = 1 << i;
        int byteCode = *reinterpret_cast<char*>(block) & mask;
        str+=(byteCode > 0) ? '1' : '0';
    }
    return str;
}

#pragma endregion
