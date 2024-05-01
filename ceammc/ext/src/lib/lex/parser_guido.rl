# include "parser_guido.h"
# include "fmt/core.h"
# include "ceammc_log.h"

namespace ceammc {
namespace parser {

%%{
    machine guido_note_parser;

    alt_sharps   = '#'  {1,2} ${ ++alt; };
    alt_flats    = '&'  {1,2} ${ --alt; };
    note_names   = 'a'   %{ pitch = music::PitchClass::A; } ('is' %{ ++alt; })? |
                   'b'   %{ pitch = music::PitchClass::B; } |
                   'c'   ${ pitch = music::PitchClass::C; } ('is' %{ ++alt; })? |
                   'd'   ${ pitch = music::PitchClass::D; } ('is' %{ ++alt; })? |
                   'e'   %{ pitch = music::PitchClass::E; } |
                   'f'   ${ pitch = music::PitchClass::F; } ('is' %{ ++alt; })? |
                   'g'   %{ pitch = music::PitchClass::G; } ('is' %{ ++alt; })? |
                   'h'   %{ pitch = music::PitchClass::B; } |
                   'do'  %{ pitch = music::PitchClass::C; } |
                   're'  %{ pitch = music::PitchClass::D; } |
                   'mi'  %{ pitch = music::PitchClass::E; } |
                   'fa'  %{ pitch = music::PitchClass::F; } |
                   'sol' %{ pitch = music::PitchClass::G; } |
                   'la'  %{ pitch = music::PitchClass::A; } |
                   'si'  %{ pitch = music::PitchClass::B; } |
                   'ti'  %{ pitch = music::PitchClass::B; };
    note_alt = alt_sharps | alt_flats;
    octave  = '-'? @{ oct_sign = -1; } [0-9] @{ oct = oct_sign * (fc - '0'); };
    dots = '.'{1,3};
    duration = '/' [0-9] dots?;

    note = note_names note_alt? octave? duration?;
    rest = '_' duration?;

    main := (rest | note) 0 @{ fbreak; };
    write data;
}%%

bool parse_guido_note(const char* str, ceammc::music::PitchClass& pc, int& octave, music::Duration& d)
{
    if (str == 0) {
        return false;
    }

    int cs = 0;
    const char* p = str;

    auto pitch = music::PitchClass::C;
    auto alt = ceammc::music::Alteration::NATURAL;
    int oct = octave;
    int oct_sign = 1;
    auto dur = d;

    %% write init;
    %% write exec noend;

    const auto ok = cs >= %%{ write first_final; }%%;

    if (ok) {
        pitch.setAlteration(alt);
        pc = pitch;
        octave = oct;
        d = dur;
        return true;
    } else {
        return false;
    }
}

}
}
