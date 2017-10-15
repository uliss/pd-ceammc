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

    template <class T>
    T rect(size_t idx, size_t n);

    template <>
    float rect<float>(size_t /*idx*/, size_t /*n*/)
    {
        return 1;
    }

    template <class T>
    T sine(size_t idx, size_t n);

    template <>
    float sine<float>(size_t idx, size_t n)
    {
        return sinf((M_PI * idx) / float(n - 1));
    }

    template <class T>
    T hamming(size_t idx, size_t n);

    template <>
    float hamming<float>(size_t idx, size_t n)
    {
        static const float a = 0.54f;
        static const float b = 1 - a;
        return a - b * cosf((2 * M_PI * idx) / float(n - 1));
    }

    template <class T>
    T blackman(size_t idx, size_t n);

    template <>
    float blackman<float>(size_t idx, size_t n)
    {
        static const float a = 0.16f;
        static const float a0 = (1 - a) / 2;
        static const float a1 = 0.5f;
        static const float a2 = a / 2;
        const float N = n - 1;

        return a0
            - a1 * cosf((2 * M_PI * idx) / N)
            + a2 * cosf((4 * M_PI * idx) / N);
    }

    template <class T>
    T nuttall(size_t idx, size_t n);

    template <>
    float nuttall<float>(size_t idx, size_t n)
    {
        static const float a0 = 0.355768f;
        static const float a1 = 0.487396f;
        static const float a2 = 0.144232f;
        static const float a3 = 0.012604f;
        const float N = n - 1;

        return a0
            - a1 * cosf((2 * M_PI * idx) / N)
            + a2 * cosf((4 * M_PI * idx) / N)
            - a3 * cosf((6 * M_PI * idx) / N);
    }

    template <class T>
    T blackman_harris(size_t idx, size_t n);

    template <>
    float blackman_harris<float>(size_t idx, size_t n)
    {
        static const float a0 = 0.35875f;
        static const float a1 = 0.48829f;
        static const float a2 = 0.14128f;
        static const float a3 = 0.01168f;
        const float N = n - 1;

        return a0
            - a1 * cosf((2 * M_PI * idx) / N)
            + a2 * cosf((4 * M_PI * idx) / N)
            - a3 * cosf((6 * M_PI * idx) / N);
    }

    template <class T>
    T flattop(size_t idx, size_t n);

    template <>
    float flattop<float>(size_t idx, size_t n)
    {
        static const float a0 = 0.21557895f;
        static const float a1 = 0.41663158f;
        static const float a2 = 0.27726316f;
        static const float a3 = 0.08357895f;
        static const float a4 = 0.00694737f;
        const float N = n - 1;

        return a0
            - a1 * cosf((2 * M_PI * idx) / N)
            + a2 * cosf((4 * M_PI * idx) / N)
            - a3 * cosf((6 * M_PI * idx) / N)
            + a4 * cosf((8 * M_PI * idx) / N);
    }

    template <int SIGMA>
    float gauss(size_t idx, size_t n)
    {
        const float A = (n - 1) / 2.0f;
        const float x = (idx - A) / ((float(SIGMA) / 100.f) * A);
        return expf(-0.5 * x * x);
    }
}
}

#endif // CEAMMC_WINDOW_H
