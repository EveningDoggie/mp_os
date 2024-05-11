#include <not_implemented.h>

#include "../include/allocator_red_black_tree.h"


#pragma region Object methods

void allocator_red_black_tree::deallocate_object_fields()
{

    auto* logger = get_logger();
    if (logger != nullptr) logger->trace("Called method void allocator_red_black_tree::deallocate_object_fields()");

    if (_trusted_memory != nullptr)
    {
        allocator* a = get_allocator();
        if (a != nullptr)
            a->deallocate(_trusted_memory);
        else
            ::delete a;

        _trusted_memory = nullptr;
    }

    if (logger != nullptr) logger->trace("Successfully executed method void allocator_red_black_tree::deallocate_object_fields()");
}

allocator_red_black_tree::~allocator_red_black_tree()
{
    auto* logger = get_logger();
    if (logger != nullptr) logger->debug("Called method allocator_red_black_tree::~allocator_boundary_tags()");

    deallocate_object_fields();

    if (logger != nullptr) logger->debug("Successfully executed method allocator_red_black_tree::~allocator_boundary_tags()");
}

allocator_red_black_tree::allocator_red_black_tree(
    allocator_red_black_tree &&other) noexcept
{
    _trusted_memory = other._trusted_memory;
    other._trusted_memory = nullptr;

    debug_with_guard("Called method allocator_red_black_tree::allocator_boundary_tags(allocator_boundary_tags && other) noexcept");
    debug_with_guard("Successfully executed method allocator_red_black_tree::allocator_boundary_tags(allocator_boundary_tags && other) noexcept");
}

allocator_red_black_tree &allocator_red_black_tree::operator=(
    allocator_red_black_tree &&other) noexcept
{
    debug_with_guard("Called method allocator_boundary_tags& allocator_red_black_tree::operator=(allocator_boundary_tags && other) noexcept");

    if (&other != this)
    {
        deallocate_object_fields();
        _trusted_memory = other._trusted_memory;
        other._trusted_memory = nullptr;
    }

    debug_with_guard("Successfully executed method allocator_boundary_tags& allocator_red_black_tree::operator=(allocator_boundary_tags && other) noexcept");
    return *this;
}

#pragma endregion


#pragma region Memory methods

allocator_red_black_tree::allocator_red_black_tree(
    size_t space_size,
    allocator *parent_allocator,
    logger *logger,
    allocator_with_fit_mode::fit_mode allocate_fit_mode)
{
    if (logger != nullptr) logger->debug("Called method allocator_red_black_tree::allocator_boundary_tags(size_t space_size, allocator * parent_allocator,logger * logger,allocator_with_fit_mode::fit_mode allocate_fit_mode) with request: memory:\n" + std::to_string(space_size) + ", metadata:" + std::to_string(get_allocator_metadata_size()));

    size_t space_size_with_metadata = space_size + get_allocator_metadata_size();

    try
    {
        if (logger != nullptr) logger->warning("The amount of allocated memory has been overridden in method allocator_red_black_tree::allocator_red_black_tree(size_t space_size, allocator * parent_allocator,logger * logger,allocator_with_fit_mode::fit_mode allocate_fit_mode): allocator's metadata and occupied block metadata added");
        _trusted_memory =
            parent_allocator != nullptr ?
            parent_allocator->allocate(space_size_with_metadata, 1) :
            ::operator new(space_size_with_metadata);
    }
    catch (std::bad_alloc const& ex)
    {
        if (logger != nullptr) logger->error(std::string("Failed to perfom method allocator_red_black_tree::allocator_red_black_tree(size_t space_size, allocator * parent_allocator,logger * logger,allocator_with_fit_mode::fit_mode allocate_fit_mode): exception of type std::badalloc with an error: std::bad_alloc: ") + ex.what());
        if (logger != nullptr) logger->debug(std::string("Cancel with error execute method allocator_red_black_tree::allocator_red_black_tree(size_t space_size, allocator * parent_allocator,logger * logger,allocator_with_fit_mode::fit_mode allocate_fit_mode) with exception of type std::badalloc with an error: std::bad_alloc: ") + ex.what());
        throw;
    }


    allocator** parent_allocator_ptr = reinterpret_cast<allocator**>(_trusted_memory);
    *parent_allocator_ptr = parent_allocator;

    class logger** logger_ptr = reinterpret_cast<class logger**>(parent_allocator_ptr + 1);
    *logger_ptr = logger;

    size_t* space_size_ptr = reinterpret_cast<size_t*>(logger_ptr + 1);
    *space_size_ptr = space_size;

    std::mutex* sync_object_ptr = reinterpret_cast<std::mutex*>(space_size_ptr + 1);
    allocator::construct(sync_object_ptr);

    allocator_with_fit_mode::fit_mode* fit_mode_ptr = reinterpret_cast<allocator_with_fit_mode::fit_mode*>(sync_object_ptr + 1);
    *fit_mode_ptr = allocate_fit_mode;

    size_t* avalaible_size = reinterpret_cast<size_t*>(fit_mode_ptr + 1);
    *avalaible_size = space_size - get_occupied_block_metadata_size();

    void** nullptr_parent = reinterpret_cast<void**>(avalaible_size + 1);
    *nullptr_parent = nullptr;

    void** root = reinterpret_cast<void**>(nullptr_parent + 1);
    *root = reinterpret_cast<void**>(root + 1);

    void* block = *root;
    set_block_size(block, *avalaible_size);
    set_block_trusted_memory(block);
    set_block_previous(block, nullptr);
    set_block_next(block, nullptr);

    auto compare = [](int a, int b) {return a == b ? 0 : (a > b ? 1 : -1); };
    tree_insert(block, compare);
   // set_block_parent(block, nullptr);
   // set_block_left_child(block, nullptr);
   // set_block_right_child(block, nullptr);
  //  set_block_color(block, Black);

    log_blocks_info();
    if (logger != nullptr) logger->debug("Succesfully executed method allocator_red_black_tree::allocator_boundary_tags(size_t space_size, allocator * parent_allocator,logger * logger,allocator_with_fit_mode::fit_mode allocate_fit_mode)");
}

