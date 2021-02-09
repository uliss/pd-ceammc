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

#ifndef FAUSTFLOAT
#define FAUSTFLOAT t_float
#endif

#include "faust/gui/UI.h"

struct faust_ui : public UI {
};

using FaustUI = faust::PdUI<faust_ui>;

LangFaustTilde::LangFaustTilde(const PdArgs& args)
    : SoundExternal(args)
    , fname_(nullptr)
{
    fname_ = new SymbolProperty("@fname", &s_);
    fname_->setArgIndex(0);
    addProperty(fname_);
}

LangFaustTilde::~LangFaustTilde() // for std::unique_ptr
{
}

void LangFaustTilde::initDone()
{
    const std::string fname = findInStdPaths(fname_->value()->s_name);
    if (fname.empty()) {
        OBJ_ERR << "Faust file is not found: " << fname_->value();
        return;
    }

    dsp_factory_.reset(new faust::LlvmDspFactory(fname.c_str()));
    if (!dsp_factory_ || !dsp_factory_->isOk()) {
        OBJ_ERR << "Faust file load error " << fname_->value();
        if (dsp_factory_ && !dsp_factory_->errors().empty())
            OBJ_ERR << dsp_factory_->errors();
        return;
    }

    dsp_ = dsp_factory_->createDsp();
    if (!dsp_ || !dsp_->isOk()) {
        OBJ_ERR << "can't create DSP instance";
        dsp_.release();
        dsp_factory_.reset();
        return;
    }

    dsp_->init(sys_getsr());
    const auto nin = dsp_->numInputs();
    const auto nout = dsp_->numOutputs();

    for (size_t i = 0; i < nin; i++)
        createSignalInlet();

    for (size_t i = 0; i < nout; i++)
        createSignalOutlet();

    ui_.reset(new FaustUI("test", ""));
    if (dsp_->buildUI(ui_.get())) {
        auto ui = static_cast<FaustUI*>(ui_.get());
        const size_t n_ui = ui->uiCount();
        OBJ_DBG << "N props: " << n_ui;
        for (size_t i = 0; i < n_ui; i++) {
            addProperty(new faust::UIProperty(ui->uiAt(i)));
        }
    } else
        OBJ_ERR << "can't build UI";
}

void LangFaustTilde::setupDSP(t_signal** in)
{
    SoundExternal::setupDSP(in);
    if (dsp_ && ui_) {
        auto ui = static_cast<FaustUI*>(ui_.get());
        std::vector<FAUSTFLOAT> z = ui->uiValues();
        dsp_->init(samplerate());
        ui->setUIValues(z);
    }
}

void LangFaustTilde::processBlock(const t_sample** in, t_sample** out)
{
    if (!dsp_ || !dsp_->isOk())
        return;

    dsp_->compute(in, out, blockSize());
}

void LangFaustTilde::m_reset(t_symbol*, const AtomListView&)
{
    if (dsp_factory_ && dsp_)
        dsp_.get()->clear();
}

void setup_lang_faust_tilde()
{
    SoundExternalFactory<LangFaustTilde> obj("lang.faust~", OBJECT_FACTORY_DEFAULT);
    obj.addMethod("reset", &LangFaustTilde::m_reset);
}
