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

namespace {

int quality2soxr(SoxrResampler::Quality q)
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

soxr_datatype_t sampleFormat(SoxrResamplerFormat fmt)
{
    switch (fmt) {
    case SoxrResamplerFormat::FLOAT_I:
        return SOXR_FLOAT32_I;
    case SoxrResamplerFormat::DOUBLE_I:
        return SOXR_FLOAT64_I;
    case SoxrResamplerFormat::FLOAT_S:
        return SOXR_FLOAT32_S;
    case SoxrResamplerFormat::DOUBLE_S:
        return SOXR_FLOAT64_S;
    }
}

template <typename T>
bool isFormat(SoxrResamplerFormat fmt);

template <>
bool isFormat<float>(SoxrResamplerFormat fmt)
{
    return static_cast<int>(fmt) & 1;
}

template <>
bool isFormat<double>(SoxrResamplerFormat fmt)
{
    return static_cast<int>(fmt) & (1 << 1);
}

bool inline isFmtInterleave(SoxrResamplerFormat fmt)
{
    return !(static_cast<int>(fmt) & (1 << 2));
}

bool inline isFmtSplit(SoxrResamplerFormat fmt)
{
    return !isFmtInterleave(fmt);
}

template <typename In, typename Out, typename Cb, size_t BUF_SIZE = 512>
SoxrResampler::ResultCode interleaveResample(soxr_t soxr, const In* in, size_t numFrames, size_t numChan, const Cb& cb)
{
    using namespace ceammc;
    Out out_buf[BUF_SIZE];

    auto it = in;
    auto in_frames = numFrames;
    size_t out_frames_done = 0;
    const auto out_frames = BUF_SIZE / numChan;

    do {
        size_t in_frames_done = 0;
        auto err = soxr_process(soxr, it, in_frames, &in_frames_done, out_buf, out_frames, &out_frames_done);
        if (err)
            return SoxrResampler::ProcessError;

        if (in != nullptr) {
            it += (in_frames_done * numChan);
            in_frames -= in_frames_done;
        }

        if (!cb(out_buf, out_frames_done, (in == nullptr)))
            return SoxrResampler::CallbackQuit;

    } while (in_frames > 0 || out_frames_done > 0);

    return SoxrResampler::Ok;
}

template <typename In, typename Out>
inline SoxrResampler::ResultCode interleaveResample(soxr_t soxr,
    const In* in,
    size_t numFrames,
    size_t numChan,
    const SoxrResampler::CallbackVariant& cb)
{
    auto x = boost::get<SoxrResampler::InterleaveCallbackT<Out>>(&cb);
    if (x && *x)
        return interleaveResample<In, Out>(soxr, in, numFrames, numChan, *x);
    else
        return ceammc::SoxrResampler::InvalidCallback;
}

template <typename In, typename Out, typename Cb, size_t BUF_SIZE = 512>
SoxrResampler::ResultCode splitResample(soxr_t soxr, const In* in, size_t numFrames, size_t numChan, const Cb& cb)
{
    using namespace ceammc;
    const auto OUT_FRAMES = BUF_SIZE / numChan;
    Out raw_buf[BUF_SIZE];
    Out* out_buf[numChan];
    // fill buffer pointer
    for (size_t i = 0; i < numChan; i++)
        out_buf[i] = &raw_buf[OUT_FRAMES * i];

    auto it = in;
    auto in_frames = numFrames;
    size_t out_frames_done = 0;

    do {
        size_t in_frames_done = 0;
        auto err = soxr_process(soxr, it, in_frames, &in_frames_done, out_buf, OUT_FRAMES, &out_frames_done);
        if (err)
            return SoxrResampler::ProcessError;

        if (in != nullptr) {
            it += (in_frames_done * numChan);
            in_frames -= in_frames_done;
        }

        if (!cb(out_buf, out_frames_done, (in == nullptr)))
            return SoxrResampler::CallbackQuit;

    } while (in_frames > 0 || out_frames_done > 0);

    return SoxrResampler::Ok;
}

template <typename In, typename Out>
inline SoxrResampler::ResultCode splitResample(soxr_t soxr,
    const In* in,
    size_t numFrames,
    size_t numChan,
    const SoxrResampler::CallbackVariant& cb)
{
    auto x = boost::get<SoxrResampler::SplitCallbackT<Out>>(&cb);
    if (x && *x)
        return splitResample<In, Out>(soxr, in, numFrames, numChan, *x);
    else
        return ceammc::SoxrResampler::InvalidCallback;
}

}

