#include "conv_pitch2midi.h"
#include "datatype_string.h"
#include "ceammc_convert.h"
#include "ceammc_factory.h"

PitchToMIDI::PitchToMIDI(const PdArgs& a)
    : BaseObject(a)
{
    createOutlet();
}

void PitchToMIDI::onDataT(const DataTypeString& s)
{
    convert(s.str().c_str());
}

void PitchToMIDI::convert(const char* p)
{
    int midi_note = spn2midi(p);

    if (midi_note < 0) {
        OBJ_ERR << "invalid pitch name: " << p;
        return;
    }

    floatTo(0, midi_note);
}

void PitchToMIDI::onSymbol(t_symbol* p)
{
    convert(p->s_name);
}

void PitchToMIDI::onList(const AtomList& lst)
{
    AtomList res;

    for (size_t i = 0; i < lst.size(); i++) {
        res.append(Atom(spn2midi(lst[i].asSymbol()->s_name)));
    }

    listTo(0, res);
}

int PitchToMIDI::spn2midi(const char* p)
{
    return convert::spn2midi(p);
}

extern "C" void setup_conv0x2epitch2midi()
{
    ObjectFactory<PitchToMIDI> obj("conv.pitch2midi");
    obj.addAlias("pitch->midi");
    obj.processData<DataTypeString>();
}
