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
#ifndef CEAMMC_MUSIC_SPN_H
#define CEAMMC_MUSIC_SPN_H

#include "ceammc_convert.h"
#include "ceammc_music_theory_pitch_class.h"

namespace ceammc {
namespace music {

    enum OctaveType : int8_t {
        OCTAVE_ABS = 0,
        OCTAVE_REL
    };

    constexpr int8_t ABS_OCTAVE_MIN = -1;
    constexpr int8_t ABS_OCTAVE_MAX = 10;
    constexpr int8_t ABS_OCTAVE_DEF = 4;
    constexpr int8_t REL_OCTAVE_MIN = -10;
    constexpr int8_t REL_OCTAVE_MAX = 10;

    struct InvalidMidi { };
    using EitherMidi = Either<std::uint8_t, InvalidMidi>;

    class Octave {
        int8_t oct_;
        OctaveType type_;

    public:
        explicit Octave(int8_t oct = ABS_OCTAVE_DEF, OctaveType type = OCTAVE_ABS)
            : type_(type)
        {
            if (isAbs())
                setAbsOctave(oct);
            else
                setRelOctave(oct);
        }

        bool operator==(const Octave& o) const
        {
            return o.oct_ == oct_ && o.type_ == type_;
        }

        /**
         * set absolute octave
         */
        void setAbsOctave(int8_t oct)
        {
            oct_ = clip<int8_t, ABS_OCTAVE_MIN, ABS_OCTAVE_MAX>(oct);
            type_ = OCTAVE_ABS;
        }

        /**
         * set octave based on previous absolute octave
         */
        void setAbsOctave(const Octave& prev)
        {
            if (prev.isAbs() && !isAbs())
                setAbsOctave(prev.oct_ + oct_);
        }

        /**
         * set relative octave
         */
        void setRelOctave(int8_t oct)
        {
            oct_ = clip<int8_t, REL_OCTAVE_MIN, REL_OCTAVE_MAX>(oct);
            type_ = OCTAVE_REL;
        }

        std::int8_t octave() const { return oct_; }
        OctaveType type() const { return type_; }

        /**
         * check if octave is absolute
         */
        bool isAbs() const { return type_ == OCTAVE_ABS; }

        /**
         * check if octave is relative
         */
        bool isRelative() const { return type_ == OCTAVE_REL; }

        /**
         * convert absolute octave to midi pitch
         * @return InvalidMidi if octave is relative
         */
        EitherMidi toMidi() const
        {
            if (type_ == OCTAVE_ABS)
                return (oct_ + 1) * 12; // C4 == 60
            else
                return InvalidMidi {};
        }

        EitherMidi toMidi(std::uint8_t relative_oct) const
        {
            if (type_ == OCTAVE_ABS)
                return (oct_ + 1) * 12;
            else {
                auto oct = relative_oct + oct_ + 1;
                if (oct < 0 || oct > ABS_OCTAVE_MAX)
                    return InvalidMidi {};
                else
                    return oct * 12;
            }
        }

        std::string toString() const;
    };

    std::ostream& operator<<(std::ostream& os, const Octave& oct);

    class Spn {
        PitchClass pitch_;
        Octave oct_;
        int8_t dev_ { 0 };
        bool is_rest_ { false };

    public:
        Spn()
            : pitch_(PitchName::C, Alteration::NATURAL)
            , is_rest_(true)
        {
        }

        /**
         * create SPN from MIDI pitch
         */
        explicit Spn(std::uint8_t midi)
            : pitch_(midi % 12)
            , oct_((midi / 12) - 1)
        {
        }

        Spn(PitchClass pitch, Octave oct = Octave {})
            : pitch_(pitch)
            , oct_(oct)
        {
        }

        Octave& octave() { return oct_; }
        const Octave& octave() const { return oct_; }

        const PitchClass& pitch() const { return pitch_; }
        PitchClass& pitch() { return pitch_; }
        std::int8_t deviation() const { return dev_; }

        void setPitch(const PitchClass& p) { pitch_ = p; }
        void setOctave(const Octave& oct) { oct_ = oct; }
        void setDeviation(std::int8_t dev) { dev_ = dev; }

        bool isAbs() const { return oct_.isAbs(); }
        bool isRelative() const { return oct_.isRelative(); }

        void setMidi(std::uint8_t p);

        bool operator==(const Spn& spn) const;
        bool operator!=(const Spn& spn) const { return !operator==(spn); }

        EitherMidi toMidi(std::int8_t rel_octave = 4) const;
        t_float asMidi(std::int8_t rel_octave = 4) const;

        bool isRest() const { return is_rest_; }

        std::string toString() const;

        static int8_t nextIntervalOctave(int8_t notePrev, int8_t noteNext);

        static Spn makeRest();

        template <typename It>
        static void setAbsOctaves(It begin, It end, std::int8_t rel_octave = 4)
        {
            if (begin == end)
                return;

            if (!begin->isAbs())
                begin->setAbsOctave(rel_octave);

            for (It a = begin, b = begin + 1; b != end; ++a, ++b) {
                b->octave().setAbsOctave(a->octave());
            }
        }
    };

    std::ostream& operator<<(std::ostream& os, const Spn& spn);

}
}

#endif // CEAMMC_MUSIC_SPN_H
