
#line 1 "lex/parser_dict_expr.rl"
# include "parser_dict_expr.h"
# include "ragel_common.h"
# include "parser_numeric.h"
# include "ceammc_string.h"

# include <cstring>

namespace ceammc {
namespace parser {


#line 15 "lex/parser_dict_expr.cpp"
static const int dict_expr_start = 1;
static const int dict_expr_first_final = 6;
static const int dict_expr_error = 0;

static const int dict_expr_en_main = 1;


#line 89 "lex/parser_dict_expr.rl"


bool parse_dict_match_expr(const char* expr, DictExprMatchList* matches)
{
    int cs = 0;
    const char* p = expr;
    const char* pe = p + strlen(expr);
    const char* eof = pe;

    DECLARE_RAGEL_COMMON_VARS;
    DECLARE_RAGEL_NUMERIC_VARS;

    const char* key_start = nullptr;
    DictExprMatcher match;

    
#line 40 "lex/parser_dict_expr.cpp"
	{
	cs = dict_expr_start;
	}

#line 105 "lex/parser_dict_expr.rl"
    
#line 47 "lex/parser_dict_expr.cpp"
	{
	if ( p == pe )
		goto _test_eof;
	switch ( cs )
	{
case 1:
	if ( (*p) == 47 )
		goto st2;
	goto st0;
st0:
cs = 0;
	goto _out;
tr13:
#line 59 "lex/parser_dict_expr.rl"
	{
        if (matches) {
            matches->push_back(match);
            match = {};
            key_start = nullptr;
        }
    }
	goto st2;
tr15:
#line 67 "lex/parser_dict_expr.rl"
	{
        match.no_error = true;
    }
#line 59 "lex/parser_dict_expr.rl"
	{
        if (matches) {
            matches->push_back(match);
            match = {};
            key_start = nullptr;
        }
    }
	goto st2;
tr16:
#line 38 "lex/parser_dict_expr.rl"
	{ match.type = DictExprMatchType::ANY; }
#line 59 "lex/parser_dict_expr.rl"
	{
        if (matches) {
            matches->push_back(match);
            match = {};
            key_start = nullptr;
        }
    }
	goto st2;
tr17:
#line 30 "lex/ragel_numeric.rl"
	{
        ragel_num.vint *= ragel_num.sign;
        ragel_type = TYPE_INT;
        ragel_cat  = CAT_NUMBER;
    }
#line 40 "lex/parser_dict_expr.rl"
	{
        match.array_slice_begin = ragel_num.getInteger();
        match.array_slice_length = ARRAY_SLICE_LENGTH_END;
        ragel_num = {};
    }
#line 72 "lex/parser_dict_expr.rl"
	{ match.array_slice_length = 1; }
#line 47 "lex/parser_dict_expr.rl"
	{
        match.type = DictExprMatchType::LIST;
#ifndef NDEBUG
        if (match.array_slice_length == ARRAY_SLICE_LENGTH_END)
            printf("array range: %lld:END\n", match.array_slice_begin);
        else if (match.array_slice_length == ARRAY_SLICE_LENGTH_NONE)
            printf("array range: %lld:?\n", match.array_slice_begin);
        else
            printf("array range: %lld:%lld\n", match.array_slice_begin, match.array_slice_length);
#endif
    }
#line 59 "lex/parser_dict_expr.rl"
	{
        if (matches) {
            matches->push_back(match);
            match = {};
            key_start = nullptr;
        }
    }
	goto st2;
tr20:
#line 46 "lex/parser_dict_expr.rl"
	{ match.array_slice_length = ARRAY_SLICE_LENGTH_END; }
#line 47 "lex/parser_dict_expr.rl"
	{
        match.type = DictExprMatchType::LIST;
#ifndef NDEBUG
        if (match.array_slice_length == ARRAY_SLICE_LENGTH_END)
            printf("array range: %lld:END\n", match.array_slice_begin);
        else if (match.array_slice_length == ARRAY_SLICE_LENGTH_NONE)
            printf("array range: %lld:?\n", match.array_slice_begin);
        else
            printf("array range: %lld:%lld\n", match.array_slice_begin, match.array_slice_length);
#endif
    }
#line 59 "lex/parser_dict_expr.rl"
	{
        if (matches) {
            matches->push_back(match);
            match = {};
            key_start = nullptr;
        }
    }
	goto st2;
tr24:
#line 30 "lex/ragel_numeric.rl"
	{
        ragel_num.vint *= ragel_num.sign;
        ragel_type = TYPE_INT;
        ragel_cat  = CAT_NUMBER;
    }
#line 45 "lex/parser_dict_expr.rl"
	{ match.array_slice_length = ragel_num.getInteger(); ragel_num = {}; }
#line 47 "lex/parser_dict_expr.rl"
	{
        match.type = DictExprMatchType::LIST;
#ifndef NDEBUG
        if (match.array_slice_length == ARRAY_SLICE_LENGTH_END)
            printf("array range: %lld:END\n", match.array_slice_begin);
        else if (match.array_slice_length == ARRAY_SLICE_LENGTH_NONE)
            printf("array range: %lld:?\n", match.array_slice_begin);
        else
            printf("array range: %lld:%lld\n", match.array_slice_begin, match.array_slice_length);
#endif
    }
#line 59 "lex/parser_dict_expr.rl"
	{
        if (matches) {
            matches->push_back(match);
            match = {};
            key_start = nullptr;
        }
    }
	goto st2;
tr27:
#line 17 "lex/parser_dict_expr.rl"
	{
        if (key_start) {
            constexpr int BUF_SIZE = 256;
            char buf[BUF_SIZE+1];

            auto str_len = std::min<int>(&(*p) - key_start, BUF_SIZE);
            int i;
            for (i = 0; i < str_len; i++) {
                buf[i] = key_start[i];
            }
            buf[i] = '\0'; // set c-string null byte
            match.key_name = gensym(buf);

#ifndef NDEBUG
            printf(" key: %s\n", match.key_name->s_name);
#endif

            match.type = DictExprMatchType::DICT;
        }
    }
#line 59 "lex/parser_dict_expr.rl"
	{
        if (matches) {
            matches->push_back(match);
            match = {};
            key_start = nullptr;
        }
    }
	goto st2;
st2:
	if ( ++p == pe )
		goto _test_eof2;
case 2:
#line 221 "lex/parser_dict_expr.cpp"
	switch( (*p) ) {
		case 34: goto tr2;
		case 42: goto st8;
		case 43: goto tr4;
		case 45: goto tr4;
		case 48: goto tr5;
		case 95: goto tr7;
	}
	if ( (*p) < 65 ) {
		if ( 49 <= (*p) && (*p) <= 57 )
			goto tr6;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr7;
	} else
		goto tr7;
	goto st0;
tr2:
#line 38 "lex/parser_dict_expr.rl"
	{ match.type = DictExprMatchType::ANY; }
	goto st3;
st3:
	if ( ++p == pe )
		goto _test_eof3;
case 3:
#line 247 "lex/parser_dict_expr.cpp"
	switch( (*p) ) {
		case 34: goto st0;
		case 91: goto st0;
		case 93: goto st0;
	}
	if ( 40 <= (*p) && (*p) <= 41 )
		goto st0;
	goto tr8;
tr8:
#line 16 "lex/parser_dict_expr.rl"
	{ key_start = &(*p); }
	goto st4;
st4:
	if ( ++p == pe )
		goto _test_eof4;
case 4:
#line 264 "lex/parser_dict_expr.cpp"
	switch( (*p) ) {
		case 34: goto tr10;
		case 91: goto st0;
		case 93: goto st0;
	}
	if ( 40 <= (*p) && (*p) <= 41 )
		goto st0;
	goto st4;
tr10:
#line 17 "lex/parser_dict_expr.rl"
	{
        if (key_start) {
            constexpr int BUF_SIZE = 256;
            char buf[BUF_SIZE+1];

            auto str_len = std::min<int>(&(*p) - key_start, BUF_SIZE);
            int i;
            for (i = 0; i < str_len; i++) {
                buf[i] = key_start[i];
            }
            buf[i] = '\0'; // set c-string null byte
            match.key_name = gensym(buf);

#ifndef NDEBUG
            printf(" key: %s\n", match.key_name->s_name);
#endif

            match.type = DictExprMatchType::DICT;
        }
    }
	goto st6;
st6:
	if ( ++p == pe )
		goto _test_eof6;
case 6:
#line 300 "lex/parser_dict_expr.cpp"
	switch( (*p) ) {
		case 47: goto tr13;
		case 63: goto st7;
	}
	goto st0;
tr29:
#line 17 "lex/parser_dict_expr.rl"
	{
        if (key_start) {
            constexpr int BUF_SIZE = 256;
            char buf[BUF_SIZE+1];

            auto str_len = std::min<int>(&(*p) - key_start, BUF_SIZE);
            int i;
            for (i = 0; i < str_len; i++) {
                buf[i] = key_start[i];
            }
            buf[i] = '\0'; // set c-string null byte
            match.key_name = gensym(buf);

#ifndef NDEBUG
            printf(" key: %s\n", match.key_name->s_name);
#endif

            match.type = DictExprMatchType::DICT;
        }
    }
	goto st7;
tr19:
#line 30 "lex/ragel_numeric.rl"
	{
        ragel_num.vint *= ragel_num.sign;
        ragel_type = TYPE_INT;
        ragel_cat  = CAT_NUMBER;
    }
#line 40 "lex/parser_dict_expr.rl"
	{
        match.array_slice_begin = ragel_num.getInteger();
        match.array_slice_length = ARRAY_SLICE_LENGTH_END;
        ragel_num = {};
    }
#line 72 "lex/parser_dict_expr.rl"
	{ match.array_slice_length = 1; }
#line 47 "lex/parser_dict_expr.rl"
	{
        match.type = DictExprMatchType::LIST;
#ifndef NDEBUG
        if (match.array_slice_length == ARRAY_SLICE_LENGTH_END)
            printf("array range: %lld:END\n", match.array_slice_begin);
        else if (match.array_slice_length == ARRAY_SLICE_LENGTH_NONE)
            printf("array range: %lld:?\n", match.array_slice_begin);
        else
            printf("array range: %lld:%lld\n", match.array_slice_begin, match.array_slice_length);
#endif
    }
	goto st7;
tr23:
#line 46 "lex/parser_dict_expr.rl"
	{ match.array_slice_length = ARRAY_SLICE_LENGTH_END; }
#line 47 "lex/parser_dict_expr.rl"
	{
        match.type = DictExprMatchType::LIST;
#ifndef NDEBUG
        if (match.array_slice_length == ARRAY_SLICE_LENGTH_END)
            printf("array range: %lld:END\n", match.array_slice_begin);
        else if (match.array_slice_length == ARRAY_SLICE_LENGTH_NONE)
            printf("array range: %lld:?\n", match.array_slice_begin);
        else
            printf("array range: %lld:%lld\n", match.array_slice_begin, match.array_slice_length);
#endif
    }
	goto st7;
tr25:
#line 30 "lex/ragel_numeric.rl"
	{
        ragel_num.vint *= ragel_num.sign;
        ragel_type = TYPE_INT;
        ragel_cat  = CAT_NUMBER;
    }
#line 45 "lex/parser_dict_expr.rl"
	{ match.array_slice_length = ragel_num.getInteger(); ragel_num = {}; }
#line 47 "lex/parser_dict_expr.rl"
	{
        match.type = DictExprMatchType::LIST;
#ifndef NDEBUG
        if (match.array_slice_length == ARRAY_SLICE_LENGTH_END)
            printf("array range: %lld:END\n", match.array_slice_begin);
        else if (match.array_slice_length == ARRAY_SLICE_LENGTH_NONE)
            printf("array range: %lld:?\n", match.array_slice_begin);
        else
            printf("array range: %lld:%lld\n", match.array_slice_begin, match.array_slice_length);
#endif
    }
	goto st7;
st7:
	if ( ++p == pe )
		goto _test_eof7;
case 7:
#line 399 "lex/parser_dict_expr.cpp"
	if ( (*p) == 47 )
		goto tr15;
	goto st0;
st8:
	if ( ++p == pe )
		goto _test_eof8;
case 8:
	if ( (*p) == 47 )
		goto tr16;
	goto st0;
tr4:
#line 38 "lex/parser_dict_expr.rl"
	{ match.type = DictExprMatchType::ANY; }
#line 21 "lex/ragel_numeric.rl"
	{ ragel_num.sign = ((*p)=='-') ? -1 : 1; }
	goto st5;
st5:
	if ( ++p == pe )
		goto _test_eof5;
case 5:
#line 420 "lex/parser_dict_expr.cpp"
	if ( (*p) == 48 )
		goto tr11;
	if ( 49 <= (*p) && (*p) <= 57 )
		goto tr12;
	goto st0;
tr5:
#line 38 "lex/parser_dict_expr.rl"
	{ match.type = DictExprMatchType::ANY; }
#line 29 "lex/ragel_numeric.rl"
	{ (ragel_num.vint *= 10) += ((*p)-'0'); }
	goto st9;
tr11:
#line 29 "lex/ragel_numeric.rl"
	{ (ragel_num.vint *= 10) += ((*p)-'0'); }
	goto st9;
st9:
	if ( ++p == pe )
		goto _test_eof9;
case 9:
#line 440 "lex/parser_dict_expr.cpp"
	switch( (*p) ) {
		case 47: goto tr17;
		case 58: goto tr18;
		case 63: goto tr19;
	}
	goto st0;
tr18:
#line 30 "lex/ragel_numeric.rl"
	{
        ragel_num.vint *= ragel_num.sign;
        ragel_type = TYPE_INT;
        ragel_cat  = CAT_NUMBER;
    }
#line 40 "lex/parser_dict_expr.rl"
	{
        match.array_slice_begin = ragel_num.getInteger();
        match.array_slice_length = ARRAY_SLICE_LENGTH_END;
        ragel_num = {};
    }
	goto st10;
st10:
	if ( ++p == pe )
		goto _test_eof10;
case 10:
#line 465 "lex/parser_dict_expr.cpp"
	switch( (*p) ) {
		case 47: goto tr20;
		case 48: goto tr21;
		case 63: goto tr23;
	}
	if ( 49 <= (*p) && (*p) <= 57 )
		goto tr22;
	goto st0;
tr21:
#line 46 "lex/parser_dict_expr.rl"
	{ match.array_slice_length = ARRAY_SLICE_LENGTH_END; }
#line 29 "lex/ragel_numeric.rl"
	{ (ragel_num.vint *= 10) += ((*p)-'0'); }
	goto st11;
st11:
	if ( ++p == pe )
		goto _test_eof11;
case 11:
#line 484 "lex/parser_dict_expr.cpp"
	switch( (*p) ) {
		case 47: goto tr24;
		case 63: goto tr25;
	}
	goto st0;
tr26:
#line 29 "lex/ragel_numeric.rl"
	{ (ragel_num.vint *= 10) += ((*p)-'0'); }
	goto st12;
tr22:
#line 46 "lex/parser_dict_expr.rl"
	{ match.array_slice_length = ARRAY_SLICE_LENGTH_END; }
#line 29 "lex/ragel_numeric.rl"
	{ (ragel_num.vint *= 10) += ((*p)-'0'); }
	goto st12;
st12:
	if ( ++p == pe )
		goto _test_eof12;
case 12:
#line 504 "lex/parser_dict_expr.cpp"
	switch( (*p) ) {
		case 47: goto tr24;
		case 63: goto tr25;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr26;
	goto st0;
tr6:
#line 38 "lex/parser_dict_expr.rl"
	{ match.type = DictExprMatchType::ANY; }
#line 29 "lex/ragel_numeric.rl"
	{ (ragel_num.vint *= 10) += ((*p)-'0'); }
	goto st13;
tr12:
#line 29 "lex/ragel_numeric.rl"
	{ (ragel_num.vint *= 10) += ((*p)-'0'); }
	goto st13;
st13:
	if ( ++p == pe )
		goto _test_eof13;
case 13:
#line 526 "lex/parser_dict_expr.cpp"
	switch( (*p) ) {
		case 47: goto tr17;
		case 58: goto tr18;
		case 63: goto tr19;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr12;
	goto st0;
tr7:
#line 38 "lex/parser_dict_expr.rl"
	{ match.type = DictExprMatchType::ANY; }
#line 16 "lex/parser_dict_expr.rl"
	{ key_start = &(*p); }
	goto st14;
st14:
	if ( ++p == pe )
		goto _test_eof14;
case 14:
#line 545 "lex/parser_dict_expr.cpp"
	switch( (*p) ) {
		case 47: goto tr27;
		case 63: goto tr29;
		case 95: goto st14;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st14;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st14;
	} else
		goto st14;
	goto st0;
	}
	_test_eof2: cs = 2; goto _test_eof; 
	_test_eof3: cs = 3; goto _test_eof; 
	_test_eof4: cs = 4; goto _test_eof; 
	_test_eof6: cs = 6; goto _test_eof; 
	_test_eof7: cs = 7; goto _test_eof; 
	_test_eof8: cs = 8; goto _test_eof; 
	_test_eof5: cs = 5; goto _test_eof; 
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
	case 6: 
#line 59 "lex/parser_dict_expr.rl"
	{
        if (matches) {
            matches->push_back(match);
            match = {};
            key_start = nullptr;
        }
    }
	break;
	case 14: 
#line 17 "lex/parser_dict_expr.rl"
	{
        if (key_start) {
            constexpr int BUF_SIZE = 256;
            char buf[BUF_SIZE+1];

            auto str_len = std::min<int>(&(*p) - key_start, BUF_SIZE);
            int i;
            for (i = 0; i < str_len; i++) {
                buf[i] = key_start[i];
            }
            buf[i] = '\0'; // set c-string null byte
            match.key_name = gensym(buf);

#ifndef NDEBUG
            printf(" key: %s\n", match.key_name->s_name);
#endif

            match.type = DictExprMatchType::DICT;
        }
    }
#line 59 "lex/parser_dict_expr.rl"
	{
        if (matches) {
            matches->push_back(match);
            match = {};
            key_start = nullptr;
        }
    }
	break;
	case 8: 
#line 38 "lex/parser_dict_expr.rl"
	{ match.type = DictExprMatchType::ANY; }
#line 59 "lex/parser_dict_expr.rl"
	{
        if (matches) {
            matches->push_back(match);
            match = {};
            key_start = nullptr;
        }
    }
	break;
	case 7: 
#line 67 "lex/parser_dict_expr.rl"
	{
        match.no_error = true;
    }
#line 59 "lex/parser_dict_expr.rl"
	{
        if (matches) {
            matches->push_back(match);
            match = {};
            key_start = nullptr;
        }
    }
	break;
	case 10: 
#line 46 "lex/parser_dict_expr.rl"
	{ match.array_slice_length = ARRAY_SLICE_LENGTH_END; }
#line 47 "lex/parser_dict_expr.rl"
	{
        match.type = DictExprMatchType::LIST;
#ifndef NDEBUG
        if (match.array_slice_length == ARRAY_SLICE_LENGTH_END)
            printf("array range: %lld:END\n", match.array_slice_begin);
        else if (match.array_slice_length == ARRAY_SLICE_LENGTH_NONE)
            printf("array range: %lld:?\n", match.array_slice_begin);
        else
            printf("array range: %lld:%lld\n", match.array_slice_begin, match.array_slice_length);
#endif
    }
#line 59 "lex/parser_dict_expr.rl"
	{
        if (matches) {
            matches->push_back(match);
            match = {};
            key_start = nullptr;
        }
    }
	break;
	case 11: 
	case 12: 
#line 30 "lex/ragel_numeric.rl"
	{
        ragel_num.vint *= ragel_num.sign;
        ragel_type = TYPE_INT;
        ragel_cat  = CAT_NUMBER;
    }
#line 45 "lex/parser_dict_expr.rl"
	{ match.array_slice_length = ragel_num.getInteger(); ragel_num = {}; }
#line 47 "lex/parser_dict_expr.rl"
	{
        match.type = DictExprMatchType::LIST;
#ifndef NDEBUG
        if (match.array_slice_length == ARRAY_SLICE_LENGTH_END)
            printf("array range: %lld:END\n", match.array_slice_begin);
        else if (match.array_slice_length == ARRAY_SLICE_LENGTH_NONE)
            printf("array range: %lld:?\n", match.array_slice_begin);
        else
            printf("array range: %lld:%lld\n", match.array_slice_begin, match.array_slice_length);
#endif
    }
#line 59 "lex/parser_dict_expr.rl"
	{
        if (matches) {
            matches->push_back(match);
            match = {};
            key_start = nullptr;
        }
    }
	break;
	case 9: 
	case 13: 
#line 30 "lex/ragel_numeric.rl"
	{
        ragel_num.vint *= ragel_num.sign;
        ragel_type = TYPE_INT;
        ragel_cat  = CAT_NUMBER;
    }
#line 40 "lex/parser_dict_expr.rl"
	{
        match.array_slice_begin = ragel_num.getInteger();
        match.array_slice_length = ARRAY_SLICE_LENGTH_END;
        ragel_num = {};
    }
#line 72 "lex/parser_dict_expr.rl"
	{ match.array_slice_length = 1; }
#line 47 "lex/parser_dict_expr.rl"
	{
        match.type = DictExprMatchType::LIST;
#ifndef NDEBUG
        if (match.array_slice_length == ARRAY_SLICE_LENGTH_END)
            printf("array range: %lld:END\n", match.array_slice_begin);
        else if (match.array_slice_length == ARRAY_SLICE_LENGTH_NONE)
            printf("array range: %lld:?\n", match.array_slice_begin);
        else
            printf("array range: %lld:%lld\n", match.array_slice_begin, match.array_slice_length);
#endif
    }
#line 59 "lex/parser_dict_expr.rl"
	{
        if (matches) {
            matches->push_back(match);
            match = {};
            key_start = nullptr;
        }
    }
	break;
#line 738 "lex/parser_dict_expr.cpp"
	}
	}

	_out: {}
	}

#line 106 "lex/parser_dict_expr.rl"

    const auto ok = cs >= 6;

    return ok;
}

}
}
