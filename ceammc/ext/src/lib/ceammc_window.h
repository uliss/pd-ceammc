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

#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif

#include <cmath>
#include <cstddef>
#include <type_traits>

namespace ceammc {
namespace window {
    template <class Iterator, class FillFunc>
    inline bool fill(Iterator begin, Iterator end, FillFunc fn)
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
    inline T triangle(size_t idx, size_t n)
    {
        static_assert(std::is_floating_point<T>::value, "not a float type");

        if (idx >= n || n < 2)
            return 0;

        return 1 - std::fabs(T(2 * idx) / T(n - 1) - 1);
    }

    template <class T>
    inline T welch(size_t idx, size_t n)
    {
        static_assert(std::is_floating_point<T>::value, "not a float type");

        if (idx >= n || n < 2)
            return 0;

        auto x = ((2 * idx) / double(n - 1) - 1);
        return static_cast<T>(1 - x * x);
    }

    template <class T>
    inline T hann(size_t idx, size_t n)
    {
        static_assert(std::is_floating_point<T>::value, "not a float type");

        if (idx >= n || n < 2)
            return 0;

        return static_cast<T>(0.5 * (1 - std::cos(double(2 * M_PI * idx) / (n - 1))));
    }

    template <class T>
    inline T rect(size_t /*idx*/, size_t /*n*/)
    {
        return 1;
    }

    template <class T>
    inline T sine(size_t idx, size_t n)
    {
        static_assert(std::is_floating_point<T>::value, "not a float type");

        return static_cast<T>(std::sin((M_PI * idx) / double(n - 1)));
    }

    template <class T>
    inline T hamming(size_t idx, size_t n)
    {
        static_assert(std::is_floating_point<T>::value, "not a float type");

        constexpr double a = 0.54;
        constexpr double b = 1 - a;
        return static_cast<T>(a - b * std::cos((2 * M_PI * idx) / double(n - 1)));
    }

    template <class T>
    inline T blackman(size_t idx, size_t n)
    {
        static_assert(std::is_floating_point<T>::value, "not a float type");

        constexpr double a = 0.16;
        constexpr double a0 = (1 - a) / 2;
        constexpr double a1 = 0.5;
        constexpr double a2 = a / 2;
        const double N = n - 1;

        return static_cast<T>(a0
            - a1 * std::cos((2 * M_PI * idx) / N)
            + a2 * std::cos((4 * M_PI * idx) / N));
    }

    template <class T>
    inline T nuttall(size_t idx, size_t n)
    {
        static_assert(std::is_floating_point<T>::value, "not a float type");

        constexpr double a0 = 0.355768;
        constexpr double a1 = 0.487396;
        constexpr double a2 = 0.144232;
        constexpr double a3 = 0.012604;
        const double N = n - 1;

        return static_cast<T>(a0
            - a1 * std::cos((2 * M_PI * idx) / N)
            + a2 * std::cos((4 * M_PI * idx) / N)
            - a3 * std::cos((6 * M_PI * idx) / N));
    }

    template <class T>
    inline T blackman_harris(size_t idx, size_t n)
    {
        static_assert(std::is_floating_point<T>::value, "not a float type");

        constexpr double a0 = 0.35875;
        constexpr double a1 = 0.48829;
        constexpr double a2 = 0.14128;
        constexpr double a3 = 0.01168;
        const double N = n - 1;

        return static_cast<T>(a0
            - a1 * std::cos((2 * M_PI * idx) / N)
            + a2 * std::cos((4 * M_PI * idx) / N)
            - a3 * std::cos((6 * M_PI * idx) / N));
    }

    template <class T>
    inline T flattop(size_t idx, size_t n)
    {
        static_assert(std::is_floating_point<T>::value, "not a float type");

        constexpr double a0 = 0.21557895;
        constexpr double a1 = 0.41663158;
        constexpr double a2 = 0.27726316;
        constexpr double a3 = 0.08357895;
        constexpr double a4 = 0.00694737;
        const double N = n - 1;

        return static_cast<T>(a0
            - a1 * std::cos((2 * M_PI * idx) / N)
            + a2 * std::cos((4 * M_PI * idx) / N)
            - a3 * std::cos((6 * M_PI * idx) / N)
            + a4 * std::cos((8 * M_PI * idx) / N));
    }

    template <class T, int SIGMA>
    inline T gauss(size_t idx, size_t n)
    {
        const double A = (n - 1) / double(2);
        const double x = (idx - A) / ((SIGMA / double(100)) * A);
        return static_cast<T>(std::exp(-0.5 * x * x));
    }
}
}

#endif // CEAMMC_WINDOW_H
