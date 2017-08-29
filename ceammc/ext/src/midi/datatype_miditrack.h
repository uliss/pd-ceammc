#ifndef DATATYPE_MIDITRACK_H
#define DATATYPE_MIDITRACK_H

#include "MidiEventList.h"
#include "ceammc_abstractdata.h"

using namespace ceammc;

class DataTypeMidiTrack : public AbstractData {
    MidiEventList events_;

public:
    DataTypeMidiTrack();
    DataTypeMidiTrack(const MidiEventList& lst);

    DataTypeMidiTrack* clone() const;
    DataType type() const;

    size_t eventCount() const;
    MidiEventList& events();
    MidiEvent* eventAt(size_t n);

public:
    static const DataType dataType;
};

#endif // DATATYPE_MIDITRACK_H
