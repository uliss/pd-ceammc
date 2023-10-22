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
#ifndef CEAMMC_PARSER_MUSIC_H
#define CEAMMC_PARSER_MUSIC_H

#include <array>
#include <cstddef>
#include <cstdint>

#include "ceammc_music_chord.h"
#include "ceammc_music_spn.h"
#include "ceammc_music_theory_tempo.h"
#include "ceammc_music_theory_timesig.h"

namespace ceammc {
namespace parser {

    //    struct Bpm {
    //        float bpm { 0 };
    //        int beat_num { 1 };
    //        int beat_div { 4 };

    //        Bpm() { }

    //        Bpm(float b, int beatNum, int beatDiv)
    //            : bpm(b)
    //            , beat_num(beatNum)
    //            , beat_div(beatDiv)
    //        {
    //        }


    //        Bpm(const fsm::BpmData& d)
    //            : bpm(d.ival + double(d.fnum) / double(d.fden))
    //            , beat_num(d.dur_num)
    //            , beat_div(d.dur_den)
    //        {
    //        }

    //        bool operator==(const Bpm& b) const
    //        {
    //            return bpm == b.bpm
    //                && beat_num == b.beat_num
    //                && beat_div == b.beat_div;
    //        }

    //        bool operator!=(const Bpm& b) const
    //        {
    //            return !operator==(b);
    //        }

    //        t_float value() const { return bpm; }
    //        t_float freqHz() const { return value() / 60.0; }
    //        t_float ratio() const { return beat_div > 0 ? (t_float(beat_num) / beat_div) : 0; }

    //        t_float beatPeriodMs(t_float def = 0) const
    //        {
    //            return (bpm > 0) ? (4 * beat_num * 60000.0) / (bpm * beat_div) : def;
    //        }

    //        t_float beatPeriodSamp(t_float sr, t_float def = 0) const
    //        {
    //            return (bpm > 0) ? sr * 60.0 / value() : def;
    //        }

    //        t_float wholePeriodMs(t_float def = 0) const
    //        {
    //            if (bpm > 0) {
    //                return (60000.0 * beat_div) / (bpm * beat_num);
    //            } else
    //                return def;
    //        }
    //    };

    //    template <size_t N>
    //    using SmallBpmVecN = boost::container::small_vector<Bpm, N>;
    //    using SmallBpmVec = SmallBpmVecN<8>;

    //    class BpmFullMatch {
    //    public:
    //        bool parse(const char* str, Bpm& bpm);
    //        bool parse(const Atom& a, Bpm& bpm);
    //        size_t parse(const AtomListView& lv, SmallBpmVec& out);

    //        template <size_t N>
    //        std::array<Bpm, N> parseN(const AtomListView& lv, const Bpm& def = {})
    //        {
    //            std::array<Bpm, N> res;

    //            const auto M = lv.size();
    //            for (size_t i = 0; i < N; i++) {
    //                if (i < M && parse(lv[i], res[i]))
    //                    continue;
    //                else
    //                    res[i] = def;
    //            }

    //            return res;
    //        }
    //    };

    bool parse_tempo(const char* str, music::Tempo& t);
    bool parse_tempo(const Atom& a, music::Tempo& t);

    template <typename Container>
    int parse_tempo(const AtomListView& lv, Container& out)
    {
        music::Tempo t { 60, 4 };
        int res = 0;

        for (auto& a : lv) {
            if (parse_tempo(a, t)) {
                out.push_back(t);
                res++;
            } else
                break;
        }

        return res;
    }

    template <size_t N>
    using TempoArray = std::array<music::Tempo, N>;

    template <size_t N>
    TempoArray<N> parse_tempo_n(const AtomListView& lv, const music::Tempo& def)
    {
        TempoArray<N> res;

        const auto M = lv.size();
        for (size_t i = 0; i < N; i++) {
            if (i < M && parse_tempo(lv[i], res[i]))
                continue;
            else
                res[i] = def;
        }

        return res;
    }

    /**
     * Parse SPN notation: C4, Eb2, A##
     * Also cent deviations are supported: C#5(+25c) - 25 cents up, Gb4(-23c) - 23 cents lower
     * C#^^ - relative octave (two octaves up)
     */
    bool parse_spn(const char* str, music::Spn& res);
    bool parse_spn(const Atom& a, music::Spn& res);

    template <typename Container>
    bool parse_spn(const AtomListView& lv, Container& out)
    {
        music::Spn spn;

        for (auto& a : lv) {
            if (parse_spn(a, spn))
                out.push_back(spn);
        }

        return true;
    }

    template <size_t N>
    using SpnArray = std::array<music::Spn, N>;

    template <size_t N>
    SpnArray<N> parse_spn_n(const AtomListView& lv, const music::Spn& def = music::Spn::makeRest())
    {
        SpnArray<N> res;
        music::Spn spn;

        const auto M = lv.size();
        for (size_t i = 0; i < N; i++) {
            if (i < M && parse_spn(lv[i], spn))
                res[i] = spn;
            else
                res[i] = def;
        }

        return res;
    }

    /**
     * Parse PitchClass in SPN notation (without octave), like C##, Ebb
     */
    bool parse_pitch_class(const char* str, music::PitchClass& res);
    bool parse_pitch_class(const Atom& a, music::PitchClass& res);

    /**
     * Parse notation.
     * @example R4, C#4, Eb2(-35c), C#5|1/4..
     */
    bool parse_notation(const char* str, music::Notation& note);
    bool parse_notation(const Atom& a, music::Notation& note);

    template <typename Container>
    bool parse_notation(const AtomListView& lv, Container& out)
    {
        music::Notation n;

        for (auto& a : lv) {
            if (parse_notation(a, n)) {
                out.push_back(n);
                if (n.numRepeats() > 1) {
                    for (int i = 1; i < n.numRepeats(); i++) {
                        out.push_back(n);
                        out.back().setNumRepeats(1);
                    }
                }
            } else
                return false;
        }

        return true;
    }

    bool parse_duration(const char* str, music::Duration& dur);
    bool parse_duration(const Atom& a, music::Duration& dur);

    template <typename Container>
    int parse_duration(const AtomListView& lv, Container& out)
    {
        music::Duration dur;
        int res = 0;

        for (auto& a : lv) {
            if (parse_duration(a, dur)) {
                out.push_back(dur);
                if (dur.numRepeats() > 1) {
                    for (int i = 1; i < dur.numRepeats(); i++) {
                        out.push_back(dur);
                        out.back().setNumRepeats(1);
                    }
                }
                res++;
            }
        }

        return res;
    }

    bool parse_time_signature(const char* str, music::TimeSignature& ts);

    bool parse_chord_class(const char* str, music::ChordClass& res);
}
}

#endif // CEAMMC_PARSER_MUSIC_H
