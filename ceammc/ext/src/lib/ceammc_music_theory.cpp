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
    if (abs(fifthsCircleIndex(pitch_, m)) > 7) {
        Enharmonics en = p.enharmonics();
        for (size_t i = 0; i < en.size(); i++) {
            if (abs(fifthsCircleIndex(en[i], m)) < 7) {
                pitch_ = en[i];
            }
        }
    }

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

size_t Tonality::numSharps() const
{
    size_t res = 0;
    for (size_t i = 0; i < scale_.size(); i++) {
        if (scale_[i].alteration().type() == ALTERATION_SHARP)
            res++;
    }

    return res;
}

size_t Tonality::numFlats() const
{
    size_t res = 0;
    for (size_t i = 0; i < scale_.size(); i++) {
        if (scale_[i].alteration().type() == ALTERATION_FLAT)
            res++;
    }

    return res;
}

size_t Tonality::numKeys() const
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

static int pitchToFithIndex(const PitchName& n)
{
    static int fiths[] = {
        +0, // C,
        +2, // D,
        +4, // E
        -1, // F
        +1, // G
        +3, // A
        +5, // B
    };

    return fiths[n.index()];
}

int Tonality::fifthsCircleIndex(const PitchClass& c, mode_t m)
{
    if (m == MAJOR)
        return pitchToFithIndex(c.pitchName()) + 7 * c.alteration().semitones();
    else
        return pitchToFithIndex(c.pitchName()) - 3 + 7 * c.alteration().semitones();
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
    } else {
        scale_[1] = scale_[0].toneUp();
        scale_[2] = scale_[1].semitoneUp();
        scale_[3] = scale_[2].toneUp();
        scale_[4] = scale_[3].toneUp();
        scale_[5] = scale_[4].toneUp();
        scale_[6] = scale_[5].toneUp();
    }
}

bool Tonality::isValid() const
{
    for (size_t i = 0; i < scale_.size(); i++) {
        if (abs(scale_[i].alteration().semitones()) > 1)
            return false;
    }

    return true;
}
