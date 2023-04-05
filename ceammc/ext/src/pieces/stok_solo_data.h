/*****************************************************************************
 * Copyright 2023 Serge Poltavski. All rights reserved.
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
#ifndef STOK_SOLO_DATA_H
#define STOK_SOLO_DATA_H

#include "ceammc_log.h"
#include "fmt/core.h"

#include <array>
#include <cstddef>
#include <cstdint>
#include <ctime>
#include <random>
#include <vector>

#include <boost/container/small_vector.hpp>

namespace solo {

enum {
    EVENT_VALUE_NOPERF = 200,
    EVENT_VALUE_CRESC,
    EVENT_VALUE_DIM,
    EVENT_VALUE_DIM_CRESC,
    EVENT_VALUE_BURST,
    EVENT_VALUE_BURST_SLOWDOWN,
};

using namespace ceammc;

enum EventType : std::int8_t {
    EVENT_OFF,
    EVENT_ON,
    EVENT_CYCLE,
    EVENT_CRESC,
    EVENT_DIM,
    EVENT_DIM_CRESC,
    EVENT_BURST,
};

enum SoloEventType : std::int8_t {
    SOLO_EVENT_OFF,
    SOLO_EVENT_ON
};

enum SoloTrack : std::uint8_t {
    TRACK_MIC1 = 0,
    TRACK_MIC2,
    TRACK_FB1,
    TRACK_FB2,
    TRACK_OUT1,
    TRACK_OUT2,
    TRACK_COUNT,
};

enum SoloCycle : std::uint8_t {
    CYCLE_A = 0,
    CYCLE_B,
    CYCLE_C,
    CYCLE_D,
    CYCLE_E,
    CYCLE_F,
    CYCLE_COUNT,
};

struct PositionInfo {
    double phase;
    int cycle;
    double cycle_phase;
    int cycle_period;
    double period_phase;
    bool operator==(const PositionInfo& pi) const
    {
        return phase == pi.phase
            && cycle == pi.cycle
            && cycle_phase == pi.cycle_phase
            && cycle_period == pi.cycle_period
            && period_phase == pi.period_phase;
    }
    bool operator!=(const PositionInfo& pi) const
    {
        return !operator==(pi);
    }
};

struct Period {
    float abs_length { 0 }, rel_pos { 0 }, rel_length { 1 }, from { 0 }, to { 0 };
    EventType event { EVENT_OFF };
    const SoloCycle cycle_ { CYCLE_A };
    std::uint8_t nperf { 0 };

    Period(SoloCycle cycle, EventType type, float length)
        : cycle_(cycle)
        , event(type)
        , abs_length(length)
    {
    }

    SoloCycle cycle() const { return cycle_; }

    float attackTime() const
    {
        return rel_pos * abs_length;
    }

    float attackTimeMs() const
    {
        return 1000 * attackTime();
    }

    float releaseTime() const
    {
        return (rel_pos + rel_length) * abs_length;
    }

    float releaseTimeMs() const
    {
        return 1000 * releaseTime();
    }

    float duration() const
    {
        return rel_length * abs_length;
    }

    float durationMs() const
    {
        return 1000 * duration();
    }

    float fullLengthTime() const
    {
        return abs_length;
    }

    float fullLengthTimeMs() const
    {
        return 1000 * fullLengthTime();
    }

    Period& setRelOffset(float attack, float length)
    {
        rel_pos = attack;
        rel_length = length;
        return *this;
    }

    Period& setPerf(int n)
    {
        nperf = n;
        return *this;
    }

    char toString() const
    {
        switch (event) {
        case EVENT_ON:
            return nperf ? ('0' + nperf) : 'E';
        case EVENT_OFF:
            return '_';
        case EVENT_CRESC:
            return '<';
        case EVENT_DIM:
            return '>';
        case EVENT_DIM_CRESC:
            return 'X';
        default:
            return '?';
        }
    }
};

class CycleInfo {
    float length_ { 0 };
    int periods_ { 0 };

public:
    CycleInfo()
        : CycleInfo(0, 0)
    {
    }

    CycleInfo(int repeats, float periodLength)
        : length_(periodLength)
        , periods_(repeats)
    {
    }

    int periodCount() const { return periods_; }
    float periodLength() const { return length_; }
    float cycleLength() const { return length_ * periods_; }
};

struct PeriodTrack : public std::vector<Period> {
    solo::SoloTrack track;
};

struct Scheme {
    std::array<PeriodTrack, TRACK_COUNT> tracks_;
    std::array<CycleInfo, CYCLE_COUNT> cycles_;
    int scheme_ { 0 };

    explicit Scheme(size_t n = 0)
    {
        setScheme(n);
    }

    /**
     * return current scheme
     */
    int scheme() const { return scheme_; }

    const std::array<PeriodTrack, TRACK_COUNT>& tracks() const { return tracks_; }
    const std::array<CycleInfo, TRACK_COUNT>& cycles() const { return cycles_; }

    /**
     * return number of cycles in the scheme (always should return 6)
     */
    size_t cycleCount() const { return cycles_.size(); }

    /**
     * return period index for the begining of cycle
     */
    size_t cycleBeginPeriodIndex(SoloCycle c) const
    {
        size_t res = 0;

        int idx = 0;
        for (auto& x : cycles_) {
            if (idx++ < c)
                res += x.periodCount();
            else
                break;
        }

        return res;
    }

    /**
     * return period index for the end of cycle
     */
    size_t cycleEndPeriodIndex(SoloCycle c) const
    {
        size_t res = 0;

        int idx = 0;
        for (auto& x : cycles_) {
            if (idx++ <= c)
                res += x.periodCount();
            else
                break;
        }

        return res;
    }

    /**
     * return time in seconds before specified period position
     */
    float periodTimeSec(size_t pos) const
    {
        if (pos > tracks_.front().size()) { // (!) using '>', not the '>='
            LIB_ERR << fmt::format("invalid position index: {}", pos);
            return 0;
        }

        float res = 0;

        for (size_t i = 0; i < pos; i++)
            res += tracks_.front()[i].abs_length;

        return res;
    }

    void set(const CycleInfo& a,
        const CycleInfo& b,
        const CycleInfo& c,
        const CycleInfo& d,
        const CycleInfo& e,
        const CycleInfo& f)
    {
        cycles_[0] = a;
        cycles_[1] = b;
        cycles_[2] = c;
        cycles_[3] = d;
        cycles_[4] = e;
        cycles_[5] = f;

        int num_periods = 0;
        for (auto& ci : cycles_)
            num_periods += ci.periodCount();

        for (auto& t : tracks_)
            t.reserve(num_periods);
    }

    /**
     * return total scheme length in seconds
     */
    float lengthSec() const
    {
        float res = 0;
        for (auto& ci : cycles_)
            res += ci.cycleLength();

        return res;
    }

    /**
     * clear all track events
     * @note tracks are not removed
     */
    void clear()
    {
        for (auto& t : tracks_)
            t.clear();
    }

    void addTrackCycleEvents(PeriodTrack& track, float cycleOffset, SoloCycle cycle, const std::initializer_list<int>& init)
    {
        const auto& ci = cycles_[cycle];
        const auto CN = ci.periodCount();

        if (init.size() != CN)
            LIB_DBG << fmt::format("warning: period number mismatch: {} != {} for cycle{}",
                CN, init.size(), cycle);

        const auto N = std::min<size_t>(init.size(), CN);
        for (size_t i = 0; i < N; i++) {
            auto x = *(init.begin() + i);

            switch (x) {
            case 0:
                track.emplace_back(cycle, EVENT_OFF, ci.periodLength());
                break;
            case EVENT_VALUE_NOPERF:
                track.emplace_back(cycle, EVENT_ON, ci.periodLength());
                track.back().nperf = 0;
                break;
            case EVENT_VALUE_CRESC:
                track.emplace_back(cycle, EVENT_CRESC, ci.periodLength());
                track.back().nperf = 0;
                break;
            case EVENT_VALUE_BURST:
                track.emplace_back(cycle, EVENT_BURST, ci.periodLength());
                track.back().nperf = 1;
                break;
            default:
                track.emplace_back(cycle, EVENT_ON, ci.periodLength());
                track.back().nperf = x;
                break;
            }
        }
    }

    void addTrackEvents(PeriodTrack& track,
        const std::initializer_list<int>& a, const std::initializer_list<int>& b,
        const std::initializer_list<int>& c, const std::initializer_list<int>& d,
        const std::initializer_list<int>& e, const std::initializer_list<int>& f)
    {
        float offset = 0;
        addTrackCycleEvents(track, offset, CYCLE_A, a);
        offset += cycles_[0].cycleLength();
        addTrackCycleEvents(track, offset, CYCLE_B, b);
        offset += cycles_[1].cycleLength();
        addTrackCycleEvents(track, offset, CYCLE_C, c);
        offset += cycles_[2].cycleLength();
        addTrackCycleEvents(track, offset, CYCLE_D, d);
        offset += cycles_[3].cycleLength();
        addTrackCycleEvents(track, offset, CYCLE_E, e);
        offset += cycles_[4].cycleLength();
        addTrackCycleEvents(track, offset, CYCLE_F, f);
    }

    void addTrackEvents(int track,
        const std::initializer_list<int>& a, const std::initializer_list<int>& b,
        const std::initializer_list<int>& c, const std::initializer_list<int>& d,
        const std::initializer_list<int>& e, const std::initializer_list<int>& f)
    {
        if (track >= 0 && track < TRACK_COUNT) {
            tracks_[track].track = static_cast<SoloTrack>(track);
            addTrackEvents(tracks_[track], a, b, c, d, e, f);
        } else
            LIB_ERR << "invalid track index: " << track;
    }

    std::string toString(size_t n) const
    {
        std::string res;
        if (n >= TRACK_COUNT)
            return res;

        for (auto& x : tracks_[n])
            res += x.toString();

        return res;
    }

    void setScheme1()
    {
        scheme_ = 1;
        clear();
        set({ 1, 1 }, { 1, 1 }, { 1, 1 }, { 1, 1 }, { 1, 1 }, { 1, 1 });

        addTrackEvents(0,
            {},
            {},
            {},
            {},
            {},
            {});

        addTrackEvents(1,
            {},
            {},
            {},
            {},
            {},
            {});

        addTrackEvents(2,
            {},
            {},
            {},
            {},
            {},
            {});

        addTrackEvents(3,
            {},
            {},
            {},
            {},
            {},
            {});

        addTrackEvents(4,
            {},
            {},
            {},
            {},
            {},
            {});

        addTrackEvents(5,
            {},
            {},
            {},
            {},
            {},
            {});
    }

    void setScheme2()
    {
        scheme_ = 2;
        clear();
        set({ 9, 12 }, { 7, 24 }, { 11, 6 }, { 10, 8.5 }, { 6, 30.4 }, { 8, 17.1 });

        addTrackEvents(TRACK_MIC1,
            { 0, 3, 2, 0, 2, 3, 0, 2, 0 },
            { 4, 1, 3, 1, 2, 0, 1 },
            { 2, 0, 1, 0, 1, 1, 0, 2, 0, E, 0 },
            { 0, 2, 1, 0, 1, 1, 0, 2, 1, 0 },
            { 3, 0, 0, 0, 0, 8 },
            { 0, 0, 3, 0, 0, 0, 6, 0 });

        addTrackEvents(TRACK_MIC2,
            { 2, 0, 0, 3, 0, 0, 2, 0, 2 },
            { 1, 4, 1, 2, 1, 3, 0 },
            { 0, 1, 0, 2, 0, 0, 1, 0, 2, 0, E },
            { 2, 0, 0, 1, 0, 0, 0, 0, 0, E },
            { 0, 5, E, E, 3, 0 },
            { 4, 0, 0, 0, 5, 0, 0, 0 });

        addTrackEvents(TRACK_FB1,
            { 0, 0, 2, 0, 0, 3, 0, 0, 2 },
            { 0, 2, 1, 1, 1, 1, E },
            { 0, 1, 1, 1, 1, 1, 0, 0, 1, E, E },
            { 0, 0, 1, E, 1, 2, 0, 0, 1, E },
            { 0, 1, 2, 2, 1, 1 },
            { 1, 1, 0, 2, 0, 0, 0, 0 });

        addTrackEvents(TRACK_FB2,
            { 0, 2, 0, 0, 2, 0, 0, 2, 1 },
            { 0, 1, 1, 1, 1, 1, E },
            { 0, 0, 1, 1, 1, 1, 1, 0, 0, E, E },
            { 0, 1, E, 1, E, 1, 1, E, 1, E },
            { 0, 0, E, E, 3, E },
            { 0, 2, 0, 0, 0, 3, 0, 0 });

        addTrackEvents(TRACK_OUT1,
            { 0, 0, E, E, 0, E, E, 0, E },
            { C, E, 0, E, 0, E, 0 },
            { E, E, E, E, E, E, E, 0, E, E, E },
            { 0, 0, E, E, 0, E, E, 0, E, E },
            { E, E, E, E, E, E },
            { B, B, B, E, E, 0, 0, E });

        periodAt(TRACK_OUT1, CYCLE_B, 0)->setRelOffset(0.5, 0.5);
        periodAt(TRACK_OUT1, CYCLE_C, 0)->setRelOffset(0.25, 0.25);
        periodAt(TRACK_OUT1, CYCLE_E, 0)->setRelOffset(0, 0.125);
        periodAt(TRACK_OUT1, CYCLE_F, 0)->setPerf(1);
        periodAt(TRACK_OUT1, CYCLE_F, 1)->setPerf(2);
        periodAt(TRACK_OUT1, CYCLE_F, 2)->setPerf(3);

        addTrackEvents(TRACK_OUT2,
            { 0, E, E, 0, E, E, 0, E, E },
            { C, 0, E, 0, E, 0, E },
            { E, 0, E, E, E, E, 0, E, 0, E, E },
            { 0, E, E, 0, E, 0, E, E, 0, E },
            { E, 0, E, 0, 0, E },
            { 2, E, E, 0, 0, E, E, 0 });

        periodAt(TRACK_OUT2, CYCLE_B, 0)->setRelOffset(0.5, 0.5);
        periodAt(TRACK_OUT2, CYCLE_C, 0)->setRelOffset(0.5, 0.25);
        periodAt(TRACK_OUT2, CYCLE_E, 0)->setRelOffset(0, 0.125);
    }

    bool setScheme(size_t idx)
    {
        switch (idx) {
        case 1:
            setScheme1();
            return true;
        case 2:
            setScheme2();
            return true;
        default:
            return false;
        }
    }

    /**
     * return pointer to period by specified multiindex (TRACK/CYCLE/PERIOD_IN_CYCLE)
     */
    Period* periodAt(SoloTrack track, SoloCycle cycle, size_t periodIdx)
    {
        auto& t = tracks_.at(track);
        auto idx = cycleBeginPeriodIndex(cycle) + periodIdx;
        return (idx < t.size()) ? &t.at(idx) : nullptr;
    }

    int periodCount() const
    {
        auto it = std::minmax_element(tracks_.begin(), tracks_.end(),
            [](const PeriodTrack& a, const PeriodTrack& b) {
                return a.size() < b.size();
            });

        if (it.first != it.second && it.first->size() != it.second->size()) {
            auto i0 = it.first - tracks_.begin();
            auto i1 = it.second - tracks_.begin();
            LIB_ERR << fmt::format("track length mismatch: [{}]{}!=[{}]{}", i0, it.first->size(), i1, it.second->size());
        }

        return tracks_.front().size();
    }

    float cyclePhase(double timeSec) const
    {
        double tm = 0;
        for (auto& ci : cycles_) {
            tm += ci.cycleLength();
            if (timeSec < tm)
                return 1 - ((tm - timeSec) / ci.cycleLength());
        }

        return 0;
    }

    int cycleAt(double timeSec) const
    {
        if (timeSec < 0)
            return -1;

        double tm = 0;
        int idx = 0;
        for (auto& ci : cycles_) {
            tm += ci.cycleLength();
            if (timeSec < tm)
                return (idx < CYCLE_COUNT) ? idx : -1;
            else
                ++idx;
        }

        return -1;
    }

    int periodAt(double timeSec) const
    {
        if (timeSec < 0)
            return -1;

        double tm = 0;
        int cidx = 0;
        for (auto& ci : cycles_) {
            tm += ci.cycleLength();
            if (timeSec < tm) {
                if (cidx >= CYCLE_COUNT)
                    return -1;

                auto time_from_cycle = (timeSec - (tm - ci.cycleLength()));
                return std::floor(time_from_cycle / ci.periodLength());

            } else
                ++cidx;
        }

        return -1;
    }

    float periodPhase(double timeSec) const
    {
        if (timeSec < 0)
            return 0;

        double tm = 0;
        int cidx = 0;
        for (auto& ci : cycles_) {
            tm += ci.cycleLength();
            if (timeSec < tm) {
                if (cidx >= CYCLE_COUNT)
                    return 0;

                auto time_from_cycle = (timeSec - (tm - ci.cycleLength()));
                return std::fmod(time_from_cycle, ci.periodLength()) / ci.periodLength();
            } else
                ++cidx;
        }

        return 0;
    }

    PositionInfo atTime(double timeSec) const
    {
        if (timeSec < 0)
            return { 0, 0, 0, 0, 0 };

        auto global_phase = timeSec / lengthSec();
        double tm = 0;
        int cidx = 0;
        for (auto& ci : cycles_) {
            tm += ci.cycleLength();
            if (timeSec < tm) {
                if (cidx >= CYCLE_COUNT)
                    return { 0, 0, 0, 0 };

                const auto time_from_cycle = (timeSec - (tm - ci.cycleLength()));
                const auto cycle_phase = time_from_cycle / ci.cycleLength();
                const int period = std::floor(time_from_cycle / ci.periodLength());
                const auto period_phase = std::fmod(time_from_cycle, ci.periodLength()) / ci.periodLength();

                return { global_phase, cidx, cycle_phase, period, period_phase };
            } else
                ++cidx;
        }

        return { 0, 0, 0, 0, 0 };
    }

