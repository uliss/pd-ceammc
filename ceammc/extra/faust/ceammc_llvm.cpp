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
#include "ceammc_llvm.h"

#define FAUSTFLOAT t_sample
#include "faust/dsp/llvm-dsp.h"

#include <iostream>

constexpr int OPT_LEVEL = -1;
static const std::string CURRENT_MACH_TARGET;

namespace ceammc {
namespace faust {

    static void delete_factory(llvm_dsp_factory* f)
    {
        deleteDSPFactory(f);
    }

    LlvmDspFactory::LlvmDspFactory(const char* fname)
        : factory_(nullptr, delete_factory)
    {
        int argc = 0; //(int)faust_opt_manager_get_noptions(x->f_opt_manager);
        const char** argv = nullptr; //faust_opt_manager_get_options(x->f_opt_manager);

        auto f = createDSPFactoryFromFile(fname, argc, argv, CURRENT_MACH_TARGET, errors_, OPT_LEVEL);
        factory_.reset(f);
    }

    LlvmDspFactory::LlvmDspFactory(LlvmDspFactory&& f)
        : factory_(std::move(f.factory_))
        , errors_(std::move(f.errors_))
    {
    }

    LlvmDspFactory::~LlvmDspFactory()
    {
        factory_.reset();
    }

    std::unique_ptr<LlvmDsp> LlvmDspFactory::createDsp()
    {
        if (!factory_)
            return {};

        auto dsp = factory_->createDSPInstance();
        if (!dsp)
            return {};

        return std::unique_ptr<LlvmDsp>(new LlvmDsp(dsp));
    }

    LlvmDsp::LlvmDsp(llvm_dsp* dsp)
        : dsp_(dsp)
    {
    }

    LlvmDsp::LlvmDsp(LlvmDsp&& dsp)
        : dsp_(std::move(dsp.dsp_))
    {
    }

    LlvmDsp::LlvmDsp()
    {
    }

    LlvmDsp::~LlvmDsp()
    {
        dsp_.reset();
    }

    size_t LlvmDsp::numInputs() const
    {
        return dsp_ ? dsp_->getNumInputs() : 0;
    }

    size_t LlvmDsp::numOutputs() const
    {
        return dsp_ ? dsp_->getNumOutputs() : 0;
    }

    double LlvmDsp::samplerate() const
    {
        return dsp_ ? dsp_->getSampleRate() : 0;
    }

    bool LlvmDsp::buildUI(UI* ui)
    {
        if (!dsp_ || !ui)
            return false;

        dsp_->buildUserInterface(ui);
        return true;
    }

    void LlvmDsp::clear()
    {
        if (dsp_)
            dsp_->instanceClear();
    }

    void LlvmDsp::init(int sr)
    {
        if (dsp_)
            dsp_->init(sr);
    }

    void LlvmDsp::compute(const t_float** in, t_float** out, size_t bs)
    {
        if (dsp_)
            dsp_->compute(bs, (t_float**)in, out);
    }

}
}
