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
#include "list_set.h"
#include "../data/datatype_mlist.h"
#include "ceammc_convert.h"
#include "ceammc_factory.h"

ListSet::ListSet(const PdArgs& args)
    : BaseObject(args)
    , idx_(positionalFloatArgument(0))
    , value_(positionalArgument(1))
{
    createInlet(&idx_);
    createInlet();
    createOutlet();
}

void ListSet::onList(const AtomList& lst)
{
    if (!value_.isValid()) {
        OBJ_ERR << "empty value";
        return;
    }

    auto idx = relativeIndex<long>(idx_, lst.size());

    if (idx < 0) {
        OBJ_ERR << "invalid index: " << idx_;
        return;
    }

    AtomList res(lst);
    res[idx] = value_.toAtom();
    listTo(0, res);
}

void ListSet::onInlet(size_t n, const AtomList& lst)
{
    if (lst.empty())
        value_ = DataAtom();
    else
        value_ = DataAtom(lst[0]);
}

void ListSet::onDataT(const DataTypeMList& lst)
{
    if (!value_.isValid()) {
        OBJ_ERR << "empty value";
        return;
    }

    auto idx = relativeIndex<long>(idx_, lst.size());

    if (idx < 0) {
        OBJ_ERR << "invalid index: " << idx_;
        return;
    }

    auto res = lst.clone();
    res->at(idx) = value_;
    dataTo(0, DataPtr(res));
}

t_float ListSet::index() const
{
    return idx_;
}

const DataAtom& ListSet::value() const
{
    return value_;
}

void setup_list_set()
{
    ObjectFactory<ListSet> obj("list.set");
    obj.processData<DataTypeMList>();
    obj.mapFloatToList();
    obj.mapSymbolToList();
}
