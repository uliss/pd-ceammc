#ifndef PARSER_PROP_RAGEL_H_
#define PARSER_PROP_RAGEL_H_

# include "ceammc_atomlist_view.h"

# include <algorithm>
# include <cmath>
# include <cstdlib>
# include <cstring>
# include <iostream>
# include <limits>
# include <random>

namespace ceammc {
namespace parser {

enum class PropParseRes {
    OK,
    UNKNOWN,
    NORANGE,
    INVALID_RANDOM_ARGS,
    DIVBYZERO,
    UNKNOWN_OP
};

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

static BoolPropOp parse_bool_prop(const char* str)
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

static const char* bool_prop_expected()
{
    return "bool value (1|0|true|false) or operation (random|defaut|!|~)";
}

static PropParseRes bool_prop_calc(bool prev, bool def, const AtomListView& lv, bool& res)
{
    if (lv.empty())
        return PropParseRes::UNKNOWN;

    if (lv.isBool()) {
        res = lv[0].asBool();
        return PropParseRes::OK;
    } else if (lv.isSymbol()) {

        auto s = lv[0].asT<t_symbol*>();
        auto op = parse_bool_prop(s->s_name);

        switch (op) {
        case BoolPropOp::TRUE:
            res = true;
            break;
        case BoolPropOp::FALSE:
            res = false;
            break;
        case BoolPropOp::INVERT:
            res = !prev;
            break;
        case BoolPropOp::DEFAULT:
            res  = def;
            break;
        case BoolPropOp::RANDOM:
            res = std::rand() & 0x1;
            break;
        default:
            return PropParseRes::UNKNOWN;
        }

        return PropParseRes::OK;
    } else
        return PropParseRes::UNKNOWN;
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

static const char* float_prop_expected()
{
    return "float or operation (+|-|*|/|%|random|defaut)";
}

static const char* int_prop_expected()
{
    return "int or operation (+|-|*|/|%|random|defaut)";
}

static NumericPropOp parse_numeric_prop_op(const char* str)
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

static bool numeric_prop_random_range(const PropertyInfo& info, t_float& new_min, t_float& new_max)
{
    constexpr auto max_float = std::numeric_limits<t_float>::max();
    constexpr auto min_float = -std::numeric_limits<t_float>::max();

    auto c = info.constraints();
    switch (c) {
    case PropValueConstraints::CLOSED_OPEN_RANGE:
        new_min = info.minFloat();
        new_max = info.maxFloat();
        return true;
    case PropValueConstraints::CLOSED_RANGE:
        new_min = info.minFloat();
        new_max = std::nextafter(info.maxFloat(), max_float);
        return true;
    case PropValueConstraints::OPEN_RANGE:
        new_min = std::nextafter(info.minFloat(), min_float);
        new_max = info.maxFloat();
        return true;
    case PropValueConstraints::OPEN_CLOSED_RANGE:
        new_min = std::nextafter(info.minFloat(), min_float);
        new_max = std::nextafter(info.maxFloat(), max_float);
        return true;
    default:
        return false;
    }
}

static bool numeric_prop_random_range(const PropertyInfo& info, int& new_min, int& new_max)
{
    switch (info.constraints()) {
    case PropValueConstraints::CLOSED_RANGE:
        new_min = info.minInt();
        new_max = info.maxInt();
        return true;
    case PropValueConstraints::OPEN_RANGE:
        new_min = info.minInt() + 1;
        new_max = info.maxInt() - 1;
        return true;
    case PropValueConstraints::OPEN_CLOSED_RANGE:
        new_min = info.minInt() + 1;
        new_max = info.maxInt();
        return true;
    case PropValueConstraints::CLOSED_OPEN_RANGE:
        new_min = info.minInt();
        new_max = info.maxInt() - 1;
        return true;
    default:
        return false;
    }
}

static t_float numeric_prop_calc_random(t_float min, t_float max)
{
    std::random_device rnd;
    std::uniform_real_distribution<t_float> dist(min, max);
    return dist(rnd);
}

static int numeric_prop_calc_random(int min, int max)
{
    std::random_device rnd;
    std::uniform_int_distribution<int> dist(min, max);
    return dist(rnd);
}

template <typename T>
static PropParseRes numeric_prop_calc_math(NumericPropOp op, T arg, T prev, T& res)
{
    switch (op) {
    case NumericPropOp::ADD:
        res = prev + arg;
        return PropParseRes::OK;
    case NumericPropOp::SUB:
        res = prev - arg;
        return PropParseRes::OK;
    case NumericPropOp::MUL:
        res = prev * arg;
        return PropParseRes::OK;
    case NumericPropOp::DIV: {
        if (arg == 0)
            return PropParseRes::DIVBYZERO;
        res = prev / arg;
        return PropParseRes::OK;
    }
    case NumericPropOp::MOD: {
        if (arg == 0)
            return PropParseRes::DIVBYZERO;
        res = std::fmod(prev, arg);
        return PropParseRes::OK;
    }
    default:
        return PropParseRes::UNKNOWN_OP;
    }
}

template <typename T>
static PropParseRes numeric_prop_calc(T prev, const PropertyInfo& info, const AtomListView& lv, T& res)
{
    if (lv.empty())
        return PropParseRes::UNKNOWN;

    if (lv.size() == 1 && lv[0].isFloat()) {
        res = lv[0].asT<T>();
        return PropParseRes::OK;
    } else if (lv[0].isSymbol()) {

        auto s = lv[0].asT<t_symbol*>();
        auto op = parse_numeric_prop_op(s->s_name);

        switch (op) {
        case NumericPropOp::DEFAULT: {
            T def = 0;
            if (!info.getDefault(def))
                return PropParseRes::UNKNOWN;

            res = def;
            return PropParseRes::OK;
        }
        case NumericPropOp::RANDOM: {
            T new_min = 0;
            T new_max = 0;

            if (!numeric_prop_random_range(info, new_min, new_max))
                return PropParseRes::NORANGE;

            const bool void_random_args = (lv.size() == 1);
            const bool random_args = (lv.size() == 3 && lv[1].isA<T>() && lv[2].isA<T>());

            if (!void_random_args && !random_args)
                return PropParseRes::INVALID_RANDOM_ARGS;

            if (random_args) {
                new_min = std::max(new_min, lv[1].asT<T>());
                new_max = std::min(new_max, lv[2].asT<T>());
            }

            res = numeric_prop_calc_random(new_min, new_max);
            return PropParseRes::OK;
        } break;
        case NumericPropOp::UNKNOWN:
            return PropParseRes::UNKNOWN;
        default: // math
            if (lv.size() == 2 && lv[1].isFloat()) {
                const T arg = lv[1].asT<T>();
                return numeric_prop_calc_math(op, arg, prev, res);
            } else
                return PropParseRes::UNKNOWN;

        } // end switch

    } else {
        return PropParseRes::UNKNOWN;
    }
}

}
}

#endif // PARSER_PROP_RAGEL_H_


