#ifndef MIDI_PROGCHANGE2STR_H
#define MIDI_PROGCHANGE2STR_H

#include "ceammc_object.h"

using namespace ceammc;

class ProgChange2Str : public BaseObject {
    FlagProperty* as_symbol_;
    FlagProperty* family_;

public:
    ProgChange2Str(const PdArgs& a);
    void onFloat(float v);
};

void setup_midi_progchange2str();

#endif // MIDI_PROGCHANGE2STR_H
