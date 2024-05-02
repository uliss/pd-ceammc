
#line 1 "lex/parser_guido.rl"
# include "parser_guido.h"
# include "fmt/core.h"
# include "ceammc_log.h"

namespace ceammc {
namespace parser {


#line 12 "lex/parser_guido.cpp"
static const int guido_note_parser_start = 1;
static const int guido_note_parser_first_final = 48;
static const int guido_note_parser_error = 0;

static const int guido_note_parser_en_main = 1;


#line 47 "lex/parser_guido.rl"


bool parse_guido_note(const char* str, ceammc::music::PitchClass& pc, int& octave, music::Duration& d)
{
    if (str == 0) {
        return false;
    }

    int cs = 0;
    const char* p = str;

    auto pitch = music::PitchClass::C;
    auto alt = ceammc::music::Alteration::NATURAL;
    int oct = octave;
    int oct_sign = 1;
    auto dur = d;
    int num = 0;
    int dur_num = 0;
    int dur_den = 1;
    int dots = 0;

    
#line 43 "lex/parser_guido.cpp"
	{
	cs = guido_note_parser_start;
	}

#line 69 "lex/parser_guido.rl"
    
#line 50 "lex/parser_guido.cpp"
	{
	switch ( cs )
	{
case 1:
	switch( (*p) ) {
		case 95: goto st2;
		case 97: goto st12;
		case 98: goto st19;
		case 99: goto tr4;
		case 100: goto tr5;
		case 101: goto st27;
		case 102: goto tr7;
		case 103: goto st32;
		case 104: goto st35;
		case 108: goto st36;
		case 109: goto st38;
		case 114: goto st40;
		case 115: goto st42;
		case 116: goto st46;
	}
	goto st0;
st0:
cs = 0;
	goto _out;
tr40:
#line 16 "lex/parser_guido.rl"
	{ pitch = music::PitchClass::A; }
#line 33 "lex/parser_guido.rl"
	{ oct = oct_sign * ((*p) - '0'); }
	goto st2;
tr44:
#line 33 "lex/parser_guido.rl"
	{ oct = oct_sign * ((*p) - '0'); }
	goto st2;
tr53:
#line 16 "lex/parser_guido.rl"
	{ ++alt; }
#line 33 "lex/parser_guido.rl"
	{ oct = oct_sign * ((*p) - '0'); }
	goto st2;
tr60:
#line 17 "lex/parser_guido.rl"
	{ pitch = music::PitchClass::B; }
#line 33 "lex/parser_guido.rl"
	{ oct = oct_sign * ((*p) - '0'); }
	goto st2;
tr71:
#line 18 "lex/parser_guido.rl"
	{ ++alt; }
#line 33 "lex/parser_guido.rl"
	{ oct = oct_sign * ((*p) - '0'); }
	goto st2;
tr81:
#line 19 "lex/parser_guido.rl"
	{ ++alt; }
#line 33 "lex/parser_guido.rl"
	{ oct = oct_sign * ((*p) - '0'); }
	goto st2;
tr88:
#line 24 "lex/parser_guido.rl"
	{ pitch = music::PitchClass::C; }
#line 33 "lex/parser_guido.rl"
	{ oct = oct_sign * ((*p) - '0'); }
	goto st2;
tr95:
#line 20 "lex/parser_guido.rl"
	{ pitch = music::PitchClass::E; }
#line 33 "lex/parser_guido.rl"
	{ oct = oct_sign * ((*p) - '0'); }
	goto st2;
tr104:
#line 27 "lex/parser_guido.rl"
	{ pitch = music::PitchClass::F; }
#line 33 "lex/parser_guido.rl"
	{ oct = oct_sign * ((*p) - '0'); }
	goto st2;
tr112:
#line 21 "lex/parser_guido.rl"
	{ ++alt; }
#line 33 "lex/parser_guido.rl"
	{ oct = oct_sign * ((*p) - '0'); }
	goto st2;
tr119:
#line 22 "lex/parser_guido.rl"
	{ pitch = music::PitchClass::G; }
#line 33 "lex/parser_guido.rl"
	{ oct = oct_sign * ((*p) - '0'); }
	goto st2;
tr128:
#line 22 "lex/parser_guido.rl"
	{ ++alt; }
#line 33 "lex/parser_guido.rl"
	{ oct = oct_sign * ((*p) - '0'); }
	goto st2;
tr135:
#line 23 "lex/parser_guido.rl"
	{ pitch = music::PitchClass::B; }
#line 33 "lex/parser_guido.rl"
	{ oct = oct_sign * ((*p) - '0'); }
	goto st2;
tr143:
#line 29 "lex/parser_guido.rl"
	{ pitch = music::PitchClass::A; }
#line 33 "lex/parser_guido.rl"
	{ oct = oct_sign * ((*p) - '0'); }
	goto st2;
tr151:
#line 26 "lex/parser_guido.rl"
	{ pitch = music::PitchClass::E; }
#line 33 "lex/parser_guido.rl"
	{ oct = oct_sign * ((*p) - '0'); }
	goto st2;
tr159:
#line 25 "lex/parser_guido.rl"
	{ pitch = music::PitchClass::D; }
#line 33 "lex/parser_guido.rl"
	{ oct = oct_sign * ((*p) - '0'); }
	goto st2;
tr168:
#line 30 "lex/parser_guido.rl"
	{ pitch = music::PitchClass::B; }
#line 33 "lex/parser_guido.rl"
	{ oct = oct_sign * ((*p) - '0'); }
	goto st2;
tr176:
#line 28 "lex/parser_guido.rl"
	{ pitch = music::PitchClass::G; }
#line 33 "lex/parser_guido.rl"
	{ oct = oct_sign * ((*p) - '0'); }
	goto st2;
tr184:
#line 31 "lex/parser_guido.rl"
	{ pitch = music::PitchClass::B; }
#line 33 "lex/parser_guido.rl"
	{ oct = oct_sign * ((*p) - '0'); }
	goto st2;
st2:
	p += 1;
case 2:
#line 190 "lex/parser_guido.cpp"
	switch( (*p) ) {
		case 0: goto tr15;
		case 42: goto tr16;
		case 47: goto tr17;
	}
	goto st0;
tr15:
#line 45 "lex/parser_guido.rl"
	{ {p++; cs = 48; goto _out;} }
	goto st48;
tr19:
#line 38 "lex/parser_guido.rl"
	{ dur_num = num; dur_den = 1; }
#line 40 "lex/parser_guido.rl"
	{ dur.set(dur_num, dur_den, dots); }
#line 45 "lex/parser_guido.rl"
	{ {p++; cs = 48; goto _out;} }
	goto st48;
tr23:
#line 40 "lex/parser_guido.rl"
	{ dur.set(dur_num, dur_den, dots); }
#line 45 "lex/parser_guido.rl"
	{ {p++; cs = 48; goto _out;} }
	goto st48;
tr27:
#line 37 "lex/parser_guido.rl"
	{ dur_den = num; }
#line 40 "lex/parser_guido.rl"
	{ dur.set(dur_num, dur_den, dots); }
#line 45 "lex/parser_guido.rl"
	{ {p++; cs = 48; goto _out;} }
	goto st48;
tr31:
#line 39 "lex/parser_guido.rl"
	{ dur_den = num; dur_num = 1; }
#line 40 "lex/parser_guido.rl"
	{ dur.set(dur_num, dur_den, dots); }
#line 45 "lex/parser_guido.rl"
	{ {p++; cs = 48; goto _out;} }
	goto st48;
tr34:
#line 16 "lex/parser_guido.rl"
	{ pitch = music::PitchClass::A; }
#line 45 "lex/parser_guido.rl"
	{ {p++; cs = 48; goto _out;} }
	goto st48;
tr47:
#line 16 "lex/parser_guido.rl"
	{ ++alt; }
#line 45 "lex/parser_guido.rl"
	{ {p++; cs = 48; goto _out;} }
	goto st48;
tr54:
#line 17 "lex/parser_guido.rl"
	{ pitch = music::PitchClass::B; }
#line 45 "lex/parser_guido.rl"
	{ {p++; cs = 48; goto _out;} }
	goto st48;
tr65:
#line 18 "lex/parser_guido.rl"
	{ ++alt; }
#line 45 "lex/parser_guido.rl"
	{ {p++; cs = 48; goto _out;} }
	goto st48;
tr75:
#line 19 "lex/parser_guido.rl"
	{ ++alt; }
#line 45 "lex/parser_guido.rl"
	{ {p++; cs = 48; goto _out;} }
	goto st48;
tr82:
#line 24 "lex/parser_guido.rl"
	{ pitch = music::PitchClass::C; }
#line 45 "lex/parser_guido.rl"
	{ {p++; cs = 48; goto _out;} }
	goto st48;
tr89:
#line 20 "lex/parser_guido.rl"
	{ pitch = music::PitchClass::E; }
#line 45 "lex/parser_guido.rl"
	{ {p++; cs = 48; goto _out;} }
	goto st48;
tr98:
#line 27 "lex/parser_guido.rl"
	{ pitch = music::PitchClass::F; }
#line 45 "lex/parser_guido.rl"
	{ {p++; cs = 48; goto _out;} }
	goto st48;
tr106:
#line 21 "lex/parser_guido.rl"
	{ ++alt; }
#line 45 "lex/parser_guido.rl"
	{ {p++; cs = 48; goto _out;} }
	goto st48;
tr113:
#line 22 "lex/parser_guido.rl"
	{ pitch = music::PitchClass::G; }
#line 45 "lex/parser_guido.rl"
	{ {p++; cs = 48; goto _out;} }
	goto st48;
tr122:
#line 22 "lex/parser_guido.rl"
	{ ++alt; }
#line 45 "lex/parser_guido.rl"
	{ {p++; cs = 48; goto _out;} }
	goto st48;
tr129:
#line 23 "lex/parser_guido.rl"
	{ pitch = music::PitchClass::B; }
#line 45 "lex/parser_guido.rl"
	{ {p++; cs = 48; goto _out;} }
	goto st48;
tr137:
#line 29 "lex/parser_guido.rl"
	{ pitch = music::PitchClass::A; }
#line 45 "lex/parser_guido.rl"
	{ {p++; cs = 48; goto _out;} }
	goto st48;
tr145:
#line 26 "lex/parser_guido.rl"
	{ pitch = music::PitchClass::E; }
#line 45 "lex/parser_guido.rl"
	{ {p++; cs = 48; goto _out;} }
	goto st48;
tr153:
#line 25 "lex/parser_guido.rl"
	{ pitch = music::PitchClass::D; }
#line 45 "lex/parser_guido.rl"
	{ {p++; cs = 48; goto _out;} }
	goto st48;
tr162:
#line 30 "lex/parser_guido.rl"
	{ pitch = music::PitchClass::B; }
#line 45 "lex/parser_guido.rl"
	{ {p++; cs = 48; goto _out;} }
	goto st48;
tr170:
#line 28 "lex/parser_guido.rl"
	{ pitch = music::PitchClass::G; }
#line 45 "lex/parser_guido.rl"
	{ {p++; cs = 48; goto _out;} }
	goto st48;
tr178:
#line 31 "lex/parser_guido.rl"
	{ pitch = music::PitchClass::B; }
#line 45 "lex/parser_guido.rl"
	{ {p++; cs = 48; goto _out;} }
	goto st48;
st48:
	p += 1;
case 48:
#line 342 "lex/parser_guido.cpp"
	goto st0;
tr16:
#line 40 "lex/parser_guido.rl"
	{ num = 0; }
	goto st3;
tr37:
#line 16 "lex/parser_guido.rl"
	{ pitch = music::PitchClass::A; }
#line 40 "lex/parser_guido.rl"
	{ num = 0; }
	goto st3;
tr50:
#line 16 "lex/parser_guido.rl"
	{ ++alt; }
#line 40 "lex/parser_guido.rl"
	{ num = 0; }
	goto st3;
tr57:
#line 17 "lex/parser_guido.rl"
	{ pitch = music::PitchClass::B; }
#line 40 "lex/parser_guido.rl"
	{ num = 0; }
	goto st3;
tr68:
#line 18 "lex/parser_guido.rl"
	{ ++alt; }
#line 40 "lex/parser_guido.rl"
	{ num = 0; }
	goto st3;
tr78:
#line 19 "lex/parser_guido.rl"
	{ ++alt; }
#line 40 "lex/parser_guido.rl"
	{ num = 0; }
	goto st3;
tr85:
#line 24 "lex/parser_guido.rl"
	{ pitch = music::PitchClass::C; }
#line 40 "lex/parser_guido.rl"
	{ num = 0; }
	goto st3;
tr92:
#line 20 "lex/parser_guido.rl"
	{ pitch = music::PitchClass::E; }
#line 40 "lex/parser_guido.rl"
	{ num = 0; }
	goto st3;
tr101:
#line 27 "lex/parser_guido.rl"
	{ pitch = music::PitchClass::F; }
#line 40 "lex/parser_guido.rl"
	{ num = 0; }
	goto st3;
tr109:
#line 21 "lex/parser_guido.rl"
	{ ++alt; }
#line 40 "lex/parser_guido.rl"
	{ num = 0; }
	goto st3;
tr116:
#line 22 "lex/parser_guido.rl"
	{ pitch = music::PitchClass::G; }
#line 40 "lex/parser_guido.rl"
	{ num = 0; }
	goto st3;
tr125:
#line 22 "lex/parser_guido.rl"
	{ ++alt; }
#line 40 "lex/parser_guido.rl"
	{ num = 0; }
	goto st3;
tr132:
#line 23 "lex/parser_guido.rl"
	{ pitch = music::PitchClass::B; }
#line 40 "lex/parser_guido.rl"
	{ num = 0; }
	goto st3;
tr140:
#line 29 "lex/parser_guido.rl"
	{ pitch = music::PitchClass::A; }
#line 40 "lex/parser_guido.rl"
	{ num = 0; }
	goto st3;
tr148:
#line 26 "lex/parser_guido.rl"
	{ pitch = music::PitchClass::E; }
#line 40 "lex/parser_guido.rl"
	{ num = 0; }
	goto st3;
tr156:
#line 25 "lex/parser_guido.rl"
	{ pitch = music::PitchClass::D; }
#line 40 "lex/parser_guido.rl"
	{ num = 0; }
	goto st3;
tr165:
#line 30 "lex/parser_guido.rl"
	{ pitch = music::PitchClass::B; }
#line 40 "lex/parser_guido.rl"
	{ num = 0; }
	goto st3;
tr173:
#line 28 "lex/parser_guido.rl"
	{ pitch = music::PitchClass::G; }
#line 40 "lex/parser_guido.rl"
	{ num = 0; }
	goto st3;
tr181:
#line 31 "lex/parser_guido.rl"
	{ pitch = music::PitchClass::B; }
#line 40 "lex/parser_guido.rl"
	{ num = 0; }
	goto st3;
st3:
	p += 1;
case 3:
#line 459 "lex/parser_guido.cpp"
	if ( 49 <= (*p) && (*p) <= 57 )
		goto tr18;
	goto st0;
tr18:
#line 36 "lex/parser_guido.rl"
	{ num = (*p) - '0'; }
	goto st4;
tr22:
#line 36 "lex/parser_guido.rl"
	{ num = (10 * num) + ((*p) - '0'); }
	goto st4;
st4:
	p += 1;
case 4:
#line 474 "lex/parser_guido.cpp"
	switch( (*p) ) {
		case 0: goto tr19;
		case 46: goto tr20;
		case 47: goto tr21;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr22;
	goto st0;
tr20:
#line 38 "lex/parser_guido.rl"
	{ dur_num = num; dur_den = 1; }
#line 34 "lex/parser_guido.rl"
	{ dots++; }
	goto st5;
tr28:
#line 37 "lex/parser_guido.rl"
	{ dur_den = num; }
#line 34 "lex/parser_guido.rl"
	{ dots++; }
	goto st5;
tr32:
#line 39 "lex/parser_guido.rl"
	{ dur_den = num; dur_num = 1; }
#line 34 "lex/parser_guido.rl"
	{ dots++; }
	goto st5;
st5:
	p += 1;
case 5:
#line 504 "lex/parser_guido.cpp"
	switch( (*p) ) {
		case 0: goto tr23;
		case 46: goto tr24;
	}
	goto st0;
tr24:
#line 34 "lex/parser_guido.rl"
	{ dots++; }
	goto st6;
st6:
	p += 1;
case 6:
#line 517 "lex/parser_guido.cpp"
	switch( (*p) ) {
		case 0: goto tr23;
		case 46: goto tr25;
	}
	goto st0;
tr25:
#line 34 "lex/parser_guido.rl"
	{ dots++; }
	goto st7;
st7:
	p += 1;
case 7:
#line 530 "lex/parser_guido.cpp"
	if ( (*p) == 0 )
		goto tr23;
	goto st0;
tr21:
#line 37 "lex/parser_guido.rl"
	{ dur_num = num; }
	goto st8;
st8:
	p += 1;
case 8:
#line 541 "lex/parser_guido.cpp"
	if ( 49 <= (*p) && (*p) <= 57 )
		goto tr26;
	goto st0;
tr26:
#line 36 "lex/parser_guido.rl"
	{ num = (*p) - '0'; }
	goto st9;
tr29:
#line 36 "lex/parser_guido.rl"
	{ num = (10 * num) + ((*p) - '0'); }
	goto st9;
st9:
	p += 1;
case 9:
#line 556 "lex/parser_guido.cpp"
	switch( (*p) ) {
		case 0: goto tr27;
		case 46: goto tr28;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr29;
	goto st0;
tr17:
#line 40 "lex/parser_guido.rl"
	{ num = 0; }
	goto st10;
tr39:
#line 16 "lex/parser_guido.rl"
	{ pitch = music::PitchClass::A; }
#line 40 "lex/parser_guido.rl"
	{ num = 0; }
	goto st10;
tr52:
#line 16 "lex/parser_guido.rl"
	{ ++alt; }
#line 40 "lex/parser_guido.rl"
	{ num = 0; }
	goto st10;
tr59:
#line 17 "lex/parser_guido.rl"
	{ pitch = music::PitchClass::B; }
#line 40 "lex/parser_guido.rl"
	{ num = 0; }
	goto st10;
tr70:
#line 18 "lex/parser_guido.rl"
	{ ++alt; }
#line 40 "lex/parser_guido.rl"
	{ num = 0; }
	goto st10;
tr80:
#line 19 "lex/parser_guido.rl"
	{ ++alt; }
#line 40 "lex/parser_guido.rl"
	{ num = 0; }
	goto st10;
tr87:
#line 24 "lex/parser_guido.rl"
	{ pitch = music::PitchClass::C; }
#line 40 "lex/parser_guido.rl"
	{ num = 0; }
	goto st10;
tr94:
#line 20 "lex/parser_guido.rl"
	{ pitch = music::PitchClass::E; }
#line 40 "lex/parser_guido.rl"
	{ num = 0; }
	goto st10;
tr103:
#line 27 "lex/parser_guido.rl"
	{ pitch = music::PitchClass::F; }
#line 40 "lex/parser_guido.rl"
	{ num = 0; }
	goto st10;
tr111:
#line 21 "lex/parser_guido.rl"
	{ ++alt; }
#line 40 "lex/parser_guido.rl"
	{ num = 0; }
	goto st10;
tr118:
#line 22 "lex/parser_guido.rl"
	{ pitch = music::PitchClass::G; }
#line 40 "lex/parser_guido.rl"
	{ num = 0; }
	goto st10;
tr127:
#line 22 "lex/parser_guido.rl"
	{ ++alt; }
#line 40 "lex/parser_guido.rl"
	{ num = 0; }
	goto st10;
tr134:
#line 23 "lex/parser_guido.rl"
	{ pitch = music::PitchClass::B; }
#line 40 "lex/parser_guido.rl"
	{ num = 0; }
	goto st10;
tr142:
#line 29 "lex/parser_guido.rl"
	{ pitch = music::PitchClass::A; }
#line 40 "lex/parser_guido.rl"
	{ num = 0; }
	goto st10;
tr150:
#line 26 "lex/parser_guido.rl"
	{ pitch = music::PitchClass::E; }
#line 40 "lex/parser_guido.rl"
	{ num = 0; }
	goto st10;
tr158:
#line 25 "lex/parser_guido.rl"
	{ pitch = music::PitchClass::D; }
#line 40 "lex/parser_guido.rl"
	{ num = 0; }
	goto st10;
tr167:
#line 30 "lex/parser_guido.rl"
	{ pitch = music::PitchClass::B; }
#line 40 "lex/parser_guido.rl"
	{ num = 0; }
	goto st10;
tr175:
#line 28 "lex/parser_guido.rl"
	{ pitch = music::PitchClass::G; }
#line 40 "lex/parser_guido.rl"
	{ num = 0; }
	goto st10;
tr183:
#line 31 "lex/parser_guido.rl"
	{ pitch = music::PitchClass::B; }
#line 40 "lex/parser_guido.rl"
	{ num = 0; }
	goto st10;
st10:
	p += 1;
case 10:
#line 679 "lex/parser_guido.cpp"
	if ( 49 <= (*p) && (*p) <= 57 )
		goto tr30;
	goto st0;
tr30:
#line 36 "lex/parser_guido.rl"
	{ num = (*p) - '0'; }
	goto st11;
tr33:
#line 36 "lex/parser_guido.rl"
	{ num = (10 * num) + ((*p) - '0'); }
	goto st11;
st11:
	p += 1;
case 11:
#line 694 "lex/parser_guido.cpp"
	switch( (*p) ) {
		case 0: goto tr31;
		case 46: goto tr32;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr33;
	goto st0;
st12:
	p += 1;
case 12:
	switch( (*p) ) {
		case 0: goto tr34;
		case 35: goto tr35;
		case 38: goto tr36;
		case 42: goto tr37;
		case 45: goto tr38;
		case 47: goto tr39;
		case 105: goto tr41;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr40;
	goto st0;
tr35:
#line 16 "lex/parser_guido.rl"
	{ pitch = music::PitchClass::A; }
#line 14 "lex/parser_guido.rl"
	{ ++alt; }
	goto st13;
tr61:
#line 14 "lex/parser_guido.rl"
	{ ++alt; }
	goto st13;
tr48:
#line 16 "lex/parser_guido.rl"
	{ ++alt; }
#line 14 "lex/parser_guido.rl"
	{ ++alt; }
	goto st13;
tr55:
#line 17 "lex/parser_guido.rl"
	{ pitch = music::PitchClass::B; }
#line 14 "lex/parser_guido.rl"
	{ ++alt; }
	goto st13;
tr66:
#line 18 "lex/parser_guido.rl"
	{ ++alt; }
#line 14 "lex/parser_guido.rl"
	{ ++alt; }
	goto st13;
tr76:
#line 19 "lex/parser_guido.rl"
	{ ++alt; }
#line 14 "lex/parser_guido.rl"
	{ ++alt; }
	goto st13;
tr83:
#line 24 "lex/parser_guido.rl"
	{ pitch = music::PitchClass::C; }
#line 14 "lex/parser_guido.rl"
	{ ++alt; }
	goto st13;
tr90:
#line 20 "lex/parser_guido.rl"
	{ pitch = music::PitchClass::E; }
#line 14 "lex/parser_guido.rl"
	{ ++alt; }
	goto st13;
tr99:
#line 27 "lex/parser_guido.rl"
	{ pitch = music::PitchClass::F; }
#line 14 "lex/parser_guido.rl"
	{ ++alt; }
	goto st13;
tr107:
#line 21 "lex/parser_guido.rl"
	{ ++alt; }
#line 14 "lex/parser_guido.rl"
	{ ++alt; }
	goto st13;
tr114:
#line 22 "lex/parser_guido.rl"
	{ pitch = music::PitchClass::G; }
#line 14 "lex/parser_guido.rl"
	{ ++alt; }
	goto st13;
tr123:
#line 22 "lex/parser_guido.rl"
	{ ++alt; }
#line 14 "lex/parser_guido.rl"
	{ ++alt; }
	goto st13;
tr130:
#line 23 "lex/parser_guido.rl"
	{ pitch = music::PitchClass::B; }
#line 14 "lex/parser_guido.rl"
	{ ++alt; }
	goto st13;
tr138:
#line 29 "lex/parser_guido.rl"
	{ pitch = music::PitchClass::A; }
#line 14 "lex/parser_guido.rl"
	{ ++alt; }
	goto st13;
tr146:
#line 26 "lex/parser_guido.rl"
	{ pitch = music::PitchClass::E; }
#line 14 "lex/parser_guido.rl"
	{ ++alt; }
	goto st13;
tr154:
#line 25 "lex/parser_guido.rl"
	{ pitch = music::PitchClass::D; }
#line 14 "lex/parser_guido.rl"
	{ ++alt; }
	goto st13;
tr163:
#line 30 "lex/parser_guido.rl"
	{ pitch = music::PitchClass::B; }
#line 14 "lex/parser_guido.rl"
	{ ++alt; }
	goto st13;
tr171:
#line 28 "lex/parser_guido.rl"
	{ pitch = music::PitchClass::G; }
#line 14 "lex/parser_guido.rl"
	{ ++alt; }
	goto st13;
tr179:
#line 31 "lex/parser_guido.rl"
	{ pitch = music::PitchClass::B; }
#line 14 "lex/parser_guido.rl"
	{ ++alt; }
	goto st13;
st13:
	p += 1;
case 13:
#line 832 "lex/parser_guido.cpp"
	switch( (*p) ) {
		case 0: goto tr15;
		case 35: goto tr42;
		case 42: goto tr16;
		case 45: goto tr43;
		case 47: goto tr17;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr44;
	goto st0;
tr42:
#line 14 "lex/parser_guido.rl"
	{ ++alt; }
	goto st14;
tr45:
#line 15 "lex/parser_guido.rl"
	{ --alt; }
	goto st14;
st14:
	p += 1;
case 14:
#line 854 "lex/parser_guido.cpp"
	switch( (*p) ) {
		case 0: goto tr15;
		case 42: goto tr16;
		case 45: goto tr43;
		case 47: goto tr17;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr44;
	goto st0;
tr38:
#line 16 "lex/parser_guido.rl"
	{ pitch = music::PitchClass::A; }
#line 33 "lex/parser_guido.rl"
	{ oct_sign = -1; }
	goto st15;
tr43:
#line 33 "lex/parser_guido.rl"
	{ oct_sign = -1; }
	goto st15;
tr51:
#line 16 "lex/parser_guido.rl"
	{ ++alt; }
#line 33 "lex/parser_guido.rl"
	{ oct_sign = -1; }
	goto st15;
tr58:
#line 17 "lex/parser_guido.rl"
	{ pitch = music::PitchClass::B; }
#line 33 "lex/parser_guido.rl"
	{ oct_sign = -1; }
	goto st15;
tr69:
#line 18 "lex/parser_guido.rl"
	{ ++alt; }
#line 33 "lex/parser_guido.rl"
	{ oct_sign = -1; }
	goto st15;
tr79:
#line 19 "lex/parser_guido.rl"
	{ ++alt; }
#line 33 "lex/parser_guido.rl"
	{ oct_sign = -1; }
	goto st15;
tr86:
#line 24 "lex/parser_guido.rl"
	{ pitch = music::PitchClass::C; }
#line 33 "lex/parser_guido.rl"
	{ oct_sign = -1; }
	goto st15;
tr93:
#line 20 "lex/parser_guido.rl"
	{ pitch = music::PitchClass::E; }
#line 33 "lex/parser_guido.rl"
	{ oct_sign = -1; }
	goto st15;
tr102:
#line 27 "lex/parser_guido.rl"
	{ pitch = music::PitchClass::F; }
#line 33 "lex/parser_guido.rl"
	{ oct_sign = -1; }
	goto st15;
tr110:
#line 21 "lex/parser_guido.rl"
	{ ++alt; }
#line 33 "lex/parser_guido.rl"
	{ oct_sign = -1; }
	goto st15;
tr117:
#line 22 "lex/parser_guido.rl"
	{ pitch = music::PitchClass::G; }
#line 33 "lex/parser_guido.rl"
	{ oct_sign = -1; }
	goto st15;
tr126:
#line 22 "lex/parser_guido.rl"
	{ ++alt; }
#line 33 "lex/parser_guido.rl"
	{ oct_sign = -1; }
	goto st15;
tr133:
#line 23 "lex/parser_guido.rl"
	{ pitch = music::PitchClass::B; }
#line 33 "lex/parser_guido.rl"
	{ oct_sign = -1; }
	goto st15;
tr141:
#line 29 "lex/parser_guido.rl"
	{ pitch = music::PitchClass::A; }
#line 33 "lex/parser_guido.rl"
	{ oct_sign = -1; }
	goto st15;
tr149:
#line 26 "lex/parser_guido.rl"
	{ pitch = music::PitchClass::E; }
#line 33 "lex/parser_guido.rl"
	{ oct_sign = -1; }
	goto st15;
tr157:
#line 25 "lex/parser_guido.rl"
	{ pitch = music::PitchClass::D; }
#line 33 "lex/parser_guido.rl"
	{ oct_sign = -1; }
	goto st15;
tr166:
#line 30 "lex/parser_guido.rl"
	{ pitch = music::PitchClass::B; }
#line 33 "lex/parser_guido.rl"
	{ oct_sign = -1; }
	goto st15;
tr174:
#line 28 "lex/parser_guido.rl"
	{ pitch = music::PitchClass::G; }
#line 33 "lex/parser_guido.rl"
	{ oct_sign = -1; }
	goto st15;
tr182:
#line 31 "lex/parser_guido.rl"
	{ pitch = music::PitchClass::B; }
#line 33 "lex/parser_guido.rl"
	{ oct_sign = -1; }
	goto st15;
st15:
	p += 1;
case 15:
#line 979 "lex/parser_guido.cpp"
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr44;
	goto st0;
tr36:
#line 16 "lex/parser_guido.rl"
	{ pitch = music::PitchClass::A; }
#line 15 "lex/parser_guido.rl"
	{ --alt; }
	goto st16;
tr62:
#line 15 "lex/parser_guido.rl"
	{ --alt; }
	goto st16;
tr49:
#line 16 "lex/parser_guido.rl"
	{ ++alt; }
#line 15 "lex/parser_guido.rl"
	{ --alt; }
	goto st16;
tr56:
#line 17 "lex/parser_guido.rl"
	{ pitch = music::PitchClass::B; }
#line 15 "lex/parser_guido.rl"
	{ --alt; }
	goto st16;
tr67:
#line 18 "lex/parser_guido.rl"
	{ ++alt; }
#line 15 "lex/parser_guido.rl"
	{ --alt; }
	goto st16;
tr77:
#line 19 "lex/parser_guido.rl"
	{ ++alt; }
#line 15 "lex/parser_guido.rl"
	{ --alt; }
	goto st16;
tr84:
#line 24 "lex/parser_guido.rl"
	{ pitch = music::PitchClass::C; }
#line 15 "lex/parser_guido.rl"
	{ --alt; }
	goto st16;
tr91:
#line 20 "lex/parser_guido.rl"
	{ pitch = music::PitchClass::E; }
#line 15 "lex/parser_guido.rl"
	{ --alt; }
	goto st16;
tr100:
#line 27 "lex/parser_guido.rl"
	{ pitch = music::PitchClass::F; }
#line 15 "lex/parser_guido.rl"
	{ --alt; }
	goto st16;
tr108:
#line 21 "lex/parser_guido.rl"
	{ ++alt; }
#line 15 "lex/parser_guido.rl"
	{ --alt; }
	goto st16;
tr115:
#line 22 "lex/parser_guido.rl"
	{ pitch = music::PitchClass::G; }
#line 15 "lex/parser_guido.rl"
	{ --alt; }
	goto st16;
tr124:
#line 22 "lex/parser_guido.rl"
	{ ++alt; }
#line 15 "lex/parser_guido.rl"
	{ --alt; }
	goto st16;
tr131:
#line 23 "lex/parser_guido.rl"
	{ pitch = music::PitchClass::B; }
#line 15 "lex/parser_guido.rl"
	{ --alt; }
	goto st16;
tr139:
#line 29 "lex/parser_guido.rl"
	{ pitch = music::PitchClass::A; }
#line 15 "lex/parser_guido.rl"
	{ --alt; }
	goto st16;
tr147:
#line 26 "lex/parser_guido.rl"
	{ pitch = music::PitchClass::E; }
#line 15 "lex/parser_guido.rl"
	{ --alt; }
	goto st16;
tr155:
#line 25 "lex/parser_guido.rl"
	{ pitch = music::PitchClass::D; }
#line 15 "lex/parser_guido.rl"
	{ --alt; }
	goto st16;
tr164:
#line 30 "lex/parser_guido.rl"
	{ pitch = music::PitchClass::B; }
#line 15 "lex/parser_guido.rl"
	{ --alt; }
	goto st16;
tr172:
#line 28 "lex/parser_guido.rl"
	{ pitch = music::PitchClass::G; }
#line 15 "lex/parser_guido.rl"
	{ --alt; }
	goto st16;
tr180:
#line 31 "lex/parser_guido.rl"
	{ pitch = music::PitchClass::B; }
#line 15 "lex/parser_guido.rl"
	{ --alt; }
	goto st16;
st16:
	p += 1;
case 16:
#line 1098 "lex/parser_guido.cpp"
	switch( (*p) ) {
		case 0: goto tr15;
		case 38: goto tr45;
		case 42: goto tr16;
		case 45: goto tr43;
		case 47: goto tr17;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr44;
	goto st0;
tr41:
#line 16 "lex/parser_guido.rl"
	{ pitch = music::PitchClass::A; }
	goto st17;
st17:
	p += 1;
case 17:
#line 1116 "lex/parser_guido.cpp"
	if ( (*p) == 115 )
		goto st18;
	goto st0;
st18:
	p += 1;
case 18:
	switch( (*p) ) {
		case 0: goto tr47;
		case 35: goto tr48;
		case 38: goto tr49;
		case 42: goto tr50;
		case 45: goto tr51;
		case 47: goto tr52;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr53;
	goto st0;
st19:
	p += 1;
case 19:
	switch( (*p) ) {
		case 0: goto tr54;
		case 35: goto tr55;
		case 38: goto tr56;
		case 42: goto tr57;
		case 45: goto tr58;
		case 47: goto tr59;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr60;
	goto st0;
tr4:
#line 18 "lex/parser_guido.rl"
	{ pitch = music::PitchClass::C; }
	goto st20;
st20:
	p += 1;
case 20:
#line 1155 "lex/parser_guido.cpp"
	switch( (*p) ) {
		case 0: goto tr15;
		case 35: goto tr61;
		case 38: goto tr62;
		case 42: goto tr16;
		case 45: goto tr43;
		case 47: goto tr17;
		case 105: goto st21;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr44;
	goto st0;
st21:
	p += 1;
case 21:
	if ( (*p) == 115 )
		goto st22;
	goto st0;
st22:
	p += 1;
case 22:
	switch( (*p) ) {
		case 0: goto tr65;
		case 35: goto tr66;
		case 38: goto tr67;
		case 42: goto tr68;
		case 45: goto tr69;
		case 47: goto tr70;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr71;
	goto st0;
tr5:
#line 19 "lex/parser_guido.rl"
	{ pitch = music::PitchClass::D; }
	goto st23;
st23:
	p += 1;
case 23:
#line 1195 "lex/parser_guido.cpp"
	switch( (*p) ) {
		case 0: goto tr15;
		case 35: goto tr61;
		case 38: goto tr62;
		case 42: goto tr16;
		case 45: goto tr43;
		case 47: goto tr17;
		case 105: goto st24;
		case 111: goto st26;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr44;
	goto st0;
st24:
	p += 1;
case 24:
	if ( (*p) == 115 )
		goto st25;
	goto st0;
st25:
	p += 1;
case 25:
	switch( (*p) ) {
		case 0: goto tr75;
		case 35: goto tr76;
		case 38: goto tr77;
		case 42: goto tr78;
		case 45: goto tr79;
		case 47: goto tr80;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr81;
	goto st0;
st26:
	p += 1;
case 26:
	switch( (*p) ) {
		case 0: goto tr82;
		case 35: goto tr83;
		case 38: goto tr84;
		case 42: goto tr85;
		case 45: goto tr86;
		case 47: goto tr87;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr88;
	goto st0;
st27:
	p += 1;
case 27:
	switch( (*p) ) {
		case 0: goto tr89;
		case 35: goto tr90;
		case 38: goto tr91;
		case 42: goto tr92;
		case 45: goto tr93;
		case 47: goto tr94;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr95;
	goto st0;
tr7:
#line 21 "lex/parser_guido.rl"
	{ pitch = music::PitchClass::F; }
	goto st28;
st28:
	p += 1;
case 28:
#line 1264 "lex/parser_guido.cpp"
	switch( (*p) ) {
		case 0: goto tr15;
		case 35: goto tr61;
		case 38: goto tr62;
		case 42: goto tr16;
		case 45: goto tr43;
		case 47: goto tr17;
		case 97: goto st29;
		case 105: goto st30;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr44;
	goto st0;
st29:
	p += 1;
case 29:
	switch( (*p) ) {
		case 0: goto tr98;
		case 35: goto tr99;
		case 38: goto tr100;
		case 42: goto tr101;
		case 45: goto tr102;
		case 47: goto tr103;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr104;
	goto st0;
st30:
	p += 1;
case 30:
	if ( (*p) == 115 )
		goto st31;
	goto st0;
st31:
	p += 1;
case 31:
	switch( (*p) ) {
		case 0: goto tr106;
		case 35: goto tr107;
		case 38: goto tr108;
		case 42: goto tr109;
		case 45: goto tr110;
		case 47: goto tr111;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr112;
	goto st0;
st32:
	p += 1;
case 32:
	switch( (*p) ) {
		case 0: goto tr113;
		case 35: goto tr114;
		case 38: goto tr115;
		case 42: goto tr116;
		case 45: goto tr117;
		case 47: goto tr118;
		case 105: goto tr120;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr119;
	goto st0;
tr120:
#line 22 "lex/parser_guido.rl"
	{ pitch = music::PitchClass::G; }
	goto st33;
st33:
	p += 1;
case 33:
#line 1334 "lex/parser_guido.cpp"
	if ( (*p) == 115 )
		goto st34;
	goto st0;
st34:
	p += 1;
case 34:
	switch( (*p) ) {
		case 0: goto tr122;
		case 35: goto tr123;
		case 38: goto tr124;
		case 42: goto tr125;
		case 45: goto tr126;
		case 47: goto tr127;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr128;
	goto st0;
st35:
	p += 1;
case 35:
	switch( (*p) ) {
		case 0: goto tr129;
		case 35: goto tr130;
		case 38: goto tr131;
		case 42: goto tr132;
		case 45: goto tr133;
		case 47: goto tr134;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr135;
	goto st0;
st36:
	p += 1;
case 36:
	if ( (*p) == 97 )
		goto st37;
	goto st0;
st37:
	p += 1;
case 37:
	switch( (*p) ) {
		case 0: goto tr137;
		case 35: goto tr138;
		case 38: goto tr139;
		case 42: goto tr140;
		case 45: goto tr141;
		case 47: goto tr142;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr143;
	goto st0;
st38:
	p += 1;
case 38:
	if ( (*p) == 105 )
		goto st39;
	goto st0;
st39:
	p += 1;
case 39:
	switch( (*p) ) {
		case 0: goto tr145;
		case 35: goto tr146;
		case 38: goto tr147;
		case 42: goto tr148;
		case 45: goto tr149;
		case 47: goto tr150;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr151;
	goto st0;
st40:
	p += 1;
case 40:
	if ( (*p) == 101 )
		goto st41;
	goto st0;
st41:
	p += 1;
case 41:
	switch( (*p) ) {
		case 0: goto tr153;
		case 35: goto tr154;
		case 38: goto tr155;
		case 42: goto tr156;
		case 45: goto tr157;
		case 47: goto tr158;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr159;
	goto st0;
st42:
	p += 1;
case 42:
	switch( (*p) ) {
		case 105: goto st43;
		case 111: goto st44;
	}
	goto st0;
st43:
	p += 1;
case 43:
	switch( (*p) ) {
		case 0: goto tr162;
		case 35: goto tr163;
		case 38: goto tr164;
		case 42: goto tr165;
		case 45: goto tr166;
		case 47: goto tr167;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr168;
	goto st0;
st44:
	p += 1;
case 44:
	if ( (*p) == 108 )
		goto st45;
	goto st0;
st45:
	p += 1;
case 45:
	switch( (*p) ) {
		case 0: goto tr170;
		case 35: goto tr171;
		case 38: goto tr172;
		case 42: goto tr173;
		case 45: goto tr174;
		case 47: goto tr175;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr176;
	goto st0;
st46:
	p += 1;
case 46:
	if ( (*p) == 105 )
		goto st47;
	goto st0;
st47:
	p += 1;
case 47:
	switch( (*p) ) {
		case 0: goto tr178;
		case 35: goto tr179;
		case 38: goto tr180;
		case 42: goto tr181;
		case 45: goto tr182;
		case 47: goto tr183;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr184;
	goto st0;
	}

	_out: {}
	}

#line 70 "lex/parser_guido.rl"

    const auto ok = cs >= 48;

    if (ok) {
        pitch.setAlteration(alt);
        pc = pitch;
        octave = oct;
        d = dur;
        return true;
    } else {
        return false;
    }
}

}
}
