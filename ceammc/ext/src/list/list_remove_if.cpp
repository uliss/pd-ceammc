/*****************************************************************************
 * Copyright 2017 Serge Poltavsky. All rights reserved.
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
#include "list_remove_if.h"
#include "ceammc_factory.h"
#include "datatype_mlist.h"

ListRemoveIf::ListRemoveIf(const PdArgs& a)
    : BaseObject(a)
    , remove_(false)
{
    createInlet();
    createOutlet();
    createOutlet();
}

void ListRemoveIf::onList(const AtomList& l)
{
    remove_ = true;
    AtomList res;

    for (size_t i = 0; i < l.size(); i++) {
        remove_ = true;

        atomTo(1, l[i]);
        if (!remove_)
            res.append(l[i]);
    }

    listTo(0, res);
}

void ListRemoveIf::onInlet(size_t n, const AtomListView& l)
{
    if (l.empty())
        return;

    remove_ = l[0].asFloat() != 0;
}

void ListRemoveIf::onDataT(const MListAtom& ml)
{
    remove_ = true;
    MListAtom res;

    for (size_t i = 0; i < ml->size(); i++) {
        remove_ = true;

        atomTo(1, ml->at(i));
        if (!remove_)
            res->append(ml->at(i));
    }

    atomTo(0, res);
}

void setup_list_remove_if()
{
    ObjectFactory<ListRemoveIf> obj("list.remove_if");
    obj.processData<DataTypeMList>();

    obj.setDescription("remove elements by predicate");
    obj.addAuthor("Serge Poltavsky");
    obj.setKeywords({ "list", "remove", "predicate" });
    obj.setCategory("list");
    obj.setSinceVersion(0, 1);

    ListRemoveIf::setInletsInfo(obj.classPointer(), { "list or Mlist", "int: 1 or 0 from predicate" });
    ListRemoveIf::setOutletsInfo(obj.classPointer(), { "list or Mlist", "atom: to predicate" });
}
