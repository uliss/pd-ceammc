#ifndef CEAMMC_MUSIC_THEORY_H
#define CEAMMC_MUSIC_THEORY_H

#include <iostream>
#include <string>

#include "ceammc_music_theory_alteration.h"
#include "ceammc_music_theory_keyboard.h"
#include "ceammc_music_theory_names.h"
#include "ceammc_music_theory_pitch_class.h"
#include "ceammc_music_theory_pitch_name.h"

namespace ceammc {
namespace music {

    enum HarmonicModus {
        MAJOR = 0,
        MINOR
    };

    enum AlterationDir {
        ALTERATE_UP = 0,
        ALTERATE_DOWN
    };

    using PitchList = std::vector<PitchClass>;

    class Tonality {
        PitchClass pitch_;
        HarmonicModus modus_;

    public:
        Tonality(const PitchClass& p, HarmonicModus m);

        PitchClass pitch() const { return pitch_; }
        void setPitch(PitchClass name);
        HarmonicModus modus() const { return modus_; }
        void setModus(HarmonicModus m);

        bool operator==(const Tonality& t) const { return pitch_ == t.pitch_ && modus_ == t.modus_; }
        bool operator!=(const Tonality& t) const { return !this->operator==(t); }
        bool enharmonicEqual(const Tonality& t) const;

        std::string name() const;

        const PitchList& scale() const;
        const PitchList& alterations(AlterationDir dir = ALTERATE_UP) const;
        const PitchList& chromatic(AlterationDir dir = ALTERATE_UP) const;

        size_t numSharps() const;
        size_t numFlats() const;
        size_t numKeys() const;

        Tonality simplify() const;

    public:
        static int fifthsCircleIndex(const PitchClass& c, HarmonicModus m);
        static PitchClass correctAlteration(size_t pitch,
            const Tonality& t,
            AlterationDir dir = ALTERATE_UP);

    private:
        PitchList scale_;
        PitchList chrom_up_;
        PitchList chrom_down_;
        PitchList alt_up_;
        PitchList alt_down_;

        void calcScale();
    };

    std::ostream& operator<<(std::ostream& os, const Tonality& t);
}
}

#endif // CEAMMC_MUSIC_THEORY_H
