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
#ifndef PARSER_TIMELINE_H
#define PARSER_TIMELINE_H

#include <cstdint>
#include <set>
#include <vector>

#include "ceammc_atomlist.h"
#include "ceammc_convert.h"
#include "ceammc_music_theory_tempo.h"
#include "ceammc_music_theory_timesig.h"

namespace ceammc {
namespace parser {

    using BarNumber = std::uint16_t;
    using BeatNumber = std::uint16_t;

    struct TimeLineTempo {
        music::Tempo tempo;
        BarNumber bar { 0 };
        BeatNumber beat { 0 };
        bool accel { false };

        bool operator<(const TimeLineTempo& t) const { return hash() < t.hash(); };
        std::int32_t hash() const { return bar * 1000 + beat; }
    };

    struct TimeLineBar {
        music::TimeSignature sig;
        std::uint16_t count { 1 };

        double durationMs(const music::Tempo& t) const { return sig.timeMs(t); }
        double beatStartMs(const music::Tempo& t, float beat) const
        {
            return std::min<float>(beat, count - 1) * (t.wholeNoteDurationMs() / sig.subDivision());
        }
    };

    struct TimeLineEventSend {
        t_symbol* target { &s_ };
        AtomList args;
    };

    struct TimeLineEventOutput {
        AtomList args;
    };

    struct TimeLineEventPreset {
        int idx { -1 };
    };

    struct TimeLineEventDef {
        t_symbol* name { &s_ };
        TimeLineEventSend send;
        TimeLineEventOutput out;
        TimeLineEventPreset preset;
    };

    struct TimeLineVarDef {
        t_symbol* name { &s_ };
        double def { 0 };
        float preset { 0 };
    };

    struct TimeLineEvent {
        double time { 0 };
        int idx { -1 };
        mutable double next_time { 0 };
        bool operator<(const TimeLineEvent& e) const { return time < e.time; };
    };

    struct TimeLine {
        double duration { 0 };
        std::vector<TimeLineBar> bars;
        std::vector<TimeLineVarDef> var_defs;
        std::vector<TimeLineEventDef> event_defs;
        std::multiset<TimeLineEvent> events;
        std::set<TimeLineTempo> tempo;

        TimeLine() { }

        void calcBarDuration(bool inf)
        {
            if (inf) {
                duration = std::numeric_limits<t_float>::max();
            } else {
                duration = 0;
                BarNumber bar_count = 0;
                for (auto& b : bars) {
                    for (BarNumber i = 0; i < b.count; i++, bar_count++) {
                        auto t = barTempo(bar_count);
                        duration += b.durationMs(t);
                    }
                }
            }
        }

        int calcNumBars() const
        {
            int res = 0;
            for (auto& b : bars)
                res += b.count;

            return res;
        }

        void dump();

        void clear()
        {
            bars.clear();
            var_defs.clear();
            events.clear();
            event_defs.clear();
            tempo.clear();
        }

        void addEventAction(t_symbol* name, const TimeLineEventSend& send)
        {
            for (auto& ev : event_defs) {
                if (ev.name == name) {
                    ev.send = send;
                    return;
                }
            }

            TimeLineEventDef ev;
            ev.name = name;
            ev.send = send;
            event_defs.push_back(ev);
        }

        void addEventAction(t_symbol* name, const TimeLineEventOutput& out)
        {
            for (auto& ev : event_defs) {
                if (ev.name == name) {
                    ev.out = out;
                    return;
                }
            }

            TimeLineEventDef ev;
            ev.name = name;
            ev.out = out;
            event_defs.push_back(ev);
        }

        void addEventAction(t_symbol* name, const TimeLineEventPreset& preset)
        {
            for (auto& ev : event_defs) {
                if (ev.name == name) {
                    ev.preset = preset;
                    return;
                }
            }
        }

        void addEvent(t_symbol* name)
        {
            for (auto& ev : event_defs) {
                if (ev.name == name)
                    return;
            }

            TimeLineEventDef ev;
            ev.name = name;
            event_defs.push_back(ev);
        }

        void addEventAt(t_symbol* name, double time, bool relativeToLast);

        int findEventByName(t_symbol* name)
        {
            auto it = std::find_if(event_defs.begin(), event_defs.end(), [name](const TimeLineEventDef& a) { return a.name == name; });
            if (it == event_defs.end())
                return -1;
            else
                return std::distance(event_defs.begin(), it);
        }

        std::vector<double> findEventTime(t_symbol* name)
        {
            std::vector<double> res;

            for (auto& ev : events) {
                if (ev.idx < 0)
                    continue;

                if (ev.idx < event_defs.size()) {
                    if (event_defs[ev.idx].name == name)
                        res.push_back(ev.time);
                }
            }

            return res;
        }

        double findBarTime(int bar, int beat, float beatFraq = 0);

        const TimeLineEvent& eventByIdx(int i) const
        {
            auto it = events.begin();
            std::advance(it, i);
            return *it;
        }

        bool setBarBpm(BarNumber bar, BeatNumber beat, const music::Tempo& t, bool accel = false)
        {
            TimeLineTempo x;
            x.tempo = t;
            x.bar = bar;
            x.beat = beat;
            x.accel = accel;
            tempo.insert(x);
            return true;
        }

        music::Tempo barTempo(BarNumber bar) const
        {
            if (tempo.empty())
                return {};

            for (auto it = tempo.rbegin(); it != tempo.rend(); ++it) {
                if (it->bar <= bar) {
                    if (it->accel) {
                        auto next = std::prev(it);
                        if (next != tempo.rend()) {
                            auto dur = barDuration(bar, 1);
                            auto k = convert::lin2lin<float>(bar, it->bar, next->bar, 0, 1);
                            return music::Tempo::intepolate(it->tempo, next->tempo, k);
                        } else {
                            return it->tempo;
                        }
                    } else {
                        return it->tempo;
                    }
                }
            }

            return {};
        }

        void calcNextTimes()
        {
            double prev = 0;
            for (auto it = events.begin(); it != events.end(); ++it) {
                auto next = std::next(it);
                if (next == events.end())
                    it->next_time = 0;
                else
                    it->next_time = (next->time - it->time);
            }
        }

        music::Duration barDuration(BarNumber bar, size_t len) const
        {
            music::Duration res(0, 4);
            BarNumber bar_count = 0;
            for (auto& b : bars) {
                for (BarNumber i = 0; i < b.count && len > 0; i++, bar_count++) {
                    if (bar_count < bar)
                        continue;

                    res += b.sig.duration();
                    len--;
                }
            }

            return res;
        }

        music::Duration tempoSegmentDuration(std::uint16_t idx) const
        {
            music::Duration res(0, 4);
            if (idx >= tempo.size())
                return res;

            auto it = std::next(tempo.begin(), idx);
            auto it2 = std::next(it);

            if (it2 == tempo.end())
                return barDuration(it->bar, std::numeric_limits<int>::max());
            else
                return barDuration(it->bar, it2->bar - it->bar);
        }
    };

    bool parse_timelime(const char* str, TimeLine& tl);

}
}

#endif // PARSER_TIMELINE_H
