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

#include "../data/data_protocol.h"
#include "ceammc_containers.h"
#include "ceammc_editor_object.h"
#include "ceammc_faust.h"
#include "ceammc_save_object.h"
#include "ceammc_sound_external.h"

using namespace ceammc;

#include "ceammc_llvm.h"

class UI;
using FaustUIPtr = std::unique_ptr<UI>;
using FactoryPtr = std::unique_ptr<faust::LlvmDspFactory>;
using FaustDspPtr = std::unique_ptr<faust::LlvmDsp>;
using LangFaustBase = SaveObject<EditorObject<FilesystemIFace<SoundExternal>, EditorSyntax::FAUST, EditorEscapeMode::LUA>>;

class LangFaustTilde : public LangFaustBase {
public:
    using FaustProperyList = std::vector<faust::UIProperty*>;
    using SourceCodeLine = SmallAtomListN<8>;
    using SourceCode = boost::container::small_vector<SourceCodeLine, 48>;

private:
    ListProperty* include_dirs_ { nullptr };
    SymbolProperty* fname_ { nullptr };
    FaustProperyList faust_properties_;

    FactoryPtr dsp_factory_;
    FaustDspPtr dsp_;
    FaustUIPtr ui_;

    SourceCode src_;

public:
    LangFaustTilde(const PdArgs& args);
    ~LangFaustTilde();
    void initDone() override;

    void setupDSP(t_signal** in) override;
    void processBlock(const t_sample** in, t_sample** out) final;

    void m_reset(t_symbol*, const AtomListView&);
    void m_open(t_symbol*, const AtomListView&);

    // SaveObject virtual
    void saveUser(t_binbuf* b) final;
    void onRestore(const AtomListView& lv) final;

    // EditorObject virtual
    void editorClear() final;
    void editorAddLine(t_symbol* sel, const AtomListView& lv) final;
    EditorLineList getContentForEditor() const final;
    EditorTitleString editorTitle() const final { return "Faust Editor"; }
    void editorSync() final;

    // ReaderObject
    bool proto_read(const std::string& path) final;
    // WriterObject
    bool proto_write(const std::string& path) const final;

    void dump() const override;

public:
    static void addIncludePath(const std::string& path);

protected:
    FaustProperyList& faustProperties();
    virtual void compile();
    virtual void createCustomUI();
    std::string sourceCode() const;
    t_symbol* name() const;

private:
    std::string canvasDir() const;
    void createFaustUI();
    bool initFaustDsp();
    bool initFaustDspFactory(const faust::FaustConfig& cfg);
    void initInputs();
    void initOutputs();
    faust::FaustConfig makeFaustConfig();

private:
    static faust::FaustConfig& faust_config_base();
};

#ifdef _WIN32
#define FAUST_LANG_EXPORT extern "C" __declspec(dllexport)
#else
#define FAUST_LANG_EXPORT extern "C"
#endif

FAUST_LANG_EXPORT void setup_lang0x2efaust_tilde();

void setup_lang_faust_non_external();

#endif // LANG_FAUST_TILDE_H
