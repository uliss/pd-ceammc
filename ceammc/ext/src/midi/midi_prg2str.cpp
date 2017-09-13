#include "midi_prg2str.h"
#include "../string/datatype_string.h"
#include "ceammc_factory.h"
#include "midi_common.h"

Prg2Str::Prg2Str(const PdArgs& a)
    : BaseObject(a)
    , as_symbol_(0)
    , family_(0)
{
    as_symbol_ = new FlagProperty("@symbol");
    family_ = new FlagProperty("@family");
    createProperty(as_symbol_);
    createProperty(family_);

    createOutlet();
}

void Prg2Str::onFloat(float v)
{
    int val = v;

    if (!(0 < val && val <= 128)) {
        OBJ_ERR << "invalid program change value:" << v;
        return;
    }

    t_symbol* name = family_->value() ? midi::instrument_family(val) : midi::instrument_name(val);

    if (as_symbol_->value())
        symbolTo(0, name);
    else
        dataTo(0, DataPtr(new DataTypeString(name->s_name)));
}

void setup_midi_prg2str()
{
    ObjectFactory<Prg2Str> obj("midi.prg2str");
    obj.addAlias("midi.prg->str");
}
