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
#include "fmt/core.h"

extern "C" {
#include "cst_wave.h"
#include "flite.h"

extern cst_voice* register_cmu_us_kal16();
extern cst_voice* register_cmu_us_slt();
extern cst_voice* register_cmu_us_rms();
extern cst_voice* register_cmu_us_awb();

using register_fn = typeof(&register_cmu_us_kal16);

void usenglish_init(cst_voice* v);
cst_lexicon* cmulex_init(void);
}

namespace {

void add_voice(cst_voice* v)
{
    if (!v)
        return;

    if (flite_add_voice(v))
        LIB_DBG << fmt::format("[flite] voice added: {}", v->name);
    else
        LIB_ERR << fmt::format("[flite] can't add voice: {}", v->name);
}

void load_voice(const char* path)
{
    auto v = flite_voice_load(path);
    if (!v) {
        LIB_ERR << fmt::format("[flite] can't load voice: {}", path);
        return;
    }

    add_voice(v);
}

void register_voice(register_fn fn, const char* name)
{
    auto v = fn();
    if (!v) {
        LIB_ERR << fmt::format("[flite] can't register voice: {}", name);
        return;
    }

    add_voice(v);
}

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
    flite_init();

    register_voice(register_cmu_us_kal16, "kal16");
    register_voice(register_cmu_us_slt, "slt");
    register_voice(register_cmu_us_rms, "rms");
    register_voice(register_cmu_us_awb, "awb");

    flite_add_lang("eng", usenglish_init, cmulex_init);
    flite_add_lang("usenglish", usenglish_init, cmulex_init);

    ObjectFactory<SpeechFlite> obj("speech.flite");
    obj.processData<DataTypeString>();
}
