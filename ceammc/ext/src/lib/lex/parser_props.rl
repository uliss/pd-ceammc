#ifndef PARSER_PROP_RAGEL_H_
#define PARSER_PROP_RAGEL_H_

# include <cstring>

namespace ceammc {
namespace parser {

%%{
    machine bool_prop;

    true =   (('T'|'t') 'rue')  %{ type = BoolPropOp::TRUE; };
    false =  (('F'|'f') 'alse') %{ type = BoolPropOp::FALSE; };
    random = 'random'           %{ type = BoolPropOp::RANDOM; };
    invert = ('!'|'~')          %{ type = BoolPropOp::INVERT; };
    default = ('def'|'default') %{ type = BoolPropOp::DEFAULT; };
    bool = true | false | random | invert | default;

    main := bool;
    write data;
}%%

enum class BoolPropOp {
    UNKNOWN,
    TRUE,
    FALSE,
    RANDOM,
    INVERT,
    DEFAULT
};

static inline BoolPropOp parse_bool_prop(const char* str)
{
    const auto len = std::strlen(str);
    if (len == 0)
        return BoolPropOp::UNKNOWN;

    int cs = 0;
    const char* p = str;
    const char* pe = p + len;
    const char* eof = pe;
    BoolPropOp type = BoolPropOp::UNKNOWN;

    %% write init;
    %% write exec;

    const bool ok = cs >= %%{ write first_final; }%%;
    if (ok)
        return type;
    else
        return BoolPropOp::UNKNOWN;
}

enum class NumericPropOp {
    UNKNOWN,
    ADD,
    SUB,
    MUL,
    DIV,
    MOD,
    DEFAULT,
    RANDOM
};

%%{
    machine numeric_prop;

    add = '+'  %{ type = NumericPropOp::ADD; };
    sub = '-'  %{ type = NumericPropOp::SUB; };
    mul = '*'  %{ type = NumericPropOp::MUL; };
    div = '/'  %{ type = NumericPropOp::DIV; };
    mod = '%'  %{ type = NumericPropOp::MOD; };
    random = 'random'           %{ type = NumericPropOp::RANDOM; };
    default = ('def'|'default') %{ type = NumericPropOp::DEFAULT; };
    numeric = random | default | add | sub | mul | div | mod;

    main := numeric;
    write data;
}%%

static inline NumericPropOp parse_numeric_prop_op(const char* str)
{
    const auto len = std::strlen(str);
    if (len == 0)
        return NumericPropOp::UNKNOWN;

    int cs = 0;
    const char* p = str;
    const char* pe = p + len;
    const char* eof = pe;
    NumericPropOp type = NumericPropOp::UNKNOWN;

    %% write init;
    %% write exec;

    const bool ok = cs >= %%{ write first_final; }%%;
    if (ok)
        return type;
    else
        return NumericPropOp::UNKNOWN;
}

}
}

#endif // PARSER_PROP_RAGEL_H_


