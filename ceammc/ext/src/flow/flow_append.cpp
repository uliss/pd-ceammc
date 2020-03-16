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
#include "flow_append.h"
#include "ceammc_factory.h"

FlowAppend::FlowAppend(const PdArgs& args)
    : BaseObject(args)
    , delay_time_(0)
    , as_msg_(0)
    , clock_(this, &FlowAppend::tick)
{
    delay_time_ = new FloatProperty("@delay", 0);
    delay_time_->setFloatCheck(PropValueConstraints::GREATER_THEN, 0);
    delay_time_->setUnits(PropValueUnits::MSEC);
    createProperty(delay_time_);

    as_msg_ = new FlagProperty("@msg");
    createProperty(as_msg_);

    msg_ = positionalArguments();

    createOutlet();
}

void FlowAppend::onBang()
{
    bangTo(0);
    process();
}

void FlowAppend::onFloat(t_float f)
{
    floatTo(0, f);
    process();
}

void FlowAppend::onSymbol(t_symbol* s)
{
    symbolTo(0, s);
    process();
}

void FlowAppend::onList(const AtomList& lst)
{
    listTo(0, lst);
    process();
}

void FlowAppend::onAny(t_symbol* s, const AtomList& lst)
{
    anyTo(0, s, lst);
    process();
}

bool FlowAppend::processAnyInlets(t_symbol* sel, const AtomList& lst)
{
    return false;
}

bool FlowAppend::processAnyProps(t_symbol* s, const AtomList& lst)
{
    static t_symbol* SYM_PROP_GET_DELAY = gensym("@delay?");
    static t_symbol* SYM_PROP_DELAY = gensym("@delay");

    // get
    if (s == SYM_PROP_GET_DELAY) {
        anyTo(0, SYM_PROP_DELAY, delay_time_->get());
        return true;
    }

    // set
    if (s == SYM_PROP_DELAY) {
        delay_time_->set(lst);
        return true;
    }

    return false;
}

void FlowAppend::parseProperties()
{
    std::deque<AtomList> p = args().properties();
    for (size_t i = 0; i < p.size(); i++) {
        if (p[i].size() < 1)
            continue;

        t_symbol* pname = p[i][0].asSymbol();

        if (!hasProperty(pname)) {
            msg_.append(p[i]);
            continue;
        }

        // skip readonly properties
        if (property(pname)->isReadOnly())
            continue;

        property(pname)->setInit(p[i].slice(1));
    }
}

void FlowAppend::process()
{
    if (delay_time_->value() >= 0) {
        clock_.delay(delay_time_->value());
        return;
    } else {
        outputAppend();
    }
}

void FlowAppend::tick()
{
    outputAppend();
}

void FlowAppend::outputAppend()
{
    if (as_msg_->value()) {
        anyTo(0, msg_);
    } else {
        listTo(0, msg_);
    }
}

void setup_flow_append()
{
    ObjectFactory<FlowAppend> obj("flow.append");
}
