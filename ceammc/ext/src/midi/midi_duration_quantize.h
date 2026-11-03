#ifndef MIDI_DURATION_QUANTIZE_H
#define MIDI_DURATION_QUANTIZE_H

#include "ceammc_object.h"

#include <cstdint>
#include <limits>

namespace ceammc {

class MidiDurationQuantize : public BaseObject {
public:
    struct MidiNote {
        std::int8_t pitch { -1 };
        std::int8_t velocity { -1 };
        bool is_valid() const { return pitch >= 0 && velocity >= 0; }
        bool is_off() const { return velocity == 0; }
    };

    enum class MidiNoteState : std::uint8_t {
        Init,
        On,
        Duration
    };

    enum class NoteMeasure {
        Add,
        Remove,
        None
    };

    struct MidiOnOff {
        double value;
        MidiNoteState state { MidiNoteState::Init };
        NoteMeasure note_on();
        NoteMeasure note_off();
        bool get_duration_ms(double& msec) const;
        bool has_duration() const { return state == MidiNoteState::Duration; }
    };

public:
    MidiDurationQuantize(const PdArgs& args);

    void onList(const AtomListView& lv) final;

    void quantize(std::int8_t idx);

public:
    static MidiNote parse_midi_msg(const AtomListView& lv);

private:
    MidiOnOff note_times_[std::numeric_limits<std::int8_t>::max()];
    size_t note_stat_counter_ { 0 };
};

} // namespace ceammc

void setup_midi_duration_quantize();

#endif // MIDI_DURATION_QUANTIZE_H
