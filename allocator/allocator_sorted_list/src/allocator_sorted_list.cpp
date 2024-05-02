#include <not_implemented.h>
#include <mutex>

#include "../include/allocator_sorted_list.h"
#include <functional>

//debug_with_guard("Successfully executed method #");
//debug_with_guard("Called method #");
//error_with_guard(std::string("Failed to perfom method #: exception of type std::badalloc with an error: ") + ex.what());
//debug_with_guard(std::string("Cancel execute method # with exception of type std::badalloc with an error: ") + ex.what());

allocator_sorted_list::~allocator_sorted_list()
{
    throw not_implemented("allocator_sorted_list::~allocator_sorted_list()", "your code should be here...");
}

allocator_sorted_list::allocator_sorted_list(
    allocator_sorted_list const &other)
{
    throw not_implemented("allocator_sorted_list::allocator_sorted_list(allocator_sorted_list const &)", "your code should be here...");
}

allocator_sorted_list &allocator_sorted_list::operator=(
    allocator_sorted_list const &other)
{
    throw not_implemented("allocator_sorted_list &allocator_sorted_list::operator=(allocator_sorted_list const &)", "your code should be here...");
}

allocator_sorted_list::allocator_sorted_list(
    allocator_sorted_list &&other) noexcept
{
    throw not_implemented("allocator_sorted_list::allocator_sorted_list(allocator_sorted_list &&) noexcept", "your code should be here...");
}

allocator_sorted_list &allocator_sorted_list::operator=(
    allocator_sorted_list &&other) noexcept
{
    throw not_implemented("allocator_sorted_list &allocator_sorted_list::operator=(allocator_sorted_list &&) noexcept", "your code should be here...");
}

allocator_sorted_list::allocator_sorted_list(
    size_t space_size,
    allocator *parent_allocator,
    logger *logger,
    allocator_with_fit_mode::fit_mode allocate_fit_mode)
{
    if(logger!=nullptr) logger->debug("Called method allocator_sorted_list::allocator_sorted_list(size_t space_size, allocator * parent_allocator,logger * logger,allocator_with_fit_mode::fit_mode allocate_fit_mode)");


    if (space_size < get_free_block_metadata_size()) //если будет меньше - не сможем записать ни один блок
    {
        if (logger != nullptr) logger->error(std::string("Failed to perfom method allocator_sorted_list::allocator_sorted_list(size_t space_size, allocator * parent_allocator,logger * logger,allocator_with_fit_mode::fit_mode allocate_fit_mode): Can't initializate allocator: too low memory"));
        if (logger != nullptr) logger->debug(std::string("Cancel execute method allocator_sorted_list::allocator_sorted_list(size_t space_size, allocator * parent_allocator,logger * logger,allocator_with_fit_mode::fit_mode allocate_fit_mode): Can't initializate allocator: too low memory"));
        throw std::logic_error("Can't initializate allocator: too low memory");
    }

    space_size += get_allocator_metadata_size();
    
    try
    {
        _trusted_memory =
            parent_allocator != nullptr ?
            parent_allocator->allocate(space_size, 1) :
            ::operator new(space_size);

    }
    catch (std::bad_alloc const& ex)
    {
        
        throw;
    }


    allocator** parent_allocator_ptr = reinterpret_cast<allocator**>(_trusted_memory);
    *parent_allocator_ptr = parent_allocator;

    class logger** logger_ptr = reinterpret_cast<class logger**>(parent_allocator_ptr + 1);
    *logger_ptr = logger;

    size_t* space_size_ptr = reinterpret_cast<size_t*>(logger_ptr + 1);
    *space_size_ptr = space_size;

    std::mutex* sync_object_ptr = reinterpret_cast<std::mutex*>(space_size_ptr + 1);
    allocator::construct(sync_object_ptr); //внутри абстракция на new (sync_object_space) std::mutex();

    allocator_with_fit_mode::fit_mode* fit_mode_ptr = reinterpret_cast<allocator_with_fit_mode::fit_mode*>(sync_object_ptr + 1);
    *fit_mode_ptr = allocate_fit_mode;

    void** first_free_block_ptr = reinterpret_cast<void**>(fit_mode_ptr + 1);
    *first_free_block_ptr = first_free_block_ptr + 1; //ссылка на первый свободный блок (в списке занятых поменять на nullptr)



    size_t* firstblock_size_ptr = reinterpret_cast<size_t*>(*first_free_block_ptr);
    *firstblock_size_ptr = space_size - get_free_block_metadata_size();

    void ** firstblock_trusted_memory_ptr = reinterpret_cast<void**>(firstblock_size_ptr+1);
    *firstblock_trusted_memory_ptr = _trusted_memory;

    void ** firstblock_next_free_block_ptr = reinterpret_cast<void**>(firstblock_trusted_memory_ptr + 1);
    *firstblock_next_free_block_ptr = nullptr;
    
    if (logger != nullptr) logger->debug("Called method allocator_sorted_list::allocator_sorted_list(size_t space_size, allocator * parent_allocator,logger * logger,allocator_with_fit_mode::fit_mode allocate_fit_mode)");
}

