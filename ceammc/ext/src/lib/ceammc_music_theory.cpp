#include "ceammc_music_theory.h"

#include <boost/algorithm/minmax.hpp>
#include <boost/assign/list_of.hpp>
#include <boost/unordered_map.hpp>
#include <cstdlib>

using namespace ceammc;
using namespace ceammc::music;

static const char* ALTERATION_SHORT_NAMES[] = {
    "--", "-", "", "#", "##"
};

static const char* ALTERATION_FULL_NAMES[] = {
    "double flat", "flat", "natural", "sharp", "double sharp"
};

static const char* INTEGER_PITCH_NAMES[] = {
    "C", "", "D", "", "E", "F", "", "G", "", "A", "", "B"
};

const PitchClass PitchClass::Cff(IntegerPitch::C, Alteration::DOUBLE_FLAT);
const PitchClass PitchClass::Cf(IntegerPitch::C, Alteration::FLAT);
const PitchClass PitchClass::C(IntegerPitch::C, Alteration::NATURAL);
const PitchClass PitchClass::Cs(IntegerPitch::C, Alteration::SHARP);
const PitchClass PitchClass::Css(IntegerPitch::C, Alteration::DOUBLE_SHARP);

const PitchClass PitchClass::Dff(IntegerPitch::D, Alteration::DOUBLE_FLAT);
const PitchClass PitchClass::Df(IntegerPitch::D, Alteration::FLAT);
const PitchClass PitchClass::D(IntegerPitch::D, Alteration::NATURAL);
const PitchClass PitchClass::Ds(IntegerPitch::D, Alteration::SHARP);
const PitchClass PitchClass::Dss(IntegerPitch::D, Alteration::DOUBLE_SHARP);

const PitchClass PitchClass::Eff(IntegerPitch::E, Alteration::DOUBLE_FLAT);
const PitchClass PitchClass::Ef(IntegerPitch::E, Alteration::FLAT);
const PitchClass PitchClass::E(IntegerPitch::E, Alteration::NATURAL);
const PitchClass PitchClass::Es(IntegerPitch::E, Alteration::SHARP);
const PitchClass PitchClass::Ess(IntegerPitch::E, Alteration::DOUBLE_SHARP);

const PitchClass PitchClass::Fff(IntegerPitch::F, Alteration::DOUBLE_FLAT);
const PitchClass PitchClass::Ff(IntegerPitch::F, Alteration::FLAT);
const PitchClass PitchClass::F(IntegerPitch::F, Alteration::NATURAL);
const PitchClass PitchClass::Fs(IntegerPitch::F, Alteration::SHARP);
const PitchClass PitchClass::Fss(IntegerPitch::F, Alteration::DOUBLE_SHARP);

const PitchClass PitchClass::Gff(IntegerPitch::G, Alteration::DOUBLE_FLAT);
const PitchClass PitchClass::Gf(IntegerPitch::G, Alteration::FLAT);
const PitchClass PitchClass::G(IntegerPitch::G, Alteration::NATURAL);
const PitchClass PitchClass::Gs(IntegerPitch::G, Alteration::SHARP);
const PitchClass PitchClass::Gss(IntegerPitch::G, Alteration::DOUBLE_SHARP);

const PitchClass PitchClass::Aff(IntegerPitch::A, Alteration::DOUBLE_FLAT);
const PitchClass PitchClass::Af(IntegerPitch::A, Alteration::FLAT);
const PitchClass PitchClass::A(IntegerPitch::A, Alteration::NATURAL);
const PitchClass PitchClass::As(IntegerPitch::A, Alteration::SHARP);
const PitchClass PitchClass::Ass(IntegerPitch::A, Alteration::DOUBLE_SHARP);

const PitchClass PitchClass::Bff(IntegerPitch::B, Alteration::DOUBLE_FLAT);
const PitchClass PitchClass::Bf(IntegerPitch::B, Alteration::FLAT);
const PitchClass PitchClass::B(IntegerPitch::B, Alteration::NATURAL);
const PitchClass PitchClass::Bs(IntegerPitch::B, Alteration::SHARP);
const PitchClass PitchClass::Bss(IntegerPitch::B, Alteration::DOUBLE_SHARP);

std::ostream& ceammc::music::operator<<(std::ostream& os, const Alteration& a)
{
    os << a.shortName();
    return os;
}

typedef boost::unordered_map<Tonality, Scale> TonalityScaleMap;

static TonalityScaleMap& tonality_scale_map()
{
    static TonalityScaleMap instance_;
    return instance_;
}

