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

#include "config.h"
#ifdef _WIN32
#include <windows.h>
#endif

#ifdef HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif

#ifdef HAVE_SYS_STAT_H
#include <sys/stat.h>
#endif

#include <cerrno>
#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <cstring>

extern "C" {
int ceammc_init_done();
void ceammc_setup();
}

enum RunEditorRc {
    RUN_OK = 0,
    RUN_FILE_NOT_FOUND = 1000,
    RUN_NO_ASSOC,
    RUN_ERR_UNKNOWN
};

static int runEditorCommand(const std::string& path)
{
    char msg[MAXPDSTRING];

#ifdef _WIN32
    char temp[MAXPDSTRING];
    sys_bashfilename(path.c_str(), temp);
    sprintf(msg, "\"%s\"", temp);
    auto rc = reinterpret_cast<uint64_t>(ShellExecute(NULL, "edit", msg, NULL, NULL, SW_SHOWNORMAL));
    if (rc > 32)
        return RUN_OK;

    switch (rc) {
    case ERROR_FILE_NOT_FOUND:
    case ERROR_PATH_NOT_FOUND:
        return RUN_FILE_NOT_FOUND;
    case SE_ERR_NOASSOC:
        return RUN_NO_ASSOC;
    default:
        return RUN_ERR_UNKNOWN;
    }

    return 0;
#elif __APPLE__
    snprintf(msg, sizeof(msg) - 1, "open -t %s", path.c_str());
    return system(msg);

#else
    snprintf(msg, sizeof(msg) - 1, "xdg-open %s", path.c_str());
    return system(msg);
#endif
}

static time_t getModificationTime(const std::string& path)
{
#ifdef _WIN32
#define stat _stat
#endif
    struct stat attrib;
    int err = stat(path.c_str(), &attrib);
    if (!err) {
        return attrib.st_mtime;
    } else {
        fprintf(stderr, "stat error for file '%s': %s", path.c_str(), strerror(errno));
        return 0;
    }
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
    : SoundExternal(args)
    , fname_(nullptr)
    , include_dirs_(nullptr)
    , autocompile_(nullptr)
    , autocompile_clock_([this]() {
        if (!mod_time_.valid()) {
            try {
                mod_time_ = std::async(std::launch::async, getModificationTime, full_path_);
            } catch (std::exception& e) {
                OBJ_ERR << "can't get file modification time: " << e.what();
                return;
            }
        } else {
            auto rc = mod_time_.wait_for(std::chrono::seconds(0));
            if (rc == std::future_status::ready) {
                auto mtime = mod_time_.get();
                if (mtime != last_mod_time_) {
                    last_mod_time_ = mtime;
                    compile();
                }
            }
        }

        if (autocompile_->value())
            autocompile_clock_.delay(250);
    })
    , last_mod_time_(0)
{
    fname_ = new SymbolProperty("@fname", &s_);
    fname_->setSuccessFn([this](Property*) { if(!isPatchLoading()) compile(); });
    fname_->setArgIndex(0);
    addProperty(fname_);

    include_dirs_ = new ListProperty("@include");
    addProperty(include_dirs_);

    autocompile_ = new BoolProperty("@auto", false);
    autocompile_->setSuccessFn([this](Property*) {
        if (autocompile_->value())
            autocompile_clock_.delay(0);
        else
            autocompile_clock_.unset();
    });
    addProperty(autocompile_);
}

LangFaustTilde::~LangFaustTilde() // for std::unique_ptr
{
}

void LangFaustTilde::initDone()
{
    SoundExternal::initDone();
    compile();
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
                OBJ_ERR << "UI control already exists: " << name << ", skipping";
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

    OBJ_DBG << "compiled from source: " << full_path_;

    dsp_->init(sys_getsr());
    return true;
}

bool LangFaustTilde::initFaustDspFactory(const faust::FaustConfig& cfg)
{
    dsp_factory_.reset(new faust::LlvmDspFactory(full_path_.c_str(), cfg));
    if (!dsp_factory_ || !dsp_factory_->isOk()) {
        OBJ_ERR << "Faust file load error " << fname_->value();
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
    if (full_path_.empty() || !platform::path_exists(full_path_.c_str())) {
        OBJ_ERR << "file not exists: " << full_path_;
        return;
    }

    if (run_editor_.valid()) {
        try {
            auto st = run_editor_.wait_for(std::chrono::seconds(0));
            if (st != std::future_status::ready) {
                OBJ_ERR << "exec error: " << __FUNCTION__;
            } else {
                auto rc = run_editor_.get();
                switch (rc) {
                case RUN_FILE_NOT_FOUND:
                    OBJ_DBG << "file not found error";
                    break;
                case RUN_NO_ASSOC:
                    OBJ_DBG << "no editor associated with file";
                    break;
                default:
                    OBJ_DBG << "result code: " << rc;
                    break;
                }
            }
        } catch (std::exception& e) {
            OBJ_ERR << e.what();
            return;
        }
    }

    try {
        run_editor_ = std::async(std::launch::async, runEditorCommand, full_path_);
    } catch (std::exception& e) {
        OBJ_ERR << "can't run editor: " << e.what();
    }

    return;
}

void LangFaustTilde::m_update(t_symbol*, const AtomListView&)
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

void LangFaustTilde::onClick(t_floatarg xpos, t_floatarg ypos, t_floatarg shift, t_floatarg ctrl, t_floatarg alt)
{
    if (!alt)
        return;

    m_open(gensym("open"), {});
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

    for (auto& p : properties()) {
        if (dynamic_cast<faust::UIProperty*>(p)) {
            delete p;
            p = nullptr;
        }
    }

    auto& props = properties();
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

    full_path_ = findInStdPaths(fname_->value()->s_name);
    if (full_path_.empty()) {
        OBJ_DBG << "Faust file is not found: " << fname_->value();
        return;
    }

    last_mod_time_ = getModificationTime(full_path_);
    if (autocompile_->value())
        autocompile_clock_.delay(0);

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

void setup_lang0x2efaust_tilde()
{
    if (!ceammc_init_done())
        ceammc_setup();

    setup_lang_faust_non_external();
}

void setup_lang_faust_non_external()
{
    SoundExternalFactory<LangFaustTilde> obj("lang.faust~", OBJECT_FACTORY_DEFAULT);
    obj.addMethod("reset", &LangFaustTilde::m_reset);
    obj.addMethod("open", &LangFaustTilde::m_open);
    obj.addMethod("update", &LangFaustTilde::m_update);
    obj.useClick();

    std::string path = class_gethelpdir(obj.classPointer());
    path += "/faust";
    LangFaustTilde::addIncludePath(path);
}
