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
#include "ceammc_music_theory_timesig.h"

#include <boost/integer/common_factor.hpp>
#include <iostream>

using namespace ceammc::music;

TimeSignature::TimeSignature() noexcept
{
    sig_.emplace_back(4, 4, 0);
}

TimeSignature::TimeSignature(int num, int div)
{
    Duration dur(num, div);
    sig_.push_back(dur);
}

bool TimeSignature::set(int num, int div)
{
    Duration dur;
    if (!dur.set(num, div, 0))
        return false;

    sig_.assign(1, dur);
    return true;
}

bool TimeSignature::append(int num, int div)
{
    Duration dur;
    if (!dur.set(num, div, 0))
        return false;

    sig_.push_back(dur);
    return true;
}

std::string TimeSignature::toString() const
{
    std::string res = "|";
    for (auto& ts : sig_) {
        res += ts.toString();
        res += '+';
    }

    if (res.size() > 0 && res.back() == '+')
        res.back() = '|';

    return res;
}

Duration TimeSignature::duration() const
{
    if (sig_.size() == 1)
        return sig_[0];

    Duration dur { 0, 1 };
    for (auto& s : sig_) {
        dur += s;
    }

    return dur;
}

int TimeSignature::subDivision() const
{
    int lcm = 1;
    for (auto& s : sig_)
        lcm = boost::integer::lcm(lcm, s.division());

    return lcm;
}

double TimeSignature::timeMs(const Tempo& t) const
{
    return duration().timeMs(t);
}

BeatList TimeSignature::beatList() const
{
    bool downb_flag = true;
    BeatList res;
    for (auto& s : sig_) {
        auto DIV = std::uint8_t(s.division());
        auto N = s.numerator();
        for (int i = 0; i < N; i++) {
            if (i == 0) {
                res.push_back({ downb_flag ? BEAT_DOWN : BEAT_ON, DIV });
                downb_flag = false;
            } else {
                if (N == 5 && i == 3)
                    res.push_back({ BEAT_ON, DIV });
                else if (N == 7 && (i == 3 || i == 5))
                    res.push_back({ BEAT_ON, DIV });
                else if (N == 8 && i == 4)
                    res.push_back({ BEAT_ON, DIV });
                else if (N == 9 && (i == 3 || i == 6))
                    res.push_back({ BEAT_ON, DIV });
                else
                    res.push_back({ BEAT_OFF, DIV });
            }
        }
    }

    return res;
}

bool BeatData::operator==(const BeatData& b) const
{
    return type == b.type && division == b.division;
}

std::ostream& ceammc::music::operator<<(std::ostream& os, const TimeSignature& ts)
{
    os << ts.toString();
    return os;
}
