/*****************************************************************************
 * Copyright 2023 Serge Poltavski. All rights reserved.
 *
 * This file may be distributed under the terms of GNU Public License version
 * 3 (GPL v3) as defined by the Free Software Foundation (FSF). A copy of the
 * license should have been included with this file, or the project in which
 * this file belongs to. You may also find the details of GPL v3 at:
 * http://www.gnu.org/licenses/gpl-3.0.txt
 *
 * If you have any questions regarding the use of this file, feel free to
 * contact the author of this file, or the owner of the project in which
 * this file belongs to.
 *****************************************************************************/
#include "ceammc_music_spn.h"
#include "fmt/core.h"

using namespace ceammc::music;

void Spn::setMidi(std::uint8_t p)
{
    dev_ = 0;
    oct_.setAbsOctave((p / 12) - 1);
    pitch_ = PitchClass(p % 12);
}

bool Spn::operator==(const Spn& spn) const
{
    return pitch_ == spn.pitch_
        && oct_ == spn.oct_
        && dev_ == spn.dev_;
}

EitherMidi Spn::toMidi(std::int8_t rel_octave) const
{
    if (is_rest_)
        return InvalidMidi {};

    auto res = oct_.toMidi(rel_octave);
    if (res.isOk())
        return res.value() + pitch_.pitch();
    else
        return res;
}

t_float Spn::asMidi(std::int8_t rel_octave) const
{
    if (is_rest_)
        return -1;

    auto res = oct_.toMidi(rel_octave);
    if (res.isOk())
        return res.value() + pitch_.pitch() + (dev_ / 100.0);
    else
        return -1;
}

std::string Spn::toString() const
{
    if (is_rest_)
        return "R";

    std::string res;
    if (isRelative()) {
        res = fmt::format("{:c}{}{}",
            pitch_.pitchName().letterName(),
            pitch_.alteration().shortName(),
            std::string(std::abs(oct_.octave()), oct_.octave() < 0 ? '_' : '^'));
    } else {
        res = fmt::format("{:c}{}{:d}",
            pitch_.pitchName().letterName(),
            pitch_.alteration().shortName(),
            oct_.octave());
    }

    if (dev_ != 0)
        res += fmt::format("({:+d}c)", dev_);

    return res;
}

int8_t Spn::nextIntervalOctave(int8_t notePrev, int8_t noteNext)
{
    const auto a = notePrev;
    const auto b = noteNext;

    if ((b - a) > 4)
        return -1;
    else if ((a - b) > 3)
        return 1;
    else
        return 0;
}

Spn Spn::makeRest()
{
    Spn res;
    res.is_rest_ = true;
    return res;
}

std::ostream& ceammc::music::operator<<(std::ostream& os, const Octave& oct)
{
    os << oct.toString();
    return os;
}

std::ostream& ceammc::music::operator<<(std::ostream& os, const Spn& spn)
{
    os << spn.toString();
    return os;
}

std::string Octave::toString() const
{
    if (isRelative()) {
        return fmt::format("{}{}", std::string(std::abs(oct_), oct_ < 0 ? '_' : '^'));
    } else {
        return fmt::format("{:d}", oct_);
    }
}
