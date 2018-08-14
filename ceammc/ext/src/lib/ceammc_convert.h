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

#include <algorithm>
#include <cmath>
#include <string>
#include <type_traits>

namespace ceammc {

// assume min <= max
template <class T>
T clip(T v, T min, T max)
{
    return std::min(max, std::max(v, min));
}

// not assume v0 <= v1
template <class T>
T clip_any(T v, T v0, T v1)
{
    auto r = std::minmax(v0, v1);
    return clip<T>(v, r.first, r.second);
}

/**
 * Wraps input values to specified range:
 * -3 -2 -1 0 1 2 3
 * becames
 * +0 +1 +2 0 1 2 3 etc..
 *
 * @param n
 * @return number in range [0, n)
 */
template <typename T>
typename std::make_unsigned<T>::type wrapInteger(T v, typename std::make_unsigned<T>::type n)
{
    static_assert(std::is_integral<T>(), "Integral type expected");
    static_assert(std::is_signed<T>(), "Signed type expected");

    typedef typename std::make_unsigned<T>::type Unsigned;
    typedef typename std::make_signed<T>::type Signed;

    if (v >= 0)
        return Unsigned(v) % n;
    else
        return (n - 1) - ((Unsigned(-v) - 1) % n);
}

/**
 * Folds input values to specified range:
 * -7 -6 -5 -4 -3 -2 -1 0 1 2 3 4 5 6 7 8 9
 * becames
 * +1 +0 +1 +2 +3 +2 +1 0 1 2 3 2 1 0 1 2 3 etc..
 *
 * @param n
 * @return number in range [0, n)
 */
template <typename T>
typename std::make_unsigned<T>::type foldInteger(T v, typename std::make_unsigned<T>::type n)
{
    static_assert(std::is_integral<T>(), "Integral type expected");
    static_assert(std::is_signed<T>(), "Signed type expected");

    typedef typename std::make_unsigned<T>::type Unsigned;
    typedef typename std::make_signed<T>::type Signed;

    if (n == 1)
        return 0;

    if (v < 0)
        v = -v;

    Unsigned period = 2 * (n - 1);
    Unsigned wrap2n = Unsigned(v) % period;
    Unsigned wrap2n_comp = period - wrap2n;
    Unsigned is_upper = wrap2n / (n - 1);
    Unsigned is_lower = Unsigned(1) ^ is_upper;

    return (is_lower * wrap2n) | (is_upper * wrap2n_comp);
}

template <typename T>
T relativeIndex(T v, typename std::make_unsigned<T>::type n)
{
    static_assert(std::is_integral<T>(), "Integral type expected");
    static_assert(std::is_signed<T>(), "Signed type expected");

    typedef typename std::make_unsigned<T>::type Unsigned;
    typedef typename std::make_signed<T>::type Signed;

    const Signed N(n);

    if (v >= N || v < -N)
        return -1;

    return (v < 0) ? v + N : v;
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

    template <class T>
    T lin2lin_clip(T v, T x0, T x1, T y0, T y1)
    {
        auto yr = std::minmax(y0, y1);
        return clip<T>(lin2lin<T>(v, x0, x1, y0, y1), yr.first, yr.second);
    }

    float lin2exp(float x, float x0, float x1, float y0, float y1);
    double lin2exp(double x, double x0, double x1, double y0, double y1);
    float exp2lin(float x, float x0, float x1, float y0, float y1);
    double exp2lin(double x, double x0, double x1, double y0, double y1);
    float lin2curve(float x, float x0, float x1, float y0, float y1, float curve);
    double lin2curve(double x, double x0, double x1, double y0, double y1, double curve);

    int spn2midi(const char* p);

    double lin2sin2(double x, double x0, double x1, double y0, double y1);
    double lin2sigmoid(double x, double x0, double x1, double y0, double y1, double skew = 10);

    float dbfs2amp(float db);
    double dbfs2amp(double db);
    float amp2dbfs(float amp);
    double amp2dbfs(double amp);

    // 12-TET convert
    float midi2freq(float note, float a_freq = 440.f);
    double midi2freq(double note, double a_freq = 440);

    /**
     * @brief converts frequency to midi scale
     * @param freq - input frequency
     * @param a_freq - base A freq
     * @return -1 on invalid arguments
     */
    float freq2midi(float freq, float a_freq = 440.f);
    double freq2midi(double freq, double a_freq = 440);
}
}

#endif // CEAMMC_CONVERT_H
