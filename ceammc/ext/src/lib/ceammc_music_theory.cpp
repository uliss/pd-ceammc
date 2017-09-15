#include "ceammc_music_theory.h"

#include <boost/algorithm/minmax.hpp>
#include <boost/assign/list_of.hpp>
#include <boost/unordered_map.hpp>
#include <cstdlib>

using namespace ceammc;
using namespace ceammc::music;

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

//size_t ceammc::music::hash_value(const Tonality& c)
//{
//    size_t seed = 0;
//    boost::hash_combine(seed, c.pitch());
//    boost::hash_combine(seed, c.modus());
//    return seed;
//}

//size_t ceammc::music::hash_value(const PitchClass& c)
//{
//    size_t seed = 0;
//    boost::hash_combine(seed, c.pitchName());
//    boost::hash_combine(seed, c.alteration());
//    return seed;
//}

//size_t ceammc::music::hash_value(const Alteration& a)
//{
//    return boost::hash_value(a.semitones());
//}
