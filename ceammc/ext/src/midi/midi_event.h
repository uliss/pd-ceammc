#ifndef MIDI_EVENT_H
#define MIDI_EVENT_H

#include "ceammc_object.h"

#include <boost/shared_ptr.hpp>

using namespace ceammc;

class MidiEvent;

class XMidiEvent {
    bool valid_;
    AtomList raw_;
    boost::shared_ptr<MidiEvent> event_;
    t_float duration_;
    int track_;

    XMidiEvent(XMidiEvent&);

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

class MidiEventToNote : public BaseObject {
    XMidiEvent ev_;
    AtomList note_;

public:
    MidiEventToNote(const PdArgs& args);
    void onAny(t_symbol* s, const AtomList& args);
};

void setup_midi_event();

#endif // MIDI_EVENT_H
