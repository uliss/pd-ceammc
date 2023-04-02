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
#include <random>
#include <vector>

namespace solo {

constexpr int NUM_TRACKS = 6;
constexpr int NUM_CYCLES = 6;

enum {
    EVENT_VALUE_NOPERF = 200,
    EVENT_VALUE_CRESC,
    EVENT_VALUE_DIM,
    EVENT_VALUE_DIM_CRESC,
};

using namespace ceammc;

enum EventType : std::int8_t {
    EVENT_OFF,
    EVENT_ON,
    EVENT_CRESC,
    EVENT_DIM,
    EVENT_DIM_CRESC,
};

enum SoloEventType : std::int8_t {
    SOLO_EVENT_OFF,
    SOLO_EVENT_ON
};

enum SoloEventTrack : std::int8_t {
    EVENT_TRACK_MIC1 = 0,
    EVENT_TRACK_MIC2,
    EVENT_TRACK_FB1,
    EVENT_TRACK_FB2,
    EVENT_TRACK_OUT1,
    EVENT_TRACK_OUT2,
};

enum SoloEventCycle : std::int8_t {
    EVENT_CYCLE_A = 0,
    EVENT_CYCLE_B,
    EVENT_CYCLE_C,
    EVENT_CYCLE_D,
    EVENT_CYCLE_E,
    EVENT_CYCLE_F,
};

struct Period {
    float abs_length { 0 }, rel_pos { 0 }, rel_length { 0 }, from { 0 }, to { 0 };
    EventType event { EVENT_OFF };
    const SoloEventCycle cycle_ { EVENT_CYCLE_A };
    std::uint8_t nperf { 0 };

    Period(SoloEventCycle cycle, EventType type, float length)
        : cycle_(cycle)
        , event(type)
        , abs_length(length)
    {
    }

    SoloEventCycle cycle() const { return cycle_; }

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

    void setRelOffset(float attack, float length)
    {
        rel_pos = attack;
        rel_length = length;
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
    solo::SoloEventTrack track;
};

struct Scheme {
    std::array<PeriodTrack, NUM_TRACKS> tracks_;
    std::array<CycleInfo, NUM_CYCLES> cycles_;
    int scheme_ { 0 };

    explicit Scheme(size_t n = 0)
    {
        setScheme(n);
    }

    /**
     * return current scheme
     */
    int scheme() const { return scheme_; }

    const std::array<PeriodTrack, NUM_TRACKS>& tracks() const { return tracks_; }
    const std::array<CycleInfo, NUM_TRACKS>& cycles() const { return cycles_; }

    /**
     * return number of cycles in the scheme (always should return 6)
     */
    size_t cycleCount() const { return cycles_.size(); }

    /**
     * return period index for the begining of cycle
     */
    size_t cycleBeginPeriodIndex(SoloEventCycle c) const
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
    size_t cycleEndPeriodIndex(SoloEventCycle c) const
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

    float schemeLength() const
    {
        float res = 0;
        for (auto& ci : cycles_)
            res += ci.cycleLength();

        return res;
    }

    void clear()
    {
        for (auto& t : tracks_)
            t.clear();
    }

    void addTrackCycleEvents(PeriodTrack& track, float cycleOffset, SoloEventCycle cycle, const std::initializer_list<int>& init)
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
        addTrackCycleEvents(track, offset, EVENT_CYCLE_A, a);
        offset += cycles_[0].cycleLength();
        addTrackCycleEvents(track, offset, EVENT_CYCLE_B, b);
        offset += cycles_[1].cycleLength();
        addTrackCycleEvents(track, offset, EVENT_CYCLE_C, c);
        offset += cycles_[2].cycleLength();
        addTrackCycleEvents(track, offset, EVENT_CYCLE_D, d);
        offset += cycles_[3].cycleLength();
        addTrackCycleEvents(track, offset, EVENT_CYCLE_E, e);
        offset += cycles_[4].cycleLength();
        addTrackCycleEvents(track, offset, EVENT_CYCLE_F, f);
    }

    void addTrackEvents(int track,
        const std::initializer_list<int>& a, const std::initializer_list<int>& b,
        const std::initializer_list<int>& c, const std::initializer_list<int>& d,
        const std::initializer_list<int>& e, const std::initializer_list<int>& f)
    {
        if (track >= 0 && track < NUM_TRACKS) {
            tracks_[track].track = static_cast<SoloEventTrack>(track);
            addTrackEvents(tracks_[track], a, b, c, d, e, f);
        } else
            LIB_ERR << "invalid track index: " << track;
    }

