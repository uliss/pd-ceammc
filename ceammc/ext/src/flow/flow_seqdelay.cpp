/*****************************************************************************
 * Copyright 2022 Serge Poltavsky. All rights reserved.
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
#include "flow_seqdelay.h"
#include "ceammc_factory.h"
#include "fmt/format.h"

FlowSeqDelay::FlowSeqDelay(const PdArgs& args)
    : BaseObject(args)
    , clock_([this]() {
        messageTo(idx_++, msg_);

        while (scheduleNext())
            ;
    })
    , idx_(0)
    , in_process_(false)
    , on_init_(true)
{
    auto prop = createCbListProperty(
        "@t", [this]() -> AtomList {
            AtomList res;
            res.reserve(time_.size());

            for (auto t : time_)
                res.append(t);

            return res; },
        [this](const AtomListView& lv) {
            if (on_init_) {
                for (auto& a : lv) {
                    if (a.isFloat() && a.asT<t_float>() >= 0) {
                        time_.push_back(a.asT<t_float>());
                    } else {
                        OBJ_ERR << "invalid delay value: " << a << ", skipping";
                        continue;
                    }
                }
            } else {
                if (lv.size() != time_.size()) {
                    OBJ_ERR << fmt::format("expected list of delays with size={}, got list with size={}", time_.size(), lv.size());
                    return false;
                }

                // expected lv.size() == time_.size()
                for (size_t i = 0; i < time_.size(); i++) {
                    auto& a = lv[i];
                    if (a.isFloat() && a.asT<t_float>() >= 0) {
                        time_[i] = a.asT<t_float>();
                    } else {
                        OBJ_ERR << "invalid delay value: " << a << ", skipping";
                    }
                }
            }

            updateOutletTooltips();
            return true;
        });

    prop->setArgIndex(0);
    prop->setInitOnly();

    block_ = new BoolProperty("@block", false);
    addProperty(block_);
}

void FlowSeqDelay::initDone()
{
    for (size_t i = 0; i < time_.size(); i++)
        createOutlet();

    on_init_ = false;
}

void FlowSeqDelay::onBang()
{
    if (block_->value() && in_process_)
        return;

    in_process_ = block_->value();
    msg_.setBang();
    handleNewMessage();
}

void FlowSeqDelay::onFloat(t_float f)
{
    if (block_->value() && in_process_)
        return;

    in_process_ = block_->value();
    msg_.setFloat(f);
    handleNewMessage();
}

void FlowSeqDelay::onSymbol(t_symbol* s)
{
    if (block_->value() && in_process_)
        return;

    in_process_ = block_->value();
    msg_.setSymbol(s);
    handleNewMessage();
}

void FlowSeqDelay::onList(const AtomList& l)
{
    if (block_->value() && in_process_)
        return;

    in_process_ = block_->value();
    msg_.setList(l);
    handleNewMessage();
}

void FlowSeqDelay::onAny(t_symbol* s, const AtomListView& l)
{
    if (block_->value() && in_process_)
        return;

    in_process_ = block_->value();
    msg_.setAny(s, l);
    handleNewMessage();
}

const char* FlowSeqDelay::annotateOutlet(size_t n) const
{
    if (n >= outlet_tooltips_.size())
        return "";
    else
        return outlet_tooltips_[n].c_str();
}

void FlowSeqDelay::handleNewMessage()
{
    clock_.unset();
    idx_ = 0;

    while (scheduleNext())
        ;
}

bool FlowSeqDelay::scheduleNext()
{
    if (idx_ >= time_.size())
        return false;

    const auto t = time_[idx_];
    if (t == 0) {
        messageTo(idx_++, msg_);
        return true;
    } else {
        clock_.delay(t);
        return false;
    }
}

void FlowSeqDelay::updateOutletTooltips()
{
    outlet_tooltips_.assign(time_.size(), {});

    t_float sum = 0;
    for (size_t i = 0; i < time_.size(); i++) {
        const auto t = time_[i];
        sum += t;
        if (sum != t)
            outlet_tooltips_[i] = fmt::format("delay: {}ms ({}ms total)", t, sum);
        else
            outlet_tooltips_[i] = fmt::format("delay: {}ms", t);
    }
}

void setup_flow_seqdelay()
{
    ObjectFactory<FlowSeqDelay> obj("flow.seqdelay");
    obj.addAlias("flow.seqdel");
    obj.setInletsInfo({ "input message" });
}
