
#line 1 "lex/parser_props.rl"
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


#line 30 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_props.h"
static const int bool_prop_start = 1;
static const int bool_prop_first_final = 19;
static const int bool_prop_error = 0;

static const int bool_prop_en_main = 1;


#line 38 "lex/parser_props.rl"


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

    
#line 63 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_props.h"
	{
	cs = bool_prop_start;
	}

#line 62 "lex/parser_props.rl"
    
#line 70 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_props.h"
	{
	if ( p == pe )
		goto _test_eof;
	switch ( cs )
	{
case 1:
	switch( (*p) ) {
		case 33: goto st19;
		case 70: goto st2;
		case 84: goto st6;
		case 100: goto st9;
		case 102: goto st2;
		case 114: goto st14;
		case 116: goto st6;
		case 126: goto st19;
	}
	goto st0;
st0:
cs = 0;
	goto _out;
st19:
	if ( ++p == pe )
		goto _test_eof19;
case 19:
	goto st0;
st2:
	if ( ++p == pe )
		goto _test_eof2;
case 2:
	if ( (*p) == 97 )
		goto st3;
	goto st0;
st3:
	if ( ++p == pe )
		goto _test_eof3;
case 3:
	if ( (*p) == 108 )
		goto st4;
	goto st0;
st4:
	if ( ++p == pe )
		goto _test_eof4;
case 4:
	if ( (*p) == 115 )
		goto st5;
	goto st0;
st5:
	if ( ++p == pe )
		goto _test_eof5;
case 5:
	if ( (*p) == 101 )
		goto st20;
	goto st0;
st20:
	if ( ++p == pe )
		goto _test_eof20;
case 20:
	goto st0;
st6:
	if ( ++p == pe )
		goto _test_eof6;
case 6:
	if ( (*p) == 114 )
		goto st7;
	goto st0;
st7:
	if ( ++p == pe )
		goto _test_eof7;
case 7:
	if ( (*p) == 117 )
		goto st8;
	goto st0;
st8:
	if ( ++p == pe )
		goto _test_eof8;
case 8:
	if ( (*p) == 101 )
		goto st21;
	goto st0;
st21:
	if ( ++p == pe )
		goto _test_eof21;
case 21:
	goto st0;
st9:
	if ( ++p == pe )
		goto _test_eof9;
case 9:
	if ( (*p) == 101 )
		goto st10;
	goto st0;
st10:
	if ( ++p == pe )
		goto _test_eof10;
case 10:
	if ( (*p) == 102 )
		goto st22;
	goto st0;
st22:
	if ( ++p == pe )
		goto _test_eof22;
case 22:
	if ( (*p) == 97 )
		goto st11;
	goto st0;
st11:
	if ( ++p == pe )
		goto _test_eof11;
case 11:
	if ( (*p) == 117 )
		goto st12;
	goto st0;
st12:
	if ( ++p == pe )
		goto _test_eof12;
case 12:
	if ( (*p) == 108 )
		goto st13;
	goto st0;
st13:
	if ( ++p == pe )
		goto _test_eof13;
case 13:
	if ( (*p) == 116 )
		goto st23;
	goto st0;
st23:
	if ( ++p == pe )
		goto _test_eof23;
case 23:
	goto st0;
st14:
	if ( ++p == pe )
		goto _test_eof14;
case 14:
	if ( (*p) == 97 )
		goto st15;
	goto st0;
st15:
	if ( ++p == pe )
		goto _test_eof15;
case 15:
	if ( (*p) == 110 )
		goto st16;
	goto st0;
st16:
	if ( ++p == pe )
		goto _test_eof16;
case 16:
	if ( (*p) == 100 )
		goto st17;
	goto st0;
st17:
	if ( ++p == pe )
		goto _test_eof17;
case 17:
	if ( (*p) == 111 )
		goto st18;
	goto st0;
st18:
	if ( ++p == pe )
		goto _test_eof18;
case 18:
	if ( (*p) == 109 )
		goto st24;
	goto st0;
st24:
	if ( ++p == pe )
		goto _test_eof24;
case 24:
	goto st0;
	}
	_test_eof19: cs = 19; goto _test_eof; 
	_test_eof2: cs = 2; goto _test_eof; 
	_test_eof3: cs = 3; goto _test_eof; 
	_test_eof4: cs = 4; goto _test_eof; 
	_test_eof5: cs = 5; goto _test_eof; 
	_test_eof20: cs = 20; goto _test_eof; 
	_test_eof6: cs = 6; goto _test_eof; 
	_test_eof7: cs = 7; goto _test_eof; 
	_test_eof8: cs = 8; goto _test_eof; 
	_test_eof21: cs = 21; goto _test_eof; 
	_test_eof9: cs = 9; goto _test_eof; 
	_test_eof10: cs = 10; goto _test_eof; 
	_test_eof22: cs = 22; goto _test_eof; 
	_test_eof11: cs = 11; goto _test_eof; 
	_test_eof12: cs = 12; goto _test_eof; 
	_test_eof13: cs = 13; goto _test_eof; 
	_test_eof23: cs = 23; goto _test_eof; 
	_test_eof14: cs = 14; goto _test_eof; 
	_test_eof15: cs = 15; goto _test_eof; 
	_test_eof16: cs = 16; goto _test_eof; 
	_test_eof17: cs = 17; goto _test_eof; 
	_test_eof18: cs = 18; goto _test_eof; 
	_test_eof24: cs = 24; goto _test_eof; 

	_test_eof: {}
	if ( p == eof )
	{
	switch ( cs ) {
	case 21: 
#line 29 "lex/parser_props.rl"
	{ type = BoolPropOp::TRUE; }
	break;
	case 20: 
#line 30 "lex/parser_props.rl"
	{ type = BoolPropOp::FALSE; }
	break;
	case 24: 
#line 31 "lex/parser_props.rl"
	{ type = BoolPropOp::RANDOM; }
	break;
	case 19: 
#line 32 "lex/parser_props.rl"
	{ type = BoolPropOp::INVERT; }
	break;
	case 22: 
	case 23: 
#line 33 "lex/parser_props.rl"
	{ type = BoolPropOp::DEFAULT; }
	break;
#line 292 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_props.h"
	}
	}

	_out: {}
	}

