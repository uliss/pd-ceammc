#include "midi_ctl2str.h"
#include "ceammc_factory.h"
#include "datatype_string.h"
#include "midi_common.h"

MidiCtl2Str::MidiCtl2Str(const PdArgs& args)
    : BaseObject(args)
    , as_symbol_(0)
{
    as_symbol_ = new FlagProperty("@symbol");
    addProperty(as_symbol_);

    createOutlet();
}

void MidiCtl2Str::onFloat(t_float f)
{
    if (f < 0 || f > 127) {
        OBJ_ERR << "invalid controller number: " << f;
        return;
    }

    size_t n = f;
    auto name = midi::controller_name(n);

    if (as_symbol_->value())
        symbolTo(0, gensym(name));
    else
        atomTo(0, new DataTypeString(name));
}

void setup_midi_ctl2str()
{
    ObjectFactory<MidiCtl2Str> obj("midi.ctl2str");
    obj.addAlias("midi.ctl->str");

    obj.setDescription("convert Standard MIDI Controllers number to name");
    obj.setCategory("midi");
    obj.setKeywords({"midi", "controller", "name"});
}