[[nodiscard]] void *allocator_red_black_tree::allocate(
    size_t value_size,
    size_t values_count)
{
    //тогда в сам блок непосредственно сохраняю размер = (общий размер всего блока - метаданные занятого). 
    //не важно что там лежат мета пустого. они порежутся при выделении
    //не по костыльному с классом было бы node.parent = y;


    std::lock_guard<std::mutex> lock(get_sync_object());
    debug_with_guard("Called method [[nodiscard]] void* allocator_red_black_tree::allocate(size_t value_size, size_t values_count) with request: memory:" + std::to_string(value_size * values_count) + ", metadata:" + std::to_string(get_free_block_metadata_size()));

    size_t required_data_size = value_size * values_count;
    warning_with_guard("The amount of allocated memory has been overridden in method allocator_red_black_tree::allocator_red_black_tree(size_t space_size, allocator * parent_allocator,logger * logger,allocator_with_fit_mode::fit_mode allocate_fit_mode): free block metadata added");

    if (required_data_size > get_avalaible_size())  
    {
        error_with_guard(get_typename() + ": can't allocate memory - requested memory is more than heap");
        debug_with_guard("Cancel with error method [[nodiscard]] void* allocator_red_black_tree::allocate(size_t value_size, size_t values_count): can't allocate memory - requested memory is more than heap");
        throw std::bad_alloc();
    }

    allocator_with_fit_mode::fit_mode fit_mode = get_fit_mode();
    void* current_target = nullptr;
    auto compare = [](int a, int b) {return a == b ? 0 : (a > b ? 1 : -1); };

    switch (fit_mode)
    {
        case allocator_with_fit_mode::fit_mode::first_fit:
            current_target = tree_first(get_root(), required_data_size);
            break;
        case allocator_with_fit_mode::fit_mode::the_best_fit:
            current_target = tree_search(get_root(), required_data_size, compare);
            if(current_target==nullptr) current_target = tree_successor(get_root());
            break;
        case allocator_with_fit_mode::fit_mode::the_worst_fit:
            current_target = tree_maximum(get_root());
            break;
    }

    if (current_target == nullptr || get_block_size(current_target) < required_data_size)
    {
        error_with_guard(get_typename() + ": can't allocate memory - no free memory");
        debug_with_guard("Cancel with error method [[nodiscard]] void* allocator_red_black_tree::allocate(size_t value_size, size_t values_count): can't allocate memory - no free memory");
        throw std::bad_alloc();
    }

    void* previous_target = get_block_previous(current_target);
    void* next_target = get_block_next(current_target);
    size_t size_optimal = get_block_size(current_target);
    size_t right_block_size = size_optimal - required_data_size;

    if (right_block_size <= get_block_minimum_size()) 
    {
        //удалить блок из списка свободных
        warning_with_guard("The amount of allocated memory has been overridden in method allocator_red_black_tree::allocator_red_black_tree(size_t space_size, allocator * parent_allocator,logger * logger,allocator_with_fit_mode::fit_mode allocate_fit_mode): a small remainder from the neighboring block is given to the requested one");
        tree_delete(current_target, compare);
        increase_avalaible_size(-size_optimal);
    }
    else
    {
        //левый занятый, правый свободный
        //левый удалить из дерева
        //разделяем, выставляем у левого и правого пред, след
        //у правого еще начало памяти, размер. правый добавляем в дерево

        tree_delete(current_target, compare);

        void* subtraction_right_block = reinterpret_cast<void*>(reinterpret_cast<unsigned char*>(current_target) + get_occupied_block_metadata_size() + required_data_size);
        set_block_size(subtraction_right_block, right_block_size - get_occupied_block_metadata_size());
        set_block_trusted_memory(subtraction_right_block);
        set_block_next(subtraction_right_block, next_target);
        set_block_previous(subtraction_right_block, current_target);
        set_block_next(current_target, subtraction_right_block);
        if (next_target != nullptr)
            set_block_previous(next_target, subtraction_right_block);

        tree_insert(subtraction_right_block, compare);

        increase_avalaible_size(-(required_data_size + get_occupied_block_metadata_size()));
    }

    debug_with_guard("Successfully executed method [[nodiscard]] void* allocator_red_black_tree::allocate(size_t value_size, size_t values_count)");
    log_blocks_info();
    information_with_guard("Avalaible size: " + std::to_string(get_avalaible_size()));

    return reinterpret_cast<void*>(reinterpret_cast<unsigned char*>(current_target) + get_occupied_block_metadata_size());
   
}

