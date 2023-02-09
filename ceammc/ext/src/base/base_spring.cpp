/*****************************************************************************
 * Copyright 2021 Serge Poltavsky. All rights reserved.
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
#include "base_spring.h"
#include "ceammc_convert.h"
#include "ceammc_factory.h"

#include <cmath>

constexpr t_float EPSILON = 0.00001;
constexpr t_int TAU = 50;

BaseSpring::BaseSpring(const PdArgs& args)
    : BaseObject(args)
    , value_(nullptr)
    , total_time_(nullptr)
    , accel_(nullptr)
    , clock_([this]() {
        const auto v = value_->value();
        const auto delta = v - start_value_;
        if (std::abs(delta) < EPSILON) {
            bangTo(1);
            floatTo(0, v);
            state_ = STATE_INIT;
            return;
        } else {
            const auto ms_left = total_time_->value() - t_;
            if (ms_left > TAU) {
                floatTo(0, valueAtTime(t_));
                t_ += TAU;
                clock_.delay(TAU);
            } else if (ms_left > 0) {
                floatTo(0, valueAtTime(t_));
                t_ += ms_left;
                clock_.delay(ms_left);
            } else {
                bangTo(1);
                floatTo(0, v);
                state_ = STATE_INIT;
            }
        }
    })
    , start_value_(0)
    , state_(STATE_INIT)
{
    createInlet();
    createOutlet();
    createOutlet();

    value_ = new FloatProperty("@value", 0);
    value_->setArgIndex(0);
    addProperty(value_);

    total_time_ = new FloatProperty("@time", 10 * TAU);
    total_time_->setArgIndex(1);
    total_time_->setUnits(PropValueUnits::MSEC);
    total_time_->checkClosedRange(TAU, 100 * TAU);
    addProperty(total_time_);

    createCbIntProperty(
        "@mouse_up", []() { return 0; },
        [this](int v) -> bool {
            handleMouse(v);
            if (v == 1) clock_.exec();
            return true; });

    createCbIntProperty(
        "@mouse_down", []() { return 0; }, [this](int v) { handleMouse(v); return true; });
    createCbIntProperty(
        "@mouse_move", []() { return 0; }, [this](int v) { handleMouse(v); return true; });
    createCbIntProperty(
        "@mouse_drag", []() { return 0; }, [this](int v) { handleMouse(v); return true; });

    accel_ = new FloatProperty("@accel", 0);
    accel_->checkClosedRange(-5, 5);
    addProperty(accel_);
}

void BaseSpring::onFloat(t_float f)
{
    switch (state_) {
    case STATE_INIT:
        t_ = 0;
        start_value_ = f;
        state_ = STATE_RUNNING;
        clock_.unset();
        clock_.exec();
        break;
    case STATE_SET:
        t_ = 0;
        start_value_ = f;
        state_ = STATE_RUNNING;
        clock_.unset();
        break;
    default:
        break;
    }
}

void BaseSpring::onInlet(size_t n, const AtomListView& lv)
{
    value_->set(lv);
}

t_float BaseSpring::valueAtTime(t_float t) const
{
    if (t < 0)
        return start_value_;
    else if (t >= total_time_->value())
        return value_->value();
    else
        return convert::lin2curve(t, 0, total_time_->value(), start_value_, value_->value(), -accel_->value());
}

void BaseSpring::reset()
{
    start_value_ = value_->value();
    t_ = 0;
    state_ = STATE_INIT;
    clock_.unset();
}

void BaseSpring::handleMouse(bool on)
{
    if (on)
        state_ = STATE_SET;
    else
        state_ = STATE_INIT;
}

void setup_base_spring()
{
    ObjectFactory<BaseSpring> obj("spring");
    obj.setXletsInfo({ "float: in value", "float: set target value" }, { "float: out value", "bang: on done" });

    obj.setDescription("float value \"spring\"");
}
