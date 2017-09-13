#ifndef DATATYPE_MIDISTREAM_H
#define DATATYPE_MIDISTREAM_H

#include "ceammc_abstractdata.h"

#include <boost/shared_ptr.hpp>

class MidiFile;
using namespace ceammc;

class DataTypeMidiStream : public AbstractData {
    boost::shared_ptr<MidiFile> midi_file_;
    int total_ticks_;
    double total_secs_;
    double total_quarters_;

public:
    DataTypeMidiStream();
    DataTypeMidiStream(const MidiFile& midifile);
    DataTypeMidiStream(const char* fname);
    ~DataTypeMidiStream();
    size_t trackCount() const;
    size_t tempo() const;
    t_symbol* filename() const;

    DataTypeMidiStream* clone() const;
    DataType type() const;

    double totalTimeInQuarters() const;
    int totalTimeInTicks() const;
    double totalTimeInSeconds() const;

    MidiFile* midifile();
    const MidiFile* midifile() const;

public:
    static const DataType dataType;

private:
    void calcTime();
};

#endif // DATATYPE_MIDISTREAM_H
