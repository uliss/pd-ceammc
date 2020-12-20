#ifndef CEAMMC_MUSIC_THEORY_PITCH_CLASS_H
#define CEAMMC_MUSIC_THEORY_PITCH_CLASS_H

#include <array>
#include <cstddef>
#include <iostream>
#include <vector>

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
        Alteration(const Alteration& a);
        Alteration& operator=(const Alteration& a);

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

        AlterationType type() const;
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

    enum PitchNameType {
        PITCH_NAME_C = 0,
        PITCH_NAME_D,
        PITCH_NAME_E,
        PITCH_NAME_F,
        PITCH_NAME_G,
        PITCH_NAME_A,
        PITCH_NAME_B
    };

    class PitchName {
        unsigned char value_;
        PitchName(unsigned char v);

    public:
        PitchName(const PitchName& p);
        PitchName& operator=(const PitchName& p);

        bool operator==(const PitchName& p) const { return value_ == p.value_; }
        bool operator!=(const PitchName& p) const { return value_ != p.value_; }

        PitchName operator+(int i) const;
        PitchName operator-(int i) const;

        PitchNameType type() const;

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
        static const char* pitch_names_[7];
    };

    class PitchClass;
    typedef std::vector<PitchClass> Enharmonics;

    class PitchClass {
        PitchName pitch_name_;
        Alteration alt_;
        bool invalid_;

    public:
        explicit PitchClass(size_t semitoneValue);
        PitchClass(PitchName p, Alteration a);

        PitchName pitchName() const { return pitch_name_; }
        void setPitchName(PitchName p) { pitch_name_ = p; }
        Alteration alteration() const { return alt_; }
        void setAlteration(Alteration a) { alt_ = a; }

        bool operator==(const PitchClass& c) const;
        bool operator!=(const PitchClass& c) const;

        bool enharmonicEqual(const PitchClass& c) const { return absolutePitch() == c.absolutePitch(); }

        /** return pitch value in [0..11] range */
        size_t absolutePitch() const;

        operator bool() const;

        PitchClass simplifyFull() const;
        PitchClass simplifyDouble() const;

        PitchClass alterate(int n) const;

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
        Enharmonics enharmonics() const;

    public:
        static bool tryAlterateToEqPitch(PitchClass& target, const PitchClass& pattern);
        static size_t minSemitoneDistance(const PitchClass& c1, const PitchClass& c2);
        static int minSemitonesFromTo(const PitchClass& from, const PitchClass& to);

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
        static const std::array<PitchClass, 35> all;
    };

    std::ostream& operator<<(std::ostream& os, const Alteration& a);
    std::ostream& operator<<(std::ostream& os, const PitchName& p);
    std::ostream& operator<<(std::ostream& os, const PitchClass& p);
}
}

#endif // CEAMMC_MUSIC_THEORY_PITCH_CLASS_H
