
#line 1 "ceammc_editor_unescape.rl"
# include "ceammc_editor_object.h"

# include <algorithm>
# include <cstring>

constexpr const char REPLACE_CHAR = '\0';


#line 12 "ceammc_editor_unescape.cpp"
static const int editor_unescape_pd_start = 0;
static const int editor_unescape_pd_first_final = 0;
static const int editor_unescape_pd_error = -1;

static const int editor_unescape_pd_en_main = 0;


#line 24 "ceammc_editor_unescape.rl"


namespace ceammc {

bool editor_string_unescape_lua(std::string& str)
{
    int cs = 0;
    char* p = const_cast<char*>(str.data());
    auto pe = p + str.length();
    auto eof = pe;

    
#line 33 "ceammc_editor_unescape.cpp"
	{
	cs = editor_unescape_pd_start;
	}

#line 36 "ceammc_editor_unescape.rl"
    
#line 40 "ceammc_editor_unescape.cpp"
	{
	if ( p == pe )
		goto _test_eof;
	switch ( cs )
	{
tr9:
#line 11 "ceammc_editor_unescape.rl"
	{ p[-4] = '\t';  memset(p-3, REPLACE_CHAR, 3); }
	goto st0;
tr13:
#line 17 "ceammc_editor_unescape.rl"
	{ p[-4] = '$';   memset(p-3, REPLACE_CHAR, 3); }
	goto st0;
tr15:
#line 12 "ceammc_editor_unescape.rl"
	{ p[-4] = ',';   memset(p-3, REPLACE_CHAR, 3); }
	goto st0;
tr18:
#line 15 "ceammc_editor_unescape.rl"
	{ p[-4] = ';';   memset(p-3, REPLACE_CHAR, 3); }
	goto st0;
tr21:
#line 16 "ceammc_editor_unescape.rl"
	{ p[-4] = '\\';  memset(p-3, REPLACE_CHAR, 3); }
	goto st0;
tr25:
#line 13 "ceammc_editor_unescape.rl"
	{ p[-4] = '{';   memset(p-3, REPLACE_CHAR, 3); }
	goto st0;
tr27:
#line 14 "ceammc_editor_unescape.rl"
	{ p[-4] = '}';   memset(p-3, REPLACE_CHAR, 3); }
	goto st0;
st0:
	if ( ++p == pe )
		goto _test_eof0;
case 0:
#line 78 "ceammc_editor_unescape.cpp"
	if ( (*p) == 92 )
		goto st1;
	goto st0;
tr10:
#line 11 "ceammc_editor_unescape.rl"
	{ p[-4] = '\t';  memset(p-3, REPLACE_CHAR, 3); }
	goto st1;
tr14:
#line 17 "ceammc_editor_unescape.rl"
	{ p[-4] = '$';   memset(p-3, REPLACE_CHAR, 3); }
	goto st1;
tr16:
#line 12 "ceammc_editor_unescape.rl"
	{ p[-4] = ',';   memset(p-3, REPLACE_CHAR, 3); }
	goto st1;
tr19:
#line 15 "ceammc_editor_unescape.rl"
	{ p[-4] = ';';   memset(p-3, REPLACE_CHAR, 3); }
	goto st1;
tr22:
#line 16 "ceammc_editor_unescape.rl"
	{ p[-4] = '\\';  memset(p-3, REPLACE_CHAR, 3); }
	goto st1;
tr26:
#line 13 "ceammc_editor_unescape.rl"
	{ p[-4] = '{';   memset(p-3, REPLACE_CHAR, 3); }
	goto st1;
tr28:
#line 14 "ceammc_editor_unescape.rl"
	{ p[-4] = '}';   memset(p-3, REPLACE_CHAR, 3); }
	goto st1;
st1:
	if ( ++p == pe )
		goto _test_eof1;
case 1:
#line 114 "ceammc_editor_unescape.cpp"
	switch( (*p) ) {
		case 92: goto st1;
		case 120: goto st2;
	}
	goto st0;
st2:
	if ( ++p == pe )
		goto _test_eof2;
case 2:
	switch( (*p) ) {
		case 48: goto st3;
		case 50: goto st5;
		case 51: goto st8;
		case 53: goto st10;
		case 55: goto st12;
		case 92: goto st1;
	}
	goto st0;
st3:
	if ( ++p == pe )
		goto _test_eof3;
case 3:
	switch( (*p) ) {
		case 57: goto st4;
		case 92: goto st1;
	}
	goto st0;
st4:
	if ( ++p == pe )
		goto _test_eof4;
case 4:
	if ( (*p) == 92 )
		goto tr10;
	goto tr9;
st5:
	if ( ++p == pe )
		goto _test_eof5;
case 5:
	switch( (*p) ) {
		case 52: goto st6;
		case 92: goto st1;
		case 99: goto st7;
	}
	goto st0;
st6:
	if ( ++p == pe )
		goto _test_eof6;
case 6:
	if ( (*p) == 92 )
		goto tr14;
	goto tr13;
st7:
	if ( ++p == pe )
		goto _test_eof7;
case 7:
	if ( (*p) == 92 )
		goto tr16;
	goto tr15;
st8:
	if ( ++p == pe )
		goto _test_eof8;
case 8:
	switch( (*p) ) {
		case 92: goto st1;
		case 98: goto st9;
	}
	goto st0;
st9:
	if ( ++p == pe )
		goto _test_eof9;
case 9:
	if ( (*p) == 92 )
		goto tr19;
	goto tr18;
st10:
	if ( ++p == pe )
		goto _test_eof10;
case 10:
	switch( (*p) ) {
		case 92: goto st1;
		case 99: goto st11;
	}
	goto st0;
st11:
	if ( ++p == pe )
		goto _test_eof11;
case 11:
	if ( (*p) == 92 )
		goto tr22;
	goto tr21;
st12:
	if ( ++p == pe )
		goto _test_eof12;
case 12:
	switch( (*p) ) {
		case 92: goto st1;
		case 98: goto st13;
		case 100: goto st14;
	}
	goto st0;
st13:
	if ( ++p == pe )
		goto _test_eof13;
case 13:
	if ( (*p) == 92 )
		goto tr26;
	goto tr25;
st14:
	if ( ++p == pe )
		goto _test_eof14;
case 14:
	if ( (*p) == 92 )
		goto tr28;
	goto tr27;
	}
	_test_eof0: cs = 0; goto _test_eof; 
	_test_eof1: cs = 1; goto _test_eof; 
	_test_eof2: cs = 2; goto _test_eof; 
	_test_eof3: cs = 3; goto _test_eof; 
	_test_eof4: cs = 4; goto _test_eof; 
	_test_eof5: cs = 5; goto _test_eof; 
	_test_eof6: cs = 6; goto _test_eof; 
	_test_eof7: cs = 7; goto _test_eof; 
	_test_eof8: cs = 8; goto _test_eof; 
	_test_eof9: cs = 9; goto _test_eof; 
	_test_eof10: cs = 10; goto _test_eof; 
	_test_eof11: cs = 11; goto _test_eof; 
	_test_eof12: cs = 12; goto _test_eof; 
	_test_eof13: cs = 13; goto _test_eof; 
	_test_eof14: cs = 14; goto _test_eof; 

	_test_eof: {}
	if ( p == eof )
	{
	switch ( cs ) {
	case 4: 
#line 11 "ceammc_editor_unescape.rl"
	{ p[-4] = '\t';  memset(p-3, REPLACE_CHAR, 3); }
	break;
	case 7: 
#line 12 "ceammc_editor_unescape.rl"
	{ p[-4] = ',';   memset(p-3, REPLACE_CHAR, 3); }
	break;
	case 13: 
#line 13 "ceammc_editor_unescape.rl"
	{ p[-4] = '{';   memset(p-3, REPLACE_CHAR, 3); }
	break;
	case 14: 
#line 14 "ceammc_editor_unescape.rl"
	{ p[-4] = '}';   memset(p-3, REPLACE_CHAR, 3); }
	break;
	case 9: 
#line 15 "ceammc_editor_unescape.rl"
	{ p[-4] = ';';   memset(p-3, REPLACE_CHAR, 3); }
	break;
	case 11: 
#line 16 "ceammc_editor_unescape.rl"
	{ p[-4] = '\\';  memset(p-3, REPLACE_CHAR, 3); }
	break;
	case 6: 
#line 17 "ceammc_editor_unescape.rl"
	{ p[-4] = '$';   memset(p-3, REPLACE_CHAR, 3); }
	break;
#line 278 "ceammc_editor_unescape.cpp"
	}
	}

	}

#line 37 "ceammc_editor_unescape.rl"

    // algo:
    // replace all unescape gaps with REPLACE_CHAR (we replace 4 characters with 1)
    // remove all REPLACE_CHAR
    // resize string

    auto it = std::remove(str.begin(), str.end(), REPLACE_CHAR);
    str.erase(it, str.end());

    if (cs >= 0)
        return true;

    return false;
}

bool editor_string_unescape(std::string& str, EditorEscapeMode mode)
{
    switch(mode) {
    case EditorEscapeMode::LUA:
        return editor_string_unescape_lua(str);
    default:
        return false;
    }
}
}
