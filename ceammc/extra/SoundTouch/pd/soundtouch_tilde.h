#ifndef SOUNDTOUCH_TILDE_H
#define SOUNDTOUCH_TILDE_H

#include "SoundTouch.h"

#include "ceammc_object.h"
using namespace ceammc;

class SoundTouchExt : public SoundExternal {
    soundtouch::SoundTouch stouch_;
    BoolProperty* bypass_;
    float pitch_;

public:
    SoundTouchExt(const PdArgs& a);
    void processBlock(const t_sample** in, t_sample** out);

    /**
     * Enable/disable anti-alias filter in pitch transposer (0 = disable)
     */
    AtomList propAnitAlias() const;
    void propSetAntiAlias(const AtomList& l);

    /**
     * Pitch transposer anti-alias filter length (8 .. 128 taps, default = 32)
     */
    AtomList propAnitAliasLength() const;
    void propSetAntiAliasLength(const AtomList& l);

    /**
     * @brief pitch shift in semitones
     */
    AtomList propPitch() const;
    void propSetPitch(const AtomList& l);

private:
    void initSoundTouch();
};

#endif // SOUNDTOUCH_TILDE_H
