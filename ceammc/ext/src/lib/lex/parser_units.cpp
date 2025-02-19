
#line 1 "lex/parser_units.rl"
# include "parser_units.h"
# include "parser_numeric.h"
# include "ceammc_convert.h"
# include "ceammc_log.h"
# include "fmt/core.h"

# include <cstdint>
# include <cstring>
# include <cstdio>

using ragel_char = std::uint8_t;

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


#line 41 "lex/parser_units.cpp"
static const int units_full_start = 1;
static const int units_full_first_final = 49;
static const int units_full_error = 0;

static const int units_full_en_main = 1;


#line 44 "lex/parser_units.rl"


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

    const ragel_char* p = (const ragel_char*)str;
    const ragel_char* pe = p + len;
    const ragel_char* eof = pe;

    PositionType pos_ = POSITION_ABS;

    DECLARE_RAGEL_COMMON_VARS;
    DECLARE_RAGEL_NUMERIC_VARS;

    RagelTempo bpm;
    fsm::SmpteData smpte;

    reset();

    
#line 107 "lex/parser_units.cpp"
	{
	cs = units_full_start;
	}

#line 101 "lex/parser_units.rl"
    
#line 114 "lex/parser_units.cpp"
	{
	if ( p == pe )
		goto _test_eof;
	switch ( cs )
	{
case 1:
	switch( (*p) ) {
		case 36u: goto tr0;
		case 43u: goto tr2;
		case 45u: goto tr2;
		case 48u: goto tr3;
		case 99u: goto tr6;
		case 101u: goto tr7;
	}
	if ( (*p) > 53u ) {
		if ( 54u <= (*p) && (*p) <= 57u )
			goto tr5;
	} else if ( (*p) >= 49u )
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
#line 148 "lex/parser_units.cpp"
	switch( (*p) ) {
		case 43u: goto tr8;
		case 45u: goto tr8;
		case 48u: goto tr9;
	}
	if ( (*p) > 53u ) {
		if ( 54u <= (*p) && (*p) <= 57u )
			goto tr11;
	} else if ( (*p) >= 49u )
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
tr59:
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
#line 196 "lex/parser_units.cpp"
	if ( (*p) == 48u )
		goto tr12;
	if ( (*p) > 53u ) {
		if ( 54u <= (*p) && (*p) <= 57u )
			goto tr14;
	} else if ( (*p) >= 49u )
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
	goto st49;
tr12:
#line 29 "lex/ragel_numeric.rl"
	{ (ragel_num.vint *= 10) += ((*p)-'0'); }
#line 42 "lex/ragel_numeric.rl"
	{ (ragel_num.ratio.num *= 10) += ((*p)-'0'); }
#line 67 "lex/ragel_units.rl"
	{smpte.hour = 0; smpte.np++;}
#line 67 "lex/ragel_units.rl"
	{(smpte.hour *= 10) += ((*p) - '0');}
	goto st49;
tr60:
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
	goto st49;
st49:
	if ( ++p == pe )
		goto _test_eof49;
case 49:
#line 247 "lex/parser_units.cpp"
	switch( (*p) ) {
		case 37u: goto tr65;
		case 42u: goto tr66;
		case 46u: goto tr67;
		case 47u: goto st32;
		case 72u: goto tr70;
		case 95u: goto tr71;
		case 99u: goto tr72;
		case 100u: goto tr73;
		case 104u: goto tr74;
		case 109u: goto tr75;
		case 112u: goto tr76;
		case 114u: goto tr77;
		case 115u: goto tr78;
		case 194u: goto tr79;
	}
	if ( 48u <= (*p) && (*p) <= 57u )
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
#line 110 "lex/ragel_numeric.rl"
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
#line 292 "lex/parser_units.cpp"
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
#line 115 "lex/ragel_numeric.rl"
	{ragel_num.vdouble = ragel_num.vint;}
	goto st51;
tr81:
#line 58 "lex/ragel_numeric.rl"
	{
        const auto frac = double(ragel_num.ratio.num) / ragel_num.ratio.den;
        ragel_num.vdouble = ragel_num.vint + ragel_num.sign * frac;
        ragel_type = TYPE_FLOAT;
        ragel_cat  = CAT_NUMBER;
    }
	goto st51;
st51:
	if ( ++p == pe )
		goto _test_eof51;
case 51:
#line 319 "lex/parser_units.cpp"
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
#line 338 "lex/parser_units.cpp"
	if ( 48u <= (*p) && (*p) <= 57u )
		goto tr15;
	goto st0;
tr15:
#line 54 "lex/ragel_numeric.rl"
	{
        (ragel_num.ratio.num *= 10) += ((*p) - '0');
        ragel_num.ratio.den *= 10;
    }
	goto st52;
st52:
	if ( ++p == pe )
		goto _test_eof52;
case 52:
#line 353 "lex/parser_units.cpp"
	switch( (*p) ) {
		case 37u: goto tr80;
		case 42u: goto tr81;
		case 72u: goto tr82;
		case 95u: goto tr83;
		case 99u: goto tr84;
		case 100u: goto tr85;
		case 104u: goto tr86;
		case 109u: goto tr87;
		case 112u: goto tr88;
		case 114u: goto tr89;
		case 115u: goto tr90;
		case 194u: goto tr91;
	}
	if ( 48u <= (*p) && (*p) <= 57u )
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
tr82:
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
#line 394 "lex/parser_units.cpp"
	if ( (*p) == 122u )
		goto st53;
	goto st0;
st53:
	if ( ++p == pe )
		goto _test_eof53;
case 53:
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
#line 110 "lex/ragel_numeric.rl"
	{ragel_num.vdouble = ragel_num.vint;}
#line 115 "lex/ragel_numeric.rl"
	{ragel_num.vdouble = ragel_num.vint;}
	goto st54;
tr83:
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
#line 430 "lex/parser_units.cpp"
	switch( (*p) ) {
		case 37u: goto st50;
		case 42u: goto st55;
		case 72u: goto st5;
		case 99u: goto st56;
		case 100u: goto st58;
		case 104u: goto st63;
		case 109u: goto st65;
		case 112u: goto st15;
		case 114u: goto st23;
		case 115u: goto st72;
		case 194u: goto st31;
	}
	goto st0;
st55:
	if ( ++p == pe )
		goto _test_eof55;
case 55:
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
	goto st56;
tr84:
#line 58 "lex/ragel_numeric.rl"
	{
        const auto frac = double(ragel_num.ratio.num) / ragel_num.ratio.den;
        ragel_num.vdouble = ragel_num.vint + ragel_num.sign * frac;
        ragel_type = TYPE_FLOAT;
        ragel_cat  = CAT_NUMBER;
    }
	goto st56;
st56:
	if ( ++p == pe )
		goto _test_eof56;
case 56:
#line 473 "lex/parser_units.cpp"
	if ( (*p) == 101u )
		goto st6;
	goto st0;
st6:
	if ( ++p == pe )
		goto _test_eof6;
case 6:
	if ( (*p) == 110u )
		goto st7;
	goto st0;
st7:
	if ( ++p == pe )
		goto _test_eof7;
case 7:
	if ( (*p) == 116u )
		goto st57;
	goto st0;
st57:
	if ( ++p == pe )
		goto _test_eof57;
case 57:
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
	goto st58;
tr85:
#line 58 "lex/ragel_numeric.rl"
	{
        const auto frac = double(ragel_num.ratio.num) / ragel_num.ratio.den;
        ragel_num.vdouble = ragel_num.vint + ragel_num.sign * frac;
        ragel_type = TYPE_FLOAT;
        ragel_cat  = CAT_NUMBER;
    }
	goto st58;
st58:
	if ( ++p == pe )
		goto _test_eof58;
case 58:
#line 519 "lex/parser_units.cpp"
	switch( (*p) ) {
		case 97u: goto st8;
		case 98u: goto st60;
		case 101u: goto st10;
	}
	goto st0;
st8:
	if ( ++p == pe )
		goto _test_eof8;
case 8:
	if ( (*p) == 121u )
		goto st59;
	goto st0;
st59:
	if ( ++p == pe )
		goto _test_eof59;
case 59:
	goto st0;
st60:
	if ( ++p == pe )
		goto _test_eof60;
case 60:
	if ( (*p) == 102u )
		goto st9;
	goto st0;
st9:
	if ( ++p == pe )
		goto _test_eof9;
case 9:
	if ( (*p) == 115u )
		goto st61;
	goto st0;
st61:
	if ( ++p == pe )
		goto _test_eof61;
case 61:
	goto st0;
st10:
	if ( ++p == pe )
		goto _test_eof10;
case 10:
	if ( (*p) == 103u )
		goto st62;
	goto st0;
st62:
	if ( ++p == pe )
		goto _test_eof62;
case 62:
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
	goto st63;
tr86:
#line 58 "lex/ragel_numeric.rl"
	{
        const auto frac = double(ragel_num.ratio.num) / ragel_num.ratio.den;
        ragel_num.vdouble = ragel_num.vint + ragel_num.sign * frac;
        ragel_type = TYPE_FLOAT;
        ragel_cat  = CAT_NUMBER;
    }
	goto st63;
st63:
	if ( ++p == pe )
		goto _test_eof63;
case 63:
#line 592 "lex/parser_units.cpp"
	switch( (*p) ) {
		case 111u: goto st11;
		case 122u: goto st53;
	}
	goto st0;
st11:
	if ( ++p == pe )
		goto _test_eof11;
case 11:
	if ( (*p) == 117u )
		goto st12;
	goto st0;
st12:
	if ( ++p == pe )
		goto _test_eof12;
case 12:
	if ( (*p) == 114u )
		goto st64;
	goto st0;
st64:
	if ( ++p == pe )
		goto _test_eof64;
case 64:
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
	goto st65;
tr87:
#line 58 "lex/ragel_numeric.rl"
	{
        const auto frac = double(ragel_num.ratio.num) / ragel_num.ratio.den;
        ragel_num.vdouble = ragel_num.vint + ragel_num.sign * frac;
        ragel_type = TYPE_FLOAT;
        ragel_cat  = CAT_NUMBER;
    }
	goto st65;
st65:
	if ( ++p == pe )
		goto _test_eof65;
case 65:
#line 640 "lex/parser_units.cpp"
	switch( (*p) ) {
		case 105u: goto st13;
		case 115u: goto st67;
	}
	goto st0;
st13:
	if ( ++p == pe )
		goto _test_eof13;
case 13:
	if ( (*p) == 110u )
		goto st66;
	goto st0;
st66:
	if ( ++p == pe )
		goto _test_eof66;
case 66:
	goto st0;
st67:
	if ( ++p == pe )
		goto _test_eof67;
case 67:
	if ( (*p) == 101u )
		goto st14;
	goto st0;
st14:
	if ( ++p == pe )
		goto _test_eof14;
case 14:
	if ( (*p) == 99u )
		goto st68;
	goto st0;
st68:
	if ( ++p == pe )
		goto _test_eof68;
case 68:
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
#line 115 "lex/ragel_numeric.rl"
	{ragel_num.vdouble = ragel_num.vint;}
	goto st15;
tr88:
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
#line 702 "lex/parser_units.cpp"
	switch( (*p) ) {
		case 101u: goto st16;
		case 104u: goto st20;
	}
	goto st0;
st16:
	if ( ++p == pe )
		goto _test_eof16;
case 16:
	if ( (*p) == 114u )
		goto st17;
	goto st0;
st17:
	if ( ++p == pe )
		goto _test_eof17;
case 17:
	if ( (*p) == 99u )
		goto st69;
	goto st0;
st69:
	if ( ++p == pe )
		goto _test_eof69;
case 69:
	if ( (*p) == 101u )
		goto st18;
	goto st0;
st18:
	if ( ++p == pe )
		goto _test_eof18;
case 18:
	if ( (*p) == 110u )
		goto st19;
	goto st0;
st19:
	if ( ++p == pe )
		goto _test_eof19;
case 19:
	if ( (*p) == 116u )
		goto st70;
	goto st0;
st70:
	if ( ++p == pe )
		goto _test_eof70;
case 70:
	goto st0;
st20:
	if ( ++p == pe )
		goto _test_eof20;
case 20:
	if ( (*p) == 97u )
		goto st21;
	goto st0;
st21:
	if ( ++p == pe )
		goto _test_eof21;
case 21:
	if ( (*p) == 115u )
		goto st22;
	goto st0;
st22:
	if ( ++p == pe )
		goto _test_eof22;
case 22:
	if ( (*p) == 101u )
		goto st51;
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
tr89:
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
#line 792 "lex/parser_units.cpp"
	if ( (*p) == 97u )
		goto st24;
	goto st0;
st24:
	if ( ++p == pe )
		goto _test_eof24;
case 24:
	if ( (*p) == 100u )
		goto st71;
	goto st0;
st71:
	if ( ++p == pe )
		goto _test_eof71;
case 71:
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
	goto st72;
tr90:
#line 58 "lex/ragel_numeric.rl"
	{
        const auto frac = double(ragel_num.ratio.num) / ragel_num.ratio.den;
        ragel_num.vdouble = ragel_num.vint + ragel_num.sign * frac;
        ragel_type = TYPE_FLOAT;
        ragel_cat  = CAT_NUMBER;
    }
	goto st72;
st72:
	if ( ++p == pe )
		goto _test_eof72;
case 72:
#line 831 "lex/parser_units.cpp"
	switch( (*p) ) {
		case 97u: goto st73;
		case 101u: goto st26;
	}
	goto st0;
st73:
	if ( ++p == pe )
		goto _test_eof73;
case 73:
	if ( (*p) == 109u )
		goto st25;
	goto st0;
st25:
	if ( ++p == pe )
		goto _test_eof25;
case 25:
	if ( (*p) == 112u )
		goto st74;
	goto st0;
st74:
	if ( ++p == pe )
		goto _test_eof74;
case 74:
	goto st0;
st26:
	if ( ++p == pe )
		goto _test_eof26;
case 26:
	switch( (*p) ) {
		case 99u: goto st75;
		case 109u: goto st27;
	}
	goto st0;
st75:
	if ( ++p == pe )
		goto _test_eof75;
case 75:
	goto st0;
st27:
	if ( ++p == pe )
		goto _test_eof27;
case 27:
	if ( (*p) == 105u )
		goto st76;
	goto st0;
st76:
	if ( ++p == pe )
		goto _test_eof76;
case 76:
	if ( (*p) == 116u )
		goto st28;
	goto st0;
st28:
	if ( ++p == pe )
		goto _test_eof28;
case 28:
	if ( (*p) == 111u )
		goto st29;
	goto st0;
st29:
	if ( ++p == pe )
		goto _test_eof29;
case 29:
	if ( (*p) == 110u )
		goto st30;
	goto st0;
st30:
	if ( ++p == pe )
		goto _test_eof30;
case 30:
	if ( (*p) == 101u )
		goto st77;
	goto st0;
st77:
	if ( ++p == pe )
		goto _test_eof77;
case 77:
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
	goto st31;
tr91:
#line 58 "lex/ragel_numeric.rl"
	{
        const auto frac = double(ragel_num.ratio.num) / ragel_num.ratio.den;
        ragel_num.vdouble = ragel_num.vint + ragel_num.sign * frac;
        ragel_type = TYPE_FLOAT;
        ragel_cat  = CAT_NUMBER;
    }
	goto st31;
st31:
	if ( ++p == pe )
		goto _test_eof31;
case 31:
#line 933 "lex/parser_units.cpp"
	if ( (*p) == 176u )
		goto st62;
	goto st0;
st32:
	if ( ++p == pe )
		goto _test_eof32;
case 32:
	if ( (*p) == 48u )
		goto tr44;
	if ( 49u <= (*p) && (*p) <= 57u )
		goto tr45;
	goto st0;
tr44:
#line 43 "lex/ragel_numeric.rl"
	{ (ragel_num.ratio.den *= 10) += ((*p)-'0'); }
	goto st78;
st78:
	if ( ++p == pe )
		goto _test_eof78;
case 78:
#line 954 "lex/parser_units.cpp"
	goto st0;
tr45:
#line 43 "lex/ragel_numeric.rl"
	{ (ragel_num.ratio.den *= 10) += ((*p)-'0'); }
	goto st79;
st79:
	if ( ++p == pe )
		goto _test_eof79;
case 79:
#line 964 "lex/parser_units.cpp"
	if ( 48u <= (*p) && (*p) <= 57u )
		goto tr45;
	goto st0;
tr69:
#line 67 "lex/ragel_units.rl"
	{(smpte.hour *= 10) += ((*p) - '0');}
	goto st33;
st33:
	if ( ++p == pe )
		goto _test_eof33;
case 33:
#line 976 "lex/parser_units.cpp"
	if ( (*p) == 58u )
		goto st34;
	goto st0;
st34:
	if ( ++p == pe )
		goto _test_eof34;
case 34:
	if ( 48u <= (*p) && (*p) <= 53u )
		goto tr47;
	goto st0;
tr47:
#line 68 "lex/ragel_units.rl"
	{smpte.min = 0; smpte.np++;}
#line 68 "lex/ragel_units.rl"
	{(smpte.min *= 10) += ((*p) - '0');}
	goto st35;
st35:
	if ( ++p == pe )
		goto _test_eof35;
case 35:
#line 997 "lex/parser_units.cpp"
	if ( 48u <= (*p) && (*p) <= 57u )
		goto tr48;
	goto st0;
tr48:
#line 68 "lex/ragel_units.rl"
	{(smpte.min *= 10) += ((*p) - '0');}
	goto st80;
st80:
	if ( ++p == pe )
		goto _test_eof80;
case 80:
#line 1009 "lex/parser_units.cpp"
	switch( (*p) ) {
		case 46u: goto st36;
		case 58u: goto st37;
	}
	goto st0;
st36:
	if ( ++p == pe )
		goto _test_eof36;
case 36:
	if ( 48u <= (*p) && (*p) <= 57u )
		goto tr49;
	goto st0;
tr49:
#line 70 "lex/ragel_units.rl"
	{smpte.frame = 0;}
#line 70 "lex/ragel_units.rl"
	{(smpte.frame *= 10) += ((*p) - '0');}
	goto st81;
st81:
	if ( ++p == pe )
		goto _test_eof81;
case 81:
#line 1032 "lex/parser_units.cpp"
	if ( 48u <= (*p) && (*p) <= 57u )
		goto tr119;
	goto st0;
tr119:
#line 70 "lex/ragel_units.rl"
	{(smpte.frame *= 10) += ((*p) - '0');}
	goto st82;
st82:
	if ( ++p == pe )
		goto _test_eof82;
case 82:
#line 1044 "lex/parser_units.cpp"
	goto st0;
st37:
	if ( ++p == pe )
		goto _test_eof37;
case 37:
	if ( 48u <= (*p) && (*p) <= 53u )
		goto tr50;
	goto st0;
tr50:
#line 69 "lex/ragel_units.rl"
	{smpte.sec = 0; smpte.np++;}
#line 69 "lex/ragel_units.rl"
	{(smpte.sec *= 10) += ((*p) - '0');}
	goto st38;
st38:
	if ( ++p == pe )
		goto _test_eof38;
case 38:
#line 1063 "lex/parser_units.cpp"
	if ( 48u <= (*p) && (*p) <= 57u )
		goto tr51;
	goto st0;
tr51:
#line 69 "lex/ragel_units.rl"
	{(smpte.sec *= 10) += ((*p) - '0');}
	goto st83;
st83:
	if ( ++p == pe )
		goto _test_eof83;
case 83:
#line 1075 "lex/parser_units.cpp"
	if ( (*p) == 46u )
		goto st36;
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
	goto st84;
tr13:
#line 29 "lex/ragel_numeric.rl"
	{ (ragel_num.vint *= 10) += ((*p)-'0'); }
#line 42 "lex/ragel_numeric.rl"
	{ (ragel_num.ratio.num *= 10) += ((*p)-'0'); }
#line 67 "lex/ragel_units.rl"
	{smpte.hour = 0; smpte.np++;}
#line 67 "lex/ragel_units.rl"
	{(smpte.hour *= 10) += ((*p) - '0');}
	goto st84;
tr61:
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
	goto st84;
st84:
	if ( ++p == pe )
		goto _test_eof84;
case 84:
#line 1121 "lex/parser_units.cpp"
	switch( (*p) ) {
		case 37u: goto tr65;
		case 42u: goto tr66;
		case 46u: goto tr67;
		case 47u: goto st32;
		case 72u: goto tr70;
		case 95u: goto tr71;
		case 99u: goto tr72;
		case 100u: goto tr73;
		case 104u: goto tr74;
		case 109u: goto tr75;
		case 112u: goto tr76;
		case 114u: goto tr77;
		case 115u: goto tr78;
		case 194u: goto tr79;
	}
	if ( 48u <= (*p) && (*p) <= 57u )
		goto tr120;
	goto st0;
tr120:
#line 29 "lex/ragel_numeric.rl"
	{ (ragel_num.vint *= 10) += ((*p)-'0'); }
#line 42 "lex/ragel_numeric.rl"
	{ (ragel_num.ratio.num *= 10) += ((*p)-'0'); }
#line 67 "lex/ragel_units.rl"
	{(smpte.hour *= 10) += ((*p) - '0');}
	goto st85;
st85:
	if ( ++p == pe )
		goto _test_eof85;
case 85:
#line 1153 "lex/parser_units.cpp"
	switch( (*p) ) {
		case 37u: goto tr65;
		case 42u: goto tr66;
		case 46u: goto tr67;
		case 47u: goto st32;
		case 58u: goto st34;
		case 72u: goto tr70;
		case 95u: goto tr71;
		case 99u: goto tr72;
		case 100u: goto tr73;
		case 104u: goto tr74;
		case 109u: goto tr75;
		case 112u: goto tr76;
		case 114u: goto tr77;
		case 115u: goto tr78;
		case 194u: goto tr79;
	}
	if ( 48u <= (*p) && (*p) <= 57u )
		goto tr14;
	goto st0;
tr11:
#line 82 "lex/ragel_units.rl"
	{pos_ = POSITION_END;}
#line 29 "lex/ragel_numeric.rl"
	{ (ragel_num.vint *= 10) += ((*p)-'0'); }
#line 42 "lex/ragel_numeric.rl"
	{ (ragel_num.ratio.num *= 10) += ((*p)-'0'); }
	goto st86;
tr14:
#line 29 "lex/ragel_numeric.rl"
	{ (ragel_num.vint *= 10) += ((*p)-'0'); }
#line 42 "lex/ragel_numeric.rl"
	{ (ragel_num.ratio.num *= 10) += ((*p)-'0'); }
	goto st86;
tr62:
#line 83 "lex/ragel_units.rl"
	{pos_ = POSITION_CURRENT;}
#line 29 "lex/ragel_numeric.rl"
	{ (ragel_num.vint *= 10) += ((*p)-'0'); }
#line 42 "lex/ragel_numeric.rl"
	{ (ragel_num.ratio.num *= 10) += ((*p)-'0'); }
	goto st86;
st86:
	if ( ++p == pe )
		goto _test_eof86;
case 86:
#line 1200 "lex/parser_units.cpp"
	switch( (*p) ) {
		case 37u: goto tr65;
		case 42u: goto tr66;
		case 46u: goto tr67;
		case 47u: goto st32;
		case 72u: goto tr70;
		case 95u: goto tr71;
		case 99u: goto tr72;
		case 100u: goto tr73;
		case 104u: goto tr74;
		case 109u: goto tr75;
		case 112u: goto tr76;
		case 114u: goto tr77;
		case 115u: goto tr78;
		case 194u: goto tr79;
	}
	if ( 48u <= (*p) && (*p) <= 57u )
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
	goto st87;
st87:
	if ( ++p == pe )
		goto _test_eof87;
case 87:
#line 1244 "lex/parser_units.cpp"
	switch( (*p) ) {
		case 37u: goto tr65;
		case 42u: goto tr66;
		case 46u: goto tr121;
		case 47u: goto st32;
		case 72u: goto tr70;
		case 95u: goto tr122;
		case 98u: goto st40;
		case 99u: goto tr72;
		case 100u: goto tr73;
		case 104u: goto tr74;
		case 109u: goto tr75;
		case 112u: goto tr76;
		case 114u: goto tr77;
		case 115u: goto tr78;
		case 124u: goto st42;
		case 194u: goto tr79;
	}
	if ( 48u <= (*p) && (*p) <= 57u )
		goto tr69;
	goto st0;
tr121:
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
	goto st39;
st39:
	if ( ++p == pe )
		goto _test_eof39;
case 39:
#line 1283 "lex/parser_units.cpp"
	if ( 48u <= (*p) && (*p) <= 57u )
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
	goto st88;
st88:
	if ( ++p == pe )
		goto _test_eof88;
case 88:
#line 1300 "lex/parser_units.cpp"
	switch( (*p) ) {
		case 37u: goto tr80;
		case 42u: goto tr81;
		case 72u: goto tr82;
		case 95u: goto tr125;
		case 98u: goto st40;
		case 99u: goto tr84;
		case 100u: goto tr85;
		case 104u: goto tr86;
		case 109u: goto tr87;
		case 112u: goto tr88;
		case 114u: goto tr89;
		case 115u: goto tr90;
		case 124u: goto st42;
		case 194u: goto tr91;
	}
	if ( 48u <= (*p) && (*p) <= 57u )
		goto tr52;
	goto st0;
tr122:
#line 30 "lex/ragel_numeric.rl"
	{
        ragel_num.vint *= ragel_num.sign;
        ragel_type = TYPE_INT;
        ragel_cat  = CAT_NUMBER;
    }
#line 7 "lex/ragel_units.rl"
	{ ragel_num.vdouble = ragel_num.vint; }
#line 110 "lex/ragel_numeric.rl"
	{ragel_num.vdouble = ragel_num.vint;}
#line 115 "lex/ragel_numeric.rl"
	{ragel_num.vdouble = ragel_num.vint;}
	goto st89;
tr125:
#line 58 "lex/ragel_numeric.rl"
	{
        const auto frac = double(ragel_num.ratio.num) / ragel_num.ratio.den;
        ragel_num.vdouble = ragel_num.vint + ragel_num.sign * frac;
        ragel_type = TYPE_FLOAT;
        ragel_cat  = CAT_NUMBER;
    }
	goto st89;
st89:
	if ( ++p == pe )
		goto _test_eof89;
case 89:
#line 1347 "lex/parser_units.cpp"
	switch( (*p) ) {
		case 37u: goto st50;
		case 42u: goto st55;
		case 72u: goto st5;
		case 98u: goto st40;
		case 99u: goto st56;
		case 100u: goto st58;
		case 104u: goto st63;
		case 109u: goto st65;
		case 112u: goto st15;
		case 114u: goto st23;
		case 115u: goto st72;
		case 194u: goto st31;
	}
	goto st0;
tr130:
#line 24 "lex/ragel_music.rl"
	{bpm.dur_num = 1;}
	goto st40;
st40:
	if ( ++p == pe )
		goto _test_eof40;
case 40:
#line 1371 "lex/parser_units.cpp"
	if ( (*p) == 112u )
		goto st41;
	goto st0;
st41:
	if ( ++p == pe )
		goto _test_eof41;
case 41:
	if ( (*p) == 109u )
		goto st90;
	goto st0;
st90:
	if ( ++p == pe )
		goto _test_eof90;
case 90:
	goto st0;
st42:
	if ( ++p == pe )
		goto _test_eof42;
case 42:
	if ( 49u <= (*p) && (*p) <= 57u )
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
	goto st91;
tr128:
#line 12 "lex/ragel_music.rl"
	{ (bpm.dur_num *= 10) += ((*p) - '0'); }
#line 14 "lex/ragel_music.rl"
	{ (bpm.dur_den *= 10) += ((*p) - '0'); }
	goto st91;
st91:
	if ( ++p == pe )
		goto _test_eof91;
case 91:
#line 1414 "lex/parser_units.cpp"
	switch( (*p) ) {
		case 46u: goto tr126;
		case 47u: goto st43;
		case 95u: goto tr129;
		case 98u: goto tr130;
	}
	if ( 48u <= (*p) && (*p) <= 57u )
		goto tr128;
	goto st0;
tr126:
#line 24 "lex/ragel_music.rl"
	{bpm.dur_num = 1;}
#line 8 "lex/ragel_music.rl"
	{ bpm.dur_num *= 3; bpm.dur_den *= 2; }
	goto st92;
tr132:
#line 8 "lex/ragel_music.rl"
	{ bpm.dur_num *= 3; bpm.dur_den *= 2; }
	goto st92;
st92:
	if ( ++p == pe )
		goto _test_eof92;
case 92:
#line 1438 "lex/parser_units.cpp"
	switch( (*p) ) {
		case 95u: goto st93;
		case 98u: goto st40;
	}
	goto st0;
tr129:
#line 24 "lex/ragel_music.rl"
	{bpm.dur_num = 1;}
	goto st93;
st93:
	if ( ++p == pe )
		goto _test_eof93;
case 93:
#line 1452 "lex/parser_units.cpp"
	if ( (*p) == 98u )
		goto st40;
	goto st0;
st43:
	if ( ++p == pe )
		goto _test_eof43;
case 43:
	if ( 49u <= (*p) && (*p) <= 57u )
		goto tr56;
	goto st0;
tr56:
#line 13 "lex/ragel_music.rl"
	{ bpm.dur_den = 0;}
#line 14 "lex/ragel_music.rl"
	{ (bpm.dur_den *= 10) += ((*p) - '0'); }
	goto st94;
tr133:
#line 14 "lex/ragel_music.rl"
	{ (bpm.dur_den *= 10) += ((*p) - '0'); }
	goto st94;
st94:
	if ( ++p == pe )
		goto _test_eof94;
case 94:
#line 1477 "lex/parser_units.cpp"
	switch( (*p) ) {
		case 46u: goto tr132;
		case 95u: goto st93;
		case 98u: goto st40;
	}
	if ( 48u <= (*p) && (*p) <= 57u )
		goto tr133;
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
	goto st95;
st95:
	if ( ++p == pe )
		goto _test_eof95;
case 95:
#line 1510 "lex/parser_units.cpp"
	switch( (*p) ) {
		case 37u: goto tr65;
		case 42u: goto tr66;
		case 46u: goto tr121;
		case 47u: goto st32;
		case 72u: goto tr70;
		case 95u: goto tr122;
		case 98u: goto st40;
		case 99u: goto tr72;
		case 100u: goto tr73;
		case 104u: goto tr74;
		case 109u: goto tr75;
		case 112u: goto tr76;
		case 114u: goto tr77;
		case 115u: goto tr78;
		case 124u: goto st42;
		case 194u: goto tr79;
	}
	if ( 48u <= (*p) && (*p) <= 57u )
		goto tr134;
	goto st0;
tr134:
#line 29 "lex/ragel_numeric.rl"
	{ (ragel_num.vint *= 10) += ((*p)-'0'); }
#line 42 "lex/ragel_numeric.rl"
	{ (ragel_num.ratio.num *= 10) += ((*p)-'0'); }
#line 67 "lex/ragel_units.rl"
	{(smpte.hour *= 10) += ((*p) - '0');}
#line 6 "lex/ragel_music.rl"
	{ (bpm.ival *= 10) += ((*p) - '0'); }
	goto st96;
st96:
	if ( ++p == pe )
		goto _test_eof96;
case 96:
#line 1546 "lex/parser_units.cpp"
	switch( (*p) ) {
		case 37u: goto tr65;
		case 42u: goto tr66;
		case 46u: goto tr121;
		case 47u: goto st32;
		case 58u: goto st34;
		case 72u: goto tr70;
		case 95u: goto tr122;
		case 98u: goto st40;
		case 99u: goto tr72;
		case 100u: goto tr73;
		case 104u: goto tr74;
		case 109u: goto tr75;
		case 112u: goto tr76;
		case 114u: goto tr77;
		case 115u: goto tr78;
		case 124u: goto st42;
		case 194u: goto tr79;
	}
	if ( 48u <= (*p) && (*p) <= 57u )
		goto tr135;
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
	goto st97;
tr135:
#line 29 "lex/ragel_numeric.rl"
	{ (ragel_num.vint *= 10) += ((*p)-'0'); }
#line 42 "lex/ragel_numeric.rl"
	{ (ragel_num.ratio.num *= 10) += ((*p)-'0'); }
#line 6 "lex/ragel_music.rl"
	{ (bpm.ival *= 10) += ((*p) - '0'); }
	goto st97;
st97:
	if ( ++p == pe )
		goto _test_eof97;
case 97:
#line 1595 "lex/parser_units.cpp"
	switch( (*p) ) {
		case 37u: goto tr65;
		case 42u: goto tr66;
		case 46u: goto tr121;
		case 47u: goto st32;
		case 72u: goto tr70;
		case 95u: goto tr122;
		case 98u: goto st40;
		case 99u: goto tr72;
		case 100u: goto tr73;
		case 104u: goto tr74;
		case 109u: goto tr75;
		case 112u: goto tr76;
		case 114u: goto tr77;
		case 115u: goto tr78;
		case 124u: goto st42;
		case 194u: goto tr79;
	}
	if ( 48u <= (*p) && (*p) <= 57u )
		goto tr135;
	goto st0;
tr6:
#line 8 "lex/ragel_units.rl"
	{ ragel_type = TYPE_UNKNOWN; ragel_cat = CAT_UNKNOWN; }
#line 88 "lex/ragel_units.rl"
	{pos_ = POSITION_ABS;}
	goto st44;
st44:
	if ( ++p == pe )
		goto _test_eof44;
case 44:
#line 1627 "lex/parser_units.cpp"
	if ( (*p) == 117u )
		goto st45;
	goto st0;
st45:
	if ( ++p == pe )
		goto _test_eof45;
case 45:
	if ( (*p) == 114u )
		goto st46;
	goto st0;
st46:
	if ( ++p == pe )
		goto _test_eof46;
case 46:
	switch( (*p) ) {
		case 43u: goto tr59;
		case 45u: goto tr59;
		case 48u: goto tr60;
	}
	if ( (*p) > 53u ) {
		if ( 54u <= (*p) && (*p) <= 57u )
			goto tr62;
	} else if ( (*p) >= 49u )
		goto tr61;
	goto st0;
tr7:
#line 8 "lex/ragel_units.rl"
	{ ragel_type = TYPE_UNKNOWN; ragel_cat = CAT_UNKNOWN; }
#line 88 "lex/ragel_units.rl"
	{pos_ = POSITION_ABS;}
	goto st47;
st47:
	if ( ++p == pe )
		goto _test_eof47;
case 47:
#line 1663 "lex/parser_units.cpp"
	if ( (*p) == 110u )
		goto st48;
	goto st0;
st48:
	if ( ++p == pe )
		goto _test_eof48;
case 48:
	if ( (*p) == 100u )
		goto st2;
	goto st0;
	}
	_test_eof2: cs = 2; goto _test_eof; 
	_test_eof3: cs = 3; goto _test_eof; 
	_test_eof49: cs = 49; goto _test_eof; 
	_test_eof50: cs = 50; goto _test_eof; 
	_test_eof51: cs = 51; goto _test_eof; 
	_test_eof4: cs = 4; goto _test_eof; 
	_test_eof52: cs = 52; goto _test_eof; 
	_test_eof5: cs = 5; goto _test_eof; 
	_test_eof53: cs = 53; goto _test_eof; 
	_test_eof54: cs = 54; goto _test_eof; 
	_test_eof55: cs = 55; goto _test_eof; 
	_test_eof56: cs = 56; goto _test_eof; 
	_test_eof6: cs = 6; goto _test_eof; 
	_test_eof7: cs = 7; goto _test_eof; 
	_test_eof57: cs = 57; goto _test_eof; 
	_test_eof58: cs = 58; goto _test_eof; 
	_test_eof8: cs = 8; goto _test_eof; 
	_test_eof59: cs = 59; goto _test_eof; 
	_test_eof60: cs = 60; goto _test_eof; 
	_test_eof9: cs = 9; goto _test_eof; 
	_test_eof61: cs = 61; goto _test_eof; 
	_test_eof10: cs = 10; goto _test_eof; 
	_test_eof62: cs = 62; goto _test_eof; 
	_test_eof63: cs = 63; goto _test_eof; 
	_test_eof11: cs = 11; goto _test_eof; 
	_test_eof12: cs = 12; goto _test_eof; 
	_test_eof64: cs = 64; goto _test_eof; 
	_test_eof65: cs = 65; goto _test_eof; 
	_test_eof13: cs = 13; goto _test_eof; 
	_test_eof66: cs = 66; goto _test_eof; 
	_test_eof67: cs = 67; goto _test_eof; 
	_test_eof14: cs = 14; goto _test_eof; 
	_test_eof68: cs = 68; goto _test_eof; 
	_test_eof15: cs = 15; goto _test_eof; 
	_test_eof16: cs = 16; goto _test_eof; 
	_test_eof17: cs = 17; goto _test_eof; 
	_test_eof69: cs = 69; goto _test_eof; 
	_test_eof18: cs = 18; goto _test_eof; 
	_test_eof19: cs = 19; goto _test_eof; 
	_test_eof70: cs = 70; goto _test_eof; 
	_test_eof20: cs = 20; goto _test_eof; 
	_test_eof21: cs = 21; goto _test_eof; 
	_test_eof22: cs = 22; goto _test_eof; 
	_test_eof23: cs = 23; goto _test_eof; 
	_test_eof24: cs = 24; goto _test_eof; 
	_test_eof71: cs = 71; goto _test_eof; 
	_test_eof72: cs = 72; goto _test_eof; 
	_test_eof73: cs = 73; goto _test_eof; 
	_test_eof25: cs = 25; goto _test_eof; 
	_test_eof74: cs = 74; goto _test_eof; 
	_test_eof26: cs = 26; goto _test_eof; 
	_test_eof75: cs = 75; goto _test_eof; 
	_test_eof27: cs = 27; goto _test_eof; 
	_test_eof76: cs = 76; goto _test_eof; 
	_test_eof28: cs = 28; goto _test_eof; 
	_test_eof29: cs = 29; goto _test_eof; 
	_test_eof30: cs = 30; goto _test_eof; 
	_test_eof77: cs = 77; goto _test_eof; 
	_test_eof31: cs = 31; goto _test_eof; 
	_test_eof32: cs = 32; goto _test_eof; 
	_test_eof78: cs = 78; goto _test_eof; 
	_test_eof79: cs = 79; goto _test_eof; 
	_test_eof33: cs = 33; goto _test_eof; 
	_test_eof34: cs = 34; goto _test_eof; 
	_test_eof35: cs = 35; goto _test_eof; 
	_test_eof80: cs = 80; goto _test_eof; 
	_test_eof36: cs = 36; goto _test_eof; 
	_test_eof81: cs = 81; goto _test_eof; 
	_test_eof82: cs = 82; goto _test_eof; 
	_test_eof37: cs = 37; goto _test_eof; 
	_test_eof38: cs = 38; goto _test_eof; 
	_test_eof83: cs = 83; goto _test_eof; 
	_test_eof84: cs = 84; goto _test_eof; 
	_test_eof85: cs = 85; goto _test_eof; 
	_test_eof86: cs = 86; goto _test_eof; 
	_test_eof87: cs = 87; goto _test_eof; 
	_test_eof39: cs = 39; goto _test_eof; 
	_test_eof88: cs = 88; goto _test_eof; 
	_test_eof89: cs = 89; goto _test_eof; 
	_test_eof40: cs = 40; goto _test_eof; 
	_test_eof41: cs = 41; goto _test_eof; 
	_test_eof90: cs = 90; goto _test_eof; 
	_test_eof42: cs = 42; goto _test_eof; 
	_test_eof91: cs = 91; goto _test_eof; 
	_test_eof92: cs = 92; goto _test_eof; 
	_test_eof93: cs = 93; goto _test_eof; 
	_test_eof43: cs = 43; goto _test_eof; 
	_test_eof94: cs = 94; goto _test_eof; 
	_test_eof95: cs = 95; goto _test_eof; 
	_test_eof96: cs = 96; goto _test_eof; 
	_test_eof97: cs = 97; goto _test_eof; 
	_test_eof44: cs = 44; goto _test_eof; 
	_test_eof45: cs = 45; goto _test_eof; 
	_test_eof46: cs = 46; goto _test_eof; 
	_test_eof47: cs = 47; goto _test_eof; 
	_test_eof48: cs = 48; goto _test_eof; 

	_test_eof: {}
	if ( p == eof )
	{
	switch ( cs ) {
	case 54: 
#line 9 "lex/ragel_units.rl"
	{ ragel_cat = CAT_UNIT; }
	break;
	case 52: 
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
	case 89: 
	case 90: 
	case 92: 
	case 93: 
	case 94: 
#line 10 "lex/ragel_music.rl"
	{ ragel_cat = CAT_UNIT; ragel_type = TYPE_BPM; }
#line 9 "lex/ragel_units.rl"
	{ ragel_cat = CAT_UNIT; }
	break;
	case 56: 
	case 57: 
#line 12 "lex/ragel_units.rl"
	{ragel_type = TYPE_CENT;}
#line 9 "lex/ragel_units.rl"
	{ ragel_cat = CAT_UNIT; }
	break;
	case 58: 
	case 59: 
#line 13 "lex/ragel_units.rl"
	{ragel_type = TYPE_DAY;}
#line 9 "lex/ragel_units.rl"
	{ ragel_cat = CAT_UNIT; }
	break;
	case 60: 
	case 61: 
#line 14 "lex/ragel_units.rl"
	{ragel_type = TYPE_DB;}
#line 9 "lex/ragel_units.rl"
	{ ragel_cat = CAT_UNIT; }
	break;
	case 62: 
#line 15 "lex/ragel_units.rl"
	{ragel_type = TYPE_DEGREE;}
#line 9 "lex/ragel_units.rl"
	{ ragel_cat = CAT_UNIT; }
	break;
	case 53: 
#line 16 "lex/ragel_units.rl"
	{ragel_type = TYPE_HZ;}
#line 9 "lex/ragel_units.rl"
	{ ragel_cat = CAT_UNIT; }
	break;
	case 63: 
	case 64: 
#line 17 "lex/ragel_units.rl"
	{ragel_type = TYPE_HOUR;}
#line 9 "lex/ragel_units.rl"
	{ ragel_cat = CAT_UNIT; }
	break;
	case 65: 
	case 66: 
#line 18 "lex/ragel_units.rl"
	{ragel_type = TYPE_MIN;}
#line 9 "lex/ragel_units.rl"
	{ ragel_cat = CAT_UNIT; }
	break;
	case 67: 
	case 68: 
#line 19 "lex/ragel_units.rl"
	{ragel_type = TYPE_MSEC;}
#line 9 "lex/ragel_units.rl"
	{ ragel_cat = CAT_UNIT; }
	break;
	case 71: 
#line 20 "lex/ragel_units.rl"
	{ragel_type = TYPE_RADIAN;}
#line 9 "lex/ragel_units.rl"
	{ ragel_cat = CAT_UNIT; }
	break;
	case 73: 
	case 74: 
#line 21 "lex/ragel_units.rl"
	{ragel_type = TYPE_SAMP;}
#line 9 "lex/ragel_units.rl"
	{ ragel_cat = CAT_UNIT; }
	break;
	case 72: 
	case 75: 
#line 22 "lex/ragel_units.rl"
	{ragel_type = TYPE_SEC;}
#line 9 "lex/ragel_units.rl"
	{ ragel_cat = CAT_UNIT; }
	break;
	case 76: 
	case 77: 
#line 23 "lex/ragel_units.rl"
	{ragel_type = TYPE_SEMITONE;}
#line 9 "lex/ragel_units.rl"
	{ ragel_cat = CAT_UNIT; }
	break;
	case 69: 
	case 70: 
#line 26 "lex/ragel_units.rl"
	{ragel_type = TYPE_PERCENT;}
#line 9 "lex/ragel_units.rl"
	{ ragel_cat = CAT_UNIT; }
	break;
	case 55: 
#line 29 "lex/ragel_units.rl"
	{ragel_type = TYPE_PHASE;}
#line 9 "lex/ragel_units.rl"
	{ ragel_cat = CAT_UNIT; }
	break;
	case 80: 
	case 81: 
	case 82: 
	case 83: 
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
	case 49: 
	case 84: 
	case 85: 
	case 86: 
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
	case 78: 
	case 79: 
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
	case 88: 
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
	case 91: 
#line 24 "lex/ragel_music.rl"
	{bpm.dur_num = 1;}
#line 10 "lex/ragel_music.rl"
	{ ragel_cat = CAT_UNIT; ragel_type = TYPE_BPM; }
#line 9 "lex/ragel_units.rl"
	{ ragel_cat = CAT_UNIT; }
	break;
	case 87: 
	case 95: 
	case 96: 
	case 97: 
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
	case 50: 
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
	case 51: 
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
#line 2008 "lex/parser_units.cpp"
	}
	}

	_out: {}
	}

