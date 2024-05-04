#include <not_implemented.h>
#include <mutex>
#include "../include/allocator_sorted_list.h"
#include <functional>
//в других проектах тоже везде втыкнуть логгер. там где нельз€ - нижнего типа
//также удал€ть перед присваиванием и перемещением если надо
//изменить условие
//проверить вспомогательыне методы

void allocator_sorted_list::deallocate_object_fields()
{

    auto* logger = get_logger();
    if (logger != nullptr) logger->trace("Called method void allocator_sorted_list::deallocate_object_fields()");

    if (_trusted_memory != nullptr)
    {
        allocator* a = get_allocator();
        if (a != nullptr)
            a->deallocate(_trusted_memory);
        else
            ::delete a;

        _trusted_memory = nullptr; //после удалени€ не забывать
    }

    if (logger != nullptr) logger->trace("Successfully executed method void allocator_sorted_list::deallocate_object_fields()");
}

allocator_sorted_list::~allocator_sorted_list()
{
    auto* logger = get_logger();
    if (logger != nullptr) logger->debug("Called method allocator_sorted_list::~allocator_sorted_list()");

    deallocate_object_fields();

    if (logger != nullptr) logger->debug("Successfully executed method allocator_sorted_list::~allocator_sorted_list()");
}

allocator_sorted_list::allocator_sorted_list(
    allocator_sorted_list &&other) noexcept
{
    //¬Ќ»ћј“≈Ћ№Ќќ! там не объект текущий а то откуда выдел€лась пам€ть. не надо set_allocator(this);

    _trusted_memory = other._trusted_memory;
    other._trusted_memory = nullptr;

    debug_with_guard("Called method allocator_sorted_list::allocator_sorted_list(allocator_sorted_list && other) noexcept");
    debug_with_guard("Successfully executed method allocator_sorted_list::allocator_sorted_list(allocator_sorted_list && other) noexcept");
}

allocator_sorted_list &allocator_sorted_list::operator=(
    allocator_sorted_list &&other) noexcept
{
    debug_with_guard("Called method allocator_sorted_list& allocator_sorted_list::operator=(allocator_sorted_list && other) noexcept");

    if (&other != this)     
    {   
        deallocate_object_fields();                   //Ќ≈ ——џЋј“№—я “”“ Ќј  ќЌ—“–” “ќ–. ќбъект уже есть. Ёто плохо
        _trusted_memory = other._trusted_memory;      //не забывать тут чистить
        other._trusted_memory = nullptr;
    }
    
    debug_with_guard("Successfully executed method allocator_sorted_list& allocator_sorted_list::operator=(allocator_sorted_list && other) noexcept");
    return *this;   
}

