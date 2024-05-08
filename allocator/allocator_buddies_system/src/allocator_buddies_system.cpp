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
    size_t required_pow,
    allocator *parent_allocator,
    logger *logger,
    allocator_with_fit_mode::fit_mode allocate_fit_mode)
{
    if (logger != nullptr) logger->debug("Called method allocator_buddies_system::allocator_buddies_system(size_t space_size, allocator * parent_allocator,logger * logger,allocator_with_fit_mode::fit_mode allocate_fit_mode) with:\nmemory:" + std::to_string(required_pow) + ", metadata:" + std::to_string(get_allocator_metadata_size()));

    size_t datablock_size = get_value_from_pow(required_pow);
    size_t space_size_with_metadata = datablock_size + get_allocator_metadata_size();

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
    *space_size_ptr = required_pow;

    std::mutex* sync_object_ptr = reinterpret_cast<std::mutex*>(space_size_ptr + 1);
    allocator::construct(sync_object_ptr);

    allocator_with_fit_mode::fit_mode* fit_mode_ptr = reinterpret_cast<allocator_with_fit_mode::fit_mode*>(sync_object_ptr + 1);
    *fit_mode_ptr = allocate_fit_mode;

    void** first_block = reinterpret_cast<void**>(fit_mode_ptr + 1);
    size_t* avalaible_size_ptr = reinterpret_cast<size_t*>(first_block + 1);

    *first_block = reinterpret_cast<void*>(avalaible_size_ptr + 1); 
    *avalaible_size_ptr = datablock_size - get_occupied_block_metadata_size();


 
    void* new_free_block = *first_block;
    set_block_flag(new_free_block, false);      //false - не занят, true - занят
    set_block_pow(new_free_block, required_pow);
    set_block_trusted_memory(new_free_block);
    set_block_previous_block_ptr(new_free_block, nullptr);
    set_block_next_block_ptr(new_free_block, nullptr);


    log_blocks_info();
    if (logger != nullptr) logger->debug("Succesfully executed method allocator_buddies_system::allocator_buddies_system(size_t space_size, allocator * parent_allocator,logger * logger,allocator_with_fit_mode::fit_mode allocate_fit_mode) with: \nresult space size with metadata: " + std::to_string(space_size_with_metadata) + ";   first byte state in new block: " + byte_to_string(get_first_free_block_address()));
}

