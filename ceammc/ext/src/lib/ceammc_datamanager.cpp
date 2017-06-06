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
#include "ceammc_datamanager.h"
#include "ceammc_log.h"

namespace ceammc {

DataManager::DataManager()
{
}

DataManager& DataManager::instance()
{
    static DataManager fact;
    return fact;
}

void DataManager::registerData(DataType type, const DataManagerFnRecord& rec)
{
    LIB_DBG << "registered data type: " << type;
    fn_[type] = rec;
}

Data<AbstractData>* DataManager::clone(const DataDesc& d)
{
    FnMap::iterator it = fn_.find(d.type);
    if (it == fn_.end()) {
        LIB_ERR << "type not found: " << d.type;
        return 0;
    } else {
        return it->second.clone(d.id);
    }
}

Data<AbstractData>* DataManager::get(const DataDesc& d)
{
    FnMap::iterator it = fn_.find(d.type);
    if (it == fn_.end()) {
        LIB_ERR << "type not found: " << d.type;
        return 0;
    } else
        return it->second.get(d.id);
}

Data<AbstractData>* DataManager::add(AbstractData* id)
{
    if (!id)
        return 0;

    FnMap::iterator it = fn_.find(id->type());
    if (it == fn_.end()) {
        LIB_ERR << "type not found: " << id->type();
        return 0;
    } else
        return it->second.create(id);
}
}
