
#line 1 "lex/parser_hexbeat.rl"
# include "parser_hexbeat.h"


#line 7 "lex/parser_hexbeat.cpp"
static const int hexbeat_start = 1;
static const int hexbeat_first_final = 2;
static const int hexbeat_error = 0;

static const int hexbeat_en_main = 1;


#line 14 "lex/parser_hexbeat.rl"


namespace ceammc {
namespace parser {

static char hextob(char ch)
{
    if (ch >= '0' && ch <= '9') return ch - '0';
    if (ch >= 'A' && ch <= 'F') return ch - 'A' + 10;
    if (ch >= 'a' && ch <= 'f') return ch - 'a' + 10;
    return 0;
}

static void push_bits(HexBeatResult& bits, char x)
{
    bits <<= 4;
    bits |= (0x0F & x);
}

std::uint16_t parse_hexbeat(const char* str, HexBeatResult& bits)
{
    int cs = 0;
    const char* p = str;
    if (p[0] == '0' && p[1] == 'x')
        p += 2;

    int nbits = 0;


#line 45 "lex/parser_hexbeat.cpp"
	{
	cs = hexbeat_start;
	}

#line 50 "lex/parser_hexbeat.cpp"
	{
	switch ( cs )
	{
tr2:
#line 7 "lex/parser_hexbeat.rl"
	{ push_bits(bits, hextob((*p))); nbits += 4; }
	goto st1;
st1:
	p += 1;
case 1:
#line 61 "lex/parser_hexbeat.cpp"
	if ( (*p) == 0 )
		goto tr1;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr2;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto tr2;
	} else
		goto tr2;
	goto st1;
tr1:
#line 12 "lex/parser_hexbeat.rl"
	{ {p++; cs = 2; goto _out;} }
	goto st2;
st2:
	p += 1;
case 2:
#line 80 "lex/parser_hexbeat.cpp"
	goto st0;
st0:
cs = 0;
	goto _out;
	}

	_out: {}
	}

#line 45 "lex/parser_hexbeat.rl"


    const auto ok = cs >= 2;

    return ok ? nbits : 0;
}

}
}
