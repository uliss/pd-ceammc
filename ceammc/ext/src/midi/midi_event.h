#ifndef MIDI_EVENT_H
#define MIDI_EVENT_H

#include "MidiEvent.h"
#include "ceammc_object.h"

using namespace ceammc;

class XMidiEvent {
    bool valid_;
    AtomList raw_;
    MidiEvent event_;
    t_float duration_;
    int track_;

public:
    XMidiEvent(const AtomList& l);
    bool parse(const AtomList& l);
    bool isNote() const;
    bool isValid() const;
    t_float duration() const;
    MidiEvent& event();
    int track() const;
};

class MidiEventIsNote : public BaseObject {
    XMidiEvent ev_;

public:
    MidiEventIsNote(const PdArgs& args);
    void onAny(t_symbol* s, const AtomList& args);
};

class MidiEventNote : public BaseObject {
    XMidiEvent ev_;
    AtomList note_;

public:
    MidiEventNote(const PdArgs& args);
    void onAny(t_symbol* s, const AtomList& args);
};

void setup_midi_event();

#endif // MIDI_EVENT_H
