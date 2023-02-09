#include "midi_prg2str.h"
#include "ceammc_factory.h"
#include "datatype_string.h"
#include "midi_common.h"

Prg2Str::Prg2Str(const PdArgs& a)
    : BaseObject(a)
    , as_symbol_(0)
    , family_(0)
{
    as_symbol_ = new FlagProperty("@symbol");
    family_ = new FlagProperty("@family");
    addProperty(as_symbol_);
    addProperty(family_);

    createOutlet();
}

void Prg2Str::onFloat(t_float v)
{
    int val = v;

    if (!(0 < val && val <= 128)) {
        OBJ_ERR << "invalid program change value:" << v;
        return;
    }

    auto name = family_->value() ? midi::instrument_family(val) : midi::instrument_name(val);

    if (as_symbol_->value())
        symbolTo(0, gensym(name));
    else
        atomTo(0, new DataTypeString(name));
}

void setup_midi_prg2str()
{
    ObjectFactory<Prg2Str> obj("midi.prg2str");
    obj.addAlias("midi.prg->str");

    obj.setDescription("convert GM Instrument number to name (string or symbol)");
    obj.setCategory("midi");
    obj.setKeywords({"midi", "convert", "midi"});
}
