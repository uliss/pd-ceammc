# include "parser_color.h"

# include <cstdint>
# include <cstring>

namespace ceammc {
namespace parser {

static uint8_t xchar2digit(char c)
{
    switch (c) {
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
        return c - '0';
    case 'A':
    case 'B':
    case 'C':
    case 'D':
    case 'E':
    case 'F':
        return c - 'A' + 10;
    default:
        return c - 'a' + 10;
    }
}

%%{
    machine color;
    include color_common "ragel_color.rl";

    main := color_rgb;
    write data;
}%%

RgbHexFullMatch::RgbHexFullMatch()
{
    reset();
}

void RgbHexFullMatch::reset()
{
    color_ = {};
}

bool RgbHexFullMatch::parse(const Atom& a)
{
    if (a.isSymbol())
        return parse(a.asT<t_symbol*>()->s_name);
    else
        return false;
}

bool RgbHexFullMatch::parse(const char* str)
{
    const auto len = strlen(str);
    if (len == 0)
        return false;

    const char* p = str;
    const char* pe = p + len;
    const char* eof = pe;
    ColorRagelData color;
    AtomCategory cat_ {CAT_UNKNOWN};
    AtomType type_ = {TYPE_UNKNOWN};

    reset();

    %% write init;
    %% write exec;

    const auto ok = cs >= %%{ write first_final; }%%;
    if(ok) {
        color_.r = color.r;
        color_.g = color.g;
        color_.b = color.b;
    }

    return ok;
}

size_t RgbHexFullMatch::parse(const AtomListView& lv, SmallColorVec& out)
{
    const size_t N = lv.size();

    for (size_t i = 0; i < N; i++) {
        const auto& a = lv[i];
        if (!parse(a))
            return i;

        out.push_back(asInt());
    }

    return N;
}

}
}


