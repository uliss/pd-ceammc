
#line 1 "lex/parser_path_template.rl"
# include "lex/parser_path_template.h"
# include "lex/ragel_common.h"

# include <cstring>
# include <limits>
# include <cstdint>

namespace ceammc {
namespace parser {


#line 15 "lex/parser_path_template.cpp"
static const int parser_path_template_start = 1;
static const int parser_path_template_first_final = 76;
static const int parser_path_template_error = 0;

static const int parser_path_template_en_main = 1;


#line 40 "lex/parser_path_template.rl"


DirectoryTemplate path_get_dir_template(const char* path, const char** begin, const char** end) {
    int cs = 0;
    const char* p = path;
    DirectoryTemplate res = DirectoryTemplate::None;

    if (begin)
        *begin = nullptr;

    if (end)
        *end = nullptr;

    
#line 38 "lex/parser_path_template.cpp"
	{
	cs = parser_path_template_start;
	}

#line 54 "lex/parser_path_template.rl"
    
#line 45 "lex/parser_path_template.cpp"
	{
	switch ( cs )
	{
case 1:
	switch( (*p) ) {
		case 37: goto tr0;
		case 102: goto st69;
		case 126: goto tr3;
	}
	goto st0;
st0:
cs = 0;
	goto _out;
tr0:
#line 15 "lex/parser_path_template.rl"
	{ if(begin) *begin = p; }
	goto st2;
st2:
	p += 1;
case 2:
#line 66 "lex/parser_path_template.cpp"
	switch( (*p) ) {
		case 65: goto st6;
		case 68: goto st12;
		case 72: goto st37;
		case 73: goto st42;
		case 77: goto st48;
		case 80: goto st58;
		case 86: goto st65;
	}
	if ( (*p) > 49 ) {
		if ( 66 <= (*p) && (*p) <= 90 )
			goto st3;
	} else if ( (*p) >= 48 )
		goto st3;
	goto st0;
st3:
	p += 1;
case 3:
	if ( (*p) == 37 )
		goto st4;
	if ( (*p) > 49 ) {
		if ( 65 <= (*p) && (*p) <= 90 )
			goto st3;
	} else if ( (*p) >= 48 )
		goto st3;
	goto st0;
st4:
	p += 1;
case 4:
	switch( (*p) ) {
		case 0: goto tr14;
		case 48: goto st0;
	}
	goto tr13;
tr13:
#line 31 "lex/parser_path_template.rl"
	{ res = DirectoryTemplate::Unknown; }
#line 16 "lex/parser_path_template.rl"
	{ if(end)   *end = p; }
	goto st5;
tr22:
#line 19 "lex/parser_path_template.rl"
	{ res = DirectoryTemplate::Audio; }
#line 16 "lex/parser_path_template.rl"
	{ if(end)   *end = p; }
	goto st5;
tr32:
#line 23 "lex/parser_path_template.rl"
	{ res = DirectoryTemplate::Desktop; }
#line 16 "lex/parser_path_template.rl"
	{ if(end)   *end = p; }
	goto st5;
tr38:
#line 22 "lex/parser_path_template.rl"
	{ res = DirectoryTemplate::Document; }
#line 16 "lex/parser_path_template.rl"
	{ if(end)   *end = p; }
	goto st5;
tr52:
#line 24 "lex/parser_path_template.rl"
	{ res = DirectoryTemplate::Download; }
#line 16 "lex/parser_path_template.rl"
	{ if(end)   *end = p; }
	goto st5;
tr58:
#line 18 "lex/parser_path_template.rl"
	{ res = DirectoryTemplate::Home; }
#line 16 "lex/parser_path_template.rl"
	{ if(end)   *end = p; }
	goto st5;
tr65:
#line 21 "lex/parser_path_template.rl"
	{ res = DirectoryTemplate::Picture; }
#line 16 "lex/parser_path_template.rl"
	{ if(end)   *end = p; }
	goto st5;
tr74:
#line 20 "lex/parser_path_template.rl"
	{ res = DirectoryTemplate::Video; }
#line 16 "lex/parser_path_template.rl"
	{ if(end)   *end = p; }
	goto st5;
st5:
	p += 1;
case 5:
#line 152 "lex/parser_path_template.cpp"
	switch( (*p) ) {
		case 0: goto tr16;
		case 48: goto st0;
	}
	goto st5;
tr14:
#line 31 "lex/parser_path_template.rl"
	{ res = DirectoryTemplate::Unknown; }
#line 16 "lex/parser_path_template.rl"
	{ if(end)   *end = p; }
#line 37 "lex/parser_path_template.rl"
	{ {p++; cs = 76; goto _out;} }
	goto st76;
tr16:
#line 37 "lex/parser_path_template.rl"
	{ {p++; cs = 76; goto _out;} }
	goto st76;
tr23:
#line 19 "lex/parser_path_template.rl"
	{ res = DirectoryTemplate::Audio; }
#line 16 "lex/parser_path_template.rl"
	{ if(end)   *end = p; }
#line 37 "lex/parser_path_template.rl"
	{ {p++; cs = 76; goto _out;} }
	goto st76;
tr33:
#line 23 "lex/parser_path_template.rl"
	{ res = DirectoryTemplate::Desktop; }
#line 16 "lex/parser_path_template.rl"
	{ if(end)   *end = p; }
#line 37 "lex/parser_path_template.rl"
	{ {p++; cs = 76; goto _out;} }
	goto st76;
tr39:
#line 22 "lex/parser_path_template.rl"
	{ res = DirectoryTemplate::Document; }
#line 16 "lex/parser_path_template.rl"
	{ if(end)   *end = p; }
#line 37 "lex/parser_path_template.rl"
	{ {p++; cs = 76; goto _out;} }
	goto st76;
tr53:
#line 24 "lex/parser_path_template.rl"
	{ res = DirectoryTemplate::Download; }
#line 16 "lex/parser_path_template.rl"
	{ if(end)   *end = p; }
#line 37 "lex/parser_path_template.rl"
	{ {p++; cs = 76; goto _out;} }
	goto st76;
tr59:
#line 18 "lex/parser_path_template.rl"
	{ res = DirectoryTemplate::Home; }
#line 16 "lex/parser_path_template.rl"
	{ if(end)   *end = p; }
#line 37 "lex/parser_path_template.rl"
	{ {p++; cs = 76; goto _out;} }
	goto st76;
tr66:
#line 21 "lex/parser_path_template.rl"
	{ res = DirectoryTemplate::Picture; }
#line 16 "lex/parser_path_template.rl"
	{ if(end)   *end = p; }
#line 37 "lex/parser_path_template.rl"
	{ {p++; cs = 76; goto _out;} }
	goto st76;
tr75:
#line 20 "lex/parser_path_template.rl"
	{ res = DirectoryTemplate::Video; }
#line 16 "lex/parser_path_template.rl"
	{ if(end)   *end = p; }
#line 37 "lex/parser_path_template.rl"
	{ {p++; cs = 76; goto _out;} }
	goto st76;
st76:
	p += 1;
case 76:
#line 229 "lex/parser_path_template.cpp"
	switch( (*p) ) {
		case 0: goto tr16;
		case 48: goto st0;
	}
	goto st5;
st6:
	p += 1;
case 6:
	switch( (*p) ) {
		case 37: goto st4;
		case 85: goto st7;
	}
	if ( (*p) > 49 ) {
		if ( 65 <= (*p) && (*p) <= 90 )
			goto st3;
	} else if ( (*p) >= 48 )
		goto st3;
	goto st0;
st7:
	p += 1;
case 7:
	switch( (*p) ) {
		case 37: goto st4;
		case 68: goto st8;
	}
	if ( (*p) > 49 ) {
		if ( 65 <= (*p) && (*p) <= 90 )
			goto st3;
	} else if ( (*p) >= 48 )
		goto st3;
	goto st0;
st8:
	p += 1;
case 8:
	switch( (*p) ) {
		case 37: goto st4;
		case 73: goto st9;
	}
	if ( (*p) > 49 ) {
		if ( 65 <= (*p) && (*p) <= 90 )
			goto st3;
	} else if ( (*p) >= 48 )
		goto st3;
	goto st0;
st9:
	p += 1;
case 9:
	switch( (*p) ) {
		case 37: goto st4;
		case 79: goto st10;
	}
	if ( (*p) > 49 ) {
		if ( 65 <= (*p) && (*p) <= 90 )
			goto st3;
	} else if ( (*p) >= 48 )
		goto st3;
	goto st0;
st10:
	p += 1;
case 10:
	if ( (*p) == 37 )
		goto st11;
	if ( (*p) > 49 ) {
		if ( 65 <= (*p) && (*p) <= 90 )
			goto st3;
	} else if ( (*p) >= 48 )
		goto st3;
	goto st0;
st11:
	p += 1;
case 11:
	switch( (*p) ) {
		case 0: goto tr23;
		case 48: goto st0;
	}
	goto tr22;
st12:
	p += 1;
case 12:
	switch( (*p) ) {
		case 37: goto st4;
		case 69: goto st13;
		case 79: goto st20;
	}
	if ( (*p) > 49 ) {
		if ( 65 <= (*p) && (*p) <= 90 )
			goto st3;
	} else if ( (*p) >= 48 )
		goto st3;
	goto st0;
st13:
	p += 1;
case 13:
	switch( (*p) ) {
		case 37: goto st4;
		case 83: goto st14;
	}
	if ( (*p) > 49 ) {
		if ( 65 <= (*p) && (*p) <= 90 )
			goto st3;
	} else if ( (*p) >= 48 )
		goto st3;
	goto st0;
st14:
	p += 1;
case 14:
	switch( (*p) ) {
		case 37: goto st4;
		case 75: goto st15;
	}
	if ( (*p) > 49 ) {
		if ( 65 <= (*p) && (*p) <= 90 )
			goto st3;
	} else if ( (*p) >= 48 )
		goto st3;
	goto st0;
st15:
	p += 1;
case 15:
	switch( (*p) ) {
		case 37: goto st4;
		case 84: goto st16;
	}
	if ( (*p) > 49 ) {
		if ( 65 <= (*p) && (*p) <= 90 )
			goto st3;
	} else if ( (*p) >= 48 )
		goto st3;
	goto st0;
st16:
	p += 1;
case 16:
	switch( (*p) ) {
		case 37: goto st4;
		case 79: goto st17;
	}
	if ( (*p) > 49 ) {
		if ( 65 <= (*p) && (*p) <= 90 )
			goto st3;
	} else if ( (*p) >= 48 )
		goto st3;
	goto st0;
st17:
	p += 1;
case 17:
	switch( (*p) ) {
		case 37: goto st4;
		case 80: goto st18;
	}
	if ( (*p) > 49 ) {
		if ( 65 <= (*p) && (*p) <= 90 )
			goto st3;
	} else if ( (*p) >= 48 )
		goto st3;
	goto st0;
st18:
	p += 1;
case 18:
	if ( (*p) == 37 )
		goto st19;
	if ( (*p) > 49 ) {
		if ( 65 <= (*p) && (*p) <= 90 )
			goto st3;
	} else if ( (*p) >= 48 )
		goto st3;
	goto st0;
st19:
	p += 1;
case 19:
	switch( (*p) ) {
		case 0: goto tr33;
		case 48: goto st0;
	}
	goto tr32;
st20:
	p += 1;
case 20:
	switch( (*p) ) {
		case 37: goto st4;
		case 67: goto st21;
		case 87: goto st29;
	}
	if ( (*p) > 49 ) {
		if ( 65 <= (*p) && (*p) <= 90 )
			goto st3;
	} else if ( (*p) >= 48 )
		goto st3;
	goto st0;
st21:
	p += 1;
case 21:
	switch( (*p) ) {
		case 37: goto st22;
		case 85: goto st23;
	}
	if ( (*p) > 49 ) {
		if ( 65 <= (*p) && (*p) <= 90 )
			goto st3;
	} else if ( (*p) >= 48 )
		goto st3;
	goto st0;
st22:
	p += 1;
case 22:
	switch( (*p) ) {
		case 0: goto tr39;
		case 48: goto st0;
	}
	goto tr38;
st23:
	p += 1;
case 23:
	switch( (*p) ) {
		case 37: goto st4;
		case 77: goto st24;
	}
	if ( (*p) > 49 ) {
		if ( 65 <= (*p) && (*p) <= 90 )
			goto st3;
	} else if ( (*p) >= 48 )
		goto st3;
	goto st0;
st24:
	p += 1;
case 24:
	switch( (*p) ) {
		case 37: goto st4;
		case 69: goto st25;
	}
	if ( (*p) > 49 ) {
		if ( 65 <= (*p) && (*p) <= 90 )
			goto st3;
	} else if ( (*p) >= 48 )
		goto st3;
	goto st0;
st25:
	p += 1;
case 25:
	switch( (*p) ) {
		case 37: goto st4;
		case 78: goto st26;
	}
	if ( (*p) > 49 ) {
		if ( 65 <= (*p) && (*p) <= 90 )
			goto st3;
	} else if ( (*p) >= 48 )
		goto st3;
	goto st0;
st26:
	p += 1;
case 26:
	switch( (*p) ) {
		case 37: goto st4;
		case 84: goto st27;
	}
	if ( (*p) > 49 ) {
		if ( 65 <= (*p) && (*p) <= 90 )
			goto st3;
	} else if ( (*p) >= 48 )
		goto st3;
	goto st0;
st27:
	p += 1;
case 27:
	switch( (*p) ) {
		case 37: goto st22;
		case 83: goto st28;
	}
	if ( (*p) > 49 ) {
		if ( 65 <= (*p) && (*p) <= 90 )
			goto st3;
	} else if ( (*p) >= 48 )
		goto st3;
	goto st0;
st28:
	p += 1;
case 28:
	if ( (*p) == 37 )
		goto st22;
	if ( (*p) > 49 ) {
		if ( 65 <= (*p) && (*p) <= 90 )
			goto st3;
	} else if ( (*p) >= 48 )
		goto st3;
	goto st0;
st29:
	p += 1;
case 29:
	switch( (*p) ) {
		case 37: goto st4;
		case 78: goto st30;
	}
	if ( (*p) > 49 ) {
		if ( 65 <= (*p) && (*p) <= 90 )
			goto st3;
	} else if ( (*p) >= 48 )
		goto st3;
	goto st0;
st30:
	p += 1;
case 30:
	switch( (*p) ) {
		case 37: goto st4;
		case 76: goto st31;
	}
	if ( (*p) > 49 ) {
		if ( 65 <= (*p) && (*p) <= 90 )
			goto st3;
	} else if ( (*p) >= 48 )
		goto st3;
	goto st0;
st31:
	p += 1;
case 31:
	switch( (*p) ) {
		case 37: goto st4;
		case 79: goto st32;
	}
	if ( (*p) > 49 ) {
		if ( 65 <= (*p) && (*p) <= 90 )
			goto st3;
	} else if ( (*p) >= 48 )
		goto st3;
	goto st0;
st32:
	p += 1;
case 32:
	switch( (*p) ) {
		case 37: goto st4;
		case 65: goto st33;
	}
	if ( (*p) > 49 ) {
		if ( 66 <= (*p) && (*p) <= 90 )
			goto st3;
	} else if ( (*p) >= 48 )
		goto st3;
	goto st0;
st33:
	p += 1;
case 33:
	switch( (*p) ) {
		case 37: goto st4;
		case 68: goto st34;
	}
	if ( (*p) > 49 ) {
		if ( 65 <= (*p) && (*p) <= 90 )
			goto st3;
	} else if ( (*p) >= 48 )
		goto st3;
	goto st0;
st34:
	p += 1;
case 34:
	switch( (*p) ) {
		case 37: goto st35;
		case 83: goto st36;
	}
	if ( (*p) > 49 ) {
		if ( 65 <= (*p) && (*p) <= 90 )
			goto st3;
	} else if ( (*p) >= 48 )
		goto st3;
	goto st0;
st35:
	p += 1;
case 35:
	switch( (*p) ) {
		case 0: goto tr53;
		case 48: goto st0;
	}
	goto tr52;
st36:
	p += 1;
case 36:
	if ( (*p) == 37 )
		goto st35;
	if ( (*p) > 49 ) {
		if ( 65 <= (*p) && (*p) <= 90 )
			goto st3;
	} else if ( (*p) >= 48 )
		goto st3;
	goto st0;
st37:
	p += 1;
case 37:
	switch( (*p) ) {
		case 37: goto st4;
		case 79: goto st38;
	}
	if ( (*p) > 49 ) {
		if ( 65 <= (*p) && (*p) <= 90 )
			goto st3;
	} else if ( (*p) >= 48 )
		goto st3;
	goto st0;
st38:
	p += 1;
case 38:
	switch( (*p) ) {
		case 37: goto st4;
		case 77: goto st39;
	}
	if ( (*p) > 49 ) {
		if ( 65 <= (*p) && (*p) <= 90 )
			goto st3;
	} else if ( (*p) >= 48 )
		goto st3;
	goto st0;
st39:
	p += 1;
case 39:
	switch( (*p) ) {
		case 37: goto st4;
		case 69: goto st40;
	}
	if ( (*p) > 49 ) {
		if ( 65 <= (*p) && (*p) <= 90 )
			goto st3;
	} else if ( (*p) >= 48 )
		goto st3;
	goto st0;
st40:
	p += 1;
case 40:
	if ( (*p) == 37 )
		goto st41;
	if ( (*p) > 49 ) {
		if ( 65 <= (*p) && (*p) <= 90 )
			goto st3;
	} else if ( (*p) >= 48 )
		goto st3;
	goto st0;
tr3:
#line 15 "lex/parser_path_template.rl"
	{ if(begin) *begin = p; }
	goto st41;
st41:
	p += 1;
case 41:
#line 669 "lex/parser_path_template.cpp"
	switch( (*p) ) {
		case 0: goto tr59;
		case 48: goto st0;
	}
	goto tr58;
st42:
	p += 1;
case 42:
	switch( (*p) ) {
		case 37: goto st4;
		case 77: goto st43;
	}
	if ( (*p) > 49 ) {
		if ( 65 <= (*p) && (*p) <= 90 )
			goto st3;
	} else if ( (*p) >= 48 )
		goto st3;
	goto st0;
st43:
	p += 1;
case 43:
	switch( (*p) ) {
		case 37: goto st4;
		case 65: goto st44;
	}
	if ( (*p) > 49 ) {
		if ( 66 <= (*p) && (*p) <= 90 )
			goto st3;
	} else if ( (*p) >= 48 )
		goto st3;
	goto st0;
st44:
	p += 1;
case 44:
	switch( (*p) ) {
		case 37: goto st4;
		case 71: goto st45;
	}
	if ( (*p) > 49 ) {
		if ( 65 <= (*p) && (*p) <= 90 )
			goto st3;
	} else if ( (*p) >= 48 )
		goto st3;
	goto st0;
st45:
	p += 1;
case 45:
	switch( (*p) ) {
		case 37: goto st4;
		case 69: goto st46;
	}
	if ( (*p) > 49 ) {
		if ( 65 <= (*p) && (*p) <= 90 )
			goto st3;
	} else if ( (*p) >= 48 )
		goto st3;
	goto st0;
st46:
	p += 1;
case 46:
	if ( (*p) == 37 )
		goto st47;
	if ( (*p) > 49 ) {
		if ( 65 <= (*p) && (*p) <= 90 )
			goto st3;
	} else if ( (*p) >= 48 )
		goto st3;
	goto st0;
st47:
	p += 1;
case 47:
	switch( (*p) ) {
		case 0: goto tr66;
		case 48: goto st0;
	}
	goto tr65;
st48:
	p += 1;
case 48:
	switch( (*p) ) {
		case 37: goto st4;
		case 79: goto st49;
		case 85: goto st55;
	}
	if ( (*p) > 49 ) {
		if ( 65 <= (*p) && (*p) <= 90 )
			goto st3;
	} else if ( (*p) >= 48 )
		goto st3;
	goto st0;
st49:
	p += 1;
case 49:
	switch( (*p) ) {
		case 37: goto st4;
		case 86: goto st50;
	}
	if ( (*p) > 49 ) {
		if ( 65 <= (*p) && (*p) <= 90 )
			goto st3;
	} else if ( (*p) >= 48 )
		goto st3;
	goto st0;
st50:
	p += 1;
case 50:
	switch( (*p) ) {
		case 37: goto st4;
		case 73: goto st51;
	}
	if ( (*p) > 49 ) {
		if ( 65 <= (*p) && (*p) <= 90 )
			goto st3;
	} else if ( (*p) >= 48 )
		goto st3;
	goto st0;
st51:
	p += 1;
case 51:
	switch( (*p) ) {
		case 37: goto st4;
		case 69: goto st52;
	}
	if ( (*p) > 49 ) {
		if ( 65 <= (*p) && (*p) <= 90 )
			goto st3;
	} else if ( (*p) >= 48 )
		goto st3;
	goto st0;
st52:
	p += 1;
case 52:
	switch( (*p) ) {
		case 37: goto st53;
		case 83: goto st54;
	}
	if ( (*p) > 49 ) {
		if ( 65 <= (*p) && (*p) <= 90 )
			goto st3;
	} else if ( (*p) >= 48 )
		goto st3;
	goto st0;
st53:
	p += 1;
case 53:
	switch( (*p) ) {
		case 0: goto tr75;
		case 48: goto st0;
	}
	goto tr74;
st54:
	p += 1;
case 54:
	if ( (*p) == 37 )
		goto st53;
	if ( (*p) > 49 ) {
		if ( 65 <= (*p) && (*p) <= 90 )
			goto st3;
	} else if ( (*p) >= 48 )
		goto st3;
	goto st0;
st55:
	p += 1;
case 55:
	switch( (*p) ) {
		case 37: goto st4;
		case 83: goto st56;
	}
	if ( (*p) > 49 ) {
		if ( 65 <= (*p) && (*p) <= 90 )
			goto st3;
	} else if ( (*p) >= 48 )
		goto st3;
	goto st0;
st56:
	p += 1;
case 56:
	switch( (*p) ) {
		case 37: goto st4;
		case 73: goto st57;
	}
	if ( (*p) > 49 ) {
		if ( 65 <= (*p) && (*p) <= 90 )
			goto st3;
	} else if ( (*p) >= 48 )
		goto st3;
	goto st0;
st57:
	p += 1;
case 57:
	switch( (*p) ) {
		case 37: goto st4;
		case 67: goto st10;
	}
	if ( (*p) > 49 ) {
		if ( 65 <= (*p) && (*p) <= 90 )
			goto st3;
	} else if ( (*p) >= 48 )
		goto st3;
	goto st0;
st58:
	p += 1;
case 58:
	switch( (*p) ) {
		case 37: goto st4;
		case 73: goto st59;
	}
	if ( (*p) > 49 ) {
		if ( 65 <= (*p) && (*p) <= 90 )
			goto st3;
	} else if ( (*p) >= 48 )
		goto st3;
	goto st0;
st59:
	p += 1;
case 59:
	switch( (*p) ) {
		case 37: goto st4;
		case 67: goto st60;
	}
	if ( (*p) > 49 ) {
		if ( 65 <= (*p) && (*p) <= 90 )
			goto st3;
	} else if ( (*p) >= 48 )
		goto st3;
	goto st0;
st60:
	p += 1;
case 60:
	switch( (*p) ) {
		case 37: goto st4;
		case 84: goto st61;
	}
	if ( (*p) > 49 ) {
		if ( 65 <= (*p) && (*p) <= 90 )
			goto st3;
	} else if ( (*p) >= 48 )
		goto st3;
	goto st0;
st61:
	p += 1;
case 61:
	switch( (*p) ) {
		case 37: goto st4;
		case 85: goto st62;
	}
	if ( (*p) > 49 ) {
		if ( 65 <= (*p) && (*p) <= 90 )
			goto st3;
	} else if ( (*p) >= 48 )
		goto st3;
	goto st0;
st62:
	p += 1;
case 62:
	switch( (*p) ) {
		case 37: goto st4;
		case 82: goto st63;
	}
	if ( (*p) > 49 ) {
		if ( 65 <= (*p) && (*p) <= 90 )
			goto st3;
	} else if ( (*p) >= 48 )
		goto st3;
	goto st0;
st63:
	p += 1;
case 63:
	switch( (*p) ) {
		case 37: goto st4;
		case 69: goto st64;
	}
	if ( (*p) > 49 ) {
		if ( 65 <= (*p) && (*p) <= 90 )
			goto st3;
	} else if ( (*p) >= 48 )
		goto st3;
	goto st0;
st64:
	p += 1;
case 64:
	switch( (*p) ) {
		case 37: goto st47;
		case 83: goto st46;
	}
	if ( (*p) > 49 ) {
		if ( 65 <= (*p) && (*p) <= 90 )
			goto st3;
	} else if ( (*p) >= 48 )
		goto st3;
	goto st0;
st65:
	p += 1;
case 65:
	switch( (*p) ) {
		case 37: goto st4;
		case 73: goto st66;
	}
	if ( (*p) > 49 ) {
		if ( 65 <= (*p) && (*p) <= 90 )
			goto st3;
	} else if ( (*p) >= 48 )
		goto st3;
	goto st0;
st66:
	p += 1;
case 66:
	switch( (*p) ) {
		case 37: goto st4;
		case 68: goto st67;
	}
	if ( (*p) > 49 ) {
		if ( 65 <= (*p) && (*p) <= 90 )
			goto st3;
	} else if ( (*p) >= 48 )
		goto st3;
	goto st0;
st67:
	p += 1;
case 67:
	switch( (*p) ) {
		case 37: goto st4;
		case 69: goto st68;
	}
	if ( (*p) > 49 ) {
		if ( 65 <= (*p) && (*p) <= 90 )
			goto st3;
	} else if ( (*p) >= 48 )
		goto st3;
	goto st0;
st68:
	p += 1;
case 68:
	switch( (*p) ) {
		case 37: goto st4;
		case 79: goto st54;
	}
	if ( (*p) > 49 ) {
		if ( 65 <= (*p) && (*p) <= 90 )
			goto st3;
	} else if ( (*p) >= 48 )
		goto st3;
	goto st0;
st69:
	p += 1;
case 69:
	if ( (*p) == 105 )
		goto st70;
	goto st0;
st70:
	p += 1;
case 70:
	if ( (*p) == 108 )
		goto st71;
	goto st0;
st71:
	p += 1;
case 71:
	if ( (*p) == 101 )
		goto st72;
	goto st0;
st72:
	p += 1;
case 72:
	if ( (*p) == 58 )
		goto st73;
	goto st0;
st73:
	p += 1;
case 73:
	if ( (*p) == 47 )
		goto st74;
	goto st0;
st74:
	p += 1;
case 74:
	if ( (*p) == 47 )
		goto st75;
	goto st0;
st75:
	p += 1;
case 75:
	switch( (*p) ) {
		case 37: goto tr0;
		case 126: goto tr3;
	}
	goto st0;
	}

	_out: {}
	}

#line 55 "lex/parser_path_template.rl"

    if (cs >= 76)
        return res;
    else
        return DirectoryTemplate::None;
}

} // namespace parser
} // namespace ceammc
