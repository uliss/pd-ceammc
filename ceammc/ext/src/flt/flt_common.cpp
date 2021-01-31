/*****************************************************************************
 * Copyright 2021 Serge Poltavsky. All rights reserved.
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
#include "flt_common.h"

#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif

#include <cmath>

namespace ceammc {
namespace flt {

    constexpr double ln2 = M_LN2;
    constexpr double ln2_2 = ln2 / 2;

    double bandwidth2q(double bw, double w)
    {
        // 1/Q = 2 * sinh(log2/2 * BW * (w/sin(w)))
        return 1 / (2 * std::sinh(ln2_2 * bw * w / std::sin(w)));
    }

    double q2bandwidth(double q, double w)
    {
        // 1/Q = 2 * sinh(log2/2 * BW * (w/sin(w)))
        // asinh(1/2q) = log2/2 * BW * (w/sin(w))
        // BW = asinh(1/2q) * (sin(w) * 2) / (w * log2)
        const auto sin_w_2 = std::sin(w) * 2;
        const auto wlog2 = w * ln2;

        return std::asinh(1 / (2 * q)) * sin_w_2 / wlog2;
    }
}
}
