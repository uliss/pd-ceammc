#include "datatype_midistream.h"
#include "MidiFile.h"
#include "ceammc_datatypes.h"
#include "ceammc_log.h"

const DataType DataTypeMidiStream::dataType = ceammc::data::DATA_MIDI_STREAM;

DataTypeMidiStream::DataTypeMidiStream()
    : midi_file_(new MidiFile())
    , total_ticks_(0)
    , total_secs_(0)
    , total_quarters_(0)
    , is_open_(false)
{
}

DataTypeMidiStream::DataTypeMidiStream(const MidiFile& midifile)
    : midi_file_(new MidiFile(midifile))
    , total_ticks_(0)
    , total_secs_(0)
    , total_quarters_(0)
    , is_open_(midi_file_->status())
{
    calcTime();
}

DataTypeMidiStream::DataTypeMidiStream(const char* fname)
    : midi_file_(new MidiFile(fname))
    , total_ticks_(0)
    , total_secs_(0)
    , total_quarters_(0)
    , is_open_(midi_file_->status() != 0)
{
    if (!is_open_)
        midi_file_->setFilename("");

    calcTime();
}

DataTypeMidiStream::~DataTypeMidiStream()
{
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
    return gensym(midi_file_->getFilename().c_str());
}

DataTypeMidiStream* DataTypeMidiStream::clone() const
{
    return new DataTypeMidiStream(*this);
}

DataType DataTypeMidiStream::type() const
{
    return data::DATA_MIDI_STREAM;
}

double DataTypeMidiStream::totalTimeInQuarters() const
{
    return total_quarters_;
}

int DataTypeMidiStream::totalTimeInTicks() const
{
    return total_ticks_;
}

double DataTypeMidiStream::totalTimeInSeconds() const
{
    return total_secs_;
}

MidiFile* DataTypeMidiStream::midifile()
{
    return midi_file_.get();
}

const MidiFile* DataTypeMidiStream::midifile() const
{
    return midi_file_.get();
}

bool DataTypeMidiStream::is_open() const
{
    return is_open_;
}

void DataTypeMidiStream::calcTime()
{
    total_ticks_ = midi_file_->getTotalTimeInTicks();
    total_secs_ = midi_file_->getTotalTimeInSeconds();
    total_quarters_ = midi_file_->getTotalTimeInQuarters();
}