void allocator_red_black_tree::deallocate(
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

    void* target_block = reinterpret_cast<void*>(reinterpret_cast<unsigned char*>(at) - get_occupied_block_metadata_size());

    if (target_block < get_memory_start() || target_block>get_memory_end() || get_block_trusted_memory(target_block) != _trusted_memory)
    {
        error_with_guard(get_typename() + ": can't deallocate memory - the pointer referenced an invalid memory location");
        debug_with_guard("Cancel with error method: void allocator_sorted_list::deallocate(void* at): can't deallocate memory - the pointer referenced an invalid memory locationr");
        throw std::logic_error(get_typename() + ": can't deallocate memory - the pointer referenced an invalid memory location");
    }


    
    auto compare = [](int a, int b) {return a == b ? 0 : (a > b ? 1 : -1); };
    void* previous_block = get_block_previous(target_block);
    void* next_block = get_block_next(target_block);

    set_block_size(target_block, reinterpret_cast<unsigned char*>(next_block!=nullptr? next_block:get_memory_end())- reinterpret_cast<unsigned char*>(target_block)-get_occupied_block_metadata_size());
    increase_avalaible_size(get_block_size(target_block));

    //Проверка правой границы
    if (next_block != nullptr && tree_is_node_exists(get_root(), next_block, compare))
    {
        tree_delete(next_block, compare);
        *reinterpret_cast<size_t*>(target_block) += get_block_size(next_block) + get_occupied_block_metadata_size();
        set_block_next(target_block, get_block_next(next_block));
        if (get_block_next(next_block) != nullptr) 
            set_block_previous(get_block_next(next_block), target_block);
        increase_avalaible_size(get_occupied_block_metadata_size());
    }

    //Проверка левой границы
    if (previous_block != nullptr && tree_is_node_exists(get_root(), previous_block, compare))
    {
        *reinterpret_cast<size_t*>(previous_block) += (get_block_size(target_block) + get_occupied_block_metadata_size());
        set_block_next(previous_block, get_block_next(target_block));
         if (get_block_next(target_block) != nullptr) 
            set_block_previous(get_block_next(target_block), previous_block);
        increase_avalaible_size(get_occupied_block_metadata_size());
    }
    else
    {
        tree_insert(target_block, compare);
    }

    debug_with_guard("Successfully executed method: void allocator_sorted_list::deallocate(void* at)");
    log_blocks_info();
    information_with_guard("Free avalaible size: " + std::to_string(get_avalaible_size()));
}

#pragma endregion


#pragma region Metadata allocator methods

inline size_t& allocator_red_black_tree::get_space_size() const
{
    trace_with_guard("Called method inline size_t& allocator_red_black_tree::get_space_size() const");
    trace_with_guard("Successfully executed method inline size_t& allocator_red_black_tree::get_space_size() const");

    return *reinterpret_cast<size_t*>(
        reinterpret_cast<unsigned char*>(_trusted_memory)
        + sizeof(allocator*)
        + sizeof(logger*));
}

inline size_t& allocator_red_black_tree::get_avalaible_size() const
{
    trace_with_guard("Called method inline size_t& allocator_red_black_tree::get_avalaible_size() const");
    trace_with_guard("Successfully executed method inline size_t& allocator_red_black_tree::get_avalaible_size() const");

    return *reinterpret_cast<size_t*>(
        reinterpret_cast<unsigned char*>(_trusted_memory)
        + sizeof(allocator*)
        + sizeof(logger*)
        + sizeof(size_t)
        + sizeof(std::mutex)
        + sizeof(allocator_with_fit_mode::fit_mode));
}

inline allocator_with_fit_mode::fit_mode allocator_red_black_tree::get_fit_mode() const
{
    trace_with_guard("Called method inline allocator_with_fit_mode::fit_mode allocator_red_black_tree::get_fit_mode() const");
    trace_with_guard("Successfully executed method inline allocator_with_fit_mode::fit_mode allocator_red_black_tree::get_fit_mode() const");
    return *reinterpret_cast<fit_mode*>(
        reinterpret_cast<unsigned char*>(_trusted_memory)
        + sizeof(allocator*)
        + sizeof(logger*)
        + sizeof(size_t)
        + sizeof(std::mutex));
}

inline void allocator_red_black_tree::set_fit_mode(
    allocator_with_fit_mode::fit_mode mode)
{

    debug_with_guard("Called method inline void allocator_red_black_tree::set_fit_mode(allocator_with_fit_mode::fit_mode mode)");
    *reinterpret_cast<fit_mode*>(
        reinterpret_cast<unsigned char*>(_trusted_memory)
        + sizeof(allocator*)
        + sizeof(logger*)
        + sizeof(size_t)
        + sizeof(std::mutex)) = mode;
    debug_with_guard("Successfully executed method inline void allocator_red_black_tree::set_fit_mode(allocator_with_fit_mode::fit_mode mode)");

}

inline std::string allocator_red_black_tree::get_typename() const noexcept
{
    debug_with_guard("Called method inline std::string allocator_red_black_tree::get_typename() const noexcept");
    debug_with_guard("Successfully executed method inline std::string allocator_red_black_tree::get_typename() const noexcept");
    return "string allocator_boundary_tags";
}

inline logger* allocator_red_black_tree::get_logger() const
{
    return *reinterpret_cast<logger**>(
        reinterpret_cast<unsigned char*>(_trusted_memory)
        + sizeof(allocator*));
}


inline allocator* allocator_red_black_tree::get_allocator() const
{
    debug_with_guard("Called method inline allocator* allocator_red_black_tree::get_allocator() const");
    debug_with_guard("Successfully executed method inline allocator* allocator_red_black_tree::get_allocator() const");
    return *reinterpret_cast<allocator**>(_trusted_memory);
}

inline void allocator_red_black_tree::set_allocator(allocator* a)
{
    trace_with_guard("Called method inline void allocator_red_black_tree::set_allocator(allocator * a) const");
    *reinterpret_cast<allocator**>(_trusted_memory) = a;
    debug_with_guard("Successfully executed method inline void allocator_red_black_tree::set_allocator(allocator * a) const");
}



