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
#include "ceammc_factory.h"

#include <type_traits>

static t_symbol* SYM_BASIC;
static t_symbol* SYM_MAXRE;
static t_symbol* SYM_INPHASE;

HoaOptim::HoaOptim(const PdArgs& args)
    : HoaBase(args)
    , mode_(nullptr)
{
    mode_ = new SymbolEnumProperty("@mode", { SYM_BASIC, SYM_MAXRE, SYM_INPHASE });
    mode_->setArgIndex(1);
    mode_->setSuccessFn([this](Property*) { adjustMode(); });
    addProperty(mode_);

    addProperty(new SymbolEnumAlias("@basic", mode_, SYM_BASIC));
    addProperty(new SymbolEnumAlias("@maxre", mode_, SYM_MAXRE));
    addProperty(new SymbolEnumAlias("@inphase", mode_, SYM_INPHASE));
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

    if (mode_->value() == SYM_BASIC)
        optim_->setMode(Optim2d::Basic);
    else if (mode_->value() == SYM_MAXRE)
        optim_->setMode(Optim2d::MaxRe);
    else if (mode_->value() == SYM_INPHASE)
        optim_->setMode(Optim2d::InPhase);
}

void setup_spat_hoa_optim()
{
    SYM_BASIC = gensym("basic");
    SYM_MAXRE = gensym("maxre");
    SYM_INPHASE = gensym("inphase");

    SoundExternalFactory<HoaOptim> obj("hoa.2d.optim~");
    obj.addAlias("hoa.optim~");
}
