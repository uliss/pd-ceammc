#ifndef DATATYPE_MIDISTREAM_H
#define DATATYPE_MIDISTREAM_H

#include "ceammc_abstractdata.h"

#include <memory>

class MidiFile;
using namespace ceammc;

class DataTypeMidiStream : public AbstractData {
    std::unique_ptr<MidiFile> midi_file_;
    int total_ticks_;
    double total_secs_;
    double total_quarters_;
    bool is_open_;

public:
    DataTypeMidiStream();
    explicit DataTypeMidiStream(const MidiFile& midifile);
    explicit DataTypeMidiStream(const char* fname);

    DataTypeMidiStream(const DataTypeMidiStream& s);
    ~DataTypeMidiStream();

    size_t trackCount() const;
    size_t tempo() const;
    t_symbol* filename() const;

    DataTypeMidiStream* clone() const;
    int type() const noexcept;

    double totalTimeInQuarters() const;
    int totalTimeInTicks() const;
    double totalTimeInSeconds() const;

    MidiFile* midifile();
    const MidiFile* midifile() const;

    /**
     * Checks if midi stream successfully opened
     */
    bool is_open() const;

public:
    static const int dataType;

private:
    void calcTime();
};

#endif // DATATYPE_MIDISTREAM_H
