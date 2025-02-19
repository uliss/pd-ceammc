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

#include "byte_code.h"
#include "ceammc_array.h"

#include <cassert>
#include <cmath>
#include <cstdint>
#include <functional>
#include <iostream>
#include <memory.h>
#include <regex>
#include <utility>

namespace ceammc {

enum GrainCalcMoment : uint8_t {
    GRAIN_CALC_ONCE,
    GRAIN_CALC_ONDONE,
    GRAIN_CALC_ONBLOCK
};

enum GrainPropOverflow : uint8_t {
    GRAIN_PROP_OVERFLOW_CLIP,
    GRAIN_PROP_OVERFLOW_FOLD,
    GRAIN_PROP_OVERFLOW_WRAP
};

enum GrainPropId : uint8_t {
    GRAIN_PROP_AMP = 0,
    GRAIN_PROP_AT,
    GRAIN_PROP_LENGTH,
    GRAIN_PROP_PAN,
    GRAIN_PROP_SPEED,
    GRAIN_PROP_TIME_BEFORE,
    GRAIN_PROP_TIME_AFTER,
    GRAIN_PROP_TAG,
    GRAIN_PROP_REPEATS,
    GRAIN_PROP_NONE
};

enum GrainWindowType : uint8_t {
    GRAIN_WIN_RECT,
    GRAIN_WIN_TRI,
    GRAIN_WIN_HANN,
    GRAIN_WIN_TRPZ,
    GRAIN_WIN_LINUP,
    GRAIN_WIN_LINDOWN,
};

enum GrainState : uint8_t {
    GRAIN_FINISHED,
    GRAIN_PAUSE,
    GRAIN_PLAYING,
};

enum GrainPan : uint8_t {
    GRAIN_PAN_NONE,
    GRAIN_PAN_LINEAR,
    GRAIN_PAN_SQRT
};

enum GrainInterp : uint8_t {
    GRAIN_INTERP_NONE,
    GRAIN_INTERP_LINEAR,
    GRAIN_INTERP_CUBIC
};

enum GrainModulation : uint8_t {
    GRAIN_MOD_NONE,
    GRAIN_MOD_SIN,
    GRAIN_MOD_TRI,
    GRAIN_MOD_SQR,
    GRAIN_MOD_SAWUP,
    GRAIN_MOD_SAWDOWN,
};

class GrainPropAct {
    ByteCode bc_;

public:
    GrainPropAct() = default;
    GrainPropAct(const ByteCode& bc)
        : bc_(bc)
    {
    }

    ByteCode& byteCode() { return bc_; }
    const ByteCode& byteCode() const { return bc_; }

    void setByteCode(const ByteCode& bc)
    {
        bc_ = bc;
    }

    static GrainPropAct* make(const ByteCode& bc);

    static void free(GrainPropAct* act);
};

class GrainExprParser;
class Grain;

class GrainPropActions {
    static const size_t NPROPS = 9;
    GrainPropAct* acts[NPROPS] = { 0 };

public:
    GrainPropActions() = default;
    ~GrainPropActions();

    bool evalPropAction(const Grain& grain, GrainPropId id, double& res);

    void setAction(GrainPropId id, const ByteCode& bc);
    void removeAction(GrainPropId id);
};

class GrainPropModulator {
    float freq_;
    float min_, max_;
    GrainModulation mtype_;

public:
    GrainPropModulator(GrainModulation m = GRAIN_MOD_NONE, float freq = 0, float min = 0, float max = 1)
        : freq_(freq)
        , min_(min)
        , max_(max)
        , mtype_(m)
    {
    }

    double mod(double sr, double t);
    GrainModulation type() const { return mtype_; }
};

template <typename T>
std::unique_ptr<T> unique_clone(const std::unique_ptr<T>& up)
{
    if (up)
        return std::unique_ptr<T>(new T(*up));
    else
        return {};
}

class GrainPropMods {
    GrainPropModulator speed_;
    GrainPropModulator amp_;
    GrainPropModulator pan_;

public:
    GrainPropMods() = default;

    double mod(GrainPropId prop, double sr, double t);

    void setAmp(const GrainPropModulator& m) { amp_ = m; }
    void setSpeed(const GrainPropModulator& m) { speed_ = m; }
    void setPan(const GrainPropModulator& m) { pan_ = m; }