    std::string toString(size_t n) const
    {
        std::string res;
        if (n >= NUM_TRACKS)
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

        addTrackEvents(EVENT_TRACK_MIC1,
            { 0, 3, 2, 0, 2, 3, 0, 2, 0 },
            { 4, 1, 3, 1, 2, 0, 1 },
            { 2, 0, 1, 0, 1, 1, 0, 2, 0, E, 0 },
            { 0, 2, 1, 0, 1, 1, 0, 2, 1, 0 },
            { 3, 0, 0, 0, 0, 8 },
            { 0, 0, 3, 0, 0, 0, 6, 0 });

        addTrackEvents(EVENT_TRACK_MIC2,
            { 2, 0, 0, 3, 0, 0, 2, 0, 2 },
            { 1, 4, 1, 2, 1, 3, 0 },
            { 0, 1, 0, 2, 0, 0, 1, 0, 2, 0, E },
            { 2, 0, 0, 1, 0, 0, 0, 0, 0, E },
            { 0, 5, E, E, 3, 0 },
            { 4, 0, 0, 0, 5, 0, 0, 0 });

        addTrackEvents(EVENT_TRACK_FB1,
            { 0, 0, 2, 0, 0, 3, 0, 0, 2 },
            { 0, 2, 1, 1, 1, 1, E },
            { 0, 1, 1, 1, 1, 1, 0, 0, 1, E, E },
            { 0, 0, 1, E, 1, 2, 0, 0, 1, E },
            { 0, 1, 2, 2, 1, 1 },
            { 1, 1, 0, 2, 0, 0, 0, 0 });

        addTrackEvents(EVENT_TRACK_FB2,
            { 0, 2, 0, 0, 2, 0, 0, 2, 1 },
            { 0, 1, 1, 1, 1, 1, E },
            { 0, 0, 1, 1, 1, 1, 1, 0, 0, E, E },
            { 0, 1, E, 1, E, 1, 1, E, 1, E },
            { 0, 0, E, E, 3, E },
            { 0, 2, 0, 0, 0, 3, 0, 0 });

        addTrackEvents(EVENT_TRACK_OUT1,
            { 0, 0, 1, 1, 0, 1, 1, 0, 1 },
            { C, 1, 0, 1, 0, 1, 0 },
            { E, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1 },
            { 0, 0, 1, 1, 0, 1, 1, 0, 1, 1 },
            { 0, 1, 1, 1, 1, 1 },
            { 0, 0, 0, 1, 1, 0, 0, 1 });

        periodAt(EVENT_TRACK_OUT1, EVENT_CYCLE_C, 0)->setRelOffset(0.25, 0.25);

        addTrackEvents(EVENT_TRACK_OUT2,
            { 0, 1, 1, 0, 1, 1, 0, 1, 1 },
            { C, 0, 1, 0, 1, 0, 1 },
            { E, 0, 1, 1, 1, 1, 0, 1, 0, 1, 1 },
            { 0, 1, 1, 0, 1, 0, 1, 1, 0, 1 },
            { 0, 0, 1, 0, 0, 1 },
            { 0, 1, 1, 0, 0, 1, 1, 0 });

        periodAt(EVENT_TRACK_OUT2, EVENT_CYCLE_C, 0)->setRelOffset(0.5, 0.25);
    }

    Period* periodAt(SoloEventTrack track, SoloEventCycle cycle, size_t periodIdx)
    {
        auto& t = tracks_.at(track);
        auto idx = cycleBeginPeriodIndex(cycle) + periodIdx;
        return (idx < t.size()) ? &t.at(idx) : nullptr;
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

private:
    const int E = EVENT_VALUE_NOPERF;
    const int C = EVENT_VALUE_CRESC;
    const int D = EVENT_VALUE_DIM;
    const int X = EVENT_VALUE_DIM_CRESC;
};

class SoloEvent {
    double abs_time_msec_ { 0 };
    float value_ { 0 };
    SoloEventType type_ { SOLO_EVENT_OFF };
    SoloEventTrack track_ { EVENT_TRACK_MIC1 };
    SoloEventCycle cycle_ { EVENT_CYCLE_A };
    int period_ { 0 };

public:
    SoloEvent(SoloEventCycle cycle, SoloEventTrack part, SoloEventType type, double time_msec)
        : abs_time_msec_(time_msec)
        , type_(type)
        , track_(part)
        , cycle_(cycle)
    {
    }

    int period() const { return period_; }
    SoloEventCycle cycle() const { return cycle_; }
    SoloEventType type() const { return type_; }
    SoloEventTrack track() const { return track_; }
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

        return std::string(TRACKS[track_]) + ':' + TYPES[type_];
    }

    static SoloEvent off(SoloEventCycle cycle, SoloEventTrack part, double time_msec, int period)
    {
        return SoloEvent(cycle, part, SOLO_EVENT_OFF, time_msec).setValue(0).setPeriod(period);
    }

    static SoloEvent on(SoloEventCycle cycle, SoloEventTrack part, double time_msec, int period)
    {
        return SoloEvent(cycle, part, SOLO_EVENT_ON, time_msec).setValue(1).setPeriod(period);
    }
};

class SoloEventList {
    std::vector<SoloEvent> data_;
    long current_ { -1 };

public:
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

    const std::vector<SoloEvent>& data() const { return data_; }

    bool isValidCurrent() const { return current_ >= 0 && current_ < data_.size(); }
    bool isValidNext() const { return current_ >= 0 && (current_ + 1) < data_.size(); }

    SoloEvent* currentPtr() { return isValidCurrent() ? &data_[current_] : nullptr; }
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

    bool empty() const { return data_.empty(); }
};
}

#endif // STOK_SOLO_DATA_H
