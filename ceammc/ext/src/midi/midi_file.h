#ifndef MIDI_FILE_H
#define MIDI_FILE_H

#include "ceammc_object.h"

using namespace ceammc;

class XMidiFile : public ceammc::BaseObject {
public:
    XMidiFile(const PdArgs& a);
};

extern "C" void setup_midi_file();

#endif // MIDI_FILE_H
