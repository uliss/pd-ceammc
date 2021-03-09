/*****************************************************************************
 * Copyright 2018 Serge Poltavsky. All rights reserved.
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
#include "conv_list2props.h"
#include "ceammc_factory.h"

ConvList2Props::ConvList2Props(const PdArgs& args)
    : BaseObject(args)
{
    createOutlet();
    createOutlet();
}

void ConvList2Props::onList(const AtomList& lst)
{
    AtomList non_props;
    for (auto& a : lst) {
        if (a.isProperty())
            break;

        non_props.append(a);
    }

    listTo(1, non_props);

    auto props = lst.properties();
    for (auto& l : props)
        anyTo(0, l);
}

void ConvList2Props::onAny(t_symbol* s, const AtomListView& lst)
{
    AtomList lprop(s);
    lprop.append(lst);
    onList(lprop);
}

bool ConvList2Props::processAnyProps(t_symbol* sel, const AtomListView& lst)
{
    return false;
}

void setup_conv_list2props()
{
    ObjectFactory<ConvList2Props> obj("conv.list2props");
    obj.addAlias("list->props");
    obj.addAlias("any->props");
}
