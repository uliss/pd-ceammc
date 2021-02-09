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
#include "faust/dsp/llvm-c-dsp.h"

constexpr size_t MAXFAUSTSTRING = 2048;
constexpr int OPT_LEVEL = -1;
constexpr const char* CURRENT_MACH_TARGET = "";

namespace ceammc {
namespace faust {

    LlvmDspFactory::LlvmDspFactory(const char* fname)
        : factory_(nullptr)
    {
        char errors[MAXFAUSTSTRING];
        int noptions = 0; //(int)faust_opt_manager_get_noptions(x->f_opt_manager);
        char const** options = nullptr; //faust_opt_manager_get_options(x->f_opt_manager);

        factory_ = createCDSPFactoryFromFile(fname, noptions, options, CURRENT_MACH_TARGET, errors, OPT_LEVEL);
        if (strnlen(errors, MAXFAUSTSTRING)) {
            factory_ = nullptr;
            return;
        }
    }

    LlvmDspFactory::LlvmDspFactory(LlvmDspFactory&& f)
        : factory_(f.factory_)
    {
        f.factory_ = nullptr;
    }

    LlvmDspFactory::~LlvmDspFactory()
    {
        if (factory_) {
            const auto f = static_cast<llvm_dsp_factory*>(factory_);
            deleteCDSPFactory(f);
        }
    }

    //    LlvmDspInstance LlvmDspFactory::createInstance() const
    //    {
    ////        if
    //    }

    LlvmDspInstance::LlvmDspInstance(const std::shared_ptr<LlvmDspFactory>& f)
        : instance_(nullptr)
        , factory_(f)
        , num_inputs_(0)
        , num_outputs_(0)
    {
        if (f->isOk()) {
            auto fptr = static_cast<llvm_dsp_factory*>(factory_->get());
            instance_ = createCDSPInstance(fptr);
            if (!instance_) {
                factory_.reset();
                return;
            } else {
                auto inst = static_cast<llvm_dsp*>(instance_);

                num_inputs_ = getNumInputsCDSPInstance(inst);
                num_outputs_ = getNumOutputsCDSPInstance(inst);
            }
        }
    }

    LlvmDspInstance::~LlvmDspInstance()
    {
        if (instance_) {
            auto inst = static_cast<llvm_dsp*>(instance_);
            deleteCDSPInstance(inst);
            instance_ = nullptr;
        }
    }

}
}
