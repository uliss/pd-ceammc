/*****************************************************************************
 * Copyright 2021 Serge Poltavsky. All rights reserved.
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
#include "lang_faust_tilde.h"
#include "ceammc_factory.h"

LangFaustTilde::LangFaustTilde(const PdArgs& args)
    : SoundExternal(args)
    , fname_(nullptr)
{
    fname_ = new SymbolProperty("@fname", &s_);
    fname_->setArgIndex(0);
    addProperty(fname_);
}

void LangFaustTilde::initDone()
{
    dsp_factory_.reset(new faust::LlvmDspFactory(fname_->value()->s_name));
    if (!dsp_factory_ || !dsp_factory_->isOk()) {
        OBJ_ERR << "can't create factory " << fname_->value();
        return;
    }

    dsp_ = dsp_factory_->createDsp();
    if (!dsp_ || !dsp_->isOk()) {
        OBJ_ERR << "can't create DSP instance";
        dsp_.release();
        dsp_factory_.reset();
        return;
    }

    const auto nin = dsp_->numInputs();
    const auto nout = dsp_->numOutputs();

    for (size_t i = 1; i < nin; i++)
        createSignalInlet();

    for (size_t i = 0; i < nout; i++)
        createSignalOutlet();
}

void LangFaustTilde::processBlock(const t_sample** in, t_sample** out)
{
}

void setup_lang_faust_tilde()
{
    ObjectFactory<LangFaustTilde> obj("lang.faust~");
}
