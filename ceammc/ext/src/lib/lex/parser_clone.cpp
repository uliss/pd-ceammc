
#line 1 "lex/parser_clone.rl"
# include "parser_clone.h"
# include <cstring>

namespace ceammc {
namespace parser {

static inline void set_args(CloneMessage& res, ArgumentType type, int16_t first, int16_t last, int16_t step, int8_t inlet) {
    res.first = first;
    res.last = last;
    res.step = step;
    res.type = type;
    res.inlet = inlet;
}


#line 19 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
static const int clone_target_start = 1;
static const int clone_target_first_final = 22;
static const int clone_target_error = 0;

static const int clone_target_en_main = 1;


#line 51 "lex/parser_clone.rl"


bool parse_clone_action(const char* str, CloneMessage& res)
{
    const auto len = strlen(str);
    if (len == 0) {
        res.type = ARG_TYPE_NONE;
        return false;
    }

    int cs = 0;
    const char* p = str;
    const char* pe = p + len;
    const char* eof = pe;

    int16_t id = 0, id0 = 1, id1 = -1, step = 1, inlet = -1;

    
#line 46 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
	{
	cs = clone_target_start;
	}

#line 69 "lex/parser_clone.rl"
    
#line 53 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
	{
	if ( p == pe )
		goto _test_eof;
	switch ( cs )
	{
case 1:
	if ( (*p) == 35 )
		goto st2;
	goto st0;
st0:
cs = 0;
	goto _out;
st2:
	if ( ++p == pe )
		goto _test_eof2;
case 2:
	switch( (*p) ) {
		case 33: goto st3;
		case 42: goto tr3;
		case 48: goto tr4;
		case 60: goto st12;
		case 61: goto st16;
		case 62: goto st17;
		case 63: goto tr9;
	}
	if ( 49 <= (*p) && (*p) <= 57 )
		goto tr5;
	goto st0;
st3:
	if ( ++p == pe )
		goto _test_eof3;
case 3:
	if ( (*p) == 48 )
		goto tr10;
	if ( 49 <= (*p) && (*p) <= 57 )
		goto tr11;
	goto st0;
tr10:
#line 18 "lex/parser_clone.rl"
	{ id = 0; }
#line 29 "lex/parser_clone.rl"
	{ set_args(res, ARG_TYPE_EXCEPT, id, -1, 1,    inlet); }
	goto st22;
tr54:
#line 19 "lex/parser_clone.rl"
	{ id = 10*id+((*p)-'0'); }
#line 29 "lex/parser_clone.rl"
	{ set_args(res, ARG_TYPE_EXCEPT, id, -1, 1,    inlet); }
	goto st22;
st22:
	if ( ++p == pe )
		goto _test_eof22;
case 22:
#line 107 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
	if ( (*p) == 124 )
		goto st4;
	goto st0;
st4:
	if ( ++p == pe )
		goto _test_eof4;
case 4:
	if ( (*p) == 48 )
		goto tr12;
	if ( 49 <= (*p) && (*p) <= 57 )
		goto tr13;
	goto st0;
tr12:
#line 24 "lex/parser_clone.rl"
	{ inlet = 0; }
#line 29 "lex/parser_clone.rl"
	{ set_args(res, ARG_TYPE_EXCEPT, id, -1, 1,    inlet); }
	goto st23;
tr14:
#line 24 "lex/parser_clone.rl"
	{ inlet = 0; }
#line 27 "lex/parser_clone.rl"
	{ set_args(res, ARG_TYPE_ALL,    -1, -1, 1,    inlet); }
	goto st23;
tr20:
#line 24 "lex/parser_clone.rl"
	{ inlet = 0; }
#line 36 "lex/parser_clone.rl"
	{ set_args(res, ARG_TYPE_RANGE, id0, id1, step, inlet); }
	goto st23;
tr23:
#line 24 "lex/parser_clone.rl"
	{ inlet = 0; }
#line 30 "lex/parser_clone.rl"
	{ set_args(res, ARG_TYPE_EQ,     id, -1, step, inlet); }
	goto st23;
tr28:
#line 24 "lex/parser_clone.rl"
	{ inlet = 0; }
#line 33 "lex/parser_clone.rl"
	{ set_args(res, ARG_TYPE_LT,     id, -1, 1,    inlet); }
	goto st23;
tr32:
#line 24 "lex/parser_clone.rl"
	{ inlet = 0; }
#line 34 "lex/parser_clone.rl"
	{ set_args(res, ARG_TYPE_LE,     id, -1, 1,    inlet); }
	goto st23;
tr39:
#line 24 "lex/parser_clone.rl"
	{ inlet = 0; }
#line 31 "lex/parser_clone.rl"
	{ set_args(res, ARG_TYPE_GT,     id, -1, 1,    inlet); }
	goto st23;
tr43:
#line 24 "lex/parser_clone.rl"
	{ inlet = 0; }
#line 32 "lex/parser_clone.rl"
	{ set_args(res, ARG_TYPE_GE,     id, -1, 1,    inlet); }
	goto st23;
tr45:
#line 24 "lex/parser_clone.rl"
	{ inlet = 0; }
#line 28 "lex/parser_clone.rl"
	{ set_args(res, ARG_TYPE_RANDOM, -1, -1, 1,    inlet); }
	goto st23;
tr50:
#line 25 "lex/parser_clone.rl"
	{ inlet = 10*inlet+((*p)-'0'); }
#line 29 "lex/parser_clone.rl"
	{ set_args(res, ARG_TYPE_EXCEPT, id, -1, 1,    inlet); }
	goto st23;
tr58:
#line 25 "lex/parser_clone.rl"
	{ inlet = 10*inlet+((*p)-'0'); }
#line 27 "lex/parser_clone.rl"
	{ set_args(res, ARG_TYPE_ALL,    -1, -1, 1,    inlet); }
	goto st23;
tr70:
#line 25 "lex/parser_clone.rl"
	{ inlet = 10*inlet+((*p)-'0'); }
#line 36 "lex/parser_clone.rl"
	{ set_args(res, ARG_TYPE_RANGE, id0, id1, step, inlet); }
	goto st23;
tr81:
#line 25 "lex/parser_clone.rl"
	{ inlet = 10*inlet+((*p)-'0'); }
#line 30 "lex/parser_clone.rl"
	{ set_args(res, ARG_TYPE_EQ,     id, -1, step, inlet); }
	goto st23;
tr89:
#line 25 "lex/parser_clone.rl"
	{ inlet = 10*inlet+((*p)-'0'); }
#line 33 "lex/parser_clone.rl"
	{ set_args(res, ARG_TYPE_LT,     id, -1, 1,    inlet); }
	goto st23;
tr97:
#line 25 "lex/parser_clone.rl"
	{ inlet = 10*inlet+((*p)-'0'); }
#line 34 "lex/parser_clone.rl"
	{ set_args(res, ARG_TYPE_LE,     id, -1, 1,    inlet); }
	goto st23;
tr109:
#line 25 "lex/parser_clone.rl"
	{ inlet = 10*inlet+((*p)-'0'); }
#line 31 "lex/parser_clone.rl"
	{ set_args(res, ARG_TYPE_GT,     id, -1, 1,    inlet); }
	goto st23;
tr117:
#line 25 "lex/parser_clone.rl"
	{ inlet = 10*inlet+((*p)-'0'); }
#line 32 "lex/parser_clone.rl"
	{ set_args(res, ARG_TYPE_GE,     id, -1, 1,    inlet); }
	goto st23;
tr125:
#line 25 "lex/parser_clone.rl"
	{ inlet = 10*inlet+((*p)-'0'); }
#line 28 "lex/parser_clone.rl"
	{ set_args(res, ARG_TYPE_RANDOM, -1, -1, 1,    inlet); }
	goto st23;
st23:
	if ( ++p == pe )
		goto _test_eof23;
case 23:
#line 232 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
	goto st0;
tr13:
#line 25 "lex/parser_clone.rl"
	{ inlet = (*p)-'0'; }
#line 29 "lex/parser_clone.rl"
	{ set_args(res, ARG_TYPE_EXCEPT, id, -1, 1,    inlet); }
	goto st24;
st24:
	if ( ++p == pe )
		goto _test_eof24;
case 24:
#line 244 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr48;
	goto st0;
tr48:
#line 25 "lex/parser_clone.rl"
	{ inlet = 10*inlet+((*p)-'0'); }
#line 29 "lex/parser_clone.rl"
	{ set_args(res, ARG_TYPE_EXCEPT, id, -1, 1,    inlet); }
	goto st25;
st25:
	if ( ++p == pe )
		goto _test_eof25;
case 25:
#line 258 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr49;
	goto st0;
tr49:
#line 25 "lex/parser_clone.rl"
	{ inlet = 10*inlet+((*p)-'0'); }
#line 29 "lex/parser_clone.rl"
	{ set_args(res, ARG_TYPE_EXCEPT, id, -1, 1,    inlet); }
	goto st26;
st26:
	if ( ++p == pe )
		goto _test_eof26;
case 26:
#line 272 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr50;
	goto st0;
tr11:
#line 19 "lex/parser_clone.rl"
	{ id = (*p)-'0'; }
#line 29 "lex/parser_clone.rl"
	{ set_args(res, ARG_TYPE_EXCEPT, id, -1, 1,    inlet); }
	goto st27;
st27:
	if ( ++p == pe )
		goto _test_eof27;
case 27:
#line 286 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
	if ( (*p) == 124 )
		goto st4;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr51;
	goto st0;
tr51:
#line 19 "lex/parser_clone.rl"
	{ id = 10*id+((*p)-'0'); }
#line 29 "lex/parser_clone.rl"
	{ set_args(res, ARG_TYPE_EXCEPT, id, -1, 1,    inlet); }
	goto st28;
st28:
	if ( ++p == pe )
		goto _test_eof28;
case 28:
#line 302 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
	if ( (*p) == 124 )
		goto st4;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr52;
	goto st0;
tr52:
#line 19 "lex/parser_clone.rl"
	{ id = 10*id+((*p)-'0'); }
#line 29 "lex/parser_clone.rl"
	{ set_args(res, ARG_TYPE_EXCEPT, id, -1, 1,    inlet); }
	goto st29;
st29:
	if ( ++p == pe )
		goto _test_eof29;
case 29:
#line 318 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
	if ( (*p) == 124 )
		goto st4;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr53;
	goto st0;
tr53:
#line 19 "lex/parser_clone.rl"
	{ id = 10*id+((*p)-'0'); }
#line 29 "lex/parser_clone.rl"
	{ set_args(res, ARG_TYPE_EXCEPT, id, -1, 1,    inlet); }
	goto st30;
st30:
	if ( ++p == pe )
		goto _test_eof30;
case 30:
#line 334 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
	if ( (*p) == 124 )
		goto st4;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr54;
	goto st0;
tr3:
#line 27 "lex/parser_clone.rl"
	{ set_args(res, ARG_TYPE_ALL,    -1, -1, 1,    inlet); }
	goto st31;
st31:
	if ( ++p == pe )
		goto _test_eof31;
case 31:
#line 348 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
	if ( (*p) == 124 )
		goto st5;
	goto st0;
st5:
	if ( ++p == pe )
		goto _test_eof5;
case 5:
	if ( (*p) == 48 )
		goto tr14;
	if ( 49 <= (*p) && (*p) <= 57 )
		goto tr15;
	goto st0;
tr15:
#line 25 "lex/parser_clone.rl"
	{ inlet = (*p)-'0'; }
#line 27 "lex/parser_clone.rl"
	{ set_args(res, ARG_TYPE_ALL,    -1, -1, 1,    inlet); }
	goto st32;
st32:
	if ( ++p == pe )
		goto _test_eof32;
case 32:
#line 371 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr56;
	goto st0;
tr56:
#line 25 "lex/parser_clone.rl"
	{ inlet = 10*inlet+((*p)-'0'); }
#line 27 "lex/parser_clone.rl"
	{ set_args(res, ARG_TYPE_ALL,    -1, -1, 1,    inlet); }
	goto st33;
st33:
	if ( ++p == pe )
		goto _test_eof33;
case 33:
#line 385 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr57;
	goto st0;
tr57:
#line 25 "lex/parser_clone.rl"
	{ inlet = 10*inlet+((*p)-'0'); }
#line 27 "lex/parser_clone.rl"
	{ set_args(res, ARG_TYPE_ALL,    -1, -1, 1,    inlet); }
	goto st34;
st34:
	if ( ++p == pe )
		goto _test_eof34;
case 34:
#line 399 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr58;
	goto st0;
tr4:
#line 18 "lex/parser_clone.rl"
	{ id = 0; }
#line 30 "lex/parser_clone.rl"
	{ set_args(res, ARG_TYPE_EQ,     id, -1, step, inlet); }
#line 20 "lex/parser_clone.rl"
	{ id0 = id; }
	goto st35;
tr85:
#line 19 "lex/parser_clone.rl"
	{ id = 10*id+((*p)-'0'); }
#line 30 "lex/parser_clone.rl"
	{ set_args(res, ARG_TYPE_EQ,     id, -1, step, inlet); }
#line 20 "lex/parser_clone.rl"
	{ id0 = id; }
	goto st35;
st35:
	if ( ++p == pe )
		goto _test_eof35;
case 35:
#line 423 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
	switch( (*p) ) {
		case 46: goto st6;
		case 58: goto st10;
		case 124: goto st11;
	}
	goto st0;
st6:
	if ( ++p == pe )
		goto _test_eof6;
case 6:
	if ( (*p) == 46 )
		goto st7;
	goto st0;
st7:
	if ( ++p == pe )
		goto _test_eof7;
case 7:
	if ( (*p) == 48 )
		goto tr17;
	if ( 49 <= (*p) && (*p) <= 57 )
		goto tr18;
	goto st0;
tr17:
#line 18 "lex/parser_clone.rl"
	{ id = 0; }
#line 21 "lex/parser_clone.rl"
	{ id1 = id; }
#line 36 "lex/parser_clone.rl"
	{ set_args(res, ARG_TYPE_RANGE, id0, id1, step, inlet); }
	goto st36;
tr74:
#line 19 "lex/parser_clone.rl"
	{ id = 10*id+((*p)-'0'); }
#line 21 "lex/parser_clone.rl"
	{ id1 = id; }
#line 36 "lex/parser_clone.rl"
	{ set_args(res, ARG_TYPE_RANGE, id0, id1, step, inlet); }
	goto st36;
st36:
	if ( ++p == pe )
		goto _test_eof36;
case 36:
#line 466 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
	switch( (*p) ) {
		case 58: goto st8;
		case 124: goto st9;
	}
	goto st0;
st8:
	if ( ++p == pe )
		goto _test_eof8;
case 8:
	if ( 49 <= (*p) && (*p) <= 57 )
		goto tr19;
	goto st0;
tr19:
#line 22 "lex/parser_clone.rl"
	{ step = (*p)-'0'; }
#line 36 "lex/parser_clone.rl"
	{ set_args(res, ARG_TYPE_RANGE, id0, id1, step, inlet); }
	goto st37;
st37:
	if ( ++p == pe )
		goto _test_eof37;
case 37:
#line 489 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
	if ( (*p) == 124 )
		goto st9;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr64;
	goto st0;
tr64:
#line 22 "lex/parser_clone.rl"
	{ step = 10*step+((*p)-'0'); }
#line 36 "lex/parser_clone.rl"
	{ set_args(res, ARG_TYPE_RANGE, id0, id1, step, inlet); }
	goto st38;
st38:
	if ( ++p == pe )
		goto _test_eof38;
case 38:
#line 505 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
	if ( (*p) == 124 )
		goto st9;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr65;
	goto st0;
tr65:
#line 22 "lex/parser_clone.rl"
	{ step = 10*step+((*p)-'0'); }
#line 36 "lex/parser_clone.rl"
	{ set_args(res, ARG_TYPE_RANGE, id0, id1, step, inlet); }
	goto st39;
st39:
	if ( ++p == pe )
		goto _test_eof39;
case 39:
#line 521 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
	if ( (*p) == 124 )
		goto st9;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr66;
	goto st0;
tr66:
#line 22 "lex/parser_clone.rl"
	{ step = 10*step+((*p)-'0'); }
#line 36 "lex/parser_clone.rl"
	{ set_args(res, ARG_TYPE_RANGE, id0, id1, step, inlet); }
	goto st40;
st40:
	if ( ++p == pe )
		goto _test_eof40;
case 40:
#line 537 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
	if ( (*p) == 124 )
		goto st9;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr67;
	goto st0;
tr67:
#line 22 "lex/parser_clone.rl"
	{ step = 10*step+((*p)-'0'); }
#line 36 "lex/parser_clone.rl"
	{ set_args(res, ARG_TYPE_RANGE, id0, id1, step, inlet); }
	goto st41;
st41:
	if ( ++p == pe )
		goto _test_eof41;
case 41:
#line 553 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
	if ( (*p) == 124 )
		goto st9;
	goto st0;
st9:
	if ( ++p == pe )
		goto _test_eof9;
case 9:
	if ( (*p) == 48 )
		goto tr20;
	if ( 49 <= (*p) && (*p) <= 57 )
		goto tr21;
	goto st0;
tr21:
#line 25 "lex/parser_clone.rl"
	{ inlet = (*p)-'0'; }
#line 36 "lex/parser_clone.rl"
	{ set_args(res, ARG_TYPE_RANGE, id0, id1, step, inlet); }
	goto st42;
st42:
	if ( ++p == pe )
		goto _test_eof42;
case 42:
#line 576 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr68;
	goto st0;
tr68:
#line 25 "lex/parser_clone.rl"
	{ inlet = 10*inlet+((*p)-'0'); }
#line 36 "lex/parser_clone.rl"
	{ set_args(res, ARG_TYPE_RANGE, id0, id1, step, inlet); }
	goto st43;
st43:
	if ( ++p == pe )
		goto _test_eof43;
case 43:
#line 590 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr69;
	goto st0;
tr69:
#line 25 "lex/parser_clone.rl"
	{ inlet = 10*inlet+((*p)-'0'); }
#line 36 "lex/parser_clone.rl"
	{ set_args(res, ARG_TYPE_RANGE, id0, id1, step, inlet); }
	goto st44;
st44:
	if ( ++p == pe )
		goto _test_eof44;
case 44:
#line 604 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr70;
	goto st0;
tr18:
#line 19 "lex/parser_clone.rl"
	{ id = (*p)-'0'; }
#line 21 "lex/parser_clone.rl"
	{ id1 = id; }
#line 36 "lex/parser_clone.rl"
	{ set_args(res, ARG_TYPE_RANGE, id0, id1, step, inlet); }
	goto st45;
st45:
	if ( ++p == pe )
		goto _test_eof45;
case 45:
#line 620 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
	switch( (*p) ) {
		case 58: goto st8;
		case 124: goto st9;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr71;
	goto st0;
tr71:
#line 19 "lex/parser_clone.rl"
	{ id = 10*id+((*p)-'0'); }
#line 21 "lex/parser_clone.rl"
	{ id1 = id; }
#line 36 "lex/parser_clone.rl"
	{ set_args(res, ARG_TYPE_RANGE, id0, id1, step, inlet); }
	goto st46;
st46:
	if ( ++p == pe )
		goto _test_eof46;
case 46:
#line 640 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
	switch( (*p) ) {
		case 58: goto st8;
		case 124: goto st9;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr72;
	goto st0;
tr72:
#line 19 "lex/parser_clone.rl"
	{ id = 10*id+((*p)-'0'); }
#line 21 "lex/parser_clone.rl"
	{ id1 = id; }
#line 36 "lex/parser_clone.rl"
	{ set_args(res, ARG_TYPE_RANGE, id0, id1, step, inlet); }
	goto st47;
st47:
	if ( ++p == pe )
		goto _test_eof47;
case 47:
#line 660 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
	switch( (*p) ) {
		case 58: goto st8;
		case 124: goto st9;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr73;
	goto st0;
tr73:
#line 19 "lex/parser_clone.rl"
	{ id = 10*id+((*p)-'0'); }
#line 21 "lex/parser_clone.rl"
	{ id1 = id; }
#line 36 "lex/parser_clone.rl"
	{ set_args(res, ARG_TYPE_RANGE, id0, id1, step, inlet); }
	goto st48;
st48:
	if ( ++p == pe )
		goto _test_eof48;
case 48:
#line 680 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
	switch( (*p) ) {
		case 58: goto st8;
		case 124: goto st9;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr74;
	goto st0;
st10:
	if ( ++p == pe )
		goto _test_eof10;
case 10:
	if ( 49 <= (*p) && (*p) <= 57 )
		goto tr22;
	goto st0;
tr22:
#line 22 "lex/parser_clone.rl"
	{ step = (*p)-'0'; }
#line 30 "lex/parser_clone.rl"
	{ set_args(res, ARG_TYPE_EQ,     id, -1, step, inlet); }
	goto st49;
st49:
	if ( ++p == pe )
		goto _test_eof49;
case 49:
#line 705 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
	if ( (*p) == 124 )
		goto st11;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr75;
	goto st0;
tr75:
#line 22 "lex/parser_clone.rl"
	{ step = 10*step+((*p)-'0'); }
#line 30 "lex/parser_clone.rl"
	{ set_args(res, ARG_TYPE_EQ,     id, -1, step, inlet); }
	goto st50;
st50:
	if ( ++p == pe )
		goto _test_eof50;
case 50:
#line 721 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
	if ( (*p) == 124 )
		goto st11;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr76;
	goto st0;
tr76:
#line 22 "lex/parser_clone.rl"
	{ step = 10*step+((*p)-'0'); }
#line 30 "lex/parser_clone.rl"
	{ set_args(res, ARG_TYPE_EQ,     id, -1, step, inlet); }
	goto st51;
st51:
	if ( ++p == pe )
		goto _test_eof51;
case 51:
#line 737 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
	if ( (*p) == 124 )
		goto st11;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr77;
	goto st0;
tr77:
#line 22 "lex/parser_clone.rl"
	{ step = 10*step+((*p)-'0'); }
#line 30 "lex/parser_clone.rl"
	{ set_args(res, ARG_TYPE_EQ,     id, -1, step, inlet); }
	goto st52;
st52:
	if ( ++p == pe )
		goto _test_eof52;
case 52:
#line 753 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
	if ( (*p) == 124 )
		goto st11;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr78;
	goto st0;
tr78:
#line 22 "lex/parser_clone.rl"
	{ step = 10*step+((*p)-'0'); }
#line 30 "lex/parser_clone.rl"
	{ set_args(res, ARG_TYPE_EQ,     id, -1, step, inlet); }
	goto st53;
st53:
	if ( ++p == pe )
		goto _test_eof53;
case 53:
#line 769 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
	if ( (*p) == 124 )
		goto st11;
	goto st0;
st11:
	if ( ++p == pe )
		goto _test_eof11;
case 11:
	if ( (*p) == 48 )
		goto tr23;
	if ( 49 <= (*p) && (*p) <= 57 )
		goto tr24;
	goto st0;
tr24:
#line 25 "lex/parser_clone.rl"
	{ inlet = (*p)-'0'; }
#line 30 "lex/parser_clone.rl"
	{ set_args(res, ARG_TYPE_EQ,     id, -1, step, inlet); }
	goto st54;
st54:
	if ( ++p == pe )
		goto _test_eof54;
case 54:
#line 792 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr79;
	goto st0;
tr79:
#line 25 "lex/parser_clone.rl"
	{ inlet = 10*inlet+((*p)-'0'); }
#line 30 "lex/parser_clone.rl"
	{ set_args(res, ARG_TYPE_EQ,     id, -1, step, inlet); }
	goto st55;
st55:
	if ( ++p == pe )
		goto _test_eof55;
case 55:
#line 806 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr80;
	goto st0;
tr80:
#line 25 "lex/parser_clone.rl"
	{ inlet = 10*inlet+((*p)-'0'); }
#line 30 "lex/parser_clone.rl"
	{ set_args(res, ARG_TYPE_EQ,     id, -1, step, inlet); }
	goto st56;
st56:
	if ( ++p == pe )
		goto _test_eof56;
case 56:
#line 820 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr81;
	goto st0;
tr5:
#line 19 "lex/parser_clone.rl"
	{ id = (*p)-'0'; }
#line 30 "lex/parser_clone.rl"
	{ set_args(res, ARG_TYPE_EQ,     id, -1, step, inlet); }
#line 20 "lex/parser_clone.rl"
	{ id0 = id; }
	goto st57;
st57:
	if ( ++p == pe )
		goto _test_eof57;
case 57:
#line 836 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
	switch( (*p) ) {
		case 46: goto st6;
		case 58: goto st10;
		case 124: goto st11;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr82;
	goto st0;
tr82:
#line 19 "lex/parser_clone.rl"
	{ id = 10*id+((*p)-'0'); }
#line 30 "lex/parser_clone.rl"
	{ set_args(res, ARG_TYPE_EQ,     id, -1, step, inlet); }
#line 20 "lex/parser_clone.rl"
	{ id0 = id; }
	goto st58;
st58:
	if ( ++p == pe )
		goto _test_eof58;
case 58:
#line 857 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
	switch( (*p) ) {
		case 46: goto st6;
		case 58: goto st10;
		case 124: goto st11;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr83;
	goto st0;
tr83:
#line 19 "lex/parser_clone.rl"
	{ id = 10*id+((*p)-'0'); }
#line 30 "lex/parser_clone.rl"
	{ set_args(res, ARG_TYPE_EQ,     id, -1, step, inlet); }
#line 20 "lex/parser_clone.rl"
	{ id0 = id; }
	goto st59;
st59:
	if ( ++p == pe )
		goto _test_eof59;
case 59:
#line 878 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
	switch( (*p) ) {
		case 46: goto st6;
		case 58: goto st10;
		case 124: goto st11;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr84;
	goto st0;
tr84:
#line 19 "lex/parser_clone.rl"
	{ id = 10*id+((*p)-'0'); }
#line 30 "lex/parser_clone.rl"
	{ set_args(res, ARG_TYPE_EQ,     id, -1, step, inlet); }
#line 20 "lex/parser_clone.rl"
	{ id0 = id; }
	goto st60;
st60:
	if ( ++p == pe )
		goto _test_eof60;
case 60:
#line 899 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
	switch( (*p) ) {
		case 46: goto st6;
		case 58: goto st10;
		case 124: goto st11;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr85;
	goto st0;
st12:
	if ( ++p == pe )
		goto _test_eof12;
case 12:
	switch( (*p) ) {
		case 48: goto tr25;
		case 61: goto st14;
	}
	if ( 49 <= (*p) && (*p) <= 57 )
		goto tr26;
	goto st0;
tr25:
#line 18 "lex/parser_clone.rl"
	{ id = 0; }
#line 33 "lex/parser_clone.rl"
	{ set_args(res, ARG_TYPE_LT,     id, -1, 1,    inlet); }
	goto st61;
tr93:
#line 19 "lex/parser_clone.rl"
	{ id = 10*id+((*p)-'0'); }
#line 33 "lex/parser_clone.rl"
	{ set_args(res, ARG_TYPE_LT,     id, -1, 1,    inlet); }
	goto st61;
st61:
	if ( ++p == pe )
		goto _test_eof61;
case 61:
#line 935 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
	if ( (*p) == 124 )
		goto st13;
	goto st0;
st13:
	if ( ++p == pe )
		goto _test_eof13;
case 13:
	if ( (*p) == 48 )
		goto tr28;
	if ( 49 <= (*p) && (*p) <= 57 )
		goto tr29;
	goto st0;
tr29:
#line 25 "lex/parser_clone.rl"
	{ inlet = (*p)-'0'; }
#line 33 "lex/parser_clone.rl"
	{ set_args(res, ARG_TYPE_LT,     id, -1, 1,    inlet); }
	goto st62;
st62:
	if ( ++p == pe )
		goto _test_eof62;
case 62:
#line 958 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr87;
	goto st0;
tr87:
#line 25 "lex/parser_clone.rl"
	{ inlet = 10*inlet+((*p)-'0'); }
#line 33 "lex/parser_clone.rl"
	{ set_args(res, ARG_TYPE_LT,     id, -1, 1,    inlet); }
	goto st63;
st63:
	if ( ++p == pe )
		goto _test_eof63;
case 63:
#line 972 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr88;
	goto st0;
tr88:
#line 25 "lex/parser_clone.rl"
	{ inlet = 10*inlet+((*p)-'0'); }
#line 33 "lex/parser_clone.rl"
	{ set_args(res, ARG_TYPE_LT,     id, -1, 1,    inlet); }
	goto st64;
st64:
	if ( ++p == pe )
		goto _test_eof64;
case 64:
#line 986 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr89;
	goto st0;
tr26:
#line 19 "lex/parser_clone.rl"
	{ id = (*p)-'0'; }
#line 33 "lex/parser_clone.rl"
	{ set_args(res, ARG_TYPE_LT,     id, -1, 1,    inlet); }
	goto st65;
st65:
	if ( ++p == pe )
		goto _test_eof65;
case 65:
#line 1000 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
	if ( (*p) == 124 )
		goto st13;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr90;
	goto st0;
tr90:
#line 19 "lex/parser_clone.rl"
	{ id = 10*id+((*p)-'0'); }
#line 33 "lex/parser_clone.rl"
	{ set_args(res, ARG_TYPE_LT,     id, -1, 1,    inlet); }
	goto st66;
st66:
	if ( ++p == pe )
		goto _test_eof66;
case 66:
#line 1016 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
	if ( (*p) == 124 )
		goto st13;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr91;
	goto st0;
tr91:
#line 19 "lex/parser_clone.rl"
	{ id = 10*id+((*p)-'0'); }
#line 33 "lex/parser_clone.rl"
	{ set_args(res, ARG_TYPE_LT,     id, -1, 1,    inlet); }
	goto st67;
st67:
	if ( ++p == pe )
		goto _test_eof67;
case 67:
#line 1032 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
	if ( (*p) == 124 )
		goto st13;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr92;
	goto st0;
tr92:
#line 19 "lex/parser_clone.rl"
	{ id = 10*id+((*p)-'0'); }
#line 33 "lex/parser_clone.rl"
	{ set_args(res, ARG_TYPE_LT,     id, -1, 1,    inlet); }
	goto st68;
st68:
	if ( ++p == pe )
		goto _test_eof68;
case 68:
#line 1048 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
	if ( (*p) == 124 )
		goto st13;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr93;
	goto st0;
st14:
	if ( ++p == pe )
		goto _test_eof14;
case 14:
	if ( (*p) == 48 )
		goto tr30;
	if ( 49 <= (*p) && (*p) <= 57 )
		goto tr31;
	goto st0;
tr30:
#line 18 "lex/parser_clone.rl"
	{ id = 0; }
#line 34 "lex/parser_clone.rl"
	{ set_args(res, ARG_TYPE_LE,     id, -1, 1,    inlet); }
	goto st69;
tr101:
#line 19 "lex/parser_clone.rl"
	{ id = 10*id+((*p)-'0'); }
#line 34 "lex/parser_clone.rl"
	{ set_args(res, ARG_TYPE_LE,     id, -1, 1,    inlet); }
	goto st69;
st69:
	if ( ++p == pe )
		goto _test_eof69;
case 69:
#line 1079 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
	if ( (*p) == 124 )
		goto st15;
	goto st0;
st15:
	if ( ++p == pe )
		goto _test_eof15;
case 15:
	if ( (*p) == 48 )
		goto tr32;
	if ( 49 <= (*p) && (*p) <= 57 )
		goto tr33;
	goto st0;
tr33:
#line 25 "lex/parser_clone.rl"
	{ inlet = (*p)-'0'; }
#line 34 "lex/parser_clone.rl"
	{ set_args(res, ARG_TYPE_LE,     id, -1, 1,    inlet); }
	goto st70;
st70:
	if ( ++p == pe )
		goto _test_eof70;
case 70:
#line 1102 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr95;
	goto st0;
tr95:
#line 25 "lex/parser_clone.rl"
	{ inlet = 10*inlet+((*p)-'0'); }
#line 34 "lex/parser_clone.rl"
	{ set_args(res, ARG_TYPE_LE,     id, -1, 1,    inlet); }
	goto st71;
st71:
	if ( ++p == pe )
		goto _test_eof71;
case 71:
#line 1116 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr96;
	goto st0;
tr96:
#line 25 "lex/parser_clone.rl"
	{ inlet = 10*inlet+((*p)-'0'); }
#line 34 "lex/parser_clone.rl"
	{ set_args(res, ARG_TYPE_LE,     id, -1, 1,    inlet); }
	goto st72;
st72:
	if ( ++p == pe )
		goto _test_eof72;
case 72:
#line 1130 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr97;
	goto st0;
tr31:
#line 19 "lex/parser_clone.rl"
	{ id = (*p)-'0'; }
#line 34 "lex/parser_clone.rl"
	{ set_args(res, ARG_TYPE_LE,     id, -1, 1,    inlet); }
	goto st73;
st73:
	if ( ++p == pe )
		goto _test_eof73;
case 73:
#line 1144 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
	if ( (*p) == 124 )
		goto st15;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr98;
	goto st0;
tr98:
#line 19 "lex/parser_clone.rl"
	{ id = 10*id+((*p)-'0'); }
#line 34 "lex/parser_clone.rl"
	{ set_args(res, ARG_TYPE_LE,     id, -1, 1,    inlet); }
	goto st74;
st74:
	if ( ++p == pe )
		goto _test_eof74;
case 74:
#line 1160 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
	if ( (*p) == 124 )
		goto st15;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr99;
	goto st0;
tr99:
#line 19 "lex/parser_clone.rl"
	{ id = 10*id+((*p)-'0'); }
#line 34 "lex/parser_clone.rl"
	{ set_args(res, ARG_TYPE_LE,     id, -1, 1,    inlet); }
	goto st75;
st75:
	if ( ++p == pe )
		goto _test_eof75;
case 75:
#line 1176 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
	if ( (*p) == 124 )
		goto st15;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr100;
	goto st0;
tr100:
#line 19 "lex/parser_clone.rl"
	{ id = 10*id+((*p)-'0'); }
#line 34 "lex/parser_clone.rl"
	{ set_args(res, ARG_TYPE_LE,     id, -1, 1,    inlet); }
	goto st76;
st76:
	if ( ++p == pe )
		goto _test_eof76;
case 76:
#line 1192 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
	if ( (*p) == 124 )
		goto st15;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr101;
	goto st0;
st16:
	if ( ++p == pe )
		goto _test_eof16;
case 16:
	if ( (*p) == 48 )
		goto tr34;
	if ( 49 <= (*p) && (*p) <= 57 )
		goto tr35;
	goto st0;
tr34:
#line 18 "lex/parser_clone.rl"
	{ id = 0; }
#line 30 "lex/parser_clone.rl"
	{ set_args(res, ARG_TYPE_EQ,     id, -1, step, inlet); }
	goto st77;
tr105:
#line 19 "lex/parser_clone.rl"
	{ id = 10*id+((*p)-'0'); }
#line 30 "lex/parser_clone.rl"
	{ set_args(res, ARG_TYPE_EQ,     id, -1, step, inlet); }
	goto st77;
st77:
	if ( ++p == pe )
		goto _test_eof77;
case 77:
#line 1223 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
	switch( (*p) ) {
		case 58: goto st10;
		case 124: goto st11;
	}
	goto st0;
tr35:
#line 19 "lex/parser_clone.rl"
	{ id = (*p)-'0'; }
#line 30 "lex/parser_clone.rl"
	{ set_args(res, ARG_TYPE_EQ,     id, -1, step, inlet); }
	goto st78;
st78:
	if ( ++p == pe )
		goto _test_eof78;
case 78:
#line 1239 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
	switch( (*p) ) {
		case 58: goto st10;
		case 124: goto st11;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr102;
	goto st0;
tr102:
#line 19 "lex/parser_clone.rl"
	{ id = 10*id+((*p)-'0'); }
#line 30 "lex/parser_clone.rl"
	{ set_args(res, ARG_TYPE_EQ,     id, -1, step, inlet); }
	goto st79;
st79:
	if ( ++p == pe )
		goto _test_eof79;
case 79:
#line 1257 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
	switch( (*p) ) {
		case 58: goto st10;
		case 124: goto st11;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr103;
	goto st0;
tr103:
#line 19 "lex/parser_clone.rl"
	{ id = 10*id+((*p)-'0'); }
#line 30 "lex/parser_clone.rl"
	{ set_args(res, ARG_TYPE_EQ,     id, -1, step, inlet); }
	goto st80;
st80:
	if ( ++p == pe )
		goto _test_eof80;
case 80:
#line 1275 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
	switch( (*p) ) {
		case 58: goto st10;
		case 124: goto st11;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr104;
	goto st0;
tr104:
#line 19 "lex/parser_clone.rl"
	{ id = 10*id+((*p)-'0'); }
#line 30 "lex/parser_clone.rl"
	{ set_args(res, ARG_TYPE_EQ,     id, -1, step, inlet); }
	goto st81;
st81:
	if ( ++p == pe )
		goto _test_eof81;
case 81:
#line 1293 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
	switch( (*p) ) {
		case 58: goto st10;
		case 124: goto st11;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr105;
	goto st0;
st17:
	if ( ++p == pe )
		goto _test_eof17;
case 17:
	switch( (*p) ) {
		case 48: goto tr36;
		case 61: goto st19;
	}
	if ( 49 <= (*p) && (*p) <= 57 )
		goto tr37;
	goto st0;
tr36:
#line 18 "lex/parser_clone.rl"
	{ id = 0; }
#line 31 "lex/parser_clone.rl"
	{ set_args(res, ARG_TYPE_GT,     id, -1, 1,    inlet); }
	goto st82;
tr113:
#line 19 "lex/parser_clone.rl"
	{ id = 10*id+((*p)-'0'); }
#line 31 "lex/parser_clone.rl"
	{ set_args(res, ARG_TYPE_GT,     id, -1, 1,    inlet); }
	goto st82;
st82:
	if ( ++p == pe )
		goto _test_eof82;
case 82:
#line 1328 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
	if ( (*p) == 124 )
		goto st18;
	goto st0;
st18:
	if ( ++p == pe )
		goto _test_eof18;
case 18:
	if ( (*p) == 48 )
		goto tr39;
	if ( 49 <= (*p) && (*p) <= 57 )
		goto tr40;
	goto st0;
tr40:
#line 25 "lex/parser_clone.rl"
	{ inlet = (*p)-'0'; }
#line 31 "lex/parser_clone.rl"
	{ set_args(res, ARG_TYPE_GT,     id, -1, 1,    inlet); }
	goto st83;
st83:
	if ( ++p == pe )
		goto _test_eof83;
case 83:
#line 1351 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr107;
	goto st0;
tr107:
#line 25 "lex/parser_clone.rl"
	{ inlet = 10*inlet+((*p)-'0'); }
#line 31 "lex/parser_clone.rl"
	{ set_args(res, ARG_TYPE_GT,     id, -1, 1,    inlet); }
	goto st84;
st84:
	if ( ++p == pe )
		goto _test_eof84;
case 84:
#line 1365 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr108;
	goto st0;
tr108:
#line 25 "lex/parser_clone.rl"
	{ inlet = 10*inlet+((*p)-'0'); }
#line 31 "lex/parser_clone.rl"
	{ set_args(res, ARG_TYPE_GT,     id, -1, 1,    inlet); }
	goto st85;
st85:
	if ( ++p == pe )
		goto _test_eof85;
case 85:
#line 1379 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr109;
	goto st0;
tr37:
#line 19 "lex/parser_clone.rl"
	{ id = (*p)-'0'; }
#line 31 "lex/parser_clone.rl"
	{ set_args(res, ARG_TYPE_GT,     id, -1, 1,    inlet); }
	goto st86;
st86:
	if ( ++p == pe )
		goto _test_eof86;
case 86:
#line 1393 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
	if ( (*p) == 124 )
		goto st18;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr110;
	goto st0;
tr110:
#line 19 "lex/parser_clone.rl"
	{ id = 10*id+((*p)-'0'); }
#line 31 "lex/parser_clone.rl"
	{ set_args(res, ARG_TYPE_GT,     id, -1, 1,    inlet); }
	goto st87;
st87:
	if ( ++p == pe )
		goto _test_eof87;
case 87:
#line 1409 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
	if ( (*p) == 124 )
		goto st18;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr111;
	goto st0;
tr111:
#line 19 "lex/parser_clone.rl"
	{ id = 10*id+((*p)-'0'); }
#line 31 "lex/parser_clone.rl"
	{ set_args(res, ARG_TYPE_GT,     id, -1, 1,    inlet); }
	goto st88;
st88:
	if ( ++p == pe )
		goto _test_eof88;
case 88:
#line 1425 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
	if ( (*p) == 124 )
		goto st18;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr112;
	goto st0;
tr112:
#line 19 "lex/parser_clone.rl"
	{ id = 10*id+((*p)-'0'); }
#line 31 "lex/parser_clone.rl"
	{ set_args(res, ARG_TYPE_GT,     id, -1, 1,    inlet); }
	goto st89;
st89:
	if ( ++p == pe )
		goto _test_eof89;
case 89:
#line 1441 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
	if ( (*p) == 124 )
		goto st18;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr113;
	goto st0;
st19:
	if ( ++p == pe )
		goto _test_eof19;
case 19:
	if ( (*p) == 48 )
		goto tr41;
	if ( 49 <= (*p) && (*p) <= 57 )
		goto tr42;
	goto st0;
tr41:
#line 18 "lex/parser_clone.rl"
	{ id = 0; }
#line 32 "lex/parser_clone.rl"
	{ set_args(res, ARG_TYPE_GE,     id, -1, 1,    inlet); }
	goto st90;
tr121:
#line 19 "lex/parser_clone.rl"
	{ id = 10*id+((*p)-'0'); }
#line 32 "lex/parser_clone.rl"
	{ set_args(res, ARG_TYPE_GE,     id, -1, 1,    inlet); }
	goto st90;
st90:
	if ( ++p == pe )
		goto _test_eof90;
case 90:
#line 1472 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
	if ( (*p) == 124 )
		goto st20;
	goto st0;
st20:
	if ( ++p == pe )
		goto _test_eof20;
case 20:
	if ( (*p) == 48 )
		goto tr43;
	if ( 49 <= (*p) && (*p) <= 57 )
		goto tr44;
	goto st0;
tr44:
#line 25 "lex/parser_clone.rl"
	{ inlet = (*p)-'0'; }
#line 32 "lex/parser_clone.rl"
	{ set_args(res, ARG_TYPE_GE,     id, -1, 1,    inlet); }
	goto st91;
st91:
	if ( ++p == pe )
		goto _test_eof91;
case 91:
#line 1495 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr115;
	goto st0;
tr115:
#line 25 "lex/parser_clone.rl"
	{ inlet = 10*inlet+((*p)-'0'); }
#line 32 "lex/parser_clone.rl"
	{ set_args(res, ARG_TYPE_GE,     id, -1, 1,    inlet); }
	goto st92;
st92:
	if ( ++p == pe )
		goto _test_eof92;
case 92:
#line 1509 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr116;
	goto st0;
tr116:
#line 25 "lex/parser_clone.rl"
	{ inlet = 10*inlet+((*p)-'0'); }
#line 32 "lex/parser_clone.rl"
	{ set_args(res, ARG_TYPE_GE,     id, -1, 1,    inlet); }
	goto st93;
st93:
	if ( ++p == pe )
		goto _test_eof93;
case 93:
#line 1523 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr117;
	goto st0;
tr42:
#line 19 "lex/parser_clone.rl"
	{ id = (*p)-'0'; }
#line 32 "lex/parser_clone.rl"
	{ set_args(res, ARG_TYPE_GE,     id, -1, 1,    inlet); }
	goto st94;
st94:
	if ( ++p == pe )
		goto _test_eof94;
case 94:
#line 1537 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
	if ( (*p) == 124 )
		goto st20;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr118;
	goto st0;
tr118:
#line 19 "lex/parser_clone.rl"
	{ id = 10*id+((*p)-'0'); }
#line 32 "lex/parser_clone.rl"
	{ set_args(res, ARG_TYPE_GE,     id, -1, 1,    inlet); }
	goto st95;
st95:
	if ( ++p == pe )
		goto _test_eof95;
case 95:
#line 1553 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
	if ( (*p) == 124 )
		goto st20;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr119;
	goto st0;
tr119:
#line 19 "lex/parser_clone.rl"
	{ id = 10*id+((*p)-'0'); }
#line 32 "lex/parser_clone.rl"
	{ set_args(res, ARG_TYPE_GE,     id, -1, 1,    inlet); }
	goto st96;
st96:
	if ( ++p == pe )
		goto _test_eof96;
case 96:
#line 1569 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
	if ( (*p) == 124 )
		goto st20;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr120;
	goto st0;
tr120:
#line 19 "lex/parser_clone.rl"
	{ id = 10*id+((*p)-'0'); }
#line 32 "lex/parser_clone.rl"
	{ set_args(res, ARG_TYPE_GE,     id, -1, 1,    inlet); }
	goto st97;
st97:
	if ( ++p == pe )
		goto _test_eof97;
case 97:
#line 1585 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
	if ( (*p) == 124 )
		goto st20;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr121;
	goto st0;
tr9:
#line 28 "lex/parser_clone.rl"
	{ set_args(res, ARG_TYPE_RANDOM, -1, -1, 1,    inlet); }
	goto st98;
st98:
	if ( ++p == pe )
		goto _test_eof98;
case 98:
#line 1599 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
	if ( (*p) == 124 )
		goto st21;
	goto st0;
st21:
	if ( ++p == pe )
		goto _test_eof21;
case 21:
	if ( (*p) == 48 )
		goto tr45;
	if ( 49 <= (*p) && (*p) <= 57 )
		goto tr46;
	goto st0;
tr46:
#line 25 "lex/parser_clone.rl"
	{ inlet = (*p)-'0'; }
#line 28 "lex/parser_clone.rl"
	{ set_args(res, ARG_TYPE_RANDOM, -1, -1, 1,    inlet); }
	goto st99;
st99:
	if ( ++p == pe )
		goto _test_eof99;
case 99:
#line 1622 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr123;
	goto st0;
tr123:
#line 25 "lex/parser_clone.rl"
	{ inlet = 10*inlet+((*p)-'0'); }
#line 28 "lex/parser_clone.rl"
	{ set_args(res, ARG_TYPE_RANDOM, -1, -1, 1,    inlet); }
	goto st100;
st100:
	if ( ++p == pe )
		goto _test_eof100;
case 100:
#line 1636 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr124;
	goto st0;
tr124:
#line 25 "lex/parser_clone.rl"
	{ inlet = 10*inlet+((*p)-'0'); }
#line 28 "lex/parser_clone.rl"
	{ set_args(res, ARG_TYPE_RANDOM, -1, -1, 1,    inlet); }
	goto st101;
st101:
	if ( ++p == pe )
		goto _test_eof101;
case 101:
#line 1650 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr125;
	goto st0;
	}
	_test_eof2: cs = 2; goto _test_eof; 
	_test_eof3: cs = 3; goto _test_eof; 
	_test_eof22: cs = 22; goto _test_eof; 
	_test_eof4: cs = 4; goto _test_eof; 
	_test_eof23: cs = 23; goto _test_eof; 
	_test_eof24: cs = 24; goto _test_eof; 
	_test_eof25: cs = 25; goto _test_eof; 
	_test_eof26: cs = 26; goto _test_eof; 
	_test_eof27: cs = 27; goto _test_eof; 
	_test_eof28: cs = 28; goto _test_eof; 
	_test_eof29: cs = 29; goto _test_eof; 
	_test_eof30: cs = 30; goto _test_eof; 
	_test_eof31: cs = 31; goto _test_eof; 
	_test_eof5: cs = 5; goto _test_eof; 
	_test_eof32: cs = 32; goto _test_eof; 
	_test_eof33: cs = 33; goto _test_eof; 
	_test_eof34: cs = 34; goto _test_eof; 
	_test_eof35: cs = 35; goto _test_eof; 
	_test_eof6: cs = 6; goto _test_eof; 
	_test_eof7: cs = 7; goto _test_eof; 
	_test_eof36: cs = 36; goto _test_eof; 
	_test_eof8: cs = 8; goto _test_eof; 
	_test_eof37: cs = 37; goto _test_eof; 
	_test_eof38: cs = 38; goto _test_eof; 
	_test_eof39: cs = 39; goto _test_eof; 
	_test_eof40: cs = 40; goto _test_eof; 
	_test_eof41: cs = 41; goto _test_eof; 
	_test_eof9: cs = 9; goto _test_eof; 
	_test_eof42: cs = 42; goto _test_eof; 
	_test_eof43: cs = 43; goto _test_eof; 
	_test_eof44: cs = 44; goto _test_eof; 
	_test_eof45: cs = 45; goto _test_eof; 
	_test_eof46: cs = 46; goto _test_eof; 
	_test_eof47: cs = 47; goto _test_eof; 
	_test_eof48: cs = 48; goto _test_eof; 
	_test_eof10: cs = 10; goto _test_eof; 
	_test_eof49: cs = 49; goto _test_eof; 
	_test_eof50: cs = 50; goto _test_eof; 
	_test_eof51: cs = 51; goto _test_eof; 
	_test_eof52: cs = 52; goto _test_eof; 
	_test_eof53: cs = 53; goto _test_eof; 
	_test_eof11: cs = 11; goto _test_eof; 
	_test_eof54: cs = 54; goto _test_eof; 
	_test_eof55: cs = 55; goto _test_eof; 
	_test_eof56: cs = 56; goto _test_eof; 
	_test_eof57: cs = 57; goto _test_eof; 
	_test_eof58: cs = 58; goto _test_eof; 
	_test_eof59: cs = 59; goto _test_eof; 
	_test_eof60: cs = 60; goto _test_eof; 
	_test_eof12: cs = 12; goto _test_eof; 
	_test_eof61: cs = 61; goto _test_eof; 
	_test_eof13: cs = 13; goto _test_eof; 
	_test_eof62: cs = 62; goto _test_eof; 
	_test_eof63: cs = 63; goto _test_eof; 
	_test_eof64: cs = 64; goto _test_eof; 
	_test_eof65: cs = 65; goto _test_eof; 
	_test_eof66: cs = 66; goto _test_eof; 
	_test_eof67: cs = 67; goto _test_eof; 
	_test_eof68: cs = 68; goto _test_eof; 
	_test_eof14: cs = 14; goto _test_eof; 
	_test_eof69: cs = 69; goto _test_eof; 
	_test_eof15: cs = 15; goto _test_eof; 
	_test_eof70: cs = 70; goto _test_eof; 
	_test_eof71: cs = 71; goto _test_eof; 
	_test_eof72: cs = 72; goto _test_eof; 
	_test_eof73: cs = 73; goto _test_eof; 
	_test_eof74: cs = 74; goto _test_eof; 
	_test_eof75: cs = 75; goto _test_eof; 
	_test_eof76: cs = 76; goto _test_eof; 
	_test_eof16: cs = 16; goto _test_eof; 
	_test_eof77: cs = 77; goto _test_eof; 
	_test_eof78: cs = 78; goto _test_eof; 
	_test_eof79: cs = 79; goto _test_eof; 
	_test_eof80: cs = 80; goto _test_eof; 
	_test_eof81: cs = 81; goto _test_eof; 
	_test_eof17: cs = 17; goto _test_eof; 
	_test_eof82: cs = 82; goto _test_eof; 
	_test_eof18: cs = 18; goto _test_eof; 
	_test_eof83: cs = 83; goto _test_eof; 
	_test_eof84: cs = 84; goto _test_eof; 
	_test_eof85: cs = 85; goto _test_eof; 
	_test_eof86: cs = 86; goto _test_eof; 
	_test_eof87: cs = 87; goto _test_eof; 
	_test_eof88: cs = 88; goto _test_eof; 
	_test_eof89: cs = 89; goto _test_eof; 
	_test_eof19: cs = 19; goto _test_eof; 
	_test_eof90: cs = 90; goto _test_eof; 
	_test_eof20: cs = 20; goto _test_eof; 
	_test_eof91: cs = 91; goto _test_eof; 
	_test_eof92: cs = 92; goto _test_eof; 
	_test_eof93: cs = 93; goto _test_eof; 
	_test_eof94: cs = 94; goto _test_eof; 
	_test_eof95: cs = 95; goto _test_eof; 
	_test_eof96: cs = 96; goto _test_eof; 
	_test_eof97: cs = 97; goto _test_eof; 
	_test_eof98: cs = 98; goto _test_eof; 
	_test_eof21: cs = 21; goto _test_eof; 
	_test_eof99: cs = 99; goto _test_eof; 
	_test_eof100: cs = 100; goto _test_eof; 
	_test_eof101: cs = 101; goto _test_eof; 

	_test_eof: {}
	_out: {}
	}

#line 70 "lex/parser_clone.rl"

    return cs >= 22;
}


#line 1766 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
static const int clone_action_name_start = 1;
static const int clone_action_name_first_final = 14;
static const int clone_action_name_error = 0;

static const int clone_action_name_en_main = 1;


#line 85 "lex/parser_clone.rl"


MessageType parse_clone_message_type(const char* str)
{
    const auto len = strlen(str);
    if (len == 0)
        return MSG_TYPE_NONE;

    int cs = 0;
    const char* p = str;
    const char* pe = p + len;
    const char* eof = pe;

    MessageType type = MSG_TYPE_NONE;

    
#line 1791 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
	{
	cs = clone_action_name_start;
	}

#line 101 "lex/parser_clone.rl"
    
#line 1798 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
	{
	if ( p == pe )
		goto _test_eof;
	switch ( cs )
	{
case 1:
	switch( (*p) ) {
		case 100: goto st2;
		case 115: goto st4;
		case 126: goto st11;
	}
	goto st0;
st0:
cs = 0;
	goto _out;
st2:
	if ( ++p == pe )
		goto _test_eof2;
case 2:
	if ( (*p) == 115 )
		goto st3;
	goto st0;
st3:
	if ( ++p == pe )
		goto _test_eof3;
case 3:
	if ( (*p) == 112 )
		goto tr5;
	goto st0;
tr5:
#line 78 "lex/parser_clone.rl"
	{ type = MSG_TYPE_DSP_SET; }
	goto st14;
tr9:
#line 80 "lex/parser_clone.rl"
	{ type = MSG_TYPE_SEND; }
	goto st14;
tr13:
#line 81 "lex/parser_clone.rl"
	{ type = MSG_TYPE_SEND_SPREAD; }
	goto st14;
tr16:
#line 79 "lex/parser_clone.rl"
	{ type = MSG_TYPE_DSP_TOGGLE; }
	goto st14;
st14:
	if ( ++p == pe )
		goto _test_eof14;
case 14:
#line 1848 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
	goto st0;
st4:
	if ( ++p == pe )
		goto _test_eof4;
case 4:
	switch( (*p) ) {
		case 101: goto st5;
		case 112: goto st7;
	}
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
	if ( (*p) == 100 )
		goto tr9;
	goto st0;
st7:
	if ( ++p == pe )
		goto _test_eof7;
case 7:
	if ( (*p) == 114 )
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
	if ( (*p) == 97 )
		goto st10;
	goto st0;
st10:
	if ( ++p == pe )
		goto _test_eof10;
case 10:
	if ( (*p) == 100 )
		goto tr13;
	goto st0;
st11:
	if ( ++p == pe )
		goto _test_eof11;
case 11:
	if ( (*p) == 100 )
		goto st12;
	goto st0;
st12:
	if ( ++p == pe )
		goto _test_eof12;
case 12:
	if ( (*p) == 115 )
		goto st13;
	goto st0;
st13:
	if ( ++p == pe )
		goto _test_eof13;
case 13:
	if ( (*p) == 112 )
		goto tr16;
	goto st0;
	}
	_test_eof2: cs = 2; goto _test_eof; 
	_test_eof3: cs = 3; goto _test_eof; 
	_test_eof14: cs = 14; goto _test_eof; 
	_test_eof4: cs = 4; goto _test_eof; 
	_test_eof5: cs = 5; goto _test_eof; 
	_test_eof6: cs = 6; goto _test_eof; 
	_test_eof7: cs = 7; goto _test_eof; 
	_test_eof8: cs = 8; goto _test_eof; 
	_test_eof9: cs = 9; goto _test_eof; 
	_test_eof10: cs = 10; goto _test_eof; 
	_test_eof11: cs = 11; goto _test_eof; 
	_test_eof12: cs = 12; goto _test_eof; 
	_test_eof13: cs = 13; goto _test_eof; 

	_test_eof: {}
	_out: {}
	}

#line 102 "lex/parser_clone.rl"

    return (cs < 14) ? MSG_TYPE_NONE : type;
}

}
}
