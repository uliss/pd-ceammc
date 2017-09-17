#include "ceammc_music_theory_pitch_name.h"

#include <algorithm>
#include <cstdlib>

using namespace ceammc::music;

const PitchName PitchName::C(PITCH_NAME_C);
const PitchName PitchName::D(PITCH_NAME_D);
const PitchName PitchName::E(PITCH_NAME_E);
const PitchName PitchName::F(PITCH_NAME_F);
const PitchName PitchName::G(PITCH_NAME_G);
const PitchName PitchName::A(PITCH_NAME_A);
const PitchName PitchName::B(PITCH_NAME_B);

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
