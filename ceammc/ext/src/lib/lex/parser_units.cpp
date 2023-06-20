
#line 1 "lex/parser_units.rl"
# include "parser_units.h"
# include "parser_numeric.h"
# include "ceammc_log.h"

# include <cstdint>
# include <cstring>
# include <cstdio>

namespace {
    using namespace ceammc;
    using namespace ceammc::music;

    struct RagelTempo {
        int ival { 0 };
        int fnum { 0 };
        int fden { 1 };
        int dur_num { 1 };
        int dur_den { 4 };
    };

    Tempo fromRagel(const RagelTempo& t)
    {
        float bpm = t.ival + t.fnum / float(t.fden);
        Tempo res { bpm, t.dur_den };
        res.setDuration(Duration(t.dur_num, t.dur_den));
        return res;
    }
}

namespace ceammc {
namespace parser {


#line 37 "lex/parser_units.cpp"
static const int units_full_start = 1;
static const int units_full_first_final = 48;
static const int units_full_error = 0;

static const int units_full_en_main = 1;


#line 39 "lex/parser_units.rl"


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

    RagelTempo bpm;
    fsm::SmpteData smpte;

    reset();

    
#line 103 "lex/parser_units.cpp"
	{
	cs = units_full_start;
	}

#line 96 "lex/parser_units.rl"
    
#line 110 "lex/parser_units.cpp"
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
#line 87 "lex/ragel_units.rl"
	{pos_ = POSITION_ABS;}
	goto st2;
st2:
	if ( ++p == pe )
		goto _test_eof2;
case 2:
#line 144 "lex/parser_units.cpp"
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
#line 87 "lex/ragel_units.rl"
	{pos_ = POSITION_ABS;}
#line 21 "lex/ragel_numeric.rl"
	{ ragel_num.sign = ((*p)=='-') ? -1 : 1; }
#line 78 "lex/ragel_units.rl"
	{smpte.np = 0;}
#line 73 "lex/ragel_units.rl"
	{smpte.sign = ((*p)=='-')?-1:1;}
	goto st3;
tr8:
#line 81 "lex/ragel_units.rl"
	{pos_ = POSITION_END;}
#line 21 "lex/ragel_numeric.rl"
	{ ragel_num.sign = ((*p)=='-') ? -1 : 1; }
#line 78 "lex/ragel_units.rl"
	{smpte.np = 0;}
#line 73 "lex/ragel_units.rl"
	{smpte.sign = ((*p)=='-')?-1:1;}
	goto st3;
tr59:
#line 82 "lex/ragel_units.rl"
	{pos_ = POSITION_CURRENT;}
#line 21 "lex/ragel_numeric.rl"
	{ ragel_num.sign = ((*p)=='-') ? -1 : 1; }
#line 78 "lex/ragel_units.rl"
	{smpte.np = 0;}
#line 73 "lex/ragel_units.rl"
	{smpte.sign = ((*p)=='-')?-1:1;}
	goto st3;
st3:
	if ( ++p == pe )
		goto _test_eof3;
case 3:
#line 192 "lex/parser_units.cpp"
	if ( (*p) == 48 )
		goto tr12;
	if ( (*p) > 53 ) {
		if ( 54 <= (*p) && (*p) <= 57 )
			goto tr14;
	} else if ( (*p) >= 49 )
		goto tr13;
	goto st0;
tr9:
#line 81 "lex/ragel_units.rl"
	{pos_ = POSITION_END;}
#line 29 "lex/ragel_numeric.rl"
	{ (ragel_num.vint *= 10) += ((*p)-'0'); }
#line 42 "lex/ragel_numeric.rl"
	{ (ragel_num.ratio.num *= 10) += ((*p)-'0'); }
#line 78 "lex/ragel_units.rl"
	{smpte.np = 0;}
#line 66 "lex/ragel_units.rl"
	{smpte.hour = 0; smpte.np++;}
#line 66 "lex/ragel_units.rl"
	{(smpte.hour *= 10) += ((*p) - '0');}
	goto st48;
tr12:
#line 29 "lex/ragel_numeric.rl"
	{ (ragel_num.vint *= 10) += ((*p)-'0'); }
#line 42 "lex/ragel_numeric.rl"
	{ (ragel_num.ratio.num *= 10) += ((*p)-'0'); }
#line 66 "lex/ragel_units.rl"
	{smpte.hour = 0; smpte.np++;}
#line 66 "lex/ragel_units.rl"
	{(smpte.hour *= 10) += ((*p) - '0');}
	goto st48;
tr60:
#line 82 "lex/ragel_units.rl"
	{pos_ = POSITION_CURRENT;}
#line 29 "lex/ragel_numeric.rl"
	{ (ragel_num.vint *= 10) += ((*p)-'0'); }
#line 42 "lex/ragel_numeric.rl"
	{ (ragel_num.ratio.num *= 10) += ((*p)-'0'); }
#line 78 "lex/ragel_units.rl"
	{smpte.np = 0;}
#line 66 "lex/ragel_units.rl"
	{smpte.hour = 0; smpte.np++;}
#line 66 "lex/ragel_units.rl"
	{(smpte.hour *= 10) += ((*p) - '0');}
	goto st48;
st48:
	if ( ++p == pe )
		goto _test_eof48;
case 48:
#line 243 "lex/parser_units.cpp"
	switch( (*p) ) {
		case 37: goto tr65;
		case 42: goto tr66;
		case 46: goto tr67;
		case 47: goto st31;
		case 72: goto tr70;
		case 95: goto tr71;
		case 99: goto tr72;
		case 100: goto tr73;
		case 104: goto tr74;
		case 109: goto tr75;
		case 112: goto tr76;
		case 114: goto tr77;
		case 115: goto tr78;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr69;
	goto st0;
tr65:
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
	goto st49;
tr79:
#line 58 "lex/ragel_numeric.rl"
	{
        const auto frac = double(ragel_num.ratio.num) / ragel_num.ratio.den;
        ragel_num.vdouble = ragel_num.vint + ragel_num.sign * frac;
        ragel_type = TYPE_FLOAT;
        ragel_cat  = CAT_NUMBER;
    }
	goto st49;
st49:
	if ( ++p == pe )
		goto _test_eof49;
case 49:
#line 287 "lex/parser_units.cpp"
	goto st0;
tr66:
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
	goto st50;
tr80:
#line 58 "lex/ragel_numeric.rl"
	{
        const auto frac = double(ragel_num.ratio.num) / ragel_num.ratio.den;
        ragel_num.vdouble = ragel_num.vint + ragel_num.sign * frac;
        ragel_type = TYPE_FLOAT;
        ragel_cat  = CAT_NUMBER;
    }
	goto st50;
st50:
	if ( ++p == pe )
		goto _test_eof50;
case 50:
#line 314 "lex/parser_units.cpp"
	goto st0;
tr67:
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
#line 333 "lex/parser_units.cpp"
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr15;
	goto st0;
tr15:
#line 54 "lex/ragel_numeric.rl"
	{
        (ragel_num.ratio.num *= 10) += ((*p) - '0');
        ragel_num.ratio.den *= 10;
    }
	goto st51;
st51:
	if ( ++p == pe )
		goto _test_eof51;
case 51:
#line 348 "lex/parser_units.cpp"
	switch( (*p) ) {
		case 37: goto tr79;
		case 42: goto tr80;
		case 72: goto tr81;
		case 95: goto tr82;
		case 99: goto tr83;
		case 100: goto tr84;
		case 104: goto tr85;
		case 109: goto tr86;
		case 112: goto tr87;
		case 114: goto tr88;
		case 115: goto tr89;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr15;
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
	goto st5;
tr81:
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
#line 388 "lex/parser_units.cpp"
	if ( (*p) == 122 )
		goto st52;
	goto st0;
st52:
	if ( ++p == pe )
		goto _test_eof52;
case 52:
	goto st0;
tr71:
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
	goto st53;
tr82:
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
#line 424 "lex/parser_units.cpp"
	switch( (*p) ) {
		case 37: goto st49;
		case 42: goto st54;
		case 72: goto st5;
		case 99: goto st55;
		case 100: goto st57;
		case 104: goto st62;
		case 109: goto st64;
		case 112: goto st15;
		case 114: goto st23;
		case 115: goto st71;
	}
	goto st0;
st54:
	if ( ++p == pe )
		goto _test_eof54;
case 54:
	goto st0;
tr72:
#line 30 "lex/ragel_numeric.rl"
	{
        ragel_num.vint *= ragel_num.sign;
        ragel_type = TYPE_INT;
        ragel_cat  = CAT_NUMBER;
    }
#line 7 "lex/ragel_units.rl"
	{ ragel_num.vdouble = ragel_num.vint; }
	goto st55;
tr83:
#line 58 "lex/ragel_numeric.rl"
	{
        const auto frac = double(ragel_num.ratio.num) / ragel_num.ratio.den;
        ragel_num.vdouble = ragel_num.vint + ragel_num.sign * frac;
        ragel_type = TYPE_FLOAT;
        ragel_cat  = CAT_NUMBER;
    }
	goto st55;
st55:
	if ( ++p == pe )
		goto _test_eof55;
case 55:
#line 466 "lex/parser_units.cpp"
	if ( (*p) == 101 )
		goto st6;
	goto st0;
st6:
	if ( ++p == pe )
		goto _test_eof6;
case 6:
	if ( (*p) == 110 )
		goto st7;
	goto st0;
st7:
	if ( ++p == pe )
		goto _test_eof7;
case 7:
	if ( (*p) == 116 )
		goto st56;
	goto st0;
st56:
	if ( ++p == pe )
		goto _test_eof56;
case 56:
	goto st0;
tr73:
#line 30 "lex/ragel_numeric.rl"
	{
        ragel_num.vint *= ragel_num.sign;
        ragel_type = TYPE_INT;
        ragel_cat  = CAT_NUMBER;
    }
#line 7 "lex/ragel_units.rl"
	{ ragel_num.vdouble = ragel_num.vint; }
	goto st57;
tr84:
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
#line 512 "lex/parser_units.cpp"
	switch( (*p) ) {
		case 97: goto st8;
		case 98: goto st59;
		case 101: goto st10;
	}
	goto st0;
st8:
	if ( ++p == pe )
		goto _test_eof8;
case 8:
	if ( (*p) == 121 )
		goto st58;
	goto st0;
st58:
	if ( ++p == pe )
		goto _test_eof58;
case 58:
	goto st0;
st59:
	if ( ++p == pe )
		goto _test_eof59;
case 59:
	if ( (*p) == 102 )
		goto st9;
	goto st0;
st9:
	if ( ++p == pe )
		goto _test_eof9;
case 9:
	if ( (*p) == 115 )
		goto st60;
	goto st0;
st60:
	if ( ++p == pe )
		goto _test_eof60;
case 60:
	goto st0;
st10:
	if ( ++p == pe )
		goto _test_eof10;
case 10:
	if ( (*p) == 103 )
		goto st61;
	goto st0;
st61:
	if ( ++p == pe )
		goto _test_eof61;
case 61:
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
	goto st62;
tr85:
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
#line 585 "lex/parser_units.cpp"
	switch( (*p) ) {
		case 111: goto st11;
		case 122: goto st52;
	}
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
	if ( (*p) == 114 )
		goto st63;
	goto st0;
st63:
	if ( ++p == pe )
		goto _test_eof63;
case 63:
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
	goto st64;
tr86:
#line 58 "lex/ragel_numeric.rl"
	{
        const auto frac = double(ragel_num.ratio.num) / ragel_num.ratio.den;
        ragel_num.vdouble = ragel_num.vint + ragel_num.sign * frac;
        ragel_type = TYPE_FLOAT;
        ragel_cat  = CAT_NUMBER;
    }
	goto st64;
st64:
	if ( ++p == pe )
		goto _test_eof64;
case 64:
#line 633 "lex/parser_units.cpp"
	switch( (*p) ) {
		case 105: goto st13;
		case 115: goto st66;
	}
	goto st0;
st13:
	if ( ++p == pe )
		goto _test_eof13;
case 13:
	if ( (*p) == 110 )
		goto st65;
	goto st0;
st65:
	if ( ++p == pe )
		goto _test_eof65;
case 65:
	goto st0;
st66:
	if ( ++p == pe )
		goto _test_eof66;
case 66:
	if ( (*p) == 101 )
		goto st14;
	goto st0;
st14:
	if ( ++p == pe )
		goto _test_eof14;
case 14:
	if ( (*p) == 99 )
		goto st67;
	goto st0;
st67:
	if ( ++p == pe )
		goto _test_eof67;
case 67:
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
#line 113 "lex/ragel_numeric.rl"
	{ragel_num.vdouble = ragel_num.vint;}
	goto st15;
tr87:
#line 58 "lex/ragel_numeric.rl"
	{
        const auto frac = double(ragel_num.ratio.num) / ragel_num.ratio.den;
        ragel_num.vdouble = ragel_num.vint + ragel_num.sign * frac;
        ragel_type = TYPE_FLOAT;
        ragel_cat  = CAT_NUMBER;
    }
	goto st15;
st15:
	if ( ++p == pe )
		goto _test_eof15;
case 15:
#line 695 "lex/parser_units.cpp"
	switch( (*p) ) {
		case 101: goto st16;
		case 104: goto st20;
	}
	goto st0;
st16:
	if ( ++p == pe )
		goto _test_eof16;
case 16:
	if ( (*p) == 114 )
		goto st17;
	goto st0;
st17:
	if ( ++p == pe )
		goto _test_eof17;
case 17:
	if ( (*p) == 99 )
		goto st68;
	goto st0;
st68:
	if ( ++p == pe )
		goto _test_eof68;
case 68:
	if ( (*p) == 101 )
		goto st18;
	goto st0;
st18:
	if ( ++p == pe )
		goto _test_eof18;
case 18:
	if ( (*p) == 110 )
		goto st19;
	goto st0;
st19:
	if ( ++p == pe )
		goto _test_eof19;
case 19:
	if ( (*p) == 116 )
		goto st69;
	goto st0;
st69:
	if ( ++p == pe )
		goto _test_eof69;
case 69:
	goto st0;
st20:
	if ( ++p == pe )
		goto _test_eof20;
case 20:
	if ( (*p) == 97 )
		goto st21;
	goto st0;
st21:
	if ( ++p == pe )
		goto _test_eof21;
case 21:
	if ( (*p) == 115 )
		goto st22;
	goto st0;
st22:
	if ( ++p == pe )
		goto _test_eof22;
case 22:
	if ( (*p) == 101 )
		goto st50;
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
	goto st23;
tr88:
#line 58 "lex/ragel_numeric.rl"
	{
        const auto frac = double(ragel_num.ratio.num) / ragel_num.ratio.den;
        ragel_num.vdouble = ragel_num.vint + ragel_num.sign * frac;
        ragel_type = TYPE_FLOAT;
        ragel_cat  = CAT_NUMBER;
    }
	goto st23;
st23:
	if ( ++p == pe )
		goto _test_eof23;
case 23:
#line 785 "lex/parser_units.cpp"
	if ( (*p) == 97 )
		goto st24;
	goto st0;
st24:
	if ( ++p == pe )
		goto _test_eof24;
case 24:
	if ( (*p) == 100 )
		goto st70;
	goto st0;
st70:
	if ( ++p == pe )
		goto _test_eof70;
case 70:
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
	goto st71;
tr89:
#line 58 "lex/ragel_numeric.rl"
	{
        const auto frac = double(ragel_num.ratio.num) / ragel_num.ratio.den;
        ragel_num.vdouble = ragel_num.vint + ragel_num.sign * frac;
        ragel_type = TYPE_FLOAT;
        ragel_cat  = CAT_NUMBER;
    }
	goto st71;
st71:
	if ( ++p == pe )
		goto _test_eof71;
case 71:
#line 824 "lex/parser_units.cpp"
	switch( (*p) ) {
		case 97: goto st72;
		case 101: goto st26;
	}
	goto st0;
st72:
	if ( ++p == pe )
		goto _test_eof72;
case 72:
	if ( (*p) == 109 )
		goto st25;
	goto st0;
st25:
	if ( ++p == pe )
		goto _test_eof25;
case 25:
	if ( (*p) == 112 )
		goto st73;
	goto st0;
st73:
	if ( ++p == pe )
		goto _test_eof73;
case 73:
	goto st0;
st26:
	if ( ++p == pe )
		goto _test_eof26;
case 26:
	switch( (*p) ) {
		case 99: goto st74;
		case 109: goto st27;
	}
	goto st0;
st74:
	if ( ++p == pe )
		goto _test_eof74;
case 74:
	goto st0;
st27:
	if ( ++p == pe )
		goto _test_eof27;
case 27:
	if ( (*p) == 105 )
		goto st75;
	goto st0;
st75:
	if ( ++p == pe )
		goto _test_eof75;
case 75:
	if ( (*p) == 116 )
		goto st28;
	goto st0;
st28:
	if ( ++p == pe )
		goto _test_eof28;
case 28:
	if ( (*p) == 111 )
		goto st29;
	goto st0;
st29:
	if ( ++p == pe )
		goto _test_eof29;
case 29:
	if ( (*p) == 110 )
		goto st30;
	goto st0;
st30:
	if ( ++p == pe )
		goto _test_eof30;
case 30:
	if ( (*p) == 101 )
		goto st76;
	goto st0;
st76:
	if ( ++p == pe )
		goto _test_eof76;
case 76:
	goto st0;
st31:
	if ( ++p == pe )
		goto _test_eof31;
case 31:
	if ( (*p) == 48 )
		goto tr44;
	if ( 49 <= (*p) && (*p) <= 57 )
		goto tr45;
	goto st0;
tr44:
#line 43 "lex/ragel_numeric.rl"
	{ (ragel_num.ratio.den *= 10) += ((*p)-'0'); }
	goto st77;
st77:
	if ( ++p == pe )
		goto _test_eof77;
case 77:
#line 920 "lex/parser_units.cpp"
	goto st0;
tr45:
#line 43 "lex/ragel_numeric.rl"
	{ (ragel_num.ratio.den *= 10) += ((*p)-'0'); }
	goto st78;
st78:
	if ( ++p == pe )
		goto _test_eof78;
case 78:
#line 930 "lex/parser_units.cpp"
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr45;
	goto st0;
tr69:
#line 66 "lex/ragel_units.rl"
	{(smpte.hour *= 10) += ((*p) - '0');}
	goto st32;
st32:
	if ( ++p == pe )
		goto _test_eof32;
case 32:
#line 942 "lex/parser_units.cpp"
	if ( (*p) == 58 )
		goto st33;
	goto st0;
st33:
	if ( ++p == pe )
		goto _test_eof33;
case 33:
	if ( 48 <= (*p) && (*p) <= 53 )
		goto tr47;
	goto st0;
tr47:
#line 67 "lex/ragel_units.rl"
	{smpte.min = 0; smpte.np++;}
#line 67 "lex/ragel_units.rl"
	{(smpte.min *= 10) += ((*p) - '0');}
	goto st34;
st34:
	if ( ++p == pe )
		goto _test_eof34;
case 34:
#line 963 "lex/parser_units.cpp"
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr48;
	goto st0;
tr48:
#line 67 "lex/ragel_units.rl"
	{(smpte.min *= 10) += ((*p) - '0');}
	goto st79;
st79:
	if ( ++p == pe )
		goto _test_eof79;
case 79:
#line 975 "lex/parser_units.cpp"
	switch( (*p) ) {
		case 46: goto st35;
		case 58: goto st36;
	}
	goto st0;
st35:
	if ( ++p == pe )
		goto _test_eof35;
case 35:
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr49;
	goto st0;
tr49:
#line 69 "lex/ragel_units.rl"
	{smpte.frame = 0;}
#line 69 "lex/ragel_units.rl"
	{(smpte.frame *= 10) += ((*p) - '0');}
	goto st80;
st80:
	if ( ++p == pe )
		goto _test_eof80;
case 80:
#line 998 "lex/parser_units.cpp"
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr116;
	goto st0;
tr116:
#line 69 "lex/ragel_units.rl"
	{(smpte.frame *= 10) += ((*p) - '0');}
	goto st81;
st81:
	if ( ++p == pe )
		goto _test_eof81;
case 81:
#line 1010 "lex/parser_units.cpp"
	goto st0;
st36:
	if ( ++p == pe )
		goto _test_eof36;
case 36:
	if ( 48 <= (*p) && (*p) <= 53 )
		goto tr50;
	goto st0;
tr50:
#line 68 "lex/ragel_units.rl"
	{smpte.sec = 0; smpte.np++;}
#line 68 "lex/ragel_units.rl"
	{(smpte.sec *= 10) += ((*p) - '0');}
	goto st37;
st37:
	if ( ++p == pe )
		goto _test_eof37;
case 37:
#line 1029 "lex/parser_units.cpp"
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr51;
	goto st0;
tr51:
#line 68 "lex/ragel_units.rl"
	{(smpte.sec *= 10) += ((*p) - '0');}
	goto st82;
st82:
	if ( ++p == pe )
		goto _test_eof82;
case 82:
#line 1041 "lex/parser_units.cpp"
	if ( (*p) == 46 )
		goto st35;
	goto st0;
tr10:
#line 81 "lex/ragel_units.rl"
	{pos_ = POSITION_END;}
#line 29 "lex/ragel_numeric.rl"
	{ (ragel_num.vint *= 10) += ((*p)-'0'); }
#line 42 "lex/ragel_numeric.rl"
	{ (ragel_num.ratio.num *= 10) += ((*p)-'0'); }
#line 78 "lex/ragel_units.rl"
	{smpte.np = 0;}
#line 66 "lex/ragel_units.rl"
	{smpte.hour = 0; smpte.np++;}
#line 66 "lex/ragel_units.rl"
	{(smpte.hour *= 10) += ((*p) - '0');}
	goto st83;
tr13:
#line 29 "lex/ragel_numeric.rl"
	{ (ragel_num.vint *= 10) += ((*p)-'0'); }
#line 42 "lex/ragel_numeric.rl"
	{ (ragel_num.ratio.num *= 10) += ((*p)-'0'); }
#line 66 "lex/ragel_units.rl"
	{smpte.hour = 0; smpte.np++;}
#line 66 "lex/ragel_units.rl"
	{(smpte.hour *= 10) += ((*p) - '0');}
	goto st83;
tr61:
#line 82 "lex/ragel_units.rl"
	{pos_ = POSITION_CURRENT;}
#line 29 "lex/ragel_numeric.rl"
	{ (ragel_num.vint *= 10) += ((*p)-'0'); }
#line 42 "lex/ragel_numeric.rl"
	{ (ragel_num.ratio.num *= 10) += ((*p)-'0'); }
#line 78 "lex/ragel_units.rl"
	{smpte.np = 0;}
#line 66 "lex/ragel_units.rl"
	{smpte.hour = 0; smpte.np++;}
#line 66 "lex/ragel_units.rl"
	{(smpte.hour *= 10) += ((*p) - '0');}
	goto st83;
st83:
	if ( ++p == pe )
		goto _test_eof83;
case 83:
#line 1087 "lex/parser_units.cpp"
	switch( (*p) ) {
		case 37: goto tr65;
		case 42: goto tr66;
		case 46: goto tr67;
		case 47: goto st31;
		case 72: goto tr70;
		case 95: goto tr71;
		case 99: goto tr72;
		case 100: goto tr73;
		case 104: goto tr74;
		case 109: goto tr75;
		case 112: goto tr76;
		case 114: goto tr77;
		case 115: goto tr78;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr117;
	goto st0;
tr117:
#line 29 "lex/ragel_numeric.rl"
	{ (ragel_num.vint *= 10) += ((*p)-'0'); }
#line 42 "lex/ragel_numeric.rl"
	{ (ragel_num.ratio.num *= 10) += ((*p)-'0'); }
#line 66 "lex/ragel_units.rl"
	{(smpte.hour *= 10) += ((*p) - '0');}
	goto st84;
st84:
	if ( ++p == pe )
		goto _test_eof84;
case 84:
#line 1118 "lex/parser_units.cpp"
	switch( (*p) ) {
		case 37: goto tr65;
		case 42: goto tr66;
		case 46: goto tr67;
		case 47: goto st31;
		case 58: goto st33;
		case 72: goto tr70;
		case 95: goto tr71;
		case 99: goto tr72;
		case 100: goto tr73;
		case 104: goto tr74;
		case 109: goto tr75;
		case 112: goto tr76;
		case 114: goto tr77;
		case 115: goto tr78;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr14;
	goto st0;
tr11:
#line 81 "lex/ragel_units.rl"
	{pos_ = POSITION_END;}
#line 29 "lex/ragel_numeric.rl"
	{ (ragel_num.vint *= 10) += ((*p)-'0'); }
#line 42 "lex/ragel_numeric.rl"
	{ (ragel_num.ratio.num *= 10) += ((*p)-'0'); }
	goto st85;
tr14:
#line 29 "lex/ragel_numeric.rl"
	{ (ragel_num.vint *= 10) += ((*p)-'0'); }
#line 42 "lex/ragel_numeric.rl"
	{ (ragel_num.ratio.num *= 10) += ((*p)-'0'); }
	goto st85;
tr62:
#line 82 "lex/ragel_units.rl"
	{pos_ = POSITION_CURRENT;}
#line 29 "lex/ragel_numeric.rl"
	{ (ragel_num.vint *= 10) += ((*p)-'0'); }
#line 42 "lex/ragel_numeric.rl"
	{ (ragel_num.ratio.num *= 10) += ((*p)-'0'); }
	goto st85;
st85:
	if ( ++p == pe )
		goto _test_eof85;
case 85:
#line 1164 "lex/parser_units.cpp"
	switch( (*p) ) {
		case 37: goto tr65;
		case 42: goto tr66;
		case 46: goto tr67;
		case 47: goto st31;
		case 72: goto tr70;
		case 95: goto tr71;
		case 99: goto tr72;
		case 100: goto tr73;
		case 104: goto tr74;
		case 109: goto tr75;
		case 112: goto tr76;
		case 114: goto tr77;
		case 115: goto tr78;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr14;
	goto st0;
tr3:
#line 8 "lex/ragel_units.rl"
	{ ragel_type = TYPE_UNKNOWN; ragel_cat = CAT_UNKNOWN; }
#line 87 "lex/ragel_units.rl"
	{pos_ = POSITION_ABS;}
#line 29 "lex/ragel_numeric.rl"
	{ (ragel_num.vint *= 10) += ((*p)-'0'); }
#line 42 "lex/ragel_numeric.rl"
	{ (ragel_num.ratio.num *= 10) += ((*p)-'0'); }
#line 78 "lex/ragel_units.rl"
	{smpte.np = 0;}
#line 66 "lex/ragel_units.rl"
	{smpte.hour = 0; smpte.np++;}
#line 66 "lex/ragel_units.rl"
	{(smpte.hour *= 10) += ((*p) - '0');}
#line 9 "lex/ragel_music.rl"
	{ bpm.dur_num = 1; bpm.dur_den = 4; }
#line 6 "lex/ragel_music.rl"
	{ (bpm.ival *= 10) += ((*p) - '0'); }
	goto st86;
st86:
	if ( ++p == pe )
		goto _test_eof86;
case 86:
#line 1207 "lex/parser_units.cpp"
	switch( (*p) ) {
		case 37: goto tr65;
		case 42: goto tr66;
		case 46: goto tr118;
		case 47: goto st31;
		case 72: goto tr70;
		case 95: goto tr119;
		case 98: goto st39;
		case 99: goto tr72;
		case 100: goto tr73;
		case 104: goto tr74;
		case 109: goto tr75;
		case 112: goto tr76;
		case 114: goto tr77;
		case 115: goto tr78;
		case 124: goto st41;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr69;
	goto st0;
tr118:
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
	goto st38;
st38:
	if ( ++p == pe )
		goto _test_eof38;
case 38:
#line 1245 "lex/parser_units.cpp"
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr52;
	goto st0;
tr52:
#line 54 "lex/ragel_numeric.rl"
	{
        (ragel_num.ratio.num *= 10) += ((*p) - '0');
        ragel_num.ratio.den *= 10;
    }
#line 7 "lex/ragel_music.rl"
	{ (bpm.fnum *= 10) += ((*p) - '0'); bpm.fden *= 10; }
	goto st87;
st87:
	if ( ++p == pe )
		goto _test_eof87;
case 87:
#line 1262 "lex/parser_units.cpp"
	switch( (*p) ) {
		case 37: goto tr79;
		case 42: goto tr80;
		case 72: goto tr81;
		case 95: goto tr122;
		case 98: goto st39;
		case 99: goto tr83;
		case 100: goto tr84;
		case 104: goto tr85;
		case 109: goto tr86;
		case 112: goto tr87;
		case 114: goto tr88;
		case 115: goto tr89;
		case 124: goto st41;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr52;
	goto st0;
tr119:
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
	goto st88;
tr122:
#line 58 "lex/ragel_numeric.rl"
	{
        const auto frac = double(ragel_num.ratio.num) / ragel_num.ratio.den;
        ragel_num.vdouble = ragel_num.vint + ragel_num.sign * frac;
        ragel_type = TYPE_FLOAT;
        ragel_cat  = CAT_NUMBER;
    }
	goto st88;
st88:
	if ( ++p == pe )
		goto _test_eof88;
case 88:
#line 1308 "lex/parser_units.cpp"
	switch( (*p) ) {
		case 37: goto st49;
		case 42: goto st54;
		case 72: goto st5;
		case 98: goto st39;
		case 99: goto st55;
		case 100: goto st57;
		case 104: goto st62;
		case 109: goto st64;
		case 112: goto st15;
		case 114: goto st23;
		case 115: goto st71;
	}
	goto st0;
tr127:
#line 24 "lex/ragel_music.rl"
	{bpm.dur_num = 1;}
	goto st39;
st39:
	if ( ++p == pe )
		goto _test_eof39;
case 39:
#line 1331 "lex/parser_units.cpp"
	if ( (*p) == 112 )
		goto st40;
	goto st0;
st40:
	if ( ++p == pe )
		goto _test_eof40;
case 40:
	if ( (*p) == 109 )
		goto st89;
	goto st0;
st89:
	if ( ++p == pe )
		goto _test_eof89;
case 89:
	goto st0;
st41:
	if ( ++p == pe )
		goto _test_eof41;
case 41:
	if ( 49 <= (*p) && (*p) <= 57 )
		goto tr55;
	goto st0;
tr55:
#line 11 "lex/ragel_music.rl"
	{ bpm.dur_num = 0; }
#line 12 "lex/ragel_music.rl"
	{ (bpm.dur_num *= 10) += ((*p) - '0'); }
#line 13 "lex/ragel_music.rl"
	{ bpm.dur_den = 0;}
#line 14 "lex/ragel_music.rl"
	{ (bpm.dur_den *= 10) += ((*p) - '0'); }
	goto st90;
tr125:
#line 12 "lex/ragel_music.rl"
	{ (bpm.dur_num *= 10) += ((*p) - '0'); }
#line 14 "lex/ragel_music.rl"
	{ (bpm.dur_den *= 10) += ((*p) - '0'); }
	goto st90;
st90:
	if ( ++p == pe )
		goto _test_eof90;
case 90:
#line 1374 "lex/parser_units.cpp"
	switch( (*p) ) {
		case 46: goto tr123;
		case 47: goto st42;
		case 95: goto tr126;
		case 98: goto tr127;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr125;
	goto st0;
tr123:
#line 24 "lex/ragel_music.rl"
	{bpm.dur_num = 1;}
#line 8 "lex/ragel_music.rl"
	{ bpm.dur_num *= 3; bpm.dur_den *= 2; }
	goto st91;
tr129:
#line 8 "lex/ragel_music.rl"
	{ bpm.dur_num *= 3; bpm.dur_den *= 2; }
	goto st91;
st91:
	if ( ++p == pe )
		goto _test_eof91;
case 91:
#line 1398 "lex/parser_units.cpp"
	switch( (*p) ) {
		case 95: goto st92;
		case 98: goto st39;
	}
	goto st0;
tr126:
#line 24 "lex/ragel_music.rl"
	{bpm.dur_num = 1;}
	goto st92;
st92:
	if ( ++p == pe )
		goto _test_eof92;
case 92:
#line 1412 "lex/parser_units.cpp"
	if ( (*p) == 98 )
		goto st39;
	goto st0;
st42:
	if ( ++p == pe )
		goto _test_eof42;
case 42:
	if ( 49 <= (*p) && (*p) <= 57 )
		goto tr56;
	goto st0;
tr56:
#line 13 "lex/ragel_music.rl"
	{ bpm.dur_den = 0;}
#line 14 "lex/ragel_music.rl"
	{ (bpm.dur_den *= 10) += ((*p) - '0'); }
	goto st93;
tr130:
#line 14 "lex/ragel_music.rl"
	{ (bpm.dur_den *= 10) += ((*p) - '0'); }
	goto st93;
st93:
	if ( ++p == pe )
		goto _test_eof93;
case 93:
#line 1437 "lex/parser_units.cpp"
	switch( (*p) ) {
		case 46: goto tr129;
		case 95: goto st92;
		case 98: goto st39;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr130;
	goto st0;
tr4:
#line 8 "lex/ragel_units.rl"
	{ ragel_type = TYPE_UNKNOWN; ragel_cat = CAT_UNKNOWN; }
#line 87 "lex/ragel_units.rl"
	{pos_ = POSITION_ABS;}
#line 29 "lex/ragel_numeric.rl"
	{ (ragel_num.vint *= 10) += ((*p)-'0'); }
#line 42 "lex/ragel_numeric.rl"
	{ (ragel_num.ratio.num *= 10) += ((*p)-'0'); }
#line 78 "lex/ragel_units.rl"
	{smpte.np = 0;}
#line 66 "lex/ragel_units.rl"
	{smpte.hour = 0; smpte.np++;}
#line 66 "lex/ragel_units.rl"
	{(smpte.hour *= 10) += ((*p) - '0');}
#line 9 "lex/ragel_music.rl"
	{ bpm.dur_num = 1; bpm.dur_den = 4; }
#line 6 "lex/ragel_music.rl"
	{ (bpm.ival *= 10) += ((*p) - '0'); }
	goto st94;
st94:
	if ( ++p == pe )
		goto _test_eof94;
case 94:
#line 1470 "lex/parser_units.cpp"
	switch( (*p) ) {
		case 37: goto tr65;
		case 42: goto tr66;
		case 46: goto tr118;
		case 47: goto st31;
		case 72: goto tr70;
		case 95: goto tr119;
		case 98: goto st39;
		case 99: goto tr72;
		case 100: goto tr73;
		case 104: goto tr74;
		case 109: goto tr75;
		case 112: goto tr76;
		case 114: goto tr77;
		case 115: goto tr78;
		case 124: goto st41;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr131;
	goto st0;
tr131:
#line 29 "lex/ragel_numeric.rl"
	{ (ragel_num.vint *= 10) += ((*p)-'0'); }
#line 42 "lex/ragel_numeric.rl"
	{ (ragel_num.ratio.num *= 10) += ((*p)-'0'); }
#line 66 "lex/ragel_units.rl"
	{(smpte.hour *= 10) += ((*p) - '0');}
#line 6 "lex/ragel_music.rl"
	{ (bpm.ival *= 10) += ((*p) - '0'); }
	goto st95;
st95:
	if ( ++p == pe )
		goto _test_eof95;
case 95:
#line 1505 "lex/parser_units.cpp"
	switch( (*p) ) {
		case 37: goto tr65;
		case 42: goto tr66;
		case 46: goto tr118;
		case 47: goto st31;
		case 58: goto st33;
		case 72: goto tr70;
		case 95: goto tr119;
		case 98: goto st39;
		case 99: goto tr72;
		case 100: goto tr73;
		case 104: goto tr74;
		case 109: goto tr75;
		case 112: goto tr76;
		case 114: goto tr77;
		case 115: goto tr78;
		case 124: goto st41;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr132;
	goto st0;
tr5:
#line 8 "lex/ragel_units.rl"
	{ ragel_type = TYPE_UNKNOWN; ragel_cat = CAT_UNKNOWN; }
#line 87 "lex/ragel_units.rl"
	{pos_ = POSITION_ABS;}
#line 29 "lex/ragel_numeric.rl"
	{ (ragel_num.vint *= 10) += ((*p)-'0'); }
#line 42 "lex/ragel_numeric.rl"
	{ (ragel_num.ratio.num *= 10) += ((*p)-'0'); }
#line 9 "lex/ragel_music.rl"
	{ bpm.dur_num = 1; bpm.dur_den = 4; }
#line 6 "lex/ragel_music.rl"
	{ (bpm.ival *= 10) += ((*p) - '0'); }
	goto st96;
tr132:
#line 29 "lex/ragel_numeric.rl"
	{ (ragel_num.vint *= 10) += ((*p)-'0'); }
#line 42 "lex/ragel_numeric.rl"
	{ (ragel_num.ratio.num *= 10) += ((*p)-'0'); }
#line 6 "lex/ragel_music.rl"
	{ (bpm.ival *= 10) += ((*p) - '0'); }
	goto st96;
st96:
	if ( ++p == pe )
		goto _test_eof96;
case 96:
#line 1553 "lex/parser_units.cpp"
	switch( (*p) ) {
		case 37: goto tr65;
		case 42: goto tr66;
		case 46: goto tr118;
		case 47: goto st31;
		case 72: goto tr70;
		case 95: goto tr119;
		case 98: goto st39;
		case 99: goto tr72;
		case 100: goto tr73;
		case 104: goto tr74;
		case 109: goto tr75;
		case 112: goto tr76;
		case 114: goto tr77;
		case 115: goto tr78;
		case 124: goto st41;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr132;
	goto st0;
tr6:
#line 8 "lex/ragel_units.rl"
	{ ragel_type = TYPE_UNKNOWN; ragel_cat = CAT_UNKNOWN; }
#line 87 "lex/ragel_units.rl"
	{pos_ = POSITION_ABS;}
	goto st43;
st43:
	if ( ++p == pe )
		goto _test_eof43;
case 43:
#line 1584 "lex/parser_units.cpp"
	if ( (*p) == 117 )
		goto st44;
	goto st0;
st44:
	if ( ++p == pe )
		goto _test_eof44;
case 44:
	if ( (*p) == 114 )
		goto st45;
	goto st0;
st45:
	if ( ++p == pe )
		goto _test_eof45;
case 45:
	switch( (*p) ) {
		case 43: goto tr59;
		case 45: goto tr59;
		case 48: goto tr60;
	}
	if ( (*p) > 53 ) {
		if ( 54 <= (*p) && (*p) <= 57 )
			goto tr62;
	} else if ( (*p) >= 49 )
		goto tr61;
	goto st0;
tr7:
#line 8 "lex/ragel_units.rl"
	{ ragel_type = TYPE_UNKNOWN; ragel_cat = CAT_UNKNOWN; }
#line 87 "lex/ragel_units.rl"
	{pos_ = POSITION_ABS;}
	goto st46;
st46:
	if ( ++p == pe )
		goto _test_eof46;
case 46:
#line 1620 "lex/parser_units.cpp"
	if ( (*p) == 110 )
		goto st47;
	goto st0;
st47:
	if ( ++p == pe )
		goto _test_eof47;
case 47:
	if ( (*p) == 100 )
		goto st2;
	goto st0;
	}
	_test_eof2: cs = 2; goto _test_eof; 
	_test_eof3: cs = 3; goto _test_eof; 
	_test_eof48: cs = 48; goto _test_eof; 
	_test_eof49: cs = 49; goto _test_eof; 
	_test_eof50: cs = 50; goto _test_eof; 
	_test_eof4: cs = 4; goto _test_eof; 
	_test_eof51: cs = 51; goto _test_eof; 
	_test_eof5: cs = 5; goto _test_eof; 
	_test_eof52: cs = 52; goto _test_eof; 
	_test_eof53: cs = 53; goto _test_eof; 
	_test_eof54: cs = 54; goto _test_eof; 
	_test_eof55: cs = 55; goto _test_eof; 
	_test_eof6: cs = 6; goto _test_eof; 
	_test_eof7: cs = 7; goto _test_eof; 
	_test_eof56: cs = 56; goto _test_eof; 
	_test_eof57: cs = 57; goto _test_eof; 
	_test_eof8: cs = 8; goto _test_eof; 
	_test_eof58: cs = 58; goto _test_eof; 
	_test_eof59: cs = 59; goto _test_eof; 
	_test_eof9: cs = 9; goto _test_eof; 
	_test_eof60: cs = 60; goto _test_eof; 
	_test_eof10: cs = 10; goto _test_eof; 
	_test_eof61: cs = 61; goto _test_eof; 
	_test_eof62: cs = 62; goto _test_eof; 
	_test_eof11: cs = 11; goto _test_eof; 
	_test_eof12: cs = 12; goto _test_eof; 
	_test_eof63: cs = 63; goto _test_eof; 
	_test_eof64: cs = 64; goto _test_eof; 
	_test_eof13: cs = 13; goto _test_eof; 
	_test_eof65: cs = 65; goto _test_eof; 
	_test_eof66: cs = 66; goto _test_eof; 
	_test_eof14: cs = 14; goto _test_eof; 
	_test_eof67: cs = 67; goto _test_eof; 
	_test_eof15: cs = 15; goto _test_eof; 
	_test_eof16: cs = 16; goto _test_eof; 
	_test_eof17: cs = 17; goto _test_eof; 
	_test_eof68: cs = 68; goto _test_eof; 
	_test_eof18: cs = 18; goto _test_eof; 
	_test_eof19: cs = 19; goto _test_eof; 
	_test_eof69: cs = 69; goto _test_eof; 
	_test_eof20: cs = 20; goto _test_eof; 
	_test_eof21: cs = 21; goto _test_eof; 
	_test_eof22: cs = 22; goto _test_eof; 
	_test_eof23: cs = 23; goto _test_eof; 
	_test_eof24: cs = 24; goto _test_eof; 
	_test_eof70: cs = 70; goto _test_eof; 
	_test_eof71: cs = 71; goto _test_eof; 
	_test_eof72: cs = 72; goto _test_eof; 
	_test_eof25: cs = 25; goto _test_eof; 
	_test_eof73: cs = 73; goto _test_eof; 
	_test_eof26: cs = 26; goto _test_eof; 
	_test_eof74: cs = 74; goto _test_eof; 
	_test_eof27: cs = 27; goto _test_eof; 
	_test_eof75: cs = 75; goto _test_eof; 
	_test_eof28: cs = 28; goto _test_eof; 
	_test_eof29: cs = 29; goto _test_eof; 
	_test_eof30: cs = 30; goto _test_eof; 
	_test_eof76: cs = 76; goto _test_eof; 
	_test_eof31: cs = 31; goto _test_eof; 
	_test_eof77: cs = 77; goto _test_eof; 
	_test_eof78: cs = 78; goto _test_eof; 
	_test_eof32: cs = 32; goto _test_eof; 
	_test_eof33: cs = 33; goto _test_eof; 
	_test_eof34: cs = 34; goto _test_eof; 
	_test_eof79: cs = 79; goto _test_eof; 
	_test_eof35: cs = 35; goto _test_eof; 
	_test_eof80: cs = 80; goto _test_eof; 
	_test_eof81: cs = 81; goto _test_eof; 
	_test_eof36: cs = 36; goto _test_eof; 
	_test_eof37: cs = 37; goto _test_eof; 
	_test_eof82: cs = 82; goto _test_eof; 
	_test_eof83: cs = 83; goto _test_eof; 
	_test_eof84: cs = 84; goto _test_eof; 
	_test_eof85: cs = 85; goto _test_eof; 
	_test_eof86: cs = 86; goto _test_eof; 
	_test_eof38: cs = 38; goto _test_eof; 
	_test_eof87: cs = 87; goto _test_eof; 
	_test_eof88: cs = 88; goto _test_eof; 
	_test_eof39: cs = 39; goto _test_eof; 
	_test_eof40: cs = 40; goto _test_eof; 
	_test_eof89: cs = 89; goto _test_eof; 
	_test_eof41: cs = 41; goto _test_eof; 
	_test_eof90: cs = 90; goto _test_eof; 
	_test_eof91: cs = 91; goto _test_eof; 
	_test_eof92: cs = 92; goto _test_eof; 
	_test_eof42: cs = 42; goto _test_eof; 
	_test_eof93: cs = 93; goto _test_eof; 
	_test_eof94: cs = 94; goto _test_eof; 
	_test_eof95: cs = 95; goto _test_eof; 
	_test_eof96: cs = 96; goto _test_eof; 
	_test_eof43: cs = 43; goto _test_eof; 
	_test_eof44: cs = 44; goto _test_eof; 
	_test_eof45: cs = 45; goto _test_eof; 
	_test_eof46: cs = 46; goto _test_eof; 
	_test_eof47: cs = 47; goto _test_eof; 

	_test_eof: {}
	if ( p == eof )
	{
	switch ( cs ) {
	case 53: 
#line 9 "lex/ragel_units.rl"
	{ ragel_cat = CAT_UNIT; }
	break;
	case 51: 
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
	case 88: 
	case 89: 
	case 91: 
	case 92: 
	case 93: 
#line 10 "lex/ragel_music.rl"
	{ ragel_cat = CAT_UNIT; ragel_type = TYPE_BPM; }
#line 9 "lex/ragel_units.rl"
	{ ragel_cat = CAT_UNIT; }
	break;
	case 55: 
	case 56: 
#line 12 "lex/ragel_units.rl"
	{ragel_type = TYPE_CENT;}
#line 9 "lex/ragel_units.rl"
	{ ragel_cat = CAT_UNIT; }
	break;
	case 57: 
	case 58: 
#line 13 "lex/ragel_units.rl"
	{ragel_type = TYPE_DAY;}
#line 9 "lex/ragel_units.rl"
	{ ragel_cat = CAT_UNIT; }
	break;
	case 59: 
	case 60: 
#line 14 "lex/ragel_units.rl"
	{ragel_type = TYPE_DB;}
#line 9 "lex/ragel_units.rl"
	{ ragel_cat = CAT_UNIT; }
	break;
	case 61: 
#line 15 "lex/ragel_units.rl"
	{ragel_type = TYPE_DEGREE;}
#line 9 "lex/ragel_units.rl"
	{ ragel_cat = CAT_UNIT; }
	break;
	case 52: 
#line 16 "lex/ragel_units.rl"
	{ragel_type = TYPE_HZ;}
#line 9 "lex/ragel_units.rl"
	{ ragel_cat = CAT_UNIT; }
	break;
	case 62: 
	case 63: 
#line 17 "lex/ragel_units.rl"
	{ragel_type = TYPE_HOUR;}
#line 9 "lex/ragel_units.rl"
	{ ragel_cat = CAT_UNIT; }
	break;
	case 64: 
	case 65: 
#line 18 "lex/ragel_units.rl"
	{ragel_type = TYPE_MIN;}
#line 9 "lex/ragel_units.rl"
	{ ragel_cat = CAT_UNIT; }
	break;
	case 66: 
	case 67: 
#line 19 "lex/ragel_units.rl"
	{ragel_type = TYPE_MSEC;}
#line 9 "lex/ragel_units.rl"
	{ ragel_cat = CAT_UNIT; }
	break;
	case 70: 
#line 20 "lex/ragel_units.rl"
	{ragel_type = TYPE_RADIAN;}
#line 9 "lex/ragel_units.rl"
	{ ragel_cat = CAT_UNIT; }
	break;
	case 72: 
	case 73: 
#line 21 "lex/ragel_units.rl"
	{ragel_type = TYPE_SAMP;}
#line 9 "lex/ragel_units.rl"
	{ ragel_cat = CAT_UNIT; }
	break;
	case 71: 
	case 74: 
#line 22 "lex/ragel_units.rl"
	{ragel_type = TYPE_SEC;}
#line 9 "lex/ragel_units.rl"
	{ ragel_cat = CAT_UNIT; }
	break;
	case 75: 
	case 76: 
#line 23 "lex/ragel_units.rl"
	{ragel_type = TYPE_SEMITONE;}
#line 9 "lex/ragel_units.rl"
	{ ragel_cat = CAT_UNIT; }
	break;
	case 68: 
	case 69: 
#line 26 "lex/ragel_units.rl"
	{ragel_type = TYPE_PERCENT;}
#line 9 "lex/ragel_units.rl"
	{ ragel_cat = CAT_UNIT; }
	break;
	case 54: 
#line 29 "lex/ragel_units.rl"
	{ragel_type = TYPE_PHASE;}
#line 9 "lex/ragel_units.rl"
	{ ragel_cat = CAT_UNIT; }
	break;
	case 79: 
	case 80: 
	case 81: 
	case 82: 
#line 52 "lex/ragel_units.rl"
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
	case 48: 
	case 83: 
	case 84: 
	case 85: 
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
	case 77: 
	case 78: 
#line 44 "lex/ragel_numeric.rl"
	{
        ragel_num.ratio.num *= ragel_num.sign;
        ragel_type = TYPE_RATIO;
        ragel_cat  = CAT_NUMBER;
    }
#line 91 "lex/ragel_units.rl"
	{ragel_type = TYPE_RATIO;}
#line 9 "lex/ragel_units.rl"
	{ ragel_cat = CAT_UNIT; }
	break;
	case 87: 
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
	case 90: 
#line 24 "lex/ragel_music.rl"
	{bpm.dur_num = 1;}
#line 10 "lex/ragel_music.rl"
	{ ragel_cat = CAT_UNIT; ragel_type = TYPE_BPM; }
#line 9 "lex/ragel_units.rl"
	{ ragel_cat = CAT_UNIT; }
	break;
	case 86: 
	case 94: 
	case 95: 
	case 96: 
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
	case 49: 
#line 26 "lex/ragel_units.rl"
	{ragel_type = TYPE_PERCENT;}
#line 70 "lex/ragel_numeric.rl"
	{
        ragel_type = TYPE_PERCENT;
        ragel_cat = CAT_NUMBER;
    }
#line 89 "lex/ragel_units.rl"
	{ragel_type = TYPE_PERCENT;}
#line 9 "lex/ragel_units.rl"
	{ ragel_cat = CAT_UNIT; }
	break;
	case 50: 
#line 29 "lex/ragel_units.rl"
	{ragel_type = TYPE_PHASE;}
#line 75 "lex/ragel_numeric.rl"
	{
        ragel_type = TYPE_PHASE;
        ragel_cat = CAT_NUMBER;
    }
#line 90 "lex/ragel_units.rl"
	{ragel_type = TYPE_PHASE;}
#line 9 "lex/ragel_units.rl"
	{ ragel_cat = CAT_UNIT; }
	break;
#line 1964 "lex/parser_units.cpp"
	}
	}

	_out: {}
	}

#line 97 "lex/parser_units.rl"

    const bool ok = cs >= 48;
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
        unit_.tempo = fromRagel(bpm);
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


#line 2021 "lex/parser_units.cpp"
static const int units_type_start = 1;
static const int units_type_first_final = 30;
static const int units_type_error = 0;

static const int units_type_en_main = 1;


#line 151 "lex/parser_units.rl"


bool UnitTypeFullMatch::parse(const char* str)
{
    reset();

    const auto len = strlen(str);
    if (len == 0)
        return false;

    const char* p = str;
    const char* pe = p + len;
    const char* eof = pe;

    
#line 2045 "lex/parser_units.cpp"
	{
	cs = units_type_start;
	}

#line 166 "lex/parser_units.rl"
    
#line 2052 "lex/parser_units.cpp"
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
#line 2559 "lex/parser_units.cpp"
	}
	}

	_out: {}
	}

#line 167 "lex/parser_units.rl"

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


