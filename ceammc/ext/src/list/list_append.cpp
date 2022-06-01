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
#include "list_append.h"
#include "ceammc_factory.h"
#include "datatype_mlist.h"

ListAppend::ListAppend(const PdArgs& args)
    : BaseObject(args)
    , lst_(nullptr)
{
    lst_ = new ListProperty("@value");
    lst_->setArgIndex(0);
    addProperty(lst_);

    createInlet();
    createOutlet();
}

void ListAppend::onBang()
{
    listTo(0, lst_->value());
}

void ListAppend::onList(const AtomList& lst)
{
    listTo(0, lst + lst_->value());
}

void ListAppend::onDataT(const MListAtom& d)
{
    MListAtom ml = d;
    ml.detachData();
    ml->append(lst_->value());
    atomTo(0, ml);
}

void ListAppend::onData(const Atom& d)
{
    onList(d);
}

void ListAppend::onInlet(size_t n, const AtomListView& lv)
{
    lst_->set(lv);
}

void setup_list_append()
{
    ObjectFactory<ListAppend> obj("list.append");
    obj.processData<DataTypeMList>();
    obj.useDefaultPdFloatFn();
    obj.useDefaultPdSymbolFn();

    obj.setXletsInfo({ "list: input list", "atom or list: set appended value" }, { "list: output" });
}
