#include "soundtouch_tilde.h"
#include "ceammc_convert.h"
#include "ceammc_factory.h"

#include <cmath>

constexpr t_float DEF_PITCH = 0;
static t_symbol* PROP_ANTIALIAS_LENGTH;

SoundTouchExt::SoundTouchExt(const PdArgs& a)
    : SoundExternal(a)
    , bypass_(nullptr)
    , pitch_(nullptr)
    , pitch_value_(DEF_PITCH)
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

    initSoundTouch();
}

void SoundTouchExt::processBlock(const t_sample** in, t_sample** out)
{
    if (bypass_->value()) {
        std::copy(in[0], in[0] + blockSize(), out[0]);
        return;
    }

    const size_t bs = blockSize();

#if PD_FLOATSIZE == 32
    stouch_.putSamples(in[0], bs);
    stouch_.receiveSamples(out[0], bs);
#elif PD_FLOATSIZE == 64
    float fin[bs];
    float fout[bs];

    for(size_t i = 0; i < bs; i++)
        fin[i] = in[0][i];

    stouch_.putSamples(fin, bs);
    stouch_.receiveSamples(fout, bs);

    for(size_t i = 0; i < bs; i++)
        out[0][i] = fout[i];
#endif
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
    PROP_ANTIALIAS_LENGTH = gensym("@aalength");

    SoundExternalFactory<SoundTouchExt> obj("soundtouch~");
}
