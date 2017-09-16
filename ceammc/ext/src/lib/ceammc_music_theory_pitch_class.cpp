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

const boost::array<PitchClass, 35> PitchClass::all = {
    Cff, Cf, C, Cs, Css,
    Dff, Df, D, Ds, Dss,
    Eff, Ef, E, Es, Ess,
    Fff, Ff, F, Fs, Fss,
    Gff, Gf, G, Gs, Gss,
    Aff, Af, A, As, Ass,
    Bff, Bf, B, Bs, Bss
};

PitchClass::PitchClass(PitchName p, Alteration a)
    : pitch_name_(p)
    , alt_(a)
    , invalid_(false)
{
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

std::string PitchClass::name() const
{
    return to_string(pitch_name_) + alt_.shortName();
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
    os << p.name();
    return os;
}
