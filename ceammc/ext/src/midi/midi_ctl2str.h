#ifndef MIDI_CTL2STR_H
#define MIDI_CTL2STR_H

#include "ceammc_object.h"

using namespace ceammc;

class MidiCtl2Str : public BaseObject {
    FlagProperty* as_symbol_;

public:
    MidiCtl2Str(const PdArgs& args);
    void onFloat(t_float f) override;
};

void setup_midi_ctl2str();

#endif // MIDI_CTL2STR_H