[[nodiscard]] void* allocator_buddies_system::allocate(
    size_t value_size,
    size_t values_count)
{

    std::lock_guard<std::mutex> lock(get_sync_object());
    debug_with_guard("Called method [[nodiscard]] void* allocator_sorted_list::allocate(size_t value_size, size_t values_count) with\nrequest: memory:" + std::to_string(value_size * values_count) + "; free_blocK_metadata:" + std::to_string(get_free_block_metadata_size()) + ", occupied_block_metadata:" + std::to_string(get_occupied_block_metadata_size()));

    size_t required_data_size = value_size * values_count + get_occupied_block_metadata_size();
    if (required_data_size < get_free_block_metadata_size()) required_data_size = get_free_block_metadata_size();
    if (required_data_size > get_avalaible_size())
    {
        error_with_guard(get_typename() + ": can't allocate memory - requested memory is more than heap");
        debug_with_guard("Cancel with error method [[nodiscard]] void* allocator_sorted_list::allocate(size_t value_size, size_t values_count): can't allocate memory - requested memory is more than heap");
        throw std::bad_alloc();
    }

    size_t required_pow_size = get_pow_from_value(required_data_size);
    warning_with_guard("The amount of allocated memory has been overridden in method allocator_sorted_list::allocator_sorted_list(size_t space_size, allocator * parent_allocator,logger * logger,allocator_with_fit_mode::fit_mode allocate_fit_mode): metadata added");

    allocator_with_fit_mode::fit_mode fit_mode = get_fit_mode();
    void* current_block = get_first_free_block_address();
    void* previous_block = nullptr;
    void* current_target = nullptr;
    size_t current_pow;
    if (current_block != nullptr) current_pow = get_block_pow(current_block);


    while (current_block != nullptr)
    {
        size_t current_block_pow = get_block_pow(current_block);

        if (current_block_pow >= required_pow_size)
        {
            bool belonging_сondition = false;
            bool break_condition = false;

            switch (fit_mode)
            {
            case allocator_with_fit_mode::fit_mode::first_fit:
                belonging_сondition = true;
                break_condition = true;
                break;
            case allocator_with_fit_mode::fit_mode::the_best_fit:
                belonging_сondition = current_block_pow <= current_pow;
                if (current_block_pow == required_pow_size) break_condition = true;
                break;
            case allocator_with_fit_mode::fit_mode::the_worst_fit:
                belonging_сondition = current_block_pow >= current_pow;
                break;
            }

            if (belonging_сondition)
            {
                current_pow = current_block_pow;
                current_target = current_block;
                if (break_condition) break;
            }
        };

        previous_block = current_block;
        current_block = get_block_next_block_ptr(current_block);
    }

    if (current_target == nullptr) 
    {
        error_with_guard(get_typename() + ": can't allocate memory - no free memory");
        debug_with_guard("Cancel with error method [[nodiscard]] void* allocator_sorted_list::allocate(size_t value_size, size_t values_count): can't allocate memory - no free memory");
        throw std::bad_alloc();
    }

    //на этом этапе мы нашли блок с которым будем работать
    //далее мы его будем делить пока он не будет равен нужной нам степени

    void* left_block = current_target;
    void* right_block;
    while(--current_pow>=required_pow_size)
    {
        set_block_pow(left_block, current_pow);
        right_block = reinterpret_cast<unsigned char*>(left_block) + get_block_size(left_block);

        set_block_flag(right_block, false);
        set_block_pow(right_block, current_pow);
        set_block_trusted_memory(right_block);
        set_block_next_block_ptr(right_block, get_block_next_block_ptr(left_block));
        set_block_previous_block_ptr(right_block, left_block);
        set_block_next_block_ptr(left_block, right_block);
    }

    //Дошли до нужной степени. Теперь блок нужно занять.
    set_block_flag(current_target, true);

    void* current_target_next_block = get_block_next_block_ptr(current_target);
    void* current_target_previous_block = get_block_previous_block_ptr(current_target);
    if(current_target_next_block!=nullptr)
        set_block_previous_block_ptr(current_target_next_block, current_target_previous_block);
    if (current_target_previous_block != nullptr)
        set_block_next_block_ptr(current_target_previous_block, current_target_next_block);
    else
        set_first_free_block_address(current_target_next_block);


  //  increase_avalaible_size(get_occupied_block_size(target_block) + metadata_size);
    debug_with_guard("Successfully executed method [[nodiscard]] void* allocator_sorted_list::allocate(size_t value_size, size_t values_count) with\nRequired data size: " + std::to_string(required_data_size) + "; Required pow size: " + std::to_string(required_pow_size));
    log_blocks_info();
    information_with_guard("Avalaible size: " + std::to_string(get_avalaible_size()));
    
    return reinterpret_cast<void*>(reinterpret_cast<unsigned char*>(current_target) + get_occupied_block_metadata_size());

}

void allocator_buddies_system::deallocate(
    void *at)
{
    std::lock_guard<std::mutex> lock(get_sync_object());
    debug_with_guard("Called method: void allocator_sorted_list::deallocate(void* at)");

    if (at == nullptr)
    {
        error_with_guard(get_typename() + ": can't deallocate memory - null pointer");
        debug_with_guard("Cancel with error method: void allocator_sorted_list::deallocate(void* at): can't deallocate memory - null pointer");
        throw std::logic_error(get_typename() + ": can't deallocate memory - null pointer");
    }

    size_t metadata_size = get_occupied_block_metadata_size();
    void* target_block = reinterpret_cast<void*>(reinterpret_cast<unsigned char*>(at) - metadata_size);

    if (target_block < get_memory_start() || target_block>get_memory_end() || get_block_trusted_memory(target_block) != _trusted_memory)
    {
        error_with_guard(get_typename() + ": can't deallocate memory - the pointer referenced an invalid memory location");
        debug_with_guard("Cancel with error method: void allocator_sorted_list::deallocate(void* at): can't deallocate memory - the pointer referenced an invalid memory locationr");
        throw std::logic_error(get_typename() + ": can't deallocate memory - the pointer referenced an invalid memory location");
    }

  


   // increase_avalaible_size(get_occupied_block_size(target_block) + metadata_size);


  /*
    void* previous_block = get_occupied_block_previous_block_ptr(target_block);
    void* next_block = get_occupied_block_next_block_ptr(target_block);

    //если прошлый есть то ставим ссылку на текущий в него. иначе в начало. 
    // автоматом проставляется nullptr если это был последний занятый (хранится в next block). ниже аналогично
    if (previous_block != nullptr)
        set_occupied_block_next_block_ptr(previous_block, next_block);
    else
        set_first_occupied_block_address(next_block);

    if (next_block != nullptr)
        set_occupied_block_previous_block_ptr(next_block, previous_block);
    */

    debug_with_guard("Successfully executed method: void allocator_sorted_list::deallocate(void* at)");
    log_blocks_info();
    information_with_guard("Free avalaible size: " + std::to_string(get_avalaible_size()));
}

