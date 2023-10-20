
#line 1 "lex/parser_ui_bind.rl"
# include "parser_ui_bind.h"

# include <cstring>
# include <cstdio>

namespace ceammc {
namespace parser {

static inline void add_digit(std::uint8_t& v, char ch) { (v *= 10) += (ch - '0'); }


#line 15 "lex/parser_ui_bind.cpp"
static const int ui_bind_start = 1;
static const int ui_bind_first_final = 82;
static const int ui_bind_error = 0;

static const int ui_bind_en_main = 1;


#line 55 "lex/parser_ui_bind.rl"


bool parse_ui_bind(const char* str, UIBindOptions& opts)
{
    int cs = 0;
    const auto len = strlen(str);
    const char* p = str;
    const char* pe = p + len;
    const char* eof = pe;

    
#line 35 "lex/parser_ui_bind.cpp"
	{
	cs = ui_bind_start;
	}

#line 66 "lex/parser_ui_bind.rl"
    
#line 42 "lex/parser_ui_bind.cpp"
	{
	if ( p == pe )
		goto _test_eof;
	switch ( cs )
	{
case 1:
	switch( (*p) ) {
		case 99: goto st2;
		case 107: goto st18;
		case 110: goto st53;
		case 112: goto st71;
	}
	goto st0;
st0:
cs = 0;
	goto _out;
st2:
	if ( ++p == pe )
		goto _test_eof2;
case 2:
	if ( (*p) == 99 )
		goto st3;
	goto st0;
st3:
	if ( ++p == pe )
		goto _test_eof3;
case 3:
	if ( (*p) == 91 )
		goto st4;
	goto st0;
st4:
	if ( ++p == pe )
		goto _test_eof4;
case 4:
	if ( (*p) == 48 )
		goto tr7;
	if ( 49 <= (*p) && (*p) <= 57 )
		goto tr8;
	goto st0;
tr7:
#line 31 "lex/parser_ui_bind.rl"
	{ opts.midi_param = 0; }
#line 15 "lex/parser_ui_bind.rl"
	{ add_digit(opts.midi_param, *p); }
	goto st5;
tr28:
#line 15 "lex/parser_ui_bind.rl"
	{ add_digit(opts.midi_param, *p); }
	goto st5;
st5:
	if ( ++p == pe )
		goto _test_eof5;
case 5:
#line 96 "lex/parser_ui_bind.cpp"
	switch( (*p) ) {
		case 58: goto tr9;
		case 93: goto st82;
	}
	goto st0;
tr9:
#line 29 "lex/parser_ui_bind.rl"
	{ opts.midi_chan = 0; }
	goto st6;
st6:
	if ( ++p == pe )
		goto _test_eof6;
case 6:
#line 110 "lex/parser_ui_bind.cpp"
	if ( (*p) == 48 )
		goto tr11;
	if ( 49 <= (*p) && (*p) <= 57 )
		goto tr12;
	goto st0;
tr11:
#line 14 "lex/parser_ui_bind.rl"
	{ add_digit(opts.midi_chan, *p); }
	goto st7;
st7:
	if ( ++p == pe )
		goto _test_eof7;
case 7:
#line 124 "lex/parser_ui_bind.cpp"
	if ( (*p) == 93 )
		goto st82;
	goto st0;
st82:
	if ( ++p == pe )
		goto _test_eof82;
case 82:
	switch( (*p) ) {
		case 60: goto st8;
		case 61: goto st10;
		case 62: goto st12;
	}
	goto st0;
st8:
	if ( ++p == pe )
		goto _test_eof8;
case 8:
	switch( (*p) ) {
		case 48: goto tr13;
		case 61: goto st9;
	}
	if ( 49 <= (*p) && (*p) <= 57 )
		goto tr14;
	goto st0;
tr13:
#line 25 "lex/parser_ui_bind.rl"
	{ opts.cmp = UI_BIND_CMP_LT; }
#line 32 "lex/parser_ui_bind.rl"
	{ opts.midi_value = 0; }
#line 16 "lex/parser_ui_bind.rl"
	{ add_digit(opts.midi_value, *p); }
	goto st83;
tr16:
#line 26 "lex/parser_ui_bind.rl"
	{ opts.cmp = UI_BIND_CMP_LE; }
#line 32 "lex/parser_ui_bind.rl"
	{ opts.midi_value = 0; }
#line 16 "lex/parser_ui_bind.rl"
	{ add_digit(opts.midi_value, *p); }
	goto st83;
tr19:
#line 22 "lex/parser_ui_bind.rl"
	{ opts.cmp = UI_BIND_CMP_EQ; }
#line 32 "lex/parser_ui_bind.rl"
	{ opts.midi_value = 0; }
#line 16 "lex/parser_ui_bind.rl"
	{ add_digit(opts.midi_value, *p); }
	goto st83;
tr21:
#line 23 "lex/parser_ui_bind.rl"
	{ opts.cmp = UI_BIND_CMP_GT; }
#line 32 "lex/parser_ui_bind.rl"
	{ opts.midi_value = 0; }
#line 16 "lex/parser_ui_bind.rl"
	{ add_digit(opts.midi_value, *p); }
	goto st83;
tr24:
#line 24 "lex/parser_ui_bind.rl"
	{ opts.cmp = UI_BIND_CMP_GE; }
#line 32 "lex/parser_ui_bind.rl"
	{ opts.midi_value = 0; }
#line 16 "lex/parser_ui_bind.rl"
	{ add_digit(opts.midi_value, *p); }
	goto st83;
tr118:
#line 16 "lex/parser_ui_bind.rl"
	{ add_digit(opts.midi_value, *p); }
	goto st83;
st83:
	if ( ++p == pe )
		goto _test_eof83;
case 83:
#line 197 "lex/parser_ui_bind.cpp"
	goto st0;
tr14:
#line 25 "lex/parser_ui_bind.rl"
	{ opts.cmp = UI_BIND_CMP_LT; }
#line 32 "lex/parser_ui_bind.rl"
	{ opts.midi_value = 0; }
#line 16 "lex/parser_ui_bind.rl"
	{ add_digit(opts.midi_value, *p); }
	goto st84;
tr17:
#line 26 "lex/parser_ui_bind.rl"
	{ opts.cmp = UI_BIND_CMP_LE; }
#line 32 "lex/parser_ui_bind.rl"
	{ opts.midi_value = 0; }
#line 16 "lex/parser_ui_bind.rl"
	{ add_digit(opts.midi_value, *p); }
	goto st84;
tr20:
#line 22 "lex/parser_ui_bind.rl"
	{ opts.cmp = UI_BIND_CMP_EQ; }
#line 32 "lex/parser_ui_bind.rl"
	{ opts.midi_value = 0; }
#line 16 "lex/parser_ui_bind.rl"
	{ add_digit(opts.midi_value, *p); }
	goto st84;
tr22:
#line 23 "lex/parser_ui_bind.rl"
	{ opts.cmp = UI_BIND_CMP_GT; }
#line 32 "lex/parser_ui_bind.rl"
	{ opts.midi_value = 0; }
#line 16 "lex/parser_ui_bind.rl"
	{ add_digit(opts.midi_value, *p); }
	goto st84;
tr25:
#line 24 "lex/parser_ui_bind.rl"
	{ opts.cmp = UI_BIND_CMP_GE; }
#line 32 "lex/parser_ui_bind.rl"
	{ opts.midi_value = 0; }
#line 16 "lex/parser_ui_bind.rl"
	{ add_digit(opts.midi_value, *p); }
	goto st84;
st84:
	if ( ++p == pe )
		goto _test_eof84;
case 84:
#line 243 "lex/parser_ui_bind.cpp"
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr117;
	goto st0;
tr117:
#line 16 "lex/parser_ui_bind.rl"
	{ add_digit(opts.midi_value, *p); }
	goto st85;
st85:
	if ( ++p == pe )
		goto _test_eof85;
case 85:
#line 255 "lex/parser_ui_bind.cpp"
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr118;
	goto st0;
st9:
	if ( ++p == pe )
		goto _test_eof9;
case 9:
	if ( (*p) == 48 )
		goto tr16;
	if ( 49 <= (*p) && (*p) <= 57 )
		goto tr17;
	goto st0;
st10:
	if ( ++p == pe )
		goto _test_eof10;
case 10:
	if ( (*p) == 61 )
		goto st11;
	goto st0;
st11:
	if ( ++p == pe )
		goto _test_eof11;
case 11:
	if ( (*p) == 48 )
		goto tr19;
	if ( 49 <= (*p) && (*p) <= 57 )
		goto tr20;
	goto st0;
st12:
	if ( ++p == pe )
		goto _test_eof12;
case 12:
	switch( (*p) ) {
		case 48: goto tr21;
		case 61: goto st13;
	}
	if ( 49 <= (*p) && (*p) <= 57 )
		goto tr22;
	goto st0;
st13:
	if ( ++p == pe )
		goto _test_eof13;
case 13:
	if ( (*p) == 48 )
		goto tr24;
	if ( 49 <= (*p) && (*p) <= 57 )
		goto tr25;
	goto st0;
tr12:
#line 14 "lex/parser_ui_bind.rl"
	{ add_digit(opts.midi_chan, *p); }
	goto st14;
st14:
	if ( ++p == pe )
		goto _test_eof14;
case 14:
#line 312 "lex/parser_ui_bind.cpp"
	if ( (*p) == 93 )
		goto st82;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr26;
	goto st0;
tr26:
#line 14 "lex/parser_ui_bind.rl"
	{ add_digit(opts.midi_chan, *p); }
	goto st15;
st15:
	if ( ++p == pe )
		goto _test_eof15;
case 15:
#line 326 "lex/parser_ui_bind.cpp"
	if ( (*p) == 93 )
		goto st82;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr11;
	goto st0;
tr8:
#line 31 "lex/parser_ui_bind.rl"
	{ opts.midi_param = 0; }
#line 15 "lex/parser_ui_bind.rl"
	{ add_digit(opts.midi_param, *p); }
	goto st16;
st16:
	if ( ++p == pe )
		goto _test_eof16;
case 16:
#line 342 "lex/parser_ui_bind.cpp"
	switch( (*p) ) {
		case 58: goto tr9;
		case 93: goto st82;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr27;
	goto st0;
tr27:
#line 15 "lex/parser_ui_bind.rl"
	{ add_digit(opts.midi_param, *p); }
	goto st17;
st17:
	if ( ++p == pe )
		goto _test_eof17;
case 17:
#line 358 "lex/parser_ui_bind.cpp"
	switch( (*p) ) {
		case 58: goto tr9;
		case 93: goto st82;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr28;
	goto st0;
st18:
	if ( ++p == pe )
		goto _test_eof18;
case 18:
	if ( (*p) == 101 )
		goto st19;
	goto st0;
st19:
	if ( ++p == pe )
		goto _test_eof19;
case 19:
	if ( (*p) == 121 )
		goto st20;
	goto st0;
st20:
	if ( ++p == pe )
		goto _test_eof20;
case 20:
	switch( (*p) ) {
		case 61: goto st21;
		case 91: goto st22;
		case 110: goto st35;
	}
	goto st0;
st21:
	if ( ++p == pe )
		goto _test_eof21;
case 21:
	if ( (*p) == 48 )
		goto tr34;
	if ( 49 <= (*p) && (*p) <= 57 )
		goto tr35;
	goto st0;
tr34:
#line 47 "lex/parser_ui_bind.rl"
	{ opts.key_code = 0; }
#line 17 "lex/parser_ui_bind.rl"
	{ add_digit(opts.key_code, *p); }
	goto st86;
tr120:
#line 17 "lex/parser_ui_bind.rl"
	{ add_digit(opts.key_code, *p); }
	goto st86;
st86:
	if ( ++p == pe )
		goto _test_eof86;
case 86:
#line 413 "lex/parser_ui_bind.cpp"
	goto st0;
tr35:
#line 47 "lex/parser_ui_bind.rl"
	{ opts.key_code = 0; }
#line 17 "lex/parser_ui_bind.rl"
	{ add_digit(opts.key_code, *p); }
	goto st87;
st87:
	if ( ++p == pe )
		goto _test_eof87;
case 87:
#line 425 "lex/parser_ui_bind.cpp"
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr119;
	goto st0;
tr119:
#line 17 "lex/parser_ui_bind.rl"
	{ add_digit(opts.key_code, *p); }
	goto st88;
st88:
	if ( ++p == pe )
		goto _test_eof88;
case 88:
#line 437 "lex/parser_ui_bind.cpp"
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr120;
	goto st0;
tr41:
#line 42 "lex/parser_ui_bind.rl"
	{ opts.key_mode |= UI_BIND_MODE_ALT; }
	goto st22;
tr45:
#line 41 "lex/parser_ui_bind.rl"
	{ opts.key_mode |= UI_BIND_MODE_CTL; }
	goto st22;
tr51:
#line 43 "lex/parser_ui_bind.rl"
	{ opts.key_mode |= UI_BIND_MODE_SHIFT; }
	goto st22;
st22:
	if ( ++p == pe )
		goto _test_eof22;
case 22:
#line 457 "lex/parser_ui_bind.cpp"
	switch( (*p) ) {
		case 97: goto st23;
		case 99: goto st27;
		case 115: goto st30;
	}
	goto st0;
st23:
	if ( ++p == pe )
		goto _test_eof23;
case 23:
	if ( (*p) == 108 )
		goto st24;
	goto st0;
st24:
	if ( ++p == pe )
		goto _test_eof24;
case 24:
	if ( (*p) == 116 )
		goto st25;
	goto st0;
st25:
	if ( ++p == pe )
		goto _test_eof25;
case 25:
	switch( (*p) ) {
		case 43: goto tr41;
		case 93: goto tr42;
	}
	goto st0;
tr42:
#line 42 "lex/parser_ui_bind.rl"
	{ opts.key_mode |= UI_BIND_MODE_ALT; }
	goto st26;
tr46:
#line 41 "lex/parser_ui_bind.rl"
	{ opts.key_mode |= UI_BIND_MODE_CTL; }
	goto st26;
tr52:
#line 43 "lex/parser_ui_bind.rl"
	{ opts.key_mode |= UI_BIND_MODE_SHIFT; }
	goto st26;
st26:
	if ( ++p == pe )
		goto _test_eof26;
case 26:
#line 503 "lex/parser_ui_bind.cpp"
	if ( (*p) == 61 )
		goto st21;
	goto st0;
st27:
	if ( ++p == pe )
		goto _test_eof27;
case 27:
	if ( (*p) == 116 )
		goto st28;
	goto st0;
st28:
	if ( ++p == pe )
		goto _test_eof28;
case 28:
	if ( (*p) == 108 )
		goto st29;
	goto st0;
st29:
	if ( ++p == pe )
		goto _test_eof29;
case 29:
	switch( (*p) ) {
		case 43: goto tr45;
		case 93: goto tr46;
	}
	goto st0;
st30:
	if ( ++p == pe )
		goto _test_eof30;
case 30:
	if ( (*p) == 104 )
		goto st31;
	goto st0;
st31:
	if ( ++p == pe )
		goto _test_eof31;
case 31:
	if ( (*p) == 105 )
		goto st32;
	goto st0;
st32:
	if ( ++p == pe )
		goto _test_eof32;
case 32:
	if ( (*p) == 102 )
		goto st33;
	goto st0;
st33:
	if ( ++p == pe )
		goto _test_eof33;
case 33:
	if ( (*p) == 116 )
		goto st34;
	goto st0;
st34:
	if ( ++p == pe )
		goto _test_eof34;
case 34:
	switch( (*p) ) {
		case 43: goto tr51;
		case 93: goto tr52;
	}
	goto st0;
st35:
	if ( ++p == pe )
		goto _test_eof35;
case 35:
	if ( (*p) == 97 )
		goto st36;
	goto st0;
st36:
	if ( ++p == pe )
		goto _test_eof36;
case 36:
	if ( (*p) == 109 )
		goto st37;
	goto st0;
st37:
	if ( ++p == pe )
		goto _test_eof37;
case 37:
	if ( (*p) == 101 )
		goto st38;
	goto st0;
st38:
	if ( ++p == pe )
		goto _test_eof38;
case 38:
	switch( (*p) ) {
		case 61: goto st39;
		case 91: goto st40;
	}
	goto st0;
st39:
	if ( ++p == pe )
		goto _test_eof39;
case 39:
	if ( (*p) == 95 )
		goto tr58;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr58;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr58;
	} else
		goto tr58;
	goto st0;
tr58:
#line 18 "lex/parser_ui_bind.rl"
	{ opts.key_name[0] = 0; opts.name_len = 0; }
#line 19 "lex/parser_ui_bind.rl"
	{ opts.key_name[opts.name_len++] = *p; opts.key_name[opts.name_len] = 0; }
	goto st89;
st89:
	if ( ++p == pe )
		goto _test_eof89;
case 89:
#line 622 "lex/parser_ui_bind.cpp"
	if ( (*p) == 95 )
		goto tr121;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr121;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr121;
	} else
		goto tr121;
	goto st0;
tr121:
#line 19 "lex/parser_ui_bind.rl"
	{ opts.key_name[opts.name_len++] = *p; opts.key_name[opts.name_len] = 0; }
	goto st90;
st90:
	if ( ++p == pe )
		goto _test_eof90;
case 90:
#line 642 "lex/parser_ui_bind.cpp"
	if ( (*p) == 95 )
		goto tr122;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr122;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr122;
	} else
		goto tr122;
	goto st0;
tr122:
#line 19 "lex/parser_ui_bind.rl"
	{ opts.key_name[opts.name_len++] = *p; opts.key_name[opts.name_len] = 0; }
	goto st91;
st91:
	if ( ++p == pe )
		goto _test_eof91;
case 91:
#line 662 "lex/parser_ui_bind.cpp"
	if ( (*p) == 95 )
		goto tr123;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr123;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr123;
	} else
		goto tr123;
	goto st0;
tr123:
#line 19 "lex/parser_ui_bind.rl"
	{ opts.key_name[opts.name_len++] = *p; opts.key_name[opts.name_len] = 0; }
	goto st92;
st92:
	if ( ++p == pe )
		goto _test_eof92;
case 92:
#line 682 "lex/parser_ui_bind.cpp"
	if ( (*p) == 95 )
		goto tr124;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr124;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr124;
	} else
		goto tr124;
	goto st0;
tr124:
#line 19 "lex/parser_ui_bind.rl"
	{ opts.key_name[opts.name_len++] = *p; opts.key_name[opts.name_len] = 0; }
	goto st93;
st93:
	if ( ++p == pe )
		goto _test_eof93;
case 93:
#line 702 "lex/parser_ui_bind.cpp"
	if ( (*p) == 95 )
		goto tr125;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr125;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr125;
	} else
		goto tr125;
	goto st0;
tr125:
#line 19 "lex/parser_ui_bind.rl"
	{ opts.key_name[opts.name_len++] = *p; opts.key_name[opts.name_len] = 0; }
	goto st94;
st94:
	if ( ++p == pe )
		goto _test_eof94;
case 94:
#line 722 "lex/parser_ui_bind.cpp"
	if ( (*p) == 95 )
		goto tr126;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr126;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr126;
	} else
		goto tr126;
	goto st0;
