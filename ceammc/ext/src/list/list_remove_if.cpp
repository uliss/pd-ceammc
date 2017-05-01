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
    remove_ = false;
    passed_values_.clear();

    for (size_t i = 0; i < l.size(); i++) {
        atomTo(1, l[i]);
        if (!remove_)
            passed_values_.append(l[i]);
    }

    listTo(0, passed_values_);
}

void ListRemoveIf::onInlet(size_t n, const AtomList& l)
{
    if (n != 1 || l.empty())
        return;

    remove_ = (l[0].asInt(0) == 1);
}

extern "C" void setup_list0x2eremove_if()
{
    ObjectFactory<ListRemoveIf>("list.remove_if");
}
