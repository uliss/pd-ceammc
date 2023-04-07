
#line 1 "lex/parser_units.rl"
# include "parser_units.h"
# include "parser_numeric.h"

# include <cstdint>
# include <cstring>
# include <cstdio>

namespace ceammc {
namespace parser {


#line 15 "lex/parser_units.cpp"
static const int units_full_start = 1;
static const int units_full_first_final = 52;
static const int units_full_error = 0;

static const int units_full_en_main = 1;


#line 17 "lex/parser_units.rl"


UnitsFullMatch::UnitsFullMatch()
{
    reset();
}

void UnitsFullMatch::reset()
{
    unit_ = { };
}

bool UnitsFullMatch::parse(const Atom& a)
{
    reset();

    if (a.isSymbol())
        return parse(a.asT<t_symbol*>()->s_name);
    else if(a.isFloat()) {
        unit_.value = a.asT<t_float>();
        if(a.isInteger())
            unit_.type = TYPE_INT;
        else
            unit_.type = TYPE_FLOAT;

        return true;
    } else
        return false;
}

bool UnitsFullMatch::parseAs(const Atom& a, AtomType t)
{
    const bool ok = parse(a);
    return ok && (unit_.type == t || unit_.type == TYPE_INT || unit_.type == TYPE_FLOAT);
}

bool UnitsFullMatch::parse(const char* str)
{
    const auto len = strlen(str);
    if (len == 0)
        return false;

    const char* p = str;
    const char* pe = p + len;
    const char* eof = pe;

    PositionType pos_ = POSITION_ABS;

    DECLARE_RAGEL_COMMON_VARS;
    DECLARE_RAGEL_NUMERIC_VARS;

    fsm::BpmData bpm;
    fsm::SmpteData smpte;

    reset();

    
#line 81 "lex/parser_units.cpp"
	{
	cs = units_full_start;
	}

#line 74 "lex/parser_units.rl"
    
#line 88 "lex/parser_units.cpp"
	{
	if ( p == pe )
		goto _test_eof;
	switch ( cs )
	{
case 1:
	switch( (*p) ) {
		case 36: goto tr0;
		case 43: goto tr2;
		case 45: goto tr2;
		case 48: goto tr3;
		case 99: goto tr6;
		case 101: goto tr7;
	}
	if ( (*p) > 53 ) {
		if ( 54 <= (*p) && (*p) <= 57 )
			goto tr5;
	} else if ( (*p) >= 49 )
		goto tr4;
	goto st0;
st0:
cs = 0;
	goto _out;
tr0:
#line 8 "lex/ragel_units.rl"
	{ ragel_type = TYPE_UNKNOWN; ragel_cat = CAT_UNKNOWN; }
#line 88 "lex/ragel_units.rl"
	{pos_ = POSITION_ABS;}
	goto st2;
st2:
	if ( ++p == pe )
		goto _test_eof2;
case 2:
#line 122 "lex/parser_units.cpp"
	switch( (*p) ) {
		case 43: goto tr8;
		case 45: goto tr8;
		case 48: goto tr9;
	}
	if ( (*p) > 53 ) {
		if ( 54 <= (*p) && (*p) <= 57 )
			goto tr11;
	} else if ( (*p) >= 49 )
		goto tr10;
	goto st0;
tr2:
#line 8 "lex/ragel_units.rl"
	{ ragel_type = TYPE_UNKNOWN; ragel_cat = CAT_UNKNOWN; }
#line 88 "lex/ragel_units.rl"
	{pos_ = POSITION_ABS;}
#line 21 "lex/ragel_numeric.rl"
	{ ragel_num.sign = ((*p)=='-') ? -1 : 1; }
#line 79 "lex/ragel_units.rl"
	{smpte.np = 0;}
#line 74 "lex/ragel_units.rl"
	{smpte.sign = ((*p)=='-')?-1:1;}
	goto st3;
tr8:
#line 82 "lex/ragel_units.rl"
	{pos_ = POSITION_END;}
#line 21 "lex/ragel_numeric.rl"
	{ ragel_num.sign = ((*p)=='-') ? -1 : 1; }
#line 79 "lex/ragel_units.rl"
	{smpte.np = 0;}
#line 74 "lex/ragel_units.rl"
	{smpte.sign = ((*p)=='-')?-1:1;}
	goto st3;
tr63:
#line 83 "lex/ragel_units.rl"
	{pos_ = POSITION_CURRENT;}
#line 21 "lex/ragel_numeric.rl"
	{ ragel_num.sign = ((*p)=='-') ? -1 : 1; }
#line 79 "lex/ragel_units.rl"
	{smpte.np = 0;}
#line 74 "lex/ragel_units.rl"
	{smpte.sign = ((*p)=='-')?-1:1;}
	goto st3;
st3:
	if ( ++p == pe )
		goto _test_eof3;
case 3:
#line 170 "lex/parser_units.cpp"
	if ( (*p) == 48 )
		goto tr12;
	if ( (*p) > 53 ) {
		if ( 54 <= (*p) && (*p) <= 57 )
			goto tr14;
	} else if ( (*p) >= 49 )
		goto tr13;
	goto st0;
tr9:
#line 82 "lex/ragel_units.rl"
	{pos_ = POSITION_END;}
#line 29 "lex/ragel_numeric.rl"
	{ (ragel_num.vint *= 10) += ((*p)-'0'); }
#line 42 "lex/ragel_numeric.rl"
	{ (ragel_num.ratio.num *= 10) += ((*p)-'0'); }
#line 79 "lex/ragel_units.rl"
	{smpte.np = 0;}
#line 67 "lex/ragel_units.rl"
	{smpte.hour = 0; smpte.np++;}
#line 67 "lex/ragel_units.rl"
	{(smpte.hour *= 10) += ((*p) - '0');}
	goto st52;
tr12:
#line 29 "lex/ragel_numeric.rl"
	{ (ragel_num.vint *= 10) += ((*p)-'0'); }
#line 42 "lex/ragel_numeric.rl"
	{ (ragel_num.ratio.num *= 10) += ((*p)-'0'); }
#line 67 "lex/ragel_units.rl"
	{smpte.hour = 0; smpte.np++;}
#line 67 "lex/ragel_units.rl"
	{(smpte.hour *= 10) += ((*p) - '0');}
	goto st52;
tr64:
#line 83 "lex/ragel_units.rl"
	{pos_ = POSITION_CURRENT;}
#line 29 "lex/ragel_numeric.rl"
	{ (ragel_num.vint *= 10) += ((*p)-'0'); }
#line 42 "lex/ragel_numeric.rl"
	{ (ragel_num.ratio.num *= 10) += ((*p)-'0'); }
#line 79 "lex/ragel_units.rl"
	{smpte.np = 0;}
#line 67 "lex/ragel_units.rl"
	{smpte.hour = 0; smpte.np++;}
#line 67 "lex/ragel_units.rl"
	{(smpte.hour *= 10) += ((*p) - '0');}
	goto st52;
st52:
	if ( ++p == pe )
		goto _test_eof52;
case 52:
#line 221 "lex/parser_units.cpp"
	switch( (*p) ) {
		case 37: goto tr69;
		case 42: goto tr70;
		case 46: goto tr71;
		case 47: goto st33;
		case 72: goto tr74;
		case 95: goto tr75;
		case 98: goto tr76;
		case 99: goto tr77;
		case 100: goto tr78;
		case 104: goto tr79;
		case 109: goto tr80;
		case 112: goto tr81;
		case 114: goto tr82;
		case 115: goto tr83;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr73;
	goto st0;
tr69:
#line 30 "lex/ragel_numeric.rl"
	{
        ragel_num.vint *= ragel_num.sign;
        ragel_type = TYPE_INT;
        ragel_cat  = CAT_NUMBER;
    }
#line 7 "lex/ragel_units.rl"
	{ ragel_num.vdouble = ragel_num.vint; }
#line 108 "lex/ragel_numeric.rl"
	{ragel_num.vdouble = ragel_num.vint;}
	goto st53;
tr84:
#line 58 "lex/ragel_numeric.rl"
	{
        const auto frac = double(ragel_num.ratio.num) / ragel_num.ratio.den;
        ragel_num.vdouble = ragel_num.vint + ragel_num.sign * frac;
        ragel_type = TYPE_FLOAT;
        ragel_cat  = CAT_NUMBER;
    }
	goto st53;
st53:
	if ( ++p == pe )
		goto _test_eof53;
case 53:
#line 266 "lex/parser_units.cpp"
	goto st0;
tr70:
#line 30 "lex/ragel_numeric.rl"
	{
        ragel_num.vint *= ragel_num.sign;
        ragel_type = TYPE_INT;
        ragel_cat  = CAT_NUMBER;
    }
#line 7 "lex/ragel_units.rl"
	{ ragel_num.vdouble = ragel_num.vint; }
#line 113 "lex/ragel_numeric.rl"
	{ragel_num.vdouble = ragel_num.vint;}
	goto st54;
tr85:
#line 58 "lex/ragel_numeric.rl"
	{
        const auto frac = double(ragel_num.ratio.num) / ragel_num.ratio.den;
        ragel_num.vdouble = ragel_num.vint + ragel_num.sign * frac;
        ragel_type = TYPE_FLOAT;
        ragel_cat  = CAT_NUMBER;
    }
	goto st54;
st54:
	if ( ++p == pe )
		goto _test_eof54;
case 54:
#line 293 "lex/parser_units.cpp"
	goto st0;
tr71:
#line 30 "lex/ragel_numeric.rl"
	{
        ragel_num.vint *= ragel_num.sign;
        ragel_type = TYPE_INT;
        ragel_cat  = CAT_NUMBER;
    }
#line 50 "lex/ragel_numeric.rl"
	{
        ragel_num.ratio.num = 0;
        ragel_num.ratio.den = 1;
    }
	goto st4;
st4:
	if ( ++p == pe )
		goto _test_eof4;
case 4:
#line 312 "lex/parser_units.cpp"
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr15;
	goto st0;
tr15:
#line 54 "lex/ragel_numeric.rl"
	{
        (ragel_num.ratio.num *= 10) += ((*p) - '0');
        ragel_num.ratio.den *= 10;
    }
	goto st55;
st55:
	if ( ++p == pe )
		goto _test_eof55;
case 55:
#line 327 "lex/parser_units.cpp"
	switch( (*p) ) {
		case 37: goto tr84;
		case 42: goto tr85;
		case 72: goto tr86;
		case 95: goto tr87;
		case 98: goto tr88;
		case 99: goto tr89;
		case 100: goto tr90;
		case 104: goto tr91;
		case 109: goto tr92;
		case 112: goto tr93;
		case 114: goto tr94;
		case 115: goto tr95;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr15;
	goto st0;
tr74:
#line 30 "lex/ragel_numeric.rl"
	{
        ragel_num.vint *= ragel_num.sign;
        ragel_type = TYPE_INT;
        ragel_cat  = CAT_NUMBER;
    }
#line 7 "lex/ragel_units.rl"
	{ ragel_num.vdouble = ragel_num.vint; }
	goto st5;
tr86:
#line 58 "lex/ragel_numeric.rl"
	{
        const auto frac = double(ragel_num.ratio.num) / ragel_num.ratio.den;
        ragel_num.vdouble = ragel_num.vint + ragel_num.sign * frac;
        ragel_type = TYPE_FLOAT;
        ragel_cat  = CAT_NUMBER;
    }
	goto st5;
st5:
	if ( ++p == pe )
		goto _test_eof5;
case 5:
#line 368 "lex/parser_units.cpp"
	if ( (*p) == 122 )
		goto st56;
	goto st0;
st56:
	if ( ++p == pe )
		goto _test_eof56;
case 56:
	goto st0;
tr75:
#line 30 "lex/ragel_numeric.rl"
	{
        ragel_num.vint *= ragel_num.sign;
        ragel_type = TYPE_INT;
        ragel_cat  = CAT_NUMBER;
    }
#line 7 "lex/ragel_units.rl"
	{ ragel_num.vdouble = ragel_num.vint; }
#line 108 "lex/ragel_numeric.rl"
	{ragel_num.vdouble = ragel_num.vint;}
#line 113 "lex/ragel_numeric.rl"
	{ragel_num.vdouble = ragel_num.vint;}
	goto st57;
tr87:
#line 58 "lex/ragel_numeric.rl"
	{
        const auto frac = double(ragel_num.ratio.num) / ragel_num.ratio.den;
        ragel_num.vdouble = ragel_num.vint + ragel_num.sign * frac;
        ragel_type = TYPE_FLOAT;
        ragel_cat  = CAT_NUMBER;
    }
	goto st57;
st57:
	if ( ++p == pe )
		goto _test_eof57;
case 57:
#line 404 "lex/parser_units.cpp"
	switch( (*p) ) {
		case 37: goto st53;
		case 42: goto st58;
		case 72: goto st5;
		case 98: goto st6;
		case 99: goto st60;
		case 100: goto st62;
		case 104: goto st67;
		case 109: goto st69;
		case 112: goto st17;
		case 114: goto st25;
		case 115: goto st76;
	}
	goto st0;
st58:
	if ( ++p == pe )
		goto _test_eof58;
case 58:
	goto st0;
tr76:
#line 30 "lex/ragel_numeric.rl"
	{
        ragel_num.vint *= ragel_num.sign;
        ragel_type = TYPE_INT;
        ragel_cat  = CAT_NUMBER;
    }
#line 7 "lex/ragel_units.rl"
	{ ragel_num.vdouble = ragel_num.vint; }
	goto st6;
tr88:
#line 58 "lex/ragel_numeric.rl"
	{
        const auto frac = double(ragel_num.ratio.num) / ragel_num.ratio.den;
        ragel_num.vdouble = ragel_num.vint + ragel_num.sign * frac;
        ragel_type = TYPE_FLOAT;
        ragel_cat  = CAT_NUMBER;
    }
	goto st6;
st6:
	if ( ++p == pe )
		goto _test_eof6;
case 6:
#line 447 "lex/parser_units.cpp"
	if ( (*p) == 112 )
		goto st7;
	goto st0;
st7:
	if ( ++p == pe )
		goto _test_eof7;
case 7:
	if ( (*p) == 109 )
		goto st59;
	goto st0;
st59:
	if ( ++p == pe )
		goto _test_eof59;
case 59:
	goto st0;
tr77:
#line 30 "lex/ragel_numeric.rl"
	{
        ragel_num.vint *= ragel_num.sign;
        ragel_type = TYPE_INT;
        ragel_cat  = CAT_NUMBER;
    }
#line 7 "lex/ragel_units.rl"
	{ ragel_num.vdouble = ragel_num.vint; }
	goto st60;
tr89:
#line 58 "lex/ragel_numeric.rl"
	{
        const auto frac = double(ragel_num.ratio.num) / ragel_num.ratio.den;
        ragel_num.vdouble = ragel_num.vint + ragel_num.sign * frac;
        ragel_type = TYPE_FLOAT;
        ragel_cat  = CAT_NUMBER;
    }
	goto st60;
st60:
	if ( ++p == pe )
		goto _test_eof60;
case 60:
#line 486 "lex/parser_units.cpp"
	if ( (*p) == 101 )
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
	if ( (*p) == 116 )
		goto st61;
	goto st0;
st61:
	if ( ++p == pe )
		goto _test_eof61;
case 61:
	goto st0;
tr78:
#line 30 "lex/ragel_numeric.rl"
	{
        ragel_num.vint *= ragel_num.sign;
        ragel_type = TYPE_INT;
        ragel_cat  = CAT_NUMBER;
    }
#line 7 "lex/ragel_units.rl"
	{ ragel_num.vdouble = ragel_num.vint; }
	goto st62;
tr90:
#line 58 "lex/ragel_numeric.rl"
	{
        const auto frac = double(ragel_num.ratio.num) / ragel_num.ratio.den;
        ragel_num.vdouble = ragel_num.vint + ragel_num.sign * frac;
        ragel_type = TYPE_FLOAT;
        ragel_cat  = CAT_NUMBER;
    }
	goto st62;
st62:
	if ( ++p == pe )
		goto _test_eof62;
case 62:
#line 532 "lex/parser_units.cpp"
	switch( (*p) ) {
		case 97: goto st10;
		case 98: goto st64;
		case 101: goto st12;
	}
	goto st0;
st10:
	if ( ++p == pe )
		goto _test_eof10;
case 10:
	if ( (*p) == 121 )
		goto st63;
	goto st0;
st63:
	if ( ++p == pe )
		goto _test_eof63;
case 63:
	goto st0;
st64:
	if ( ++p == pe )
		goto _test_eof64;
case 64:
	if ( (*p) == 102 )
		goto st11;
	goto st0;
st11:
	if ( ++p == pe )
		goto _test_eof11;
case 11:
	if ( (*p) == 115 )
		goto st65;
	goto st0;
st65:
	if ( ++p == pe )
		goto _test_eof65;
case 65:
	goto st0;
st12:
	if ( ++p == pe )
		goto _test_eof12;
case 12:
	if ( (*p) == 103 )
		goto st66;
	goto st0;
st66:
	if ( ++p == pe )
		goto _test_eof66;
case 66:
	goto st0;
tr79:
#line 30 "lex/ragel_numeric.rl"
	{
        ragel_num.vint *= ragel_num.sign;
        ragel_type = TYPE_INT;
        ragel_cat  = CAT_NUMBER;
    }
#line 7 "lex/ragel_units.rl"
	{ ragel_num.vdouble = ragel_num.vint; }
	goto st67;
tr91:
#line 58 "lex/ragel_numeric.rl"
	{
        const auto frac = double(ragel_num.ratio.num) / ragel_num.ratio.den;
        ragel_num.vdouble = ragel_num.vint + ragel_num.sign * frac;
        ragel_type = TYPE_FLOAT;
        ragel_cat  = CAT_NUMBER;
    }
	goto st67;
st67:
	if ( ++p == pe )
		goto _test_eof67;
case 67:
#line 605 "lex/parser_units.cpp"
	switch( (*p) ) {
		case 111: goto st13;
		case 122: goto st56;
	}
	goto st0;
st13:
	if ( ++p == pe )
		goto _test_eof13;
case 13:
	if ( (*p) == 117 )
		goto st14;
	goto st0;
st14:
	if ( ++p == pe )
		goto _test_eof14;
case 14:
	if ( (*p) == 114 )
		goto st68;
	goto st0;
st68:
	if ( ++p == pe )
		goto _test_eof68;
case 68:
	goto st0;
tr80:
#line 30 "lex/ragel_numeric.rl"
	{
        ragel_num.vint *= ragel_num.sign;
        ragel_type = TYPE_INT;
        ragel_cat  = CAT_NUMBER;
    }
#line 7 "lex/ragel_units.rl"
	{ ragel_num.vdouble = ragel_num.vint; }
	goto st69;
tr92:
#line 58 "lex/ragel_numeric.rl"
	{
        const auto frac = double(ragel_num.ratio.num) / ragel_num.ratio.den;
        ragel_num.vdouble = ragel_num.vint + ragel_num.sign * frac;
        ragel_type = TYPE_FLOAT;
        ragel_cat  = CAT_NUMBER;
    }
	goto st69;
st69:
	if ( ++p == pe )
		goto _test_eof69;
case 69:
#line 653 "lex/parser_units.cpp"
	switch( (*p) ) {
		case 105: goto st15;
		case 115: goto st71;
	}
	goto st0;
st15:
	if ( ++p == pe )
		goto _test_eof15;
case 15:
	if ( (*p) == 110 )
		goto st70;
	goto st0;
st70:
	if ( ++p == pe )
		goto _test_eof70;
case 70:
	goto st0;
st71:
	if ( ++p == pe )
		goto _test_eof71;
case 71:
	if ( (*p) == 101 )
		goto st16;
	goto st0;
st16:
	if ( ++p == pe )
		goto _test_eof16;
case 16:
	if ( (*p) == 99 )
		goto st72;
	goto st0;
st72:
	if ( ++p == pe )
		goto _test_eof72;
case 72:
	goto st0;
tr81:
#line 30 "lex/ragel_numeric.rl"
	{
        ragel_num.vint *= ragel_num.sign;
        ragel_type = TYPE_INT;
        ragel_cat  = CAT_NUMBER;
    }
#line 7 "lex/ragel_units.rl"
	{ ragel_num.vdouble = ragel_num.vint; }
#line 113 "lex/ragel_numeric.rl"
	{ragel_num.vdouble = ragel_num.vint;}
	goto st17;
tr93:
#line 58 "lex/ragel_numeric.rl"
	{
        const auto frac = double(ragel_num.ratio.num) / ragel_num.ratio.den;
        ragel_num.vdouble = ragel_num.vint + ragel_num.sign * frac;
        ragel_type = TYPE_FLOAT;
        ragel_cat  = CAT_NUMBER;
    }
	goto st17;
st17:
	if ( ++p == pe )
		goto _test_eof17;
case 17:
#line 715 "lex/parser_units.cpp"
	switch( (*p) ) {
		case 101: goto st18;
		case 104: goto st22;
	}
	goto st0;
st18:
	if ( ++p == pe )
		goto _test_eof18;
case 18:
	if ( (*p) == 114 )
		goto st19;
	goto st0;
st19:
	if ( ++p == pe )
		goto _test_eof19;
case 19:
	if ( (*p) == 99 )
		goto st73;
	goto st0;
st73:
	if ( ++p == pe )
		goto _test_eof73;
case 73:
	if ( (*p) == 101 )
		goto st20;
	goto st0;
st20:
	if ( ++p == pe )
		goto _test_eof20;
case 20:
	if ( (*p) == 110 )
		goto st21;
	goto st0;
st21:
	if ( ++p == pe )
		goto _test_eof21;
case 21:
	if ( (*p) == 116 )
		goto st74;
	goto st0;
st74:
	if ( ++p == pe )
		goto _test_eof74;
case 74:
	goto st0;
st22:
	if ( ++p == pe )
		goto _test_eof22;
case 22:
	if ( (*p) == 97 )
		goto st23;
	goto st0;
st23:
	if ( ++p == pe )
		goto _test_eof23;
case 23:
	if ( (*p) == 115 )
		goto st24;
	goto st0;
st24:
	if ( ++p == pe )
		goto _test_eof24;
case 24:
	if ( (*p) == 101 )
		goto st54;
	goto st0;
tr82:
#line 30 "lex/ragel_numeric.rl"
	{
        ragel_num.vint *= ragel_num.sign;
        ragel_type = TYPE_INT;
        ragel_cat  = CAT_NUMBER;
    }
#line 7 "lex/ragel_units.rl"
	{ ragel_num.vdouble = ragel_num.vint; }
	goto st25;
tr94:
#line 58 "lex/ragel_numeric.rl"
	{
        const auto frac = double(ragel_num.ratio.num) / ragel_num.ratio.den;
        ragel_num.vdouble = ragel_num.vint + ragel_num.sign * frac;
        ragel_type = TYPE_FLOAT;
        ragel_cat  = CAT_NUMBER;
    }
	goto st25;
st25:
	if ( ++p == pe )
		goto _test_eof25;
case 25:
#line 805 "lex/parser_units.cpp"
	if ( (*p) == 97 )
		goto st26;
	goto st0;
st26:
	if ( ++p == pe )
		goto _test_eof26;
case 26:
	if ( (*p) == 100 )
		goto st75;
	goto st0;
st75:
	if ( ++p == pe )
		goto _test_eof75;
case 75:
	goto st0;
tr83:
#line 30 "lex/ragel_numeric.rl"
	{
        ragel_num.vint *= ragel_num.sign;
        ragel_type = TYPE_INT;
        ragel_cat  = CAT_NUMBER;
    }
#line 7 "lex/ragel_units.rl"
	{ ragel_num.vdouble = ragel_num.vint; }
	goto st76;
tr95:
#line 58 "lex/ragel_numeric.rl"
	{
        const auto frac = double(ragel_num.ratio.num) / ragel_num.ratio.den;
        ragel_num.vdouble = ragel_num.vint + ragel_num.sign * frac;
        ragel_type = TYPE_FLOAT;
        ragel_cat  = CAT_NUMBER;
    }
	goto st76;
st76:
	if ( ++p == pe )
		goto _test_eof76;
case 76:
#line 844 "lex/parser_units.cpp"
	switch( (*p) ) {
		case 97: goto st77;
		case 101: goto st28;
	}
	goto st0;
st77:
	if ( ++p == pe )
		goto _test_eof77;
case 77:
	if ( (*p) == 109 )
		goto st27;
	goto st0;
st27:
	if ( ++p == pe )
		goto _test_eof27;
case 27:
	if ( (*p) == 112 )
		goto st78;
	goto st0;
st78:
	if ( ++p == pe )
		goto _test_eof78;
case 78:
	goto st0;
st28:
	if ( ++p == pe )
		goto _test_eof28;
case 28:
	switch( (*p) ) {
		case 99: goto st79;
		case 109: goto st29;
	}
	goto st0;
st79:
	if ( ++p == pe )
		goto _test_eof79;
case 79:
	goto st0;
st29:
	if ( ++p == pe )
		goto _test_eof29;
case 29:
	if ( (*p) == 105 )
		goto st80;
	goto st0;
st80:
	if ( ++p == pe )
		goto _test_eof80;
case 80:
	if ( (*p) == 116 )
		goto st30;
	goto st0;
st30:
	if ( ++p == pe )
		goto _test_eof30;
case 30:
	if ( (*p) == 111 )
		goto st31;
	goto st0;
st31:
	if ( ++p == pe )
		goto _test_eof31;
case 31:
	if ( (*p) == 110 )
		goto st32;
	goto st0;
st32:
	if ( ++p == pe )
		goto _test_eof32;
case 32:
	if ( (*p) == 101 )
		goto st81;
	goto st0;
st81:
	if ( ++p == pe )
		goto _test_eof81;
case 81:
	goto st0;
st33:
	if ( ++p == pe )
		goto _test_eof33;
case 33:
	if ( (*p) == 48 )
		goto tr46;
	if ( 49 <= (*p) && (*p) <= 57 )
		goto tr47;
	goto st0;
tr46:
#line 43 "lex/ragel_numeric.rl"
	{ (ragel_num.ratio.den *= 10) += ((*p)-'0'); }
	goto st82;
st82:
	if ( ++p == pe )
		goto _test_eof82;
case 82:
#line 940 "lex/parser_units.cpp"
	goto st0;
tr47:
#line 43 "lex/ragel_numeric.rl"
	{ (ragel_num.ratio.den *= 10) += ((*p)-'0'); }
	goto st83;
st83:
	if ( ++p == pe )
		goto _test_eof83;
case 83:
#line 950 "lex/parser_units.cpp"
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr47;
	goto st0;
tr73:
#line 67 "lex/ragel_units.rl"
	{(smpte.hour *= 10) += ((*p) - '0');}
	goto st34;
st34:
	if ( ++p == pe )
		goto _test_eof34;
case 34:
#line 962 "lex/parser_units.cpp"
	if ( (*p) == 58 )
		goto st35;
	goto st0;
st35:
	if ( ++p == pe )
		goto _test_eof35;
case 35:
	if ( 48 <= (*p) && (*p) <= 53 )
		goto tr49;
	goto st0;
tr49:
#line 68 "lex/ragel_units.rl"
	{smpte.min = 0; smpte.np++;}
#line 68 "lex/ragel_units.rl"
	{(smpte.min *= 10) += ((*p) - '0');}
	goto st36;
st36:
	if ( ++p == pe )
		goto _test_eof36;
case 36:
#line 983 "lex/parser_units.cpp"
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr50;
	goto st0;
tr50:
#line 68 "lex/ragel_units.rl"
	{(smpte.min *= 10) += ((*p) - '0');}
	goto st84;
st84:
	if ( ++p == pe )
		goto _test_eof84;
case 84:
#line 995 "lex/parser_units.cpp"
	switch( (*p) ) {
		case 46: goto st37;
		case 58: goto st38;
	}
	goto st0;
st37:
	if ( ++p == pe )
		goto _test_eof37;
case 37:
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr51;
	goto st0;
tr51:
#line 70 "lex/ragel_units.rl"
	{smpte.frame = 0;}
#line 70 "lex/ragel_units.rl"
	{(smpte.frame *= 10) += ((*p) - '0');}
	goto st85;
st85:
	if ( ++p == pe )
		goto _test_eof85;
case 85:
#line 1018 "lex/parser_units.cpp"
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr123;
	goto st0;
tr123:
#line 70 "lex/ragel_units.rl"
	{(smpte.frame *= 10) += ((*p) - '0');}
	goto st86;
st86:
	if ( ++p == pe )
		goto _test_eof86;
case 86:
#line 1030 "lex/parser_units.cpp"
	goto st0;
st38:
	if ( ++p == pe )
		goto _test_eof38;
case 38:
	if ( 48 <= (*p) && (*p) <= 53 )
		goto tr52;
	goto st0;
tr52:
#line 69 "lex/ragel_units.rl"
	{smpte.sec = 0; smpte.np++;}
#line 69 "lex/ragel_units.rl"
	{(smpte.sec *= 10) += ((*p) - '0');}
	goto st39;
st39:
	if ( ++p == pe )
		goto _test_eof39;
case 39:
#line 1049 "lex/parser_units.cpp"
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr53;
	goto st0;
tr53:
#line 69 "lex/ragel_units.rl"
	{(smpte.sec *= 10) += ((*p) - '0');}
	goto st87;
st87:
	if ( ++p == pe )
		goto _test_eof87;
case 87:
#line 1061 "lex/parser_units.cpp"
	if ( (*p) == 46 )
		goto st37;
	goto st0;
tr10:
#line 82 "lex/ragel_units.rl"
	{pos_ = POSITION_END;}
#line 29 "lex/ragel_numeric.rl"
	{ (ragel_num.vint *= 10) += ((*p)-'0'); }
#line 42 "lex/ragel_numeric.rl"
	{ (ragel_num.ratio.num *= 10) += ((*p)-'0'); }
#line 79 "lex/ragel_units.rl"
	{smpte.np = 0;}
#line 67 "lex/ragel_units.rl"
	{smpte.hour = 0; smpte.np++;}
#line 67 "lex/ragel_units.rl"
	{(smpte.hour *= 10) += ((*p) - '0');}
	goto st88;
tr13:
#line 29 "lex/ragel_numeric.rl"
	{ (ragel_num.vint *= 10) += ((*p)-'0'); }
#line 42 "lex/ragel_numeric.rl"
	{ (ragel_num.ratio.num *= 10) += ((*p)-'0'); }
#line 67 "lex/ragel_units.rl"
	{smpte.hour = 0; smpte.np++;}
#line 67 "lex/ragel_units.rl"
	{(smpte.hour *= 10) += ((*p) - '0');}
	goto st88;
tr65:
#line 83 "lex/ragel_units.rl"
	{pos_ = POSITION_CURRENT;}
#line 29 "lex/ragel_numeric.rl"
	{ (ragel_num.vint *= 10) += ((*p)-'0'); }
#line 42 "lex/ragel_numeric.rl"
	{ (ragel_num.ratio.num *= 10) += ((*p)-'0'); }
#line 79 "lex/ragel_units.rl"
	{smpte.np = 0;}
#line 67 "lex/ragel_units.rl"
	{smpte.hour = 0; smpte.np++;}
#line 67 "lex/ragel_units.rl"
	{(smpte.hour *= 10) += ((*p) - '0');}
	goto st88;
st88:
	if ( ++p == pe )
		goto _test_eof88;
case 88:
#line 1107 "lex/parser_units.cpp"
	switch( (*p) ) {
		case 37: goto tr69;
		case 42: goto tr70;
		case 46: goto tr71;
		case 47: goto st33;
		case 72: goto tr74;
		case 95: goto tr75;
		case 98: goto tr76;
		case 99: goto tr77;
		case 100: goto tr78;
		case 104: goto tr79;
		case 109: goto tr80;
		case 112: goto tr81;
		case 114: goto tr82;
		case 115: goto tr83;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr124;
	goto st0;
tr124:
#line 29 "lex/ragel_numeric.rl"
	{ (ragel_num.vint *= 10) += ((*p)-'0'); }
#line 42 "lex/ragel_numeric.rl"
	{ (ragel_num.ratio.num *= 10) += ((*p)-'0'); }
#line 67 "lex/ragel_units.rl"
	{(smpte.hour *= 10) += ((*p) - '0');}
	goto st89;
st89:
	if ( ++p == pe )
		goto _test_eof89;
case 89:
#line 1139 "lex/parser_units.cpp"
	switch( (*p) ) {
		case 37: goto tr69;
		case 42: goto tr70;
		case 46: goto tr71;
		case 47: goto st33;
		case 58: goto st35;
		case 72: goto tr74;
		case 95: goto tr75;
		case 98: goto tr76;
		case 99: goto tr77;
		case 100: goto tr78;
		case 104: goto tr79;
		case 109: goto tr80;
		case 112: goto tr81;
		case 114: goto tr82;
		case 115: goto tr83;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr14;
	goto st0;
tr11:
#line 82 "lex/ragel_units.rl"
	{pos_ = POSITION_END;}
#line 29 "lex/ragel_numeric.rl"
	{ (ragel_num.vint *= 10) += ((*p)-'0'); }
#line 42 "lex/ragel_numeric.rl"
	{ (ragel_num.ratio.num *= 10) += ((*p)-'0'); }
	goto st90;
tr14:
#line 29 "lex/ragel_numeric.rl"
	{ (ragel_num.vint *= 10) += ((*p)-'0'); }
#line 42 "lex/ragel_numeric.rl"
	{ (ragel_num.ratio.num *= 10) += ((*p)-'0'); }
	goto st90;
tr66:
#line 83 "lex/ragel_units.rl"
	{pos_ = POSITION_CURRENT;}
#line 29 "lex/ragel_numeric.rl"
	{ (ragel_num.vint *= 10) += ((*p)-'0'); }
#line 42 "lex/ragel_numeric.rl"
	{ (ragel_num.ratio.num *= 10) += ((*p)-'0'); }
	goto st90;
st90:
	if ( ++p == pe )
		goto _test_eof90;
case 90:
#line 1186 "lex/parser_units.cpp"
	switch( (*p) ) {
		case 37: goto tr69;
		case 42: goto tr70;
		case 46: goto tr71;
		case 47: goto st33;
		case 72: goto tr74;
		case 95: goto tr75;
		case 98: goto tr76;
		case 99: goto tr77;
		case 100: goto tr78;
		case 104: goto tr79;
		case 109: goto tr80;
		case 112: goto tr81;
		case 114: goto tr82;
		case 115: goto tr83;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr14;
	goto st0;
tr3:
#line 8 "lex/ragel_units.rl"
	{ ragel_type = TYPE_UNKNOWN; ragel_cat = CAT_UNKNOWN; }
#line 88 "lex/ragel_units.rl"
	{pos_ = POSITION_ABS;}
#line 29 "lex/ragel_numeric.rl"
	{ (ragel_num.vint *= 10) += ((*p)-'0'); }
#line 42 "lex/ragel_numeric.rl"
	{ (ragel_num.ratio.num *= 10) += ((*p)-'0'); }
#line 79 "lex/ragel_units.rl"
	{smpte.np = 0;}
#line 67 "lex/ragel_units.rl"
	{smpte.hour = 0; smpte.np++;}
#line 67 "lex/ragel_units.rl"
	{(smpte.hour *= 10) += ((*p) - '0');}
#line 9 "lex/ragel_music.rl"
	{ bpm.dur_num = 1; bpm.dur_den = 4; }
#line 6 "lex/ragel_music.rl"
	{ (bpm.ival *= 10) += ((*p) - '0'); }
	goto st91;
st91:
	if ( ++p == pe )
		goto _test_eof91;
case 91:
#line 1230 "lex/parser_units.cpp"
	switch( (*p) ) {
		case 37: goto tr69;
		case 42: goto tr70;
		case 46: goto tr125;
		case 47: goto st33;
		case 72: goto tr74;
		case 95: goto tr126;
		case 98: goto tr127;
		case 99: goto tr77;
		case 100: goto tr78;
		case 104: goto tr79;
		case 109: goto tr80;
		case 112: goto tr81;
		case 114: goto tr82;
		case 115: goto tr83;
		case 124: goto st43;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr73;
	goto st0;
tr125:
#line 30 "lex/ragel_numeric.rl"
	{
        ragel_num.vint *= ragel_num.sign;
        ragel_type = TYPE_INT;
        ragel_cat  = CAT_NUMBER;
    }
#line 50 "lex/ragel_numeric.rl"
	{
        ragel_num.ratio.num = 0;
        ragel_num.ratio.den = 1;
    }
	goto st40;
st40:
	if ( ++p == pe )
		goto _test_eof40;
case 40:
#line 1268 "lex/parser_units.cpp"
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr54;
	goto st0;
tr54:
#line 54 "lex/ragel_numeric.rl"
	{
        (ragel_num.ratio.num *= 10) += ((*p) - '0');
        ragel_num.ratio.den *= 10;
    }
#line 7 "lex/ragel_music.rl"
	{ (bpm.fnum *= 10) += ((*p) - '0'); bpm.fden *= 10; }
	goto st92;
st92:
	if ( ++p == pe )
		goto _test_eof92;
case 92:
#line 1285 "lex/parser_units.cpp"
	switch( (*p) ) {
		case 37: goto tr84;
		case 42: goto tr85;
		case 72: goto tr86;
		case 95: goto tr129;
		case 98: goto tr130;
		case 99: goto tr89;
		case 100: goto tr90;
		case 104: goto tr91;
		case 109: goto tr92;
		case 112: goto tr93;
		case 114: goto tr94;
		case 115: goto tr95;
		case 124: goto st43;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr54;
	goto st0;
tr126:
#line 30 "lex/ragel_numeric.rl"
	{
        ragel_num.vint *= ragel_num.sign;
        ragel_type = TYPE_INT;
        ragel_cat  = CAT_NUMBER;
    }
#line 7 "lex/ragel_units.rl"
	{ ragel_num.vdouble = ragel_num.vint; }
#line 108 "lex/ragel_numeric.rl"
	{ragel_num.vdouble = ragel_num.vint;}
#line 113 "lex/ragel_numeric.rl"
	{ragel_num.vdouble = ragel_num.vint;}
	goto st93;
tr129:
#line 58 "lex/ragel_numeric.rl"
	{
        const auto frac = double(ragel_num.ratio.num) / ragel_num.ratio.den;
        ragel_num.vdouble = ragel_num.vint + ragel_num.sign * frac;
        ragel_type = TYPE_FLOAT;
        ragel_cat  = CAT_NUMBER;
    }
	goto st93;
st93:
	if ( ++p == pe )
		goto _test_eof93;
case 93:
#line 1331 "lex/parser_units.cpp"
	switch( (*p) ) {
		case 37: goto st53;
		case 42: goto st58;
		case 72: goto st5;
		case 98: goto st41;
		case 99: goto st60;
		case 100: goto st62;
		case 104: goto st67;
		case 109: goto st69;
		case 112: goto st17;
		case 114: goto st25;
		case 115: goto st76;
	}
	goto st0;
tr127:
#line 30 "lex/ragel_numeric.rl"
	{
        ragel_num.vint *= ragel_num.sign;
        ragel_type = TYPE_INT;
        ragel_cat  = CAT_NUMBER;
    }
#line 7 "lex/ragel_units.rl"
	{ ragel_num.vdouble = ragel_num.vint; }
	goto st41;
tr130:
#line 58 "lex/ragel_numeric.rl"
	{
        const auto frac = double(ragel_num.ratio.num) / ragel_num.ratio.den;
        ragel_num.vdouble = ragel_num.vint + ragel_num.sign * frac;
        ragel_type = TYPE_FLOAT;
        ragel_cat  = CAT_NUMBER;
    }
	goto st41;
st41:
	if ( ++p == pe )
		goto _test_eof41;
case 41:
#line 1369 "lex/parser_units.cpp"
	if ( (*p) == 112 )
		goto st42;
	goto st0;
st42:
	if ( ++p == pe )
		goto _test_eof42;
case 42:
	if ( (*p) == 109 )
		goto st94;
	goto st0;
st94:
	if ( ++p == pe )
		goto _test_eof94;
case 94:
	goto st0;
st43:
	if ( ++p == pe )
		goto _test_eof43;
case 43:
	if ( 49 <= (*p) && (*p) <= 57 )
		goto tr57;
	goto st0;
tr57:
#line 11 "lex/ragel_music.rl"
	{ bpm.dur_num = 0; }
#line 12 "lex/ragel_music.rl"
	{ (bpm.dur_num *= 10) += ((*p) - '0'); }
#line 13 "lex/ragel_music.rl"
	{ bpm.dur_den = 0;}
#line 14 "lex/ragel_music.rl"
	{ (bpm.dur_den *= 10) += ((*p) - '0'); }
	goto st95;
tr134:
#line 12 "lex/ragel_music.rl"
	{ (bpm.dur_num *= 10) += ((*p) - '0'); }
#line 14 "lex/ragel_music.rl"
	{ (bpm.dur_den *= 10) += ((*p) - '0'); }
	goto st95;
st95:
	if ( ++p == pe )
		goto _test_eof95;
case 95:
#line 1412 "lex/parser_units.cpp"
	switch( (*p) ) {
		case 46: goto tr132;
		case 47: goto st46;
		case 95: goto tr135;
		case 98: goto tr136;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr134;
	goto st0;
tr132:
#line 24 "lex/ragel_music.rl"
	{bpm.dur_num = 1;}
#line 8 "lex/ragel_music.rl"
	{ bpm.dur_num *= 3; bpm.dur_den *= 2; }
	goto st96;
tr139:
#line 8 "lex/ragel_music.rl"
	{ bpm.dur_num *= 3; bpm.dur_den *= 2; }
	goto st96;
st96:
	if ( ++p == pe )
		goto _test_eof96;
case 96:
#line 1436 "lex/parser_units.cpp"
	switch( (*p) ) {
		case 95: goto st97;
		case 98: goto st44;
	}
	goto st0;
tr135:
#line 24 "lex/ragel_music.rl"
	{bpm.dur_num = 1;}
	goto st97;
st97:
	if ( ++p == pe )
		goto _test_eof97;
case 97:
#line 1450 "lex/parser_units.cpp"
	if ( (*p) == 98 )
		goto st44;
	goto st0;
tr136:
#line 24 "lex/ragel_music.rl"
	{bpm.dur_num = 1;}
	goto st44;
st44:
	if ( ++p == pe )
		goto _test_eof44;
case 44:
#line 1462 "lex/parser_units.cpp"
	if ( (*p) == 112 )
		goto st45;
	goto st0;
st45:
	if ( ++p == pe )
		goto _test_eof45;
case 45:
	if ( (*p) == 109 )
		goto st98;
	goto st0;
st98:
	if ( ++p == pe )
		goto _test_eof98;
case 98:
	goto st0;
st46:
	if ( ++p == pe )
		goto _test_eof46;
case 46:
	if ( 49 <= (*p) && (*p) <= 57 )
		goto tr60;
	goto st0;
tr60:
#line 13 "lex/ragel_music.rl"
	{ bpm.dur_den = 0;}
#line 14 "lex/ragel_music.rl"
	{ (bpm.dur_den *= 10) += ((*p) - '0'); }
	goto st99;
tr140:
#line 14 "lex/ragel_music.rl"
	{ (bpm.dur_den *= 10) += ((*p) - '0'); }
	goto st99;
st99:
	if ( ++p == pe )
		goto _test_eof99;
case 99:
#line 1499 "lex/parser_units.cpp"
	switch( (*p) ) {
		case 46: goto tr139;
		case 95: goto st97;
		case 98: goto st44;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr140;
	goto st0;
tr4:
#line 8 "lex/ragel_units.rl"
	{ ragel_type = TYPE_UNKNOWN; ragel_cat = CAT_UNKNOWN; }
#line 88 "lex/ragel_units.rl"
	{pos_ = POSITION_ABS;}
#line 29 "lex/ragel_numeric.rl"
	{ (ragel_num.vint *= 10) += ((*p)-'0'); }
#line 42 "lex/ragel_numeric.rl"
	{ (ragel_num.ratio.num *= 10) += ((*p)-'0'); }
#line 79 "lex/ragel_units.rl"
	{smpte.np = 0;}
#line 67 "lex/ragel_units.rl"
	{smpte.hour = 0; smpte.np++;}
#line 67 "lex/ragel_units.rl"
	{(smpte.hour *= 10) += ((*p) - '0');}
#line 9 "lex/ragel_music.rl"
	{ bpm.dur_num = 1; bpm.dur_den = 4; }
#line 6 "lex/ragel_music.rl"
	{ (bpm.ival *= 10) += ((*p) - '0'); }
	goto st100;
st100:
	if ( ++p == pe )
		goto _test_eof100;
case 100:
#line 1532 "lex/parser_units.cpp"
	switch( (*p) ) {
		case 37: goto tr69;
		case 42: goto tr70;
		case 46: goto tr125;
		case 47: goto st33;
		case 72: goto tr74;
		case 95: goto tr126;
		case 98: goto tr127;
		case 99: goto tr77;
		case 100: goto tr78;
		case 104: goto tr79;
		case 109: goto tr80;
		case 112: goto tr81;
		case 114: goto tr82;
		case 115: goto tr83;
		case 124: goto st43;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr141;
	goto st0;
tr141:
#line 29 "lex/ragel_numeric.rl"
	{ (ragel_num.vint *= 10) += ((*p)-'0'); }
#line 42 "lex/ragel_numeric.rl"
	{ (ragel_num.ratio.num *= 10) += ((*p)-'0'); }
#line 67 "lex/ragel_units.rl"
	{(smpte.hour *= 10) += ((*p) - '0');}
#line 6 "lex/ragel_music.rl"
	{ (bpm.ival *= 10) += ((*p) - '0'); }
	goto st101;
st101:
	if ( ++p == pe )
		goto _test_eof101;
case 101:
#line 1567 "lex/parser_units.cpp"
	switch( (*p) ) {
		case 37: goto tr69;
		case 42: goto tr70;
		case 46: goto tr125;
		case 47: goto st33;
		case 58: goto st35;
		case 72: goto tr74;
		case 95: goto tr126;
		case 98: goto tr127;
		case 99: goto tr77;
		case 100: goto tr78;
		case 104: goto tr79;
		case 109: goto tr80;
		case 112: goto tr81;
		case 114: goto tr82;
		case 115: goto tr83;
		case 124: goto st43;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr142;
	goto st0;
tr5:
#line 8 "lex/ragel_units.rl"
	{ ragel_type = TYPE_UNKNOWN; ragel_cat = CAT_UNKNOWN; }
#line 88 "lex/ragel_units.rl"
	{pos_ = POSITION_ABS;}
#line 29 "lex/ragel_numeric.rl"
	{ (ragel_num.vint *= 10) += ((*p)-'0'); }
#line 42 "lex/ragel_numeric.rl"
	{ (ragel_num.ratio.num *= 10) += ((*p)-'0'); }
#line 9 "lex/ragel_music.rl"
	{ bpm.dur_num = 1; bpm.dur_den = 4; }
#line 6 "lex/ragel_music.rl"
	{ (bpm.ival *= 10) += ((*p) - '0'); }
	goto st102;
tr142:
#line 29 "lex/ragel_numeric.rl"
	{ (ragel_num.vint *= 10) += ((*p)-'0'); }
#line 42 "lex/ragel_numeric.rl"
	{ (ragel_num.ratio.num *= 10) += ((*p)-'0'); }
#line 6 "lex/ragel_music.rl"
	{ (bpm.ival *= 10) += ((*p) - '0'); }
	goto st102;
st102:
	if ( ++p == pe )
		goto _test_eof102;
case 102:
#line 1615 "lex/parser_units.cpp"
	switch( (*p) ) {
		case 37: goto tr69;
		case 42: goto tr70;
		case 46: goto tr125;
		case 47: goto st33;
		case 72: goto tr74;
		case 95: goto tr126;
		case 98: goto tr127;
		case 99: goto tr77;
		case 100: goto tr78;
		case 104: goto tr79;
		case 109: goto tr80;
		case 112: goto tr81;
		case 114: goto tr82;
		case 115: goto tr83;
		case 124: goto st43;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr142;
	goto st0;
tr6:
#line 8 "lex/ragel_units.rl"
	{ ragel_type = TYPE_UNKNOWN; ragel_cat = CAT_UNKNOWN; }
#line 88 "lex/ragel_units.rl"
	{pos_ = POSITION_ABS;}
	goto st47;
st47:
	if ( ++p == pe )
		goto _test_eof47;
case 47:
#line 1646 "lex/parser_units.cpp"
	if ( (*p) == 117 )
		goto st48;
	goto st0;
st48:
	if ( ++p == pe )
		goto _test_eof48;
case 48:
	if ( (*p) == 114 )
		goto st49;
	goto st0;
st49:
	if ( ++p == pe )
		goto _test_eof49;
case 49:
	switch( (*p) ) {
		case 43: goto tr63;
		case 45: goto tr63;
		case 48: goto tr64;
	}
	if ( (*p) > 53 ) {
		if ( 54 <= (*p) && (*p) <= 57 )
			goto tr66;
	} else if ( (*p) >= 49 )
		goto tr65;
	goto st0;
tr7:
#line 8 "lex/ragel_units.rl"
	{ ragel_type = TYPE_UNKNOWN; ragel_cat = CAT_UNKNOWN; }
#line 88 "lex/ragel_units.rl"
	{pos_ = POSITION_ABS;}
	goto st50;
st50:
	if ( ++p == pe )
		goto _test_eof50;
case 50:
#line 1682 "lex/parser_units.cpp"
	if ( (*p) == 110 )
		goto st51;
	goto st0;
st51:
	if ( ++p == pe )
		goto _test_eof51;
case 51:
	if ( (*p) == 100 )
		goto st2;
	goto st0;
	}
	_test_eof2: cs = 2; goto _test_eof; 
	_test_eof3: cs = 3; goto _test_eof; 
	_test_eof52: cs = 52; goto _test_eof; 
	_test_eof53: cs = 53; goto _test_eof; 
	_test_eof54: cs = 54; goto _test_eof; 
	_test_eof4: cs = 4; goto _test_eof; 
	_test_eof55: cs = 55; goto _test_eof; 
	_test_eof5: cs = 5; goto _test_eof; 
	_test_eof56: cs = 56; goto _test_eof; 
	_test_eof57: cs = 57; goto _test_eof; 
	_test_eof58: cs = 58; goto _test_eof; 
	_test_eof6: cs = 6; goto _test_eof; 
	_test_eof7: cs = 7; goto _test_eof; 
	_test_eof59: cs = 59; goto _test_eof; 
	_test_eof60: cs = 60; goto _test_eof; 
	_test_eof8: cs = 8; goto _test_eof; 
	_test_eof9: cs = 9; goto _test_eof; 
	_test_eof61: cs = 61; goto _test_eof; 
	_test_eof62: cs = 62; goto _test_eof; 
	_test_eof10: cs = 10; goto _test_eof; 
	_test_eof63: cs = 63; goto _test_eof; 
	_test_eof64: cs = 64; goto _test_eof; 
	_test_eof11: cs = 11; goto _test_eof; 
	_test_eof65: cs = 65; goto _test_eof; 
	_test_eof12: cs = 12; goto _test_eof; 
	_test_eof66: cs = 66; goto _test_eof; 
	_test_eof67: cs = 67; goto _test_eof; 
	_test_eof13: cs = 13; goto _test_eof; 
	_test_eof14: cs = 14; goto _test_eof; 
	_test_eof68: cs = 68; goto _test_eof; 
	_test_eof69: cs = 69; goto _test_eof; 
	_test_eof15: cs = 15; goto _test_eof; 
	_test_eof70: cs = 70; goto _test_eof; 
	_test_eof71: cs = 71; goto _test_eof; 
	_test_eof16: cs = 16; goto _test_eof; 
	_test_eof72: cs = 72; goto _test_eof; 
	_test_eof17: cs = 17; goto _test_eof; 
	_test_eof18: cs = 18; goto _test_eof; 
	_test_eof19: cs = 19; goto _test_eof; 
	_test_eof73: cs = 73; goto _test_eof; 
	_test_eof20: cs = 20; goto _test_eof; 
	_test_eof21: cs = 21; goto _test_eof; 
	_test_eof74: cs = 74; goto _test_eof; 
	_test_eof22: cs = 22; goto _test_eof; 
	_test_eof23: cs = 23; goto _test_eof; 
	_test_eof24: cs = 24; goto _test_eof; 
	_test_eof25: cs = 25; goto _test_eof; 
	_test_eof26: cs = 26; goto _test_eof; 
	_test_eof75: cs = 75; goto _test_eof; 
	_test_eof76: cs = 76; goto _test_eof; 
	_test_eof77: cs = 77; goto _test_eof; 
	_test_eof27: cs = 27; goto _test_eof; 
	_test_eof78: cs = 78; goto _test_eof; 
	_test_eof28: cs = 28; goto _test_eof; 
	_test_eof79: cs = 79; goto _test_eof; 
	_test_eof29: cs = 29; goto _test_eof; 
	_test_eof80: cs = 80; goto _test_eof; 
	_test_eof30: cs = 30; goto _test_eof; 
	_test_eof31: cs = 31; goto _test_eof; 
	_test_eof32: cs = 32; goto _test_eof; 
	_test_eof81: cs = 81; goto _test_eof; 
	_test_eof33: cs = 33; goto _test_eof; 
	_test_eof82: cs = 82; goto _test_eof; 
	_test_eof83: cs = 83; goto _test_eof; 
	_test_eof34: cs = 34; goto _test_eof; 
	_test_eof35: cs = 35; goto _test_eof; 
	_test_eof36: cs = 36; goto _test_eof; 
	_test_eof84: cs = 84; goto _test_eof; 
	_test_eof37: cs = 37; goto _test_eof; 
	_test_eof85: cs = 85; goto _test_eof; 
	_test_eof86: cs = 86; goto _test_eof; 
	_test_eof38: cs = 38; goto _test_eof; 
	_test_eof39: cs = 39; goto _test_eof; 
	_test_eof87: cs = 87; goto _test_eof; 
	_test_eof88: cs = 88; goto _test_eof; 
	_test_eof89: cs = 89; goto _test_eof; 
	_test_eof90: cs = 90; goto _test_eof; 
	_test_eof91: cs = 91; goto _test_eof; 
	_test_eof40: cs = 40; goto _test_eof; 
	_test_eof92: cs = 92; goto _test_eof; 
	_test_eof93: cs = 93; goto _test_eof; 
	_test_eof41: cs = 41; goto _test_eof; 
	_test_eof42: cs = 42; goto _test_eof; 
	_test_eof94: cs = 94; goto _test_eof; 
	_test_eof43: cs = 43; goto _test_eof; 
	_test_eof95: cs = 95; goto _test_eof; 
	_test_eof96: cs = 96; goto _test_eof; 
	_test_eof97: cs = 97; goto _test_eof; 
	_test_eof44: cs = 44; goto _test_eof; 
	_test_eof45: cs = 45; goto _test_eof; 
	_test_eof98: cs = 98; goto _test_eof; 
	_test_eof46: cs = 46; goto _test_eof; 
	_test_eof99: cs = 99; goto _test_eof; 
	_test_eof100: cs = 100; goto _test_eof; 
	_test_eof101: cs = 101; goto _test_eof; 
	_test_eof102: cs = 102; goto _test_eof; 
	_test_eof47: cs = 47; goto _test_eof; 
	_test_eof48: cs = 48; goto _test_eof; 
	_test_eof49: cs = 49; goto _test_eof; 
	_test_eof50: cs = 50; goto _test_eof; 
	_test_eof51: cs = 51; goto _test_eof; 

	_test_eof: {}
	if ( p == eof )
	{
	switch ( cs ) {
	case 57: 
#line 9 "lex/ragel_units.rl"
	{ ragel_cat = CAT_UNIT; }
	break;
	case 55: 
#line 58 "lex/ragel_numeric.rl"
	{
        const auto frac = double(ragel_num.ratio.num) / ragel_num.ratio.den;
        ragel_num.vdouble = ragel_num.vint + ragel_num.sign * frac;
        ragel_type = TYPE_FLOAT;
        ragel_cat  = CAT_NUMBER;
    }
#line 9 "lex/ragel_units.rl"
	{ ragel_cat = CAT_UNIT; }
	break;
	case 93: 
	case 96: 
	case 97: 
	case 98: 
	case 99: 
#line 10 "lex/ragel_music.rl"
	{ ragel_cat = CAT_UNIT; ragel_type = TYPE_BPM; }
#line 9 "lex/ragel_units.rl"
	{ ragel_cat = CAT_UNIT; }
	break;
	case 59: 
#line 11 "lex/ragel_units.rl"
	{ragel_type = TYPE_BPM;}
#line 9 "lex/ragel_units.rl"
	{ ragel_cat = CAT_UNIT; }
	break;
	case 60: 
	case 61: 
#line 12 "lex/ragel_units.rl"
	{ragel_type = TYPE_CENT;}
#line 9 "lex/ragel_units.rl"
	{ ragel_cat = CAT_UNIT; }
	break;
	case 62: 
	case 63: 
#line 13 "lex/ragel_units.rl"
	{ragel_type = TYPE_DAY;}
#line 9 "lex/ragel_units.rl"
	{ ragel_cat = CAT_UNIT; }
	break;
	case 64: 
	case 65: 
#line 14 "lex/ragel_units.rl"
	{ragel_type = TYPE_DB;}
#line 9 "lex/ragel_units.rl"
	{ ragel_cat = CAT_UNIT; }
	break;
	case 66: 
#line 15 "lex/ragel_units.rl"
	{ragel_type = TYPE_DEGREE;}
#line 9 "lex/ragel_units.rl"
	{ ragel_cat = CAT_UNIT; }
	break;
	case 56: 
#line 16 "lex/ragel_units.rl"
	{ragel_type = TYPE_HZ;}
#line 9 "lex/ragel_units.rl"
	{ ragel_cat = CAT_UNIT; }
	break;
	case 67: 
	case 68: 
#line 17 "lex/ragel_units.rl"
	{ragel_type = TYPE_HOUR;}
#line 9 "lex/ragel_units.rl"
	{ ragel_cat = CAT_UNIT; }
	break;
	case 69: 
	case 70: 
#line 18 "lex/ragel_units.rl"
	{ragel_type = TYPE_MIN;}
#line 9 "lex/ragel_units.rl"
	{ ragel_cat = CAT_UNIT; }
	break;
	case 71: 
	case 72: 
#line 19 "lex/ragel_units.rl"
	{ragel_type = TYPE_MSEC;}
#line 9 "lex/ragel_units.rl"
	{ ragel_cat = CAT_UNIT; }
	break;
	case 75: 
#line 20 "lex/ragel_units.rl"
	{ragel_type = TYPE_RADIAN;}
#line 9 "lex/ragel_units.rl"
	{ ragel_cat = CAT_UNIT; }
	break;
	case 77: 
	case 78: 
#line 21 "lex/ragel_units.rl"
	{ragel_type = TYPE_SAMP;}
#line 9 "lex/ragel_units.rl"
	{ ragel_cat = CAT_UNIT; }
	break;
	case 76: 
	case 79: 
#line 22 "lex/ragel_units.rl"
	{ragel_type = TYPE_SEC;}
#line 9 "lex/ragel_units.rl"
	{ ragel_cat = CAT_UNIT; }
	break;
	case 80: 
	case 81: 
#line 23 "lex/ragel_units.rl"
	{ragel_type = TYPE_SEMITONE;}
#line 9 "lex/ragel_units.rl"
	{ ragel_cat = CAT_UNIT; }
	break;
	case 73: 
	case 74: 
#line 26 "lex/ragel_units.rl"
	{ragel_type = TYPE_PERCENT;}
#line 9 "lex/ragel_units.rl"
	{ ragel_cat = CAT_UNIT; }
	break;
	case 58: 
#line 29 "lex/ragel_units.rl"
	{ragel_type = TYPE_PHASE;}
#line 9 "lex/ragel_units.rl"
	{ ragel_cat = CAT_UNIT; }
	break;
	case 84: 
	case 85: 
	case 86: 
	case 87: 
#line 53 "lex/ragel_units.rl"
	{
    ragel_type = TYPE_SMPTE;
    smpte.hour *= smpte.sign;
    smpte.min *= smpte.sign;
    smpte.sec *= smpte.sign;
    smpte.frame *= smpte.sign;

    if (smpte.np == 2) {
        smpte.sec = smpte.min;
        smpte.min = smpte.hour;
        smpte.hour = 0;
    }
}
#line 9 "lex/ragel_units.rl"
	{ ragel_cat = CAT_UNIT; }
	break;
	case 52: 
	case 88: 
	case 89: 
	case 90: 
#line 30 "lex/ragel_numeric.rl"
	{
        ragel_num.vint *= ragel_num.sign;
        ragel_type = TYPE_INT;
        ragel_cat  = CAT_NUMBER;
    }
#line 7 "lex/ragel_units.rl"
	{ ragel_num.vdouble = ragel_num.vint; }
#line 9 "lex/ragel_units.rl"
	{ ragel_cat = CAT_UNIT; }
	break;
	case 82: 
	case 83: 
#line 44 "lex/ragel_numeric.rl"
	{
        ragel_num.ratio.num *= ragel_num.sign;
        ragel_type = TYPE_RATIO;
        ragel_cat  = CAT_NUMBER;
    }
#line 92 "lex/ragel_units.rl"
	{ragel_type = TYPE_RATIO;}
#line 9 "lex/ragel_units.rl"
	{ ragel_cat = CAT_UNIT; }
	break;
	case 92: 
#line 58 "lex/ragel_numeric.rl"
	{
        const auto frac = double(ragel_num.ratio.num) / ragel_num.ratio.den;
        ragel_num.vdouble = ragel_num.vint + ragel_num.sign * frac;
        ragel_type = TYPE_FLOAT;
        ragel_cat  = CAT_NUMBER;
    }
#line 10 "lex/ragel_music.rl"
	{ ragel_cat = CAT_UNIT; ragel_type = TYPE_BPM; }
#line 9 "lex/ragel_units.rl"
	{ ragel_cat = CAT_UNIT; }
	break;
	case 95: 
#line 24 "lex/ragel_music.rl"
	{bpm.dur_num = 1;}
#line 10 "lex/ragel_music.rl"
	{ ragel_cat = CAT_UNIT; ragel_type = TYPE_BPM; }
#line 9 "lex/ragel_units.rl"
	{ ragel_cat = CAT_UNIT; }
	break;
	case 94: 
#line 11 "lex/ragel_units.rl"
	{ragel_type = TYPE_BPM;}
#line 10 "lex/ragel_music.rl"
	{ ragel_cat = CAT_UNIT; ragel_type = TYPE_BPM; }
#line 9 "lex/ragel_units.rl"
	{ ragel_cat = CAT_UNIT; }
	break;
	case 91: 
	case 100: 
	case 101: 
	case 102: 
#line 30 "lex/ragel_numeric.rl"
	{
        ragel_num.vint *= ragel_num.sign;
        ragel_type = TYPE_INT;
        ragel_cat  = CAT_NUMBER;
    }
#line 7 "lex/ragel_units.rl"
	{ ragel_num.vdouble = ragel_num.vint; }
#line 10 "lex/ragel_music.rl"
	{ ragel_cat = CAT_UNIT; ragel_type = TYPE_BPM; }
#line 9 "lex/ragel_units.rl"
	{ ragel_cat = CAT_UNIT; }
	break;
	case 53: 
#line 26 "lex/ragel_units.rl"
	{ragel_type = TYPE_PERCENT;}
#line 70 "lex/ragel_numeric.rl"
	{
        ragel_type = TYPE_PERCENT;
        ragel_cat = CAT_NUMBER;
    }
#line 90 "lex/ragel_units.rl"
	{ragel_type = TYPE_PERCENT;}
#line 9 "lex/ragel_units.rl"
	{ ragel_cat = CAT_UNIT; }
	break;
	case 54: 
#line 29 "lex/ragel_units.rl"
	{ragel_type = TYPE_PHASE;}
#line 75 "lex/ragel_numeric.rl"
	{
        ragel_type = TYPE_PHASE;
        ragel_cat = CAT_NUMBER;
    }
#line 91 "lex/ragel_units.rl"
	{ragel_type = TYPE_PHASE;}
#line 9 "lex/ragel_units.rl"
	{ ragel_cat = CAT_UNIT; }
	break;
#line 2046 "lex/parser_units.cpp"
	}
	}

	_out: {}
	}

#line 75 "lex/parser_units.rl"

    const bool ok = cs >= 52;
    if (ok) {
        unit_.value = ragel_num.vdouble;

        switch(ragel_type) {
        case TYPE_RATIO:
            if (ragel_num.ratio.den == 0) {
                fprintf(stderr, "division by zero: %s\n", str);
                return false;
            }

            unit_.value = ragel_num.getRatioAsFloat();
        break;
        case TYPE_INT:
            unit_.value = ragel_num.getInteger();
        break;
        default:
        break;
        }

        unit_.type = ragel_type;
        unit_.pos = pos_;
        unit_.smpte.hour = smpte.hour;
        unit_.smpte.min = smpte.min;
        unit_.smpte.sec = smpte.sec;
        unit_.smpte.frame = smpte.frame;
        unit_.bpm = bpm;
    }

    return ok;
}

size_t UnitsFullMatch::parse(const AtomListView& lv, UnitVec& out)
{
    const size_t N = lv.size();

    for (size_t i = 0; i < N; i++) {
        const auto& a = lv[i];
        if (!parse(a))
            return i;

        out.push_back(unit_);
    }

    return N;
}


#line 2103 "lex/parser_units.cpp"
static const int units_type_start = 1;
static const int units_type_first_final = 30;
static const int units_type_error = 0;

static const int units_type_en_main = 1;


#line 129 "lex/parser_units.rl"


bool UnitTypeFullMatch::parse(const char* str)
{
    reset();

    const auto len = strlen(str);
    if (len == 0)
        return false;

    const char* p = str;
    const char* pe = p + len;
    const char* eof = pe;

    
#line 2127 "lex/parser_units.cpp"
	{
	cs = units_type_start;
	}

#line 144 "lex/parser_units.rl"
    
#line 2134 "lex/parser_units.cpp"
	{
	if ( p == pe )
		goto _test_eof;
	switch ( cs )
	{
case 1:
	switch( (*p) ) {
		case 37: goto st30;
		case 42: goto st31;
		case 72: goto st2;
		case 98: goto st3;
		case 99: goto st34;
		case 100: goto st36;
		case 104: goto st41;
		case 109: goto st43;
		case 112: goto st14;
		case 114: goto st22;
		case 115: goto st49;
	}
	goto st0;
st0:
cs = 0;
	goto _out;
st30:
	if ( ++p == pe )
		goto _test_eof30;
case 30:
	goto st0;
st31:
	if ( ++p == pe )
		goto _test_eof31;
case 31:
	goto st0;
st2:
	if ( ++p == pe )
		goto _test_eof2;
case 2:
	if ( (*p) == 122 )
		goto st32;
	goto st0;
st32:
	if ( ++p == pe )
		goto _test_eof32;
case 32:
	goto st0;
st3:
	if ( ++p == pe )
		goto _test_eof3;
case 3:
	if ( (*p) == 112 )
		goto st4;
	goto st0;
st4:
	if ( ++p == pe )
		goto _test_eof4;
case 4:
	if ( (*p) == 109 )
		goto st33;
	goto st0;
st33:
	if ( ++p == pe )
		goto _test_eof33;
case 33:
	goto st0;
st34:
	if ( ++p == pe )
		goto _test_eof34;
case 34:
	if ( (*p) == 101 )
		goto st5;
	goto st0;
st5:
	if ( ++p == pe )
		goto _test_eof5;
case 5:
	if ( (*p) == 110 )
		goto st6;
	goto st0;
st6:
	if ( ++p == pe )
		goto _test_eof6;
case 6:
	if ( (*p) == 116 )
		goto st35;
	goto st0;
st35:
	if ( ++p == pe )
		goto _test_eof35;
case 35:
	goto st0;
st36:
	if ( ++p == pe )
		goto _test_eof36;
case 36:
	switch( (*p) ) {
		case 97: goto st7;
		case 98: goto st38;
		case 101: goto st9;
	}
	goto st0;
st7:
	if ( ++p == pe )
		goto _test_eof7;
case 7:
	if ( (*p) == 121 )
		goto st37;
	goto st0;
st37:
	if ( ++p == pe )
		goto _test_eof37;
case 37:
	goto st0;
st38:
	if ( ++p == pe )
		goto _test_eof38;
case 38:
	if ( (*p) == 102 )
		goto st8;
	goto st0;
st8:
	if ( ++p == pe )
		goto _test_eof8;
case 8:
	if ( (*p) == 115 )
		goto st39;
	goto st0;
st39:
	if ( ++p == pe )
		goto _test_eof39;
case 39:
	goto st0;
st9:
	if ( ++p == pe )
		goto _test_eof9;
case 9:
	if ( (*p) == 103 )
		goto st40;
	goto st0;
st40:
	if ( ++p == pe )
		goto _test_eof40;
case 40:
	goto st0;
st41:
	if ( ++p == pe )
		goto _test_eof41;
case 41:
	switch( (*p) ) {
		case 111: goto st10;
		case 122: goto st32;
	}
	goto st0;
st10:
	if ( ++p == pe )
		goto _test_eof10;
case 10:
	if ( (*p) == 117 )
		goto st11;
	goto st0;
st11:
	if ( ++p == pe )
		goto _test_eof11;
case 11:
	if ( (*p) == 114 )
		goto st42;
	goto st0;
st42:
	if ( ++p == pe )
		goto _test_eof42;
case 42:
	goto st0;
st43:
	if ( ++p == pe )
		goto _test_eof43;
case 43:
	switch( (*p) ) {
		case 105: goto st12;
		case 115: goto st45;
	}
	goto st0;
st12:
	if ( ++p == pe )
		goto _test_eof12;
case 12:
	if ( (*p) == 110 )
		goto st44;
	goto st0;
st44:
	if ( ++p == pe )
		goto _test_eof44;
case 44:
	goto st0;
st45:
	if ( ++p == pe )
		goto _test_eof45;
case 45:
	if ( (*p) == 101 )
		goto st13;
	goto st0;
st13:
	if ( ++p == pe )
		goto _test_eof13;
case 13:
	if ( (*p) == 99 )
		goto st46;
	goto st0;
st46:
	if ( ++p == pe )
		goto _test_eof46;
case 46:
	goto st0;
st14:
	if ( ++p == pe )
		goto _test_eof14;
case 14:
	switch( (*p) ) {
		case 101: goto st15;
		case 104: goto st19;
	}
	goto st0;
st15:
	if ( ++p == pe )
		goto _test_eof15;
case 15:
	if ( (*p) == 114 )
		goto st16;
	goto st0;
st16:
	if ( ++p == pe )
		goto _test_eof16;
case 16:
	if ( (*p) == 99 )
		goto st47;
	goto st0;
st47:
	if ( ++p == pe )
		goto _test_eof47;
case 47:
	if ( (*p) == 101 )
		goto st17;
	goto st0;
st17:
	if ( ++p == pe )
		goto _test_eof17;
case 17:
	if ( (*p) == 110 )
		goto st18;
	goto st0;
st18:
	if ( ++p == pe )
		goto _test_eof18;
case 18:
	if ( (*p) == 116 )
		goto st30;
	goto st0;
st19:
	if ( ++p == pe )
		goto _test_eof19;
case 19:
	if ( (*p) == 97 )
		goto st20;
	goto st0;
st20:
	if ( ++p == pe )
		goto _test_eof20;
case 20:
	if ( (*p) == 115 )
		goto st21;
	goto st0;
st21:
	if ( ++p == pe )
		goto _test_eof21;
case 21:
	if ( (*p) == 101 )
		goto st31;
	goto st0;
st22:
	if ( ++p == pe )
		goto _test_eof22;
case 22:
	if ( (*p) == 97 )
		goto st23;
	goto st0;
st23:
	if ( ++p == pe )
		goto _test_eof23;
case 23:
	if ( (*p) == 100 )
		goto st48;
	goto st0;
st48:
	if ( ++p == pe )
		goto _test_eof48;
case 48:
	goto st0;
st49:
	if ( ++p == pe )
		goto _test_eof49;
case 49:
	switch( (*p) ) {
		case 97: goto st50;
		case 101: goto st25;
	}
	goto st0;
st50:
	if ( ++p == pe )
		goto _test_eof50;
case 50:
	if ( (*p) == 109 )
		goto st24;
	goto st0;
st24:
	if ( ++p == pe )
		goto _test_eof24;
case 24:
	if ( (*p) == 112 )
		goto st51;
	goto st0;
st51:
	if ( ++p == pe )
		goto _test_eof51;
case 51:
	goto st0;
st25:
	if ( ++p == pe )
		goto _test_eof25;
case 25:
	switch( (*p) ) {
		case 99: goto st52;
		case 109: goto st26;
	}
	goto st0;
st52:
	if ( ++p == pe )
		goto _test_eof52;
case 52:
	goto st0;
st26:
	if ( ++p == pe )
		goto _test_eof26;
case 26:
	if ( (*p) == 105 )
		goto st53;
	goto st0;
st53:
	if ( ++p == pe )
		goto _test_eof53;
case 53:
	if ( (*p) == 116 )
		goto st27;
	goto st0;
st27:
	if ( ++p == pe )
		goto _test_eof27;
case 27:
	if ( (*p) == 111 )
		goto st28;
	goto st0;
st28:
	if ( ++p == pe )
		goto _test_eof28;
case 28:
	if ( (*p) == 110 )
		goto st29;
	goto st0;
st29:
	if ( ++p == pe )
		goto _test_eof29;
case 29:
	if ( (*p) == 101 )
		goto st54;
	goto st0;
st54:
	if ( ++p == pe )
		goto _test_eof54;
case 54:
	goto st0;
	}
	_test_eof30: cs = 30; goto _test_eof; 
	_test_eof31: cs = 31; goto _test_eof; 
	_test_eof2: cs = 2; goto _test_eof; 
	_test_eof32: cs = 32; goto _test_eof; 
	_test_eof3: cs = 3; goto _test_eof; 
	_test_eof4: cs = 4; goto _test_eof; 
	_test_eof33: cs = 33; goto _test_eof; 
	_test_eof34: cs = 34; goto _test_eof; 
	_test_eof5: cs = 5; goto _test_eof; 
	_test_eof6: cs = 6; goto _test_eof; 
	_test_eof35: cs = 35; goto _test_eof; 
	_test_eof36: cs = 36; goto _test_eof; 
	_test_eof7: cs = 7; goto _test_eof; 
	_test_eof37: cs = 37; goto _test_eof; 
	_test_eof38: cs = 38; goto _test_eof; 
	_test_eof8: cs = 8; goto _test_eof; 
	_test_eof39: cs = 39; goto _test_eof; 
	_test_eof9: cs = 9; goto _test_eof; 
	_test_eof40: cs = 40; goto _test_eof; 
	_test_eof41: cs = 41; goto _test_eof; 
	_test_eof10: cs = 10; goto _test_eof; 
	_test_eof11: cs = 11; goto _test_eof; 
	_test_eof42: cs = 42; goto _test_eof; 
	_test_eof43: cs = 43; goto _test_eof; 
	_test_eof12: cs = 12; goto _test_eof; 
	_test_eof44: cs = 44; goto _test_eof; 
	_test_eof45: cs = 45; goto _test_eof; 
	_test_eof13: cs = 13; goto _test_eof; 
	_test_eof46: cs = 46; goto _test_eof; 
	_test_eof14: cs = 14; goto _test_eof; 
	_test_eof15: cs = 15; goto _test_eof; 
	_test_eof16: cs = 16; goto _test_eof; 
	_test_eof47: cs = 47; goto _test_eof; 
	_test_eof17: cs = 17; goto _test_eof; 
	_test_eof18: cs = 18; goto _test_eof; 
	_test_eof19: cs = 19; goto _test_eof; 
	_test_eof20: cs = 20; goto _test_eof; 
	_test_eof21: cs = 21; goto _test_eof; 
	_test_eof22: cs = 22; goto _test_eof; 
	_test_eof23: cs = 23; goto _test_eof; 
	_test_eof48: cs = 48; goto _test_eof; 
	_test_eof49: cs = 49; goto _test_eof; 
	_test_eof50: cs = 50; goto _test_eof; 
	_test_eof24: cs = 24; goto _test_eof; 
	_test_eof51: cs = 51; goto _test_eof; 
	_test_eof25: cs = 25; goto _test_eof; 
	_test_eof52: cs = 52; goto _test_eof; 
	_test_eof26: cs = 26; goto _test_eof; 
	_test_eof53: cs = 53; goto _test_eof; 
	_test_eof27: cs = 27; goto _test_eof; 
	_test_eof28: cs = 28; goto _test_eof; 
	_test_eof29: cs = 29; goto _test_eof; 
	_test_eof54: cs = 54; goto _test_eof; 

	_test_eof: {}
	if ( p == eof )
	{
	switch ( cs ) {
	case 33: 
#line 11 "lex/ragel_units.rl"
	{ragel_type = TYPE_BPM;}
	break;
	case 34: 
	case 35: 
#line 12 "lex/ragel_units.rl"
	{ragel_type = TYPE_CENT;}
	break;
	case 36: 
	case 37: 
#line 13 "lex/ragel_units.rl"
	{ragel_type = TYPE_DAY;}
	break;
	case 38: 
	case 39: 
#line 14 "lex/ragel_units.rl"
	{ragel_type = TYPE_DB;}
	break;
	case 40: 
#line 15 "lex/ragel_units.rl"
	{ragel_type = TYPE_DEGREE;}
	break;
	case 32: 
#line 16 "lex/ragel_units.rl"
	{ragel_type = TYPE_HZ;}
	break;
	case 41: 
	case 42: 
#line 17 "lex/ragel_units.rl"
	{ragel_type = TYPE_HOUR;}
	break;
	case 43: 
	case 44: 
#line 18 "lex/ragel_units.rl"
	{ragel_type = TYPE_MIN;}
	break;
	case 45: 
	case 46: 
#line 19 "lex/ragel_units.rl"
	{ragel_type = TYPE_MSEC;}
	break;
	case 48: 
#line 20 "lex/ragel_units.rl"
	{ragel_type = TYPE_RADIAN;}
	break;
	case 50: 
	case 51: 
#line 21 "lex/ragel_units.rl"
	{ragel_type = TYPE_SAMP;}
	break;
	case 49: 
	case 52: 
#line 22 "lex/ragel_units.rl"
	{ragel_type = TYPE_SEC;}
	break;
	case 53: 
	case 54: 
#line 23 "lex/ragel_units.rl"
	{ragel_type = TYPE_SEMITONE;}
	break;
	case 30: 
	case 47: 
#line 26 "lex/ragel_units.rl"
	{ragel_type = TYPE_PERCENT;}
	break;
	case 31: 
#line 29 "lex/ragel_units.rl"
	{ragel_type = TYPE_PHASE;}
	break;
#line 2641 "lex/parser_units.cpp"
	}
	}

	_out: {}
	}

#line 145 "lex/parser_units.rl"

    return cs >= 30;
}

bool UnitTypeFullMatch::parse(const Atom& a)
{
    if (a.isSymbol()) {
        return parse(a.asT<t_symbol*>()->s_name);
    } else if(a.isFloat()) {
        reset();
        return true;
    } else
        return false;
}

}
}


