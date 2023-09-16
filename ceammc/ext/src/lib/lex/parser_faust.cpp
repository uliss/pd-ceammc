
#line 1 "lex/parser_faust.rl"
# include "ragel_common.h"
# include "parser_faust.h"

# include <cstdint>
# include <string>

namespace ceammc {
namespace parser {


#line 14 "lex/parser_faust.cpp"
static const int faust_style_menu_start = 1;
static const int faust_style_menu_first_final = 11;
static const int faust_style_menu_error = 0;

static const int faust_style_menu_en_main = 1;


#line 25 "lex/parser_faust.rl"


bool parse_faust_style_menu(const char* str, FaustStyleMenuCb fn)
{
    int cs = 0;
    const char* p = str;

    std::string key;
    int idx = 0;

    
#line 34 "lex/parser_faust.cpp"
	{
	cs = faust_style_menu_start;
	}

#line 36 "lex/parser_faust.rl"
    
#line 41 "lex/parser_faust.cpp"
	{
	switch ( cs )
	{
case 1:
	if ( (*p) == 123 )
		goto st2;
	goto st0;
st0:
cs = 0;
	goto _out;
st2:
	p += 1;
case 2:
	switch( (*p) ) {
		case 32: goto st2;
		case 39: goto tr2;
	}
	if ( 9 <= (*p) && (*p) <= 13 )
		goto st2;
	goto st0;
tr2:
#line 15 "lex/parser_faust.rl"
	{ key.clear(); idx = 0; }
#line 13 "lex/parser_faust.rl"
	{ key.clear(); }
	goto st3;
st3:
	p += 1;
case 3:
#line 71 "lex/parser_faust.cpp"
	if ( (*p) == 95 )
		goto tr3;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr3;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr3;
	} else
		goto tr3;
	goto st0;
tr3:
#line 14 "lex/parser_faust.rl"
	{ key += (*p); }
	goto st4;
st4:
	p += 1;
case 4:
#line 90 "lex/parser_faust.cpp"
	switch( (*p) ) {
		case 39: goto st5;
		case 95: goto tr3;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr3;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr3;
	} else
		goto tr3;
	goto st0;
st5:
	p += 1;
case 5:
	if ( (*p) == 58 )
		goto st6;
	goto st0;
st6:
	p += 1;
case 6:
	if ( (*p) == 32 )
		goto st6;
	if ( (*p) > 13 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr6;
	} else if ( (*p) >= 9 )
		goto st6;
	goto st0;
tr6:
#line 16 "lex/parser_faust.rl"
	{ idx = (*p) - '0'; }
	goto st7;
st7:
	p += 1;
case 7:
#line 128 "lex/parser_faust.cpp"
	switch( (*p) ) {
		case 32: goto tr7;
		case 44: goto tr8;
		case 125: goto tr9;
	}
	if ( 9 <= (*p) && (*p) <= 13 )
		goto tr7;
	goto st0;
tr7:
#line 17 "lex/parser_faust.rl"
	{ fn(key, idx); }
	goto st8;
st8:
	p += 1;
case 8:
#line 144 "lex/parser_faust.cpp"
	switch( (*p) ) {
		case 32: goto st8;
		case 125: goto st9;
	}
	if ( 9 <= (*p) && (*p) <= 13 )
		goto st8;
	goto st0;
tr9:
#line 17 "lex/parser_faust.rl"
	{ fn(key, idx); }
	goto st9;
st9:
	p += 1;
case 9:
#line 159 "lex/parser_faust.cpp"
	if ( (*p) == 0 )
		goto tr12;
	goto st0;
tr12:
#line 23 "lex/parser_faust.rl"
	{ {p++; cs = 11; goto _out;} }
	goto st11;
st11:
	p += 1;
case 11:
#line 170 "lex/parser_faust.cpp"
	goto st0;
tr8:
#line 17 "lex/parser_faust.rl"
	{ fn(key, idx); }
	goto st10;
st10:
	p += 1;
case 10:
#line 179 "lex/parser_faust.cpp"
	if ( (*p) == 39 )
		goto tr2;
	goto st0;
	}

	_out: {}
	}

#line 37 "lex/parser_faust.rl"

    const auto ok = cs >= 11;
    return ok;
}

}
}


