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

#include <algorithm>
#include <cstdint>
#include <memory>

namespace ceammc {
namespace dsp {

    class Delay {
        std::unique_ptr<t_sample[]> data_;
        const uint32_t max_size_;
        uint32_t in_pos_;
        uint32_t out_pos_;
        uint32_t delay_;

        Delay(const Delay&) = delete;
        Delay& operator=(const Delay& d) = delete;

    public:
        explicit Delay(uint32_t maxSize, uint32_t delay)
            : max_size_(maxSize)
            , in_pos_(0)
            , out_pos_(0)
            , delay_(0)
        {
            if (max_size_ == 0) {
                throw std::invalid_argument("non zero value expected");
            } else {
                data_.reset(new t_sample[max_size_]);
                setDelay(delay);
            }
        }

        Delay(Delay&& d) noexcept
            : data_(std::move(d.data_))
            , max_size_(d.max_size_)
            , in_pos_(d.in_pos_)
            , out_pos_(d.out_pos_)
            , delay_(d.delay_)
        {
        }

        t_sample push(t_sample v) noexcept
        {
            data_[in_pos_++] = v;

            if (in_pos_ == max_size_)
                in_pos_ = 0;

            const auto out = data_[out_pos_++];

            if (out_pos_ == max_size_)
                out_pos_ = 0;

            return out;
        }

        bool setDelay(uint32_t delay) noexcept
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
}
}

#endif // CEAMMC_DSP_H
