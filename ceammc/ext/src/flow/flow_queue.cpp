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
#include "flow_queue.h"
#include "ceammc_factory.h"

#include <cassert>

MessageQueue::MessageQueue(size_t max)
    : queue_(max)
    , max_size_(max)
{
}

bool MessageQueue::push(const Message& m)
{
    if (total_ == max_size_)
        return false;

    assert(max_size_ > 0);

    if (total_ == 0) {
        begin_ = 0;
        end_ = 1 % max_size_;
        total_ = 1;
        queue_[0] = m;
        return true;
    }

    queue_[end_] = m;
    end_ = (end_ + 1) % max_size_;
    total_++;
    return true;
}

bool MessageQueue::pop()
{
    if (total_ == 0)
        return false;

    queue_[begin_] = Message();
    begin_ = (begin_ + 1) % max_size_;
    total_--;
    return true;
}

const Message& MessageQueue::at(size_t n) const
{
    assert(max_size_ > 0);

    auto idx = (begin_ + n) % max_size_;
    return queue_[idx];
}

void MessageQueue::clear()
{
    begin_ = 0;
    end_ = 0;
    total_ = 0;
}

FlowQueue::FlowQueue(const PdArgs& a)
    : BaseObject(a)
    , ctrl_({ this })
    , control_(&ctrl_)
    , max_size_(nullptr)
    , auto_pop_(nullptr)
{
    max_size_ = new IntProperty("@max_size", 32);
    max_size_->checkClosedRange(1, 1024);
    max_size_->setArgIndex(0);
    addProperty(max_size_);

    auto_pop_ = new BoolProperty("@auto_pop", false);
    addProperty(auto_pop_);
}

void FlowQueue::initDone()
{
    auto N = positionalConstant<1, 1, 32>(1);
    size_t SZ = max_size_->value();

    queue_.reserve(N);
    inlets_.reserve(N);
    for (size_t i = 0; i < N; i++) {
        queue_.emplace_back(SZ);
        inlets_.emplace_back(this);
        inlet_new(owner(), &inlets_.back().x_obj, nullptr, nullptr);
        createOutlet();
    }

    inlet_new(owner(), &control_.x_obj, nullptr, nullptr);
}

void FlowQueue::proxy_any(FlowQueue::Inlet* x, t_symbol* s, const AtomListView& v)
{
    auto it = std::find_if(inlets_.cbegin(), inlets_.cend(), [x](const Inlet& i) { return &i == x; });
    if (it == inlets_.cend())
        return; // should never happen

    auto idx = std::distance(inlets_.cbegin(), it);

    assert(inlets_.size() == queue_.size());
    assert(idx >= 0 && idx < queue_.size());

    auto& q = queue_[idx];
    if (q.capacity() == 0) {
        if (auto_pop_->value()) {
            messageTo(idx, q.at(0));
            q.pop();
        } else {
            OBJ_ERR << "no free space at query[" << idx << ']';
            return;
        }
    }

    q.push(Message(s, v));
}

void FlowQueue::proxy_pop()
{
    for (size_t i = queue_.size(); i > 0; i--) {
        auto& q = queue_[i - 1];
        if (q.empty())
            continue;

        messageTo(i - 1, q.at(0));
        q.pop();
    }
}

void FlowQueue::proxy_clear()
{
    for (auto& q : queue_)
        q.clear();
}

void FlowQueue::proxy_output()
{
    for (size_t i = queue_.size(); i > 0; i--) {
        auto& q = queue_[i - 1];
        if (q.empty())
            continue;

        for (size_t j = 0; j < q.size(); j++)
            messageTo(i - 1, q.at(j));
    }
}

void FlowQueue::proxy_flush()
{
    proxy_output();
    proxy_clear();
}

void setup_flow_queue()
{
    ObjectFactory<FlowQueue> obj("flow.queue", OBJECT_FACTORY_NO_DEFAULT_INLET);
    FlowQueue::Inlet::init();
    FlowQueue::Inlet::set_any_callback(&FlowQueue::proxy_any);
    FlowQueue::ControlProxy::init();
    FlowQueue::ControlProxy::set_bang_callback(&FlowQueue::ControlInlet::on_bang);
    FlowQueue::ControlProxy::set_method_callback(gensym("pop"), &FlowQueue::ControlInlet::m_pop);
    FlowQueue::ControlProxy::set_method_callback(gensym("clear"), &FlowQueue::ControlInlet::m_clear);
    FlowQueue::ControlProxy::set_method_callback(gensym("output"), &FlowQueue::ControlInlet::m_output);
    FlowQueue::ControlProxy::set_method_callback(gensym("flush"), &FlowQueue::ControlInlet::m_flush);
}
