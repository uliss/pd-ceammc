/*****************************************************************************
 * Copyright 2023 Serge Poltavski. All rights reserved.
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
#ifndef HOA_WIDER_BASE_H
#define HOA_WIDER_BASE_H

#include "hoa_common.h"

#include <functional>

struct XletAnnotationTraits {
    static constexpr const char* unknownInlet() { return "?"; }
    static constexpr const char* unknownOutlet() { return "?"; }
    static void initInlet(size_t idx, char* str, size_t len)
    {
        snprintf(str, len, inletFormat(), static_cast<int>(idx));
    }
    static void initOutlet(size_t idx, char* str, size_t len)
    {
        snprintf(str, len, outletFormat(), static_cast<int>(idx));
    }
    static constexpr const char* inletFormat() { return "\\[%d\\]"; }
    static constexpr const char* outletFormat() { return "\\[%d\\]"; }
};

template <size_t XLET_NUMBER, typename Traits = XletAnnotationTraits, size_t LEN = 32>
class XletAnnotations {
    std::array<char[LEN], XLET_NUMBER> in_, out_;
    XletAnnotations()
    {
        initInlets();
        initOutlets();
    }

    void initInlets()
    {
        for (size_t i = 0; i < XLET_NUMBER; i++)
            Traits::initInlet(i, in_[i], LEN);
    }

    void initOutlets()
    {
        for (size_t i = 0; i < XLET_NUMBER; i++)
            Traits::initOutlet(i, in_[i], LEN);
    }

public:
    static XletAnnotations& instance()
    {
        static XletAnnotations m_;
        return m_;
    }

    const char* inletInfo(size_t idx) const
    {
        return (idx < in_.size()) ? in_[idx] : Traits::unknownInlet();
    }

    const char* outletInfo(size_t idx) const
    {
        return (idx < out_.size()) ? out_[idx] : Traits::unknownOutlet();
    }
};

namespace ceammc {

struct HoaWiderXlet : XletAnnotationTraits { };

using HoaWiderXLetInfo = XletAnnotations<HOA_MAX_ORDER, HoaWiderXlet, 32>;

template <typename T>
class HoaWiderBase : public HoaBase {
    std::unique_ptr<T> wider_;
    Buffer in_buf_;
    Buffer out_buf_;

public:
    HoaWiderBase(const PdArgs& args)
        : HoaBase(args)
    {
    }

    void initDone() final
    {
        wider_.reset(new T(order()));

        const size_t N = wider_->getNumberOfHarmonics();
        in_buf_.resize(N * HOA_DEFAULT_BLOCK_SIZE);
        out_buf_.resize(N * HOA_DEFAULT_BLOCK_SIZE);

        createSignalInlets(N + 1);
        createSignalOutlets(N);
    }

    void processBlock(const t_sample** in, t_sample** out) final
    {
        const size_t NOUTS = numOutputChannels();
        const size_t NINS = numInputChannels() - 1; // last input is for Widening
        const size_t BS = blockSize();

        for (size_t i = 0; i < NINS; i++)
            Signal::copy(BS, &in[i][0], 1, &in_buf_[i], NINS);

        for (size_t i = 0; i < BS; i++) {
            wider_->setWidening(in[NINS][i]); // read from last inlet
            wider_->process(&in_buf_[NINS * i], &out_buf_[NOUTS * i]);
        }

        for (size_t i = 0; i < NOUTS; i++)
            Signal::copy(BS, &out_buf_[i], NOUTS, &out[i][0], 1);
    }

    const char* annotateInlet(size_t n) const final
    {
        const size_t N = numInputChannels() - 1;
        if (n < N)
            return HoaWiderXLetInfo::instance().inletInfo(n);
        else if (n == N)
            return "signal: widening in \\[0-1\\] range";
        else
            return nullptr;
    }

    const char* annotateOutlet(size_t n) const final
    {
        return HoaWiderXLetInfo::instance().inletInfo(n);
    }

    static void initXletInfo()
    {
        HoaWiderXLetInfo::instance();
    }
};

}

#endif // HOA_WIDER_BASE_H
