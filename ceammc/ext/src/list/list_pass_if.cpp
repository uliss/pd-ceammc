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
#include "list_pass_if.h"
#include "ceammc_factory.h"
#include "datatype_mlist.h"

ListPassIf::ListPassIf(const PdArgs& a)
    : BaseObject(a)
    , pass_flag_(false)
{
    createInlet();
    createOutlet();
    createOutlet();
}

void ListPassIf::onList(const AtomList& lst)
{
    AtomList res;

    for (size_t i = 0; i < lst.size(); i++) {
        pass_flag_ = false;

        atomTo(1, lst[i]);

        if (pass_flag_)
            res.append(lst[i]);
    }

    listTo(0, res);
}

void ListPassIf::onInlet(size_t n, const AtomListView& l)
{
    if (n != 1)
        return;

    pass_flag_ = (!l.empty() && l[0].asInt(0) != 0);
}

void ListPassIf::onDataT(const MListAtom& ml)
{
    MListAtom res;

    for (size_t i = 0; i < ml->size(); i++) {
        pass_flag_ = false;

        atomTo(1, ml->at(i));

        if (pass_flag_)
            res->append(ml->at(i));
    }

    atomTo(0, res);
}

void setup_list_pass_if()
{
    ObjectFactory<ListPassIf> obj("list.pass_if");
    obj.addAlias("list.filter");
    obj.useDefaultPdFloatFn();

    obj.processData<DataTypeMList>();

    obj.setDescription("leave only elements accepted by predicate");
    obj.addAuthor("Serge Poltavsky");
    obj.setKeywords({ "list", "pass", "accept", "leave", "predicate" });
    obj.setCategory("list");
    obj.setSinceVersion(0, 3);

    ListPassIf::setInletsInfo(obj.classPointer(), { "list or Mlist", "int: 1 or 0 from predicate" });
    ListPassIf::setOutletsInfo(obj.classPointer(), { "list or Mlist", "atom: to predicate" });
}
