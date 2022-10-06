#include "midi_common.h"
#include "ceammc_crc32.h"

#include <algorithm>
#include <utility>

using instr_entry = std::pair<const char*, uint32_t>;
#define INSTR(v) std::make_pair(v, ceammc::crc32_constexpr(v))

static const std::array<instr_entry, 128> PG_INSTR_NAMES {
    INSTR("Acoustic Grand Piano"),
    INSTR("Bright Acoustic Piano"),
    INSTR("Electric Grand Piano"),
    INSTR("Honky-tonk Piano"),
    INSTR("Electric Piano 1"),
    INSTR("Electric Piano 2"),
    INSTR("Harpsichord"),
    INSTR("Clavi"),
    INSTR("Celesta"),
    INSTR("Glockenspiel"),
    INSTR("Music Box"),
    INSTR("Vibraphone"),
    INSTR("Marimba"),
    INSTR("Xylophone"),
    INSTR("Tubular Bells"),
    INSTR("Dulcimer"),
    INSTR("Drawbar Organ"),
    INSTR("Percussive Organ"),
    INSTR("Rock Organ"),
    INSTR("Church Organ"),
    INSTR("Reed Organ"),
    INSTR("Accordion"),
    INSTR("Harmonica"),
    INSTR("Tango Accordion"),
    INSTR("Acoustic Guitar (nylon)"),
    INSTR("Acoustic Guitar (steel)"),
    INSTR("Electric Guitar (jazz)"),
    INSTR("Electric Guitar (clean)"),
    INSTR("Electric Guitar (muted)"),
    INSTR("Overdriven Guitar"),
    INSTR("Distortion Guitar"),
    INSTR("Guitar harmonics"),
    INSTR("Acoustic Bass"),
    INSTR("Electric Bass (finger)"),
    INSTR("Electric Bass (pick)"),
    INSTR("Fretless Bass"),
    INSTR("Slap Bass 1"),
    INSTR("Slap Bass 2"),
    INSTR("Synth Bass 1"),
    INSTR("Synth Bass 2"),
    INSTR("Violin"),
    INSTR("Viola"),
    INSTR("Cello"),
    INSTR("Contrabass"),
    INSTR("Tremolo Strings"),
    INSTR("Pizzicato Strings"),
    INSTR("Orchestral Harp"),
    INSTR("Timpani"),
    INSTR("String Ensemble 1"),
    INSTR("String Ensemble 2"),
    INSTR("SynthStrings 1"),
    INSTR("SynthStrings 2"),
    INSTR("Choir Aahs"),
    INSTR("Voice Oohs"),
    INSTR("Synth Voice"),
    INSTR("Orchestra Hit"),
    INSTR("Trumpet"),
    INSTR("Trombone"),
    INSTR("Tuba"),
    INSTR("Muted Trumpet"),
    INSTR("French Horn"),
    INSTR("Brass Section"),
    INSTR("SynthBrass 1"),
    INSTR("SynthBrass 2"),
    INSTR("Soprano Sax"),
    INSTR("Alto Sax"),
    INSTR("Tenor Sax"),
    INSTR("Baritone Sax"),
    INSTR("Oboe"),
    INSTR("English Horn"),
    INSTR("Bassoon"),
    INSTR("Clarinet"),
    INSTR("Piccolo"),
    INSTR("Flute"),
    INSTR("Recorder"),
    INSTR("Pan Flute"),
    INSTR("Blown Bottle"),
    INSTR("Shakuhachi"),
    INSTR("Whistle"),
    INSTR("Ocarina"),
    INSTR("Lead 1 (square)"),
    INSTR("Lead 2 (sawtooth)"),
    INSTR("Lead 3 (calliope)"),
    INSTR("Lead 4 (chiff)"),
    INSTR("Lead 5 (charang)"),
    INSTR("Lead 6 (voice)"),
    INSTR("Lead 7 (fifths)"),
    INSTR("Lead 8 (bass + lead)"),
    INSTR("Pad 1 (new age)"),
    INSTR("Pad 2 (warm)"),
    INSTR("Pad 3 (polysynth)"),
    INSTR("Pad 4 (choir)"),
    INSTR("Pad 5 (bowed)"),
    INSTR("Pad 6 (metallic)"),
    INSTR("Pad 7 (halo)"),
    INSTR("Pad 8 (sweep)"),
    INSTR("FX 1 (rain)"),
    INSTR("FX 2 (soundtrack)"),
    INSTR("FX 3 (crystal)"),
    INSTR("FX 4 (atmosphere)"),
    INSTR("FX 5 (brightness)"),
    INSTR("FX 6 (goblins)"),
    INSTR("FX 7 (echoes)"),
    INSTR("FX 8 (sci-fi)"),
    INSTR("Sitar"),
    INSTR("Banjo"),
    INSTR("Shamisen"),
    INSTR("Koto"),
    INSTR("Kalimba"),
    INSTR("Bag pipe"),
    INSTR("Fiddle"),
    INSTR("Shanai"),
    INSTR("Tinkle Bell"),
    INSTR("Agogo"),
    INSTR("Steel Drums"),
    INSTR("Woodblock"),
    INSTR("Taiko Drum"),
    INSTR("Melodic Tom"),
    INSTR("Synth Drum"),
    INSTR("Reverse Cymbal"),
    INSTR("Guitar Fret Noise"),
    INSTR("Breath Noise"),
    INSTR("Seashore"),
    INSTR("Bird Tweet"),
    INSTR("Telephone Ring"),
    INSTR("Helicopter"),
    INSTR("Applause"),
    INSTR("Gunshot"),
};

