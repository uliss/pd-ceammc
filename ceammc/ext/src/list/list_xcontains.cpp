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
#include "list_xcontains.h"
#include "../data/datatype_mlist.h"
#include "ceammc_factory.h"
#include "ceammc_fn_list.h"

ListXContains::ListXContains(const PdArgs& args)
    : BaseObject(args)
    , lst_(positionalArguments())
{
    createInlet();
    createOutlet();
}

void ListXContains::onFloat(t_float f)
{
    contains<Atom>(Atom(f));
}

void ListXContains::onSymbol(t_symbol* s)
{
    contains<Atom>(Atom(s));
}

void ListXContains::onList(const AtomList& lst)
{
    contains(lst);
}

void ListXContains::onData(const DataPtr& ptr)
{
    auto mlist = lst_.asSingle<DataTypeMList>();

    if (mlist) {
        auto input_mlist = ptr.as<DataTypeMList>();

        if (input_mlist)
            output(mlist->contains(*input_mlist));
        else
            output(mlist->contains(ptr));
    } else {
        output(lst_.contains(ptr));
    }
}

void ListXContains::onInlet(size_t n, const AtomList& lst)
{
    if (lst.isData() && !lst.isDataType<DataTypeMList>()) {
        OBJ_ERR << "invalid datatype. Only data.mlist is supported!";
        return;
    }

    lst_ = lst;
}

void ListXContains::output(bool v)
{
    floatTo(0, v ? 1 : 0);
}

void setup_list_xcontains()
{
    ObjectFactory<ListXContains> obj("list.^contains");
    obj.processData();
}
