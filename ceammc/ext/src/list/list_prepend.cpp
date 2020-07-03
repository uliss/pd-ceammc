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
#include "ceammc_factory.h"
#include "datatype_mlist.h"

ListPrepend::ListPrepend(const PdArgs& args)
    : BaseObject(args)
    , lst_(args.args)
{
    createInlet();
    createOutlet();
}

void ListPrepend::parseProperties()
{
    // empty
}

void ListPrepend::onBang()
{
    // bang processed as empty list
    listTo(0, lst_);
}

void ListPrepend::onFloat(t_float f)
{
    onList(AtomList(f));
}

void ListPrepend::onSymbol(t_symbol* s)
{
    onList(AtomList(s));
}

void ListPrepend::onList(const AtomList& lst)
{
    listTo(0, lst_ + lst);
}

void ListPrepend::onData(const Atom& d)
{
    onList(d);
}

void ListPrepend::onDataT(const MListAtom& ml)
{
    MListAtom res(ml);
    res.detachData();
    res->prepend(lst_);
    atomTo(0, res);
}

void ListPrepend::onInlet(size_t n, const AtomList& lst)
{
    lst_ = lst;
}

void setup_list_prepend()
{
    ObjectFactory<ListPrepend> obj("list.prepend");
    obj.processData<DataTypeMList>();
}
