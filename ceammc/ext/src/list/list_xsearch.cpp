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

ListXSearch::ListXSearch(const PdArgs& args)
    : BaseObject(args)
    , lst_(positionalArguments())
    , start_(nullptr)
    , end_(nullptr)
{
    createInlet();
    createOutlet();

    start_ = new SizeTProperty("@start", 0);
    createProperty(start_);
    end_ = new IntProperty("@end", -1);
    createProperty(end_);
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
    search(lst);
}

void ListXSearch::onData(const DataPtr& ptr)
{
    auto mlist = lst_.asSingle<DataTypeMList>();

    if (mlist) {
        auto input_mlist = ptr.as<DataTypeMList>();

        if (input_mlist)
            output(mlist->data().search(input_mlist->toList()));
        else
            output(mlist->data().search(ptr));
    } else
        output(lst_.search(ptr, start_->value(), end()));
}

void ListXSearch::onInlet(size_t n, const AtomList& lst)
{
    if (lst.isData() && !lst.isDataType<DataTypeMList>()) {
        OBJ_ERR << "invalid datatype. Only data.mlist is supported!";
        return;
    }

    lst_ = lst;
}

void ListXSearch::output(long idx)
{
    floatTo(0, idx);
}

size_t ListXSearch::end() const
{
    auto v = end_->value();
    return v < 0 ? DataAtomList::END : v;
}

void setup_list_xsearch()
{
    ObjectFactory<ListXSearch> obj("list.^search");
    obj.processData();
}
