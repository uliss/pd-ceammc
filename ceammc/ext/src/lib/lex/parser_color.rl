# include "parser_color.h"

# include <cstdint>
# include <cstring>

namespace ceammc {
namespace parser {

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


