
#line 1 "lex/parser_numeric.rl"
# include "parser_numeric.h"

# include <cstdint>
# include <cstring>

namespace ceammc {
namespace parser {

static uint8_t xchar2digit(char c)
{
    switch (c) {
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
        return c - '0';
    case 'A':
    case 'B':
    case 'C':
    case 'D':
    case 'E':
    case 'F':
        return c - 'A' + 10;
    default:
        return c - 'a' + 10;
    }
}


#line 39 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_numeric.cpp"
static const int numeric_full_start = 1;
static const int numeric_full_first_final = 14;
static const int numeric_full_error = 0;

static const int numeric_full_en_main = 1;


#line 41 "lex/parser_numeric.rl"


NumericFullMatch::NumericFullMatch()
{
    reset();
}

void NumericFullMatch::reset()
{
    res_ = {};
}

bool NumericFullMatch::parse(const Atom& a)
{
    reset();

    if (a.isSymbol())
        return parse(a.asT<t_symbol*>()->s_name);
    else if(a.isFloat()) {
        if(a.isInteger()) {
            res_.t = TYPE_INT;
            res_.f = a.asT<t_float>();
            res_.i0 = res_.f;
        } else {
            res_.t = TYPE_FLOAT;
            res_.f = a.asT<t_float>();
        }

        return true;
    } else
        return false;
}

bool NumericFullMatch::parse(const char* str)
{
    const auto len = strlen(str);
    if (len == 0)
        return false;

    const char* p = str;
    const char* pe = p + len;
    const char* eof = pe;
    int cat_ = 0;
    AtomType type_;
    NumericRagelData num_;

    reset();

    
#line 97 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_numeric.cpp"
	{
	cs = numeric_full_start;
	}

#line 90 "lex/parser_numeric.rl"
    
#line 104 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_numeric.cpp"
	{
	if ( p == pe )
		goto _test_eof;
	switch ( cs )
	{
case 1:
	switch( (*p) ) {
		case 43: goto tr0;
		case 45: goto tr0;
		case 48: goto tr2;
		case 105: goto st10;
	}
	if ( 49 <= (*p) && (*p) <= 57 )
		goto tr3;
	goto st0;
st0:
cs = 0;
	goto _out;
tr0:
#line 4 "lex/ragel_numeric.rl"
	{ num_.sign = ((*p)=='-') ? -1 : 1; }
	goto st2;
st2:
	if ( ++p == pe )
		goto _test_eof2;
case 2:
#line 131 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_numeric.cpp"
	switch( (*p) ) {
		case 48: goto tr5;
		case 105: goto st10;
	}
	if ( 49 <= (*p) && (*p) <= 57 )
		goto tr3;
	goto st0;
tr5:
#line 9 "lex/ragel_numeric.rl"
	{ (num_.ival *= 10) += ((*p)-'0'); }
#line 15 "lex/ragel_numeric.rl"
	{ (num_.inum *= 10) += ((*p)-'0'); }
	goto st14;
st14:
	if ( ++p == pe )
		goto _test_eof14;
case 14:
#line 149 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_numeric.cpp"
	switch( (*p) ) {
		case 37: goto tr19;
		case 42: goto tr20;
		case 46: goto tr21;
		case 47: goto st9;
		case 95: goto tr23;
		case 112: goto tr24;
	}
	goto st0;
tr19:
#line 10 "lex/ragel_numeric.rl"
	{ num_.ival *= num_.sign; cat_ = CAT_NUMBER; type_ = TYPE_INT; }
#line 60 "lex/ragel_numeric.rl"
	{num_.fval = num_.ival;}
	goto st15;
tr25:
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
#line 177 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_numeric.cpp"
	goto st0;
tr20:
#line 10 "lex/ragel_numeric.rl"
	{ num_.ival *= num_.sign; cat_ = CAT_NUMBER; type_ = TYPE_INT; }
#line 65 "lex/ragel_numeric.rl"
	{num_.fval = num_.ival;}
	goto st16;
tr26:
#line 19 "lex/ragel_numeric.rl"
	{
        cat_ = CAT_NUMBER;
        type_ = TYPE_FLOAT;
        num_.fval = num_.ival + num_.sign * (double(num_.inum)/num_.iden);
    }
	goto st16;
st16:
	if ( ++p == pe )
		goto _test_eof16;
case 16:
#line 197 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_numeric.cpp"
	goto st0;
tr21:
#line 10 "lex/ragel_numeric.rl"
	{ num_.ival *= num_.sign; cat_ = CAT_NUMBER; type_ = TYPE_INT; }
#line 54 "lex/ragel_numeric.rl"
	{num_.inum = 0; num_.iden = 1;}
	goto st3;
st3:
	if ( ++p == pe )
		goto _test_eof3;
case 3:
#line 209 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_numeric.cpp"
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr6;
	goto st0;
tr6:
#line 46 "lex/ragel_numeric.rl"
	{(num_.inum *= 10) += ((*p) - '0'); num_.iden *= 10;}
	goto st17;
st17:
	if ( ++p == pe )
		goto _test_eof17;
case 17:
#line 221 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_numeric.cpp"
	switch( (*p) ) {
		case 37: goto tr25;
		case 42: goto tr26;
		case 95: goto tr27;
		case 112: goto tr28;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr6;
	goto st0;
tr23:
#line 10 "lex/ragel_numeric.rl"
	{ num_.ival *= num_.sign; cat_ = CAT_NUMBER; type_ = TYPE_INT; }
#line 60 "lex/ragel_numeric.rl"
	{num_.fval = num_.ival;}
#line 65 "lex/ragel_numeric.rl"
	{num_.fval = num_.ival;}
	goto st4;
tr27:
#line 19 "lex/ragel_numeric.rl"
	{
        cat_ = CAT_NUMBER;
        type_ = TYPE_FLOAT;
        num_.fval = num_.ival + num_.sign * (double(num_.inum)/num_.iden);
    }
	goto st4;
st4:
	if ( ++p == pe )
		goto _test_eof4;
case 4:
#line 251 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_numeric.cpp"
	switch( (*p) ) {
		case 37: goto st15;
		case 112: goto st5;
	}
	goto st0;
tr24:
#line 10 "lex/ragel_numeric.rl"
	{ num_.ival *= num_.sign; cat_ = CAT_NUMBER; type_ = TYPE_INT; }
#line 65 "lex/ragel_numeric.rl"
	{num_.fval = num_.ival;}
	goto st5;
tr28:
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
#line 275 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_numeric.cpp"
	if ( (*p) == 104 )
		goto st6;
	goto st0;
st6:
	if ( ++p == pe )
		goto _test_eof6;
case 6:
	if ( (*p) == 97 )
		goto st7;
	goto st0;
st7:
	if ( ++p == pe )
		goto _test_eof7;
case 7:
	if ( (*p) == 115 )
		goto st8;
	goto st0;
st8:
	if ( ++p == pe )
		goto _test_eof8;
case 8:
	if ( (*p) == 101 )
		goto st16;
	goto st0;
st9:
	if ( ++p == pe )
		goto _test_eof9;
case 9:
	if ( (*p) == 48 )
		goto tr13;
	if ( 49 <= (*p) && (*p) <= 57 )
		goto tr14;
	goto st0;
tr13:
#line 16 "lex/ragel_numeric.rl"
	{ (num_.iden *= 10) += ((*p)-'0'); }
	goto st18;
st18:
	if ( ++p == pe )
		goto _test_eof18;
case 18:
#line 317 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_numeric.cpp"
	goto st0;
tr14:
#line 16 "lex/ragel_numeric.rl"
	{ (num_.iden *= 10) += ((*p)-'0'); }
	goto st19;
st19:
	if ( ++p == pe )
		goto _test_eof19;
case 19:
#line 327 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_numeric.cpp"
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr14;
	goto st0;
tr3:
#line 9 "lex/ragel_numeric.rl"
	{ (num_.ival *= 10) += ((*p)-'0'); }
#line 15 "lex/ragel_numeric.rl"
	{ (num_.inum *= 10) += ((*p)-'0'); }
	goto st20;
st20:
	if ( ++p == pe )
		goto _test_eof20;
case 20:
#line 341 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_numeric.cpp"
	switch( (*p) ) {
		case 37: goto tr19;
		case 42: goto tr20;
		case 46: goto tr21;
		case 47: goto st9;
		case 95: goto tr23;
		case 112: goto tr24;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr3;
	goto st0;
st10:
	if ( ++p == pe )
		goto _test_eof10;
case 10:
	if ( (*p) == 110 )
		goto st11;
	goto st0;
st11:
	if ( ++p == pe )
		goto _test_eof11;
case 11:
	if ( (*p) == 102 )
		goto st21;
	goto st0;
st21:
	if ( ++p == pe )
		goto _test_eof21;
case 21:
	goto st0;
tr2:
#line 9 "lex/ragel_numeric.rl"
	{ (num_.ival *= 10) += ((*p)-'0'); }
#line 15 "lex/ragel_numeric.rl"
	{ (num_.inum *= 10) += ((*p)-'0'); }
	goto st22;
st22:
	if ( ++p == pe )
		goto _test_eof22;
case 22:
#line 382 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_numeric.cpp"
	switch( (*p) ) {
		case 37: goto tr19;
		case 42: goto tr20;
		case 46: goto tr21;
		case 47: goto st9;
		case 95: goto tr23;
		case 98: goto st12;
		case 112: goto tr24;
		case 120: goto st13;
	}
	goto st0;
st12:
	if ( ++p == pe )
		goto _test_eof12;
case 12:
	if ( 48 <= (*p) && (*p) <= 49 )
		goto tr17;
	goto st0;
tr17:
#line 6 "lex/ragel_numeric.rl"
	{ (num_.ival <<= 1) |= ((*p)=='1'); }
	goto st23;
st23:
	if ( ++p == pe )
		goto _test_eof23;
case 23:
#line 409 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_numeric.cpp"
	if ( 48 <= (*p) && (*p) <= 49 )
		goto tr17;
	goto st0;
st13:
	if ( ++p == pe )
		goto _test_eof13;
case 13:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr18;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto tr18;
	} else
		goto tr18;
	goto st0;
tr18:
#line 12 "lex/ragel_numeric.rl"
	{ (num_.ival <<= 4) |= xchar2digit((*p)); }
	goto st24;
st24:
	if ( ++p == pe )
		goto _test_eof24;
case 24:
#line 434 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_numeric.cpp"
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr18;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto tr18;
	} else
		goto tr18;
	goto st0;
	}
	_test_eof2: cs = 2; goto _test_eof; 
	_test_eof14: cs = 14; goto _test_eof; 
	_test_eof15: cs = 15; goto _test_eof; 
	_test_eof16: cs = 16; goto _test_eof; 
	_test_eof3: cs = 3; goto _test_eof; 
	_test_eof17: cs = 17; goto _test_eof; 
	_test_eof4: cs = 4; goto _test_eof; 
	_test_eof5: cs = 5; goto _test_eof; 
	_test_eof6: cs = 6; goto _test_eof; 
	_test_eof7: cs = 7; goto _test_eof; 
	_test_eof8: cs = 8; goto _test_eof; 
	_test_eof9: cs = 9; goto _test_eof; 
	_test_eof18: cs = 18; goto _test_eof; 
	_test_eof19: cs = 19; goto _test_eof; 
	_test_eof20: cs = 20; goto _test_eof; 
	_test_eof10: cs = 10; goto _test_eof; 
	_test_eof11: cs = 11; goto _test_eof; 
	_test_eof21: cs = 21; goto _test_eof; 
	_test_eof22: cs = 22; goto _test_eof; 
	_test_eof12: cs = 12; goto _test_eof; 
	_test_eof23: cs = 23; goto _test_eof; 
	_test_eof13: cs = 13; goto _test_eof; 
	_test_eof24: cs = 24; goto _test_eof; 

	_test_eof: {}
	if ( p == eof )
	{
	switch ( cs ) {
	case 23: 
#line 7 "lex/ragel_numeric.rl"
	{ cat_ = CAT_NUMBER; type_ = TYPE_BIN; }
	break;
	case 14: 
	case 20: 
	case 22: 
#line 10 "lex/ragel_numeric.rl"
	{ num_.ival *= num_.sign; cat_ = CAT_NUMBER; type_ = TYPE_INT; }
	break;
	case 24: 
#line 13 "lex/ragel_numeric.rl"
	{ cat_ = CAT_NUMBER; type_ = TYPE_HEX; }
	break;
	case 18: 
	case 19: 
#line 17 "lex/ragel_numeric.rl"
	{ num_.inum *= num_.sign; cat_ = CAT_NUMBER; type_ = TYPE_RATIO; }
	break;
	case 17: 
#line 19 "lex/ragel_numeric.rl"
	{
        cat_ = CAT_NUMBER;
        type_ = TYPE_FLOAT;
        num_.fval = num_.ival + num_.sign * (double(num_.inum)/num_.iden);
    }
	break;
	case 21: 
#line 25 "lex/ragel_numeric.rl"
	{
        cat_ = CAT_NUMBER;
        type_ = TYPE_INF;
    }
	break;
	case 15: 
#line 63 "lex/ragel_numeric.rl"
	{type_ = TYPE_PERCENT;}
	break;
	case 16: 
#line 68 "lex/ragel_numeric.rl"
	{type_ = TYPE_PHASE;}
	break;
#line 515 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_numeric.cpp"
	}
	}

	_out: {}
	}

#line 91 "lex/parser_numeric.rl"

    switch(type_) {
    case TYPE_PHASE:
    case TYPE_PERCENT:
    case TYPE_FLOAT:
        res_.f = num_.fval;
        break;
    case TYPE_INT:
    case TYPE_HEX:
    case TYPE_BIN:
        res_.i0 = num_.ival;
        break;
    case TYPE_RATIO:
        res_.i0 = num_.inum;
        res_.i1 = num_.iden;
        break;
    case TYPE_INF:
        res_.f = std::numeric_limits<typeof(res_.f)>::infinity();
        break;
    default:
        break;
    }

    res_.t = type_;

    return cs >= 14;
}

bool NumericFullMatch::getBin(t_int& b) const
{
    if(!isBin()) return false;
    b = res_.i0;
    return true;
}

bool NumericFullMatch::getInt(t_int& i) const
{
    if(!isInt()) return false;
    i = res_.i0;
    return true;
}

bool NumericFullMatch::getHex(t_int& x) const
{
    if(!isHex()) return false;
    x = res_.i0;
    return true;
}

bool NumericFullMatch::getFloat(t_float& f) const
{
    if(!isFloat()) return false;
    f = res_.f;
    return true;
}

bool NumericFullMatch::getRatio(t_int& num, t_int& den) const
{
    if(!isRatio()) return false;
    num = res_.i0;
    den = res_.i1;
    return true;
}

bool NumericFullMatch::parseAll(const AtomListView& lv, SmallFVec& out)
{
    return parseSome(lv, out) == lv.size();
}

size_t NumericFullMatch::parseSome(const AtomListView& lv, SmallFVec& out)
{
    const size_t N = lv.size();

    for (size_t i = 0; i < N; i++) {
        const auto& a = lv[i];
        if (!parse(a))
            return i;

        out.push_back(asFloat());
    }

    return N;
}


#line 608 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_numeric.cpp"
static const int numeric_some_start = 1;
static const int numeric_some_first_final = 14;
static const int numeric_some_error = 0;

static const int numeric_some_en_main = 1;


#line 186 "lex/parser_numeric.rl"


NumericMatchSome::NumericMatchSome()
{
    reset();
}

void NumericMatchSome::reset()
{
    
#line 627 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_numeric.cpp"
	{
	cs = numeric_some_start;
	}

#line 196 "lex/parser_numeric.rl"
    res_.clear();
}

bool NumericMatchSome::parseSingle(const char* str, const char*& endptr)
{
    const auto len = strlen(str);
    if (len == 0)
        return false;

    const char* p = str;
    const char* pe = p + len;
    const char* eof = pe;
    int cat_ = 0;
    AtomType type_;
    NumericRagelData num_;

    bool ok = false;

    
#line 652 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_numeric.cpp"
	{
	cs = numeric_some_start;
	}

#line 215 "lex/parser_numeric.rl"
    
#line 659 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_numeric.cpp"
	{
	if ( p == pe )
		goto _test_eof;
	switch ( cs )
	{
st1:
	if ( ++p == pe )
		goto _test_eof1;
case 1:
	switch( (*p) ) {
		case 9: goto st1;
		case 32: goto st1;
		case 43: goto tr2;
		case 45: goto tr2;
		case 48: goto tr3;
		case 105: goto st10;
	}
	if ( 49 <= (*p) && (*p) <= 57 )
		goto tr4;
	goto st0;
st0:
cs = 0;
	goto _out;
tr2:
#line 4 "lex/ragel_numeric.rl"
	{ num_.sign = ((*p)=='-') ? -1 : 1; }
	goto st2;
st2:
	if ( ++p == pe )
		goto _test_eof2;
case 2:
#line 691 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_numeric.cpp"
	switch( (*p) ) {
		case 48: goto tr6;
		case 105: goto st10;
	}
	if ( 49 <= (*p) && (*p) <= 57 )
		goto tr4;
	goto st0;
tr6:
#line 9 "lex/ragel_numeric.rl"
	{ (num_.ival *= 10) += ((*p)-'0'); }
#line 15 "lex/ragel_numeric.rl"
	{ (num_.inum *= 10) += ((*p)-'0'); }
	goto st14;
st14:
	if ( ++p == pe )
		goto _test_eof14;
case 14:
#line 709 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_numeric.cpp"
	switch( (*p) ) {
		case 9: goto tr20;
		case 32: goto tr20;
		case 37: goto tr21;
		case 42: goto tr22;
		case 46: goto tr23;
		case 47: goto st9;
		case 95: goto tr25;
		case 112: goto tr26;
	}
	goto st0;
tr20:
#line 10 "lex/ragel_numeric.rl"
	{ num_.ival *= num_.sign; cat_ = CAT_NUMBER; type_ = TYPE_INT; }
	goto st15;
tr29:
#line 63 "lex/ragel_numeric.rl"
	{type_ = TYPE_PERCENT;}
	goto st15;
tr30:
#line 68 "lex/ragel_numeric.rl"
	{type_ = TYPE_PHASE;}
	goto st15;
tr31:
#line 19 "lex/ragel_numeric.rl"
	{
        cat_ = CAT_NUMBER;
        type_ = TYPE_FLOAT;
        num_.fval = num_.ival + num_.sign * (double(num_.inum)/num_.iden);
    }
	goto st15;
tr36:
#line 17 "lex/ragel_numeric.rl"
	{ num_.inum *= num_.sign; cat_ = CAT_NUMBER; type_ = TYPE_RATIO; }
	goto st15;
tr37:
#line 25 "lex/ragel_numeric.rl"
	{
        cat_ = CAT_NUMBER;
        type_ = TYPE_INF;
    }
	goto st15;
tr40:
#line 7 "lex/ragel_numeric.rl"
	{ cat_ = CAT_NUMBER; type_ = TYPE_BIN; }
	goto st15;
tr41:
#line 13 "lex/ragel_numeric.rl"
	{ cat_ = CAT_NUMBER; type_ = TYPE_HEX; }
	goto st15;
st15:
	if ( ++p == pe )
		goto _test_eof15;
case 15:
#line 179 "lex/parser_numeric.rl"
	{if((*p)!=' '&&(*p)!='\t'){p--;{p++; cs = 15; goto _out;}}}
#line 766 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_numeric.cpp"
	switch( (*p) ) {
		case 9: goto st15;
		case 32: goto st15;
	}
	goto st16;
st16:
	if ( ++p == pe )
		goto _test_eof16;
case 16:
	goto st16;
tr21:
#line 10 "lex/ragel_numeric.rl"
	{ num_.ival *= num_.sign; cat_ = CAT_NUMBER; type_ = TYPE_INT; }
#line 60 "lex/ragel_numeric.rl"
	{num_.fval = num_.ival;}
	goto st17;
tr32:
#line 19 "lex/ragel_numeric.rl"
	{
        cat_ = CAT_NUMBER;
        type_ = TYPE_FLOAT;
        num_.fval = num_.ival + num_.sign * (double(num_.inum)/num_.iden);
    }
	goto st17;
st17:
	if ( ++p == pe )
		goto _test_eof17;
case 17:
#line 795 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_numeric.cpp"
	switch( (*p) ) {
		case 9: goto tr29;
		case 32: goto tr29;
	}
	goto st0;
tr22:
#line 10 "lex/ragel_numeric.rl"
	{ num_.ival *= num_.sign; cat_ = CAT_NUMBER; type_ = TYPE_INT; }
#line 65 "lex/ragel_numeric.rl"
	{num_.fval = num_.ival;}
	goto st18;
tr33:
#line 19 "lex/ragel_numeric.rl"
	{
        cat_ = CAT_NUMBER;
        type_ = TYPE_FLOAT;
        num_.fval = num_.ival + num_.sign * (double(num_.inum)/num_.iden);
    }
	goto st18;
st18:
	if ( ++p == pe )
		goto _test_eof18;
case 18:
#line 819 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_numeric.cpp"
	switch( (*p) ) {
		case 9: goto tr30;
		case 32: goto tr30;
	}
	goto st0;
tr23:
#line 10 "lex/ragel_numeric.rl"
	{ num_.ival *= num_.sign; cat_ = CAT_NUMBER; type_ = TYPE_INT; }
#line 54 "lex/ragel_numeric.rl"
	{num_.inum = 0; num_.iden = 1;}
	goto st3;
st3:
	if ( ++p == pe )
		goto _test_eof3;
case 3:
#line 835 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_numeric.cpp"
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr7;
	goto st0;
tr7:
#line 46 "lex/ragel_numeric.rl"
	{(num_.inum *= 10) += ((*p) - '0'); num_.iden *= 10;}
	goto st19;
st19:
	if ( ++p == pe )
		goto _test_eof19;
case 19:
#line 847 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_numeric.cpp"
	switch( (*p) ) {
		case 9: goto tr31;
		case 32: goto tr31;
		case 37: goto tr32;
		case 42: goto tr33;
		case 95: goto tr34;
		case 112: goto tr35;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr7;
	goto st0;
tr25:
#line 10 "lex/ragel_numeric.rl"
	{ num_.ival *= num_.sign; cat_ = CAT_NUMBER; type_ = TYPE_INT; }
#line 60 "lex/ragel_numeric.rl"
	{num_.fval = num_.ival;}
#line 65 "lex/ragel_numeric.rl"
	{num_.fval = num_.ival;}
	goto st4;
tr34:
#line 19 "lex/ragel_numeric.rl"
	{
        cat_ = CAT_NUMBER;
        type_ = TYPE_FLOAT;
        num_.fval = num_.ival + num_.sign * (double(num_.inum)/num_.iden);
    }
	goto st4;
st4:
	if ( ++p == pe )
		goto _test_eof4;
case 4:
#line 879 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_numeric.cpp"
	switch( (*p) ) {
		case 37: goto st17;
		case 112: goto st5;
	}
	goto st0;
tr26:
#line 10 "lex/ragel_numeric.rl"
	{ num_.ival *= num_.sign; cat_ = CAT_NUMBER; type_ = TYPE_INT; }
#line 65 "lex/ragel_numeric.rl"
	{num_.fval = num_.ival;}
	goto st5;
tr35:
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
#line 903 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_numeric.cpp"
	if ( (*p) == 104 )
		goto st6;
	goto st0;
st6:
	if ( ++p == pe )
		goto _test_eof6;
case 6:
	if ( (*p) == 97 )
		goto st7;
	goto st0;
st7:
	if ( ++p == pe )
		goto _test_eof7;
case 7:
	if ( (*p) == 115 )
		goto st8;
	goto st0;
st8:
	if ( ++p == pe )
		goto _test_eof8;
case 8:
	if ( (*p) == 101 )
		goto st18;
	goto st0;
st9:
	if ( ++p == pe )
		goto _test_eof9;
case 9:
	if ( (*p) == 48 )
		goto tr14;
	if ( 49 <= (*p) && (*p) <= 57 )
		goto tr15;
	goto st0;
tr14:
#line 16 "lex/ragel_numeric.rl"
	{ (num_.iden *= 10) += ((*p)-'0'); }
	goto st20;
st20:
	if ( ++p == pe )
		goto _test_eof20;
case 20:
#line 945 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_numeric.cpp"
	switch( (*p) ) {
		case 9: goto tr36;
		case 32: goto tr36;
	}
	goto st0;
tr15:
#line 16 "lex/ragel_numeric.rl"
	{ (num_.iden *= 10) += ((*p)-'0'); }
	goto st21;
st21:
	if ( ++p == pe )
		goto _test_eof21;
case 21:
#line 959 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_numeric.cpp"
	switch( (*p) ) {
		case 9: goto tr36;
		case 32: goto tr36;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr15;
	goto st0;
tr4:
#line 9 "lex/ragel_numeric.rl"
	{ (num_.ival *= 10) += ((*p)-'0'); }
#line 15 "lex/ragel_numeric.rl"
	{ (num_.inum *= 10) += ((*p)-'0'); }
	goto st22;
st22:
	if ( ++p == pe )
		goto _test_eof22;
case 22:
#line 977 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_numeric.cpp"
	switch( (*p) ) {
		case 9: goto tr20;
		case 32: goto tr20;
		case 37: goto tr21;
		case 42: goto tr22;
		case 46: goto tr23;
		case 47: goto st9;
		case 95: goto tr25;
		case 112: goto tr26;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr4;
	goto st0;
st10:
	if ( ++p == pe )
		goto _test_eof10;
case 10:
	if ( (*p) == 110 )
		goto st11;
	goto st0;
st11:
	if ( ++p == pe )
		goto _test_eof11;
case 11:
	if ( (*p) == 102 )
		goto st23;
	goto st0;
st23:
	if ( ++p == pe )
		goto _test_eof23;
case 23:
	switch( (*p) ) {
		case 9: goto tr37;
		case 32: goto tr37;
	}
	goto st0;
tr3:
#line 9 "lex/ragel_numeric.rl"
	{ (num_.ival *= 10) += ((*p)-'0'); }
#line 15 "lex/ragel_numeric.rl"
	{ (num_.inum *= 10) += ((*p)-'0'); }
	goto st24;
st24:
	if ( ++p == pe )
		goto _test_eof24;
case 24:
#line 1024 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_numeric.cpp"
	switch( (*p) ) {
		case 9: goto tr20;
		case 32: goto tr20;
		case 37: goto tr21;
		case 42: goto tr22;
		case 46: goto tr23;
		case 47: goto st9;
		case 95: goto tr25;
		case 98: goto st12;
		case 112: goto tr26;
		case 120: goto st13;
	}
	goto st0;
st12:
	if ( ++p == pe )
		goto _test_eof12;
case 12:
	if ( 48 <= (*p) && (*p) <= 49 )
		goto tr18;
	goto st0;
tr18:
#line 6 "lex/ragel_numeric.rl"
	{ (num_.ival <<= 1) |= ((*p)=='1'); }
	goto st25;
st25:
	if ( ++p == pe )
		goto _test_eof25;
case 25:
#line 1053 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_numeric.cpp"
	switch( (*p) ) {
		case 9: goto tr40;
		case 32: goto tr40;
	}
	if ( 48 <= (*p) && (*p) <= 49 )
		goto tr18;
	goto st0;
st13:
	if ( ++p == pe )
		goto _test_eof13;
case 13:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr19;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto tr19;
	} else
		goto tr19;
	goto st0;
tr19:
#line 12 "lex/ragel_numeric.rl"
	{ (num_.ival <<= 4) |= xchar2digit((*p)); }
	goto st26;
st26:
	if ( ++p == pe )
		goto _test_eof26;
case 26:
#line 1082 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_numeric.cpp"
	switch( (*p) ) {
		case 9: goto tr41;
		case 32: goto tr41;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr19;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto tr19;
	} else
		goto tr19;
	goto st0;
	}
	_test_eof1: cs = 1; goto _test_eof; 
	_test_eof2: cs = 2; goto _test_eof; 
	_test_eof14: cs = 14; goto _test_eof; 
	_test_eof15: cs = 15; goto _test_eof; 
	_test_eof16: cs = 16; goto _test_eof; 
	_test_eof17: cs = 17; goto _test_eof; 
	_test_eof18: cs = 18; goto _test_eof; 
	_test_eof3: cs = 3; goto _test_eof; 
	_test_eof19: cs = 19; goto _test_eof; 
	_test_eof4: cs = 4; goto _test_eof; 
	_test_eof5: cs = 5; goto _test_eof; 
	_test_eof6: cs = 6; goto _test_eof; 
	_test_eof7: cs = 7; goto _test_eof; 
	_test_eof8: cs = 8; goto _test_eof; 
	_test_eof9: cs = 9; goto _test_eof; 
	_test_eof20: cs = 20; goto _test_eof; 
	_test_eof21: cs = 21; goto _test_eof; 
	_test_eof22: cs = 22; goto _test_eof; 
	_test_eof10: cs = 10; goto _test_eof; 
	_test_eof11: cs = 11; goto _test_eof; 
	_test_eof23: cs = 23; goto _test_eof; 
	_test_eof24: cs = 24; goto _test_eof; 
	_test_eof12: cs = 12; goto _test_eof; 
	_test_eof25: cs = 25; goto _test_eof; 
	_test_eof13: cs = 13; goto _test_eof; 
	_test_eof26: cs = 26; goto _test_eof; 

	_test_eof: {}
	if ( p == eof )
	{
	switch ( cs ) {
	case 25: 
#line 7 "lex/ragel_numeric.rl"
	{ cat_ = CAT_NUMBER; type_ = TYPE_BIN; }
	break;
	case 14: 
	case 22: 
	case 24: 
#line 10 "lex/ragel_numeric.rl"
	{ num_.ival *= num_.sign; cat_ = CAT_NUMBER; type_ = TYPE_INT; }
	break;
	case 26: 
#line 13 "lex/ragel_numeric.rl"
	{ cat_ = CAT_NUMBER; type_ = TYPE_HEX; }
	break;
	case 20: 
	case 21: 
#line 17 "lex/ragel_numeric.rl"
	{ num_.inum *= num_.sign; cat_ = CAT_NUMBER; type_ = TYPE_RATIO; }
	break;
	case 19: 
#line 19 "lex/ragel_numeric.rl"
	{
        cat_ = CAT_NUMBER;
        type_ = TYPE_FLOAT;
        num_.fval = num_.ival + num_.sign * (double(num_.inum)/num_.iden);
    }
	break;
	case 23: 
#line 25 "lex/ragel_numeric.rl"
	{
        cat_ = CAT_NUMBER;
        type_ = TYPE_INF;
    }
	break;
	case 17: 
#line 63 "lex/ragel_numeric.rl"
	{type_ = TYPE_PERCENT;}
	break;
	case 18: 
#line 68 "lex/ragel_numeric.rl"
	{type_ = TYPE_PHASE;}
	break;
#line 1170 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_numeric.cpp"
	}
	}

	_out: {}
	}

#line 216 "lex/parser_numeric.rl"

    ok = cs >= 14;

    if(ok) {
        switch(type_) {
        case TYPE_PHASE:
        case TYPE_PERCENT:
        case TYPE_FLOAT:
            res_.push_back({});
            res_.back().f = num_.fval;
            break;
        case TYPE_INT:
        case TYPE_HEX:
        case TYPE_BIN:
            res_.push_back({});
            res_.back().i0 = num_.ival;
            break;
        case TYPE_RATIO:
            res_.push_back({});
            res_.back().i0 = num_.inum;
            res_.back().i1 = num_.iden;
            break;
        case TYPE_INF:
            res_.push_back({});
            res_.back().f = std::numeric_limits<typeof(res_[0].f)>::infinity();
            break;
        default:
            return false;
            break;
        }

        res_.back().t = type_;
    }

    endptr = p;
    return ok;
}

size_t NumericMatchSome::parseMany(const char *str, const char *&endptr)
{
    reset();

    auto pstr = str;
    const char* end = nullptr;
    while(parseSingle(pstr, end))
        pstr = end;

    endptr = end;
    return res_.size();
}

bool NumericMatchSome::getBinAt(size_t idx, t_int& b) const
{
    if(!isBinAt(idx)) return false;
    b = res_[idx].i0;
    return true;
}

bool NumericMatchSome::getIntAt(size_t idx, t_int& i) const
{
    if(!isIntAt(idx)) return false;
    i = res_[idx].i0;
    return true;
}

bool NumericMatchSome::getHexAt(size_t idx, t_int& x) const
{
    if(!isHexAt(idx)) return false;
    x = res_[idx].i0;
    return true;
}

bool NumericMatchSome::getFloatAt(size_t idx, t_float& f) const
{
    if(!isFloatAt(idx)) return false;
    f = res_[idx].f;
    return true;
}

bool NumericMatchSome::getRatioAt(size_t idx, t_int& num, t_int& den) const
{
    if(!isRatioAt(idx)) return false;
    num = res_[idx].i0;
    den = res_[idx].i1;
    return true;
}

double NumericMatchSome::asFloatAt(size_t idx, double def) const
{
    if(idx >= res_.size())
        return def;

    auto& n = res_[idx];
    return n.asFloat(def);
}

}
}


