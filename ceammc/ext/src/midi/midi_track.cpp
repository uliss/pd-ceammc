#include "midi_track.h"
#include "MidiFile.h"
#include "ceammc_factory.h"
#include "datatype_midistream.h"

static t_symbol* SYM_MIDI_EVENT = gensym("MidiEvent");

MidiTrack::MidiTrack(const PdArgs& args)
    : BaseObject(args)
    , midi_track_()
    , join_(0)
    , track_idx_(0)
    , tempo_(0)
    , current_event_idx_(0)
{
    join_ = new FlagProperty("@join");
    track_idx_ = new SizeTProperty("@index", positionalFloatArgument(0, 0));
    tempo_ = new IntProperty("@tempo", 0);

    createProperty(join_);
    createProperty(track_idx_);

    createOutlet();
    createOutlet();

    createCbProperty("@events", &MidiTrack::p_events);

    // reserve space to avoid runtime reallocations
    current_event_.reserve(10);
}

void MidiTrack::onBang()
{
    dataTo(0, midi_track_.clone());
}

void MidiTrack::onDataT(const DataTypeMidiStream& s)
{
    if (join_->value()) {
        // copy
        MidiFile mf = *s.midifile();
        mf.joinTracks();

        midi_track_ = DataTypeMidiTrack(mf[0]);
        tempo_->setValue(mf.getTicksPerQuarterNote());

    } else {
        const size_t trackN = track_idx_->value();
        const MidiFile mf = *s.midifile();
        if (mf.getTrackCount() <= trackN) {
            OBJ_ERR << "invalid track index: " << trackN;
            return;
        }

        midi_track_ = DataTypeMidiTrack(mf[trackN]);
        tempo_->setValue(mf.getTicksPerQuarterNote());
    }

    current_event_idx_ = 0;
}

AtomList MidiTrack::p_events() const
{
    return Atom(midi_track_.eventCount());
}

void MidiTrack::m_next(t_symbol*, const AtomList&)
{
    if (current_event_idx_ >= midi_track_.eventCount()) {
        OBJ_DBG << "end of track reached";
        return;
    }

    outputCurrent();
}

void MidiTrack::m_reset(t_symbol*, const AtomList&)
{
    current_event_idx_ = 0;
}

void MidiTrack::m_output(t_symbol*, const AtomList&)
{
    for (size_t i = 0; i < midi_track_.eventCount(); i++)
        outputEvent(midi_track_.eventAt(i));
}

void MidiTrack::outputEvent(MidiEvent* ev)
{
    current_event_.clear();

    current_event_.append(ev->tick);
    current_event_.append(ev->track);
    current_event_.append(ev->getDurationInSeconds() / 1000.f);

    const size_t size = ev->size();
    for (size_t i = 0; i < size; i++)
        current_event_.append(Atom(ev->operator[](i)));

    anyTo(0, SYM_MIDI_EVENT, current_event_);
}

int MidiTrack::currentTick() const
{
    if (current_event_idx_ >= midi_track_.eventCount())
        return 0;

    return midi_track_.eventAt(current_event_idx_)->tick;
}

struct NewTickFinder {
    const int tick;
    NewTickFinder(int t)
        : tick(t)
    {
    }

    bool operator()(MidiEvent* e) { return e->tick != tick; }
};

struct EventOutput {
    MidiTrack* track;
    EventOutput(MidiTrack* t)
        : track(t)
    {
    }

    void operator()(MidiEvent* e) { track->outputEvent(e); }
};

void MidiTrack::outputCurrent()
{
    MidiEvent* e = midi_track_.eventAt(current_event_idx_);
    const int current_tick = e->tick;

    MidiEvent** cur = midi_track_.events().data() + current_event_idx_;
    MidiEvent** end = midi_track_.events().data() + midi_track_.eventCount();
    NewTickFinder new_tick_finder(current_tick);
    MidiEvent** next_tick = std::find_if(cur, end, new_tick_finder);

    int tick_duration = (next_tick == end) ? 0 : (*next_tick)->tick - current_tick;
    const size_t n_events = (next_tick - cur);

    floatTo(1, tick_duration);

    // output all
    EventOutput event_out(this);
    std::for_each(cur, next_tick, event_out);

    current_event_idx_ += n_events;
}

void setup_midi_track()
{
    ObjectFactory<MidiTrack> obj("midi.track");
    obj.processData<DataTypeMidiStream>();
    obj.addMethod("next", &MidiTrack::m_next);
    obj.addMethod("reset", &MidiTrack::m_reset);
    obj.addMethod("output", &MidiTrack::m_output);
}
