
#line 1 "lex/parser_units.rl"
# include "parser_units.h"

# include <cstdint>
# include <cstring>
# include <cstdio>

namespace ceammc {
namespace parser {


#line 14 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
static const int units_full_start = 1;
static const int units_full_first_final = 48;
static const int units_full_error = 0;

static const int units_full_en_main = 1;


#line 16 "lex/parser_units.rl"


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
    int cat_ = 0;
    AtomType type_ = TYPE_UNKNOWN;
    PositionType pos_ = POSITION_ABS;
    NumericRagelData num_;
    fsm::BpmData bpm;
    fsm::SmpteData smpte;

    reset();

    
#line 78 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
	{
	cs = units_full_start;
	}

#line 71 "lex/parser_units.rl"
    
#line 85 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
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
#line 75 "lex/ragel_units.rl"
	{type_ = TYPE_UNKNOWN; cat_ = CAT_UNKNOWN;}
#line 66 "lex/ragel_units.rl"
	{pos_ = POSITION_ABS;}
	goto st2;
st2:
	if ( ++p == pe )
		goto _test_eof2;
case 2:
#line 119 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
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
#line 75 "lex/ragel_units.rl"
	{type_ = TYPE_UNKNOWN; cat_ = CAT_UNKNOWN;}
#line 66 "lex/ragel_units.rl"
	{pos_ = POSITION_ABS;}
#line 4 "lex/ragel_numeric.rl"
	{ num_.sign = ((*p)=='-') ? -1 : 1; }
#line 57 "lex/ragel_units.rl"
	{smpte.np = 0;}
#line 52 "lex/ragel_units.rl"
	{smpte.sign = ((*p)=='-')?-1:1;}
	goto st3;
tr8:
#line 60 "lex/ragel_units.rl"
	{pos_ = POSITION_END;}
#line 4 "lex/ragel_numeric.rl"
	{ num_.sign = ((*p)=='-') ? -1 : 1; }
#line 57 "lex/ragel_units.rl"
	{smpte.np = 0;}
#line 52 "lex/ragel_units.rl"
	{smpte.sign = ((*p)=='-')?-1:1;}
	goto st3;
tr59:
#line 61 "lex/ragel_units.rl"
	{pos_ = POSITION_CURRENT;}
#line 4 "lex/ragel_numeric.rl"
	{ num_.sign = ((*p)=='-') ? -1 : 1; }
#line 57 "lex/ragel_units.rl"
	{smpte.np = 0;}
#line 52 "lex/ragel_units.rl"
	{smpte.sign = ((*p)=='-')?-1:1;}
	goto st3;
st3:
	if ( ++p == pe )
		goto _test_eof3;
case 3:
#line 167 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
	if ( (*p) == 48 )
		goto tr12;
	if ( (*p) > 53 ) {
		if ( 54 <= (*p) && (*p) <= 57 )
			goto tr14;
	} else if ( (*p) >= 49 )
		goto tr13;
	goto st0;
tr9:
#line 60 "lex/ragel_units.rl"
	{pos_ = POSITION_END;}
#line 9 "lex/ragel_numeric.rl"
	{ (num_.ival *= 10) += ((*p)-'0'); }
#line 15 "lex/ragel_numeric.rl"
	{ (num_.inum *= 10) += ((*p)-'0'); }
#line 57 "lex/ragel_units.rl"
	{smpte.np = 0;}
#line 45 "lex/ragel_units.rl"
	{smpte.hour = 0; smpte.np++;}
#line 45 "lex/ragel_units.rl"
	{(smpte.hour *= 10) += ((*p) - '0');}
	goto st48;
tr12:
#line 9 "lex/ragel_numeric.rl"
	{ (num_.ival *= 10) += ((*p)-'0'); }
#line 15 "lex/ragel_numeric.rl"
	{ (num_.inum *= 10) += ((*p)-'0'); }
#line 45 "lex/ragel_units.rl"
	{smpte.hour = 0; smpte.np++;}
#line 45 "lex/ragel_units.rl"
	{(smpte.hour *= 10) += ((*p) - '0');}
	goto st48;
tr60:
#line 61 "lex/ragel_units.rl"
	{pos_ = POSITION_CURRENT;}
#line 9 "lex/ragel_numeric.rl"
	{ (num_.ival *= 10) += ((*p)-'0'); }
#line 15 "lex/ragel_numeric.rl"
	{ (num_.inum *= 10) += ((*p)-'0'); }
#line 57 "lex/ragel_units.rl"
	{smpte.np = 0;}
#line 45 "lex/ragel_units.rl"
	{smpte.hour = 0; smpte.np++;}
#line 45 "lex/ragel_units.rl"
	{(smpte.hour *= 10) += ((*p) - '0');}
	goto st48;
st48:
	if ( ++p == pe )
		goto _test_eof48;
case 48:
#line 218 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
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
#line 10 "lex/ragel_numeric.rl"
	{ num_.ival *= num_.sign; cat_ = CAT_NUMBER; type_ = TYPE_INT; }
#line 28 "lex/ragel_units.rl"
	{num_.fval = num_.ival;}
#line 60 "lex/ragel_numeric.rl"
	{num_.fval = num_.ival;}
	goto st49;
tr79:
#line 19 "lex/ragel_numeric.rl"
	{
        cat_ = CAT_NUMBER;
        type_ = TYPE_FLOAT;
        num_.fval = num_.ival + num_.sign * (double(num_.inum)/num_.iden);
    }
	goto st49;
st49:
	if ( ++p == pe )
		goto _test_eof49;
case 49:
#line 257 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
	goto st0;
tr66:
#line 10 "lex/ragel_numeric.rl"
	{ num_.ival *= num_.sign; cat_ = CAT_NUMBER; type_ = TYPE_INT; }
#line 28 "lex/ragel_units.rl"
	{num_.fval = num_.ival;}
#line 65 "lex/ragel_numeric.rl"
	{num_.fval = num_.ival;}
	goto st50;
tr80:
#line 19 "lex/ragel_numeric.rl"
	{
        cat_ = CAT_NUMBER;
        type_ = TYPE_FLOAT;
        num_.fval = num_.ival + num_.sign * (double(num_.inum)/num_.iden);
    }
	goto st50;
st50:
	if ( ++p == pe )
		goto _test_eof50;
case 50:
#line 279 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
	goto st0;
tr67:
#line 10 "lex/ragel_numeric.rl"
	{ num_.ival *= num_.sign; cat_ = CAT_NUMBER; type_ = TYPE_INT; }
#line 54 "lex/ragel_numeric.rl"
	{num_.inum = 0; num_.iden = 1;}
	goto st4;
st4:
	if ( ++p == pe )
		goto _test_eof4;
case 4:
#line 291 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr15;
	goto st0;
tr15:
#line 46 "lex/ragel_numeric.rl"
	{(num_.inum *= 10) += ((*p) - '0'); num_.iden *= 10;}
	goto st51;
st51:
	if ( ++p == pe )
		goto _test_eof51;
case 51:
#line 303 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
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
#line 10 "lex/ragel_numeric.rl"
	{ num_.ival *= num_.sign; cat_ = CAT_NUMBER; type_ = TYPE_INT; }
#line 28 "lex/ragel_units.rl"
	{num_.fval = num_.ival;}
	goto st5;
tr81:
#line 19 "lex/ragel_numeric.rl"
	{
        cat_ = CAT_NUMBER;
        type_ = TYPE_FLOAT;
        num_.fval = num_.ival + num_.sign * (double(num_.inum)/num_.iden);
    }
	goto st5;
st5:
	if ( ++p == pe )
		goto _test_eof5;
case 5:
#line 338 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
	if ( (*p) == 122 )
		goto st52;
	goto st0;
st52:
	if ( ++p == pe )
		goto _test_eof52;
case 52:
	goto st0;
tr71:
#line 10 "lex/ragel_numeric.rl"
	{ num_.ival *= num_.sign; cat_ = CAT_NUMBER; type_ = TYPE_INT; }
#line 28 "lex/ragel_units.rl"
	{num_.fval = num_.ival;}
#line 60 "lex/ragel_numeric.rl"
	{num_.fval = num_.ival;}
#line 65 "lex/ragel_numeric.rl"
	{num_.fval = num_.ival;}
	goto st53;
tr82:
#line 19 "lex/ragel_numeric.rl"
	{
        cat_ = CAT_NUMBER;
        type_ = TYPE_FLOAT;
        num_.fval = num_.ival + num_.sign * (double(num_.inum)/num_.iden);
    }
	goto st53;
st53:
	if ( ++p == pe )
		goto _test_eof53;
case 53:
#line 369 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
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
		case 115: goto st72;
	}
	goto st0;
st54:
	if ( ++p == pe )
		goto _test_eof54;
case 54:
	goto st0;
tr72:
#line 10 "lex/ragel_numeric.rl"
	{ num_.ival *= num_.sign; cat_ = CAT_NUMBER; type_ = TYPE_INT; }
#line 28 "lex/ragel_units.rl"
	{num_.fval = num_.ival;}
	goto st55;
tr83:
#line 19 "lex/ragel_numeric.rl"
	{
        cat_ = CAT_NUMBER;
        type_ = TYPE_FLOAT;
        num_.fval = num_.ival + num_.sign * (double(num_.inum)/num_.iden);
    }
	goto st55;
st55:
	if ( ++p == pe )
		goto _test_eof55;
case 55:
#line 406 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
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
#line 10 "lex/ragel_numeric.rl"
	{ num_.ival *= num_.sign; cat_ = CAT_NUMBER; type_ = TYPE_INT; }
#line 28 "lex/ragel_units.rl"
	{num_.fval = num_.ival;}
	goto st57;
tr84:
#line 19 "lex/ragel_numeric.rl"
	{
        cat_ = CAT_NUMBER;
        type_ = TYPE_FLOAT;
        num_.fval = num_.ival + num_.sign * (double(num_.inum)/num_.iden);
    }
	goto st57;
st57:
	if ( ++p == pe )
		goto _test_eof57;
case 57:
#line 447 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
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
#line 10 "lex/ragel_numeric.rl"
	{ num_.ival *= num_.sign; cat_ = CAT_NUMBER; type_ = TYPE_INT; }
#line 28 "lex/ragel_units.rl"
	{num_.fval = num_.ival;}
	goto st62;
tr85:
#line 19 "lex/ragel_numeric.rl"
	{
        cat_ = CAT_NUMBER;
        type_ = TYPE_FLOAT;
        num_.fval = num_.ival + num_.sign * (double(num_.inum)/num_.iden);
    }
	goto st62;
st62:
	if ( ++p == pe )
		goto _test_eof62;
case 62:
#line 515 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
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
#line 10 "lex/ragel_numeric.rl"
	{ num_.ival *= num_.sign; cat_ = CAT_NUMBER; type_ = TYPE_INT; }
#line 28 "lex/ragel_units.rl"
	{num_.fval = num_.ival;}
	goto st64;
tr86:
#line 19 "lex/ragel_numeric.rl"
	{
        cat_ = CAT_NUMBER;
        type_ = TYPE_FLOAT;
        num_.fval = num_.ival + num_.sign * (double(num_.inum)/num_.iden);
    }
	goto st64;
st64:
	if ( ++p == pe )
		goto _test_eof64;
case 64:
#line 558 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
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
#line 10 "lex/ragel_numeric.rl"
	{ num_.ival *= num_.sign; cat_ = CAT_NUMBER; type_ = TYPE_INT; }
#line 28 "lex/ragel_units.rl"
	{num_.fval = num_.ival;}
#line 65 "lex/ragel_numeric.rl"
	{num_.fval = num_.ival;}
	goto st15;
tr87:
#line 19 "lex/ragel_numeric.rl"
	{
        cat_ = CAT_NUMBER;
        type_ = TYPE_FLOAT;
        num_.fval = num_.ival + num_.sign * (double(num_.inum)/num_.iden);
    }
	goto st15;
st15:
	if ( ++p == pe )
		goto _test_eof15;
case 15:
#line 615 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
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
		goto st70;
	goto st0;
st70:
	if ( ++p == pe )
		goto _test_eof70;
case 70:
	goto st0;
tr77:
#line 10 "lex/ragel_numeric.rl"
	{ num_.ival *= num_.sign; cat_ = CAT_NUMBER; type_ = TYPE_INT; }
#line 28 "lex/ragel_units.rl"
	{num_.fval = num_.ival;}
	goto st23;
tr88:
#line 19 "lex/ragel_numeric.rl"
	{
        cat_ = CAT_NUMBER;
        type_ = TYPE_FLOAT;
        num_.fval = num_.ival + num_.sign * (double(num_.inum)/num_.iden);
    }
	goto st23;
st23:
	if ( ++p == pe )
		goto _test_eof23;
case 23:
#line 705 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
	if ( (*p) == 97 )
		goto st24;
	goto st0;
st24:
	if ( ++p == pe )
		goto _test_eof24;
case 24:
	if ( (*p) == 100 )
		goto st71;
	goto st0;
st71:
	if ( ++p == pe )
		goto _test_eof71;
case 71:
	goto st0;
tr78:
#line 10 "lex/ragel_numeric.rl"
	{ num_.ival *= num_.sign; cat_ = CAT_NUMBER; type_ = TYPE_INT; }
#line 28 "lex/ragel_units.rl"
	{num_.fval = num_.ival;}
	goto st72;
tr89:
#line 19 "lex/ragel_numeric.rl"
	{
        cat_ = CAT_NUMBER;
        type_ = TYPE_FLOAT;
        num_.fval = num_.ival + num_.sign * (double(num_.inum)/num_.iden);
    }
	goto st72;
st72:
	if ( ++p == pe )
		goto _test_eof72;
case 72:
#line 739 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
	switch( (*p) ) {
		case 97: goto st73;
		case 101: goto st26;
	}
	goto st0;
st73:
	if ( ++p == pe )
		goto _test_eof73;
case 73:
	if ( (*p) == 109 )
		goto st25;
	goto st0;
st25:
	if ( ++p == pe )
		goto _test_eof25;
case 25:
	if ( (*p) == 112 )
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
		case 99: goto st75;
		case 109: goto st27;
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
	if ( (*p) == 105 )
		goto st76;
	goto st0;
st76:
	if ( ++p == pe )
		goto _test_eof76;
case 76:
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
		goto st77;
	goto st0;
st77:
	if ( ++p == pe )
		goto _test_eof77;
case 77:
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
#line 16 "lex/ragel_numeric.rl"
	{ (num_.iden *= 10) += ((*p)-'0'); }
	goto st78;
st78:
	if ( ++p == pe )
		goto _test_eof78;
case 78:
#line 835 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
	goto st0;
tr45:
#line 16 "lex/ragel_numeric.rl"
	{ (num_.iden *= 10) += ((*p)-'0'); }
	goto st79;
st79:
	if ( ++p == pe )
		goto _test_eof79;
case 79:
#line 845 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr45;
	goto st0;
tr69:
#line 45 "lex/ragel_units.rl"
	{(smpte.hour *= 10) += ((*p) - '0');}
	goto st32;
st32:
	if ( ++p == pe )
		goto _test_eof32;
case 32:
#line 857 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
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
#line 46 "lex/ragel_units.rl"
	{smpte.min = 0; smpte.np++;}
#line 46 "lex/ragel_units.rl"
	{(smpte.min *= 10) += ((*p) - '0');}
	goto st34;
st34:
	if ( ++p == pe )
		goto _test_eof34;
case 34:
#line 878 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr48;
	goto st0;
tr48:
#line 46 "lex/ragel_units.rl"
	{(smpte.min *= 10) += ((*p) - '0');}
	goto st80;
st80:
	if ( ++p == pe )
		goto _test_eof80;
case 80:
#line 890 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
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
#line 48 "lex/ragel_units.rl"
	{smpte.frame = 0;}
#line 48 "lex/ragel_units.rl"
	{(smpte.frame *= 10) += ((*p) - '0');}
	goto st81;
st81:
	if ( ++p == pe )
		goto _test_eof81;
case 81:
#line 913 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr116;
	goto st0;
tr116:
#line 48 "lex/ragel_units.rl"
	{(smpte.frame *= 10) += ((*p) - '0');}
	goto st82;
st82:
	if ( ++p == pe )
		goto _test_eof82;
case 82:
#line 925 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
	goto st0;
st36:
	if ( ++p == pe )
		goto _test_eof36;
case 36:
	if ( 48 <= (*p) && (*p) <= 53 )
		goto tr50;
	goto st0;
tr50:
#line 47 "lex/ragel_units.rl"
	{smpte.sec = 0; smpte.np++;}
#line 47 "lex/ragel_units.rl"
	{(smpte.sec *= 10) += ((*p) - '0');}
	goto st37;
st37:
	if ( ++p == pe )
		goto _test_eof37;
case 37:
#line 944 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr51;
	goto st0;
tr51:
#line 47 "lex/ragel_units.rl"
	{(smpte.sec *= 10) += ((*p) - '0');}
	goto st83;
st83:
	if ( ++p == pe )
		goto _test_eof83;
case 83:
#line 956 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
	if ( (*p) == 46 )
		goto st35;
	goto st0;
tr10:
#line 60 "lex/ragel_units.rl"
	{pos_ = POSITION_END;}
#line 9 "lex/ragel_numeric.rl"
	{ (num_.ival *= 10) += ((*p)-'0'); }
#line 15 "lex/ragel_numeric.rl"
	{ (num_.inum *= 10) += ((*p)-'0'); }
#line 57 "lex/ragel_units.rl"
	{smpte.np = 0;}
#line 45 "lex/ragel_units.rl"
	{smpte.hour = 0; smpte.np++;}
#line 45 "lex/ragel_units.rl"
	{(smpte.hour *= 10) += ((*p) - '0');}
	goto st84;
tr13:
#line 9 "lex/ragel_numeric.rl"
	{ (num_.ival *= 10) += ((*p)-'0'); }
#line 15 "lex/ragel_numeric.rl"
	{ (num_.inum *= 10) += ((*p)-'0'); }
#line 45 "lex/ragel_units.rl"
	{smpte.hour = 0; smpte.np++;}
#line 45 "lex/ragel_units.rl"
	{(smpte.hour *= 10) += ((*p) - '0');}
	goto st84;
tr61:
#line 61 "lex/ragel_units.rl"
	{pos_ = POSITION_CURRENT;}
#line 9 "lex/ragel_numeric.rl"
	{ (num_.ival *= 10) += ((*p)-'0'); }
#line 15 "lex/ragel_numeric.rl"
	{ (num_.inum *= 10) += ((*p)-'0'); }
#line 57 "lex/ragel_units.rl"
	{smpte.np = 0;}
#line 45 "lex/ragel_units.rl"
	{smpte.hour = 0; smpte.np++;}
#line 45 "lex/ragel_units.rl"
	{(smpte.hour *= 10) += ((*p) - '0');}
	goto st84;
st84:
	if ( ++p == pe )
		goto _test_eof84;
case 84:
#line 1002 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
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
#line 9 "lex/ragel_numeric.rl"
	{ (num_.ival *= 10) += ((*p)-'0'); }
#line 15 "lex/ragel_numeric.rl"
	{ (num_.inum *= 10) += ((*p)-'0'); }
#line 45 "lex/ragel_units.rl"
	{(smpte.hour *= 10) += ((*p) - '0');}
	goto st85;
st85:
	if ( ++p == pe )
		goto _test_eof85;
case 85:
#line 1033 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
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
#line 60 "lex/ragel_units.rl"
	{pos_ = POSITION_END;}
#line 9 "lex/ragel_numeric.rl"
	{ (num_.ival *= 10) += ((*p)-'0'); }
#line 15 "lex/ragel_numeric.rl"
	{ (num_.inum *= 10) += ((*p)-'0'); }
	goto st86;
tr14:
#line 9 "lex/ragel_numeric.rl"
	{ (num_.ival *= 10) += ((*p)-'0'); }
#line 15 "lex/ragel_numeric.rl"
	{ (num_.inum *= 10) += ((*p)-'0'); }
	goto st86;
tr62:
#line 61 "lex/ragel_units.rl"
	{pos_ = POSITION_CURRENT;}
#line 9 "lex/ragel_numeric.rl"
	{ (num_.ival *= 10) += ((*p)-'0'); }
#line 15 "lex/ragel_numeric.rl"
	{ (num_.inum *= 10) += ((*p)-'0'); }
	goto st86;
st86:
	if ( ++p == pe )
		goto _test_eof86;
case 86:
#line 1079 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
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
#line 75 "lex/ragel_units.rl"
	{type_ = TYPE_UNKNOWN; cat_ = CAT_UNKNOWN;}
#line 66 "lex/ragel_units.rl"
	{pos_ = POSITION_ABS;}
#line 9 "lex/ragel_numeric.rl"
	{ (num_.ival *= 10) += ((*p)-'0'); }
#line 15 "lex/ragel_numeric.rl"
	{ (num_.inum *= 10) += ((*p)-'0'); }
#line 57 "lex/ragel_units.rl"
	{smpte.np = 0;}
#line 45 "lex/ragel_units.rl"
	{smpte.hour = 0; smpte.np++;}
#line 45 "lex/ragel_units.rl"
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
#line 1122 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
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
#line 10 "lex/ragel_numeric.rl"
	{ num_.ival *= num_.sign; cat_ = CAT_NUMBER; type_ = TYPE_INT; }
#line 54 "lex/ragel_numeric.rl"
	{num_.inum = 0; num_.iden = 1;}
	goto st38;
st38:
	if ( ++p == pe )
		goto _test_eof38;
case 38:
#line 1153 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr52;
	goto st0;
tr52:
#line 46 "lex/ragel_numeric.rl"
	{(num_.inum *= 10) += ((*p) - '0'); num_.iden *= 10;}
#line 7 "lex/ragel_music.rl"
	{ (bpm.fnum *= 10) += ((*p) - '0'); bpm.fden *= 10; }
	goto st88;
st88:
	if ( ++p == pe )
		goto _test_eof88;
case 88:
#line 1167 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
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
#line 10 "lex/ragel_numeric.rl"
	{ num_.ival *= num_.sign; cat_ = CAT_NUMBER; type_ = TYPE_INT; }
#line 28 "lex/ragel_units.rl"
	{num_.fval = num_.ival;}
#line 60 "lex/ragel_numeric.rl"
	{num_.fval = num_.ival;}
#line 65 "lex/ragel_numeric.rl"
	{num_.fval = num_.ival;}
	goto st89;
tr122:
#line 19 "lex/ragel_numeric.rl"
	{
        cat_ = CAT_NUMBER;
        type_ = TYPE_FLOAT;
        num_.fval = num_.ival + num_.sign * (double(num_.inum)/num_.iden);
    }
	goto st89;
st89:
	if ( ++p == pe )
		goto _test_eof89;
case 89:
#line 1208 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
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
		case 115: goto st72;
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
#line 1231 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
	if ( (*p) == 112 )
		goto st40;
	goto st0;
st40:
	if ( ++p == pe )
		goto _test_eof40;
case 40:
	if ( (*p) == 109 )
		goto st90;
	goto st0;
st90:
	if ( ++p == pe )
		goto _test_eof90;
case 90:
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
	goto st91;
tr125:
#line 12 "lex/ragel_music.rl"
	{ (bpm.dur_num *= 10) += ((*p) - '0'); }
#line 14 "lex/ragel_music.rl"
	{ (bpm.dur_den *= 10) += ((*p) - '0'); }
	goto st91;
st91:
	if ( ++p == pe )
		goto _test_eof91;
case 91:
#line 1274 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
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
	goto st92;
tr129:
#line 8 "lex/ragel_music.rl"
	{ bpm.dur_num *= 3; bpm.dur_den *= 2; }
	goto st92;
st92:
	if ( ++p == pe )
		goto _test_eof92;
case 92:
#line 1298 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
	switch( (*p) ) {
		case 95: goto st93;
		case 98: goto st39;
	}
	goto st0;
tr126:
#line 24 "lex/ragel_music.rl"
	{bpm.dur_num = 1;}
	goto st93;
st93:
	if ( ++p == pe )
		goto _test_eof93;
case 93:
#line 1312 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
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
	goto st94;
tr130:
#line 14 "lex/ragel_music.rl"
	{ (bpm.dur_den *= 10) += ((*p) - '0'); }
	goto st94;
st94:
	if ( ++p == pe )
		goto _test_eof94;
case 94:
#line 1337 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
	switch( (*p) ) {
		case 46: goto tr129;
		case 95: goto st93;
		case 98: goto st39;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr130;
	goto st0;
tr4:
#line 75 "lex/ragel_units.rl"
	{type_ = TYPE_UNKNOWN; cat_ = CAT_UNKNOWN;}
#line 66 "lex/ragel_units.rl"
	{pos_ = POSITION_ABS;}
#line 9 "lex/ragel_numeric.rl"
	{ (num_.ival *= 10) += ((*p)-'0'); }
#line 15 "lex/ragel_numeric.rl"
	{ (num_.inum *= 10) += ((*p)-'0'); }
#line 57 "lex/ragel_units.rl"
	{smpte.np = 0;}
#line 45 "lex/ragel_units.rl"
	{smpte.hour = 0; smpte.np++;}
#line 45 "lex/ragel_units.rl"
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
#line 1370 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
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
#line 9 "lex/ragel_numeric.rl"
	{ (num_.ival *= 10) += ((*p)-'0'); }
#line 15 "lex/ragel_numeric.rl"
	{ (num_.inum *= 10) += ((*p)-'0'); }
#line 45 "lex/ragel_units.rl"
	{(smpte.hour *= 10) += ((*p) - '0');}
#line 6 "lex/ragel_music.rl"
	{ (bpm.ival *= 10) += ((*p) - '0'); }
	goto st96;
st96:
	if ( ++p == pe )
		goto _test_eof96;
case 96:
#line 1405 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
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
#line 75 "lex/ragel_units.rl"
	{type_ = TYPE_UNKNOWN; cat_ = CAT_UNKNOWN;}
#line 66 "lex/ragel_units.rl"
	{pos_ = POSITION_ABS;}
#line 9 "lex/ragel_numeric.rl"
	{ (num_.ival *= 10) += ((*p)-'0'); }
#line 15 "lex/ragel_numeric.rl"
	{ (num_.inum *= 10) += ((*p)-'0'); }
#line 9 "lex/ragel_music.rl"
	{ bpm.dur_num = 1; bpm.dur_den = 4; }
#line 6 "lex/ragel_music.rl"
	{ (bpm.ival *= 10) += ((*p) - '0'); }
	goto st97;
tr132:
#line 9 "lex/ragel_numeric.rl"
	{ (num_.ival *= 10) += ((*p)-'0'); }
#line 15 "lex/ragel_numeric.rl"
	{ (num_.inum *= 10) += ((*p)-'0'); }
#line 6 "lex/ragel_music.rl"
	{ (bpm.ival *= 10) += ((*p) - '0'); }
	goto st97;
st97:
	if ( ++p == pe )
		goto _test_eof97;
case 97:
#line 1453 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
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
#line 75 "lex/ragel_units.rl"
	{type_ = TYPE_UNKNOWN; cat_ = CAT_UNKNOWN;}
#line 66 "lex/ragel_units.rl"
	{pos_ = POSITION_ABS;}
	goto st43;
st43:
	if ( ++p == pe )
		goto _test_eof43;
case 43:
#line 1484 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
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
#line 75 "lex/ragel_units.rl"
	{type_ = TYPE_UNKNOWN; cat_ = CAT_UNKNOWN;}
#line 66 "lex/ragel_units.rl"
	{pos_ = POSITION_ABS;}
	goto st46;
st46:
	if ( ++p == pe )
		goto _test_eof46;
case 46:
#line 1520 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
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
	_test_eof70: cs = 70; goto _test_eof; 
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
	_test_eof78: cs = 78; goto _test_eof; 
	_test_eof79: cs = 79; goto _test_eof; 
	_test_eof32: cs = 32; goto _test_eof; 
	_test_eof33: cs = 33; goto _test_eof; 
	_test_eof34: cs = 34; goto _test_eof; 
	_test_eof80: cs = 80; goto _test_eof; 
	_test_eof35: cs = 35; goto _test_eof; 
	_test_eof81: cs = 81; goto _test_eof; 
	_test_eof82: cs = 82; goto _test_eof; 
	_test_eof36: cs = 36; goto _test_eof; 
	_test_eof37: cs = 37; goto _test_eof; 
	_test_eof83: cs = 83; goto _test_eof; 
	_test_eof84: cs = 84; goto _test_eof; 
	_test_eof85: cs = 85; goto _test_eof; 
	_test_eof86: cs = 86; goto _test_eof; 
	_test_eof87: cs = 87; goto _test_eof; 
	_test_eof38: cs = 38; goto _test_eof; 
	_test_eof88: cs = 88; goto _test_eof; 
	_test_eof89: cs = 89; goto _test_eof; 
	_test_eof39: cs = 39; goto _test_eof; 
	_test_eof40: cs = 40; goto _test_eof; 
	_test_eof90: cs = 90; goto _test_eof; 
	_test_eof41: cs = 41; goto _test_eof; 
	_test_eof91: cs = 91; goto _test_eof; 
	_test_eof92: cs = 92; goto _test_eof; 
	_test_eof93: cs = 93; goto _test_eof; 
	_test_eof42: cs = 42; goto _test_eof; 
	_test_eof94: cs = 94; goto _test_eof; 
	_test_eof95: cs = 95; goto _test_eof; 
	_test_eof96: cs = 96; goto _test_eof; 
	_test_eof97: cs = 97; goto _test_eof; 
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
#line 76 "lex/ragel_units.rl"
	{cat_ = CAT_UNIT;}
	break;
	case 51: 
#line 19 "lex/ragel_numeric.rl"
	{
        cat_ = CAT_NUMBER;
        type_ = TYPE_FLOAT;
        num_.fval = num_.ival + num_.sign * (double(num_.inum)/num_.iden);
    }
#line 76 "lex/ragel_units.rl"
	{cat_ = CAT_UNIT;}
	break;
	case 89: 
	case 90: 
	case 92: 
	case 93: 
	case 94: 
#line 10 "lex/ragel_music.rl"
	{ cat_ = CAT_UNIT; type_ = TYPE_BPM; }
#line 76 "lex/ragel_units.rl"
	{cat_ = CAT_UNIT;}
	break;
	case 52: 
#line 9 "lex/ragel_units.rl"
	{type_ = TYPE_HZ;}
#line 76 "lex/ragel_units.rl"
	{cat_ = CAT_UNIT;}
	break;
	case 73: 
	case 74: 
#line 10 "lex/ragel_units.rl"
	{type_ = TYPE_SAMP;}
#line 76 "lex/ragel_units.rl"
	{cat_ = CAT_UNIT;}
	break;
	case 71: 
#line 11 "lex/ragel_units.rl"
	{type_ = TYPE_RADIAN;}
#line 76 "lex/ragel_units.rl"
	{cat_ = CAT_UNIT;}
	break;
	case 61: 
#line 12 "lex/ragel_units.rl"
	{type_ = TYPE_DEGREE;}
#line 76 "lex/ragel_units.rl"
	{cat_ = CAT_UNIT;}
	break;
	case 59: 
	case 60: 
#line 13 "lex/ragel_units.rl"
	{type_ = TYPE_DB;}
#line 76 "lex/ragel_units.rl"
	{cat_ = CAT_UNIT;}
	break;
	case 57: 
	case 58: 
#line 14 "lex/ragel_units.rl"
	{type_ = TYPE_DAY;}
#line 76 "lex/ragel_units.rl"
	{cat_ = CAT_UNIT;}
	break;
	case 62: 
	case 63: 
#line 15 "lex/ragel_units.rl"
	{type_ = TYPE_HOUR;}
#line 76 "lex/ragel_units.rl"
	{cat_ = CAT_UNIT;}
	break;
	case 64: 
	case 65: 
#line 16 "lex/ragel_units.rl"
	{type_ = TYPE_MIN;}
#line 76 "lex/ragel_units.rl"
	{cat_ = CAT_UNIT;}
	break;
	case 72: 
	case 75: 
#line 17 "lex/ragel_units.rl"
	{type_ = TYPE_SEC;}
#line 76 "lex/ragel_units.rl"
	{cat_ = CAT_UNIT;}
	break;
	case 66: 
	case 67: 
#line 18 "lex/ragel_units.rl"
	{type_ = TYPE_MSEC;}
#line 76 "lex/ragel_units.rl"
	{cat_ = CAT_UNIT;}
	break;
	case 55: 
	case 56: 
#line 19 "lex/ragel_units.rl"
	{type_ = TYPE_CENT;}
#line 76 "lex/ragel_units.rl"
	{cat_ = CAT_UNIT;}
	break;
	case 76: 
	case 77: 
#line 20 "lex/ragel_units.rl"
	{type_ = TYPE_SEMITONE;}
#line 76 "lex/ragel_units.rl"
	{cat_ = CAT_UNIT;}
	break;
	case 68: 
	case 69: 
#line 22 "lex/ragel_units.rl"
	{type_ = TYPE_PERCENT;}
#line 76 "lex/ragel_units.rl"
	{cat_ = CAT_UNIT;}
	break;
	case 54: 
#line 23 "lex/ragel_units.rl"
	{type_ = TYPE_PHASE;}
#line 76 "lex/ragel_units.rl"
	{cat_ = CAT_UNIT;}
	break;
	case 80: 
	case 81: 
	case 82: 
	case 83: 
#line 31 "lex/ragel_units.rl"
	{
    type_ = TYPE_SMPTE;
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
#line 76 "lex/ragel_units.rl"
	{cat_ = CAT_UNIT;}
	break;
	case 48: 
	case 84: 
	case 85: 
	case 86: 
#line 10 "lex/ragel_numeric.rl"
	{ num_.ival *= num_.sign; cat_ = CAT_NUMBER; type_ = TYPE_INT; }
#line 28 "lex/ragel_units.rl"
	{num_.fval = num_.ival;}
#line 76 "lex/ragel_units.rl"
	{cat_ = CAT_UNIT;}
	break;
	case 78: 
	case 79: 
#line 17 "lex/ragel_numeric.rl"
	{ num_.inum *= num_.sign; cat_ = CAT_NUMBER; type_ = TYPE_RATIO; }
#line 70 "lex/ragel_units.rl"
	{type_ = TYPE_RATIO;}
#line 76 "lex/ragel_units.rl"
	{cat_ = CAT_UNIT;}
	break;
	case 88: 
#line 19 "lex/ragel_numeric.rl"
	{
        cat_ = CAT_NUMBER;
        type_ = TYPE_FLOAT;
        num_.fval = num_.ival + num_.sign * (double(num_.inum)/num_.iden);
    }
#line 10 "lex/ragel_music.rl"
	{ cat_ = CAT_UNIT; type_ = TYPE_BPM; }
#line 76 "lex/ragel_units.rl"
	{cat_ = CAT_UNIT;}
	break;
	case 91: 
#line 24 "lex/ragel_music.rl"
	{bpm.dur_num = 1;}
#line 10 "lex/ragel_music.rl"
	{ cat_ = CAT_UNIT; type_ = TYPE_BPM; }
#line 76 "lex/ragel_units.rl"
	{cat_ = CAT_UNIT;}
	break;
	case 87: 
	case 95: 
	case 96: 
	case 97: 
#line 10 "lex/ragel_numeric.rl"
	{ num_.ival *= num_.sign; cat_ = CAT_NUMBER; type_ = TYPE_INT; }
#line 28 "lex/ragel_units.rl"
	{num_.fval = num_.ival;}
#line 10 "lex/ragel_music.rl"
	{ cat_ = CAT_UNIT; type_ = TYPE_BPM; }
#line 76 "lex/ragel_units.rl"
	{cat_ = CAT_UNIT;}
	break;
	case 49: 
#line 21 "lex/ragel_units.rl"
	{type_ = TYPE_PERCENT;}
#line 63 "lex/ragel_numeric.rl"
	{type_ = TYPE_PERCENT;}
#line 68 "lex/ragel_units.rl"
	{type_ = TYPE_PERCENT;}
#line 76 "lex/ragel_units.rl"
	{cat_ = CAT_UNIT;}
	break;
	case 50: 
#line 23 "lex/ragel_units.rl"
	{type_ = TYPE_PHASE;}
#line 68 "lex/ragel_numeric.rl"
	{type_ = TYPE_PHASE;}
#line 69 "lex/ragel_units.rl"
	{type_ = TYPE_PHASE;}
#line 76 "lex/ragel_units.rl"
	{cat_ = CAT_UNIT;}
	break;
	case 70: 
#line 24 "lex/ragel_units.rl"
	{type_ = TYPE_PHASE;}
#line 68 "lex/ragel_numeric.rl"
	{type_ = TYPE_PHASE;}
#line 69 "lex/ragel_units.rl"
	{type_ = TYPE_PHASE;}
#line 76 "lex/ragel_units.rl"
	{cat_ = CAT_UNIT;}
	break;
#line 1855 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
	}
	}

