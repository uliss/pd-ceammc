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
#ifndef CEAMMC_NUI_RECT_H
#define CEAMMC_NUI_RECT_H

#include "nui/point.h"
#include "nui/size.h"

#include <algorithm>

namespace ceammc {
namespace ui {

    template <typename T>
    class RectT {
        T x0_, y0_, x1_, y1_;

    public:
        RectT()
            : RectT(0, 0, 0, 0)
        {
        }

        RectT(T x0, T y0, T x1, T y1)
            : x0_(x0)
            , y0_(y0)
            , x1_(x1)
            , y1_(y1)
        {
        }

        RectT(T x, T y, const SizeT<T>& sz)
            : x0_(x)
            , y0_(y)
            , x1_(x + sz.width())
            , y1_(y + sz.height())
        {
        }

        template <typename U>
        RectT(const PointT<U>& pt, const SizeT<U>& sz)
            : RectT<T>(pt.x(), pt.y(), pt.x() + sz.width(), pt.y() + sz.height())
        {
        }

        template <typename U>
        RectT(const RectT<U>& r)
        {
            x0_ = r.left();
            x1_ = r.right();
            y0_ = r.top();
            y1_ = r.bottom();
        }

        RectT& operator=(const RectT& r)
        {
            x0_ = r.x0_;
            x1_ = r.x1_;
            y0_ = r.y0_;
            y1_ = r.y1_;
            return *this;
        }

        template <typename U>
        RectT<T>& operator=(const RectT<U>& r)
        {
            x0_ = r.x0_;
            x1_ = r.x1_;
            y0_ = r.y0_;
            y1_ = r.y1_;
            return *this;
        }

        RectT<T>& normalize()
        {
            std::tie(x0_, x1_) = std::minmax<T>(x0_, x1_);
            std::tie(y0_, y1_) = std::minmax<T>(y0_, y1_);
            return *this;
        }

        template <typename U>
        RectT<T> operator*(U k) const
        {
            return RectT<T>(x0_ * k, y0_ * k, x1_ * k, y1_ * k);
        }

        template <typename U>
        RectT<T>& operator*=(U k)
        {
            x0_ *= k;
            x1_ *= k;
            y0_ *= k;
            y1_ *= k;
            return *this;
        }

        template <typename U>
        RectT<T>& operator+=(U k)
        {
            x0_ += k;
            x1_ += k;
            y0_ += k;
            y1_ += k;
            return *this;
        }

        template <typename U>
        RectT<T>& operator-=(U k)
        {
            x0_ -= k;
            x1_ -= k;
            y0_ -= k;
            y1_ -= k;
            return *this;
        }

        template <typename U>
        RectT<T> operator/(U k) const
        {
            return RectT<T>(x0_ / k, y0_ / k, x1_ / k, y1_ / k);
        }

        template <typename U>
        RectT<T>& operator/=(U k)
        {
            x0_ /= k;
            x1_ /= k;
            y0_ /= k;
            y1_ /= k;
            return *this;
        }

        T width() const { return x1_ - x0_; }
        T height() const { return y1_ - y0_; }
        T left() const { return x0_; }
        T top() const { return y0_; }
        T right() const { return x1_; }
        T bottom() const { return y1_; }

        SizeT<T> size() const { return SizeT<T>(width(), height()); }

        PointT<T> pt0() const { return PointT<T>(x0_, y0_); }
        PointT<T> pt1() const { return PointT<T>(x1_, y1_); }

        T area() const { return size().area(); }

        bool operator==(const RectT<T>& r) const
        {
            return x0_ == r.x0_
                && x1_ == r.x1_
                && y0_ == r.y0_
                && y1_ == r.y1_;
        }

        bool operator!=(const RectT<T>& r) const
        {
            return !this->operator==(r);
        }

        RectT<T> unite(const RectT<T>& r) const
        {
            return RectT<T>(
                std::min<T>(left(), r.left()),
                std::min<T>(top(), r.top()),
                std::max<T>(right(), r.right()),
                std::max<T>(bottom(), r.bottom()));
        }

        bool overlap(const RectT& r) const
        {
            const bool no_overlap = x0_ > r.x1_
                || x1_ < r.x0_
                || y0_ > r.y1_
                || y1_ < r.y0_;

            return !no_overlap;
        }

        RectT<T> intersect(const RectT<T>& r) const
        {
            const T x0 = std::max<T>(left(), r.left());
            const T x1 = std::min<T>(right(), r.right());

            if (x0 > x1)
                return RectT<T>(0, 0, 0, 0);

            const T y0 = std::max<T>(top(), r.top());
            const T y1 = std::min<T>(bottom(), r.bottom());
            if (y0 > y1)
                return RectT<T>(0, 0, 0, 0);

            return RectT<T>(x0, y0, x1, y1);
        }

        bool contains(const PointT<T>& p) const
        {
            return p.x() >= left() && p.x() <= right()
                && p.y() >= top() && p.y() <= bottom();
        }

        RectT<T>& moveBy(T dx, T dy)
        {
            x0_ += dx;
            x1_ += dx;
            y0_ += dy;
            y1_ += dy;
            return *this;
        }

        RectT<T>& moveBy(const PointT<T>& dt)
        {
            x0_ += dt.x();
            x1_ += dt.x();
            y0_ += dt.y();
            y1_ += dt.y();
            return *this;
        }
    };

    using Rect = RectT<int32_t>;
    using RectF = RectT<float>;

    template <typename T>
    std::ostream& operator<<(std::ostream& os, const RectT<T>& r)
    {
        os << "Rect: " << r.left() << ' ' << r.top() << ' ' << r.width() << ' ' << r.height();
        return os;
    }
}
}

#endif // CEAMMC_NUI_RECT_H
