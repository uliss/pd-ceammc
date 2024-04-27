
#line 1 "lex/parser_dict_expr.rl"
# include "parser_dict_expr.h"
# include "ragel_common.h"
# include "parser_numeric.h"

# include <cstring>

namespace ceammc {
namespace parser {


#line 14 "lex/parser_dict_expr.cpp"
static const int dict_expr_start = 1;
static const int dict_expr_first_final = 12;
static const int dict_expr_error = 0;

static const int dict_expr_en_main = 1;


#line 71 "lex/parser_dict_expr.rl"


bool parse_dict_expr(const char* expr, std::vector<DictExprMatcher>* matches)
{
    int cs = 0;
    const char* p = expr;
    const char* pe = p + strlen(expr);
    const char* eof = pe;

    DECLARE_RAGEL_COMMON_VARS;
    DECLARE_RAGEL_NUMERIC_VARS;

    const char* key_start = nullptr;
    DictExprMatcher match;

    
#line 39 "lex/parser_dict_expr.cpp"
	{
	cs = dict_expr_start;
	}

#line 87 "lex/parser_dict_expr.rl"
    
#line 46 "lex/parser_dict_expr.cpp"
	{
	if ( p == pe )
		goto _test_eof;
	switch ( cs )
	{
case 1:
	if ( (*p) == 46 )
		goto st12;
	goto st0;
st0:
cs = 0;
	goto _out;
tr20:
#line 26 "lex/parser_dict_expr.rl"
	{ match.type = DictExprMatchType::ANY; }
#line 62 "lex/parser_dict_expr.rl"
	{
                if (matches) {
                    matches->push_back(match);
                    match = {};
                    key_start = nullptr;
                }
             }
	goto st12;
tr23:
#line 16 "lex/parser_dict_expr.rl"
	{
        if (key_start) {
            auto key_end = &(*p);
            match.key_name.assign(key_start, key_end - key_start);

            printf(" key: %s\n", match.key_name.c_str());
            match.type = DictExprMatchType::DICT;
        }
    }
#line 62 "lex/parser_dict_expr.rl"
	{
                if (matches) {
                    matches->push_back(match);
                    match = {};
                    key_start = nullptr;
                }
             }
	goto st12;
tr25:
#line 62 "lex/parser_dict_expr.rl"
	{
                if (matches) {
                    matches->push_back(match);
                    match = {};
                    key_start = nullptr;
                }
             }
	goto st12;
st12:
	if ( ++p == pe )
		goto _test_eof12;
case 12:
#line 105 "lex/parser_dict_expr.cpp"
	switch( (*p) ) {
		case 46: goto tr20;
		case 91: goto st2;
		case 95: goto tr21;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr21;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr21;
	} else
		goto tr21;
	goto st0;
tr21:
#line 26 "lex/parser_dict_expr.rl"
	{ match.type = DictExprMatchType::ANY; }
#line 15 "lex/parser_dict_expr.rl"
	{ key_start = &(*p); }
	goto st13;
st13:
	if ( ++p == pe )
		goto _test_eof13;
case 13:
#line 130 "lex/parser_dict_expr.cpp"
	switch( (*p) ) {
		case 46: goto tr23;
		case 95: goto st13;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st13;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st13;
	} else
		goto st13;
	goto st0;
st2:
	if ( ++p == pe )
		goto _test_eof2;
case 2:
	switch( (*p) ) {
		case 34: goto tr2;
		case 43: goto tr3;
		case 45: goto tr3;
		case 48: goto tr4;
		case 93: goto tr6;
	}
	if ( 49 <= (*p) && (*p) <= 57 )
		goto tr5;
	goto st0;
tr2:
#line 26 "lex/parser_dict_expr.rl"
	{ match.type = DictExprMatchType::ANY; }
	goto st3;
st3:
	if ( ++p == pe )
		goto _test_eof3;
case 3:
#line 166 "lex/parser_dict_expr.cpp"
	switch( (*p) ) {
		case 34: goto st0;
		case 91: goto st0;
		case 93: goto st0;
	}
	if ( 40 <= (*p) && (*p) <= 41 )
		goto st0;
	goto tr7;
tr7:
#line 15 "lex/parser_dict_expr.rl"
	{ key_start = &(*p); }
	goto st4;
st4:
	if ( ++p == pe )
		goto _test_eof4;
case 4:
#line 183 "lex/parser_dict_expr.cpp"
	switch( (*p) ) {
		case 34: goto tr9;
		case 91: goto st0;
		case 93: goto st0;
	}
	if ( 40 <= (*p) && (*p) <= 41 )
		goto st0;
	goto st4;
tr9:
#line 16 "lex/parser_dict_expr.rl"
	{
        if (key_start) {
            auto key_end = &(*p);
            match.key_name.assign(key_start, key_end - key_start);

            printf(" key: %s\n", match.key_name.c_str());
            match.type = DictExprMatchType::DICT;
        }
    }
	goto st5;
st5:
	if ( ++p == pe )
		goto _test_eof5;
case 5:
#line 208 "lex/parser_dict_expr.cpp"
	if ( (*p) == 93 )
		goto st14;
	goto st0;
tr6:
#line 26 "lex/parser_dict_expr.rl"
	{ match.type = DictExprMatchType::ANY; }
	goto st14;
tr14:
#line 30 "lex/ragel_numeric.rl"
	{
        ragel_num.vint *= ragel_num.sign;
        ragel_type = TYPE_INT;
        ragel_cat  = CAT_NUMBER;
    }
#line 28 "lex/parser_dict_expr.rl"
	{
        match.array_slice_begin = ragel_num.getInteger();
        match.array_slice_length = ARRAY_SLICE_LENGTH_END;
        ragel_num = {};
    }
#line 46 "lex/parser_dict_expr.rl"
	{ match.array_slice_length = 1; }
#line 35 "lex/parser_dict_expr.rl"
	{
        match.type = DictExprMatchType::LIST;
        if (match.array_slice_length == ARRAY_SLICE_LENGTH_END)
            printf("array range: %lld:END\n", match.array_slice_begin);
        else if (match.array_slice_length == ARRAY_SLICE_LENGTH_NONE)
            printf("array range: %lld:?\n", match.array_slice_begin);
        else
            printf("array range: %lld:%lld\n", match.array_slice_begin, match.array_slice_length);
    }
	goto st14;
tr17:
#line 34 "lex/parser_dict_expr.rl"
	{ match.array_slice_length = ARRAY_SLICE_LENGTH_END; }
#line 35 "lex/parser_dict_expr.rl"
	{
        match.type = DictExprMatchType::LIST;
        if (match.array_slice_length == ARRAY_SLICE_LENGTH_END)
            printf("array range: %lld:END\n", match.array_slice_begin);
        else if (match.array_slice_length == ARRAY_SLICE_LENGTH_NONE)
            printf("array range: %lld:?\n", match.array_slice_begin);
        else
            printf("array range: %lld:%lld\n", match.array_slice_begin, match.array_slice_length);
    }
	goto st14;
tr18:
#line 30 "lex/ragel_numeric.rl"
	{
        ragel_num.vint *= ragel_num.sign;
        ragel_type = TYPE_INT;
        ragel_cat  = CAT_NUMBER;
    }
#line 33 "lex/parser_dict_expr.rl"
	{ match.array_slice_length = ragel_num.getInteger(); ragel_num = {}; }
#line 35 "lex/parser_dict_expr.rl"
	{
        match.type = DictExprMatchType::LIST;
        if (match.array_slice_length == ARRAY_SLICE_LENGTH_END)
            printf("array range: %lld:END\n", match.array_slice_begin);
        else if (match.array_slice_length == ARRAY_SLICE_LENGTH_NONE)
            printf("array range: %lld:?\n", match.array_slice_begin);
        else
            printf("array range: %lld:%lld\n", match.array_slice_begin, match.array_slice_length);
    }
	goto st14;
st14:
	if ( ++p == pe )
		goto _test_eof14;
case 14:
#line 280 "lex/parser_dict_expr.cpp"
	if ( (*p) == 46 )
		goto tr25;
	goto st0;
tr3:
#line 26 "lex/parser_dict_expr.rl"
	{ match.type = DictExprMatchType::ANY; }
#line 21 "lex/ragel_numeric.rl"
	{ ragel_num.sign = ((*p)=='-') ? -1 : 1; }
	goto st6;
st6:
	if ( ++p == pe )
		goto _test_eof6;
case 6:
#line 294 "lex/parser_dict_expr.cpp"
	if ( (*p) == 48 )
		goto tr11;
	if ( 49 <= (*p) && (*p) <= 57 )
		goto tr12;
	goto st0;
tr4:
#line 26 "lex/parser_dict_expr.rl"
	{ match.type = DictExprMatchType::ANY; }
#line 29 "lex/ragel_numeric.rl"
	{ (ragel_num.vint *= 10) += ((*p)-'0'); }
	goto st7;
tr11:
#line 29 "lex/ragel_numeric.rl"
	{ (ragel_num.vint *= 10) += ((*p)-'0'); }
	goto st7;
st7:
	if ( ++p == pe )
		goto _test_eof7;
case 7:
#line 314 "lex/parser_dict_expr.cpp"
	switch( (*p) ) {
		case 58: goto tr13;
		case 93: goto tr14;
	}
	goto st0;
tr13:
#line 30 "lex/ragel_numeric.rl"
	{
        ragel_num.vint *= ragel_num.sign;
        ragel_type = TYPE_INT;
        ragel_cat  = CAT_NUMBER;
    }
#line 28 "lex/parser_dict_expr.rl"
	{
        match.array_slice_begin = ragel_num.getInteger();
        match.array_slice_length = ARRAY_SLICE_LENGTH_END;
        ragel_num = {};
    }
	goto st8;
st8:
	if ( ++p == pe )
		goto _test_eof8;
case 8:
#line 338 "lex/parser_dict_expr.cpp"
	switch( (*p) ) {
		case 48: goto tr15;
		case 93: goto tr17;
	}
	if ( 49 <= (*p) && (*p) <= 57 )
		goto tr16;
	goto st0;
tr15:
#line 34 "lex/parser_dict_expr.rl"
	{ match.array_slice_length = ARRAY_SLICE_LENGTH_END; }
#line 29 "lex/ragel_numeric.rl"
	{ (ragel_num.vint *= 10) += ((*p)-'0'); }
	goto st9;
st9:
	if ( ++p == pe )
		goto _test_eof9;
case 9:
#line 356 "lex/parser_dict_expr.cpp"
	if ( (*p) == 93 )
		goto tr18;
	goto st0;
tr19:
#line 29 "lex/ragel_numeric.rl"
	{ (ragel_num.vint *= 10) += ((*p)-'0'); }
	goto st10;
tr16:
#line 34 "lex/parser_dict_expr.rl"
	{ match.array_slice_length = ARRAY_SLICE_LENGTH_END; }
#line 29 "lex/ragel_numeric.rl"
	{ (ragel_num.vint *= 10) += ((*p)-'0'); }
	goto st10;
st10:
	if ( ++p == pe )
		goto _test_eof10;
case 10:
#line 374 "lex/parser_dict_expr.cpp"
	if ( (*p) == 93 )
		goto tr18;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr19;
	goto st0;
tr5:
#line 26 "lex/parser_dict_expr.rl"
	{ match.type = DictExprMatchType::ANY; }
#line 29 "lex/ragel_numeric.rl"
	{ (ragel_num.vint *= 10) += ((*p)-'0'); }
	goto st11;
tr12:
#line 29 "lex/ragel_numeric.rl"
	{ (ragel_num.vint *= 10) += ((*p)-'0'); }
	goto st11;
st11:
	if ( ++p == pe )
		goto _test_eof11;
case 11:
#line 394 "lex/parser_dict_expr.cpp"
	switch( (*p) ) {
		case 58: goto tr13;
		case 93: goto tr14;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr12;
	goto st0;
	}
	_test_eof12: cs = 12; goto _test_eof; 
	_test_eof13: cs = 13; goto _test_eof; 
	_test_eof2: cs = 2; goto _test_eof; 
	_test_eof3: cs = 3; goto _test_eof; 
	_test_eof4: cs = 4; goto _test_eof; 
	_test_eof5: cs = 5; goto _test_eof; 
	_test_eof14: cs = 14; goto _test_eof; 
	_test_eof6: cs = 6; goto _test_eof; 
	_test_eof7: cs = 7; goto _test_eof; 
	_test_eof8: cs = 8; goto _test_eof; 
	_test_eof9: cs = 9; goto _test_eof; 
	_test_eof10: cs = 10; goto _test_eof; 
	_test_eof11: cs = 11; goto _test_eof; 

	_test_eof: {}
	if ( p == eof )
	{
	switch ( cs ) {
	case 14: 
#line 62 "lex/parser_dict_expr.rl"
	{
                if (matches) {
                    matches->push_back(match);
                    match = {};
                    key_start = nullptr;
                }
             }
	break;
	case 13: 
#line 16 "lex/parser_dict_expr.rl"
	{
        if (key_start) {
            auto key_end = &(*p);
            match.key_name.assign(key_start, key_end - key_start);

            printf(" key: %s\n", match.key_name.c_str());
            match.type = DictExprMatchType::DICT;
        }
    }
#line 62 "lex/parser_dict_expr.rl"
	{
                if (matches) {
                    matches->push_back(match);
                    match = {};
                    key_start = nullptr;
                }
             }
	break;
	case 12: 
#line 26 "lex/parser_dict_expr.rl"
	{ match.type = DictExprMatchType::ANY; }
#line 62 "lex/parser_dict_expr.rl"
	{
                if (matches) {
                    matches->push_back(match);
                    match = {};
                    key_start = nullptr;
                }
             }
	break;
#line 463 "lex/parser_dict_expr.cpp"
	}
	}

	_out: {}
	}

#line 88 "lex/parser_dict_expr.rl"

    const auto ok = cs >= 12;

    return ok;
}

}
}
