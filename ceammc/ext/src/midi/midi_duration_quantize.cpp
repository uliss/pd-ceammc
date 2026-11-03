#include "midi_duration_quantize.h"
#include "ceammc_factory.h"

using namespace ceammc;

MidiDurationQuantize::MidiDurationQuantize(const PdArgs& args)
    : BaseObject(args)
{
    createOutlet();
}

void MidiDurationQuantize::onList(const AtomListView& lv)
{
}

void setup_midi_duration_quantize()
{
    ObjectFactory<MidiDurationQuantize> obj("midi.duration.quantize");
}
