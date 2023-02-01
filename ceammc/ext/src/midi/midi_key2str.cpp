#include "midi_key2str.h"
#include "ceammc_factory.h"
#include "ceammc_format.h"
#include "datatype_string.h"
#include "midi_common.h"

#include <cstring>

MidiKey2Str::MidiKey2Str(const PdArgs& args)
    : BaseObject(args)
    , tonality_(music::PitchClass::C, music::MAJOR)
    , as_symbol_(0)
{
    clearCache();

    createCbListProperty(
        "@tonality",
        [this]() -> AtomList { return p_tonality(); },
        [this](const AtomListView& lv) -> bool { return p_setTonality(lv); })
        ->setArgIndex(0);

    as_symbol_ = new FlagProperty("@symbol");
    addProperty(as_symbol_);

    createOutlet();
}

void MidiKey2Str::onFloat(t_float f)
{
    int key = f;
    if (key < 0 || key > 127) {
        OBJ_ERR << "invalid midi key value: " << key;
        return;
    }

    key %= 12;

    if (cache_[key] == 0)
        cache_[key] = gensym(midi::key_to_name(key, tonality_, true).c_str());

    if (as_symbol_->value())
        symbolTo(0, cache_[key]);
    else
        atomTo(0, new DataTypeString(cache_[key]->s_name));
}

AtomList MidiKey2Str::p_tonality() const
{
    std::string name = music::to_string(tonality_, music::NAMING_SCHEME_SPN);
    return Atom(gensym(name.c_str()));
}

bool MidiKey2Str::p_setTonality(const AtomListView& lv)
{
    std::string str = to_string(lv);
    if (!music::from_string(str, tonality_, music::NAMING_SCHEME_SPN)) {
        OBJ_ERR << "can't parse tonality: " << str;
        return false;
    }

    clearCache();
    return true;
}

void MidiKey2Str::clearCache()
{
    memset(cache_, 0, sizeof(t_symbol*) * 12);
}

void setup_midi_key2str()
{
    ObjectFactory<MidiKey2Str> obj("midi.key2str");
    obj.addAlias("midi.key->str");
}
