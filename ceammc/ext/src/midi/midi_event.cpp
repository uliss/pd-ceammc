#include "midi_event.h"
#include "MidiEvent.h"
#include "ceammc_factory.h"

static t_symbol* SYM_MIDI_EVENT = gensym("MidiEvent");

XMidiEvent::XMidiEvent(const AtomList& l)
    : valid_(false)
    , raw_(l)
    , event_(new MidiEvent())
    , duration_(0)
    , track_(0)
{
    parse(l);
}

XMidiEvent::~XMidiEvent()
{
    delete event_;
}

bool XMidiEvent::parse(const AtomList& l)
{
    valid_ = (l.size() >= 5);

    if (valid_) {
        std::vector<uchar> v;

        for (size_t i = 3; i < l.size(); i++)
            v.push_back(l[i].asInt());

        *event_ = MidiEvent(l[0].asInt(), l[1].asInt(), v);
        track_ = l[1].asInt();
        duration_ = l[2].asFloat();
    }

    return valid_;
}

bool XMidiEvent::isNote() const
{
    if (!valid_)
        return false;

    return event_->isNote();
}

bool XMidiEvent::isValid() const
{
    return valid_;
}

t_float XMidiEvent::duration() const
{
    return duration_;
}

MidiEvent& XMidiEvent::event()
{
    return *event_;
}

int XMidiEvent::track() const
{
    return track_;
}

MidiEventIsNote::MidiEventIsNote(const PdArgs& args)
    : BaseObject(args)
    , ev_(AtomList())
{
    createOutlet();
}

void MidiEventIsNote::onAny(t_symbol* s, const AtomList& args)
{
    if (s != SYM_MIDI_EVENT) {
        OBJ_ERR << "MidiEvent expected: " << s->s_name;
        return floatTo(0, 0);
    }

    ev_.parse(args);
    floatTo(0, ev_.isNote());
}

MidiEventToNote::MidiEventToNote(const PdArgs& args)
    : BaseObject(args)
    , ev_(AtomList())
{
    createOutlet();
    createOutlet();
    note_.fill(Atom(0.f), 2);
}

void MidiEventToNote::onAny(t_symbol* s, const AtomList& args)
{
    if (s != SYM_MIDI_EVENT) {
        OBJ_ERR << "MidiEvent expected: " << s->s_name;
        return;
    }

    if (!ev_.parse(args))
        return;

    if (!ev_.isNote())
        return;

    floatTo(1, ev_.track());

    note_[0].setFloat(ev_.event().getKeyNumber());
    note_[1].setFloat(ev_.event().getVelocity());

    listTo(0, note_);
}

void setup_midi_event()
{
    ObjectFactory<MidiEventIsNote> is_note("midievent.is_note");
    ObjectFactory<MidiEventToNote> note("midi.ev->note");
}
