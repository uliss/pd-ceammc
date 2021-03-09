/*****************************************************************************
 * Copyright 2018 Serge Poltavsky. All rights reserved.
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
#ifndef CEAMMC_CONTROL_H
#define CEAMMC_CONTROL_H

#include "m_pd.h"

#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif

#include <cmath>
#include <limits>

namespace ceammc {
class SmoothControlValue {
    t_float n_;
    t_float smooth_;

public:
    SmoothControlValue(size_t ms = 25);

    void setSmoothTime(size_t ms, size_t sampleRate = 44100, size_t step = 1);

    t_float get(t_float v)
    {
        if (std::isnan(v) || std::isinf(v))
            return smooth_;

        smooth_ = (1 - n_) * v + n_ * smooth_;
        return smooth_;
    }
};

namespace control {
    template <typename FloatType>
    struct NoFloatFix {
        static inline FloatType fix(FloatType v) { return v; }
    };

    template <typename FloatType>
    struct FloatFix {
        static inline FloatType fix(FloatType v)
        {
            return std::isnormal(v) ? v : 0;
        }
    };

    template <typename FloatType, typename ValueFix = FloatFix<FloatType>>
    class Smooth {
    public:
        explicit Smooth(FloatType z = 0)
            : z_(z)
        {
            setSmoothTime(20, 44100);
        }

        /**
         * @param ms - smooth type in milliseconds
         * @param Fs - sampling frequency
         */
        Smooth(FloatType ms, FloatType Fs)
        {
            setSmoothTime(ms, Fs);
        }

        void setSmoothTime(FloatType ms, FloatType Fs)
        {
            constexpr FloatType two_pi = 2 * M_PI;
            a_ = std::exp(-two_pi / (ms * 0.001f * Fs));
        }

        inline FloatType process(FloatType in)
        {
            return operator()(in);
        }

        inline FloatType operator()(FloatType in)
        {
            z_ = (ValueFix::fix(in) * (1 - a_)) + (z_ * a_);
            return z_;
        }

    private:
        FloatType a_ { 0 };
        FloatType z_ { 0 };
    };
}
}
#endif // CEAMMC_CONTROL_H
