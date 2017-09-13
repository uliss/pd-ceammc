#ifndef DATATYPE_MIDITRACK_H
#define DATATYPE_MIDITRACK_H

#include "ceammc_abstractdata.h"

#include <boost/shared_ptr.hpp>
#include <vector>

using namespace ceammc;

class MidiEvent;
class MidiEventList;

class DataTypeMidiTrack : public AbstractData {
    boost::shared_ptr<MidiEventList> events_;

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
    typedef std::vector<MidiEvent*>::iterator iterator;
    typedef std::vector<MidiEvent*>::const_iterator const_iterator;

    iterator begin();
    iterator end();

    const_iterator begin() const;
    const_iterator end() const;

public:
    static const DataType dataType;

private:
    DataTypeMidiTrack(const DataTypeMidiTrack& dt);
};

#endif // DATATYPE_MIDITRACK_H
