
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
static const int parser_path_template_first_final = 84;
static const int parser_path_template_error = 0;

static const int parser_path_template_en_main = 1;


#line 42 "lex/parser_path_template.rl"


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

#line 56 "lex/parser_path_template.rl"
    
#line 45 "lex/parser_path_template.cpp"
	{
	switch ( cs )
	{
case 1:
	switch( (*p) ) {
		case 37: goto tr0;
		case 102: goto st77;
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
		case 67: goto st12;
		case 68: goto st16;
		case 72: goto st41;
		case 73: goto st46;
		case 77: goto st52;
		case 80: goto st62;
		case 84: goto st69;
		case 86: goto st73;
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
		case 0: goto tr16;
		case 48: goto st0;
	}
	goto tr15;
tr15:
#line 33 "lex/parser_path_template.rl"
	{ res = DirectoryTemplate::Unknown; }
#line 16 "lex/parser_path_template.rl"
	{ if(end)   *end = p; }
	goto st5;
tr24:
#line 19 "lex/parser_path_template.rl"
	{ res = DirectoryTemplate::Audio; }
#line 16 "lex/parser_path_template.rl"
	{ if(end)   *end = p; }
	goto st5;
tr29:
#line 26 "lex/parser_path_template.rl"
	{ res = DirectoryTemplate::Cwd; }
#line 16 "lex/parser_path_template.rl"
	{ if(end)   *end = p; }
	goto st5;
tr39:
#line 24 "lex/parser_path_template.rl"
	{ res = DirectoryTemplate::Desktop; }
#line 16 "lex/parser_path_template.rl"
	{ if(end)   *end = p; }
	goto st5;
tr46:
#line 22 "lex/parser_path_template.rl"
	{ res = DirectoryTemplate::Document; }
#line 16 "lex/parser_path_template.rl"
	{ if(end)   *end = p; }
	goto st5;
tr59:
#line 23 "lex/parser_path_template.rl"
	{ res = DirectoryTemplate::Download; }
#line 16 "lex/parser_path_template.rl"
	{ if(end)   *end = p; }
	goto st5;
tr65:
#line 18 "lex/parser_path_template.rl"
	{ res = DirectoryTemplate::Home; }
#line 16 "lex/parser_path_template.rl"
	{ if(end)   *end = p; }
	goto st5;
tr72:
#line 21 "lex/parser_path_template.rl"
	{ res = DirectoryTemplate::Picture; }
#line 16 "lex/parser_path_template.rl"
	{ if(end)   *end = p; }
	goto st5;
tr81:
#line 20 "lex/parser_path_template.rl"
	{ res = DirectoryTemplate::Video; }
#line 16 "lex/parser_path_template.rl"
	{ if(end)   *end = p; }
	goto st5;
tr94:
#line 25 "lex/parser_path_template.rl"
	{ res = DirectoryTemplate::Tmp; }
#line 16 "lex/parser_path_template.rl"
	{ if(end)   *end = p; }
	goto st5;
st5:
	p += 1;
case 5:
#line 166 "lex/parser_path_template.cpp"
	switch( (*p) ) {
		case 0: goto tr18;
		case 48: goto st0;
	}
	goto st5;
tr16:
#line 33 "lex/parser_path_template.rl"
	{ res = DirectoryTemplate::Unknown; }
#line 16 "lex/parser_path_template.rl"
	{ if(end)   *end = p; }
#line 39 "lex/parser_path_template.rl"
	{ {p++; cs = 84; goto _out;} }
	goto st84;
tr18:
#line 39 "lex/parser_path_template.rl"
	{ {p++; cs = 84; goto _out;} }
	goto st84;
tr25:
#line 19 "lex/parser_path_template.rl"
	{ res = DirectoryTemplate::Audio; }
#line 16 "lex/parser_path_template.rl"
	{ if(end)   *end = p; }
#line 39 "lex/parser_path_template.rl"
	{ {p++; cs = 84; goto _out;} }
	goto st84;
tr30:
#line 26 "lex/parser_path_template.rl"
	{ res = DirectoryTemplate::Cwd; }
#line 16 "lex/parser_path_template.rl"
	{ if(end)   *end = p; }
#line 39 "lex/parser_path_template.rl"
	{ {p++; cs = 84; goto _out;} }
	goto st84;
tr40:
#line 24 "lex/parser_path_template.rl"
	{ res = DirectoryTemplate::Desktop; }
#line 16 "lex/parser_path_template.rl"
	{ if(end)   *end = p; }
#line 39 "lex/parser_path_template.rl"
	{ {p++; cs = 84; goto _out;} }
	goto st84;
tr47:
#line 22 "lex/parser_path_template.rl"
	{ res = DirectoryTemplate::Document; }
#line 16 "lex/parser_path_template.rl"
	{ if(end)   *end = p; }
#line 39 "lex/parser_path_template.rl"
	{ {p++; cs = 84; goto _out;} }
	goto st84;
tr60:
#line 23 "lex/parser_path_template.rl"
	{ res = DirectoryTemplate::Download; }
#line 16 "lex/parser_path_template.rl"
	{ if(end)   *end = p; }
#line 39 "lex/parser_path_template.rl"
	{ {p++; cs = 84; goto _out;} }
	goto st84;
tr66:
#line 18 "lex/parser_path_template.rl"
	{ res = DirectoryTemplate::Home; }
#line 16 "lex/parser_path_template.rl"
	{ if(end)   *end = p; }
#line 39 "lex/parser_path_template.rl"
	{ {p++; cs = 84; goto _out;} }
	goto st84;
tr73:
#line 21 "lex/parser_path_template.rl"
	{ res = DirectoryTemplate::Picture; }
#line 16 "lex/parser_path_template.rl"
	{ if(end)   *end = p; }
#line 39 "lex/parser_path_template.rl"
	{ {p++; cs = 84; goto _out;} }
	goto st84;
tr82:
#line 20 "lex/parser_path_template.rl"
	{ res = DirectoryTemplate::Video; }
#line 16 "lex/parser_path_template.rl"
	{ if(end)   *end = p; }
#line 39 "lex/parser_path_template.rl"
	{ {p++; cs = 84; goto _out;} }
	goto st84;
tr95:
#line 25 "lex/parser_path_template.rl"
	{ res = DirectoryTemplate::Tmp; }
#line 16 "lex/parser_path_template.rl"
	{ if(end)   *end = p; }
#line 39 "lex/parser_path_template.rl"
	{ {p++; cs = 84; goto _out;} }
	goto st84;
st84:
	p += 1;
case 84:
#line 259 "lex/parser_path_template.cpp"
	switch( (*p) ) {
		case 0: goto tr18;
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
		case 0: goto tr25;
		case 48: goto st0;
	}
	goto tr24;
st12:
	p += 1;
case 12:
	switch( (*p) ) {
		case 37: goto st4;
		case 87: goto st13;
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
		case 68: goto st14;
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
	if ( (*p) == 37 )
		goto st15;
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
		case 0: goto tr30;
		case 48: goto st0;
	}
	goto tr29;
st16:
	p += 1;
case 16:
	switch( (*p) ) {
		case 37: goto st4;
		case 69: goto st17;
		case 79: goto st24;
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
		case 83: goto st18;
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
	switch( (*p) ) {
		case 37: goto st4;
		case 75: goto st19;
	}
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
		case 37: goto st4;
		case 84: goto st20;
	}
	if ( (*p) > 49 ) {
		if ( 65 <= (*p) && (*p) <= 90 )
			goto st3;
	} else if ( (*p) >= 48 )
		goto st3;
	goto st0;
st20:
	p += 1;
case 20:
	switch( (*p) ) {
		case 37: goto st4;
		case 79: goto st21;
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
		case 37: goto st4;
		case 80: goto st22;
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
	if ( (*p) == 37 )
		goto st23;
	if ( (*p) > 49 ) {
		if ( 65 <= (*p) && (*p) <= 90 )
			goto st3;
	} else if ( (*p) >= 48 )
		goto st3;
	goto st0;
st23:
	p += 1;
case 23:
	switch( (*p) ) {
		case 0: goto tr40;
		case 48: goto st0;
	}
	goto tr39;
st24:
	p += 1;
case 24:
	switch( (*p) ) {
		case 37: goto st4;
		case 67: goto st25;
		case 87: goto st33;
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
		case 37: goto st26;
		case 83: goto st27;
		case 85: goto st28;
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
		case 0: goto tr47;
		case 48: goto st0;
	}
	goto tr46;
st27:
	p += 1;
case 27:
	if ( (*p) == 37 )
		goto st26;
	if ( (*p) > 49 ) {
		if ( 65 <= (*p) && (*p) <= 90 )
			goto st3;
	} else if ( (*p) >= 48 )
		goto st3;
	goto st0;
st28:
	p += 1;
case 28:
	switch( (*p) ) {
		case 37: goto st4;
		case 77: goto st29;
	}
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
		case 69: goto st30;
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
		case 78: goto st31;
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
		case 84: goto st32;
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
		case 37: goto st26;
		case 83: goto st27;
	}
	if ( (*p) > 49 ) {
		if ( 65 <= (*p) && (*p) <= 90 )
			goto st3;
	} else if ( (*p) >= 48 )
		goto st3;
	goto st0;
st33:
	p += 1;
case 33:
	switch( (*p) ) {
		case 37: goto st4;
		case 78: goto st34;
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
		case 37: goto st4;
		case 76: goto st35;
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
		case 37: goto st4;
		case 79: goto st36;
	}
	if ( (*p) > 49 ) {
		if ( 65 <= (*p) && (*p) <= 90 )
			goto st3;
	} else if ( (*p) >= 48 )
		goto st3;
	goto st0;
st36:
	p += 1;
case 36:
	switch( (*p) ) {
		case 37: goto st4;
		case 65: goto st37;
	}
	if ( (*p) > 49 ) {
		if ( 66 <= (*p) && (*p) <= 90 )
			goto st3;
	} else if ( (*p) >= 48 )
		goto st3;
	goto st0;
st37:
	p += 1;
case 37:
	switch( (*p) ) {
		case 37: goto st4;
		case 68: goto st38;
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
		case 37: goto st39;
		case 83: goto st40;
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
		case 0: goto tr60;
		case 48: goto st0;
	}
	goto tr59;
st40:
	p += 1;
case 40:
	if ( (*p) == 37 )
		goto st39;
	if ( (*p) > 49 ) {
		if ( 65 <= (*p) && (*p) <= 90 )
			goto st3;
	} else if ( (*p) >= 48 )
		goto st3;
	goto st0;
st41:
	p += 1;
case 41:
	switch( (*p) ) {
		case 37: goto st4;
		case 79: goto st42;
	}
	if ( (*p) > 49 ) {
		if ( 65 <= (*p) && (*p) <= 90 )
			goto st3;
	} else if ( (*p) >= 48 )
		goto st3;
	goto st0;
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
		case 69: goto st44;
	}
	if ( (*p) > 49 ) {
		if ( 65 <= (*p) && (*p) <= 90 )
			goto st3;
	} else if ( (*p) >= 48 )
		goto st3;
	goto st0;
st44:
	p += 1;
case 44:
	if ( (*p) == 37 )
		goto st45;
	if ( (*p) > 49 ) {
		if ( 65 <= (*p) && (*p) <= 90 )
			goto st3;
	} else if ( (*p) >= 48 )
		goto st3;
	goto st0;
tr3:
#line 15 "lex/parser_path_template.rl"
	{ if(begin) *begin = p; }
	goto st45;
st45:
	p += 1;
case 45:
#line 745 "lex/parser_path_template.cpp"
	switch( (*p) ) {
		case 0: goto tr66;
		case 48: goto st0;
	}
	goto tr65;
st46:
	p += 1;
case 46:
	switch( (*p) ) {
		case 37: goto st4;
		case 77: goto st47;
	}
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
		case 37: goto st4;
		case 65: goto st48;
		case 71: goto st50;
	}
	if ( (*p) > 49 ) {
		if ( 66 <= (*p) && (*p) <= 90 )
			goto st3;
	} else if ( (*p) >= 48 )
		goto st3;
	goto st0;
