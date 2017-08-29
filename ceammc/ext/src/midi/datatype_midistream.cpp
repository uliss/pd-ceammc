#include "datatype_midistream.h"
#include "MidiFile.h"
#include "ceammc_datatypes.h"
#include "ceammc_log.h"

const DataType DataTypeMidiStream::dataType = ceammc::data::DATA_MIDI_STREAM;

DataTypeMidiStream::DataTypeMidiStream()
    : midi_file_(new MidiFile())
{
    LIB_DBG << "MidiStream created";
}

DataTypeMidiStream::DataTypeMidiStream(const MidiFile& midifile)
    : midi_file_(new MidiFile(midifile))
{
    LIB_DBG << "MidiStream created";
}

DataTypeMidiStream::DataTypeMidiStream(const char* fname)
    : midi_file_(new MidiFile(fname))
{
    LIB_DBG << "MidiStream created: " << fname;
}

DataTypeMidiStream::~DataTypeMidiStream()
{
    LIB_DBG << "MidiStream destroyed";
}

size_t DataTypeMidiStream::trackCount() const
{
    return midi_file_->getTrackCount();
}

size_t DataTypeMidiStream::tempo() const
{
    return midi_file_->getTicksPerQuarterNote();
}

t_symbol* DataTypeMidiStream::filename() const
{
    return gensym(midi_file_->getFilename());
}

DataTypeMidiStream* DataTypeMidiStream::clone() const
{
    return new DataTypeMidiStream(*this);
}

DataType DataTypeMidiStream::type() const
{
    return data::DATA_MIDI_STREAM;
}

const MidiFile& DataTypeMidiStream::midifile() const
{
    return *midi_file_.get();
}
