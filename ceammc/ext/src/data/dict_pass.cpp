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
#include "dict_pass.h"
#include "ceammc_factory.h"

DictPass::DictPass(const PdArgs& args)
    : BaseObject(args)
    , keys_(args.args)
{
    createInlet();
    createOutlet();
}

void DictPass::parseProperties()
{
    // skip arguments property parsing
}

void DictPass::onInlet(size_t, const AtomList& lst)
{
    keys_ = lst;
}

void DictPass::onDataT(const DataTPtr<DataTypeDict>& dptr)
{
    DataTypeDict res(*dptr);

    const auto& dict = dptr->innerData();
    for (auto& kv : dict) {
        if (!keys_.contains(kv.first))
            res.remove(kv.first);
    }

    dataTo(0, DataTPtr<DataTypeDict>(res));
}

void setup_dict_pass()
{
    ObjectFactory<DictPass> obj("dict.pass");
    obj.processData<DataTypeDict>();
}
