#include <not_implemented.h>
#include "../include/allocator_boundary_tags.h"


#pragma region Object methods

void allocator_boundary_tags::deallocate_object_fields()
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

allocator_boundary_tags::~allocator_boundary_tags()
{
    auto* logger = get_logger();
    if (logger != nullptr) logger->debug("Called method allocator_boundary_tags::~allocator_boundary_tags()");

    deallocate_object_fields();

    if (logger != nullptr) logger->debug("Successfully executed method allocator_boundary_tags::~allocator_boundary_tags()");
}

allocator_boundary_tags::allocator_boundary_tags(
    allocator_boundary_tags &&other) noexcept
{
    _trusted_memory = other._trusted_memory;
    other._trusted_memory = nullptr;

    debug_with_guard("Called method allocator_boundary_tags::allocator_boundary_tags(allocator_boundary_tags && other) noexcept");
    debug_with_guard("Successfully executed method allocator_boundary_tags::allocator_boundary_tags(allocator_boundary_tags && other) noexcept");
}

allocator_boundary_tags &allocator_boundary_tags::operator=(
    allocator_boundary_tags &&other) noexcept
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


allocator_boundary_tags::allocator_boundary_tags(
    size_t space_size,
    allocator *parent_allocator,
    logger *logger,
    allocator_with_fit_mode::fit_mode allocate_fit_mode)
{
    if (logger != nullptr) logger->debug("Called method allocator_boundary_tags::allocator_boundary_tags(size_t space_size, allocator * parent_allocator,logger * logger,allocator_with_fit_mode::fit_mode allocate_fit_mode) with request: memory:\n" + std::to_string(space_size) + ", metadata:" + std::to_string(get_allocator_metadata_size()));


    size_t space_size_with_metadata = space_size + get_allocator_metadata_size();

    try
    {
        if (logger != nullptr) logger->warning("The amount of allocated memory has been overridden in method allocator_sorted_list::allocator_sorted_list(size_t space_size, allocator * parent_allocator,logger * logger,allocator_with_fit_mode::fit_mode allocate_fit_mode): allocator's metadata and occupied block metadata added");
        _trusted_memory =
            parent_allocator != nullptr ?
            parent_allocator->allocate(space_size_with_metadata, 1) :
            ::operator new(space_size_with_metadata);
    }
    catch (std::bad_alloc const& ex)
    {
        if (logger != nullptr) logger->error(std::string("Failed to perfom method allocator_sorted_list::allocator_sorted_list(size_t space_size, allocator * parent_allocator,logger * logger,allocator_with_fit_mode::fit_mode allocate_fit_mode): exception of type std::badalloc with an error: std::bad_alloc: ") + ex.what());
        if (logger != nullptr) logger->debug(std::string("Cancel with error execute method allocator_sorted_list::allocator_sorted_list(size_t space_size, allocator * parent_allocator,logger * logger,allocator_with_fit_mode::fit_mode allocate_fit_mode) with exception of type std::badalloc with an error: std::bad_alloc: ") + ex.what());
        throw;//просто throw если тот же тип
    }


    allocator** parent_allocator_ptr = reinterpret_cast<allocator**>(_trusted_memory);
    *parent_allocator_ptr = parent_allocator;

    class logger** logger_ptr = reinterpret_cast<class logger**>(parent_allocator_ptr + 1);
    *logger_ptr = logger;

    size_t* space_size_ptr = reinterpret_cast<size_t*>(logger_ptr + 1);
    *space_size_ptr = space_size;

    std::mutex* sync_object_ptr = reinterpret_cast<std::mutex*>(space_size_ptr + 1);
    allocator::construct(sync_object_ptr); //аналогично new (sync_object_space) std::mutex();

    allocator_with_fit_mode::fit_mode* fit_mode_ptr = reinterpret_cast<allocator_with_fit_mode::fit_mode*>(sync_object_ptr + 1);
    *fit_mode_ptr = allocate_fit_mode;

    void** first_occupied_block_ptr = reinterpret_cast<void**>(fit_mode_ptr + 1);
    *first_occupied_block_ptr = nullptr; 

    size_t* avalaible_size = reinterpret_cast<size_t*>(first_occupied_block_ptr + 1);
    *avalaible_size = space_size;


    log_blocks_info();
    if (logger != nullptr) logger->debug("Succesfully executed method allocator_boundary_tags::allocator_boundary_tags(size_t space_size, allocator * parent_allocator,logger * logger,allocator_with_fit_mode::fit_mode allocate_fit_mode)");
}

