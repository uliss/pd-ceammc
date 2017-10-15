#ifndef MIDI_EVENT_TO_CTL_H
#define MIDI_EVENT_TO_CTL_H

#include "midi_event.h"

class MidiEventToCtl : public BaseMidiEventExternal {
    AtomList msg_;

public:
    MidiEventToCtl(const PdArgs& args);

    void processEvent();
};

void setup_midi_event2ctl();

#endif // MIDI_EVENT_TO_CTL_H
