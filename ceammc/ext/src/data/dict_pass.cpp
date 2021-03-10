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
    : DictBase(args)
    , keys_(nullptr)
{
    keys_ = new ListProperty("@keys");
    keys_->setArgIndex(0);
    addProperty(keys_);

    createInlet();
    createOutlet();
}

void DictPass::onInlet(size_t, const AtomListView& lst)
{
    keys_->set(lst);
}

void DictPass::onDataT(const DictAtom& dict)
{
    DictAtom res = dict;
    res.detachData();

    res->removeIf([this](const Atom& k) -> bool { return !keys_->value().contains(k); });

    atomTo(0, res);
}

void setup_dict_pass()
{
    ObjectFactory<DictPass> obj("dict.pass");
    obj.processData<DataTypeDict>();

    obj.setDescription("passes specified keys in dict");
    obj.addAuthor("Serge Poltavsky");
    obj.setKeywords({ "pass", "dictionary" });
    obj.setCategory("data");
    obj.setSinceVersion(0, 7);

    DictPass::setInletsInfo(obj.classPointer(), { "Dict", "list: set passed keys" });
    DictPass::setOutletsInfo(obj.classPointer(), { "Dict" });
}
