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
#include "flow_pass.h"
#include "ceammc_factory.h"

FlowPass::FlowPass(const PdArgs& a)
    : BaseObject(a)
    , values_(nullptr)
{
    createOutlet();

    values_ = new ListProperty("@values", a.args);
    values_->setArgIndex(0);
    addProperty(values_);
}

void FlowPass::onBang()
{
    bangTo(0);
}

void FlowPass::onFloat(t_float v)
{
    if (values_->value().contains(v))
        floatTo(0, v);
}

void FlowPass::onSymbol(t_symbol* s)
{
    if (values_->value().contains(s))
        symbolTo(0, s);
}

void FlowPass::onList(const AtomList& l)
{
    if (l.empty())
        return bangTo(0);

    if (values_->value().contains(l[0]))
        listTo(0, l);
}

void FlowPass::onAny(t_symbol* s, const AtomListView& lst)
{
    if (values_->value().contains(s))
        anyTo(0, s, lst);
}

void setup_flow_pass()
{
    ObjectFactory<FlowPass> obj("flow.pass");
    obj.addAlias("pass");
}
