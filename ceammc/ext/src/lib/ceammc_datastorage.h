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
#include "ceammc_data.h"

#include <boost/container/static_vector.hpp>
#include <map>

namespace ceammc {

constexpr size_t DATA_TYPES_MAX = 1024;

using CreateFromListFn = Atom (*)(const AtomListView&);
using CreateFromDictFn = Atom (*)(const DictAtom&);

class DataStorage {

    struct DataTypeRecord {
        DataTypeName name;
        CreateFromListFn from_list_fn;
        CreateFromDictFn from_dict_fn;
        DataTypeId type;

        DataTypeRecord(DataTypeId type_,
            const DataTypeName& name_,
            CreateFromListFn list_fn,
            CreateFromDictFn dict_fn)
            : name(name_)
            , from_list_fn(list_fn)
            , from_dict_fn(dict_fn)
            , type(type_)
        {
        }

        DataTypeRecord(DataTypeId type_,
            const char* name_,
            CreateFromListFn list_fn,
            CreateFromDictFn dict_fn)
            : name(name_)
            , from_list_fn(list_fn)
            , from_dict_fn(dict_fn)
            , type(type_)
        {
        }
    };

    using TypeList = boost::container::static_vector<DataTypeRecord, DATA_TYPES_MAX>;
    using type_iterator = TypeList::const_iterator;

    DataStorage();
    DataStorage(const DataStorage& s) = delete;
    void operator=(const DataStorage& s) = delete;

    type_iterator findByName(const DataTypeName& name) const;
    type_iterator findByType(DataTypeId type) const;

public:
    /**
     * DataStorage instance
     */
    static DataStorage& instance();

    // types functions
    DataTypeId registerNewType(const DataTypeName& name,
        CreateFromListFn fromListFn = nullptr,
        CreateFromDictFn fromDictFn = nullptr);

    DataTypeId typeByName(const DataTypeName& name) const;
    DataTypeName nameByType(DataTypeId type) const;
    CreateFromListFn fromListFunction(const DataTypeName& name) const;
    CreateFromDictFn fromDictFunction(const DataTypeName& name) const;

    void clearAll();

private:
    TypeList type_list_;
};

}

#endif // CEAMMC_XDATASTORAGE_H
