#include "conv_pitch2midi.h"
#include "../string/datatype_string.h"
#include "ceammc_convert.h"
#include "ceammc_factory.h"

static t_symbol* MODE_SPN = gensym("spn");
static t_symbol* MODE_HELMHOLTZ = gensym("helmholtz");

PitchToMIDI::PitchToMIDI(const PdArgs& a)
    : BaseObject(a)
    , mode_(0)
    , mode_spn_(0)
    , mode_helmholtz_(0)
{
    createOutlet();

    mode_ = new SymbolEnumProperty("@mode", "spn");
    mode_->appendEnum("helmholtz");

    mode_spn_ = new SymbolEnumAlias("@spn", mode_, MODE_SPN);
    mode_helmholtz_ = new SymbolEnumAlias("@helmholtz", mode_, MODE_HELMHOLTZ);

    createProperty(mode_);
    createProperty(mode_spn_);
    createProperty(mode_helmholtz_);
}

void PitchToMIDI::onDataT(const DataTypeString& s)
{
    convert(s.str().c_str());
}

void PitchToMIDI::convert(const char* p)
{
    int midi_note = 0;

    if (mode_->value() == MODE_SPN)
        midi_note = spn2midi(p);
    else if (mode_->value() == MODE_HELMHOLTZ)
        midi_note = helmholtz2midi(p);
    else {
        OBJ_ERR << "unsupported mode: " << mode_->value();
        return;
    }

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

int PitchToMIDI::spn2midi(const char* p)
{
    return convert::spn2midi(p);
}

int PitchToMIDI::helmholtz2midi(const char* p)
{
    return -1;
}

extern "C" void setup_conv0x2epitch2midi()
{
    ObjectFactory<PitchToMIDI> obj("conv.pitch2midi");
    obj.addAlias("pitch->midi");
    obj.processData<DataTypeString>();
}
