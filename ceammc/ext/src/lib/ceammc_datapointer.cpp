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
#include "ceammc_datapointer.h"
#include "ceammc_log.h"

#include <cstdio>
#include <typeinfo>

using namespace ceammc;

BaseData::~BaseData() {}

std::string BaseData::toString() const
{
    char buf[120];
    sprintf(buf, "[Data %s id: %i]", typeid(*this).name(), type());
    return buf;
}

DataFactory::DataFactory()
{
}

DataFactory& DataFactory::instance()
{
    static DataFactory fact;
    return fact;
}

void DataFactory::add(DataType type, RawDataPointerFn fn)
{
    LIB_DBG << "registered data type: " << type;
    fn_[type] = fn;
}

BaseData* DataFactory::rawData(const DataDesc& d)
{
    DataFnMap::iterator it = fn_.find(d.type);
    if (it == fn_.end()) {
        LIB_ERR << "type not found: " << d.type;
        return 0;
    } else
        return it->second(d.id);
}

BaseData* DataFactory::rawData(DataType type, DataId id)
{
    return rawData(DataDesc(type, id));
}
