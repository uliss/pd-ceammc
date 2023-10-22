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
#include "ceammc_containers.h"

FlowPipe::FlowPipe(const PdArgs& a)
    : BaseObject(a)
    , delay_(nullptr)
    , inlet_proxy_(this)
    , cleanup_([this]() {
        pipe_.remove_if([](ClockLambdaFunction& c) { return !c.isActive(); });
    })
{
    createOutlet();

    delay_ = new FloatProperty("@delay", 0);
    delay_->setArgIndex(0);
    delay_->checkMinEq(0);
    delay_->setUnits(PropValueUnits::MSEC);
    addProperty(delay_);

    createCbIntProperty("@size", [this]() -> int { return pipe_.size(); });
    createCbBoolProperty("@empty", [this]() -> bool { return pipe_.empty(); });

    inlet_new(owner(), &inlet_proxy_.x_obj, nullptr, nullptr);
}

void FlowPipe::onBang()
{
    pipe_.emplace_front([this]() { bangTo(0); pop(); });
    pipe_.front().delay(delay_->value());
}

void FlowPipe::onFloat(t_float f)
{
    pipe_.emplace_front([this, f]() { floatTo(0, f); pop(); });
    pipe_.front().delay(delay_->value());
}

void FlowPipe::onSymbol(t_symbol* s)
{
    pipe_.emplace_front([this, s]() { symbolTo(0, s); pop(); });
    pipe_.front().delay(delay_->value());
}

void FlowPipe::onList(const AtomListView& lv)
{
    // make a copy for lambda capture
    AtomList16 l;
    l.insert_back(lv);

    pipe_.emplace_front([this, l]() { listTo(0, l.view()); pop(); });
    pipe_.front().delay(delay_->value());
}

void FlowPipe::onAny(t_symbol* s, const AtomListView& lv)
{
    // make a copy for lambda capture
    AtomList16 l;
    l.insert_back(lv);

    pipe_.emplace_front([this, s, l]() { anyTo(0, s, l.view()); pop(); });
    pipe_.front().delay(delay_->value());
}

void FlowPipe::pop()
{
    cleanup_.delay(0);
}

void FlowPipe::proxy_bang(int)
{
    pipe_.clear();
}

void FlowPipe::proxy_float(int, t_float f)
{
    delay_->setValue(f);
}

void FlowPipe::proxy_reset(const AtomListView& v)
{
    pipe_.clear();
}

void FlowPipe::proxy_flush(const AtomListView& v)
{
    for (auto it = pipe_.rbegin(); it != pipe_.rend(); ++it) {
        if (it->isActive())
            it->exec();
    }
}

void setup_flow_pipe()
{
    ObjectFactory<FlowPipe> obj("flow.pipe");
    obj.noPropsDispatch();

    obj.setXletsInfo({ "any", "bang: clear pipe\n"
                              "float: set delay time\n"
                              "reset: clear pipe\n"
                              "flush: output all messages and clear" },
        { "any" });

    InletProxy<FlowPipe>::init();
    InletProxy<FlowPipe>::set_bang_callback(&FlowPipe::proxy_bang);
    InletProxy<FlowPipe>::set_float_callback(&FlowPipe::proxy_float);
    InletProxy<FlowPipe>::set_method_callback(gensym("reset"), &FlowPipe::proxy_reset);
    InletProxy<FlowPipe>::set_method_callback(gensym("flush"), &FlowPipe::proxy_flush);

    obj.setDescription("enhanced version on vanilla pipe object");
    obj.setCategory("flow");
    obj.setKeywords({"flow", "pipe"});
}
