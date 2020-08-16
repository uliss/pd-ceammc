#ifndef MIDI_FILE_H
#define MIDI_FILE_H

#include "ceammc_data.h"
#include "ceammc_object.h"
#include "datatype_midistream.h"

using namespace ceammc;

using MidiStreamAtom = DataAtom<DataTypeMidiStream>;

class XMidiFile : public BaseObject {
    MidiStreamAtom midi_stream_;

public:
    XMidiFile(const PdArgs& a);

    void onBang() override;

    void m_clear(t_symbol*, const AtomListView&);
    void m_info(t_symbol*, const AtomListView&);
    void m_read(t_symbol*, const AtomListView& l);
    void m_write(t_symbol*, const AtomListView& l);

    void onDataT(const MidiStreamAtom& data);

private:
    bool open(const char* path);
};

void setup_midi_file();

#endif // MIDI_FILE_H