namespace {

bool init_tonality_scale_map()
{
    using namespace boost::assign;

#define MOLL(c) Tonality(PitchClass::c, MINOR)
#define DUR(c) Tonality(PitchClass::c, MAJOR)
#define P(c) (PitchClass::c)

    TonalityScaleMap& m = tonality_scale_map();
    //    m[DUR(C)] = list_of P(C) P(D) P(E) P(F) P(G) P(A) P(B);
    //    m[DUR(Cs)] = list_of P(Cs) P(Ds) P(Es) P(Fs) P(Gs) P(As) P(Bs);
    //    m[DUR(Df)] = list_of P(Df) P(Ef) P(F) P(Gf) P(Af) P(Bf) P(C);
    //    m[DUR(D)] = list_of P(D) P(E) P(Fs) P(G) P(A) P(B) P(Cs);
    //    m[DUR(Eg)] = list_of P(Ef) P(F) P(G) P(Af) P(Bs) P(C) P(D);
    //    m[DUR(E)] = list_of P(E) P(Fs) P(Gs) P(A) P(B) P(Cs) P(Ds);
    //    m[DUR(F)] = list_of P(F) P(G) P(A) P(Bf) P(C) P(D) P(E);
    //    m[DUR(D)] = list_of P(D) P(E) P(Fs) P(G) P(A) P(B) P(Cs);
    return true;

#undef P
#undef DUR
#undef MOLL
}

bool init_tonality_scale_map_flag = init_tonality_scale_map();
}

bool Alteration::operator++()
{
    if (type_ == DOUBLE_SHARP)
        return false;

    type_ = Type(type_ + 1);
    return true;
}

bool Alteration::operator--()
{
    if (type_ == DOUBLE_FLAT)
        return false;

    type_ = Type(type_ - 1);
    return true;
}

bool Alteration::alterate(int n)
{
    if (n == 0)
        return true;

    if ((type_ + n) < DOUBLE_FLAT || (type_ + n) > DOUBLE_SHARP)
        return false;

    type_ = Type(type_ + n);
    return true;
}

const char* Alteration::fullName() const
{
    return ALTERATION_FULL_NAMES[type_ - DOUBLE_FLAT];
}

const char* Alteration::shortName() const
{
    return ALTERATION_SHORT_NAMES[type_ - DOUBLE_FLAT];
}

size_t Alteration::hash_value() const
{
    return boost::hash_value(type_);
}

IntegerPitch IntegerPitch::next() const
{
    if (pitch_ == B)
        return IntegerPitch(C);

    return IntegerPitch(pitch_ == E ? F : Pitch(pitch_ + 2));
}

IntegerPitch IntegerPitch::prev() const
{
    if (pitch_ == C)
        return IntegerPitch(B);

    return IntegerPitch(pitch_ == F ? E : Pitch(pitch_ - 2));
}

const char* IntegerPitch::name() const
{
    return INTEGER_PITCH_NAMES[pitch_ - C];
}

std::ostream& ceammc::music::operator<<(std::ostream& os, const IntegerPitch& p)
{
    os << p.name();
    return os;
}

std::ostream& ceammc::music::operator<<(std::ostream& os, const PitchClass& p)
{
    os << p.name();
    return os;
}

std::string PitchClass::name() const
{
    return std::string(p_.name()) + alt_.shortName();
}

PitchClass PitchClass::simplifyFull() const
{
    switch (alt_.get()) {
    case Alteration::FLAT: {
        // F-flat -> E and C-flat -> B
        if (p_ == IntegerPitch::F || p_ == IntegerPitch::C) {
            return PitchClass(p_.prev(), Alteration::NATURAL);
        }
    } break;
    case Alteration::DOUBLE_FLAT: {
        // F-flat -> E-flat and C-flat -> B-flat
        if (p_ == IntegerPitch::F || p_ == IntegerPitch::C) {
            return PitchClass(p_.prev(), Alteration::FLAT);
        } else {
            return PitchClass(p_.prev(), Alteration::NATURAL);
        }
    } break;
    case Alteration::SHARP: {
        // E-sharp -> F and B-sharp -> C
        if (p_ == IntegerPitch::E || p_ == IntegerPitch::B) {
            return PitchClass(p_.next(), Alteration::NATURAL);
        }
    } break;
    case Alteration::DOUBLE_SHARP: {
        // E-sharp -> F-sharp and B-sharp -> C-sharp
        if (p_ == IntegerPitch::E || p_ == IntegerPitch::B) {
            return PitchClass(p_.next(), Alteration::SHARP);
        } else {
            return PitchClass(p_.next(), Alteration::NATURAL);
        }
    } break;
    case Alteration::NATURAL:
    default:
        break;
    }

    return *this;
}