private:
    const int E = EVENT_VALUE_NOPERF;
    const int C = EVENT_VALUE_CRESC;
    const int D = EVENT_VALUE_DIM;
    const int X = EVENT_VALUE_DIM_CRESC;
    const int B = EVENT_VALUE_BURST;
};

class SoloEvent {
    double abs_time_msec_ { 0 };
    float value_ { 0 };
    SoloEventType type_ { SOLO_EVENT_OFF };
    SoloTrack track_ { TRACK_MIC1 };
    SoloCycle cycle_ { CYCLE_A };
    int period_ { 0 };

public:
    SoloEvent(SoloCycle cycle, SoloTrack part, SoloEventType type, double time_msec)
        : abs_time_msec_(time_msec)
        , type_(type)
        , track_(part)
        , cycle_(cycle)
    {
    }

    int period() const { return period_; }
    SoloCycle cycle() const { return cycle_; }
    SoloEventType type() const { return type_; }
    SoloTrack track() const { return track_; }
    double absTimeMsec() const { return abs_time_msec_; }
    float value() const { return value_; }

    SoloEvent& setValue(float v)
    {
        value_ = v;
        return *this;
    }

    SoloEvent& setPeriod(int p)
    {
        period_ = p;
        return *this;
    }

    std::string toString() const
    {
        static const char* TRACKS[] = { "MIC1", "MIC2", "FB1", "FB2", "OUT1", "OUT2" };
        static const char* TYPES[] = { "OFF", "ON" };

        return fmt::format("{}: {} [{}]", TRACKS[track_], TYPES[type_], value_);
    }

