
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
static const int parser_path_template_first_final = 74;
static const int parser_path_template_error = 0;

static const int parser_path_template_en_main = 1;


#line 36 "lex/parser_path_template.rl"


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

#line 50 "lex/parser_path_template.rl"
    
#line 45 "lex/parser_path_template.cpp"
	{
	switch ( cs )
	{
case 1:
	switch( (*p) ) {
		case 37: goto tr0;
		case 102: goto st67;
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
		case 65: goto st3;
		case 68: goto st10;
		case 72: goto st35;
		case 73: goto st40;
		case 77: goto st46;
		case 80: goto st56;
		case 86: goto st63;
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
		case 0: goto tr17;
		case 48: goto st0;
	}
	goto tr16;
tr16:
#line 19 "lex/parser_path_template.rl"
	{ res = DirectoryTemplate::Audio; }
#line 16 "lex/parser_path_template.rl"
	{ if(end)   *end = p; }
	goto st9;
tr28:
#line 23 "lex/parser_path_template.rl"
	{ res = DirectoryTemplate::Desktop; }
#line 16 "lex/parser_path_template.rl"
	{ if(end)   *end = p; }
	goto st9;
tr34:
#line 22 "lex/parser_path_template.rl"
	{ res = DirectoryTemplate::Document; }
#line 16 "lex/parser_path_template.rl"
	{ if(end)   *end = p; }
	goto st9;
tr48:
#line 24 "lex/parser_path_template.rl"
	{ res = DirectoryTemplate::Download; }
#line 16 "lex/parser_path_template.rl"
	{ if(end)   *end = p; }
	goto st9;
tr54:
#line 18 "lex/parser_path_template.rl"
	{ res = DirectoryTemplate::Home; }
#line 16 "lex/parser_path_template.rl"
	{ if(end)   *end = p; }
	goto st9;
tr61:
#line 21 "lex/parser_path_template.rl"
	{ res = DirectoryTemplate::Picture; }
#line 16 "lex/parser_path_template.rl"
	{ if(end)   *end = p; }
	goto st9;
tr70:
#line 20 "lex/parser_path_template.rl"
	{ res = DirectoryTemplate::Video; }
#line 16 "lex/parser_path_template.rl"
	{ if(end)   *end = p; }
	goto st9;
st9:
	p += 1;
case 9:
#line 160 "lex/parser_path_template.cpp"
	switch( (*p) ) {
		case 0: goto tr19;
		case 48: goto st0;
	}
	goto st9;
tr17:
#line 19 "lex/parser_path_template.rl"
	{ res = DirectoryTemplate::Audio; }
#line 16 "lex/parser_path_template.rl"
	{ if(end)   *end = p; }
#line 33 "lex/parser_path_template.rl"
	{ {p++; cs = 74; goto _out;} }
	goto st74;
tr19:
#line 33 "lex/parser_path_template.rl"
	{ {p++; cs = 74; goto _out;} }
	goto st74;
tr29:
#line 23 "lex/parser_path_template.rl"
	{ res = DirectoryTemplate::Desktop; }
#line 16 "lex/parser_path_template.rl"
	{ if(end)   *end = p; }
#line 33 "lex/parser_path_template.rl"
	{ {p++; cs = 74; goto _out;} }
	goto st74;
tr35:
#line 22 "lex/parser_path_template.rl"
	{ res = DirectoryTemplate::Document; }
#line 16 "lex/parser_path_template.rl"
	{ if(end)   *end = p; }
#line 33 "lex/parser_path_template.rl"
	{ {p++; cs = 74; goto _out;} }
	goto st74;
tr49:
#line 24 "lex/parser_path_template.rl"
	{ res = DirectoryTemplate::Download; }
#line 16 "lex/parser_path_template.rl"
	{ if(end)   *end = p; }
#line 33 "lex/parser_path_template.rl"
	{ {p++; cs = 74; goto _out;} }
	goto st74;
tr55:
#line 18 "lex/parser_path_template.rl"
	{ res = DirectoryTemplate::Home; }
#line 16 "lex/parser_path_template.rl"
	{ if(end)   *end = p; }
#line 33 "lex/parser_path_template.rl"
	{ {p++; cs = 74; goto _out;} }
	goto st74;
tr62:
#line 21 "lex/parser_path_template.rl"
	{ res = DirectoryTemplate::Picture; }
#line 16 "lex/parser_path_template.rl"
	{ if(end)   *end = p; }
#line 33 "lex/parser_path_template.rl"
	{ {p++; cs = 74; goto _out;} }
	goto st74;
tr71:
#line 20 "lex/parser_path_template.rl"
	{ res = DirectoryTemplate::Video; }
#line 16 "lex/parser_path_template.rl"
	{ if(end)   *end = p; }
#line 33 "lex/parser_path_template.rl"
	{ {p++; cs = 74; goto _out;} }
	goto st74;
st74:
	p += 1;
case 74:
#line 229 "lex/parser_path_template.cpp"
	switch( (*p) ) {
		case 0: goto tr19;
		case 48: goto st0;
	}
	goto st9;
st10:
	p += 1;
case 10:
	switch( (*p) ) {
		case 69: goto st11;
		case 79: goto st18;
	}
	goto st0;
st11:
	p += 1;
case 11:
	if ( (*p) == 83 )
		goto st12;
	goto st0;
st12:
	p += 1;
case 12:
	if ( (*p) == 75 )
		goto st13;
	goto st0;
st13:
	p += 1;
case 13:
	if ( (*p) == 84 )
		goto st14;
	goto st0;
st14:
	p += 1;
case 14:
	if ( (*p) == 79 )
		goto st15;
	goto st0;
st15:
	p += 1;
case 15:
	if ( (*p) == 80 )
		goto st16;
	goto st0;
st16:
	p += 1;
case 16:
	if ( (*p) == 37 )
		goto st17;
	goto st0;
st17:
	p += 1;
case 17:
	switch( (*p) ) {
		case 0: goto tr29;
		case 48: goto st0;
	}
	goto tr28;
st18:
	p += 1;
case 18:
	switch( (*p) ) {
		case 67: goto st19;
		case 87: goto st27;
	}
	goto st0;
st19:
	p += 1;
case 19:
	switch( (*p) ) {
		case 37: goto st20;
		case 85: goto st21;
	}
	goto st0;
st20:
	p += 1;
case 20:
	switch( (*p) ) {
		case 0: goto tr35;
		case 48: goto st0;
	}
	goto tr34;
st21:
	p += 1;
case 21:
	if ( (*p) == 77 )
		goto st22;
	goto st0;
st22:
	p += 1;
case 22:
	if ( (*p) == 69 )
		goto st23;
	goto st0;
st23:
	p += 1;
case 23:
	if ( (*p) == 78 )
		goto st24;
	goto st0;
st24:
	p += 1;
case 24:
	if ( (*p) == 84 )
		goto st25;
	goto st0;
st25:
	p += 1;
case 25:
	switch( (*p) ) {
		case 37: goto st20;
		case 83: goto st26;
	}
	goto st0;
st26:
	p += 1;
case 26:
	if ( (*p) == 37 )
		goto st20;
	goto st0;
st27:
	p += 1;
case 27:
	if ( (*p) == 78 )
		goto st28;
	goto st0;
st28:
	p += 1;
case 28:
	if ( (*p) == 76 )
		goto st29;
	goto st0;
st29:
	p += 1;
case 29:
	if ( (*p) == 79 )
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
	if ( (*p) == 68 )
		goto st32;
	goto st0;
st32:
	p += 1;
case 32:
	switch( (*p) ) {
		case 37: goto st33;
		case 83: goto st34;
	}
	goto st0;
st33:
	p += 1;
case 33:
	switch( (*p) ) {
		case 0: goto tr49;
		case 48: goto st0;
	}
	goto tr48;
st34:
	p += 1;
case 34:
	if ( (*p) == 37 )
		goto st33;
	goto st0;
st35:
	p += 1;
case 35:
	if ( (*p) == 79 )
		goto st36;
	goto st0;
st36:
	p += 1;
case 36:
	if ( (*p) == 77 )
		goto st37;
	goto st0;
st37:
	p += 1;
case 37:
	if ( (*p) == 69 )
		goto st38;
	goto st0;
st38:
	p += 1;
case 38:
	if ( (*p) == 37 )
		goto st39;
	goto st0;
tr3:
#line 15 "lex/parser_path_template.rl"
	{ if(begin) *begin = p; }
	goto st39;
st39:
	p += 1;
case 39:
#line 432 "lex/parser_path_template.cpp"
	switch( (*p) ) {
		case 0: goto tr55;
		case 48: goto st0;
	}
	goto tr54;
st40:
	p += 1;
case 40:
	if ( (*p) == 77 )
		goto st41;
	goto st0;
st41:
	p += 1;
case 41:
	if ( (*p) == 65 )
		goto st42;
	goto st0;
st42:
	p += 1;
case 42:
	if ( (*p) == 71 )
		goto st43;
	goto st0;
st43:
	p += 1;
case 43:
	if ( (*p) == 69 )
		goto st44;
	goto st0;
st44:
	p += 1;
case 44:
	if ( (*p) == 37 )
		goto st45;
	goto st0;
st45:
	p += 1;
case 45:
	switch( (*p) ) {
		case 0: goto tr62;
		case 48: goto st0;
	}
	goto tr61;
st46:
	p += 1;
case 46:
	switch( (*p) ) {
		case 79: goto st47;
		case 85: goto st53;
	}
	goto st0;
st47:
	p += 1;
case 47:
	if ( (*p) == 86 )
		goto st48;
	goto st0;
st48:
	p += 1;
case 48:
	if ( (*p) == 73 )
		goto st49;
	goto st0;
st49:
	p += 1;
case 49:
	if ( (*p) == 69 )
		goto st50;
	goto st0;
st50:
	p += 1;
case 50:
	switch( (*p) ) {
		case 37: goto st51;
		case 83: goto st52;
	}
	goto st0;
st51:
	p += 1;
case 51:
	switch( (*p) ) {
		case 0: goto tr71;
		case 48: goto st0;
	}
	goto tr70;
st52:
	p += 1;
case 52:
	if ( (*p) == 37 )
		goto st51;
	goto st0;
st53:
	p += 1;
case 53:
	if ( (*p) == 83 )
		goto st54;
	goto st0;
st54:
	p += 1;
case 54:
	if ( (*p) == 73 )
		goto st55;
	goto st0;
st55:
	p += 1;
case 55:
	if ( (*p) == 67 )
		goto st7;
	goto st0;
st56:
	p += 1;
case 56:
	if ( (*p) == 73 )
		goto st57;
	goto st0;
st57:
	p += 1;
case 57:
	if ( (*p) == 67 )
		goto st58;
	goto st0;
st58:
	p += 1;
case 58:
	if ( (*p) == 84 )
		goto st59;
	goto st0;
st59:
	p += 1;
case 59:
	if ( (*p) == 85 )
		goto st60;
	goto st0;
st60:
	p += 1;
case 60:
	if ( (*p) == 82 )
		goto st61;
	goto st0;
st61:
	p += 1;
case 61:
	if ( (*p) == 69 )
		goto st62;
	goto st0;
st62:
	p += 1;
case 62:
	switch( (*p) ) {
		case 37: goto st45;
		case 83: goto st44;
	}
	goto st0;
st63:
	p += 1;
case 63:
	if ( (*p) == 73 )
		goto st64;
	goto st0;
st64:
	p += 1;
case 64:
	if ( (*p) == 68 )
		goto st65;
	goto st0;
st65:
	p += 1;
case 65:
	if ( (*p) == 69 )
		goto st66;
	goto st0;
st66:
	p += 1;
case 66:
	if ( (*p) == 79 )
		goto st52;
	goto st0;
st67:
	p += 1;
case 67:
	if ( (*p) == 105 )
		goto st68;
	goto st0;
st68:
	p += 1;
case 68:
	if ( (*p) == 108 )
		goto st69;
	goto st0;
st69:
	p += 1;
case 69:
	if ( (*p) == 101 )
		goto st70;
	goto st0;
st70:
	p += 1;
case 70:
	if ( (*p) == 58 )
		goto st71;
	goto st0;
st71:
	p += 1;
case 71:
	if ( (*p) == 47 )
		goto st72;
	goto st0;
st72:
	p += 1;
case 72:
	if ( (*p) == 47 )
		goto st73;
	goto st0;
st73:
	p += 1;
case 73:
	switch( (*p) ) {
		case 37: goto tr0;
		case 126: goto tr3;
	}
	goto st0;
	}

	_out: {}
	}

#line 51 "lex/parser_path_template.rl"

    if (cs >= 74)
        return res;
    else
        return DirectoryTemplate::None;
}

} // namespace parser
} // namespace ceammc
