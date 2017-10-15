/*****************************************************************************
 * Copyright 2016 Serge Poltavsky. All rights reserved.
 *
 * This file may be distributed under the terms of GNU Public License version
 * 3 (GPL v3) as defined by the Free Software Foundation (FSF). A copy of the
 * license should have been included with this file, or the project in which
 * this file belongs to. You may also find the details of GPL v3 at:
 * http://www.gnu.org/licenses/gpl-3.0.txt
 *
 * If you have any questions regarding the use of this file, feel free to
 * contact the author of this file, or the owner of the project in which
 * this file belongs to.
 *****************************************************************************/
#ifndef CEAMMC_SHAREDDATA_H
#define CEAMMC_SHAREDDATA_H

#include <iostream>
#include <map>
#include <string>
#include <utility>
#include <vector>

#include <m_pd.h>

namespace ceammc {

template <typename T>
class NamedDataDict {
public:
    typedef std::pair<size_t, T*> Value;
    typedef std::map<std::string, Value> Map;
    typedef typename Map::iterator iterator;
    typedef typename Map::const_iterator const_iterator;

private:
    Map map_;
    inline void ref_remove(iterator it) { map_.erase(it); }
    static inline bool ref_is_zero(iterator it) { return it->second.first == 0; }
    static inline void ref_free(iterator it) { delete it->second.second; }
    static inline void ref_inc(iterator it) { ++(it->second.first); }
    static inline void ref_dec(iterator it) { --(it->second.first); }
    static T* ref_val(iterator it) { return it->second.second; }

public:
    NamedDataDict() {}

    ~NamedDataDict()
    {
        if (size() != 0) {
            std::cerr << "[ERROR | NamedDataDict] not all elements were released:\n ";
        }

        for (iterator it = map_.begin(); it != map_.end(); ++it) {
            std::cerr << "\t" << it->first << ": refcount = " << it->second.first << "\n";
            delete it->second.second;
        }
    }

    /**
     * @brief check if dict contains named element
     * @param name - name of element
     * @return true if element exists
     */
    bool contains(const std::string& name)
    {
        return map_.find(name) != map_.end();
    }

    /**
     * @brief size of dictionary - number of key/value pairs.
     * @return
     */
    size_t size() const
    {
        return map_.size();
    }

    /**
     * @brief refCount for named element
     * @param name - element name
     * @return element reference count or 0, if element not found in dict
     */
    size_t refCount(const std::string& name) const
    {
        const_iterator it = map_.find(name);
        if (it == map_.end())
            return 0;

        return it->second.first;
    }

    /**
     * @brief adds new named element in dict
     * @param name - element name
     * @param p - pointer to element
     * @return true if new element inserted, false if element already exists
     */
    bool create(const std::string& name, T* p)
    {
        iterator it = map_.find(name);
        if (it != map_.end())
            return false;

        map_[name] = std::make_pair(0, p);
        return true;
    }

    /**
     * @brief returns acquired object and increases ref count
     * @param name - element name
     * @return pointer to element or NULL if not found
     */
    T* acquire(const std::string& name)
    {
        iterator it = map_.find(name);
        if (it == map_.end())
            return 0;

        ref_inc(it);
        return ref_val(it);
    }

    /**
     * @brief release previously acquired object and decrease ref count
     * @param name - element name
     * @return true on succesfull release or false if object not found
     */
    bool release(const std::string& name)
    {
        iterator it = map_.find(name);
        if (it == map_.end())
            return false;

        // not acquired value found:
        if (ref_is_zero(it)) {
            std::cerr << "[ERROR | NamedDataDict] not acquired element: " << it->first << "\n";
            ref_free(it);
            ref_remove(it);
            return true;
        }

        // decrement
        ref_dec(it);

        // free is zero
        if (ref_is_zero(it)) {
            ref_free(it);
            ref_remove(it);
        }

        return true;
    }

    /**
      * Retrieve all dict keys
      */
    void keys(std::vector<std::string>& res) const
    {
        typename Map::const_iterator it;
        for (it = map_.begin(); it != map_.end(); ++it) {
            res.push_back(it->first);
        }
    }
};

template <typename T>
class GlobalData {
private:
    typedef typename NamedDataDict<T>::iterator iterator;
    static NamedDataDict<T> data_;
    const static int log_level = 0;

private:
    T* ptr_;
    std::string name_;
    std::string descr_;

public:
    /**
     * @brief creates GlobalData<T> with given name. T should have default contstructor.
     * @param name - data name
     * @param descr - data description
     */
    GlobalData(const std::string& name, const std::string& desc = "")
        : ptr_(0)
        , name_(name)
        , descr_(desc)
    {
        ptr_ = data_.acquire(name);
        if (ptr_ == 0) { // if not found
            data_.create(name, new T());
            ptr_ = data_.acquire(name);

            verbose(log_level, "[%s %s] created", descr_.c_str(), name_.c_str());
        }

        verbose(log_level, "[%s %s] +1", descr_.c_str(), name_.c_str());
    }

    ~GlobalData()
    {
        verbose(log_level, "[%s %s] -1", descr_.c_str(), name_.c_str());
        data_.release(name_);

        if (!data_.contains(name_))
            verbose(log_level, "[%s %s] destroyed", descr_.c_str(), name_.c_str());
    }

    /**
     * You can access data via pointer
     */
    T* operator->() { return ptr_; }
    const T* operator->() const { return ptr_; }

    /**
     * Returns data name
     */
    std::string name() const { return name_; }

    /**
     * Returns data description
     */
    std::string description() const { return descr_; }

    /**
     * Returns reference to data
     */
    T& ref() { return *ptr_; }
    const T& ref() const { return *ptr_; }

    /**
      * Returns number of references to global data
      */
    size_t refCount() const { return data_.refCount(name_); }

    /**
      * Retrieve all dict keys
      */
    static void keys(std::vector<std::string>& res)
    {
        data_.keys(res);
    }
};

template <typename T>
NamedDataDict<T> GlobalData<T>::data_;
}

#endif // CEAMMC_SHAREDDATA_H
