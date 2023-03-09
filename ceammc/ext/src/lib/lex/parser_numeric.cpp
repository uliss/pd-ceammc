
#line 1 "lex/parser_numeric.rl"
# include "parser_numeric.h"

# include <cstdint>
# include <cstring>
# include <limits>

namespace ceammc {
namespace parser {


#line 14 "lex/parser_numeric.cpp"
static const int numeric_full_start = 1;
static const int numeric_full_first_final = 14;
static const int numeric_full_error = 0;

static const int numeric_full_en_main = 1;


#line 16 "lex/parser_numeric.rl"


NumericFullMatch::NumericFullMatch()
{
    reset();
}

void NumericFullMatch::reset()
{
    res_ = {};
}

bool NumericFullMatch::parseAs(const Atom& a, AtomType t)
{
    const bool ok = parse(a);
    return ok && (res_.t == t || res_.t == TYPE_INT || res_.t == TYPE_FLOAT);
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

    DECLARE_RAGEL_COMMON_VARS;
    DECLARE_RAGEL_NUMERIC_VARS;

    reset();

    
#line 78 "lex/parser_numeric.cpp"
	{
	cs = numeric_full_start;
	}

#line 71 "lex/parser_numeric.rl"
    
#line 85 "lex/parser_numeric.cpp"
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
#line 21 "lex/ragel_numeric.rl"
	{ ragel_num.sign = ((*p)=='-') ? -1 : 1; }
	goto st2;
st2:
	if ( ++p == pe )
		goto _test_eof2;
case 2:
#line 112 "lex/parser_numeric.cpp"
	switch( (*p) ) {
		case 48: goto tr5;
		case 105: goto st10;
	}
	if ( 49 <= (*p) && (*p) <= 57 )
		goto tr3;
	goto st0;
tr5:
#line 29 "lex/ragel_numeric.rl"
	{ (ragel_num.vint *= 10) += ((*p)-'0'); }
#line 42 "lex/ragel_numeric.rl"
	{ (ragel_num.ratio.num *= 10) += ((*p)-'0'); }
	goto st14;
st14:
	if ( ++p == pe )
		goto _test_eof14;
case 14:
#line 130 "lex/parser_numeric.cpp"
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
#line 30 "lex/ragel_numeric.rl"
	{
        ragel_num.vint *= ragel_num.sign;
        ragel_type = TYPE_INT;
        ragel_cat  = CAT_NUMBER;
    }
#line 108 "lex/ragel_numeric.rl"
	{ragel_num.vdouble = ragel_num.vint;}
	goto st15;
tr25:
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
#line 163 "lex/parser_numeric.cpp"
	goto st0;
tr20:
#line 30 "lex/ragel_numeric.rl"
	{
        ragel_num.vint *= ragel_num.sign;
        ragel_type = TYPE_INT;
        ragel_cat  = CAT_NUMBER;
    }
#line 113 "lex/ragel_numeric.rl"
	{ragel_num.vdouble = ragel_num.vint;}
	goto st16;
tr26:
#line 58 "lex/ragel_numeric.rl"
	{
        const auto frac = double(ragel_num.ratio.num) / ragel_num.ratio.den;
        ragel_num.vdouble = ragel_num.vint + ragel_num.sign * frac;
        ragel_type = TYPE_FLOAT;
        ragel_cat  = CAT_NUMBER;
    }
	goto st16;
st16:
	if ( ++p == pe )
		goto _test_eof16;
case 16:
#line 188 "lex/parser_numeric.cpp"
	goto st0;
tr21:
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
#line 207 "lex/parser_numeric.cpp"
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr6;
	goto st0;
tr6:
#line 54 "lex/ragel_numeric.rl"
	{
        (ragel_num.ratio.num *= 10) += ((*p) - '0');
        ragel_num.ratio.den *= 10;
    }
	goto st17;
st17:
	if ( ++p == pe )
		goto _test_eof17;
case 17:
#line 222 "lex/parser_numeric.cpp"
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
#line 30 "lex/ragel_numeric.rl"
	{
        ragel_num.vint *= ragel_num.sign;
        ragel_type = TYPE_INT;
        ragel_cat  = CAT_NUMBER;
    }
#line 108 "lex/ragel_numeric.rl"
	{ragel_num.vdouble = ragel_num.vint;}
#line 113 "lex/ragel_numeric.rl"
	{ragel_num.vdouble = ragel_num.vint;}
	goto st4;
tr27:
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
#line 257 "lex/parser_numeric.cpp"
	switch( (*p) ) {
		case 37: goto st15;
		case 112: goto st5;
	}
	goto st0;
tr24:
#line 30 "lex/ragel_numeric.rl"
	{
        ragel_num.vint *= ragel_num.sign;
        ragel_type = TYPE_INT;
        ragel_cat  = CAT_NUMBER;
    }
#line 113 "lex/ragel_numeric.rl"
	{ragel_num.vdouble = ragel_num.vint;}
	goto st5;
tr28:
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
#line 286 "lex/parser_numeric.cpp"
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
#line 43 "lex/ragel_numeric.rl"
	{ (ragel_num.ratio.den *= 10) += ((*p)-'0'); }
	goto st18;
st18:
	if ( ++p == pe )
		goto _test_eof18;
case 18:
#line 328 "lex/parser_numeric.cpp"
	goto st0;
tr14:
#line 43 "lex/ragel_numeric.rl"
	{ (ragel_num.ratio.den *= 10) += ((*p)-'0'); }
	goto st19;
st19:
	if ( ++p == pe )
		goto _test_eof19;
case 19:
#line 338 "lex/parser_numeric.cpp"
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr14;
	goto st0;
tr3:
#line 29 "lex/ragel_numeric.rl"
	{ (ragel_num.vint *= 10) += ((*p)-'0'); }
#line 42 "lex/ragel_numeric.rl"
	{ (ragel_num.ratio.num *= 10) += ((*p)-'0'); }
	goto st20;
st20:
	if ( ++p == pe )
		goto _test_eof20;
case 20:
#line 352 "lex/parser_numeric.cpp"
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
#line 29 "lex/ragel_numeric.rl"
	{ (ragel_num.vint *= 10) += ((*p)-'0'); }
#line 42 "lex/ragel_numeric.rl"
	{ (ragel_num.ratio.num *= 10) += ((*p)-'0'); }
	goto st22;
st22:
	if ( ++p == pe )
		goto _test_eof22;
case 22:
#line 393 "lex/parser_numeric.cpp"
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
#line 23 "lex/ragel_numeric.rl"
	{ (ragel_num.vint <<= 1) |= ((*p)=='1'); }
	goto st23;
st23:
	if ( ++p == pe )
		goto _test_eof23;
case 23:
#line 420 "lex/parser_numeric.cpp"
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
#line 36 "lex/ragel_numeric.rl"
	{ (ragel_num.vint <<= 4) |= xchar2digit((*p)); }
	goto st24;
st24:
	if ( ++p == pe )
		goto _test_eof24;
case 24:
#line 445 "lex/parser_numeric.cpp"
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
#line 24 "lex/ragel_numeric.rl"
	{
        ragel_type = TYPE_BIN;
        ragel_cat  = CAT_NUMBER;
    }
	break;
	case 14: 
	case 20: 
	case 22: 
#line 30 "lex/ragel_numeric.rl"
	{
        ragel_num.vint *= ragel_num.sign;
        ragel_type = TYPE_INT;
        ragel_cat  = CAT_NUMBER;
    }
	break;
	case 24: 
#line 37 "lex/ragel_numeric.rl"
	{
        ragel_type = TYPE_HEX;
        ragel_cat  = CAT_NUMBER;
    }
	break;
	case 18: 
	case 19: 
#line 44 "lex/ragel_numeric.rl"
	{
        ragel_num.ratio.num *= ragel_num.sign;
        ragel_type = TYPE_RATIO;
        ragel_cat  = CAT_NUMBER;
    }
	break;
	case 17: 
#line 58 "lex/ragel_numeric.rl"
	{
        const auto frac = double(ragel_num.ratio.num) / ragel_num.ratio.den;
        ragel_num.vdouble = ragel_num.vint + ragel_num.sign * frac;
        ragel_type = TYPE_FLOAT;
        ragel_cat  = CAT_NUMBER;
    }
	break;
	case 21: 
#line 65 "lex/ragel_numeric.rl"
	{
        ragel_type = TYPE_INF;
        ragel_cat = CAT_NUMBER;
    }
	break;
	case 15: 
#line 70 "lex/ragel_numeric.rl"
	{
        ragel_type = TYPE_PERCENT;
        ragel_cat = CAT_NUMBER;
    }
	break;
	case 16: 
#line 75 "lex/ragel_numeric.rl"
	{
        ragel_type = TYPE_PHASE;
        ragel_cat = CAT_NUMBER;
    }
	break;
#line 547 "lex/parser_numeric.cpp"
	}
	}

