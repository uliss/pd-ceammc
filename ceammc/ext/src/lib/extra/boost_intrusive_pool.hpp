/*
 * A C++ memory pool that is Boost-friendly and performance oriented.
 *
 * Inspired by:
 *  - https://thinkingeek.com/2017/11/19/simple-memory-pool/: a C++ memory pool for raw pointers
 *    (no smart pointers) using free lists and perfect forwarding to handle non-default ctors.
 *  - https://github.com/steinwurf/recycle: a C++ memory pool for std::shared_ptr<>, which however
 *    produces 1 malloc operation for each allocation/recycle operation
 *
 * Author: fmontorsi
 * Created: Feb 2019
 * License: BSD license
 *
 */

#pragma once

//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------

// #include <boost/intrusive/slist.hpp> // not really used finally
#include <boost/smart_ptr/intrusive_ptr.hpp>
#include <boost/smart_ptr/intrusive_ref_counter.hpp>

namespace memorypool {
//------------------------------------------------------------------------------
// Constants
//------------------------------------------------------------------------------

#define BOOST_INTRUSIVE_POOL_DEFAULT_POOL_SIZE (64)
#define BOOST_INTRUSIVE_POOL_INCREASE_STEP (64)
#define BOOST_INTRUSIVE_POOL_NO_MAX_SIZE (0)

#ifndef BOOST_INTRUSIVE_POOL_DEBUG_CHECKS
// if you define BOOST_INTRUSIVE_POOL_DEBUG_CHECKS=1 before including this header file,
// you will activate a lot more debug checks on the memory pool to verify its integrity;
// this is useful during e.g. debug builds
#define BOOST_INTRUSIVE_POOL_DEBUG_CHECKS (0)
#endif

#ifndef BOOST_INTRUSIVE_POOL_DEBUG_THREAD_ACCESS
// if you define BOOST_INTRUSIVE_POOL_DEBUG_THREAD_ACCESS=1 before including this header file,
// you will activate a pthread-specific check about correct thread access to the memory pool
#define BOOST_INTRUSIVE_POOL_DEBUG_THREAD_ACCESS (0)
#else
// checks are active: we use pthread_self() API:
#include <pthread.h>
#endif

#ifndef BOOST_INTRUSIVE_POOL_DEBUG_MAX_REFCOUNT
// completely-arbitrary threshold about what range of refcounts can be considered
// sane and valid and which range cannot be considered valid!
#define BOOST_INTRUSIVE_POOL_DEBUG_MAX_REFCOUNT (1024)
#endif

typedef enum {
    RECYCLE_METHOD_NONE,
    RECYCLE_METHOD_DESTROY_FUNCTION,
    RECYCLE_METHOD_CUSTOM_FUNCTION,
    // RECYCLE_METHOD_DTOR,
} recycle_method_e;

//------------------------------------------------------------------------------
// Forward declarations
//------------------------------------------------------------------------------

class boost_intrusive_pool_item;

//------------------------------------------------------------------------------
// boost_intrusive_pool_iface
//------------------------------------------------------------------------------

class boost_intrusive_pool_iface
    : public boost::intrusive_ref_counter<boost_intrusive_pool_iface, boost::thread_unsafe_counter> {
public:
    virtual ~boost_intrusive_pool_iface() = default;

    virtual void recycle(boost_intrusive_pool_item* item) = 0;

    virtual bool is_bounded() const = 0;
    virtual bool is_memory_exhausted() const = 0;
};

//------------------------------------------------------------------------------
// boost_intrusive_pool_item
// Base class for any C++ class that will be used inside a boost_intrusive_pool
//------------------------------------------------------------------------------

class boost_intrusive_pool_item {
    friend void intrusive_ptr_add_ref(boost_intrusive_pool_item* x);
    friend void intrusive_ptr_release(boost_intrusive_pool_item* x);

public:
    boost_intrusive_pool_item()
    {
        m_boost_intrusive_pool_next = nullptr;
        m_boost_intrusive_pool_refcount = 0;
        m_boost_intrusive_pool_owner = nullptr;
    }
    boost_intrusive_pool_item(const boost_intrusive_pool_item& other)
    {
        // IMPORTANT:
        //   do not copy the members of this class around:
        //   whether "this" instance is inside a memory pool or not, that has to stay that way,
        //   regardless of whether "other" is inside a memory pool or not.

        m_boost_intrusive_pool_next = nullptr;
        m_boost_intrusive_pool_refcount = 0;
        m_boost_intrusive_pool_owner = nullptr;
    }
    boost_intrusive_pool_item(const boost_intrusive_pool_item&& other)
    {
        // IMPORTANT:
        //   do not copy the members of this class around:
        //   whether "this" instance is inside a memory pool or not, that has to stay that way,
        //   regardless of whether "other" is inside a memory pool or not.

        m_boost_intrusive_pool_next = nullptr;
        m_boost_intrusive_pool_refcount = 0;
        m_boost_intrusive_pool_owner = nullptr;
    }
    virtual ~boost_intrusive_pool_item() {}

