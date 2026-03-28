/*****************************************************************************
 * Copyright 2026 Serge Poltavski. All rights reserved.
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
#include "misc_whisper_tilde.h"
#include "ceammc_crc32.h"
#include "ceammc_factory.h"
#include "datatype_string.h"

namespace ceammc {

CEAMMC_DEFINE_HASH(auto)
CEAMMC_DEFINE_HASH(en)
CEAMMC_DEFINE_HASH(ru)

CEAMMC_DEFINE_HASH(list)
CEAMMC_DEFINE_HASH(string)
CEAMMC_DEFINE_HASH(symbol)

ceammc_whisper_lang to_lang(t_symbol* lang)
{
    switch (ceammc::crc32_hash(lang)) {
    case hash_ru:
        return ceammc_whisper_lang::Russian;
    case hash_en:
        return ceammc_whisper_lang::English;
    default:
        return ceammc_whisper_lang::Auto;
    }
}

MiscWhisperTilde::MiscWhisperTilde(const PdArgs& args)
    : RustMessageObject<SoundExternal>(args)
    , w_(nullptr, &ceammc_misc_whisper_free)
{
    createOutlet();

    model_ = new SymbolProperty("@model", &s_);
    model_->setInitOnly();
    addProperty(model_);

    lang_ = new SymbolEnumProperty("@lang", { str_auto, str_en, str_ru });
    addProperty(lang_);

    output_mode_ = new SymbolEnumProperty("@output", { str_list, str_string, str_symbol });
    addProperty(output_mode_);

    num_threads_ = new IntProperty("@threads", 1);
    num_threads_->checkClosedRange(1, 16);
    addProperty(num_threads_);

    max_length_ = new FloatProperty("@max_length", 10);
    max_length_->checkClosedRange(0, 120);
    max_length_->setUnitsSec();
    addProperty(max_length_);
}

void MiscWhisperTilde::initDone()
{
    if (model_->cstr()[0] != 0) {
        w_.reset(ceammc_misc_whisper_create(model_->cstr(),
            on_notify(),
            on_message(),
            {
                this,
                [](void* user, const char* txt) {
                    auto obj = static_cast<MiscWhisperTilde*>(user);
                    if (obj) {
                        switch (crc32_hash(obj->output_mode_->value())) {
                        case hash_symbol:
                            obj->symbolTo(0, gensym(txt));
                            break;
                        case hash_string:
                            obj->atomTo(0, StringAtom(txt));
                            break;
                        case hash_list:
                        default:
                            obj->listTo(0, AtomList::parseString(txt));
                            break;
                        }
                    }
                },
            }));
    } else {
        OBJ_ERR << "model is not specified";
    }
}

bool MiscWhisperTilde::notify(int code)
{
    return ceammc_misc_whisper_process_reply(w_.get());
}

void MiscWhisperTilde::processBlock(const t_sample** in, t_sample** out)
{
    if (!w_ || !do_record_)
        return;

    const auto BS = blockSize();
    const auto SR = samplerate();
    size_t max_samples = SR * max_length_->value();
    if (recorded_samples_ >= max_samples) {
        do_record_ = false;
        return;
    }

#if PD_FLOATSIZE == 32
    ceammc_misc_whisper_append_samples(w_.get(), &in[0][0], BS);
#elif PD_FLOATSIZE == 64
    float buf[BS];
    for (size_t i = 0; i < BS; i++) {
        buf[i] = in[0][i];
    }
    ceammc_misc_whisper_append_samples(w_.get(), buf, BS);
#else
#endif
}

void MiscWhisperTilde::m_clear(t_symbol* s, const AtomListView& lv)
{
    ceammc_misc_whisper_clear(w_.get());
}

void MiscWhisperTilde::m_recognize(t_symbol* s, const AtomListView& lv)
{
    do_record_ = false;
    ceammc_misc_whisper_set_language(w_.get(), to_lang(lang_->symbol()))
        && ceammc_misc_whisper_set_num_threads(w_.get(), num_threads_->value())
        && ceammc_misc_whisper_recognize(w_.get(), samplerate());
}

void MiscWhisperTilde::m_record(t_symbol* s, const AtomListView& lv)
{
    do_record_ = true;
    recorded_samples_ = 0;
}

void MiscWhisperTilde::m_stop(t_symbol* s, const AtomListView& lv)
{
    do_record_ = false;
}

void setup_misc_whisper_tilde()
{
    SoundExternalFactory<MiscWhisperTilde> obj("misc.whisper~");

    obj.addMethod("clear", &MiscWhisperTilde::m_clear);
    obj.addMethod("recognize", &MiscWhisperTilde::m_recognize);
    obj.addMethod("record", &MiscWhisperTilde::m_record);
    obj.addMethod("stop", &MiscWhisperTilde::m_stop);
}

} // namespace ceammc
