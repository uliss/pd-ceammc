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
#include "ceammc_music_theory_duration.h"
#include "ceammc_music_theory_tempo.h"
#include "fmt/core.h"
#include "lex/parser_music.h"

#include <boost/rational.hpp>
#include <stdexcept>

using Rational = boost::rational<int>;

using namespace ceammc::music;

namespace {
template <int MIN, int MAX>
int clip_minmax(int v)
{
    return std::max(MIN, std::min(MAX, v));
}

Rational dur2ratio(const Duration& dur)
{
    Rational res(dur.numerator(), dur.division());

    switch (dur.dots()) {
    case 1:
        res *= { 3, 2 };
        break;
    case 2:
        res *= { 7, 4 };
        break;
    case 3:
        res *= { 15, 8 };
        break;
    default:
        break;
    }

    return res;
}
}

Duration::Duration() noexcept
    : num_ { 1 }
    , div_ { 4 }
    , dots_ { 0 }
{
}

Duration::Duration(int div)
    : num_ { 1 }
    , div_ { div }
    , dots_ { 0 }
{
    if (div_ == 0)
        throw std::invalid_argument("zero duration division");
}

Duration::Duration(int num, int div, int dots)
    : num_ { num }
    , div_ { div }
    , dots_ { clip_minmax<0, DURATION_MAX_DOTS>(dots) }
{
    if (div_ == 0)
        throw std::invalid_argument("zero duration division");
}

bool Duration::operator==(const Duration& dur) const noexcept
{
    if (&dur == this)
        return true;

    return normalized().strictEqual(dur.normalized());
}

bool Duration::operator<(const Duration& dur) const noexcept
{
    return dur2ratio(*this) < dur2ratio(dur);
}

bool Duration::operator<=(const Duration& dur) const noexcept
{
    return dur2ratio(*this) <= dur2ratio(dur);
}

bool Duration::operator>(const Duration& dur) const noexcept
{
    return dur2ratio(*this) > dur2ratio(dur);
}

bool Duration::operator>=(const Duration& dur) const noexcept
{
    return dur2ratio(*this) >= dur2ratio(dur);
}

Duration Duration::normalized() const
{
    auto res = dur2ratio(*this);
    return { res.numerator(), res.denominator() };
}

Duration Duration::subDivision() const
{
    if (dots_ > 0) {
        auto res = dur2ratio(*this);
        return { 1, res.denominator() };
    } else {
        return { 1, div_ };
    }
}

bool Duration::strictEqual(const Duration& dur) const
{
    return num_ == dur.num_
        && div_ == dur.div_
        && dots_ == dur.dots_;
}

bool Duration::setNumerator(int num) noexcept
{
    num_ = num;
    return true;
}

bool Duration::setDivision(int div) noexcept
{
    if (div == 0) {
        return false;
    } else {
        div_ = div;
        return true;
    }
}

bool Duration::setDots(int dots) noexcept
{
    if (dots < 0 || dots > DURATION_MAX_DOTS) {
        return false;
    } else {
        dots_ = dots;
        return true;
    }
}

bool Duration::set(int num, int div, int dots) noexcept
{
    if (!setNumerator(num))
        return false;

    if (!setDivision(div))
        return false;

    if (!setDots(dots))
        return false;

    return true;
}

std::string Duration::toString() const
{
    const char* dots = &"..."[(DURATION_MAX_DOTS - clip_minmax<0, DURATION_MAX_DOTS>(dots_))];
    return fmt::format("{}/{}{}", num_, div_, dots);
}

double Duration::ratio() const noexcept
{
    double res = double(num_) / div_;
    switch (dots_) {
    case 1:
        return res * 1.5;
    case 2:
        return res * 1.75;
    case 3:
        return res * 1.875;
    default:
        return res;
    }
}

Duration Duration::operator+(const Duration& dur) const
{
    auto res = dur2ratio(*this) + dur2ratio(dur);
    return { res.numerator(), res.denominator() };
}

Duration& Duration::operator+=(const Duration& dur)
{
    auto res = dur2ratio(*this) + dur2ratio(dur);
    num_ = res.numerator();
    div_ = res.denominator();
    dots_ = 0;
    return *this;
}

Duration Duration::operator-(const Duration& dur) const
{
    auto res = dur2ratio(*this) - dur2ratio(dur);
    return { res.numerator(), res.denominator() };
}

Duration Duration::operator*(int k) const
{
    auto res = dur2ratio(*this) * k;
    return { res.numerator(), res.denominator() };
}

Duration Duration::operator/(int div) const
{
    if (div == 0)
        throw std::invalid_argument("division by zero");

    auto res = dur2ratio(*this) / div;
    return { res.numerator(), res.denominator() };
}

double Duration::timeMs(const Tempo& t) const
{
    auto d = dur2ratio(*this) / dur2ratio(t.beatDuration());
    return t.beatDurationMs() * d.numerator() / d.denominator();
}

bool Duration::parse(const char* str) noexcept
{
    parser::DurationFullMatch p;
    if (!p.parse(str))
        return false;

    auto& dur = p.result();
    if (!dur.isAbs())
        return false;

    num_ = dur.num;
    if (dur.repeats > 0)
        num_ += dur.repeats;

    div_ = dur.den;
    dots_ = dur.dots;
    return true;
}

Duration Duration::operator*(const Duration& dur) const
{
    auto res = dur2ratio(*this) * dur2ratio(dur);
    return { res.numerator(), res.denominator() };
}

std::ostream& ceammc::music::operator<<(std::ostream& os, const Duration& dur)
{
    os << dur.toString();
    return os;
}