[[nodiscard]] void *allocator_boundary_tags::allocate(
    size_t value_size,
    size_t values_count)
{

    std::lock_guard<std::mutex> lock(get_sync_object());
    debug_with_guard("Called method [[nodiscard]] void* allocator_sorted_list::allocate(size_t value_size, size_t values_count) with request: memory:\n" + std::to_string(value_size * values_count) + ", metadata:" + std::to_string(get_occupied_block_metadata_size()));

    size_t required_data_size = value_size * values_count + get_occupied_block_metadata_size();
    warning_with_guard("The amount of allocated memory has been overridden in method allocator_sorted_list::allocator_sorted_list(size_t space_size, allocator * parent_allocator,logger * logger,allocator_with_fit_mode::fit_mode allocate_fit_mode): occupied block metadata added");

    if (required_data_size > get_avalaible_size())  
    {
        error_with_guard(get_typename() + ": can't allocate memory - requested memory is more than heap");
        debug_with_guard("Cancel with error method [[nodiscard]] void* allocator_sorted_list::allocate(size_t value_size, size_t values_count): can't allocate memory - requested memory is more than heap");
        throw std::bad_alloc();
    }

    allocator_with_fit_mode::fit_mode fit_mode = get_fit_mode();
    void* memory_start = get_memory_start();
    void* memory_end = get_memory_end();
    size_t between_block_size;

    void* current_block = get_first_occupied_block_address();
    void* previous_block = nullptr;
    void* current_target = nullptr;
    void* previous_target = nullptr;
    size_t size_optimal = fit_mode == allocator_with_fit_mode::fit_mode::the_best_fit ? get_space_size()+1 : -1;



    if (current_block == nullptr) //нет ни одного занятого блока
    {
        previous_target = nullptr;
        current_target = nullptr;
        size_optimal = get_space_size();
    }
    else if (memory_start != current_block) //слева есть свободный блок
    {
        between_block_size = reinterpret_cast<unsigned char*>(current_block) -
            reinterpret_cast<unsigned char*>(memory_start);

        if (between_block_size >= required_data_size)
        {
            previous_target = nullptr;
            current_target = current_block;
            size_optimal = between_block_size;
            if (fit_mode == allocator_with_fit_mode::fit_mode::first_fit) current_block = nullptr;
        }
    }
    
    
    while (current_block != nullptr) //на последней итерации будет проверено наличие свободной ячейки перед концом памяти
    {
        previous_block = current_block;
        current_block = get_occupied_block_next_block_ptr(current_block);

        between_block_size = //если есть след блок то отнимаем от него. иначе от конца памяти
            reinterpret_cast<unsigned char*>(current_block != nullptr ? current_block : memory_end) -
            reinterpret_cast<unsigned char*>(previous_block) -
            get_occupied_block_size(previous_block) -
            get_occupied_block_metadata_size();
        
        if (between_block_size >= required_data_size)
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
                    belonging_сondition = between_block_size <= size_optimal;
                    if (between_block_size == required_data_size) break_condition = true;
                    break;
                case allocator_with_fit_mode::fit_mode::the_worst_fit:
                    belonging_сondition = between_block_size >= size_optimal;
                    break;
            }

            if (belonging_сondition)
            {
                size_optimal = between_block_size;
                current_target = current_block;
                previous_target = previous_block;
                if (break_condition) break;
            }
        }
    }
    
  
    if (size_optimal==-1 || size_optimal==get_space_size()+1) //подходящих блоков не найдено
    {
        error_with_guard(get_typename() + ": can't allocate memory - no occupied memory");
        debug_with_guard("Cancel with error method [[nodiscard]] void* allocator_sorted_list::allocate(size_t value_size, size_t values_count): can't allocate memory - no occupied memory");
        throw std::bad_alloc();
    }



    size_t right_block_size = size_optimal - required_data_size;
    void* current;
    
    if (right_block_size <= get_occupied_block_minimum_size()) //отдать весь блок, если размер правого блока = 0 или меньше допустимого (мета+минЗнач - оптимизация из кнута)
    {
        warning_with_guard("The amount of allocated memory has been overridden in method allocator_sorted_list::allocator_sorted_list(size_t space_size, allocator * parent_allocator,logger * logger,allocator_with_fit_mode::fit_mode allocate_fit_mode): a small remainder from the neighboring block is given to the requested one");
        current = add_finded_block_to_occupied_list(previous_target, current_target, size_optimal - get_occupied_block_metadata_size());
        increase_avalaible_size(-size_optimal);
    }
    else
    {
        current = add_finded_block_to_occupied_list(previous_target, current_target, required_data_size - get_occupied_block_metadata_size());
        increase_avalaible_size(-required_data_size);
    }

  
    debug_with_guard("Successfully executed method [[nodiscard]] void* allocator_sorted_list::allocate(size_t value_size, size_t values_count)");
    log_blocks_info();
    log_current_block_reference_info(current);
    information_with_guard("Avalaible size: " + std::to_string(get_avalaible_size()));

    return reinterpret_cast<void*>(reinterpret_cast<unsigned char*>(current) + get_occupied_block_metadata_size());

}