[[nodiscard]] void* allocator_sorted_list::allocate(
    size_t value_size,
    size_t values_count)
{
    
    std::lock_guard<std::mutex> lock(get_sync_object());

    size_t size = value_size * values_count + get_free_block_metadata_size();
    if (size > get_space_size())  //если меньше памяти чем у нас вообще есть
    {
        error_with_guard(get_typename() + ": can't allocate memory");
        throw std::bad_alloc();
    }

    void* current_block = get_first_free_block_address();
    void* previous_block = nullptr;
    void* current_target = nullptr;
    void* previous_target = nullptr;
    if (current_block == nullptr) throw;
    size_t targetblock_size = get_free_block_size(current_block);

    while (current_block != nullptr)
    {
        size_t block_size = get_free_block_size(current_block);
        allocator_with_fit_mode::fit_mode fit_mode = get_fit_mode();

        bool firstfit_condition = fit_mode == allocator_with_fit_mode::fit_mode::first_fit;
        bool thebestfit_condition = fit_mode == allocator_with_fit_mode::fit_mode::the_best_fit && block_size < targetblock_size;
        bool theworstfit_condition = fit_mode == allocator_with_fit_mode::fit_mode::the_worst_fit && block_size > targetblock_size;

        if (block_size >= size && (firstfit_condition || thebestfit_condition || theworstfit_condition))
        {
            targetblock_size = get_free_block_size(current_block);
            current_target = current_block;
            previous_target = previous_block;
            if (firstfit_condition || thebestfit_condition && block_size == size) break;
        };

        previous_block = current_block;
        current_block = get_free_block_next_block_ptr(current_block);
    }
 
    if (current_target == nullptr) //подходящих блоков не найдено
    {
        throw;
    }
    
    const size_t deviation = get_free_block_minimum_size();
    size_t negation = targetblock_size - size;
   
    if (negation == 0 || negation < deviation) //если размер равен требуемому или отрезаемый кусок слишком мал удалить блок из списка свободных
    {
        if (previous_target != nullptr)
            set_free_block_next_block_ptr(previous_target, get_free_block_next_block_ptr(current_target));
        else
            set_first_free_block_address(get_free_block_next_block_ptr(current_target));
    }
    else
    {
        //располовинить
        set_free_block_size(current_target, size);
        void * negations_right_block = reinterpret_cast<void*>(reinterpret_cast<unsigned char*>(current_target) + size);

        set_free_block_size(negations_right_block, negation-get_free_block_metadata_size());
        set_free_block_trusted_memory_ptr(negations_right_block);
        set_free_block_next_block_ptr(negations_right_block, get_free_block_next_block_ptr(current_target));

        if (previous_target != nullptr)
            set_free_block_next_block_ptr(previous_target, negations_right_block);
        else
            set_first_free_block_address(negations_right_block);
    }
}

void allocator_sorted_list::deallocate(
    void *at)
{
    std::lock_guard<std::mutex> lock(get_sync_object());
}






inline size_t& allocator_sorted_list::get_free_block_size(void * free_block) const
{
    return *reinterpret_cast<size_t*>(free_block);
}

inline void allocator_sorted_list::set_free_block_size(void* free_block, size_t size) const
{
    *reinterpret_cast<size_t*>(free_block) = size;
}

inline void allocator_sorted_list::set_free_block_next_block_ptr(void* free_block, void * ptr) const
{//allocator*, logger*, size_t, mutex, allocator_with_fit_mode::fit_mode, void*
    *reinterpret_cast<void**>(
        reinterpret_cast<unsigned char*>(free_block)
        + sizeof(size_t)) = ptr;
}

inline void * allocator_sorted_list::get_free_block_next_block_ptr(void * free_block) const
{
    return *reinterpret_cast<void**>(
        reinterpret_cast<unsigned char*>(free_block)
        + sizeof(size_t));
}

