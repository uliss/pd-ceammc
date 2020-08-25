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
#include "flow_mem.h"
#include "ceammc_factory.h"

#include <array>
#include <cassert>

constexpr size_t MIN_NUM = 1;
constexpr size_t DEF_NUM = 1;
constexpr size_t MAX_NUM = 32;

std::array<std::string, MAX_NUM> inlet_info;
std::array<std::string, MAX_NUM> outlet_info;

FlowMem::FlowMem(const PdArgs& a)
    : BaseObject(a)
    , num_(nullptr)
    , ttl_(nullptr)
    , free_(nullptr)
{
    num_ = new IntProperty("@n", DEF_NUM);
    num_->checkClosedRange(MIN_NUM, MAX_NUM);
    num_->setArgIndex(0);
    addProperty(num_);

    ttl_ = new IntProperty("@ttl", 0);
    num_->checkClosedRange(0, 1024);
    ttl_->setArgIndex(1);
    ttl_->setSuccessFn([this](Property*) { mem_ttl_.assign(mem_ttl_.size(), ttl_->value()); });
    addProperty(ttl_);

    free_ = new BoolProperty("@free", true);
    addProperty(free_);
}

void FlowMem::initDone()
{
    mem_.resize(num_->value());
    mem_ttl_.resize(num_->value(), ttl_->value());
    inlets_.reserve(num_->value());

    for (int i = 0; i < num_->value(); i++) {
        inlets_.emplace_back(this);
        inlet_new(owner(), &inlets_.back().x_obj, nullptr, nullptr);

        createOutlet();
    }

    createInlet();
}

void FlowMem::onInlet(size_t, const AtomList& v)
{
    const auto ttl = ttl_->value();

    if (v.empty()) { // bang
        for (int i = num_->value(); i > 0; i--) {
            if (ttl == 0)
                messageTo(i - 1, mem_[i - 1]);
            else if (mem_ttl_[i - 1] > 0) {
                messageTo(i - 1, mem_[i - 1]);
                mem_ttl_[i - 1]--;
            } else if (free_->value()) // free value when ttl is expired and @free is true
                mem_[i - 1] = Message();
        }
    } else if (v.isFloat()) {
        auto idx = v.asT<int>();
        if (idx >= 0 && idx < num_->value()) {
            if (ttl == 0)
                messageTo(idx, mem_[idx]);
            else if (mem_ttl_[idx] > 0) {
                messageTo(idx, mem_[idx]);
                mem_ttl_[idx]--;
            } else if (free_->value()) // free value when ttl is expired and @free is true
                mem_[idx] = Message();
        }
    }
}

void FlowMem::proxy_any(InletProxy<FlowMem>* x, t_symbol* s, const AtomListView& v)
{
    auto it = std::find_if(inlets_.begin(), inlets_.end(), [x](const InletProxy<FlowMem>& i) { return x == &i; });
    if (it == inlets_.end()) {
        OBJ_ERR << "invalid inlet";
        return;
    }

    auto idx = std::distance(inlets_.begin(), it);
    assert(idx >= 0 && idx < mem_.size());
    mem_[idx] = Message(s, v);
    mem_ttl_[idx] = ttl_->value();
}

const char* FlowMem::annotateInlet(size_t n) const
{
    const size_t N = std::min(inlet_info.size(), mem_.size());

    if (n < N)
        return inlet_info[n].c_str();
    else if (n == N)
        return "bang: output all\n"
               "int:  output specified cell";
    else
        return nullptr;
}

const char* FlowMem::annotateOutlet(size_t n) const
{
    if (n < outlet_info.size())
        return outlet_info[n].c_str();
    else
        return nullptr;
}

void setup_flow_mem()
{
    for (size_t i = 0; i < inlet_info.size(); i++) {
        char buf[32];
        snprintf(buf, sizeof(buf), "store \\[%d\\]", (int)i);
        inlet_info[i] = buf;
    }

    for (size_t i = 0; i < outlet_info.size(); i++) {
        char buf[32];
        snprintf(buf, sizeof(buf), "output \\[%d\\]", (int)i);
        outlet_info[i] = buf;
    }

    ObjectFactory<FlowMem>
        obj("flow.mem", OBJECT_FACTORY_NO_DEFAULT_INLET);
    obj.noPropsDispatch();

    InletProxy<FlowMem>::init();
    InletProxy<FlowMem>::set_any_callback(&FlowMem::proxy_any);
}
