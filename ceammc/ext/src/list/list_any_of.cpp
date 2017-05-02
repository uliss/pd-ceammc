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
#include "list_any_of.h"
#include "ceammc_factory.h"

ListAnyOf::ListAnyOf(const PdArgs& a)
    : BaseObject(a)
    , any_(false)
{
    createOutlet();
    createOutlet();

    createInlet();
}

void ListAnyOf::onList(const AtomList& l)
{
    if (l.empty())
        return floatTo(0, 0);

    any_ = false;

    for (size_t i = 0; i < l.size(); i++) {
        if (any_)
            break;

        atomTo(1, l[i]);
    }

    floatTo(0, any_ ? 1 : 0);
}

void ListAnyOf::onInlet(size_t n, const AtomList& l)
{
    if (n != 1 || l.empty())
        return;

    if (any_)
        return;

    if (l.first()->asInt(0) == 1)
        any_ = true;
}

extern "C" void setup_list0x2eany_of()
{
    ObjectFactory<ListAnyOf> obj("list.any_of");
}
