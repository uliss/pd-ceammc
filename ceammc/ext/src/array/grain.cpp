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
#include "ceammc_sound.h"
#include "ceammc_window.h"

#include "fmt/core.h"
#include "grain_expr_parser.h"
#include "grain_random.h"

#include <algorithm>
#include <array>
#include <boost/pool/object_pool.hpp>
#include <limits>
#include <stdexcept>

namespace ceammc {

constexpr size_t WIN_SIZE = 257;
constexpr double MIN_SPEED = -10;
constexpr double MAX_SPEED = 10;

static std::array<t_sample, WIN_SIZE + 3> win_triangle = { 0 };
static std::array<t_sample, WIN_SIZE + 3> win_hann = { 0 };
static std::array<t_sample, WIN_SIZE + 3> win_gauss = { 0 };

static const auto win_init = Grain::initWinTables();

static float foldFloat(float x, float max)
{
    auto max2 = 2 * max;
    auto w = wrapFloatMax(x, max2);
    return std::min<float>(max2 - w, w);
}

static const char* panMode2str(GrainPan pan)
{
    static const char* txt[3] = { "none", "lin", "sqrt" };
    return txt[pan];
}

static const char* interp2str(GrainInterp i)
{
    static const char* txt[3] = { "none", "lin", "cubic" };
    return txt[i];
}

static std::string tag2str(t_symbol* tag)
{
    return tag ? (std::string(1, '.') + tag->s_name) : std::string {};
}

Grain::Grain()
    : pan_(0.5)
    , state_(GRAIN_FINISHED)
    , pan_overflow_(GRAIN_PROP_OVERFLOW_CLIP)
    , pan_mode_(GRAIN_PAN_LINEAR)
    , play_interp_(GRAIN_INTERP_NONE)
    , win_type_(GRAIN_WIN_RECT)
    , pre_delay_done_(false)
{
}

Grain::Grain(size_t array_pos, size_t length, size_t pre_delay)
    : pan_(0.5)
    , state_(GRAIN_FINISHED)
    , pan_overflow_(GRAIN_PROP_OVERFLOW_CLIP)
    , pan_mode_(GRAIN_PAN_LINEAR)
    , play_interp_(GRAIN_INTERP_NONE)
    , win_type_(GRAIN_WIN_RECT)
{
    src_pos_ = array_pos;
    length_ = length;
    pre_delay_ = pre_delay;
}

bool Grain::matchTag(const std::regex& rx) const
{
    if (tag_ && tag_->s_name[0] != '\0')
        return std::regex_match(tag_->s_name, rx);
    else
        return false;
}

void Grain::setSpeed(float s)
{
    play_speed_ = clip<float>(s, MIN_SPEED, MAX_SPEED);
}

void Grain::setPan(float pan)
{
    switch (pan_overflow_) {
    case GRAIN_PROP_OVERFLOW_WRAP:
        pan_ = wrapFloatMax<float>(pan, 1);
        break;
    case GRAIN_PROP_OVERFLOW_FOLD:
        pan_ = foldFloat(pan, 1);
        break;
    case GRAIN_PROP_OVERFLOW_CLIP:
    default:
        pan_ = clip<float>(pan, 0, 1);
        break;
    }
}

void Grain::setPanOverflow(GrainPropOverflow po)
{
    pan_overflow_ = po;
}

void Grain::setPanMode(GrainPan m)
{
    pan_mode_ = m;
}

void Grain::setAmplitude(float amp)
{
    amp_ = clip<float, 0, 256>(amp);
}

void Grain::start(size_t playPosSamp)
{
    state_ = GRAIN_PLAYING;
    play_pos_ = playPosSamp;
}

GrainState Grain::done()
{
    state_ = GRAIN_FINISHED;
    cnt_repeats_++;

    if (ondone_) {
        double res = amp_;
        if (ondone_->evalPropAction(*this, GRAIN_PROP_AMP, res))
            setAmplitude(res);

        res = play_speed_;
        if (ondone_->evalPropAction(*this, GRAIN_PROP_SPEED, res))
            setSpeed(res);

        res = pan_;
        if (ondone_->evalPropAction(*this, GRAIN_PROP_PAN, res))
            setPan(res);

        res = pre_delay_;
        if (ondone_->evalPropAction(*this, GRAIN_PROP_TIME_BEFORE, res))
            setTimeBefore(std::max<double>(0, res));

        res = time_after_;
        if (ondone_->evalPropAction(*this, GRAIN_PROP_TIME_AFTER, res))
            setTimeAfter(std::max<double>(0, res));

        res = length_;
        if (ondone_->evalPropAction(*this, GRAIN_PROP_LENGTH, res))
            setLengthInSamples(std::max<double>(10, res));

        res = src_pos_;
        if (ondone_->evalPropAction(*this, GRAIN_PROP_AT, res))
            setArrayPosInSamples(std::max<double>(0, res));

        res = repeats_;
        if (ondone_->evalPropAction(*this, GRAIN_PROP_REPEATS, res))
            setRepeats(res);
    }

    play_pos_ = 0;
    pre_delay_done_ = true;
    return state_;
}

std::ostream& operator<<(std::ostream& os, const Grain& g)
{

    os << fmt::format("grain(#{}{},"
                      "@at={},@l={},@d={},@tb={},@ta={},"
                      "@amp={},@s={},@p={},@r={},@cnt={},"
                      "panmode={},"
                      "interp={})",
        g.id(),
        tag2str(g.tag()),
        g.arrayPosInSamples(),
        g.lengthInSamples(),
        g.durationInSamples(),
        g.timeBefore(),
        g.timeAfter(),
        g.amplitude(),
        g.speed(),
        g.pan(),
        g.repeats(),
        g.doneCounter(),
        panMode2str(g.panMode()),
        interp2str(g.playInterpolation()));

    return os;
}

GrainState Grain::process(ArrayIterator in, size_t in_size, t_sample** buf, uint32_t bs, uint32_t sr, uint32_t buf_offset, uint32_t* done_samp)
{
    constexpr float SLOW_LIMIT = 0.001;
    constexpr float QUIT_LIMIT = 0.00001;

    // invalid
    if (state_ == GRAIN_FINISHED || state_ == GRAIN_PAUSE)
        return state_;

    if (buf_offset < bs) {
        bs -= buf_offset;
    } else
        buf_offset = 0;

    // optimization: silence before grain
    const double next_block = play_pos_ + bs;
    if (next_block < grainStartInSamples()) {
        play_pos_ = next_block;
        return state_;
    }

    // optim: silence after grain
    if (afterGrain()) {
        play_pos_ += bs;

        // should finish
        if (shouldDone())
            return done();

        return state_;
    }

    // zero speed or zero amp
    const bool zero_speed = !mods_ && (-SLOW_LIMIT < play_speed_ && play_speed_ < SLOW_LIMIT);
    const bool zero_amp = amp_ < QUIT_LIMIT && !mods_;
    if (zero_speed || zero_amp)
        return done();

    const auto pan_coeffs = panSample(1);
    const double step_incr = std::abs(play_speed_);

    // before grain fill with silence
    size_t i = 0;
    if (beforeGrain()) {
        i = pre_delay_ - play_pos_;
        play_pos_ += i;
        if (done_samp)
            (*done_samp) += i;
    }

    for (; i < bs && play_pos_ < grainEndInSamples(); i++) {
        // only grain itself expected here, without silence before/after
        assert(play_pos_ >= grainStartInSamples() && play_pos_ < grainEndInSamples());

        // increment done samples
        if (done_samp)
            (*done_samp)++;

        // array play position
        const double arr_idx = currentArrayPlayPos();
        if (arr_idx < 0 || arr_idx >= in_size)
            return done();

        assert(arr_idx >= 0 && arr_idx < in_size);

        t_sample value = 0;
        const auto idx = static_cast<size_t>(arr_idx);

        switch (play_interp_) {
        case GRAIN_INTERP_LINEAR: {
            const auto x0 = in[idx];
            const auto x1 = (idx + 1 >= in_size) ? x0 : in[idx + 1];
            const double t1 = arr_idx - double(idx); // fractional part
            value = interpolate::linear<double>(x0, x1, t1);
        } break;
        case GRAIN_INTERP_CUBIC: {
            const auto x0 = (idx < 1) ? in[idx] : in[idx - 1];
            const auto x1 = in[idx];
            const auto x2 = (idx + 1 >= in_size) ? x1 : in[idx + 1];
            const auto x3 = (idx + 2 >= in_size) ? x2 : in[idx + 2];
            const double t1 = arr_idx - double(idx); // fractional part
            value = interpolate::cubic_hermite<double>(x0, x1, x2, x3, t1);
        } break;
        case GRAIN_INTERP_NONE:
        default:
            value = in[idx];
            break;
        }

        // apply window
        if (win_type_ != GRAIN_WIN_RECT) {
            const double win_fpos = convert::lin2lin_clip<double>(currentLogicPlayPos(), 0, length_ - 1, 0, WIN_SIZE - 1);
            const double win_ipos = static_cast<size_t>(win_fpos);
            const double win_t = win_fpos - static_cast<size_t>(win_ipos); // fractional part

            switch (win_type_) {
            case GRAIN_WIN_TRI:
                value *= interpolate::linear<t_sample>(
                    win_triangle[win_ipos + 0],
                    win_triangle[win_ipos + 1],
                    win_t);
                break;
            case GRAIN_WIN_HANN:
                value *= interpolate::linear<t_sample>(
                    win_hann[win_ipos + 0],
                    win_hann[win_ipos + 1],
                    win_t);
                break;
            case GRAIN_WIN_TRPZ: {
                const auto pos = currentLogicPlayPos();
                const int param = (win_param_ <= 0) ? 512 : win_param_;
                const auto RAMP_SAMP = std::min<double>(param, length_ * 0.25);
                if (pos < RAMP_SAMP)
                    value *= convert::lin2lin_clip<t_sample>(pos, 0, RAMP_SAMP, 0, 1);
                else if (pos > length_ - RAMP_SAMP - 1)
                    value *= convert::lin2lin_clip<t_sample>(length_ - pos - 1, 0, RAMP_SAMP, 0, 1);
            } break;
            case GRAIN_WIN_LINUP: {
                const auto pos = currentLogicPlayPos();
                const int param = (win_param_ <= 0) ? 64 : win_param_;
                const auto RAMP_DOWN_SAMP = std::min<double>(param, length_ * 0.125);
                const double ramp_down = length_ - RAMP_DOWN_SAMP - 1;
                if (pos > ramp_down)
                    value *= convert::lin2lin_clip<t_sample>(length_ - pos - 1, 0, RAMP_DOWN_SAMP, 0, 1);
                else
                    value *= convert::lin2lin_clip<t_sample>(pos, 0, ramp_down, 0, 1);
            } break;
            case GRAIN_WIN_LINDOWN: {
                const auto pos = currentLogicPlayPos();
                const int param = (win_param_ <= 0) ? 64 : win_param_;
                const auto RAMP_UP_SAMP = std::min<double>(param, length_ * 0.125);
                if (pos < RAMP_UP_SAMP)
                    value *= convert::lin2lin_clip<t_sample>(pos, 0, RAMP_UP_SAMP, 0, 1);
                else
                    value *= convert::lin2lin_clip<t_sample>(pos - RAMP_UP_SAMP, RAMP_UP_SAMP, length_ - 1, 1, 0);

            } break;
            default:
                break;
            }
        }

        if (mods_) {
            const double t = play_pos_ - pre_delay_;
            if (mods_->modAmp())
                amp_ = mods_->mod(GRAIN_PROP_AMP, sr, t);

            if (mods_->modSpeed())
                play_speed_ = mods_->mod(GRAIN_PROP_SPEED, sr, t);

            if (mods_->modPan())
                pan_ = mods_->mod(GRAIN_PROP_PAN, sr, t);
        }

        // apply amp
        const auto vamp = value * amp_;

        // apply pan
        buf[0][i + buf_offset] += pan_coeffs.first * vamp;
        buf[1][i + buf_offset] += pan_coeffs.second * vamp;

        play_pos_ += step_incr;

        if (shouldDone())
            return done();
    }

    if (shouldDone())
        return done();
    else if (afterGrain()) {
        auto left_samp = bs - i;
        play_pos_ += left_samp;
        if (done_samp && time_after_ <= left_samp)
            (*done_samp) += time_after_;

        // need to check again
        if (shouldDone())
            return done();
    }

    return GRAIN_PLAYING;
}

void Grain::setOnDone(GrainPropId id, const ByteCode& bc)
{
    if (!ondone_)
        ondone_.reset(new GrainPropActions);

    ondone_->setAction(id, bc);
}

void Grain::initByteCodeConst(ByteCode& bc) const
{
    bc.setConst(0, amp_);
    bc.setConst(1, src_pos_);
    bc.setConst(2, length_);
    bc.setConst(3, pan_);
    bc.setConst(4, play_speed_);
    bc.setConst(5, pre_delay_);
    bc.setConst(6, sys_getsr());
    bc.setConst(7, 64);
    bc.setConst(8, array_size_);
    bc.setConst(9, cnt_repeats_);
    bc.setConst(10, 0);
    bc.setConst(11, 0);
    bc.setConst(12, id_);
    bc.setConst(13, time_after_);
}

void Grain::setModulation(GrainPropId id, const GrainPropModulator& mod)
{
    if (!mods_)
        mods_.reset(new GrainPropMods);

    switch (id) {
    case GRAIN_PROP_AMP:
        mods_->setAmp(mod);
        break;
    case GRAIN_PROP_SPEED:
        mods_->setSpeed(mod);
        break;
    case GRAIN_PROP_PAN:
        mods_->setPan(mod);
        break;
    default:
        LIB_ERR << "unsupported prop to modulate: " << id;
        break;
    }
}

bool Grain::hasModulation(GrainPropId id) const
{
    return mods_ && mods_->hasModulation(id);
}

bool Grain::initWinTables()
{
    window::fill(win_triangle.begin(), win_triangle.begin() + WIN_SIZE, &window::triangle<t_sample>);
    window::fill(win_hann.begin(), win_hann.begin() + WIN_SIZE, &window::hann<t_sample>);
    window::fill(win_gauss.begin(), win_gauss.begin() + WIN_SIZE, &window::gauss<t_sample, 1>);

    return true;
}

static boost::object_pool<GrainPropAct> grain_calc_pool;

GrainPropAct* GrainPropAct::make(const ByteCode& bc)
{
    auto res = grain_calc_pool.construct();
    res->setByteCode(bc);
    return res;
}

void GrainPropAct::free(GrainPropAct* act)
{
    if (act != nullptr)
        grain_calc_pool.free(act);
}

GrainPropActions::~GrainPropActions()
{
    for (size_t i = 0; i < NPROPS; i++)
        GrainPropAct::free(acts[i]);
}

bool GrainPropActions::evalPropAction(const Grain& grain, GrainPropId id, double& res)
{
    auto act = acts[id];
    if (!act)
        return false;

    auto& bc = act->byteCode();
    grain.initByteCodeConst(bc);
    return bc.eval(res);
}

void GrainPropActions::setAction(GrainPropId id, const ByteCode& bc)
{
    if (!acts[id])
        acts[id] = GrainPropAct::make(bc);
    else
        acts[id]->setByteCode(bc);
}

void GrainPropActions::removeAction(GrainPropId id)
{
    if (acts[id]) {
        GrainPropAct::free(acts[id]);
        acts[id] = nullptr;
    }
}

double GrainPropMods::mod(GrainPropId prop, double sr, double t)
{
    switch (prop) {
    case GRAIN_PROP_AMP:
        return amp_.mod(sr, t);
    case GRAIN_PROP_SPEED:
        return speed_.mod(sr, t);
    case GRAIN_PROP_PAN:
        return pan_.mod(sr, t);
    default:
        return t;
    }
}

bool GrainPropMods::hasModulation(GrainPropId prop) const
{
    switch (prop) {
    case GRAIN_PROP_AMP:
    case GRAIN_PROP_SPEED:
    case GRAIN_PROP_PAN:
        return true;
    default:
        return false;
    }
}

double GrainPropModulator::mod(double sr, double t)
{
    static const auto two_pi = 2 * std::acos(-1);

    if (freq_ <= 0)
        return 0;

    switch (mtype_) {
    case GRAIN_MOD_SIN:
        return convert::lin2lin<double, -1, 1>(std::sin(two_pi * freq_ * t / sr), min_, max_);
    case GRAIN_MOD_SAWUP:
        return convert::lin2lin<double, 0, 1>(std::fmod(freq_ * t / sr, 1), min_, max_);
    case GRAIN_MOD_SAWDOWN:
        return convert::lin2lin<double, 0, 1>(1 - std::fmod(freq_ * t / sr, 1), min_, max_);
    case GRAIN_MOD_SQR:
        return convert::lin2lin<double, 0, 1>(std::fmod(freq_ * t / sr, 1) >= 0.5 ? 1 : 0, min_, max_);
    case GRAIN_MOD_TRI:
        return convert::lin2lin<double, 0, 1>(std::abs(std::fmod(2 * freq_ * t / sr, 2) - 1), min_, max_);
    default:
        return 0;
    }
}

}
