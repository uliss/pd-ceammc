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
#ifndef CEAMMC_LLVM_H
#define CEAMMC_LLVM_H

#include "m_pd.h"

#include <memory>
#include <string>

class llvm_dsp;
class llvm_dsp_factory;
class UI;

namespace ceammc {
namespace faust {

    class LlvmDsp;

    class LlvmDspFactory {
        std::unique_ptr<llvm_dsp_factory, void (*)(llvm_dsp_factory*)> factory_;
        std::string errors_;

        LlvmDspFactory(const LlvmDspFactory&) = delete;
        LlvmDspFactory& operator=(const LlvmDspFactory&) = delete;

    public:
        explicit LlvmDspFactory(const char* fname);
        LlvmDspFactory(LlvmDspFactory&& f);
        ~LlvmDspFactory();

        bool isOk() const { return factory_.get(); }

        const std::string& errors() const { return errors_; }

        std::unique_ptr<LlvmDsp> createDsp();
    };

    class LlvmDsp {
        std::unique_ptr<llvm_dsp> dsp_;
        size_t num_outs_ = { 0 };

        friend class LlvmDspFactory;

        LlvmDsp();
        LlvmDsp(llvm_dsp* dsp);

        LlvmDsp(const LlvmDsp&) = delete;
        LlvmDsp& operator=(const LlvmDsp&) = delete;

    public:
        LlvmDsp(LlvmDsp&& dsp);
        ~LlvmDsp();

        bool isOk() const { return dsp_.get(); }

        size_t numInputs() const;
        size_t numOutputs() const;
        double samplerate() const;

        bool buildUI(UI* ui);

        void clear();
        void init(int sr);

        void compute(const t_float** in, t_float** out, size_t bs);
    };

}
}

#endif // CEAMMC_LLVM_H
