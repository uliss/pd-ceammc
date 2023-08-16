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
#ifndef CEAMMC_SOXR_RESAMPLER_H
#define CEAMMC_SOXR_RESAMPLER_H

#include <array>
#include <functional>
#include <memory>
#include <stdexcept>

#include <boost/variant.hpp>

#include "m_pd.h"

// forward declaration
struct soxr;

namespace ceammc {

enum class SoxrResamplerFormat : std::uint8_t {
    FLOAT_I = 1, // float channel interleave
    DOUBLE_I = 1 << 1, // double channel interleave
    T_SAMPLE_I = (std::is_same<float, t_sample>::value) ? FLOAT_I : DOUBLE_I, // pd channel interleave
    FLOAT_S = FLOAT_I | (1 << 2), // float channel split
    DOUBLE_S = DOUBLE_I | (1 << 2), // double channel split
    T_SAMPLE_S = (std::is_same<float, t_sample>::value) ? FLOAT_S : DOUBLE_S, // pd channel split
};

struct SoxrResamplerOptions {
    bool variableRate;
    SoxrResamplerFormat in_fmt;
    SoxrResamplerFormat out_fmt;

    SoxrResamplerOptions(
        bool varRate = false,
        SoxrResamplerFormat in_format = SoxrResamplerFormat::T_SAMPLE_I,
        SoxrResamplerFormat out_format = SoxrResamplerFormat::T_SAMPLE_I)
        : variableRate(varRate)
        , in_fmt(in_format)
        , out_fmt(out_format)
    {
    }
};

class SoxrResampler {
    using SoxRPtr = std::unique_ptr<soxr, void (*)(soxr*)>;

public:
    /**
     * pointer to output buffer with interleaved samples
     * length of output in sample frames
     * is_done - true if done
     */
    template <typename T>
    using InterleaveCallbackT = std::function<bool(const T*, size_t, bool)>;

    /**
     * pointer to buffer with channel pointers
     * length of output in sample frames
     * is_done - true if done
     */
    template <typename T>
    using SplitCallbackT = std::function<bool(const T* const*, size_t, bool)>;

    using InterleaveFloatCallback = InterleaveCallbackT<float>;
    using InterleaveDoubleCallback = InterleaveCallbackT<double>;
    using SplitFloatCallback = SplitCallbackT<float>;
    using SplitDoubleCallback = SplitCallbackT<double>;

    using CallbackVariant = boost::variant<
        InterleaveFloatCallback,
        InterleaveDoubleCallback,
        SplitFloatCallback,
        SplitDoubleCallback>;

    using Exception = std::runtime_error;

    enum ResultCode { // if change, update strError()
        Ok,
        NullHandle,
        InvalidInputFormat,
        InvalidOutputFormat,
        InvalidCallback,
        ProcessError,
        CallbackQuit,
    };

    enum Quality {
        QUICK,
        LOW,
        MEDIUM,
        HIGH,
        BEST,
    };

public:
    /**
     * @brief SoxrResampler
     * @param inRate - input rate
     * @param outRate - output rate
     * @param numChannels - number of input channels
     * @param q - resample quality
     * @throw Exception on error
     */
    SoxrResampler(double inRate, double outRate, size_t numChannels, Quality q = QUICK, SoxrResamplerOptions opts = {});
    ~SoxrResampler();

    bool setOutputCallback(const InterleaveFloatCallback& cb);
    bool setOutputCallback(const InterleaveDoubleCallback& cb);
    bool setOutputCallback(const SplitFloatCallback& cb);
    bool setOutputCallback(const SplitDoubleCallback& cb);

    double ratio() const { return in_rate_ / out_rate_; }
    size_t numChannels() const { return num_chan_; }

    bool setResampleRatio(double x);

    double delay() const;
    void reset();

    ResultCode process(const float* in, size_t numFrames);
    ResultCode process(const double* in, size_t numFrames);

    ResultCode processDone();

public:
    static const char* strError(ResultCode c);

private:
    double in_rate_, out_rate_;
    size_t num_chan_;
    SoxRPtr soxr_;
    SoxrResamplerOptions opts_;
    CallbackVariant cb_;
};

}

class ceammc_soxr_resampler {
public:
    ceammc_soxr_resampler();
};

#endif // CEAMMC_SOXR_RESAMPLER_H
