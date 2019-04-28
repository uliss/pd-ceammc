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
#include "hoa_rotate.h"
#include "ceammc_factory.h"

HoaRotate::HoaRotate(const PdArgs& args)
    : HoaBase(args)
{
}

void HoaRotate::parseProperties()
{
    HoaBase::parseProperties();

    rotate_.reset(new Rotate2d(order()));

    const size_t NHARM = rotate_->getNumberOfHarmonics();

    createSignalInlets(NHARM + 1);
    createSignalOutlets(NHARM);

    // alloc buffers
    in_buf_.resize(NHARM * HOA_DEFAULT_BLOCK_SIZE);
    out_buf_.resize(NHARM * HOA_DEFAULT_BLOCK_SIZE);
}

void HoaRotate::processBlock(const t_sample** in, t_sample** out)
{
    const size_t NOUTS = numOutputChannels();
    const size_t NINS = numInputChannels();
    const size_t BS = blockSize();

    for (size_t i = 0; i < NOUTS; i++) {
        Signal::copy(BS, &in[i][0], 1, &in_buf_[i], NINS);
    }

    for (size_t i = 0; i < BS; i++) {
        rotate_->setYaw(in[NINS - 1][i]);
        // CHECK THIS
        // WARN!
        rotate_->process(&in_buf_[NINS * i], &out_buf_[NOUTS * i]);
    }

    for (size_t i = 0; i < NOUTS; i++) {
        Signal::copy(BS, &out_buf_[i], NOUTS, &out[i][0], 1);
    }
}

void HoaRotate::blocksizeChanged(size_t bs)
{
    const size_t SZ = rotate_->getNumberOfHarmonics() * bs;
    in_buf_.resize(SZ);
    out_buf_.resize(SZ);
}

void setup_spat_hoa_rotate()
{
    SoundExternalFactory<HoaRotate> obj("hoa.rotate~");
}