st48:
	p += 1;
case 48:
	switch( (*p) ) {
		case 37: goto st4;
		case 71: goto st49;
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
		case 69: goto st50;
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
	if ( (*p) == 37 )
		goto st51;
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
		case 0: goto tr73;
		case 48: goto st0;
	}
	goto tr72;
st52:
	p += 1;
case 52:
	switch( (*p) ) {
		case 37: goto st4;
		case 79: goto st53;
		case 85: goto st59;
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
		case 37: goto st4;
		case 86: goto st54;
	}
	if ( (*p) > 49 ) {
		if ( 65 <= (*p) && (*p) <= 90 )
			goto st3;
	} else if ( (*p) >= 48 )
		goto st3;
	goto st0;
st54:
	p += 1;
case 54:
	switch( (*p) ) {
		case 37: goto st4;
		case 73: goto st55;
	}
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
		case 69: goto st56;
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
		case 37: goto st57;
		case 83: goto st58;
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
		case 0: goto tr82;
		case 48: goto st0;
	}
	goto tr81;
st58:
	p += 1;
case 58:
	if ( (*p) == 37 )
		goto st57;
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
		case 83: goto st60;
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
		case 73: goto st61;
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
		case 67: goto st10;
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
		case 73: goto st63;
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
		case 67: goto st64;
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
		case 37: goto st51;
		case 83: goto st50;
		case 84: goto st65;
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
		case 85: goto st66;
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
		case 82: goto st67;
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
		case 37: goto st51;
		case 83: goto st50;
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
	switch( (*p) ) {
		case 37: goto st4;
		case 77: goto st70;
	}
	if ( (*p) > 49 ) {
		if ( 65 <= (*p) && (*p) <= 90 )
			goto st3;
	} else if ( (*p) >= 48 )
		goto st3;
	goto st0;
