#ifndef SOUNDTOUCH_TILDE_H
#define SOUNDTOUCH_TILDE_H

#include "SoundTouch.h"

#include "ceammc_signal.h"
#include "ceammc_sound_external.h"
using namespace ceammc;

class SoundTouchExt : public SoundExternal {
    soundtouch::SoundTouch stouch_;
    BoolProperty* bypass_ { nullptr };
    Property* pitch_ { nullptr };
    t_float pitch_value_;
    SmoothLinT<t_sample> drywet_;

public:
    SoundTouchExt(const PdArgs& a);
    void setupDSP(t_signal** sp) final;
    void processBlock(const t_sample** in, t_sample** out) final;

    void onInlet(size_t, const AtomListView&) override;

private:
    void initSoundTouch();
};

#endif // SOUNDTOUCH_TILDE_H
