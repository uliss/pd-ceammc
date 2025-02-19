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
#include "data_set.h"
#include "ceammc_factory.h"
#include "fmt/core.h"

DataSet::DataSet(const PdArgs& a)
    : DataSetBase(a)
{
    if (set_.looksLikeCtor(a.args))
        set_.setFromDataList(a.args);
    else
        set_ = DataTypeSet(a.args);

    createOutlet();
}

void setup_data_set()
{
    ColectionIFaceFactory<DataSet> obj("data.set");
    obj.processData<DataTypeSet>();

    DataSet::factoryEditorObjectInit(obj);

    obj.setDescription("container that store unique elements");
    obj.setCategory("data");
    obj.setKeywords({ "data", "set" });

    LIB_LOG << fmt::format("Set datatype id: {:d}", DataTypeSet::staticType());
}
