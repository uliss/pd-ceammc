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
#ifndef CEAMMC_DATAMANAGER_H
#define CEAMMC_DATAMANAGER_H

#include "ceammc_datapointer.h"

namespace ceammc {

typedef Data<BaseData>* (*GetDataFn)(DataId id);
typedef Data<BaseData>* (*CloneDataFn)(DataId id);

class DataManager {
    typedef std::map<DataType, GetDataFn> GetFnMap;
    typedef std::map<DataType, CloneDataFn> CloneFnMap;
    GetFnMap fn_get_;
    CloneFnMap fn_clone_;
    DataManager();

public:
    static DataManager& instance();
    void addGetFn(DataType type, GetDataFn fn);
    void addCloneFn(DataType type, CloneDataFn fn);
    Data<BaseData>* clone(const DataDesc& d);
    Data<BaseData>* get(const DataDesc& d);
};

namespace data {
    template <class T>
    bool registerData()
    {
        DataManager::instance().addGetFn(T::dataType, reinterpret_cast<GetDataFn>(Data<T>::getData));
        DataManager::instance().addCloneFn(T::dataType, reinterpret_cast<CloneDataFn>(Data<T>::cloneData));
        return true;
    }
}
}

#endif // CEAMMC_DATAMANAGER_H
