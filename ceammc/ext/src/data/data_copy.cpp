/*****************************************************************************
 * Copyright 2021 Serge Poltavsky. All rights reserved.
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
#include "data_copy.h"
#include "ceammc_abstractdata.h"
#include "ceammc_factory.h"

DataCopy::DataCopy(const PdArgs& args)
    : BaseObject(args)
{
    createOutlet();
}

void DataCopy::onData(const Atom& datom)
{
    auto dptr = datom.asData();
    if (!dptr) {
        OBJ_ERR << "null data pointer";
        return;
    }

    auto new_data = dptr->clone();
    if (!new_data) {
        OBJ_ERR << "can't copy data";
        return;
    }

    atomTo(0, Atom(new_data));
}

void setup_data_copy()
{
    ObjectFactory<DataCopy> obj("data.copy");
    obj.processData();

    obj.setDescription("make copy for any data types: MList, Set etc...");
    obj.setCategory("data");
    obj.setKeywords({"data", "copy"});
}
