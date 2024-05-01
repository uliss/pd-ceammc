
#line 1 "lex/parser_guido.rl"
# include "parser_guido.h"
# include "fmt/core.h"
# include "ceammc_log.h"

namespace ceammc {
namespace parser {


#line 12 "lex/parser_guido.cpp"
static const int guido_note_parser_start = 1;
static const int guido_note_parser_first_final = 44;
static const int guido_note_parser_error = 0;

static const int guido_note_parser_en_main = 1;


#line 39 "lex/parser_guido.rl"


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

    
#line 39 "lex/parser_guido.cpp"
	{
	cs = guido_note_parser_start;
	}

#line 57 "lex/parser_guido.rl"
    
#line 46 "lex/parser_guido.cpp"
	{
	switch ( cs )
	{
case 1:
	switch( (*p) ) {
		case 95: goto st2;
		case 97: goto st8;
		case 98: goto st15;
		case 99: goto tr4;
		case 100: goto tr5;
		case 101: goto st23;
		case 102: goto tr7;
		case 103: goto st28;
		case 104: goto st31;
		case 108: goto st32;
		case 109: goto st34;
		case 114: goto st36;
		case 115: goto st38;
		case 116: goto st42;
	}
	goto st0;
st0:
cs = 0;
	goto _out;
tr26:
#line 13 "lex/parser_guido.rl"
	{ pitch = music::PitchClass::A; }
#line 30 "lex/parser_guido.rl"
	{ oct = oct_sign * ((*p) - '0'); }
	goto st2;
tr30:
#line 30 "lex/parser_guido.rl"
	{ oct = oct_sign * ((*p) - '0'); }
	goto st2;
tr38:
#line 13 "lex/parser_guido.rl"
	{ ++alt; }
#line 30 "lex/parser_guido.rl"
	{ oct = oct_sign * ((*p) - '0'); }
	goto st2;
tr44:
#line 14 "lex/parser_guido.rl"
	{ pitch = music::PitchClass::B; }
#line 30 "lex/parser_guido.rl"
	{ oct = oct_sign * ((*p) - '0'); }
	goto st2;
tr54:
#line 15 "lex/parser_guido.rl"
	{ ++alt; }
#line 30 "lex/parser_guido.rl"
	{ oct = oct_sign * ((*p) - '0'); }
	goto st2;
tr63:
#line 16 "lex/parser_guido.rl"
	{ ++alt; }
#line 30 "lex/parser_guido.rl"
	{ oct = oct_sign * ((*p) - '0'); }
	goto st2;
tr69:
#line 21 "lex/parser_guido.rl"
	{ pitch = music::PitchClass::C; }
#line 30 "lex/parser_guido.rl"
	{ oct = oct_sign * ((*p) - '0'); }
	goto st2;
tr75:
#line 17 "lex/parser_guido.rl"
	{ pitch = music::PitchClass::E; }
#line 30 "lex/parser_guido.rl"
	{ oct = oct_sign * ((*p) - '0'); }
	goto st2;
tr83:
#line 24 "lex/parser_guido.rl"
	{ pitch = music::PitchClass::F; }
#line 30 "lex/parser_guido.rl"
	{ oct = oct_sign * ((*p) - '0'); }
	goto st2;
tr90:
#line 18 "lex/parser_guido.rl"
	{ ++alt; }
#line 30 "lex/parser_guido.rl"
	{ oct = oct_sign * ((*p) - '0'); }
	goto st2;
tr96:
#line 19 "lex/parser_guido.rl"
	{ pitch = music::PitchClass::G; }
#line 30 "lex/parser_guido.rl"
	{ oct = oct_sign * ((*p) - '0'); }
	goto st2;
tr104:
#line 19 "lex/parser_guido.rl"
	{ ++alt; }
#line 30 "lex/parser_guido.rl"
	{ oct = oct_sign * ((*p) - '0'); }
	goto st2;
tr110:
#line 20 "lex/parser_guido.rl"
	{ pitch = music::PitchClass::B; }
#line 30 "lex/parser_guido.rl"
	{ oct = oct_sign * ((*p) - '0'); }
	goto st2;
tr117:
#line 26 "lex/parser_guido.rl"
	{ pitch = music::PitchClass::A; }
#line 30 "lex/parser_guido.rl"
	{ oct = oct_sign * ((*p) - '0'); }
	goto st2;
tr124:
#line 23 "lex/parser_guido.rl"
	{ pitch = music::PitchClass::E; }
#line 30 "lex/parser_guido.rl"
	{ oct = oct_sign * ((*p) - '0'); }
	goto st2;
tr131:
#line 22 "lex/parser_guido.rl"
	{ pitch = music::PitchClass::D; }
#line 30 "lex/parser_guido.rl"
	{ oct = oct_sign * ((*p) - '0'); }
	goto st2;
tr139:
#line 27 "lex/parser_guido.rl"
	{ pitch = music::PitchClass::B; }
#line 30 "lex/parser_guido.rl"
	{ oct = oct_sign * ((*p) - '0'); }
	goto st2;
tr146:
#line 25 "lex/parser_guido.rl"
	{ pitch = music::PitchClass::G; }
#line 30 "lex/parser_guido.rl"
	{ oct = oct_sign * ((*p) - '0'); }
	goto st2;
tr153:
#line 28 "lex/parser_guido.rl"
	{ pitch = music::PitchClass::B; }
#line 30 "lex/parser_guido.rl"
	{ oct = oct_sign * ((*p) - '0'); }
	goto st2;
st2:
	p += 1;
case 2:
#line 186 "lex/parser_guido.cpp"
	switch( (*p) ) {
		case 0: goto tr15;
		case 47: goto st3;
	}
	goto st0;
tr15:
#line 37 "lex/parser_guido.rl"
	{ {p++; cs = 44; goto _out;} }
	goto st44;
tr21:
#line 13 "lex/parser_guido.rl"
	{ pitch = music::PitchClass::A; }
#line 37 "lex/parser_guido.rl"
	{ {p++; cs = 44; goto _out;} }
	goto st44;
tr33:
#line 13 "lex/parser_guido.rl"
	{ ++alt; }
#line 37 "lex/parser_guido.rl"
	{ {p++; cs = 44; goto _out;} }
	goto st44;
tr39:
#line 14 "lex/parser_guido.rl"
	{ pitch = music::PitchClass::B; }
#line 37 "lex/parser_guido.rl"
	{ {p++; cs = 44; goto _out;} }
	goto st44;
tr49:
#line 15 "lex/parser_guido.rl"
	{ ++alt; }
#line 37 "lex/parser_guido.rl"
	{ {p++; cs = 44; goto _out;} }
	goto st44;
tr58:
#line 16 "lex/parser_guido.rl"
	{ ++alt; }
#line 37 "lex/parser_guido.rl"
	{ {p++; cs = 44; goto _out;} }
	goto st44;
tr64:
#line 21 "lex/parser_guido.rl"
	{ pitch = music::PitchClass::C; }
#line 37 "lex/parser_guido.rl"
	{ {p++; cs = 44; goto _out;} }
	goto st44;
tr70:
#line 17 "lex/parser_guido.rl"
	{ pitch = music::PitchClass::E; }
#line 37 "lex/parser_guido.rl"
	{ {p++; cs = 44; goto _out;} }
	goto st44;
tr78:
#line 24 "lex/parser_guido.rl"
	{ pitch = music::PitchClass::F; }
#line 37 "lex/parser_guido.rl"
	{ {p++; cs = 44; goto _out;} }
	goto st44;
tr85:
#line 18 "lex/parser_guido.rl"
	{ ++alt; }
#line 37 "lex/parser_guido.rl"
	{ {p++; cs = 44; goto _out;} }
	goto st44;
tr91:
#line 19 "lex/parser_guido.rl"
	{ pitch = music::PitchClass::G; }
#line 37 "lex/parser_guido.rl"
	{ {p++; cs = 44; goto _out;} }
	goto st44;
tr99:
#line 19 "lex/parser_guido.rl"
	{ ++alt; }
#line 37 "lex/parser_guido.rl"
	{ {p++; cs = 44; goto _out;} }
	goto st44;
tr105:
#line 20 "lex/parser_guido.rl"
	{ pitch = music::PitchClass::B; }
#line 37 "lex/parser_guido.rl"
	{ {p++; cs = 44; goto _out;} }
	goto st44;
tr112:
#line 26 "lex/parser_guido.rl"
	{ pitch = music::PitchClass::A; }
#line 37 "lex/parser_guido.rl"
	{ {p++; cs = 44; goto _out;} }
	goto st44;
tr119:
#line 23 "lex/parser_guido.rl"
	{ pitch = music::PitchClass::E; }
#line 37 "lex/parser_guido.rl"
	{ {p++; cs = 44; goto _out;} }
	goto st44;
tr126:
#line 22 "lex/parser_guido.rl"
	{ pitch = music::PitchClass::D; }
#line 37 "lex/parser_guido.rl"
	{ {p++; cs = 44; goto _out;} }
	goto st44;
tr134:
#line 27 "lex/parser_guido.rl"
	{ pitch = music::PitchClass::B; }
#line 37 "lex/parser_guido.rl"
	{ {p++; cs = 44; goto _out;} }
	goto st44;
tr141:
#line 25 "lex/parser_guido.rl"
	{ pitch = music::PitchClass::G; }
#line 37 "lex/parser_guido.rl"
	{ {p++; cs = 44; goto _out;} }
	goto st44;
tr148:
#line 28 "lex/parser_guido.rl"
	{ pitch = music::PitchClass::B; }
#line 37 "lex/parser_guido.rl"
	{ {p++; cs = 44; goto _out;} }
	goto st44;
st44:
	p += 1;
case 44:
#line 307 "lex/parser_guido.cpp"
	goto st0;
tr25:
#line 13 "lex/parser_guido.rl"
	{ pitch = music::PitchClass::A; }
	goto st3;
tr37:
#line 13 "lex/parser_guido.rl"
	{ ++alt; }
	goto st3;
tr43:
#line 14 "lex/parser_guido.rl"
	{ pitch = music::PitchClass::B; }
	goto st3;
tr53:
#line 15 "lex/parser_guido.rl"
	{ ++alt; }
	goto st3;
tr62:
#line 16 "lex/parser_guido.rl"
	{ ++alt; }
	goto st3;
tr68:
#line 21 "lex/parser_guido.rl"
	{ pitch = music::PitchClass::C; }
	goto st3;
tr74:
#line 17 "lex/parser_guido.rl"
	{ pitch = music::PitchClass::E; }
	goto st3;
tr82:
#line 24 "lex/parser_guido.rl"
	{ pitch = music::PitchClass::F; }
	goto st3;
tr89:
#line 18 "lex/parser_guido.rl"
	{ ++alt; }
	goto st3;
tr95:
#line 19 "lex/parser_guido.rl"
	{ pitch = music::PitchClass::G; }
	goto st3;
tr103:
#line 19 "lex/parser_guido.rl"
	{ ++alt; }
	goto st3;
tr109:
#line 20 "lex/parser_guido.rl"
	{ pitch = music::PitchClass::B; }
	goto st3;
tr116:
#line 26 "lex/parser_guido.rl"
	{ pitch = music::PitchClass::A; }
	goto st3;
tr123:
#line 23 "lex/parser_guido.rl"
	{ pitch = music::PitchClass::E; }
	goto st3;
tr130:
#line 22 "lex/parser_guido.rl"
	{ pitch = music::PitchClass::D; }
	goto st3;
tr138:
#line 27 "lex/parser_guido.rl"
	{ pitch = music::PitchClass::B; }
	goto st3;
tr145:
#line 25 "lex/parser_guido.rl"
	{ pitch = music::PitchClass::G; }
	goto st3;
tr152:
#line 28 "lex/parser_guido.rl"
	{ pitch = music::PitchClass::B; }
	goto st3;
st3:
	p += 1;
case 3:
#line 384 "lex/parser_guido.cpp"
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st4;
	goto st0;
st4:
	p += 1;
case 4:
	switch( (*p) ) {
		case 0: goto tr15;
		case 46: goto st5;
	}
	goto st0;
st5:
	p += 1;
case 5:
	switch( (*p) ) {
		case 0: goto tr15;
		case 46: goto st6;
	}
	goto st0;
st6:
	p += 1;
case 6:
	switch( (*p) ) {
		case 0: goto tr15;
		case 46: goto st7;
	}
	goto st0;
st7:
	p += 1;
case 7:
	if ( (*p) == 0 )
		goto tr15;
	goto st0;
st8:
	p += 1;
case 8:
	switch( (*p) ) {
		case 0: goto tr21;
		case 35: goto tr22;
		case 38: goto tr23;
		case 45: goto tr24;
		case 47: goto tr25;
		case 105: goto tr27;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr26;
	goto st0;
tr22:
#line 13 "lex/parser_guido.rl"
	{ pitch = music::PitchClass::A; }
#line 11 "lex/parser_guido.rl"
	{ ++alt; }
	goto st9;
tr45:
#line 11 "lex/parser_guido.rl"
	{ ++alt; }
	goto st9;
tr34:
#line 13 "lex/parser_guido.rl"
	{ ++alt; }
#line 11 "lex/parser_guido.rl"
	{ ++alt; }
	goto st9;
tr40:
#line 14 "lex/parser_guido.rl"
	{ pitch = music::PitchClass::B; }
#line 11 "lex/parser_guido.rl"
	{ ++alt; }
	goto st9;
tr50:
#line 15 "lex/parser_guido.rl"
	{ ++alt; }
#line 11 "lex/parser_guido.rl"
	{ ++alt; }
	goto st9;
tr59:
#line 16 "lex/parser_guido.rl"
	{ ++alt; }
#line 11 "lex/parser_guido.rl"
	{ ++alt; }
	goto st9;
tr65:
#line 21 "lex/parser_guido.rl"
	{ pitch = music::PitchClass::C; }
#line 11 "lex/parser_guido.rl"
	{ ++alt; }
	goto st9;
tr71:
#line 17 "lex/parser_guido.rl"
	{ pitch = music::PitchClass::E; }
#line 11 "lex/parser_guido.rl"
	{ ++alt; }
	goto st9;
tr79:
#line 24 "lex/parser_guido.rl"
	{ pitch = music::PitchClass::F; }
#line 11 "lex/parser_guido.rl"
	{ ++alt; }
	goto st9;
tr86:
#line 18 "lex/parser_guido.rl"
	{ ++alt; }
#line 11 "lex/parser_guido.rl"
	{ ++alt; }
	goto st9;
tr92:
#line 19 "lex/parser_guido.rl"
	{ pitch = music::PitchClass::G; }
#line 11 "lex/parser_guido.rl"
	{ ++alt; }
	goto st9;
tr100:
#line 19 "lex/parser_guido.rl"
	{ ++alt; }
#line 11 "lex/parser_guido.rl"
	{ ++alt; }
	goto st9;
tr106:
#line 20 "lex/parser_guido.rl"
	{ pitch = music::PitchClass::B; }
#line 11 "lex/parser_guido.rl"
	{ ++alt; }
	goto st9;
tr113:
#line 26 "lex/parser_guido.rl"
	{ pitch = music::PitchClass::A; }
#line 11 "lex/parser_guido.rl"
	{ ++alt; }
	goto st9;
tr120:
#line 23 "lex/parser_guido.rl"
	{ pitch = music::PitchClass::E; }
#line 11 "lex/parser_guido.rl"
	{ ++alt; }
	goto st9;
tr127:
#line 22 "lex/parser_guido.rl"
	{ pitch = music::PitchClass::D; }
#line 11 "lex/parser_guido.rl"
	{ ++alt; }
	goto st9;
tr135:
#line 27 "lex/parser_guido.rl"
	{ pitch = music::PitchClass::B; }
#line 11 "lex/parser_guido.rl"
	{ ++alt; }
	goto st9;
tr142:
#line 25 "lex/parser_guido.rl"
	{ pitch = music::PitchClass::G; }
#line 11 "lex/parser_guido.rl"
	{ ++alt; }
	goto st9;
tr149:
#line 28 "lex/parser_guido.rl"
	{ pitch = music::PitchClass::B; }
#line 11 "lex/parser_guido.rl"
	{ ++alt; }
	goto st9;
st9:
	p += 1;
case 9:
#line 547 "lex/parser_guido.cpp"
	switch( (*p) ) {
		case 0: goto tr15;
		case 35: goto tr28;
		case 45: goto tr29;
		case 47: goto st3;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr30;
	goto st0;
tr28:
#line 11 "lex/parser_guido.rl"
	{ ++alt; }
	goto st10;
tr31:
#line 12 "lex/parser_guido.rl"
	{ --alt; }
	goto st10;
st10:
	p += 1;
case 10:
#line 568 "lex/parser_guido.cpp"
	switch( (*p) ) {
		case 0: goto tr15;
		case 45: goto tr29;
		case 47: goto st3;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr30;
	goto st0;
tr24:
#line 13 "lex/parser_guido.rl"
	{ pitch = music::PitchClass::A; }
#line 30 "lex/parser_guido.rl"
	{ oct_sign = -1; }
	goto st11;
tr29:
#line 30 "lex/parser_guido.rl"
	{ oct_sign = -1; }
	goto st11;
tr36:
#line 13 "lex/parser_guido.rl"
	{ ++alt; }
#line 30 "lex/parser_guido.rl"
	{ oct_sign = -1; }
	goto st11;
tr42:
#line 14 "lex/parser_guido.rl"
	{ pitch = music::PitchClass::B; }
#line 30 "lex/parser_guido.rl"
	{ oct_sign = -1; }
	goto st11;
tr52:
#line 15 "lex/parser_guido.rl"
	{ ++alt; }
#line 30 "lex/parser_guido.rl"
	{ oct_sign = -1; }
	goto st11;
tr61:
#line 16 "lex/parser_guido.rl"
	{ ++alt; }
#line 30 "lex/parser_guido.rl"
	{ oct_sign = -1; }
	goto st11;
tr67:
#line 21 "lex/parser_guido.rl"
	{ pitch = music::PitchClass::C; }
#line 30 "lex/parser_guido.rl"
	{ oct_sign = -1; }
	goto st11;
tr73:
#line 17 "lex/parser_guido.rl"
	{ pitch = music::PitchClass::E; }
#line 30 "lex/parser_guido.rl"
	{ oct_sign = -1; }
	goto st11;
tr81:
#line 24 "lex/parser_guido.rl"
	{ pitch = music::PitchClass::F; }
#line 30 "lex/parser_guido.rl"
	{ oct_sign = -1; }
	goto st11;
tr88:
#line 18 "lex/parser_guido.rl"
	{ ++alt; }
#line 30 "lex/parser_guido.rl"
	{ oct_sign = -1; }
	goto st11;
tr94:
#line 19 "lex/parser_guido.rl"
	{ pitch = music::PitchClass::G; }
#line 30 "lex/parser_guido.rl"
	{ oct_sign = -1; }
	goto st11;
tr102:
#line 19 "lex/parser_guido.rl"
	{ ++alt; }
#line 30 "lex/parser_guido.rl"
	{ oct_sign = -1; }
	goto st11;
tr108:
#line 20 "lex/parser_guido.rl"
	{ pitch = music::PitchClass::B; }
#line 30 "lex/parser_guido.rl"
	{ oct_sign = -1; }
	goto st11;
tr115:
#line 26 "lex/parser_guido.rl"
	{ pitch = music::PitchClass::A; }
#line 30 "lex/parser_guido.rl"
	{ oct_sign = -1; }
	goto st11;
tr122:
#line 23 "lex/parser_guido.rl"
	{ pitch = music::PitchClass::E; }
#line 30 "lex/parser_guido.rl"
	{ oct_sign = -1; }
	goto st11;
tr129:
#line 22 "lex/parser_guido.rl"
	{ pitch = music::PitchClass::D; }
#line 30 "lex/parser_guido.rl"
	{ oct_sign = -1; }
	goto st11;
tr137:
#line 27 "lex/parser_guido.rl"
	{ pitch = music::PitchClass::B; }
#line 30 "lex/parser_guido.rl"
	{ oct_sign = -1; }
	goto st11;
tr144:
#line 25 "lex/parser_guido.rl"
	{ pitch = music::PitchClass::G; }
#line 30 "lex/parser_guido.rl"
	{ oct_sign = -1; }
	goto st11;
tr151:
#line 28 "lex/parser_guido.rl"
	{ pitch = music::PitchClass::B; }
#line 30 "lex/parser_guido.rl"
	{ oct_sign = -1; }
	goto st11;
st11:
	p += 1;
case 11:
#line 692 "lex/parser_guido.cpp"
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr30;
	goto st0;
tr23:
#line 13 "lex/parser_guido.rl"
	{ pitch = music::PitchClass::A; }
#line 12 "lex/parser_guido.rl"
	{ --alt; }
	goto st12;
tr46:
#line 12 "lex/parser_guido.rl"
	{ --alt; }
	goto st12;
tr35:
#line 13 "lex/parser_guido.rl"
	{ ++alt; }
#line 12 "lex/parser_guido.rl"
	{ --alt; }
	goto st12;
tr41:
#line 14 "lex/parser_guido.rl"
	{ pitch = music::PitchClass::B; }
#line 12 "lex/parser_guido.rl"
	{ --alt; }
	goto st12;
tr51:
#line 15 "lex/parser_guido.rl"
	{ ++alt; }
#line 12 "lex/parser_guido.rl"
	{ --alt; }
	goto st12;
tr60:
#line 16 "lex/parser_guido.rl"
	{ ++alt; }
#line 12 "lex/parser_guido.rl"
	{ --alt; }
	goto st12;
tr66:
#line 21 "lex/parser_guido.rl"
	{ pitch = music::PitchClass::C; }
#line 12 "lex/parser_guido.rl"
	{ --alt; }
	goto st12;
tr72:
#line 17 "lex/parser_guido.rl"
	{ pitch = music::PitchClass::E; }
#line 12 "lex/parser_guido.rl"
	{ --alt; }
	goto st12;
tr80:
#line 24 "lex/parser_guido.rl"
	{ pitch = music::PitchClass::F; }
#line 12 "lex/parser_guido.rl"
	{ --alt; }
	goto st12;
tr87:
#line 18 "lex/parser_guido.rl"
	{ ++alt; }
#line 12 "lex/parser_guido.rl"
	{ --alt; }
	goto st12;
tr93:
#line 19 "lex/parser_guido.rl"
	{ pitch = music::PitchClass::G; }
#line 12 "lex/parser_guido.rl"
	{ --alt; }
	goto st12;
tr101:
#line 19 "lex/parser_guido.rl"
	{ ++alt; }
#line 12 "lex/parser_guido.rl"
	{ --alt; }
	goto st12;
tr107:
#line 20 "lex/parser_guido.rl"
	{ pitch = music::PitchClass::B; }
#line 12 "lex/parser_guido.rl"
	{ --alt; }
	goto st12;
tr114:
#line 26 "lex/parser_guido.rl"
	{ pitch = music::PitchClass::A; }
#line 12 "lex/parser_guido.rl"
	{ --alt; }
	goto st12;
tr121:
#line 23 "lex/parser_guido.rl"
	{ pitch = music::PitchClass::E; }
#line 12 "lex/parser_guido.rl"
	{ --alt; }
	goto st12;
tr128:
#line 22 "lex/parser_guido.rl"
	{ pitch = music::PitchClass::D; }
#line 12 "lex/parser_guido.rl"
	{ --alt; }
	goto st12;
tr136:
#line 27 "lex/parser_guido.rl"
	{ pitch = music::PitchClass::B; }
#line 12 "lex/parser_guido.rl"
	{ --alt; }
	goto st12;
tr143:
#line 25 "lex/parser_guido.rl"
	{ pitch = music::PitchClass::G; }
#line 12 "lex/parser_guido.rl"
	{ --alt; }
	goto st12;
tr150:
#line 28 "lex/parser_guido.rl"
	{ pitch = music::PitchClass::B; }
#line 12 "lex/parser_guido.rl"
	{ --alt; }
	goto st12;
st12:
	p += 1;
case 12:
#line 811 "lex/parser_guido.cpp"
	switch( (*p) ) {
		case 0: goto tr15;
		case 38: goto tr31;
		case 45: goto tr29;
		case 47: goto st3;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr30;
	goto st0;
tr27:
#line 13 "lex/parser_guido.rl"
	{ pitch = music::PitchClass::A; }
	goto st13;
st13:
	p += 1;
case 13:
#line 828 "lex/parser_guido.cpp"
	if ( (*p) == 115 )
		goto st14;
	goto st0;
st14:
	p += 1;
case 14:
	switch( (*p) ) {
		case 0: goto tr33;
		case 35: goto tr34;
		case 38: goto tr35;
		case 45: goto tr36;
		case 47: goto tr37;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr38;
	goto st0;
st15:
	p += 1;
case 15:
	switch( (*p) ) {
		case 0: goto tr39;
		case 35: goto tr40;
		case 38: goto tr41;
		case 45: goto tr42;
		case 47: goto tr43;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr44;
	goto st0;
tr4:
#line 15 "lex/parser_guido.rl"
	{ pitch = music::PitchClass::C; }
	goto st16;
st16:
	p += 1;
case 16:
#line 865 "lex/parser_guido.cpp"
	switch( (*p) ) {
		case 0: goto tr15;
		case 35: goto tr45;
		case 38: goto tr46;
		case 45: goto tr29;
		case 47: goto st3;
		case 105: goto st17;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr30;
	goto st0;
st17:
	p += 1;
case 17:
	if ( (*p) == 115 )
		goto st18;
	goto st0;
st18:
	p += 1;
case 18:
	switch( (*p) ) {
		case 0: goto tr49;
		case 35: goto tr50;
		case 38: goto tr51;
		case 45: goto tr52;
		case 47: goto tr53;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr54;
	goto st0;
tr5:
#line 16 "lex/parser_guido.rl"
	{ pitch = music::PitchClass::D; }
	goto st19;
st19:
	p += 1;
case 19:
#line 903 "lex/parser_guido.cpp"
	switch( (*p) ) {
		case 0: goto tr15;
		case 35: goto tr45;
		case 38: goto tr46;
		case 45: goto tr29;
		case 47: goto st3;
		case 105: goto st20;
		case 111: goto st22;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr30;
	goto st0;
st20:
	p += 1;
case 20:
	if ( (*p) == 115 )
		goto st21;
	goto st0;
st21:
	p += 1;
case 21:
	switch( (*p) ) {
		case 0: goto tr58;
		case 35: goto tr59;
		case 38: goto tr60;
		case 45: goto tr61;
		case 47: goto tr62;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr63;
	goto st0;
st22:
	p += 1;
case 22:
	switch( (*p) ) {
		case 0: goto tr64;
		case 35: goto tr65;
		case 38: goto tr66;
		case 45: goto tr67;
		case 47: goto tr68;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr69;
	goto st0;
st23:
	p += 1;
case 23:
	switch( (*p) ) {
		case 0: goto tr70;
		case 35: goto tr71;
		case 38: goto tr72;
		case 45: goto tr73;
		case 47: goto tr74;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr75;
	goto st0;
tr7:
#line 18 "lex/parser_guido.rl"
	{ pitch = music::PitchClass::F; }
	goto st24;
st24:
	p += 1;
case 24:
#line 968 "lex/parser_guido.cpp"
	switch( (*p) ) {
		case 0: goto tr15;
		case 35: goto tr45;
		case 38: goto tr46;
		case 45: goto tr29;
		case 47: goto st3;
		case 97: goto st25;
		case 105: goto st26;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr30;
	goto st0;
st25:
	p += 1;
case 25:
	switch( (*p) ) {
		case 0: goto tr78;
		case 35: goto tr79;
		case 38: goto tr80;
		case 45: goto tr81;
		case 47: goto tr82;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr83;
	goto st0;
st26:
	p += 1;
case 26:
	if ( (*p) == 115 )
		goto st27;
	goto st0;
st27:
	p += 1;
case 27:
	switch( (*p) ) {
		case 0: goto tr85;
		case 35: goto tr86;
		case 38: goto tr87;
		case 45: goto tr88;
		case 47: goto tr89;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr90;
	goto st0;
st28:
	p += 1;
case 28:
	switch( (*p) ) {
		case 0: goto tr91;
		case 35: goto tr92;
		case 38: goto tr93;
		case 45: goto tr94;
		case 47: goto tr95;
		case 105: goto tr97;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr96;
	goto st0;
tr97:
#line 19 "lex/parser_guido.rl"
	{ pitch = music::PitchClass::G; }
	goto st29;
st29:
	p += 1;
case 29:
#line 1034 "lex/parser_guido.cpp"
	if ( (*p) == 115 )
		goto st30;
	goto st0;
st30:
	p += 1;
case 30:
	switch( (*p) ) {
		case 0: goto tr99;
		case 35: goto tr100;
		case 38: goto tr101;
		case 45: goto tr102;
		case 47: goto tr103;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr104;
	goto st0;
st31:
	p += 1;
case 31:
	switch( (*p) ) {
		case 0: goto tr105;
		case 35: goto tr106;
		case 38: goto tr107;
		case 45: goto tr108;
		case 47: goto tr109;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr110;
	goto st0;
st32:
	p += 1;
case 32:
	if ( (*p) == 97 )
		goto st33;
	goto st0;
st33:
	p += 1;
case 33:
	switch( (*p) ) {
		case 0: goto tr112;
		case 35: goto tr113;
		case 38: goto tr114;
		case 45: goto tr115;
		case 47: goto tr116;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr117;
	goto st0;
st34:
	p += 1;
case 34:
	if ( (*p) == 105 )
		goto st35;
	goto st0;
st35:
	p += 1;
case 35:
	switch( (*p) ) {
		case 0: goto tr119;
		case 35: goto tr120;
		case 38: goto tr121;
		case 45: goto tr122;
		case 47: goto tr123;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr124;
	goto st0;
st36:
	p += 1;
case 36:
	if ( (*p) == 101 )
		goto st37;
	goto st0;
st37:
	p += 1;
case 37:
	switch( (*p) ) {
		case 0: goto tr126;
		case 35: goto tr127;
		case 38: goto tr128;
		case 45: goto tr129;
		case 47: goto tr130;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr131;
	goto st0;
st38:
	p += 1;
case 38:
	switch( (*p) ) {
		case 105: goto st39;
		case 111: goto st40;
	}
	goto st0;
st39:
	p += 1;
case 39:
	switch( (*p) ) {
		case 0: goto tr134;
		case 35: goto tr135;
		case 38: goto tr136;
		case 45: goto tr137;
		case 47: goto tr138;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr139;
	goto st0;
st40:
	p += 1;
case 40:
	if ( (*p) == 108 )
		goto st41;
	goto st0;
st41:
	p += 1;
case 41:
	switch( (*p) ) {
		case 0: goto tr141;
		case 35: goto tr142;
		case 38: goto tr143;
		case 45: goto tr144;
		case 47: goto tr145;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr146;
	goto st0;
st42:
	p += 1;
case 42:
	if ( (*p) == 105 )
		goto st43;
	goto st0;
st43:
	p += 1;
case 43:
	switch( (*p) ) {
		case 0: goto tr148;
		case 35: goto tr149;
		case 38: goto tr150;
		case 45: goto tr151;
		case 47: goto tr152;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr153;
	goto st0;
	}

	_out: {}
	}

#line 58 "lex/parser_guido.rl"

    const auto ok = cs >= 44;

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
