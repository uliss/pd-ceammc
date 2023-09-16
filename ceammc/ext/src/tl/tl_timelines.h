/*****************************************************************************
 * Copyright 2018 Serge Poltavsky. All rights reserved.
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
#ifndef TL_TIMELINES_H
#define TL_TIMELINES_H

#include "ceammc_clock.h"
#include "ceammc_log.h"
#include "tl_eventlist.h"

#include <cmath>
#include <functional>
#include <limits>
#include <numeric>
#include <vector>

namespace ceammc {
namespace tl {

#define TL_ERR ceammc::LIB_ERR << "[tl] "
#define TL_DBG ceammc::LIB_DBG << "[tl] "

    enum RunState {
        STATE_INIT = 0,
        STATE_STOP,
        STATE_RUN,
        STATE_PAUSE,
        STATE_GOTO
    };

    enum LoopMode {
        MODE_FIXED = 0,
        MODE_INFINITE
    };

    static const size_t NUM_RUN_STATES = 5;

    template <class T>
    class TimeLineSingle {
        T* owner_;
        EventList events_;
        size_t event_idx_;
        double start_time_sys_;
        double pause_time_ms_;
        ceammc::ClockLambdaFunction clock_;
        t_float len_;
        RunState state_;
        LoopMode mode_;
        bool loop_;
        double inf_mode_event_tmp_;

        // params
        double goto_time_ms_;

        typedef bool (TimeLineSingle::*fsmActionFn)();
        struct FsmEntry {
            RunState state;
            fsmActionFn fns[NUM_RUN_STATES];
        };

        static FsmEntry fsm_[NUM_RUN_STATES];

    public:
        TimeLineSingle(T* owner, t_float length_ms)
            : owner_(owner)
            , clock_([this]() { tick(); })
            , event_idx_(0)
            , start_time_sys_(0)
            , pause_time_ms_(0)
            , len_(length_ms > 0 ? length_ms : 0)
            , state_(STATE_INIT)
            , mode_(MODE_FIXED)
            , goto_time_ms_(0)
            , loop_(false)
            , inf_mode_event_tmp_(0)
        {
            clear();
        }

        t_float length() const { return len_; }
        void setLength(t_float l) { len_ = (l > 0) ? l : 0; clear(); }
        RunState state() const { return state_; }
        bool isLoop() const { return loop_; }
        void setLoop(bool value) { loop_ = value; }

        LoopMode mode() const { return mode_; }

        void setMode(LoopMode m)
        {
            if (events_.empty()) {
                TL_ERR << "empty events";
                return;
            }

            mode_ = m;

            switch (m) {
            case MODE_INFINITE:
                inf_mode_event_tmp_ = events_.back().abs_time;
                events_.back().abs_time = std::numeric_limits<double>::max();
                calcRelativeEvents();
                break;
            case MODE_FIXED:
                events_.back().abs_time = inf_mode_event_tmp_;
                calcRelativeEvents();
                break;
            }
        }

        const EventList& events() const { return events_; }

        double currentTime() const
        {
            switch (state_) {
            case STATE_PAUSE:
                return pause_time_ms_;
            case STATE_GOTO:
                return goto_time_ms_;
            case STATE_RUN:
                return clock_gettimesince(start_time_sys_);
            case STATE_INIT:
            case STATE_STOP:
            default:
                return 0;
            }
        }

        bool start()
        {
            return toState(STATE_RUN);
        }

        bool stop()
        {
            return toState(STATE_STOP);
        }

        bool pause()
        {
            return toState(STATE_PAUSE);
        }

        bool removeByName(t_symbol* s)
        {
            auto idx = events_.findPosByName(s);
            if (idx < 0)
                return false;

            return removeAtPos(idx);
        }

        bool removeAtTime(double ms)
        {
            auto idx = events_.findPosByAbsTime(ms);
            if (idx < 0)
                return false;

            return removeAtPos(idx);
        }

        bool removeAtPos(int pos)
        {
            auto ptr = events_.at(pos);
            if (!ptr)
                return false;

            const auto removed_time = ptr->abs_time;

            if (!events_.removeAtPos(pos))
                return false;

            if (state_ == STATE_RUN && pos < events_.size()) {
                const auto& next_ev = events_[pos];
                const auto t = currentTime();
                if (removed_time > t) {
                    clock_.delay(next_ev.abs_time - t);
                }
            }

            return true;
        }

        void clear()
        {
            switch (mode_) {
            case MODE_FIXED:
                events_.clear(len_);
                break;
            case MODE_INFINITE:
                events_.clear(std::numeric_limits<double>::max());
                break;
            }

            if (state_ == STATE_RUN) {
                // delay until end
                clock_.delay(events_.back().abs_time - currentTime());
            }

            event_idx_ = 0;
        }

        void insertProcess(size_t idx)
        {
            if (state_ == STATE_RUN) {
                // added time in future
                if (events_[idx].abs_time > currentTime()) {
                    event_idx_ = idx;
                    clock_.delay(events_[idx].abs_time - currentTime());
                } else
                    event_idx_++;
            }
        }

        bool add(double ms, t_symbol* name = &s_)
        {
            if (mode_ == MODE_INFINITE && ms <= 0) {
                TL_ERR << "use positive time values in infinite mode: " << ms;
                return false;
            }

            ms = fromRelativeTime(ms);
            if (ms < 0)
                return false;

            Event ev(ms, name);
            if (events_.exists(ev)) {
                TL_ERR << "event already exists on timeline: " << ev;
                return false;
            }

            auto idx = events_.addAbsEvent(ev);
            insertProcess(idx);

            return true;
        }

        bool addRelative(t_symbol* name, double ms, t_symbol* relName)
        {
            Event ev = Event::relEvent(name, ms, relName);
            auto idx = events_.addRelEvent(ev);
            if (idx < 0) {
                TL_ERR << "can't add relative event: " << ev;
                return false;
            }

            insertProcess(idx);
            return true;
        }

        void calcRelativeEvents()
        {
            events_.calcRelEvents();
            events_.calcNextEvents();
        }

        void tick()
        {
            const auto N = events_.size();
            // assert(N > 0);

            // not last event
            if ((event_idx_ + 1) < N) {
                owner_->event(event_idx_, events_[event_idx_]);
                clock_.delay(events_[++event_idx_].next_time);
            } else {
                // last event
                owner_->eventEnd();

                // looped
                if (loop_) {
                    event_idx_ = 0;
                    start_time_sys_ = clock_getlogicaltime();
                    clock_.delay(events_[event_idx_].next_time);
                } else
                    toState(STATE_STOP);
            }
        }

        void reset()
        {
            if (!toState(STATE_INIT))
                TL_ERR << "can reset only when stopped";
        }

        bool toTime(t_float ms)
        {
            goto_time_ms_ = fromRelativeTime(ms);
            if (!toState(STATE_GOTO) || ms < 0) {
                TL_ERR << "invalid time value: " << ms;
                return false;
            }

            return true;
        }

        bool toEvent(t_symbol* name)
        {
            auto idx = events_.findPosByName(name);
            if (idx < 0)
                return false;

            return toEvent(idx);
        }

        bool toEvent(int idx)
        {
            const int N = events_.size();
            if (idx < -N || idx >= N) {
                TL_ERR "invalid event index: " << idx;
                return false;
            }

            if (idx < 0)
                idx += N;

            goto_time_ms_ = events_[idx].abs_time;
            return toState(STATE_GOTO);
        }

        t_float fromRelativeTime(t_float ms) const
        {
            if (ms >= len_) {
                TL_ERR << "invalid event time: should be < " << len_ << " ms";
                return -1;
            }

            if (ms < 0) {
                if (ms < -len_) {
                    TL_ERR << "invalid event time: should be >= " << -len_ << " ms";
                    return -1;
                } else
                    ms += len_;
            }

            return ms;
        }

        // FSM
        bool init2run()
        {
            // init
            start_time_sys_ = 0;
            pause_time_ms_ = 0;
            event_idx_ = 0;

            if (events_.empty()) {
                TL_ERR << "no events";
                return false;
            }

            // save start time
            start_time_sys_ = clock_getlogicaltime();
            owner_->eventStart();

            // fire first event
            clock_.delay(events_.front().next_time);
            return true;
        }

        bool init2goto()
        {
            if (goto_time_ms_ < 0)
                return false;

            return true;
        }

        bool stop2init()
        {
            clock_.unset();
            start_time_sys_ = 0;
            pause_time_ms_ = 0;
            event_idx_ = 0;
            return true;
        }

        bool run2pause()
        {
            clock_.unset();
            pause_time_ms_ = clock_gettimesince(start_time_sys_);

            return true;
        }

        bool run2stop()
        {
            clock_.unset();
            event_idx_ = 0;

            return true;
        }

        bool pause2run()
        {
            static const double UNIT = clock_getsystimeafter(1) - clock_getsystime();

            if (events_.empty()) {
                TL_ERR << "no events";
                return false;
            }

            if (event_idx_ >= events_.size()) {
                TL_ERR << "all events are expired";
                return false;
            }

            double from_start = clock_getlogicaltime() - start_time_sys_;
            double off = from_start - pause_time_ms_ * UNIT;
            start_time_sys_ += off;

            clock_.delay(events_[event_idx_].abs_time - pause_time_ms_);
            return true;
        }

        bool pause2stop()
        {
            pause_time_ms_ = 0;
            start_time_sys_ = 0;
            event_idx_ = 0;

            return true;
        }

        bool goto2init()
        {
            goto_time_ms_ = 0;
            start_time_sys_ = 0;
            event_idx_ = 0;

            return true;
        }

        bool goto2goto()
        {
            return true;
        }

        bool goto2run()
        {
            static const double UNIT = clock_getsystimeafter(1) - clock_getsystime();

            if (events_.empty()) {
                TL_ERR << "no events";
                return false;
            }

            if (event_idx_ >= events_.size()) {
                TL_ERR << "all events are expired";
                return false;
            }

            // update start time
            start_time_sys_ = clock_getlogicaltime() - goto_time_ms_ * UNIT;

            // find nearest event
            auto it = std::upper_bound(events_.begin(), events_.end(), Event(goto_time_ms_),
                [](const Event& e0, const Event& e1) {
                    return e0.abs_time <= e1.abs_time;
                });

            if (it != events_.end()) {
                // now *it >= ms

                event_idx_ = std::distance(events_.begin(), it);

                // if exact match found
                if (std::fabs(it->abs_time - goto_time_ms_) < std::numeric_limits<float>::epsilon()) {
                    // output current event
                    if (event_idx_ == (events_.size() - 1)) {
                        owner_->eventEnd();
                    } else {
                        owner_->event(event_idx_, events_[event_idx_].abs_time);
                        clock_.delay(events_[++event_idx_].next_time);
                    }
                } else {
                    // *it > ms
                    auto del_ms = events_[event_idx_].abs_time - goto_time_ms_;
                    clock_.delay(del_ms);
                }
            } else {
                TL_ERR << "all events are expired";
                return false;
            }

            return true;
        }

        bool stop2goto()
        {
            if (goto_time_ms_ < 0)
                return false;

            return true;
        }

        bool pause2goto()
        {
            if (goto_time_ms_ < 0)
                return false;

            return true;
        }

        bool toState(RunState st_new)
        {
            static const char* STATES[NUM_RUN_STATES] = {
                "init", "stop", "run", "pause", "goto"
            };

            FsmEntry act = fsm_[state_];
            fsmActionFn mem_fn = act.fns[st_new];

            // no arc
            if (!mem_fn) {
                TL_ERR << "invalid transition: " << STATES[state_] << "->" << STATES[st_new];
                return false;
            }

            // unsuccessfull call
            if (!(*this.*mem_fn)())
                return false;

            // go to next state
            state_ = st_new;
            return true;
        }
    };

// clang-format off
#define FSM_TO_INIT(name) &TimeLineSingle<T>::name##2init
#define FSM_TO_STOP(name) &TimeLineSingle<T>::name##2stop
#define FSM_TO_RUN(name) &TimeLineSingle<T>::name##2run
#define FSM_TO_PAUSE(name) &TimeLineSingle<T>::name##2pause
#define FSM_TO_GOTO(name) &TimeLineSingle<T>::name##2goto
template <typename T>
typename TimeLineSingle<T>::FsmEntry TimeLineSingle<T>::fsm_[NUM_RUN_STATES] = {
    //               init,              stop,               run,               pause,             goto
    { STATE_INIT,  { nullptr,           nullptr,            FSM_TO_RUN(init),  nullptr,           FSM_TO_GOTO(init) } },
    { STATE_STOP,  { FSM_TO_INIT(stop), nullptr,            FSM_TO_RUN(init),  nullptr,           FSM_TO_GOTO(stop) } },
    { STATE_RUN,   { nullptr,           FSM_TO_STOP(run),   nullptr,           FSM_TO_PAUSE(run), nullptr } },
    { STATE_PAUSE, { nullptr,           FSM_TO_STOP(pause), FSM_TO_RUN(pause), nullptr,           FSM_TO_GOTO(pause) } },
    { STATE_GOTO,  { FSM_TO_INIT(goto), nullptr,            FSM_TO_RUN(goto),  nullptr,           FSM_TO_GOTO(goto) } }
};

// clanf-format on
}
}

#endif // TL_TIMELINES_H