SoxrResampler::SoxrResampler(double inRate, double outRate, size_t numChannels, Quality q, SoxrResamplerOptions opts)
    : in_rate_(inRate)
    , out_rate_(outRate)
    , num_chan_(numChannels)
    , soxr_(nullptr, soxr_delete)
    , opts_(opts)
{

    soxr_io_spec_t io_spec {
        sampleFormat(opts.in_fmt),
        sampleFormat(opts.out_fmt),
        1,
        nullptr,
        0,
    };

    auto q_spec = soxr_quality_spec(quality2soxr(q), SOXR_LINEAR_PHASE);

    if (opts.variableRate)
        q_spec.flags |= SOXR_VR;

    soxr_error_t error;
    soxr_.reset(soxr_create(inRate, outRate, numChannels, &error, &io_spec, &q_spec, nullptr));

    if (error)
        throw Exception(fmt::format("soxr_create() error: {}", error));
}

bool SoxrResampler::setOutputCallback(const InterleaveFloatCallback& cb)
{
    if (opts_.out_fmt != SoxrResamplerFormat::FLOAT_I)
        return false;

    cb_ = cb;
    return true;
}

bool SoxrResampler::setOutputCallback(const InterleaveDoubleCallback& cb)
{
    if (opts_.out_fmt != SoxrResamplerFormat::DOUBLE_I)
        return false;

    cb_ = cb;
    return true;
}

bool SoxrResampler::setOutputCallback(const SplitFloatCallback& cb)
{
    if (opts_.out_fmt != SoxrResamplerFormat::FLOAT_S)
        return false;

    cb_ = cb;
    return true;
}

bool SoxrResampler::setOutputCallback(const SplitDoubleCallback& cb)
{
    if (opts_.out_fmt != SoxrResamplerFormat::DOUBLE_S)
        return false;

    cb_ = cb;
    return true;
}

SoxrResampler::~SoxrResampler() = default;

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

SoxrResampler::ResultCode SoxrResampler::process(const float* in, size_t numFrames)
{
    if (!soxr_)
        return NullHandle;

    if (!isFormat<float>(opts_.in_fmt))
        return InvalidInputFormat;

    switch (opts_.out_fmt) {
    case SoxrResamplerFormat::FLOAT_I:
        return interleaveResample<float, float>(soxr_.get(), in, numFrames, num_chan_, cb_);
    case SoxrResamplerFormat::FLOAT_S:
        return splitResample<float, float>(soxr_.get(), in, numFrames, num_chan_, cb_);
    default:
        return InvalidOutputFormat;
    }
}

SoxrResampler::ResultCode SoxrResampler::process(const double* in, size_t numFrames)
{
    if (!soxr_)
        return NullHandle;

    if (!isFormat<double>(opts_.in_fmt))
        return InvalidInputFormat;

    switch (opts_.out_fmt) {
    case SoxrResamplerFormat::FLOAT_I:
        return interleaveResample<double, float>(soxr_.get(), in, numFrames, num_chan_, cb_);
    case SoxrResamplerFormat::FLOAT_S:
        return splitResample<double, float>(soxr_.get(), in, numFrames, num_chan_, cb_);
    case SoxrResamplerFormat::DOUBLE_S:
        return splitResample<double, double>(soxr_.get(), in, numFrames, num_chan_, cb_);
    case SoxrResamplerFormat::DOUBLE_I:
        return interleaveResample<double, double>(soxr_.get(), in, numFrames, num_chan_, cb_);
    default:
        return InvalidOutputFormat;
    }
}

SoxrResampler::ResultCode SoxrResampler::processDone()
{
    if (!soxr_)
        return NullHandle;

    switch (opts_.out_fmt) {
    case SoxrResamplerFormat::FLOAT_I:
        return interleaveResample<double, float>(soxr_.get(), nullptr, 0, num_chan_, cb_);
    case SoxrResamplerFormat::FLOAT_S:
        return splitResample<double, float>(soxr_.get(), nullptr, 0, num_chan_, cb_);
    case SoxrResamplerFormat::DOUBLE_S:
        return splitResample<double, double>(soxr_.get(), nullptr, 0, num_chan_, cb_);
    case SoxrResamplerFormat::DOUBLE_I:
        return interleaveResample<double, double>(soxr_.get(), nullptr, 0, num_chan_, cb_);
    default:
        return InvalidOutputFormat;
    }
}

const char* SoxrResampler::strError(ResultCode c)
{
    switch (c) {
    case Ok:
        return "Ok";
    case NullHandle:
        return "Null SoxR handle";
    case InvalidInputFormat:
        return "Invalid input sample format";
    case InvalidOutputFormat:
        return "Invalid output sampel format";
    case InvalidCallback:
        return "Invalid callback";
    case ProcessError:
        return "sox_process() error";
    case CallbackQuit:
        return "call quit";
    default:
        return "???";
    }
}
