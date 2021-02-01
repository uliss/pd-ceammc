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
#ifndef CEAMMC_FILTER_H
#define CEAMMC_FILTER_H

#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif

#include <array>
#include <cmath>
#include <complex>
#include <type_traits>

namespace ceammc {
namespace flt {
    template <class T>
    using ArrayBA = std::array<T, 6>;

    constexpr double m_pi = M_PI;
    constexpr double m_2pi = 2 * M_PI;
    constexpr double ln2 = M_LN2;
    constexpr double ln2_2 = ln2 / 2;

    /**
     * converts frequency in herz to angular frequency
     * @param f0 - frequency
     * @param Fs - samplerate frequency
     * @return angular frequency
     */
    template <typename T = double>
    T freq2ang(T f0, T Fs)
    {
        return m_2pi * f0 / Fs;
    }

    template <typename T, typename It>
    std::complex<T> calcz(T w, It b, It e)
    {
        static_assert(std::is_floating_point<T>::value, "Float value expected");

        if (b == e)
            return std::complex<T>();

        std::complex<T> a(*b++);

        T j = 1;
        while (b != e) {
            auto ejw = std::complex<T>(0, -j * w);
            a += (*b) * std::exp(ejw);
            ++j;
            ++b;
        }

        return a;
    }

    template <typename T, typename It>
    std::complex<T> freqz(T w, It bbegin, It bend, It abegin, It aend)
    {
        const auto Ajw = calcz(w, abegin, aend);
        if (Ajw == std::complex<T>())
            return Ajw;

        const auto Bjw = calcz(w, bbegin, bend);

        return Bjw / Ajw;
    }

    template <typename T = double>
    ArrayBA<T> calc_lpf(T w, T q)
    {
        const T cosw = std::cos(w);
        const T sinw = std::sin(w);
        const T a = sinw / (2 * q);
        const T norm = 1 / (1 + a);

        const T b0 = (1 - cosw) / 2 * norm;
        const T b1 = (1 - cosw) * norm;
        const T b2 = b0;
        const T a1 = (-2 * cosw) * norm;
        const T a2 = (1 - a) * norm;

        return { b0, b1, b2, 1, a1, a2 };
    }

    template <typename T = double>
    ArrayBA<T> calc_hpf(T w, T q)
    {
        const T cosw = std::cos(w);
        const T sinw = std::sin(w);
        const T a = sinw / (2 * q);

        const T b0 = (1 + cosw) / 2;
        const T b1 = -(1 + cosw);
        const T b2 = b0;
        const T a0 = 1 + a;
        const T a1 = -2 * cosw;
        const T a2 = 1 - a;

        return { b0 / a0, b1 / a0, b2 / a0, 1, a1 / a0, a2 / a0 };
    }

    template <typename T = double>
    ArrayBA<T> calc_bpfq(T w, T q)
    {
        const T cosw = std::cos(w);
        const T sinw = std::sin(w);
        const T a = sinw / (2 * q);

        const T b0 = sinw / 2;
        const T b1 = 0;
        const T b2 = -sinw / 2;
        const T a0 = 1 + a;
        const T a1 = -2 * cosw;
        const T a2 = 1 - a;

        return { b0 / a0, b1 / a0, b2 / a0, 1, a1 / a0, a2 / a0 };
    }

    template <typename T = double>
    ArrayBA<T> calc_peak_eq(T w, T q, T db)
    {
        const T A = std::pow(10, db / 40);
        const T cosw = std::cos(w);
        const T sinw = std::sin(w);
        const T a = sinw / (2 * q);

        const T b0 = 1 + (a * A);
        const T b1 = -2 * cosw;
        const T b2 = 1 - (a * A);
        const T a0 = 1 + (a / A);
        const T a1 = -2 * cosw;
        const T a2 = 1 - (a / A);

        return { b0 / a0, b1 / a0, b2 / a0, 1, a1 / a0, a2 / a0 };
    }

    template <typename T = double>
    ArrayBA<T> calc_notch(T w, T q)
    {
        const T cosw = std::cos(w);
        const T sinw = std::sin(w);
        const T a = sinw / (2 * q);

        const T b0 = 1;
        const T b1 = -2 * cosw;
        const T b2 = 1;
        const T a0 = 1 + a;
        const T a1 = -2 * cosw;
        const T a2 = 1 - a;

        return { b0 / a0, b1 / a0, b2 / a0, 1, a1 / a0, a2 / a0 };
    }

    template <typename T = double>
    ArrayBA<T> calc_lowshelf(T w, T db, T slope = 1)
    {
        const T A = std::pow(10, db / 40);
        const T cosw = std::cos(w);
        const T sinw = std::sin(w);
        const T a = sinw / 2 * std::sqrt((A + (1 / A)) * ((1 / slope) - 1) + 2);

        const T b0 = A * ((A + 1) - ((A - 1) * cosw) + (2 * std::sqrt(A) * a));
        const T b1 = 2 * A * ((A - 1) - (A + 1) * cosw);
        const T b2 = A * ((A + 1) - ((A - 1) * cosw) - (2 * std::sqrt(A) * a));
        const T a0 = (A + 1) + ((A - 1) * cosw) + (2 * std::sqrt(A) * a);
        const T a1 = -2 * ((A - 1) + (A + 1) * cosw);
        const T a2 = (A + 1) + ((A - 1) * cosw) - (2 * std::sqrt(A) * a);

        return { b0 / a0, b1 / a0, b2 / a0, 1, a1 / a0, a2 / a0 };
    }

    template <typename T = double>
    ArrayBA<T> calc_highshelf(T w, T db, T slope = 1)
    {
        const T A = std::pow(10, db / 40);
        const T cosw = std::cos(w);
        const T sinw = std::sin(w);
        const T a = sinw / 2 * std::sqrt((A + (1 / A)) * ((1 / slope) - 1) + 2);

        const T b0 = A * ((A + 1) + ((A - 1) * cosw) + (2 * std::sqrt(A) * a));
        const T b1 = -2 * A * ((A - 1) + (A + 1) * cosw);
        const T b2 = A * ((A + 1) + ((A - 1) * cosw) - (2 * std::sqrt(A) * a));
        const T a0 = (A + 1) - ((A - 1) * cosw) + (2 * std::sqrt(A) * a);
        const T a1 = 2 * ((A - 1) - (A + 1) * cosw);
        const T a2 = (A + 1) - ((A - 1) * cosw) - (2 * std::sqrt(A) * a);

        return { b0 / a0, b1 / a0, b2 / a0, 1, a1 / a0, a2 / a0 };
    }

    template <typename T = double>
    T bandwidth2q(T bw, T w)
    {
        /**
         * from 'Audio-EQ-Cookbook' by Robert Bristow-Johnson
         * 1/Q = 2 * sinh(log2/2 * BW * (w/sin(w)))
         */
        return 1 / (2 * std::sinh(ln2_2 * bw * w / std::sin(w)));
    }

    template <typename T = double>
    T q2bandwidth(T q, T w)
    {
        /**
         * from 'Audio-EQ-Cookbook' by Robert Bristow-Johnson
         * 1/Q = 2 * sinh(log2/2 * BW * (w/sin(w)))
         * asinh(1/2q) = log2/2 * BW * (w/sin(w))
         * BW = asinh(1/2q) * (sin(w) * 2) / (w * log2)
         */
        const auto sin_w_2 = std::sin(w) * 2;
        const auto wlog2 = w * ln2;

        return std::asinh(1 / (2 * q)) * sin_w_2 / wlog2;
    }

}
}

#endif // CEAMMC_FILTER_H
