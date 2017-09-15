#ifndef CEAMMC_MUSIC_THEORY_PITCH_NAME_H
#define CEAMMC_MUSIC_THEORY_PITCH_NAME_H

#include <cstddef>
#include <iostream>

namespace ceammc {
namespace music {

    class PitchName {
        unsigned char value_;
        PitchName(unsigned char v);

    public:
        PitchName(const PitchName& p);

        bool operator==(const PitchName& p) const { return value_ == p.value_; }
        bool operator!=(const PitchName& p) const { return value_ != p.value_; }

        PitchName operator+(int i) const;
        PitchName operator-(int i) const;

        unsigned int index() const;
        unsigned int absolutePitch() const;

    public:
        static const PitchName C;
        static const PitchName D;
        static const PitchName E;
        static const PitchName F;
        static const PitchName G;
        static const PitchName A;
        static const PitchName B;

    public:
        static size_t distance(const PitchName& p1, const PitchName& p2);
        static size_t minDistance(const PitchName& p1, const PitchName& p2);

        static size_t upSteps(const PitchName& from, const PitchName& to);
        static size_t downSteps(const PitchName& from, const PitchName& to);
        static int minSteps(const PitchName& from, const PitchName& to);

    private:
        friend std::ostream& operator<<(std::ostream& os, const PitchName& p);
        friend std::string to_string(const PitchName& p);
        static const char* pitch_names_[7];
    };

    std::ostream& operator<<(std::ostream& os, const PitchName& p);
    std::string to_string(const PitchName& p);
}
}
#endif // CEAMMC_MUSIC_THEORY_PITCH_NAME_H