PitchClass& PitchClass::simplifyDouble()
{
    switch (alt_.get()) {
    case Alteration::DOUBLE_FLAT: {
        // F-flat -> E-flat and C-flat -> B-flat
        if (p_ == IntegerPitch::F || p_ == IntegerPitch::C) {
            p_ = p_.prev();
            alt_ = Alteration::FLAT;
        } else {
            p_ = p_.prev();
            alt_ = Alteration::NATURAL;
        }
    } break;
    case Alteration::DOUBLE_SHARP: {
        // E-sharp -> F-sharp and B-sharp -> C-sharp
        if (p_ == IntegerPitch::E || p_ == IntegerPitch::B) {
            p_ = p_.next();
            alt_ = Alteration::SHARP;
        } else {
            p_ = p_.next();
            alt_ = Alteration::NATURAL;
        }
    } break;
    case Alteration::NATURAL:
    default:
        break;
    }

    return *this;
}

PitchClass PitchClass::toneUp() const
{
    PitchClass new_pitch(p_.next(), alt_);
    size_t semi = minSemitoneDistance(*this, new_pitch);

    if (semi == 1) {
        Alteration a(alt_);
        if (!++a) {
        }
        new_pitch.setAlt(a);
    }

    return new_pitch;
}

PitchClass PitchClass::semitoneUp() const
{
    PitchClass new_pitch(p_.next(), alt_);
    size_t semi = minSemitoneDistance(*this, new_pitch);

    if (semi == 2) {
        Alteration a(alt_);
        if (!--a) {
        }
        new_pitch.setAlt(a);
    }

    return new_pitch;
}

PitchClass PitchClass::stepTranspose(int n) const
{
    return PitchClass(p_ + n, alt_);
}

bool PitchClass::tryAlterateToEqPattern(PitchClass& pitch, const PitchClass& pattern)
{
    size_t min_semi = minSemitoneDistance(pattern, pitch);
    int semi = pattern.enharmonicPitch() - pitch.enharmonicPitch();
    int sign = (min_semi < semi) ? -1 : 1;

    Alteration a = pitch.alt();
    if (!a.alterate(min_semi * sign))
        return false;

    pitch.setAlt(a);
    return true;
}

Enharmonics PitchClass::upperEnharmonics() const
{
    Enharmonics res;

    PitchClass e1 = stepTranspose(1);
    PitchClass e2 = stepTranspose(2);

    if (tryAlterateToEqPattern(e1, *this))
        res.push_back(e1);
    if (tryAlterateToEqPattern(e2, *this))
        res.push_back(e2);

    return res;
}

Enharmonics PitchClass::lowerEnharmonics() const
{
    Enharmonics res;

    PitchClass e1 = stepTranspose(-1);
    PitchClass e2 = stepTranspose(-2);

    if (tryAlterateToEqPattern(e1, *this))
        res.push_back(e1);
    if (tryAlterateToEqPattern(e2, *this))
        res.push_back(e2);

    return res;
}

Enharmonics PitchClass::enharmonic() const
{
    Enharmonics res;

    PitchClass e1 = stepTranspose(-2);
    PitchClass e2 = stepTranspose(-1);
    PitchClass e3 = stepTranspose(1);
    PitchClass e4 = stepTranspose(2);

    if (tryAlterateToEqPattern(e1, *this))
        res.push_back(e1);
    if (tryAlterateToEqPattern(e2, *this))
        res.push_back(e2);
    if (tryAlterateToEqPattern(e3, *this))
        res.push_back(e3);
    if (tryAlterateToEqPattern(e4, *this))
        res.push_back(e4);

    return res;
}

std::ostream& ceammc::music::operator<<(std::ostream& os, const Tonality& t)
{
    os << t.name();
    return os;
}

Tonality::Tonality(const PitchClass& p, HarmonicModus m)
    : pitch_(p)
    , modus_(m)
{
    pitch_.simplifyDouble();
    calcScale();
}

bool Tonality::enharmonicEqual(const Tonality& t) const
{
    return modus_ == t.modus_ && pitch_.enharmonicEqual(t.pitch());
}

std::string Tonality::name() const
{
    std::string res = pitch_.pitch().name();

    if (pitch_.alt() != Alteration::NATURAL) {
        res += '-';
        res += pitch_.alt().fullName();
    }

    res += ((modus_ == MAJOR) ? " major" : " minor");

    return res;
}

const Scale& Tonality::scale() const
{
    return scale_;
}

