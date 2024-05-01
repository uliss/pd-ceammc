# include "parser_guido.h"
# include "fmt/core.h"
# include "ceammc_log.h"

namespace ceammc {
namespace parser {

%%{
    machine guido_note_parser;

    alt_sharps_0 = '#'  {1,2} ${ ++alt; };
    alt_flats_0  = '&'  {1,2} ${ --alt; };
    note_names   = 'a'   %{ pc = music::PitchClass::A; } ('is' %{ ++alt; })? |
                   'b'   %{ pc = music::PitchClass::B; } |
                   'c'   ${ pc = music::PitchClass::C; } ('is' %{ ++alt; })? |
                   'd'   ${ pc = music::PitchClass::D; } ('is' %{ ++alt; })? |
                   'e'   %{ pc = music::PitchClass::E; } |
                   'f'   ${ pc = music::PitchClass::F; } ('is' %{ ++alt; })? |
                   'g'   %{ pc = music::PitchClass::G; } ('is' %{ ++alt; })? |
                   'h'   %{ pc = music::PitchClass::B; } |
                   'do'  %{ pc = music::PitchClass::C; } |
                   're'  %{ pc = music::PitchClass::D; } |
                   'mi'  %{ pc = music::PitchClass::E; } |
                   'fa'  %{ pc = music::PitchClass::F; } |
                   'sol' %{ pc = music::PitchClass::G; } |
                   'la'  %{ pc = music::PitchClass::A; } |
                   'si'  %{ pc = music::PitchClass::B; } |
                   'ti'  %{ pc = music::PitchClass::B; };
    note_alt = alt_sharps_0 | alt_flats_0;
    octave  = '-'? @{ oct_sign = -1; } [0-9] @{ octave = oct_sign * (fc - '0'); };
    dots = '.'{1,3};
    duration = '/' [0-9] dots?;

    note = note_names note_alt? octave? duration?;
    rest = '_' duration?;

    main := (rest | note) 0 @{ fbreak; };
    write data;
}%%

bool parse_guido_note(const char* str, ceammc::music::PitchClass& pc, int& octave)
{
    if (str == 0) {
        return false;
    }

    int cs = 0;
    const char* p = str;

    auto alt = ceammc::music::Alteration::NATURAL;
    int oct_sign = 1;

    %% write init;
    %% write exec noend;

    const auto ok = cs >= %%{ write first_final; }%%;

    if (ok) {
        pc.setAlteration(alt);
        return true;
    } else {
        return false;
    }
}

}
}
