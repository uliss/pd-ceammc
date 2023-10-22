#include "midi_event_to_ctl.h"
#include "ceammc_factory.h"

#include "MidiEvent.h"

MidiEventToCtl::MidiEventToCtl(const PdArgs& args)
    : BaseMidiEventExternal(args)
{
    createOutlet();

    msg_.fill(Atom(0.f), 2);
}

void MidiEventToCtl::processEvent()
{
    if (!event_.isControl())
        return;

    floatTo(1, event_.track());

    msg_[0].setFloat(event_.event().getP1());
    msg_[1].setFloat(event_.event().getP2());

    listTo(0, msg_);
}

void setup_midi_event2ctl()
{
    ObjectFactory<MidiEventToCtl> obj("midi.event2ctl");
    obj.addAlias("midi.ev->ctl");

    obj.setDescription("convert MidiEvent message to controller channel/value pair");
    obj.setCategory("midi");
    obj.setKeywords({"midi", "event", "control"});
}