allocator_sorted_list::allocator_sorted_list(
    size_t space_size,
    allocator *parent_allocator,
    logger *logger,
    allocator_with_fit_mode::fit_mode allocate_fit_mode)
{
    if(logger!=nullptr) logger->debug("Called method allocator_sorted_list::allocator_sorted_list(size_t space_size, allocator * parent_allocator,logger * logger,allocator_with_fit_mode::fit_mode allocate_fit_mode) with request: memory:" + std::to_string(space_size) + ", metadata:"+std::to_string(get_allocator_metadata_size()));

    
    if (space_size < get_free_block_metadata_size()) //если будет меньше - не сможем записать ни один блок
    {
        if (logger != nullptr) logger->error(std::string("Failed to perfom method allocator_sorted_list::allocator_sorted_list(size_t space_size, allocator * parent_allocator,logger * logger,allocator_with_fit_mode::fit_mode allocate_fit_mode): Can't initializate allocator: too low memory"));
        if (logger != nullptr) logger->debug(std::string("Cancel with error execute method allocator_sorted_list::allocator_sorted_list(size_t space_size, allocator * parent_allocator,logger * logger,allocator_with_fit_mode::fit_mode allocate_fit_mode): Can't initializate allocator: too low memory"));
        throw std::logic_error("Can't initializate allocator: too low memory");
    }

    size_t space_size_with_metadata = space_size + get_allocator_metadata_size();
    try
    {
        if (logger != nullptr) logger->warning("The amount of allocated memory has been overridden in method allocator_sorted_list::allocator_sorted_list(size_t space_size, allocator * parent_allocator,logger * logger,allocator_with_fit_mode::fit_mode allocate_fit_mode): allocator's metadata added");
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

    void ** first_free_block_ptr = reinterpret_cast<void**>(fit_mode_ptr + 1);
    *first_free_block_ptr = reinterpret_cast<void*>(first_free_block_ptr+1); //ссылка на первый свободный блок


    
     size_t* firstblock_size_ptr = reinterpret_cast<size_t*>(*first_free_block_ptr);
    *firstblock_size_ptr = space_size - get_free_block_metadata_size();

    void** firstblock_trusted_memory_ptr = reinterpret_cast<void**>(firstblock_size_ptr + 1);
    *firstblock_trusted_memory_ptr = _trusted_memory;

    void** firstblock_next_free_block_ptr = reinterpret_cast<void**>(firstblock_trusted_memory_ptr + 1);
    *firstblock_next_free_block_ptr = nullptr;

    log_blocks_info();
    if (logger != nullptr) logger->debug("Succesfully executed method allocator_sorted_list::allocator_sorted_list(size_t space_size, allocator * parent_allocator,logger * logger,allocator_with_fit_mode::fit_mode allocate_fit_mode)");
}

[[nodiscard]] void* allocator_sorted_list::allocate(
    size_t value_size,
    size_t values_count)
{

    std::lock_guard<std::mutex> lock(get_sync_object());
    debug_with_guard("Called method [[nodiscard]] void* allocator_sorted_list::allocate(size_t value_size, size_t values_count) with request: memory:" + std::to_string(value_size*values_count)+ ", metadata:" +std::to_string(get_free_block_metadata_size()));

    size_t required_data_size = value_size * values_count;
    warning_with_guard("The amount of allocated memory has been overridden in method allocator_sorted_list::allocator_sorted_list(size_t space_size, allocator * parent_allocator,logger * logger,allocator_with_fit_mode::fit_mode allocate_fit_mode): free block metadata added");

    if (required_data_size > get_space_size())  //если надо больше пам€ти чем у нас вообще есть (здесь провер€ем остаток свободной!!! оптимизаци€)
    {
        error_with_guard(get_typename() + ": can't allocate memory - requested memory is more than heap");
        debug_with_guard("Cancel with error method [[nodiscard]] void* allocator_sorted_list::allocate(size_t value_size, size_t values_count): can't allocate memory - requested memory is more than heap");
        throw std::bad_alloc();
    }


    void* current_block = get_first_free_block_address(); //был лишний код - не надо провер€ть на нулл - не приведет к оптимизации. тк цикл сразу же скипнетс€
    void* previous_block = nullptr;
    void* current_target = nullptr;
    void* previous_target = nullptr;
    size_t size_optimal = get_free_block_size(current_block);
    
    while (current_block != nullptr)
    {
        size_t current_block_size = get_free_block_size(current_block);
        allocator_with_fit_mode::fit_mode fit_mode = get_fit_mode();

        //лучше свитч или большое условие
        bool firstfit_condition = fit_mode == allocator_with_fit_mode::fit_mode::first_fit;
        bool thebestfit_condition = fit_mode == allocator_with_fit_mode::fit_mode::the_best_fit && current_block_size <= size_optimal;
        bool theworstfit_condition = fit_mode == allocator_with_fit_mode::fit_mode::the_worst_fit && current_block_size >= size_optimal;

        if (current_block_size >= required_data_size && (firstfit_condition || thebestfit_condition || theworstfit_condition))
        {
            size_optimal = current_block_size;
            current_target = current_block;
            previous_target = previous_block;
            if (firstfit_condition || thebestfit_condition && current_block_size == required_data_size) break;
        };

        previous_block = current_block;
        current_block = get_free_block_next_block_ptr(current_block);
    }
 
    if (current_target == nullptr) //подход€щих блоков не найдено
    {
        error_with_guard(get_typename() + ": can't allocate memory - no free memory");
        debug_with_guard("Cancel with error method [[nodiscard]] void* allocator_sorted_list::allocate(size_t value_size, size_t values_count): can't allocate memory - no free memory");
        throw std::bad_alloc();
    }

    size_t right_block_size = size_optimal - required_data_size;
    if (right_block_size < get_free_block_minimum_size()) //отдать весь блок, если размер правого блока = 0 или меньше допустимого (мета+мин«нач - оптимизаци€ из кнута)
    {
        //удалить блок из списка свободных
        warning_with_guard("The amount of allocated memory has been overridden in method allocator_sorted_list::allocator_sorted_list(size_t space_size, allocator * parent_allocator,logger * logger,allocator_with_fit_mode::fit_mode allocate_fit_mode): a small remainder from the neighboring block is given to the requested one");
        if (previous_target != nullptr)
            set_free_block_next_block_ptr(previous_target, get_free_block_next_block_ptr(current_target));
        else
            set_first_free_block_address(get_free_block_next_block_ptr(current_target));
    }
    else
    {
        //разбить на два
        set_free_block_size(current_target, required_data_size);
        void* subtraction_right_block = reinterpret_cast<void*>(reinterpret_cast<unsigned char*>(current_target) + get_free_block_metadata_size() + required_data_size);
        set_free_block_size(subtraction_right_block, right_block_size - get_free_block_metadata_size());
        set_free_block_trusted_memory(subtraction_right_block);
        set_free_block_next_block_ptr(subtraction_right_block, get_free_block_next_block_ptr(current_target));

        if (previous_target != nullptr)
            set_free_block_next_block_ptr(previous_target, subtraction_right_block);
        else
            set_first_free_block_address(subtraction_right_block);
    }

    debug_with_guard("Successfully executed method [[nodiscard]] void* allocator_sorted_list::allocate(size_t value_size, size_t values_count)");
    log_blocks_info();
    information_with_guard("Free avalaible size: " + std::to_string(get_avalaible_size()));
    
    return reinterpret_cast<void*>(reinterpret_cast<unsigned char*>(current_target) + get_free_block_metadata_size()); 
}

void allocator_sorted_list::deallocate(
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

    //проверить также диапазон пам€ти
    auto* memory_start = reinterpret_cast<unsigned char*>(_trusted_memory) + get_allocator_metadata_size();
    auto* memory_end = memory_start + get_space_size();
    if (at < memory_start || at>memory_end)
    {
        error_with_guard(get_typename() + ": can't deallocate memory - the pointer referenced an invalid memory location");
        debug_with_guard("Cancel with error method: void allocator_sorted_list::deallocate(void* at): can't deallocate memory - the pointer referenced an invalid memory locationr");
        throw std::logic_error(get_typename() + ": can't deallocate memory - the pointer referenced an invalid memory location");
    }


    void* target_block = reinterpret_cast<void*>(reinterpret_cast<unsigned char*>(at) - get_free_block_metadata_size());

    void * at_trusted_memory_ptr = get_free_block_trusted_memory(target_block);
    if (get_allocator() != *reinterpret_cast<allocator**>(at_trusted_memory_ptr)) //туда ли возвращаем
    {
        error_with_guard(get_typename() + ": can't deallocate memory - another allocator pointer");
        debug_with_guard("Cancel with error method: void allocator_sorted_list::deallocate(void* at): can't deallocate memory - null pointer");
        throw std::logic_error(get_typename() + ": can't deallocate memory - another allocator pointer");
    };


    void* next_block = get_first_free_block_address();
    void* previous_block = nullptr;
    while (next_block!=nullptr && next_block<target_block)
    {
        previous_block = next_block;
        next_block = get_free_block_next_block_ptr(next_block);
    }
 
   
    set_free_block_next_block_ptr(target_block, next_block); //устанавливаем ссылку на след блок или на конец пам€ти
    if (previous_block == nullptr) //если перед целевым блоком нет свободных блоков то запишем в начало. иначе в предыдущий блок
        set_first_free_block_address(target_block);
    else  
        set_free_block_next_block_ptr(previous_block, target_block);


    //проверка правой границы
    if (next_block != nullptr && next_block == reinterpret_cast<void*>(reinterpret_cast<unsigned char*>(target_block) + get_free_block_size(target_block) + get_free_block_metadata_size()))
    {
        *reinterpret_cast<size_t*>(target_block) += (get_free_block_size(next_block) + get_free_block_metadata_size());
        set_free_block_next_block_ptr(target_block, get_free_block_next_block_ptr(next_block));
    }

    //проверка левой границы
    if (previous_block != nullptr && target_block == reinterpret_cast<void*>(reinterpret_cast<unsigned char*>(previous_block) + get_free_block_size(previous_block) + get_free_block_metadata_size()))
    {
        *reinterpret_cast<size_t*>(previous_block) += (get_free_block_size(target_block) + get_free_block_metadata_size());
        set_free_block_next_block_ptr(previous_block, get_free_block_next_block_ptr(target_block));
    }
    





    debug_with_guard("Successfully executed method: void allocator_sorted_list::deallocate(void* at)");
    log_blocks_info();
    information_with_guard("Free avalaible size: " + std::to_string(get_avalaible_size()));
}




#pragma region Metadata main methods

inline std::string allocator_sorted_list::get_typename() const noexcept
{
    debug_with_guard("Called method inline std::string allocator_sorted_list::get_typename() const noexcept");
    debug_with_guard("Successfully executed method inline std::string allocator_sorted_list::get_typename() const noexcept");
    return "string allocator_sorted_list";
}

inline logger* allocator_sorted_list::get_logger() const
{
    return *reinterpret_cast<logger**>(
        reinterpret_cast<unsigned char*>(_trusted_memory)
        + sizeof(allocator*));
}


inline allocator* allocator_sorted_list::get_allocator() const
{

    debug_with_guard("Called method inline allocator* allocator_sorted_list::get_allocator() const");
    debug_with_guard("Successfully executed method inline allocator* allocator_sorted_list::get_allocator() const");
    return *reinterpret_cast<allocator**>(_trusted_memory);
}

inline void allocator_sorted_list::set_allocator(allocator* a) const
{
    trace_with_guard("Called method inline void allocator_sorted_list::set_allocator(allocator * a) const");
    *reinterpret_cast<allocator**>(_trusted_memory) = a;
    debug_with_guard("Successfully executed method inline void allocator_sorted_list::set_allocator(allocator * a) const");
}

inline size_t& allocator_sorted_list::get_space_size() const
{
    trace_with_guard("Called method inline size_t& allocator_sorted_list::get_space_size() const");
    trace_with_guard("Successfully executed method inline size_t& allocator_sorted_list::get_space_size() const");

    return *reinterpret_cast<size_t*>(
        reinterpret_cast<unsigned char*>(_trusted_memory)
        + sizeof(allocator*)
        + sizeof(logger*));
}

inline std::mutex& allocator_sorted_list::get_sync_object() const
{
    trace_with_guard("Called method inline std::mutex& allocator_sorted_list::get_sync_object() const");
    trace_with_guard("Successfully executed method inline std::mutex& allocator_sorted_list::get_sync_object() const");
    return *reinterpret_cast<std::mutex*>(
        reinterpret_cast<unsigned char*>(_trusted_memory)
        + sizeof(allocator*)
        + sizeof(logger*)
        + sizeof(size_t));
}

inline allocator_with_fit_mode::fit_mode allocator_sorted_list::get_fit_mode() const
{
    trace_with_guard("Called method inline allocator_with_fit_mode::fit_mode allocator_sorted_list::get_fit_mode() const");
    trace_with_guard("Successfully executed method inline allocator_with_fit_mode::fit_mode allocator_sorted_list::get_fit_mode() const");
    return *reinterpret_cast<fit_mode*>(
        reinterpret_cast<unsigned char*>(_trusted_memory)
        + sizeof(allocator*)
        + sizeof(logger*)
        + sizeof(size_t)
        + sizeof(std::mutex));
}

inline void allocator_sorted_list::set_fit_mode(
    allocator_with_fit_mode::fit_mode mode)
{

    debug_with_guard("Called method inline void allocator_sorted_list::set_fit_mode(allocator_with_fit_mode::fit_mode mode)");
    *reinterpret_cast<fit_mode*>(
        reinterpret_cast<unsigned char*>(_trusted_memory)
        + sizeof(allocator*)
        + sizeof(logger*)
        + sizeof(size_t)
        + sizeof(std::mutex)) = mode;
    debug_with_guard("Successfully executed method inline void allocator_sorted_list::set_fit_mode(allocator_with_fit_mode::fit_mode mode)");

}

size_t allocator_sorted_list::get_allocator_metadata_size() const
{
    //аллокатор, логгер, размер кучи, мьютекс, фитмод, указатель на 1 элемент
    //allocator*, logger*, size_t, mutex, allocator_with_fit_mode::fit_mode, void*
    //size_ptr, void* trusted, void* next
    return sizeof(allocator*) + sizeof(logger*) + sizeof(size_t) + sizeof(std::mutex) + sizeof(allocator_with_fit_mode::fit_mode) + sizeof(void*);
}
#pragma endregion

#pragma region Metadata first_free_block methods

inline void* allocator_sorted_list::get_first_free_block_address() const
{
    trace_with_guard("Called method inline void* allocator_sorted_list::get_first_free_block_address() const");
    trace_with_guard("Successfully executed method inline void* allocator_sorted_list::get_first_free_block_address() const");
    return *reinterpret_cast<void**>(
        reinterpret_cast<unsigned char*>(_trusted_memory)
        + sizeof(allocator*)
        + sizeof(logger*)
        + sizeof(size_t)
        + sizeof(std::mutex)
        + sizeof(allocator_with_fit_mode::fit_mode));
}

inline void allocator_sorted_list::set_first_free_block_address(void* pointer)
{
    trace_with_guard("Called method inline void allocator_sorted_list::set_first_free_block_address(void * pointer)");
    trace_with_guard("Successfully executed method inline void allocator_sorted_list::set_first_free_block_address(void * pointer)");
    *reinterpret_cast<void**>(
        reinterpret_cast<unsigned char*>(_trusted_memory)
        + sizeof(allocator*)
        + sizeof(logger*)
        + sizeof(size_t)
        + sizeof(std::mutex)
        + sizeof(allocator_with_fit_mode::fit_mode)) = pointer;
}

#pragma endregion

#pragma region Metadata free_block methods

size_t allocator_sorted_list::get_free_block_metadata_size() const
{
    return sizeof(size_t) + sizeof(void*) * 2; //размер блока, начало пам€ти, указатель на след свободный
}

size_t allocator_sorted_list::get_free_block_minimum_size() const
{
    trace_with_guard("Called method size_t allocator_sorted_list::get_free_block_minimum_size() const");
    trace_with_guard("Successfully executed method size_t allocator_sorted_list::get_free_block_minimum_size() const");
    return 8; //минимальный кусок должен включать хоть сколько то малое пространство (оптимизаци€ из кнута: обычно берутс€ значени€ от 8 до 10 б)
}


inline size_t& allocator_sorted_list::get_free_block_size(void * free_block) const
{
    trace_with_guard("Called method inline size_t& allocator_sorted_list::get_free_block_size(void * free_block) const");
    trace_with_guard("Successfully executed method inline size_t& allocator_sorted_list::get_free_block_size(void * free_block) const");
    return *reinterpret_cast<size_t*>(free_block);
}

inline void allocator_sorted_list::set_free_block_size(void* free_block, size_t size) const
{
    trace_with_guard("Called method inline void allocator_sorted_list::set_free_block_size(void* free_block, size_t size) const");
    trace_with_guard("Successfully executed method inline void allocator_sorted_list::set_free_block_size(void* free_block, size_t size) const");
    *reinterpret_cast<size_t*>(free_block) = size;
}

inline void* allocator_sorted_list::get_free_block_next_block_ptr(void* free_block) const
{
    trace_with_guard("Called method inline void * allocator_sorted_list::get_free_block_next_block_ptr(void * free_block) const");
    trace_with_guard("Successfully executed method inline void * allocator_sorted_list::get_free_block_next_block_ptr(void * free_block) const");
    return *reinterpret_cast<void**>(
        reinterpret_cast<unsigned char*>(free_block)
        + sizeof(size_t)
        + sizeof(void*));
}

inline void allocator_sorted_list::set_free_block_next_block_ptr(void* free_block, void * ptr) const
{
    trace_with_guard("Called method inline void allocator_sorted_list::set_free_block_next_block_ptr(void* free_block, void * ptr) const");
    trace_with_guard("Successfully executed method inline void allocator_sorted_list::set_free_block_next_block_ptr(void* free_block, void * ptr) const");
    *reinterpret_cast<void**>(
        reinterpret_cast<unsigned char*>(free_block)
        + sizeof(size_t)
        + sizeof(void*)) = ptr;
}


inline void* allocator_sorted_list::get_free_block_trusted_memory(void* free_block) const
{

    trace_with_guard("Called method inline void* allocator_sorted_list::get_free_block_trusted_memory(void* free_block) const");
    trace_with_guard("Successfully executed method inline void* allocator_sorted_list::get_free_block_trusted_memory(void* free_block) const");
    return *reinterpret_cast<void**>(
        reinterpret_cast<unsigned char*>(free_block)
        + sizeof(size_t));
}

inline void allocator_sorted_list::set_free_block_trusted_memory(void* free_block) const
{
    trace_with_guard("Called method inline void allocator_sorted_list::set_free_block_trusted_memory(void* free_block) const");
    trace_with_guard("Successfully executed method inline void allocator_sorted_list::set_free_block_trusted_memory(void* free_block) const");
    *reinterpret_cast<void**>(
        reinterpret_cast<unsigned char*>(free_block)
        + sizeof(size_t)) = _trusted_memory;
}
#pragma endregion

#pragma region log

std::vector<allocator_test_utils::block_info> allocator_sorted_list::get_blocks_info() const noexcept
{
    debug_with_guard("Called method std::vector<allocator_test_utils::block_info> allocator_sorted_list::get_blocks_info() const noexcept");
    std::vector<allocator_test_utils::block_info> state;
    void* current_block = get_first_free_block_address();
    void* previous_block = nullptr;

    if (current_block == nullptr)
    {
        allocator_test_utils::block_info current_info{ get_space_size()-get_free_block_metadata_size(), true};
        state.push_back(current_info);
        return state;
    }

    auto* memory_start =
        reinterpret_cast<unsigned char*>(_trusted_memory) +
        get_allocator_metadata_size();
    if (current_block != memory_start)
    {
        size_t size = reinterpret_cast<unsigned char*>(current_block)
            - memory_start - get_free_block_metadata_size();
        allocator_test_utils::block_info current_info{ size, true };
        state.push_back(current_info);
    }

    while (current_block != nullptr)
    {
        if (current_block!=get_first_free_block_address())
        {
            size_t size = 
                reinterpret_cast<unsigned char*>(current_block) - 
                reinterpret_cast<unsigned char*>(previous_block) -
                get_free_block_metadata_size();

            allocator_test_utils::block_info current_info{ size, true };
            state.push_back(current_info);
        
        }

        allocator_test_utils::block_info current_info { get_free_block_size(current_block), false};
        state.push_back(current_info);

        previous_block = current_block;
        current_block = get_free_block_next_block_ptr(current_block);
    }
    
    auto* memory_end = memory_start + get_space_size();
    auto* last_free_block_end = 
        reinterpret_cast<unsigned char*>(previous_block) +
        get_free_block_metadata_size() + get_free_block_size(previous_block);
    if (memory_end != last_free_block_end)
    {
        size_t size = memory_end - last_free_block_end - get_free_block_metadata_size();
        allocator_test_utils::block_info current_info{size, true };
        state.push_back(current_info);
    }

    debug_with_guard("Successfully executed method std::vector<allocator_test_utils::block_info> allocator_sorted_list::get_blocks_info() const noexcept");
    return state;
}

void allocator_sorted_list::log_blocks_info() const
{
    debug_with_guard("Called method void allocator_sorted_list::log_blocks_info() const");

    auto state = get_blocks_info();
    std::string state_str = "Blocks info: ";

    for (auto row : state) 
    {
        state_str += std::string(row.is_block_occupied ? "1" : "0")+"_"+std::to_string(row.block_size)+"\t";
    }

    state_str+= "\t";
    get_logger()->debug(state_str);
    
    debug_with_guard("Successfully executed method void allocator_sorted_list::log_blocks_info() const");
}



size_t allocator_sorted_list::get_avalaible_size() const
{
    //можно упростить : хранить размер доступной пам€ти; также хранить размер большего блока
    debug_with_guard("Called method size_t allocator_sorted_list::get_avalaible_size() const");

    size_t avalaible_size = 0;
    void* current_block = get_first_free_block_address();
    void* previous_block = nullptr;
    if (current_block == nullptr) return 0;
   
    while (current_block != nullptr)
    {
        avalaible_size += get_free_block_size(current_block);

        previous_block = current_block;
        current_block = get_free_block_next_block_ptr(current_block);
    }

    debug_with_guard("Successfully executed method size_t allocator_sorted_list::get_avalaible_size() const");
    return avalaible_size;
}

#pragma endregion