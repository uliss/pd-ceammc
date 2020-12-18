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
#ifndef CEAMMC_SCORE_H
#define CEAMMC_SCORE_H

#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

namespace ceammc {

class TimeSignature {
public:
    TimeSignature(uint16_t n, uint16_t d)
        : num(n)
        , den(d)
    {
    }

    bool operator==(const TimeSignature& ts) const { return ts.num == num && ts.den == den; }

    uint16_t num;
    uint16_t den;
};

std::ostream& operator<<(std::ostream& os, const TimeSignature& ts);

using BeatNumber = uint16_t;

class AutomatizationValue {
public:
    float value;
    float slope;
    BeatNumber slope_start;

    float slopeAt(BeatNumber bn) const { return (bn < slope_start) ? 0 : slope * (bn - slope_start); }
    float valueAt(BeatNumber bn) const { return value + slopeAt(bn); }

    bool isConstant() const { return slope == 0; }
};

class Bar {
    TimeSignature sig_ = { 4, 4 };
    AutomatizationValue tempo_ = { 60, 0, 0 };
    uint16_t number_ = { 0 };
    uint8_t beat_subdivision_ = { 2 };
    double offset_ = { 0 };

public:
    Bar() { }
    Bar(const TimeSignature& sig, float tempo);

    size_t numBeats() const { return sig_.num; }
    size_t numSubBeats() const { return numBeats() * beat_subdivision_; }
    double tempoSlopeAt(BeatNumber beat) const { return tempo_.slopeAt(beat); }
    double tempoAt(BeatNumber beat) const { return tempo_.valueAt(beat); }
    double beatDurationMs(BeatNumber beat) const { return 60000.0 / tempoAt(beat); }
    double subbeatDurationMs(BeatNumber beat) const { return beatDurationMs(beat) / beat_subdivision_; }
    double durationMs() const;
    double timeMsAt(BeatNumber beat) const;

    const TimeSignature& sig() const { return sig_; }

    bool setAccel(float tempo0, float tempo1, BeatNumber start = 0);
};

class Score {
    std::string title_;
    std::string author_;
    std::vector<Bar> bars_;

public:
    Score() { }
    Score(const TimeSignature& sig, float tempo, size_t numBars = 60);

    size_t numBars() const;
    const Bar& barAt(size_t n) const { return bars_.at(n); }

    double timeMsAt(size_t bar, BeatNumber bn = 0) const;
};

}

#endif // CEAMMC_SCORE_H
