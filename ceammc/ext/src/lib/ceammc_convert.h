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

#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif

#include <algorithm>
#include <cmath>
#include <cstdlib> // std::ldiv
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

template <typename T, int Min, int Max>
T clip(T v)
{
    static_assert(Min < Max, "Min should be less then Max");
    return std::min<T>(T(Max), std::max<T>(v, T(Min)));
}

template <typename T>
T clip01(T v)
{
    return clip<T, 0, 1>(v);
}

template <typename T>
T clip_min(T v, T min)
{
    return std::max<T>(v, min);
}

template <typename T, int min>
T clip_min(T v)
{
    return std::max<T>(v, min);
}

template <typename T>
T clip_max(T v, T max)
{
    return std::min<T>(v, max);
}

template <typename T, int max>
T clip_max(T v)
{
    return std::min<T>(v, max);
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

/**
 * maps [-n n) range to [0 n)
 * negative indexes mapped in backward directions
 * -3 -2 -1 0 1 2
 * maps to
 * +0 +1 +2 0 1 2
 *
 * @return -1 if input value outside of [-n, n) range
 */
template <typename T>
T relativeIndex(T v, size_t n)
{
    static_assert(std::is_integral<T>(), "Integral type expected");
    static_assert(std::is_signed<T>(), "Signed type expected");

    typedef typename std::make_signed<T>::type Signed;

    const Signed N(static_cast<Signed>(n));

    if (v >= N || v < -N)
        return -1;

    return (v < 0) ? v + N : v;
}

/**
 * n > 0
 * map index -> [0, n) range,
 * negative indexes mapped in backward directions
 * @example -1 -> n-1, -2 -> n-2, etc.
 * -4 -3 -2 -1 0 1 2 3 4 5
 * with n=3 maps to
 * +1 +0 +1 +2 0 1 2 0 1 2
 */
template <typename T>
T normalizeIndex(T v, size_t n)
{
    static_assert(std::is_integral<T>(), "Integral type expected");
    static_assert(std::is_signed<T>(), "Signed type expected");

    using Signed = typename std::make_signed<T>::type;
    const Signed N(static_cast<Signed>(n));
    const auto rem = std::ldiv(v, N).rem;
    return (rem < 0) ? rem + N : rem;
}

/**
 * wrap x -> [0,max)
 */
template <typename T>
T wrapFloatMax(T x, T max)
{
    static_assert(std::is_floating_point<T>(), "Floating point type expected");
    /* integer math: `(max + x % max) % max` */
    return std::fmod(max + std::fmod(x, max), max);
}

/**
 * wrap x -> [min,max)
 */
template <typename T>
T wrapFloatMinMax(T x, T min, T max)
{
    return min + wrapFloatMax<T>(x - min, max - min);
}

namespace convert {
    namespace time {
        /**
         * @brief converts time in seconds to formatted time string: "00:00:00"
         */
        std::string sec2str(float sec, bool ms = false);

        float str2sec(const std::string& str, float def = 0.f);
    }

    namespace color {
        static inline int rgb2pd(int r, int g, int b)
        {
            return -(r * 65536 + g * 256 + b + 1);
        }

    }

    template <class T>
    T lin2lin(T v, T x0, T x1, T y0, T y1)
    {
        return (v - x0) / (x1 - x0) * (y1 - y0) + y0;
    }

    template <class T, int x0, int x1>
    T lin2lin(T v, T y0, T y1)
    {
        static_assert(x0 != x1, "Zero input range");
        return (v - T(x0)) / (T(x1) - T(x0)) * (y1 - y0) + y0;
    }

    template <class T>
    T lin2lin_clip(T v, T x0, T x1, T y0, T y1)
    {
        auto yr = std::minmax(y0, y1);
        return clip<T>(lin2lin<T>(v, x0, x1, y0, y1), yr.first, yr.second);
    }

    template <class T, int x0, int x1>
    T lin2lin_clip(T v, T y0, T y1)
    {
        static_assert(x0 != x1, "Zero input range");
        return (clip<T, x0, x1>(v) - T(x0)) / (T(x1) - T(x0)) * (y1 - y0) + y0;
    }

    float lin2exp(float x, float x0, float x1, float y0, float y1);
    double lin2exp(double x, double x0, double x1, double y0, double y1);
    float exp2lin(float x, float x0, float x1, float y0, float y1);
    double exp2lin(double x, double x0, double x1, double y0, double y1);
    float lin2curve(float x, float x0, float x1, float y0, float y1, float curve);
    double lin2curve(double x, double x0, double x1, double y0, double y1, double curve);

    double lin2sin2(double x, double x0, double x1, double y0, double y1);
    double lin2sigmoid(double x, double x0, double x1, double y0, double y1, double skew = 10);

    float dbfs2amp(float db);
    double dbfs2amp(double db);
    float amp2dbfs(float amp);
    double amp2dbfs(double amp);

    enum MidiResult {
        MIDI_NONE = -1,
        MIDI_REST = -2,
    };

    int spn2midi(const char* p);

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

    /**
     * x -> [0, 2pi)
     */
    template <typename T>
    T phase2rad(T x)
    {
        static_assert(std::is_floating_point<T>(), "Float type expected");
        return wrapFloatMax<T>(x, 1) * (2 * M_PI);
    }

    /**
     * x -> [0, 360)
     */
    template <typename T>
    T phase2degree(T x)
    {
        static_assert(std::is_floating_point<T>(), "Float type expected");
        return wrapFloatMax<T>(x * 360, 360);
    }

    /**
     * x -> [0, 1)
     */
    template <typename T>
    T degree2phase(T x)
    {
        static_assert(std::is_floating_point<T>(), "Float type expected");
        return wrapFloatMax<T>(x, 360) / 360;
    }

    /**
     * x -> [0, 2pi)
     */
    template <typename T>
    T degree2rad(T x)
    {
        static_assert(std::is_floating_point<T>(), "Float type expected");
        return (wrapFloatMax<T>(x, 360) / 360) * (2 * M_PI);
    }

    /**
     * x -> [0, 1)
     */
    template <typename T>
    T rad2phase(T x)
    {
        static_assert(std::is_floating_point<T>(), "Float type expected");
        return wrapFloatMax<T>(x / (2 * M_PI), 1);
    }

    /**
     * x -> [0, 360)
     */
    template <typename T>
    T rad2degree(T x)
    {
        static_assert(std::is_floating_point<T>(), "Float type expected");
        return wrapFloatMax<T>(x * 360 / (2 * M_PI), 360);
    }

    template <typename T>
    std::pair<T, T> cartesian2polar(T x, T y)
    {
        static_assert(std::is_floating_point<T>(), "Float type expected");
        return { std::hypot(x, y), std::atan2(y, x) };
    }

    template <typename T>
    std::pair<T, T> polar2cartesian(T r, T theta)
    {
        static_assert(std::is_floating_point<T>(), "Float type expected");
        return { r * std::cos(theta), r * std::sin(theta) };
    }

    template <typename T>
    T tau2pole(T tau, T sr)
    {
        static_assert(std::is_floating_point<T>(), "Float type expected");

        if (std::abs(tau) < std::numeric_limits<T>::epsilon())
            return 0;
        else
            return std::exp(-1 / (tau * sr));
    }

    template <typename T>
    T pole2tau(T pole, T sr)
    {
        static_assert(std::is_floating_point<T>(), "Float type expected");
        return -1.0 / (std::log(std::max(std::numeric_limits<T>::min(), pole)) * sr);
    }

    static inline double sec2tau(double s) { return s / std::log(1000); }
    static inline float sec2tau(float s) { return s / std::logf(1000); }

    static inline double tau2sec(double t) { return t * std::log(1000); }
    static inline float tau2sec(float t) { return t * std::logf(1000); }

    static inline double msec2tau(double ms) { return sec2tau(ms * 0.001); }
    static inline float msec2tau(float ms) { return sec2tau(ms * 0.001f); }

    static inline double tau2msec(double t) { return tau2sec(t) * 1000; }
    static inline float tau2msec(float t) { return tau2sec(t) * 1000; }
}
}

#endif // CEAMMC_CONVERT_H
