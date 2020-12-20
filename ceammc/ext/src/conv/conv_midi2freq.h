#ifndef CONV_MIDI2FREQ_H
#define CONV_MIDI2FREQ_H

#include "ceammc_object.h"
#include "ceammc_property_enum.h"

using namespace ceammc;

class Midi2Freq : public BaseObject {
    FloatProperty* base_a_;
    SymbolEnumProperty* prop_temperament_;

public:
    Midi2Freq(const PdArgs& args);
    void onFloat(t_float f) override;
    void onList(const AtomList& lst) override;
};

void setup_conv_midi2freq();

#endif // CONV_MIDI2FREQ_H
