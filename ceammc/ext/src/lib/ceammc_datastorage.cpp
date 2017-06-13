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

#include <boost/functional/hash.hpp>

namespace ceammc {

DataStorage& DataStorage::instance()
{
    static DataStorage ds;
    return ds;
}

size_t DataStorage::count() const
{
    return storage_.size();
}

static inline bool key_compare(
    const DataStorage::Map::value_type& a,
    const DataStorage::Map::value_type& b)
{
    return a.first.id < b.first.id;
}

struct FindById {
    DataId id_;
    FindById(DataId id)
        : id_(id)
    {
    }

    bool operator()(const DataStorage::Map::value_type& a) const
    {
        return a.first.id == id_;
    }
};

DataId DataStorage::generateId()
{
    if (storage_.empty())
        return 1;

    DataDesc max = std::max_element(storage_.begin(), storage_.end(), key_compare)->first;
    const size_t sz = storage_.size();

    if (max.id == sz)
        return max.id + 1;
    else {
        for (DataId i = 1; i < std::max<DataId>(sz, max.id); i++) {
            if (std::find_if(storage_.begin(), storage_.end(), FindById(i)) == storage_.end())
                return i;
        }
    }

    // should never happen
    return static_cast<DataId>(-1);
}

DataDesc DataStorage::generateNewDesc(AbstractData* d)
{
    if (!d)
        return DataDesc(0, 0);

    return DataDesc(d->type(), generateId());
}

Data* DataStorage::get(const DataDesc& d)
{
    Map::iterator it = storage_.find(d);
    return it == storage_.end() ? 0 : it->second;
}

bool DataStorage::add(const DataDesc& d, Data* ptr)
{
    Map::iterator it = storage_.find(d);
    if (it != storage_.end())
        return false;

    storage_.insert(std::make_pair(d, ptr));
    return true;
}

bool DataStorage::remove(const DataDesc& d)
{
    Map::iterator it = storage_.find(d);
    if (it == storage_.end())
        return false;

    storage_.erase(it);
    return true;
}

size_t hash_value(const DataDesc& d)
{
    size_t res = d.type;
    boost::hash_combine(res, boost::hash_value(d.id));
    return res;
}
}