void * allocator_boundary_tags::add_finded_block_to_occupied_list(void * previous_block, void * next_block, size_t size)
{
    debug_with_guard("Called method void * allocator_boundary_tags::add_finded_block_to_occupied_list(void * previous_block, void * next_block, size_t size)");
   
    void* current = previous_block == nullptr ?
        get_memory_start() :
        reinterpret_cast<unsigned char*>(previous_block) + get_occupied_block_size(previous_block) + get_occupied_block_metadata_size();
        
    if (next_block != nullptr) set_occupied_block_previous_block_ptr(next_block, current);
    if (previous_block != nullptr) set_occupied_block_next_block_ptr(previous_block, current);
    else set_first_occupied_block_address(current);
    
    set_occupied_block_previous_block_ptr(current, previous_block);
    set_occupied_block_next_block_ptr(current, next_block);
    set_occupied_block_size(current, size);
    set_occupied_block_trusted_memory(current);

    debug_with_guard("Successfully executed method void * allocator_boundary_tags::add_finded_block_to_occupied_list(void * previous_block, void * next_block, size_t size)");
    return current;
}


void allocator_boundary_tags::deallocate(
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

    if (at < get_memory_start() || at>get_memory_end() || get_occupied_block_trusted_memory(target_block) != _trusted_memory)
    {
        error_with_guard(get_typename() + ": can't deallocate memory - the pointer referenced an invalid memory location");
        debug_with_guard("Cancel with error method: void allocator_sorted_list::deallocate(void* at): can't deallocate memory - the pointer referenced an invalid memory locationr");
        throw std::logic_error(get_typename() + ": can't deallocate memory - the pointer referenced an invalid memory location");
    }


    increase_avalaible_size(get_occupied_block_size(target_block) + metadata_size);

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
 

    debug_with_guard("Successfully executed method: void allocator_sorted_list::deallocate(void* at)");
    log_blocks_info();
    log_current_block_reference_info(previous_block);
    log_current_block_reference_info(next_block);
    information_with_guard("Free avalaible size: " + std::to_string(get_avalaible_size()));
}

#pragma endregion


#pragma region Metadata allocator methods


inline size_t& allocator_boundary_tags::get_space_size() const
{
    trace_with_guard("Called method inline size_t& allocator_boundary_tags::get_space_size() const");
    trace_with_guard("Successfully executed method inline size_t& allocator_boundary_tags::get_space_size() const");

    return *reinterpret_cast<size_t*>(
        reinterpret_cast<unsigned char*>(_trusted_memory)
        + sizeof(allocator*)
        + sizeof(logger*));
}

inline size_t& allocator_boundary_tags::get_avalaible_size() const
{
    trace_with_guard("Called method inline size_t& allocator_boundary_tags::get_avalaible_size() const");
    trace_with_guard("Successfully executed method inline size_t& allocator_boundary_tags::get_avalaible_size() const");

    return *reinterpret_cast<size_t*>(
        reinterpret_cast<unsigned char*>(_trusted_memory)
        + sizeof(allocator*)
        + sizeof(logger*)
        + sizeof(size_t)
        + sizeof(std::mutex)
        + sizeof(allocator_with_fit_mode::fit_mode)
        + sizeof(size_t));
}

