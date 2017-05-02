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

ListAllOf::ListAllOf(const PdArgs& a)
    : BaseObject(a)
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

void ListAllOf::onInlet(size_t n, const AtomList& l)
{
    if (n != 1 || l.empty())
        return;

    if (!all_)
        return;

    if (l.first()->asInt(0) != 1)
        all_ = false;
}

extern "C" void setup_list0x2eall_of()
{
    ObjectFactory<ListAllOf> obj("list.all_of");
}
