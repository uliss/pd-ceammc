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
#include "ceammc_format.h"
#include "datatype_string.h"
#include "fliterender.h"

extern "C" {
#include "cst_wave.h"
#include "flite.h"

extern cst_voice* register_cmu_us_kal16();
extern cst_voice* register_cmu_us_slt();
extern cst_voice* register_cmu_us_rms();
extern cst_voice* register_cmu_us_awb();
extern void unregister_cmu_us_kal16();
}

SpeechFlite::SpeechFlite(const PdArgs& args)
    : BaseObject(args)
    , name_(&s_)
    , voice_name_(nullptr)
    , speed_(nullptr)
    , pitch_(nullptr)
    , render_(new FliteThread())
    , clock_(this, &SpeechFlite::clockTick)
{
    createCbSymbolProperty(
        "@array",
        [this]() -> t_symbol* { return name_; },
        [this](t_symbol* s) -> bool { name_ = s; return true; })
        ->setArgIndex(0);

    voice_name_ = new SymbolEnumProperty("@voice", { "kal16", "slt", "rms", "awb" });
    addProperty(voice_name_);

    speed_ = new FloatProperty("@speed", 1);
    speed_->checkClosedRange(1, 4);
    addProperty(speed_);

    pitch_ = new FloatProperty("@pitch", -1);
    addProperty(pitch_);

    createInlet(&name_);
    createOutlet();
}

SpeechFlite::~SpeechFlite()
{
    delete render_;
}

void SpeechFlite::onFloat(t_float v)
{
    char buf[64];
    sprintf(buf, "%g", v);
    synth(buf);
}

void SpeechFlite::onSymbol(t_symbol* s)
{
    synth(s->s_name);
}

void SpeechFlite::onList(const AtomListView& lv)
{
    synth(to_string(lv).c_str());
}

void SpeechFlite::onDataT(const StringAtom& dptr)
{
    synth(dptr->str().c_str());
}

bool SpeechFlite::synth(const char* str)
{
    if (!array_.open(name_)) {
        OBJ_ERR << "can't open array: " << name_;
        return false;
    }

    FliteFeatures ff;
    ff.speed = 1.f / speed_->value();
    ff.pitch = pitch_->value();

    if (!render_->start(str, voice_name_->value()->s_name, ff)) {
        OBJ_ERR << "worker error";
        return false;
    } else {
        clock_.delay(25);
    }

    return true;
}

void SpeechFlite::clockTick()
{
    if (render_->isRunning()) {
        clock_.delay(25);
    } else {
        // array was delete while worker running
        if (!array_.update()) {
            OBJ_ERR << "array not found: " << array_.name();
            return;
        }

        if (!render_->copyToArray(array_)) {
            OBJ_ERR << "copy error";
            return;
        }

        array_.redraw();
        bangTo(0);
    }
}

void setup_misc_speech_flite()
{
    ObjectFactory<SpeechFlite> obj("speech.flite");
    flite_init();

    flite_add_voice(register_cmu_us_kal16());
    flite_add_voice(register_cmu_us_slt());
    flite_add_voice(register_cmu_us_rms());
    flite_add_voice(register_cmu_us_awb());

    obj.processData<DataTypeString>();
}
