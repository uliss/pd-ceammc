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
#include "ceammc_convert.h"
#include "ceammc_factory.h"
#include "datatype_mlist.h"

ListSet::ListSet(const PdArgs& args)
    : BaseObject(args)
    , index_(nullptr)
    , value_(nullptr)
{
    index_ = new IntProperty("@index", 0);
    index_->setArgIndex(0);
    addProperty(index_);

    value_ = new AtomProperty("@value", Atom());
    value_->setArgIndex(1);
    addProperty(value_);

    createInlet();
    createInlet();
    createOutlet();
}

void ListSet::onList(const AtomList& lst)
{
    if (value_->value().isNone())
        return;

    auto idx = relativeIndex<long>(index_->value(), lst.size());

    if (idx < 0) {
        OBJ_ERR << "invalid index: " << index_->value();
        return;
    }

    AtomList res(lst);
    res[idx] = value_->value();
    listTo(0, res);
}

void ListSet::onInlet(size_t n, const AtomListView& lst)
{
    if (n == 2) {
        if (lst.empty())
            value_->value() = Atom();
        else
            value_->set(lst);
    } else if (n == 1)
        index_->set(lst);
}

void ListSet::onDataT(const MListAtom& ml)
{
    if (value_->value().isNone())
        return;

    auto idx = relativeIndex<long>(index_->value(), ml->size());

    if (idx < 0) {
        OBJ_ERR << "invalid index: " << index_->value();
        return;
    }

    MListAtom res(*ml);
    res.detachData();
    res->at(idx) = value_->value();
    atomTo(0, res);
}

void setup_list_set()
{
    ObjectFactory<ListSet> obj("list.set");
    obj.processData<DataTypeMList>();
    obj.useDefaultPdFloatFn();
    obj.useDefaultPdSymbolFn();

    obj.setDescription("set list value at specified position");
    obj.addAuthor("Serge Poltavsky");
    obj.setKeywords({ "list", "set" });
    obj.setCategory("list");
    obj.setSinceVersion(0, 6);

    ListSet::setInletsInfo(obj.classPointer(), { "list or Mlist", "int: set index", "atom: set value" });
    ListSet::setOutletsInfo(obj.classPointer(), { "list or Mlist" });
}
