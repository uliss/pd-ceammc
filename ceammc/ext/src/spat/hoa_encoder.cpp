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
#include "hoa_encoder.h"

#include "ceammc_factory.h"
#include "hoa_common.h"

HoaEncoder::HoaEncoder(const PdArgs& args)
    : HoaBase(args)
{
}

void HoaEncoder::parseProperties()
{
    HoaBase::parseProperties();
    encoder_.reset(new Encoder2d(order()));

    createSignalInlets(2);
    createSignalOutlets(encoder_->getNumberOfHarmonics());

    signals_.resize(encoder_->getNumberOfHarmonics() * HOA_DEFAULT_BLOCK_SIZE);
}

void HoaEncoder::processBlock(const t_sample** in, t_sample** out)
{
    const size_t BS = blockSize();
    const size_t NOUTS = numOutputChannels();

    for (size_t i = 0; i < BS; i++) {
        encoder_->setAzimuth(in[1][i]);
        encoder_->process(in[0] + i, &signals_[NOUTS * i]);
    }

    for (size_t i = 0; i < NOUTS; i++) {
        Signal::copy(BS, &signals_[i], NOUTS, out[i], 1);
    }
}

void HoaEncoder::blockSizeChanged(size_t bs)
{
    signals_.resize(encoder_->getNumberOfHarmonics() * bs);
}

const char* HoaEncoder::annotateInlet(size_t n) const
{
    if (n == 0)
        return "input signal";
    else
        return "angle in radians (ccv)";
}

void setup_spat_hoa_encoder()
{
    SoundExternalFactory<HoaEncoder> obj("hoa.2d.encoder~");
    obj.addAlias("hoa.encoder~");
}
