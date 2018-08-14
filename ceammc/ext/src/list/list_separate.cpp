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
#include "list_separate.h"
#include "../data/datatype_mlist.h"
#include "ceammc_factory.h"

ListSeparate::ListSeparate(const PdArgs& a)
    : BaseObject(a)
{
    createOutlet();
    createOutlet();
}

void ListSeparate::onList(const AtomList& l)
{
    for (auto& el : l)
        atomTo(0, el);

    bangTo(1);
}

void ListSeparate::onDataT(const DataTypeMList& l)
{
    for (auto& el : l)
        atomTo(0, el.toAtom());

    bangTo(1);
}

void setup_list_separate()
{
    ObjectFactory<ListSeparate> obj("list.separate");
    obj.addAlias("list.sep");
    obj.processData<DataTypeMList>();
}
