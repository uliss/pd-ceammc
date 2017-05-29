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

#include <boost/unordered_map.hpp>

#include "ceammc_datapointer.h"

namespace ceammc {

typedef boost::unordered_map<DataId, DataPointer*> TypedDataPointerMap;
typedef boost::unordered_map<DataType, TypedDataPointerMap*> DataPointerMap;

class DataStorage {
    DataPointerMap storage_;
    DataStorage();

public:
    static DataStorage& instance();

public:
    DataId generateId(DataType type);
    DataPointer* get(DataType type, DataId id);
    bool add(DataType type, DataId id, DataPointer* ptr);
    bool remove(DataType type, DataId id);

    bool addNewType(DataType type);

    size_t count(DataType type);

private:
    TypedDataPointerMap* typeStorage(DataType type);
};
}

#endif // CEAMMC_DATASTORAGE_H
