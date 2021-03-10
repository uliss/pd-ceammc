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
#include "list_none_of.h"
#include "ceammc_factory.h"
#include "datatype_mlist.h"

ListNoneOf::ListNoneOf(const PdArgs& a)
    : ListBase(a)
    , none_(false)
{
    createInlet();
    createOutlet();
    createOutlet();
}

void ListNoneOf::onList(const AtomList& l)
{
    if (l.empty())
        return floatTo(0, 1);

    none_ = true;

    for (size_t i = 0; i < l.size(); i++) {
        if (!none_)
            break;

        atomTo(1, l[i]);
    }

    floatTo(0, none_ ? 1 : 0);
}

void ListNoneOf::onInlet(size_t n, const AtomListView& l)
{
    if (n != 1 || l.empty())
        return;

    if (l[0].asInt(0) != 0)
        none_ = false;
}

void setup_list_none_of()
{
    ObjectFactory<ListNoneOf> obj("list.none_of");
    obj.processData<DataTypeMList>();

    obj.setDescription("checks if none of list atoms is accepted by predicate");
    obj.addAuthor("Serge Poltavsky");
    obj.setKeywords({"list", "predicate", "none"});
    obj.setCategory("list");
    obj.setSinceVersion(0, 1);

    ListNoneOf::setInletsInfo(obj.classPointer(), { "list or Mlist", "int: 1 or 0 from predicate" });
    ListNoneOf::setOutletsInfo(obj.classPointer(), { "int: 1 or 0", "atom: to predicate" });
}
