
#line 1 "parser_vlc.rl"
# include "parser_vlc.h"


#line 7 "parser_vlc.cpp"
static const int vlc_seek_parser_start = 1;
static const int vlc_seek_parser_first_final = 11;
static const int vlc_seek_parser_error = 0;

static const int vlc_seek_parser_en_main = 1;


#line 15 "parser_vlc.rl"


namespace ceammc {
namespace parser {

bool check_vlc_seek(const Atom& a)
{
    if (a.isInteger()) return true;
    if (a.isSymbol()) return check_vlc_seek(a.asT<t_symbol*>()->s_name);
    return false;
}

bool check_vlc_seek(const char* str)
{
    int cs = 0;
    auto* p = str;

    
#line 34 "parser_vlc.cpp"
	{
	cs = vlc_seek_parser_start;
	}

#line 33 "parser_vlc.rl"
    
#line 41 "parser_vlc.cpp"
	{
	switch ( cs )
	{
case 1:
	switch( (*p) ) {
		case 43: goto st2;
		case 45: goto st2;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st3;
	goto st0;
st0:
cs = 0;
	goto _out;
st2:
	p += 1;
case 2:
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st3;
	goto st0;
st3:
	p += 1;
case 3:
	switch( (*p) ) {
		case 0: goto tr3;
		case 34: goto st4;
		case 37: goto st4;
		case 39: goto st5;
		case 58: goto st6;
		case 72: goto st8;
		case 77: goto st5;
		case 83: goto st4;
		case 104: goto st8;
		case 109: goto st5;
		case 115: goto st4;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st3;
	goto st0;
tr3:
#line 12 "parser_vlc.rl"
	{ {p++; cs = 11; goto _out;} }
	goto st11;
st11:
	p += 1;
case 11:
#line 88 "parser_vlc.cpp"
	goto st0;
st4:
	p += 1;
case 4:
	if ( (*p) == 0 )
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
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st7;
	goto st0;
st7:
	p += 1;
case 7:
	switch( (*p) ) {
		case 34: goto st4;
		case 83: goto st4;
		case 115: goto st4;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st7;
	goto st0;
st8:
	p += 1;
case 8:
	if ( (*p) == 58 )
		goto st9;
	goto st0;
st9:
	p += 1;
case 9:
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st10;
	goto st0;
st10:
	p += 1;
case 10:
	switch( (*p) ) {
		case 34: goto st4;
		case 39: goto st5;
		case 58: goto st6;
		case 77: goto st5;
		case 83: goto st4;
		case 109: goto st5;
		case 115: goto st4;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st10;
	goto st0;
	}

	_out: {}
	}

#line 34 "parser_vlc.rl"

    return cs >= 11;
}

}
}


