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
    const MidiEvent* eventAt(size_t n) const;

public:
    typedef MidiEventList::iterator iterator;
    typedef MidiEventList::const_iterator const_iterator;

    iterator begin();
    iterator end();

    const_iterator begin() const;
    const_iterator end() const;

public:
    static const DataType dataType;
};

#endif // DATATYPE_MIDITRACK_H