inline void allocator_red_black_tree::increase_avalaible_size(int value) const
{
    trace_with_guard("Called method inline void allocator_red_black_tree::increase_avalaible_size(int value) const");
    trace_with_guard("Successfully executed method inline void allocator_red_black_tree::increase_avalaible_size(int value) const");

    *reinterpret_cast<size_t*>(
        reinterpret_cast<unsigned char*>(_trusted_memory)
        + sizeof(allocator*)
        + sizeof(logger*)
        + sizeof(size_t)
        + sizeof(std::mutex)
        + sizeof(allocator_with_fit_mode::fit_mode)) += value;
}

inline std::mutex& allocator_red_black_tree::get_sync_object() const
{
    trace_with_guard("Called method inline std::mutex& allocator_red_black_tree::get_sync_object() const");
    trace_with_guard("Successfully executed method inline std::mutex& allocator_red_black_tree::get_sync_object() const");
    return *reinterpret_cast<std::mutex*>(
        reinterpret_cast<unsigned char*>(_trusted_memory)
        + sizeof(allocator*)
        + sizeof(logger*)
        + sizeof(size_t));
}

size_t allocator_red_black_tree::get_allocator_metadata_size() const
{
    return sizeof(allocator*) + sizeof(logger*) + sizeof(size_t) + sizeof(std::mutex) + sizeof(allocator_with_fit_mode::fit_mode) + sizeof(size_t) + sizeof(void*) + sizeof(void*);
}


void* allocator_red_black_tree::get_memory_start() const
{
    return reinterpret_cast<unsigned char*>(_trusted_memory) + get_allocator_metadata_size();
}

void* allocator_red_black_tree::get_memory_end() const
{
    return reinterpret_cast<unsigned char*>(get_memory_start()) + get_space_size();
}

#pragma endregion


#pragma region Metadata first block methods

inline void* allocator_red_black_tree::get_nullptr_parent() const
{
    trace_with_guard("Called method inline void* allocator_red_black_tree::get_first_occupied_block_address() const");
    trace_with_guard("Successfully executed method inline void* allocator_red_black_tree::get_first_occupied_block_address() const");
    return *reinterpret_cast<void**>(
        reinterpret_cast<unsigned char*>(_trusted_memory)
        + sizeof(allocator*)
        + sizeof(logger*)
        + sizeof(size_t)
        + sizeof(std::mutex)
        + sizeof(allocator_with_fit_mode::fit_mode)
        + sizeof(size_t)
        );
}

inline void allocator_red_black_tree::set_nullptr_parent(void* parent) const
{
    trace_with_guard("Called method inline void allocator_red_black_tree::set_first_occupied_block_address(void * pointer)");
    trace_with_guard("Successfully executed method inline void allocator_red_black_tree::set_first_occupied_block_address(void * pointer)");
    *reinterpret_cast<void**>(
        reinterpret_cast<unsigned char*>(_trusted_memory)
        + sizeof(allocator*)
        + sizeof(logger*)
        + sizeof(size_t)
        + sizeof(std::mutex)
        + sizeof(allocator_with_fit_mode::fit_mode)
        + sizeof(size_t)
        ) = parent;
}


inline void* allocator_red_black_tree::get_root() const
{
    trace_with_guard("Called method inline void* allocator_red_black_tree::get_first_occupied_block_address() const");
    trace_with_guard("Successfully executed method inline void* allocator_red_black_tree::get_first_occupied_block_address() const");
    return *reinterpret_cast<void**>(
        reinterpret_cast<unsigned char*>(_trusted_memory)
        + sizeof(allocator*)
        + sizeof(logger*)
        + sizeof(size_t)
        + sizeof(std::mutex)
        + sizeof(allocator_with_fit_mode::fit_mode)
        + sizeof(size_t)
        + sizeof(void*)
        );
}

inline void allocator_red_black_tree::set_root(void* pointer)
{
    trace_with_guard("Called method inline void allocator_red_black_tree::set_first_occupied_block_address(void * pointer)");
    trace_with_guard("Successfully executed method inline void allocator_red_black_tree::set_first_occupied_block_address(void * pointer)");
    *reinterpret_cast<void**>(
        reinterpret_cast<unsigned char*>(_trusted_memory)
        + sizeof(allocator*)
        + sizeof(logger*)
        + sizeof(size_t)
        + sizeof(std::mutex)
        + sizeof(allocator_with_fit_mode::fit_mode)
        + sizeof(size_t)
        + sizeof(void*)
        ) = pointer;
}



#pragma endregion


#pragma region Metadata block methods

inline size_t allocator_red_black_tree::get_free_block_metadata_size() const
{
    return sizeof(void*) * 3 + sizeof(size_t) + sizeof(allocator_red_black_tree::Color) + sizeof(void*)*3;
}

inline size_t allocator_red_black_tree::get_occupied_block_metadata_size() const
{
    return sizeof(void*)*3;
}

inline size_t allocator_red_black_tree::get_block_minimum_size() const
{
    trace_with_guard("Called method size_t allocator_boundary_tags::get_block_minimum_size() const");
    trace_with_guard("Successfully executed method size_t allocator_boundary_tags::get_block_minimum_size() const");
    return get_occupied_block_metadata_size() + 8; //минимальный кусок должен включать хоть сколько то малое пространство (оптимизация из кнута: обычно берутся значения от 8 до 10 б)
}

