
#line 1 "lex/parser_midi.rl"
# include "parser_midi.h"

# include <cstdint>
# include <cstring>
# include <limits>

namespace ceammc {
namespace parser {


#line 14 "lex/parser_midi.cpp"
static const int midi_start = 1;
static const int midi_first_final = 8;
static const int midi_error = 0;

static const int midi_en_main = 1;


#line 16 "lex/parser_midi.rl"


static char xchar2digit(char ch)
{
    if (ch >= '0' && ch <= '9') return ch - '0';
    if (ch >= 'A' && ch <= 'F') return ch - 'A' + 10;
    if (ch >= 'a' && ch <= 'f') return ch - 'a' + 10;
    return 0;
}

bool parse_midi_byte_hex(const Atom& a, std::uint8_t& res)
{
    if (a.isFloat()) {
        int x = (int)a.asT<t_float>();
        res = (x / 10)*16 + (x % 10);
        return true;
    } else if (!a.isSymbol())
        return false;

    std::uint8_t midi_byte = 0;
    int cs = 0;
    const char* p = a.asT<t_symbol*>()->s_name;

    
#line 47 "lex/parser_midi.cpp"
	{
	cs = midi_start;
	}

#line 40 "lex/parser_midi.rl"
    
#line 54 "lex/parser_midi.cpp"
	{
	switch ( cs )
	{
case 1:
	if ( (*p) == 48 )
		goto tr0;
	if ( (*p) < 65 ) {
		if ( 49 <= (*p) && (*p) <= 57 )
			goto tr2;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto tr2;
	} else
		goto tr2;
	goto st0;
st0:
cs = 0;
	goto _out;
tr0:
#line 6 "lex/ragel_midi.rl"
	{ (midi_byte <<= 4) |= xchar2digit((*p)); }
	goto st2;
st2:
	p += 1;
case 2:
#line 80 "lex/parser_midi.cpp"
	switch( (*p) ) {
		case 72: goto st4;
		case 104: goto st4;
		case 120: goto st5;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr3;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto tr3;
	} else
		goto tr3;
	goto st0;
tr3:
#line 6 "lex/ragel_midi.rl"
	{ (midi_byte <<= 4) |= xchar2digit((*p)); }
	goto st3;
st3:
	p += 1;
case 3:
#line 102 "lex/parser_midi.cpp"
	switch( (*p) ) {
		case 72: goto st4;
		case 104: goto st4;
	}
	goto st0;
tr8:
#line 6 "lex/ragel_midi.rl"
	{ (midi_byte <<= 4) |= xchar2digit((*p)); }
	goto st4;
st4:
	p += 1;
case 4:
#line 115 "lex/parser_midi.cpp"
	if ( (*p) == 0 )
		goto tr6;
	goto st0;
tr6:
#line 14 "lex/parser_midi.rl"
	{ {p++; cs = 8; goto _out;} }
	goto st8;
st8:
	p += 1;
case 8:
#line 126 "lex/parser_midi.cpp"
	goto st0;
st5:
	p += 1;
case 5:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr7;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto tr7;
	} else
		goto tr7;
	goto st0;
tr7:
#line 6 "lex/ragel_midi.rl"
	{ (midi_byte <<= 4) |= xchar2digit((*p)); }
	goto st6;
st6:
	p += 1;
case 6:
#line 147 "lex/parser_midi.cpp"
	if ( (*p) == 0 )
		goto tr6;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr8;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto tr8;
	} else
		goto tr8;
	goto st0;
tr2:
#line 6 "lex/ragel_midi.rl"
	{ (midi_byte <<= 4) |= xchar2digit((*p)); }
	goto st7;
st7:
	p += 1;
case 7:
#line 166 "lex/parser_midi.cpp"
	switch( (*p) ) {
		case 72: goto st4;
		case 104: goto st4;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr3;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto tr3;
	} else
		goto tr3;
	goto st0;
	}

	_out: {}
	}

#line 41 "lex/parser_midi.rl"

    if(cs >= 8) {
        res = midi_byte;
        return true;
    } else
        return false;
}

}
}


