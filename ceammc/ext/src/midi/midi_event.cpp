#include "midi_event.h"
#include "ceammc_crc32.h"
#include "ceammc_factory.h"
#include "fmt/core.h"

CEAMMC_DEFINE_HASH(MidiEvent);

#include "MidiEvent.h"

// for std::unique_ptr
XMidiEvent::~XMidiEvent() = default;

XMidiEvent::XMidiEvent(const AtomListView& lv)
    : event_(new MidiEvent())
    , duration_(0)
    , track_(0)
    , valid_(false)
{
    parse(lv);
}

bool XMidiEvent::parse(const AtomListView& lv)
{
    valid_ = (lv.size() >= 5);

    if (valid_) {
        std::vector<uchar> v;
        v.reserve(lv.size() - 3);

        for (size_t i = 3; i < lv.size(); i++)
            v.push_back(lv[i].asInt());

        *event_.get() = MidiEvent(lv[0].asInt(), lv[1].asInt(), v);
        track_ = lv[1].asInt();
        duration_ = lv[2].asFloat();
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

bool XMidiEvent::isControl() const
{
    if (!valid_)
        return false;

    return event_->isController();
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

void BaseMidiEventExternal::onAny(t_symbol* s, const AtomListView& args)
{
    if (crc32_hash(s) != hash_MidiEvent) {
        OBJ_ERR << fmt::format("{} expected, got: '{}'", str_MidiEvent, s->s_name);
        return;
    }

    if (!event_.parse(args))
        return;

    processEvent();
}

MidiEventToNote::MidiEventToNote(const PdArgs& args)
    : BaseMidiEventExternal(args)
{
    createOutlet();
    createOutlet();
    msg_.fill(Atom(0.f), 2);
}

void MidiEventToNote::processEvent()
{
    if (!event_.isNote())
        return;

    floatTo(2, event_.track());
    floatTo(1, event_.duration());

    msg_[0].setFloat(event_.event().getKeyNumber());
    msg_[1].setFloat(event_.event().getVelocity());

    listTo(0, msg_);
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
    floatTo(0, event_.event().getP1() + 1);
}

void setup_midi_event()
{
    ObjectFactory<MidiEventToNote> to_note("midi.event2note");
    to_note.addAlias("midi.ev->note");
    ObjectFactory<MidiEventToPrg> to_prg("midi.event2prg");
    to_prg.addAlias("midi.ev->prg");
}