tr126:
#line 19 "lex/parser_ui_bind.rl"
	{ opts.key_name[opts.name_len++] = *p; opts.key_name[opts.name_len] = 0; }
	goto st95;
st95:
	if ( ++p == pe )
		goto _test_eof95;
case 95:
#line 742 "lex/parser_ui_bind.cpp"
	if ( (*p) == 95 )
		goto tr127;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr127;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr127;
	} else
		goto tr127;
	goto st0;
tr127:
#line 19 "lex/parser_ui_bind.rl"
	{ opts.key_name[opts.name_len++] = *p; opts.key_name[opts.name_len] = 0; }
	goto st96;
st96:
	if ( ++p == pe )
		goto _test_eof96;
case 96:
#line 762 "lex/parser_ui_bind.cpp"
	goto st0;
tr64:
#line 42 "lex/parser_ui_bind.rl"
	{ opts.key_mode |= UI_BIND_MODE_ALT; }
	goto st40;
tr68:
#line 41 "lex/parser_ui_bind.rl"
	{ opts.key_mode |= UI_BIND_MODE_CTL; }
	goto st40;
tr74:
#line 43 "lex/parser_ui_bind.rl"
	{ opts.key_mode |= UI_BIND_MODE_SHIFT; }
	goto st40;
st40:
	if ( ++p == pe )
		goto _test_eof40;
