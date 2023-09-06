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
#include "hoa_3d_encoder.h"

#include "ceammc_factory.h"
#include "hoa_common.h"

HoaEncoder3D::HoaEncoder3D(const PdArgs& args)
    : HoaBase(args)
{
}

void HoaEncoder3D::parseProperties()
{
    HoaBase::parseProperties();
    encoder_.reset(new Encoder3d(order()));

    createSignalInlets(3); // input, azimuth and elevation
    createSignalOutlets(encoder_->getNumberOfHarmonics());

    signals_.resize(encoder_->getNumberOfHarmonics() * HOA_DEFAULT_BLOCK_SIZE);
}

void HoaEncoder3D::processBlock(const t_sample** in, t_sample** out)
{
    const size_t BS = blockSize();
    const size_t NOUTS = numOutputChannels();

    for (size_t i = 0; i < BS; i++) {
        encoder_->setAzimuth(in[1][i]);
        encoder_->setElevation(in[2][i]);
        encoder_->process(in[0] + i, &signals_[NOUTS * i]);
    }

    for (size_t i = 0; i < NOUTS; i++) {
        Signal::copy(BS, &signals_[i], NOUTS, out[i], 1);
    }
}

void HoaEncoder3D::blockSizeChanged(size_t bs)
{
    signals_.resize(encoder_->getNumberOfHarmonics() * bs);
}

const char* HoaEncoder3D::annotateInlet(size_t n) const
{
    switch (n) {
    case 0:
        return "input signal";
    case 1:
        return "azimuth in radians (ccw)";
    case 2:
    default:
        return "elevation in radians";
    }
}

void setup_spat_hoa_encoder_3d()
{
    SoundExternalFactory<HoaEncoder3D> obj("hoa.3d.encoder~");
}
