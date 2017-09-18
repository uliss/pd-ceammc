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

bool XMidiEvent::parse(const AtomList& l)
{
    valid_ = (l.size() >= 5);

    if (valid_) {
        std::vector<uchar> v;

        for (size_t i = 3; i < l.size(); i++)
            v.push_back(l[i].asInt());

        *event_.get() = MidiEvent(l[0].asInt(), l[1].asInt(), v);
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

bool XMidiEvent::isProgramChange() const
{
    if (!valid_)
        return false;

    return event_->isPatchChange();
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

BaseMidiEventExternal::BaseMidiEventExternal(const PdArgs& a)
    : BaseObject(a)
    , event_(AtomList())
{
    createOutlet();
}

void BaseMidiEventExternal::onAny(t_symbol* s, const AtomList& args)
{
    if (s != SYM_MIDI_EVENT) {
        OBJ_ERR << "MidiEvent expected: " << s->s_name;
        return;
    }

    if (!event_.parse(args))
        return;

    processEvent();
}

MidiEventIsNote::MidiEventIsNote(const PdArgs& args)
    : BaseMidiEventExternal(args)
{
}

void MidiEventIsNote::processEvent()
{
    floatTo(0, event_.isNote());
}

MidiEventToNote::MidiEventToNote(const PdArgs& args)
    : BaseMidiEventExternal(args)
{
    createOutlet();
    msg_.fill(Atom(0.f), 2);
}

void MidiEventToNote::processEvent()
{
    if (!event_.isNote())
        return;

    floatTo(1, event_.track());

    msg_[0].setFloat(event_.event().getKeyNumber());
    msg_[1].setFloat(event_.event().getVelocity());

    listTo(0, msg_);
}

MidiEventIsPrg::MidiEventIsPrg(const PdArgs& args)
    : BaseMidiEventExternal(args)
{
}

void MidiEventIsPrg::processEvent()
{
    floatTo(0, event_.isProgramChange());
}

MidiEventToPrg::MidiEventToPrg(const PdArgs& args)
    : BaseMidiEventExternal(args)
{
    // second outlet
    createOutlet();
}

void MidiEventToPrg::processEvent()
{
    if (!event_.isProgramChange())
        return;

    floatTo(1, event_.track());
    floatTo(0, event_.event().getP1());
}

void setup_midi_event()
{
    ObjectFactory<MidiEventIsNote> is_note("midievent.is_note");
    ObjectFactory<MidiEventToNote> to_note("midi.event2note");
    to_note.addAlias("midi.ev->note");
    ObjectFactory<MidiEventIsPrg> is_prg("midievent.is_prg");
    ObjectFactory<MidiEventToPrg> to_prg("midi.ev->prg");
}
