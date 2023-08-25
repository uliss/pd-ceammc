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
#include "ceammc_format.h"
#include "ceammc_platform.h"
#include "fmt/core.h"

#ifndef FAUSTFLOAT
#define FAUSTFLOAT t_float
#endif

#include "faust/gui/UI.h"

#include <cerrno>
#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <cstring>

extern "C" {
int ceammc_init_done();
void ceammc_setup();
}

class DspState {
    int state_;

public:
    DspState()
        : state_(canvas_suspend_dsp())
    {
    }

    ~DspState()
    {
        canvas_resume_dsp(state_);
    }
};

class ViewState {
    BaseObject* obj_;
    bool visible_;

public:
    ViewState(BaseObject* obj)
        : obj_(obj)
    {
        visible_ = obj_->isVisible();
        if (visible_)
            obj_->show(false);
    }

    ~ViewState()
    {
        if (visible_) {
            obj_->show(true);
            obj_->fixLines();
        }
    }
};

using FaustUI = faust::PdUI;

faust::FaustConfig& LangFaustTilde::faust_config_base()
{
    static faust::FaustConfig config_;
    return config_;
}

LangFaustTilde::LangFaustTilde(const PdArgs& args)
    : LangFaustBase(args)
{
    include_dirs_ = new ListProperty("@include");
    addProperty(include_dirs_);

    nostd_ = new FlagProperty("@nostd");
    addProperty(nostd_);
}

LangFaustTilde::~LangFaustTilde() = default; // for std::unique_ptr

void LangFaustTilde::initDone()
{
    SoundExternal::initDone();
    compile(); // virtual call
}

void LangFaustTilde::createFaustUI()
{
    ui_.reset(new FaustUI(dsp_factory_->name().c_str()));
    if (dsp_->buildUI(ui_.get())) {
        auto ui = static_cast<FaustUI*>(ui_.get());
        const size_t n_ui = ui->uiCount();
        for (size_t i = 0; i < n_ui; i++) {
            auto name = ui->uiAt(i)->propInfo().name();
            if (hasProperty(name)) {
                OBJ_ERR << fmt::format("UI control already exists: '{}', skipping", name->s_name);
                continue;
            }

            auto p = new faust::UIProperty(ui->uiAt(i));
            addProperty(p);
            faust_properties_.push_back(p);
        }
    } else
        OBJ_ERR << "can't build UI";
}

bool LangFaustTilde::initFaustDsp()
{
    dsp_ = dsp_factory_->createDsp();
    if (!dsp_ || !dsp_->isOk()) {
        OBJ_ERR << "can't create DSP instance";
        dsp_.reset();
        dsp_factory_.reset();
        return false;
    }

    dsp_->init(sys_getsr());
    return true;
}

bool LangFaustTilde::initFaustDspFactory(const faust::FaustConfig& cfg)
{
    std::string code;

    for (auto& l : src_) {
        code.append(to_string(l.view()));
        code += '\n';
    }

    if (!unescapeString(code))
        return false;

    {
        OBJ_DBG << code;
    }

    dsp_factory_.reset(new faust::LlvmDspFactory(code.c_str(), cfg));
    if (!dsp_factory_ || !dsp_factory_->isOk()) {
        if (dsp_factory_ && !dsp_factory_->errors().empty())
            OBJ_ERR << dsp_factory_->errors();
        return false;
    }

    return true;
}

void LangFaustTilde::initInputs()
{
    const int n_old = numInputChannels();
    const int n_new = dsp_->numInputs();

    const int n_add = n_new - n_old;

    if (n_add > 0) {
        for (int i = 0; i < n_add; i++)
            createSignalInlet();
    } else if (n_add < 0) {
        for (int i = 0; i < (-n_add); i++)
            popInlet();
    }
}

void LangFaustTilde::initOutputs()
{
    const int n_old = numOutputChannels();
    const int n_new = dsp_->numOutputs();

    const int n_add = n_new - n_old;

    if (n_add > 0) {
        for (int i = 0; i < n_add; i++)
            createSignalOutlet();
    } else if (n_add < 0) {
        for (int i = 0; i < (-n_add); i++)
            popOutlet();
    }
}

faust::FaustConfig LangFaustTilde::makeFaustConfig()
{
    auto cfg = faust_config_base();
    const auto canvas_dir = canvasDir();
    cfg.addIncludeDirectory(canvas_dir);

    // fill includes
    for (auto& a : include_dirs_->value()) {
        auto path = to_string(a);
        // ~/path -> /home/user/path
        auto dir = platform::expand_tilde_path(path);

        // ./relpath -> /patchdir/relpath
        if (platform::is_path_relative(dir.c_str()))
            dir = canvas_dir + '/' + dir;

        if (platform::path_exists(dir.c_str())) {
            OBJ_DBG << "adding include path: " << dir;
        } else {
            OBJ_DBG << "include path not found: " << path << ", skipping";
            continue;
        }

        cfg.addIncludeDirectory(dir);
    }

    return cfg;
}