inline void* allocator_red_black_tree::get_block_next(void* block) const
{
    trace_with_guard("Called method inline void* allocator_red_black_tree::get_block_next(void* block) const");
    trace_with_guard("Successfully executed method inline void* allocator_red_black_tree::get_block_next(void* block) const");
    return *reinterpret_cast<void**>(block);
}

inline void allocator_red_black_tree::set_block_next(void* block, void* ptr)
{
    trace_with_guard("Called method inline void allocator_red_black_tree::set_block_next(void* block, void* ptr)");
    trace_with_guard("Successfully executed method inline void allocator_red_black_tree::set_block_next(void* block, void* ptr)");
    *reinterpret_cast<void**>(block) = ptr;
}

inline void* allocator_red_black_tree::get_block_previous(void* block) const
{
    trace_with_guard("Called method inline void* allocator_red_black_tree::get_block_previous(void* block) const");
    trace_with_guard("Successfully executed method inline void* allocator_red_black_tree::get_block_previous(void* block) const");
    return *reinterpret_cast<void**>(
        reinterpret_cast<unsigned char*>(block)
        + sizeof(void*)
        );
}

inline void allocator_red_black_tree::set_block_previous(void* block, void* ptr)
{
    trace_with_guard("Called method inline void* allocator_red_black_tree::set_block_previous(void* block) const");
    trace_with_guard("Successfully executed method inline void* allocator_red_black_tree::get_block_previous(void* block) const");
    *reinterpret_cast<void**>(
        reinterpret_cast<unsigned char*>(block)
        + sizeof(void*)
        ) = ptr;
}


inline void* allocator_red_black_tree::get_block_trusted_memory(void* block) const
{
    trace_with_guard("Called method inline void* allocator_red_black_tree::get_block_trusted_memory(void* block) const");
    trace_with_guard("Successfully executed method inline void* allocator_red_black_tree::get_block_trusted_memory(void* block) const");
    return *reinterpret_cast<void**>(
        reinterpret_cast<unsigned char*>(block)
        + sizeof(void*) *2
        );
}

inline void allocator_red_black_tree::set_block_trusted_memory(void* block)
{
    trace_with_guard("Called method inline void allocator_red_black_tree::set_block_trusted_memory(void* block)");
    trace_with_guard("Successfully executed method inline void allocator_red_black_tree::set_block_trusted_memory(void* block)");
    *reinterpret_cast<void**>(
    reinterpret_cast<unsigned char*>(block)
    + sizeof(void*) * 2
    ) = _trusted_memory;
}

inline allocator_red_black_tree::Color allocator_red_black_tree::get_block_color(void* block) const
{
    trace_with_guard("Called method inline bool& allocator_red_black_tree::get_block_color(void* block) const");
    trace_with_guard("Successfully executed method inline bool& allocator_red_black_tree::get_block_color(void* block) const");
    if (block == nullptr) return Black;
    return *reinterpret_cast<allocator_red_black_tree::Color*>(
        reinterpret_cast<unsigned char*>(block)
        + sizeof(void*) *3
        );
}

inline void allocator_red_black_tree::set_block_color(void* block, Color color)
{
    trace_with_guard("Called method inline void allocator_red_black_tree::set_block_color(void* block, bool size)");
    trace_with_guard("Successfully executed method inline void allocator_red_black_tree::set_block_color(void* block, bool size)");
    *reinterpret_cast<Color*>(
        reinterpret_cast<unsigned char*>(block)
        + sizeof(void*) * 3
        ) = color;
}

inline size_t allocator_red_black_tree::get_block_size(void* block) const
{
    trace_with_guard("Called method inline size_t& allocator_red_black_tree::get_block_size(void* block) const");
    trace_with_guard("Successfully executed method inline size_t& allocator_red_black_tree::get_block_size(void* block) const");
    if (block == nullptr) return -1;
    return *reinterpret_cast<size_t*>(
        reinterpret_cast<unsigned char*>(block)
        + sizeof(void*) * 3
        + sizeof(allocator_red_black_tree::Color)
        );
}

inline void allocator_red_black_tree::set_block_size(void* block, size_t size)
{
    trace_with_guard("Called method inline void allocator_red_black_tree::set_block_size(void* block, size_t size)");
    trace_with_guard("Successfully executed method inline void allocator_red_black_tree::set_block_size(void* block, size_t size)");
    *reinterpret_cast<size_t*>(
        reinterpret_cast<unsigned char*>(block)
        + sizeof(void*) * 3
        + sizeof(allocator_red_black_tree::Color)
        ) = size;
}


inline void* allocator_red_black_tree::get_block_parent(void* block) const
{
    trace_with_guard("Called method inline void* allocator_red_black_tree::get_block_parent(void* block) const");
    trace_with_guard("Successfully executed method inline void* allocator_red_black_tree::get_block_parent(void* block) const");
    if (block == nullptr) return get_nullptr_parent();
    return *reinterpret_cast <void**> (
        reinterpret_cast<unsigned char*>(block)
        + sizeof(void*) * 3
        + sizeof(allocator_red_black_tree::Color)
        + sizeof(size_t)
        );
}

inline void allocator_red_black_tree::set_block_parent(void* block, void* ptr)
{
    trace_with_guard("Called method inline void allocator_red_black_tree::set_block_parent(void* block, void* ptr)");
    trace_with_guard("Successfully executed method inline void allocator_red_black_tree::set_block_parent(void* block, void* ptr)");
    if (block == nullptr) set_nullptr_parent(ptr);
    *reinterpret_cast <void**> (
        reinterpret_cast<unsigned char*>(block)
        + sizeof(void*) * 3
        + sizeof(allocator_red_black_tree::Color)
        + sizeof(size_t)
        ) = ptr;
}

