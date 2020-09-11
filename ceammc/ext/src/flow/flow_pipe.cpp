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
#include "flow_pipe.h"
#include "ceammc_factory.h"

FlowPipe::FlowPipe(const PdArgs& a)
    : BaseObject(a)
    , delay_(nullptr)
    , inlet_proxy_(this)
{
    createOutlet();

    delay_ = new FloatProperty("@delay", 0);
    delay_->setArgIndex(0);
    delay_->checkMinEq(0);
    addProperty(delay_);

    createCbIntProperty("@size", [this]() -> int { return pipe_.size(); });
    createCbBoolProperty("@empty", [this]() -> bool { return pipe_.empty(); });

    inlet_new(owner(), &inlet_proxy_.x_obj, nullptr, nullptr);
}

void FlowPipe::onBang()
{
    pipe_.emplace_front([this]() { bangTo(0); pipe_.pop_back(); });
    pipe_.front().delay(delay_->value());
}

void FlowPipe::onFloat(t_float f)
{
    pipe_.emplace_front([this, f]() { floatTo(0, f); pipe_.pop_back(); });
    pipe_.front().delay(delay_->value());
}

void FlowPipe::onSymbol(t_symbol* s)
{
    pipe_.emplace_front([this, s]() { symbolTo(0, s); pipe_.pop_back(); });
    pipe_.front().delay(delay_->value());
}

void FlowPipe::onList(const AtomList& l)
{
    pipe_.emplace_front([this, l]() { listTo(0, l); pipe_.pop_back(); });
    pipe_.front().delay(delay_->value());
}

void FlowPipe::onAny(t_symbol* s, const AtomListView& v)
{
    AtomList l(v); // for lambda capture

    pipe_.emplace_front([this, s, l]() { anyTo(0, s, l); pipe_.pop_back(); });
    pipe_.front().delay(delay_->value());
}

void FlowPipe::proxy_bang()
{
    pipe_.clear();
}

void FlowPipe::proxy_float(t_float f)
{
    delay_->setValue(f);
}

void FlowPipe::proxy_reset(const AtomListView& v)
{
    pipe_.clear();
}

void FlowPipe::proxy_flush(const AtomListView& v)
{
    while (!pipe_.empty())
        pipe_.back().exec();
}

void setup_flow_pipe()
{
    ObjectFactory<FlowPipe> obj("flow.pipe");
    obj.noPropsDispatch();

    InletProxy<FlowPipe>::init();
    InletProxy<FlowPipe>::set_bang_callback(&FlowPipe::proxy_bang);
    InletProxy<FlowPipe>::set_float_callback(&FlowPipe::proxy_float);
    InletProxy<FlowPipe>::set_method_callback(gensym("reset"), &FlowPipe::proxy_reset);
    InletProxy<FlowPipe>::set_method_callback(gensym("flush"), &FlowPipe::proxy_flush);
}