case 40:
#line 780 "lex/parser_ui_bind.cpp"
	switch( (*p) ) {
		case 97: goto st41;
		case 99: goto st45;
		case 115: goto st48;
	}
	goto st0;
st41:
	if ( ++p == pe )
		goto _test_eof41;
case 41:
	if ( (*p) == 108 )
		goto st42;
	goto st0;
st42:
	if ( ++p == pe )
		goto _test_eof42;
case 42:
	if ( (*p) == 116 )
		goto st43;
	goto st0;
st43:
	if ( ++p == pe )
		goto _test_eof43;
case 43:
	switch( (*p) ) {
		case 43: goto tr64;
		case 93: goto tr65;
	}
	goto st0;
tr65:
#line 42 "lex/parser_ui_bind.rl"
	{ opts.key_mode |= UI_BIND_MODE_ALT; }
	goto st44;
tr69:
#line 41 "lex/parser_ui_bind.rl"
	{ opts.key_mode |= UI_BIND_MODE_CTL; }
	goto st44;
tr75:
#line 43 "lex/parser_ui_bind.rl"
	{ opts.key_mode |= UI_BIND_MODE_SHIFT; }
	goto st44;
st44:
	if ( ++p == pe )
		goto _test_eof44;
case 44:
#line 826 "lex/parser_ui_bind.cpp"
	if ( (*p) == 61 )
		goto st39;
	goto st0;
