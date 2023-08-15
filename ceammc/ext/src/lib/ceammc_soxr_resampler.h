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

#include "m_pd.h"

// forward declaration
struct soxr;

namespace ceammc {

class SoxrResampler {
    using SoxRPtr = std::unique_ptr<soxr, void (*)(soxr*)>;
    double in_rate_, out_rate_;
    size_t num_chan_;
    SoxRPtr soxr_;

public:
    using WriterCallback = std::function<bool(const t_sample*, size_t)>;
    using Exception = std::runtime_error;

    enum ResultCode {
        Ok,
        NullHandle,
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
    SoxrResampler(double inRate, double outRate, size_t numChannels, Quality q = QUICK, bool variable = false, bool splitChannels = false);
    ~SoxrResampler();

    double ratio() const { return in_rate_ / out_rate_; }
    size_t numChannels() const { return num_chan_; }

    bool setResampleRatio(double x);

    double delay() const;
    void reset();

    ResultCode process(const t_sample* in, size_t numFrames, WriterCallback cb);
    ResultCode processDone(WriterCallback cb);

};

}

class ceammc_soxr_resampler {
public:
    ceammc_soxr_resampler();
};

#endif // CEAMMC_SOXR_RESAMPLER_H
