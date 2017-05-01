/*****************************************************************************
 * Copyright 2017 Serge Poltavsky. All rights reserved.
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
#ifndef CEAMMC_CONVERT_H
#define CEAMMC_CONVERT_H

#include <cmath>
#include <string>

namespace ceammc {

template <class T>
T clip(T v, T min, T max)
{
    return std::min(max, std::max(v, min));
}

namespace convert {
    namespace time {
        /**
         * @brief converts time in seconds to formatted time string: "00:00:00"
         */
        std::string sec2str(float sec, bool ms = false);

        float str2sec(const std::string& str, float def = 0.f);
    }

    template <class T>
    T lin2lin(T v, T x0, T x1, T y0, T y1)
    {
        return (v - x0) / (x1 - x0) * (y1 - y0) + y0;
    }

    float lin2exp(float x, float x0, float x1, float y0, float y1);
    double lin2exp(double x, double x0, double x1, double y0, double y1);
    float exp2lin(float x, float x0, float x1, float y0, float y1);
    double exp2lin(double x, double x0, double x1, double y0, double y1);
    float lin2curve(float x, float x0, float x1, float y0, float y1, float curve);
    double lin2curve(double x, double x0, double x1, double y0, double y1, double curve);
}
}

#endif // CEAMMC_CONVERT_H
