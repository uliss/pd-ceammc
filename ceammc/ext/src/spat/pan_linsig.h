#ifndef PAN_LINSIG_H
#define PAN_LINSIG_H

#include "ceammc_sound_external.h"

using namespace ceammc;

class PanLinSig : public SoundExternal {
public:
    PanLinSig(const PdArgs& args);
    void processBlock(const t_sample** in, t_sample** out) override;
};

void setup_pan_linsig();

#endif // PAN_LINSIG_H
