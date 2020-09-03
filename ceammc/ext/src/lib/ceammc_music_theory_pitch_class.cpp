#include "ceammc_music_theory_pitch_class.h"
#include "ceammc_music_theory_names.h"

using namespace ceammc::music;

const char* PitchName::pitch_names_[7] = { "C", "D", "E", "F", "G", "A", "B" };
static const char* ALTERATION_SHORT_NAMES[] = { "bb", "b", "", "#", "##" };
static const char* ALTERATION_FULL_NAMES[] = { "double flat", "flat", "natural", "sharp", "double sharp" };

const PitchName PitchName::C(PITCH_NAME_C);
const PitchName PitchName::D(PITCH_NAME_D);
const PitchName PitchName::E(PITCH_NAME_E);
const PitchName PitchName::F(PITCH_NAME_F);
const PitchName PitchName::G(PITCH_NAME_G);
const PitchName PitchName::A(PITCH_NAME_A);
const PitchName PitchName::B(PITCH_NAME_B);

const Alteration Alteration::DOUBLE_FLAT(ALTERATION_DOUBLE_FLAT);
const Alteration Alteration::FLAT(ALTERATION_FLAT);
const Alteration Alteration::NATURAL(ALTERATION_NATURAL);
const Alteration Alteration::SHARP(ALTERATION_SHARP);
const Alteration Alteration::DOUBLE_SHARP(ALTERATION_DOUBLE_SHARP);

const PitchClass PitchClass::Cff(PitchName::C, Alteration::DOUBLE_FLAT);
const PitchClass PitchClass::Cf(PitchName::C, Alteration::FLAT);
const PitchClass PitchClass::C(PitchName::C, Alteration::NATURAL);
const PitchClass PitchClass::Cs(PitchName::C, Alteration::SHARP);
const PitchClass PitchClass::Css(PitchName::C, Alteration::DOUBLE_SHARP);

const PitchClass PitchClass::Dff(PitchName::D, Alteration::DOUBLE_FLAT);
const PitchClass PitchClass::Df(PitchName::D, Alteration::FLAT);
const PitchClass PitchClass::D(PitchName::D, Alteration::NATURAL);
const PitchClass PitchClass::Ds(PitchName::D, Alteration::SHARP);
const PitchClass PitchClass::Dss(PitchName::D, Alteration::DOUBLE_SHARP);

const PitchClass PitchClass::Eff(PitchName::E, Alteration::DOUBLE_FLAT);
const PitchClass PitchClass::Ef(PitchName::E, Alteration::FLAT);
const PitchClass PitchClass::E(PitchName::E, Alteration::NATURAL);
const PitchClass PitchClass::Es(PitchName::E, Alteration::SHARP);
const PitchClass PitchClass::Ess(PitchName::E, Alteration::DOUBLE_SHARP);

const PitchClass PitchClass::Fff(PitchName::F, Alteration::DOUBLE_FLAT);
const PitchClass PitchClass::Ff(PitchName::F, Alteration::FLAT);
const PitchClass PitchClass::F(PitchName::F, Alteration::NATURAL);
const PitchClass PitchClass::Fs(PitchName::F, Alteration::SHARP);
const PitchClass PitchClass::Fss(PitchName::F, Alteration::DOUBLE_SHARP);

const PitchClass PitchClass::Gff(PitchName::G, Alteration::DOUBLE_FLAT);
const PitchClass PitchClass::Gf(PitchName::G, Alteration::FLAT);
const PitchClass PitchClass::G(PitchName::G, Alteration::NATURAL);
const PitchClass PitchClass::Gs(PitchName::G, Alteration::SHARP);
const PitchClass PitchClass::Gss(PitchName::G, Alteration::DOUBLE_SHARP);

const PitchClass PitchClass::Aff(PitchName::A, Alteration::DOUBLE_FLAT);
const PitchClass PitchClass::Af(PitchName::A, Alteration::FLAT);
const PitchClass PitchClass::A(PitchName::A, Alteration::NATURAL);
const PitchClass PitchClass::As(PitchName::A, Alteration::SHARP);
const PitchClass PitchClass::Ass(PitchName::A, Alteration::DOUBLE_SHARP);

const PitchClass PitchClass::Bff(PitchName::B, Alteration::DOUBLE_FLAT);
const PitchClass PitchClass::Bf(PitchName::B, Alteration::FLAT);
const PitchClass PitchClass::B(PitchName::B, Alteration::NATURAL);
const PitchClass PitchClass::Bs(PitchName::B, Alteration::SHARP);
const PitchClass PitchClass::Bss(PitchName::B, Alteration::DOUBLE_SHARP);

const std::array<PitchClass, 35> PitchClass::all = {
    Cff, Cf, C, Cs, Css,
    Dff, Df, D, Ds, Dss,
    Eff, Ef, E, Es, Ess,
    Fff, Ff, F, Fs, Fss,
    Gff, Gf, G, Gs, Gss,
    Aff, Af, A, As, Ass,
    Bff, Bf, B, Bs, Bss
};

PitchName::PitchName(unsigned char v)
    : value_(v % 7)
{
}

