#ifndef CONV_PITCH2MIDI_H
#define CONV_PITCH2MIDI_H

#include "ceammc_data.h"
#include "ceammc_object.h"

using namespace ceammc;

class PitchToMIDI : public BaseObject {
public:
    PitchToMIDI(const PdArgs& a);

    void onSymbol(t_symbol* s) override;
    void onList(const AtomList& lst) override;
    void onDataT(const StringAtom& s);
    void onDataT(const MListAtom& ml);

private:
    void convert(const char* p);
};

void setup_conv_pitch2midi();

#endif // CONV_PITCH2MIDI_H