inline allocator_with_fit_mode::fit_mode allocator_boundary_tags::get_fit_mode() const
{
    trace_with_guard("Called method inline allocator_with_fit_mode::fit_mode allocator_boundary_tags::get_fit_mode() const");
    trace_with_guard("Successfully executed method inline allocator_with_fit_mode::fit_mode allocator_boundary_tags::get_fit_mode() const");
    return *reinterpret_cast<fit_mode*>(
        reinterpret_cast<unsigned char*>(_trusted_memory)
        + sizeof(allocator*)
        + sizeof(logger*)
        + sizeof(size_t)
        + sizeof(std::mutex));
}

inline void allocator_boundary_tags::set_fit_mode(
    allocator_with_fit_mode::fit_mode mode)
{

    debug_with_guard("Called method inline void allocator_boundary_tags::set_fit_mode(allocator_with_fit_mode::fit_mode mode)");
    *reinterpret_cast<fit_mode*>(
        reinterpret_cast<unsigned char*>(_trusted_memory)
        + sizeof(allocator*)
        + sizeof(logger*)
        + sizeof(size_t)
        + sizeof(std::mutex)) = mode;
    debug_with_guard("Successfully executed method inline void allocator_boundary_tags::set_fit_mode(allocator_with_fit_mode::fit_mode mode)");

}

inline std::string allocator_boundary_tags::get_typename() const noexcept
{
    debug_with_guard("Called method inline std::string allocator_boundary_tags::get_typename() const noexcept");
    debug_with_guard("Successfully executed method inline std::string allocator_boundary_tags::get_typename() const noexcept");
    return "string allocator_boundary_tags";
}

inline logger* allocator_boundary_tags::get_logger() const
{
    return *reinterpret_cast<logger**>(
        reinterpret_cast<unsigned char*>(_trusted_memory)
        + sizeof(allocator*));
}


inline allocator* allocator_boundary_tags::get_allocator() const
{

    debug_with_guard("Called method inline allocator* allocator_boundary_tags::get_allocator() const");
    debug_with_guard("Successfully executed method inline allocator* allocator_boundary_tags::get_allocator() const");
    return *reinterpret_cast<allocator**>(_trusted_memory);
}

inline void allocator_boundary_tags::set_allocator(allocator* a) 
{
    trace_with_guard("Called method inline void allocator_boundary_tags::set_allocator(allocator * a) const");
    *reinterpret_cast<allocator**>(_trusted_memory) = a;
    debug_with_guard("Successfully executed method inline void allocator_boundary_tags::set_allocator(allocator * a) const");
}



inline void allocator_boundary_tags::increase_avalaible_size(int value) const
{
    trace_with_guard("Called method inline void allocator_boundary_tags::increase_avalaible_size(int value) const");
    trace_with_guard("Successfully executed method inline void allocator_boundary_tags::increase_avalaible_size(int value) const");

    *reinterpret_cast<size_t*>(
        reinterpret_cast<unsigned char*>(_trusted_memory)
        + sizeof(allocator*)
        + sizeof(logger*)
        + sizeof(size_t)
        + sizeof(std::mutex)
        + sizeof(allocator_with_fit_mode::fit_mode)
        + sizeof(size_t)) += value;

}

inline std::mutex& allocator_boundary_tags::get_sync_object() const
{
    trace_with_guard("Called method inline std::mutex& allocator_boundary_tags::get_sync_object() const");
    trace_with_guard("Successfully executed method inline std::mutex& allocator_boundary_tags::get_sync_object() const");
    return *reinterpret_cast<std::mutex*>(
        reinterpret_cast<unsigned char*>(_trusted_memory)
        + sizeof(allocator*)
        + sizeof(logger*)
        + sizeof(size_t));
}

size_t allocator_boundary_tags::get_allocator_metadata_size() const
{
    //аллокатор, логгер, размер кучи, мьютекс, фитмод, указатель на 1 элемент, размер свободной памяти
    //allocator*, logger*, size_t, mutex, allocator_with_fit_mode::fit_mode, void*
    //size_ptr, void* trusted, void* next
    return sizeof(allocator*) + sizeof(logger*) + sizeof(size_t) + sizeof(std::mutex) + sizeof(allocator_with_fit_mode::fit_mode) + sizeof(void*) + sizeof(size_t);
}

void* allocator_boundary_tags::get_memory_start() const
{
    return reinterpret_cast<unsigned char*>(_trusted_memory) + get_allocator_metadata_size();
}


