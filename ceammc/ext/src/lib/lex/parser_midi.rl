# include "parser_midi.h"

# include <cstdint>
# include <cstring>
# include <limits>

namespace ceammc {
namespace parser {

%%{
    machine midi;
    include midi_common "ragel_midi.rl";

    main := midi_hex 0 @{ fbreak; };
    write data;
}%%

static char xchar2digit(char ch)
{
    if (ch >= '0' && ch <= '9') return ch - '0';
    if (ch >= 'A' && ch <= 'F') return ch - 'A' + 10;
    if (ch >= 'a' && ch <= 'f') return ch - 'a' + 10;
    return 0;
}

bool parse_midi_byte_hex(const Atom& a, std::uint8_t& res)
{
    if (a.isFloat()) {
        int x = (int)a.asT<t_float>();
        res = (x / 10)*16 + (x % 10);
        return true;
    } else if (!a.isSymbol())
        return false;

    std::uint8_t midi_byte = 0;
    int cs = 0;
    const char* p = a.asT<t_symbol*>()->s_name;

    %% write init;
    %% write exec noend;

    if(cs >= %%{ write first_final; }%%) {
        res = midi_byte;
        return true;
    } else
        return false;
}

}
}


