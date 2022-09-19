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
    /**
     * Creates empty midi stream
     */
    DataTypeMidiStream();
    explicit DataTypeMidiStream(const MidiFile& midifile);

    /**
     * Creates midi stream from midifile
     */
    explicit DataTypeMidiStream(const char* fname);

    DataTypeMidiStream(const DataTypeMidiStream& s);
    ~DataTypeMidiStream();

    DataTypeMidiStream& operator=(const DataTypeMidiStream& ms);
    DataTypeMidiStream& operator=(DataTypeMidiStream&& ms);

    /**
     * Number of tracks in midifile
     */
    size_t trackCount() const;
    size_t tempo() const;

    /**
     * Filename (not full path)
     */
    t_symbol* filename() const;

    /**
     * Polymorphic data copy
     */
    DataTypeMidiStream* clone() const;

    /**
     * Polymorphics data type
     */
    DataTypeId type() const noexcept;

    double totalTimeInQuarters() const;
    int totalTimeInTicks() const;
    double totalTimeInSeconds() const;

    /**
     * Pointer to MidiFile struct
     */
    MidiFile* midifile();
    const MidiFile* midifile() const;

    /**
     * Checks if midi stream successfully opened
     */
    bool is_open() const;

    bool set(const AbstractData* d) noexcept final;
    std::string toListStringContent() const final;
    std::string toDictStringContent() const final;

public:
    static const DataTypeId dataType;

private:
    void calcTime();
};

#endif // DATATYPE_MIDISTREAM_H
