#ifndef CONV_PITCH2MIDI_H
#define CONV_PITCH2MIDI_H

#include "ceammc_object.h"
using namespace ceammc;

class DataTypeString;

class PitchToMIDI : public BaseObject {
public:
    PitchToMIDI(const PdArgs& a);

    void onSymbol(t_symbol* s);
    void onList(const AtomList& lst);
    void onDataT(const DataTypeString& s);

private:
    void convert(const char* p);

public:
    static int spn2midi(const char* p);
};

extern "C" void setup_conv0x2epitch2midi();

#endif // CONV_PITCH2MIDI_H
