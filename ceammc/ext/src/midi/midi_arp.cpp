#include "midi_arp.h"
#include "ceammc_factory.h"

MidiArp::MidiArp(const PdArgs& args)
    : BaseObject(args)
{
    createOutlet();
}

void stup_midi_arp()
{
    ObjectFactory<MidiArp> obj("midi.arp");
}
