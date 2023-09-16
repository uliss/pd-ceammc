# include "parser_hexbeat.h"

%%{
    machine hexbeat;

    all = any - 0;
    hex = [0-9A-Fa-f] @{ push_bits(bits, hextob(fc)); nbits += 4; };
    other = all - hex;
    char = hex | other;
    prefix = '#';

    main := prefix? char* 0 @{ fbreak; };
    write data;
}%%

namespace ceammc {
namespace parser {

static char hextob(char ch)
{
    if (ch >= '0' && ch <= '9') return ch - '0';
    if (ch >= 'A' && ch <= 'F') return ch - 'A' + 10;
    if (ch >= 'a' && ch <= 'f') return ch - 'a' + 10;
    return 0;
}

static void push_bits(HexBeatResult& bits, char x)
{
    bits <<= 4;
    bits |= (0x0F & x);
}

std::uint16_t parse_hexbeat(const char* str, HexBeatResult& bits)
{
    int cs = 0;
    const char* p = str;
    if (p[0] == '0' && p[1] == 'x')
        p += 2;

    int nbits = 0;

%%{
    write init;
    write exec noend;
}%%

    const auto ok = cs >= %%{ write first_final; }%%;

    return ok ? nbits : 0;
}

}
}
