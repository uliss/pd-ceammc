#include "midi_key2str.h"
#include "ceammc_factory.h"

MidiKey2Str::MidiKey2Str(const PdArgs& args)
    : BaseObject(args)
{
    createOutlet();
}

void MidiKey2Str::onFloat(float f)
{
}

void setup_midi_key2str()
{
    ObjectFactory<MidiKey2Str> obj("midi.key2str");
    obj.addAlias("midi.key->str");
}
