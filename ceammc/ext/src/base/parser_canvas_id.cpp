
#line 1 "parser_canvas_id.rl"
# include "parser_canvas_id.h"
# include "lex/ragel_common.h"

# include <cstring>

namespace ceammc {


#line 12 "parser_canvas_id.cpp"
static const int canvas_id_start = 1;
static const int canvas_id_first_final = 7;
static const int canvas_id_error = 0;

static const int canvas_id_en_main = 1;


#line 17 "parser_canvas_id.rl"


bool try_parse_canvas_id(const char* str, CanvasId& id) {
    int cs = 0;
    const char* p = str;
    CanvasId canvas_id = 0;

    
#line 29 "parser_canvas_id.cpp"
	{
	cs = canvas_id_start;
	}

#line 25 "parser_canvas_id.rl"
    
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
	if ( (*p) == 0 )
		goto tr6;
	goto st0;
tr6:
#line 14 "parser_canvas_id.rl"
	{ {p++; cs = 7; goto _out;} }
	goto st7;
st7:
	p += 1;
case 7:
#line 97 "parser_canvas_id.cpp"
	goto st0;
	}

	_out: {}
	}

#line 26 "parser_canvas_id.rl"

    const auto ok = cs >= 7;
    if (ok)
        id = canvas_id;

    return ok;
}

}