    bool modAmp() const { return amp_.type() != GRAIN_MOD_NONE; }
    bool modPan() const { return pan_.type() != GRAIN_MOD_NONE; }
    bool modSpeed() const { return speed_.type() != GRAIN_MOD_NONE; }

    bool hasModulation(GrainPropId prop) const;
};

class Grain {
private:
    size_t array_size_ = { 0 }; ///< whole array size in samples
    size_t src_pos_ = { 0 }; ///< grain position at source array in samples
    uint32_t length_ = { 0 }; ///< grain length in samples
    int32_t repeats_ = { -1 }; ///< max number of grain repeats

    uint32_t pre_delay_ = { 0 }; ///< silence before first grain playing in samples
    uint32_t time_after_ = { 0 }; ///< silence after grain in samples
    double play_pos_ = { 0 }; ///< current grain play position in samples

    // tag
    t_symbol* tag_ { nullptr };

    // actions
    std::unique_ptr<GrainPropActions> ondone_;

    // modulators
    std::unique_ptr<GrainPropMods> mods_;

    // amplitude
    float amp_ = { 1 };

    // play speed
    float play_speed_ = { 1 };
    int32_t cnt_repeats_ = { 0 }; ///< number of grain repeats

    ///< pan
    float pan_ = { 0.5 }; // 0: left, 0.5: center, 1: right

    ///< window parameters
    float win_param_ = { 0 };

    ///< grain id
    uint16_t id_ = { 0 };

    ///< flags
    GrainState state_ : 2;
    GrainPropOverflow pan_overflow_ : 2;
    GrainPan pan_mode_ : 2;
    GrainInterp play_interp_ : 2;
    GrainWindowType win_type_ : 3;
    bool pre_delay_done_ : 1; // used to play 'upbeat' before normal grain playing

    Grain& operator=(const Grain&) = delete;

public:
    Grain();

    explicit Grain(const Grain& g)
        : array_size_(g.array_size_)
        , src_pos_(g.src_pos_)
        , length_(g.length_)
        , repeats_(g.repeats_)
        , pre_delay_(g.pre_delay_)
        , time_after_(g.time_after_)
        , play_pos_(g.play_pos_)
        , tag_(g.tag_)
        , ondone_(unique_clone(g.ondone_))
        , mods_(unique_clone(g.mods_))
        , amp_(g.amp_)
        , play_speed_(g.play_speed_)
        , cnt_repeats_(g.cnt_repeats_)
        , pan_(g.pan_)
        , id_(g.id_)
        , state_(g.state_)
        , pan_overflow_(g.pan_overflow_)
        , pan_mode_(g.pan_mode_)
        , play_interp_(g.play_interp_)
        , win_type_(g.win_type_)
        , pre_delay_done_(g.pre_delay_done_)
    {
    }

    /**
     * Grain ctor
     * @param array_pos - grain position in the array (in samples)
     * @param length - grain length (in samples)
     * @param pre_delay - 'upbeat' before the grain will be played first time (in samples)
     */
    Grain(size_t array_pos, size_t length, size_t pre_delay = 0);

    /// get grain id
    uint16_t id() const { return id_; }
    /// set grain id
    void setId(uint16_t id) { id_ = id; }

    t_symbol* tag() const { return tag_; }
    void setTag(t_symbol* tag) { tag_ = tag; }
    bool equalTag(t_symbol* tag) const { return tag_ == tag; }
    bool matchTag(const std::regex& rx) const;

    /// get grain position in the array
    size_t arrayPosInSamples() const { return src_pos_; }
    /// set grain position in the array
    void setArrayPosInSamples(size_t p) { src_pos_ = p; }

    /// get the whole array size
    size_t arraySizeInSamples() const { return array_size_; }
    /// set the whole array size
    void setArraySizeInSamples(size_t sz) { array_size_ = sz; }

    /// get grain length
    size_t lengthInSamples() const { return length_; }
    /// set grain length
    void setLengthInSamples(size_t l) { length_ = l; }

    inline size_t grainStartInSamples() const { return pre_delay_done_ ? 0 : pre_delay_; }
    inline size_t grainEndInSamples() const { return grainStartInSamples() + length_; }
    inline size_t grainDonePos() const { return grainEndInSamples() + time_after_; }
    double currentLogicPlayPos() const { return play_pos_ - grainStartInSamples(); }

