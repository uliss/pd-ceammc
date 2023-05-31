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
#ifndef CEAMMC_SIGNAL_H
#define CEAMMC_SIGNAL_H

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <limits>

namespace ceammc {

namespace interpolate {
    template <typename T>
    static T linear(T x0, T x1, T t)
    {
        return x0 + t * (x1 - x0);
    }

    template <typename T>
    T cubic_hermite(T x0, T x1, T x2, T x3, T t)
    {
        const T a = -x0 / 2 + (3 * x1) / 2 - (3 * x2) / 2 + x3 / 2;
        const T b = x0 - (5 * x1) / 2 + 2 * x2 - x3 / 2;
        const T c = -x0 / 2 + x2 / 2;
        const T d = x1;

        return a * t * t * t + b * t * t + c * t + d;
    }
}

template <typename T>
class SmoothExpT {
    T y_, x_, smooth_;

public:
    SmoothExpT(T init = 0, T target = 0, T smooth = 0.997)
        : y_(init)
        , x_(target)
        , smooth_(std::min<T>(1, std::max<T>(0, smooth)))
    {
        static_assert(std::is_floating_point<T>::value, "not floating point type");
    }

    T tick()
    {
        if (SmoothExpT::abs(x_ - y_) <= std::numeric_limits<T>::epsilon())
            y_ = x_;
        else // y[n] = (1 - s) * x[n] + s * y[n - 1]
            y_ = ((1 - smooth_) * x_) + (smooth_ * y_);

        return y_;
    }

    T operator()()
    {
        return tick();
    }

    T current() const { return y_; }
    T target() const { return x_; }
    T smooth() const { return smooth_; }

    void setTarget(T x) { x_ = x; }
    void setSmooth(T s) { smooth_ = s; }

    void setSmoothSec(T sec, T sr)
    {
        const auto tau = sec / std::log(1000);
        if (std::abs(tau) < std::numeric_limits<T>::epsilon())
            smooth_ = 0;
        else
            smooth_ = std::exp(-1 / (tau * sr));
    }

private:
    static T abs(T v) { return v < 0 ? -v : v; }
};

template <class T>
class SmoothLinT {
    T current_;
    T target_;
    T step_;
    size_t N_;
    bool done_;

public:
    SmoothLinT(T init = 0, T target = 0, size_t duration = 512)
        : current_(init)
        , target_(target)
        , step_(0)
        , N_(std::max<size_t>(duration, 1))
        , done_(false)
    {
        static_assert(std::is_floating_point<T>::value, "not floating point type");
        setTargetValue(target);
    }

    void setTargetValue(T v)
    {
        target_ = v;
        T new_step = (target_ - current_) / N_;

        if (SmoothLinT::abs(new_step) <= std::numeric_limits<T>::epsilon())
            step_ = 0;
        else
            step_ = new_step;

        done_ = false;
    }

    void setDuration(size_t samples)
    {
        N_ = std::max<size_t>(samples, 1);
        setTargetValue(target_);
    }

    void setDurationMs(double ms, double samplerate)
    {
        size_t N = static_cast<size_t>(std::max<double>(1, std::round(ms * (0.001 * samplerate))));
        setDuration(N);
    }

    T operator()()
    {
        if (done_)
            return current_;

        if (SmoothLinT::abs(target_ - current_)
            < std::max<T>(SmoothLinT::abs(step_), std::numeric_limits<T>::epsilon())) {
            current_ = target_;
            done_ = true;
        } else
            current_ += step_;

        return current_;
    }

    T current() const { return current_; }
    T step() const { return step_; }
    T target() const { return target_; }
    bool isDone() const { return done_; }

private:
    static T abs(T v) { return v < 0 ? -v : v; }
};
}

#endif // CEAMMC_SIGNAL_H
