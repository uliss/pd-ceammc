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
#include "ceammc_factory.h"

#include <algorithm>

struct Compare {
    ListSortWith* lsw_;
    Compare(ListSortWith* lsw)
        : lsw_(lsw)
    {
    }

    bool operator()(t_atom a1, t_atom a2)
    {
        lsw_->listTo(1, AtomList(Atom(a1), Atom(a2)));
        return !lsw_->less_;
    }
};

ListSortWith::ListSortWith(const PdArgs& a)
    : BaseObject(a)
    , less_(false)
{
    createInlet();
    createOutlet();
    createOutlet();
}

void ListSortWith::onList(const AtomList& l)
{
    if (l.size() < 2) {
        listTo(0, l);
        return;
    }

    less_ = true;
    AtomList lst = l;
    Compare pred(this);
    std::sort(lst.toPdData(), lst.toPdData() + lst.size(), pred);
    listTo(0, lst);
}

void ListSortWith::onInlet(size_t n, const AtomList& l)
{
    if (n == 1) {
        if (!checkArgs(l, ARG_BOOL))
            return;

        less_ = l[0].asFloat() == 0.f;
    }
}

extern "C" void setup_list0x2esort_with()
{
    ObjectFactory<ListSortWith>("list.sort_with");
}
