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
#include "sfizz_tilde.h"
#include "ceammc_factory.h"

SfizzTilde::SfizzTilde(const PdArgs& args)
    : SoundExternal(args)
    , sf_path_(nullptr)
{
    sf_path_ = new SymbolProperty("@sf", &s_);
    addProperty(sf_path_);
    sf_path_->setArgIndex(0);
    sf_path_->setSuccessFn([this](Property*) {
        sfz_.loadSfzFile(sf_path_->value()->s_name);
        updateLabels();
    });

    createCbFloatProperty(
        "@freq",
        [this]() { return sfz_.getTuningFrequency(); },
        [this](t_float f) { sfz_.setTuningFrequency(f); return true; })
        ->setUnits(PropValueUnits::HZ);

    createCbIntProperty(
        "@poly",
        [this]() { return sfz_.getNumVoices(); },
        [this](int n) { sfz_.setNumVoices(n); return true; });

    createCbIntProperty(
        "@avoices",
        [this]() { return sfz_.getNumActiveVoices(); });

    addProperty(new PointerProperty<Atom>("@key_labels", &key_lables_, PropValueAccess::READONLY));
    addProperty(new PointerProperty<Atom>("@cc_labels", &cc_lables_, PropValueAccess::READONLY));

    createSignalOutlet();
    createSignalOutlet();
}

void SfizzTilde::setupDSP(t_signal** sig)
{
    SoundExternal::setupDSP(sig);
    sfz_.setSampleRate(samplerate());
    sfz_.setSamplesPerBlock(blockSize());
}

void SfizzTilde::processBlock(const t_sample** in, t_sample** out)
{
    const auto BS = blockSize();

#if PD_FLOATSIZE == 32
    sfz_.renderBlock(out, BS, 1);
#else
    float left[BS];
    float right[BS];
    float* buf[2] = { left, right };
    sfz_.renderBlock(buf, BS, 1);
    for (size_t i = 0; i < BS; i++) {
        out[0][i] = buf[0][i];
        out[1][i] = buf[1][i];
    }
#endif
}

void SfizzTilde::dump() const
{
    SoundExternal::dump();

    {
        Post os(this);
        os << "Key labels:\n";
        for (auto& kv : sfz_.getKeyLabels())
            os << "key: " << (int)kv.first << ", label: " << kv.second << "\n";
    }

    {
        Post os(this);
        os << "CC labels:\n";
        for (auto& kv : sfz_.getCCLabels())
            os << "cc: " << (int)kv.first << ", label: " << kv.second << "\n";
    }
}

void SfizzTilde::m_note(t_symbol* s, const AtomListView& lv)
{
    if (lv.size() == 3 && lv[0].isFloat() && lv[1].isFloat() && lv[2].isFloat()) {
        // NOTE: channel is ignored, but leaved for compatibility with fluid~ external
        // that accepts both [note CHAN KEY VEL( and [note KEY VEL( messages
        // int chan = lv[0].asInt();
        int key = lv[1].asInt();
        int vel = lv[2].asInt();

        if (vel > 0)
            sfz_.noteOn(0, key, vel);
        else
            sfz_.noteOff(0, key, vel);

    } else if (lv.size() == 2 && lv[0].isFloat() && lv[1].isFloat()) {
        int key = lv[0].asInt();
        int vel = lv[1].asInt();

        if (vel > 0)
            sfz_.noteOn(0, key, vel);
        else
            sfz_.noteOff(0, key, vel);

    } else {
        METHOD_ERR(s) << "CHAN KEY VEL or KEY VEL expected: " << lv;
    }
}

void SfizzTilde::m_cc(t_symbol* s, const AtomListView& lv)
{
    if (lv.size() == 3 && lv[0].isFloat() && lv[1].isFloat() && lv[2].isFloat()) {
        // NOTE: channel is ignored, but leaved for compatibility with fluid~ external
        // that accepts both [cc CHAN CC VAL( and [note CC VAL( messages
        // int chan = lv[0].asInt();

        int num = lv[1].asInt();
        int value = lv[2].asInt();

        sfz_.cc(0, num, value);
    } else if (lv.size() == 2 && lv[0].isFloat() && lv[1].isFloat()) {
        int num = lv[0].asInt();
        int value = lv[1].asInt();
        sfz_.cc(0, num, value);
    } else {
        METHOD_ERR(s) << "CHAN CC VAL or CC VAL expected: " << lv;
    }
}

void SfizzTilde::m_bend(t_symbol* s, const AtomListView& lv)
{
    if (lv.size() == 2 && lv[0].isFloat() && lv[1].isFloat()) {
        int chan = lv[0].asInt();
        int val = lv[1].asInt();
        sfz_.pitchWheel(0, val);
    } else {
        METHOD_ERR(s) << "CHAN VAL expected: " << lv;
    }
}

void SfizzTilde::m_soundsOff(t_symbol*, const AtomListView&)
{
    sfz_.allSoundOff();
}

void SfizzTilde::updateLabels()
{
    // key labels
    key_lables_->clear();
    char buf[32];
    for (auto& kv : sfz_.getKeyLabels()) {
        snprintf(buf, sizeof(buf) - 1, "%i", static_cast<int>(kv.first));
        key_lables_->insert(buf, gensym(kv.second.c_str()));
    }

    // cc labels
    cc_lables_->clear();
    for (auto& kv : sfz_.getCCLabels()) {
        snprintf(buf, sizeof(buf) - 1, "%i", static_cast<int>(kv.first));
        cc_lables_->insert(buf, gensym(kv.second.c_str()));
    }
}

void setup_misc_sfizz_tilde()
{
    SoundExternalFactory<SfizzTilde> obj("sfizz~");

    obj.addMethod("note", &SfizzTilde::m_note);
    obj.addMethod("cc", &SfizzTilde::m_cc);
    obj.addMethod("bend", &SfizzTilde::m_bend);
    obj.addMethod("sounds_off", &SfizzTilde::m_soundsOff);
}
