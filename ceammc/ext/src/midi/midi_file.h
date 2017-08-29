#ifndef MIDI_FILE_H
#define MIDI_FILE_H

#include "ceammc_data.h"
#include "ceammc_object.h"
#include "datatype_midistream.h"

using namespace ceammc;

class XMidiFile : public BaseObject {
    DataTPtr<DataTypeMidiStream> midi_stream_;

public:
    XMidiFile(const PdArgs& a);

    void onBang();

    void m_clear(t_symbol*, const AtomList&);
    void m_info(t_symbol*, const AtomList&);
    void m_read(t_symbol*, const AtomList& l);
    void m_write(t_symbol*, const AtomList& l);

    AtomList p_filename() const;
    AtomList p_tempo() const;
    AtomList p_tracks() const;

    void onDataT(const DataTypeMidiStream& data);
};

extern "C" void setup_midi_file();

#endif // MIDI_FILE_H