st45:
	if ( ++p == pe )
		goto _test_eof45;
case 45:
	if ( (*p) == 116 )
		goto st46;
	goto st0;
st46:
	if ( ++p == pe )
		goto _test_eof46;
case 46:
	if ( (*p) == 108 )
		goto st47;
	goto st0;
st47:
	if ( ++p == pe )
		goto _test_eof47;
case 47:
	switch( (*p) ) {
		case 43: goto tr68;
		case 93: goto tr69;
	}
	goto st0;
st48:
	if ( ++p == pe )
		goto _test_eof48;
case 48:
	if ( (*p) == 104 )
		goto st49;
	goto st0;
st49:
	if ( ++p == pe )
		goto _test_eof49;
case 49:
	if ( (*p) == 105 )
		goto st50;
	goto st0;
st50:
	if ( ++p == pe )
		goto _test_eof50;
case 50:
	if ( (*p) == 102 )
		goto st51;
	goto st0;
st51:
	if ( ++p == pe )
		goto _test_eof51;
case 51:
	if ( (*p) == 116 )
		goto st52;
	goto st0;
st52:
	if ( ++p == pe )
		goto _test_eof52;
case 52:
	switch( (*p) ) {
		case 43: goto tr74;
		case 93: goto tr75;
	}
	goto st0;
