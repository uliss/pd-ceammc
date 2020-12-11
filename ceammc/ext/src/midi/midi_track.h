#ifndef MIDI_TRACK_H
#define MIDI_TRACK_H

#include "ceammc_data.h"
#include "ceammc_object.h"
#include "datatype_midistream.h"
#include "datatype_miditrack.h"

using namespace ceammc;

enum PlayState {
    PLAY_STATE_STOPPED = 0,
    PLAY_STATE_PLAYING = 1,
    PLAY_STATE_PAUSED = 2
};

using MidiStreamAtom = DataAtom<DataTypeMidiStream>;

class MidiTrack : public BaseObject {
    DataTypeMidiTrack midi_track_;
    FlagProperty* join_;
    IntProperty* track_idx_;
    IntProperty* tempo_;
    FloatProperty* speed_;
    size_t current_event_idx_;
    // used with reserved memory
    // to avoid allocation on every outputEvent() call
    AtomList current_event_;
    t_clock* clock_;
    PlayState play_state_;

public:
    using MidiEventIterator = DataTypeMidiTrack::iterator;
    using MidiEventConstIterator = DataTypeMidiTrack::const_iterator;

public:
    MidiTrack(const PdArgs& args);
    ~MidiTrack();

    void onBang() override;

    void onDataT(const MidiStreamAtom& stream);

    void m_next(t_symbol*, const AtomListView&);
    void m_reset(t_symbol*, const AtomListView&);
    void m_seek(t_symbol*, const AtomListView& l);
    void m_play(t_symbol*, const AtomListView&);
    void m_stop(t_symbol*, const AtomListView&);
    void m_pause(t_symbol*, const AtomListView&);

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
