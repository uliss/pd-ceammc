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
#include "ceammc_soxr_resampler.h"
#include "fmt/core.h"
#include "soxr.h"

#include <iostream>

using namespace ceammc;

constexpr soxr_datatype_t SAMP_INTERLEAVE
    = std::is_same<t_float, float>::value
    ? SOXR_FLOAT32_I
    : SOXR_FLOAT64_I;

constexpr soxr_datatype_t SAMP_SPLIT
    = std::is_same<t_float, float>::value
    ? SOXR_FLOAT32_S
    : SOXR_FLOAT64_S;

static int quality2soxr(SoxrResampler::Quality q)
{
    const static int sq[] = {
        SOXR_QQ,
        SOXR_LQ,
        SOXR_MQ,
        SOXR_HQ,
        SOXR_VHQ,
    };

    auto idx = static_cast<size_t>(q);
    return sq[idx];
}

SoxrResampler::SoxrResampler(double inRate, double outRate, size_t numChannels, Quality q, bool variable, bool splitChannels)
    : in_rate_(inRate)
    , out_rate_(outRate)
    , num_chan_(numChannels)
    , soxr_(nullptr, soxr_delete)
{

    soxr_io_spec_t io_spec {
        splitChannels ? SAMP_SPLIT : SAMP_INTERLEAVE,
        splitChannels ? SAMP_SPLIT : SAMP_INTERLEAVE,
        1,
        nullptr,
        0,
    };

    auto q_spec = soxr_quality_spec(quality2soxr(q), SOXR_LINEAR_PHASE);

    if (variable)
        q_spec.flags |= SOXR_VR;

    soxr_error_t error;
    soxr_.reset(soxr_create(inRate, outRate, numChannels, &error, &io_spec, &q_spec, nullptr));

    if (error)
        throw Exception(fmt::format("soxr_create() error: {}", error));
}

bool SoxrResampler::setResampleRatio(double x)
{
    auto err = soxr_set_io_ratio(soxr_.get(), x, 0);
    return !err;
}

double SoxrResampler::delay() const
{
    return soxr_delay(soxr_.get());
}

void SoxrResampler::reset()
{
    soxr_clear(soxr_.get());
}

SoxrResampler::ResultCode SoxrResampler::process(const t_sample* in, size_t numFrames, WriterCallback cb)
{
    if (!soxr_)
        return NullHandle;

    constexpr size_t OUT_BUF_SIZE = 512;
    t_sample out_buf[OUT_BUF_SIZE];

    auto it = in;
    auto in_frames = numFrames;
    size_t out_frames_done = 0;
    const auto out_frames = OUT_BUF_SIZE / num_chan_;

    do {
        size_t in_frames_done = 0;
        auto err = soxr_process(soxr_.get(), it, in_frames, &in_frames_done, out_buf, out_frames, &out_frames_done);

        if (err)
            return ProcessError;

        it += (in_frames_done * num_chan_);
        in_frames -= in_frames_done;

        if (!cb(out_buf, out_frames_done))
            return CallbackQuit;

    } while (in_frames > 0 || out_frames_done > 0);

    return Ok;
}

SoxrResampler::ResultCode SoxrResampler::processDone(WriterCallback cb)
{
    if (!soxr_)
        return NullHandle;

    constexpr size_t OUT_BUF_SIZE = 512;
    t_sample out_buf[OUT_BUF_SIZE];
    const auto out_frames = OUT_BUF_SIZE / num_chan_;
    size_t out_frames_done = 0;

    do {
        size_t in_frames_done = 0;
        auto err = soxr_process(soxr_.get(), nullptr, 0, &in_frames_done, out_buf, out_frames, &out_frames_done);
        if (err)
            return ProcessError;

        if (!cb(out_buf, out_frames_done))
            return CallbackQuit;

    } while (out_frames_done > 0);

    return Ok;
}

SoxrResampler::~SoxrResampler() = default;
