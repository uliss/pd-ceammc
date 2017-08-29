#include "midi_file.h"
#include "ceammc_factory.h"

#include "MidiFile.h"

XMidiFile::XMidiFile(const PdArgs& a)
    : BaseObject(a)
    , midi_stream_(new DataTypeMidiStream())
{
    createOutlet();

    createCbProperty("@filename", &XMidiFile::p_filename);
    createCbProperty("@tracks", &XMidiFile::p_tracks);
    createCbProperty("@tempo", &XMidiFile::p_tempo);
}

void XMidiFile::onBang()
{
    dataTo(0, midi_stream_);
}

void XMidiFile::m_clear(t_symbol*, const AtomList&)
{
    midi_stream_ = new DataTypeMidiStream();
}

void XMidiFile::m_read(t_symbol*, const AtomList& l)
{
    if (!checkArgs(l, ARG_SYMBOL))
        return;

    t_symbol* name = l[0].asSymbol();
    MidiFile mf;

    if (!mf.read(name->s_name)) {
        OBJ_ERR << "can't read MIDI file: " << name->s_name;
        return;
    }

    midi_stream_ = new DataTypeMidiStream(mf);
}

AtomList XMidiFile::p_filename() const
{
    return Atom(midi_stream_->filename());
}

AtomList XMidiFile::p_tempo() const
{
    return Atom(midi_stream_->tempo());
}

AtomList XMidiFile::p_tracks() const
{
    return Atom(midi_stream_->trackCount());
}

void setup_midi_file()
{
    ObjectFactory<XMidiFile> obj("midi.file");
    obj.addMethod("read", &XMidiFile::m_read);
    obj.addMethod("clear", &XMidiFile::m_clear);
}
