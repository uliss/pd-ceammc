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
#include "grain.h"
#include "ceammc_convert.h"
#include "ceammc_log.h"

#include <algorithm>
#include <limits>

namespace ceammc {

static_assert(sizeof(Grain) == 150, "");

static float foldFloat(float x, float max)
{
    auto max2 = 2 * max;
    auto w = wrapFloatMax(x, max2);
    return std::min<float>(max2 - w, w);
}

float foldFloat(float x, float min, float max)
{
    return min + foldFloat(x - min, max - min);
}

Grain::Grain()
    : pan_(0)
    , pan_norm_(0.5)
    , pan_overflow_(PAN_OVERFLOW_CLIP)
    , pan_mode_(PAN_MODE_LINEAR)
{
}

Grain::Grain(size_t array_pos, size_t length, size_t play_pos)
    : pan_(0)
    , pan_norm_(0.5)
    , pan_overflow_(PAN_OVERFLOW_CLIP)
    , pan_mode_(PAN_MODE_LINEAR)
{
    array_pos_samp = array_pos;
    length_samp = length;
    play_pos_samp = play_pos;
}

void Grain::setSpeed(float s)
{
    play_speed_ = clip<float>(s, speed_min_, speed_max_);
}

void Grain::setSpeedRange(float a, float b)
{
    auto mm = std::minmax(a, b);
    speed_min_ = mm.first;
    speed_max_ = mm.second;
}

void Grain::addPan(float pan)
{
    setPan(pan_ + pan);
}

void Grain::setPan(float pan)
{
    static const auto onemore = std::nextafter(1.0, std::numeric_limits<float>::max());
    const float pan_norm = convert::lin2lin<float>(pan, -1, 1, 0, 1);

    switch (pan_overflow_) {
    case PAN_OVERFLOW_WRAP:
        pan_ = pan;
        pan_norm_ = wrapFloatMax<float>(pan_norm, onemore);
        break;
    case PAN_OVERFLOW_FOLD:
        pan_ = pan;
        pan_norm_ = foldFloat(pan_norm, onemore);
        break;
    default:
        pan_ = pan;
        pan_norm_ = clip<float>(pan_norm, 0, 1);
        break;
    }
}

Grain::PlayStatus Grain::done()
{
    play_status = FINISHED;

    if (pan_done_)
        setPan(pan_done_());

    if (speed_done_) {
        setSpeed(speed_done_(this));
        LIB_DBG << "new speed: " << speed();
    }

    return play_status;
}

std::ostream& operator<<(std::ostream& os, const Grain& g)
{
    os << "grain(at=" << g.arrayPosInSamples()
       << ",len=" << g.lengthInSamples()
       << ",when=" << g.startInSamples()
       << ",speed=" << g.speed()
       << ",pan=" << g.panNorm()
       << ",panmode=" << g.panMode()
       << ")";
    return os;
}

Grain::PlayStatus Grain::process(ceammc::ArrayIterator in, size_t in_size, t_sample** buf, size_t bs)
{
    // invalid
    if (play_status == FINISHED) {
        play_status = FINISHED;
        return playStatus();
    }

    // zero speed
    if (std::fabs(play_speed_) < 0.001) {
        play_status = FINISHED;
        return playStatus();
    }

    const double step_incr = play_speed_;
    const auto pan_coeffs = panSample(1);

    for (size_t i = 0; i < bs; i++, play_pos += step_incr) {
        if (play_pos < startInSamples()) {
            continue;
        } else if (play_pos >= endInSamples()) {
            return done();
        } else
            play_status = PLAYING;

        assert(play_pos >= startInSamples());

        const size_t idx = arrayPosInSamples() + play_pos - startInSamples();
        if (idx >= in_size)
            return done();

        const t_sample value = in[idx];

        buf[0][i] += pan_coeffs.first * value;
        buf[1][i] += pan_coeffs.second * value;
    }

    if (play_pos >= endInSamples()) {
        return done();
    } else
        return PLAYING;
}

}
