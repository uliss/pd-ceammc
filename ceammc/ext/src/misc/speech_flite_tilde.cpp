/*****************************************************************************
 * Copyright 2021 Serge Poltavsky. All rights reserved.
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
#include "speech_flite_tilde.h"
#include "ceammc_factory.h"
#include "ceammc_format.h"

#include <chrono>
#include <thread>

extern "C" {
#include "cst_features.h"
#include "cst_wave.h"
#include "flite.h"
}

enum Status {
    OK = 0,
    ERR_UNKNOWN_VOICE = 1,
    ERR_SYNTH
};

static int do_speech_synth(TtsQueue& queue,
    const std::atomic_bool& run_flag,
    int SR,
    const std::string& voice,
    float speed,
    float pitch,
    const std::string& txt)
{
    cst_voice* vc = flite_voice_select(voice.c_str());
    if (!vc)
        return ERR_UNKNOWN_VOICE;

    // set features
    auto stretch = (speed > 0) ? (1 / speed) : 1;
    feat_set_float(vc->features, "duration_stretch", stretch);

    if (pitch > 0)
        feat_set_float(vc->features, "int_f0_target_mean", pitch);

    // render
    cst_wave* wave = flite_text_to_wave(txt.c_str(), vc);
    if (!wave)
        return ERR_SYNTH;

    // resample
    cst_wave_resample(wave, SR);

    // store
    int tries = 0;
    for (int i = 0; i < wave->num_samples; i++) {
        auto sample = wave->samples[i] / 32768.0;

        while (!queue.try_enqueue(sample)) {
            tries++;
            std::this_thread::sleep_for(std::chrono::microseconds(1));
            if (!run_flag)
                goto done;
        }
    }

done:
    delete_wave(wave);
    return OK;
}

SpeechFilteTilde::SpeechFilteTilde(const PdArgs& args)
    : SoundExternal(args)
    , voice_name_(nullptr)
    , queue_(1024)
    , clock_([this]() { checkSynth(); })
{
    voice_name_ = new SymbolEnumProperty("@voice", { "kal16", "slt", "rms", "awb" });
    addProperty(voice_name_);

    speed_ = new FloatProperty("@speed", 1);
    speed_->checkClosedRange(0.5, 4);
    addProperty(speed_);

    pitch_ = new FloatProperty("@pitch", -1);
    addProperty(pitch_);

    createSignalOutlet();
    createOutlet();
}

SpeechFilteTilde::~SpeechFilteTilde()
{
    clock_.unset();
    run_ = false;
    if (running_.valid()) {
        while (running_.wait_for(std::chrono::seconds(1)) != std::future_status::ready) {
            OBJ_LOG << "waiting speech engine to finish...";
        }
    }
}

void SpeechFilteTilde::onFloat(t_float f)
{
    char buf[32];
    snprintf(buf, sizeof(buf) - 1, "%g", f);
    sentences_.emplace(buf);
    checkSynth();
}

void SpeechFilteTilde::onSymbol(t_symbol* s)
{
    sentences_.emplace(s->s_name);
    checkSynth();
}

void SpeechFilteTilde::onList(const AtomListView& lv)
{
    sentences_.emplace(to_string(lv));
    checkSynth();
}

void SpeechFilteTilde::processBlock(const t_sample** in, t_sample** out)
{
    auto BS = blockSize();

    if (!run_) {
        for (size_t i = 0; i < BS; i++)
            out[0][i] = 0;
    } else {
        for (size_t i = 0; i < BS; i++) {
            float sample = 0;
            if (queue_.try_dequeue(sample)) {
                out[0][i] = sample;
            } else {
                out[0][i] = 0;
            }
        }
    }
}

void SpeechFilteTilde::m_stop(t_symbol* s, const AtomListView& lv)
{
    run_ = false;
    clock_.unset();
    while (!sentences_.empty())
        sentences_.pop();

    float f;
    while (queue_.try_dequeue(f))
        ;
}

void SpeechFilteTilde::checkSynth()
{
    if (running_.valid()) {
        try {
            auto st = running_.wait_for(std::chrono::seconds(0));
            if (st != std::future_status::ready) { // synth is running
                clock_.delay(250);
                return;
            } else { // synth done
                auto rc = running_.get();
                switch (rc) {
                case ERR_SYNTH:
                    OBJ_ERR << "synth error";
                    break;
                case ERR_UNKNOWN_VOICE:
                    OBJ_ERR << "unknown voice";
                    break;
                case OK:
                default:
                    break;
                }

                run_ = false;
                floatTo(1, sentences_.size());
            }
        } catch (std::exception& e) {
            OBJ_ERR << e.what();
            return;
        }
    }

    try {
        if (sentences_.empty())
            return;

        OBJ_LOG << "say: " << sentences_.front();
        run_ = true;
        running_ = std::async(std::launch::async,
            do_speech_synth,
            std::ref(queue_),
            std::ref(run_),
            (int)sys_getsr(),
            voice_name_->value()->s_name,
            (float)speed_->value(),
            (float)pitch_->value(),
            sentences_.front());

        sentences_.pop();
        clock_.delay(250);
    } catch (std::exception& e) {
        OBJ_ERR << "can't run speech synth: " << e.what();
    }
}

void setup_misc_speech_filte_tilde()
{
    SoundExternalFactory<SpeechFilteTilde> obj("speech.flite~", OBJECT_FACTORY_DEFAULT);
    obj.addMethod("stop", &SpeechFilteTilde::m_stop);
}
