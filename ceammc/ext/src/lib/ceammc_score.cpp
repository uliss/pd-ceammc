/*****************************************************************************
 * Copyright 2020 Serge Poltavsky. All rights reserved.
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
#include "ceammc_score.h"

#include <limits>

namespace ceammc {

Bar::Bar(const TimeSignature& sig, float tempo)
    : sig_(sig)
    , tempo_ { tempo, 0, 0 }
{
}

double Bar::durationMs() const
{
    if (tempo_.isConstant())
        return 60000 * numBeats() / tempo_.value;
    else {
        double res = 0;
        for (size_t i = 0; i < numBeats(); i++)
            res += beatDurationMs(i);

        return res;
    }
}

double Bar::timeMsAt(BeatNumber beat) const
{
    double res = 0;
    for (size_t i = 0; i < numBeats(); i++) {
        if (i == beat)
            return res;

        res += beatDurationMs(i);
    }

    return res;
}

bool Bar::setAccel(float tempo0, float tempo1, BeatNumber start)
{
    const auto len = long(numBeats()) - long(start) - 1;
    if (len <= 0 || start < 0)
        return false;

    tempo_.slope_start = start;
    tempo_.slope = (tempo1 - tempo0) / len;
    return true;
}

Score::Score(const TimeSignature& sig, float tempo, size_t numBars)
    : bars_(numBars, Bar(sig, tempo))
{
}

size_t Score::numBars() const
{
    return bars_.size();
}

double Score::timeMsAt(size_t bar, BeatNumber bn) const
{
    if (bar >= bars_.size())
        return 0;

    if (bar == 0)
        return bars_[0].timeMsAt(bn);

    double res = 0;
    for (size_t i = 0; i < bar - 1; i++)
        res += bars_[i].durationMs();

    return res + bars_[bar].timeMsAt(bn);
}

std::ostream& operator<<(std::ostream& os, const TimeSignature& ts)
{
    os << ts.num << '/' << ts.den;
    return os;
}

}
