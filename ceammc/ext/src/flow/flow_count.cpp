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
#include "ceammc_crc32.h"
#include "ceammc_factory.h"

FlowCount::FlowCount(const PdArgs& a)
    : BaseObject(a)
    , counter_(nullptr)
    , default_value_(0)
    , proxy_(this)
{
    counter_ = new IntProperty("@value", default_value_);
    counter_->setIntCheck(PropValueConstraints::GREATER_EQUAL, 0);
    counter_->setArgIndex(0);
    addProperty(counter_);

    inlet_new(owner(), &proxy_.x_obj, nullptr, nullptr);

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

void FlowCount::onList(const AtomListView& lv)
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

void FlowCount::onInletAny(int id, t_symbol* s, const AtomListView& lv)
{
    switch (crc32_hash(s)) {
    case "bang"_hash:
        counter_->setValue(default_value_);
        break;
    case "float"_hash:
        counter_->setValue(lv.floatAt(0, 0));
        break;
    case "@value"_hash:
        counter_->set(lv);
        break;
    case "@value?"_hash:
        floatTo(0, counter_->value());
        break;
    case "-"_hash:
        counter_->setValue(counter_->value() - lv.floatAt(0, 0));
        break;
    case "+"_hash:
        counter_->setValue(counter_->value() + lv.floatAt(0, 0));
        break;
    default:
        OBJ_ERR << "bang, int, +, - or @value message expected, got: " << s->s_name << ' ' << lv;
        break;
    }
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
    obj.setXletsInfo({ "any: message", "bang: reset counter\n"
                                       "int: set current value\n"
                                       "@value X: set current value\n"
                                       "@value?: output current value\n"
                                       "+ X: add value\n"
                                       "- X: substract value" },
        { "int: number of received messages" });

    InletProxy<FlowCount>::init();
    InletProxy<FlowCount>::set_any_callback(&FlowCount::onInletAny);
}
