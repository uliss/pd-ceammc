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
#include "hoa_projector.h"
#include "ceammc_factory.h"

HoaProjector::HoaProjector(const PdArgs& args)
    : HoaBase(args)
    , plane_waves_(nullptr)
{
    plane_waves_ = new IntProperty("@n", 0);
    plane_waves_->setInitOnly();
    plane_waves_->setArgIndex(1);
    addProperty(plane_waves_);
}

void HoaProjector::initDone()
{
    const int MIN_PW_COUNT = 2 * order() + 1;
    const auto N = plane_waves_->value();

    if (N == 0) { // auto calc
        plane_waves_->setValue(MIN_PW_COUNT);
    } else if (N < MIN_PW_COUNT) { // set to minimal
        OBJ_ERR << "minimal number of plane waves should be >= " << MIN_PW_COUNT << ", setting to this value";
        plane_waves_->setValue(MIN_PW_COUNT);
    }

    processor_.reset(new DecoderRegular2d(order(), plane_waves_->value()));

    createSignalInlets(processor_->getNumberOfHarmonics());
    createSignalOutlets(processor_->getNumberOfPlanewaves());

    in_buf_.resize(numInputChannels() * HOA_DEFAULT_BLOCK_SIZE);
    out_buf_.resize(numOutputChannels() * HOA_DEFAULT_BLOCK_SIZE);
}

void HoaProjector::blockSizeChanged(size_t bs)
{
    in_buf_.resize(numInputChannels() * bs);
    out_buf_.resize(numOutputChannels() * bs);
}

void HoaProjector::processBlock(const t_sample** in, t_sample** out)
{
    const size_t NINS = numInputChannels();
    const size_t NOUTS = numOutputChannels();
    const size_t BS = blockSize();

    for (size_t i = 0; i < NINS; i++)
        Signal::copy(BS, &in[i][0], 1, &in_buf_[i], NINS);

    for (size_t i = 0; i < BS; i++)
        processor_->process(&in_buf_[NINS * i], &out_buf_[NOUTS * i]);

    for (size_t i = 0; i < NOUTS; i++)
        Signal::copy(BS, &out_buf_[i], NOUTS, &out[i][0], 1);
}

void setup_spat_hoa_projector()
{
    SoundExternalFactory<HoaProjector> obj("hoa.2d.projector~");
    obj.addAlias("hoa.projector~");
}
