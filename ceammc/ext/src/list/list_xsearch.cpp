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
#include "list_xsearch.h"
#include "ceammc_factory.h"
#include "ceammc_fn_list.h"
#include "datatype_mlist.h"
#include "fmt/format.h"

ListXSearch::ListXSearch(const PdArgs& args)
    : BaseObject(args)
    , lst_(positionalArguments())
    , start_(nullptr)
    , end_(nullptr)
{
    createInlet();
    createOutlet();

    start_ = new SizeTProperty("@start", 0);
    addProperty(start_);
    end_ = new IntProperty("@end", -1);
    addProperty(end_);
}

void ListXSearch::onFloat(t_float f)
{
    search(Atom(f));
}

void ListXSearch::onSymbol(t_symbol* s)
{
    search(Atom(s));
}

void ListXSearch::onList(const AtomList& lst)
{
    if (lst.empty())
        return;

    search(lst[0]);
}

void ListXSearch::onData(const Atom& a)
{
    search(a);
}

void ListXSearch::onInlet(size_t n, const AtomList& lst)
{
    if (lst.isData() && !lst.isA<DataTypeMList>()) {
        OBJ_ERR << fmt::format("invalid datatype {}, only data.mlist is supported", lst.first()->asData()->typeName());
        return;
    }

    lst_ = lst;
}

void ListXSearch::search(const Atom& a)
{
    if (lst_.isA<DataTypeMList>()) {
        auto mlist = lst_.asD<DataTypeMList>();
        floatTo(0, mlist->data().findPos(a));
    } else
        floatTo(0, lst_.findPos(a));
}

void setup_list_xsearch()
{
    ObjectFactory<ListXSearch> obj("list.^search");
    obj.processData();
}
