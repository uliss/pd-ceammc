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
#include "dict_values.h"
#include "ceammc_factory.h"
#include "datatype_mlist.h"

DictValues::DictValues(const PdArgs& args)
    : DictBase(args)
{
    createOutlet();
}

void DictValues::onDataT(const DictAtom& dptr)
{
    AtomList res;

    for (auto& kv : *dptr)
        res.append(MListAtom(kv.second));

    listTo(0, res);
}

void setup_dict_values()
{
    ObjectFactory<DictValues> obj("dict.values");
    obj.processData<DataTypeDict>();

    obj.setDescription("dictionary values as mlist");
    obj.setCategory("data");
    obj.setKeywords({"size", "dictionary"});
}
