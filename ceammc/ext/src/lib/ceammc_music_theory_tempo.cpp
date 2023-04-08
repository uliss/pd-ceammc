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
#include "ceammc_music_theory_tempo.h"
#include "fmt/core.h"

#include <iostream>

using namespace ceammc::music;

constexpr int TEMPO_MAX_DOTS = 3;

Tempo::Tempo() noexcept
    : bpm_(60)
    , div_(4)
    , dots_(0)
{
}

Tempo::Tempo(float bpm, int div, int dots)
{
    if (!setBpm(bpm))
        throw std::invalid_argument("invalid BPM value");

    if (!setDivision(div))
        throw std::invalid_argument("invalid tempo beat division");

    if (!setDots(dots))
        throw std::invalid_argument("invalid dots");
}

bool Tempo::operator==(const Tempo& t) const
{
    return t.bpm_ == bpm_;
}

bool Tempo::strictEqual(const Tempo& t) const
{
    return bpm_ == t.bpm_ && div_ == t.div_ && dots_ == t.dots_;
}

Tempo Tempo::normalized() const
{
    auto dur = beatDuration().normalized();
    return { bpm_ * dur.numerator(), dur.division(), 0 };
}

bool Tempo::setBpm(float bpm) noexcept
{
    if (bpm <= 0) {
        return false;
    } else {
        bpm_ = bpm;
        return true;
    }
}

bool Tempo::setDots(int dots) noexcept
{
    if (dots < 0 || dots > TEMPO_MAX_DOTS) {
        return false;
    } else {
        dots_ = dots;
        return true;
    }
}

bool Tempo::setDivision(int div) noexcept
{
    if (div <= 0) {
        return false;
    } else {
        div_ = div;
        return true;
    }
}

bool Tempo::set(float bpm, int div, int dots) noexcept
{
    if (!setBpm(bpm))
        return false;

    if (!setDivision(div))
        return false;

    if (!setDots(dots))
        return false;

    return true;
}

std::string Tempo::toString() const
{
    const char* DOTS = &"..."[TEMPO_MAX_DOTS - dots_];
    return fmt::format("{}|{}{}bpm", bpm_, div_, DOTS);
}

Duration Tempo::beatDuration() const
{
    return Duration(1, div_, dots_);
}

Duration Tempo::beatSubDivDuration() const
{
    return Duration(1, div_, dots_).subDivision();
}

int Tempo::beatSubDivision() const
{
    return beatDuration().normalized().numerator();
}

double Tempo::beatDurationMs() const
{
    return 60000.0 / bpm_;
}

double Tempo::subBeatDurationMs() const
{
    return beatDurationMs() / beatSubDivision();
}

std::ostream& ceammc::music::operator<<(std::ostream& os, const Tempo& t)
{
    os << t.toString();
    return os;
}
