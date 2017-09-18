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
#include "flow_count.h"
#include "ceammc_factory.h"

static t_symbol* PROP_VALUE = gensym("@value");
static t_symbol* PROP_GET_VALUE = gensym("@value?");

FlowCount::FlowCount(const PdArgs& a)
    : BaseObject(a)
    , counter_(0)
{
    counter_ = new SizeTProperty("@value", 0);
    createProperty(counter_);

    // counter flow
    createOutlet();
}

void FlowCount::onBang()
{
    tick();
}

void FlowCount::onFloat(t_float f)
{
    tick();
}

void FlowCount::onSymbol(t_symbol* s)
{
    tick();
}

void FlowCount::onList(const AtomList& l)
{
    tick();
}

void FlowCount::onAny(t_symbol* s, const AtomList& l)
{
    tick();
}

void FlowCount::onData(const DataPtr&)
{
    tick();
}

void FlowCount::m_reset(t_symbol*, const AtomList&)
{
    counter_->setValue(0);
}

bool FlowCount::processAnyInlets(t_symbol*, const AtomList&)
{
    return false;
}

bool FlowCount::processAnyProps(t_symbol* s, const AtomList& l)
{
    // get
    if (s == PROP_GET_VALUE) {
        anyTo(0, PROP_VALUE, counter_->get());
        return true;
    }

    // set
    if (s == PROP_VALUE) {
        counter_->set(l);
        return true;
    }

    return false;
}

void FlowCount::tick()
{
    counter_->setValue(counter_->value() + 1);
    floatTo(0, counter_->value());
}

void setup_flow_count()
{
    ObjectFactory<FlowCount> obj("flow.count");
    obj.addMethod("reset", &FlowCount::m_reset);
}
