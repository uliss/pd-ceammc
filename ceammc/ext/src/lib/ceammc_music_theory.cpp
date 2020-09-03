#include "ceammc_music_theory.h"
#include "ceammc_music_theory_names.h"
#include "ceammc_music_theory_pitch_class.h"
#include "ceammc_music_theory_pitch_name.h"

#include <algorithm>
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

    scale_.reserve(7);
    alt_up_.reserve(5);
    alt_down_.reserve(5);
    chrom_up_.reserve(12);
    chrom_down_.reserve(12);

    calcScale();
}

void Tonality::setPitch(PitchClass name)
{
    pitch_ = name;
    calcScale();
}

void Tonality::setModus(HarmonicModus m)
{
    modus_ = m;
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

const Scale& Tonality::alterations(AlterationDir dir) const
{
    return dir == ALTERATE_UP ? alt_up_ : alt_down_;
}

const Scale& Tonality::chromatic(AlterationDir dir) const
{
    return dir == ALTERATE_UP ? chrom_up_ : chrom_down_;
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

int Tonality::fifthsCircleIndex(const PitchClass& c, HarmonicModus m)
{
    if (m == MAJOR)
        return pitchToFithIndex(c.pitchName()) + 7 * c.alteration().semitones();
    else
        return pitchToFithIndex(c.pitchName()) - 3 + 7 * c.alteration().semitones();
}

PitchClass Tonality::correctAlteration(size_t pitch, const Tonality& t, AlterationDir dir)
{
    pitch %= 12;
    const Scale& chrom = t.chromatic(dir);

    for (size_t i = 0; i < chrom.size(); i++) {
        if (chrom[i].absolutePitch() == pitch)
            return chrom[i];
    }

    // shoud never happend
    return PitchClass(pitch);
}

void Tonality::calcScale()
{
    scale_.assign(7, pitch_);
    alt_up_.assign(5, pitch_);
    alt_down_.assign(5, pitch_);
    chrom_up_.clear();
    chrom_down_.clear();

    if (modus_ == MAJOR) {
        scale_[1] = scale_[0].toneUp();
        scale_[2] = scale_[1].toneUp();
        scale_[3] = scale_[2].semitoneUp();
        scale_[4] = scale_[3].toneUp();
        scale_[5] = scale_[4].toneUp();
        scale_[6] = scale_[5].toneUp();

        alt_up_[0] = scale_[0].alterate(1);
        alt_up_[1] = scale_[1].alterate(1);
        alt_up_[2] = scale_[3].alterate(1);
        alt_up_[3] = scale_[4].alterate(1);
        alt_up_[4] = scale_[6].alterate(-1);

        alt_down_[0] = scale_[1].alterate(-1);
        alt_down_[1] = scale_[2].alterate(-1);
        alt_down_[2] = scale_[3].alterate(1);
        alt_down_[3] = scale_[5].alterate(-1);
        alt_down_[4] = scale_[6].alterate(-1);

        chrom_up_.push_back(scale_[0]);
        chrom_up_.push_back(scale_[0].alterate(1));
        chrom_up_.push_back(scale_[1]);
        chrom_up_.push_back(scale_[1].alterate(1));
        chrom_up_.push_back(scale_[2]);
        chrom_up_.push_back(scale_[3]);
        chrom_up_.push_back(scale_[3].alterate(1));
        chrom_up_.push_back(scale_[4]);
        chrom_up_.push_back(scale_[4].alterate(1));
        chrom_up_.push_back(scale_[5]);
        chrom_up_.push_back(scale_[6].alterate(-1));
        chrom_up_.push_back(scale_[6]);

        chrom_down_.push_back(scale_[0]);
        chrom_down_.push_back(scale_[1].alterate(-1));
        chrom_down_.push_back(scale_[1]);
        chrom_down_.push_back(scale_[2].alterate(-1));
        chrom_down_.push_back(scale_[2]);
        chrom_down_.push_back(scale_[3]);
        chrom_down_.push_back(scale_[3].alterate(1));
        chrom_down_.push_back(scale_[4]);
        chrom_down_.push_back(scale_[5].alterate(-1));
        chrom_down_.push_back(scale_[5]);
        chrom_down_.push_back(scale_[6].alterate(-1));
        chrom_down_.push_back(scale_[6]);
    } else {
        scale_[1] = scale_[0].toneUp();
        scale_[2] = scale_[1].semitoneUp();
        scale_[3] = scale_[2].toneUp();
        scale_[4] = scale_[3].toneUp();
        scale_[5] = scale_[4].semitoneUp();
        scale_[6] = scale_[5].toneUp();

        alt_up_[0] = scale_[1].alterate(-1);
        alt_up_[1] = scale_[2].alterate(1);
        alt_up_[2] = scale_[3].alterate(1);
        alt_up_[3] = scale_[5].alterate(1);
        alt_up_[4] = scale_[6].alterate(1);

        chrom_up_.push_back(scale_[0]);
        chrom_up_.push_back(scale_[1].alterate(-1));
        chrom_up_.push_back(scale_[1]);
        chrom_up_.push_back(scale_[2]);
        chrom_up_.push_back(scale_[2].alterate(1));
        chrom_up_.push_back(scale_[3]);
        chrom_up_.push_back(scale_[3].alterate(1));
        chrom_up_.push_back(scale_[4]);
        chrom_up_.push_back(scale_[5]);
        chrom_up_.push_back(scale_[5].alterate(1));
        chrom_up_.push_back(scale_[6]);
        chrom_up_.push_back(scale_[6].alterate(1));

        alt_down_ = alt_up_;
        chrom_down_ = chrom_up_;
    }
}
