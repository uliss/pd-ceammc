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
    , clock_(clock_new(this, (t_method)&MidiTrack::clockTickHandler))
    , play_state_(PLAY_STATE_STOPPED)
{
    join_ = new FlagProperty("@join");
    track_idx_ = new SizeTProperty("@index", positionalFloatArgument(0, 0));
    tempo_ = new IntProperty("@tempo", 0);

    createProperty(join_);
    createProperty(track_idx_);
    createProperty(tempo_);

    createOutlet();
    createOutlet();

    createCbProperty("@events", &MidiTrack::p_events);

    // reserve space to avoid runtime reallocations
    current_event_.reserve(10);
}

MidiTrack::~MidiTrack()
{
    if (clock_)
        clock_free(clock_);
}

void MidiTrack::onBang()
{
    outputCurrent();
    //    dataTo(0, midi_track_.clone());
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
    return Atom(size());
}

void MidiTrack::m_next(t_symbol*, const AtomList&)
{
    if (current_event_idx_ >= size()) {
        OBJ_DBG << "end of track reached";
        return;
    }

    size_t next_idx = findNextEventIndex(current_event_idx_);
    if (!next_idx) {
        floatTo(1, 0);
        OBJ_DBG << "end of track reached";
        return;
    }

    current_event_idx_ = next_idx;
}

void MidiTrack::m_reset(t_symbol*, const AtomList&)
{
    current_event_idx_ = 0;
}

void MidiTrack::m_output(t_symbol*, const AtomList&)
{
    for (size_t i = 0; i < size(); i++)
        outputEvent(midi_track_.eventAt(i));
}

void MidiTrack::m_seek(t_symbol*, const AtomList& l)
{
    if (l.empty()) {
        OBJ_ERR << "usage: seek tick_index [BEGIN|REL]";
        return;
    }

    t_symbol* mode = gensym("BEGIN");
    int tick_idx = 0;

    if (l.size() < 2) {
        if (!l[0].isInteger()) {
            OBJ_ERR << "seek: integer required for tick index";
            return;
        }

        tick_idx = l[0].asInt();

        if (tick_idx >= 0)
            seekAbs(tick_idx);
        else {
            OBJ_ERR << "negative tick are not supported yet: " << tick_idx;
            return;
        }
    }
}

void MidiTrack::m_play(t_symbol*, const AtomList&)
{
    if (play_state_ == PLAY_STATE_PLAYING) {
        OBJ_ERR << "already playing...";
        return;
    }

    play_state_ = PLAY_STATE_PLAYING;
    clockTick();
}

void MidiTrack::m_stop(t_symbol*, const AtomList&)
{
    if (play_state_ == PLAY_STATE_STOPPED) {
        OBJ_ERR << "already stopped...";
        return;
    }

    current_event_idx_ = 0;
    play_state_ = PLAY_STATE_STOPPED;
    clock_unset(clock_);
}

void MidiTrack::outputEvent(MidiEvent* ev)
{
    current_event_.clear();

    current_event_.append(ev->tick);
    current_event_.append(ev->track);
    current_event_.append(ev->getDurationInSeconds() * 1000);

    const size_t size = ev->size();
    for (size_t i = 0; i < size; i++)
        current_event_.append(Atom(ev->operator[](i)));

    anyTo(0, SYM_MIDI_EVENT, current_event_);
}

struct NewTickFinder {
    const int tick;
    NewTickFinder(int t)
        : tick(t)
    {
    }

    bool operator()(MidiEvent* e) { return e->tick != tick; }
};

MidiTrack::MidiEventIterator MidiTrack::findNextTick(MidiEventIterator ev)
{
    if (ev == end())
        return end();

    return std::find_if(ev, end(), NewTickFinder((*ev)->tick));
}

MidiTrack::MidiEventConstIterator MidiTrack::findNextTick(MidiTrack::MidiEventConstIterator ev) const
{
    if (ev == end())
        return end();

    return std::find_if(ev, end(), NewTickFinder((*ev)->tick));
}

MidiTrack::MidiEventIterator MidiTrack::findTickAt(size_t tickIdx)
{
    MidiEventIterator it = begin();

    for (size_t i = 0; i < tickIdx; i++) {
        MidiEventIterator next_tick = findNextTick(it);
        if (next_tick == end())
            return end();

        it = next_tick;
    }

    return it;
}

MidiTrack::MidiEventConstIterator MidiTrack::findTickAt(size_t tickIdx) const
{
    MidiEventConstIterator it = begin();

    for (size_t i = 0; i < tickIdx; i++) {
        MidiEventConstIterator next_tick = findNextTick(it);
        if (next_tick == end())
            return end();

        it = next_tick;
    }

    return it;
}

size_t MidiTrack::findNextEventIndex(size_t idx) const
{
    if (idx >= size())
        return 0;

    MidiEventConstIterator cur_ev = begin() + idx;
    MidiEventConstIterator next_ev = findNextTick(cur_ev);

    return next_ev - begin();
}

bool MidiTrack::seekAbs(size_t tick)
{
    MidiEventIterator ev = findTickAt(tick);
    if (ev == end()) {
        OBJ_ERR << "invalid tick position: " << tick;
        return false;
    }

    current_event_idx_ = ev - begin();
    return true;
}

int MidiTrack::currentTick() const
{
    if (current_event_idx_ >= size())
        return 0;

    return midi_track_.eventAt(current_event_idx_)->tick;
}

struct EventOutput {
    MidiTrack* track;
    EventOutput(MidiTrack* t)
        : track(t)
    {
    }

    void operator()(MidiEvent* e)
    {
        track->outputEvent(e);

        if (e->isTempo()) {
        }
    }
};

double MidiTrack::outputCurrent()
{
    if (current_event_idx_ >= size())
        return 0;

    MidiEventIterator cur_ev = begin() + current_event_idx_;
    MidiEventIterator next_ev = findNextTick(cur_ev);

    //    int tpq = getTicksPerQuarterNote();
    //    double defaultTempo = 120.0;
    //    double secondsPerTick = 60.0 / (defaultTempo * tpq);

    double tick_duration = 0;
    if (next_ev != end())
        tick_duration = (*next_ev)->seconds - (*cur_ev)->seconds;

    floatTo(1, tick_duration);

    // output all events before next tick
    EventOutput event_out(this);
    std::for_each(cur_ev, next_ev, event_out);

    return tick_duration;
}

void MidiTrack::clockTickHandler(MidiTrack* p)
{
    p->clockTick();
}

void MidiTrack::clockTick()
{
    if (play_state_ != PLAY_STATE_PLAYING)
        return;

    OBJ_DBG << "tick";
    double dur = outputCurrent();

    if (dur <= 0) {
        OBJ_DBG << "finished";
        return;
    }

    clock_delay(clock_, dur * 1000);
    m_next(0, AtomList());
}

void setup_midi_track()
{
    ObjectFactory<MidiTrack> obj("midi.track");
    obj.processData<DataTypeMidiStream>();
    obj.addMethod("next", &MidiTrack::m_next);
    obj.addMethod("reset", &MidiTrack::m_reset);
    obj.addMethod("output", &MidiTrack::m_output);
    obj.addMethod("seek", &MidiTrack::m_seek);
    obj.addMethod("play", &MidiTrack::m_play);
    obj.addMethod("stop", &MidiTrack::m_stop);
}
