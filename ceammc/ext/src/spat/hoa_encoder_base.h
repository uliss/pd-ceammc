/*****************************************************************************
 * Copyright 2023 Serge Poltavsky. All rights reserved.
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
#include "hoa_common.h"

namespace ceammc {

template <typename HoaType, size_t N, hoa::Dimension D>
class HoaEncoderBase : public HoaBase<D> {
    std::unique_ptr<HoaType> encoder_;
    Buffer signals_;

public:
    HoaEncoderBase(const PdArgs& args)
        : HoaBase<D>(args)
    {
    }

    void parseProperties() final
    {
        HoaBase<D>::parseProperties();
        encoder_.reset(new HoaType(this->order()));

        this->createSignalInlets(N); // input, azimuth and elevation
        this->createSignalOutlets(encoder_->getNumberOfHarmonics());

        signals_.resize(encoder_->getNumberOfHarmonics() * HOA_DEFAULT_BLOCK_SIZE);
    }

    void processBlock(const t_sample** in, t_sample** out) final
    {
        const size_t BS = this->blockSize();
        const size_t NOUTS = this->numOutputChannels();

        for (size_t i = 0; i < BS; i++) {
            encoder_->setAzimuth(in[1][i]);
            encoder_->setElevation(in[2][i]);
            encoder_->process(in[0] + i, &signals_[NOUTS * i]);
        }

        for (size_t i = 0; i < NOUTS; i++) {
            Signal::copy(BS, &signals_[i], NOUTS, out[i], 1);
        }
    }

    void blockSizeChanged(size_t bs) final
    {
        signals_.resize(encoder_->getNumberOfHarmonics() * bs);
    }

    const char* annotateInlet(size_t) const final { return nullptr; }
};

}