void* allocator_boundary_tags::get_memory_end() const
{
    return reinterpret_cast<unsigned char*>(get_memory_start()) + get_space_size();
}


#pragma endregion


#pragma region Metadata first_occupied_block methods

inline void* allocator_boundary_tags::get_first_occupied_block_address() const
{
    trace_with_guard("Called method inline void* allocator_boundary_tags::get_first_occupied_block_address() const");
    trace_with_guard("Successfully executed method inline void* allocator_boundary_tags::get_first_occupied_block_address() const");
    return *reinterpret_cast<void**>(
        reinterpret_cast<unsigned char*>(_trusted_memory)
        + sizeof(allocator*)
        + sizeof(logger*)
        + sizeof(size_t)
        + sizeof(std::mutex)
        + sizeof(allocator_with_fit_mode::fit_mode));
}

inline void allocator_boundary_tags::set_first_occupied_block_address(void* pointer)
{
    trace_with_guard("Called method inline void allocator_boundary_tags::set_first_occupied_block_address(void * pointer)");
    trace_with_guard("Successfully executed method inline void allocator_boundary_tags::set_first_occupied_block_address(void * pointer)");
    *reinterpret_cast<void**>(
        reinterpret_cast<unsigned char*>(_trusted_memory)
        + sizeof(allocator*)
        + sizeof(logger*)
        + sizeof(size_t)
        + sizeof(std::mutex)
        + sizeof(allocator_with_fit_mode::fit_mode)) = pointer;
}


#pragma endregion


#pragma region Metadata occupied_block methods


inline void* allocator_boundary_tags::get_occupied_block_trusted_memory(void* occupied_block) const
{

    trace_with_guard("Called method inline void* allocator_boundary_tags::get_occupied_block_trusted_memory(void* occupied_block) const");
    trace_with_guard("Successfully executed method inline void* allocator_boundary_tags::get_occupied_block_trusted_memory(void* occupied_block) const");
    return *reinterpret_cast<void**>(
        reinterpret_cast<unsigned char*>(occupied_block)
        + sizeof(size_t));
}

inline void allocator_boundary_tags::set_occupied_block_trusted_memory(void* occupied_block)
{
    trace_with_guard("Called method inline void allocator_boundary_tags::set_occupied_block_trusted_memory(void* occupied_block) const");
    trace_with_guard("Successfully executed method inline void allocator_boundary_tags::set_occupied_block_trusted_memory(void* occupied_block) const");
    *reinterpret_cast<void**>(
        reinterpret_cast<unsigned char*>(occupied_block)
        + sizeof(size_t)) = _trusted_memory;
}

size_t allocator_boundary_tags::get_occupied_block_metadata_size() const
{
    return sizeof(size_t) + sizeof(void*) * 3; //размер блока, начало памяти, указатель на след занятый, указатель на предыдущий занятый
}

size_t allocator_boundary_tags::get_occupied_block_minimum_size() const
{
    trace_with_guard("Called method size_t allocator_boundary_tags::get_occupied_block_minimum_size() const");
    trace_with_guard("Successfully executed method size_t allocator_boundary_tags::get_occupied_block_minimum_size() const");
    return get_occupied_block_metadata_size() + 8; //минимальный кусок должен включать хоть сколько то малое пространство (оптимизация из кнута: обычно берутся значения от 8 до 10 б)
}


inline size_t& allocator_boundary_tags::get_occupied_block_size(void* occupied_block) const
{
    trace_with_guard("Called method inline size_t& allocator_boundary_tags::get_occupied_block_size(void * occupied_block) const");
    trace_with_guard("Successfully executed method inline size_t& allocator_boundary_tags::get_occupied_block_size(void * occupied_block) const");
    return *reinterpret_cast<size_t*>(occupied_block);
}

inline void allocator_boundary_tags::set_occupied_block_size(void* occupied_block, size_t size) 
{
    trace_with_guard("Called method inline void allocator_boundary_tags::set_occupied_block_size(void* occupied_block, size_t size) const");
    trace_with_guard("Successfully executed method inline void allocator_boundary_tags::set_occupied_block_size(void* occupied_block, size_t size) const");
    *reinterpret_cast<size_t*>(occupied_block) = size;
}

