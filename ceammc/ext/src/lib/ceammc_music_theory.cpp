#include "ceammc_music_theory.h"

#include <boost/algorithm/minmax.hpp>
#include <cstdlib>

using namespace ceammc;
using namespace ceammc::music;

std::ostream& ceammc::music::operator<<(std::ostream& os, const Tonality& t)
{
    os << t.name();
    return os;
}

Tonality::Tonality(const PitchClass& p, HarmonicModus m)
    : pitch_(p.simplifyDouble())
    , modus_(m)
{
    calcScale();
}

bool Tonality::enharmonicEqual(const Tonality& t) const
{
    return modus_ == t.modus_ && pitch_.enharmonicEqual(t.pitch());
}

std::string Tonality::name() const
{
    std::string res = to_string(pitch_.pitchName());

    if (pitch_.alteration() != Alteration::NATURAL) {
        res += '-';
        res += pitch_.alteration().fullName();
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
        int n = scale_[i].alteration().semitones();
        if (n > 0)
            res += n;
    }

    return res;
}

size_t Tonality::flats() const
{
    size_t res = 0;
    for (size_t i = 0; i < scale_.size(); i++) {
        int n = scale_[i].alteration().semitones();
        if (n < 0)
            res -= n;
    }

    return res;
}

size_t Tonality::keys() const
{
    size_t res = 0;
    for (size_t i = 0; i < scale_.size(); i++) {
        res += abs(scale_[i].alteration().semitones());
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