inline void* allocator_red_black_tree::get_block_left_child(void* block) const
{
    trace_with_guard("Called method inline void* allocator_red_black_tree::get_block_left_child(void* block) const");
    trace_with_guard("Successfully executed method inline void* allocator_red_black_tree::get_block_left_child(void* block) const");
    return *reinterpret_cast <void**> (
        reinterpret_cast<unsigned char*>(block)
        + sizeof(void*) * 3
        + sizeof(allocator_red_black_tree::Color)
        + sizeof(size_t)
        + sizeof(void*)
        );
}

inline void allocator_red_black_tree::set_block_left_child(void* block, void* ptr)
{
    trace_with_guard("Called method inline void allocator_red_black_tree::set_block_left_child(void* block, void* ptr)");
    trace_with_guard("Successfully executed method inline void allocator_red_black_tree::set_block_left_child(void* block, void* ptr)"); 
    *reinterpret_cast <void**> (
        reinterpret_cast<unsigned char*>(block)
        + sizeof(void*) * 3
        + sizeof(allocator_red_black_tree::Color)
        + sizeof(size_t)
        + sizeof(void*)
        ) = ptr;
}

inline void* allocator_red_black_tree::get_block_right_child(void* block) const
{
    trace_with_guard("Called method inline void* allocator_red_black_tree::get_block_right_child(void* block) const");
    trace_with_guard("Successfully executed method inline void* allocator_red_black_tree::get_block_right_child(void* block) const");
    return *reinterpret_cast <void**> (
        reinterpret_cast<unsigned char*>(block)
        + sizeof(void*) * 3
        + sizeof(allocator_red_black_tree::Color)
        + sizeof(size_t)
        + sizeof(void*)*2
        );
}

inline void allocator_red_black_tree::set_block_right_child(void* block, void* ptr)
{
    trace_with_guard("Called method inline void allocator_red_black_tree::set_block_right_child(void* block, void* ptr)");
    trace_with_guard("Successfully executed method inline void allocator_red_black_tree::set_block_right_child(void* block, void* ptr)");
        *reinterpret_cast <void**> (
        reinterpret_cast<unsigned char*>(block)
        + sizeof(void*) * 3
        + sizeof(allocator_red_black_tree::Color)
        + sizeof(size_t)
        + sizeof(void*) * 2
        ) = ptr;
}

#pragma endregion


#pragma region Log methods

std::vector<allocator_test_utils::block_info> allocator_red_black_tree::get_blocks_info() noexcept
{

    debug_with_guard("Called method std::vector<allocator_test_utils::block_info> allocator_red_black_tree::get_blocks_info() const noexcept");
    std::vector<allocator_test_utils::block_info> state;
    auto compare = [](int a, int b) {return a == b ? 0 : (a > b ? 1 : -1); };

    void* current = get_root();
    void* prev = current;

    if (current == nullptr)
    {
        debug_with_guard("Successfully executed method std::vector<allocator_test_utils::block_info> allocator_red_black_tree::get_blocks_info() const noexcept");
    }

    while (prev != nullptr)
    {
        current = prev;
        prev = get_block_previous(prev);
    }

    void* next;
    while (current != nullptr)
    {
        size_t size;
        bool isFree = tree_is_node_exists(get_root(), current, compare);
        if (isFree)
        {
            size = get_block_size(current);
        }
        else
        {
            size = reinterpret_cast<unsigned char*>(get_block_next(current) != nullptr ? get_block_next(current) : get_memory_end()) - reinterpret_cast<unsigned char*>(current) - get_occupied_block_metadata_size();
        }

        allocator_test_utils::block_info current_info{ size, isFree };
        state.push_back(current_info);

        current = get_block_next(current);
    }
    
    debug_with_guard("Successfully executed method std::vector<allocator_test_utils::block_info> allocator_red_black_tree::get_blocks_info() const noexcept");

    return state;
}

void allocator_red_black_tree::log_blocks_info() 
{
    debug_with_guard("Called method void allocator_red_black_tree::log_blocks_info() const");

    auto state = get_blocks_info();
    std::string state_str = "Blocks info:\n";

    for (auto row : state)
    {
        state_str += std::string(row.is_block_occupied ? "1" : "0") + "_" + std::to_string(row.block_size) + "\t";
    }

    state_str += "\t";
    get_logger()->debug(state_str);

    debug_with_guard("Successfully executed method void allocator_red_black_tree::log_blocks_info() const");
}

#pragma endregion


#pragma region RedBlackTree methods

void* allocator_red_black_tree::tree_search(void* node, size_t size, std::function<bool(int, int)>compare)
{
    while (node != nullptr)
    {
        switch (compare(size, get_block_size(node)))
        {
            case 0: 
                return node;
            case 1: 
                node = get_block_right_child(node);
                break;
            case -1: 
                node = get_block_left_child(node);
                break;
        }        
    }

    return node;
}


bool allocator_red_black_tree::tree_is_node_exists(void* node, void * search_node, std::function<bool(int, int)>compare)
{
    void* current = node;
    while (node != nullptr)
    {
        switch (compare(get_block_size(search_node), get_block_size(node)))
        {
        case 0:
            return true;
        case 1:
            node = get_block_right_child(node);
            break;
        case -1:
            node = get_block_left_child(node);
            break;
        }
    }
    return false;
}


void* allocator_red_black_tree::tree_minimum(void* node)
{
    //идем влево до низа
    void* current = node;
    while (current != nullptr)
    {
        node = current;
        current = get_block_left_child(current);
    }
    return node;
}


