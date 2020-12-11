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

std::array<char[HoaRotate::ANNOT_LEN], HOA_MAX_ORDER> HoaRotate::xlet_annotations_;

HoaRotate::HoaRotate(const PdArgs& args)
    : HoaBase(args)
{
}

void HoaRotate::initDone()
{
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
    const size_t NINS = numInputChannels() - 1; // last input is for Yaw
    const size_t BS = blockSize();

    for (size_t i = 0; i < NINS; i++) {
        Signal::copy(BS, &in[i][0], 1, &in_buf_[i], NINS);
    }

    for (size_t i = 0; i < BS; i++) {
        rotate_->setYaw(in[NINS][i]); // read from last inlet
        rotate_->process(&in_buf_[NINS * i], &out_buf_[NOUTS * i]);
    }

    for (size_t i = 0; i < NOUTS; i++) {
        Signal::copy(BS, &out_buf_[i], NOUTS, &out[i][0], 1);
    }
}

void HoaRotate::blockSizeChanged(size_t bs)
{
    const size_t SZ = rotate_->getNumberOfHarmonics() * bs;
    in_buf_.resize(SZ);
    out_buf_.resize(SZ);
}

const char* HoaRotate::annotateInlet(size_t n) const
{
    const size_t N = numInputChannels() - 1;
    if (n < N && n < HOA_MAX_ORDER)
        return xlet_annotations_[n];
    else if (n == N)
        return "signal: rotation angle\n"
               "    in radians in \\[0..2π) range, anti-clockwise";
    else
        return nullptr;
}

const char* HoaRotate::annotateOutlet(size_t n) const
{
    if (n < in_buf_.size() && n < HOA_MAX_ORDER)
        return xlet_annotations_[n];
    else
        return nullptr;
}

void HoaRotate::initAnnotations()
{
    for (size_t i = 0; i < xlet_annotations_.size(); i++)
        snprintf(xlet_annotations_[i], ANNOT_LEN, "signal: harmonic\\[%d\\]", (int)i);
}

void setup_spat_hoa_rotate()
{
    HoaRotate::initAnnotations();
    SoundExternalFactory<HoaRotate> obj("hoa.2d.rotate~");
    obj.addAlias("hoa.rotate~");
}
