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
#include "flow_ring.h"
#include "ceammc_factory.h"

constexpr int MIN_LEN = 1;
constexpr int MAX_LEN = 24;
constexpr int DEFAULT_LEN = 2;

FlowRing::FlowRing(const PdArgs& a)
    : BaseObject(a)
    , length_(nullptr)
    , end_(0)
    , total_(0)
    , inlet_(this)
{
    length_ = new IntProperty("@length", DEFAULT_LEN);
    length_->setInitOnly();
    length_->setArgIndex(0);
    length_->checkClosedRange(MIN_LEN, MAX_LEN);
    addProperty(length_);

    inlet_new(owner(), &inlet_.x_obj, nullptr, nullptr);
}

void FlowRing::initDone()
{
    for (int i = 0; i < length_->value(); i++)
        createOutlet();

    line_.assign(length_->value(), {});
}

void FlowRing::onBang()
{
    addMessage(Message::makeBang());
    output();
}

void FlowRing::onFloat(t_float f)
{
    addMessage(Message(f));
    output();
}

void FlowRing::onSymbol(t_symbol* s)
{
    addMessage(Message(s));
    output();
}

void FlowRing::onList(const AtomList& l)
{
    addMessage(Message(l));
    output();
}

void FlowRing::onAny(t_symbol* s, const AtomListView& lv)
{
    addMessage(Message(s, lv));
    output();
}

void FlowRing::proxy_bang(int)
{
    output();
}

void FlowRing::proxy_get(const AtomListView& lv)
{
    if (!lv.isInteger()) {
        OBJ_ERR << "integer expected, got: " << lv;
        return;
    }

    auto idx = lv.asT<int>();
    if (idx < 0 || idx >= line_.size()) {
        OBJ_ERR << "invalid index: " << idx;
        return;
    }

    auto el_idx = (end_ + idx) % line_.size();

    if (!line_[el_idx].isNone())
        messageTo(idx, line_[el_idx]);
}

void FlowRing::proxy_fill(const AtomListView& lv)
{
    for (auto& m : line_)
        m = Message(lv);
}

void FlowRing::addMessage(const Message& m)
{
    // not fullfilled
    if (total_ < line_.size()) {
        line_[end_++] = m;
        total_++;
        return;
    } else { // full
        end_ = end_ % line_.size();
        line_[end_++] = m;
    }
}

void FlowRing::output()
{
    for (size_t i = 0; i < line_.size(); i++) {
        auto idx = (end_ + i) % line_.size();
        auto out_idx = line_.size() - (i + 1);
        auto& msg = line_[idx];
        if (!msg.isNone())
            messageTo(out_idx, msg);
    }
}

void setup_flow_ring()
{
    ObjectFactory<FlowRing> obj("flow.ring");
    obj.addAlias("flow.>>");
    obj.noInletsDispatch();
    obj.noPropsDispatch();

    InletProxy<FlowRing>::init();
    InletProxy<FlowRing>::set_bang_callback(&FlowRing::proxy_bang);
    InletProxy<FlowRing>::set_method_callback(gensym("get"), &FlowRing::proxy_get);
    InletProxy<FlowRing>::set_method_callback(gensym("fill"), &FlowRing::proxy_fill);

    obj.setXletsInfo({ "any: input flow", "bang: output all outlets\n"
                                          "get IDX: output specified outlet\n"
                                          "fill VALUE: fill buffer with sepcified value (no output)\n" },
        { "" });
}
