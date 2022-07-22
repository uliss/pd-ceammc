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
#include "flow_space.h"
#include "ceammc_containers.h"
#include "ceammc_factory.h"

#include <chrono>
#include <random>

FlowSpace::FlowSpace(const PdArgs& a)
    : BaseObject(a)
    , packet_sched_([this]() { packetSchedule(); })
    , delay_(nullptr)
    , deviation_(nullptr)
    , done_(nullptr)
    , done_fn_([this]() {
        bangTo(1);
    })
    , inlet_(this)
{
    createOutlet();
    createOutlet();

    inlet_new(owner(), &inlet_.x_obj, nullptr, nullptr);

    delay_ = new FloatProperty("@delay", 1);
    delay_->setArgIndex(0);
    delay_->checkMinEq(1);
    delay_->setUnits(PropValueUnits::MSEC);
    addProperty(delay_);

    deviation_ = new FloatProperty("@dev", 0);
    deviation_->checkClosedRange(0, 1);
    addProperty(deviation_);

    done_ = new IntProperty("@done", 1);
    done_->checkNonNegative();
    done_->setArgIndex(1);
    addProperty(done_);
}

void FlowSpace::packetSchedule()
{
    // finalize scheduled clocks
    // as we add events from back to beginning
    // we need to process them from begining to end
    for (auto it = pipe_.begin(); it != pipe_.end(); ++it) {
        if (it->new_one) {
            if (packet_count_-- > 0) {
                num_active_++;

                it->new_one = false;

                if (packet_count_ != 0) {
                    t_float t = packet_count_ * delay_->value();

                    if (deviation_->value() > 0) {
                        std::mt19937 dev(std::time(nullptr));
                        const t_float b = deviation_->value() * 0.49;
                        const t_float a = -b;
                        std::uniform_real_distribution<t_float> dist(a, b);
                        t += (dist(dev) * delay_->value());
                    }

                    it->clock.delay(t);
                } else // exec immidiately
                    it->clock.exec();

            } else
                break;
        }
    }

    packet_count_ = 0;
}

void FlowSpace::clockDone()
{
    if (--num_active_ == 0) {
        const auto n = done_->value();
        if (n > 0) {
            done_fn_.delay(n * delay_->value());
        } else
            bangTo(1);
    }
}

void FlowSpace::addClock(const std::function<void()>& fn)
{
    done_fn_.unset();
    packet_count_++;

    // note: inserted order is from back to beginning
    // to minimize dynamic clock allocations we try to find expired clock
    // and schedule it
    for (auto it = pipe_.rbegin(); it != pipe_.rend(); ++it) {
        if (!it->clock.isActive() && !it->new_one) { // find expired clock
            it->clock.setCallback(fn);
            it->new_one = true;
            return;
        }
    }

    // no free clocks found - insert new one
    pipe_.emplace_front(fn);
    pipe_.front().new_one = true;
}

void FlowSpace::onBang()
{
    if (packet_count_ == 0)
        packet_sched_.delay(0);

    auto fn = [this]() { bangTo(0); clockDone(); };
    addClock(fn);
}

void FlowSpace::onFloat(t_float f)
{
    if (packet_count_ == 0)
        packet_sched_.delay(0);

    auto fn = [this, f]() { floatTo(0, f); clockDone(); };
    addClock(fn);
}

void FlowSpace::onSymbol(t_symbol* s)
{
    if (packet_count_ == 0)
        packet_sched_.delay(0);

    auto fn = [this, s]() { symbolTo(0, s); clockDone(); };
    addClock(fn);
}

void FlowSpace::onList(const AtomListView& lv)
{
    if (packet_count_ == 0)
        packet_sched_.delay(0);

    // make a copy for lambda capture
    AtomList16 l;
    l.insert_back(lv);
    auto fn = [l, this]() { listTo(0, l.view()); clockDone(); };
    addClock(fn);
}

void FlowSpace::onAny(t_symbol* s, const AtomListView& lv)
{
    if (packet_count_ == 0)
        packet_sched_.delay(0);

    // make a copy for lambda capture
    AtomList16 l;
    l.insert_back(lv);
    auto fn = [this, s, l]() { anyTo(0, s, l.view()); clockDone(); };
    addClock(fn);
}

void FlowSpace::m_reset(const AtomListView&)
{
    for (auto& c : pipe_)
        c.clock.unset();

    pipe_.clear();
    packet_sched_.unset();
    done_fn_.unset();
    num_active_ = 0;
    packet_count_ = 0;
}

void FlowSpace::m_prop_delay(const AtomListView& lv)
{
    delay_->set(lv);
}

void FlowSpace::m_prop_done(const AtomListView& lv)
{
    done_->set(lv);
}

void FlowSpace::setInterval(int, t_float f)
{
    delay_->setValue(f);
}

void setup_flow_space()
{
    InletProxy<FlowSpace>::init();
    InletProxy<FlowSpace>::set_method_callback(gensym("reset"), &FlowSpace::m_reset);
    InletProxy<FlowSpace>::set_method_callback(gensym("@delay"), &FlowSpace::m_prop_delay);
    InletProxy<FlowSpace>::set_method_callback(gensym("@done"), &FlowSpace::m_prop_done);
    InletProxy<FlowSpace>::set_float_callback(&FlowSpace::setInterval);

    ObjectFactory<FlowSpace> obj("flow.space");
    obj.noPropsDispatch();

    obj.setXletsInfo({ "any message", "float: set interval (ms)\n"
                                      "reset: remove scheduled output" },
        { "any message", "bang: after @done*@delay (ms)" });
}
