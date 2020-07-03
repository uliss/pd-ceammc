#ifndef MIDI_KEY2STR_H
#define MIDI_KEY2STR_H

#include "ceammc_music_theory.h"
#include "ceammc_object.h"

using namespace ceammc;

class MidiKey2Str : public BaseObject {
    music::Tonality tonality_;
    FlagProperty* as_symbol_;

public:
    MidiKey2Str(const PdArgs& args);
    void onFloat(t_float f) override;

    AtomList p_tonality() const;
    bool p_setTonality(const AtomList& l);

private:
    bool isMajor() const;
    t_symbol* cache_[12];
    void clearCache();
};

void setup_midi_key2str();

#endif // MIDI_KEY2STR_H
