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
    , on_full_(nullptr)
    , on_empty_(nullptr)
{
    inlet_new(owner(), &inlet_.x_obj, nullptr, nullptr);
    createOutlet();

    max_size_ = new IntProperty("@max_size", 100);
    max_size_->setArgIndex(0);
    max_size_->checkMinEq(1);
    max_size_->setSuccessFn([this](Property*) { stack_.reserve(max_size_->value()); });
    addProperty(max_size_);

    createCbIntProperty("@size", [this]() { return stack_.size(); });
    createCbBoolProperty("@empty", [this]() { return stack_.empty(); });

    on_full_ = new ListProperty("@on_full");
    on_full_->setListCheckFn([this](const AtomList& l) -> bool {
        if (l.empty() || l[0].isSymbol())
            return true;

        OBJ_ERR << "[@on_full] symbol expected as first argument, got: " << l[0];
        return false;
    });
    addProperty(on_full_);

    on_empty_ = new ListProperty("@on_empty");
    on_empty_->setListCheckFn([this](const AtomList& l) -> bool {
        if (l.empty() || l[0].isSymbol())
            return true;

        OBJ_ERR << "[@on_empty] symbol expected as first argument, got: " << l[0];
        return false;
    });
    addProperty(on_empty_);
}

void FlowStack::onBang()
{
    if (stack_.size() >= max_size_->value()) {
        return;
    }

    stack_.push_back(Message::makeBang());
    check_full();
}

void FlowStack::onFloat(t_float f)
{
    if (stack_.size() >= max_size_->value()) {
        return;
    }

    stack_.push_back(Message(f));
    check_full();
}

void FlowStack::onSymbol(t_symbol* s)
{
    if (stack_.size() >= max_size_->value()) {
        return;
    }

    stack_.push_back(Message(s));
    check_full();
}

void FlowStack::onList(const AtomList& l)
{
    if (stack_.size() >= max_size_->value()) {
        return;
    }

    stack_.push_back(Message(l));
    check_full();
}

void FlowStack::onAny(t_symbol* s, const AtomListView& lv)
{
    if (stack_.size() >= max_size_->value()) {
        return;
    }

    stack_.push_back(Message(s, lv));
    check_full();
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
    check_empty();
}

void FlowStack::m_clear(const AtomListView&)
{
    stack_.clear();
    check_empty();
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
    check_empty();
}

void FlowStack::m_poptop(int)
{
    if (stack_.empty()) {
        OBJ_ERR << "stack is empty";
        return;
    }

    messageTo(0, stack_.back());
    stack_.pop_back();
    check_empty();
}

void FlowStack::check_empty()
{
    if (stack_.empty()) {
        t_symbol* s = on_empty_->value().symbolAt(0, nullptr);
        if (s && s->s_thing) {
            AtomListView v = on_empty_->value().view(1);
            if (v.empty())
                pd_bang(s->s_thing);
            else if (v.isFloat())
                pd_float(s->s_thing, v.asT<t_float>());
            else if (v.isSymbol())
                pd_symbol(s->s_thing, v.asT<t_symbol*>());
            else if (v.symbolAt(0, nullptr))
                pd_typedmess(s->s_thing, v.symbolAt(0, &s_), v.size() - 1, v.toPdData() + 1);
            else
                pd_list(s->s_thing, &s_list, v.size(), v.toPdData());
        }
    }
}

void FlowStack::check_full()
{
    if (stack_.size() == max_size_->value()) {
        t_symbol* s = on_full_->value().symbolAt(0, nullptr);
        if (s && s->s_thing) {
            AtomListView v = on_full_->value().view(1);
            if (v.empty())
                pd_bang(s->s_thing);
            else if (v.isFloat())
                pd_float(s->s_thing, v.asT<t_float>());
            else if (v.isSymbol())
                pd_symbol(s->s_thing, v.asT<t_symbol*>());
            else if (v.symbolAt(0, nullptr))
                pd_typedmess(s->s_thing, v.symbolAt(0, &s_), v.size() - 1, v.toPdData() + 1);
            else
                pd_list(s->s_thing, &s_list, v.size(), v.toPdData());
        }
    }
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

    obj.setXletsInfo({ "any: push to stack", "bang:  output top and remove it\n"
                                             "top:   output top\n"
                                             "pop:   remove top element\n"
                                             "clear: remove all\n"
                                             "flush: output and remove all" },
        { "any: output" });
}
