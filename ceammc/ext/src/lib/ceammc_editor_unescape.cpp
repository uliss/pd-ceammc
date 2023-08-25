
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


#line 301 "ceammc_editor_unescape.cpp"
static const int editor_unescape_data_start = 0;
static const int editor_unescape_data_first_final = 0;
static const int editor_unescape_data_error = -1;

static const int editor_unescape_data_en_main = 0;


#line 66 "ceammc_editor_unescape.rl"


bool editor_string_unescape_data(std::string& str)
{
    int cs = 0;
    char* p = const_cast<char*>(str.data());
    auto pe = p + str.length();
    auto eof = pe;

    
#line 320 "ceammc_editor_unescape.cpp"
	{
	cs = editor_unescape_data_start;
	}

#line 76 "ceammc_editor_unescape.rl"
    
#line 327 "ceammc_editor_unescape.cpp"
	{
	if ( p == pe )
		goto _test_eof;
	switch ( cs )
	{
tr4:
#line 58 "ceammc_editor_unescape.rl"
	{ p[-2] = ';';  p[-1] = REPLACE_CHAR; }
	goto st0;
tr10:
#line 59 "ceammc_editor_unescape.rl"
	{ p[-2] = '$';  p[-1] = REPLACE_CHAR; }
	goto st0;
tr16:
#line 56 "ceammc_editor_unescape.rl"
	{ p[-2] = '{';  p[-1] = REPLACE_CHAR; }
	goto st0;
tr20:
#line 57 "ceammc_editor_unescape.rl"
	{ p[-2] = '}';  p[-1] = REPLACE_CHAR; }
	goto st0;
tr24:
#line 55 "ceammc_editor_unescape.rl"
	{ p[-2] = ',';  p[-1] = REPLACE_CHAR; }
	goto st0;
st0:
	if ( ++p == pe )
		goto _test_eof0;
case 0:
#line 357 "ceammc_editor_unescape.cpp"
	switch( (*p) ) {
		case 59: goto st1;
		case 92: goto st2;
		case 96: goto st4;
	}
	goto st0;
tr5:
#line 58 "ceammc_editor_unescape.rl"
	{ p[-2] = ';';  p[-1] = REPLACE_CHAR; }
	goto st1;
tr11:
#line 59 "ceammc_editor_unescape.rl"
	{ p[-2] = '$';  p[-1] = REPLACE_CHAR; }
	goto st1;
tr17:
#line 56 "ceammc_editor_unescape.rl"
	{ p[-2] = '{';  p[-1] = REPLACE_CHAR; }
	goto st1;
tr21:
#line 57 "ceammc_editor_unescape.rl"
	{ p[-2] = '}';  p[-1] = REPLACE_CHAR; }
	goto st1;
tr25:
#line 55 "ceammc_editor_unescape.rl"
	{ p[-2] = ',';  p[-1] = REPLACE_CHAR; }
	goto st1;
st1:
	if ( ++p == pe )
		goto _test_eof1;
case 1:
#line 388 "ceammc_editor_unescape.cpp"
	switch( (*p) ) {
		case 59: goto tr5;
		case 92: goto tr6;
		case 96: goto tr7;
	}
	goto tr4;
tr6:
#line 58 "ceammc_editor_unescape.rl"
	{ p[-2] = ';';  p[-1] = REPLACE_CHAR; }
	goto st2;
tr12:
#line 59 "ceammc_editor_unescape.rl"
	{ p[-2] = '$';  p[-1] = REPLACE_CHAR; }
	goto st2;
tr18:
#line 56 "ceammc_editor_unescape.rl"
	{ p[-2] = '{';  p[-1] = REPLACE_CHAR; }
	goto st2;
tr22:
#line 57 "ceammc_editor_unescape.rl"
	{ p[-2] = '}';  p[-1] = REPLACE_CHAR; }
	goto st2;
tr26:
#line 55 "ceammc_editor_unescape.rl"
	{ p[-2] = ',';  p[-1] = REPLACE_CHAR; }
	goto st2;
st2:
	if ( ++p == pe )
		goto _test_eof2;
case 2:
#line 419 "ceammc_editor_unescape.cpp"
	switch( (*p) ) {
		case 36: goto st3;
		case 44: goto st7;
		case 59: goto st1;
		case 92: goto st2;
		case 96: goto st4;
	}
	goto st0;
st3:
	if ( ++p == pe )
		goto _test_eof3;
case 3:
	switch( (*p) ) {
		case 59: goto tr11;
		case 92: goto tr12;
		case 96: goto tr13;
	}
	goto tr10;
tr7:
#line 58 "ceammc_editor_unescape.rl"
	{ p[-2] = ';';  p[-1] = REPLACE_CHAR; }
	goto st4;
tr13:
#line 59 "ceammc_editor_unescape.rl"
	{ p[-2] = '$';  p[-1] = REPLACE_CHAR; }
	goto st4;
tr19:
#line 56 "ceammc_editor_unescape.rl"
	{ p[-2] = '{';  p[-1] = REPLACE_CHAR; }
	goto st4;
tr23:
#line 57 "ceammc_editor_unescape.rl"
	{ p[-2] = '}';  p[-1] = REPLACE_CHAR; }
	goto st4;
tr27:
#line 55 "ceammc_editor_unescape.rl"
	{ p[-2] = ',';  p[-1] = REPLACE_CHAR; }
	goto st4;
st4:
	if ( ++p == pe )
		goto _test_eof4;
case 4:
#line 462 "ceammc_editor_unescape.cpp"
	switch( (*p) ) {
		case 40: goto st5;
		case 41: goto st6;
		case 59: goto st1;
		case 92: goto st2;
		case 96: goto st4;
	}
	goto st0;
st5:
	if ( ++p == pe )
		goto _test_eof5;
case 5:
	switch( (*p) ) {
		case 59: goto tr17;
		case 92: goto tr18;
		case 96: goto tr19;
	}
	goto tr16;
st6:
	if ( ++p == pe )
		goto _test_eof6;
case 6:
	switch( (*p) ) {
		case 59: goto tr21;
		case 92: goto tr22;
		case 96: goto tr23;
	}
	goto tr20;
st7:
	if ( ++p == pe )
		goto _test_eof7;
case 7:
	switch( (*p) ) {
		case 59: goto tr25;
		case 92: goto tr26;
		case 96: goto tr27;
	}
	goto tr24;
	}
	_test_eof0: cs = 0; goto _test_eof; 
	_test_eof1: cs = 1; goto _test_eof; 
	_test_eof2: cs = 2; goto _test_eof; 
	_test_eof3: cs = 3; goto _test_eof; 
	_test_eof4: cs = 4; goto _test_eof; 
	_test_eof5: cs = 5; goto _test_eof; 
	_test_eof6: cs = 6; goto _test_eof; 
	_test_eof7: cs = 7; goto _test_eof; 

	_test_eof: {}
	if ( p == eof )
	{
	switch ( cs ) {
	case 7: 
#line 55 "ceammc_editor_unescape.rl"
	{ p[-2] = ',';  p[-1] = REPLACE_CHAR; }
	break;
	case 5: 
#line 56 "ceammc_editor_unescape.rl"
	{ p[-2] = '{';  p[-1] = REPLACE_CHAR; }
	break;
	case 6: 
#line 57 "ceammc_editor_unescape.rl"
	{ p[-2] = '}';  p[-1] = REPLACE_CHAR; }
	break;
	case 1: 
#line 58 "ceammc_editor_unescape.rl"
	{ p[-2] = ';';  p[-1] = REPLACE_CHAR; }
	break;
	case 3: 
#line 59 "ceammc_editor_unescape.rl"
	{ p[-2] = '$';  p[-1] = REPLACE_CHAR; }
	break;
#line 535 "ceammc_editor_unescape.cpp"
	}
	}

	}

#line 77 "ceammc_editor_unescape.rl"

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
    case EditorEscapeMode::DATA:
        return editor_string_unescape_data(str);
    default:
        return false;
    }
}
}
