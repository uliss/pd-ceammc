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
#include "flow_loop.h"
#include "ceammc_factory.h"

#include <cmath>
#include <numeric>

FlowLoop::FlowLoop(const PdArgs& a)
    : BaseObject(a)
    , n_(nullptr)
    , quant_(nullptr)
    , echo_(nullptr)
    , clock_([this]() {
        play_idx_++;
        play();
    })
    , mode_(MODE_REC)
    , last_event_time_(0)
    , play_idx_(0)
{
    createInlet();
    createOutlet();
    createOutlet();

    n_ = new IntProperty("@n", 1);
    n_->checkMinEq(1);
    n_->setArgIndex(0);
    addProperty(n_);

    quant_ = new IntProperty("@q", 0);
    quant_->checkNonNegative();
    addProperty(quant_);

    echo_ = new BoolProperty("@echo", true);
    addProperty(echo_);
}

void FlowLoop::quant()
{
    if (quant_->value() > 0) {
        auto total = std::accumulate(msg_offsets_.begin(), msg_offsets_.end(), 0);
        auto step = total / quant_->value();
        for (size_t i = 1; i < msg_offsets_.size(); i++)
            msg_offsets_[i] += msg_offsets_[i - 1];

        for (auto& off : msg_offsets_)
            off = std::round(off / step) * step;

        for (size_t i = msg_offsets_.size() - 1; i > 0; i--)
            msg_offsets_[i] -= msg_offsets_[i - 1];

        for (auto& off : msg_offsets_)
            LIB_DBG << "qoff: " << off;
    }
}

void FlowLoop::appendMessage(const Message& m)
{
    if (messages_.size() == 0) {
        messages_.push_back(m);
        if (echo_->value())
            messageTo(0, m);

    } else if (messages_.size() < n_->value()) {
        messages_.emplace_back(m);
        msg_offsets_.push_back(clock_gettimesince(last_event_time_));

        if (echo_->value())
            messageTo(0, m);
    } else if (messages_.size() >= n_->value()) {
        msg_offsets_.push_back(clock_gettimesince(last_event_time_));
        mode_ = MODE_PLAY;
        play_idx_ = 0;

        quant();
        play();
    }

    last_event_time_ = clock_getlogicaltime();
}

void FlowLoop::onBang()
{
    if (mode_ != MODE_REC)
        return;

    appendMessage(&s_bang);
}

void FlowLoop::onFloat(t_float f)
{
    if (mode_ != MODE_REC)
        return;

    appendMessage(f);
}

void FlowLoop::onSymbol(t_symbol* s)
{
    if (mode_ != MODE_REC)
        return;

    appendMessage(s);
}

void FlowLoop::onList(const AtomList& l)
{
    if (mode_ != MODE_REC)
        return;

    appendMessage(l);
}

void FlowLoop::onInlet(size_t n, const AtomListView& l)
{
    mode_ = MODE_REC;
    messages_.clear();
    msg_offsets_.clear();
}

void FlowLoop::play()
{
    if (mode_ != MODE_PLAY) {
        clock_.unset();
        return;
    }

    const size_t N = std::min(messages_.size(), msg_offsets_.size());
    if (N < 1)
        return;

    play_idx_ %= N;

    OBJ_DBG << "play:  " << play_idx_;
    OBJ_DBG << "del: " << msg_offsets_[play_idx_];

    floatTo(1, play_idx_);
    messageTo(0, messages_[play_idx_]);

    if (msg_offsets_[play_idx_] < 5)
        return;

    clock_.delay(msg_offsets_[play_idx_]);
}

void FlowLoop::messageTo(size_t n, const Message& msg)
{
    if (msg.isBang())
        bangTo(n);
    else
        BaseObject::messageTo(n, msg);
}

void setup_flow_loop()
{
    ObjectFactory<FlowLoop> obj("flow.loop");

    FlowLoop::setOutletsInfo(obj.classPointer(), { "any: output message", "int: current loop index" });
}
