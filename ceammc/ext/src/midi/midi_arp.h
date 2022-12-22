#ifndef MIDI_ARP_H
#define MIDI_ARP_H

#include "ceammc_object.h"
using namespace ceammc;

class MidiArp : public BaseObject
{
public:
    MidiArp(const PdArgs& args);
};

void setup_midi_arp();

#endif // MIDI_ARP_H
