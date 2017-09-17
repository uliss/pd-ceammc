#ifndef MIDI_KEY2STR_H
#define MIDI_KEY2STR_H

#include "ceammc_music_theory.h"
#include "ceammc_object.h"
#include <boost/array.hpp>

using namespace ceammc;

class MidiKey2Str : public BaseObject {
    music::Tonality tonality_;

public:
    MidiKey2Str(const PdArgs& args);
    void onFloat(float f);

    AtomList p_tonality() const;
    void p_setTonality(const AtomList& l);

private:
    bool isMajor() const;
    t_symbol* cache_[12];
};

void setup_midi_key2str();

#endif // MIDI_KEY2STR_H
