
#line 1 "parser_canvas_id.rl"
# include "parser_canvas_id.h"
# include "lex/ragel_common.h"

# include <cstring>

namespace ceammc {


#line 12 "parser_canvas_id.cpp"
static const int canvas_id_start = 1;
static const int canvas_id_first_final = 16;
static const int canvas_id_error = 0;

static const int canvas_id_en_main = 1;


#line 18 "parser_canvas_id.rl"


bool try_parse_canvas_id(const char* str, CanvasId& id) {
    int cs = 0;
    const char* p = str;
    CanvasId canvas_id = 0;

    
#line 29 "parser_canvas_id.cpp"
	{
	cs = canvas_id_start;
	}

#line 26 "parser_canvas_id.rl"
    
#line 36 "parser_canvas_id.cpp"
	{
	switch ( cs )
	{
case 1:
	if ( (*p) == 46 )
		goto st2;
	goto st0;
st0:
cs = 0;
	goto _out;
st2:
	p += 1;
case 2:
	if ( (*p) == 120 )
		goto st3;
	goto st0;
st3:
	p += 1;
case 3:
	if ( (*p) > 57 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto tr3;
	} else if ( (*p) >= 48 )
		goto tr3;
	goto st0;
tr3:
#line 12 "parser_canvas_id.rl"
	{ (canvas_id <<= 4) |= parser::xchar2digit((*p)); }
	goto st4;
st4:
	p += 1;
case 4:
#line 69 "parser_canvas_id.cpp"
	if ( (*p) == 46 )
		goto st5;
	if ( (*p) > 57 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto tr3;
	} else if ( (*p) >= 48 )
		goto tr3;
	goto st0;
st5:
	p += 1;
case 5:
	if ( (*p) == 99 )
		goto st6;
	goto st0;
st6:
	p += 1;
case 6:
	switch( (*p) ) {
		case 0: goto tr6;
		case 46: goto st7;
	}
	goto st0;
tr6:
#line 15 "parser_canvas_id.rl"
	{ {p++; cs = 16; goto _out;} }
	goto st16;
st16:
	p += 1;
case 16:
#line 99 "parser_canvas_id.cpp"
	goto st0;
st7:
	p += 1;
case 7:
	if ( (*p) == 101 )
		goto st8;
	goto st0;
st8:
	p += 1;
case 8:
	if ( (*p) == 99 )
		goto st9;
	goto st0;
st9:
	p += 1;
case 9:
	if ( (*p) == 97 )
		goto st10;
	goto st0;
st10:
	p += 1;
case 10:
	if ( (*p) == 110 )
		goto st11;
	goto st0;
st11:
	p += 1;
case 11:
	if ( (*p) == 118 )
		goto st12;
	goto st0;
st12:
	p += 1;
case 12:
	if ( (*p) == 97 )
		goto st13;
	goto st0;
st13:
	p += 1;
case 13:
	if ( (*p) == 115 )
		goto st14;
	goto st0;
st14:
	p += 1;
case 14:
	if ( (*p) > 57 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st15;
	} else if ( (*p) >= 48 )
		goto st15;
	goto st0;
st15:
	p += 1;
case 15:
	if ( (*p) == 0 )
		goto tr6;
	if ( (*p) > 57 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st15;
	} else if ( (*p) >= 48 )
		goto st15;
	goto st0;
	}

	_out: {}
	}

#line 27 "parser_canvas_id.rl"

    const auto ok = cs >= 16;
    if (ok)
        id = canvas_id;

    return ok;
}

}