inline void* allocator_boundary_tags::get_occupied_block_next_block_ptr(void* occupied_block) const
{
    trace_with_guard("Called method inline void * allocator_boundary_tags::get_occupied_block_next_block_ptr(void * occupied_block) const");
    trace_with_guard("Successfully executed method inline void * allocator_boundary_tags::get_occupied_block_next_block_ptr(void * occupied_block) const");
    return *reinterpret_cast<void**>(
        reinterpret_cast<unsigned char*>(occupied_block)
        + sizeof(size_t)
        + sizeof(void*));
}

inline void allocator_boundary_tags::set_occupied_block_next_block_ptr(void* occupied_block, void* ptr) 
{
    trace_with_guard("Called method inline void allocator_boundary_tags::set_occupied_block_next_block_ptr(void* occupied_block, void * ptr) const");
    trace_with_guard("Successfully executed method inline void allocator_boundary_tags::set_occupied_block_next_block_ptr(void* occupied_block, void * ptr) const");
    *reinterpret_cast<void**>(
        reinterpret_cast<unsigned char*>(occupied_block)
        + sizeof(size_t)
        + sizeof(void*)) = ptr;
}

inline void* allocator_boundary_tags::get_occupied_block_previous_block_ptr(void* occupied_block) const
{
    trace_with_guard("Called method inline void * allocator_boundary_tags::get_occupied_block_next_block_ptr(void * occupied_block) const");
    trace_with_guard("Successfully executed method inline void * allocator_boundary_tags::get_occupied_block_next_block_ptr(void * occupied_block) const");
    return *reinterpret_cast<void**>(
        reinterpret_cast<unsigned char*>(occupied_block)
        + sizeof(size_t)
        + sizeof(void*)
        + sizeof(void*));
}

inline void allocator_boundary_tags::set_occupied_block_previous_block_ptr(void* occupied_block, void* ptr)
{
    trace_with_guard("Called method inline void allocator_boundary_tags::set_occupied_block_next_block_ptr(void* occupied_block, void * ptr) const");
    trace_with_guard("Successfully executed method inline void allocator_boundary_tags::set_occupied_block_next_block_ptr(void* occupied_block, void * ptr) const");
    *reinterpret_cast<void**>(
        reinterpret_cast<unsigned char*>(occupied_block)
        + sizeof(size_t)
        + sizeof(void*)
        + sizeof(void*)) = ptr;
}

#pragma endregion


#pragma region Log methods

std::vector<allocator_test_utils::block_info> allocator_boundary_tags::get_blocks_info() const noexcept
{
   
    debug_with_guard("Called method std::vector<allocator_test_utils::block_info> allocator_boundary_tags::get_blocks_info() const noexcept");
    std::vector<allocator_test_utils::block_info> state;

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

        allocator_test_utils::block_info current_info{ get_occupied_block_size(current_block), true};
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

    debug_with_guard("Successfully executed method std::vector<allocator_test_utils::block_info> allocator_boundary_tags::get_blocks_info() const noexcept");
   
    return state;
}

void allocator_boundary_tags::log_blocks_info() const
{
    debug_with_guard("Called method void allocator_boundary_tags::log_blocks_info() const");
    
    auto state = get_blocks_info();
    std::string state_str = "Blocks info:\n";

    for (auto row : state)
    {
        state_str += std::string(row.is_block_occupied ? "1" : "0") + "_" + std::to_string(row.block_size) + "\t";
    }

    state_str += "\t";
    get_logger()->debug(state_str);
    
    debug_with_guard("Successfully executed method void allocator_boundary_tags::log_blocks_info() const");
}

void allocator_boundary_tags::log_current_block_reference_info(void* current) const
{
    if (current == nullptr) return;
    void* previous_block = get_occupied_block_previous_block_ptr(current);
    void* next_block = get_occupied_block_next_block_ptr(current);

    std::stringstream str;
    str << "\nCurrentBlock:\t" << current
        << "\nPreviousBlock:\t" << get_occupied_block_previous_block_ptr(current)
        << "\nNextBlock:\t" << get_occupied_block_next_block_ptr(current)
        << "\nFirstBlock:\t" << get_first_occupied_block_address();

    if (previous_block != nullptr) str << "\nPrevious_NextBlock:\t" << get_occupied_block_next_block_ptr(previous_block) << "\n";
    if (next_block != nullptr) str << "\nNext_PreviousBlock:\t" << get_occupied_block_previous_block_ptr(next_block) << "\n";
  
    debug_with_guard(str.str());
}

#pragma endregion