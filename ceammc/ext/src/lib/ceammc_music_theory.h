#ifndef CEAMMC_MUSIC_THEORY_H
#define CEAMMC_MUSIC_THEORY_H

#include <iostream>
#include <string>

#include "ceammc_music_theory_alteration.h"
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

    typedef std::vector<PitchClass> Scale;

    class Tonality {
        PitchClass pitch_;
        HarmonicModus modus_;

    public:
        Tonality(const PitchClass& p, HarmonicModus m);

        PitchClass pitch() const { return pitch_; }
        void setPitch(PitchClass name) { pitch_ = name; }
        HarmonicModus modus() const { return modus_; }
        void setModus(HarmonicModus m) { modus_ = m; }

        bool operator==(const Tonality& t) const { return pitch_ == t.pitch_ && modus_ == t.modus_; }
        bool operator!=(const Tonality& t) const { return !this->operator==(t); }
        bool enharmonicEqual(const Tonality& t) const;

        std::string name() const;

        const Scale& scale() const;
        const Scale& alterations(AlterationDir dir = ALTERATE_UP) const;
        const Scale& chromatic(AlterationDir dir = ALTERATE_UP) const;

        size_t numSharps() const;
        size_t numFlats() const;
        size_t numKeys() const;

        Tonality simplify() const;

    public:
        static int fifthsCircleIndex(const PitchClass& c, mode_t m);
        static PitchClass correctAlteration(size_t pitch,
            const Tonality& t,
            AlterationDir dir = ALTERATE_UP);

    private:
        Scale scale_;
        Scale chrom_up_;
        Scale chrom_down_;
        Scale alt_up_;
        Scale alt_down_;

        void calcScale();
    };

    std::ostream& operator<<(std::ostream& os, const Tonality& t);
}
}

#endif // CEAMMC_MUSIC_THEORY_H