st53:
	if ( ++p == pe )
		goto _test_eof53;
case 53:
	if ( (*p) == 111 )
		goto st54;
	goto st0;
st54:
	if ( ++p == pe )
		goto _test_eof54;
case 54:
	if ( (*p) == 116 )
		goto st55;
	goto st0;
st55:
	if ( ++p == pe )
		goto _test_eof55;
case 55:
	if ( (*p) == 101 )
		goto st56;
	goto st0;
st56:
	if ( ++p == pe )
		goto _test_eof56;
case 56:
	if ( (*p) == 91 )
		goto st57;
	goto st0;
st57:
	if ( ++p == pe )
		goto _test_eof57;
case 57:
	if ( (*p) == 48 )
		goto tr80;
	if ( 49 <= (*p) && (*p) <= 57 )
		goto tr81;
	goto st0;
tr80:
#line 31 "lex/parser_ui_bind.rl"
	{ opts.midi_param = 0; }
#line 15 "lex/parser_ui_bind.rl"
	{ add_digit(opts.midi_param, *p); }
	goto st58;
tr101:
#line 15 "lex/parser_ui_bind.rl"
	{ add_digit(opts.midi_param, *p); }
	goto st58;
st58:
	if ( ++p == pe )
		goto _test_eof58;
case 58:
#line 941 "lex/parser_ui_bind.cpp"
	switch( (*p) ) {
		case 58: goto tr82;
		case 93: goto st97;
	}
	goto st0;
tr82:
#line 29 "lex/parser_ui_bind.rl"
	{ opts.midi_chan = 0; }
	goto st59;
st59:
	if ( ++p == pe )
		goto _test_eof59;
case 59:
#line 955 "lex/parser_ui_bind.cpp"
	if ( (*p) == 48 )
		goto tr84;
	if ( 49 <= (*p) && (*p) <= 57 )
		goto tr85;
	goto st0;
tr84:
#line 14 "lex/parser_ui_bind.rl"
	{ add_digit(opts.midi_chan, *p); }
	goto st60;
st60:
	if ( ++p == pe )
		goto _test_eof60;
case 60:
#line 969 "lex/parser_ui_bind.cpp"
	if ( (*p) == 93 )
		goto st97;
	goto st0;
st97:
	if ( ++p == pe )
		goto _test_eof97;
case 97:
	switch( (*p) ) {
		case 60: goto st61;
		case 61: goto st63;
		case 62: goto st65;
	}
	goto st0;
st61:
	if ( ++p == pe )
		goto _test_eof61;
case 61:
	switch( (*p) ) {
		case 48: goto tr86;
		case 61: goto st62;
	}
	if ( 49 <= (*p) && (*p) <= 57 )
		goto tr87;
	goto st0;
tr86:
#line 25 "lex/parser_ui_bind.rl"
	{ opts.cmp = UI_BIND_CMP_LT; }
#line 32 "lex/parser_ui_bind.rl"
	{ opts.midi_value = 0; }
#line 16 "lex/parser_ui_bind.rl"
	{ add_digit(opts.midi_value, *p); }
	goto st98;
tr89:
#line 26 "lex/parser_ui_bind.rl"
	{ opts.cmp = UI_BIND_CMP_LE; }
#line 32 "lex/parser_ui_bind.rl"
	{ opts.midi_value = 0; }
#line 16 "lex/parser_ui_bind.rl"
	{ add_digit(opts.midi_value, *p); }
	goto st98;