st70:
	p += 1;
case 70:
	switch( (*p) ) {
		case 37: goto st4;
		case 80: goto st71;
	}
	if ( (*p) > 49 ) {
		if ( 65 <= (*p) && (*p) <= 90 )
			goto st3;
	} else if ( (*p) >= 48 )
		goto st3;
	goto st0;
st71:
	p += 1;
case 71:
	if ( (*p) == 37 )
		goto st72;
	if ( (*p) > 49 ) {
		if ( 65 <= (*p) && (*p) <= 90 )
			goto st3;
	} else if ( (*p) >= 48 )
		goto st3;
	goto st0;
st72:
	p += 1;
case 72:
	switch( (*p) ) {
		case 0: goto tr95;
		case 48: goto st0;
	}
	goto tr94;
st73:
	p += 1;
case 73:
	switch( (*p) ) {
		case 37: goto st4;
		case 73: goto st74;
	}
	if ( (*p) > 49 ) {
		if ( 65 <= (*p) && (*p) <= 90 )
			goto st3;
	} else if ( (*p) >= 48 )
		goto st3;
	goto st0;
st74:
	p += 1;
case 74:
	switch( (*p) ) {
		case 37: goto st4;
		case 68: goto st75;
	}
	if ( (*p) > 49 ) {
		if ( 65 <= (*p) && (*p) <= 90 )
			goto st3;
	} else if ( (*p) >= 48 )
		goto st3;
	goto st0;
