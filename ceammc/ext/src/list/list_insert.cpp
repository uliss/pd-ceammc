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
#include "list_insert.h"
#include "ceammc_factory.h"
#include "datatype_mlist.h"

ListInsert::ListInsert(const PdArgs& args)
    : BaseObject(args)
    , lst_(nullptr)
    , index_(nullptr)
{
    index_ = new SizeTProperty("@index", 0);
    addProperty(index_);

    lst_ = new ListProperty("@value");
    lst_->setArgIndex(0);
    addProperty(lst_);

    createInlet();
    createInlet();
    createOutlet();
}

void ListInsert::onList(const AtomList& lst)
{
    AtomList res;
    res.reserve(lst.size() + lst_->value().size());

    if (index_->value() > lst.size()) {
        OBJ_ERR << "index value is too big: " << index_->value();
        return;
    }

    const size_t N = std::min<size_t>(lst.size(), index_->value());

    // insert before
    for (size_t i = 0; i < N; i++)
        res.append(lst[i]);

    // main insert
    for (size_t i = 0; i < lst_->value().size(); i++)
        res.append(lst_->value().at(i));

    // insert after
    for (size_t i = N; i < lst.size(); i++)
        res.append(lst[i]);

    listTo(0, res);
}

void ListInsert::onInlet(size_t n, const AtomList& lst)
{
    switch (n) {
    case 1:
        lst_->set(lst);
        break;
    case 2:
        index_->set(lst);
        break;
    default:
        OBJ_ERR << "invalid inlet: " << n;
        break;
    }
}

void ListInsert::onDataT(const MListAtom& ml)
{
    if (index_->value() > ml->size()) {
        OBJ_ERR << "index value is too big: " << index_->value();
        return;
    }

    const size_t N = std::min<size_t>(ml->size(), index_->value());

    MListAtom res(*ml);
    res->insert(N, lst_->value());
    atomTo(0, res);
}

void setup_list_insert()
{
    ObjectFactory<ListInsert> obj("list.insert");
    obj.processData<DataTypeMList>();

    obj.setDescription("insert atom or list to the specified position of input list");
    obj.addAuthor("Serge Poltavsky");
    obj.setKeywords({ "list", "insert" });
    obj.setCategory("list");
    obj.setSinceVersion(0, 6);

    ListInsert::setInletsInfo(obj.classPointer(), { "list or Mlist", "list: inserted atoms", "int: index position" });
    ListInsert::setOutletsInfo(obj.classPointer(), { "list or Mlist" });
}