    //------------------------------------------------------------------------------
    // emulate the boost::intrusive_ref_counter class implementation:
    //------------------------------------------------------------------------------

    unsigned int use_count() const noexcept { return m_boost_intrusive_pool_refcount; }

    boost_intrusive_pool_item& operator=(const boost_intrusive_pool_item& other)
    {
        // IMPORTANT:
        //   do not copy the members of this class around:
        //   whether "this" instance is inside a memory pool or not, that has to stay that way,
        //   regardless of whether "other" is inside a memory pool or not.
        //   This is also what boost::intrusive_ref_counter implementation does.
        return *this;
    }
    boost_intrusive_pool_item& operator=(const boost_intrusive_pool_item&& other)
    {
        // IMPORTANT:
        //   do not copy the members of this class around:
        //   whether "this" instance is inside a memory pool or not, that has to stay that way,
        //   regardless of whether "other" is inside a memory pool or not.
        //   This is also what boost::intrusive_ref_counter implementation does.
        return *this;
    }

    //------------------------------------------------------------------------------
    // memorypool::boost_intrusive_pool private functions
    //------------------------------------------------------------------------------

    boost_intrusive_pool_item* _refcounted_item_get_next() { return m_boost_intrusive_pool_next; }
    void _refcounted_item_set_next(boost_intrusive_pool_item* p) { m_boost_intrusive_pool_next = p; }

    boost::intrusive_ptr<boost_intrusive_pool_iface> _refcounted_item_get_pool()
    {
        return m_boost_intrusive_pool_owner;
    }
    void _refcounted_item_set_pool(boost::intrusive_ptr<boost_intrusive_pool_iface> p)
    {
        m_boost_intrusive_pool_owner = p;
    }

    //------------------------------------------------------------------------------
    // default init-after-recycle, destroy-before-recycle methods:
    //------------------------------------------------------------------------------

    virtual void destroy() {}

    //------------------------------------------------------------------------------
    // memorypool utility functions
    //------------------------------------------------------------------------------

    // is_in_memory_pool() returns true both in case
    //  - this item is currently marked as "in use" in some memory pool
    //  - this item lies unused in some memory pool
    // This function returns false if e.g. this item has been allocated on the heap
    // bypassing any memory pool mechanism.
    bool is_in_memory_pool() const { return m_boost_intrusive_pool_owner != nullptr; }

