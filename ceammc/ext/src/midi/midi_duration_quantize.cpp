#include "midi_duration_quantize.h"
#include "ceammc_factory.h"

using namespace ceammc;

namespace {
bool in_range(t_float v)
{
    constexpr t_float MIDI_MIN = 0.0;
    constexpr t_float MIDI_MAX = 127.0;

    return MIDI_MIN <= v && v <= MIDI_MAX;
}
}

MidiDurationQuantize::MidiDurationQuantize(const PdArgs& args)
    : BaseObject(args)
{
    createOutlet();
}

MidiDurationQuantize::MidiNote MidiDurationQuantize::parse_midi_msg(const AtomListView& lv)
{
    if (lv.size() == 2
        && lv[0].isFloat()
        && lv[1].isFloat()) {
        auto pitch = lv[0].asT<t_float>();
        auto vel = lv[1].asT<t_float>();

        if (in_range(pitch) && in_range(vel))
            return MidiNote {
                static_cast<std::int8_t>(pitch),
                static_cast<std::int8_t>(vel),
            };
    }

    return MidiNote();
}

void MidiDurationQuantize::onList(const AtomListView& lv)
{
    auto note = parse_midi_msg(lv);
    if (!note.is_valid())
        return;

    auto& info = note_times_[note.pitch];
    if (note.is_off()) {
        if (info.note_off() == NoteMeasure::Add) {
            note_stat_counter_++;
            quantize(note.pitch);
        }

    } else if (info.note_on() == NoteMeasure::Remove)
        note_stat_counter_--;
}

void MidiDurationQuantize::quantize(std::int8_t idx)
{
    auto& info = note_times_[idx];
    if (info.has_duration())
        note_stat_counter_++;
}

void setup_midi_duration_quantize()
{
    ObjectFactory<MidiDurationQuantize> obj("midi.duration.quantize");
}

MidiDurationQuantize::NoteMeasure MidiDurationQuantize::MidiOnOff::note_on()
{
    switch (state) {
    case MidiNoteState::Init:
        value = clock_getlogicaltime();
        state = MidiNoteState::On;
        break;
    case MidiNoteState::On: // update on time
        value = clock_getlogicaltime();
        break;
    case MidiNoteState::Duration: // start new measure
        value = clock_getlogicaltime();
        state = MidiNoteState::On;
        return NoteMeasure::Remove;
    default:
        break;
    }

    return NoteMeasure::None;
}

MidiDurationQuantize::NoteMeasure MidiDurationQuantize::MidiOnOff::note_off()
{
    switch (state) {
    case MidiNoteState::Init: // ignore
        break;
    case MidiNoteState::On:
        value = clock_gettimesince(value);
        state = MidiNoteState::Duration;
        return NoteMeasure::Add;
    case MidiNoteState::Duration: // ignore
    default:
        break;
    }

    return NoteMeasure::None;
}

bool MidiDurationQuantize::MidiOnOff::get_duration_ms(double& msec) const
{
    if (state == MidiNoteState::Duration) {
        msec = value;
        return true;
    } else
        return false;
}
