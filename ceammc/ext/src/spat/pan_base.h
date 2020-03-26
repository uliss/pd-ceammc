#ifndef PAN_BASE_H
#define PAN_BASE_H

#include "ceammc_sound_external.h"

using namespace ceammc;

class PanBase : public SoundExternal {
    IntProperty* smooth_;

protected:
    FloatProperty* pos_;
    SmoothControlValue smooth_pos_;

public:
    PanBase(const PdArgs& args);
    void onInlet(size_t n, const AtomList& l) override;

    void setupDSP(t_signal** sp) override;
};

#endif // PAN_BASE_H
