#ifndef PARSER_PROP_RAGEL_H_
#define PARSER_PROP_RAGEL_H_

# include <cstring>

namespace ceammc {
namespace parser {

%%{
    machine bool_prop;

    true =   ('T'|'t') 'rue'  %{ type = BoolPropValue::TRUE; };
    false =  ('F'|'f') 'alse' %{ type = BoolPropValue::FALSE; };
    random = 'random'         %{ type = BoolPropValue::RANDOM; };
    invert = ('!'|'~')        %{ type = BoolPropValue::INVERT; };
    bool = true | false | random | invert;

    main := bool;
    write data;
}%%

enum class BoolPropValue {
    UNKNOWN,
    TRUE,
    FALSE,
    RANDOM,
    INVERT
};

static inline BoolPropValue parse_bool_prop(const char* str)
{
    const auto len = std::strlen(str);
    if (len == 0)
        return BoolPropValue::UNKNOWN;

    int cs = 0;
    const char* p = str;
    const char* pe = p + len;
    const char* eof = pe;
    BoolPropValue type = BoolPropValue::UNKNOWN;

    %% write init;
    %% write exec;

    const bool ok = cs >= %%{ write first_final; }%%;
    if (ok)
        return type;
    else
        return BoolPropValue::UNKNOWN;
}


}
}

#endif // PARSER_PROP_RAGEL_H_


