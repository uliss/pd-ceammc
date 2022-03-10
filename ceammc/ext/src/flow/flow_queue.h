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
#ifndef FLOW_QUEUE_H
#define FLOW_QUEUE_H

#include <vector>

#include "ceammc_object.h"
#include "ceammc_proxy.h"
using namespace ceammc;

class MessageQueue {
    std::vector<Message> queue_;
    size_t begin_ = { 0 };
    size_t end_ = { 0 };
    size_t total_ = { 0 };
    size_t max_size_ = { 0 };

public:
    explicit MessageQueue(size_t max = 32);
    bool push(const Message& m);
    bool pop();
    size_t size() const { return total_; }
    bool empty() const { return total_ == 0; }
    size_t capacity() const { return queue_.size() - total_; }
    const Message& at(size_t n) const;
    void clear();
};

class FlowQueue : public BaseObject {

public:
    struct ControlInlet {
        FlowQueue* pimpl;
        void on_bang(int) { pimpl->proxy_pop(); }
        void m_pop(const AtomListView&) { pimpl->proxy_pop(); }
        void m_clear(const AtomListView&) { pimpl->proxy_clear(); }
        void m_output(const AtomListView&) { pimpl->proxy_output(); }
        void m_flush(const AtomListView& v) { pimpl->proxy_flush(); }
    };
    using ControlProxy = InletProxy<ControlInlet>;
    using Inlet = InletProxy<FlowQueue>;

private:
    std::vector<MessageQueue> queue_;
    std::vector<Inlet> inlets_;
    ControlInlet ctrl_;
    ControlProxy control_;
    IntProperty* max_size_;
    BoolProperty* auto_pop_;

public:
    FlowQueue(const PdArgs& a);
    void initDone() override;

    void proxy_any(Inlet* x, t_symbol* s, const AtomListView& v);
    void proxy_pop();
    void proxy_clear();
    void proxy_output();
    void proxy_flush();
};

void setup_flow_queue();

#endif // FLOW_QUEUE_H
