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

#include "muParser.h"

#include <algorithm>
#include <limits>
#include <stdexcept>

namespace ceammc {

static float foldFloat(float x, float max)
{
    auto max2 = 2 * max;
    auto w = wrapFloatMax(x, max2);
    return std::min<float>(max2 - w, w);
}

static inline t_sample interpLinear(t_sample x0, t_sample x1, t_sample t)
{
    return x0 + t * (x1 - x0);
}

static t_sample interpCubicHermite(t_sample x0, t_sample x1, t_sample x2, t_sample x3, t_sample t)
{
    const t_sample a = -x0 / 2 + (3 * x1) / 2 - (3 * x2) / 2 + x3 / 2;
    const t_sample b = x0 - (5 * x1) / 2 + 2 * x2 - x3 / 2;
    const t_sample c = -x0 / 2 + x2 / 2;
    const t_sample d = x1;

    return a * t * t * t + b * t * t + c * t + d;
}

Grain::Grain()
    : pan_(0)
    , pan_norm_(0.5)
    , pan_overflow_(PAN_OVERFLOW_CLIP)
    , pan_mode_(PAN_MODE_LINEAR)
    , play_interp_(INTERP_NO)
{
}

Grain::Grain(size_t array_pos, size_t length, size_t play_pos)
    : pan_(0)
    , pan_norm_(0.5)
    , pan_overflow_(PAN_OVERFLOW_CLIP)
    , pan_mode_(PAN_MODE_LINEAR)
    , play_interp_(INTERP_NO)
{
    array_pos_samp = array_pos;
    length_samp = length;
    play_pos_samp = play_pos;
}

void Grain::initParserVars(mu::Parser& p)
{
    p.DefineNameChars("$0123456789_abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ");
    p.DefineConst("$amp", amplitude());
    p.DefineConst("$n", play_counter_);
    p.DefineConst("$pan", pan());
    p.DefineConst("$speed", speed());
    p.DefineConst("$sr", sys_getsr());
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

void Grain::setSpeedExpr(const std::string& expr)
{
    try {
        mu::Parser p;
        initParserVars(p);
        p.SetExpr(expr);
        setSpeed(p.Eval());
    } catch (mu::ParserError& e) {
        LIB_ERR << "error while evaluting \"" << expr << "\": " << e.GetMsg();
    }
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

void Grain::setPanMode(Grain::PanMode m)
{
    pan_mode_ = m;
}

void Grain::setAmplitude(float amp)
{
    amp_ = clip<float>(amp, amp_min_, amp_max_);
}

void Grain::setAmplitudeRange(float min, float max)
{
    auto mm = std::minmax(min, max);
    amp_min_ = mm.first;
    amp_max_ = mm.second;
}

void Grain::start(size_t playPosSamp)
{
    play_status = PLAYING;

    if (play_speed_ < 0) {
        const auto l = lengthInSamples();
        if (l < 1)
            play_pos = playPosSamp;
        else
            play_pos = playPosSamp + (l - 1);
    } else
        play_pos = playPosSamp;
}

Grain::PlayStatus Grain::done()
{
    play_status = FINISHED;
    play_counter_++;

    if (amp_done_)
        setAmplitude(amp_done_(this));

    if (pan_done_)
        setPan(pan_done_());

    if (speed_done_)
        setSpeed(speed_done_(this));

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
       << ",amp=" << g.amplitude()
       << ")";
    return os;
}

Grain::PlayStatus Grain::process(ArrayIterator in, size_t in_size, t_sample** buf, size_t bs)
{
    // invalid
    if (play_status == FINISHED) {
        play_status = FINISHED;
        return playStatus();
    }

    // zero speed or zero amp
    if (std::fabs(play_speed_) < 0.001 || amp_ < 0.00001)
        return done();

    const auto pan_coeffs = panSample(1);
    const double step_incr = play_speed_;

    if (play_speed_ > 0) {
        for (size_t i = 0; i < bs; i++, play_pos += step_incr) {
            if (play_pos < startInSamples()) {
                continue;
            } else if (play_pos >= endInSamples()) {
                return done();
            } else
                play_status = PLAYING;

            assert(play_pos >= startInSamples());

            // time position
            const double play_idx = arrayPosInSamples() + play_pos - startInSamples();
            if (play_idx >= in_size)
                return done();

            assert(play_idx >= 0);

            t_sample value = 0;
            const auto idx = static_cast<size_t>(play_idx);
            switch (play_interp_) {
            case INTERP_LINEAR: {
                const auto x0 = in[idx];
                const auto x1 = (idx + 1 >= in_size) ? x0 : in[idx + 1];
                const double t1 = play_idx - double(idx); // fractional part
                value = interpLinear(x0, x1, t1);
            } break;
            case INTERP_CUBIC: {
                const auto x0 = (idx < 1) ? in[idx] : in[idx - 1];
                const auto x1 = in[idx];
                const auto x2 = (idx + 1 >= in_size) ? x1 : in[idx + 1];
                const auto x3 = (idx + 2 >= in_size) ? x2 : in[idx + 2];
                const double t1 = play_idx - double(idx); // fractional part
                value = interpCubicHermite(x0, x1, x2, x3, t1);
            } break;
            case INTERP_NO:
            default:
                value = in[idx];
                break;
            }

            const auto vamp = value * amp_;

            buf[0][i] += pan_coeffs.first * vamp;
            buf[1][i] += pan_coeffs.second * vamp;
        }

        if (play_pos >= endInSamples())
            return done();
    } else {

        for (size_t i = 0; i < bs; i++, play_pos += step_incr) {
            if (play_pos < startInSamples())
                return done();
            else
                play_status = PLAYING;

            assert(play_pos >= startInSamples());

            // time position
            const double t = arrayPosInSamples() + play_pos - startInSamples();
            if (t >= in_size)
                return done();

            assert(t >= 0);

            t_sample value = 0;
            const auto idx = static_cast<size_t>(t);
            switch (play_interp_) {
            case INTERP_LINEAR: {
                const auto x0 = in[idx];
                const auto x1 = (idx + 1 >= in_size) ? x0 : in[idx + 1];
                const double t1 = t - double(idx); // fractional part
                value = interpLinear(x0, x1, t1);
            } break;
            case INTERP_CUBIC: {
                const auto x0 = (idx < 1) ? in[idx] : in[idx - 1];
                const auto x1 = in[idx];
                const auto x2 = (idx + 1 >= in_size) ? x1 : in[idx + 1];
                const auto x3 = (idx + 2 >= in_size) ? x2 : in[idx + 2];
                const double t1 = t - double(idx); // fractional part
                value = interpCubicHermite(x0, x1, x2, x3, t1);
            } break;
            case INTERP_NO:
            default:
                value = in[idx];
                break;
            }

            const auto vamp = value * amp_;

            buf[0][i] += pan_coeffs.first * vamp;
            buf[1][i] += pan_coeffs.second * vamp;
        }

        if (play_pos < startInSamples())
            return done();
    }

    return PLAYING;
}
}
