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
{
    LIB_DBG << "MidiStream created";
}

DataTypeMidiStream::DataTypeMidiStream(const MidiFile& midifile)
    : midi_file_(new MidiFile(midifile))
    , total_ticks_(0)
    , total_secs_(0)
    , total_quarters_(0)
{
    LIB_DBG << "MidiStream created";
    calcTime();
}

DataTypeMidiStream::DataTypeMidiStream(const char* fname)
    : midi_file_(new MidiFile(fname))
    , total_ticks_(0)
    , total_secs_(0)
    , total_quarters_(0)
{
    LIB_DBG << "MidiStream created: " << fname;
    calcTime();
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

void DataTypeMidiStream::calcTime()
{
    bool splitted = midi_file_->hasSplitTracks();
    if (splitted)
        midi_file_->joinTracks();

    if (midi_file_->getEventCount(0) > 0) {
        total_ticks_ = midi_file_->getTotalTimeInTicks();
        total_secs_ = midi_file_->getTotalTimeInSeconds();
        total_quarters_ = midi_file_->getTotalTimeInQuarters();
    } else {
        total_ticks_ = 0;
        total_secs_ = 0;
        total_quarters_ = 0;
    }

    if (splitted)
        midi_file_->splitTracks();
}