static const char* PG_INSTR_FAMILY[] = {
    "Piano",
    "Chromatic Percussion",
    "Organ",
    "Guitar",
    "Bass",
    "Strings",
    "Ensemble",
    "Brass",
    "Reed",
    "Pipe",
    "Synth Lead",
    "Synth Pad",
    "Synth Effects",
    "Ethnic",
    "Percussive",
    "Sound Effects",
};

static bool checkInstrRange(size_t v)
{
    return 0 < v && v <= 128;
}

const char* ceammc::midi::instrument_family(size_t instrIndex)
{
    static_assert(sizeof(PG_INSTR_FAMILY) / sizeof(t_symbol*) == 16, "invalid instrument family table size");

    if (!checkInstrRange(instrIndex))
        return "";

    return PG_INSTR_FAMILY[(instrIndex - 1) / 8];
}

const char* ceammc::midi::instrument_name(size_t instrIndex)
{
    if (!checkInstrRange(instrIndex))
        return "";

    return PG_INSTR_NAMES[instrIndex - 1].first;
}

size_t ceammc::midi::instrument_index(const char* name)
{
    auto hash = crc32_hash(name);
    auto b = PG_INSTR_NAMES.begin();
    auto e = PG_INSTR_NAMES.end();
    auto it = std::find_if(b, e, [hash](const instr_entry& e) { return e.second == hash; });

    return it == e ? 0 : (std::distance(b, it) + 1);
}

const char* ceammc::midi::controller_name(size_t ctrNum)
{
    // clang-format off
    static const char* CTR_NAMES[] = {
        "Bank Select",                            // 0
        "Modulation Wheel",                       // 1
        "Breath Contoller",                       // 2
        "",                                       // 3
        "Foot Controller",                        // 4
        "Portamento Time",                        // 5
        "Data Entry MSB",                         // 6
        "Main Volume",                            // 7
        "Balance",                                // 8
        "",                                       // 9
        "Pan",                                    // 10
        "Expression",                             // 11
        "Effect Control 1",                       // 12
        "Effect Control 2",                       // 13
        "",                                       // 14
        "",                                       // 15
        "General Purpose Slider 1",               // 16
        "General Purpose Slider 2",               // 17
        "General Purpose Slider 3",               // 18
        "General Purpose Slider 4",               // 19
        "",                                       // 20
        "", "", "", "", "", "", "", "", "", "",   // 30
        "",                                       // 31
        "Bank Select",                            // 32
        "Modulation Wheel",                       // 33
        "Breath Contoller",                       // 34
        "",                                       // 35
        "Foot Controller",                        // 36
        "Portamento Time",                        // 37
        "Data Entry MSB",                         // 38
        "Main Volume",                            // 39
        "Balance",                                // 40
        "",                                       // 41
        "Pan",                                    // 42
        "Expression",                             // 43
        "Effect Control 1",                       // 44
        "Effect Control 2",                       // 45
        "", "", "", "", "",                       // 50
        "", "", "", "", "", "", "", "", "", "",   // 60
        "", "", "",                               // 63
        "Hold Pedal",                             // 64
        "Portamento",                             // 65
        "Sostenuto",                              // 66
        "Soft Pedal",                             // 67
        "Legato Footswitch",                      // 68
        "Hold 2",                                 // 69
        "Sound Variation",                        // 70
        "Sound Timbre",                           // 71
        "Sound Release Time",                     // 72
        "Sound Attack Time",                      // 73
        "Sound Brightness",                       // 74
        "Sound Controller 6",                     // 75
        "Sound Controller 7",                     // 76
        "Sound Controller 8",                     // 77
        "Sound Controller 9",                     // 78
        "Sound Controller 10",                    // 79
        "General Purpose Button 1",               // 80
        "General Purpose Button 2",               // 81
        "General Purpose Button 3",               // 82
        "General Purpose Button 4",               // 83
        "Portamento Control",                     // 84
        "", "", "", "", "", "",                   // 90
        "Effects Level",                          // 91
        "Effects Tremolo",                        // 92
        "Effects Chorus",                         // 93
        "Effects Detune",                         // 94
        "Effects Phaser",                         // 95
        "Data Button Increment",                  // 96
        "Data Button Decrement",                  // 97
        "Non-Registered Parameter LSB",           // 98
        "Non-Registered Parameter MSB",           // 99
        "Registered Parameter LSB",               // 100
        "Registered Parameter MSB",               // 101
        "", "", "", "", "", "", "", "", "",       // 110
        "", "", "", "", "", "", "", "", "",       // 119
        "All Sound Off",                          // 120
        "All Contollers Off",                     // 121
        "Local Keyboard",                         // 122
        "All Notes Off",                          // 123
        "Omni Mode Off",                          // 124
        "Omni Mode On",                           // 125
        "Mono Operation",                         // 126
        "Poly Operation",                         // 127
    };

    // clang-format on

    static_assert(sizeof(CTR_NAMES) / sizeof(t_symbol*) == 128, "invalid table of controls size");

    return (ctrNum > 127) ? "" : CTR_NAMES[ctrNum];
}

std::string ceammc::midi::key_to_name(size_t key, const music::Tonality& t, bool up)
{
    using namespace ceammc::music;

    PitchClass p = Tonality::correctAlteration(key, t, up ? ALTERATE_UP : ALTERATE_DOWN);
    return to_string(p, NAMING_SCHEME_SPN);
}
