#include "midi_common.h"

#include <algorithm>
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

// clang-format off
static t_symbol* CTR_NAMES[] = {
    gensym("Bank Select"),                            // 0
    gensym("Modulation Wheel"),                       // 1
    gensym("Breath Contoller"),                       // 2
    &s_,                                              // 3
    gensym("Foot Controller"),                        // 4
    gensym("Portamento Time"),                        // 5
    gensym("Data Entry MSB"),                         // 6
    gensym("Main Volume"),                            // 7
    gensym("Balance"),                                // 8
    &s_,                                              // 9
    gensym("Pan"),                                    // 10
    gensym("Expression"),                             // 11
    gensym("Effect Control 1"),                       // 12
    gensym("Effect Control 2"),                       // 13
    &s_,                                              // 14
    &s_,                                              // 15
    gensym("General Purpose Slider 1"),               // 16
    gensym("General Purpose Slider 2"),               // 17
    gensym("General Purpose Slider 3"),               // 18
    gensym("General Purpose Slider 4"),               // 19
    &s_,                                              // 20
    &s_, &s_, &s_, &s_, &s_, &s_, &s_, &s_, &s_, &s_, // 30
    &s_,                                              // 31
    gensym("Bank Select"),                            // 32
    gensym("Modulation Wheel"),                       // 33
    gensym("Breath Contoller"),                       // 34
    &s_,                                              // 35
    gensym("Foot Controller"),                        // 36
    gensym("Portamento Time"),                        // 37
    gensym("Data Entry MSB"),                         // 38
    gensym("Main Volume"),                            // 39
    gensym("Balance"),                                // 40
    &s_,                                              // 41
    gensym("Pan"),                                    // 42
    gensym("Expression"),                             // 43
    gensym("Effect Control 1"),                       // 44
    gensym("Effect Control 2"),                       // 45
    &s_, &s_, &s_, &s_, &s_,                          // 50
    &s_, &s_, &s_, &s_, &s_, &s_, &s_, &s_, &s_, &s_, // 60
    &s_, &s_, &s_,                                    // 63
    gensym("Hold Pedal"),                             // 64
    gensym("Portamento"),                             // 65
    gensym("Sostenuto"),                              // 66
    gensym("Soft Pedal"),                             // 67
    gensym("Legato Footswitch"),                      // 68
    gensym("Hold 2"),                                 // 69
    gensym("Sound Variation"),                        // 70
    gensym("Sound Timbre"),                           // 71
    gensym("Sound Release Time"),                     // 72
    gensym("Sound Attack Time"),                      // 73
    gensym("Sound Brightness"),                       // 74
    gensym("Sound Controller 6"),                     // 75
    gensym("Sound Controller 7"),                     // 76
    gensym("Sound Controller 8"),                     // 77
    gensym("Sound Controller 9"),                     // 78
    gensym("Sound Controller 10"),                    // 79
    gensym("General Purpose Button 1"),               // 80
    gensym("General Purpose Button 2"),               // 81
    gensym("General Purpose Button 3"),               // 82
    gensym("General Purpose Button 4"),               // 83
    gensym("Portamento Control"),                     // 84
    &s_, &s_, &s_, &s_, &s_, &s_,                     // 90
    gensym("Effects Level"),                          // 91
    gensym("Effects Tremolo"),                        // 92
    gensym("Effects Chorus"),                         // 93
    gensym("Effects Detune"),                         // 94
    gensym("Effects Phaser"),                         // 95
    gensym("Data Button Increment"),                  // 96
    gensym("Data Button Decrement"),                  // 97
    gensym("Non-Registered Parameter LSB"),           // 98
    gensym("Non-Registered Parameter MSB"),           // 99
    gensym("Registered Parameter LSB"),               // 100
    gensym("Registered Parameter MSB"),               // 101
    &s_, &s_, &s_, &s_, &s_, &s_, &s_, &s_, &s_,      // 110
    &s_, &s_, &s_, &s_, &s_, &s_, &s_, &s_, &s_,      // 119
    gensym("All Sound Off"),                          // 120
    gensym("All Contollers Off"),                     // 121
    gensym("Local Keyboard"),                         // 122
    gensym("All Notes Off"),                          // 123
    gensym("Omni Mode Off"),                          // 124
    gensym("Omni Mode On"),                           // 125
    gensym("Mono Operation"),                         // 126
    gensym("Poly Operation"),                         // 127
};

// clang-format on

BOOST_STATIC_ASSERT(sizeof(PG_INSTR_NAMES) / sizeof(t_symbol*) == 128);
BOOST_STATIC_ASSERT(sizeof(PG_INSTR_FAMILY) / sizeof(t_symbol*) == 16);
BOOST_STATIC_ASSERT(sizeof(CTR_NAMES) / sizeof(t_symbol*) == 128);

static bool checkInstrRange(size_t v)
{
    return 0 < v && v <= 128;
}

t_symbol* ceammc::midi::instrument_family(size_t instrIndex)
{
    if (!checkInstrRange(instrIndex))
        return &s_;

    return PG_INSTR_FAMILY[(instrIndex - 1) / 8];
}

t_symbol* ceammc::midi::instrument_name(size_t instrIndex)
{
    if (!checkInstrRange(instrIndex))
        return &s_;

    return PG_INSTR_NAMES[instrIndex - 1];
}

size_t ceammc::midi::instrument_index(t_symbol* name)
{
    t_symbol** begin = &PG_INSTR_NAMES[0];
    t_symbol** end = begin + 128;

    t_symbol** p = std::find(begin, end, name);

    return p == end ? 0 : (std::distance(begin, p) + 1);
}

t_symbol* ceammc::midi::controller_name(size_t ctrNum)
{
    return (ctrNum > 127) ? &s_ : CTR_NAMES[ctrNum];
}

t_symbol* ceammc::midi::key_to_name(size_t key, const Tonality& t, bool up)
{
}
