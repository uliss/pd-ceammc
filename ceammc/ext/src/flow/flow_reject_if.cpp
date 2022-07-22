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
#include "flow_reject_if.h"
#include "ceammc_factory.h"

FlowRejectIf::FlowRejectIf(const PdArgs& a)
    : BaseObject(a)
    , reject_(0)
{
    createInlet();
    createOutlet();
    createOutlet();
}

void FlowRejectIf::onBang()
{
    reject_ = 1;
    bangTo(1);

    if (!reject_)
        bangTo(0);
}

void FlowRejectIf::onFloat(t_float f)
{
    reject_ = 1;
    floatTo(1, f);

    if (reject_ == 0)
        floatTo(0, f);
}

void FlowRejectIf::onSymbol(t_symbol* s)
{
    reject_ = 1;
    symbolTo(1, s);

    if (reject_ == 0)
        symbolTo(0, s);
}

void FlowRejectIf::onList(const AtomListView& lv)
{
    reject_ = 1;
    listTo(1, lv);

    if (reject_ == 0)
        listTo(0, lv);
}

void FlowRejectIf::onAny(t_symbol* s, const AtomListView& lv)
{
    reject_ = 1;
    anyTo(1, s, lv);

    if (reject_ == 0)
        anyTo(0, s, lv);
}

void FlowRejectIf::onInlet(size_t n, const AtomListView& lv)
{
    if (n != 1 || lv.empty())
        return;

    reject_ = (lv.toT<size_t>(0) == 0) ? 0 : 1;
}

void setup_flow_reject_if()
{
    ObjectFactory<FlowRejectIf> obj("flow.reject_if");
    obj.addAlias("reject_if");
    obj.noPropsDispatch();
}
