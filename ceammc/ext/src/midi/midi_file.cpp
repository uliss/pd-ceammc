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

    std::string path = searchFileInPaths(l[0].asSymbol()->s_name);
    if (path.empty())
        return;

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

    const_cast<MidiFile*>(midi_stream_->midifile())->write(l[0].asSymbol()->s_name);
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

std::string XMidiFile::searchFileInPaths(const char* fname)
{
    if (sys_isabsolutepath(fname))
        return fname;

    const char* patch_dir = "";
    if (cnv_ && cnv_->gl_env)
        patch_dir = canvas_getdir(cnv_)->s_name;

    char dirname[MAXPDSTRING], *filename;

    int fd = open_via_path(patch_dir, fname, "", dirname, &filename, MAXPDSTRING, 1);

    if (fd < 0) {
        OBJ_ERR << "file not found: " << fname;
        return "";
    }

    close(fd);

    std::string full_path(dirname);
    full_path += '/';
    full_path += filename;
    return full_path;
}

void setup_midi_file()
{
    ObjectFactory<XMidiFile> obj("midi.file");
    obj.processData<DataTypeMidiStream>();
    obj.addMethod("read", &XMidiFile::m_read);
    obj.addMethod("clear", &XMidiFile::m_clear);
    obj.addMethod("write", &XMidiFile::m_write);
}
