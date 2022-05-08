
#line 1 "lex/parser_clone.rl"
# include "parser_clone.h"
# include <cstring>

namespace ceammc {
namespace parser {

static inline void set_args(CloneMessage& res, ArgumentType type, int16_t first, int16_t last, int16_t step) {
    res.args.first = first;
    res.args.last = last;
    res.args.step = step;
    res.args.type = type;
}


#line 18 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
static const int clone_start = 1;
static const int clone_first_final = 19;
static const int clone_error = 0;

static const int clone_en_main = 1;


#line 57 "lex/parser_clone.rl"


bool parse_clone_action(const char* str, CloneMessage& res)
{
    const auto len = strlen(str);
    if (len == 0) {
        res.msg_type = MSG_TYPE_NONE;
        return false;
    }

    int cs = 0;
    const char* p = str;
    const char* pe = p + len;
    const char* eof = pe;

    int16_t id = 0, id0 = 1, id1 = -1, step = 1;

    
#line 45 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
	{
	cs = clone_start;
	}

#line 75 "lex/parser_clone.rl"
    
#line 52 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
	{
	if ( p == pe )
		goto _test_eof;
	switch ( cs )
	{
case 1:
	switch( (*p) ) {
		case 35: goto tr0;
		case 94: goto tr2;
		case 126: goto tr3;
	}
	goto st0;
st0:
cs = 0;
	goto _out;
tr0:
#line 37 "lex/parser_clone.rl"
	{ res.msg_type = MSG_TYPE_SEND; }
	goto st2;
tr29:
#line 39 "lex/parser_clone.rl"
	{ res.msg_type = MSG_TYPE_DSP_TOGGLE; }
	goto st2;
tr3:
#line 38 "lex/parser_clone.rl"
	{ res.msg_type = MSG_TYPE_DSP_SET; }
	goto st2;
st2:
	if ( ++p == pe )
		goto _test_eof2;
case 2:
#line 84 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
	switch( (*p) ) {
		case 33: goto st3;
		case 37: goto tr5;
		case 42: goto tr6;
		case 60: goto st6;
		case 61: goto st13;
		case 62: goto st14;
	}
	if ( 49 <= (*p) && (*p) <= 57 )
		goto tr7;
	goto st0;
st3:
	if ( ++p == pe )
		goto _test_eof3;
case 3:
	if ( 49 <= (*p) && (*p) <= 57 )
		goto tr11;
	goto st0;
tr11:
#line 17 "lex/parser_clone.rl"
	{ id = (*p)-'0'; }
#line 26 "lex/parser_clone.rl"
	{ set_args(res, ARG_TYPE_EXCEPT, id, -1, 1); }
	goto st19;
st19:
	if ( ++p == pe )
		goto _test_eof19;
case 19:
#line 113 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr30;
	goto st0;
tr30:
#line 17 "lex/parser_clone.rl"
	{ id = 10*id+((*p)-'0'); }
#line 26 "lex/parser_clone.rl"
	{ set_args(res, ARG_TYPE_EXCEPT, id, -1, 1); }
	goto st20;
st20:
	if ( ++p == pe )
		goto _test_eof20;
case 20:
#line 127 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr31;
	goto st0;
tr31:
#line 17 "lex/parser_clone.rl"
	{ id = 10*id+((*p)-'0'); }
#line 26 "lex/parser_clone.rl"
	{ set_args(res, ARG_TYPE_EXCEPT, id, -1, 1); }
	goto st21;
st21:
	if ( ++p == pe )
		goto _test_eof21;
case 21:
#line 141 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr32;
	goto st0;
tr32:
#line 17 "lex/parser_clone.rl"
	{ id = 10*id+((*p)-'0'); }
#line 26 "lex/parser_clone.rl"
	{ set_args(res, ARG_TYPE_EXCEPT, id, -1, 1); }
	goto st22;
st22:
	if ( ++p == pe )
		goto _test_eof22;
case 22:
#line 155 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr33;
	goto st0;
tr5:
#line 25 "lex/parser_clone.rl"
	{ set_args(res, ARG_TYPE_RANDOM, -1, -1, 1); }
	goto st23;
tr6:
#line 24 "lex/parser_clone.rl"
	{ set_args(res, ARG_TYPE_ALL, -1, -1, 1); }
	goto st23;
tr33:
#line 17 "lex/parser_clone.rl"
	{ id = 10*id+((*p)-'0'); }
#line 26 "lex/parser_clone.rl"
	{ set_args(res, ARG_TYPE_EXCEPT, id, -1, 1); }
	goto st23;
tr44:
#line 20 "lex/parser_clone.rl"
	{ step = 10*step+((*p)-'0'); }
#line 32 "lex/parser_clone.rl"
	{ set_args(res, ARG_TYPE_RANGE, id0, id1, step); }
	goto st23;
tr56:
#line 20 "lex/parser_clone.rl"
	{ step = 10*step+((*p)-'0'); }
#line 30 "lex/parser_clone.rl"
	{ set_args(res, ARG_TYPE_LT, id, -1, step); }
	goto st23;
tr69:
#line 20 "lex/parser_clone.rl"
	{ step = 10*step+((*p)-'0'); }
#line 34 "lex/parser_clone.rl"
	{ set_args(res, ARG_TYPE_SPREAD, id0, id1, step); }
	goto st23;
tr76:
#line 20 "lex/parser_clone.rl"
	{ step = 10*step+((*p)-'0'); }
#line 33 "lex/parser_clone.rl"
	{ set_args(res, ARG_TYPE_SPREAD, id0, -1, step); }
	goto st23;
tr85:
#line 20 "lex/parser_clone.rl"
	{ step = 10*step+((*p)-'0'); }
#line 31 "lex/parser_clone.rl"
	{ set_args(res, ARG_TYPE_LE, id, -1, step); }
	goto st23;
tr89:
#line 17 "lex/parser_clone.rl"
	{ id = 10*id+((*p)-'0'); }
#line 27 "lex/parser_clone.rl"
	{ set_args(res, ARG_TYPE_EQ, id, -1, 1); }
	goto st23;
tr98:
#line 20 "lex/parser_clone.rl"
	{ step = 10*step+((*p)-'0'); }
#line 28 "lex/parser_clone.rl"
	{ set_args(res, ARG_TYPE_GT, id, -1, step); }
	goto st23;
tr107:
#line 20 "lex/parser_clone.rl"
	{ step = 10*step+((*p)-'0'); }
#line 29 "lex/parser_clone.rl"
	{ set_args(res, ARG_TYPE_GE, id, -1, step); }
	goto st23;
st23:
	if ( ++p == pe )
		goto _test_eof23;
case 23:
#line 225 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
	goto st0;
tr7:
#line 17 "lex/parser_clone.rl"
	{ id = (*p)-'0'; }
#line 27 "lex/parser_clone.rl"
	{ set_args(res, ARG_TYPE_EQ, id, -1, 1); }
#line 18 "lex/parser_clone.rl"
	{ id0 = id; }
	goto st24;
st24:
	if ( ++p == pe )
		goto _test_eof24;
case 24:
#line 239 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
	if ( (*p) == 45 )
		goto st4;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr35;
	goto st0;
st4:
	if ( ++p == pe )
		goto _test_eof4;
case 4:
	if ( 49 <= (*p) && (*p) <= 57 )
		goto tr12;
	goto st0;
tr12:
#line 17 "lex/parser_clone.rl"
	{ id = (*p)-'0'; }
#line 19 "lex/parser_clone.rl"
	{ id1 = id; }
#line 32 "lex/parser_clone.rl"
	{ set_args(res, ARG_TYPE_RANGE, id0, id1, step); }
	goto st25;
st25:
	if ( ++p == pe )
		goto _test_eof25;
case 25:
#line 264 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
	if ( (*p) == 124 )
		goto st5;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr36;
	goto st0;
tr36:
#line 17 "lex/parser_clone.rl"
	{ id = 10*id+((*p)-'0'); }
#line 19 "lex/parser_clone.rl"
	{ id1 = id; }
#line 32 "lex/parser_clone.rl"
	{ set_args(res, ARG_TYPE_RANGE, id0, id1, step); }
	goto st26;
st26:
	if ( ++p == pe )
		goto _test_eof26;
case 26:
#line 282 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
	if ( (*p) == 124 )
		goto st5;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr38;
	goto st0;
tr38:
#line 17 "lex/parser_clone.rl"
	{ id = 10*id+((*p)-'0'); }
#line 19 "lex/parser_clone.rl"
	{ id1 = id; }
#line 32 "lex/parser_clone.rl"
	{ set_args(res, ARG_TYPE_RANGE, id0, id1, step); }
	goto st27;
st27:
	if ( ++p == pe )
		goto _test_eof27;
case 27:
#line 300 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
	if ( (*p) == 124 )
		goto st5;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr39;
	goto st0;
tr39:
#line 17 "lex/parser_clone.rl"
	{ id = 10*id+((*p)-'0'); }
#line 19 "lex/parser_clone.rl"
	{ id1 = id; }
#line 32 "lex/parser_clone.rl"
	{ set_args(res, ARG_TYPE_RANGE, id0, id1, step); }
	goto st28;
st28:
	if ( ++p == pe )
		goto _test_eof28;
case 28:
#line 318 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
	if ( (*p) == 124 )
		goto st5;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr40;
	goto st0;
tr40:
#line 17 "lex/parser_clone.rl"
	{ id = 10*id+((*p)-'0'); }
#line 19 "lex/parser_clone.rl"
	{ id1 = id; }
#line 32 "lex/parser_clone.rl"
	{ set_args(res, ARG_TYPE_RANGE, id0, id1, step); }
	goto st29;
st29:
	if ( ++p == pe )
		goto _test_eof29;
case 29:
#line 336 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
	if ( (*p) == 124 )
		goto st5;
	goto st0;
st5:
	if ( ++p == pe )
		goto _test_eof5;
case 5:
	if ( 49 <= (*p) && (*p) <= 57 )
		goto tr13;
	goto st0;
tr13:
#line 20 "lex/parser_clone.rl"
	{ step = (*p)-'0'; }
#line 32 "lex/parser_clone.rl"
	{ set_args(res, ARG_TYPE_RANGE, id0, id1, step); }
	goto st30;
st30:
	if ( ++p == pe )
		goto _test_eof30;
case 30:
#line 357 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr41;
	goto st0;
tr41:
#line 20 "lex/parser_clone.rl"
	{ step = 10*step+((*p)-'0'); }
#line 32 "lex/parser_clone.rl"
	{ set_args(res, ARG_TYPE_RANGE, id0, id1, step); }
	goto st31;
st31:
	if ( ++p == pe )
		goto _test_eof31;
case 31:
#line 371 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr42;
	goto st0;
tr42:
#line 20 "lex/parser_clone.rl"
	{ step = 10*step+((*p)-'0'); }
#line 32 "lex/parser_clone.rl"
	{ set_args(res, ARG_TYPE_RANGE, id0, id1, step); }
	goto st32;
st32:
	if ( ++p == pe )
		goto _test_eof32;
case 32:
#line 385 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr43;
	goto st0;
tr43:
#line 20 "lex/parser_clone.rl"
	{ step = 10*step+((*p)-'0'); }
#line 32 "lex/parser_clone.rl"
	{ set_args(res, ARG_TYPE_RANGE, id0, id1, step); }
	goto st33;
st33:
	if ( ++p == pe )
		goto _test_eof33;
case 33:
#line 399 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr44;
	goto st0;
tr35:
#line 17 "lex/parser_clone.rl"
	{ id = 10*id+((*p)-'0'); }
#line 27 "lex/parser_clone.rl"
	{ set_args(res, ARG_TYPE_EQ, id, -1, 1); }
#line 18 "lex/parser_clone.rl"
	{ id0 = id; }
	goto st34;
st34:
	if ( ++p == pe )
		goto _test_eof34;
case 34:
#line 415 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
	if ( (*p) == 45 )
		goto st4;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr45;
	goto st0;
tr45:
#line 17 "lex/parser_clone.rl"
	{ id = 10*id+((*p)-'0'); }
#line 27 "lex/parser_clone.rl"
	{ set_args(res, ARG_TYPE_EQ, id, -1, 1); }
#line 18 "lex/parser_clone.rl"
	{ id0 = id; }
	goto st35;
st35:
	if ( ++p == pe )
		goto _test_eof35;
case 35:
#line 433 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
	if ( (*p) == 45 )
		goto st4;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr46;
	goto st0;
tr46:
#line 17 "lex/parser_clone.rl"
	{ id = 10*id+((*p)-'0'); }
#line 27 "lex/parser_clone.rl"
	{ set_args(res, ARG_TYPE_EQ, id, -1, 1); }
#line 18 "lex/parser_clone.rl"
	{ id0 = id; }
	goto st36;
st36:
	if ( ++p == pe )
		goto _test_eof36;
case 36:
#line 451 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
	if ( (*p) == 45 )
		goto st4;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr47;
	goto st0;
tr47:
#line 17 "lex/parser_clone.rl"
	{ id = 10*id+((*p)-'0'); }
#line 27 "lex/parser_clone.rl"
	{ set_args(res, ARG_TYPE_EQ, id, -1, 1); }
#line 18 "lex/parser_clone.rl"
	{ id0 = id; }
	goto st37;
st37:
	if ( ++p == pe )
		goto _test_eof37;
case 37:
#line 469 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
	if ( (*p) == 45 )
		goto st4;
	goto st0;
st6:
	if ( ++p == pe )
		goto _test_eof6;
case 6:
	switch( (*p) ) {
		case 58: goto tr15;
		case 61: goto st11;
	}
	if ( 49 <= (*p) && (*p) <= 57 )
		goto tr14;
	goto st0;
tr14:
#line 17 "lex/parser_clone.rl"
	{ id = (*p)-'0'; }
#line 30 "lex/parser_clone.rl"
	{ set_args(res, ARG_TYPE_LT, id, -1, step); }
	goto st38;
st38:
	if ( ++p == pe )
		goto _test_eof38;
case 38:
#line 494 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
	if ( (*p) == 124 )
		goto st7;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr48;
	goto st0;
tr48:
#line 17 "lex/parser_clone.rl"
	{ id = 10*id+((*p)-'0'); }
#line 30 "lex/parser_clone.rl"
	{ set_args(res, ARG_TYPE_LT, id, -1, step); }
	goto st39;
st39:
	if ( ++p == pe )
		goto _test_eof39;
case 39:
#line 510 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
	if ( (*p) == 124 )
		goto st7;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr50;
	goto st0;
tr50:
#line 17 "lex/parser_clone.rl"
	{ id = 10*id+((*p)-'0'); }
#line 30 "lex/parser_clone.rl"
	{ set_args(res, ARG_TYPE_LT, id, -1, step); }
	goto st40;
st40:
	if ( ++p == pe )
		goto _test_eof40;
case 40:
#line 526 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
	if ( (*p) == 124 )
		goto st7;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr51;
	goto st0;
tr51:
#line 17 "lex/parser_clone.rl"
	{ id = 10*id+((*p)-'0'); }
#line 30 "lex/parser_clone.rl"
	{ set_args(res, ARG_TYPE_LT, id, -1, step); }
	goto st41;
st41:
	if ( ++p == pe )
		goto _test_eof41;
case 41:
#line 542 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
	if ( (*p) == 124 )
		goto st7;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr52;
	goto st0;
tr52:
#line 17 "lex/parser_clone.rl"
	{ id = 10*id+((*p)-'0'); }
#line 30 "lex/parser_clone.rl"
	{ set_args(res, ARG_TYPE_LT, id, -1, step); }
	goto st42;
st42:
	if ( ++p == pe )
		goto _test_eof42;
case 42:
#line 558 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
	if ( (*p) == 124 )
		goto st7;
	goto st0;
st7:
	if ( ++p == pe )
		goto _test_eof7;
case 7:
	if ( 49 <= (*p) && (*p) <= 57 )
		goto tr17;
	goto st0;
tr17:
#line 20 "lex/parser_clone.rl"
	{ step = (*p)-'0'; }
#line 30 "lex/parser_clone.rl"
	{ set_args(res, ARG_TYPE_LT, id, -1, step); }
	goto st43;
st43:
	if ( ++p == pe )
		goto _test_eof43;
case 43:
#line 579 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr53;
	goto st0;
tr53:
#line 20 "lex/parser_clone.rl"
	{ step = 10*step+((*p)-'0'); }
#line 30 "lex/parser_clone.rl"
	{ set_args(res, ARG_TYPE_LT, id, -1, step); }
	goto st44;
st44:
	if ( ++p == pe )
		goto _test_eof44;
case 44:
#line 593 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr54;
	goto st0;
tr54:
#line 20 "lex/parser_clone.rl"
	{ step = 10*step+((*p)-'0'); }
#line 30 "lex/parser_clone.rl"
	{ set_args(res, ARG_TYPE_LT, id, -1, step); }
	goto st45;
st45:
	if ( ++p == pe )
		goto _test_eof45;
case 45:
#line 607 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr55;
	goto st0;
tr55:
#line 20 "lex/parser_clone.rl"
	{ step = 10*step+((*p)-'0'); }
#line 30 "lex/parser_clone.rl"
	{ set_args(res, ARG_TYPE_LT, id, -1, step); }
	goto st46;
st46:
	if ( ++p == pe )
		goto _test_eof46;
case 46:
#line 621 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr56;
	goto st0;
tr15:
#line 33 "lex/parser_clone.rl"
	{ set_args(res, ARG_TYPE_SPREAD, id0, -1, step); }
	goto st47;
st47:
	if ( ++p == pe )
		goto _test_eof47;
case 47:
#line 633 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
	if ( (*p) == 124 )
		goto st10;
	if ( 49 <= (*p) && (*p) <= 57 )
		goto tr57;
	goto st0;
tr57:
#line 17 "lex/parser_clone.rl"
	{ id = (*p)-'0'; }
#line 18 "lex/parser_clone.rl"
	{ id0 = id; }
#line 33 "lex/parser_clone.rl"
	{ set_args(res, ARG_TYPE_SPREAD, id0, -1, step); }
	goto st48;
st48:
	if ( ++p == pe )
		goto _test_eof48;
case 48:
#line 651 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
	switch( (*p) ) {
		case 45: goto st8;
		case 124: goto st10;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr60;
	goto st0;
st8:
	if ( ++p == pe )
		goto _test_eof8;
case 8:
	if ( 49 <= (*p) && (*p) <= 57 )
		goto tr18;
	goto st0;
tr18:
#line 17 "lex/parser_clone.rl"
	{ id = (*p)-'0'; }
#line 19 "lex/parser_clone.rl"
	{ id1 = id; }
#line 34 "lex/parser_clone.rl"
	{ set_args(res, ARG_TYPE_SPREAD, id0, id1, step); }
	goto st49;
st49:
	if ( ++p == pe )
		goto _test_eof49;
case 49:
#line 678 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
	if ( (*p) == 124 )
		goto st9;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr61;
	goto st0;
tr61:
#line 17 "lex/parser_clone.rl"
	{ id = 10*id+((*p)-'0'); }
#line 19 "lex/parser_clone.rl"
	{ id1 = id; }
#line 34 "lex/parser_clone.rl"
	{ set_args(res, ARG_TYPE_SPREAD, id0, id1, step); }
	goto st50;
st50:
	if ( ++p == pe )
		goto _test_eof50;
case 50:
#line 696 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
	if ( (*p) == 124 )
		goto st9;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr63;
	goto st0;
tr63:
#line 17 "lex/parser_clone.rl"
	{ id = 10*id+((*p)-'0'); }
#line 19 "lex/parser_clone.rl"
	{ id1 = id; }
#line 34 "lex/parser_clone.rl"
	{ set_args(res, ARG_TYPE_SPREAD, id0, id1, step); }
	goto st51;
st51:
	if ( ++p == pe )
		goto _test_eof51;
case 51:
#line 714 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
	if ( (*p) == 124 )
		goto st9;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr64;
	goto st0;
tr64:
#line 17 "lex/parser_clone.rl"
	{ id = 10*id+((*p)-'0'); }
#line 19 "lex/parser_clone.rl"
	{ id1 = id; }
#line 34 "lex/parser_clone.rl"
	{ set_args(res, ARG_TYPE_SPREAD, id0, id1, step); }
	goto st52;
st52:
	if ( ++p == pe )
		goto _test_eof52;
case 52:
#line 732 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
	if ( (*p) == 124 )
		goto st9;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr65;
	goto st0;
tr65:
#line 17 "lex/parser_clone.rl"
	{ id = 10*id+((*p)-'0'); }
#line 19 "lex/parser_clone.rl"
	{ id1 = id; }
#line 34 "lex/parser_clone.rl"
	{ set_args(res, ARG_TYPE_SPREAD, id0, id1, step); }
	goto st53;
st53:
	if ( ++p == pe )
		goto _test_eof53;
case 53:
#line 750 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
	if ( (*p) == 124 )
		goto st9;
	goto st0;
st9:
	if ( ++p == pe )
		goto _test_eof9;
case 9:
	if ( 49 <= (*p) && (*p) <= 57 )
		goto tr19;
	goto st0;
tr19:
#line 20 "lex/parser_clone.rl"
	{ step = (*p)-'0'; }
#line 34 "lex/parser_clone.rl"
	{ set_args(res, ARG_TYPE_SPREAD, id0, id1, step); }
	goto st54;
st54:
	if ( ++p == pe )
		goto _test_eof54;
case 54:
#line 771 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr66;
	goto st0;
tr66:
#line 20 "lex/parser_clone.rl"
	{ step = 10*step+((*p)-'0'); }
#line 34 "lex/parser_clone.rl"
	{ set_args(res, ARG_TYPE_SPREAD, id0, id1, step); }
	goto st55;
st55:
	if ( ++p == pe )
		goto _test_eof55;
case 55:
#line 785 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr67;
	goto st0;
tr67:
#line 20 "lex/parser_clone.rl"
	{ step = 10*step+((*p)-'0'); }
#line 34 "lex/parser_clone.rl"
	{ set_args(res, ARG_TYPE_SPREAD, id0, id1, step); }
	goto st56;
st56:
	if ( ++p == pe )
		goto _test_eof56;
case 56:
#line 799 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr68;
	goto st0;
tr68:
#line 20 "lex/parser_clone.rl"
	{ step = 10*step+((*p)-'0'); }
#line 34 "lex/parser_clone.rl"
	{ set_args(res, ARG_TYPE_SPREAD, id0, id1, step); }
	goto st57;
st57:
	if ( ++p == pe )
		goto _test_eof57;
case 57:
#line 813 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr69;
	goto st0;
tr60:
#line 17 "lex/parser_clone.rl"
	{ id = 10*id+((*p)-'0'); }
#line 18 "lex/parser_clone.rl"
	{ id0 = id; }
#line 33 "lex/parser_clone.rl"
	{ set_args(res, ARG_TYPE_SPREAD, id0, -1, step); }
	goto st58;
st58:
	if ( ++p == pe )
		goto _test_eof58;
case 58:
#line 829 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
	switch( (*p) ) {
		case 45: goto st8;
		case 124: goto st10;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr70;
	goto st0;
tr70:
#line 17 "lex/parser_clone.rl"
	{ id = 10*id+((*p)-'0'); }
#line 18 "lex/parser_clone.rl"
	{ id0 = id; }
#line 33 "lex/parser_clone.rl"
	{ set_args(res, ARG_TYPE_SPREAD, id0, -1, step); }
	goto st59;
st59:
	if ( ++p == pe )
		goto _test_eof59;
case 59:
#line 849 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
	switch( (*p) ) {
		case 45: goto st8;
		case 124: goto st10;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr71;
	goto st0;
tr71:
#line 17 "lex/parser_clone.rl"
	{ id = 10*id+((*p)-'0'); }
#line 18 "lex/parser_clone.rl"
	{ id0 = id; }
#line 33 "lex/parser_clone.rl"
	{ set_args(res, ARG_TYPE_SPREAD, id0, -1, step); }
	goto st60;
st60:
	if ( ++p == pe )
		goto _test_eof60;
case 60:
#line 869 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
	switch( (*p) ) {
		case 45: goto st8;
		case 124: goto st10;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr72;
	goto st0;
tr72:
#line 17 "lex/parser_clone.rl"
	{ id = 10*id+((*p)-'0'); }
#line 18 "lex/parser_clone.rl"
	{ id0 = id; }
#line 33 "lex/parser_clone.rl"
	{ set_args(res, ARG_TYPE_SPREAD, id0, -1, step); }
	goto st61;
st61:
	if ( ++p == pe )
		goto _test_eof61;
case 61:
#line 889 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
	switch( (*p) ) {
		case 45: goto st8;
		case 124: goto st10;
	}
	goto st0;
st10:
	if ( ++p == pe )
		goto _test_eof10;
case 10:
	if ( 49 <= (*p) && (*p) <= 57 )
		goto tr20;
	goto st0;
tr20:
#line 20 "lex/parser_clone.rl"
	{ step = (*p)-'0'; }
#line 33 "lex/parser_clone.rl"
	{ set_args(res, ARG_TYPE_SPREAD, id0, -1, step); }
	goto st62;
st62:
	if ( ++p == pe )
		goto _test_eof62;
case 62:
#line 912 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr73;
	goto st0;
tr73:
#line 20 "lex/parser_clone.rl"
	{ step = 10*step+((*p)-'0'); }
#line 33 "lex/parser_clone.rl"
	{ set_args(res, ARG_TYPE_SPREAD, id0, -1, step); }
	goto st63;
st63:
	if ( ++p == pe )
		goto _test_eof63;
case 63:
#line 926 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr74;
	goto st0;
tr74:
#line 20 "lex/parser_clone.rl"
	{ step = 10*step+((*p)-'0'); }
#line 33 "lex/parser_clone.rl"
	{ set_args(res, ARG_TYPE_SPREAD, id0, -1, step); }
	goto st64;
st64:
	if ( ++p == pe )
		goto _test_eof64;
case 64:
#line 940 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr75;
	goto st0;
tr75:
#line 20 "lex/parser_clone.rl"
	{ step = 10*step+((*p)-'0'); }
#line 33 "lex/parser_clone.rl"
	{ set_args(res, ARG_TYPE_SPREAD, id0, -1, step); }
	goto st65;
st65:
	if ( ++p == pe )
		goto _test_eof65;
case 65:
#line 954 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr76;
	goto st0;
st11:
	if ( ++p == pe )
		goto _test_eof11;
case 11:
	if ( 49 <= (*p) && (*p) <= 57 )
		goto tr21;
	goto st0;
tr21:
#line 17 "lex/parser_clone.rl"
	{ id = (*p)-'0'; }
#line 31 "lex/parser_clone.rl"
	{ set_args(res, ARG_TYPE_LE, id, -1, step); }
	goto st66;
st66:
	if ( ++p == pe )
		goto _test_eof66;
case 66:
#line 975 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
	if ( (*p) == 124 )
		goto st12;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr77;
	goto st0;
tr77:
#line 17 "lex/parser_clone.rl"
	{ id = 10*id+((*p)-'0'); }
#line 31 "lex/parser_clone.rl"
	{ set_args(res, ARG_TYPE_LE, id, -1, step); }
	goto st67;
st67:
	if ( ++p == pe )
		goto _test_eof67;
case 67:
#line 991 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
	if ( (*p) == 124 )
		goto st12;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr79;
	goto st0;
tr79:
#line 17 "lex/parser_clone.rl"
	{ id = 10*id+((*p)-'0'); }
#line 31 "lex/parser_clone.rl"
	{ set_args(res, ARG_TYPE_LE, id, -1, step); }
	goto st68;
st68:
	if ( ++p == pe )
		goto _test_eof68;
case 68:
#line 1007 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
	if ( (*p) == 124 )
		goto st12;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr80;
	goto st0;
tr80:
#line 17 "lex/parser_clone.rl"
	{ id = 10*id+((*p)-'0'); }
#line 31 "lex/parser_clone.rl"
	{ set_args(res, ARG_TYPE_LE, id, -1, step); }
	goto st69;
st69:
	if ( ++p == pe )
		goto _test_eof69;
case 69:
#line 1023 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
	if ( (*p) == 124 )
		goto st12;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr81;
	goto st0;
tr81:
#line 17 "lex/parser_clone.rl"
	{ id = 10*id+((*p)-'0'); }
#line 31 "lex/parser_clone.rl"
	{ set_args(res, ARG_TYPE_LE, id, -1, step); }
	goto st70;
st70:
	if ( ++p == pe )
		goto _test_eof70;
case 70:
#line 1039 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
	if ( (*p) == 124 )
		goto st12;
	goto st0;
st12:
	if ( ++p == pe )
		goto _test_eof12;
case 12:
	if ( 49 <= (*p) && (*p) <= 57 )
		goto tr22;
	goto st0;
tr22:
#line 20 "lex/parser_clone.rl"
	{ step = (*p)-'0'; }
#line 31 "lex/parser_clone.rl"
	{ set_args(res, ARG_TYPE_LE, id, -1, step); }
	goto st71;
st71:
	if ( ++p == pe )
		goto _test_eof71;
case 71:
#line 1060 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr82;
	goto st0;
tr82:
#line 20 "lex/parser_clone.rl"
	{ step = 10*step+((*p)-'0'); }
#line 31 "lex/parser_clone.rl"
	{ set_args(res, ARG_TYPE_LE, id, -1, step); }
	goto st72;
st72:
	if ( ++p == pe )
		goto _test_eof72;
case 72:
#line 1074 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr83;
	goto st0;
tr83:
#line 20 "lex/parser_clone.rl"
	{ step = 10*step+((*p)-'0'); }
#line 31 "lex/parser_clone.rl"
	{ set_args(res, ARG_TYPE_LE, id, -1, step); }
	goto st73;
st73:
	if ( ++p == pe )
		goto _test_eof73;
case 73:
#line 1088 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr84;
	goto st0;
tr84:
#line 20 "lex/parser_clone.rl"
	{ step = 10*step+((*p)-'0'); }
#line 31 "lex/parser_clone.rl"
	{ set_args(res, ARG_TYPE_LE, id, -1, step); }
	goto st74;
st74:
	if ( ++p == pe )
		goto _test_eof74;
case 74:
#line 1102 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr85;
	goto st0;
st13:
	if ( ++p == pe )
		goto _test_eof13;
case 13:
	if ( 49 <= (*p) && (*p) <= 57 )
		goto tr23;
	goto st0;
tr23:
#line 17 "lex/parser_clone.rl"
	{ id = (*p)-'0'; }
#line 27 "lex/parser_clone.rl"
	{ set_args(res, ARG_TYPE_EQ, id, -1, 1); }
	goto st75;
st75:
	if ( ++p == pe )
		goto _test_eof75;
case 75:
#line 1123 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr86;
	goto st0;
tr86:
#line 17 "lex/parser_clone.rl"
	{ id = 10*id+((*p)-'0'); }
#line 27 "lex/parser_clone.rl"
	{ set_args(res, ARG_TYPE_EQ, id, -1, 1); }
	goto st76;
st76:
	if ( ++p == pe )
		goto _test_eof76;
case 76:
#line 1137 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr87;
	goto st0;
tr87:
#line 17 "lex/parser_clone.rl"
	{ id = 10*id+((*p)-'0'); }
#line 27 "lex/parser_clone.rl"
	{ set_args(res, ARG_TYPE_EQ, id, -1, 1); }
	goto st77;
st77:
	if ( ++p == pe )
		goto _test_eof77;
case 77:
#line 1151 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr88;
	goto st0;
tr88:
#line 17 "lex/parser_clone.rl"
	{ id = 10*id+((*p)-'0'); }
#line 27 "lex/parser_clone.rl"
	{ set_args(res, ARG_TYPE_EQ, id, -1, 1); }
	goto st78;
st78:
	if ( ++p == pe )
		goto _test_eof78;
case 78:
#line 1165 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr89;
	goto st0;
st14:
	if ( ++p == pe )
		goto _test_eof14;
case 14:
	if ( (*p) == 61 )
		goto st16;
	if ( 49 <= (*p) && (*p) <= 57 )
		goto tr24;
	goto st0;
tr24:
#line 17 "lex/parser_clone.rl"
	{ id = (*p)-'0'; }
#line 28 "lex/parser_clone.rl"
	{ set_args(res, ARG_TYPE_GT, id, -1, step); }
	goto st79;
st79:
	if ( ++p == pe )
		goto _test_eof79;
case 79:
#line 1188 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
	if ( (*p) == 124 )
		goto st15;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr90;
	goto st0;
tr90:
#line 17 "lex/parser_clone.rl"
	{ id = 10*id+((*p)-'0'); }
#line 28 "lex/parser_clone.rl"
	{ set_args(res, ARG_TYPE_GT, id, -1, step); }
	goto st80;
st80:
	if ( ++p == pe )
		goto _test_eof80;
case 80:
#line 1204 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
	if ( (*p) == 124 )
		goto st15;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr92;
	goto st0;
tr92:
#line 17 "lex/parser_clone.rl"
	{ id = 10*id+((*p)-'0'); }
#line 28 "lex/parser_clone.rl"
	{ set_args(res, ARG_TYPE_GT, id, -1, step); }
	goto st81;
st81:
	if ( ++p == pe )
		goto _test_eof81;
case 81:
#line 1220 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
	if ( (*p) == 124 )
		goto st15;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr93;
	goto st0;
tr93:
#line 17 "lex/parser_clone.rl"
	{ id = 10*id+((*p)-'0'); }
#line 28 "lex/parser_clone.rl"
	{ set_args(res, ARG_TYPE_GT, id, -1, step); }
	goto st82;
st82:
	if ( ++p == pe )
		goto _test_eof82;
case 82:
#line 1236 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
	if ( (*p) == 124 )
		goto st15;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr94;
	goto st0;
tr94:
#line 17 "lex/parser_clone.rl"
	{ id = 10*id+((*p)-'0'); }
#line 28 "lex/parser_clone.rl"
	{ set_args(res, ARG_TYPE_GT, id, -1, step); }
	goto st83;
st83:
	if ( ++p == pe )
		goto _test_eof83;
case 83:
#line 1252 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
	if ( (*p) == 124 )
		goto st15;
	goto st0;
st15:
	if ( ++p == pe )
		goto _test_eof15;
case 15:
	if ( 49 <= (*p) && (*p) <= 57 )
		goto tr26;
	goto st0;
tr26:
#line 20 "lex/parser_clone.rl"
	{ step = (*p)-'0'; }
#line 28 "lex/parser_clone.rl"
	{ set_args(res, ARG_TYPE_GT, id, -1, step); }
	goto st84;
st84:
	if ( ++p == pe )
		goto _test_eof84;
case 84:
#line 1273 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr95;
	goto st0;
tr95:
#line 20 "lex/parser_clone.rl"
	{ step = 10*step+((*p)-'0'); }
#line 28 "lex/parser_clone.rl"
	{ set_args(res, ARG_TYPE_GT, id, -1, step); }
	goto st85;
st85:
	if ( ++p == pe )
		goto _test_eof85;
case 85:
#line 1287 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr96;
	goto st0;
tr96:
#line 20 "lex/parser_clone.rl"
	{ step = 10*step+((*p)-'0'); }
#line 28 "lex/parser_clone.rl"
	{ set_args(res, ARG_TYPE_GT, id, -1, step); }
	goto st86;
st86:
	if ( ++p == pe )
		goto _test_eof86;
case 86:
#line 1301 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr97;
	goto st0;
tr97:
#line 20 "lex/parser_clone.rl"
	{ step = 10*step+((*p)-'0'); }
#line 28 "lex/parser_clone.rl"
	{ set_args(res, ARG_TYPE_GT, id, -1, step); }
	goto st87;
st87:
	if ( ++p == pe )
		goto _test_eof87;
case 87:
#line 1315 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr98;
	goto st0;
st16:
	if ( ++p == pe )
		goto _test_eof16;
case 16:
	if ( 49 <= (*p) && (*p) <= 57 )
		goto tr27;
	goto st0;
tr27:
#line 17 "lex/parser_clone.rl"
	{ id = (*p)-'0'; }
#line 29 "lex/parser_clone.rl"
	{ set_args(res, ARG_TYPE_GE, id, -1, step); }
	goto st88;
st88:
	if ( ++p == pe )
		goto _test_eof88;
case 88:
#line 1336 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
	if ( (*p) == 124 )
		goto st17;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr99;
	goto st0;
tr99:
#line 17 "lex/parser_clone.rl"
	{ id = 10*id+((*p)-'0'); }
#line 29 "lex/parser_clone.rl"
	{ set_args(res, ARG_TYPE_GE, id, -1, step); }
	goto st89;
st89:
	if ( ++p == pe )
		goto _test_eof89;
case 89:
#line 1352 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
	if ( (*p) == 124 )
		goto st17;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr101;
	goto st0;
tr101:
#line 17 "lex/parser_clone.rl"
	{ id = 10*id+((*p)-'0'); }
#line 29 "lex/parser_clone.rl"
	{ set_args(res, ARG_TYPE_GE, id, -1, step); }
	goto st90;
st90:
	if ( ++p == pe )
		goto _test_eof90;
case 90:
#line 1368 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
	if ( (*p) == 124 )
		goto st17;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr102;
	goto st0;
tr102:
#line 17 "lex/parser_clone.rl"
	{ id = 10*id+((*p)-'0'); }
#line 29 "lex/parser_clone.rl"
	{ set_args(res, ARG_TYPE_GE, id, -1, step); }
	goto st91;
st91:
	if ( ++p == pe )
		goto _test_eof91;
case 91:
#line 1384 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
	if ( (*p) == 124 )
		goto st17;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr103;
	goto st0;
tr103:
#line 17 "lex/parser_clone.rl"
	{ id = 10*id+((*p)-'0'); }
#line 29 "lex/parser_clone.rl"
	{ set_args(res, ARG_TYPE_GE, id, -1, step); }
	goto st92;
st92:
	if ( ++p == pe )
		goto _test_eof92;
case 92:
#line 1400 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
	if ( (*p) == 124 )
		goto st17;
	goto st0;
st17:
	if ( ++p == pe )
		goto _test_eof17;
case 17:
	if ( 49 <= (*p) && (*p) <= 57 )
		goto tr28;
	goto st0;
tr28:
#line 20 "lex/parser_clone.rl"
	{ step = (*p)-'0'; }
#line 29 "lex/parser_clone.rl"
	{ set_args(res, ARG_TYPE_GE, id, -1, step); }
	goto st93;
st93:
	if ( ++p == pe )
		goto _test_eof93;
case 93:
#line 1421 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr104;
	goto st0;
tr104:
#line 20 "lex/parser_clone.rl"
	{ step = 10*step+((*p)-'0'); }
#line 29 "lex/parser_clone.rl"
	{ set_args(res, ARG_TYPE_GE, id, -1, step); }
	goto st94;
st94:
	if ( ++p == pe )
		goto _test_eof94;
case 94:
#line 1435 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr105;
	goto st0;
tr105:
#line 20 "lex/parser_clone.rl"
	{ step = 10*step+((*p)-'0'); }
#line 29 "lex/parser_clone.rl"
	{ set_args(res, ARG_TYPE_GE, id, -1, step); }
	goto st95;
st95:
	if ( ++p == pe )
		goto _test_eof95;
case 95:
#line 1449 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr106;
	goto st0;
tr106:
#line 20 "lex/parser_clone.rl"
	{ step = 10*step+((*p)-'0'); }
#line 29 "lex/parser_clone.rl"
	{ set_args(res, ARG_TYPE_GE, id, -1, step); }
	goto st96;
st96:
	if ( ++p == pe )
		goto _test_eof96;
case 96:
#line 1463 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr107;
	goto st0;
tr2:
#line 39 "lex/parser_clone.rl"
	{ res.msg_type = MSG_TYPE_DSP_TOGGLE; }
	goto st18;
st18:
	if ( ++p == pe )
		goto _test_eof18;
case 18:
#line 1475 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
	if ( (*p) == 126 )
		goto tr29;
	goto st0;
	}
	_test_eof2: cs = 2; goto _test_eof; 
	_test_eof3: cs = 3; goto _test_eof; 
	_test_eof19: cs = 19; goto _test_eof; 
	_test_eof20: cs = 20; goto _test_eof; 
	_test_eof21: cs = 21; goto _test_eof; 
	_test_eof22: cs = 22; goto _test_eof; 
	_test_eof23: cs = 23; goto _test_eof; 
	_test_eof24: cs = 24; goto _test_eof; 
	_test_eof4: cs = 4; goto _test_eof; 
	_test_eof25: cs = 25; goto _test_eof; 
	_test_eof26: cs = 26; goto _test_eof; 
	_test_eof27: cs = 27; goto _test_eof; 
	_test_eof28: cs = 28; goto _test_eof; 
	_test_eof29: cs = 29; goto _test_eof; 
	_test_eof5: cs = 5; goto _test_eof; 
	_test_eof30: cs = 30; goto _test_eof; 
	_test_eof31: cs = 31; goto _test_eof; 
	_test_eof32: cs = 32; goto _test_eof; 
	_test_eof33: cs = 33; goto _test_eof; 
	_test_eof34: cs = 34; goto _test_eof; 
	_test_eof35: cs = 35; goto _test_eof; 
	_test_eof36: cs = 36; goto _test_eof; 
	_test_eof37: cs = 37; goto _test_eof; 
	_test_eof6: cs = 6; goto _test_eof; 
	_test_eof38: cs = 38; goto _test_eof; 
	_test_eof39: cs = 39; goto _test_eof; 
	_test_eof40: cs = 40; goto _test_eof; 
	_test_eof41: cs = 41; goto _test_eof; 
	_test_eof42: cs = 42; goto _test_eof; 
	_test_eof7: cs = 7; goto _test_eof; 
	_test_eof43: cs = 43; goto _test_eof; 
	_test_eof44: cs = 44; goto _test_eof; 
	_test_eof45: cs = 45; goto _test_eof; 
	_test_eof46: cs = 46; goto _test_eof; 
	_test_eof47: cs = 47; goto _test_eof; 
	_test_eof48: cs = 48; goto _test_eof; 
	_test_eof8: cs = 8; goto _test_eof; 
	_test_eof49: cs = 49; goto _test_eof; 
	_test_eof50: cs = 50; goto _test_eof; 
	_test_eof51: cs = 51; goto _test_eof; 
	_test_eof52: cs = 52; goto _test_eof; 
	_test_eof53: cs = 53; goto _test_eof; 
	_test_eof9: cs = 9; goto _test_eof; 
	_test_eof54: cs = 54; goto _test_eof; 
	_test_eof55: cs = 55; goto _test_eof; 
	_test_eof56: cs = 56; goto _test_eof; 
	_test_eof57: cs = 57; goto _test_eof; 
	_test_eof58: cs = 58; goto _test_eof; 
	_test_eof59: cs = 59; goto _test_eof; 
	_test_eof60: cs = 60; goto _test_eof; 
	_test_eof61: cs = 61; goto _test_eof; 
	_test_eof10: cs = 10; goto _test_eof; 
	_test_eof62: cs = 62; goto _test_eof; 
	_test_eof63: cs = 63; goto _test_eof; 
	_test_eof64: cs = 64; goto _test_eof; 
	_test_eof65: cs = 65; goto _test_eof; 
	_test_eof11: cs = 11; goto _test_eof; 
	_test_eof66: cs = 66; goto _test_eof; 
	_test_eof67: cs = 67; goto _test_eof; 
	_test_eof68: cs = 68; goto _test_eof; 
	_test_eof69: cs = 69; goto _test_eof; 
	_test_eof70: cs = 70; goto _test_eof; 
	_test_eof12: cs = 12; goto _test_eof; 
	_test_eof71: cs = 71; goto _test_eof; 
	_test_eof72: cs = 72; goto _test_eof; 
	_test_eof73: cs = 73; goto _test_eof; 
	_test_eof74: cs = 74; goto _test_eof; 
	_test_eof13: cs = 13; goto _test_eof; 
	_test_eof75: cs = 75; goto _test_eof; 
	_test_eof76: cs = 76; goto _test_eof; 
	_test_eof77: cs = 77; goto _test_eof; 
	_test_eof78: cs = 78; goto _test_eof; 
	_test_eof14: cs = 14; goto _test_eof; 
	_test_eof79: cs = 79; goto _test_eof; 
	_test_eof80: cs = 80; goto _test_eof; 
	_test_eof81: cs = 81; goto _test_eof; 
	_test_eof82: cs = 82; goto _test_eof; 
	_test_eof83: cs = 83; goto _test_eof; 
	_test_eof15: cs = 15; goto _test_eof; 
	_test_eof84: cs = 84; goto _test_eof; 
	_test_eof85: cs = 85; goto _test_eof; 
	_test_eof86: cs = 86; goto _test_eof; 
	_test_eof87: cs = 87; goto _test_eof; 
	_test_eof16: cs = 16; goto _test_eof; 
	_test_eof88: cs = 88; goto _test_eof; 
	_test_eof89: cs = 89; goto _test_eof; 
	_test_eof90: cs = 90; goto _test_eof; 
	_test_eof91: cs = 91; goto _test_eof; 
	_test_eof92: cs = 92; goto _test_eof; 
	_test_eof17: cs = 17; goto _test_eof; 
	_test_eof93: cs = 93; goto _test_eof; 
	_test_eof94: cs = 94; goto _test_eof; 
	_test_eof95: cs = 95; goto _test_eof; 
	_test_eof96: cs = 96; goto _test_eof; 
	_test_eof18: cs = 18; goto _test_eof; 

	_test_eof: {}
	_out: {}
	}

#line 76 "lex/parser_clone.rl"

    return cs >= 19;
}

}
}
