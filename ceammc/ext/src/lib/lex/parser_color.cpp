
#line 1 "lex/parser_color.rl"
# include "parser_color.h"

# include <cstdint>
# include <cstring>

namespace ceammc {
namespace parser {

static uint8_t xchar2digit(char c)
{
    switch (c) {
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
        return c - '0';
    case 'A':
    case 'B':
    case 'C':
    case 'D':
    case 'E':
    case 'F':
        return c - 'A' + 10;
    default:
        return c - 'a' + 10;
    }
}


#line 39 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_color.cpp"
static const int color_start = 1;
static const int color_first_final = 8;
static const int color_error = 0;

static const int color_en_main = 1;


#line 41 "lex/parser_color.rl"


RgbHexFullMatch::RgbHexFullMatch()
{
    reset();
}

void RgbHexFullMatch::reset()
{
    color_ = {};
}

bool RgbHexFullMatch::parse(const Atom& a)
{
    if (a.isSymbol())
        return parse(a.asT<t_symbol*>()->s_name);
    else
        return false;
}

bool RgbHexFullMatch::parse(const char* str)
{
    const auto len = strlen(str);
    if (len == 0)
        return false;

    const char* p = str;
    const char* pe = p + len;
    const char* eof = pe;
    ColorRagelData color;
    AtomCategory cat_ {CAT_UNKNOWN};
    AtomType type_ = {TYPE_UNKNOWN};

    reset();

    
#line 84 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_color.cpp"
	{
	cs = color_start;
	}

#line 77 "lex/parser_color.rl"
    
#line 91 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_color.cpp"
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
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr2;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto tr2;
	} else
		goto tr2;
	goto st0;
tr2:
#line 5 "lex/ragel_color.rl"
	{ color.x = xchar2digit((*p)); }
#line 6 "lex/ragel_color.rl"
	{ (color.x *= 16) += xchar2digit((*p)); }
	goto st3;
st3:
	if ( ++p == pe )
		goto _test_eof3;
case 3:
#line 127 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_color.cpp"
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
#line 6 "lex/ragel_color.rl"
	{ (color.x *= 16) += xchar2digit((*p)); }
	goto st4;
st4:
	if ( ++p == pe )
		goto _test_eof4;
case 4:
#line 145 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_color.cpp"
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr4;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto tr4;
	} else
		goto tr4;
	goto st0;
tr4:
#line 11 "lex/ragel_color.rl"
	{color.r = color.x;}
#line 5 "lex/ragel_color.rl"
	{ color.x = xchar2digit((*p)); }
#line 6 "lex/ragel_color.rl"
	{ (color.x *= 16) += xchar2digit((*p)); }
	goto st5;
st5:
	if ( ++p == pe )
		goto _test_eof5;
case 5:
#line 167 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_color.cpp"
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr5;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto tr5;
	} else
		goto tr5;
	goto st0;
tr5:
#line 6 "lex/ragel_color.rl"
	{ (color.x *= 16) += xchar2digit((*p)); }
	goto st6;
st6:
	if ( ++p == pe )
		goto _test_eof6;
case 6:
#line 185 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_color.cpp"
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr6;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto tr6;
	} else
		goto tr6;
	goto st0;
tr6:
#line 12 "lex/ragel_color.rl"
	{color.g = color.x;}
#line 5 "lex/ragel_color.rl"
	{ color.x = xchar2digit((*p)); }
#line 6 "lex/ragel_color.rl"
	{ (color.x *= 16) += xchar2digit((*p)); }
	goto st7;
st7:
	if ( ++p == pe )
		goto _test_eof7;
case 7:
#line 207 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_color.cpp"
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
#line 6 "lex/ragel_color.rl"
	{ (color.x *= 16) += xchar2digit((*p)); }
	goto st8;
st8:
	if ( ++p == pe )
		goto _test_eof8;
case 8:
#line 225 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_color.cpp"
	goto st0;
	}
	_test_eof2: cs = 2; goto _test_eof; 
	_test_eof3: cs = 3; goto _test_eof; 
	_test_eof4: cs = 4; goto _test_eof; 
	_test_eof5: cs = 5; goto _test_eof; 
	_test_eof6: cs = 6; goto _test_eof; 
	_test_eof7: cs = 7; goto _test_eof; 
	_test_eof8: cs = 8; goto _test_eof; 

	_test_eof: {}
	if ( p == eof )
	{
	switch ( cs ) {
	case 8: 
#line 13 "lex/ragel_color.rl"
	{color.b = color.x;}
#line 14 "lex/ragel_color.rl"
	{ cat_ = CAT_COLOR; type_ = TYPE_HEX; }
	break;
#line 246 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_color.cpp"
	}
	}

	_out: {}
	}

#line 78 "lex/parser_color.rl"

    const auto ok = cs >= 8;
    if(ok) {
        color_.r = color.r;
        color_.g = color.g;
        color_.b = color.b;
    }

    return ok;
}

size_t RgbHexFullMatch::parse(const AtomListView& lv, SmallColorVec& out)
{
    const size_t N = lv.size();

    for (size_t i = 0; i < N; i++) {
        const auto& a = lv[i];
        if (!parse(a))
            return i;

        out.push_back(asInt());
    }

    return N;
}

}
}


