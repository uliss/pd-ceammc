#include "midi_file.h"
#include "ceammc_factory.h"

#include "MidiFile.h"

extern "C" {
#include "g_canvas.h"
}

XMidiFile::XMidiFile(const PdArgs& a)
    : BaseObject(a)
    , midi_stream_(new DataTypeMidiStream())
    , cnv_(canvas_getcurrent())
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

    std::string path = findInStdPaths(l[0].asSymbol()->s_name);
    if (path.empty()) {
        OBJ_ERR << "file is not found in search paths: " << l;
        return;
    }

    MidiFile mf;

    if (!mf.read(path.c_str())) {
        OBJ_ERR << "can't read MIDI file: " << path.c_str();
        return;
    }

    midi_stream_ = new DataTypeMidiStream(mf);
}

void XMidiFile::m_write(t_symbol*, const AtomList& l)
{
    if (!checkArgs(l, ARG_SYMBOL))
        return;

    std::string filepath = l[0].asSymbol()->s_name;
    if (!isAbsolutePath(filepath.c_str())) {
        if (!patchDirectory().empty())
            filepath = patchDirectory() + "/" + filepath;
    }

    MidiFile* mf = const_cast<MidiFile*>(midi_stream_->midifile());
    mf->write(filepath.c_str());
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

void XMidiFile::onDataT(const DataTypeMidiStream& data)
{
    midi_stream_ = data.clone();
}

void setup_midi_file()
{
    ObjectFactory<XMidiFile> obj("midi.file");
    obj.processData<DataTypeMidiStream>();
    obj.addMethod("read", &XMidiFile::m_read);
    obj.addMethod("clear", &XMidiFile::m_clear);
    obj.addMethod("write", &XMidiFile::m_write);
}