PitchName::PitchName(const PitchName& p)
    : value_(p.value_)
{
}

PitchName& PitchName::operator=(const PitchName& p)
{
    value_ = p.value_;
    return *this;
}

PitchName PitchName::operator+(int i) const
{
    return PitchName((i < 0) ? (7 + value_ - ((-i) % 7))
                             : (value_ + i) % 7);
}

PitchName PitchName::operator-(int i) const
{
    return *this + (-i);
}

PitchNameType PitchName::type() const
{
    return PitchNameType(value_);
}

unsigned int PitchName::index() const
{
    return value_;
}

unsigned int PitchName::absolutePitch() const
{
    return value_ < 3 ? value_ * 2
                      : value_ * 2 - 1;
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

size_t PitchName::upSteps(const PitchName& from, const PitchName& to)
{
    if (from.value_ <= to.value_)
        return to.value_ - from.value_;
    else
        return (7 + to.value_ - from.value_) % 7;
}

size_t PitchName::downSteps(const PitchName& from, const PitchName& to)
{
    if (to.value_ <= from.value_)
        return from.value_ - to.value_;
    else
        return (7 + from.value_ - to.value_) % 7;
}

int PitchName::minSteps(const PitchName& from, const PitchName& to)
{
    size_t up = upSteps(from, to);
    size_t down = downSteps(from, to);

    return up < down ? up : -down;
}

std::ostream& ceammc::music::operator<<(std::ostream& os, const PitchName& p)
{
    os << p.pitch_names_[p.value_];
    return os;
}

std::ostream& ceammc::music::operator<<(std::ostream& os, const Alteration& a)
{
    os << a.shortName();
    return os;
}

Alteration::Alteration(const Alteration& a)
    : value_(a.value_)
{
}

Alteration& Alteration::operator=(const Alteration& a)
{
    value_ = a.value_;
    return *this;
}

bool Alteration::operator++()
{
    if (value_ == ALTERATION_DOUBLE_SHARP)
        return false;

    value_++;
    return true;
}

bool Alteration::operator--()
{
    if (value_ == ALTERATION_DOUBLE_FLAT)
        return false;

    value_--;
    return true;
}

bool Alteration::alterate(int n)
{
    if (n == 0)
        return true;

    if ((value_ + n) < ALTERATION_DOUBLE_FLAT || (value_ + n) > ALTERATION_DOUBLE_SHARP)
        return false;

    value_ += n;
    return true;
}

const char* Alteration::fullName() const
{
    return ALTERATION_FULL_NAMES[value_ - ALTERATION_DOUBLE_FLAT];
}

const char* Alteration::shortName() const
{
    return ALTERATION_SHORT_NAMES[value_ - ALTERATION_DOUBLE_FLAT];
}

AlterationType Alteration::type() const
{
    return AlterationType(value_);
}

int Alteration::semitones() const
{
    return value_;
}

double Alteration::cents() const
{
    return value_ * 100;
}

Alteration::Alteration(signed char t)
    : value_(t)
{
}

PitchClass::PitchClass(size_t semitoneValue)
    : pitch_name_(PitchName::C)
    , alt_(Alteration::NATURAL)
    , invalid_(false)
{
    size_t p = (semitoneValue < 5) ? (semitoneValue / 2) : ((semitoneValue + 1) / 2);
    size_t a = (semitoneValue < 5) ? (semitoneValue % 2) : ((semitoneValue + 1) % 2);
    pitch_name_ = PitchName::C + p;
    if (a)
        alt_.alterate(1);
}

PitchClass::PitchClass(PitchName p, Alteration a)
    : pitch_name_(p)
    , alt_(a)
    , invalid_(false)
{
}

bool PitchClass::operator==(const PitchClass& c) const
{
    return pitch_name_ == c.pitch_name_ && alt_ == c.alt_;
}

bool PitchClass::operator!=(const PitchClass& c) const
{
    return !this->operator==(c);
}

size_t PitchClass::absolutePitch() const
{
    int res = int(pitch_name_.absolutePitch()) + alt_.semitones();
    return res < 0 ? (12 + res) % 12 : (res % 12);
}

PitchClass::operator bool() const
{
    return invalid_ == false;
}

PitchClass PitchClass::simplifyFull() const
{
    switch (alt_.type()) {
    case ALTERATION_FLAT: {
        // F-flat -> E and C-flat -> B
        if (pitch_name_ == PitchName::F || pitch_name_ == PitchName::C) {
            return PitchClass(pitch_name_ - 1, Alteration::NATURAL);
        }
    } break;
    case ALTERATION_DOUBLE_FLAT: {
        // F-flat -> E-flat and C-flat -> B-flat
        if (pitch_name_ == PitchName::F || pitch_name_ == PitchName::C) {
            return PitchClass(pitch_name_ - 1, Alteration::FLAT);
        } else {
            return PitchClass(pitch_name_ - 1, Alteration::NATURAL);
        }
    } break;
    case ALTERATION_SHARP: {
        // E-sharp -> F and B-sharp -> C
        if (pitch_name_ == PitchName::E || pitch_name_ == PitchName::B) {
            return PitchClass(pitch_name_ + 1, Alteration::NATURAL);
        }
    } break;
    case ALTERATION_DOUBLE_SHARP: {
        // E-sharp -> F-sharp and B-sharp -> C-sharp
        if (pitch_name_ == PitchName::E || pitch_name_ == PitchName::B) {
            return PitchClass(pitch_name_ + 1, Alteration::SHARP);
        } else {
            return PitchClass(pitch_name_ + 1, Alteration::NATURAL);
        }
    } break;
    default:
        break;
    }

    return *this;
}

PitchClass PitchClass::simplifyDouble() const
{
    switch (alt_.type()) {
    case ALTERATION_DOUBLE_FLAT: {
        // F-flat -> E-flat and C-flat -> B-flat
        if (pitch_name_ == PitchName::F || pitch_name_ == PitchName::C) {
            return PitchClass(pitch_name_ - 1, Alteration::FLAT);
        } else {
            return PitchClass(pitch_name_ - 1, Alteration::NATURAL);
        }
    } break;
    case ALTERATION_DOUBLE_SHARP: {
        // E-sharp -> F-sharp and B-sharp -> C-sharp
        if (pitch_name_ == PitchName::E || pitch_name_ == PitchName::B) {
            return PitchClass(pitch_name_ + 1, Alteration::SHARP);
        } else {
            return PitchClass(pitch_name_ + 1, Alteration::NATURAL);
        }
    } break;
    default:
        break;
    }

    return *this;
}

PitchClass PitchClass::toneUp() const
{
    PitchClass new_pitch(pitch_name_ + 1, alt_);
    size_t semi = minSemitoneDistance(*this, new_pitch);

    if (semi == 1) {
        Alteration a = alt_;
        if (!a.alterate(1))
            new_pitch.invalid_ = true;

        new_pitch.setAlteration(a);
    }

    return new_pitch;
}

PitchClass PitchClass::semitoneUp() const
{
    PitchClass new_pitch(pitch_name_ + 1, alt_);
    size_t semi = minSemitoneDistance(*this, new_pitch);

    if (semi == 2) {
        Alteration a = alt_;
        if (!a.alterate(-1))
            new_pitch.invalid_ = true;

        new_pitch.setAlteration(a);
    }

    return new_pitch;
}

PitchClass PitchClass::stepTranspose(int n) const
{
    return PitchClass(pitch_name_ + n, alt_);
}

bool PitchClass::tryAlterateToEqPitch(PitchClass& target, const PitchClass& pattern)
{
    int min_semi = minSemitonesFromTo(target, pattern);

    Alteration a = target.alteration();
    if (!a.alterate(min_semi))
        return false;

    target.setAlteration(a);
    return true;
}

Enharmonics PitchClass::upperEnharmonics() const
{
    Enharmonics res;

    PitchClass e1 = stepTranspose(1);
    PitchClass e2 = stepTranspose(2);

    if (tryAlterateToEqPitch(e1, *this))
        res.push_back(e1);
    if (tryAlterateToEqPitch(e2, *this))
        res.push_back(e2);

    return res;
}

Enharmonics PitchClass::lowerEnharmonics() const
{
    Enharmonics res;

    PitchClass e1 = stepTranspose(-2);
    PitchClass e2 = stepTranspose(-1);

    if (tryAlterateToEqPitch(e1, *this))
        res.push_back(e1);
    if (tryAlterateToEqPitch(e2, *this))
        res.push_back(e2);

    return res;
}

Enharmonics PitchClass::enharmonics() const
{
    Enharmonics res;

    PitchClass e1 = stepTranspose(-2);
    PitchClass e2 = stepTranspose(-1);
    PitchClass e3 = stepTranspose(1);
    PitchClass e4 = stepTranspose(2);

    if (tryAlterateToEqPitch(e1, *this))
        res.push_back(e1);
    if (tryAlterateToEqPitch(e2, *this))
        res.push_back(e2);
    if (tryAlterateToEqPitch(e3, *this))
        res.push_back(e3);
    if (tryAlterateToEqPitch(e4, *this))
        res.push_back(e4);

    return res;
}

size_t PitchClass::minSemitoneDistance(const PitchClass& c1, const PitchClass& c2)
{
    return abs(minSemitonesFromTo(c1, c2));
}

int PitchClass::minSemitonesFromTo(const PitchClass& from, const PitchClass& to)
{
    int p_from = from.absolutePitch();
    int p_to = to.absolutePitch();
    int p_dist = p_to - p_from;
    int dist = abs(p_dist);

    if (dist < 12 - dist)
        return dist * (p_dist < 0 ? -1 : 1);
    else
        return (12 - dist) * (p_dist > 0 ? -1 : 1);
}

std::ostream& ceammc::music::operator<<(std::ostream& os, const PitchClass& p)
{
    os << to_string(p);
    return os;
}

PitchClass PitchClass::alterate(int n) const
{
    PitchClass pitch(*this);
    Alteration new_alt(alt_);
    if (!new_alt.alterate(n)) {
        pitch.invalid_ = true;
        return pitch;
    }

    pitch.setAlteration(new_alt);
    return pitch;
}
