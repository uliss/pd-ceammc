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
#include "list_prepend.h"
#include "ceammc_containers.h"
#include "ceammc_factory.h"
#include "datatype_mlist.h"

ListPrepend::ListPrepend(const PdArgs& args)
    : BaseObject(args)
    , lst_(nullptr)
{
    lst_ = new ListProperty("@value");
    lst_->setArgIndex(0);
    addProperty(lst_);
    createInlet();
    createOutlet();
}

void ListPrepend::onBang()
{
    // bang processed as empty list
    listTo(0, lst_->value());
}

void ListPrepend::onFloat(t_float f)
{
    AtomList32 res;
    res.reserve(lst_->value().size() + 1);

    for (auto& a : lst_->value())
        res.push_back(a);

    res.push_back(f);
    listTo(0, res.view());
}

void ListPrepend::onSymbol(t_symbol* s)
{
    SmallAtomList res;
    res.reserve(lst_->value().size() + 1);

    for (auto& a : lst_->value())
        res.push_back(a);

    res.push_back(s);
    listTo(0, res.view());
}

void ListPrepend::onList(const AtomListView& lv)
{
    AtomList32 res;

    res.reserve(lst_->value().size() + lv.size());
    for (auto& a : lst_->value())
        res.push_back(a);

    for (auto& a : lv)
        res.push_back(a);

    listTo(0, res.view());
}

void ListPrepend::onData(const Atom& d)
{
    onList(d);
}

void ListPrepend::onDataT(const MListAtom& ml)
{
    MListAtom res(ml);
    res.detachData();
    res->prepend(lst_->value());
    atomTo(0, res);
}

void ListPrepend::onInlet(size_t n, const AtomListView& lst)
{
    lst_->setValue(lst);
}

void setup_list_prepend()
{
    ObjectFactory<ListPrepend> obj("list.prepend");
    obj.processData<DataTypeMList>();

    obj.setDescription("prepend atom or list before input list");
    obj.setCategory("list");
    obj.setKeywords({"list", "prepend"});
}
