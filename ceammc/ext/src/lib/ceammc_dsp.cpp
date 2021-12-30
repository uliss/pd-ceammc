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
#include "ceammc_dsp.h"

#include <cmath>

namespace ceammc {

namespace dsp {

    constexpr long double operator""__2(long double v) { return v * v; }

    double curves::a_weight(double f)
    {
        const auto f2 = f * f;
        const auto f4 = f2 * f2;

        return (12194 * 12194 * f4) / ( //
                   (f2 + 20.6 * 20.6) //
                   * std::sqrt((f2 + 107.7 * 107.7) * (f2 + 737.9 * 737.9)) //
                   * (f2 + 12194 * 12194) //
               );
    }

}
}
