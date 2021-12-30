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
#ifndef CEAMMC_DSP_H
#define CEAMMC_DSP_H

#include "m_pd.h"

#include "ceammc_convert.h"
#include "ceammc_signal.h"

#include <algorithm>
#include <cstdint>
#include <iostream>
#include <memory>

namespace ceammc {
namespace dsp {

    class DelayIface {
    public:
        virtual ~DelayIface() { }
        virtual uint32_t maxSize() const noexcept = 0;
        virtual uint32_t delay() const noexcept = 0;
        virtual bool setDelay(uint32_t delay) = 0;
        virtual t_sample push(t_sample v) noexcept = 0;
        virtual void fillWith(t_sample v) noexcept = 0;

        void clear() { fillWith(0); }
    };

    class Delay : public DelayIface {
        std::unique_ptr<t_sample[]> data_;
        const uint32_t max_size_;
        uint32_t in_pos_;
        uint32_t out_pos_;
        uint32_t delay_;

        Delay(const Delay&) = delete;
        Delay& operator=(const Delay& d) = delete;

    public:
        explicit Delay(uint32_t maxSize, uint32_t delay, t_sample init = 0)
            : max_size_(maxSize)
            , in_pos_(0)
            , out_pos_(0)
            , delay_(0)
        {
            if (max_size_ == 0) {
                throw std::invalid_argument("non zero value expected");
            } else {
                data_.reset(new t_sample[max_size_]);
                fillWith(init);
                setDelay(delay);
            }
        }

        uint32_t maxSize() const noexcept final { return max_size_; }
        uint32_t delay() const noexcept final { return delay_; }

        void fillWith(t_sample v) noexcept final { std::fill(data_.get(), data_.get() + max_size_, v); }

        Delay(Delay&& d) noexcept
            : data_(std::move(d.data_))
            , max_size_(d.max_size_)
            , in_pos_(d.in_pos_)
            , out_pos_(d.out_pos_)
            , delay_(d.delay_)
        {
        }

        t_sample push(t_sample v) noexcept final
        {
            data_[in_pos_++] = v;

            if (in_pos_ == max_size_)
                in_pos_ = 0;

            const auto out = data_[out_pos_++];

            if (out_pos_ == max_size_)
                out_pos_ = 0;

            return out;
        }

        bool setDelay(uint32_t delay) noexcept final
        {
            if (delay >= max_size_)
                return false;

            if (in_pos_ >= delay)
                out_pos_ = in_pos_ - delay;
            else
                out_pos_ = max_size_ + in_pos_ - delay;

            delay_ = delay;
            return true;
        }
    };

    template <uint32_t N>
    class StaticDelay : public DelayIface {
        t_sample data_[N];
        uint32_t in_pos_;
        uint32_t out_pos_;
        uint32_t delay_;

        StaticDelay(const StaticDelay<N>&) = delete;
        StaticDelay& operator=(const StaticDelay<N>& d) = delete;

    public:
        explicit StaticDelay(uint32_t delay, t_sample init = 0)
            : in_pos_(0)
            , out_pos_(0)
            , delay_(0)
        {
            fillWith(init);
            setDelay(delay);
        }

        uint32_t maxSize() const noexcept final { return N; }
        uint32_t delay() const noexcept final { return delay_; }

        void fillWith(t_sample v) noexcept final { std::fill(data_, data_ + N, v); }

        t_sample push(t_sample v) noexcept final
        {
            data_[in_pos_++] = v;

            if (in_pos_ == N)
                in_pos_ = 0;

            const auto out = data_[out_pos_++];

            if (out_pos_ == N)
                out_pos_ = 0;

            return out;
        }

        bool setDelay(uint32_t delay) noexcept final
        {
            if (delay >= N)
                return false;

            if (in_pos_ >= delay)
                out_pos_ = in_pos_ - delay;
            else
                out_pos_ = N + in_pos_ - delay;

            delay_ = delay;
            return true;
        }
    };

    namespace curves {
        double a_weight(double freq);
    }

    template <typename T, size_t N>
    class TableFunction {
        T data_[N];
        T a_, b_;

        using Fn = double(double);

    public:
        TableFunction(Fn f, T a, T b)
            : a_(a)
            , b_(b)
        {
            for (size_t i = 0; i < N; i++)
                data_[i] = f(convert::lin2lin_clip<T, 0, N - 1>(i, a, b));
        }

        T at(T x) const
        {
            const auto idx = convert::lin2lin_clip<T>(x, a_, b_, 0, N - 1);
            const size_t idx0 = std::floor(idx);
            const size_t idx1 = std::ceil(idx);
            return interpolate::linear<T>(data_[idx0], data_[idx1], idx1 - idx);
        }

        T min() const { return a_; }
        T max() const { return b_; }
    };
}
}

#endif // CEAMMC_DSP_H
