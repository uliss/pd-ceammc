
#line 1 "grain_expr.rl"
# include <cstddef>
# include <cstdio>
# include <cstdlib>
# include <cstring>
# include <cassert>
# include <iostream>
# include <cmath>

# include "grain_expr_parser.h"

# ifndef NDEBUG
# define NDEBUG
# endif

namespace {
# include "grain_expr_parser.cpp"
}


#line 147 "grain_expr.rl"



#line 27 "/Users/serge/work/music/pure-data/ceammc/ext/src/array/grain_expr.cpp"
static const int grain_expr_start = 39;
static const int grain_expr_first_final = 39;
static const int grain_expr_error = 0;

static const int grain_expr_en_main = 39;


#line 150 "grain_expr.rl"

static_assert(GrainExprParser::BUFSIZE >= sizeof(yyParser), "");

void GrainExprParser::init()
{
    res_ = 0;
    parse_grain_exprInit(buf_);
    parser_ = buf_;
}

bool GrainExprParser::execute(const char* data)
{
    const size_t len = strlen(data);
    const char* p = data;
    const char* pe = data + len;
    const char* eof = pe;
    int stack[4];
    int top;
    int dintp = 0;
    int dfracp = 0;
    int dfracnum = 0;
    int dfracden = 1;
    double dsign = 1;
    double dval = 0;

    // clean error message
    err_buf_[0] = '\0';

    
#line 65 "/Users/serge/work/music/pure-data/ceammc/ext/src/array/grain_expr.cpp"
	{
	cs = grain_expr_start;
	ts = 0;
	te = 0;
	act = 0;
	}

#line 179 "grain_expr.rl"

    
#line 76 "/Users/serge/work/music/pure-data/ceammc/ext/src/array/grain_expr.cpp"
	{
	if ( p == pe )
		goto _test_eof;
	switch ( cs )
	{
tr2:
#line 66 "grain_expr.rl"
	{te = p+1;{
    parse_grain_expr(parser_, TK_DOUBLE, n_, this);
}}
	goto st39;
tr6:
#line 58 "grain_expr.rl"
	{te = p+1;{
    parse_grain_expr(parser_, TK_DOUBLE, amp_, this);
}}
	goto st39;
tr8:
#line 78 "grain_expr.rl"
	{te = p+1;{
    parse_grain_expr(parser_, TK_DOUBLE, std::acos(-1), this);
}}
	goto st39;
tr9:
#line 70 "grain_expr.rl"
	{te = p+1;{
    parse_grain_expr(parser_, TK_DOUBLE, pan_, this);
}}
	goto st39;
tr11:
#line 62 "grain_expr.rl"
	{te = p+1;{
    parse_grain_expr(parser_, TK_DOUBLE, sr_, this);
}}
	goto st39;
tr14:
#line 74 "grain_expr.rl"
	{te = p+1;{
    parse_grain_expr(parser_, TK_DOUBLE, speed_, this);
}}
	goto st39;
tr19:
#line 82 "grain_expr.rl"
	{te = p+1;{ parse_grain_expr(parser_, TK_FN_ABS, 0, this); }}
	goto st39;
tr21:
#line 88 "grain_expr.rl"
	{te = p+1;{ parse_grain_expr(parser_, TK_FN_ACOS, 0, this); }}
	goto st39;
tr23:
#line 89 "grain_expr.rl"
	{te = p+1;{ parse_grain_expr(parser_, TK_FN_ASIN, 0, this); }}
	goto st39;
tr25:
#line 90 "grain_expr.rl"
	{te = p+1;{ parse_grain_expr(parser_, TK_FN_ATAN, 0, this); }}
	goto st39;
tr29:
#line 92 "grain_expr.rl"
	{te = p+1;{ parse_grain_expr(parser_, TK_FN_CEIL, 0, this); }}
	goto st39;
tr30:
#line 85 "grain_expr.rl"
	{te = p+1;{ parse_grain_expr(parser_, TK_FN_COS, 0, this); }}
	goto st39;
tr34:
#line 93 "grain_expr.rl"
	{te = p+1;{ parse_grain_expr(parser_, TK_FN_FLOOR, 0, this); }}
	goto st39;
tr37:
#line 96 "grain_expr.rl"
	{{p = ((te))-1;}{ parse_grain_expr(parser_, TK_FN_LOG, 0, this); }}
	goto st39;
tr38:
#line 97 "grain_expr.rl"
	{te = p+1;{ parse_grain_expr(parser_, TK_FN_LOG10, 0, this); }}
	goto st39;
tr42:
#line 94 "grain_expr.rl"
	{te = p+1;{ parse_grain_expr(parser_, TK_FN_ROUND, 0, this); }}
	goto st39;
tr45:
#line 86 "grain_expr.rl"
	{te = p+1;{ parse_grain_expr(parser_, TK_FN_SIN, 0, this); }}
	goto st39;
tr47:
#line 83 "grain_expr.rl"
	{te = p+1;{ parse_grain_expr(parser_, TK_FN_SQRT, 0, this); }}
	goto st39;
tr49:
#line 87 "grain_expr.rl"
	{te = p+1;{ parse_grain_expr(parser_, TK_FN_TAN, 0, this); }}
	goto st39;
tr51:
#line 42 "grain_expr.rl"
	{te = p+1;{
    parse_grain_expr(parser_, TK_MOD, 0, this);
}}
	goto st39;
tr52:
#line 46 "grain_expr.rl"
	{te = p+1;{
    parse_grain_expr(parser_, TK_OPENP, 0, this);
}}
	goto st39;
tr53:
#line 50 "grain_expr.rl"
	{te = p+1;{
    parse_grain_expr(parser_, TK_CLOSEP, 0, this);
}}
	goto st39;
tr54:
#line 30 "grain_expr.rl"
	{te = p+1;{
    parse_grain_expr(parser_, TK_TIMES, 0, this);
}}
	goto st39;
tr55:
#line 22 "grain_expr.rl"
	{te = p+1;{
    parse_grain_expr(parser_, TK_PLUS, 0, this);
}}
	goto st39;
tr57:
#line 34 "grain_expr.rl"
	{te = p+1;{
    parse_grain_expr(parser_, TK_DIVIDE, 0, this);
}}
	goto st39;
tr59:
#line 38 "grain_expr.rl"
	{te = p+1;{
    parse_grain_expr(parser_, TK_POW, 0, this);
}}
	goto st39;
tr67:
#line 26 "grain_expr.rl"
	{te = p;p--;{
    parse_grain_expr(parser_, TK_MINUS, 0, this);
}}
	goto st39;
tr68:
#line 105 "grain_expr.rl"
	{ dval = dsign * (double(dintp) + double(dfracnum) / double(dfracden)); }
#line 54 "grain_expr.rl"
	{te = p;p--;{
    parse_grain_expr(parser_, TK_DOUBLE, dval, this);
}}
	goto st39;
tr71:
#line 102 "grain_expr.rl"
	{dfracnum = 0; dfracden = 1;}
#line 105 "grain_expr.rl"
	{ dval = dsign * (double(dintp) + double(dfracnum) / double(dfracden)); }
#line 54 "grain_expr.rl"
	{te = p;p--;{
    parse_grain_expr(parser_, TK_DOUBLE, dval, this);
}}
	goto st39;
tr74:
#line 96 "grain_expr.rl"
	{te = p;p--;{ parse_grain_expr(parser_, TK_FN_LOG, 0, this); }}
	goto st39;
tr76:
#line 98 "grain_expr.rl"
	{te = p+1;{ parse_grain_expr(parser_, TK_FN_LOG2, 0, this); }}
	goto st39;
st39:
#line 1 "NONE"
	{ts = 0;}
	if ( ++p == pe )
		goto _test_eof39;
case 39:
#line 1 "NONE"
	{ts = p;}
#line 252 "/Users/serge/work/music/pure-data/ceammc/ext/src/array/grain_expr.cpp"
	switch( (*p) ) {
		case 36: goto st1;
		case 37: goto tr51;
		case 40: goto tr52;
		case 41: goto tr53;
		case 42: goto tr54;
		case 43: goto tr55;
		case 45: goto tr56;
		case 47: goto tr57;
		case 94: goto tr59;
		case 97: goto st10;
		case 99: goto st18;
		case 102: goto st22;
		case 108: goto st26;
		case 114: goto st29;
		case 115: goto st33;
		case 116: goto st37;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr58;
	goto st0;
st0:
cs = 0;
	goto _out;
st1:
	if ( ++p == pe )
		goto _test_eof1;
case 1:
	switch( (*p) ) {
		case 97: goto st2;
		case 110: goto tr2;
		case 112: goto st4;
		case 115: goto st6;
	}
	goto st0;
st2:
	if ( ++p == pe )
		goto _test_eof2;
case 2:
	if ( (*p) == 109 )
		goto st3;
	goto st0;
st3:
	if ( ++p == pe )
		goto _test_eof3;
case 3:
	if ( (*p) == 112 )
		goto tr6;
	goto st0;
st4:
	if ( ++p == pe )
		goto _test_eof4;
case 4:
	switch( (*p) ) {
		case 97: goto st5;
		case 105: goto tr8;
	}
	goto st0;
st5:
	if ( ++p == pe )
		goto _test_eof5;
case 5:
	if ( (*p) == 110 )
		goto tr9;
	goto st0;
st6:
	if ( ++p == pe )
		goto _test_eof6;
case 6:
	switch( (*p) ) {
		case 112: goto st7;
		case 114: goto tr11;
	}
	goto st0;
st7:
	if ( ++p == pe )
		goto _test_eof7;
case 7:
	if ( (*p) == 101 )
		goto st8;
	goto st0;
st8:
	if ( ++p == pe )
		goto _test_eof8;
case 8:
	if ( (*p) == 101 )
		goto st9;
	goto st0;
st9:
	if ( ++p == pe )
		goto _test_eof9;
case 9:
	if ( (*p) == 100 )
		goto tr14;
	goto st0;
tr56:
#line 100 "grain_expr.rl"
	{ dsign = -1; }
	goto st40;
st40:
	if ( ++p == pe )
		goto _test_eof40;
case 40:
#line 356 "/Users/serge/work/music/pure-data/ceammc/ext/src/array/grain_expr.cpp"
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr58;
	goto tr67;
tr58:
#line 101 "grain_expr.rl"
	{dintp = 0;}
#line 101 "grain_expr.rl"
	{ (dintp *= 10) += ((*p)-'0'); }
	goto st41;
tr70:
#line 101 "grain_expr.rl"
	{ (dintp *= 10) += ((*p)-'0'); }
	goto st41;
st41:
	if ( ++p == pe )
		goto _test_eof41;
case 41:
#line 374 "/Users/serge/work/music/pure-data/ceammc/ext/src/array/grain_expr.cpp"
	if ( (*p) == 46 )
		goto st42;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr70;
	goto tr68;
st42:
	if ( ++p == pe )
		goto _test_eof42;
case 42:
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr72;
	goto tr71;
tr72:
#line 102 "grain_expr.rl"
	{dfracnum = 0; dfracden = 1;}
#line 102 "grain_expr.rl"
	{ (dfracnum *= 10) += ((*p)-'0'); dfracden*=10; }
	goto st43;
tr73:
#line 102 "grain_expr.rl"
	{ (dfracnum *= 10) += ((*p)-'0'); dfracden*=10; }
	goto st43;
st43:
	if ( ++p == pe )
		goto _test_eof43;
case 43:
#line 401 "/Users/serge/work/music/pure-data/ceammc/ext/src/array/grain_expr.cpp"
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr73;
	goto tr68;
st10:
	if ( ++p == pe )
		goto _test_eof10;
case 10:
	switch( (*p) ) {
		case 98: goto st11;
		case 99: goto st12;
		case 115: goto st14;
		case 116: goto st16;
	}
	goto st0;
st11:
	if ( ++p == pe )
		goto _test_eof11;
case 11:
	if ( (*p) == 115 )
		goto tr19;
	goto st0;
st12:
	if ( ++p == pe )
		goto _test_eof12;
case 12:
	if ( (*p) == 111 )
		goto st13;
	goto st0;
st13:
	if ( ++p == pe )
		goto _test_eof13;
case 13:
	if ( (*p) == 115 )
		goto tr21;
	goto st0;
st14:
	if ( ++p == pe )
		goto _test_eof14;
case 14:
	if ( (*p) == 105 )
		goto st15;
	goto st0;
st15:
	if ( ++p == pe )
		goto _test_eof15;
case 15:
	if ( (*p) == 110 )
		goto tr23;
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
	if ( (*p) == 110 )
		goto tr25;
	goto st0;
st18:
	if ( ++p == pe )
		goto _test_eof18;
case 18:
	switch( (*p) ) {
		case 101: goto st19;
		case 111: goto st21;
	}
	goto st0;
st19:
	if ( ++p == pe )
		goto _test_eof19;
case 19:
	if ( (*p) == 105 )
		goto st20;
	goto st0;
st20:
	if ( ++p == pe )
		goto _test_eof20;
case 20:
	if ( (*p) == 108 )
		goto tr29;
	goto st0;
st21:
	if ( ++p == pe )
		goto _test_eof21;
case 21:
	if ( (*p) == 115 )
		goto tr30;
	goto st0;
st22:
	if ( ++p == pe )
		goto _test_eof22;
case 22:
	if ( (*p) == 108 )
		goto st23;
	goto st0;
st23:
	if ( ++p == pe )
		goto _test_eof23;
case 23:
	if ( (*p) == 111 )
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
	if ( (*p) == 114 )
		goto tr34;
	goto st0;
st26:
	if ( ++p == pe )
		goto _test_eof26;
case 26:
	if ( (*p) == 111 )
		goto st27;
	goto st0;
st27:
	if ( ++p == pe )
		goto _test_eof27;
case 27:
	if ( (*p) == 103 )
		goto tr36;
	goto st0;
tr36:
#line 1 "NONE"
	{te = p+1;}
	goto st44;
st44:
	if ( ++p == pe )
		goto _test_eof44;
case 44:
#line 545 "/Users/serge/work/music/pure-data/ceammc/ext/src/array/grain_expr.cpp"
	switch( (*p) ) {
		case 49: goto st28;
		case 50: goto tr76;
	}
	goto tr74;
st28:
	if ( ++p == pe )
		goto _test_eof28;
case 28:
	if ( (*p) == 48 )
		goto tr38;
	goto tr37;
st29:
	if ( ++p == pe )
		goto _test_eof29;
case 29:
	if ( (*p) == 111 )
		goto st30;
	goto st0;
st30:
	if ( ++p == pe )
		goto _test_eof30;
case 30:
	if ( (*p) == 117 )
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
	if ( (*p) == 100 )
		goto tr42;
	goto st0;
st33:
	if ( ++p == pe )
		goto _test_eof33;
case 33:
	switch( (*p) ) {
		case 105: goto st34;
		case 113: goto st35;
	}
	goto st0;
st34:
	if ( ++p == pe )
		goto _test_eof34;
case 34:
	if ( (*p) == 110 )
		goto tr45;
	goto st0;
st35:
	if ( ++p == pe )
		goto _test_eof35;
case 35:
	if ( (*p) == 114 )
		goto st36;
	goto st0;
st36:
	if ( ++p == pe )
		goto _test_eof36;
case 36:
	if ( (*p) == 116 )
		goto tr47;
	goto st0;
st37:
	if ( ++p == pe )
		goto _test_eof37;
case 37:
	if ( (*p) == 97 )
		goto st38;
	goto st0;
st38:
	if ( ++p == pe )
		goto _test_eof38;
case 38:
	if ( (*p) == 110 )
		goto tr49;
	goto st0;
	}
	_test_eof39: cs = 39; goto _test_eof; 
	_test_eof1: cs = 1; goto _test_eof; 
	_test_eof2: cs = 2; goto _test_eof; 
	_test_eof3: cs = 3; goto _test_eof; 
	_test_eof4: cs = 4; goto _test_eof; 
	_test_eof5: cs = 5; goto _test_eof; 
	_test_eof6: cs = 6; goto _test_eof; 
	_test_eof7: cs = 7; goto _test_eof; 
	_test_eof8: cs = 8; goto _test_eof; 
	_test_eof9: cs = 9; goto _test_eof; 
	_test_eof40: cs = 40; goto _test_eof; 
	_test_eof41: cs = 41; goto _test_eof; 
	_test_eof42: cs = 42; goto _test_eof; 
	_test_eof43: cs = 43; goto _test_eof; 
	_test_eof10: cs = 10; goto _test_eof; 
	_test_eof11: cs = 11; goto _test_eof; 
	_test_eof12: cs = 12; goto _test_eof; 
	_test_eof13: cs = 13; goto _test_eof; 
	_test_eof14: cs = 14; goto _test_eof; 
	_test_eof15: cs = 15; goto _test_eof; 
	_test_eof16: cs = 16; goto _test_eof; 
	_test_eof17: cs = 17; goto _test_eof; 
	_test_eof18: cs = 18; goto _test_eof; 
	_test_eof19: cs = 19; goto _test_eof; 
	_test_eof20: cs = 20; goto _test_eof; 
	_test_eof21: cs = 21; goto _test_eof; 
	_test_eof22: cs = 22; goto _test_eof; 
	_test_eof23: cs = 23; goto _test_eof; 
	_test_eof24: cs = 24; goto _test_eof; 
	_test_eof25: cs = 25; goto _test_eof; 
	_test_eof26: cs = 26; goto _test_eof; 
	_test_eof27: cs = 27; goto _test_eof; 
	_test_eof44: cs = 44; goto _test_eof; 
	_test_eof28: cs = 28; goto _test_eof; 
	_test_eof29: cs = 29; goto _test_eof; 
	_test_eof30: cs = 30; goto _test_eof; 
	_test_eof31: cs = 31; goto _test_eof; 
	_test_eof32: cs = 32; goto _test_eof; 
	_test_eof33: cs = 33; goto _test_eof; 
	_test_eof34: cs = 34; goto _test_eof; 
	_test_eof35: cs = 35; goto _test_eof; 
	_test_eof36: cs = 36; goto _test_eof; 
	_test_eof37: cs = 37; goto _test_eof; 
	_test_eof38: cs = 38; goto _test_eof; 

	_test_eof: {}
	if ( p == eof )
	{
	switch ( cs ) {
	case 40: goto tr67;
	case 41: goto tr68;
	case 42: goto tr71;
	case 43: goto tr68;
	case 44: goto tr74;
	case 28: goto tr37;
	}
	}

	_out: {}
	}

#line 181 "grain_expr.rl"

    if (cs == grain_expr_error) {
        setErrorMsg("unknown token");
        return false;
    }

    // done parsing
    parse_grain_expr(parser_, 0, 0, this);
    return true;
}

void GrainExprParser::setErrorMsg(const char* str)
{
    const size_t n = sizeof(err_buf_) - 1;
    for (size_t i = 0; i < n; i++) {
        char c = str[i];
        err_buf_[i] = c;
        if (c == '\0')
            break;
    }
}
