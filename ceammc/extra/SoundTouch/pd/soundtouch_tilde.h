#ifndef SOUNDTOUCH_TILDE_H
#define SOUNDTOUCH_TILDE_H

#include "SoundTouch.h"

#include "ceammc_sound_external.h"
using namespace ceammc;

class SoundTouchExt : public SoundExternal {
    soundtouch::SoundTouch stouch_;
    BoolProperty* bypass_;
    Property* pitch_;
    t_float pitch_value_;

public:
    SoundTouchExt(const PdArgs& a);
    void processBlock(const t_sample** in, t_sample** out) override;

    void onInlet(size_t, const AtomList&) override;

private:
    void initSoundTouch();
};

#endif // SOUNDTOUCH_TILDE_H
