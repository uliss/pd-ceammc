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
#ifndef CEAMMC_WINDOW_H
#define CEAMMC_WINDOW_H

#include <cmath>
#include <cstddef>

namespace ceammc {
namespace window {
    template <class Iterator, class FillFunc>
    bool fill(Iterator begin, Iterator end, FillFunc fn)
    {
        std::ptrdiff_t N = end - begin;
        if (N < 1)
            return false;

        if (N == 1) {
            begin[0] = 0;
            return true;
        }

        for (size_t i = 0; i < size_t(N); i++)
            begin[i] = fn(i, N);

        return true;
    }

    template <class T>
    T triangle(size_t idx, size_t n);

    template <>
    float triangle<float>(size_t idx, size_t n)
    {
        if (idx >= n || n < 2)
            return 0.f;

        return 1 - fabsf(float(2 * idx) / float(n - 1) - 1);
    }

    template <class T>
    T welch(size_t idx, size_t n)
    {
        if (idx >= n || n < 2)
            return 0.f;

        T x = (T(2 * idx) / T(n - 1) - 1);
        return 1 - x * x;
    }

    template <class T>
    T hann(size_t idx, size_t n);

    template <>
    float hann<float>(size_t idx, size_t n)
    {
        if (idx >= n || n < 2)
            return 0.f;

        return 0.5f * (1 - cosf(float(2.0 * M_PI * idx) / (n - 1)));
    }
}
}

#endif // CEAMMC_WINDOW_H
