#ifndef CEAMMC_MUSIC_THEORY_H
#define CEAMMC_MUSIC_THEORY_H

#include <boost/functional/hash.hpp>
#include <iostream>
#include <string>
#include <vector>

namespace ceammc {
namespace music {

    class Alteration {
    public:
        enum Type {
            DOUBLE_FLAT = -2,
            FLAT,
            NATURAL,
            SHARP,
            DOUBLE_SHARP
        };

    public:
        Alteration(Type t = NATURAL)
            : type_(t)
        {
        }

        Alteration::Type get() const { return type_; }
        void set(Type t) { type_ = t; }

        bool operator==(Type t) const { return type_ == t; }
        bool operator==(const Alteration& a) const { return type_ == a.type_; }
        bool operator!=(const Alteration& a) const { return type_ != a.type_; }
        bool operator<(const Alteration& a) const { return type_ < a.type_; }
        bool operator<=(const Alteration& a) const { return type_ <= a.type_; }
        bool operator>(const Alteration& a) const { return type_ > a.type_; }
        bool operator>=(const Alteration& a) const { return type_ >= a.type_; }

        bool operator++();
        bool operator--();

        bool alterate(int n);

        const char* fullName() const;
        const char* shortName() const;

        size_t hash_value() const;

    private:
        Type type_;
    };

    std::ostream& operator<<(std::ostream& os, const Alteration& a);
    size_t hash_value(const Alteration& a);

    class PitchName {
        unsigned char value_;
        PitchName(unsigned char v);

    public:
        PitchName(const PitchName& p);

        bool operator==(const PitchName& p) const { return value_ == p.value_; }
        bool operator!=(const PitchName& p) const { return value_ != p.value_; }

        PitchName operator+(int i) const;
        PitchName operator-(int i) const;

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

    private:
        friend std::ostream& operator<<(std::ostream& os, const PitchName& p);
        static const char* pitch_names_[7];
    };

    std::ostream& operator<<(std::ostream& os, const PitchName& p);

    class IntegerPitch {
    public:
        enum Pitch {
            C = 0,
            D = 2,
            E = 4,
            F = 5,
            G = 7,
            A = 9,
            B = 11
        };

        IntegerPitch(Pitch p)
            : pitch_(p)
        {
        }

        Pitch get() const { return pitch_; }
        void set(Pitch p) { pitch_ = p; }

        bool operator==(Pitch p) const { return pitch_ == p; }
        bool operator==(const IntegerPitch& p) const { return pitch_ == p.pitch_; }
        bool operator!=(const IntegerPitch& p) const { return pitch_ != p.pitch_; }
        bool operator<(const IntegerPitch& p) const { return pitch_ < p.pitch_; }
        bool operator<=(const IntegerPitch& p) const { return pitch_ <= p.pitch_; }
        bool operator>(const IntegerPitch& p) const { return pitch_ > p.pitch_; }
        bool operator>=(const IntegerPitch& p) const { return pitch_ >= p.pitch_; }

        IntegerPitch next() const;
        IntegerPitch prev() const;

        const char* name() const;

    private:
        Pitch pitch_;
    };

    int semitones(const IntegerPitch& p1, const IntegerPitch& p2);
    IntegerPitch operator+(const IntegerPitch& p, int steps);
    IntegerPitch operator-(const IntegerPitch& p, int steps);

    std::ostream& operator<<(std::ostream& os, const IntegerPitch& p);
    size_t hash_value(const IntegerPitch& p);

    class PitchClass;
    typedef std::vector<PitchClass> Enharmonics;

    class PitchClass {
        IntegerPitch p_;
        Alteration alt_;

    public:
        PitchClass(IntegerPitch::Pitch p, Alteration::Type a = Alteration::NATURAL)
            : p_(IntegerPitch(p))
            , alt_(Alteration(a))
        {
        }

        PitchClass(IntegerPitch p, Alteration a = Alteration(Alteration::NATURAL))
            : p_(p)
            , alt_(a)
        {
        }

        IntegerPitch pitch() const { return p_; }
        void setPitch(IntegerPitch p) { p_ = p; }
        Alteration alt() const { return alt_; }
        void setAlt(Alteration a) { alt_ = a; }

        bool operator==(const PitchClass& c) const { return p_ == c.p_ && alt_ == c.alt_; }
        bool operator!=(const PitchClass& c) const { return !this->operator==(c); }

        bool enharmonicEqual(const PitchClass& c) const { return enharmonicPitch() == c.enharmonicPitch(); }
        int enharmonicPitch() const { return p_.get() + alt_.get(); }

        bool operator<(const PitchClass& c) const { return enharmonicPitch() < c.enharmonicPitch(); }
        bool operator<=(const PitchClass& c) const { return enharmonicPitch() <= c.enharmonicPitch(); }
        bool operator>(const PitchClass& c) const { return enharmonicPitch() > c.enharmonicPitch(); }
        bool operator>=(const PitchClass& c) const { return enharmonicPitch() >= c.enharmonicPitch(); }

        std::string name() const;

        PitchClass simplifyFull() const;
        PitchClass& simplifyDouble();

        PitchClass toneUp() const;
        PitchClass semitoneUp() const;
        PitchClass stepTranspose(int n) const;

        Enharmonics upperEnharmonics() const;
        Enharmonics lowerEnharmonics() const;
        Enharmonics enharmonic() const;

    public:
        static bool tryAlterateToEqPattern(PitchClass& pitch, const PitchClass& pattern);

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
    size_t hash_value(const PitchClass& c);

    size_t minSemitoneDistance(const PitchClass& c1, const PitchClass& c2);
    int minSemitonesFromTo(const PitchClass& c1, const PitchClass& c2);

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