    // sanity checks for this item. Useful for debug only.
    void check() const
    {
        if (is_in_memory_pool()) {
            assert(m_boost_intrusive_pool_refcount < BOOST_INTRUSIVE_POOL_DEBUG_MAX_REFCOUNT);

            if (m_boost_intrusive_pool_refcount == 0) {
                // this item is apparently inside the free list of the memory pool:
                // in such case it should be always linked to the list; the only case where
                // the "next" pointer can be NULL is in the case the memory pool is memory-bounded
                // and the free items are exhausted or the memory pool is infinite but the memory is over
                assert(m_boost_intrusive_pool_next != nullptr || m_boost_intrusive_pool_owner->is_bounded()
                    || m_boost_intrusive_pool_owner->is_memory_exhausted());
            } else {
                // this item is in use and thus must be UNLINKED from the free list of the memory pool:
                assert(m_boost_intrusive_pool_next == nullptr);
            }
        }
    }

private:
    size_t m_boost_intrusive_pool_refcount; // intrusive refcount
    boost_intrusive_pool_item* m_boost_intrusive_pool_next; // we use a free-list-based memory pool algorithm
    boost::intrusive_ptr<boost_intrusive_pool_iface>
        m_boost_intrusive_pool_owner; // used for auto-return to the memory pool
};

inline void intrusive_ptr_add_ref(boost_intrusive_pool_item* x)
{
#if BOOST_INTRUSIVE_POOL_DEBUG_CHECKS
    assert(x->m_boost_intrusive_pool_refcount < BOOST_INTRUSIVE_POOL_DEBUG_MAX_REFCOUNT - 1);
#endif
    ++x->m_boost_intrusive_pool_refcount;
}

inline void intrusive_ptr_release(boost_intrusive_pool_item* x)
{
#if BOOST_INTRUSIVE_POOL_DEBUG_CHECKS
    x->check();
#endif
    if (--x->m_boost_intrusive_pool_refcount == 0) {
        if (x->m_boost_intrusive_pool_owner)
            x->m_boost_intrusive_pool_owner->recycle(x);
        else
            // assume the item has been allocated out of the pool:
            delete x;
    }
}

//------------------------------------------------------------------------------
// boost_intrusive_pool_arena
// Internal helper class for a boost_intrusive_pool.
//------------------------------------------------------------------------------

// Arena of items. This is just an array of items and a pointer
// to another arena. All arenas are singly linked between them.
template <typename Item> class boost_intrusive_pool_arena {
public:
    // Creates an arena with arena_size items.
    boost_intrusive_pool_arena(size_t arena_size, boost::intrusive_ptr<boost_intrusive_pool_iface> p)
    {
        assert(arena_size > 0 && p);
        m_storage_size = arena_size;
        m_storage = new Item[arena_size];
        if (m_storage) {
            for (size_t i = 1; i < arena_size; i++) {
                m_storage[i - 1]._refcounted_item_set_next(&m_storage[i]);
                m_storage[i - 1]._refcounted_item_set_pool(p);
            }
            m_storage[arena_size - 1]._refcounted_item_set_next(nullptr);
            m_storage[arena_size - 1]._refcounted_item_set_pool(p);
        }
        // else: malloc failed!

        m_next_arena = nullptr;
    }

    boost_intrusive_pool_arena(const boost_intrusive_pool_arena& other) = delete;
    boost_intrusive_pool_arena(const boost_intrusive_pool_arena&& other) = delete;

    ~boost_intrusive_pool_arena()
    {
        // m_storage is automatically cleaned up
        if (m_storage) {
            delete[] m_storage;
            m_storage = nullptr;
        }
    }

    // Returns a pointer to the array of items. This is used by the arena
    // itself. This is only used to update free_list during initialization
    // or when creating a new arena when the current one is full.
    boost_intrusive_pool_item* get_first_item() const
    {
        return dynamic_cast<boost_intrusive_pool_item*>(&m_storage[0]);
    }

    size_t get_stored_item_count() const { return m_storage_size; }

    // Sets the next arena. Used when the current arena is full and
    // we have created this one to get more storage.
    void set_next_arena(boost_intrusive_pool_arena* p)
    {
        assert(!m_next_arena && p);
        m_next_arena = p;
        m_storage[m_storage_size - 1]._refcounted_item_set_next(p->get_first_item());
    }
    boost_intrusive_pool_arena* get_next_arena() { return m_next_arena; }
    const boost_intrusive_pool_arena* get_next_arena() const { return m_next_arena; }

    boost_intrusive_pool_arena operator=(const boost_intrusive_pool_arena& other) = delete;
    boost_intrusive_pool_arena operator=(const boost_intrusive_pool_arena&& other) = delete;

private:
    // Pointer to the next arena.
    boost_intrusive_pool_arena* m_next_arena;

    // Storage of this arena.
    // std::unique_ptr<Item[]> m_storage;
    size_t m_storage_size;
    Item* m_storage;
};

//------------------------------------------------------------------------------
// boost_intrusive_pool
// The actual memory pool implementation.
//------------------------------------------------------------------------------

template <class Item> class boost_intrusive_pool {
public:
    // using dummy = typename std::enable_if<std::is_base_of<boost_intrusive_pool_item, Item>::value>::type;

    // The type of pointers provided by this memory pool implementation
    using item_ptr = boost::intrusive_ptr<Item>;

    // The allocate function type
    using allocate_function = std::function<void(Item&)>;

    // The recycle function type
    // If specified the recycle function will be called every time a resource gets recycled into the pool. This allows
    // temporary resources, e.g., file handles to be closed when an object is longer used.
    using recycle_function = std::function<void(Item&)>;

public:
    // Default constructor
    // Constructs a memory pool quite small which increases its size by rather small steps.
    // Tuning of these steps is critical for performances.
    // The ctor also allows you to specify which function should be run on items returning to the pool.
    boost_intrusive_pool(size_t init_size = BOOST_INTRUSIVE_POOL_DEFAULT_POOL_SIZE,
        size_t enlarge_size = BOOST_INTRUSIVE_POOL_INCREASE_STEP, size_t max_size = BOOST_INTRUSIVE_POOL_NO_MAX_SIZE,
        recycle_method_e method = RECYCLE_METHOD_NONE, recycle_function recycle = nullptr)
    {
        assert(init_size > 0);
        assert((max_size == BOOST_INTRUSIVE_POOL_NO_MAX_SIZE) || (max_size >= init_size && enlarge_size > 0));

        m_pool = boost::intrusive_ptr<impl>(new impl(enlarge_size, max_size, method, recycle));

        // do initial malloc
        m_pool->enlarge(init_size);
    }
    virtual ~boost_intrusive_pool() { m_pool->trigger_self_destruction(); }

    // Copy constructor
    boost_intrusive_pool(const boost_intrusive_pool& other) = delete;

    // Move constructor
    boost_intrusive_pool(boost_intrusive_pool&& other) = delete;

    // Copy assignment
    boost_intrusive_pool& operator=(const boost_intrusive_pool& other) = delete;

    // Move assignment
    boost_intrusive_pool& operator=(boost_intrusive_pool&& other) = delete;

    //------------------------------------------------------------------------------
    // configuration methods
    //------------------------------------------------------------------------------

    void set_recycle_method(recycle_method_e method, recycle_function recycle_fn = nullptr)
    {
        m_pool->set_recycle_method(method, recycle_fn);
    }

    //------------------------------------------------------------------------------
    // allocate method variants
    //------------------------------------------------------------------------------

    item_ptr allocate()
    {
        Item* recycled_item = m_pool->allocate_safe_get_recycled_item();
        if (!recycled_item)
            return nullptr;

        // in this case we don't need to call ANY function

        item_ptr ret_ptr(recycled_item);
#if BOOST_INTRUSIVE_POOL_DEBUG_CHECKS
        ret_ptr->check();
#endif
        return ret_ptr;
    }

    // Returns first available free item or, if necessary and the memory pool is unbounded and has not reached the
    // maximum size, allocates a new item. Uses C++11 perfect forwarding to the init() function of the memory pooled
    // item.
    template <typename... Args> item_ptr allocate_through_init(Args&&... args)
    {
        Item* recycled_item = m_pool->allocate_safe_get_recycled_item();
        if (!recycled_item)
            return nullptr;

        // Construct the object in the obtained storage
        // using the init() method of the item itself:
        recycled_item->init(std::forward<Args>(args)...);

        // AFTER the init() call, run the check() function
        item_ptr ret_ptr(recycled_item);
#if BOOST_INTRUSIVE_POOL_DEBUG_CHECKS
        ret_ptr->check();
#endif
        return ret_ptr;
    }

    // Returns first available free item or, if necessary and the memory pool is unbounded,
    // allocates a new item.
    template <typename... Args> item_ptr allocate_through_function(allocate_function fn)
    {
        Item* recycled_item = m_pool->allocate_safe_get_recycled_item();
        if (!recycled_item)
            return nullptr;

        // Construct the object in the obtained storage
        // uses perfect forwarding to the class ctor:
        /// new (recycled_item) Item(std::forward<Args>(args)...);
        fn(*recycled_item);

        // relinking the item to the pool is instead a critical step: we just executed
        // the ctor of the recycled item; that resulted in a call to
        // boost_intrusive_pool_item::boost_intrusive_pool_item()!
        recycled_item->_refcounted_item_set_pool(m_pool);

        // AFTER the ctor call, run the check() function
        item_ptr ret_ptr(recycled_item);
#if BOOST_INTRUSIVE_POOL_DEBUG_CHECKS
        ret_ptr->check();
#endif
        return ret_ptr;
    }

    //------------------------------------------------------------------------------
    // other functions operating on items
    //------------------------------------------------------------------------------

    void clear()
    {
        // VERY IMPORTANT: this function is very tricky: to do this correctly we cannot
        // simply call m_pool->clear(): that would remove all arenas that are the memory
        // support of memory pool items.
        // At this point we don't know yet if there are boost::intrusive_ptr<> out there
        // still alive... so we must play safe:
        size_t init_size = m_pool->m_enlarge_step;
        size_t enlarge_size = m_pool->m_enlarge_step;
        size_t max_size = m_pool->m_max_size;
        recycle_method_e method = m_pool->m_recycle_method;
        recycle_function recycle = m_pool->m_recycle_fn;
        m_pool->trigger_self_destruction();
        m_pool = nullptr; // release old pool
        m_pool = boost::intrusive_ptr<impl>(new impl(enlarge_size, max_size, method, recycle));
    }

    void check() { m_pool->check(); }

    //------------------------------------------------------------------------------
    // getters
    //------------------------------------------------------------------------------

    // returns true if there are no elements in use.
    // Note that if empty()==true, it does not mean that capacity()==0 as well!
    bool empty() const { return m_pool->empty(); }

    bool is_bounded() const { return m_pool->is_bounded(); }

    bool is_limited() const { return m_pool->is_limited(); }

    bool is_memory_exhausted() const { return m_pool->is_memory_exhausted(); }

    // returns the current (=maximum) capacity of the object pool
    size_t capacity() const { return m_pool->capacity(); }

    size_t max_size() const { return m_pool->max_size(); }

    // returns the number of free entries of the pool
    size_t unused_count() const { return m_pool->unused_count(); }

    // returns the number of items currently malloc()ed from this pool
    size_t inuse_count() const { return m_pool->inuse_count(); }

    // returns the number of mallocs done so far
    size_t enlarge_steps_done() const { return m_pool->enlarge_steps_done(); }

private:
    /// The actual pool implementation. We use the
    /// enable_shared_from_this helper to make sure we can pass a
    /// "back-pointer" to the pooled objects. The idea behind this
    /// is that we need objects to be able to add themselves back
    /// into the pool once they go out of scope.
    class impl : public boost_intrusive_pool_iface {
    public:
        impl(size_t enlarge_size, size_t max_size, recycle_method_e method, recycle_function recycle)
        {
            // assert(enlarge_size > 0); // NOTE: enlarge_size can be zero to create a limited-size memory pool

            // configurations
            m_recycle_method = method;
            m_recycle_fn = recycle;
            m_enlarge_step = enlarge_size;
            m_max_size = max_size;

            // status
            m_first_free_item = nullptr;
            m_first_arena = nullptr;
            m_last_arena = nullptr;
            m_memory_exhausted = false;
            m_trigger_self_destruction = false;

            // stats
            m_free_count = 0;
            m_inuse_count = 0;
            m_total_count = 0;

#if BOOST_INTRUSIVE_POOL_DEBUG_THREAD_ACCESS
            m_allowed_thread = 0;
#endif
        }

        ~impl()
        {
            // if this dtor is called, it means that all memory pooled items have been destroyed:
            // they are holding a shared_ptr<> back to us, so if one of them was alive, this dtor would not be called!
            clear();
        }

        void set_recycle_method(recycle_method_e method, recycle_function recycle = nullptr)
        {
            m_recycle_method = method;
            m_recycle_fn = recycle;
        }

        void trigger_self_destruction()
        {
            m_trigger_self_destruction = true;

            // walk over the free list and reduce our own refcount by removing the link between the items and ourselves:
            // this is important because it allows the last item that will return to this pool to trigger the pool
            // dtor: see recycle() implementation
            boost_intrusive_pool_item* pcurr = m_first_free_item;
            while (pcurr) {
                pcurr->_refcounted_item_set_pool(nullptr);
                pcurr = pcurr->_refcounted_item_get_next();
            }
        }

        size_t get_effective_enlarge_step() const
        {
            size_t enlarge_step = m_enlarge_step;
            if (m_enlarge_step > 0 && (m_max_size > 0 && (m_total_count + m_enlarge_step > m_max_size)))
                enlarge_step = m_max_size - m_total_count; // enlarge_step can be zero if we reach the max_size
            return enlarge_step;
        }

        Item* allocate_safe_get_recycled_item()
        {
#if BOOST_INTRUSIVE_POOL_DEBUG_THREAD_ACCESS
            if (m_allowed_thread == 0)
                m_allowed_thread = pthread_self();
            else
                assert(m_allowed_thread == pthread_self());
#endif

            if (m_free_count == 0) {
                assert(m_first_free_item == nullptr);
                size_t enlarge_step = get_effective_enlarge_step();
                if (enlarge_step == 0 || !enlarge(enlarge_step)) {
                    m_memory_exhausted = true;
                    return nullptr; // allocation by enlarge() failed or this is a fixed-size memory pool!
                }
            }

            // get first item from free list
            assert(m_first_free_item != nullptr);
            Item* recycled_item = dynamic_cast<Item*>(m_first_free_item); // downcast (base class -> derived class)
            assert(recycled_item != nullptr); // we always allocate all items of the same type,
                                              // so the dynamic cast cannot fail
            assert(
                recycled_item->_refcounted_item_get_pool().get() == this); // this was set during arena initialization
                                                                           // and must be valid at all times

            // update stats
            m_free_count--;
            m_inuse_count++;

            // update the pointer to the next free item available
            m_first_free_item = m_first_free_item->_refcounted_item_get_next();
            if (m_first_free_item == nullptr && m_enlarge_step > 0) {
                size_t enlarge_step = get_effective_enlarge_step();
                if (enlarge_step == 0) { // enlarge_step can be zero if we reach the max_size
                    m_memory_exhausted = true;
                } else {
                    // this is a memory pool which can be still enlarged:
                    // exit the function leaving the m_first_free_item as a valid pointer to a free item!
                    // this is just to simplify debugging and make more effective the check() function implementation!

                    assert(m_free_count == 0);
                    if (!enlarge(enlarge_step)) {
                        m_memory_exhausted = true;
                        // We tried to fetch memory from the O.S. but we failed. However we succeeded in getting the
                        // last available item. So fallback and provide that last item to the caller.
                    }
                }
            }

            // unlink the item to return
            recycled_item->_refcounted_item_set_next(nullptr);
            return recycled_item;
        }

        bool enlarge(size_t arena_size)
        {
#if BOOST_INTRUSIVE_POOL_DEBUG_THREAD_ACCESS
            assert(m_allowed_thread == 0 || m_allowed_thread == pthread_self());
#endif
            // If the current arena is full, create a new one.
            boost_intrusive_pool_arena<Item>* new_arena = new boost_intrusive_pool_arena<Item>(arena_size, this);
            if (!new_arena)
                return false; // malloc failed... memory finished... very likely this is a game over

            // Link the new arena to the last one.
            if (m_last_arena)
                m_last_arena->set_next_arena(new_arena);
            if (m_first_arena == nullptr)
                m_first_arena = new_arena; // apparently we are initializing the memory pool for the very first time

            // Seek pointer to last arena
            m_last_arena = new_arena;

            // Update the free_list with the storage of the just created arena.
            if (m_first_free_item == nullptr)
                m_first_free_item = m_last_arena->get_first_item();

            m_free_count += arena_size;
            m_total_count += arena_size;

            return true;
        }

        virtual void recycle(boost_intrusive_pool_item* pitem_base) override
        {
#if BOOST_INTRUSIVE_POOL_DEBUG_THREAD_ACCESS
            assert(m_allowed_thread != 0);
            assert(m_allowed_thread == pthread_self());
#endif
            assert(pitem_base
                && pitem_base->_refcounted_item_get_next()
                    == nullptr); // Recycling an item that has been already recycled?
            assert(pitem_base->_refcounted_item_get_pool().get() == this);

            Item* pitem = dynamic_cast<Item*>(pitem_base); // downcast (base class -> derived class)
            assert(pitem != nullptr); // we always allocate all items of the same type,
                                      // so the dynamic cast cannot fail
            switch (m_recycle_method) {
            case RECYCLE_METHOD_NONE:
                break;

            case RECYCLE_METHOD_DESTROY_FUNCTION:
                pitem->destroy();
                break;

            case RECYCLE_METHOD_CUSTOM_FUNCTION:
                m_recycle_fn(*pitem);
                break;

                // the big problem with using the class destructor is that the virtual table of the item will
                // be destroyed; attempting to dynamic_cast<> the item later will fail (NULL returned).
                // so this recycling option is disabled for now
                // case RECYCLE_METHOD_DTOR:
                // Destroy the object using its dtor:
                // pitem->Item::~Item();
                // break;
            }

            // sanity check:
            if (!is_bounded()) {
                assert(m_first_free_item != nullptr || m_memory_exhausted);
            }

            // Add the item at the beginning of the free list.
            pitem_base->_refcounted_item_set_next(m_first_free_item);
            m_first_free_item = pitem_base;
            m_free_count++;

            assert(m_inuse_count > 0);
            m_inuse_count--;

#if BOOST_INTRUSIVE_POOL_DEBUG_CHECKS
            pitem_base->check();
#endif

            // test for self-destruction:
            // is this an orphan pool (i.e. a pool without any boost_intrusive_pool<> associated to it anymore)?
            if (m_trigger_self_destruction) {
                // in such case break the link between the items being recycled and this pool:
                // in this way the very last memory-pooled item returning to this pool will lower the refcount
                // to this pool to zero, and impl::~impl() will get called, freeing all arenas!
                pitem->_refcounted_item_set_pool(nullptr);
            }
        }

        //------------------------------------------------------------------------------
        // other functions operating on items
        //------------------------------------------------------------------------------

        // THIS IS A SUPER DANGEROUS FUNCTION: IT JUST REMOVES ALL ARENAS OF THIS MEMORY POOL WITHOUT ANY
        // CHECK WHETHER THERE ARE boost::intrusive_ptr<> OUT THERE STILL POINTING TO ITEMS INSIDE THOSE
        // ARENAS. USE
        void clear()
        {
            if (m_first_arena) {
                size_t init_size = m_first_arena->get_stored_item_count();
                boost_intrusive_pool_arena<Item>* pcurr = m_first_arena;
                while (pcurr) {
                    boost_intrusive_pool_arena<Item>* pnext = pcurr->get_next_arena();
                    delete pcurr;
                    pcurr = pnext;
                }
            } else {
                // this memory pool has just been clear()ed... the last arena pointer should be null as well:
                assert(m_last_arena == nullptr);
            }

            // status
            m_first_free_item = nullptr;
            m_first_arena = nullptr;
            m_last_arena = nullptr;
            m_memory_exhausted = false;

            // stats
            m_free_count = 0;
            m_inuse_count = 0;
            m_total_count = 0;
        }

        void check()
        {
            if (m_first_arena) {
                // this memory pool has been correctly initialized
                assert(m_last_arena);
                assert(m_total_count > 0);

                // this condition should hold at any time:
                assert(m_free_count + m_inuse_count == m_total_count);
                if (is_bounded()) {
                    // when the memory pool is bounded it contains only 1 arena of a fixed size:
                    assert(m_first_arena == m_last_arena);
                } else {
                    // infinite or max size memory pool: either we have a valid free element or the last malloc() must
                    // have failed or the maximum size has been reached:
                    assert(m_first_free_item != nullptr || m_memory_exhausted);
                }
            } else {
                // this memory pool has just been cleared with clear() apparently:
                assert(!m_last_arena);
                assert(!m_first_free_item);
                assert(m_free_count == 0);
                assert(m_inuse_count == 0);
                assert(m_total_count == 0);
            }
        }

        //------------------------------------------------------------------------------
        // getters
        //------------------------------------------------------------------------------

        // returns true if there are no elements in use.
        // Note that if empty()==true, it does not mean that capacity()==0 as well!
        bool empty() const { return m_free_count == m_total_count; }

        bool is_bounded() const final { return m_enlarge_step == 0; }

        bool is_limited() const { return (m_enlarge_step == 0 || m_max_size != 0); }

        bool can_be_enlarged() const { return m_enlarge_step > 0 && (m_max_size == 0 || m_total_count < m_max_size); }

        bool is_memory_exhausted() const final { return m_memory_exhausted; }

        // returns the current (=maximum) capacity of the object pool
        size_t capacity() const { return m_total_count; }

        size_t max_size() const { return (m_enlarge_step != 0) ? m_max_size : m_total_count; }

        // returns the number of free entries of the pool
        size_t unused_count() const { return m_free_count; }

        // returns the number of items currently malloc()ed from this pool
        size_t inuse_count() const { return m_inuse_count; }

        // returns the number of mallocs done so far
        size_t enlarge_steps_done() const
        {
            size_t num_arenas_allocated = 0;

            const boost_intrusive_pool_arena<Item>* pcurr = m_first_arena;
            while (pcurr) {
                pcurr = pcurr->get_next_arena();
                num_arenas_allocated++;
            }

            return num_arenas_allocated;
        }

    public:
        // The recycle strategy & function
        recycle_method_e m_recycle_method;
        recycle_function m_recycle_fn;

        // How many new items to add each time the pool becomes full?
        // If this is zero, then this is a bounded pool, which cannot grow beyond
        // the initial size provided at construction time.
        size_t m_enlarge_step;
        // Maximum pool size.
        // If this is zero, then it is ignored.
        // If this is greater then zero, then no more items will be added to the pull if resulting size would exceed
        // this value. If enlarge_step is zero, the max_size parameter become meaningless.
        size_t m_max_size;

        // Pointers to first and last arenas.
        // First arena is changed only at
        //  - construction time
        //  - in clear()
        //  - in reset(size_t)
        // Pointer to last arena is instead updated on every enlarge step.
        boost_intrusive_pool_arena<Item>* m_first_arena;
        boost_intrusive_pool_arena<Item>* m_last_arena;

        // List of free elements. The list can be threaded between different arenas
        // depending on the deallocation pattern.
        // This pointer can be NULL only whether:
        // - an infinite memory pool has exhausted memory (malloc returned NULL);
        // - a bounded memory pool has exhausted all its items
        // - a maximum size memory pool has exhausted all its items and reached the limit
        // In such cases m_memory_exhausted==true
        boost_intrusive_pool_item* m_first_free_item;
        // This flag can be true if allocation by enlarge() failed or this is a fixed-size memory pool or this is a
        // maximum size memory pool!
        bool m_memory_exhausted;

        // stats
        // This should hold always:
        //         m_free_count+m_inuse_count == m_total_count
        size_t m_free_count;
        size_t m_inuse_count;
        size_t m_total_count;

        bool m_trigger_self_destruction;

#if BOOST_INTRUSIVE_POOL_DEBUG_THREAD_ACCESS
        pthread_t m_allowed_thread;
#endif
    };

private:
    // The pool impl
    boost::intrusive_ptr<impl> m_pool;
};

} // namespace memorypool