#line 102 "lex/parser_units.rl"

    const bool ok = cs >= 49;
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


#line 2065 "lex/parser_units.cpp"
static const int units_type_start = 1;
static const int units_type_first_final = 31;
static const int units_type_error = 0;

static const int units_type_en_main = 1;


#line 157 "lex/parser_units.rl"


bool UnitTypeFullMatch::parse(const char* str)
{
    reset();

    const auto len = strlen(str);
    if (len == 0)
        return false;

    const ragel_char* p = (const ragel_char*)str;
    const ragel_char* pe = p + len;
    const ragel_char* eof = pe;

    
#line 2089 "lex/parser_units.cpp"
	{
	cs = units_type_start;
	}

#line 172 "lex/parser_units.rl"
    
#line 2096 "lex/parser_units.cpp"
	{
	if ( p == pe )
		goto _test_eof;
	switch ( cs )
	{
case 1:
	switch( (*p) ) {
		case 37u: goto st31;
		case 42u: goto st32;
		case 72u: goto st2;
		case 98u: goto st3;
		case 99u: goto st35;
		case 100u: goto st37;
		case 104u: goto st42;
		case 109u: goto st44;
		case 112u: goto st14;
		case 114u: goto st22;
		case 115u: goto st50;
		case 194u: goto st30;
	}
	goto st0;
st0:
cs = 0;
	goto _out;
st31:
	if ( ++p == pe )
		goto _test_eof31;
case 31:
	goto st0;
st32:
	if ( ++p == pe )
		goto _test_eof32;
case 32:
	goto st0;
st2:
	if ( ++p == pe )
		goto _test_eof2;
case 2:
	if ( (*p) == 122u )
		goto st33;
	goto st0;
st33:
	if ( ++p == pe )
		goto _test_eof33;
case 33:
	goto st0;
st3:
	if ( ++p == pe )
		goto _test_eof3;
case 3:
	if ( (*p) == 112u )
		goto st4;
	goto st0;
st4:
	if ( ++p == pe )
		goto _test_eof4;
case 4:
	if ( (*p) == 109u )
		goto st34;
	goto st0;
st34:
	if ( ++p == pe )
		goto _test_eof34;
case 34:
	goto st0;
st35:
	if ( ++p == pe )
		goto _test_eof35;
case 35:
	if ( (*p) == 101u )
		goto st5;
	goto st0;
st5:
	if ( ++p == pe )
		goto _test_eof5;
case 5:
	if ( (*p) == 110u )
		goto st6;
	goto st0;
st6:
	if ( ++p == pe )
		goto _test_eof6;
case 6:
	if ( (*p) == 116u )
		goto st36;
	goto st0;
st36:
	if ( ++p == pe )
		goto _test_eof36;
case 36:
	goto st0;
st37:
	if ( ++p == pe )
		goto _test_eof37;
case 37:
	switch( (*p) ) {
		case 97u: goto st7;
		case 98u: goto st39;
		case 101u: goto st9;
	}
	goto st0;
st7:
	if ( ++p == pe )
		goto _test_eof7;
case 7:
	if ( (*p) == 121u )
		goto st38;
	goto st0;
st38:
	if ( ++p == pe )
		goto _test_eof38;
case 38:
	goto st0;
st39:
	if ( ++p == pe )
		goto _test_eof39;
case 39:
	if ( (*p) == 102u )
		goto st8;
	goto st0;
st8:
	if ( ++p == pe )
		goto _test_eof8;
case 8:
	if ( (*p) == 115u )
		goto st40;
	goto st0;
st40:
	if ( ++p == pe )
		goto _test_eof40;
case 40:
	goto st0;
st9:
	if ( ++p == pe )
		goto _test_eof9;
case 9:
	if ( (*p) == 103u )
		goto st41;
	goto st0;
st41:
	if ( ++p == pe )
		goto _test_eof41;
case 41:
	goto st0;
st42:
	if ( ++p == pe )
		goto _test_eof42;
case 42:
	switch( (*p) ) {
		case 111u: goto st10;
		case 122u: goto st33;
	}
	goto st0;
st10:
	if ( ++p == pe )
		goto _test_eof10;
case 10:
	if ( (*p) == 117u )
		goto st11;
	goto st0;
st11:
	if ( ++p == pe )
		goto _test_eof11;
case 11:
	if ( (*p) == 114u )
		goto st43;
	goto st0;
st43:
	if ( ++p == pe )
		goto _test_eof43;
case 43:
	goto st0;
st44:
	if ( ++p == pe )
		goto _test_eof44;
case 44:
	switch( (*p) ) {
		case 105u: goto st12;
		case 115u: goto st46;
	}
	goto st0;
st12:
	if ( ++p == pe )
		goto _test_eof12;
case 12:
	if ( (*p) == 110u )
		goto st45;
	goto st0;
st45:
	if ( ++p == pe )
		goto _test_eof45;
case 45:
	goto st0;
st46:
	if ( ++p == pe )
		goto _test_eof46;
case 46:
	if ( (*p) == 101u )
		goto st13;
	goto st0;
st13:
	if ( ++p == pe )
		goto _test_eof13;
case 13:
	if ( (*p) == 99u )
		goto st47;
	goto st0;
st47:
	if ( ++p == pe )
		goto _test_eof47;
case 47:
	goto st0;
st14:
	if ( ++p == pe )
		goto _test_eof14;
case 14:
	switch( (*p) ) {
		case 101u: goto st15;
		case 104u: goto st19;
	}
	goto st0;
st15:
	if ( ++p == pe )
		goto _test_eof15;
case 15:
	if ( (*p) == 114u )
		goto st16;
	goto st0;
st16:
	if ( ++p == pe )
		goto _test_eof16;
case 16:
	if ( (*p) == 99u )
		goto st48;
	goto st0;
st48:
	if ( ++p == pe )
		goto _test_eof48;
case 48:
	if ( (*p) == 101u )
		goto st17;
	goto st0;
st17:
	if ( ++p == pe )
		goto _test_eof17;
case 17:
	if ( (*p) == 110u )
		goto st18;
	goto st0;
st18:
	if ( ++p == pe )
		goto _test_eof18;
case 18:
	if ( (*p) == 116u )
		goto st31;
	goto st0;
st19:
	if ( ++p == pe )
		goto _test_eof19;
case 19:
	if ( (*p) == 97u )
		goto st20;
	goto st0;
st20:
	if ( ++p == pe )
		goto _test_eof20;
case 20:
	if ( (*p) == 115u )
		goto st21;
	goto st0;
st21:
	if ( ++p == pe )
		goto _test_eof21;
case 21:
	if ( (*p) == 101u )
		goto st32;
	goto st0;
st22:
	if ( ++p == pe )
		goto _test_eof22;
case 22:
	if ( (*p) == 97u )
		goto st23;
	goto st0;
st23:
	if ( ++p == pe )
		goto _test_eof23;
case 23:
	if ( (*p) == 100u )
		goto st49;
	goto st0;
st49:
	if ( ++p == pe )
		goto _test_eof49;
case 49:
	goto st0;
st50:
	if ( ++p == pe )
		goto _test_eof50;
case 50:
	switch( (*p) ) {
		case 97u: goto st51;
		case 101u: goto st25;
	}
	goto st0;
st51:
	if ( ++p == pe )
		goto _test_eof51;
case 51:
	if ( (*p) == 109u )
		goto st24;
	goto st0;
st24:
	if ( ++p == pe )
		goto _test_eof24;
case 24:
	if ( (*p) == 112u )
		goto st52;
	goto st0;
st52:
	if ( ++p == pe )
		goto _test_eof52;
case 52:
	goto st0;
st25:
	if ( ++p == pe )
		goto _test_eof25;
case 25:
	switch( (*p) ) {
		case 99u: goto st53;
		case 109u: goto st26;
	}
	goto st0;
st53:
	if ( ++p == pe )
		goto _test_eof53;
case 53:
	goto st0;
st26:
	if ( ++p == pe )
		goto _test_eof26;
case 26:
	if ( (*p) == 105u )
		goto st54;
	goto st0;
st54:
	if ( ++p == pe )
		goto _test_eof54;
case 54:
	if ( (*p) == 116u )
		goto st27;
	goto st0;
st27:
	if ( ++p == pe )
		goto _test_eof27;
case 27:
	if ( (*p) == 111u )
		goto st28;
	goto st0;
st28:
	if ( ++p == pe )
		goto _test_eof28;
case 28:
	if ( (*p) == 110u )
		goto st29;
	goto st0;
st29:
	if ( ++p == pe )
		goto _test_eof29;
case 29:
	if ( (*p) == 101u )
		goto st55;
	goto st0;
st55:
	if ( ++p == pe )
		goto _test_eof55;
case 55:
	goto st0;
st30:
	if ( ++p == pe )
		goto _test_eof30;
case 30:
	if ( (*p) == 176u )
		goto st41;
	goto st0;
	}
	_test_eof31: cs = 31; goto _test_eof; 
	_test_eof32: cs = 32; goto _test_eof; 
	_test_eof2: cs = 2; goto _test_eof; 
	_test_eof33: cs = 33; goto _test_eof; 
	_test_eof3: cs = 3; goto _test_eof; 
	_test_eof4: cs = 4; goto _test_eof; 
	_test_eof34: cs = 34; goto _test_eof; 
	_test_eof35: cs = 35; goto _test_eof; 
	_test_eof5: cs = 5; goto _test_eof; 
	_test_eof6: cs = 6; goto _test_eof; 
	_test_eof36: cs = 36; goto _test_eof; 
	_test_eof37: cs = 37; goto _test_eof; 
	_test_eof7: cs = 7; goto _test_eof; 
	_test_eof38: cs = 38; goto _test_eof; 
	_test_eof39: cs = 39; goto _test_eof; 
	_test_eof8: cs = 8; goto _test_eof; 
	_test_eof40: cs = 40; goto _test_eof; 
	_test_eof9: cs = 9; goto _test_eof; 
	_test_eof41: cs = 41; goto _test_eof; 
	_test_eof42: cs = 42; goto _test_eof; 
	_test_eof10: cs = 10; goto _test_eof; 
	_test_eof11: cs = 11; goto _test_eof; 
	_test_eof43: cs = 43; goto _test_eof; 
	_test_eof44: cs = 44; goto _test_eof; 
	_test_eof12: cs = 12; goto _test_eof; 
	_test_eof45: cs = 45; goto _test_eof; 
	_test_eof46: cs = 46; goto _test_eof; 
	_test_eof13: cs = 13; goto _test_eof; 
	_test_eof47: cs = 47; goto _test_eof; 
	_test_eof14: cs = 14; goto _test_eof; 
	_test_eof15: cs = 15; goto _test_eof; 
	_test_eof16: cs = 16; goto _test_eof; 
	_test_eof48: cs = 48; goto _test_eof; 
	_test_eof17: cs = 17; goto _test_eof; 
	_test_eof18: cs = 18; goto _test_eof; 
	_test_eof19: cs = 19; goto _test_eof; 
	_test_eof20: cs = 20; goto _test_eof; 
	_test_eof21: cs = 21; goto _test_eof; 
	_test_eof22: cs = 22; goto _test_eof; 
	_test_eof23: cs = 23; goto _test_eof; 
	_test_eof49: cs = 49; goto _test_eof; 
	_test_eof50: cs = 50; goto _test_eof; 
	_test_eof51: cs = 51; goto _test_eof; 
	_test_eof24: cs = 24; goto _test_eof; 
	_test_eof52: cs = 52; goto _test_eof; 
	_test_eof25: cs = 25; goto _test_eof; 
	_test_eof53: cs = 53; goto _test_eof; 
	_test_eof26: cs = 26; goto _test_eof; 
	_test_eof54: cs = 54; goto _test_eof; 
	_test_eof27: cs = 27; goto _test_eof; 
	_test_eof28: cs = 28; goto _test_eof; 
	_test_eof29: cs = 29; goto _test_eof; 
	_test_eof55: cs = 55; goto _test_eof; 
	_test_eof30: cs = 30; goto _test_eof; 

	_test_eof: {}
	if ( p == eof )
	{
	switch ( cs ) {
	case 34: 
#line 11 "lex/ragel_units.rl"
	{ragel_type = TYPE_BPM;}
	break;
	case 35: 
	case 36: 
#line 12 "lex/ragel_units.rl"
	{ragel_type = TYPE_CENT;}
	break;
	case 37: 
	case 38: 
#line 13 "lex/ragel_units.rl"
	{ragel_type = TYPE_DAY;}
	break;
	case 39: 
	case 40: 
#line 14 "lex/ragel_units.rl"
	{ragel_type = TYPE_DB;}
	break;
	case 41: 
#line 15 "lex/ragel_units.rl"
	{ragel_type = TYPE_DEGREE;}
	break;
	case 33: 
#line 16 "lex/ragel_units.rl"
	{ragel_type = TYPE_HZ;}
	break;
	case 42: 
	case 43: 
#line 17 "lex/ragel_units.rl"
	{ragel_type = TYPE_HOUR;}
	break;
	case 44: 
	case 45: 
#line 18 "lex/ragel_units.rl"
	{ragel_type = TYPE_MIN;}
	break;
	case 46: 
	case 47: 
#line 19 "lex/ragel_units.rl"
	{ragel_type = TYPE_MSEC;}
	break;
	case 49: 
#line 20 "lex/ragel_units.rl"
	{ragel_type = TYPE_RADIAN;}
	break;
	case 51: 
	case 52: 
#line 21 "lex/ragel_units.rl"
	{ragel_type = TYPE_SAMP;}
	break;
	case 50: 
	case 53: 
#line 22 "lex/ragel_units.rl"
	{ragel_type = TYPE_SEC;}
	break;
	case 54: 
	case 55: 
#line 23 "lex/ragel_units.rl"
	{ragel_type = TYPE_SEMITONE;}
	break;
	case 31: 
	case 48: 
#line 26 "lex/ragel_units.rl"
	{ragel_type = TYPE_PERCENT;}
	break;
	case 32: 
#line 29 "lex/ragel_units.rl"
	{ragel_type = TYPE_PHASE;}
	break;
#line 2612 "lex/parser_units.cpp"
	}
	}

	_out: {}
	}

#line 173 "lex/parser_units.rl"

    return cs >= 31;
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


#line 2636 "lex/parser_units.cpp"
static const int parse_angles_start = 1;
static const int parse_angles_first_final = 11;
static const int parse_angles_error = 0;

static const int parse_angles_en_main = 1;


#line 199 "lex/parser_units.rl"


using AngleUnit = std::pair<float, parser::AtomType>;

Either<float> parse_angle_as(const Atom& a, AtomType type)
{
    if (a.isFloat())
        return a.asT<t_float>();
    else if(a.isSymbol()) {
        auto res = parse_angle(a.asT<t_symbol*>()->s_name);
        if (res.isOk()) {
            auto& x = res.value();
            switch (x.second) {
            case TYPE_RADIAN:
                if (type == TYPE_DEGREE)
                    return convert::rad2degree(x.first);
                else
                    return x.first;
            case TYPE_DEGREE:
                if (type == TYPE_RADIAN)
                    return convert::degree2rad(x.first);
                else
                    return x.first;
            case TYPE_INT:
            case TYPE_FLOAT:
                return x.first;
            default:
                return RuntimeError(fmt::format("invalid unit type: {}", (int)x.second));
            }
        } else
            return res.error();
    } else
        return RuntimeError("invalid atom type");
}

Either<AngleUnit> parse_angle(const Atom &a, AtomType def)
{
    if (a.isFloat())
        return AngleUnit(a.asT<t_float>(), def);
    else if(a.isSymbol())
        return parse_angle(a.asT<t_symbol*>()->s_name);
    else
        return RuntimeError("invalid atom type");
}

Either<AngleUnit> parse_angle(const char* str)
{
    if (!str)
        return RuntimeError("NULL string pointer");

    const auto len = strlen(str);
    if (len == 0)
        return RuntimeError("empty string");

    int cs = 0;
    const ragel_char* p = (const ragel_char*)str;
    const ragel_char* pe = p + len;
    const ragel_char* eof = pe;
    DECLARE_RAGEL_COMMON_VARS;
    DECLARE_RAGEL_NUMERIC_VARS;

    
#line 2707 "lex/parser_units.cpp"
	{
	cs = parse_angles_start;
	}

#line 261 "lex/parser_units.rl"
    
#line 2714 "lex/parser_units.cpp"
	{
	if ( p == pe )
		goto _test_eof;
	switch ( cs )
	{
case 1:
	switch( (*p) ) {
		case 43u: goto tr0;
		case 45u: goto tr0;
		case 48u: goto tr2;
	}
	if ( 49u <= (*p) && (*p) <= 57u )
		goto tr3;
	goto st0;
st0:
cs = 0;
	goto _out;
tr0:
#line 21 "lex/ragel_numeric.rl"
	{ ragel_num.sign = ((*p)=='-') ? -1 : 1; }
	goto st2;
st2:
	if ( ++p == pe )
		goto _test_eof2;
case 2:
#line 2740 "lex/parser_units.cpp"
	if ( (*p) == 48u )
		goto tr2;
	if ( 49u <= (*p) && (*p) <= 57u )
		goto tr3;
	goto st0;
tr2:
#line 29 "lex/ragel_numeric.rl"
	{ (ragel_num.vint *= 10) += ((*p)-'0'); }
	goto st11;
st11:
	if ( ++p == pe )
		goto _test_eof11;
case 11:
#line 2754 "lex/parser_units.cpp"
	switch( (*p) ) {
		case 46u: goto tr10;
		case 100u: goto tr11;
		case 112u: goto tr12;
		case 114u: goto tr13;
		case 194u: goto tr14;
		case 207u: goto tr15;
	}
	goto st0;
tr10:
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
	goto st3;
st3:
	if ( ++p == pe )
		goto _test_eof3;
case 3:
#line 2781 "lex/parser_units.cpp"
	if ( 48u <= (*p) && (*p) <= 57u )
		goto tr4;
	goto st0;
tr4:
#line 54 "lex/ragel_numeric.rl"
	{
        (ragel_num.ratio.num *= 10) += ((*p) - '0');
        ragel_num.ratio.den *= 10;
    }
	goto st12;
st12:
	if ( ++p == pe )
		goto _test_eof12;
case 12:
#line 2796 "lex/parser_units.cpp"
	switch( (*p) ) {
		case 100u: goto tr16;
		case 112u: goto tr17;
		case 114u: goto tr18;
		case 194u: goto tr19;
		case 207u: goto tr20;
	}
	if ( 48u <= (*p) && (*p) <= 57u )
		goto tr4;
	goto st0;
tr11:
#line 30 "lex/ragel_numeric.rl"
	{
        ragel_num.vint *= ragel_num.sign;
        ragel_type = TYPE_INT;
        ragel_cat  = CAT_NUMBER;
    }
#line 195 "lex/parser_units.rl"
	{ ragel_num.vdouble = ragel_num.vint;}
	goto st4;
tr16:
#line 58 "lex/ragel_numeric.rl"
	{
        const auto frac = double(ragel_num.ratio.num) / ragel_num.ratio.den;
        ragel_num.vdouble = ragel_num.vint + ragel_num.sign * frac;
        ragel_type = TYPE_FLOAT;
        ragel_cat  = CAT_NUMBER;
    }
	goto st4;
st4:
	if ( ++p == pe )
		goto _test_eof4;
case 4:
#line 2830 "lex/parser_units.cpp"
	if ( (*p) == 101u )
		goto st5;
	goto st0;
st5:
	if ( ++p == pe )
		goto _test_eof5;
case 5:
	if ( (*p) == 103u )
		goto st13;
	goto st0;
st13:
	if ( ++p == pe )
		goto _test_eof13;
case 13:
	goto st0;
tr12:
#line 30 "lex/ragel_numeric.rl"
	{
        ragel_num.vint *= ragel_num.sign;
        ragel_type = TYPE_INT;
        ragel_cat  = CAT_NUMBER;
    }
#line 195 "lex/parser_units.rl"
	{ ragel_num.vdouble = ragel_num.vint;}
	goto st6;
tr17:
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
#line 2869 "lex/parser_units.cpp"
	if ( (*p) == 105u )
		goto st14;
	goto st0;
st14:
	if ( ++p == pe )
		goto _test_eof14;
case 14:
	goto st0;
tr13:
#line 30 "lex/ragel_numeric.rl"
	{
        ragel_num.vint *= ragel_num.sign;
        ragel_type = TYPE_INT;
        ragel_cat  = CAT_NUMBER;
    }
#line 195 "lex/parser_units.rl"
	{ ragel_num.vdouble = ragel_num.vint;}
	goto st7;
tr18:
#line 58 "lex/ragel_numeric.rl"
	{
        const auto frac = double(ragel_num.ratio.num) / ragel_num.ratio.den;
        ragel_num.vdouble = ragel_num.vint + ragel_num.sign * frac;
        ragel_type = TYPE_FLOAT;
        ragel_cat  = CAT_NUMBER;
    }
	goto st7;
st7:
	if ( ++p == pe )
		goto _test_eof7;
case 7:
#line 2901 "lex/parser_units.cpp"
	if ( (*p) == 97u )
		goto st8;
	goto st0;
st8:
	if ( ++p == pe )
		goto _test_eof8;
case 8:
	if ( (*p) == 100u )
		goto st15;
	goto st0;
st15:
	if ( ++p == pe )
		goto _test_eof15;
case 15:
	goto st0;
tr14:
#line 30 "lex/ragel_numeric.rl"
	{
        ragel_num.vint *= ragel_num.sign;
        ragel_type = TYPE_INT;
        ragel_cat  = CAT_NUMBER;
    }
#line 195 "lex/parser_units.rl"
	{ ragel_num.vdouble = ragel_num.vint;}
	goto st9;
tr19:
#line 58 "lex/ragel_numeric.rl"
	{
        const auto frac = double(ragel_num.ratio.num) / ragel_num.ratio.den;
        ragel_num.vdouble = ragel_num.vint + ragel_num.sign * frac;
        ragel_type = TYPE_FLOAT;
        ragel_cat  = CAT_NUMBER;
    }
	goto st9;
st9:
	if ( ++p == pe )
		goto _test_eof9;
case 9:
#line 2940 "lex/parser_units.cpp"
	if ( (*p) == 176u )
		goto st13;
	goto st0;
tr15:
#line 30 "lex/ragel_numeric.rl"
	{
        ragel_num.vint *= ragel_num.sign;
        ragel_type = TYPE_INT;
        ragel_cat  = CAT_NUMBER;
    }
#line 195 "lex/parser_units.rl"
	{ ragel_num.vdouble = ragel_num.vint;}
	goto st10;
tr20:
#line 58 "lex/ragel_numeric.rl"
	{
        const auto frac = double(ragel_num.ratio.num) / ragel_num.ratio.den;
        ragel_num.vdouble = ragel_num.vint + ragel_num.sign * frac;
        ragel_type = TYPE_FLOAT;
        ragel_cat  = CAT_NUMBER;
    }
	goto st10;
st10:
	if ( ++p == pe )
		goto _test_eof10;
case 10:
#line 2967 "lex/parser_units.cpp"
	if ( (*p) == 128u )
		goto st14;
	goto st0;
tr3:
#line 29 "lex/ragel_numeric.rl"
	{ (ragel_num.vint *= 10) += ((*p)-'0'); }
	goto st16;
st16:
	if ( ++p == pe )
		goto _test_eof16;
case 16:
#line 2979 "lex/parser_units.cpp"
	switch( (*p) ) {
		case 46u: goto tr10;
		case 100u: goto tr11;
		case 112u: goto tr12;
		case 114u: goto tr13;
		case 194u: goto tr14;
		case 207u: goto tr15;
	}
	if ( 48u <= (*p) && (*p) <= 57u )
		goto tr3;
	goto st0;
	}
	_test_eof2: cs = 2; goto _test_eof; 
	_test_eof11: cs = 11; goto _test_eof; 
	_test_eof3: cs = 3; goto _test_eof; 
	_test_eof12: cs = 12; goto _test_eof; 
	_test_eof4: cs = 4; goto _test_eof; 
	_test_eof5: cs = 5; goto _test_eof; 
	_test_eof13: cs = 13; goto _test_eof; 
	_test_eof6: cs = 6; goto _test_eof; 
	_test_eof14: cs = 14; goto _test_eof; 
	_test_eof7: cs = 7; goto _test_eof; 
	_test_eof8: cs = 8; goto _test_eof; 
	_test_eof15: cs = 15; goto _test_eof; 
	_test_eof9: cs = 9; goto _test_eof; 
	_test_eof10: cs = 10; goto _test_eof; 
	_test_eof16: cs = 16; goto _test_eof; 

	_test_eof: {}
	if ( p == eof )
	{
	switch ( cs ) {
	case 12: 
#line 58 "lex/ragel_numeric.rl"
	{
        const auto frac = double(ragel_num.ratio.num) / ragel_num.ratio.den;
        ragel_num.vdouble = ragel_num.vint + ragel_num.sign * frac;
        ragel_type = TYPE_FLOAT;
        ragel_cat  = CAT_NUMBER;
    }
	break;
	case 13: 
#line 15 "lex/ragel_units.rl"
	{ragel_type = TYPE_DEGREE;}
	break;
	case 15: 
#line 20 "lex/ragel_units.rl"
	{ragel_type = TYPE_RADIAN;}
	break;
	case 14: 
#line 193 "lex/parser_units.rl"
	{ ragel_num.vdouble *= std::acos(-1); ragel_type = TYPE_RADIAN; }
	break;
	case 11: 
	case 16: 
#line 30 "lex/ragel_numeric.rl"
	{
        ragel_num.vint *= ragel_num.sign;
        ragel_type = TYPE_INT;
        ragel_cat  = CAT_NUMBER;
    }
#line 195 "lex/parser_units.rl"
	{ ragel_num.vdouble = ragel_num.vint;}
	break;
#line 3044 "lex/parser_units.cpp"
	}
	}

	_out: {}
	}

#line 262 "lex/parser_units.rl"

    if (cs >= 11) {
        if (ragel_type == TYPE_INT)
            return AngleUnit(ragel_num.getInteger(), TYPE_FLOAT);
        else if (ragel_type == TYPE_FLOAT)
            return AngleUnit(ragel_num.getFloat(), ragel_type);
        else
            return AngleUnit(ragel_num.getFloat(), ragel_type);
    } else
        return RuntimeError(fmt::format("can't parse string: '{}'", str));
}

}
}


