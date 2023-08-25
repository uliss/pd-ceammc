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

#include <cstring>
#include <iostream>
#include <type_traits>

static const std::string CURRENT_MACH_TARGET;

namespace ceammc {
namespace faust {

    static void delete_factory(llvm_dsp_factory* f)
    {
        deleteDSPFactory(f);
    }

    LlvmDspFactory::LlvmDspFactory(const char* code, const FaustConfig& config)
        : factory_(nullptr, delete_factory)
    {
        auto f = createDSPFactoryFromString("pd_faust", code,
            config.numOptions(),
            config.options(),
            CURRENT_MACH_TARGET,
            errors_,
            config.optLevel());

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

    void LlvmDspFactory::dumpLibs(std::ostream& os, const std::string& prefix) const
    {
        if (factory_) {
            for (auto& s : factory_->getLibraryList())
                os << prefix << s << "\n";
        }
    }

    void LlvmDspFactory::dumpOpts(std::ostream& os) const
    {
        if (factory_)
            os << factory_->getCompileOptions();
    }

    std::string LlvmDspFactory::name() const
    {
        return factory_ ? factory_->getName() : std::string();
    }

    void LlvmDspFactory::deleteAll()
    {
        deleteAllDSPFactories();
    }

    void LlvmDspFactory::dumpIncludeDirs(std::ostream& os, const std::string& prefix) const
    {
        if (factory_) {
            for (auto& s : factory_->getIncludePathnames())
                os << prefix << s << "\n";
        }
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
        if (dsp_) {
            dsp_->init(sr);
            num_outs_ = dsp_->getNumOutputs();
        } else
            num_outs_ = 0;
    }

    void LlvmDsp::compute(const t_float** in, t_float** out, size_t bs)
    {
        if (dsp_) {
            const size_t N = num_outs_;

            t_float buf[N * bs];
            t_float* pbuf[N];
            for (size_t i = 0; i < N; i++)
                pbuf[i] = &buf[i * bs];

            dsp_->compute(bs, const_cast<t_float**>(in), pbuf);

            for (size_t i = 0; i < N; i++)
                memcpy(out[i], pbuf[i], sizeof(t_float) * bs);
        }
    }

    FaustConfig::FaustConfig(FaustConfig&& config)
        : opts_(std::move(config.opts_))
        , copts_(std::move(config.copts_))
        , opt_level_(config.opt_level_)
    {
    }

    FaustConfig::FaustConfig()
        : opt_level_(LEVEL_OMAX)
    {
        opts_.reserve(16);
        copts_.reserve(16);

        addOption("-ftz");
        addOption("1");
        addOption("-exp10");
        addOption("-mapp");
        addOption("-vec");
        addOption("-vs");
        addOption("64");

        if (std::is_same<t_float, double>::value)
            addOption("-double");
        else
            addOption("-single");

        syncOptions();
    }

    FaustConfig::FaustConfig(const FaustConfig& config)
        : opts_(config.opts_)
        , opt_level_(config.opt_level_)
    {
        syncOptions();
    }

    FaustConfig& FaustConfig::operator=(const FaustConfig& config)
    {
        opts_ = config.opts_;
        opt_level_ = config.opt_level_;
        syncOptions();
        return *this;
    }

    void FaustConfig::addIncludeDirectory(const std::string& path)
    {
        if (path.empty())
            return;

        addOption("-I");
        addOption(path);
        syncOptions();
    }

    void FaustConfig::addOption(const std::string& opt)
    {
        opts_.push_back(opt);
        syncOptions();
    }

    void FaustConfig::syncOptions()
    {
        copts_.resize(opts_.size());

        for (size_t i = 0; i < opts_.size(); i++)
            copts_[i] = opts_[i].c_str();
    }

}
}
