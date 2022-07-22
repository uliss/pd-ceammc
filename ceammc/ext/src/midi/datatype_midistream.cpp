#include "datatype_midistream.h"
#include "ceammc_datastorage.h"
#include "ceammc_datatypes.h"
#include "ceammc_log.h"

#include "MidiFile.h"

const DataTypeId DataTypeMidiStream::dataType = DataStorage::instance().registerNewType("MidiStream");

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

DataTypeMidiStream::DataTypeMidiStream(const DataTypeMidiStream& s)
    : midi_file_(new MidiFile(*s.midi_file_))
    , total_ticks_(s.total_ticks_)
    , total_secs_(s.total_secs_)
    , total_quarters_(s.total_quarters_)
    , is_open_(s.is_open_)
{
}

DataTypeMidiStream& DataTypeMidiStream::operator=(const DataTypeMidiStream& ms)
{
    midi_file_.reset(new MidiFile(*ms.midi_file_));
    total_ticks_ = ms.total_ticks_;
    total_secs_ = ms.total_secs_;
    total_quarters_ = ms.total_quarters_;
    is_open_ = ms.is_open_;
    return *this;
}

DataTypeMidiStream& DataTypeMidiStream::operator=(DataTypeMidiStream&& ms)
{
    midi_file_ = std::move(ms.midi_file_);
    total_ticks_ = std::move(ms.total_ticks_);
    total_secs_ = std::move(ms.total_secs_);
    total_quarters_ = std::move(ms.total_quarters_);
    is_open_ = std::move(ms.is_open_);
    return *this;
}

DataTypeMidiStream::~DataTypeMidiStream() = default;

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

DataTypeId DataTypeMidiStream::type() const noexcept
{
    return dataType;
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

bool DataTypeMidiStream::set(const AbstractData* d) noexcept
{
    return setDataT<DataTypeMidiStream>(d);
}

std::string DataTypeMidiStream::toListStringContent() const
{
    return " ";
}

std::string DataTypeMidiStream::toDictStringContent() const
{
    return " ";
}

void DataTypeMidiStream::calcTime()
{
    total_ticks_ = midi_file_->getTotalTimeInTicks();
    total_secs_ = midi_file_->getTotalTimeInSeconds();
    total_quarters_ = midi_file_->getTotalTimeInQuarters();
}
