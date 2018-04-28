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
#include "ceammc_platform.h"

#include <fluidsynth.h>

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

    const char* fn = lst.symbolAt(0, &s_)->s_name;
    std::string filename = findInStdPaths(fn);

    if (filename.empty()) {
        filename = platform::find_in_exernal_dir(owner(), fn);

        if (filename.empty()) {
            OBJ_ERR << "sound font is not found: " << lst;
            return;
        }
    }

    if (fluid_synth_sfload(synth_, filename.c_str(), 0) >= 0) {

        OBJ_DBG << "loaded soundfont: " << filename;
        fluid_synth_program_reset(synth_);

        sound_font_ = lst.symbolAt(0, &s_);
    } else {
        OBJ_ERR << "can't load soundfont: " << lst;
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
    } else if (lst.size() == 2 && lst[0].isFloat() && lst[1].isFloat()) {
        int key = lst[0].asInt();
        int vel = lst[1].asInt();
        fluid_synth_noteon(synth_, 0, key, vel);
    } else {
        METHOD_ERR(s) << "CHAN KEY VEL or KEY VEL expected: " << lst;
    }
}

void Fluid::m_cc(t_symbol* s, const AtomList& lst)
{
    if (synth_ == nullptr)
        return;

    if (lst.size() == 3 && lst[0].isFloat() && lst[1].isFloat() && lst[2].isFloat()) {
        int chan = lst[0].asInt();
        int key = lst[1].asInt();
        int vel = lst[2].asInt();

        fluid_synth_cc(synth_, chan - 1, key, vel);
    } else if (lst.size() == 2 && lst[0].isFloat() && lst[1].isFloat()) {
        int key = lst[0].asInt();
        int vel = lst[1].asInt();
        fluid_synth_cc(synth_, 0, key, vel);
    } else {
        METHOD_ERR(s) << "CHAN KEY VAL or KEY VAL expected: " << lst;
    }
}

void Fluid::m_prog(t_symbol* s, const AtomList& lst)
{
    if (synth_ == nullptr)
        return;

    if (lst.size() == 2 && lst[0].isFloat() && lst[1].isFloat()) {
        int chan = lst[0].asInt();
        int program = lst[1].asInt();
        fluid_synth_program_change(synth_, chan - 1, program);
    } else {
        METHOD_ERR(s) << "CHAN PROGRAM expected: " << lst;
    }
}

void Fluid::m_bank(t_symbol* s, const AtomList& lst)
{
    if (synth_ == nullptr)
        return;

    if (lst.size() == 2 && lst[0].isFloat() && lst[1].isFloat()) {
        int chan = lst[0].asInt();
        int bank = lst[1].asInt();
        fluid_synth_bank_select(synth_, chan - 1, bank);
    } else {
        METHOD_ERR(s) << "CHAN BANK expected: " << lst;
    }
}

void Fluid::m_bend(t_symbol* s, const AtomList& lst)
{
    if (synth_ == nullptr)
        return;

    if (lst.size() == 2 && lst[0].isFloat() && lst[1].isFloat()) {
        int chan = lst[0].asInt();
        int val = lst[1].asInt();
        fluid_synth_pitch_bend(synth_, chan - 1, val);
    } else {
        METHOD_ERR(s) << "CHAN VAL expected: " << lst;
    }
}

void Fluid::m_gen(t_symbol* s, const AtomList& lst)
{
    if (synth_ == nullptr)
        return;

    if (lst.size() == 3 && lst[0].isFloat() && lst[1].isFloat() && lst[2].isFloat()) {
        int chan = lst[0].asInt();
        int param = lst[1].asInt();
        int value = lst[2].asInt();

        fluid_synth_set_gen(synth_, chan - 1, param, value);
    } else if (lst.size() == 2 && lst[0].isFloat() && lst[1].isFloat()) {
        int param = lst[0].asInt();
        int value = lst[1].asInt();
        fluid_synth_cc(synth_, 0, param, value);
    } else {
        METHOD_ERR(s) << "CHAN PARAM VAL or PARAM VAL expected: " << lst;
    }
}

void Fluid::m_reset(t_symbol* s, const AtomList& lst)
{
    if (synth_ == nullptr)
        return;

    fluid_synth_system_reset(synth_);
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

    obj.addMethod("note", &Fluid::m_note);
    obj.addMethod("cc", &Fluid::m_cc);
    obj.addMethod("prog", &Fluid::m_prog);
    obj.addMethod("bank", &Fluid::m_bank);
    obj.addMethod("bend", &Fluid::m_bend);
    obj.addMethod("gen", &Fluid::m_gen);
    obj.addMethod("reset", &Fuild::m_reset);
}
