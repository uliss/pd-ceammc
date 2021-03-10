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
#include "list_all_of.h"
#include "ceammc_factory.h"
#include "datatype_mlist.h"

ListAllOf::ListAllOf(const PdArgs& a)
    : ListBase(a)
    , all_(false)
{
    createOutlet();
    createOutlet();

    createInlet();
}

void ListAllOf::onList(const AtomList& l)
{
    if (l.empty())
        return floatTo(0, 1);

    all_ = true;

    for (size_t i = 0; i < l.size(); i++) {
        if (!all_)
            break;

        atomTo(1, l[i]);
    }

    floatTo(0, all_ ? 1 : 0);
}

void ListAllOf::onInlet(size_t n, const AtomListView& l)
{
    if (n != 1 || l.empty())
        return;

    if (!all_)
        return;

    if (l[0].asInt(0) != 1)
        all_ = false;
}

void setup_list_all_of()
{
    ObjectFactory<ListAllOf> obj("list.all_of");
    obj.processData<DataTypeMList>();

    obj.setDescription("checks if all list atoms accepted by predicate");
    obj.addAuthor("Serge Poltavsky");
    obj.setKeywords({ "list", "predicate", "all" });
    obj.setCategory("list");
    obj.setSinceVersion(0, 1);

    ListAllOf::setInletsInfo(obj.classPointer(), { "list or Mlist", "int: 1 or 0 from predicate" });
    ListAllOf::setOutletsInfo(obj.classPointer(), { "int: 1 or 0", "atom: to predicate" });
}