    static SoloEvent off(SoloCycle c, SoloTrack t, double timeMs, int period)
    {
        return SoloEvent(c, t, SOLO_EVENT_OFF, timeMs).setValue(0).setPeriod(period);
    }

    static SoloEvent on(SoloCycle c, SoloTrack t, double timeMs, int period)
    {
        return SoloEvent(c, t, SOLO_EVENT_ON, timeMs).setValue(1).setPeriod(period);
    }
};

using SoloEventCont = std::vector<SoloEvent>;

class SoloEventList {
    SoloEventCont data_;
    std::default_random_engine gen_;
    long current_ { -1 };

public:
    SoloEventList()
        : gen_(std::time(nullptr))
    {
    }

    void seed(size_t s) { gen_.seed(s); }

    SoloEvent& add(const SoloEvent& v)
    {
        for (auto it = data_.begin(); it != data_.end(); ++it) {
            if (it->absTimeMsec() > v.absTimeMsec()) {
                auto x = data_.insert(it, v);
                return *x;
            }
        }

        data_.push_back(v);
        return data_.back();
    }

    const SoloEvent& back() const { return data_.back(); }
    SoloEvent& back() { return data_.back(); }

    void clear()
    {
        data_.clear();
        current_ = -1;
    }

    void reset()
    {
        current_ = data_.empty() ? -1 : 0;
    }

