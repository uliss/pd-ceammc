#ifndef MIDI_CTR2STR_H
#define MIDI_CTR2STR_H

#include "ceammc_object.h"

using namespace ceammc;

class MidiCtr2Str : public BaseObject {
    FlagProperty* as_symbol_;

public:
    MidiCtr2Str(const PdArgs& args);
    void onFloat(float f);
};

void setup_midi_ctr2str();

#endif // MIDI_CTR2STR_H
