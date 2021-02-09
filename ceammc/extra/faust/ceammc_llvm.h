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

namespace ceammc {
namespace faust {

    class LlvmDspInstance;

    class LlvmDspFactory {
        void* factory_;

        LlvmDspFactory(const LlvmDspFactory&) = delete;
        LlvmDspFactory& operator=(const LlvmDspFactory&) = delete;

    public:
        LlvmDspFactory(const char* fname);
        LlvmDspFactory(LlvmDspFactory&& f);
        ~LlvmDspFactory();

        bool isOk() const { return factory_; }
        void* get() { return factory_; }

//        LlvmDspInstance createInstance() const;
    };

    class LlvmDspInstance {
        void* instance_;
        std::shared_ptr<LlvmDspFactory> factory_;
        int num_inputs_, num_outputs_;

    public:
        LlvmDspInstance(const std::shared_ptr<LlvmDspFactory>& f);
        ~LlvmDspInstance();

        bool isOk() const { return instance_; }
    };

}
}

#endif // CEAMMC_LLVM_H
