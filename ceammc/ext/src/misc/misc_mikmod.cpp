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
#include "ceammc_factory.h"

#include <fstream>
#include <limits>

ModPlug::ModPlug(const PdArgs& a)
    : SoundExternal(a)
    , path_(gensym(""))
    , file_(0)
    , play_(false)
{
    createSignalOutlet();
    createSignalOutlet();
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
    //    md_mixfreq = sample_rate_;

    if (!play_)
        return;

    char buf[BS * 2];
    if (ModPlug_Read(file_, &buf, BS * 2) <= 0) {
        play_ = false;
        return;
    }

    for (size_t i = 0; i < BS; i++) {
        out[0][i] = (buf[i * 2] - 127.f) / 255.f;
        out[1][i] = (buf[i * 2 + 1] - 127.f) / 255.f;
    }
}

void ModPlug::m_play(t_symbol*, const AtomList&)
{
    if (!file_) {
        OBJ_ERR << "file not loaded";
        return;
    }

    play_ = true;
    OBJ_DBG << "play";
}

void ModPlug::m_stop(t_symbol*, const AtomList&)
{
    if (!play_) {
        OBJ_ERR << "already stopped";
        return;
    }

    play_ = false;
    OBJ_DBG << "stop";
}

void ModPlug::load()
{
    OBJ_DBG << "load";

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

extern "C" void setup_misc0x2emikmod_tilde()
{
    ModPlug_Settings s;
    s.mFlags = MODPLUG_ENABLE_OVERSAMPLING
        | MODPLUG_ENABLE_NOISE_REDUCTION;

    s.mChannels = 2;
    s.mBits = 8;
    s.mFrequency = 44100;
    s.mResamplingMode = MODPLUG_RESAMPLE_LINEAR;
    s.mStereoSeparation = 1;
    s.mMaxMixChannels = 128;
    s.mLoopCount = 0;

    ModPlug_SetSettings(&s);

    SoundExternalFactory<ModPlug> obj("modplug~");
    obj.addMethod("play", &ModPlug::m_play);
    obj.addMethod("stop", &ModPlug::m_stop);
}
