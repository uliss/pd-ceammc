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
    typedef DataTypeMidiTrack::iterator MidiEventIterator;
    typedef DataTypeMidiTrack::const_iterator MidiEventConstIterator;

public:
    MidiTrack(const PdArgs& args);

    void onBang();

    void onDataT(const DataTypeMidiStream& s);

    AtomList p_events() const;
    void m_next(t_symbol*, const AtomList&);
    void m_reset(t_symbol*, const AtomList&);
    void m_output(t_symbol*, const AtomList&);

    void outputEvent(MidiEvent* ev);

    /**
     * find next event after specified event index
     * @param ev - event iterator
     * @return next event iterator or end() iterator if not found
     */
    MidiEventIterator findNextEvent(MidiEventIterator ev);
    MidiEventConstIterator findNextEvent(MidiEventConstIterator ev) const;
    size_t findNextEventIndex(size_t idx) const;

    MidiEventIterator begin() { return midi_track_.begin(); }
    MidiEventIterator end() { return midi_track_.end(); }
    MidiEventConstIterator begin() const { return midi_track_.begin(); }
    MidiEventConstIterator end() const { return midi_track_.end(); }
    size_t size() const { return midi_track_.eventCount(); }

private:
    int currentTick() const;
    void outputCurrent();
};

void setup_midi_track();

#endif // MIDI_TRACK_H