	_out: {}
	}

#line 72 "lex/parser_numeric.rl"

    switch(ragel_type) {
    case TYPE_PHASE:
    case TYPE_PERCENT:
    case TYPE_FLOAT:
        res_.f = ragel_num.getFloat();
        break;
    case TYPE_INT:
    case TYPE_HEX:
    case TYPE_BIN:
        res_.i0 = ragel_num.getInteger();
        break;
    case TYPE_RATIO:
        res_.i0 = ragel_num.ratio.num;
        res_.i1 = ragel_num.ratio.den;
        break;
    case TYPE_INF:
        res_.f = std::numeric_limits<typeof(res_.f)>::infinity();
        break;
    default:
        break;
    }

    res_.t = ragel_type;

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


#line 640 "lex/parser_numeric.cpp"
static const int numeric_some_start = 1;
static const int numeric_some_first_final = 14;
static const int numeric_some_error = 0;

static const int numeric_some_en_main = 1;


#line 167 "lex/parser_numeric.rl"


NumericMatchSome::NumericMatchSome()
{
    reset();
}

void NumericMatchSome::reset()
{
    
#line 659 "lex/parser_numeric.cpp"
	{
	cs = numeric_some_start;
	}

#line 177 "lex/parser_numeric.rl"
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

    DECLARE_RAGEL_COMMON_VARS;
    DECLARE_RAGEL_NUMERIC_VARS;

    bool ok = false;

    
#line 684 "lex/parser_numeric.cpp"
	{
	cs = numeric_some_start;
	}

#line 196 "lex/parser_numeric.rl"
    
#line 691 "lex/parser_numeric.cpp"
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
#line 21 "lex/ragel_numeric.rl"
	{ ragel_num.sign = ((*p)=='-') ? -1 : 1; }
	goto st2;
st2:
	if ( ++p == pe )
		goto _test_eof2;
case 2:
#line 723 "lex/parser_numeric.cpp"
	switch( (*p) ) {
		case 48: goto tr6;
		case 105: goto st10;
	}
	if ( 49 <= (*p) && (*p) <= 57 )
		goto tr4;
	goto st0;
tr6:
#line 29 "lex/ragel_numeric.rl"
	{ (ragel_num.vint *= 10) += ((*p)-'0'); }
#line 42 "lex/ragel_numeric.rl"
	{ (ragel_num.ratio.num *= 10) += ((*p)-'0'); }
	goto st14;
st14:
	if ( ++p == pe )
		goto _test_eof14;
case 14:
#line 741 "lex/parser_numeric.cpp"
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
#line 30 "lex/ragel_numeric.rl"
	{
        ragel_num.vint *= ragel_num.sign;
        ragel_type = TYPE_INT;
        ragel_cat  = CAT_NUMBER;
    }
	goto st15;
tr29:
#line 70 "lex/ragel_numeric.rl"
	{
        ragel_type = TYPE_PERCENT;
        ragel_cat = CAT_NUMBER;
    }
	goto st15;
tr30:
#line 75 "lex/ragel_numeric.rl"
	{
        ragel_type = TYPE_PHASE;
        ragel_cat = CAT_NUMBER;
    }
	goto st15;
tr31:
#line 58 "lex/ragel_numeric.rl"
	{
        const auto frac = double(ragel_num.ratio.num) / ragel_num.ratio.den;
        ragel_num.vdouble = ragel_num.vint + ragel_num.sign * frac;
        ragel_type = TYPE_FLOAT;
        ragel_cat  = CAT_NUMBER;
    }
	goto st15;
tr36:
#line 44 "lex/ragel_numeric.rl"
	{
        ragel_num.ratio.num *= ragel_num.sign;
        ragel_type = TYPE_RATIO;
        ragel_cat  = CAT_NUMBER;
    }
	goto st15;
tr37:
#line 65 "lex/ragel_numeric.rl"
	{
        ragel_type = TYPE_INF;
        ragel_cat = CAT_NUMBER;
    }
	goto st15;
tr40:
#line 24 "lex/ragel_numeric.rl"
	{
        ragel_type = TYPE_BIN;
        ragel_cat  = CAT_NUMBER;
    }
	goto st15;
tr41:
#line 37 "lex/ragel_numeric.rl"
	{
        ragel_type = TYPE_HEX;
        ragel_cat  = CAT_NUMBER;
    }
	goto st15;
st15:
	if ( ++p == pe )
		goto _test_eof15;
case 15:
#line 160 "lex/parser_numeric.rl"
	{if((*p)!=' '&&(*p)!='\t'){p--;{p++; cs = 15; goto _out;}}}
#line 819 "lex/parser_numeric.cpp"
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
#line 30 "lex/ragel_numeric.rl"
	{
        ragel_num.vint *= ragel_num.sign;
        ragel_type = TYPE_INT;
        ragel_cat  = CAT_NUMBER;
    }
#line 108 "lex/ragel_numeric.rl"
	{ragel_num.vdouble = ragel_num.vint;}
	goto st17;
tr32:
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
#line 853 "lex/parser_numeric.cpp"
	switch( (*p) ) {
		case 9: goto tr29;
		case 32: goto tr29;
	}
	goto st0;
tr22:
#line 30 "lex/ragel_numeric.rl"
	{
        ragel_num.vint *= ragel_num.sign;
        ragel_type = TYPE_INT;
        ragel_cat  = CAT_NUMBER;
    }
#line 113 "lex/ragel_numeric.rl"
	{ragel_num.vdouble = ragel_num.vint;}
	goto st18;
tr33:
#line 58 "lex/ragel_numeric.rl"
	{
        const auto frac = double(ragel_num.ratio.num) / ragel_num.ratio.den;
        ragel_num.vdouble = ragel_num.vint + ragel_num.sign * frac;
        ragel_type = TYPE_FLOAT;
        ragel_cat  = CAT_NUMBER;
    }
	goto st18;
st18:
	if ( ++p == pe )
		goto _test_eof18;
case 18:
#line 882 "lex/parser_numeric.cpp"
	switch( (*p) ) {
		case 9: goto tr30;
		case 32: goto tr30;
	}
	goto st0;
tr23:
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
#line 905 "lex/parser_numeric.cpp"
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr7;
	goto st0;
tr7:
#line 54 "lex/ragel_numeric.rl"
	{
        (ragel_num.ratio.num *= 10) += ((*p) - '0');
        ragel_num.ratio.den *= 10;
    }
	goto st19;
st19:
	if ( ++p == pe )
		goto _test_eof19;
case 19:
#line 920 "lex/parser_numeric.cpp"
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
#line 30 "lex/ragel_numeric.rl"
	{
        ragel_num.vint *= ragel_num.sign;
        ragel_type = TYPE_INT;
        ragel_cat  = CAT_NUMBER;
    }
#line 108 "lex/ragel_numeric.rl"
	{ragel_num.vdouble = ragel_num.vint;}
#line 113 "lex/ragel_numeric.rl"
	{ragel_num.vdouble = ragel_num.vint;}
	goto st4;
tr34:
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
#line 957 "lex/parser_numeric.cpp"
	switch( (*p) ) {
		case 37: goto st17;
		case 112: goto st5;
	}
	goto st0;
tr26:
#line 30 "lex/ragel_numeric.rl"
	{
        ragel_num.vint *= ragel_num.sign;
        ragel_type = TYPE_INT;
        ragel_cat  = CAT_NUMBER;
    }
#line 113 "lex/ragel_numeric.rl"
	{ragel_num.vdouble = ragel_num.vint;}
	goto st5;
tr35:
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
#line 986 "lex/parser_numeric.cpp"
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
#line 43 "lex/ragel_numeric.rl"
	{ (ragel_num.ratio.den *= 10) += ((*p)-'0'); }
	goto st20;
st20:
	if ( ++p == pe )
		goto _test_eof20;
case 20:
#line 1028 "lex/parser_numeric.cpp"
	switch( (*p) ) {
		case 9: goto tr36;
		case 32: goto tr36;
	}
	goto st0;
tr15:
#line 43 "lex/ragel_numeric.rl"
	{ (ragel_num.ratio.den *= 10) += ((*p)-'0'); }
	goto st21;
st21:
	if ( ++p == pe )
		goto _test_eof21;
case 21:
#line 1042 "lex/parser_numeric.cpp"
	switch( (*p) ) {
		case 9: goto tr36;
		case 32: goto tr36;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr15;
	goto st0;
tr4:
#line 29 "lex/ragel_numeric.rl"
	{ (ragel_num.vint *= 10) += ((*p)-'0'); }
#line 42 "lex/ragel_numeric.rl"
	{ (ragel_num.ratio.num *= 10) += ((*p)-'0'); }
	goto st22;
st22:
	if ( ++p == pe )
		goto _test_eof22;
case 22:
#line 1060 "lex/parser_numeric.cpp"
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
#line 29 "lex/ragel_numeric.rl"
	{ (ragel_num.vint *= 10) += ((*p)-'0'); }
#line 42 "lex/ragel_numeric.rl"
	{ (ragel_num.ratio.num *= 10) += ((*p)-'0'); }
	goto st24;
st24:
	if ( ++p == pe )
		goto _test_eof24;
case 24:
#line 1107 "lex/parser_numeric.cpp"
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
#line 23 "lex/ragel_numeric.rl"
	{ (ragel_num.vint <<= 1) |= ((*p)=='1'); }
	goto st25;
st25:
	if ( ++p == pe )
		goto _test_eof25;
case 25:
#line 1136 "lex/parser_numeric.cpp"
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
#line 36 "lex/ragel_numeric.rl"
	{ (ragel_num.vint <<= 4) |= xchar2digit((*p)); }
	goto st26;
st26:
	if ( ++p == pe )
		goto _test_eof26;
case 26:
#line 1165 "lex/parser_numeric.cpp"
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
#line 24 "lex/ragel_numeric.rl"
	{
        ragel_type = TYPE_BIN;
        ragel_cat  = CAT_NUMBER;
    }
	break;
	case 14: 
	case 22: 
	case 24: 
#line 30 "lex/ragel_numeric.rl"
	{
        ragel_num.vint *= ragel_num.sign;
        ragel_type = TYPE_INT;
        ragel_cat  = CAT_NUMBER;
    }
	break;
	case 26: 
#line 37 "lex/ragel_numeric.rl"
	{
        ragel_type = TYPE_HEX;
        ragel_cat  = CAT_NUMBER;
    }
	break;
	case 20: 
	case 21: 
#line 44 "lex/ragel_numeric.rl"
	{
        ragel_num.ratio.num *= ragel_num.sign;
        ragel_type = TYPE_RATIO;
        ragel_cat  = CAT_NUMBER;
    }
	break;
	case 19: 
#line 58 "lex/ragel_numeric.rl"
	{
        const auto frac = double(ragel_num.ratio.num) / ragel_num.ratio.den;
        ragel_num.vdouble = ragel_num.vint + ragel_num.sign * frac;
        ragel_type = TYPE_FLOAT;
        ragel_cat  = CAT_NUMBER;
    }
	break;
	case 23: 
#line 65 "lex/ragel_numeric.rl"
	{
        ragel_type = TYPE_INF;
        ragel_cat = CAT_NUMBER;
    }
	break;
	case 17: 
#line 70 "lex/ragel_numeric.rl"
	{
        ragel_type = TYPE_PERCENT;
        ragel_cat = CAT_NUMBER;
    }
	break;
	case 18: 
#line 75 "lex/ragel_numeric.rl"
	{
        ragel_type = TYPE_PHASE;
        ragel_cat = CAT_NUMBER;
    }
	break;
#line 1274 "lex/parser_numeric.cpp"
	}
	}

	_out: {}
	}

#line 197 "lex/parser_numeric.rl"

    ok = cs >= 14;

    if(ok) {
        switch(ragel_type) {
        case TYPE_PHASE:
        case TYPE_PERCENT:
        case TYPE_FLOAT:
            res_.push_back({});
            res_.back().f = ragel_num.getFloat();
            break;
        case TYPE_INT:
        case TYPE_HEX:
        case TYPE_BIN:
            res_.push_back({});
            res_.back().i0 = ragel_num.getInteger();
            break;
        case TYPE_RATIO:
            res_.push_back({});
            res_.back().i0 = ragel_num.ratio.num;
            res_.back().i1 = ragel_num.ratio.den;
            break;
        case TYPE_INF:
            res_.push_back({});
            res_.back().f = std::numeric_limits<typeof(res_[0].f)>::infinity();
            break;
        default:
            return false;
            break;
        }

        res_.back().t = ragel_type;
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


