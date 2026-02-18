
#line 1 "lex/parser_path_template.rl"
# include "lex/parser_path_template.h"
# include "lex/ragel_common.h"

# include <cstring>
# include <limits>

namespace ceammc {
namespace parser {


#line 14 "lex/parser_path_template.cpp"
static const int parser_path_template_start = 1;
static const int parser_path_template_first_final = 55;
static const int parser_path_template_error = 0;

static const int parser_path_template_en_main = 1;


#line 29 "lex/parser_path_template.rl"


bool path_contains_template(const char* str) {
    int cs = 0;
    const char* p = str;

    
#line 30 "lex/parser_path_template.cpp"
	{
	cs = parser_path_template_start;
	}

#line 36 "lex/parser_path_template.rl"
    
#line 37 "lex/parser_path_template.cpp"
	{
	switch ( cs )
	{
case 1:
	switch( (*p) ) {
		case 37: goto st2;
		case 102: goto st48;
		case 126: goto st8;
	}
	goto st0;
st0:
cs = 0;
	goto _out;
st2:
	p += 1;
case 2:
	switch( (*p) ) {
		case 65: goto st3;
		case 68: goto st9;
		case 72: goto st26;
		case 73: goto st29;
		case 77: goto st32;
		case 80: goto st40;
		case 86: goto st45;
	}
	goto st0;
st3:
	p += 1;
case 3:
	if ( (*p) == 85 )
		goto st4;
	goto st0;
st4:
	p += 1;
case 4:
	if ( (*p) == 68 )
		goto st5;
	goto st0;
st5:
	p += 1;
case 5:
	if ( (*p) == 73 )
		goto st6;
	goto st0;
st6:
	p += 1;
case 6:
	if ( (*p) == 79 )
		goto st7;
	goto st0;
st7:
	p += 1;
case 7:
	if ( (*p) == 37 )
		goto st8;
	goto st0;
st8:
	p += 1;
case 8:
	switch( (*p) ) {
		case 0: goto tr15;
		case 48: goto st0;
	}
	goto st8;
tr15:
#line 26 "lex/parser_path_template.rl"
	{ {p++; cs = 55; goto _out;} }
	goto st55;
st55:
	p += 1;
case 55:
#line 109 "lex/parser_path_template.cpp"
	switch( (*p) ) {
		case 0: goto tr15;
		case 48: goto st0;
	}
	goto st8;
st9:
	p += 1;
case 9:
	switch( (*p) ) {
		case 69: goto st10;
		case 79: goto st15;
	}
	goto st0;
st10:
	p += 1;
case 10:
	if ( (*p) == 83 )
		goto st11;
	goto st0;
st11:
	p += 1;
case 11:
	if ( (*p) == 75 )
		goto st12;
	goto st0;
st12:
	p += 1;
case 12:
	if ( (*p) == 84 )
		goto st13;
	goto st0;
st13:
	p += 1;
case 13:
	if ( (*p) == 79 )
		goto st14;
	goto st0;
st14:
	p += 1;
case 14:
	if ( (*p) == 80 )
		goto st7;
	goto st0;
st15:
	p += 1;
case 15:
	switch( (*p) ) {
		case 67: goto st16;
		case 87: goto st21;
	}
	goto st0;
st16:
	p += 1;
case 16:
	switch( (*p) ) {
		case 37: goto st8;
		case 85: goto st17;
	}
	goto st0;
st17:
	p += 1;
case 17:
	if ( (*p) == 77 )
		goto st18;
	goto st0;
st18:
	p += 1;
case 18:
	if ( (*p) == 69 )
		goto st19;
	goto st0;
st19:
	p += 1;
case 19:
	if ( (*p) == 78 )
		goto st20;
	goto st0;
st20:
	p += 1;
case 20:
	if ( (*p) == 84 )
		goto st7;
	goto st0;
st21:
	p += 1;
case 21:
	if ( (*p) == 78 )
		goto st22;
	goto st0;
st22:
	p += 1;
case 22:
	if ( (*p) == 76 )
		goto st23;
	goto st0;
st23:
	p += 1;
case 23:
	if ( (*p) == 79 )
		goto st24;
	goto st0;
st24:
	p += 1;
case 24:
	if ( (*p) == 65 )
		goto st25;
	goto st0;
st25:
	p += 1;
case 25:
	if ( (*p) == 68 )
		goto st7;
	goto st0;
st26:
	p += 1;
case 26:
	if ( (*p) == 79 )
		goto st27;
	goto st0;
st27:
	p += 1;
case 27:
	if ( (*p) == 77 )
		goto st28;
	goto st0;
st28:
	p += 1;
case 28:
	if ( (*p) == 69 )
		goto st7;
	goto st0;
st29:
	p += 1;
case 29:
	if ( (*p) == 77 )
		goto st30;
	goto st0;
st30:
	p += 1;
case 30:
	if ( (*p) == 65 )
		goto st31;
	goto st0;
st31:
	p += 1;
case 31:
	if ( (*p) == 71 )
		goto st28;
	goto st0;
st32:
	p += 1;
case 32:
	switch( (*p) ) {
		case 79: goto st33;
		case 85: goto st37;
	}
	goto st0;
st33:
	p += 1;
case 33:
	if ( (*p) == 86 )
		goto st34;
	goto st0;
st34:
	p += 1;
case 34:
	if ( (*p) == 73 )
		goto st35;
	goto st0;
st35:
	p += 1;
case 35:
	if ( (*p) == 69 )
		goto st36;
	goto st0;
st36:
	p += 1;
case 36:
	switch( (*p) ) {
		case 37: goto st8;
		case 83: goto st7;
	}
	goto st0;
st37:
	p += 1;
case 37:
	if ( (*p) == 83 )
		goto st38;
	goto st0;
st38:
	p += 1;
case 38:
	if ( (*p) == 73 )
		goto st39;
	goto st0;
st39:
	p += 1;
case 39:
	if ( (*p) == 67 )
		goto st7;
	goto st0;
st40:
	p += 1;
case 40:
	if ( (*p) == 73 )
		goto st41;
	goto st0;
st41:
	p += 1;
case 41:
	if ( (*p) == 67 )
		goto st42;
	goto st0;
st42:
	p += 1;
case 42:
	if ( (*p) == 84 )
		goto st43;
	goto st0;
st43:
	p += 1;
case 43:
	if ( (*p) == 85 )
		goto st44;
	goto st0;
st44:
	p += 1;
case 44:
	if ( (*p) == 82 )
		goto st28;
	goto st0;
st45:
	p += 1;
case 45:
	if ( (*p) == 73 )
		goto st46;
	goto st0;
st46:
	p += 1;
case 46:
	if ( (*p) == 68 )
		goto st47;
	goto st0;
st47:
	p += 1;
case 47:
	if ( (*p) == 69 )
		goto st6;
	goto st0;
st48:
	p += 1;
case 48:
	if ( (*p) == 105 )
		goto st49;
	goto st0;
st49:
	p += 1;
case 49:
	if ( (*p) == 108 )
		goto st50;
	goto st0;
st50:
	p += 1;
case 50:
	if ( (*p) == 101 )
		goto st51;
	goto st0;
st51:
	p += 1;
case 51:
	if ( (*p) == 58 )
		goto st52;
	goto st0;
st52:
	p += 1;
case 52:
	if ( (*p) == 47 )
		goto st53;
	goto st0;
st53:
	p += 1;
case 53:
	if ( (*p) == 47 )
		goto st54;
	goto st0;
st54:
	p += 1;
case 54:
	switch( (*p) ) {
		case 37: goto st2;
		case 126: goto st8;
	}
	goto st0;
	}

	_out: {}
	}

#line 37 "lex/parser_path_template.rl"

    const auto ok = cs >= 55;
    return ok;
}

} // namespace parser
} // namespace ceammc
