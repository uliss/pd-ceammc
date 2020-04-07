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
#include "datatype_dict.h"

DictContains::DictContains(const PdArgs& args)
    : BaseObject(args)
    , keys_(nullptr)
{
    keys_ = new ListProperty("@keys");
    keys_->setArgIndex(0);
    addProperty(keys_);

    createInlet();
    createOutlet();
}

void DictContains::onInlet(size_t n, const AtomList& lst)
{
    keys_->set(lst);
}

void DictContains::onDataT(const DictAtom& dict)
{
    if (keys_->value().empty()) {
        OBJ_ERR << "no key specified: " << keys_;
        return;
    }

    listTo(0, keys_->value().map([&dict](const Atom& a) -> Atom {
        return Atom(dict->contains(a) ? 1 : 0);
    }));
}

void setup_dict_contains()
{
    ObjectFactory<DictContains> obj("dict.contains");
    obj.parseOnlyPositionalProps(true);
    obj.processData<DataTypeDict>();
}
