#ifndef CEAMMC_MUSIC_THEORY_H
#define CEAMMC_MUSIC_THEORY_H

#include <boost/functional/hash.hpp>
#include <iostream>
#include <string>

#include "ceammc_music_theory_alteration.h"
#include "ceammc_music_theory_pitch_class.h"
#include "ceammc_music_theory_pitch_name.h"

namespace ceammc {
namespace music {

    enum HarmonicModus {
        MAJOR = 0,
        MINOR
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

        size_t sharps() const;
        size_t flats() const;
        size_t keys() const;

        Tonality simplify() const;

    private:
        Scale scale_;
        void calcScale();
    };

    std::ostream& operator<<(std::ostream& os, const Tonality& t);
    size_t hash_value(const Tonality& c);
}
}

#endif // CEAMMC_MUSIC_THEORY_H
