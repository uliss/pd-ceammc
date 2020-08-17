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
#include "flow_stack.h"
#include "ceammc_factory.h"

FlowStack::FlowStack(const PdArgs& a)
    : BaseObject(a)
    , inlet_(this)
    , max_size_(nullptr)
{
    inlet_new(owner(), &inlet_.x_obj, nullptr, nullptr);
    createOutlet();

    max_size_ = new IntProperty("@max_size", 100);
    max_size_->setArgIndex(0);
    max_size_->checkPositive();
    max_size_->setSuccessFn([this](Property*) { stack_.reserve(max_size_->value()); });
    addProperty(max_size_);

    createCbIntProperty("@size", [this]() { return stack_.size(); });
    createCbBoolProperty("@empty", [this]() { return stack_.empty(); });
}

void FlowStack::onBang()
{
    if (stack_.size() >= max_size_->value()) {
        return;
    }

    stack_.push_back(Message::makeBang());
}

void FlowStack::onFloat(t_float f)
{
    if (stack_.size() >= max_size_->value()) {
        return;
    }

    stack_.push_back(Message(f));
}

void FlowStack::onSymbol(t_symbol* s)
{
    if (stack_.size() >= max_size_->value()) {
        return;
    }

    stack_.push_back(Message(s));
}

void FlowStack::onList(const AtomList& l)
{
    if (stack_.size() >= max_size_->value()) {
        return;
    }

    stack_.push_back(Message(l));
}

void FlowStack::onAny(t_symbol* s, const AtomListView& lv)
{
    if (stack_.size() >= max_size_->value()) {
        return;
    }

    stack_.push_back(Message(s, lv));
}

void FlowStack::initDone()
{
    stack_.reserve(max_size_->value());
}

void FlowStack::dump() const
{
    BaseObject::dump();
    size_t idx = 0;
    for (auto it = stack_.crbegin(); it != stack_.crend(); ++it, ++idx) {
        OBJ_POST << '[' << idx << "] " << *it;
    }
}

void FlowStack::m_pop(const AtomListView&)
{
    if (stack_.empty()) {
        OBJ_ERR << "stack is empty";
        return;
    }

    stack_.pop_back();
}

void FlowStack::m_clear(const AtomListView&)
{
    stack_.clear();
}

void FlowStack::m_top(const AtomListView&)
{
    if (stack_.empty()) {
        OBJ_ERR << "stack is empty";
        return;
    }

    messageTo(0, stack_.back());
}

void FlowStack::m_flush(const AtomListView&)
{
    if (stack_.empty()) {
        OBJ_ERR << "stack is empty";
        return;
    }

    for (auto it = stack_.rbegin(); it != stack_.rend(); ++it)
        messageTo(0, *it);

    stack_.clear();
}

void FlowStack::m_poptop()
{
    if (stack_.empty()) {
        OBJ_ERR << "stack is empty";
        return;
    }

    messageTo(0, stack_.back());
    stack_.pop_back();
}

void setup_flow_stack()
{
    ObjectFactory<FlowStack> obj("flow.stack");
    obj.noInletsDispatch();
    obj.noPropsDispatch();

    using IProxy = InletProxy<FlowStack>;
    IProxy::init();
    IProxy::set_method_callback(gensym("clear"), &FlowStack::m_clear);
    IProxy::set_method_callback(gensym("flush"), &FlowStack::m_flush);
    IProxy::set_method_callback(gensym("pop"), &FlowStack::m_pop);
    IProxy::set_method_callback(gensym("top"), &FlowStack::m_top);
    IProxy::set_bang_callback(&FlowStack::m_poptop);

    obj.setXletsInfo({ "any: data flow", "bang:  output top and remove it\n"
                                         "top:   output top\n"
                                         "pop:   remove top element\n"
                                         "clear: remove all\n"
                                         "flush: output and remove all" },
        { "any: output" });
}
