#ifndef MIDI_EVENT_H
#define MIDI_EVENT_H

#include "ceammc_object.h"

#include <array>
#include <memory>

using namespace ceammc;

class MidiEvent;

class XMidiEvent {
    std::unique_ptr<MidiEvent> event_;
    t_float duration_;
    int track_;
    bool valid_;

    XMidiEvent(XMidiEvent&) = delete;

public:
    explicit XMidiEvent(const AtomListView& lv);
    ~XMidiEvent();

    bool parse(const AtomListView& lv);
    bool isNote() const;
    bool isNoteOn() const;
    bool isNoteOff() const;
    bool isProgramChange() const;
    bool isControl() const;
    bool isValid() const;
    t_float duration() const;
    MidiEvent& event();
    int track() const;
};

class BaseMidiEventExternal : public BaseObject {
protected:
    XMidiEvent event_;

public:
    BaseMidiEventExternal(const PdArgs& a);

    void onAny(t_symbol* s, const AtomListView& args) override;
    virtual void processEvent() = 0;
};

class MidiEventToNote : public BaseMidiEventExternal {
    std::array<Atom, 2> msg_;

public:
    MidiEventToNote(const PdArgs& args);
    void processEvent();
};

class MidiEventToPrg : public BaseMidiEventExternal {
public:
    MidiEventToPrg(const PdArgs& args);
    void processEvent();
};

void setup_midi_event();

#endif // MIDI_EVENT_H
