#ifndef SYNTH_WURLEY_H
#define SYNTH_WURLEY_H

#include "stksynth.h"

class SynthWurley : public StkSynth {
public:
    SynthWurley(const PdArgs& args);
};

void setup_synth_wurley();

#endif // SYNTH_WURLEY_H
