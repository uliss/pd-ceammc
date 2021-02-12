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
#ifndef CEAMMC_NUI_SIZE_H
#define CEAMMC_NUI_SIZE_H

#include "ceammc_nui_point.h"

#include <cstdint>

namespace ceammc {
namespace ui {

    template <typename T>
    class SizeT {
        T w_, h_;

    public:
        SizeT()
            : SizeT(0, 0)
        {
        }

        SizeT(T w, T h)
            : w_(w)
            , h_(h)
        {
        }

        template <typename U>
        SizeT(const SizeT<U>& sz)
        {
            w_ = sz.width();
            h_ = sz.height();
        }

        T width() const { return w_; }
        T height() const { return h_; }

        void setHeight(T h) { h_ = h; }
        void setWidth(T w) { w_ = w; }

        void set(T w, T h)
        {
            w_ = w;
            h_ = h;
        }

        T& rwidth() { return w_; }
        T& rheight() { return h_; }

        template <typename U>
        SizeT<T> operator*(U x) const { return SizeT<T>(w_ * x, h_ * x); }

        template <typename U>
        SizeT<T> operator/(U x) const { return SizeT<T>(w_ / x, h_ / x); }

        bool operator==(const SizeT<T>& sz) const
        {
            return w_ == sz.w_ && h_ == sz.h_;
        }

        bool operator!=(const SizeT<T>& sz) const
        {
            return !this->operator==(sz);
        }

        PointT<T> center() const
        {
            return PointT<T>(w_ / 2, h_ / 2);
        }

        PointT<T> leftCenter() const
        {
            return PointT<T>(0, h_ / 2);
        }

        T area() const { return w_ * h_; }

        SizeT<T>& enlarge(T dw, T dh)
        {
            w_ += dw;
            h_ += dh;
            return *this;
        }
    };

    using Size = SizeT<int32_t>;
    using SizeF = SizeT<float>;
}
}

#endif // CEAMMC_NUI_SIZE_H