    const SoloEventCont& data() const { return data_; }

    bool isValidCurrent() const { return current_ >= 0 && current_ < data_.size(); }
    bool isValidNext() const { return current_ >= 0 && (current_ + 1) < data_.size(); }

    SoloEvent* currentPtr() { return isValidCurrent() ? &data_[current_] : nullptr; }
    const SoloEvent* currentPtr() const { return isValidCurrent() ? &data_[current_] : nullptr; }
    SoloEvent* nextPtr() { return isValidNext() ? &data_[current_ + 1] : nullptr; }
    long currentIdx() const { return current_; }

    double timeToNextEvent() const
    {
        if (!isValidNext())
            return -1;

        return data_[current_ + 1].absTimeMsec() - data_[current_].absTimeMsec();
    }

    void moveNext(std::function<void(const SoloEvent&)> fn)
    {
        if (isValidNext()) {
            if (fn)
                fn(*nextPtr());
        }

        current_++;
    }

    void moveSame(std::function<void(const SoloEvent&)> fn)
    {
        while (isValidNext()) {
            if (currentPtr()->absTimeMsec() == nextPtr()->absTimeMsec())
                moveNext(fn);
            else
                break;
        }
    }

    void addScheme(const Scheme& sc)
    {
        for (auto& t : sc.tracks()) {
            float time_ms = 0;
            int period_idx = 0;
            for (auto& p : t) {
                addPeriod(p, t.track, period_idx++, time_ms);
                time_ms += p.fullLengthTimeMs();
            }
        }
    }

