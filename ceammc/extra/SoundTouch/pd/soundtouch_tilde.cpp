#include "soundtouch_tilde.h"
#include "ceammc_factory.h"

#include <cmath>

constexpr t_float DEF_PITCH = 0;

SoundTouchExt::SoundTouchExt(const PdArgs& a)
    : SoundExternal(a)
    , pitch_value_(DEF_PITCH)
    , drywet_(0, 1)
{
    createSignalOutlet();
    createInlet();

    pitch_ = createCbFloatProperty(
        "@pitch",
        [this]() -> t_float { return pitch_value_; },
        [this](t_float f) -> bool {
            pitch_value_ = f;
            stouch_.setPitchSemiTones(pitch_value_);
            return true;
        });
    pitch_->setUnits(PropValueUnits::SEMITONE);
    pitch_->setArgIndex(0);

    // antialias
    createCbBoolProperty(
        "@antialias",
        [this]() -> bool { return stouch_.getSetting(SETTING_USE_AA_FILTER); },
        [this](bool b) -> bool { return stouch_.setSetting(SETTING_USE_AA_FILTER, b); });

    // antialias length
    auto alen = createCbIntProperty(
        "@aalength",
        [this]() -> int { return stouch_.getSetting(SETTING_AA_FILTER_LENGTH); },
        [this](int v) -> bool { return stouch_.setSetting(SETTING_AA_FILTER_LENGTH, v); });
    alen->setDefault(8);
    alen->setIntCheck(PropValueConstraints::CLOSED_RANGE, 8, 128);

    bypass_ = new BoolProperty("@bypass", false);
    addProperty(bypass_);

    {
        auto dw = new FloatProperty("@drywet", 1);
        dw->checkClosedRange(0, 1);
        dw->setSuccessFn([dw, this](Property* p) { drywet_.setTargetValue(dw->value()); });

        addProperty(dw);
    }

    initSoundTouch();
}

void SoundTouchExt::setupDSP(t_signal** sp)
{
    SoundExternal::setupDSP(sp);
    drywet_.setDurationMs(100, samplerate());
}

void SoundTouchExt::processBlock(const t_sample** in, t_sample** out)
{
    if (bypass_->value()) {
        std::copy(in[0], in[0] + blockSize(), out[0]);
        return;
    }

    const size_t bs = blockSize();

    float fin[bs];
    float fout[bs];

    for (size_t i = 0; i < bs; i++)
        fin[i] = in[0][i];

    stouch_.putSamples(fin, bs);
    auto nsamp = stouch_.receiveSamples(fout, bs);
    for (size_t i = nsamp; i < bs; i++)
        fout[i] = 0;

    for (size_t i = 0; i < bs; i++) {
        out[0][i] = interpolate::linear<t_sample>(in[0][i], fout[i], drywet_());
    }
}

void SoundTouchExt::onInlet(size_t, const AtomListView& lst)
{
    pitch_->set(lst);
}

void SoundTouchExt::initSoundTouch()
{
    stouch_.setChannels(1);
    stouch_.setSampleRate(sys_getsr() ? sys_getsr() : 44100);
    stouch_.setTempo(1);
    stouch_.setRate(1);
    stouch_.setPitch(1);
    stouch_.setSetting(SETTING_USE_QUICKSEEK, 0);
    stouch_.setSetting(SETTING_USE_AA_FILTER, 0);

    stouch_.setSetting(SETTING_SEQUENCE_MS, 40);
    stouch_.setSetting(SETTING_SEEKWINDOW_MS, 20);
    stouch_.setSetting(SETTING_OVERLAP_MS, 10);
}

extern "C" void soundtouch_tilde_setup()
{
    SoundExternalFactory<SoundTouchExt> obj("soundtouch~");

    LIB_DBG << "Soundtouch version: " << SOUNDTOUCH_VERSION;
}