void LangFaustTilde::setupDSP(t_signal** in)
{
    SoundExternal::setupDSP(in);

    if (dsp_ && ui_) {
        auto ui = static_cast<FaustUI*>(ui_.get());
        auto z = ui->uiValues();
        dsp_->init(samplerate());
        ui->setUIValues(z);
    }
}

void LangFaustTilde::processBlock(const t_sample** in, t_sample** out)
{
    if (!dsp_)
        return;

    dsp_->compute(in, out, blockSize());
}

void LangFaustTilde::m_reset(t_symbol*, const AtomListView&)
{
    if (dsp_factory_ && dsp_)
        dsp_.get()->clear();
}

void LangFaustTilde::m_open(t_symbol*, const AtomListView&)
{
    openEditor(0, 0);
}

void LangFaustTilde::saveUser(t_binbuf* b)
{
    auto symA = gensym(sym_A);
    auto symR = gensym(sym_restore);

    for (auto& l : src_) {
        if (l.empty())
            continue;

        binbuf_addv(b, "ss", symA, symR);
        binbuf_add(b, l.size(), &l.front().atom());
        binbuf_addsemi(b);
    }

    binbuf_addv(b, "ss", symA, symR);
    binbuf_addsemi(b);
}

void LangFaustTilde::onRestore(const AtomListView& lv)
{
    if (lv.empty()) {
        compile();
    } else {
        src_.push_back({});
        auto& back = src_.back();

        for (auto& a : lv)
            back.push_back(a);
    }
}

void LangFaustTilde::editorClear()
{
    src_.clear();
    //    if (!nostd_->value())
    //        src_.push_back({ Atom(gensym("import(\"stdfaust.lib\");")) });
}

void LangFaustTilde::editorAddLine(t_symbol* sel, const AtomListView& lv)
{
    src_.push_back({});
    auto& b = src_.back();
    b.reserve(lv.size());

    for (auto& a : lv)
        b.push_back(a);
}

EditorLineList LangFaustTilde::getContentForEditor() const
{
    EditorLineList res;
    res.reserve(src_.size());

    for (auto& l : src_) {
        if (l.empty())
            continue;

        auto str = EditorStringPool::pool().allocate();
        str->append(l.view());
        res.push_back(str);
    }

    return res;
}

void LangFaustTilde::editorSync()
{
    compile();
}

void LangFaustTilde::dump() const
{
    SoundExternal::dump();

    Post os(this);
    os << "include directories: \n";
    if (dsp_factory_)
        dsp_factory_->dumpIncludeDirs(os, " - ");

    os << "used libraries: \n";
    if (dsp_factory_)
        dsp_factory_->dumpLibs(os, " - ");

    os << "compile options: ";
    if (dsp_factory_)
        dsp_factory_->dumpOpts(os);
}

void LangFaustTilde::addIncludePath(const std::string& path)
{
    faust_config_base().addIncludeDirectory(path);
}

LangFaustTilde::FaustProperyList& LangFaustTilde::faustProperties()
{
    return faust_properties_;
}

void LangFaustTilde::compile()
{
    ViewState view_guard(this);

    auto& props = properties();
    for (auto& p : props) {
        if (dynamic_cast<faust::UIProperty*>(p)) {
            delete p;
            p = nullptr;
        }
    }
    auto it = std::remove_if(props.begin(), props.end(), [](Property* p) { return p == nullptr; });
    props.erase(it, props.end());
    faust_properties_.clear();

    dsp_.reset();
    dsp_factory_.reset();
    ui_.reset();

    // time measure
    const auto clock_begin = std::chrono::steady_clock::now();

    // dps suspend/resume
    DspState dsp_state_guard;

    if (!initFaustDspFactory(makeFaustConfig()))
        return;

    if (!initFaustDsp())
        return;

    initInputs();
    initOutputs();

    createFaustUI();
    createCustomUI();

    const auto clock_end = std::chrono::steady_clock::now();
    OBJ_DBG << "compilation time: " << std::chrono::duration_cast<std::chrono::milliseconds>(clock_end - clock_begin).count() << "ms";
}

void LangFaustTilde::createCustomUI()
{
}

std::string LangFaustTilde::canvasDir() const
{
    auto cnv = canvas();
    return cnv ? canvas_getdir(cnv)->s_name : std::string();
}

void setup_lang_faust_non_external()
{
    SoundExternalFactory<LangFaustTilde> obj("lang.faust~", OBJECT_FACTORY_DEFAULT);
    obj.addMethod("reset", &LangFaustTilde::m_reset);
    obj.addMethod("open", &LangFaustTilde::m_open);

    std::string path = class_gethelpdir(obj.classPointer());
    path += "/faust";
    LangFaustTilde::addIncludePath(path);

    LangFaustTilde::factoryEditorObjectInit(obj);
    LangFaustTilde::factorySaveObjectInit(obj);
}

void setup_lang0x2efaust_tilde()
{
    if (!ceammc_init_done())
        ceammc_setup();

    setup_lang_faust_non_external();
}
