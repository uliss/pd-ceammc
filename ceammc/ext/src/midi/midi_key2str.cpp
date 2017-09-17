#include "midi_key2str.h"
#include "ceammc_factory.h"
#include "ceammc_format.h"
#include "midi_common.h"

MidiKey2Str::MidiKey2Str(const PdArgs& args)
    : BaseObject(args)
    , tonality_(music::PitchClass::C, music::MAJOR)
{
    // init cache with zeroes
    memset(cache_, 0, sizeof(t_symbol*) * 12);

    if (positionalArguments().size() > 0) {
        std::string str = to_string(positionalArguments());
        if (!music::from_string(str, tonality_, music::NAMING_SCHEME_SPN))
            OBJ_ERR << "can't parse tonality: " << str;
    }

    createCbProperty("@tonality", &MidiKey2Str::p_tonality, &MidiKey2Str::p_setTonality);
    createOutlet();
}

void MidiKey2Str::onFloat(float f)
{
    int key = f;
    if (key < 0 || key > 127) {
        OBJ_ERR << "invalid midi key value: " << key;
        return;
    }

    key %= 12;

    if (cache_[key] == 0)
        cache_[key] = midi::key_to_name(key, tonality_, true);

    symbolTo(0, cache_[key]);
}

AtomList MidiKey2Str::p_tonality() const
{
    std::string name = music::to_string(tonality_, music::NAMING_SCHEME_SPN);
    return Atom(gensym(name.c_str()));
}

void MidiKey2Str::p_setTonality(const AtomList& l)
{
    std::string str = to_string(l);
    if (!music::from_string(str, tonality_, music::NAMING_SCHEME_SPN))
        OBJ_ERR << "can't parse tonality: " << str;
}

void setup_midi_key2str()
{
    ObjectFactory<MidiKey2Str> obj("midi.key2str");
    obj.addAlias("midi.key->str");
}
