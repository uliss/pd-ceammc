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
#ifndef CEAMMC_NUMERIC_H
#define CEAMMC_NUMERIC_H

#include <algorithm>
#include <cmath>
#include <limits>
#include <type_traits>

namespace ceammc {
namespace math {
    template <class T>
    static inline bool is_integer(T v)
    {
        return std::ceil(v) == v;
    }

    template <class T>
    static inline bool is_natural(T v)
    {
        return (v >= 0 && is_integer(v));
    }

    template <typename T, size_t ULP = 1>
    bool float_compare(T x, T y)
    {
        static_assert(std::is_floating_point<T>::value, "floating point expected");

        return std::abs(x - y) <= std::numeric_limits<T>::epsilon()
            * std::max(std::abs(x), std::abs(y))
            * ULP;
    }
}
}

#endif // CEAMMC_NUMERIC_H
