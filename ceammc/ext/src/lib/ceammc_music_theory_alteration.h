#ifndef CEAMMC_MUSIC_THEORY_ALTERATION_H
#define CEAMMC_MUSIC_THEORY_ALTERATION_H

#include <iostream>

namespace ceammc {
namespace music {

    enum AlterationType {
        ALTERATION_DOUBLE_FLAT = -2,
        ALTERATION_FLAT = -1,
        ALTERATION_NATURAL = 0,
        ALTERATION_SHARP = 1,
        ALTERATION_DOUBLE_SHARP = 2
    };

    class Alteration {
    public:
        Alteration(const Alteration& a)
            : value_(a.value_)
        {
        }

        bool operator==(const Alteration& a) const { return value_ == a.value_; }
        bool operator!=(const Alteration& a) const { return value_ != a.value_; }

        bool operator<(const Alteration& a) const { return value_ < a.value_; }
        bool operator<=(const Alteration& a) const { return value_ <= a.value_; }
        bool operator>(const Alteration& a) const { return value_ > a.value_; }
        bool operator>=(const Alteration& a) const { return value_ >= a.value_; }

        bool operator++();
        bool operator--();

        bool alterate(int n);

        const char* fullName() const;
        const char* shortName() const;

        AlterationType type() const { return AlterationType(value_); }
        int semitones() const;
        double cents() const;

    public:
        static const Alteration DOUBLE_FLAT;
        static const Alteration FLAT;
        static const Alteration NATURAL;
        static const Alteration SHARP;
        static const Alteration DOUBLE_SHARP;

    private:
        signed char value_;
        Alteration(signed char t);
    };

    std::ostream& operator<<(std::ostream& os, const Alteration& a);
}
}

#endif // CEAMMC_MUSIC_THEORY_ALTERATION_H
