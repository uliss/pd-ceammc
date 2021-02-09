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
#ifndef LANG_FAUST_TILDE_H
#define LANG_FAUST_TILDE_H

#include "ceammc_sound_external.h"
using namespace ceammc;

#ifdef WITH_FAUST
#include "ceammc_llvm.h"
#include <memory>

using FactoryPtr = std::unique_ptr<faust::LlvmDspFactory>;
#endif

class LangFaustTilde : public SoundExternal {
#ifdef WITH_FAUST
    FactoryPtr dsp_factory_;
    std::unique_ptr<faust::LlvmDsp> dsp_;
#endif

    SymbolProperty* fname_;

public:
    LangFaustTilde(const PdArgs& args);
    void initDone() override;

    void processBlock(const t_sample** in, t_sample** out) final;
};

void setup_lang_faust_tilde();

#endif // LANG_FAUST_TILDE_H