size_t Tonality::sharps() const
{
    size_t res = 0;
    for (size_t i = 0; i < scale_.size(); i++) {
        int n = scale_[i].alt().get();
        if (n > 0)
            res += n;
    }

    return res;
}

size_t Tonality::flats() const
{
    size_t res = 0;
    for (size_t i = 0; i < scale_.size(); i++) {
        int n = scale_[i].alt().get();
        if (n < 0)
            res -= n;
    }

    return res;
}

size_t Tonality::keys() const
{
    size_t res = 0;
    for (size_t i = 0; i < scale_.size(); i++) {
        res += abs(scale_[i].alt().get());
    }

    return res;
}

Tonality Tonality::simplify() const
{
    Tonality res(*this);
    res.pitch_ = pitch_.simplifyFull();
    return res;
}

void Tonality::calcScale()
{
    scale_.reserve(7);
    scale_.assign(7, pitch_);

    if (modus_ == MAJOR) {
        scale_[1] = scale_[0].toneUp();
        scale_[2] = scale_[1].toneUp();
        scale_[3] = scale_[2].semitoneUp();
        scale_[4] = scale_[3].toneUp();
        scale_[5] = scale_[4].toneUp();
        scale_[6] = scale_[5].toneUp();
    }
}

size_t ceammc::music::hash_value(const Tonality& c)
{
    size_t seed = 0;
    boost::hash_combine(seed, c.pitch());
    boost::hash_combine(seed, c.modus());
    return seed;
}

size_t ceammc::music::hash_value(const PitchClass& c)
{
    size_t seed = 0;
    boost::hash_combine(seed, c.pitch());
    boost::hash_combine(seed, c.alt());
    return seed;
}

size_t ceammc::music::hash_value(const IntegerPitch& p)
{
    return boost::hash_value(p.get());
}

size_t ceammc::music::hash_value(const Alteration& a)
{
    return boost::hash_value(a.get());
}

int ceammc::music::semitones(const IntegerPitch& p1, const IntegerPitch& p2)
{
    int dist = abs(p1.get() - p2.get());
    return std::min(dist, 12 - dist);
}

IntegerPitch ceammc::music::operator+(const IntegerPitch& p, int steps)
{
    IntegerPitch res = p;

    // optimize steps
    if (steps < 0) {
        // to fix negative modulo division implementation defined
        steps = -((-steps) % 7);
    } else
        steps %= 7;

    if (steps == 0)
        return res;

    if (steps < 0) {
        while (steps++ < 0)
            res = res.prev();
    } else {
        while (steps-- > 0)
            res = res.next();
    }

    return res;
}

IntegerPitch ceammc::music::operator-(const IntegerPitch& p, int steps)
{
    return p + (-steps);
}

size_t ceammc::music::minSemitoneDistance(const PitchClass& c1, const PitchClass& c2)
{
    const int dist = abs(c2.enharmonicPitch() - c1.enharmonicPitch());
    return std::min(dist, 12 - dist);
}

int minSemitonesFromTo(const PitchClass& c1, const PitchClass& c2)
{
    const int dist = abs(c2.enharmonicPitch() - c1.enharmonicPitch());
    const int min_dist = std::min(dist, 12 - dist);
}

const PitchName PitchName::C(0);
const PitchName PitchName::D(1);
const PitchName PitchName::E(2);
const PitchName PitchName::F(3);
const PitchName PitchName::G(4);
const PitchName PitchName::A(5);
const PitchName PitchName::B(6);

const char* PitchName::pitch_names_[7] = { "C", "D", "E", "F", "G", "A", "B" };

PitchName::PitchName(unsigned char v)
    : value_(v % 7)
{
}

PitchName::PitchName(const PitchName& p)
    : value_(p.value_)
{
}

PitchName PitchName::operator+(int i) const
{
    return PitchName((i < 0) ? (7 + value_ - ((-i) % 7))
                             : (value_ + i) % 7);
}

PitchName PitchName::operator-(int i) const
{
    return PitchName((i < 0) ? ((value_ + ((-i) % 7)))
                             : (7 + value_ - (i % 7)));
}

size_t PitchName::distance(const PitchName& p1, const PitchName& p2)
{
    return abs(int(p1.value_) - int(p2.value_));
}

size_t PitchName::minDistance(const PitchName& p1, const PitchName& p2)
{
    int dist = distance(p1, p2);
    return std::min(dist, 7 - dist);
}

std::ostream& ceammc::music::operator<<(std::ostream& os, const PitchName& p)
{
    os << p.pitch_names_[p.value_];
    return os;
}
