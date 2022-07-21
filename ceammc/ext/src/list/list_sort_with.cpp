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
#include "list_sort_with.h"
#include "ceammc_containers.h"
#include "ceammc_factory.h"
#include "datatype_mlist.h"

#include <algorithm>

ListSortWith::ListSortWith(const PdArgs& a)
    : BaseObject(a)
    , less_(false)
{
    createInlet();
    createOutlet();
    createOutlet();
}

void ListSortWith::onList(const AtomListView& lv)
{
    if (lv.size() < 2) {
        listTo(0, lv);
        return;
    }

    less_ = true;

    AtomList64 res;
    res.insert_back(lv);

    std::sort(res.begin(), res.end(), [this](const Atom& a0, const Atom& a1) {
        StaticAtomList<2> msg { a0, a1 };
        listTo(1, msg.view());
        return !this->less_;
    });

    listTo(0, res.view());
}

void ListSortWith::onInlet(size_t n, const AtomListView& l)
{
    if (n == 1) {
        if (!checkArgs(l, ARG_BOOL))
            return;

        less_ = l[0].asFloat() == 0.f;
    }
}

void ListSortWith::onDataT(const MListAtom& ml)
{
    if (ml->size() < 2) {
        atomTo(0, MListAtom(*ml));
        return;
    }

    less_ = true;

    MListAtom res(*ml);
    std::sort(res->begin(), res->end(),
        [this](const Atom& a0, const Atom& a1) {
            StaticAtomList<2> msg { a0, a1 };
            listTo(1, msg.view());
            return !this->less_;
        });

    atomTo(0, res);
}

void setup_list_sort_with()
{
    ObjectFactory<ListSortWith> obj("list.sort_with");
    obj.processData<DataTypeMList>();
}
