/*****************************************************************************
 * Copyright 2019 Serge Poltavsky. All rights reserved.
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
#ifndef CEAMMC_MAYBE_H
#define CEAMMC_MAYBE_H

#include <functional>
#include <iostream>
#include <stdexcept>
#include <type_traits>

namespace ceammc {

template <typename T>
class Maybe {
    T value_;
    bool null_;

public:
    Maybe(T v)
        : value_(v)
        , null_(false)
    {
    }

    Maybe()
        : value_()
        , null_(true)
    {
    }

    Maybe(const Maybe& m)
        : value_(m.value_)
        , null_(m.null_)
    {
    }

    Maybe(Maybe&& m)
        : value_(std::move(m.value_))
        , null_(m.null_)
    {
    }

    const Maybe<T>& operator=(const Maybe& m) const
    {
        value_ = m.value_;
        null_ = m.null_;
        return *this;
    }

    const Maybe<T>& operator=(Maybe&& m) const
    {
        value_ = std::move(m.value_);
        null_ = m.null_;
        return *this;
    }

    operator bool() const
    {
        return !null_;
    }

    const T& operator*() const
    {
        return value();
    }

    const T& value() const
    {
        if (null_)
            throw std::runtime_error("null value");

        return value_;
    }

    T& value()
    {
        if (null_)
            throw std::runtime_error("null value");

        return value_;
    }

    template <class U, class F>
    Maybe<U> call(std::function<U(T)> fn)
    {
        if (null_)
            return Maybe<U>();
        else
            return Maybe<U>(fn(value_));
    }
};

template <typename T>
std::ostream& operator<<(std::ostream& os, const Maybe<T>& m)
{
    if (m)
        os << *m;
    else
        os << "Nothing";

    return os;
}

}

#endif // CEAMMC_MAYBE_H
