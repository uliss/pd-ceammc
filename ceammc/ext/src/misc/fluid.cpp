/*****************************************************************************
 * Copyright 2018 Serge Poltavsky. All rights reserved.
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
#include "fluid.h"
#include "ceammc_factory.h"

#include "fluidsynth.h"

Fluid::Fluid(const PdArgs& args)
    : SoundExternal(args)
    , synth_(nullptr)
    , sound_font_(&s_)
{
    createSignalOutlet();
    createSignalOutlet();

    fluid_settings_t* settings = new_fluid_settings();
    if (settings == nullptr) {
        OBJ_ERR << "couldn't create synth settings";
        return;
    }

    // settings:
    fluid_settings_setnum(settings, "synth.midi-channels", 16);
    fluid_settings_setnum(settings, "synth.polyphony", 256);
    fluid_settings_setnum(settings, "synth.gain", 0.600000);
    fluid_settings_setnum(settings, "synth.sample-rate", 44100);
    fluid_settings_setstr(settings, "synth.chorus.active", "no");
    fluid_settings_setstr(settings, "synth.reverb.active", "no");
    fluid_settings_setstr(settings, "synth.ladspa.active", "no");

    // Create fluidsynth instance:
    synth_ = new_fluid_synth(settings);

    if (synth_ == nullptr)
        OBJ_ERR << "couldn't create synth";

    createCbProperty("@sf", &Fluid::propSoundFont, &Fluid::propSetSoundFont);

    // We're done constructing:
    if (synth_)
        OBJ_DBG << "done";
}

Fluid::~Fluid()
{
    if (synth_) {
        fluid_settings_t* settings = fluid_synth_get_settings(synth_);
        delete_fluid_settings(settings);
    }

    delete_fluid_synth(synth_);
}

void Fluid::onList(const AtomList& lst)
{
    m_note(&s_, lst);
}

void Fluid::setupDSP(t_signal** sp)
{
    SoundExternal::setupDSP(sp);

    if (synth_) {
        fluid_settings_t* s = fluid_synth_get_settings(synth_);
        if (fluid_settings_setnum(s, "synth.sample-rate", samplerate()) != FLUID_OK) {
            OBJ_ERR << "can't set samplerate: " << samplerate();
        }
    }
}

AtomList Fluid::propSoundFont() const
{
    return Atom(sound_font_);
}

void Fluid::propSetSoundFont(const AtomList& lst)
{
    if (!synth_) {
        OBJ_ERR << "NULL synth";
        return;
    }

    if (!checkArgs(lst, ARG_SYMBOL)) {
        OBJ_ERR << "path to soundfont expected: " << lst;
        return;
    }

    t_symbol* filename = lst.symbolAt(0, &s_);

    if (fluid_synth_sfload(synth_, filename->s_name, 0) >= 0) {

        OBJ_DBG << "loaded soundfont: " << filename;
        fluid_synth_program_reset(synth_);

        sound_font_ = filename;
    } else {
        OBJ_ERR << "can't load soundfont: " << filename;
    }
}

void Fluid::m_note(t_symbol* s, const AtomList& lst)
{
    if (synth_ == nullptr)
        return;

    if (lst.size() == 3 && lst[0].isFloat() && lst[1].isFloat() && lst[2].isFloat()) {
        int chan = lst[0].asInt();
        int key = lst[1].asInt();
        int vel = lst[2].asInt();

        fluid_synth_noteon(synth_, chan - 1, key, vel);
    }

    if (lst.size() == 2 && lst[0].isFloat() && lst[1].isFloat()) {
        int key = lst[0].asInt();
        int vel = lst[1].asInt();
        fluid_synth_noteon(synth_, 0, key, vel);
    }
}

void Fluid::processBlock(const t_sample** in, t_sample** out)
{
    if (synth_ == nullptr)
        return;

    float* left = out[0];
    float* right = out[1];
    fluid_synth_write_float(synth_, blockSize(), left, 0, 1, right, 0, 1);
}

void setup_misc_fluid()
{
    SoundExternalFactory<Fluid> obj("fluid~", OBJECT_FACTORY_DEFAULT);
}
