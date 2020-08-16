/*****************************************************************************
 * Copyright 2017 Serge Poltavsky. All rights reserved.
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
#include "misc_modplug.h"
#include "../base/function.h"
#include "ceammc_factory.h"

#include <cmath>
#include <fstream>
#include <limits>

ModPlug::ModPlug(const PdArgs& a)
    : SoundExternal(a)
    , path_(&s_)
    , file_(0)
    , play_(false)
    , play_prop_(0)
    , pos_(0)
    , func_on_end_(&s_)
    , log_error_(false)
{
    createSignalOutlet();
    createSignalOutlet();

    play_prop_ = new PointerProperty<bool>("@play", &play_, PropValueAccess::READWRITE);
    addProperty(play_prop_);

    createCbFloatProperty(
        "@pos",
        [this]() -> t_float { return p_pos(); },
        [this](t_float f) -> bool { return p_set_pos(f); })
        ->setUnitsMs();

    createCbFloatProperty(
        "@rpos",
        [this]() -> t_float { return p_rel_pos(); },
        [this](t_float f) -> bool { return p_set_rel_pos(f); })
        ->setFloatCheck(PropValueConstraints::CLOSED_RANGE, 0, 1);

    createCbFloatProperty(
        "@len",
        [this]() -> t_float { return p_len(); })
        ->setUnitsMs();

    createCbSymbolProperty(
        "@name",
        [this]() -> t_symbol* { return p_name(); });

    createCbSymbolProperty(
        "@done",
        [this]() -> t_symbol* { return func_on_end_; },
        [this](t_symbol* s) -> bool { return p_set_on_end(s); });
}

ModPlug::~ModPlug()
{
    unload();
}

void ModPlug::onSymbol(t_symbol* s)
{
    path_ = s;
    load();
}

void ModPlug::processBlock(const t_sample** /*in*/, t_sample** out)
{
    const size_t BS = blockSize();

    if (!play_) {
        for (size_t i = 0; i < BS; i++) {
            out[0][i] = 0.f;
            out[1][i] = 0.f;
        }
        return;
    }

    // stereo buffer
    int32_t buf[BS * 2];
    if (ModPlug_Read(file_, &buf, BS * 2 * sizeof(int32_t)) <= 0) {
        play_ = false;
        ModPlug_Seek(file_, 0);
        pos_ = 0;

        if (func_on_end_ != &s_) {
            Function* f = Function::function(func_on_end_);
            if (!f) {
                OBJ_ERR << "function is not found: " << func_on_end_->s_name;
            } else {
                f->onBang();
            }
        }

        return;
    }

    // pos in milliseconds
    pos_ += (float(BS) / sys_getsr()) * 1000.f;

    for (size_t i = 0; i < BS; i++) {
        double norm = std::numeric_limits<int32_t>::max();
        out[0][i] = buf[i * 2] / norm;
        out[1][i] = buf[i * 2 + 1] / norm;
    }
}

void ModPlug::m_play(t_symbol*, const AtomListView&)
{
    if (!file_) {
        OBJ_ERR << "file is not loaded";
        return;
    }

    play_ = true;
    OBJ_DBG << "play";
}

void ModPlug::m_stop(t_symbol*, const AtomListView&)
{
    if (!file_) {
        OBJ_ERR << "file is not loaded";
        return;
    }

    if (!play_) {
        OBJ_ERR << "already stopped";
        return;
    }

    ModPlug_Seek(file_, 0);
    play_ = false;
    pos_ = 0;
    OBJ_DBG << "stop";
}

void ModPlug::m_pause(t_symbol*, const AtomListView&)
{
    play_ = false;
    OBJ_DBG << "pause";
}

t_float ModPlug::p_rel_pos() const
{
    if (!isOpened())
        return 0;

    t_float len = ModPlug_GetLength(file_);
    if (len > 0)
        return pos_ / len;
    else
        return 0;
}

bool ModPlug::p_set_rel_pos(t_float pos)
{
    if (!isOpened())
        return false;

    float len = ModPlug_GetLength(file_);
    int off = static_cast<int>(std::round(pos * len));
    ModPlug_Seek(file_, off);
    pos_ = off;

    return true;
}

bool ModPlug::p_set_on_end(t_symbol* s)
{
    func_on_end_ = s;
    if (Function::function(func_on_end_) == 0) {
        OBJ_ERR << "function not exists: " << func_on_end_;
        return false;
    }

    return true;
}

void ModPlug::initDone()
{
    SoundExternal::initDone();
    log_error_ = true;
}

bool ModPlug::p_set_pos(t_float pos)
{
    if (!isOpened())
        return false;

    int off = static_cast<int>(pos);
    if (off < 0) {
        OBJ_ERR << "position should be positive" << off;
        return false;
    }

    ModPlug_Seek(file_, off);
    pos_ = off;
    return pos;
}

t_float ModPlug::p_pos() const
{
    if (!isOpened())
        return -1;

    return pos_;
}

t_symbol* ModPlug::p_name() const
{
    if (!isOpened())
        return &s_;

    return gensym(ModPlug_GetName(file_));
}

t_float ModPlug::p_len() const
{
    if (!isOpened())
        return 0;

    return ModPlug_GetLength(file_);
}

void ModPlug::load()
{
    if (!file_)
        unload();

    // search in standard locations
    std::string path = findInStdPaths(path_->s_name);
    if (path.empty()) {
        OBJ_ERR << "can't find file: " << path_->s_name;
        return;
    }

    std::ifstream file(path.c_str(), std::ios::binary | std::ios::ate);
    if (!file) {
        OBJ_ERR << "can't read file: " << path;
        return;
    }

    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    std::vector<char> buffer(size);
    if (!file.read(buffer.data(), size)) {
        OBJ_ERR << "can't read file: " << path;
        return;
    }

    file_ = ModPlug_Load(buffer.data(), int(buffer.size()));
    if (!file_) {
        OBJ_ERR << "can't load file: " << path;
        return;
    }

    OBJ_DBG << "Mod loaded:   " << path;
    OBJ_DBG << "  - name:        " << ModPlug_GetName(file_);
    OBJ_DBG << "  - length (ms): " << ModPlug_GetLength(file_);
}

void ModPlug::unload()
{
    if (!file_)
        return;

    ModPlug_Unload(file_);
    file_ = 0;
}

bool ModPlug::isOpened() const
{
    if (!file_) {
        if (log_error_) {
            OBJ_ERR << "file is not loaded";
        }

        return false;
    }

    return true;
}

void setup_misc_modplug_tilde()
{
    ModPlug_Settings s;
    s.mFlags = MODPLUG_ENABLE_OVERSAMPLING
        | MODPLUG_ENABLE_NOISE_REDUCTION;

    s.mChannels = 2;
    s.mBits = 32;
    s.mFrequency = 44100;
    s.mResamplingMode = MODPLUG_RESAMPLE_LINEAR;
    s.mStereoSeparation = 1;
    s.mMaxMixChannels = 128;
    s.mLoopCount = 0;

    ModPlug_SetSettings(&s);

    SoundExternalFactory<ModPlug> obj("modplug~", OBJECT_FACTORY_DEFAULT);
    obj.useDefaultPdFloatFn();
    obj.addMethod("play", &ModPlug::m_play);
    obj.addMethod("stop", &ModPlug::m_stop);
    obj.addMethod("pause", &ModPlug::m_pause);
}
