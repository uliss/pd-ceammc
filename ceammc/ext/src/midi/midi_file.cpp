#include "midi_file.h"
#include "ceammc_canvas.h"
#include "ceammc_factory.h"
#include "datatype_dict.h"

#include "MidiFile.h"

XMidiFile::XMidiFile(const PdArgs& a)
    : BaseObject(a)
{
    createOutlet();

    createCbSymbolProperty(
        "@filename",
        [this]() -> t_symbol* { return midi_stream_->filename(); },
        [this](t_symbol* path) -> bool { return open(path->s_name); })
        ->setArgIndex(0);

    createCbIntProperty("@tracks",
        [this]() -> int { return midi_stream_->trackCount(); });
    createCbIntProperty("@tempo",
        [this]() -> int { return midi_stream_->tempo(); });
    createCbFloatProperty("@length_sec",
        [this]() -> t_float { return midi_stream_->totalTimeInSeconds(); })
        ->setUnits(PropValueUnits::SEC);
    createCbIntProperty("@length_tick",
        [this]() -> int { return midi_stream_->totalTimeInTicks(); });
    createCbFloatProperty("@length_beat",
        [this]() -> t_float { return midi_stream_->totalTimeInQuarters(); });
}

void XMidiFile::onBang()
{
    atomTo(0, midi_stream_);
}

void XMidiFile::m_clear(t_symbol*, const AtomListView&)
{
    midi_stream_ = MidiStreamAtom();
}

void XMidiFile::m_info(t_symbol*, const AtomListView&)
{
    DictAtom info;
    info->insert("filename", midi_stream_->filename());

    atomTo(0, info);
}

void XMidiFile::m_read(t_symbol*, const AtomListView& l)
{
    if (!checkArgs(l, ARG_SYMBOL))
        return;

    std::string path = findInStdPaths(l[0].asSymbol()->s_name);
    if (path.empty()) {
        OBJ_ERR << "file is not found in search paths: " << l;
        return;
    }

    midi_stream_ = MidiStreamAtom(path.c_str());
    if (!midi_stream_->is_open()) {
        OBJ_ERR << "can't read MIDI file: " << path.c_str();
        return;
    }

    OBJ_DBG << "file read from: \"" << path << "\"";
}

void XMidiFile::m_write(t_symbol*, const AtomListView& l)
{
    if (!checkArgs(l, ARG_SYMBOL))
        return;

    std::string filepath = l[0].asSymbol()->s_name;
    if (!isAbsolutePath(filepath.c_str())) {

        std::string top_level_dir = canvasDir(CanvasType::TOPLEVEL)->s_name;

        if (!top_level_dir.empty())
            filepath = top_level_dir + "/" + filepath;
    }

    MidiFile* mf = const_cast<MidiFile*>(midi_stream_->midifile());
    mf->write(filepath.c_str());

    OBJ_DBG << "file written to: \"" << filepath << "\"";
}

void XMidiFile::onDataT(const MidiStreamAtom& data)
{
    midi_stream_ = data;
}

bool XMidiFile::open(const char* path)
{
    std::string full_path = findInStdPaths(path);
    if (full_path.empty()) {
        OBJ_ERR << "file is not found in search paths: " << path;
        return false;
    }

    midi_stream_ = MidiStreamAtom(full_path.c_str());
    if (!midi_stream_->is_open()) {
        OBJ_ERR << "can't read MIDI file: " << full_path;
        return false;
    }

    return true;
}

void setup_midi_file()
{
    ObjectFactory<XMidiFile> obj("midi.file");
    obj.processData<DataTypeMidiStream>();
    obj.addMethod("read", &XMidiFile::m_read);
    obj.addMethod("clear", &XMidiFile::m_clear);
    obj.addMethod("write", &XMidiFile::m_write);

    obj.setDescription("standard midi file SMF reader and writer");
    obj.setCategory("midi");
    obj.setKeywords({"midi", "file"});
}
