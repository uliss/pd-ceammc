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

#include "ceammc_data.h"

namespace ceammc {

/**
 * Typed data storage. For each data type it stores mapping between DataId and pointer
 * to data itself.
 * Mainly for internal use. But if you know data type, you can access data by its DataId.
 */
class DataStorage {
public:
    typedef boost::unordered_map<DataDesc, Data*> Map;

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
     * Generates new atom descriptor (not used)
     */
    DataDesc generateNewDesc(AbstractData* d);

    /**
     * get data from storage by ID
     * @param id - data descriptor
     * @return pointer to data on success, or 0-pointer if data is not found for given ID
     */
    Data* get(const DataDesc& d);

    /**
     * adds new created data pointer to storage
     * @param id - data id
     * @param ptr - pointer to data
     * @return true on success, false if data with given ID already exists
     */
    bool add(const DataDesc& d, Data* ptr);

    /**
     * removes data from storage
     * @param id - data id
     * @return true on success, false on error
     */
    bool remove(const DataDesc& d);

    /**
     * Returns number of data pointers in storage
     */
    size_t count() const;

private:
    DataStorage() {}

private:
    Map storage_;
};
}

#endif // CEAMMC_DATASTORAGE_H
