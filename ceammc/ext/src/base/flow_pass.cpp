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
    , values_(0)
{
    createOutlet();

    values_ = new ListProperty("@values", a.args);
    createProperty(values_);
}

void FlowPass::onBang()
{
    bangTo(0);
}

void FlowPass::onFloat(float v)
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
    listTo(0, l);
}

void FlowPass::onAny(t_symbol* s, const AtomList& lst)
{
    if (values_->value().contains(s))
        anyTo(0, s, lst);
}

void FlowPass::parseProperties()
{
}

bool FlowPass::processAnyProps(t_symbol* sel, const AtomList& lst)
{
    static t_symbol* s_prop_values = gensym("@values");
    static t_symbol* s_get_values = gensym("@values?");

    if (sel == s_get_values) {
        anyTo(0, s_prop_values, values_->value());
        return true;
    }

    if (sel == s_prop_values) {
        values_->set(lst);
        return true;
    }

    return false;
}

extern "C" void setup_flow0x2epass()
{
    ObjectFactory<FlowPass> obj("flow.pass");
    obj.addAlias("pass");
}