	_out: {}
	}

#line 72 "lex/parser_units.rl"

    const bool ok = cs >= 48;
    if (ok) {
        unit_.value = num_.fval;
        if  (type_ == TYPE_RATIO) {
            if (num_.iden == 0) {
                fprintf(stderr, "division by zero: %s\n", str);
                return false;
            }

            unit_.value = double(num_.inum) / double(num_.iden);
        }
        else if(type_ == TYPE_INT)
            unit_.value = num_.ival;

        unit_.type = type_;
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


#line 1906 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
static const int units_type_start = 1;
static const int units_type_first_final = 28;
static const int units_type_error = 0;

static const int units_type_en_main = 1;


#line 120 "lex/parser_units.rl"


bool UnitTypeFullMatch::parse(const char* str)
{
    reset();

    const auto len = strlen(str);
    if (len == 0)
        return false;

    const char* p = str;
    const char* pe = p + len;
    const char* eof = pe;

    
#line 1930 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
	{
	cs = units_type_start;
	}

#line 135 "lex/parser_units.rl"
    
#line 1937 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
	{
	if ( p == pe )
		goto _test_eof;
	switch ( cs )
	{
case 1:
	switch( (*p) ) {
		case 37: goto st28;
		case 42: goto st29;
		case 72: goto st2;
		case 99: goto st31;
		case 100: goto st33;
		case 104: goto st38;
		case 109: goto st40;
		case 112: goto st12;
		case 114: goto st20;
		case 115: goto st48;
	}
	goto st0;
st0:
cs = 0;
	goto _out;
st28:
	if ( ++p == pe )
		goto _test_eof28;
case 28:
	goto st0;
st29:
	if ( ++p == pe )
		goto _test_eof29;
case 29:
	goto st0;
st2:
	if ( ++p == pe )
		goto _test_eof2;
case 2:
	if ( (*p) == 122 )
		goto st30;
	goto st0;
st30:
	if ( ++p == pe )
		goto _test_eof30;
case 30:
	goto st0;
st31:
	if ( ++p == pe )
		goto _test_eof31;
case 31:
	if ( (*p) == 101 )
		goto st3;
	goto st0;
st3:
	if ( ++p == pe )
		goto _test_eof3;
case 3:
	if ( (*p) == 110 )
		goto st4;
	goto st0;
st4:
	if ( ++p == pe )
		goto _test_eof4;
case 4:
	if ( (*p) == 116 )
		goto st32;
	goto st0;
st32:
	if ( ++p == pe )
		goto _test_eof32;
case 32:
	goto st0;
st33:
	if ( ++p == pe )
		goto _test_eof33;
case 33:
	switch( (*p) ) {
		case 97: goto st5;
		case 98: goto st35;
		case 101: goto st7;
	}
	goto st0;
st5:
	if ( ++p == pe )
		goto _test_eof5;
case 5:
	if ( (*p) == 121 )
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
	if ( (*p) == 102 )
		goto st6;
	goto st0;
st6:
	if ( ++p == pe )
		goto _test_eof6;
case 6:
	if ( (*p) == 115 )
		goto st36;
	goto st0;
st36:
	if ( ++p == pe )
		goto _test_eof36;
case 36:
	goto st0;
st7:
	if ( ++p == pe )
		goto _test_eof7;
case 7:
	if ( (*p) == 103 )
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
	switch( (*p) ) {
		case 111: goto st8;
		case 122: goto st30;
	}
	goto st0;
st8:
	if ( ++p == pe )
		goto _test_eof8;
case 8:
	if ( (*p) == 117 )
		goto st9;
	goto st0;
st9:
	if ( ++p == pe )
		goto _test_eof9;
case 9:
	if ( (*p) == 114 )
		goto st39;
	goto st0;
st39:
	if ( ++p == pe )
		goto _test_eof39;
case 39:
	goto st0;
st40:
	if ( ++p == pe )
		goto _test_eof40;
case 40:
	switch( (*p) ) {
		case 105: goto st10;
		case 115: goto st42;
	}
	goto st0;
st10:
	if ( ++p == pe )
		goto _test_eof10;
case 10:
	if ( (*p) == 110 )
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
	if ( (*p) == 101 )
		goto st11;
	goto st0;
st11:
	if ( ++p == pe )
		goto _test_eof11;
case 11:
	if ( (*p) == 99 )
		goto st43;
	goto st0;
st43:
	if ( ++p == pe )
		goto _test_eof43;
case 43:
	goto st0;
st12:
	if ( ++p == pe )
		goto _test_eof12;
case 12:
	switch( (*p) ) {
		case 101: goto st13;
		case 104: goto st17;
	}
	goto st0;
st13:
	if ( ++p == pe )
		goto _test_eof13;
case 13:
	if ( (*p) == 114 )
		goto st14;
	goto st0;
st14:
	if ( ++p == pe )
		goto _test_eof14;
case 14:
	if ( (*p) == 99 )
		goto st44;
	goto st0;
st44:
	if ( ++p == pe )
		goto _test_eof44;
case 44:
	if ( (*p) == 101 )
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
	if ( (*p) == 116 )
		goto st45;
	goto st0;
st45:
	if ( ++p == pe )
		goto _test_eof45;
case 45:
	goto st0;
st17:
	if ( ++p == pe )
		goto _test_eof17;
case 17:
	if ( (*p) == 97 )
		goto st18;
	goto st0;
st18:
	if ( ++p == pe )
		goto _test_eof18;
case 18:
	if ( (*p) == 115 )
		goto st19;
	goto st0;
st19:
	if ( ++p == pe )
		goto _test_eof19;
case 19:
	if ( (*p) == 101 )
		goto st46;
	goto st0;
st46:
	if ( ++p == pe )
		goto _test_eof46;
case 46:
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
	if ( (*p) == 100 )
		goto st47;
	goto st0;
st47:
	if ( ++p == pe )
		goto _test_eof47;
case 47:
	goto st0;
st48:
	if ( ++p == pe )
		goto _test_eof48;
case 48:
	switch( (*p) ) {
		case 97: goto st49;
		case 101: goto st23;
	}
	goto st0;
st49:
	if ( ++p == pe )
		goto _test_eof49;
case 49:
	if ( (*p) == 109 )
		goto st22;
	goto st0;
st22:
	if ( ++p == pe )
		goto _test_eof22;
case 22:
	if ( (*p) == 112 )
		goto st50;
	goto st0;
st50:
	if ( ++p == pe )
		goto _test_eof50;
case 50:
	goto st0;
st23:
	if ( ++p == pe )
		goto _test_eof23;
case 23:
	switch( (*p) ) {
		case 99: goto st51;
		case 109: goto st24;
	}
	goto st0;
st51:
	if ( ++p == pe )
		goto _test_eof51;
case 51:
	goto st0;
st24:
	if ( ++p == pe )
		goto _test_eof24;
case 24:
	if ( (*p) == 105 )
		goto st52;
	goto st0;
st52:
	if ( ++p == pe )
		goto _test_eof52;
case 52:
	if ( (*p) == 116 )
		goto st25;
	goto st0;
st25:
	if ( ++p == pe )
		goto _test_eof25;
case 25:
	if ( (*p) == 111 )
		goto st26;
	goto st0;
st26:
	if ( ++p == pe )
		goto _test_eof26;
case 26:
	if ( (*p) == 110 )
		goto st27;
	goto st0;
st27:
	if ( ++p == pe )
		goto _test_eof27;
case 27:
	if ( (*p) == 101 )
		goto st53;
	goto st0;
st53:
	if ( ++p == pe )
		goto _test_eof53;
case 53:
	goto st0;
	}
	_test_eof28: cs = 28; goto _test_eof; 
	_test_eof29: cs = 29; goto _test_eof; 
	_test_eof2: cs = 2; goto _test_eof; 
	_test_eof30: cs = 30; goto _test_eof; 
	_test_eof31: cs = 31; goto _test_eof; 
	_test_eof3: cs = 3; goto _test_eof; 
	_test_eof4: cs = 4; goto _test_eof; 
	_test_eof32: cs = 32; goto _test_eof; 
	_test_eof33: cs = 33; goto _test_eof; 
	_test_eof5: cs = 5; goto _test_eof; 
	_test_eof34: cs = 34; goto _test_eof; 
	_test_eof35: cs = 35; goto _test_eof; 
	_test_eof6: cs = 6; goto _test_eof; 
	_test_eof36: cs = 36; goto _test_eof; 
	_test_eof7: cs = 7; goto _test_eof; 
	_test_eof37: cs = 37; goto _test_eof; 
	_test_eof38: cs = 38; goto _test_eof; 
	_test_eof8: cs = 8; goto _test_eof; 
	_test_eof9: cs = 9; goto _test_eof; 
	_test_eof39: cs = 39; goto _test_eof; 
	_test_eof40: cs = 40; goto _test_eof; 
	_test_eof10: cs = 10; goto _test_eof; 
	_test_eof41: cs = 41; goto _test_eof; 
	_test_eof42: cs = 42; goto _test_eof; 
	_test_eof11: cs = 11; goto _test_eof; 
	_test_eof43: cs = 43; goto _test_eof; 
	_test_eof12: cs = 12; goto _test_eof; 
	_test_eof13: cs = 13; goto _test_eof; 
	_test_eof14: cs = 14; goto _test_eof; 
	_test_eof44: cs = 44; goto _test_eof; 
	_test_eof15: cs = 15; goto _test_eof; 
	_test_eof16: cs = 16; goto _test_eof; 
	_test_eof45: cs = 45; goto _test_eof; 
	_test_eof17: cs = 17; goto _test_eof; 
	_test_eof18: cs = 18; goto _test_eof; 
	_test_eof19: cs = 19; goto _test_eof; 
	_test_eof46: cs = 46; goto _test_eof; 
	_test_eof20: cs = 20; goto _test_eof; 
	_test_eof21: cs = 21; goto _test_eof; 
	_test_eof47: cs = 47; goto _test_eof; 
	_test_eof48: cs = 48; goto _test_eof; 
	_test_eof49: cs = 49; goto _test_eof; 
	_test_eof22: cs = 22; goto _test_eof; 
	_test_eof50: cs = 50; goto _test_eof; 
	_test_eof23: cs = 23; goto _test_eof; 
	_test_eof51: cs = 51; goto _test_eof; 
	_test_eof24: cs = 24; goto _test_eof; 
	_test_eof52: cs = 52; goto _test_eof; 
	_test_eof25: cs = 25; goto _test_eof; 
	_test_eof26: cs = 26; goto _test_eof; 
	_test_eof27: cs = 27; goto _test_eof; 
	_test_eof53: cs = 53; goto _test_eof; 

	_test_eof: {}
	if ( p == eof )
	{
	switch ( cs ) {
	case 30: 
#line 9 "lex/ragel_units.rl"
	{type_ = TYPE_HZ;}
	break;
	case 49: 
	case 50: 
#line 10 "lex/ragel_units.rl"
	{type_ = TYPE_SAMP;}
	break;
	case 47: 
#line 11 "lex/ragel_units.rl"
	{type_ = TYPE_RADIAN;}
	break;
	case 37: 
#line 12 "lex/ragel_units.rl"
	{type_ = TYPE_DEGREE;}
	break;
	case 35: 
	case 36: 
#line 13 "lex/ragel_units.rl"
	{type_ = TYPE_DB;}
	break;
	case 33: 
	case 34: 
#line 14 "lex/ragel_units.rl"
	{type_ = TYPE_DAY;}
	break;
	case 38: 
	case 39: 
#line 15 "lex/ragel_units.rl"
	{type_ = TYPE_HOUR;}
	break;
	case 40: 
	case 41: 
#line 16 "lex/ragel_units.rl"
	{type_ = TYPE_MIN;}
	break;
	case 48: 
	case 51: 
#line 17 "lex/ragel_units.rl"
	{type_ = TYPE_SEC;}
	break;
	case 42: 
	case 43: 
#line 18 "lex/ragel_units.rl"
	{type_ = TYPE_MSEC;}
	break;
	case 31: 
	case 32: 
#line 19 "lex/ragel_units.rl"
	{type_ = TYPE_CENT;}
	break;
	case 52: 
	case 53: 
#line 20 "lex/ragel_units.rl"
	{type_ = TYPE_SEMITONE;}
	break;
	case 28: 
#line 21 "lex/ragel_units.rl"
	{type_ = TYPE_PERCENT;}
	break;
	case 44: 
	case 45: 
#line 22 "lex/ragel_units.rl"
	{type_ = TYPE_PERCENT;}
	break;
	case 29: 
#line 23 "lex/ragel_units.rl"
	{type_ = TYPE_PHASE;}
	break;
	case 46: 
#line 24 "lex/ragel_units.rl"
	{type_ = TYPE_PHASE;}
	break;
#line 2437 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
	}
	}

	_out: {}
	}

#line 136 "lex/parser_units.rl"

    return cs >= 28;
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


