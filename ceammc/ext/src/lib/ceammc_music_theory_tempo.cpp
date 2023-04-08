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
#include "lex/parser_music.h"

#include <iostream>

using namespace ceammc::music;

constexpr int TEMPO_MAX_DOTS = 3;

Tempo::Tempo() noexcept
    : bpm_(60)
    , dur_ { 1, 4 }
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
    return dur_.strictEqual(t.dur_);
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
    return dur_.setDots(dots);
}

bool Tempo::setDivision(int div) noexcept
{
    return dur_.setDivision(div);
}

bool Tempo::set(float bpm, int div, int dots) noexcept
{
    if (!setBpm(bpm))
        return false;

    return dur_.set(1, div, dots);
}

bool Tempo::parse(const char* str)
{
    parser::BpmFullMatch p;
    parser::Bpm bpm;
    if (!p.parse(str, bpm))
        return false;

    Tempo tmp;
    if (!tmp.setBpm(bpm.bpm))
        return false;

    if (!tmp.dur_.set(bpm.beat_num, bpm.beat_div, 0))
        return false;

    *this = tmp;
    return true;
}

std::string Tempo::toString() const
{
    const char* DOTS = &"..."[TEMPO_MAX_DOTS - dur_.dots()];
    if (dur_.numerator() == 1)
        return fmt::format("{}|{}{}bpm", bpm_, dur_.division(), DOTS);
    else
        return fmt::format("{}|{}bpm", bpm_, dur_.toString());
}

Duration Tempo::beatDuration() const
{
    return dur_;
}

Duration Tempo::beatSubDivDuration() const
{
    return dur_.subDivision();
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

double Tempo::wholeNoteDurationMs() const
{
    return Duration { 1, 1 }.timeMs(*this);
}

std::ostream& ceammc::music::operator<<(std::ostream& os, const Tempo& t)
{
    os << t.toString();
    return os;
}