void* allocator_red_black_tree::tree_first(void* node, size_t size)
{
    //идем вправо до низа
    if (get_block_size(node) >= size) return node;

    void* current = node;
    while (current != nullptr)
    {
        node = current;
        if (get_block_size(node) >= size) return node;
        current = get_block_right_child(current);
    }
    return node;
}

void* allocator_red_black_tree::tree_maximum(void* node)
{
    //идем вправо до низа
    void* current = node;
    while (current != nullptr)
    {
        node = current;
        current = get_block_right_child(current);
    }
    return node;
}

void* allocator_red_black_tree::tree_successor(void* node)
{
    //если есть правая ветка то ищем там минимум
    //если нет то идем вверх пока что не окажемся в левой ветке (это значит что родитель больше нас. также он минимален)

    void* right_child = get_block_right_child(node);
    if (right_child != nullptr)
        return tree_minimum(right_child);

    void* parent = get_block_parent(node);
    while (parent!=nullptr && node==get_block_right_child(parent))
    {
        node = parent;
        parent = get_block_parent(node);
    }

    return parent;
}

void* allocator_red_black_tree::tree_predecessor(void* node)
{
    //если есть левая ветка то ищем там максимум
    //если нет то идем вверх пока что не окажемся в правой ветке (это значит что родитель меньше нас. также он минимален)

    void* left_child = get_block_left_child(node);
    if (left_child != nullptr)
        return tree_maximum(left_child);

    void* parent = get_block_parent(node);
    while (parent != nullptr && node == get_block_left_child(parent))
    {
        node = parent;
        parent = get_block_parent(node);
    }

    return parent;
}

//внутрення ветвь перемещается. крайние остаются на местах. элементы двигаются влево
void allocator_red_black_tree::tree_left_rotate(void* x)
{
    void* y = get_block_right_child(x);

    //перемещаем внутреннюю левую ветвь от y к x
    void* y_left_child = get_block_left_child(y);
    set_block_right_child(x, y_left_child);
    if (y_left_child != nullptr)
        set_block_parent(y_left_child, x);

    //меняем родителя
    set_block_parent(y, get_block_parent(x));

    if (get_block_parent(x) == nullptr)
    {
        set_root(y);
    }
    else
    {
        //ставим y именно туда где мы ранее находились
        if (x == get_block_left_child(get_block_parent(x)))
            set_block_left_child(get_block_parent(x), y);
        else
            set_block_right_child(get_block_parent(x), y);
    }
    
    set_block_left_child(y, x);
    set_block_parent(x, y);
}


//внутрення ветвь перемещается. крайние остаются на местах. элементы двигаются вправо
void allocator_red_black_tree::tree_right_rotate(void* y)
{
    void* x = get_block_left_child(y);

    //перемещаем внутреннюю правую ветвь от x к y
    void* x_right_child = get_block_right_child(x);
    set_block_left_child(y, x_right_child);
    if (x_right_child != nullptr)
        set_block_parent(x_right_child, y);

    set_block_parent(x, get_block_parent(y));

    if (get_block_parent(x) == nullptr)
    {
        set_root(x);
    }
    else
    {
        if (y == get_block_left_child(get_block_parent(y)))
            set_block_left_child(get_block_parent(y), x);
        else
            set_block_right_child(get_block_parent(y), x);
    }

    set_block_right_child(x, y);
    set_block_parent(y, x);
}

void allocator_red_black_tree::tree_insert(void* node, std::function<int(int, int)>compare)
{
    void* y = nullptr;
    void* x = get_root();

    while (x != nullptr) //идем до самого низу - определяем подходящее место
    {
        y = x;
        if (compare(get_block_size(node), get_block_size(x))==-1)
            x = get_block_left_child(x);
        else
            x = get_block_right_child(x);
    }

    set_block_parent(node, y); 

    if (y == nullptr) 
    {
        set_root(node);
    }
    else
    {
        if (compare(get_block_size(node), get_block_size(y)) == -1) //если мы меньше чем найденный родитель то вставляем влево иначе вправо
            set_block_left_child(y, node);
        else
            set_block_right_child(y, node);
    }

    set_block_left_child(node, nullptr);
    set_block_right_child(node, nullptr);
    set_block_color(node, Red);

    tree_insert_fixup(node, compare);
}

void allocator_red_black_tree::tree_insert_fixup(void* node, std::function<int(int, int)>compare)
{
    //пока что ячейка красная и не равна нулл птр (если нулл птр то вернется черный цвет)
    while (get_block_color(get_block_parent(node))==Red)
    {
        void* father = get_block_parent(node);
        void* grandfather = get_block_parent(get_block_parent(node));

        if (father == get_block_left_child(grandfather))
        {
            void* uncle = get_block_right_child(grandfather);
            if (get_block_color(uncle) == Red)
            {
                set_block_color(father, Black);
                set_block_color(uncle, Black);
                set_block_color(grandfather, Red);
                node = grandfather;
            }
            else
            {
                if (node == get_block_right_child(father))
                {
                    node = father;
                    tree_left_rotate(node);
                    father = get_block_parent(node);
                    grandfather = get_block_parent(get_block_parent(node));
                }

                set_block_color(father, Black);
                set_block_color(grandfather, Red);
                tree_right_rotate(grandfather);
            }
        }
        else
        {
            void* uncle = get_block_left_child(grandfather);
            if (get_block_color(uncle) == Red)
            {
                set_block_color(father, Black);
                set_block_color(uncle, Black);
                set_block_color(grandfather, Red);
                node = grandfather;
            }
            else
            {
                if (node == get_block_left_child(father))
                {
                    node = father;
                    tree_right_rotate(node);
                    father = get_block_parent(node);
                    grandfather = get_block_parent(get_block_parent(node));
                }

                set_block_color(father, Black);
                set_block_color(grandfather, Red);
                tree_left_rotate(grandfather);
            }
        }
    }
}

