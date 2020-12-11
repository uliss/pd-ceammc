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

FlowCount::FlowCount(const PdArgs& a)
    : BaseObject(a)
    , counter_(nullptr)
    , default_value_(0)
{
    counter_ = new IntProperty("@value", default_value_);
    counter_->setIntCheck(PropValueConstraints::GREATER_EQUAL, 0);
    counter_->setArgIndex(0);
    addProperty(counter_);

    createInlet();
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

void FlowCount::onAny(t_symbol* s, const AtomListView& l)
{
    tick();
}

void FlowCount::onData(const Atom&)
{
    tick();
}

void FlowCount::onInlet(size_t, const AtomList&)
{
    counter_->setValue(default_value_);
}

bool FlowCount::processAnyProps(t_symbol* s, const AtomListView& l)
{
    return false;
}

void FlowCount::initDone()
{
    default_value_ = counter_->value();
}

void FlowCount::tick()
{
    counter_->setValue(counter_->value() + 1);
    floatTo(0, counter_->value());
}

void setup_flow_count()
{
    ObjectFactory<FlowCount> obj("flow.count");
    obj.setXletsInfo({ "any: message", "bang: reset counter" }, { "int: number of received messages" });
}