#line 63 "lex/parser_props.rl"

    const bool ok = cs >= 19;
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


#line 363 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_props.h"
static const int numeric_prop_start = 1;
static const int numeric_prop_first_final = 12;
static const int numeric_prop_error = 0;

static const int numeric_prop_en_main = 1;


#line 139 "lex/parser_props.rl"


static const char* float_prop_expected()
{
    return "float or operation (+|-|*|/|%|random|defaut)";
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

    
#line 392 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_props.h"
	{
	cs = numeric_prop_start;
	}

#line 159 "lex/parser_props.rl"
    
#line 399 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_props.h"
	{
	if ( p == pe )
		goto _test_eof;
	switch ( cs )
	{
case 1:
	switch( (*p) ) {
		case 37: goto st12;
		case 42: goto st13;
		case 43: goto st14;
		case 45: goto st15;
		case 47: goto st16;
		case 100: goto st2;
		case 114: goto st7;
	}
	goto st0;
st0:
cs = 0;
	goto _out;
st12:
	if ( ++p == pe )
		goto _test_eof12;
case 12:
	goto st0;
st13:
	if ( ++p == pe )
		goto _test_eof13;
case 13:
	goto st0;
st14:
	if ( ++p == pe )
		goto _test_eof14;
case 14:
	goto st0;
st15:
	if ( ++p == pe )
		goto _test_eof15;
case 15:
	goto st0;
st16:
	if ( ++p == pe )
		goto _test_eof16;
case 16:
	goto st0;
st2:
	if ( ++p == pe )
		goto _test_eof2;
case 2:
	if ( (*p) == 101 )
		goto st3;
	goto st0;
st3:
	if ( ++p == pe )
		goto _test_eof3;
case 3:
	if ( (*p) == 102 )
		goto st17;
	goto st0;
st17:
	if ( ++p == pe )
		goto _test_eof17;
case 17:
	if ( (*p) == 97 )
		goto st4;
	goto st0;
st4:
	if ( ++p == pe )
		goto _test_eof4;
case 4:
	if ( (*p) == 117 )
		goto st5;
	goto st0;
st5:
	if ( ++p == pe )
		goto _test_eof5;
case 5:
	if ( (*p) == 108 )
		goto st6;
	goto st0;
st6:
	if ( ++p == pe )
		goto _test_eof6;
case 6:
	if ( (*p) == 116 )
		goto st18;
	goto st0;
st18:
	if ( ++p == pe )
		goto _test_eof18;
case 18:
	goto st0;
st7:
	if ( ++p == pe )
		goto _test_eof7;
case 7:
	if ( (*p) == 97 )
		goto st8;
	goto st0;
st8:
	if ( ++p == pe )
		goto _test_eof8;
case 8:
	if ( (*p) == 110 )
		goto st9;
	goto st0;
st9:
	if ( ++p == pe )
		goto _test_eof9;
case 9:
	if ( (*p) == 100 )
		goto st10;
	goto st0;
st10:
	if ( ++p == pe )
		goto _test_eof10;
case 10:
	if ( (*p) == 111 )
		goto st11;
	goto st0;
st11:
	if ( ++p == pe )
		goto _test_eof11;
case 11:
	if ( (*p) == 109 )
		goto st19;
	goto st0;
st19:
	if ( ++p == pe )
		goto _test_eof19;
case 19:
	goto st0;
	}
	_test_eof12: cs = 12; goto _test_eof; 
	_test_eof13: cs = 13; goto _test_eof; 
	_test_eof14: cs = 14; goto _test_eof; 
	_test_eof15: cs = 15; goto _test_eof; 
	_test_eof16: cs = 16; goto _test_eof; 
	_test_eof2: cs = 2; goto _test_eof; 
	_test_eof3: cs = 3; goto _test_eof; 
	_test_eof17: cs = 17; goto _test_eof; 
	_test_eof4: cs = 4; goto _test_eof; 
	_test_eof5: cs = 5; goto _test_eof; 
	_test_eof6: cs = 6; goto _test_eof; 
	_test_eof18: cs = 18; goto _test_eof; 
	_test_eof7: cs = 7; goto _test_eof; 
	_test_eof8: cs = 8; goto _test_eof; 
	_test_eof9: cs = 9; goto _test_eof; 
	_test_eof10: cs = 10; goto _test_eof; 
	_test_eof11: cs = 11; goto _test_eof; 
	_test_eof19: cs = 19; goto _test_eof; 

	_test_eof: {}
	if ( p == eof )
	{
	switch ( cs ) {
	case 14: 
#line 128 "lex/parser_props.rl"
	{ type = NumericPropOp::ADD; }
	break;
	case 15: 
#line 129 "lex/parser_props.rl"
	{ type = NumericPropOp::SUB; }
	break;
	case 13: 
#line 130 "lex/parser_props.rl"
	{ type = NumericPropOp::MUL; }
	break;
	case 16: 
#line 131 "lex/parser_props.rl"
	{ type = NumericPropOp::DIV; }
	break;
	case 12: 
#line 132 "lex/parser_props.rl"
	{ type = NumericPropOp::MOD; }
	break;
	case 19: 
#line 133 "lex/parser_props.rl"
	{ type = NumericPropOp::RANDOM; }
	break;
	case 17: 
	case 18: 
#line 134 "lex/parser_props.rl"
	{ type = NumericPropOp::DEFAULT; }
	break;
#line 584 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_props.h"
	}
	}

	_out: {}
	}

#line 160 "lex/parser_props.rl"

    const bool ok = cs >= 12;
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

static t_float numeric_prop_calc_random(t_float min, t_float max)
{
    std::random_device rnd;
    std::uniform_real_distribution<t_float> dist(min, max);
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

    if (lv.size() == 1 && lv[0].isA<T>()) {
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
            if (lv.size() == 2 && lv[1].isA<T>()) {
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


