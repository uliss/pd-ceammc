/*****************************************************************************
 * Copyright 2020 Serge Poltavsky. All rights reserved.
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
#include "flow_dup.h"
#include "ceammc_factory.h"

FlowDup::FlowDup(const PdArgs& a)
    : BaseObject(a)
    , times_(nullptr)
{
    times_ = new IntProperty("@n", 2);
    times_->checkMinEq(0);
    times_->setArgIndex(0);
    addProperty(times_);

    createInlet();
    createOutlet();
}

void FlowDup::onBang()
{
    for (int i = times_->value(); i > 0; i--)
        bangTo(0);
}

void FlowDup::onFloat(t_float f)
{
    for (int i = times_->value(); i > 0; i--)
        floatTo(0, f);
}

void FlowDup::onSymbol(t_symbol* s)
{
    for (int i = times_->value(); i > 0; i--)
        symbolTo(0, s);
}

void FlowDup::onList(const AtomListView& lv)
{
    for (int i = times_->value(); i > 0; i--)
        listTo(0, lv);
}

void FlowDup::onAny(t_symbol* s, const AtomListView& lv)
{
    for (int i = times_->value(); i > 0; i--)
        anyTo(0, s, lv);
}

void FlowDup::onInlet(size_t n, const AtomListView& l)
{
    times_->set(l);
}

void setup_flow_dup()
{
    ObjectFactory<FlowDup> obj("flow.dup");
    obj.noPropsDispatch();

    obj.setXletsInfo({ "input messages", "int: number of repeats for each message" }, { "output messages" });
}
