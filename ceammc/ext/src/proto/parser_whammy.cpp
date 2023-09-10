
#line 1 "parser_whammy.rl"
# include "parser_whammy_common.h"

# include <cstring>

namespace ceammc {
namespace proto {


#line 12 "parser_whammy.cpp"
static const int whammy_ragel_start = 1;
static const int whammy_ragel_first_final = 115;
static const int whammy_ragel_error = 0;

static const int whammy_ragel_en_main = 1;


#line 38 "parser_whammy.rl"


bool nameToWhammyMode(const char* str, WhammyMode& mode) {
    int cs = 0;
    const char* p = str;
    WhammyMode mode_ = WHAMMY_MODE_UP_OCT;

    
#line 29 "parser_whammy.cpp"
	{
	cs = whammy_ragel_start;
	}

#line 46 "parser_whammy.rl"
    
#line 36 "parser_whammy.cpp"
	{
	switch ( cs )
	{
case 1:
	switch( (*p) ) {
		case 43: goto st2;
		case 45: goto st53;
		case 49: goto st3;
		case 50: goto st89;
		case 52: goto st90;
		case 53: goto st93;
		case 98: goto st96;
		case 100: goto st100;
		case 111: goto st4;
		case 115: goto st109;
	}
	goto st0;
st0:
cs = 0;
	goto _out;
st2:
	p += 1;
case 2:
	switch( (*p) ) {
		case 49: goto st3;
		case 50: goto st7;
		case 51: goto st18;
		case 52: goto st34;
		case 53: goto st42;
		case 111: goto st4;
	}
	goto st0;
st3:
	p += 1;
case 3:
	if ( (*p) == 111 )
		goto st4;
	goto st0;
st4:
	p += 1;
case 4:
	if ( (*p) == 99 )
		goto st5;
	goto st0;
st5:
	p += 1;
case 5:
	if ( (*p) == 116 )
		goto st6;
	goto st0;
st6:
	p += 1;
case 6:
	if ( (*p) == 0 )
		goto tr17;
	goto st0;
tr17:
#line 14 "parser_whammy.rl"
	{ mode_ = WHAMMY_MODE_UP_OCT; }
#line 35 "parser_whammy.rl"
	{ {p++; cs = 115; goto _out;} }
	goto st115;
tr22:
#line 25 "parser_whammy.rl"
	{ mode_ = WHAMMY_MODE_UP_2ND_UP_3RD; }
#line 35 "parser_whammy.rl"
	{ {p++; cs = 115; goto _out;} }
	goto st115;
tr29:
#line 13 "parser_whammy.rl"
	{ mode_ = WHAMMY_MODE_UP_2OCT; }
#line 35 "parser_whammy.rl"
	{ {p++; cs = 115; goto _out;} }
	goto st115;
tr34:
#line 27 "parser_whammy.rl"
	{ mode_ = WHAMMY_MODE_UP_3RD_UP_4TH; }
#line 35 "parser_whammy.rl"
	{ {p++; cs = 115; goto _out;} }
	goto st115;
tr38:
#line 26 "parser_whammy.rl"
	{ mode_ = WHAMMY_MODE_UP_3RD_UP_3RD; }
#line 35 "parser_whammy.rl"
	{ {p++; cs = 115; goto _out;} }
	goto st115;
tr50:
#line 28 "parser_whammy.rl"
	{ mode_ = WHAMMY_MODE_UP_4TH_UP_5TH; }
#line 35 "parser_whammy.rl"
	{ {p++; cs = 115; goto _out;} }
	goto st115;
tr52:
#line 16 "parser_whammy.rl"
	{ mode_ = WHAMMY_MODE_UP_4TH; }
#line 35 "parser_whammy.rl"
	{ {p++; cs = 115; goto _out;} }
	goto st115;
tr60:
#line 29 "parser_whammy.rl"
	{ mode_ = WHAMMY_MODE_UP_5TH_UP_6TH; }
#line 35 "parser_whammy.rl"
	{ {p++; cs = 115; goto _out;} }
	goto st115;
tr61:
#line 30 "parser_whammy.rl"
	{ mode_ = WHAMMY_MODE_UP_5TH_UP_7TH; }
#line 35 "parser_whammy.rl"
	{ {p++; cs = 115; goto _out;} }
	goto st115;
tr63:
#line 15 "parser_whammy.rl"
	{ mode_ = WHAMMY_MODE_UP_5TH; }
#line 35 "parser_whammy.rl"
	{ {p++; cs = 115; goto _out;} }
	goto st115;
tr78:
#line 20 "parser_whammy.rl"
	{ mode_ = WHAMMY_MODE_DOWN_OCT; }
#line 35 "parser_whammy.rl"
	{ {p++; cs = 115; goto _out;} }
	goto st115;
tr82:
#line 17 "parser_whammy.rl"
	{ mode_ = WHAMMY_MODE_DOWN_2ND; }
#line 35 "parser_whammy.rl"
	{ {p++; cs = 115; goto _out;} }
	goto st115;
tr85:
#line 21 "parser_whammy.rl"
	{ mode_ = WHAMMY_MODE_DOWN_2OCT; }
#line 35 "parser_whammy.rl"
	{ {p++; cs = 115; goto _out;} }
	goto st115;
tr89:
#line 31 "parser_whammy.rl"
	{ mode_ = WHAMMY_MODE_DOWN_4TH_DOWN_3TH; }
#line 35 "parser_whammy.rl"
	{ {p++; cs = 115; goto _out;} }
	goto st115;
tr91:
#line 18 "parser_whammy.rl"
	{ mode_ = WHAMMY_MODE_DOWN_4TH; }
#line 35 "parser_whammy.rl"
	{ {p++; cs = 115; goto _out;} }
	goto st115;
tr98:
#line 32 "parser_whammy.rl"
	{ mode_ = WHAMMY_MODE_DOWN_5TH_DOWN_4TH; }
#line 35 "parser_whammy.rl"
	{ {p++; cs = 115; goto _out;} }
	goto st115;
tr100:
#line 19 "parser_whammy.rl"
	{ mode_ = WHAMMY_MODE_DOWN_5TH; }
#line 35 "parser_whammy.rl"
	{ {p++; cs = 115; goto _out;} }
	goto st115;
tr106:
#line 33 "parser_whammy.rl"
	{ mode_ = WHAMMY_MODE_DOWN_OCT_UP_OCT; }
#line 35 "parser_whammy.rl"
	{ {p++; cs = 115; goto _out;} }
	goto st115;
tr119:
#line 22 "parser_whammy.rl"
	{ mode_ = WHAMMY_MODE_DIVE_BOMB; }
#line 35 "parser_whammy.rl"
	{ {p++; cs = 115; goto _out;} }
	goto st115;
tr124:
#line 23 "parser_whammy.rl"
	{ mode_ = WHAMMY_MODE_DETUNE_DEEP; }
#line 35 "parser_whammy.rl"
	{ {p++; cs = 115; goto _out;} }
	goto st115;
tr129:
#line 24 "parser_whammy.rl"
	{ mode_ = WHAMMY_MODE_DETUNE_SHALLOW; }
#line 35 "parser_whammy.rl"
	{ {p++; cs = 115; goto _out;} }
	goto st115;
st115:
	p += 1;
case 115:
#line 222 "parser_whammy.cpp"
	goto st0;
st7:
	p += 1;
case 7:
	switch( (*p) ) {
		case 43: goto st8;
		case 110: goto st10;
		case 111: goto st15;
	}
	goto st0;
st8:
	p += 1;
case 8:
	if ( (*p) == 51 )
		goto st9;
	goto st0;
st9:
	p += 1;
case 9:
	if ( (*p) == 0 )
		goto tr22;
	goto st0;
st10:
	p += 1;
case 10:
	if ( (*p) == 100 )
		goto st11;
	goto st0;
st11:
	p += 1;
case 11:
	if ( (*p) == 43 )
		goto st12;
	goto st0;
st12:
	p += 1;
case 12:
	if ( (*p) == 51 )
		goto st13;
	goto st0;
st13:
	p += 1;
case 13:
	if ( (*p) == 114 )
		goto st14;
	goto st0;
st14:
	p += 1;
case 14:
	if ( (*p) == 100 )
		goto st9;
	goto st0;
st15:
	p += 1;
case 15:
	if ( (*p) == 99 )
		goto st16;
	goto st0;
st16:
	p += 1;
case 16:
	if ( (*p) == 116 )
		goto st17;
	goto st0;
st17:
	p += 1;
case 17:
	if ( (*p) == 0 )
		goto tr29;
	goto st0;
st18:
	p += 1;
case 18:
	switch( (*p) ) {
		case 43: goto st19;
		case 98: goto st21;
		case 114: goto st29;
	}
	goto st0;
st19:
	p += 1;
case 19:
	if ( (*p) == 52 )
		goto st20;
	goto st0;
st20:
	p += 1;
case 20:
	if ( (*p) == 0 )
		goto tr34;
	goto st0;
st21:
	p += 1;
case 21:
	switch( (*p) ) {
		case 43: goto st22;
		case 114: goto st24;
	}
	goto st0;
st22:
	p += 1;
case 22:
	if ( (*p) == 51 )
		goto st23;
	goto st0;
st23:
	p += 1;
case 23:
	if ( (*p) == 0 )
		goto tr38;
	goto st0;
st24:
	p += 1;
case 24:
	if ( (*p) == 100 )
		goto st25;
	goto st0;
st25:
	p += 1;
case 25:
	if ( (*p) == 43 )
		goto st26;
	goto st0;
st26:
	p += 1;
case 26:
	if ( (*p) == 51 )
		goto st27;
	goto st0;
st27:
	p += 1;
case 27:
	if ( (*p) == 114 )
		goto st28;
	goto st0;
st28:
	p += 1;
case 28:
	if ( (*p) == 100 )
		goto st23;
	goto st0;
st29:
	p += 1;
case 29:
	if ( (*p) == 100 )
		goto st30;
	goto st0;
st30:
	p += 1;
case 30:
	if ( (*p) == 43 )
		goto st31;
	goto st0;
st31:
	p += 1;
case 31:
	if ( (*p) == 52 )
		goto st32;
	goto st0;
st32:
	p += 1;
case 32:
	if ( (*p) == 116 )
		goto st33;
	goto st0;
st33:
	p += 1;
case 33:
	if ( (*p) == 104 )
		goto st20;
	goto st0;
st34:
	p += 1;
case 34:
	switch( (*p) ) {
		case 43: goto st35;
		case 116: goto st37;
	}
	goto st0;
st35:
	p += 1;
case 35:
	if ( (*p) == 53 )
		goto st36;
	goto st0;
st36:
	p += 1;
case 36:
	if ( (*p) == 0 )
		goto tr50;
	goto st0;
st37:
	p += 1;
case 37:
	if ( (*p) == 104 )
		goto st38;
	goto st0;
st38:
	p += 1;
case 38:
	switch( (*p) ) {
		case 0: goto tr52;
		case 43: goto st39;
	}
	goto st0;
st39:
	p += 1;
case 39:
	if ( (*p) == 53 )
		goto st40;
	goto st0;
st40:
	p += 1;
case 40:
	if ( (*p) == 116 )
		goto st41;
	goto st0;
st41:
	p += 1;
case 41:
	if ( (*p) == 104 )
		goto st36;
	goto st0;
st42:
	p += 1;
case 42:
	switch( (*p) ) {
		case 43: goto st43;
		case 116: goto st46;
	}
	goto st0;
st43:
	p += 1;
case 43:
	switch( (*p) ) {
		case 54: goto st44;
		case 55: goto st45;
	}
	goto st0;
st44:
	p += 1;
case 44:
	if ( (*p) == 0 )
		goto tr60;
	goto st0;
st45:
	p += 1;
case 45:
	if ( (*p) == 0 )
		goto tr61;
	goto st0;
st46:
	p += 1;
case 46:
	if ( (*p) == 104 )
		goto st47;
	goto st0;
st47:
	p += 1;
case 47:
	switch( (*p) ) {
		case 0: goto tr63;
		case 43: goto st48;
	}
	goto st0;
st48:
	p += 1;
case 48:
	switch( (*p) ) {
		case 54: goto st49;
		case 55: goto st51;
	}
	goto st0;
st49:
	p += 1;
case 49:
	if ( (*p) == 116 )
		goto st50;
	goto st0;
st50:
	p += 1;
case 50:
	if ( (*p) == 104 )
		goto st44;
	goto st0;
st51:
	p += 1;
case 51:
	if ( (*p) == 116 )
		goto st52;
	goto st0;
st52:
	p += 1;
case 52:
	if ( (*p) == 104 )
		goto st45;
	goto st0;
st53:
	p += 1;
case 53:
	switch( (*p) ) {
		case 49: goto st54;
		case 50: goto st58;
		case 52: goto st64;
		case 53: goto st72;
		case 56: goto st80;
		case 111: goto st83;
	}
	goto st0;
st54:
	p += 1;
case 54:
	if ( (*p) == 111 )
		goto st55;
	goto st0;
st55:
	p += 1;
case 55:
	if ( (*p) == 99 )
		goto st56;
	goto st0;
st56:
	p += 1;
case 56:
	if ( (*p) == 116 )
		goto st57;
	goto st0;
st57:
	p += 1;
case 57:
	if ( (*p) == 0 )
		goto tr78;
	goto st0;
st58:
	p += 1;
case 58:
	switch( (*p) ) {
		case 110: goto st59;
		case 111: goto st61;
	}
	goto st0;
st59:
	p += 1;
case 59:
	if ( (*p) == 100 )
		goto st60;
	goto st0;
st60:
	p += 1;
case 60:
	if ( (*p) == 0 )
		goto tr82;
	goto st0;
st61:
	p += 1;
case 61:
	if ( (*p) == 99 )
		goto st62;
	goto st0;
st62:
	p += 1;
case 62:
	if ( (*p) == 116 )
		goto st63;
	goto st0;
st63:
	p += 1;
case 63:
	if ( (*p) == 0 )
		goto tr85;
	goto st0;
st64:
	p += 1;
case 64:
	switch( (*p) ) {
		case 45: goto st65;
		case 116: goto st67;
	}
	goto st0;
st65:
	p += 1;
case 65:
	if ( (*p) == 51 )
		goto st66;
	goto st0;
st66:
	p += 1;
case 66:
	if ( (*p) == 0 )
		goto tr89;
	goto st0;
st67:
	p += 1;
case 67:
	if ( (*p) == 104 )
		goto st68;
	goto st0;
st68:
	p += 1;
case 68:
	switch( (*p) ) {
		case 0: goto tr91;
		case 45: goto st69;
	}
	goto st0;
st69:
	p += 1;
case 69:
	if ( (*p) == 51 )
		goto st70;
	goto st0;
st70:
	p += 1;
case 70:
	if ( (*p) == 114 )
		goto st71;
	goto st0;
st71:
	p += 1;
case 71:
	if ( (*p) == 100 )
		goto st66;
	goto st0;
st72:
	p += 1;
case 72:
	switch( (*p) ) {
		case 45: goto st73;
		case 116: goto st75;
	}
	goto st0;
st73:
	p += 1;
case 73:
	if ( (*p) == 52 )
		goto st74;
	goto st0;
st74:
	p += 1;
case 74:
	if ( (*p) == 0 )
		goto tr98;
	goto st0;
st75:
	p += 1;
case 75:
	if ( (*p) == 104 )
		goto st76;
	goto st0;
st76:
	p += 1;
case 76:
	switch( (*p) ) {
		case 0: goto tr100;
		case 45: goto st77;
	}
	goto st0;
st77:
	p += 1;
case 77:
	if ( (*p) == 52 )
		goto st78;
	goto st0;
st78:
	p += 1;
case 78:
	if ( (*p) == 116 )
		goto st79;
	goto st0;
st79:
	p += 1;
case 79:
	if ( (*p) == 104 )
		goto st74;
	goto st0;
st80:
	p += 1;
case 80:
	if ( (*p) == 43 )
		goto st81;
	goto st0;
st81:
	p += 1;
case 81:
	if ( (*p) == 56 )
		goto st82;
	goto st0;
st82:
	p += 1;
case 82:
	if ( (*p) == 0 )
		goto tr106;
	goto st0;
st83:
	p += 1;
case 83:
	if ( (*p) == 99 )
		goto st84;
	goto st0;
st84:
	p += 1;
case 84:
	if ( (*p) == 116 )
		goto st85;
	goto st0;
st85:
	p += 1;
case 85:
	switch( (*p) ) {
		case 0: goto tr78;
		case 43: goto st86;
	}
	goto st0;
st86:
	p += 1;
case 86:
	if ( (*p) == 111 )
		goto st87;
	goto st0;
st87:
	p += 1;
case 87:
	if ( (*p) == 99 )
		goto st88;
	goto st0;
st88:
	p += 1;
case 88:
	if ( (*p) == 116 )
		goto st82;
	goto st0;
st89:
	p += 1;
case 89:
	if ( (*p) == 111 )
		goto st15;
	goto st0;
st90:
	p += 1;
case 90:
	if ( (*p) == 116 )
		goto st91;
	goto st0;
st91:
	p += 1;
case 91:
	if ( (*p) == 104 )
		goto st92;
	goto st0;
st92:
	p += 1;
case 92:
	if ( (*p) == 0 )
		goto tr52;
	goto st0;
st93:
	p += 1;
case 93:
	if ( (*p) == 116 )
		goto st94;
	goto st0;
st94:
	p += 1;
case 94:
	if ( (*p) == 104 )
		goto st95;
	goto st0;
st95:
	p += 1;
case 95:
	if ( (*p) == 0 )
		goto tr63;
	goto st0;
st96:
	p += 1;
case 96:
	if ( (*p) == 111 )
		goto st97;
	goto st0;
st97:
	p += 1;
case 97:
	if ( (*p) == 109 )
		goto st98;
	goto st0;
st98:
	p += 1;
case 98:
	if ( (*p) == 98 )
		goto st99;
	goto st0;
st99:
	p += 1;
case 99:
	if ( (*p) == 0 )
		goto tr119;
	goto st0;
st100:
	p += 1;
case 100:
	if ( (*p) == 101 )
		goto st101;
	goto st0;
st101:
	p += 1;
case 101:
	switch( (*p) ) {
		case 101: goto st102;
		case 116: goto st104;
	}
	goto st0;
st102:
	p += 1;
case 102:
	if ( (*p) == 112 )
		goto st103;
	goto st0;
st103:
	p += 1;
case 103:
	if ( (*p) == 0 )
		goto tr124;
	goto st0;
st104:
	p += 1;
case 104:
	if ( (*p) == 117 )
		goto st105;
	goto st0;
st105:
	p += 1;
case 105:
	if ( (*p) == 110 )
		goto st106;
	goto st0;
st106:
	p += 1;
case 106:
	if ( (*p) == 101 )
		goto st107;
	goto st0;
st107:
	p += 1;
case 107:
	switch( (*p) ) {
		case 49: goto st108;
		case 50: goto st103;
	}
	goto st0;
st108:
	p += 1;
case 108:
	if ( (*p) == 0 )
		goto tr129;
	goto st0;
st109:
	p += 1;
case 109:
	if ( (*p) == 104 )
		goto st110;
	goto st0;
st110:
	p += 1;
case 110:
	if ( (*p) == 97 )
		goto st111;
	goto st0;
st111:
	p += 1;
case 111:
	if ( (*p) == 108 )
		goto st112;
	goto st0;
st112:
	p += 1;
case 112:
	if ( (*p) == 108 )
		goto st113;
	goto st0;
st113:
	p += 1;
case 113:
	if ( (*p) == 111 )
		goto st114;
	goto st0;
st114:
	p += 1;
case 114:
	if ( (*p) == 119 )
		goto st108;
	goto st0;
	}

	_out: {}
	}

#line 47 "parser_whammy.rl"

    const auto ok = cs >= 115;
    if (ok)
        mode = mode_;

    return ok;
}

}
}