tr92:
#line 22 "lex/parser_ui_bind.rl"
	{ opts.cmp = UI_BIND_CMP_EQ; }
#line 32 "lex/parser_ui_bind.rl"
	{ opts.midi_value = 0; }
#line 16 "lex/parser_ui_bind.rl"
	{ add_digit(opts.midi_value, *p); }
	goto st98;
tr94:
#line 23 "lex/parser_ui_bind.rl"
	{ opts.cmp = UI_BIND_CMP_GT; }
#line 32 "lex/parser_ui_bind.rl"
	{ opts.midi_value = 0; }
#line 16 "lex/parser_ui_bind.rl"
	{ add_digit(opts.midi_value, *p); }
	goto st98;
tr97:
#line 24 "lex/parser_ui_bind.rl"
	{ opts.cmp = UI_BIND_CMP_GE; }
#line 32 "lex/parser_ui_bind.rl"
	{ opts.midi_value = 0; }
#line 16 "lex/parser_ui_bind.rl"
	{ add_digit(opts.midi_value, *p); }
	goto st98;
tr132:
#line 16 "lex/parser_ui_bind.rl"
	{ add_digit(opts.midi_value, *p); }
	goto st98;
st98:
	if ( ++p == pe )
		goto _test_eof98;
case 98:
#line 1042 "lex/parser_ui_bind.cpp"
	goto st0;
tr87:
#line 25 "lex/parser_ui_bind.rl"
	{ opts.cmp = UI_BIND_CMP_LT; }
#line 32 "lex/parser_ui_bind.rl"
	{ opts.midi_value = 0; }
#line 16 "lex/parser_ui_bind.rl"
	{ add_digit(opts.midi_value, *p); }
	goto st99;
tr90:
#line 26 "lex/parser_ui_bind.rl"
	{ opts.cmp = UI_BIND_CMP_LE; }
#line 32 "lex/parser_ui_bind.rl"
	{ opts.midi_value = 0; }
#line 16 "lex/parser_ui_bind.rl"
	{ add_digit(opts.midi_value, *p); }
	goto st99;
tr93:
#line 22 "lex/parser_ui_bind.rl"
	{ opts.cmp = UI_BIND_CMP_EQ; }
#line 32 "lex/parser_ui_bind.rl"
	{ opts.midi_value = 0; }
#line 16 "lex/parser_ui_bind.rl"
	{ add_digit(opts.midi_value, *p); }
	goto st99;
tr95:
#line 23 "lex/parser_ui_bind.rl"
	{ opts.cmp = UI_BIND_CMP_GT; }
#line 32 "lex/parser_ui_bind.rl"
	{ opts.midi_value = 0; }
#line 16 "lex/parser_ui_bind.rl"
	{ add_digit(opts.midi_value, *p); }
	goto st99;
tr98:
#line 24 "lex/parser_ui_bind.rl"
	{ opts.cmp = UI_BIND_CMP_GE; }
#line 32 "lex/parser_ui_bind.rl"
	{ opts.midi_value = 0; }
#line 16 "lex/parser_ui_bind.rl"
	{ add_digit(opts.midi_value, *p); }
	goto st99;
st99:
	if ( ++p == pe )
		goto _test_eof99;
case 99:
#line 1088 "lex/parser_ui_bind.cpp"
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr131;
	goto st0;
tr131:
#line 16 "lex/parser_ui_bind.rl"
	{ add_digit(opts.midi_value, *p); }
	goto st100;
st100:
	if ( ++p == pe )
		goto _test_eof100;
case 100:
#line 1100 "lex/parser_ui_bind.cpp"
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr132;
	goto st0;
st62:
	if ( ++p == pe )
		goto _test_eof62;
case 62:
	if ( (*p) == 48 )
		goto tr89;
	if ( 49 <= (*p) && (*p) <= 57 )
		goto tr90;
	goto st0;
st63:
	if ( ++p == pe )
		goto _test_eof63;
case 63:
	if ( (*p) == 61 )
		goto st64;
	goto st0;
st64:
	if ( ++p == pe )
		goto _test_eof64;
case 64:
	if ( (*p) == 48 )
		goto tr92;
	if ( 49 <= (*p) && (*p) <= 57 )
		goto tr93;
	goto st0;
st65:
	if ( ++p == pe )
		goto _test_eof65;
case 65:
	switch( (*p) ) {
		case 48: goto tr94;
		case 61: goto st66;
	}
	if ( 49 <= (*p) && (*p) <= 57 )
		goto tr95;
	goto st0;
st66:
	if ( ++p == pe )
		goto _test_eof66;
case 66:
	if ( (*p) == 48 )
		goto tr97;
	if ( 49 <= (*p) && (*p) <= 57 )
		goto tr98;
	goto st0;
tr85:
#line 14 "lex/parser_ui_bind.rl"
	{ add_digit(opts.midi_chan, *p); }
	goto st67;
st67:
	if ( ++p == pe )
		goto _test_eof67;
case 67:
#line 1157 "lex/parser_ui_bind.cpp"
	if ( (*p) == 93 )
		goto st97;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr99;
	goto st0;
tr99:
#line 14 "lex/parser_ui_bind.rl"
	{ add_digit(opts.midi_chan, *p); }
	goto st68;
st68:
	if ( ++p == pe )
		goto _test_eof68;
case 68:
#line 1171 "lex/parser_ui_bind.cpp"
	if ( (*p) == 93 )
		goto st97;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr84;
	goto st0;
