#include "conv_pitch2midi.h"
#include "ceammc_containers.h"
#include "ceammc_convert.h"
#include "ceammc_factory.h"
#include "datatype_mlist.h"
#include "datatype_string.h"

PitchToMIDI::PitchToMIDI(const PdArgs& a)
    : BaseObject(a)
{
    createOutlet();
}

void PitchToMIDI::onDataT(const StringAtom& s)
{
    convert(s->str().c_str());
}

void PitchToMIDI::onDataT(const MListAtom& ml)
{
    onList(ml->data());
}

void PitchToMIDI::convert(const char* p)
{
    const auto note = convert::spn2midi(p);

    if (note == convert::MIDI_NONE) {
        OBJ_ERR << "invalid pitch name: " << p;
        return;
    } else if (note == convert::MIDI_REST) {
        floatTo(0, -1);
    } else {
        floatTo(0, note);
    }
}

void PitchToMIDI::onSymbol(t_symbol* p)
{
    convert(p->s_name);
}

void PitchToMIDI::onList(const AtomListView& lv)
{
    AtomList256 res;
    res.reserve(lv.size());

    for (auto& a : lv) {
        if (!a.isSymbol())
            continue;

        auto note = convert::spn2midi(a.asT<t_symbol*>()->s_name);
        if (note == convert::MIDI_NONE) {
            OBJ_ERR << "invalid pitch: " << a;
            continue;
        } else if (note == convert::MIDI_REST)
            res.push_back(-1);
        else
            res.push_back(note);
    }

    listTo(0, res.view());
}

void setup_conv_pitch2midi()
{
    ObjectFactory<PitchToMIDI> obj("conv.pitch2midi");
    obj.addAlias("pitch->midi");
    obj.processData<DataTypeString, DataTypeMList>();

    obj.setXletsInfo({ "symbol: SPN note\n"
                       "list: list of SPN notes\n"
                       "String: SPN note\n"
                       "MList: list of SPN notes" },
        { "int: MIDI pitch 0..127 or -1 on rest" });
}
