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
#include "ceammc_crc32.h"
#include "ceammc_factory.h"
#include "fmt/core.h"

#define PROP_TIME "@t"
#define PROP_BLOCK "@block"
#define METHOD_DUMP "dump"
#define METHOD_RESET "reset"

constexpr int INLET_MAIN = 0;
constexpr int INLET_CTL = 1;

FlowSeqDelay::FlowSeqDelay(const PdArgs& args)
    : BaseObject(args)
    , clock_([this]() {
        messageTo(idx_++, msg_);

        while (scheduleNext())
            ;
    })
    , main_inlet_(this, INLET_MAIN)
    , ctl_inlet_(this, INLET_CTL)
    , idx_(0)
    , in_process_(false)
    , is_init_(true)
{
    appendInlet(inlet_new(owner(), main_inlet_.target(), nullptr, nullptr));
    appendInlet(inlet_new(owner(), ctl_inlet_.target(), nullptr, nullptr));

    auto prop = createCbListProperty(
        PROP_TIME, [this]() -> AtomList {
            AtomList res;
            res.reserve(time_.size());

            for (auto t : time_)
                res.append(t);

            return res; },
        [this](const AtomListView& lv) {
            if (is_init_) {
                time_.reserve(lv.size());

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
                    OBJ_ERR << fmt::format("expected list of size={}, got list with size={}", time_.size(), lv.size());
                    return false;
                }

                for (size_t i = 0; i < time_.size(); i++) {
                    const auto& a = lv[i];
                    if (a.isFloat() && a.asT<t_float>() >= 0) {
                        time_[i] = a.asT<t_float>();
                    } else {
                        OBJ_ERR << "invalid delay time: " << a;
                        continue;
                    }
                }

                idx_ = 0;
                in_process_ = false;
                clock_.unset();
            }

            return true;
        });

    prop->setArgIndex(0);
    prop->setUnitsMs();

    block_ = new BoolProperty(PROP_BLOCK, false);
    addProperty(block_);
}

void FlowSeqDelay::initDone()
{
    for (size_t i = 0; i < time_.size(); i++)
        createOutlet();

    is_init_ = false;
}

void FlowSeqDelay::m_reset(t_symbol* s, const AtomListView& lv)
{
    clock_.unset();
    idx_ = 0;
    in_process_ = false;
}

void FlowSeqDelay::on_proxy_any(int idx, t_symbol* s, const AtomListView& lv)
{
    if (idx == INLET_MAIN) {
        if (block_->value() && in_process_)
            return;

        in_process_ = block_->value();

        if (s == &s_bang && lv.empty())
            msg_.setBang();
        else if (s == &s_float && lv.isFloat())
            msg_.setFloat(lv.asFloat());
        else if (s == &s_symbol && lv.isSymbol())
            msg_.setSymbol(lv.asSymbol());
        else if (s == &s_list)
            msg_.setList(lv);
        else
            msg_.setAny(s, lv);

        handleNewMessage();
    } else if (idx == INLET_CTL) {

        switch (crc32_hash(s)) {
        case PROP_TIME ""_hash:
        case PROP_BLOCK ""_hash:
            setProperty(s, lv);
            break;
        case METHOD_DUMP ""_hash:
            dump();
            break;
        case METHOD_RESET ""_hash:
            m_reset(s, lv);
            break;
        case "list"_hash:
            setProperty(PROP_TIME, lv);
            break;
        default:
            OBJ_ERR << "unknown message: " << Message(s, lv);
            break;
        }
    }
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
    if (idx_ >= time_.size()) {
        in_process_ = false;
        return false;
    }

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
    InletProxy<FlowSeqDelay>::init();
    InletProxy<FlowSeqDelay>::set_any_callback(&FlowSeqDelay::on_proxy_any);

    ObjectFactory<FlowSeqDelay> obj("flow.seqdelay", OBJECT_FACTORY_NO_DEFAULT_INLET);
    obj.addAlias("flow.seqdel");
    obj.setInletsInfo({ "input message", "control inlet for properties" });
    obj.addMethod("reset", &FlowSeqDelay::m_reset);
}
