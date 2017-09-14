#ifndef MIDI_KEY2STR_H
#define MIDI_KEY2STR_H

#include "ceammc_object.h"

using namespace ceammc;

class MidiKey2Str : public BaseObject {
public:
    MidiKey2Str(const PdArgs& args);
    void onFloat(float f);
};

void setup_midi_key2str();

#endif // MIDI_KEY2STR_H
