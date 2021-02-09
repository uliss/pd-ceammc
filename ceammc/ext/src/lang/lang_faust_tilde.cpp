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

#ifndef FAUSTFLOAT
#define FAUSTFLOAT t_float
#endif

#include "faust/gui/UI.h"

#ifdef _WIN32
#include <windows.h>
#endif

#include <chrono>
#include <cstdlib>

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

static faust::FaustConfig faust_config_base;

struct faust_ui : public UI {
};

using FaustUI = faust::PdUI<faust_ui>;

LangFaustTilde::LangFaustTilde(const PdArgs& args)
    : SoundExternal(args)
    , fname_(nullptr)
    , include_dirs_(nullptr)
{
    fname_ = new SymbolProperty("@fname", &s_);
    fname_->setArgIndex(0);
    addProperty(fname_);

    include_dirs_ = new ListProperty("@include");
    addProperty(include_dirs_);
}

LangFaustTilde::~LangFaustTilde() // for std::unique_ptr
{
}

void LangFaustTilde::initDone()
{
    // time measure
    const auto clock_begin = std::chrono::steady_clock::now();

    // dps suspend/resume
    DspState dsp_state_guard;

    full_path_ = findInStdPaths(fname_->value()->s_name);
    if (full_path_.empty()) {
        OBJ_DBG << "Faust file is not found: " << fname_->value();
        return;
    }

    faust::FaustConfig cfg = faust_config_base;
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

    dsp_factory_.reset(new faust::LlvmDspFactory(full_path_.c_str(), cfg));
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

    OBJ_DBG << "compiled from source: " << full_path_;

    dsp_->init(sys_getsr());
    const auto nin = dsp_->numInputs();
    const auto nout = dsp_->numOutputs();

    for (size_t i = 0; i < nin; i++)
        createSignalInlet();

    for (size_t i = 0; i < nout; i++)
        createSignalOutlet();

    ui_.reset(new FaustUI(dsp_factory_->name(), ""));
    if (dsp_->buildUI(ui_.get())) {
        auto ui = static_cast<FaustUI*>(ui_.get());
        const size_t n_ui = ui->uiCount();
        for (size_t i = 0; i < n_ui; i++) {
            addProperty(new faust::UIProperty(ui->uiAt(i)));
        }
    } else
        OBJ_ERR << "can't build UI";

    const auto clock_end = std::chrono::steady_clock::now();
    OBJ_DBG << "compilation time: " << std::chrono::duration_cast<std::chrono::milliseconds>(clock_end - clock_begin).count() << "ms";
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

void LangFaustTilde::m_open(t_symbol*, const AtomListView&)
{
    if (full_path_.empty() || !platform::path_exists(full_path_.c_str())) {
        OBJ_ERR << "file not exists: " << full_path_;
        return;
    }

    char msg[MAXPDSTRING];

#ifdef _WIN32
    char temp[MAXPDSTRING];
    sys_bashfilename(full_path_.c_str(), temp);
    sprintf(msg, "\"%s\"", temp);
    WinExec(msg, SW_HIDE);
    return;
#elif __APPLE__
    snprintf(msg, sizeof(msg) - 1, "open -t %s", full_path_.c_str());
    system(msg);
    return;
#else
    snprintf(msg, sizeof(msg) - 1, "xdg-open %s", full_path_.c_str());
    system(msg);
    return;
#endif
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

std::string LangFaustTilde::canvasDir() const
{
    auto cnv = canvas();
    return cnv ? canvas_getdir(cnv)->s_name : std::string();
}

void setup_lang_faust_tilde()
{
    extern t_class* ceammc_class;
    std::string path = class_gethelpdir(ceammc_class);
    path += "/faust";
    faust_config_base.addIncludeDirectory(path);

    SoundExternalFactory<LangFaustTilde> obj("lang.faust~", OBJECT_FACTORY_DEFAULT);
    obj.addMethod("reset", &LangFaustTilde::m_reset);
    obj.addMethod("open", &LangFaustTilde::m_open);
}
