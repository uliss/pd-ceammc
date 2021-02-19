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

#include "ceammc_faust.h"
#include "ceammc_sound_external.h"

using namespace ceammc;

#ifdef WITH_FAUST
#include "ceammc_llvm.h"
#include <memory>

using FactoryPtr = std::unique_ptr<faust::LlvmDspFactory>;
class UI;
using FaustUIPtr = std::unique_ptr<UI>;
using FaustDspPtr = std::unique_ptr<faust::LlvmDsp>;

#endif

class LangFaustTilde : public SoundExternal {
public:
    using FaustProperyList = std::vector<faust::UIProperty*>;

private:
    SymbolProperty* fname_;
    ListProperty* include_dirs_;
    std::string full_path_;
    FaustProperyList faust_properties_;

#ifdef WITH_FAUST
    FactoryPtr dsp_factory_;
    FaustDspPtr dsp_;
    FaustUIPtr ui_;
#endif

public:
    LangFaustTilde(const PdArgs& args);
    ~LangFaustTilde();
    void initDone() override;

    void setupDSP(t_signal** in) override;
    void processBlock(const t_sample** in, t_sample** out) final;

    void m_reset(t_symbol*, const AtomListView&);
    void m_open(t_symbol*, const AtomListView&);

    void dump() const override;

protected:
    FaustProperyList& faustProperties();

private:
    std::string canvasDir() const;
};

void setup_lang_faust_tilde();

#endif // LANG_FAUST_TILDE_H
