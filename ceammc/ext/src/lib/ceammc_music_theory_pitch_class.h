#ifndef CEAMMC_MUSIC_THEORY_PITCH_CLASS_H
#define CEAMMC_MUSIC_THEORY_PITCH_CLASS_H

#include "ceammc_music_theory_alteration.h"
#include "ceammc_music_theory_pitch_name.h"

#include <cstddef>
#include <iostream>
#include <vector>

namespace ceammc {
namespace music {

    class PitchClass;
    typedef std::vector<PitchClass> Enharmonics;

    class PitchClass {
        PitchName pitch_name_;
        Alteration alt_;
        bool invalid_;

    public:
        PitchClass(PitchName p, Alteration a = Alteration(Alteration::NATURAL));

        PitchName pitchName() const { return pitch_name_; }
        void setPitchName(PitchName p) { pitch_name_ = p; }
        Alteration alteration() const { return alt_; }
        void setAlteration(Alteration a) { alt_ = a; }

        bool operator==(const PitchClass& c) const { return pitch_name_ == c.pitch_name_ && alt_ == c.alt_; }
        bool operator!=(const PitchClass& c) const { return !this->operator==(c); }

        bool enharmonicEqual(const PitchClass& c) const { return absolutePitch() == c.absolutePitch(); }
        size_t absolutePitch() const;

        operator bool() const;

        std::string name() const;

        PitchClass simplifyFull() const;
        PitchClass simplifyDouble() const;

        /**
         * Moves pitch class tone up
         * @return invalid pitch on error - test with operator bool()
         * @see semitoneUp()
         */
        PitchClass toneUp() const;

        /**
         * Moves pitch class semitone up
         * @return invalid pitch on error - test with operator bool()
         * @see toneUp()
         */
        PitchClass semitoneUp() const;
        PitchClass stepTranspose(int n) const;

        /**
         * Returns list of upper pitch class enharmonics sorted by PitchName
         * @see upperEnharmonics()
         */
        Enharmonics upperEnharmonics() const;

        /**
         * Returns list of lower pitch class enharmonics sorted by PitchName
         * @see upperEnharmonics()
         */
        Enharmonics lowerEnharmonics() const;
        Enharmonics enharmonic() const;

    public:
        static bool tryAlterateToEqPitch(PitchClass& target, const PitchClass& pattern);
        static size_t minSemitoneDistance(const PitchClass& c1, const PitchClass& c2);
        static int minSemitonesFromTo(const PitchClass& c1, const PitchClass& c2);

    public:
        static const PitchClass Cff;
        static const PitchClass Cf;
        static const PitchClass C;
        static const PitchClass Dff;
        static const PitchClass Cs;
        static const PitchClass Df;
        static const PitchClass Css;
        static const PitchClass D;
        static const PitchClass Eff;
        static const PitchClass Ds;
        static const PitchClass Ef;
        static const PitchClass Fff;
        static const PitchClass Dss;
        static const PitchClass E;
        static const PitchClass Ff;
        static const PitchClass Es;
        static const PitchClass F;
        static const PitchClass Gff;
        static const PitchClass Ess;
        static const PitchClass Fs;
        static const PitchClass Gf;
        static const PitchClass Fss;
        static const PitchClass G;
        static const PitchClass Aff;
        static const PitchClass Gs;
        static const PitchClass Af;
        static const PitchClass Gss;
        static const PitchClass A;
        static const PitchClass Bff;
        static const PitchClass As;
        static const PitchClass Bf;
        static const PitchClass Ass;
        static const PitchClass B;
        static const PitchClass Bs;
        static const PitchClass Bss;
    };

    std::ostream& operator<<(std::ostream& os, const PitchClass& p);
}
}

#endif // CEAMMC_MUSIC_THEORY_PITCH_CLASS_H
