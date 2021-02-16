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
#ifndef CEAMMC_NUI_POINT_H
#define CEAMMC_NUI_POINT_H

#include <cmath>
#include <complex>
#include <cstdint>
#include <type_traits>

namespace ceammc {
namespace ui {

    template <typename T>
    class PointT {
        T x_, y_;

    public:
        PointT()
            : PointT(0, 0)
        {
        }

        PointT(T x, T y)
            : x_(x)
            , y_(y)
        {
        }

        template <typename U>
        PointT(const PointT<U>& pt)
            : x_(pt.x())
            , y_(pt.y())
        {
        }

        template <typename U>
        PointT<T>& operator=(const PointT<U>& pt)
        {
            x_ = pt.x();
            y_ = pt.y();
            return *this;
        }

        T x() const { return x_; }
        T y() const { return y_; }

        void setX(T x) { x_ = x; }
        void setY(T y) { y_ = y; }

        void set(T x, T y)
        {
            x_ = x;
            y_ = y;
        }

        T& rx() { return x_; }
        T& ry() { return y_; }

        PointT<T> operator+(const PointT<T>& pt) const
        {
            return PointT<T>(x_ + pt.x_, y_ + pt.y_);
        }

        PointT<T>& operator+=(const PointT<T>& pt)
        {
            x_ += pt.x_;
            y_ += pt.y_;
            return *this;
        }

        PointT<T> operator-(const PointT<T>& pt) const
        {
            return PointT<T>(x_ - pt.x_, y_ - pt.y_);
        }

        PointT<T>& operator-=(const PointT<T>& pt)
        {
            x_ -= pt.x_;
            y_ -= pt.y_;
            return *this;
        }

        template <typename U>
        PointT<T> operator*(U k) const
        {
            return PointT<T>(x_ * k, y_ * k);
        }

        template <typename U>
        PointT<T>& operator*=(U k)
        {
            x_ *= k;
            y_ *= k;
            return *this;
        }

        template <typename U>
        PointT<T> operator/(U k) const
        {
            return PointT<T>(x_ / k, y_ / k);
        }

        template <typename U>
        PointT<T>& operator/=(U k)
        {
            x_ /= k;
            y_ /= k;
            return *this;
        }

        bool operator==(const PointT<T>& pt) const
        {
            return x_ == pt.x_ && y_ == pt.y_;
        }

        bool operator!=(const PointT<T>& pt) const
        {
            return !operator==(pt);
        }

        T angle() const
        {
            static_assert(std::is_floating_point<T>::value, "float type expected");
            return std::arg(std::complex<T>(x_, y_));
        }

        T distance(const PointT<T>& pt) const
        {
            static_assert(std::is_floating_point<T>::value, "float type expected");

            const auto dx = pt.x_ - x_;
            const auto dy = pt.y_ - y_;
            return std::sqrt(dx * dx + dy * dy);
        }
    };

    using Point = PointT<int32_t>;
    using PointF = PointT<float>;
}
}

#endif // CEAMMC_NUI_POINT_H
