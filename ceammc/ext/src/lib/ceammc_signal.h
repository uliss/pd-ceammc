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
#include <cstddef>
#include <limits>

namespace ceammc {

template <class T, size_t N = 512>
class SmoothLinT {
    T step_;
    T current_;
    T target_;

public:
    SmoothLinT(T init = 0, T target = 0)
        : current_(init)
        , target_(target)
        , step_(0)
    {
        setTargetValue(target);
    }

    void setTargetValue(T v)
    {
        target_ = v;
        T new_step = (target_ - current_) / N;

        if (SmoothLinT::abs(new_step) <= std::numeric_limits<T>::epsilon())
            step_ = 0;
        else
            step_ = new_step;
    }

    T operator()()
    {
        if (SmoothLinT::abs(target_ - current_) < std::max<T>(SmoothLinT::abs(step_), std::numeric_limits<T>::epsilon()))
            current_ = target_;
        else
            current_ += step_;

        return current_;
    }

    T current() const { return current_; }
    T step() const { return step_; }
    T target() const { return target_; }

private:
    static T abs(T v) { return v < 0 ? -v : v; }
};
}

#endif // CEAMMC_SIGNAL_H
