#ifndef MIDI_TRACK_H
#define MIDI_TRACK_H

#include "ceammc_object.h"
#include "datatype_midistream.h"
#include "datatype_miditrack.h"

using namespace ceammc;

enum PlayState {
    PLAY_STATE_STOPPED = 0,
    PLAY_STATE_PLAYING = 1,
    PLAY_STATE_PAUSED = 2
};

class MidiTrack : public BaseObject {
    DataTypeMidiTrack midi_track_;
    FlagProperty* join_;
    SizeTProperty* track_idx_;
    IntProperty* tempo_;
    size_t current_event_idx_;
    // used with reserved memory
    // to avoid allocation on every outputEvent() call
    AtomList current_event_;
    t_clock* clock_;
    PlayState play_state_;

public:
    typedef DataTypeMidiTrack::iterator MidiEventIterator;
    typedef DataTypeMidiTrack::const_iterator MidiEventConstIterator;

public:
    MidiTrack(const PdArgs& args);
    ~MidiTrack();

    void onBang();

    void onDataT(const DataTypeMidiStream& s);

    AtomList p_events() const;
    AtomList p_current() const;

    void m_next(t_symbol*, const AtomList&);
    void m_reset(t_symbol*, const AtomList&);
    void m_output(t_symbol*, const AtomList&);
    void m_seek(t_symbol*, const AtomList& l);
    void m_play(t_symbol*, const AtomList&);
    void m_stop(t_symbol*, const AtomList&);

    void outputEvent(MidiEvent* ev);

    /**
     * find next tick event after specified event
     * @param ev - event iterator
     * @return next event iterator or end() iterator if next tick is not found
     */
    MidiEventIterator findNextTick(MidiEventIterator ev);
    MidiEventConstIterator findNextTick(MidiEventConstIterator ev) const;

    /**
     * Find event by tick index (not tick time!) in track
     * @param tickIdx tick index from begining of MIDI track
     * @return event iterator or end() iterator if tick index is not found
     */
    MidiEventIterator findEventAt(size_t tickIndex);
    MidiEventConstIterator findEventAt(size_t tickIndex) const;

    /**
     * Find event index (not tick index) of next tick event
     * @param idx
     */
    size_t findNextTickEventIndex(size_t idx) const;

    MidiEventIterator begin() { return midi_track_.begin(); }
    MidiEventIterator end() { return midi_track_.end(); }
    MidiEventConstIterator begin() const { return midi_track_.begin(); }
    MidiEventConstIterator end() const { return midi_track_.end(); }
    size_t size() const { return midi_track_.eventCount(); }

    /**
     * Moves current track event to given tick index from begining of track
     * @param pos - tick index
     * @return true on success, false if invalid tick index is given
     */
    bool seekAbs(size_t tickIndex);

private:
    int currentTick() const;

    /**
     * Returns duration until next tick in milliseconds
     */
    double outputCurrent();

    static void clockTickHandler(MidiTrack* p);
    void clockTick();
};

void setup_midi_track();

#endif // MIDI_TRACK_H
