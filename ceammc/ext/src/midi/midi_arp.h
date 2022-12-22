#ifndef MIDI_ARP_H
#define MIDI_ARP_H

#include "ceammc_clock.h"
#include "ceammc_object.h"
using namespace ceammc;

#include <bitset>
#include <cstdint>

#include <boost/container/static_vector.hpp>

class MidiArp : public BaseObject {
    boost::container::static_vector<std::uint8_t, 127> notes_;
    BoolProperty* external_clock_;
    FloatProperty* delay_;
    IntProperty* min_notes_;
    BoolProperty* pass_;
    ClockLambdaFunction clock_;
    int cur_note_idx_;
    int prev_note_;

    enum MidiArpEvent {
        NOTE_ON,
        NOTE_OFF,
        PLAY_NOTE,
    };

    enum MidiArpState {
        STATE_EMPTY,
        STATE_NOARP,
        STATE_ARP,
    };

    MidiArpState state_;

public:
    MidiArp(const PdArgs& args);
    ~MidiArp();

    void onList(const AtomListView& lv) override;
    void onInlet(size_t n, const AtomListView& lv) override;

public:
    void onEvent(MidiArpEvent ev, std::uint8_t note);
    void playNote();
    void sendNote(std::uint8_t note, std::uint8_t vel);
    void addNote(std::uint8_t note, std::uint8_t vel);
    void removeNote(std::uint8_t note);
};

void setup_midi_arp();

#endif // MIDI_ARP_H
