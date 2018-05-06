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
#include "list_index.h"
#include "ceammc_factory.h"

ListIndex::ListIndex(const PdArgs& args)
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

void ListIndex::onFloat(t_float f)
{
    output(lst_.search(Atom(f), start_->value(), end()));
}

void ListIndex::onSymbol(t_symbol* s)
{
    output(lst_.search(Atom(s), start_->value(), end()));
}

void ListIndex::onList(const AtomList& lst)
{
    output(lst_.search(lst, start_->value(), end()));
}

void ListIndex::onData(const DataPtr& ptr)
{
    output(lst_.search(ptr, start_->value(), end()));
}

void ListIndex::onInlet(size_t n, const AtomList& lst)
{
    lst_ = lst;
}

void ListIndex::output(long idx)
{
    floatTo(0, idx);
}

size_t ListIndex::end() const
{
    auto v = end_->value();
    return v < 0 ? DataAtomList::END : v;
}

void setup_list_index()
{
    ObjectFactory<ListIndex> obj("list.index");
    obj.processData();
}
