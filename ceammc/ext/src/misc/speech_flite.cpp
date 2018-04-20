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
#include "speech_flite.h"
#include "ceammc_factory.h"

extern "C" {
#include "cst_wave.h"
#include "flite.h"

extern cst_voice* register_cmu_us_kal16();
extern cst_voice* register_cmu_us_slt();
extern cst_voice* register_cmu_us_rms();
extern void unregister_cmu_us_kal16();
}

static t_symbol* SYM_VOICE_KAL16 = gensym("kal16");
static t_symbol* SYM_VOICE_SLT = gensym("slt");
static t_symbol* SYM_VOICE_DEFAULT = SYM_VOICE_KAL16;

SpeechFlite::SpeechFlite(const PdArgs& args)
    : BaseObject(args)
    , name_(positionalSymbolArgument(0, &s_))
    , voice_(0)
    , voice_name_(SYM_VOICE_DEFAULT)
{
    createProperty(new PointerProperty<t_symbol*>("@array", &name_, false));

    propSetVoice(Atom(SYM_VOICE_DEFAULT));
    createCbProperty("@voice", &SpeechFlite::propVoice, &SpeechFlite::propSetVoice);

    createInlet(&name_);
    createOutlet();
}

void SpeechFlite::onSymbol(t_symbol* s)
{
    synth(s->s_name);
}

AtomList SpeechFlite::propVoice() const
{
    return Atom(voice_name_);
}

void SpeechFlite::propSetVoice(const AtomList& lst)
{
    if (!checkArgs(lst, ARG_SYMBOL)) {
        OBJ_ERR << "voice name expected: " << lst;
        return;
    }

    t_symbol* name = lst.symbolAt(0, SYM_VOICE_DEFAULT);
    cst_voice* v = flite_voice_select(name->s_name);
    if (!v) {
        OBJ_ERR << "can't load voice: " << lst;
        return;
    }

    voice_ = v;
}

bool SpeechFlite::synth(const char* str)
{
    if (!array_.open(name_)) {
        OBJ_ERR << "can't open array: " << name_;
        return false;
    }

    if (!voice_) {
        OBJ_ERR << "no voice selected";
        return false;
    }

    cst_wave* wave = flite_text_to_wave(str, voice_);

    if (!wave) {
        OBJ_ERR << "synthesis failed for text '" << str << "'";
        return false;
    }

    cst_wave_resample(wave, sys_getsr());

    if (!array_.resize(wave->num_samples)) {
        OBJ_ERR << "can't resize array to " << wave->num_samples;
        return false;
    }

    for (int i = 0; i < wave->num_samples; i++)
        array_[i] = wave->samples[i] / 32767.0;

    delete_wave(wave);
    array_.redraw();

    bangTo(0);

    return true;
}

void setup_misc_speech_flite()
{
    ObjectFactory<SpeechFlite> obj("speech.flite");
    flite_init();

    flite_add_voice(register_cmu_us_kal16());
    flite_add_voice(register_cmu_us_slt());
    flite_add_voice(register_cmu_us_rms());
}
