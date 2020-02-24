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

void ListRemoveIf::onInlet(size_t n, const AtomList& l)
{
    if (l.empty())
        return;

    remove_ = l[0].asFloat() != 0;
}

void ListRemoveIf::onDataT(const DataTPtr<DataTypeMList>& dptr)
{
    remove_ = true;
    DataTypeMList res;

    for (size_t i = 0; i < dptr->size(); i++) {
        remove_ = true;

        atomTo(1, dptr->at(i).asAtom());
        if (!remove_)
            res.append(dptr->at(i));
    }

    dataTo(0, DataTPtr<DataTypeMList>(res));
}

void setup_list_remove_if()
{
    ObjectFactory<ListRemoveIf> obj("list.remove_if");
    obj.processData<DataTypeMList>();
}
