#ifndef MIDI_DURATION_QUANTIZE_H
#define MIDI_DURATION_QUANTIZE_H

#include "ceammc_object.h"

namespace ceammc {

class MidiDurationQuantize : public BaseObject {
public:
    MidiDurationQuantize(const PdArgs& args);

    void onList(const AtomListView& lv) final;
};

} // namespace ceammc

void setup_midi_duration_quantize();

#endif // MIDI_DURATION_QUANTIZE_H