st75:
	p += 1;
case 75:
	switch( (*p) ) {
		case 37: goto st4;
		case 69: goto st76;
	}
	if ( (*p) > 49 ) {
		if ( 65 <= (*p) && (*p) <= 90 )
			goto st3;
	} else if ( (*p) >= 48 )
		goto st3;
	goto st0;
st76:
	p += 1;
case 76:
	switch( (*p) ) {
		case 37: goto st4;
		case 79: goto st58;
	}
	if ( (*p) > 49 ) {
		if ( 65 <= (*p) && (*p) <= 90 )
			goto st3;
	} else if ( (*p) >= 48 )
		goto st3;
	goto st0;
st77:
	p += 1;
case 77:
	if ( (*p) == 105 )
		goto st78;
	goto st0;
st78:
	p += 1;
case 78:
	if ( (*p) == 108 )
		goto st79;
	goto st0;
st79:
	p += 1;
case 79:
	if ( (*p) == 101 )
		goto st80;
	goto st0;
st80:
	p += 1;
case 80:
	if ( (*p) == 58 )
		goto st81;
	goto st0;
st81:
	p += 1;
case 81:
	if ( (*p) == 47 )
		goto st82;
	goto st0;
st82:
	p += 1;
case 82:
	if ( (*p) == 47 )
		goto st83;
	goto st0;
st83:
	p += 1;
case 83:
	switch( (*p) ) {
		case 37: goto tr0;
		case 126: goto tr3;
	}
	goto st0;
	}

	_out: {}
	}

#line 57 "lex/parser_path_template.rl"

    if (cs >= 84)
        return res;
    else
        return DirectoryTemplate::None;
}

} // namespace parser
} // namespace ceammc
