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
#include "ceammc_datatypes.h"

#include <boost/functional/hash.hpp>

using namespace ceammc;

size_t DataStorageHasher::operator()(const DataDesc& d) const
{
    size_t res = d.type;
    boost::hash_combine(res, boost::hash_value(d.id));
    return res;
}

DataStorage::DataStorage()
{
}

DataStorage& DataStorage::instance()
{
    static DataStorage s;
    return s;
}

size_t DataStorage::size() const
{
    return map_.size();
}

DataDesc DataStorage::add(const AbstractData* data)
{
    if (data == 0)
        return DataDesc(data::DATA_INVALID, DataId(-1));

    DataDesc desc(data->type(), generateId(data));
    auto it = map_.find(desc);
    if (it != map_.end()) {
        it->second.ref_count++;
        return desc;
    }

    Entry entry;
    entry.ref_count = 1;
    entry.data = data;
    map_[desc] = entry;
    return desc;
}

const AbstractData* DataStorage::acquire(const DataDesc& desc)
{
    auto it = map_.find(desc);
    if (it == map_.end())
        return nullptr;

    it->second.ref_count++;
    return it->second.data;
}

void DataStorage::release(const DataDesc& desc)
{
    auto it = map_.find(desc);
    if (it == map_.end())
        return;

    it->second.ref_count--;
    if (it->second.ref_count == 0) {
        delete it->second.data;
        map_.erase(desc);
    }
}

size_t DataStorage::refCount(const DataDesc& desc)
{
    auto it = map_.find(desc);
    return (it == map_.end()) ? 0 : it->second.ref_count;
}

DataId DataStorage::generateId(const AbstractData* data)
{
    size_t hash = 0;
    boost::hash_combine(hash, reinterpret_cast<ptrdiff_t>(data));
    boost::hash_combine(hash, data->type());
    // NB: data type truncation!
    Atom a(DataDesc(0, DataId(hash)));
    return a.getData().id;
}
