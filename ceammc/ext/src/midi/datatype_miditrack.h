#ifndef DATATYPE_MIDITRACK_H
#define DATATYPE_MIDITRACK_H

#include "ceammc_abstractdata.h"

#include <memory>
#include <vector>

using namespace ceammc;

class MidiEvent;
class MidiEventList;

class DataTypeMidiTrack : public AbstractData {
    // pimpl
    std::unique_ptr<MidiEventList> events_;

public:
    DataTypeMidiTrack();
    DataTypeMidiTrack(const DataTypeMidiTrack& mt);
    DataTypeMidiTrack(const MidiEventList& lst);

    DataTypeMidiTrack& operator=(const DataTypeMidiTrack& mt);

    ~DataTypeMidiTrack();

    /**
     * Polymorphic copy creation
     */
    DataTypeMidiTrack* clone() const;

    /**
     * Polymorphic data type
     */
    DataTypeId type() const noexcept;

    size_t eventCount() const;
    MidiEventList& events();
    MidiEvent* eventAt(size_t n);
    const MidiEvent* eventAt(size_t n) const;

    void setEventList(const MidiEventList& lst);

    /**
     * Polymorphic set function
     */
    bool set(const AbstractData* d) noexcept final;
    std::string toListStringContent() const final;
    std::string toDictStringContent() const final;

public:
    using iterator = std::vector<MidiEvent*>::iterator;
    using const_iterator = std::vector<MidiEvent*>::const_iterator;

    // iterators
    iterator begin();
    iterator end();
    const_iterator begin() const;
    const_iterator end() const;

public:
    static const DataTypeId dataType;
};

#endif // DATATYPE_MIDITRACK_H
