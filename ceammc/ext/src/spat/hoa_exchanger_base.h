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
#ifndef HOA_EXCHANGER_BASE_H
#define HOA_EXCHANGER_BASE_H

#include "ceammc_property_enum.h"
#include "ceammc_xlet_info.h"
#include "hoa_common.h"

namespace ceammc {

struct HoaExchangerXlet : XletAnnotationTraits {
    static const char* inletFormat()  { return "harmonic: \\[%d\\]"; }
    static const char* outletFormat() { return "harmonic: \\[%d\\]"; }
};
using XLetInfo = XletAnnotations<HOA_MAX_ORDER, HoaExchangerXlet, 32>;

template <typename HoaType>
class HoaExchangerBase : public HoaBase {
    Buffer in_buf_;
    Buffer out_buf_;
    std::unique_ptr<HoaType> hoa_;
    SymbolEnumProperty* num_ { nullptr };
    SymbolEnumProperty* norm_ { nullptr };

public:
    HoaExchangerBase(const PdArgs& args);
    void initDone() final
    {
        hoa_.reset(new HoaType(order()));

        const size_t NHARM = hoa_->getNumberOfHarmonics();
        hoa_->setNumbering(to_numbering(num_->value()));
        hoa_->setNormalization(to_norm(norm_->value()));

        createSignalInlets(NHARM);
        createSignalOutlets(NHARM);

        // alloc buffers
        in_buf_.resize(NHARM * HOA_DEFAULT_BLOCK_SIZE);
        out_buf_.resize(NHARM * HOA_DEFAULT_BLOCK_SIZE);
    }

    void processBlock(const t_sample** in, t_sample** out) final
    {
        const size_t NOUTS = numOutputChannels();
        const size_t NINS = numInputChannels() - 1; // last input is for Yaw
        const size_t BS = blockSize();

        for (size_t i = 0; i < NINS; i++) {
            Signal::copy(BS, &in[i][0], 1, &in_buf_[i], NINS);
        }

        for (size_t i = 0; i < BS; i++) {
            hoa_->process(&in_buf_[NINS * i], &out_buf_[NOUTS * i]);
        }

        for (size_t i = 0; i < NOUTS; i++) {
            Signal::copy(BS, &out_buf_[i], NOUTS, &out[i][0], 1);
        }
    }

    void blockSizeChanged(size_t bs) final
    {
        const size_t SZ = hoa_->getNumberOfHarmonics() * bs;
        in_buf_.resize(SZ);
        out_buf_.resize(SZ);
    }

    const char* annotateInlet(size_t n) const final
    {
        return XLetInfo::instance().inletInfo(n);
    }

    const char* annotateOutlet(size_t n) const final
    {
        return XLetInfo::instance().outletInfo(n);
    }

public:
    static typename HoaType::Numbering to_numbering(const t_symbol* s);
    static typename HoaType::Normalization to_norm(const t_symbol* s);
    static void initAnnotations() { XLetInfo::instance(); }
};
}

#endif // HOA_EXCHANGER_BASE_H
