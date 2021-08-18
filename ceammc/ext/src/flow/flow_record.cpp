/*****************************************************************************
 * Copyright 2021 Serge Poltavsky. All rights reserved.
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
#include "flow_record.h"
#include "ceammc_containers.h"
#include "ceammc_factory.h"

#include "../mempool/MemoryPool.h"

constexpr int MIN_SIZE = 0;
constexpr int MAX_SIZE = 1024 * 32;
constexpr int DEFAULT_SIZE = 256;

using MessagePool = SingletonMeyers<MemoryPool<FlowMessage>>;

static inline double sysTimeToMs(double t)
{
    const auto now = clock_getlogicaltime();
    return clock_gettimesince(now - t);
}

FlowRecord::FlowRecord(const PdArgs& args)
    : BaseObject(args)
    , control_(this)
    , clock_([this]() {
        if (events_.empty())
            return;

        if (current_idx_ >= events_.size())
            return;

        const auto last_time = events_[current_idx_].second;

        auto first = events_.begin() + current_idx_;
        // find simultaneous messages
        auto last = std::find_if_not(first, events_.end(), [last_time](const FlowEvent& e) { return e.second == last_time; });

        // output messages
        auto outl = outletAt(0);
        for (auto it = first; it != last; ++it) {
            current_idx_++;
            it->first->outputTo(outl);
        }

        // schedule next
        if (current_idx_ < events_.size()) {
            const auto next_time = events_[current_idx_].second;
            clock_.delay(next_time - last_time);
        } else {
            repeat_counter_++;
            if (repeatAgain()) {
                current_idx_ = 0;
                clock_.delay(rec_len_ms_ - last_time);
                // OBJ_DBG << "repeat: " << rec_len_ms_ - last_time;
            } else {
                state_ = STOP;
                // OBJ_DBG << "stopped";
            }
        }
    })
    , rec_start_(0)
    , rec_len_ms_(0)
    , max_size_(nullptr)
    , repeats_(nullptr)
    , auto_start_(nullptr)
    , state_(STOP)
    , repeat_counter_(0)
{
    inlet_new(owner(), &control_.x_obj, nullptr, nullptr);
    createOutlet();

    max_size_ = new IntProperty("@max", DEFAULT_SIZE);
    max_size_->checkClosedRange(MIN_SIZE, MAX_SIZE);
    addProperty(max_size_);

    repeats_ = new IntProperty("@r", 1);
    repeats_->checkMinEq(-1);
    addProperty(repeats_);

    auto_start_ = new BoolProperty("@auto", false);
    addProperty(auto_start_);
}

FlowRecord::~FlowRecord()
{
    clear();
}

void FlowRecord::onBang()
{
    appendMessage(MessagePool::instance().newElement());
}

void FlowRecord::onFloat(t_float v)
{
    appendMessage(MessagePool::instance().newElement(v));
}

void FlowRecord::onSymbol(t_symbol* s)
{
    appendMessage(MessagePool::instance().newElement(s));
}

void FlowRecord::onList(const AtomList& lst)
{
    appendMessage(MessagePool::instance().newElement(lst.view()));
}

void FlowRecord::onAny(t_symbol* s, const AtomListView& lv)
{
    appendMessage(MessagePool::instance().newElement(s, lv));
}

void FlowRecord::m_clear(const AtomListView& lv)
{
    clear();
}

void FlowRecord::m_quant(const AtomListView& lv)
{
    if (!lv.isFloat()) {
        OBJ_ERR << "quant float value expected, got: " << lv;
        return;
    }

    const auto q = lv[0].asT<t_float>();
    if (q <= 0) {
        OBJ_ERR << "positive value expected, got: " << q;
        return;
    }

    // quant events
    for (auto& e : events_)
        e.second = std::round(e.second / q) * q;

    // quant length
    rec_len_ms_ = std::round(rec_len_ms_ / q) * q;
}

void FlowRecord::m_flush(const AtomListView& lv)
{
    auto* outl = outletAt(0);

    for (size_t i = current_idx_; i < events_.size(); i++) {
        events_[i].first->outputTo(outl);
    }

    clear();
}

void FlowRecord::dump() const
{
    BaseObject::dump();
    OBJ_POST << "length: "
             << rec_len_ms_ << "ms, "
             << "events: ";
    for (auto& e : events_)
        OBJ_POST << " - [" << e.second << "] " << e.first->view();
}

void FlowRecord::appendMessage(FlowMessage* m)
{
    if (!auto_start_->value() && state_ != RECORD)
        return;

    if (auto_start_->value() && state_ == STOP)
        setState(RECORD);

    if (!sizeInf() && events_.size() > MAX_SIZE) {
        OBJ_ERR << "overfulled, max size has reached: " << events_.size();
        return;
    }

    // store event time relative to record start
    const auto time_ms = sysTimeToMs(clock_getlogicaltime() - rec_start_);
    events_.push_back({ m, time_ms });
}

void FlowRecord::setState(FlowRecord::State new_st)
{
    switch (state_) {
    case STOP: {
        switch (new_st) {
        case PLAY: {
            if (events_.empty()) {
                OBJ_DBG << "no events";
                return;
            }

            clock_.delay(events_.front().second);
            state_ = PLAY;
            current_idx_ = 0;
            repeat_counter_ = 0;
            OBJ_DBG << "playing length: " << rec_len_ms_ << "ms";
            return;
        }
        case RECORD: {
            OBJ_DBG << "record started";
            clear();
            state_ = RECORD;
            rec_start_ = clock_getlogicaltime();
            rec_len_ms_ = 0;
            return;
        }
        case STOP: {
            OBJ_DBG << "already stopped";
            return;
        }
        default: {
            OBJ_ERR << "unknown new state: " << (int)new_st;
            return;
        }
        }
    } break;
    case PLAY: {
        switch (new_st) {
        case STOP: {
            clock_.unset();
            state_ = STOP;
            OBJ_DBG << "stopped";
            return;
        }
        case PLAY: {
            OBJ_DBG << "already playing";
            return;
        }
        case RECORD: {
            OBJ_ERR << "should be stopped before recording";
            return;
        }
        default: {
            OBJ_ERR << "unknown new state: " << (int)new_st;
            return;
        }
        }
    } break;
    case RECORD: {
        switch (new_st) {
        case STOP: {
            state_ = STOP;
            rec_len_ms_ = sysTimeToMs(clock_getlogicaltime() - rec_start_);
            OBJ_DBG << "record stopped: length " << rec_len_ms_ << "ms";
            return;
        }
        case PLAY: {
            if (events_.empty()) {
                OBJ_DBG << "no events";
                return;
            }

            clock_.delay(events_.front().second);
            state_ = PLAY;
            current_idx_ = 0;
            repeat_counter_ = 0;
            rec_len_ms_ = sysTimeToMs(clock_getlogicaltime() - rec_start_);
            OBJ_DBG << "playing length: " << rec_len_ms_ << "ms";
            return;
        }
        }
    } break;
    default: {
        OBJ_ERR << "unknown state: " << state_;
        return;
    };
    }
}

void FlowRecord::clear()
{
    auto& pool = MessagePool::instance();

    for (auto& el : events_)
        pool.deleteElement(el.first);

    events_.clear();
}

void setup_flow_record()
{
    ObjectFactory<FlowRecord> obj("flow.record");
    obj.addAlias("flow.rec");

    obj.noInletsDispatch();
    obj.noPropsDispatch();

    FlowRecord::ControlProxy::init();
    FlowRecord::ControlProxy::set_method_callback(gensym("play"), &FlowRecord::m_play);
    FlowRecord::ControlProxy::set_method_callback(gensym("stop"), &FlowRecord::m_stop);
    FlowRecord::ControlProxy::set_method_callback(gensym("pause"), &FlowRecord::m_pause);
    FlowRecord::ControlProxy::set_method_callback(gensym("rec"), &FlowRecord::m_record);
    FlowRecord::ControlProxy::set_method_callback(gensym("clear"), &FlowRecord::m_clear);
    FlowRecord::ControlProxy::set_method_callback(gensym("flush"), &FlowRecord::m_flush);
    FlowRecord::ControlProxy::set_method_callback(gensym("quant"), &FlowRecord::m_quant);
}
