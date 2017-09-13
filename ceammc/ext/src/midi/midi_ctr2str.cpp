#include "midi_ctr2str.h"
#include "../string/datatype_string.h"
#include "ceammc_factory.h"
#include "midi_common.h"

MidiCtr2Str::MidiCtr2Str(const PdArgs& args)
    : BaseObject(args)
    , as_symbol_(0)
{
    as_symbol_ = new FlagProperty("@symbol");
    createProperty(as_symbol_);

    createOutlet();
}

void MidiCtr2Str::onFloat(float f)
{
    size_t n = f;

    if (n > 127) {
        OBJ_ERR << "invalid controller number: " << f;
        return;
    }

    t_symbol* name = midi::controller_name(n);

    if (as_symbol_->value())
        symbolTo(0, name);
    else
        dataTo(0, DataPtr(new DataTypeString(name->s_name)));
}

void setup_midi_ctr2str()
{
    ObjectFactory<MidiCtr2Str> obj("midi.ctr2str");
    obj.addAlias("midi.ctr->str");
}
