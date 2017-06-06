/*****************************************************************************
 * Copyright 2017 Serge Poltavsky. All rights reserved.
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
#ifndef CEAMMC_DATASTORAGE_H
#define CEAMMC_DATASTORAGE_H

#include <algorithm>
#include <boost/unordered_map.hpp>

#include "ceammc_datapointer.h"

namespace ceammc {

template <class T>
class Data;

template <class T>
class DataStorage {
public:
    typedef Data<T>* DataPtr;
    typedef boost::unordered_map<DataId, DataPtr> Map;

public:
    /**
     * Singleton pattern
     */
    static DataStorage& instance();

public:
    /**
     * @return new generated (not used) ID
     */
    DataId generateId();

    /**
     * get data from storage by ID
     * @param id - data id
     * @return pointer to data on success, or 0-pointer if data is not found for given ID
     */
    Data<T>* get(DataId id);

    /**
     * adds new created data pointer to storage
     * @param id - data id
     * @param ptr - pointer to data
     * @return true on success, false if data with given ID already exists
     */
    bool add(DataId id, Data<T>* ptr);

    /**
     * removes data from storage
     * @param id - data id
     * @return true on success, false on error
     */
    bool remove(DataId id);

    /**
     * Returns number of data pointers in storage
     */
    size_t count() const;

private:
    DataStorage() {}

private:
    Map storage_;
};

template <class T>
DataStorage<T>& DataStorage<T>::instance()
{
    static DataStorage ds;
    return ds;
}

template <class T>
size_t DataStorage<T>::count() const
{
    return storage_.size();
}

template <class T>
static inline bool key_compare(
    const typename DataStorage<T>::Map::value_type& a,
    const typename DataStorage<T>::Map::value_type& b)
{
    return a.first < b.first;
}

template <class T>
DataId DataStorage<T>::generateId()
{
    if (storage_.empty())
        return 1;

    DataId max = std::max_element(storage_.begin(), storage_.end(), key_compare<T>)->first;
    const size_t sz = storage_.size();

    if (max == sz)
        return max + 1;
    else {
        for (DataId i = 1; i < std::max<DataId>(sz, max); i++) {
            if (storage_.find(i) == storage_.end())
                return i;
        }
    }

    // should never happen
    return static_cast<DataId>(-1);
}

template <class T>
Data<T>* DataStorage<T>::get(DataId id)
{
    typename Map::iterator it = storage_.find(id);
    return it == storage_.end() ? 0 : it->second;
}

template <class T>
bool DataStorage<T>::add(DataId id, Data<T>* ptr)
{
    typename Map::iterator it = storage_.find(id);
    if (it != storage_.end())
        return false;

    storage_.insert(std::make_pair(id, ptr));
    return true;
}

template <class T>
bool DataStorage<T>::remove(DataId id)
{
    typename Map::iterator it = storage_.find(id);
    if (it == storage_.end())
        return false;

    storage_.erase(it);
    return true;
}
}

#endif // CEAMMC_DATASTORAGE_H
