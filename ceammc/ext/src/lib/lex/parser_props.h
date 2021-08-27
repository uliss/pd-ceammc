
#line 1 "lex/parser_props.rl"
#ifndef PARSER_PROP_RAGEL_H_
#define PARSER_PROP_RAGEL_H_

# include <cstring>

namespace ceammc {
namespace parser {


#line 13 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_props.h"
static const int bool_prop_start = 1;
static const int bool_prop_first_final = 14;
static const int bool_prop_error = 0;

static const int bool_prop_en_main = 1;


#line 20 "lex/parser_props.rl"


enum class BoolPropValue {
    UNKNOWN,
    TRUE,
    FALSE,
    RANDOM,
    INVERT
};

static inline BoolPropValue parse_bool_prop(const char* str)
{
    const auto len = std::strlen(str);
    if (len == 0)
        return BoolPropValue::UNKNOWN;

    int cs = 0;
    const char* p = str;
    const char* pe = p + len;
    const char* eof = pe;
    BoolPropValue type = BoolPropValue::UNKNOWN;

    
#line 45 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_props.h"
	{
	cs = bool_prop_start;
	}

#line 43 "lex/parser_props.rl"
    
#line 52 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_props.h"
	{
	if ( p == pe )
		goto _test_eof;
	switch ( cs )
	{
case 1:
	switch( (*p) ) {
		case 33: goto st14;
		case 70: goto st2;
		case 84: goto st6;
		case 102: goto st2;
		case 114: goto st9;
		case 116: goto st6;
		case 126: goto st14;
	}
	goto st0;
st0:
cs = 0;
	goto _out;
st14:
	if ( ++p == pe )
		goto _test_eof14;
case 14:
	goto st0;
st2:
	if ( ++p == pe )
		goto _test_eof2;
case 2:
	if ( (*p) == 97 )
		goto st3;
	goto st0;
st3:
	if ( ++p == pe )
		goto _test_eof3;
case 3:
	if ( (*p) == 108 )
		goto st4;
	goto st0;
st4:
	if ( ++p == pe )
		goto _test_eof4;
case 4:
	if ( (*p) == 115 )
		goto st5;
	goto st0;
st5:
	if ( ++p == pe )
		goto _test_eof5;
case 5:
	if ( (*p) == 101 )
		goto st15;
	goto st0;
st15:
	if ( ++p == pe )
		goto _test_eof15;
case 15:
	goto st0;
st6:
	if ( ++p == pe )
		goto _test_eof6;
case 6:
	if ( (*p) == 114 )
		goto st7;
	goto st0;
st7:
	if ( ++p == pe )
		goto _test_eof7;
case 7:
	if ( (*p) == 117 )
		goto st8;
	goto st0;
st8:
	if ( ++p == pe )
		goto _test_eof8;
case 8:
	if ( (*p) == 101 )
		goto st16;
	goto st0;
st16:
	if ( ++p == pe )
		goto _test_eof16;
case 16:
	goto st0;
st9:
	if ( ++p == pe )
		goto _test_eof9;
case 9:
	if ( (*p) == 97 )
		goto st10;
	goto st0;
st10:
	if ( ++p == pe )
		goto _test_eof10;
case 10:
	if ( (*p) == 110 )
		goto st11;
	goto st0;
st11:
	if ( ++p == pe )
		goto _test_eof11;
case 11:
	if ( (*p) == 100 )
		goto st12;
	goto st0;
st12:
	if ( ++p == pe )
		goto _test_eof12;
case 12:
	if ( (*p) == 111 )
		goto st13;
	goto st0;
st13:
	if ( ++p == pe )
		goto _test_eof13;
case 13:
	if ( (*p) == 109 )
		goto st17;
	goto st0;
st17:
	if ( ++p == pe )
		goto _test_eof17;
case 17:
	goto st0;
	}
	_test_eof14: cs = 14; goto _test_eof; 
	_test_eof2: cs = 2; goto _test_eof; 
	_test_eof3: cs = 3; goto _test_eof; 
	_test_eof4: cs = 4; goto _test_eof; 
	_test_eof5: cs = 5; goto _test_eof; 
	_test_eof15: cs = 15; goto _test_eof; 
	_test_eof6: cs = 6; goto _test_eof; 
	_test_eof7: cs = 7; goto _test_eof; 
	_test_eof8: cs = 8; goto _test_eof; 
	_test_eof16: cs = 16; goto _test_eof; 
	_test_eof9: cs = 9; goto _test_eof; 
	_test_eof10: cs = 10; goto _test_eof; 
	_test_eof11: cs = 11; goto _test_eof; 
	_test_eof12: cs = 12; goto _test_eof; 
	_test_eof13: cs = 13; goto _test_eof; 
	_test_eof17: cs = 17; goto _test_eof; 

	_test_eof: {}
	if ( p == eof )
	{
	switch ( cs ) {
	case 16: 
#line 12 "lex/parser_props.rl"
	{ type = BoolPropValue::TRUE; }
	break;
	case 15: 
#line 13 "lex/parser_props.rl"
	{ type = BoolPropValue::FALSE; }
	break;
	case 17: 
#line 14 "lex/parser_props.rl"
	{ type = BoolPropValue::RANDOM; }
	break;
	case 14: 
#line 15 "lex/parser_props.rl"
	{ type = BoolPropValue::INVERT; }
	break;
#line 214 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_props.h"
	}
	}

	_out: {}
	}

#line 44 "lex/parser_props.rl"

    const bool ok = cs >= 14;
    if (ok)
        return type;
    else
        return BoolPropValue::UNKNOWN;
}


}
}

#endif // PARSER_PROP_RAGEL_H_


