#include "soundtouch_tilde.h"
#include "ceammc_convert.h"
#include "ceammc_factory.h"

#include <cmath>

static t_symbol* PROP_ANTIALIAS = gensym("@antialias");
static t_symbol* PROP_ANTIALIAS_LENGTH = gensym("@aalength");

SoundTouchExt::SoundTouchExt(const PdArgs& a)
    : SoundExternal(a)
    , bypass_(0)
    , pitch_(0)
{
    createSignalOutlet();

    createCbProperty("@pitch", &SoundTouchExt::propPitch, &SoundTouchExt::propSetPitch);

    // antialias
    createCbProperty(PROP_ANTIALIAS->s_name,
        &SoundTouchExt::propAnitAlias,
        &SoundTouchExt::propSetAntiAlias);

    // antialias length
    createCbProperty(PROP_ANTIALIAS_LENGTH->s_name,
        &SoundTouchExt::propAnitAliasLength,
        &SoundTouchExt::propSetAntiAliasLength);

    bypass_ = new BoolProperty("@bypass", false);
    createProperty(bypass_);

    initSoundTouch();
}

void SoundTouchExt::processBlock(const t_sample** in, t_sample** out)
{
    if (bypass_->value()) {
        std::copy(in[0], in[0] + blockSize(), out[0]);
        return;
    }

    stouch_.putSamples(in[0], blockSize());
    stouch_.receiveSamples(out[0], blockSize());
}

AtomList SoundTouchExt::propAnitAlias() const
{
    return Atom(stouch_.getSetting(SETTING_USE_AA_FILTER));
}

void SoundTouchExt::propSetAntiAlias(const AtomList& l)
{
    if (!checkArgs(l, ARG_BOOL, gensym("@antialias")))
        return;

    stouch_.setSetting(SETTING_USE_AA_FILTER, l[0].asInt());
}

AtomList SoundTouchExt::propAnitAliasLength() const
{
    return Atom(stouch_.getSetting(SETTING_AA_FILTER_LENGTH));
}

void SoundTouchExt::propSetAntiAliasLength(const AtomList& l)
{
    if (!checkArgs(l, ARG_FLOAT, PROP_ANTIALIAS_LENGTH))
        return;

    int len = clip<int>(l[0].asInt(), 8, 128);
    stouch_.setSetting(SETTING_AA_FILTER_LENGTH, len);
}

AtomList SoundTouchExt::propPitch() const
{
    return Atom(pitch_);
}

void SoundTouchExt::propSetPitch(const AtomList& l)
{
    if (!checkArgs(l, ARG_FLOAT))
        return;

    pitch_ = l[0].asFloat();
    stouch_.setPitchSemiTones(pitch_);
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
}
