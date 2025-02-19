/*****************************************************************************
 * Copyright 2018 Serge Poltavsky. All rights reserved.
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
#include "data_mlist.h"
#include "ceammc_factory.h"
#include "fmt/core.h"

DataMList::DataMList(const PdArgs& args)
    : DataMListBase(args)
{
    value_ = new MListProperty("@value", {});
    value_->setArgIndex(0);
    addProperty(value_);

    createOutlet();
}

void setup_data_mlist()
{
    ListIFaceFactory<DataMList> obj("data.mlist");
    obj.addAlias("mlist");
    obj.addAlias("ml");
    obj.processData<DataTypeMList>();

    DataMList::factoryEditorObjectInit(obj);

    obj.setDescription("multidimensional list container");
    obj.setCategory("data");
    obj.setKeywords({ "data", "multilist" });

    LIB_LOG << fmt::format("Mlist datatype id: {:d}", DataTypeMList::staticType());
}
