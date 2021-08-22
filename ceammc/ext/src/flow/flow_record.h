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
#ifndef FLOW_RECORD_H
#define FLOW_RECORD_H

#include <deque>
#include <utility>

#include "ceammc_clock.h"
#include "ceammc_containers.h"
#include "ceammc_object.h"
#include "ceammc_proxy.h"
using namespace ceammc;

using FlowMessage = SmallMessageN<4>;

struct FlowEvent {
    const FlowMessage* msg;
    double t_ms;
};

class FlowRecord : public BaseObject {
public:
    using ControlProxy = InletProxy<FlowRecord>;
    using Events = std::deque<FlowEvent>;

    enum State {
        STOP,
        PLAY,
        RECORD
    };

private:
    ControlProxy control_;
    ClockLambdaFunction clock_;
    Events events_; ///< abs logic time ms
    double rec_start_; ///< abs logic time ms
    double rec_stop_; ///< abs logic time ms
    IntProperty* max_size_;
    IntProperty* repeats_;
    BoolProperty* auto_start_;
    BoolProperty* sync_;
    FloatProperty* speed_;
    State state_;
    size_t current_idx_;
    int repeat_counter_;
    // abs sync event time ms
    // first - prev sync time ms
    // second - current sync time ms
    std::pair<double, double> sync_time_;

public:
    FlowRecord(const PdArgs& args);
    ~FlowRecord();

    void onBang() override;
    void onFloat(t_float v) override;
    void onSymbol(t_symbol* s) override;
    void onList(const AtomList& lst) override;
    void onAny(t_symbol* s, const AtomListView& lv) override;

    void m_play(const AtomListView& lv);
    void m_stop(const AtomListView& lv) { setState(lv.boolAt(0, true) ? STOP : PLAY); }
    void m_pause(const AtomListView& lv) { setState(STOP); }
    void m_record(const AtomListView& lv) { setState(lv.boolAt(0, true) ? RECORD : STOP); }
    void m_clear(const AtomListView& lv);
    void m_flush(const AtomListView& lv);
    void m_quant(const AtomListView& lv);
    void m_qlist(const AtomListView& lv);
    void m_bang();

    void dump() const override;

    double recStartMs() const { return rec_start_; }
    double recLengthMs() const { return rec_stop_ - rec_start_; }

public:
    const Events& events() const { return events_; }

private:
    bool appendMessage(FlowMessage* m);
    bool sizeInf() const { return max_size_->value() == 0; }
    bool hasSpace() const { return max_size_->value() == 0 || ((int)events_.size() < max_size_->value()); }

    void setState(State new_st);
    void clear();

    bool repeatAgain() const { return (repeats_->value() > 0 && repeat_counter_ < repeats_->value()) || repeats_->value() < 0; }

    void schedMs(t_float ms) { clock_.delay(ms / speed_->value()); }

    void startRec();

private:
    static inline double now_sys() { return clock_getlogicaltime(); }
    static inline double now_ms() { return clock_gettimesince(0); }
    static inline double sys_time2ms(double tsys) { return clock_gettimesince(now_sys() - tsys); }
};

void setup_flow_record();

#endif // FLOW_RECORD_H
