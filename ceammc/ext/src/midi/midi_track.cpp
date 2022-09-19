#include "midi_track.h"
#include "MidiFile.h"
#include "ceammc_factory.h"
#include "datatype_midistream.h"

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
    // properties
    join_ = new FlagProperty("@join");
    track_idx_ = new IntProperty("@track", 0);
    track_idx_->checkNonNegative();
    track_idx_->setArgIndex(0);
    tempo_ = new IntProperty("@tempo", 120);
    tempo_->setReadOnly();
    //    tempo_->setUnits(PropValueUnits::BPM);

    constexpr t_float DEFAULT_SPEED = 1;
    constexpr t_float MIN_SPEED = 0.01;
    speed_ = new FloatProperty("@speed", DEFAULT_SPEED);
    speed_->checkMin(MIN_SPEED);

    addProperty(join_);
    addProperty(track_idx_);
    addProperty(tempo_);
    addProperty(speed_);

    createCbIntProperty("@nevents", [this]() -> int { return size(); });
    createCbIntProperty("@current", [this]() -> int { return current_event_idx_; });
    // play state property
    {
        auto p = createCbIntProperty("@state", [this]() -> int { return play_state_; });
        p->infoT().setConstraints(PropValueConstraints::ENUM);
        const auto ok = p->infoT().addEnums({ PLAY_STATE_STOPPED, PLAY_STATE_PLAYING, PLAY_STATE_PAUSED });
    }

    createOutlet();
    createOutlet();

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
}

void MidiTrack::onDataT(const MidiStreamAtom& stream)
{
    if (join_->value()) {
        // copy
        MidiFile mf = *stream->midifile();
        mf.joinTracks();

        midi_track_.setEventList(mf.trackAt(0));
        tempo_->setValue(mf.getTicksPerQuarterNote());

    } else {
        const int trackN = track_idx_->value();
        const MidiFile* mf = stream->midifile();
        if (mf->getTrackCount() <= trackN) {
            OBJ_ERR << "invalid track index: " << trackN;
            return;
        }

        midi_track_.setEventList(mf->trackAt(trackN));
        tempo_->setValue(mf->getTicksPerQuarterNote());
    }

    current_event_idx_ = 0;
}

void MidiTrack::m_next(t_symbol*, const AtomListView&)
{
    if (current_event_idx_ >= size()) {
        OBJ_DBG << "end of track reached";
        return;
    }

    size_t next_idx = findNextTickEventIndex(current_event_idx_);
    if (!next_idx) {
        floatTo(1, 0);
        OBJ_DBG << "end of track reached";
        return;
    }

    current_event_idx_ = next_idx;
}

void MidiTrack::m_reset(t_symbol*, const AtomListView&)
{
    current_event_idx_ = 0;
}

void MidiTrack::m_seek(t_symbol*, const AtomListView& l)
{
    if (l.empty()) {
        OBJ_ERR << "usage: seek tick_index";
        return;
    }

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
            OBJ_ERR << "negative tick is not supported: " << tick_idx;
            return;
        }
    }
}

void MidiTrack::m_play(t_symbol*, const AtomListView&)
{
    if (play_state_ == PLAY_STATE_PLAYING) {
        OBJ_ERR << "already playing...";
        return;
    }

    play_state_ = PLAY_STATE_PLAYING;
    clockTick();
}

void MidiTrack::m_stop(t_symbol*, const AtomListView&)
{
    if (play_state_ == PLAY_STATE_STOPPED) {
        OBJ_ERR << "already stopped...";
        return;
    }

    current_event_idx_ = 0;
    play_state_ = PLAY_STATE_STOPPED;
    clock_unset(clock_);
}

void MidiTrack::m_pause(t_symbol*, const AtomListView&)
{
    if (play_state_ == PLAY_STATE_PAUSED) {
        OBJ_ERR << "already paused...";
        return;
    }

    if (play_state_ == PLAY_STATE_STOPPED) {
        OBJ_ERR << "playing stopped, ignored...";
        return;
    }

    play_state_ = PLAY_STATE_PAUSED;
    clock_unset(clock_);
}

void MidiTrack::outputEvent(MidiEvent* ev)
{
    constexpr auto* STR_MIDI_EVENT = "MidiEvent";

    current_event_.clear();

    current_event_.append(ev->tick);
    current_event_.append(ev->track);
    double dur_ms = (ev->getDurationInSeconds() * 1000) / speed_->value();
    current_event_.append(dur_ms > 0 ? dur_ms : 0);

    const size_t size = ev->size();
    for (size_t i = 0; i < size; i++)
        current_event_.append(Atom(ev->operator[](i)));

    anyTo(0, gensym(STR_MIDI_EVENT), current_event_);
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

MidiTrack::MidiEventIterator MidiTrack::findEventAt(size_t tickIndex)
{
    MidiEventIterator it = begin();

    for (size_t i = 0; i < tickIndex; i++) {
        MidiEventIterator next_tick = findNextTick(it);
        if (next_tick == end())
            return end();

        it = next_tick;
    }

    return it;
}

MidiTrack::MidiEventConstIterator MidiTrack::findEventAt(size_t tickIndex) const
{
    MidiEventConstIterator it = begin();

    for (size_t i = 0; i < tickIndex; i++) {
        MidiEventConstIterator next_tick = findNextTick(it);
        if (next_tick == end())
            return end();

        it = next_tick;
    }

    return it;
}

size_t MidiTrack::findNextTickEventIndex(size_t idx) const
{
    if (idx >= size())
        return 0;

    MidiEventConstIterator cur_ev = begin() + idx;
    MidiEventConstIterator next_ev = findNextTick(cur_ev);

    return next_ev - begin();
}

bool MidiTrack::seekAbs(size_t tickIndex)
{
    MidiEventIterator ev = findEventAt(tickIndex);
    if (ev == end()) {
        OBJ_ERR << "invalid tick index: " << tickIndex;
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
    }
};

double MidiTrack::outputCurrent()
{
    if (current_event_idx_ >= size())
        return 0;

    auto cur_ev = begin() + current_event_idx_;
    auto next_ev = findNextTick(cur_ev);

    double tick_duration_ms = 0;
    if (next_ev != end())
        tick_duration_ms = (((*next_ev)->seconds - (*cur_ev)->seconds) * 1000) / speed_->value();

    floatTo(1, tick_duration_ms);

    std::for_each(cur_ev, next_ev, [this](MidiEvent* e) { outputEvent(e); });

    return tick_duration_ms;
}

void MidiTrack::clockTickHandler(MidiTrack* p)
{
    p->clockTick();
}

void MidiTrack::clockTick()
{
    if (play_state_ != PLAY_STATE_PLAYING)
        return;

    double dur_ms = outputCurrent();

    if (dur_ms <= 0) {
        OBJ_DBG << "finished";
        play_state_ = PLAY_STATE_STOPPED;
        return;
    }

    clock_delay(clock_, dur_ms);
    m_next(0, {});
}

void setup_midi_track()
{
    ObjectFactory<MidiTrack> obj("midi.track");
    obj.processData<DataTypeMidiStream>();
    obj.addMethod("next", &MidiTrack::m_next);
    obj.addMethod("reset", &MidiTrack::m_reset);
    obj.addMethod("seek", &MidiTrack::m_seek);
    obj.addMethod("play", &MidiTrack::m_play);
    obj.addMethod("stop", &MidiTrack::m_stop);
    obj.addMethod("pause", &MidiTrack::m_pause);
}
