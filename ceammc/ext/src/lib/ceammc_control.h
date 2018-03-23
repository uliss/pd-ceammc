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

#include <boost/math/special_functions/fpclassify.hpp>
#include <cmath>

namespace ceammc {
class SmoothControlValue {
    t_float n_;
    t_float smooth_;

public:
    SmoothControlValue(size_t ms = 25);

    void setSmoothTime(size_t ms, size_t sampleRate = 44100, size_t step = 1);

    t_float get(t_float v)
    {
        if (boost::math::isnan(v) || v == std::numeric_limits<t_float>::infinity())
            return smooth_;

        smooth_ = (1 - n_) * v + n_ * smooth_;
        return smooth_;
    }
};
}
#endif // CEAMMC_CONTROL_H
