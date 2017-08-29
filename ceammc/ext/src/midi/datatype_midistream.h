#ifndef DATATYPE_MIDISTREAM_H
#define DATATYPE_MIDISTREAM_H

#include "MidiFile.h"
#include "ceammc_abstractdata.h"

#include <memory>

class MidiFile;
using namespace ceammc;

class DataTypeMidiStream : public AbstractData {
    std::shared_ptr<MidiFile> midi_file_;

public:
    DataTypeMidiStream();
    DataTypeMidiStream(const MidiFile& midifile);
    DataTypeMidiStream(const char* fname);
    ~DataTypeMidiStream();
    size_t trackCount() const;
    size_t tempo() const;
    t_symbol* filename() const;

    DataTypeMidiStream* clone() const override;
    DataType type() const override;

    const MidiFile& midifile() const;

public:
    static const DataType dataType;
};

#endif // DATATYPE_MIDISTREAM_H
