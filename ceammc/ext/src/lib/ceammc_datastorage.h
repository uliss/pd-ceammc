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

#include <boost/container/static_vector.hpp>
#include <cstdint>
#include <map>
#include <string>
#include <utility>
#include <vector>

namespace ceammc {

class AtomList;

using DictEntry = std::pair<std::string, AtomList>;
using Dict = std::vector<DictEntry>;
using CreateFromListFn = Atom (*)(const AtomList&);
using CreateFromDictFn = Atom (*)(const Dict&);

class DataStorage {

    struct DataTypeRecord {
        int type;
        std::string name;
        CreateFromListFn from_list_fn;
        CreateFromDictFn from_dict_fn;

        DataTypeRecord(int type_,
            const std::string& name_,
            CreateFromListFn list_fn,
            CreateFromDictFn dict_fn)
            : type(type_)
            , name(name_)
            , from_list_fn(list_fn)
            , from_dict_fn(dict_fn)
        {
        }
    };

    using TypeList = boost::container::static_vector<DataTypeRecord, 1024>;
    using type_iterator = TypeList::const_iterator;

    DataStorage();
    DataStorage(const DataStorage& s) = delete;
    void operator=(const DataStorage& s) = delete;

    type_iterator findByName(const std::string& name) const;
    type_iterator findByType(int type) const;

public:
    /**
     * DataStorage instance
     */
    static DataStorage& instance();

    // types functions
    int registerNewType(const std::string& name,
        CreateFromListFn fromListFn = nullptr,
        CreateFromDictFn fromDictFn = nullptr);

    int typeByName(const std::string& name) const;
    std::string nameByType(int type) const;
    CreateFromListFn fromListFunction(const std::string& name) const;
    CreateFromDictFn fromDictFunction(const std::string& name) const;

    void clearAll();

private:
    TypeList type_list_;
};

}

#endif // CEAMMC_XDATASTORAGE_H
