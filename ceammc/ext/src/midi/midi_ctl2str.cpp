#include "midi_ctl2str.h"
#include "../string/datatype_string.h"
#include "ceammc_factory.h"
#include "midi_common.h"

MidiCtl2Str::MidiCtl2Str(const PdArgs& args)
    : BaseObject(args)
    , as_symbol_(0)
{
    as_symbol_ = new FlagProperty("@symbol");
    createProperty(as_symbol_);

    createOutlet();
}

void MidiCtl2Str::onFloat(float f)
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

void setup_midi_ctl2str()
{
    ObjectFactory<MidiCtl2Str> obj("midi.ctl2str");
    obj.addAlias("midi.ctl->str");
}
