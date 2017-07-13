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

#include <mikmod.h>

static BOOL stdout_IsThere(void);
static BOOL stdout_Init(void);
static void stdout_Exit(void);
static void stdout_Update(void);
static BOOL stdout_Reset(void);

#define BUFFERSIZE 128

static float* audiobuffer = NULL;

MIKMODAPI MDRIVER drv_puredata = {
    NULL,
    "pd",
    "PureData output driver v1.0",
    0, 255,
    "pd",
    NULL,
    NULL,
    stdout_IsThere,
    VC_SampleLoad,
    VC_SampleUnload,
    VC_SampleSpace,
    VC_SampleLength,
    stdout_Init,
    stdout_Exit,
    stdout_Reset,
    VC_SetNumVoices,
    VC_PlayStart,
    VC_PlayStop,
    stdout_Update,
    NULL,
    VC_VoiceSetVolume,
    VC_VoiceGetVolume,
    VC_VoiceSetFrequency,
    VC_VoiceGetFrequency,
    VC_VoiceSetPanning,
    VC_VoiceGetPanning,
    VC_VoicePlay,
    VC_VoiceStop,
    VC_VoiceStopped,
    VC_VoiceGetPosition,
    VC_VoiceRealVolume
};

MikMod::MikMod(const PdArgs& a)
    : SoundExternal(a)
    , path_(gensym(""))
    , module_(0)
{
    createSignalOutlet();
    createSignalOutlet();
}

MikMod::~MikMod()
{
    Player_Stop();

    if (module_)
        Player_Free(module_);
}

void MikMod::onSymbol(t_symbol* s)
{
    path_ = s;

    if (module_ != 0)
        Player_Free(module_);

    /* load module */
    module_ = Player_Load(path_->s_name, 64, 0);

    if (module_ == 0) {
        OBJ_ERR << "Could not load module, reason: " << MikMod_strerror(MikMod_errno);
        return;
    }

    OBJ_DBG << "loaded: " << s->s_name;
}

void MikMod::processBlock(const t_sample** in, t_sample** out)
{
    const size_t BS = blockSize();

    if (!Player_Active()) {
        for (size_t i = 0; i < BS; i++) {
            out[0][i] = 0;
            out[1][i] = 0;
        }
        return;
    }

    MikMod_Update();

    for (size_t i = 0; i < BS; i++) {
        out[0][i] = float(audiobuffer[2 * i]);
        out[1][i] = float(audiobuffer[2 * i + 1]);
    }
}

void MikMod::m_play(t_symbol*, const AtomList&)
{
    if (module_ == 0) {
        OBJ_ERR << "module is not loaded...";
        return;
    }

    Player_Start(module_);
}

void MikMod::m_stop(t_symbol*, const AtomList&)
{
    if (module_ == 0) {
        OBJ_ERR << "module is not loaded...";
        return;
    }

    Player_Stop();
}

extern "C" void setup_misc0x2emikmod_tilde()
{
    LIB_DBG << "Compiled with MikMod Sound Library version "
            << LIBMIKMOD_VERSION_MAJOR << '.'
            << LIBMIKMOD_VERSION_MINOR << '.'
            << LIBMIKMOD_REVISION;

    MikMod_RegisterDriver(&drv_puredata);
    MikMod_RegisterAllLoaders();

    /* initialize the library */
    md_mode |= DMODE_SOFT_MUSIC;
    md_mode |= DMODE_SOFT_SNDFX;
    md_mode |= DMODE_HQMIXER;
    md_mode |= DMODE_FLOAT;
    md_mode |= DMODE_INTERP;
    md_mode |= DMODE_NOISEREDUCTION;
    md_mode |= DMODE_STEREO;

    md_mixfreq = 44100;
    if (MikMod_Init("pd")) {
        LIB_ERR << "Could not initialize sound, reason: " << MikMod_strerror(MikMod_errno);
        return;
    }

    SoundExternalFactory<MikMod> obj("mikmod~");
    obj.addMethod("play", &MikMod::m_play);
    obj.addMethod("stop", &MikMod::m_stop);
}

static BOOL stdout_IsThere(void)
{
    return 1;
}

static BOOL stdout_Init(void)
{
    if (!(audiobuffer = (float*)MikMod_malloc(BUFFERSIZE * sizeof(float))))
        return 1;

    return VC_Init();
}

static void stdout_Exit(void)
{
    VC_Exit();
    if (audiobuffer) {
        MikMod_free(audiobuffer);
        audiobuffer = NULL;
    }
}

static void stdout_Update(void)
{
    VC_WriteBytes((SBYTE*)audiobuffer, BUFFERSIZE * sizeof(float));
}

static BOOL stdout_Reset(void)
{
    VC_Exit();
    return VC_Init();
}
