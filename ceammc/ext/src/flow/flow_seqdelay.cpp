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

FlowSeqDelay::FlowSeqDelay(const PdArgs& args)
    : BaseObject(args)
    , clock_([this]() {
        messageTo(idx_++, msg_);

        while (scheduleNext())
            ;
    })
    , idx_(0)
    , in_process_(false)
{
    auto prop = createCbListProperty(
        "@t", [this]() -> AtomList {
            AtomList res;
            res.reserve(time_.size());

            for (auto t : time_)
                res.append(t);

            return res; },
        [this](const AtomListView& lv) {
            for (auto& a : lv) {
                if (a.isFloat() && a.asT<t_float>() >= 0) {
                    time_.push_back(a.asT<t_float>());
                } else {
                    OBJ_ERR << "invalid delay value: " << a << ", skipping";
                    continue;
                }
            }
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

void setup_flow_seqdelay()
{
    ObjectFactory<FlowSeqDelay> obj("flow.seqdelay");
    obj.addAlias("flow.seqdel");
}
