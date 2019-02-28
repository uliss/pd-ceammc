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
#include "dict_reject.h"
#include "ceammc_factory.h"

DictReject::DictReject(const PdArgs& args)
    : BaseObject(args)
    , keys_(args.args)
{
    createInlet();
    createOutlet();
}

void DictReject::parseProperties()
{
}

void DictReject::onInlet(size_t, const AtomList& lst)
{
    keys_ = lst;
}

void DictReject::onDataT(const DataTypeDict& d)
{
    DataTypeDict* res = new DataTypeDict(d);

    for (auto& a : keys_)
        res->remove(a);

    dataTo(0, DataPtr(res));
}

void setup_dict_reject()
{
    ObjectFactory<DictReject> obj("dict.reject");
    obj.processData<DataTypeDict>();
}
