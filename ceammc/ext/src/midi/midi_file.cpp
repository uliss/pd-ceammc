#include "midi_file.h"
#include "ceammc_factory.h"

#include "MidiFile.h"

XMidiFile::XMidiFile(const PdArgs& a)
    : BaseObject(a)
{
    createOutlet();
}

void setup_midi_file()
{
    ObjectFactory<XMidiFile> f("midi.file");
}