    void addOff(SoloCycle cycle, SoloTrack t, float timeMs, int periodIdx)
    {
        add(SoloEvent::off(cycle, t, timeMs, periodIdx));
    }

    void addOn(const Period& p, SoloTrack t, float timeMs, int periodIdx)
    {
        if (p.attackTime() > 0)
            add(SoloEvent::off(p.cycle(), t, timeMs, periodIdx));

        add(SoloEvent::on(p.cycle(), t, timeMs + p.attackTimeMs(), periodIdx));

        if (p.nperf > 0) {
            float PERF_DUR_MSEC = 300;
            auto perf_dur = p.durationMs() - 3 * PERF_DUR_MSEC;

            // generate random times
            std::uniform_real_distribution<float> dist(0, 1);
            boost::container::small_vector<float, 8> randf;
            randf.reserve(p.nperf);
            float rand_sum = 0;

            for (int i = 0; i < p.nperf; i++) {
                auto v = dist(gen_);
                rand_sum += v;
                randf.push_back(v);
            }

            for (auto& rf : randf) {
                auto msec = timeMs + p.attackTime() + PERF_DUR_MSEC + (rf * perf_dur / rand_sum);
                add(SoloEvent::off(p.cycle(), t, msec, periodIdx));
                add(SoloEvent::on(p.cycle(), t, msec + PERF_DUR_MSEC, periodIdx));
            }
        }

        if (p.releaseTimeMs() < p.fullLengthTimeMs())
            add(SoloEvent::off(p.cycle(), t, timeMs + p.releaseTimeMs(), periodIdx));
    }

