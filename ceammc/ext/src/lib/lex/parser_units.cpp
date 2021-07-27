
#line 1 "lex/parser_units.rl"
# include "parser_units.h"

# include <cstdint>
# include <cstring>
# include <cstdio>

namespace ceammc {
namespace parser {


#line 14 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
static const int units_full_start = 1;
static const int units_full_first_final = 45;
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
#line 71 "lex/ragel_units.rl"
	{type_ = TYPE_UNKNOWN; cat_ = CAT_UNKNOWN;}
#line 62 "lex/ragel_units.rl"
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
#line 71 "lex/ragel_units.rl"
	{type_ = TYPE_UNKNOWN; cat_ = CAT_UNKNOWN;}
#line 62 "lex/ragel_units.rl"
	{pos_ = POSITION_ABS;}
#line 4 "lex/ragel_numeric.rl"
	{ num_.sign = ((*p)=='-') ? -1 : 1; }
#line 53 "lex/ragel_units.rl"
	{smpte.np = 0;}
#line 48 "lex/ragel_units.rl"
	{smpte.sign = ((*p)=='-')?-1:1;}
	goto st3;
tr8:
#line 56 "lex/ragel_units.rl"
	{pos_ = POSITION_END;}
#line 4 "lex/ragel_numeric.rl"
	{ num_.sign = ((*p)=='-') ? -1 : 1; }
#line 53 "lex/ragel_units.rl"
	{smpte.np = 0;}
#line 48 "lex/ragel_units.rl"
	{smpte.sign = ((*p)=='-')?-1:1;}
	goto st3;
tr56:
#line 57 "lex/ragel_units.rl"
	{pos_ = POSITION_CURRENT;}
#line 4 "lex/ragel_numeric.rl"
	{ num_.sign = ((*p)=='-') ? -1 : 1; }
#line 53 "lex/ragel_units.rl"
	{smpte.np = 0;}
#line 48 "lex/ragel_units.rl"
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
#line 56 "lex/ragel_units.rl"
	{pos_ = POSITION_END;}
#line 9 "lex/ragel_numeric.rl"
	{ (num_.ival *= 10) += ((*p)-'0'); }
#line 15 "lex/ragel_numeric.rl"
	{ (num_.inum *= 10) += ((*p)-'0'); }
#line 53 "lex/ragel_units.rl"
	{smpte.np = 0;}
#line 41 "lex/ragel_units.rl"
	{smpte.hour = 0; smpte.np++;}
#line 41 "lex/ragel_units.rl"
	{(smpte.hour *= 10) += ((*p) - '0');}
	goto st45;
tr12:
#line 9 "lex/ragel_numeric.rl"
	{ (num_.ival *= 10) += ((*p)-'0'); }
#line 15 "lex/ragel_numeric.rl"
	{ (num_.inum *= 10) += ((*p)-'0'); }
#line 41 "lex/ragel_units.rl"
	{smpte.hour = 0; smpte.np++;}
#line 41 "lex/ragel_units.rl"
	{(smpte.hour *= 10) += ((*p) - '0');}
	goto st45;
tr57:
#line 57 "lex/ragel_units.rl"
	{pos_ = POSITION_CURRENT;}
#line 9 "lex/ragel_numeric.rl"
	{ (num_.ival *= 10) += ((*p)-'0'); }
#line 15 "lex/ragel_numeric.rl"
	{ (num_.inum *= 10) += ((*p)-'0'); }
#line 53 "lex/ragel_units.rl"
	{smpte.np = 0;}
#line 41 "lex/ragel_units.rl"
	{smpte.hour = 0; smpte.np++;}
#line 41 "lex/ragel_units.rl"
	{(smpte.hour *= 10) += ((*p) - '0');}
	goto st45;
st45:
	if ( ++p == pe )
		goto _test_eof45;
case 45:
#line 218 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
	switch( (*p) ) {
		case 37: goto tr62;
		case 42: goto tr63;
		case 46: goto tr64;
		case 47: goto st27;
		case 72: goto tr67;
		case 95: goto tr68;
		case 99: goto tr69;
		case 100: goto tr70;
		case 104: goto tr71;
		case 109: goto tr72;
		case 112: goto tr73;
		case 114: goto tr74;
		case 115: goto tr75;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr66;
	goto st0;
tr62:
#line 10 "lex/ragel_numeric.rl"
	{ num_.ival *= num_.sign; cat_ = CAT_NUMBER; type_ = TYPE_INT; }
#line 60 "lex/ragel_numeric.rl"
	{num_.fval = num_.ival;}
	goto st46;
tr76:
#line 19 "lex/ragel_numeric.rl"
	{
        cat_ = CAT_NUMBER;
        type_ = TYPE_FLOAT;
        num_.fval = num_.ival + num_.sign * (double(num_.inum)/num_.iden);
    }
	goto st46;
st46:
	if ( ++p == pe )
		goto _test_eof46;
case 46:
#line 255 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
	goto st0;
tr63:
#line 10 "lex/ragel_numeric.rl"
	{ num_.ival *= num_.sign; cat_ = CAT_NUMBER; type_ = TYPE_INT; }
#line 65 "lex/ragel_numeric.rl"
	{num_.fval = num_.ival;}
	goto st47;
tr77:
#line 19 "lex/ragel_numeric.rl"
	{
        cat_ = CAT_NUMBER;
        type_ = TYPE_FLOAT;
        num_.fval = num_.ival + num_.sign * (double(num_.inum)/num_.iden);
    }
	goto st47;
st47:
	if ( ++p == pe )
		goto _test_eof47;
case 47:
#line 275 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
	goto st0;
tr64:
#line 10 "lex/ragel_numeric.rl"
	{ num_.ival *= num_.sign; cat_ = CAT_NUMBER; type_ = TYPE_INT; }
#line 54 "lex/ragel_numeric.rl"
	{num_.inum = 0; num_.iden = 1;}
	goto st4;
st4:
	if ( ++p == pe )
		goto _test_eof4;
case 4:
#line 287 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr15;
	goto st0;
tr15:
#line 46 "lex/ragel_numeric.rl"
	{(num_.inum *= 10) += ((*p) - '0'); num_.iden *= 10;}
	goto st48;
st48:
	if ( ++p == pe )
		goto _test_eof48;
case 48:
#line 299 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
	switch( (*p) ) {
		case 37: goto tr76;
		case 42: goto tr77;
		case 72: goto tr78;
		case 95: goto tr79;
		case 99: goto tr80;
		case 100: goto tr81;
		case 104: goto tr82;
		case 109: goto tr83;
		case 112: goto tr84;
		case 114: goto tr85;
		case 115: goto tr86;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr15;
	goto st0;
tr67:
#line 10 "lex/ragel_numeric.rl"
	{ num_.ival *= num_.sign; cat_ = CAT_NUMBER; type_ = TYPE_INT; }
#line 24 "lex/ragel_units.rl"
	{num_.fval = num_.ival;}
	goto st5;
tr78:
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
#line 334 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
	if ( (*p) == 122 )
		goto st49;
	goto st0;
st49:
	if ( ++p == pe )
		goto _test_eof49;
case 49:
	goto st0;
tr68:
#line 10 "lex/ragel_numeric.rl"
	{ num_.ival *= num_.sign; cat_ = CAT_NUMBER; type_ = TYPE_INT; }
#line 24 "lex/ragel_units.rl"
	{num_.fval = num_.ival;}
#line 60 "lex/ragel_numeric.rl"
	{num_.fval = num_.ival;}
#line 65 "lex/ragel_numeric.rl"
	{num_.fval = num_.ival;}
	goto st50;
tr79:
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
#line 365 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
	switch( (*p) ) {
		case 37: goto st46;
		case 72: goto st5;
		case 99: goto st51;
		case 100: goto st53;
		case 104: goto st58;
		case 109: goto st60;
		case 112: goto st15;
		case 114: goto st19;
		case 115: goto st65;
	}
	goto st0;
tr69:
#line 10 "lex/ragel_numeric.rl"
	{ num_.ival *= num_.sign; cat_ = CAT_NUMBER; type_ = TYPE_INT; }
#line 24 "lex/ragel_units.rl"
	{num_.fval = num_.ival;}
	goto st51;
tr80:
#line 19 "lex/ragel_numeric.rl"
	{
        cat_ = CAT_NUMBER;
        type_ = TYPE_FLOAT;
        num_.fval = num_.ival + num_.sign * (double(num_.inum)/num_.iden);
    }
	goto st51;
st51:
	if ( ++p == pe )
		goto _test_eof51;
case 51:
#line 396 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
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
		goto st52;
	goto st0;
st52:
	if ( ++p == pe )
		goto _test_eof52;
case 52:
	goto st0;
tr70:
#line 10 "lex/ragel_numeric.rl"
	{ num_.ival *= num_.sign; cat_ = CAT_NUMBER; type_ = TYPE_INT; }
#line 24 "lex/ragel_units.rl"
	{num_.fval = num_.ival;}
	goto st53;
tr81:
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
#line 437 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
	switch( (*p) ) {
		case 97: goto st8;
		case 98: goto st55;
		case 101: goto st10;
	}
	goto st0;
st8:
	if ( ++p == pe )
		goto _test_eof8;
case 8:
	if ( (*p) == 121 )
		goto st54;
	goto st0;
st54:
	if ( ++p == pe )
		goto _test_eof54;
case 54:
	goto st0;
st55:
	if ( ++p == pe )
		goto _test_eof55;
case 55:
	if ( (*p) == 102 )
		goto st9;
	goto st0;
st9:
	if ( ++p == pe )
		goto _test_eof9;
case 9:
	if ( (*p) == 115 )
		goto st56;
	goto st0;
st56:
	if ( ++p == pe )
		goto _test_eof56;
case 56:
	goto st0;
st10:
	if ( ++p == pe )
		goto _test_eof10;
case 10:
	if ( (*p) == 103 )
		goto st57;
	goto st0;
st57:
	if ( ++p == pe )
		goto _test_eof57;
case 57:
	goto st0;
tr71:
#line 10 "lex/ragel_numeric.rl"
	{ num_.ival *= num_.sign; cat_ = CAT_NUMBER; type_ = TYPE_INT; }
#line 24 "lex/ragel_units.rl"
	{num_.fval = num_.ival;}
	goto st58;
tr82:
#line 19 "lex/ragel_numeric.rl"
	{
        cat_ = CAT_NUMBER;
        type_ = TYPE_FLOAT;
        num_.fval = num_.ival + num_.sign * (double(num_.inum)/num_.iden);
    }
	goto st58;
st58:
	if ( ++p == pe )
		goto _test_eof58;
case 58:
#line 505 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
	switch( (*p) ) {
		case 111: goto st11;
		case 122: goto st49;
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
		goto st59;
	goto st0;
st59:
	if ( ++p == pe )
		goto _test_eof59;
case 59:
	goto st0;
tr72:
#line 10 "lex/ragel_numeric.rl"
	{ num_.ival *= num_.sign; cat_ = CAT_NUMBER; type_ = TYPE_INT; }
#line 24 "lex/ragel_units.rl"
	{num_.fval = num_.ival;}
	goto st60;
tr83:
#line 19 "lex/ragel_numeric.rl"
	{
        cat_ = CAT_NUMBER;
        type_ = TYPE_FLOAT;
        num_.fval = num_.ival + num_.sign * (double(num_.inum)/num_.iden);
    }
	goto st60;
st60:
	if ( ++p == pe )
		goto _test_eof60;
case 60:
#line 548 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
	switch( (*p) ) {
		case 105: goto st13;
		case 115: goto st62;
	}
	goto st0;
st13:
	if ( ++p == pe )
		goto _test_eof13;
case 13:
	if ( (*p) == 110 )
		goto st61;
	goto st0;
st61:
	if ( ++p == pe )
		goto _test_eof61;
case 61:
	goto st0;
st62:
	if ( ++p == pe )
		goto _test_eof62;
case 62:
	if ( (*p) == 101 )
		goto st14;
	goto st0;
st14:
	if ( ++p == pe )
		goto _test_eof14;
case 14:
	if ( (*p) == 99 )
		goto st63;
	goto st0;
st63:
	if ( ++p == pe )
		goto _test_eof63;
case 63:
	goto st0;
tr73:
#line 10 "lex/ragel_numeric.rl"
	{ num_.ival *= num_.sign; cat_ = CAT_NUMBER; type_ = TYPE_INT; }
#line 65 "lex/ragel_numeric.rl"
	{num_.fval = num_.ival;}
	goto st15;
tr84:
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
#line 603 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
	if ( (*p) == 104 )
		goto st16;
	goto st0;
st16:
	if ( ++p == pe )
		goto _test_eof16;
case 16:
	if ( (*p) == 97 )
		goto st17;
	goto st0;
st17:
	if ( ++p == pe )
		goto _test_eof17;
case 17:
	if ( (*p) == 115 )
		goto st18;
	goto st0;
st18:
	if ( ++p == pe )
		goto _test_eof18;
case 18:
	if ( (*p) == 101 )
		goto st47;
	goto st0;
tr74:
#line 10 "lex/ragel_numeric.rl"
	{ num_.ival *= num_.sign; cat_ = CAT_NUMBER; type_ = TYPE_INT; }
#line 24 "lex/ragel_units.rl"
	{num_.fval = num_.ival;}
	goto st19;
tr85:
#line 19 "lex/ragel_numeric.rl"
	{
        cat_ = CAT_NUMBER;
        type_ = TYPE_FLOAT;
        num_.fval = num_.ival + num_.sign * (double(num_.inum)/num_.iden);
    }
	goto st19;
st19:
	if ( ++p == pe )
		goto _test_eof19;
case 19:
#line 646 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
	if ( (*p) == 97 )
		goto st20;
	goto st0;
st20:
	if ( ++p == pe )
		goto _test_eof20;
case 20:
	if ( (*p) == 100 )
		goto st64;
	goto st0;
st64:
	if ( ++p == pe )
		goto _test_eof64;
case 64:
	goto st0;
tr75:
#line 10 "lex/ragel_numeric.rl"
	{ num_.ival *= num_.sign; cat_ = CAT_NUMBER; type_ = TYPE_INT; }
#line 24 "lex/ragel_units.rl"
	{num_.fval = num_.ival;}
	goto st65;
tr86:
#line 19 "lex/ragel_numeric.rl"
	{
        cat_ = CAT_NUMBER;
        type_ = TYPE_FLOAT;
        num_.fval = num_.ival + num_.sign * (double(num_.inum)/num_.iden);
    }
	goto st65;
st65:
	if ( ++p == pe )
		goto _test_eof65;
case 65:
#line 680 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
	switch( (*p) ) {
		case 97: goto st66;
		case 101: goto st22;
	}
	goto st0;
st66:
	if ( ++p == pe )
		goto _test_eof66;
case 66:
	if ( (*p) == 109 )
		goto st21;
	goto st0;
st21:
	if ( ++p == pe )
		goto _test_eof21;
case 21:
	if ( (*p) == 112 )
		goto st67;
	goto st0;
st67:
	if ( ++p == pe )
		goto _test_eof67;
case 67:
	goto st0;
st22:
	if ( ++p == pe )
		goto _test_eof22;
case 22:
	switch( (*p) ) {
		case 99: goto st68;
		case 109: goto st23;
	}
	goto st0;
st68:
	if ( ++p == pe )
		goto _test_eof68;
case 68:
	goto st0;
st23:
	if ( ++p == pe )
		goto _test_eof23;
case 23:
	if ( (*p) == 105 )
		goto st69;
	goto st0;
st69:
	if ( ++p == pe )
		goto _test_eof69;
case 69:
	if ( (*p) == 116 )
		goto st24;
	goto st0;
st24:
	if ( ++p == pe )
		goto _test_eof24;
case 24:
	if ( (*p) == 111 )
		goto st25;
	goto st0;
st25:
	if ( ++p == pe )
		goto _test_eof25;
case 25:
	if ( (*p) == 110 )
		goto st26;
	goto st0;
st26:
	if ( ++p == pe )
		goto _test_eof26;
case 26:
	if ( (*p) == 101 )
		goto st70;
	goto st0;
st70:
	if ( ++p == pe )
		goto _test_eof70;
case 70:
	goto st0;
st27:
	if ( ++p == pe )
		goto _test_eof27;
case 27:
	if ( (*p) == 48 )
		goto tr39;
	if ( 49 <= (*p) && (*p) <= 57 )
		goto tr40;
	goto st0;
tr39:
#line 16 "lex/ragel_numeric.rl"
	{ (num_.iden *= 10) += ((*p)-'0'); }
	goto st71;
st71:
	if ( ++p == pe )
		goto _test_eof71;
case 71:
#line 776 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
	goto st0;
tr40:
#line 16 "lex/ragel_numeric.rl"
	{ (num_.iden *= 10) += ((*p)-'0'); }
	goto st72;
st72:
	if ( ++p == pe )
		goto _test_eof72;
case 72:
#line 786 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr40;
	goto st0;
tr66:
#line 41 "lex/ragel_units.rl"
	{(smpte.hour *= 10) += ((*p) - '0');}
	goto st28;
st28:
	if ( ++p == pe )
		goto _test_eof28;
case 28:
#line 798 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
	if ( (*p) == 58 )
		goto st29;
	goto st0;
st29:
	if ( ++p == pe )
		goto _test_eof29;
case 29:
	if ( 48 <= (*p) && (*p) <= 53 )
		goto tr42;
	goto st0;
tr42:
#line 42 "lex/ragel_units.rl"
	{smpte.min = 0; smpte.np++;}
#line 42 "lex/ragel_units.rl"
	{(smpte.min *= 10) += ((*p) - '0');}
	goto st30;
st30:
	if ( ++p == pe )
		goto _test_eof30;
case 30:
#line 819 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr43;
	goto st0;
tr43:
#line 42 "lex/ragel_units.rl"
	{(smpte.min *= 10) += ((*p) - '0');}
	goto st73;
st73:
	if ( ++p == pe )
		goto _test_eof73;
case 73:
#line 831 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
	switch( (*p) ) {
		case 46: goto st31;
		case 58: goto st32;
	}
	goto st0;
st31:
	if ( ++p == pe )
		goto _test_eof31;
case 31:
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr44;
	goto st0;
tr44:
#line 44 "lex/ragel_units.rl"
	{smpte.frame = 0;}
#line 44 "lex/ragel_units.rl"
	{(smpte.frame *= 10) += ((*p) - '0');}
	goto st74;
st74:
	if ( ++p == pe )
		goto _test_eof74;
case 74:
#line 854 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr111;
	goto st0;
tr111:
#line 44 "lex/ragel_units.rl"
	{(smpte.frame *= 10) += ((*p) - '0');}
	goto st75;
st75:
	if ( ++p == pe )
		goto _test_eof75;
case 75:
#line 866 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
	goto st0;
st32:
	if ( ++p == pe )
		goto _test_eof32;
case 32:
	if ( 48 <= (*p) && (*p) <= 53 )
		goto tr45;
	goto st0;
tr45:
#line 43 "lex/ragel_units.rl"
	{smpte.sec = 0; smpte.np++;}
#line 43 "lex/ragel_units.rl"
	{(smpte.sec *= 10) += ((*p) - '0');}
	goto st33;
st33:
	if ( ++p == pe )
		goto _test_eof33;
case 33:
#line 885 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr46;
	goto st0;
tr46:
#line 43 "lex/ragel_units.rl"
	{(smpte.sec *= 10) += ((*p) - '0');}
	goto st76;
st76:
	if ( ++p == pe )
		goto _test_eof76;
case 76:
#line 897 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
	if ( (*p) == 46 )
		goto st31;
	goto st0;
tr10:
#line 56 "lex/ragel_units.rl"
	{pos_ = POSITION_END;}
#line 9 "lex/ragel_numeric.rl"
	{ (num_.ival *= 10) += ((*p)-'0'); }
#line 15 "lex/ragel_numeric.rl"
	{ (num_.inum *= 10) += ((*p)-'0'); }
#line 53 "lex/ragel_units.rl"
	{smpte.np = 0;}
#line 41 "lex/ragel_units.rl"
	{smpte.hour = 0; smpte.np++;}
#line 41 "lex/ragel_units.rl"
	{(smpte.hour *= 10) += ((*p) - '0');}
	goto st77;
tr13:
#line 9 "lex/ragel_numeric.rl"
	{ (num_.ival *= 10) += ((*p)-'0'); }
#line 15 "lex/ragel_numeric.rl"
	{ (num_.inum *= 10) += ((*p)-'0'); }
#line 41 "lex/ragel_units.rl"
	{smpte.hour = 0; smpte.np++;}
#line 41 "lex/ragel_units.rl"
	{(smpte.hour *= 10) += ((*p) - '0');}
	goto st77;
tr58:
#line 57 "lex/ragel_units.rl"
	{pos_ = POSITION_CURRENT;}
#line 9 "lex/ragel_numeric.rl"
	{ (num_.ival *= 10) += ((*p)-'0'); }
#line 15 "lex/ragel_numeric.rl"
	{ (num_.inum *= 10) += ((*p)-'0'); }
#line 53 "lex/ragel_units.rl"
	{smpte.np = 0;}
#line 41 "lex/ragel_units.rl"
	{smpte.hour = 0; smpte.np++;}
#line 41 "lex/ragel_units.rl"
	{(smpte.hour *= 10) += ((*p) - '0');}
	goto st77;
st77:
	if ( ++p == pe )
		goto _test_eof77;
case 77:
#line 943 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
	switch( (*p) ) {
		case 37: goto tr62;
		case 42: goto tr63;
		case 46: goto tr64;
		case 47: goto st27;
		case 72: goto tr67;
		case 95: goto tr68;
		case 99: goto tr69;
		case 100: goto tr70;
		case 104: goto tr71;
		case 109: goto tr72;
		case 112: goto tr73;
		case 114: goto tr74;
		case 115: goto tr75;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr112;
	goto st0;
tr112:
#line 9 "lex/ragel_numeric.rl"
	{ (num_.ival *= 10) += ((*p)-'0'); }
#line 15 "lex/ragel_numeric.rl"
	{ (num_.inum *= 10) += ((*p)-'0'); }
#line 41 "lex/ragel_units.rl"
	{(smpte.hour *= 10) += ((*p) - '0');}
	goto st78;
st78:
	if ( ++p == pe )
		goto _test_eof78;
case 78:
#line 974 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
	switch( (*p) ) {
		case 37: goto tr62;
		case 42: goto tr63;
		case 46: goto tr64;
		case 47: goto st27;
		case 58: goto st29;
		case 72: goto tr67;
		case 95: goto tr68;
		case 99: goto tr69;
		case 100: goto tr70;
		case 104: goto tr71;
		case 109: goto tr72;
		case 112: goto tr73;
		case 114: goto tr74;
		case 115: goto tr75;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr14;
	goto st0;
tr11:
#line 56 "lex/ragel_units.rl"
	{pos_ = POSITION_END;}
#line 9 "lex/ragel_numeric.rl"
	{ (num_.ival *= 10) += ((*p)-'0'); }
#line 15 "lex/ragel_numeric.rl"
	{ (num_.inum *= 10) += ((*p)-'0'); }
	goto st79;
tr14:
#line 9 "lex/ragel_numeric.rl"
	{ (num_.ival *= 10) += ((*p)-'0'); }
#line 15 "lex/ragel_numeric.rl"
	{ (num_.inum *= 10) += ((*p)-'0'); }
	goto st79;
tr59:
#line 57 "lex/ragel_units.rl"
	{pos_ = POSITION_CURRENT;}
#line 9 "lex/ragel_numeric.rl"
	{ (num_.ival *= 10) += ((*p)-'0'); }
#line 15 "lex/ragel_numeric.rl"
	{ (num_.inum *= 10) += ((*p)-'0'); }
	goto st79;
st79:
	if ( ++p == pe )
		goto _test_eof79;
case 79:
#line 1020 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
	switch( (*p) ) {
		case 37: goto tr62;
		case 42: goto tr63;
		case 46: goto tr64;
		case 47: goto st27;
		case 72: goto tr67;
		case 95: goto tr68;
		case 99: goto tr69;
		case 100: goto tr70;
		case 104: goto tr71;
		case 109: goto tr72;
		case 112: goto tr73;
		case 114: goto tr74;
		case 115: goto tr75;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr14;
	goto st0;
tr3:
#line 71 "lex/ragel_units.rl"
	{type_ = TYPE_UNKNOWN; cat_ = CAT_UNKNOWN;}
#line 62 "lex/ragel_units.rl"
	{pos_ = POSITION_ABS;}
#line 9 "lex/ragel_numeric.rl"
	{ (num_.ival *= 10) += ((*p)-'0'); }
#line 15 "lex/ragel_numeric.rl"
	{ (num_.inum *= 10) += ((*p)-'0'); }
#line 53 "lex/ragel_units.rl"
	{smpte.np = 0;}
#line 41 "lex/ragel_units.rl"
	{smpte.hour = 0; smpte.np++;}
#line 41 "lex/ragel_units.rl"
	{(smpte.hour *= 10) += ((*p) - '0');}
#line 24 "lex/ragel_music.rl"
	{bpm.dur_num = 1;bpm.dur_den = 4;}
#line 6 "lex/ragel_music.rl"
	{(bpm.ival *= 10) += ((*p) - '0');}
	goto st80;
st80:
	if ( ++p == pe )
		goto _test_eof80;
case 80:
#line 1063 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
	switch( (*p) ) {
		case 37: goto tr62;
		case 40: goto st34;
		case 42: goto tr63;
		case 46: goto tr114;
		case 47: goto st27;
		case 72: goto tr67;
		case 95: goto tr115;
		case 98: goto st36;
		case 99: goto tr69;
		case 100: goto tr70;
		case 104: goto tr71;
		case 109: goto tr72;
		case 112: goto tr73;
		case 114: goto tr74;
		case 115: goto tr75;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr66;
	goto st0;
st34:
	if ( ++p == pe )
		goto _test_eof34;
case 34:
	if ( 49 <= (*p) && (*p) <= 57 )
		goto tr47;
	goto st0;
tr47:
#line 14 "lex/ragel_music.rl"
	{bpm.dur_den=0;}
#line 14 "lex/ragel_music.rl"
	{(bpm.dur_den *= 10) += ((*p) - '0');}
	goto st35;
tr50:
#line 14 "lex/ragel_music.rl"
	{(bpm.dur_den *= 10) += ((*p) - '0');}
	goto st35;
st35:
	if ( ++p == pe )
		goto _test_eof35;
case 35:
#line 1105 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
	switch( (*p) ) {
		case 41: goto st81;
		case 46: goto tr49;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr50;
	goto st0;
st81:
	if ( ++p == pe )
		goto _test_eof81;
case 81:
	switch( (*p) ) {
		case 95: goto st82;
		case 98: goto st36;
	}
	goto st0;
st82:
	if ( ++p == pe )
		goto _test_eof82;
case 82:
	if ( (*p) == 98 )
		goto st36;
	goto st0;
st36:
	if ( ++p == pe )
		goto _test_eof36;
case 36:
	if ( (*p) == 112 )
		goto st37;
	goto st0;
st37:
	if ( ++p == pe )
		goto _test_eof37;
case 37:
	if ( (*p) == 109 )
		goto st83;
	goto st0;
st83:
	if ( ++p == pe )
		goto _test_eof83;
case 83:
	goto st0;
tr49:
#line 15 "lex/ragel_music.rl"
	{bpm.dur_num *= 3; bpm.dur_den *= 2;}
	goto st38;
st38:
	if ( ++p == pe )
		goto _test_eof38;
case 38:
#line 1156 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
	if ( (*p) == 41 )
		goto st81;
	goto st0;
tr114:
#line 10 "lex/ragel_numeric.rl"
	{ num_.ival *= num_.sign; cat_ = CAT_NUMBER; type_ = TYPE_INT; }
#line 54 "lex/ragel_numeric.rl"
	{num_.inum = 0; num_.iden = 1;}
	goto st39;
st39:
	if ( ++p == pe )
		goto _test_eof39;
case 39:
#line 1170 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr53;
	goto st0;
tr53:
#line 46 "lex/ragel_numeric.rl"
	{(num_.inum *= 10) += ((*p) - '0'); num_.iden *= 10;}
#line 7 "lex/ragel_music.rl"
	{(bpm.fnum *= 10) += ((*p) - '0'); bpm.fden *= 10;}
	goto st84;
st84:
	if ( ++p == pe )
		goto _test_eof84;
case 84:
#line 1184 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
	switch( (*p) ) {
		case 37: goto tr76;
		case 40: goto st34;
		case 42: goto tr77;
		case 72: goto tr78;
		case 95: goto tr118;
		case 98: goto st36;
		case 99: goto tr80;
		case 100: goto tr81;
		case 104: goto tr82;
		case 109: goto tr83;
		case 112: goto tr84;
		case 114: goto tr85;
		case 115: goto tr86;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr53;
	goto st0;
tr115:
#line 10 "lex/ragel_numeric.rl"
	{ num_.ival *= num_.sign; cat_ = CAT_NUMBER; type_ = TYPE_INT; }
#line 24 "lex/ragel_units.rl"
	{num_.fval = num_.ival;}
#line 60 "lex/ragel_numeric.rl"
	{num_.fval = num_.ival;}
#line 65 "lex/ragel_numeric.rl"
	{num_.fval = num_.ival;}
	goto st85;
tr118:
#line 19 "lex/ragel_numeric.rl"
	{
        cat_ = CAT_NUMBER;
        type_ = TYPE_FLOAT;
        num_.fval = num_.ival + num_.sign * (double(num_.inum)/num_.iden);
    }
	goto st85;
st85:
	if ( ++p == pe )
		goto _test_eof85;
case 85:
#line 1225 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
	switch( (*p) ) {
		case 37: goto st46;
		case 72: goto st5;
		case 98: goto st36;
		case 99: goto st51;
		case 100: goto st53;
		case 104: goto st58;
		case 109: goto st60;
		case 112: goto st15;
		case 114: goto st19;
		case 115: goto st65;
	}
	goto st0;
tr4:
#line 71 "lex/ragel_units.rl"
	{type_ = TYPE_UNKNOWN; cat_ = CAT_UNKNOWN;}
#line 62 "lex/ragel_units.rl"
	{pos_ = POSITION_ABS;}
#line 9 "lex/ragel_numeric.rl"
	{ (num_.ival *= 10) += ((*p)-'0'); }
#line 15 "lex/ragel_numeric.rl"
	{ (num_.inum *= 10) += ((*p)-'0'); }
#line 53 "lex/ragel_units.rl"
	{smpte.np = 0;}
#line 41 "lex/ragel_units.rl"
	{smpte.hour = 0; smpte.np++;}
#line 41 "lex/ragel_units.rl"
	{(smpte.hour *= 10) += ((*p) - '0');}
#line 24 "lex/ragel_music.rl"
	{bpm.dur_num = 1;bpm.dur_den = 4;}
#line 6 "lex/ragel_music.rl"
	{(bpm.ival *= 10) += ((*p) - '0');}
	goto st86;
st86:
	if ( ++p == pe )
		goto _test_eof86;
case 86:
#line 1263 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
	switch( (*p) ) {
		case 37: goto tr62;
		case 40: goto st34;
		case 42: goto tr63;
		case 46: goto tr114;
		case 47: goto st27;
		case 72: goto tr67;
		case 95: goto tr115;
		case 98: goto st36;
		case 99: goto tr69;
		case 100: goto tr70;
		case 104: goto tr71;
		case 109: goto tr72;
		case 112: goto tr73;
		case 114: goto tr74;
		case 115: goto tr75;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr119;
	goto st0;
tr119:
#line 9 "lex/ragel_numeric.rl"
	{ (num_.ival *= 10) += ((*p)-'0'); }
#line 15 "lex/ragel_numeric.rl"
	{ (num_.inum *= 10) += ((*p)-'0'); }
#line 41 "lex/ragel_units.rl"
	{(smpte.hour *= 10) += ((*p) - '0');}
#line 6 "lex/ragel_music.rl"
	{(bpm.ival *= 10) += ((*p) - '0');}
	goto st87;
st87:
	if ( ++p == pe )
		goto _test_eof87;
case 87:
#line 1298 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
	switch( (*p) ) {
		case 37: goto tr62;
		case 40: goto st34;
		case 42: goto tr63;
		case 46: goto tr114;
		case 47: goto st27;
		case 58: goto st29;
		case 72: goto tr67;
		case 95: goto tr115;
		case 98: goto st36;
		case 99: goto tr69;
		case 100: goto tr70;
		case 104: goto tr71;
		case 109: goto tr72;
		case 112: goto tr73;
		case 114: goto tr74;
		case 115: goto tr75;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr120;
	goto st0;
tr5:
#line 71 "lex/ragel_units.rl"
	{type_ = TYPE_UNKNOWN; cat_ = CAT_UNKNOWN;}
#line 62 "lex/ragel_units.rl"
	{pos_ = POSITION_ABS;}
#line 9 "lex/ragel_numeric.rl"
	{ (num_.ival *= 10) += ((*p)-'0'); }
#line 15 "lex/ragel_numeric.rl"
	{ (num_.inum *= 10) += ((*p)-'0'); }
#line 24 "lex/ragel_music.rl"
	{bpm.dur_num = 1;bpm.dur_den = 4;}
#line 6 "lex/ragel_music.rl"
	{(bpm.ival *= 10) += ((*p) - '0');}
	goto st88;
tr120:
#line 9 "lex/ragel_numeric.rl"
	{ (num_.ival *= 10) += ((*p)-'0'); }
#line 15 "lex/ragel_numeric.rl"
	{ (num_.inum *= 10) += ((*p)-'0'); }
#line 6 "lex/ragel_music.rl"
	{(bpm.ival *= 10) += ((*p) - '0');}
	goto st88;
st88:
	if ( ++p == pe )
		goto _test_eof88;
case 88:
#line 1346 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
	switch( (*p) ) {
		case 37: goto tr62;
		case 40: goto st34;
		case 42: goto tr63;
		case 46: goto tr114;
		case 47: goto st27;
		case 72: goto tr67;
		case 95: goto tr115;
		case 98: goto st36;
		case 99: goto tr69;
		case 100: goto tr70;
		case 104: goto tr71;
		case 109: goto tr72;
		case 112: goto tr73;
		case 114: goto tr74;
		case 115: goto tr75;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr120;
	goto st0;
tr6:
#line 71 "lex/ragel_units.rl"
	{type_ = TYPE_UNKNOWN; cat_ = CAT_UNKNOWN;}
#line 62 "lex/ragel_units.rl"
	{pos_ = POSITION_ABS;}
	goto st40;
st40:
	if ( ++p == pe )
		goto _test_eof40;
case 40:
#line 1377 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
	if ( (*p) == 117 )
		goto st41;
	goto st0;
st41:
	if ( ++p == pe )
		goto _test_eof41;
case 41:
	if ( (*p) == 114 )
		goto st42;
	goto st0;
st42:
	if ( ++p == pe )
		goto _test_eof42;
case 42:
	switch( (*p) ) {
		case 43: goto tr56;
		case 45: goto tr56;
		case 48: goto tr57;
	}
	if ( (*p) > 53 ) {
		if ( 54 <= (*p) && (*p) <= 57 )
			goto tr59;
	} else if ( (*p) >= 49 )
		goto tr58;
	goto st0;
tr7:
#line 71 "lex/ragel_units.rl"
	{type_ = TYPE_UNKNOWN; cat_ = CAT_UNKNOWN;}
#line 62 "lex/ragel_units.rl"
	{pos_ = POSITION_ABS;}
	goto st43;
st43:
	if ( ++p == pe )
		goto _test_eof43;
case 43:
#line 1413 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
	if ( (*p) == 110 )
		goto st44;
	goto st0;
st44:
	if ( ++p == pe )
		goto _test_eof44;
case 44:
	if ( (*p) == 100 )
		goto st2;
	goto st0;
	}
	_test_eof2: cs = 2; goto _test_eof; 
	_test_eof3: cs = 3; goto _test_eof; 
	_test_eof45: cs = 45; goto _test_eof; 
	_test_eof46: cs = 46; goto _test_eof; 
	_test_eof47: cs = 47; goto _test_eof; 
	_test_eof4: cs = 4; goto _test_eof; 
	_test_eof48: cs = 48; goto _test_eof; 
	_test_eof5: cs = 5; goto _test_eof; 
	_test_eof49: cs = 49; goto _test_eof; 
	_test_eof50: cs = 50; goto _test_eof; 
	_test_eof51: cs = 51; goto _test_eof; 
	_test_eof6: cs = 6; goto _test_eof; 
	_test_eof7: cs = 7; goto _test_eof; 
	_test_eof52: cs = 52; goto _test_eof; 
	_test_eof53: cs = 53; goto _test_eof; 
	_test_eof8: cs = 8; goto _test_eof; 
	_test_eof54: cs = 54; goto _test_eof; 
	_test_eof55: cs = 55; goto _test_eof; 
	_test_eof9: cs = 9; goto _test_eof; 
	_test_eof56: cs = 56; goto _test_eof; 
	_test_eof10: cs = 10; goto _test_eof; 
	_test_eof57: cs = 57; goto _test_eof; 
	_test_eof58: cs = 58; goto _test_eof; 
	_test_eof11: cs = 11; goto _test_eof; 
	_test_eof12: cs = 12; goto _test_eof; 
	_test_eof59: cs = 59; goto _test_eof; 
	_test_eof60: cs = 60; goto _test_eof; 
	_test_eof13: cs = 13; goto _test_eof; 
	_test_eof61: cs = 61; goto _test_eof; 
	_test_eof62: cs = 62; goto _test_eof; 
	_test_eof14: cs = 14; goto _test_eof; 
	_test_eof63: cs = 63; goto _test_eof; 
	_test_eof15: cs = 15; goto _test_eof; 
	_test_eof16: cs = 16; goto _test_eof; 
	_test_eof17: cs = 17; goto _test_eof; 
	_test_eof18: cs = 18; goto _test_eof; 
	_test_eof19: cs = 19; goto _test_eof; 
	_test_eof20: cs = 20; goto _test_eof; 
	_test_eof64: cs = 64; goto _test_eof; 
	_test_eof65: cs = 65; goto _test_eof; 
	_test_eof66: cs = 66; goto _test_eof; 
	_test_eof21: cs = 21; goto _test_eof; 
	_test_eof67: cs = 67; goto _test_eof; 
	_test_eof22: cs = 22; goto _test_eof; 
	_test_eof68: cs = 68; goto _test_eof; 
	_test_eof23: cs = 23; goto _test_eof; 
	_test_eof69: cs = 69; goto _test_eof; 
	_test_eof24: cs = 24; goto _test_eof; 
	_test_eof25: cs = 25; goto _test_eof; 
	_test_eof26: cs = 26; goto _test_eof; 
	_test_eof70: cs = 70; goto _test_eof; 
	_test_eof27: cs = 27; goto _test_eof; 
	_test_eof71: cs = 71; goto _test_eof; 
	_test_eof72: cs = 72; goto _test_eof; 
	_test_eof28: cs = 28; goto _test_eof; 
	_test_eof29: cs = 29; goto _test_eof; 
	_test_eof30: cs = 30; goto _test_eof; 
	_test_eof73: cs = 73; goto _test_eof; 
	_test_eof31: cs = 31; goto _test_eof; 
	_test_eof74: cs = 74; goto _test_eof; 
	_test_eof75: cs = 75; goto _test_eof; 
	_test_eof32: cs = 32; goto _test_eof; 
	_test_eof33: cs = 33; goto _test_eof; 
	_test_eof76: cs = 76; goto _test_eof; 
	_test_eof77: cs = 77; goto _test_eof; 
	_test_eof78: cs = 78; goto _test_eof; 
	_test_eof79: cs = 79; goto _test_eof; 
	_test_eof80: cs = 80; goto _test_eof; 
	_test_eof34: cs = 34; goto _test_eof; 
	_test_eof35: cs = 35; goto _test_eof; 
	_test_eof81: cs = 81; goto _test_eof; 
	_test_eof82: cs = 82; goto _test_eof; 
	_test_eof36: cs = 36; goto _test_eof; 
	_test_eof37: cs = 37; goto _test_eof; 
	_test_eof83: cs = 83; goto _test_eof; 
	_test_eof38: cs = 38; goto _test_eof; 
	_test_eof39: cs = 39; goto _test_eof; 
	_test_eof84: cs = 84; goto _test_eof; 
	_test_eof85: cs = 85; goto _test_eof; 
	_test_eof86: cs = 86; goto _test_eof; 
	_test_eof87: cs = 87; goto _test_eof; 
	_test_eof88: cs = 88; goto _test_eof; 
	_test_eof40: cs = 40; goto _test_eof; 
	_test_eof41: cs = 41; goto _test_eof; 
	_test_eof42: cs = 42; goto _test_eof; 
	_test_eof43: cs = 43; goto _test_eof; 
	_test_eof44: cs = 44; goto _test_eof; 

	_test_eof: {}
	if ( p == eof )
	{
	switch ( cs ) {
	case 50: 
#line 72 "lex/ragel_units.rl"
	{cat_ = CAT_UNIT;}
	break;
	case 48: 
#line 19 "lex/ragel_numeric.rl"
	{
        cat_ = CAT_NUMBER;
        type_ = TYPE_FLOAT;
        num_.fval = num_.ival + num_.sign * (double(num_.inum)/num_.iden);
    }
#line 72 "lex/ragel_units.rl"
	{cat_ = CAT_UNIT;}
	break;
	case 81: 
	case 82: 
	case 83: 
	case 85: 
#line 25 "lex/ragel_music.rl"
	{cat_ = CAT_UNIT; type_ = TYPE_BPM;}
#line 72 "lex/ragel_units.rl"
	{cat_ = CAT_UNIT;}
	break;
	case 49: 
#line 9 "lex/ragel_units.rl"
	{type_ = TYPE_HZ;}
#line 72 "lex/ragel_units.rl"
	{cat_ = CAT_UNIT;}
	break;
	case 66: 
	case 67: 
#line 10 "lex/ragel_units.rl"
	{type_ = TYPE_SAMP;}
#line 72 "lex/ragel_units.rl"
	{cat_ = CAT_UNIT;}
	break;
	case 64: 
#line 11 "lex/ragel_units.rl"
	{type_ = TYPE_RADIAN;}
#line 72 "lex/ragel_units.rl"
	{cat_ = CAT_UNIT;}
	break;
	case 57: 
#line 12 "lex/ragel_units.rl"
	{type_ = TYPE_DEGREE;}
#line 72 "lex/ragel_units.rl"
	{cat_ = CAT_UNIT;}
	break;
	case 55: 
	case 56: 
#line 13 "lex/ragel_units.rl"
	{type_ = TYPE_DB;}
#line 72 "lex/ragel_units.rl"
	{cat_ = CAT_UNIT;}
	break;
	case 53: 
	case 54: 
#line 14 "lex/ragel_units.rl"
	{type_ = TYPE_DAY;}
#line 72 "lex/ragel_units.rl"
	{cat_ = CAT_UNIT;}
	break;
	case 58: 
	case 59: 
#line 15 "lex/ragel_units.rl"
	{type_ = TYPE_HOUR;}
#line 72 "lex/ragel_units.rl"
	{cat_ = CAT_UNIT;}
	break;
	case 60: 
	case 61: 
#line 16 "lex/ragel_units.rl"
	{type_ = TYPE_MIN;}
#line 72 "lex/ragel_units.rl"
	{cat_ = CAT_UNIT;}
	break;
	case 65: 
	case 68: 
#line 17 "lex/ragel_units.rl"
	{type_ = TYPE_SEC;}
#line 72 "lex/ragel_units.rl"
	{cat_ = CAT_UNIT;}
	break;
	case 62: 
	case 63: 
#line 18 "lex/ragel_units.rl"
	{type_ = TYPE_MSEC;}
#line 72 "lex/ragel_units.rl"
	{cat_ = CAT_UNIT;}
	break;
	case 51: 
	case 52: 
#line 19 "lex/ragel_units.rl"
	{type_ = TYPE_CENT;}
#line 72 "lex/ragel_units.rl"
	{cat_ = CAT_UNIT;}
	break;
	case 69: 
	case 70: 
#line 20 "lex/ragel_units.rl"
	{type_ = TYPE_SEMITONE;}
#line 72 "lex/ragel_units.rl"
	{cat_ = CAT_UNIT;}
	break;
	case 73: 
	case 74: 
	case 75: 
	case 76: 
#line 27 "lex/ragel_units.rl"
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
#line 72 "lex/ragel_units.rl"
	{cat_ = CAT_UNIT;}
	break;
	case 45: 
	case 77: 
	case 78: 
	case 79: 
#line 10 "lex/ragel_numeric.rl"
	{ num_.ival *= num_.sign; cat_ = CAT_NUMBER; type_ = TYPE_INT; }
#line 24 "lex/ragel_units.rl"
	{num_.fval = num_.ival;}
#line 72 "lex/ragel_units.rl"
	{cat_ = CAT_UNIT;}
	break;
	case 71: 
	case 72: 
#line 17 "lex/ragel_numeric.rl"
	{ num_.inum *= num_.sign; cat_ = CAT_NUMBER; type_ = TYPE_RATIO; }
#line 66 "lex/ragel_units.rl"
	{type_ = TYPE_RATIO;}
#line 72 "lex/ragel_units.rl"
	{cat_ = CAT_UNIT;}
	break;
	case 84: 
#line 19 "lex/ragel_numeric.rl"
	{
        cat_ = CAT_NUMBER;
        type_ = TYPE_FLOAT;
        num_.fval = num_.ival + num_.sign * (double(num_.inum)/num_.iden);
    }
#line 25 "lex/ragel_music.rl"
	{cat_ = CAT_UNIT; type_ = TYPE_BPM;}
#line 72 "lex/ragel_units.rl"
	{cat_ = CAT_UNIT;}
	break;
	case 46: 
#line 63 "lex/ragel_numeric.rl"
	{type_ = TYPE_PERCENT;}
#line 64 "lex/ragel_units.rl"
	{type_ = TYPE_PERCENT;}
#line 72 "lex/ragel_units.rl"
	{cat_ = CAT_UNIT;}
	break;
	case 47: 
#line 68 "lex/ragel_numeric.rl"
	{type_ = TYPE_PHASE;}
#line 65 "lex/ragel_units.rl"
	{type_ = TYPE_PHASE;}
#line 72 "lex/ragel_units.rl"
	{cat_ = CAT_UNIT;}
	break;
	case 80: 
	case 86: 
	case 87: 
	case 88: 
#line 10 "lex/ragel_numeric.rl"
	{ num_.ival *= num_.sign; cat_ = CAT_NUMBER; type_ = TYPE_INT; }
#line 24 "lex/ragel_units.rl"
	{num_.fval = num_.ival;}
#line 25 "lex/ragel_music.rl"
	{cat_ = CAT_UNIT; type_ = TYPE_BPM;}
#line 72 "lex/ragel_units.rl"
	{cat_ = CAT_UNIT;}
	break;
#line 1703 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
	}
	}

	_out: {}
	}

#line 72 "lex/parser_units.rl"

    const bool ok = cs >= 45;
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

}
}


