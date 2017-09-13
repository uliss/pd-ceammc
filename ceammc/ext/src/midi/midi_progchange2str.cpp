#include "midi_progchange2str.h"
#include "../string/datatype_string.h"
#include "ceammc_factory.h"

#include <boost/static_assert.hpp>

static t_symbol* PG_INSTR_NAMES[] = {
    gensym("Acoustic Grand Piano"),
    gensym("Bright Acoustic Piano"),
    gensym("Electric Grand Piano"),
    gensym("Honky-tonk Piano"),
    gensym("Electric Piano 1"),
    gensym("Electric Piano 2"),
    gensym("Harpsichord"),
    gensym("Clavi"),
    gensym("Celesta"),
    gensym("Glockenspiel"),
    gensym("Music Box"),
    gensym("Vibraphone"),
    gensym("Marimba"),
    gensym("Xylophone"),
    gensym("Tubular Bells"),
    gensym("Dulcimer"),
    gensym("Drawbar Organ"),
    gensym("Percussive Organ"),
    gensym("Rock Organ"),
    gensym("Church Organ"),
    gensym("Reed Organ"),
    gensym("Accordion"),
    gensym("Harmonica"),
    gensym("Tango Accordion"),
    gensym("Acoustic Guitar (nylon)"),
    gensym("Acoustic Guitar (steel)"),
    gensym("Electric Guitar (jazz)"),
    gensym("Electric Guitar (clean)"),
    gensym("Electric Guitar (muted)"),
    gensym("Overdriven Guitar"),
    gensym("Distortion Guitar"),
    gensym("Guitar harmonics"),
    gensym("Acoustic Bass"),
    gensym("Electric Bass (finger)"),
    gensym("Electric Bass (pick)"),
    gensym("Fretless Bass"),
    gensym("Slap Bass 1"),
    gensym("Slap Bass 2"),
    gensym("Synth Bass 1"),
    gensym("Synth Bass 2"),
    gensym("Violin"),
    gensym("Viola"),
    gensym("Cello"),
    gensym("Contrabass"),
    gensym("Tremolo Strings"),
    gensym("Pizzicato Strings"),
    gensym("Orchestral Harp"),
    gensym("Timpani"),
    gensym("String Ensemble 1"),
    gensym("String Ensemble 2"),
    gensym("SynthStrings 1"),
    gensym("SynthStrings 2"),
    gensym("Choir Aahs"),
    gensym("Voice Oohs"),
    gensym("Synth Voice"),
    gensym("Orchestra Hit"),
    gensym("Trumpet"),
    gensym("Trombone"),
    gensym("Tuba"),
    gensym("Muted Trumpet"),
    gensym("French Horn"),
    gensym("Brass Section"),
    gensym("SynthBrass 1"),
    gensym("SynthBrass 2"),
    gensym("Soprano Sax"),
    gensym("Alto Sax"),
    gensym("Tenor Sax"),
    gensym("Baritone Sax"),
    gensym("Oboe"),
    gensym("English Horn"),
    gensym("Bassoon"),
    gensym("Clarinet"),
    gensym("Piccolo"),
    gensym("Flute"),
    gensym("Recorder"),
    gensym("Pan Flute"),
    gensym("Blown Bottle"),
    gensym("Shakuhachi"),
    gensym("Whistle"),
    gensym("Ocarina"),
    gensym("Lead 1 (square)"),
    gensym("Lead 2 (sawtooth)"),
    gensym("Lead 3 (calliope)"),
    gensym("Lead 4 (chiff)"),
    gensym("Lead 5 (charang)"),
    gensym("Lead 6 (voice)"),
    gensym("Lead 7 (fifths)"),
    gensym("Lead 8 (bass + lead)"),
    gensym("Pad 1 (new age)"),
    gensym("Pad 2 (warm)"),
    gensym("Pad 3 (polysynth)"),
    gensym("Pad 4 (choir)"),
    gensym("Pad 5 (bowed)"),
    gensym("Pad 6 (metallic)"),
    gensym("Pad 7 (halo)"),
    gensym("Pad 8 (sweep)"),
    gensym("FX 1 (rain)"),
    gensym("FX 2 (soundtrack)"),
    gensym("FX 3 (crystal)"),
    gensym("FX 4 (atmosphere)"),
    gensym("FX 5 (brightness)"),
    gensym("FX 6 (goblins)"),
    gensym("FX 7 (echoes)"),
    gensym("FX 8 (sci-fi)"),
    gensym("Sitar"),
    gensym("Banjo"),
    gensym("Shamisen"),
    gensym("Koto"),
    gensym("Kalimba"),
    gensym("Bag pipe"),
    gensym("Fiddle"),
    gensym("Shanai"),
    gensym("Tinkle Bell"),
    gensym("Agogo"),
    gensym("Steel Drums"),
    gensym("Woodblock"),
    gensym("Taiko Drum"),
    gensym("Melodic Tom"),
    gensym("Synth Drum"),
    gensym("Reverse Cymbal"),
    gensym("Guitar Fret Noise"),
    gensym("Breath Noise"),
    gensym("Seashore"),
    gensym("Bird Tweet"),
    gensym("Telephone Ring"),
    gensym("Helicopter"),
    gensym("Applause"),
    gensym("Gunshot")
};

static t_symbol* PG_INSTR_FAMILY[] = {
    gensym("Piano"),
    gensym("Chromatic Percussion"),
    gensym("Organ"),
    gensym("Guitar"),
    gensym("Bass"),
    gensym("Strings"),
    gensym("Ensemble"),
    gensym("Brass"),
    gensym("Reed"),
    gensym("Pipe"),
    gensym("Synth Lead"),
    gensym("Synth Pad"),
    gensym("Synth Effects"),
    gensym("Ethnic"),
    gensym("Percussive"),
    gensym("Sound Effects")
};

BOOST_STATIC_ASSERT(sizeof(PG_INSTR_NAMES) / sizeof(t_symbol*) == 128);
BOOST_STATIC_ASSERT(sizeof(PG_INSTR_FAMILY) / sizeof(t_symbol*) == 16);

static t_symbol* instrument_family(int instrIndex)
{
    return PG_INSTR_FAMILY[instrIndex / 8];
}

static t_symbol* instrument_name(int instrIndex)
{
    return PG_INSTR_NAMES[instrIndex];
}

ProgChange2Str::ProgChange2Str(const PdArgs& a)
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

void ProgChange2Str::onFloat(float v)
{
    int val = v;

    if (!(0 < val && val < 128)) {
        OBJ_ERR << "invalid program change value:" << v;
        return;
    }

    t_symbol* name = family_->value() ? instrument_family(val) : instrument_name(val);

    if (as_symbol_->value())
        symbolTo(0, name);
    else
        dataTo(0, DataPtr(new DataTypeString(name->s_name)));
}

void setup_midi_progchange2str()
{
    ObjectFactory<ProgChange2Str> obj("midi.pg->str");
}