inline void* allocator_sorted_list::get_free_block_trusted_memory_ptr(void* free_block) const
{
    return *reinterpret_cast<void**>(
        reinterpret_cast<unsigned char*>(free_block)
        + sizeof(size_t)
        + sizeof(void*));
}

inline void allocator_sorted_list::set_free_block_trusted_memory_ptr(void* free_block) const
{
    *reinterpret_cast<void**>(
        reinterpret_cast<unsigned char*>(free_block)
        + sizeof(size_t)
        + sizeof(void*)) = _trusted_memory;
}

inline void* allocator_sorted_list::get_free_block_data(void* free_block) const
{
    return *reinterpret_cast<void**>(
        reinterpret_cast<unsigned char*>(free_block)
        + sizeof(size_t)
        + sizeof(void*)*2);
}



inline std::string allocator_sorted_list::get_typename() const noexcept
{
    return "string allocator_sorted_list";
}


size_t allocator_sorted_list::get_allocator_metadata_size() const
{
    //аллокатор, логгер, размер кучи, мьютекс, фитмод, указатель на 1 элемент
    //allocator*, logger*, size_t, mutex, allocator_with_fit_mode::fit_mode, void*
    return sizeof(allocator*) + sizeof(logger*) + sizeof(size_t) + sizeof(std::mutex) + sizeof(allocator_with_fit_mode::fit_mode) + sizeof(void*);
}

size_t allocator_sorted_list::get_free_block_metadata_size() const
{
    return sizeof(size_t) + sizeof(void*)*2; //размер блока, начало памяти, указатель на след свободный
}

size_t allocator_sorted_list::get_free_block_minimum_size() const
{
    return get_free_block_metadata_size() + 8; //минимальный кусок должен включать метаданные и хоть сколько то малое пространство (оптимизация из кнута: обычно берутся значения от 8 до 10 б)
}

inline size_t& allocator_sorted_list::get_space_size() const
{
    return *reinterpret_cast<size_t*>(
        reinterpret_cast<unsigned char*>(_trusted_memory)
        + sizeof(allocator*)
        + sizeof(logger*));
}

inline std::mutex& allocator_sorted_list::get_sync_object() const
{
    return *reinterpret_cast<std::mutex*>(
        reinterpret_cast<unsigned char*>(_trusted_memory)
        + sizeof(allocator*)
        + sizeof(logger*)
        + sizeof(size_t));
    
}

inline void* allocator_sorted_list::get_first_free_block_address() const
{ 
    return *reinterpret_cast<void**>(
        reinterpret_cast<unsigned char*>(_trusted_memory) 
        + sizeof(allocator*) 
        + sizeof(logger*) 
        + sizeof(size_t) 
        + sizeof(std::mutex) 
        + sizeof(allocator_with_fit_mode::fit_mode));
}

inline void allocator_sorted_list::set_first_free_block_address(void * pointer)
{
    *reinterpret_cast<void**>(
        reinterpret_cast<unsigned char*>(_trusted_memory)
        + sizeof(allocator*)
        + sizeof(logger*)
        + sizeof(size_t)
        + sizeof(std::mutex)
        + sizeof(allocator_with_fit_mode::fit_mode)) = pointer;
}

inline logger* allocator_sorted_list::get_logger() const
{
    return *reinterpret_cast<logger**>(
        reinterpret_cast<unsigned char*>(_trusted_memory)
        + sizeof(allocator*));
}

inline allocator* allocator_sorted_list::get_allocator() const
{
    return *reinterpret_cast<allocator**>(_trusted_memory);
}

inline allocator_with_fit_mode::fit_mode allocator_sorted_list::get_fit_mode() const
{
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
    *reinterpret_cast<fit_mode*>(
        reinterpret_cast<unsigned char*>(_trusted_memory)
        + sizeof(allocator*)
        + sizeof(logger*)
        + sizeof(size_t)
        + sizeof(std::mutex)) = mode;
}

//allocator*, logger*, size_t, mutex, allocator_with_fit_mode::fit_mode, void*
std::vector<allocator_test_utils::block_info> allocator_sorted_list::get_blocks_info() const noexcept
{
    //здесь надо вернуть вектором(?) информацию о всех блоках от и до.
    //включая последний свободный.
    //смотреть видео и аудио 3 апреля 1. 
    //где то 5 мин. + скрин.
    //возвращать должен так чтобы тест сработал - те где block size и тд
    std::vector<allocator_test_utils::block_info> a;
    return a;
}
