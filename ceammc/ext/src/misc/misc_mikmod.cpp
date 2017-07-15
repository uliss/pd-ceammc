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
#include "misc_mikmod.h"
#include "../base/function.h"
#include "ceammc_factory.h"

#include <cmath>
#include <fstream>
#include <limits>

static t_symbol* SNULL = gensym("");

ModPlug::ModPlug(const PdArgs& a)
    : SoundExternal(a)
    , path_(gensym(""))
    , file_(0)
    , play_(false)
    , play_prop_(0)
    , pos_(0)
    , func_on_end_(SNULL)
{
    createSignalOutlet();
    createSignalOutlet();

    play_prop_ = new PointerProperty<bool>("@play", &play_);
    createProperty(play_prop_);
    createCbProperty("@pos", &ModPlug::p_pos, &ModPlug::p_set_pos);
    createCbProperty("@rpos", &ModPlug::p_rel_pos, &ModPlug::p_set_rel_pos);
    createCbProperty("@len", &ModPlug::p_len);
    createCbProperty("@name", &ModPlug::p_name);
    createCbProperty("@on_end", &ModPlug::p_on_end, &ModPlug::p_set_on_end);
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

        if (func_on_end_ != SNULL) {
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

void ModPlug::m_play(t_symbol*, const AtomList&)
{
    if (!file_) {
        OBJ_ERR << "file is not loaded";
        return;
    }

    play_ = true;
    OBJ_DBG << "play";
}

void ModPlug::m_stop(t_symbol*, const AtomList&)
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

void ModPlug::m_pause(t_symbol*, const AtomList&)
{
    play_ = false;
    OBJ_DBG << "pause";
}

AtomList ModPlug::p_rel_pos() const
{
    if (!isOpened())
        return AtomList();

    float len = ModPlug_GetLength(file_);
    return Atom(pos_ / len);
}

void ModPlug::p_set_rel_pos(const AtomList& pos)
{
    if (!isOpened())
        return;

    if (!checkArgs(pos, ARG_FLOAT)) {
        OBJ_ERR << "position in range [0-1] expexted: " << pos;
        return;
    }

    float p = pos[0].asFloat();
    if (0 <= p && p <= 1) {
        float len = ModPlug_GetLength(file_);
        int off = static_cast<int>(roundf(p * len));
        ModPlug_Seek(file_, off);
        pos_ = off;
    } else {
        OBJ_ERR << "position in range [0-1] expexted: " << p;
    }
}

AtomList ModPlug::p_on_end() const
{
    return Atom(func_on_end_);
}

void ModPlug::p_set_on_end(const AtomList& fn)
{
    if (!checkArgs(fn, ARG_SYMBOL)) {
        OBJ_ERR << "function name expected: " << fn;
        return;
    }

    func_on_end_ = fn[0].asSymbol();
    if (Function::function(func_on_end_) == 0) {
        OBJ_ERR << "function not exists: " << fn;
    }
}

void ModPlug::p_set_pos(const AtomList& pos)
{
    if (!isOpened())
        return;

    if (!checkArgs(pos, ARG_FLOAT)) {
        OBJ_ERR << "time position in ms expected: " << pos;
        return;
    }

    int off = pos.at(0).asInt();
    if (off < 0) {
        OBJ_ERR << "position should be positive" << off;
        return;
    }

    ModPlug_Seek(file_, off);
    pos_ = off;
}

AtomList ModPlug::p_pos() const
{
    if (!isOpened())
        return AtomList(-1);

    return AtomList(pos_);
}

AtomList ModPlug::p_name() const
{
    if (!isOpened())
        return AtomList();

    return AtomList(gensym(ModPlug_GetName(file_)));
}

AtomList ModPlug::p_len() const
{
    if (!isOpened())
        return AtomList(0.f);

    return AtomList(ModPlug_GetLength(file_));
}

void ModPlug::load()
{
    if (!file_)
        unload();

    std::ifstream file(path_->s_name, std::ios::binary | std::ios::ate);
    if (!file) {
        OBJ_ERR << "can't read file: " << path_->s_name;
        return;
    }

    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    std::vector<char> buffer(size);
    if (!file.read(buffer.data(), size)) {
        OBJ_ERR << "can't read file: " << path_->s_name;
        return;
    }

    file_ = ModPlug_Load(buffer.data(), int(buffer.size()));
    if (!file_) {
        OBJ_ERR << "can't load file: " << path_->s_name;
        return;
    }

    OBJ_DBG << "Mod loaded:   " << path_->s_name;
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
        OBJ_ERR << "file is not loaded";
        return false;
    }

    return true;
}

extern "C" void setup_misc0x2emodplug_tilde()
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

    SoundExternalFactory<ModPlug> obj("modplug~", OBJECT_FACTORY_DEFAULT | OBJECT_FACTORY_NO_FLOAT);
    obj.addMethod("play", &ModPlug::m_play);
    obj.addMethod("stop", &ModPlug::m_stop);
    obj.addMethod("pause", &ModPlug::m_pause);
}
