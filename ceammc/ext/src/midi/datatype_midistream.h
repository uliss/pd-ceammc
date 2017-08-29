#ifndef DATATYPE_MIDISTREAM_H
#define DATATYPE_MIDISTREAM_H

#include "ceammc_abstractdata.h"

#include <boost/shared_ptr.hpp>

class MidiFile;
using namespace ceammc;

class DataTypeMidiStream : public AbstractData {
    boost::shared_ptr<MidiFile> midi_file_;

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

    const MidiFile& midifile() const;

public:
    static const DataType dataType;
};

#endif // DATATYPE_MIDISTREAM_H