void allocator_buddies_system::get_buddys_ascending_order(void* any, void *& left_buddy, void*& right_buddy) const
{
    void* another_buddy = get_buddy(any);
    if (another_buddy > any)
    {
        left_buddy = any;
        right_buddy = another_buddy;
    }
    else
    {
        left_buddy = another_buddy;
        right_buddy = any;
    }
}

void* allocator_buddies_system::get_buddy(void* block) const
{
    size_t block_size = get_block_size(block);
    size_t address_from_memory_zero = reinterpret_cast<char*>(block) - reinterpret_cast<char*>(get_memory_start());
    size_t result = address_from_memory_zero ^ block_size;
    return reinterpret_cast<void*>(reinterpret_cast<unsigned char*>(get_memory_start()) + result);
}


#pragma endregion

//+
#pragma region Metadata allocator methods

inline size_t& allocator_buddies_system::get_space_in_pow() const
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

    return get_value_from_pow(get_space_in_pow());
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

inline void allocator_buddies_system::set_block_flag(void* block, bool isOccupied)
{
    trace_with_guard("Called method inline void allocator_buddies_system::set_block_size(void* block, size_t size) const");
    trace_with_guard("Successfully executed method inline void allocator_buddies_system::set_block_size(void* block, size_t size) const");
   
    set_bit_in_byte(block, 1 << 7, isOccupied);
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

inline void* allocator_buddies_system::get_block_previous_block_ptr(void* block) const
{
    trace_with_guard("Called method inline void * allocator_buddies_system::get_occupied_block_next_block_ptr(void * occupied_block) const");
    trace_with_guard("Successfully executed method inline void * allocator_buddies_system::get_occupied_block_next_block_ptr(void * occupied_block) const");
    
    return *reinterpret_cast<void**>(
        reinterpret_cast<unsigned char*>(block)
        + 1
        + sizeof(void*)
        + sizeof(void*));
        
}

inline void allocator_buddies_system::set_block_previous_block_ptr(void* block, void* ptr)
{
    trace_with_guard("Called method inline void allocator_buddies_system::set_block_next_block_ptr(void* occupied_block, void * ptr) const");
    trace_with_guard("Successfully executed method inline void allocator_buddies_system::set_block_next_block_ptr(void* occupied_block, void * ptr) const");
   
   *reinterpret_cast<void**>(
        reinterpret_cast<unsigned char*>(block)
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
    void* current_block = get_first_free_block_address();
    void* previous_block = nullptr;

    if (current_block == nullptr)
    {
        allocator_test_utils::block_info current_info{ get_space_size(), true };
        state.push_back(current_info);
        return state;
    }

    void* memory_start = get_memory_start();
    if (current_block != memory_start)
    {
        size_t size = reinterpret_cast<unsigned char*>(current_block)
            - reinterpret_cast<unsigned char*>(memory_start);
        allocator_test_utils::block_info current_info{ size, true };
        state.push_back(current_info);
    }

    while (current_block != nullptr)
    {
        if (previous_block != nullptr)
        {
            size_t size =
                reinterpret_cast<unsigned char*>(current_block) -
                (reinterpret_cast<unsigned char*>(previous_block) + get_block_size(previous_block));

            if (size != 0)
            {
                allocator_test_utils::block_info current_info2{ size, true };
                state.push_back(current_info2);
            }
        }

        allocator_test_utils::block_info current_info{ get_block_size(current_block), false };
        state.push_back(current_info);

        previous_block = current_block;
        current_block = get_block_next_block_ptr(current_block);
    }

    void* memory_end = get_memory_end();
    auto* last_free_block_end = reinterpret_cast<unsigned char*>(previous_block) + get_block_size(previous_block);
    if (memory_end != last_free_block_end)
    {
        size_t size = reinterpret_cast<unsigned char*>(memory_end) - last_free_block_end;
        allocator_test_utils::block_info current_info{ size, true };
        state.push_back(current_info);
    }

    debug_with_guard("Successfully executed method std::vector<allocator_test_utils::block_info> allocator_buddies_system::get_blocks_info() const noexcept");

    return state;
}

void allocator_buddies_system::log_blocks_info() const
{//добавить также считывание метаданных внутри. отдельной функцией
    debug_with_guard("Called method void allocator_buddies_system::log_blocks_info() const");

    auto state = get_blocks_info();
    std::string state_str = "Blocks info:\n";

    for (auto row : state)
    {
        state_str += std::string(row.is_block_occupied ? "1" : "0") + "_" + std::to_string(row.block_size) + "\t";
    }

    state_str += "\t";
    debug_with_guard(state_str);

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
