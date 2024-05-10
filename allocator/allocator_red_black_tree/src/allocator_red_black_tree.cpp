#include <not_implemented.h>

#include "../include/allocator_red_black_tree.h"

//+
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

//-
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

    void** root = reinterpret_cast<void**>(avalaible_size + 1);
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



    throw not_implemented("[[nodiscard]] void *allocator_red_black_tree::allocate(size_t, size_t)", "your code should be here...");
}

void allocator_red_black_tree::deallocate(
    void *at)
{
    throw not_implemented("void allocator_red_black_tree::deallocate(void *)", "your code should be here...");
}

#pragma endregion

//+
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
    return sizeof(allocator*) + sizeof(logger*) + sizeof(size_t) + sizeof(std::mutex) + sizeof(allocator_with_fit_mode::fit_mode) + sizeof(size_t) + sizeof(void*);
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

//+
#pragma region Metadata first block methods


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
        ) = pointer;
}


#pragma endregion

//+
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
    trace_with_guard("Called method inline void allocator_red_black_tree::set_block_previous(void* block, void* ptr)");
    trace_with_guard("Successfully executed method inline void allocator_red_black_tree::set_block_previous(void* block, void* ptr)");
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

//-
#pragma region Log methods

std::vector<allocator_test_utils::block_info> allocator_red_black_tree::get_blocks_info() const noexcept
{

    debug_with_guard("Called method std::vector<allocator_test_utils::block_info> allocator_red_black_tree::get_blocks_info() const noexcept");
    std::vector<allocator_test_utils::block_info> state;

   /* void* current_block = get_first_occupied_block_address();
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
    debug_with_guard("Successfully executed method std::vector<allocator_test_utils::block_info> allocator_red_black_tree::get_blocks_info() const noexcept");

    return state;
}

void allocator_red_black_tree::log_blocks_info() const
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

//+ проверить несколько раз по алгоритму. ПРОВЕРИТЬ КОРРЕКТНОСТЬ (например "дедов" надо обновлять каждый раз. а иногда наоборот НЕ надо)
#pragma region RedBlackTree methods

//get uncle utc


void* allocator_red_black_tree::tree_search(void* node, size_t size, std::function<bool(int, int)>compare)
{
    while (node != nullptr)
    {
        switch (comparator(size, get_block_size(node)))
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
    //пока что ячейка красная и не равна нулл птр
    while (get_block_parent(node)!=nullptr && get_block_color(get_block_parent(node))==Red)
    {
        void* father = get_block_parent(node);
        void* grandfather = get_block_parent(father);

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
                    father = get_block_parent(node);
                    grandfather = get_block_parent(father);
                    tree_left_rotate(node);
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
                    father = get_block_parent(node);
                    grandfather = get_block_parent(father);
                    tree_right_rotate(node);
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
    void* saved_parent = nullptr;

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
    if (orphan != nullptr)
        set_block_parent(orphan, get_block_parent(todestroy));
    else
        saved_parent = get_block_parent(todestroy); //сохраняем чтобы обработать в tree_delete_fiorphanup


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

    //если заменяли ячейку для удаления то переписываем ее данные
    if (todestroy != node)
    {
        set_block_size(node, get_block_size(todestroy));

        void* next = get_block_next(todestroy);
        void* previous = get_block_previous(todestroy);

        set_block_next(node, next);
        set_block_previous(node, previous);

        if (next != nullptr)
            set_block_previous(next, node);
        if (previous != nullptr)
            set_block_next(previous, node);
    }

    if (get_block_color(todestroy) == Black)
        tree_delete_fixup(orphan, saved_parent, compare);

    return todestroy;

}

void allocator_red_black_tree::tree_delete_fixup(void* node, void* saved_parent, std::function<int(int, int)>compare)
{
    
   
    while (node != get_root() && get_block_color(node) == Black)
    {
        void* parent = node == nullptr ? saved_parent : get_block_parent(node);
        void* uncle;
        if (node == get_block_left_child(parent))
        {
            uncle = get_block_right_child(parent);
            if (get_block_color(uncle) == Red)
            {
                set_block_color(uncle, Black);
                set_block_color(parent, Red);
                tree_left_rotate(parent);
                uncle = get_block_right_child(parent);
            }


            if (get_block_color(get_block_left_child(uncle)) == Black && get_block_color(get_block_right_child(uncle)) == Black)
            {
                set_block_color(uncle, Red);
                node = parent;
            }
            else
            {
                if (get_block_color(get_block_right_child(uncle)) == Black)
                {
                    set_block_color(get_block_left_child(uncle), Black);
                    set_block_color(uncle, Red);
                    tree_right_rotate(uncle);
                    uncle = get_block_right_child(parent);
                }

                set_block_color(uncle, get_block_color(parent));
                set_block_color(parent, Black);
                set_block_color(get_block_right_child(uncle), Black);
                tree_left_rotate(parent);
                break;
            }
        }
        else
        {
            uncle = get_block_left_child(parent);
            if (get_block_color(uncle) == Red)
            {
                set_block_color(uncle, Black);
                set_block_color(parent, Red);
                tree_right_rotate(parent);
                uncle = get_block_left_child(parent);
            }


            if (get_block_color(get_block_right_child(uncle)) == Black && get_block_color(get_block_left_child(uncle)) == Black)
            {
                set_block_color(uncle, Red);
                node = parent;
            }
            else
            {
                if (get_block_color(get_block_left_child(uncle)) == Black)
                {
                    set_block_color(get_block_right_child(uncle), Black);
                    set_block_color(uncle, Red);
                    tree_left_rotate(uncle);
                    uncle = get_block_left_child(parent);
                }

                set_block_color(uncle, get_block_color(parent));
                set_block_color(parent, Black);
                set_block_color(get_block_left_child(uncle), Black);
                tree_right_rotate(parent);
                break;
            }
        }
    }

}

//по хорошему для элементов занятых, свободных и тп надо делать отдельные классы - поля создавать вместо этой всей фигни и тп. тупанул. уже пофиг - долго.

#pragma endregion
