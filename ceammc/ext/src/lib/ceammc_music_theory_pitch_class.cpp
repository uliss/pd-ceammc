#include "ceammc_music_theory_pitch_class.h"

using namespace ceammc::music;

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

PitchClass::PitchClass(PitchName p, Alteration a)
    : pitch_name_(p)
    , alt_(a)
{
}

size_t PitchClass::absolutePitch() const
{
    int res = int(pitch_name_.absolutePitch()) + alt_.get();
    return res < 0 ? (12 + res) % 12 : (res % 12);
}

std::string PitchClass::name() const
{
    return to_string(pitch_name_) + alt_.shortName();
}

PitchClass PitchClass::simplifyFull() const
{
    switch (alt_.get()) {
    case Alteration::FLAT: {
        // F-flat -> E and C-flat -> B
        if (pitch_name_ == PitchName::F || pitch_name_ == PitchName::C) {
            return PitchClass(pitch_name_ - 1, Alteration::NATURAL);
        }
    } break;
    case Alteration::DOUBLE_FLAT: {
        // F-flat -> E-flat and C-flat -> B-flat
        if (pitch_name_ == PitchName::F || pitch_name_ == PitchName::C) {
            return PitchClass(pitch_name_ - 1, Alteration::FLAT);
        } else {
            return PitchClass(pitch_name_ - 1, Alteration::NATURAL);
        }
    } break;
    case Alteration::SHARP: {
        // E-sharp -> F and B-sharp -> C
        if (pitch_name_ == PitchName::E || pitch_name_ == PitchName::B) {
            return PitchClass(pitch_name_ + 1, Alteration::NATURAL);
        }
    } break;
    case Alteration::DOUBLE_SHARP: {
        // E-sharp -> F-sharp and B-sharp -> C-sharp
        if (pitch_name_ == PitchName::E || pitch_name_ == PitchName::B) {
            return PitchClass(pitch_name_ + 1, Alteration::SHARP);
        } else {
            return PitchClass(pitch_name_ + 1, Alteration::NATURAL);
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
        if (pitch_name_ == PitchName::F || pitch_name_ == PitchName::C) {
            pitch_name_ = pitch_name_ - 1;
            alt_ = Alteration::FLAT;
        } else {
            pitch_name_ = pitch_name_ - 1;
            alt_ = Alteration::NATURAL;
        }
    } break;
    case Alteration::DOUBLE_SHARP: {
        // E-sharp -> F-sharp and B-sharp -> C-sharp
        if (pitch_name_ == PitchName::E || pitch_name_ == PitchName::B) {
            pitch_name_ = pitch_name_ + 1;
            alt_ = Alteration::SHARP;
        } else {
            pitch_name_ = pitch_name_ + 1;
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
    PitchClass new_pitch(pitch_name_ + 1, alt_);
    size_t semi = minSemitoneDistance(*this, new_pitch);

    if (semi == 1) {
        Alteration a(alt_);
        if (!++a) {
        }
        new_pitch.setAlteration(a);
    }

    return new_pitch;
}

PitchClass PitchClass::semitoneUp() const
{
    PitchClass new_pitch(pitch_name_ + 1, alt_);
    size_t semi = minSemitoneDistance(*this, new_pitch);

    if (semi == 2) {
        Alteration a(alt_);
        if (!--a) {
        }
        new_pitch.setAlteration(a);
    }

    return new_pitch;
}

PitchClass PitchClass::stepTranspose(int n) const
{
    return PitchClass(pitch_name_ + n, alt_);
}

bool PitchClass::tryAlterateToEqPattern(PitchClass& pitch, const PitchClass& pattern)
{
    size_t min_semi = minSemitoneDistance(pattern, pitch);
    int semi = pattern.absolutePitch() - pitch.absolutePitch();
    int sign = (min_semi < semi) ? -1 : 1;

    Alteration a = pitch.alteration();
    if (!a.alterate(min_semi * sign))
        return false;

    pitch.setAlteration(a);
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

std::ostream& ceammc::music::operator<<(std::ostream& os, const PitchClass& p)
{
    os << p.name();
    return os;
}