    [[deprecated]] double currentAbsPlayPos() const
    {
        if (play_speed_ >= 0)
            return currentLogicPlayPos();
        else
            return double(length_) - (currentLogicPlayPos() + 1);
    }

    double currentArrayPlayPos() const
    {
        if (play_speed_ >= 0)
            return src_pos_ + currentLogicPlayPos();
        else
            return src_pos_ + (double(length_) - 1) - currentLogicPlayPos();
    }

    /// grain play duration according to playing speed
    size_t durationInSamples() const
    {
        if (play_speed_ > 0)
            return length_ / play_speed_;
        else if (play_speed_ < 0)
            return length_ / (-play_speed_);
        else
            return 0;
    }

    int32_t repeats() const { return repeats_; }
    void setRepeats(int32_t n) { repeats_ = n; }

    /// grain end pos on the global timeline
    size_t endInSamples() const { return timeBefore() + durationInSamples(); }

    /// grain playing time on the global timeline
    size_t timeBefore() const { return pre_delay_; }
    void setTimeBefore(uint32_t samp) { pre_delay_ = samp; }

    /// silence after grain in samples
    size_t timeAfter() const { return time_after_; }
    void setTimeAfter(uint32_t samp) { time_after_ = samp; }

    GrainInterp playInterpolation() const { return play_interp_; }
    void setPlayInterpolation(GrainInterp i) { play_interp_ = i; }

    // speed
    float speed() const { return play_speed_; }
    void addSpeed(float v) { setSpeed(speed() + v); }
    void setSpeed(float s);

    // window
    GrainWindowType winType() const { return win_type_; }
    void setWinType(GrainWindowType t) { win_type_ = t; }

    // win param
    float winParam() const { return win_param_; }
    void setWinParam(float v) { win_param_ = v; }

    // pan
    float pan() const { return pan_; }
    void setPan(float pan);

    GrainPropOverflow panOverflow() const { return pan_overflow_; }
    void setPanOverflow(GrainPropOverflow po);

    GrainPan panMode() const { return pan_mode_; }
    void setPanMode(GrainPan m);

    // amplitude
    float amplitude() const { return amp_; }
    void setAmplitude(float amp);

    std::pair<t_sample, t_sample> panSample(t_sample in) const
    {
        switch (pan_mode_) {
        case GRAIN_PAN_LINEAR:
            return { in * (1 - pan_), in * pan_ };
        case GRAIN_PAN_SQRT:
            return { q8_sqrt(in * (1 - pan_)), q8_sqrt(in * pan_) };
        case GRAIN_PAN_NONE:
        default:
            return { 1, 1 };
        }
    }

    /**
     * @brief start grain playing at position
     */
    void start(size_t playPosSamp);

    void resetFirstTime() { pre_delay_done_ = false; }
    void resetPlayPos() { play_pos_ = 0; }

    GrainState done();

    /**
     * @brief process grain: add grain output to specified buffers
     * @param in - input source array iterator
     * @param in_size - array size in samples
     * @param buf - point to input buffers (left and right channels)
     * @param bs - buffer block size
     * @param sr - samplerate
     * @param done_samp - pointer to write number of processed samples in current block
     * @return
     */
    GrainState process(ArrayIterator in, size_t in_size, t_sample** buf, uint32_t bs, uint32_t sr, uint32_t buf_offset = 0, uint32_t* done_samp = nullptr);

    // grain playing status
    GrainState playStatus() const { return state_; }
    void setPlayStatus(GrainState st) { state_ = st; }

    void setOnDone(GrainPropId id, const ByteCode& bc);
    int32_t doneCounter() const { return cnt_repeats_; }

    void initByteCodeConst(ByteCode& bc) const;

    inline bool canBePlayed() const { return (repeats_ == -1) || (cnt_repeats_ < repeats_); }

    void setModulation(GrainPropId id, const GrainPropModulator& mod);
    bool hasModulation(GrainPropId id) const;

private:
    inline bool beforeGrain() const { return play_pos_ < grainStartInSamples(); }
    inline bool afterGrain() const { return play_pos_ >= grainEndInSamples(); }
    inline bool validArrayPos(double pos, size_t size) const { return 0 <= pos && pos < size; }

    /// checks if the whole grain playing should be stopped
    inline bool shouldDone() const { return play_pos_ >= grainDonePos(); }

public:
    static bool initWinTables();
};

std::ostream& operator<<(std::ostream& os, const Grain& g);

}

#endif // GRAIN_H
