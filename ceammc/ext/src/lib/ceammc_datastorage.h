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
#ifndef CEAMMC_XDATASTORAGE_H
#define CEAMMC_XDATASTORAGE_H

#include "ceammc_abstractdata.h"

#include <boost/unordered_map.hpp>

namespace ceammc {
class DataStorage {
    struct Entry {
        int ref_count;
        const AbstractData* data;
    };

    typedef boost::unordered_map<DataDesc, Entry> DataMap;

    DataMap map_;

    DataStorage();
    DataStorage(const DataStorage& s);
    void operator=(const DataStorage& s);

public:
    static DataStorage& instance();
    size_t size() const;
    DataDesc add(const AbstractData* data);

    const AbstractData* acquire(const DataDesc& desc);
    void release(const DataDesc& desc);
    size_t refCount(const DataDesc& desc);

    DataId generateId(const AbstractData* data);
};

size_t hash_value(const DataDesc& d);
}

#endif // CEAMMC_XDATASTORAGE_H