void* allocator_red_black_tree::tree_delete(void* node, std::function<int(int, int)>compare)
{
    void* todestroy;
    void* orphan;

    //выбираем ячейку для удаления. если у текущей <2 наследников то берем ее. иначе ищем "следующий" элемент тк у него точно будет <2 элементов
    if (get_block_left_child(node) == nullptr || get_block_right_child(node) == nullptr)
    {
        todestroy = node;
    }
    else
    {
        todestroy = tree_successor(node);
    }

    //ищем наследника todestroy и сохраняем в orphan
    if (get_block_left_child(todestroy) != nullptr)
    {
        orphan = get_block_left_child(todestroy);
    }
    else
    {
        orphan = get_block_right_child(todestroy);
    }

    //удаляем выбранную в шаге 1 ячейку
    set_block_parent(orphan, get_block_parent(todestroy));

    if (get_block_parent(todestroy) == nullptr)
    {
        set_root(orphan); //если х = nullptr, parent(todestroy)==nullptr и todestroy мы удаляем - значит все ячейки в дереве стали занятыми. памяти больше нет.
    }
    else
    {
        void* todestroy_parent = get_block_parent(todestroy);
        if (todestroy == get_block_left_child(todestroy_parent)) 
        {
            set_block_left_child(todestroy_parent, orphan);
        }
        else
        {
            set_block_right_child(todestroy_parent, orphan);
        }
    }

    //если заменяли ячейку для удаления то переписываем ее туда
    //как в книге нельзя - у нас есть связи со списоком, адреса конкретных блоков по возрастанию и тд
    //надо подменять связи блоков дерева целиком.
    if (todestroy != node)
    {
        set_block_color(todestroy, get_block_color(node));
        set_block_right_child(todestroy, get_block_right_child(node));
        set_block_left_child(todestroy, get_block_left_child(node));
        set_block_parent(todestroy, get_block_parent(node));
        
        //сущ тк подменяли ранее. значит было две ветки. значит обе они не nullptr
        set_block_parent(get_block_left_child(node), todestroy);
        set_block_parent(get_block_right_child(node), todestroy);

        //устанавливаем себя в родителя
        void* parent = get_block_parent(node);
        if (parent == nullptr)
        {
            set_root(todestroy);
        }
        else
        {
            if (get_block_right_child(parent) == node)
            {
                set_block_right_child(parent, todestroy);
            }
            else
            {
                set_block_left_child(parent, todestroy);
            }
        }
    }

    if (get_block_color(todestroy) == Black)
        tree_delete_fixup(orphan, compare);

    return todestroy;

}


void allocator_red_black_tree::tree_delete_fixup(void* node, std::function<int(int, int)>compare)
{   
    while (node != get_root() && get_block_color(node) == Black)
    {
        void* uncle;
        if (node == get_block_left_child(get_block_parent(node)))
        {
            uncle = get_block_right_child(get_block_parent(node));
            if (get_block_color(uncle) == Red)
            {
                set_block_color(uncle, Black);
                set_block_color(get_block_parent(node), Red);
                tree_left_rotate(get_block_parent(node));
                uncle = get_block_right_child(get_block_parent(node));
            }

            if (get_block_color(get_block_left_child(uncle)) == Black && get_block_color(get_block_right_child(uncle)) == Black)
            {
                set_block_color(uncle, Red);
                node = get_block_parent(node);
            }
            else
            {
                if (get_block_color(get_block_right_child(uncle)) == Black)
                {
                    set_block_color(get_block_left_child(uncle), Black);
                    set_block_color(uncle, Red);
                    tree_right_rotate(uncle);
                    uncle = get_block_right_child(get_block_parent(node));
                }

                set_block_color(uncle, get_block_color(get_block_parent(node)));
                set_block_color(get_block_parent(node), Black);
                set_block_color(get_block_right_child(uncle), Black);
                tree_left_rotate(get_block_parent(node));
                node = get_root();
            }
        }
        else
        {
            uncle = get_block_left_child(get_block_parent(node));
            if (get_block_color(uncle) == Red)
            {
                set_block_color(uncle, Black);
                set_block_color(get_block_parent(node), Red);
                tree_right_rotate(get_block_parent(node));
                uncle = get_block_left_child(get_block_parent(node));
            }


            if (get_block_color(get_block_right_child(uncle)) == Black && get_block_color(get_block_left_child(uncle)) == Black)
            {
                set_block_color(uncle, Red);
                node = get_block_parent(node);
            }
            else
            {
                if (get_block_color(get_block_left_child(uncle)) == Black)
                {
                    set_block_color(get_block_right_child(uncle), Black);
                    set_block_color(uncle, Red);
                    tree_left_rotate(uncle);
                    uncle = get_block_left_child(get_block_parent(node));
                }

                set_block_color(uncle, get_block_color(get_block_parent(node)));
                set_block_color(get_block_parent(node), Black);
                set_block_color(get_block_left_child(uncle), Black);
                tree_right_rotate(get_block_parent(node));
                node = get_root();
            }
        }
    }

}

//по хорошему для элементов занятых, свободных и тп надо делать отдельные классы - поля создавать и тд.

#pragma endregion
