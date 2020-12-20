#ifndef MIDI_PROGCHANGE2STR_H
#define MIDI_PROGCHANGE2STR_H

#include "ceammc_object.h"

using namespace ceammc;

class Prg2Str : public BaseObject {
    FlagProperty* as_symbol_;
    FlagProperty* family_;

public:
    Prg2Str(const PdArgs& a);
    void onFloat(t_float v) override;
};

void setup_midi_prg2str();

#endif // MIDI_PROGCHANGE2STR_H
