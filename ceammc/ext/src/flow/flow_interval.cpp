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
#include "flow_interval.h"
#include "ceammc_factory.h"

FlowInterval::FlowInterval(const PdArgs& args)
    : BaseObject(args)
    , last_time_(0)
{
    createOutlet();
}

bool FlowInterval::processAnyProps(t_symbol* s, const AtomListView&)
{
    return false;
}

void FlowInterval::parseProperties()
{
}

void FlowInterval::onBang()
{
    auto ms = clock_gettimesince(last_time_);
    last_time_ = clock_getsystime();
    floatTo(0, ms);
}

void FlowInterval::onFloat(t_float f)
{
    onBang();
}

void FlowInterval::onSymbol(t_symbol* s)
{
    onBang();
}

void FlowInterval::onList(const AtomListView& lv)
{
    onBang();
}

void FlowInterval::onAny(t_symbol* s, const AtomListView& lst)
{
    onBang();
}

void setup_flow_interval()
{
    ObjectFactory<FlowInterval> obj("flow.interval");
    obj.setXletsInfo({ "input messages" }, { "float: time(ms) between current and previous message" });
}
