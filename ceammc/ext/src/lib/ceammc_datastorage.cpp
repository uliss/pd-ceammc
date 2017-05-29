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
#include "ceammc_datastorage.h"

#include <algorithm>

namespace ceammc {

DataStorage::DataStorage()
{
}

DataStorage& DataStorage::instance()
{
    static DataStorage ds;
    return ds;
}

static inline bool key_compare(
    const TypedDataPointerMap::value_type& a,
    const TypedDataPointerMap::value_type& b)
{
    return a.first < b.first;
}

DataId DataStorage::generateId(DataType type)
{
    TypedDataPointerMap* t = typeStorage(type);
    if (!t)
        return static_cast<DataId>(-1);

    if (t->empty())
        return 1;

    DataId max = std::max_element(t->begin(), t->end(), key_compare)->first;
    const size_t sz = t->size();

    if (max == sz)
        return max + 1;

    // we have gaps
    if (max < sz) {
        for (DataId i = 1; i <= sz; i++) {
            if (t->find(i) == t->end())
                return i;
        }
    }

    // should never happen
    return static_cast<DataId>(-1);
}

bool DataStorage::add(DataType type, DataId id, Data* ptr)
{
    TypedDataPointerMap* tmap = typeStorage(type);
    if (!tmap)
        return false;

    TypedDataPointerMap::iterator it = tmap->find(id);
    if (it != tmap->end())
        return false;

    tmap->insert(std::make_pair(id, ptr));
    return true;
}

bool DataStorage::remove(DataType type, DataId id)
{
    TypedDataPointerMap* tmap = typeStorage(type);
    if (!tmap)
        return false;

    TypedDataPointerMap::iterator it = tmap->find(id);
    if (it == tmap->end())
        return false;

    tmap->erase(it);
    return true;
}

bool DataStorage::addNewType(DataType type)
{
    TypedDataPointerMap* tmap = typeStorage(type);
    if (tmap == 0) {
        storage_.insert(std::make_pair(type, new TypedDataPointerMap));
        return true;
    }

    return false;
}

size_t DataStorage::count(DataType type)
{
    TypedDataPointerMap* tmap = typeStorage(type);
    return tmap == 0 ? 0 : tmap->size();
}

Data* DataStorage::get(DataType type, DataId id)
{
    TypedDataPointerMap* tmap = typeStorage(type);
    if (!tmap)
        return 0;

    TypedDataPointerMap::iterator it = tmap->find(id);
    return it == tmap->end() ? 0 : it->second;
}

TypedDataPointerMap* DataStorage::typeStorage(DataType type)
{
    DataPointerMap::iterator it = storage_.find(type);
    return it != storage_.end() ? it->second : 0;
}
}