    void addCresc(const Period& p, SoloTrack t, float timeMs, int periodIdx)
    {
        if (p.attackTime() > 0)
            add(SoloEvent::off(p.cycle(), t, timeMs, periodIdx));

        auto offset_ms = p.attackTimeMs();
        constexpr float MIN_TIME_GRAIN_MS = 10;
        const auto DIV = std::min<int>(64, std::floor(p.durationMs() / MIN_TIME_GRAIN_MS));

        if (DIV < 1) {
            add(SoloEvent::on(p.cycle(), t, timeMs + offset_ms, periodIdx));
        } else {
            const auto TIME_INCR = p.durationMs() / DIV;
            for (int i = 0; i < DIV; i++) {
                add(SoloEvent::on(p.cycle(), t, timeMs + offset_ms, periodIdx).setValue(float(i) / DIV));
                offset_ms += TIME_INCR;
            }
        }
    }

    void addBurst(const Period& p, SoloTrack t, float timeMs, int periodIdx)
    {
        if (p.nperf < 1)
            return;

        addOff(p.cycle(), t, timeMs, periodIdx);

        constexpr float BURST_TIME_MS = 150;
        size_t n = p.nperf;
        double burst_space = (p.fullLengthTimeMs() - (n * BURST_TIME_MS)) / (n + 1);
        for (size_t i = 0; i < n; i++) {
            auto ms = timeMs + burst_space * (i + 1);
            add(SoloEvent::on(p.cycle(), t, ms, periodIdx));
            add(SoloEvent::off(p.cycle(), t, ms + BURST_TIME_MS, periodIdx));
        }
    }

    void addPeriod(const Period& p, SoloTrack t, int periodIdx, double timeMs)
    {
        switch (p.event) {
        case EVENT_OFF:
            addOff(p.cycle(), t, timeMs, periodIdx);
            break;
        case EVENT_ON:
            addOn(p, t, timeMs, periodIdx);
            break;
        case EVENT_CRESC:
            addCresc(p, t, timeMs, periodIdx);
            break;
        case EVENT_BURST:
            addBurst(p, t, timeMs, periodIdx);
            break;
        default:
            break;
        }
    }

    SoloEventCont periodEvents(int periodIdx) const
    {
        SoloEventCont res;
        res.reserve(16);

        for (auto& e : data_) {
            if (e.period() == periodIdx)
                res.push_back(e);
        }

        return res;
    }

    bool empty() const { return data_.empty(); }
    size_t size() const { return data_.size(); }

    const SoloEvent& eventAt(size_t n) const
    {
        return data_.at(n);
    }
};
}

#endif // STOK_SOLO_DATA_H
