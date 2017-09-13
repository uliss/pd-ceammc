#include "datatype_miditrack.h"
#include "MidiEventList.h"
#include "ceammc_datatypes.h"

#include <boost/make_shared.hpp>

const DataType DataTypeMidiTrack::dataType = data::DATA_MIDI_TRACK;

DataTypeMidiTrack::DataTypeMidiTrack()
    : events_(boost::make_shared<MidiEventList>())
{
}

DataTypeMidiTrack::DataTypeMidiTrack(const MidiEventList& lst)
    : events_(new MidiEventList(lst))
{
    events_->linkNotePairs();
}

DataTypeMidiTrack* DataTypeMidiTrack::clone() const
{
    return new DataTypeMidiTrack(*this);
}

DataType DataTypeMidiTrack::type() const
{
    return data::DATA_MIDI_TRACK;
}

size_t DataTypeMidiTrack::eventCount() const
{
    return events_->getSize();
}

MidiEventList& DataTypeMidiTrack::events()
{
    return *events_;
}

MidiEvent* DataTypeMidiTrack::eventAt(size_t n)
{
    return &events_->operator[](n);
}

const MidiEvent* DataTypeMidiTrack::eventAt(size_t n) const
{
    return &events_->operator[](n);
}

DataTypeMidiTrack::iterator DataTypeMidiTrack::begin()
{
    return events_->begin();
}

DataTypeMidiTrack::iterator DataTypeMidiTrack::end()
{
    return events_->end();
}

DataTypeMidiTrack::const_iterator DataTypeMidiTrack::begin() const
{
    return events_->begin();
}

DataTypeMidiTrack::const_iterator DataTypeMidiTrack::end() const
{
    return events_->end();
}

DataTypeMidiTrack::DataTypeMidiTrack(const DataTypeMidiTrack& dt)
    : events_(new MidiEventList(*dt.events_))
{
}
