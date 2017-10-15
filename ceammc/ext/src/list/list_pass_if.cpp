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

void ListPassIf::onInlet(size_t n, const AtomList& l)
{
    if (n != 1)
        return;

    pass_flag_ = (!l.empty() && l[0].asInt(0) != 0);
}

extern "C" void setup_list0x2epass_if()
{
    ObjectFactory<ListPassIf>("list.pass_if");
}
