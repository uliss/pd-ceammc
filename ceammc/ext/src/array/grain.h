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
#ifndef GRAIN_H
#define GRAIN_H

#include "ceammc_array.h"

#include <cassert>
#include <cmath>
#include <cstdint>
#include <functional>
#include <iostream>
#include <utility>

namespace mu {
class Parser;
}

namespace ceammc {

struct Grain {
public:
    enum PlayStatus : uint8_t {
        FINISHED,
        PLAYING
    };

    enum PanOverflow {
        PAN_OVERFLOW_CLIP,
        PAN_OVERFLOW_FOLD,
        PAN_OVERFLOW_WRAP
    };

    enum PanMode {
        PAN_MODE_NONE,
        PAN_MODE_LINEAR,
        PAN_MODE_SQRT
    };

    enum PlayInterp {
        INTERP_NO,
        INTERP_LINEAR,
        INTERP_CUBIC
    };

    enum WindowType {
        WIN_RECT,
        WIN_TRIANGLE,
        WIN_HANN
    };

    using AmpDoneFunc = std::function<float(Grain*)>;
    using PanDoneFunc = std::function<float()>;
    using SpeedDoneFunc = std::function<float(Grain*)>;

private:
    size_t length_samp = { 0 };
    size_t array_pos_samp = { 0 };
    size_t play_pos_samp = { 0 };
    double play_pos = { 0 };
    PlayStatus play_status = { FINISHED };

private:
    // amplitude
    float amp_ = { 1 };
    float amp_min_ = { 0 };
    float amp_max_ = { 2 };
    AmpDoneFunc amp_done_;

    // play speed
    float play_speed_ = { 1 };
    float speed_min_ = { -10 };
    float speed_max_ = { 10 };
    uint32_t play_counter_ = { 0 };
    SpeedDoneFunc speed_done_;

    // pan
    float pan_ = { 0 }; // -1: left, 0: center, +1: right
    float pan_norm_ = { 0.5 }; // 0: left, 1: right
    PanDoneFunc pan_done_;
    PanOverflow pan_overflow_ : 2;
    PanMode pan_mode_ : 2;
    PlayInterp play_interp_ : 2;
    WindowType win_type_ : 2;

public:
    Grain();
    Grain(size_t array_pos, size_t length, size_t play_pos = 0);

    // init parser vars
    void initParserVars(mu::Parser& p);

    // array pos sample
    size_t arrayPosInSamples() const { return array_pos_samp; }
    void setArrayPosInSamples(size_t p) { array_pos_samp = p; }
    size_t lengthInSamples() const
    {
        if (play_speed_ > 0)
            return length_samp / play_speed_;
        else if (play_speed_ < 0)
            return length_samp / (-play_speed_);
        else
            return 0;
    }
    void setLengthInSamples(size_t l) { length_samp = l; }

    // play
    size_t startInSamples() const { return play_pos_samp; }
    void setStartInSamples(size_t t) { play_pos_samp = t; }
    PlayInterp playInterpolation() const { return play_interp_; }
    void setPlayInterpolation(PlayInterp i) { play_interp_ = i; }

    // speed
    float speed() const { return play_speed_; }
    void addSpeed(float v) { setSpeed(speed() + v); }
    void setSpeed(float s);
    void setSpeedDone(SpeedDoneFunc fn) { speed_done_ = fn; }
    float speedMin() const { return speed_min_; }
    float speedMax() const { return speed_max_; }
    std::pair<float, float> speedRange() const { return { speed_min_, speed_max_ }; }
    void setSpeedRange(float a, float b);
    void setSpeedExpr(const std::string& expr);

    // window
    WindowType winType() const { return win_type_; }
    void setWinType(WindowType t) { win_type_ = t; }

    // pan
    float pan() const { return pan_; }
    float panNorm() const { return pan_norm_; }
    void addPan(float pan);
    void setPan(float pan);
    void setPanDone(PanDoneFunc fn) { pan_done_ = fn; }

    PanOverflow panOverflow() const { return pan_overflow_; }
    void setPanOverflow(PanOverflow po) { pan_overflow_ = po; }

    PanMode panMode() const { return pan_mode_; }
    void setPanMode(PanMode m);

    // amplitude
    float amplitude() const { return amp_; }
    void setAmplitude(float amp);
    std::pair<float, float> amplitudeRange() const { return { amp_min_, amp_max_ }; }
    void setAmplitudeRange(float min, float max);
    void setAmplitudeDone(AmpDoneFunc fn) { amp_done_ = fn; }

    std::pair<t_sample, t_sample> panSample(t_sample in) const
    {
        switch (pan_mode_) {
        case PAN_MODE_LINEAR:
            return { in * (1 - pan_norm_), in * pan_norm_ };
        case PAN_MODE_SQRT:
            return { std::sqrt(in * (1 - pan_norm_)), std::sqrt(in * pan_norm_) };
        case PAN_MODE_NONE:
        default:
            return { 1, 1 };
        }
    }

    size_t endInSamples() const
    {
        return startInSamples() + lengthInSamples();
    }

    void start(size_t playPosSamp);

    PlayStatus done();

    PlayStatus process(ArrayIterator in, size_t in_size, t_sample** buf, size_t bs);

    PlayStatus playStatus() const
    {
        return play_status;
    }

    void setPlayStatus(PlayStatus st)
    {
        play_status = st;
    }

public:
    static bool initWinTables();
};

std::ostream& operator<<(std::ostream& os, const Grain& g);

}

#endif // GRAIN_H