tr81:
#line 31 "lex/parser_ui_bind.rl"
	{ opts.midi_param = 0; }
#line 15 "lex/parser_ui_bind.rl"
	{ add_digit(opts.midi_param, *p); }
	goto st69;
st69:
	if ( ++p == pe )
		goto _test_eof69;
case 69:
#line 1187 "lex/parser_ui_bind.cpp"
	switch( (*p) ) {
		case 58: goto tr82;
		case 93: goto st97;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr100;
	goto st0;
tr100:
#line 15 "lex/parser_ui_bind.rl"
	{ add_digit(opts.midi_param, *p); }
	goto st70;
st70:
	if ( ++p == pe )
		goto _test_eof70;
case 70:
#line 1203 "lex/parser_ui_bind.cpp"
	switch( (*p) ) {
		case 58: goto tr82;
		case 93: goto st97;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr101;
	goto st0;
st71:
	if ( ++p == pe )
		goto _test_eof71;
case 71:
	if ( (*p) == 103 )
		goto st72;
	goto st0;
st72:
	if ( ++p == pe )
		goto _test_eof72;
case 72:
	if ( (*p) == 109 )
		goto st73;
	goto st0;
st73:
	if ( ++p == pe )
		goto _test_eof73;
case 73:
	if ( (*p) == 91 )
		goto st74;
	goto st0;
st74:
	if ( ++p == pe )
		goto _test_eof74;
case 74:
	if ( (*p) == 48 )
		goto tr105;
	if ( 49 <= (*p) && (*p) <= 57 )
		goto tr106;
	goto st0;
tr105:
#line 31 "lex/parser_ui_bind.rl"
	{ opts.midi_param = 0; }
#line 15 "lex/parser_ui_bind.rl"
	{ add_digit(opts.midi_param, *p); }
	goto st75;
tr113:
#line 15 "lex/parser_ui_bind.rl"
	{ add_digit(opts.midi_param, *p); }
	goto st75;
st75:
	if ( ++p == pe )
		goto _test_eof75;
case 75:
#line 1255 "lex/parser_ui_bind.cpp"
	switch( (*p) ) {
		case 58: goto tr107;
		case 93: goto st101;
	}
	goto st0;
tr107:
#line 29 "lex/parser_ui_bind.rl"
	{ opts.midi_chan = 0; }
	goto st76;
st76:
	if ( ++p == pe )
		goto _test_eof76;
case 76:
#line 1269 "lex/parser_ui_bind.cpp"
	if ( (*p) == 48 )
		goto tr109;
	if ( 49 <= (*p) && (*p) <= 57 )
		goto tr110;
	goto st0;
tr109:
#line 14 "lex/parser_ui_bind.rl"
	{ add_digit(opts.midi_chan, *p); }
	goto st77;
st77:
	if ( ++p == pe )
		goto _test_eof77;
case 77:
#line 1283 "lex/parser_ui_bind.cpp"
	if ( (*p) == 93 )
		goto st101;
	goto st0;
st101:
	if ( ++p == pe )
		goto _test_eof101;
case 101:
	goto st0;
tr110:
#line 14 "lex/parser_ui_bind.rl"
	{ add_digit(opts.midi_chan, *p); }
	goto st78;
st78:
	if ( ++p == pe )
		goto _test_eof78;
case 78:
#line 1300 "lex/parser_ui_bind.cpp"
	if ( (*p) == 93 )
		goto st101;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr111;
	goto st0;
tr111:
#line 14 "lex/parser_ui_bind.rl"
	{ add_digit(opts.midi_chan, *p); }
	goto st79;
st79:
	if ( ++p == pe )
		goto _test_eof79;
case 79:
#line 1314 "lex/parser_ui_bind.cpp"
	if ( (*p) == 93 )
		goto st101;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr109;
	goto st0;
tr106:
#line 31 "lex/parser_ui_bind.rl"
	{ opts.midi_param = 0; }
#line 15 "lex/parser_ui_bind.rl"
	{ add_digit(opts.midi_param, *p); }
	goto st80;
st80:
	if ( ++p == pe )
		goto _test_eof80;
case 80:
#line 1330 "lex/parser_ui_bind.cpp"
	switch( (*p) ) {
		case 58: goto tr107;
		case 93: goto st101;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr112;
	goto st0;
tr112:
#line 15 "lex/parser_ui_bind.rl"
	{ add_digit(opts.midi_param, *p); }
	goto st81;
st81:
	if ( ++p == pe )
		goto _test_eof81;
case 81:
#line 1346 "lex/parser_ui_bind.cpp"
	switch( (*p) ) {
		case 58: goto tr107;
		case 93: goto st101;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr113;
	goto st0;
	}
	_test_eof2: cs = 2; goto _test_eof; 
	_test_eof3: cs = 3; goto _test_eof; 
	_test_eof4: cs = 4; goto _test_eof; 
	_test_eof5: cs = 5; goto _test_eof; 
	_test_eof6: cs = 6; goto _test_eof; 
	_test_eof7: cs = 7; goto _test_eof; 
	_test_eof82: cs = 82; goto _test_eof; 
	_test_eof8: cs = 8; goto _test_eof; 
	_test_eof83: cs = 83; goto _test_eof; 
	_test_eof84: cs = 84; goto _test_eof; 
	_test_eof85: cs = 85; goto _test_eof; 
	_test_eof9: cs = 9; goto _test_eof; 
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
	_test_eof86: cs = 86; goto _test_eof; 
	_test_eof87: cs = 87; goto _test_eof; 
	_test_eof88: cs = 88; goto _test_eof; 
	_test_eof22: cs = 22; goto _test_eof; 
	_test_eof23: cs = 23; goto _test_eof; 
	_test_eof24: cs = 24; goto _test_eof; 
	_test_eof25: cs = 25; goto _test_eof; 
	_test_eof26: cs = 26; goto _test_eof; 
	_test_eof27: cs = 27; goto _test_eof; 
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
	_test_eof39: cs = 39; goto _test_eof; 
	_test_eof89: cs = 89; goto _test_eof; 
	_test_eof90: cs = 90; goto _test_eof; 
	_test_eof91: cs = 91; goto _test_eof; 
	_test_eof92: cs = 92; goto _test_eof; 
	_test_eof93: cs = 93; goto _test_eof; 
	_test_eof94: cs = 94; goto _test_eof; 
	_test_eof95: cs = 95; goto _test_eof; 
	_test_eof96: cs = 96; goto _test_eof; 
	_test_eof40: cs = 40; goto _test_eof; 
	_test_eof41: cs = 41; goto _test_eof; 
	_test_eof42: cs = 42; goto _test_eof; 
	_test_eof43: cs = 43; goto _test_eof; 
	_test_eof44: cs = 44; goto _test_eof; 
	_test_eof45: cs = 45; goto _test_eof; 
	_test_eof46: cs = 46; goto _test_eof; 
	_test_eof47: cs = 47; goto _test_eof; 
	_test_eof48: cs = 48; goto _test_eof; 
	_test_eof49: cs = 49; goto _test_eof; 
	_test_eof50: cs = 50; goto _test_eof; 
	_test_eof51: cs = 51; goto _test_eof; 
	_test_eof52: cs = 52; goto _test_eof; 
	_test_eof53: cs = 53; goto _test_eof; 
	_test_eof54: cs = 54; goto _test_eof; 
	_test_eof55: cs = 55; goto _test_eof; 
	_test_eof56: cs = 56; goto _test_eof; 
	_test_eof57: cs = 57; goto _test_eof; 
	_test_eof58: cs = 58; goto _test_eof; 
	_test_eof59: cs = 59; goto _test_eof; 
	_test_eof60: cs = 60; goto _test_eof; 
	_test_eof97: cs = 97; goto _test_eof; 
	_test_eof61: cs = 61; goto _test_eof; 
	_test_eof98: cs = 98; goto _test_eof; 
	_test_eof99: cs = 99; goto _test_eof; 
	_test_eof100: cs = 100; goto _test_eof; 
	_test_eof62: cs = 62; goto _test_eof; 
	_test_eof63: cs = 63; goto _test_eof; 
	_test_eof64: cs = 64; goto _test_eof; 
	_test_eof65: cs = 65; goto _test_eof; 
	_test_eof66: cs = 66; goto _test_eof; 
	_test_eof67: cs = 67; goto _test_eof; 
	_test_eof68: cs = 68; goto _test_eof; 
	_test_eof69: cs = 69; goto _test_eof; 
	_test_eof70: cs = 70; goto _test_eof; 
	_test_eof71: cs = 71; goto _test_eof; 
	_test_eof72: cs = 72; goto _test_eof; 
	_test_eof73: cs = 73; goto _test_eof; 
	_test_eof74: cs = 74; goto _test_eof; 
	_test_eof75: cs = 75; goto _test_eof; 
	_test_eof76: cs = 76; goto _test_eof; 
	_test_eof77: cs = 77; goto _test_eof; 
	_test_eof101: cs = 101; goto _test_eof; 
	_test_eof78: cs = 78; goto _test_eof; 
	_test_eof79: cs = 79; goto _test_eof; 
	_test_eof80: cs = 80; goto _test_eof; 
	_test_eof81: cs = 81; goto _test_eof; 

	_test_eof: {}
	if ( p == eof )
	{
	switch ( cs ) {
	case 97: 
	case 98: 
	case 99: 
	case 100: 
#line 36 "lex/parser_ui_bind.rl"
	{ opts.type = UI_BIND_MIDI_NOTE; }
	break;
	case 82: 
	case 83: 
	case 84: 
	case 85: 
#line 37 "lex/parser_ui_bind.rl"
	{ opts.type = UI_BIND_MIDI_CC; }
	break;
	case 101: 
#line 38 "lex/parser_ui_bind.rl"
	{ opts.type = UI_BIND_MIDI_PGM; }
	break;
	case 86: 
	case 87: 
	case 88: 
#line 49 "lex/parser_ui_bind.rl"
	{ opts.type = UI_BIND_KEY_CODE; opts.cmp = UI_BIND_CMP_EQ; }
	break;
	case 89: 
	case 90: 
	case 91: 
	case 92: 
	case 93: 
	case 94: 
	case 95: 
	case 96: 
#line 50 "lex/parser_ui_bind.rl"
	{ opts.type = UI_BIND_KEY_NAME; opts.cmp = UI_BIND_CMP_EQ; }
	break;
#line 1495 "lex/parser_ui_bind.cpp"
	}
	}

	_out: {}
	}

#line 67 "lex/parser_ui_bind.rl"

    const auto ok = cs >= 82;
    if (ok) {

    }

    return ok;
}

}
}
