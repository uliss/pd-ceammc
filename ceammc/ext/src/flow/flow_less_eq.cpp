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
#include "flow_less_eq.h"
#include "ceammc_factory.h"

FlowLessEq::FlowLessEq(const PdArgs& a)
    : FlowCompareBase(a, &FlowLessEq::cmp, { "<=", ">" })
{
}

bool FlowLessEq::cmp(t_float f0, t_float f1)
{
    return f0 <= f1;
}

void setup_flow_less_eq()
{
    ObjectFactory<FlowLessEq> obj("flow.less_eq");
    obj.addAlias("flow.<=");
}
