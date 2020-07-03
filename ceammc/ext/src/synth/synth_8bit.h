#ifndef SYNTH_8BIT_H
#define SYNTH_8BIT_H

#include "ceammc_sound_external.h"

using namespace ceammc;

class Synth8Bit : public SoundExternal {
    unsigned int t_;

public:
    Synth8Bit(const PdArgs& args);
    void processBlock(const t_sample** in, t_sample** out) override;
};

void setup_fx_8bit();

#endif // SYNTH_8BIT_H
