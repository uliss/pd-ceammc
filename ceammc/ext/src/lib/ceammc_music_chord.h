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
#ifndef CEAMMC_MUSIC_CHORD_H
#define CEAMMC_MUSIC_CHORD_H

#include "ceammc_music_scale.h"
#include "ceammc_music_spn.h"
#include "ceammc_music_theory_duration.h"

namespace ceammc {

// clang-format off
t_float operator ""_midi(const char* str, size_t n);
// clang-format on

namespace music {

    constexpr auto CHORD_MAX_PITCH_NUM = 7;

    struct Notation {
        music::Duration dur_;
        music::Spn spn_;

    public:
        Notation() { }

        Notation(const music::Spn& spn, const music::Duration& dur)
            : spn_(spn)
            , dur_(dur)
        {
        }

        bool isRest() const { return spn_.isRest(); }
        bool isAbsPitch() const { return spn_.isAbs(); }
        bool isAbsDuration() const { return dur_.type() == DURATION_ABS; }

        Octave& octave() { return spn_.octave(); }
        const Octave& octave() const { return spn_.octave(); }
        void setOctave(const Octave& oct) { spn_.setOctave(oct); }

        const music::Spn& spn() const { return spn_; }
        const music::Duration& duration() const { return dur_; }

        double timeMs(const Tempo& t) const { return dur_.timeMs(t); }

        std::uint16_t numRepeats() const { return dur_.numRepeats(); }
        void setNumRepeats(std::uint16_t n) { dur_.setNumRepeats(n); }

        template <typename It>
        static void setAbsOctaves(It begin, It end, std::int8_t rel_octave = 4)
        {
            if (begin == end)
                return;

            if (!begin->isAbsPitch())
                begin->setOctave(Octave(rel_octave, OCTAVE_ABS));

            for (It a = begin, b = begin + 1; b != end; ++a, ++b) {
                b->octave().setAbsOctave(a->octave());
            }
        }

        t_float asMidi(std::int8_t rel_octave = 4) const
        {
            return spn_.asMidi(rel_octave);
        }
    };

    std::ostream& operator<<(std::ostream& os, const Notation& n);

    class ChordType {
        DegreeType pitches_[CHORD_MAX_PITCH_NUM + 1] = { 0 };

    public:
        ChordType(DegreeInitList init);

        bool contains(DegreeType degree) const;

        size_t size() const;
        bool empty() const;

        void setFrom(const DegreeType* begin, const DegreeType* end);

        DegreeType* begin();
        const DegreeType* begin() const;
        DegreeType* end() { return begin() + size(); }
        const DegreeType* end() const { return begin() + size(); }
        bool operator==(const DegreeList& v) const { return DegreeList(begin(), end()) == v; }
    };

    class ChordClass {
        ChordType type_;
        Spn base_spn_;

    public:
        explicit ChordClass(const Spn& basePitch = {}, const ChordType& type = { 0, 4, 7 })
            : base_spn_(basePitch)
            , type_(type)
        {
        }

        const Spn& basePitch() const { return base_spn_; }
        const ChordType& type() const { return type_; }
    };

    class Chord {
        std::vector<Notation> pitches_;
    };
}
}

#endif // CEAMMC_MUSIC_CHORD_H
