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
#include "dict_contains.h"
#include "ceammc_factory.h"

DictContains::DictContains(const PdArgs& args)
    : BaseObject(args)
{
    if (args.args.size() > 0)
        key_ = args.args[0];

    createInlet();
    createOutlet();
}

void DictContains::parseProperties()
{
    return;
}

void DictContains::onInlet(size_t n, const AtomList& lst)
{
    if (lst.empty()) {
        OBJ_ERR << "empty key: " << lst;
        return;
    }

    key_ = lst[0];
}

void DictContains::onDataT(const DataTypeDict& d)
{
    if (key_.isNone()) {
        OBJ_ERR << "no key specified: " << key_;
        return;
    }

    floatTo(0, d.contains(key_));
}

void setup_dict_contains()
{
    ObjectFactory<DictContains> obj("dict.contains");
    obj.processData<DataTypeDict>();
}
