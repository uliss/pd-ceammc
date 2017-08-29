#ifndef MIDI_TRACK_H
#define MIDI_TRACK_H

#include "ceammc_object.h"
#include "datatype_midistream.h"
#include "datatype_miditrack.h"

using namespace ceammc;

class MidiTrack : public BaseObject {
    DataTypeMidiTrack midi_track_;
    FlagProperty* join_;
    SizeTProperty* track_idx_;
    IntProperty* tempo_;
    size_t current_event_idx_;
    // used with reserved memory
    // to avoid allocation on every outputEvent() call
    AtomList current_event_;

public:
    MidiTrack(const PdArgs& args);

    void onBang();

    void onDataT(const DataTypeMidiStream& s);

    AtomList p_events() const;
    void m_next(t_symbol*, const AtomList&);
    void m_reset(t_symbol*, const AtomList&);
    void m_output(t_symbol*, const AtomList&);

private:
    void outputEvent(MidiEvent* ev);
    int currentTick();
    void outputCurrent();
};

void setup_midi_track();

#endif // MIDI_TRACK_H
