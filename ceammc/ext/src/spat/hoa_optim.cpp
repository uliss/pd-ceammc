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
#include "hoa_optim.h"
#include "ceammc_crc32.h"
#include "ceammc_factory.h"

#include <type_traits>

CEAMMC_DEFINE_SYM_HASH(basic)
CEAMMC_DEFINE_SYM_HASH(maxre)
CEAMMC_DEFINE_SYM_HASH(inphase)

HoaOptim::HoaOptim(const PdArgs& args)
    : HoaBase(args)
    , mode_(nullptr)
{
    mode_ = new SymbolEnumProperty("@mode", { sym_basic(), sym_maxre(), sym_inphase() });
    mode_->setArgIndex(1);
    mode_->setSuccessFn([this](Property*) { adjustMode(); });
    addProperty(mode_);

    addProperty(new SymbolEnumAlias("@basic", mode_, sym_basic()));
    addProperty(new SymbolEnumAlias("@maxre", mode_, sym_maxre()));
    addProperty(new SymbolEnumAlias("@inphase", mode_, sym_inphase()));
}

void HoaOptim::initDone()
{
    optim_.reset(new Optim2d(order()));
    adjustMode();

    const size_t N = optim_->getNumberOfHarmonics();
    in_buf_.resize(N * HOA_DEFAULT_BLOCK_SIZE);
    out_buf_.resize(N * HOA_DEFAULT_BLOCK_SIZE);

    createSignalInlets(N);
    createSignalOutlets(N);
}

void HoaOptim::processBlock(const t_sample** in, t_sample** out)
{
    const size_t NOUTS = numOutputChannels();
    const size_t NINS = numInputChannels();
    const size_t BS = blockSize();

    for (size_t i = 0; i < NINS; i++)
        Signal::copy(BS, &in[i][0], 1, &in_buf_[i], NINS);

    for (size_t i = 0; i < BS; i++)
        optim_->process(&in_buf_[NINS * i], &out_buf_[NOUTS * i]);

    for (size_t i = 0; i < NOUTS; i++)
        Signal::copy(BS, &out_buf_[i], NOUTS, &out[i][0], 1);
}

void HoaOptim::adjustMode()
{
    if (!optim_)
        return;

    switch (crc32_hash(mode_->value())) {
    case hash_basic:
        optim_->setMode(Optim2d::Basic);
        break;
    case hash_maxre:
        optim_->setMode(Optim2d::MaxRe);
        break;
    case hash_inphase:
        optim_->setMode(Optim2d::InPhase);
        break;
    default:
        OBJ_ERR << "unknown mode: " << mode_->value()->s_name;
        break;
    }
}

void setup_spat_hoa_optim()
{
    SoundExternalFactory<HoaOptim> obj("hoa.2d.optim~");
    obj.addAlias("hoa.optim~");
}
