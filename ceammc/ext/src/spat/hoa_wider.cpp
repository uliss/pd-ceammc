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
#include "hoa_wider.h"
#include "ceammc_factory.h"

HoaWider::HoaWider(const PdArgs& args)
    : HoaBase(args)
{
}

void HoaWider::parseProperties()
{
    HoaBase::parseProperties();

    wider_.reset(new Wider2d(order()));

    const size_t N = wider_->getNumberOfHarmonics();
    in_buf_.resize(N * HOA_DEFAULT_BLOCK_SIZE);
    out_buf_.resize(N * HOA_DEFAULT_BLOCK_SIZE);

    createSignalInlets(N + 1);
    createSignalOutlets(N);
}

void HoaWider::processBlock(const t_sample** in, t_sample** out)
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

void setup_spat_hoa_wider()
{
    SoundExternalFactory<HoaWider> obj("hoa.2d.wider~");
    obj.addAlias("hoa.wider~");
}
