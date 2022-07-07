#line 1 "lex/parser_numeric.rl"
# include "parser_numeric.h"

# include <cstdint>
# include <cstring>
# include <limits>

namespace ceammc {
	namespace parser {
		
		static uint8_t xchar2digit(char c)
		{
			switch (c) {
				case '0':
				case '1':
				case '2':
				case '3':
				case '4':
				case '5':
				case '6':
				case '7':
				case '8':
				case '9':
				return c - '0';
				case 'A':
				case 'B':
				case 'C':
				case 'D':
				case 'E':
				case 'F':
				return c - 'A' + 10;
				default:
				return c - 'a' + 10;
			}
		}
		
		
#line 39 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_numeric.cpp"
		static const int numeric_full_start = 1;
		static const int numeric_full_first_final = 14;
		static const int numeric_full_error = 0;
		
		static const int numeric_full_en_main = 1;
		
		
#line 42 "lex/parser_numeric.rl"
		
		
		NumericFullMatch::NumericFullMatch()
		{
			reset();
		}
		
		void NumericFullMatch::reset()
		{
			res_ = {};
		}
		
		bool NumericFullMatch::parse(const Atom& a)
		{
			reset();
			
			if (a.isSymbol())
				return parse(a.asT<t_symbol*>()->s_name);
			else if(a.isFloat()) {
				if(a.isInteger()) {
					res_.t = TYPE_INT;
					res_.f = a.asT<t_float>();
					res_.i0 = res_.f;
				} else {
					res_.t = TYPE_FLOAT;
					res_.f = a.asT<t_float>();
				}
				
				return true;
			} else
			return false;
		}
		
		bool NumericFullMatch::parse(const char* str)
		{
			const auto len = strlen(str);
			if (len == 0)
				return false;
			
			const char* p = str;
			const char* pe = p + len;
			const char* eof = pe;
			
			DECLARE_RAGEL_COMMON_VARS;
			DECLARE_RAGEL_NUMERIC_VARS;
			
			reset();
			
			
#line 97 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_numeric.cpp"
			{
				cs = (int)numeric_full_start;
			}
			
#line 90 "lex/parser_numeric.rl"
			
			
#line 105 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_numeric.cpp"
			{
				if ( p == pe )
					goto _test_eof;
				switch ( cs ) {
					case 1:
					goto st_case_1;
					case 0:
					goto st_case_0;
					case 2:
					goto st_case_2;
					case 14:
					goto st_case_14;
					case 15:
					goto st_case_15;
					case 16:
					goto st_case_16;
					case 3:
					goto st_case_3;
					case 17:
					goto st_case_17;
					case 4:
					goto st_case_4;
					case 5:
					goto st_case_5;
					case 6:
					goto st_case_6;
					case 7:
					goto st_case_7;
					case 8:
					goto st_case_8;
					case 9:
					goto st_case_9;
					case 18:
					goto st_case_18;
					case 19:
					goto st_case_19;
					case 20:
					goto st_case_20;
					case 10:
					goto st_case_10;
					case 11:
					goto st_case_11;
					case 21:
					goto st_case_21;
					case 22:
					goto st_case_22;
					case 12:
					goto st_case_12;
					case 23:
					goto st_case_23;
					case 13:
					goto st_case_13;
					case 24:
					goto st_case_24;
				}
				goto st_out;
				_st1:
				if ( p == eof ) {
					if ( cs >= 14 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof1;
				st_case_1:
				switch( ( (*( p))) ) {
					case 43: {
						goto _ctr2;
					}
					case 45: {
						goto _ctr2;
					}
					case 48: {
						goto _ctr3;
					}
					case 105: {
						goto _st10;
					}
				}
				if ( 49 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr4;
				}
				{
					goto _st0;
				}
				st_case_0:
				_st0:
				cs = 0;
				goto _pop;
				_ctr2:
				{
#line 21 "lex/ragel_numeric.rl"
					ragel_num.sign = ((( (*( p))))=='-') ? -1 : 1; }
				
#line 202 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_numeric.cpp"
				
				goto _st2;
				_st2:
				if ( p == eof ) {
					if ( cs >= 14 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof2;
				st_case_2:
				switch( ( (*( p))) ) {
					case 48: {
						goto _ctr7;
					}
					case 105: {
						goto _st10;
					}
				}
				if ( 49 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr4;
				}
				{
					goto _st0;
				}
				_ctr7:
				{
#line 29 "lex/ragel_numeric.rl"
					(ragel_num.vint *= 10) += ((( (*( p))))-'0'); }
				
#line 235 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_numeric.cpp"
				
				{
#line 42 "lex/ragel_numeric.rl"
					(ragel_num.ratio.num *= 10) += ((( (*( p))))-'0'); }
				
#line 241 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_numeric.cpp"
				
				goto _st14;
				_ctr26:
				{
#line 30 "lex/ragel_numeric.rl"
					
					ragel_num.vint *= ragel_num.sign;
					ragel_type = TYPE_INT;
					ragel_cat  = CAT_NUMBER;
				}
				
#line 253 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_numeric.cpp"
				
				goto _st14;
				_st14:
				if ( p == eof ) {
					if ( cs >= 14 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof14;
				st_case_14:
				switch( ( (*( p))) ) {
					case 37: {
						goto _ctr27;
					}
					case 42: {
						goto _ctr28;
					}
					case 46: {
						goto _ctr29;
					}
					case 47: {
						goto _st9;
					}
					case 95: {
						goto _ctr30;
					}
					case 112: {
						goto _ctr31;
					}
				}
				{
					goto _st0;
				}
				_ctr27:
				{
#line 30 "lex/ragel_numeric.rl"
					
					ragel_num.vint *= ragel_num.sign;
					ragel_type = TYPE_INT;
					ragel_cat  = CAT_NUMBER;
				}
				
#line 299 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_numeric.cpp"
				
				{
#line 108 "lex/ragel_numeric.rl"
					ragel_num.vdouble = ragel_num.vint;}
				
#line 305 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_numeric.cpp"
				
				goto _st15;
				_ctr32:
				{
#line 70 "lex/ragel_numeric.rl"
					
					ragel_type = TYPE_PERCENT;
					ragel_cat = CAT_NUMBER;
				}
				
#line 316 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_numeric.cpp"
				
				goto _st15;
				_ctr35:
				{
#line 58 "lex/ragel_numeric.rl"
					
					const auto frac = double(ragel_num.ratio.num) / ragel_num.ratio.den;
					ragel_num.vdouble = ragel_num.vint + ragel_num.sign * frac;
					ragel_type = TYPE_FLOAT;
					ragel_cat  = CAT_NUMBER;
				}
				
#line 329 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_numeric.cpp"
				
				goto _st15;
				_st15:
				if ( p == eof ) {
					if ( cs >= 14 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof15;
				st_case_15:
				{
					goto _st0;
				}
				_ctr28:
				{
#line 30 "lex/ragel_numeric.rl"
					
					ragel_num.vint *= ragel_num.sign;
					ragel_type = TYPE_INT;
					ragel_cat  = CAT_NUMBER;
				}
				
#line 355 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_numeric.cpp"
				
				{
#line 113 "lex/ragel_numeric.rl"
					ragel_num.vdouble = ragel_num.vint;}
				
#line 361 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_numeric.cpp"
				
				goto _st16;
				_ctr33:
				{
#line 75 "lex/ragel_numeric.rl"
					
					ragel_type = TYPE_PHASE;
					ragel_cat = CAT_NUMBER;
				}
				
#line 372 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_numeric.cpp"
				
				goto _st16;
				_ctr36:
				{
#line 58 "lex/ragel_numeric.rl"
					
					const auto frac = double(ragel_num.ratio.num) / ragel_num.ratio.den;
					ragel_num.vdouble = ragel_num.vint + ragel_num.sign * frac;
					ragel_type = TYPE_FLOAT;
					ragel_cat  = CAT_NUMBER;
				}
				
#line 385 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_numeric.cpp"
				
				goto _st16;
				_st16:
				if ( p == eof ) {
					if ( cs >= 14 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof16;
				st_case_16:
				{
					goto _st0;
				}
				_ctr29:
				{
#line 30 "lex/ragel_numeric.rl"
					
					ragel_num.vint *= ragel_num.sign;
					ragel_type = TYPE_INT;
					ragel_cat  = CAT_NUMBER;
				}
				
#line 411 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_numeric.cpp"
				
				{
#line 50 "lex/ragel_numeric.rl"
					
					ragel_num.ratio.num = 0;
					ragel_num.ratio.den = 1;
				}
				
#line 420 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_numeric.cpp"
				
				goto _st3;
				_st3:
				if ( p == eof ) {
					if ( cs >= 14 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof3;
				st_case_3:
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr9;
				}
				{
					goto _st0;
				}
				_ctr9:
				{
#line 54 "lex/ragel_numeric.rl"
					
					(ragel_num.ratio.num *= 10) += ((( (*( p)))) - '0');
					ragel_num.ratio.den *= 10;
				}
				
#line 448 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_numeric.cpp"
				
				goto _st17;
				_ctr34:
				{
#line 58 "lex/ragel_numeric.rl"
					
					const auto frac = double(ragel_num.ratio.num) / ragel_num.ratio.den;
					ragel_num.vdouble = ragel_num.vint + ragel_num.sign * frac;
					ragel_type = TYPE_FLOAT;
					ragel_cat  = CAT_NUMBER;
				}
				
#line 461 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_numeric.cpp"
				
				goto _st17;
				_st17:
				if ( p == eof ) {
					if ( cs >= 14 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof17;
				st_case_17:
				switch( ( (*( p))) ) {
					case 37: {
						goto _ctr35;
					}
					case 42: {
						goto _ctr36;
					}
					case 95: {
						goto _ctr37;
					}
					case 112: {
						goto _ctr38;
					}
				}
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr9;
				}
				{
					goto _st0;
				}
				_ctr30:
				{
#line 30 "lex/ragel_numeric.rl"
					
					ragel_num.vint *= ragel_num.sign;
					ragel_type = TYPE_INT;
					ragel_cat  = CAT_NUMBER;
				}
				
#line 504 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_numeric.cpp"
				
				{
#line 108 "lex/ragel_numeric.rl"
					ragel_num.vdouble = ragel_num.vint;}
				
#line 510 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_numeric.cpp"
				
				{
#line 113 "lex/ragel_numeric.rl"
					ragel_num.vdouble = ragel_num.vint;}
				
#line 516 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_numeric.cpp"
				
				goto _st4;
				_ctr37:
				{
#line 58 "lex/ragel_numeric.rl"
					
					const auto frac = double(ragel_num.ratio.num) / ragel_num.ratio.den;
					ragel_num.vdouble = ragel_num.vint + ragel_num.sign * frac;
					ragel_type = TYPE_FLOAT;
					ragel_cat  = CAT_NUMBER;
				}
				
#line 529 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_numeric.cpp"
				
				goto _st4;
				_st4:
				if ( p == eof ) {
					if ( cs >= 14 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof4;
				st_case_4:
				switch( ( (*( p))) ) {
					case 37: {
						goto _st15;
					}
					case 112: {
						goto _st5;
					}
				}
				{
					goto _st0;
				}
				_ctr31:
				{
#line 30 "lex/ragel_numeric.rl"
					
					ragel_num.vint *= ragel_num.sign;
					ragel_type = TYPE_INT;
					ragel_cat  = CAT_NUMBER;
				}
				
#line 563 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_numeric.cpp"
				
				{
#line 113 "lex/ragel_numeric.rl"
					ragel_num.vdouble = ragel_num.vint;}
				
#line 569 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_numeric.cpp"
				
				goto _st5;
				_ctr38:
				{
#line 58 "lex/ragel_numeric.rl"
					
					const auto frac = double(ragel_num.ratio.num) / ragel_num.ratio.den;
					ragel_num.vdouble = ragel_num.vint + ragel_num.sign * frac;
					ragel_type = TYPE_FLOAT;
					ragel_cat  = CAT_NUMBER;
				}
				
#line 582 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_numeric.cpp"
				
				goto _st5;
				_st5:
				if ( p == eof ) {
					if ( cs >= 14 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof5;
				st_case_5:
				if ( ( (*( p))) == 104 ) {
					goto _st6;
				}
				{
					goto _st0;
				}
				_st6:
				if ( p == eof ) {
					if ( cs >= 14 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof6;
				st_case_6:
				if ( ( (*( p))) == 97 ) {
					goto _st7;
				}
				{
					goto _st0;
				}
				_st7:
				if ( p == eof ) {
					if ( cs >= 14 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof7;
				st_case_7:
				if ( ( (*( p))) == 115 ) {
					goto _st8;
				}
				{
					goto _st0;
				}
				_st8:
				if ( p == eof ) {
					if ( cs >= 14 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof8;
				st_case_8:
				if ( ( (*( p))) == 101 ) {
					goto _st16;
				}
				{
					goto _st0;
				}
				_st9:
				if ( p == eof ) {
					if ( cs >= 14 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof9;
				st_case_9:
				if ( ( (*( p))) == 48 ) {
					goto _ctr18;
				}
				if ( 49 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr19;
				}
				{
					goto _st0;
				}
				_ctr18:
				{
#line 43 "lex/ragel_numeric.rl"
					(ragel_num.ratio.den *= 10) += ((( (*( p))))-'0'); }
				
#line 678 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_numeric.cpp"
				
				goto _st18;
				_ctr39:
				{
#line 44 "lex/ragel_numeric.rl"
					
					ragel_num.ratio.num *= ragel_num.sign;
					ragel_type = TYPE_RATIO;
					ragel_cat  = CAT_NUMBER;
				}
				
#line 690 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_numeric.cpp"
				
				goto _st18;
				_st18:
				if ( p == eof ) {
					if ( cs >= 14 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof18;
				st_case_18:
				{
					goto _st0;
				}
				_ctr19:
				{
#line 43 "lex/ragel_numeric.rl"
					(ragel_num.ratio.den *= 10) += ((( (*( p))))-'0'); }
				
#line 712 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_numeric.cpp"
				
				goto _st19;
				_ctr40:
				{
#line 44 "lex/ragel_numeric.rl"
					
					ragel_num.ratio.num *= ragel_num.sign;
					ragel_type = TYPE_RATIO;
					ragel_cat  = CAT_NUMBER;
				}
				
#line 724 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_numeric.cpp"
				
				goto _st19;
				_st19:
				if ( p == eof ) {
					if ( cs >= 14 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof19;
				st_case_19:
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr19;
				}
				{
					goto _st0;
				}
				_ctr4:
				{
#line 29 "lex/ragel_numeric.rl"
					(ragel_num.vint *= 10) += ((( (*( p))))-'0'); }
				
#line 749 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_numeric.cpp"
				
				{
#line 42 "lex/ragel_numeric.rl"
					(ragel_num.ratio.num *= 10) += ((( (*( p))))-'0'); }
				
#line 755 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_numeric.cpp"
				
				goto _st20;
				_ctr41:
				{
#line 30 "lex/ragel_numeric.rl"
					
					ragel_num.vint *= ragel_num.sign;
					ragel_type = TYPE_INT;
					ragel_cat  = CAT_NUMBER;
				}
				
#line 767 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_numeric.cpp"
				
				goto _st20;
				_st20:
				if ( p == eof ) {
					if ( cs >= 14 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof20;
				st_case_20:
				switch( ( (*( p))) ) {
					case 37: {
						goto _ctr27;
					}
					case 42: {
						goto _ctr28;
					}
					case 46: {
						goto _ctr29;
					}
					case 47: {
						goto _st9;
					}
					case 95: {
						goto _ctr30;
					}
					case 112: {
						goto _ctr31;
					}
				}
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr4;
				}
				{
					goto _st0;
				}
				_st10:
				if ( p == eof ) {
					if ( cs >= 14 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof10;
				st_case_10:
				if ( ( (*( p))) == 110 ) {
					goto _st11;
				}
				{
					goto _st0;
				}
				_st11:
				if ( p == eof ) {
					if ( cs >= 14 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof11;
				st_case_11:
				if ( ( (*( p))) == 102 ) {
					goto _st21;
				}
				{
					goto _st0;
				}
				_ctr42:
				{
#line 65 "lex/ragel_numeric.rl"
					
					ragel_type = TYPE_INF;
					ragel_cat = CAT_NUMBER;
				}
				
#line 849 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_numeric.cpp"
				
				goto _st21;
				_st21:
				if ( p == eof ) {
					if ( cs >= 14 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof21;
				st_case_21:
				{
					goto _st0;
				}
				_ctr3:
				{
#line 29 "lex/ragel_numeric.rl"
					(ragel_num.vint *= 10) += ((( (*( p))))-'0'); }
				
#line 871 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_numeric.cpp"
				
				{
#line 42 "lex/ragel_numeric.rl"
					(ragel_num.ratio.num *= 10) += ((( (*( p))))-'0'); }
				
#line 877 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_numeric.cpp"
				
				goto _st22;
				_ctr43:
				{
#line 30 "lex/ragel_numeric.rl"
					
					ragel_num.vint *= ragel_num.sign;
					ragel_type = TYPE_INT;
					ragel_cat  = CAT_NUMBER;
				}
				
#line 889 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_numeric.cpp"
				
				goto _st22;
				_st22:
				if ( p == eof ) {
					if ( cs >= 14 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof22;
				st_case_22:
				switch( ( (*( p))) ) {
					case 37: {
						goto _ctr27;
					}
					case 42: {
						goto _ctr28;
					}
					case 46: {
						goto _ctr29;
					}
					case 47: {
						goto _st9;
					}
					case 95: {
						goto _ctr30;
					}
					case 98: {
						goto _st12;
					}
					case 112: {
						goto _ctr31;
					}
					case 120: {
						goto _st13;
					}
				}
				{
					goto _st0;
				}
				_st12:
				if ( p == eof ) {
					if ( cs >= 14 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof12;
				st_case_12:
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 49 ) {
					goto _ctr23;
				}
				{
					goto _st0;
				}
				_ctr23:
				{
#line 23 "lex/ragel_numeric.rl"
					(ragel_num.vint <<= 1) |= ((( (*( p))))=='1'); }
				
#line 954 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_numeric.cpp"
				
				goto _st23;
				_ctr44:
				{
#line 24 "lex/ragel_numeric.rl"
					
					ragel_type = TYPE_BIN;
					ragel_cat  = CAT_NUMBER;
				}
				
#line 965 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_numeric.cpp"
				
				goto _st23;
				_st23:
				if ( p == eof ) {
					if ( cs >= 14 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof23;
				st_case_23:
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 49 ) {
					goto _ctr23;
				}
				{
					goto _st0;
				}
				_st13:
				if ( p == eof ) {
					if ( cs >= 14 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof13;
				st_case_13:
				if ( ( (*( p))) < 65 ) {
					if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
						goto _ctr25;
					}
				} else if ( ( (*( p))) > 70 ) {
					if ( 97 <= ( (*( p))) && ( (*( p))) <= 102 ) {
						goto _ctr25;
					}
				} else {
					goto _ctr25;
				}
				{
					goto _st0;
				}
				_ctr25:
				{
#line 36 "lex/ragel_numeric.rl"
					(ragel_num.vint <<= 4) |= xchar2digit((( (*( p))))); }
				
#line 1015 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_numeric.cpp"
				
				goto _st24;
				_ctr45:
				{
#line 37 "lex/ragel_numeric.rl"
					
					ragel_type = TYPE_HEX;
					ragel_cat  = CAT_NUMBER;
				}
				
#line 1026 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_numeric.cpp"
				
				goto _st24;
				_st24:
				if ( p == eof ) {
					if ( cs >= 14 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof24;
				st_case_24:
				if ( ( (*( p))) < 65 ) {
					if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
						goto _ctr25;
					}
				} else if ( ( (*( p))) > 70 ) {
					if ( 97 <= ( (*( p))) && ( (*( p))) <= 102 ) {
						goto _ctr25;
					}
				} else {
					goto _ctr25;
				}
				{
					goto _st0;
				}
				st_out:
				_test_eof1: cs = 1; goto _test_eof; 
				_test_eof2: cs = 2; goto _test_eof; 
				_test_eof14: cs = 14; goto _test_eof; 
				_test_eof15: cs = 15; goto _test_eof; 
				_test_eof16: cs = 16; goto _test_eof; 
				_test_eof3: cs = 3; goto _test_eof; 
				_test_eof17: cs = 17; goto _test_eof; 
				_test_eof4: cs = 4; goto _test_eof; 
				_test_eof5: cs = 5; goto _test_eof; 
				_test_eof6: cs = 6; goto _test_eof; 
				_test_eof7: cs = 7; goto _test_eof; 
				_test_eof8: cs = 8; goto _test_eof; 
				_test_eof9: cs = 9; goto _test_eof; 
				_test_eof18: cs = 18; goto _test_eof; 
				_test_eof19: cs = 19; goto _test_eof; 
				_test_eof20: cs = 20; goto _test_eof; 
				_test_eof10: cs = 10; goto _test_eof; 
				_test_eof11: cs = 11; goto _test_eof; 
				_test_eof21: cs = 21; goto _test_eof; 
				_test_eof22: cs = 22; goto _test_eof; 
				_test_eof12: cs = 12; goto _test_eof; 
				_test_eof23: cs = 23; goto _test_eof; 
				_test_eof13: cs = 13; goto _test_eof; 
				_test_eof24: cs = 24; goto _test_eof; 
				
				_test_eof: {}
				if ( p == eof ) {
					switch ( cs ) {
						case 1: {
							break;
						}
						case 0: {
							break;
						}
						case 2: {
							break;
						}
						case 14: {
							break;
						}
						case 15: {
							break;
						}
						case 16: {
							break;
						}
						case 3: {
							break;
						}
						case 17: {
							break;
						}
						case 4: {
							break;
						}
						case 5: {
							break;
						}
						case 6: {
							break;
						}
						case 7: {
							break;
						}
						case 8: {
							break;
						}
						case 9: {
							break;
						}
						case 18: {
							break;
						}
						case 19: {
							break;
						}
						case 20: {
							break;
						}
						case 10: {
							break;
						}
						case 11: {
							break;
						}
						case 21: {
							break;
						}
						case 22: {
							break;
						}
						case 12: {
							break;
						}
						case 23: {
							break;
						}
						case 13: {
							break;
						}
						case 24: {
							break;
						}
					}
					switch ( cs ) {
					}
					switch ( cs ) {
						case 1:
						goto _st1;case 0:
						goto _st0;case 2:
						goto _st2;case 14:
						goto _ctr26;case 15:
						goto _ctr32;case 16:
						goto _ctr33;case 3:
						goto _st3;case 17:
						goto _ctr34;case 4:
						goto _st4;case 5:
						goto _st5;case 6:
						goto _st6;case 7:
						goto _st7;case 8:
						goto _st8;case 9:
						goto _st9;case 18:
						goto _ctr39;case 19:
						goto _ctr40;case 20:
						goto _ctr41;case 10:
						goto _st10;case 11:
						goto _st11;case 21:
						goto _ctr42;case 22:
						goto _ctr43;case 12:
						goto _st12;case 23:
						goto _ctr44;case 13:
						goto _st13;case 24:
						goto _ctr45;	}
				}
				
				if ( cs >= 14 )
					goto _out; _pop: {}
				_out: {}
			}
			
#line 91 "lex/parser_numeric.rl"
			
			
			switch(ragel_type) {
				case TYPE_PHASE:
				case TYPE_PERCENT:
				case TYPE_FLOAT:
				res_.f = ragel_num.getFloat();
				break;
				case TYPE_INT:
				case TYPE_HEX:
				case TYPE_BIN:
				res_.i0 = ragel_num.getInteger();
				break;
				case TYPE_RATIO:
				res_.i0 = ragel_num.ratio.num;
				res_.i1 = ragel_num.ratio.den;
				break;
				case TYPE_INF:
				res_.f = std::numeric_limits<typeof(res_.f)>::infinity();
				break;
				default:
				break;
			}
			
			res_.t = ragel_type;
			
			return cs >= 
#line 1223 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_numeric.cpp"
			14
#line 117 "lex/parser_numeric.rl"
			;
		}
		
		bool NumericFullMatch::getBin(t_int& b) const
		{
			if(!isBin()) return false;
			b = res_.i0;
			return true;
		}
		
		bool NumericFullMatch::getInt(t_int& i) const
		{
			if(!isInt()) return false;
			i = res_.i0;
			return true;
		}
		
		bool NumericFullMatch::getHex(t_int& x) const
		{
			if(!isHex()) return false;
			x = res_.i0;
			return true;
		}
		
		bool NumericFullMatch::getFloat(t_float& f) const
		{
			if(!isFloat()) return false;
			f = res_.f;
			return true;
		}
		
		bool NumericFullMatch::getRatio(t_int& num, t_int& den) const
		{
			if(!isRatio()) return false;
			num = res_.i0;
			den = res_.i1;
			return true;
		}
		
		bool NumericFullMatch::parseAll(const AtomListView& lv, SmallFVec& out)
		{
			return parseSome(lv, out) == lv.size();
		}
		
		size_t NumericFullMatch::parseSome(const AtomListView& lv, SmallFVec& out)
		{
			const size_t N = lv.size();
			
			for (size_t i = 0; i < N; i++) {
				const auto& a = lv[i];
				if (!parse(a))
					return i;
				
				out.push_back(asFloat());
			}
			
			return N;
		}
		
		
#line 1286 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_numeric.cpp"
		static const int numeric_some_start = 1;
		static const int numeric_some_first_final = 14;
		static const int numeric_some_error = 0;
		
		static const int numeric_some_en_main = 1;
		
		
#line 187 "lex/parser_numeric.rl"
		
		
		NumericMatchSome::NumericMatchSome()
		{
			reset();
		}
		
		void NumericMatchSome::reset()
		{
			
#line 1305 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_numeric.cpp"
			{
				cs = (int)numeric_some_start;
			}
			
#line 196 "lex/parser_numeric.rl"
			
			res_.clear();
		}
		
		bool NumericMatchSome::parseSingle(const char* str, const char*& endptr)
		{
			const auto len = strlen(str);
			if (len == 0)
				return false;
			
			const char* p = str;
			const char* pe = p + len;
			const char* eof = pe;
			
			DECLARE_RAGEL_COMMON_VARS;
			DECLARE_RAGEL_NUMERIC_VARS;
			
			bool ok = false;
			
			
#line 1331 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_numeric.cpp"
			{
				cs = (int)numeric_some_start;
			}
			
#line 215 "lex/parser_numeric.rl"
			
			
#line 1339 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_numeric.cpp"
			{
				if ( p == pe )
					goto _test_eof;
				switch ( cs ) {
					case 1:
					goto st_case_1;
					case 0:
					goto st_case_0;
					case 2:
					goto st_case_2;
					case 14:
					goto st_case_14;
					case 15:
					goto st_case_15;
					case 16:
					goto st_case_16;
					case 17:
					goto st_case_17;
					case 18:
					goto st_case_18;
					case 3:
					goto st_case_3;
					case 19:
					goto st_case_19;
					case 4:
					goto st_case_4;
					case 5:
					goto st_case_5;
					case 6:
					goto st_case_6;
					case 7:
					goto st_case_7;
					case 8:
					goto st_case_8;
					case 9:
					goto st_case_9;
					case 20:
					goto st_case_20;
					case 21:
					goto st_case_21;
					case 22:
					goto st_case_22;
					case 10:
					goto st_case_10;
					case 11:
					goto st_case_11;
					case 23:
					goto st_case_23;
					case 24:
					goto st_case_24;
					case 12:
					goto st_case_12;
					case 25:
					goto st_case_25;
					case 13:
					goto st_case_13;
					case 26:
					goto st_case_26;
				}
				goto st_out;
				_st1:
				if ( p == eof ) {
					if ( cs >= 14 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof1;
				st_case_1:
				switch( ( (*( p))) ) {
					case 9: {
						goto _st1;
					}
					case 32: {
						goto _st1;
					}
					case 43: {
						goto _ctr2;
					}
					case 45: {
						goto _ctr2;
					}
					case 48: {
						goto _ctr3;
					}
					case 105: {
						goto _st10;
					}
				}
				if ( 49 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr4;
				}
				{
					goto _st0;
				}
				st_case_0:
				_st0:
				cs = 0;
				goto _pop;
				_ctr2:
				{
#line 21 "lex/ragel_numeric.rl"
					ragel_num.sign = ((( (*( p))))=='-') ? -1 : 1; }
				
#line 1446 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_numeric.cpp"
				
				goto _st2;
				_st2:
				if ( p == eof ) {
					if ( cs >= 14 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof2;
				st_case_2:
				switch( ( (*( p))) ) {
					case 48: {
						goto _ctr7;
					}
					case 105: {
						goto _st10;
					}
				}
				if ( 49 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr4;
				}
				{
					goto _st0;
				}
				_ctr7:
				{
#line 29 "lex/ragel_numeric.rl"
					(ragel_num.vint *= 10) += ((( (*( p))))-'0'); }
				
#line 1479 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_numeric.cpp"
				
				{
#line 42 "lex/ragel_numeric.rl"
					(ragel_num.ratio.num *= 10) += ((( (*( p))))-'0'); }
				
#line 1485 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_numeric.cpp"
				
				goto _st14;
				_ctr26:
				{
#line 30 "lex/ragel_numeric.rl"
					
					ragel_num.vint *= ragel_num.sign;
					ragel_type = TYPE_INT;
					ragel_cat  = CAT_NUMBER;
				}
				
#line 1497 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_numeric.cpp"
				
				goto _st14;
				_st14:
				if ( p == eof ) {
					if ( cs >= 14 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof14;
				st_case_14:
				switch( ( (*( p))) ) {
					case 9: {
						goto _ctr27;
					}
					case 32: {
						goto _ctr27;
					}
					case 37: {
						goto _ctr28;
					}
					case 42: {
						goto _ctr29;
					}
					case 46: {
						goto _ctr30;
					}
					case 47: {
						goto _st9;
					}
					case 95: {
						goto _ctr31;
					}
					case 112: {
						goto _ctr32;
					}
				}
				{
					goto _st0;
				}
				_ctr27:
				{
#line 30 "lex/ragel_numeric.rl"
					
					ragel_num.vint *= ragel_num.sign;
					ragel_type = TYPE_INT;
					ragel_cat  = CAT_NUMBER;
				}
				
#line 1549 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_numeric.cpp"
				
				goto _st15;
				_ctr36:
				{
#line 70 "lex/ragel_numeric.rl"
					
					ragel_type = TYPE_PERCENT;
					ragel_cat = CAT_NUMBER;
				}
				
#line 1560 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_numeric.cpp"
				
				goto _st15;
				_ctr38:
				{
#line 75 "lex/ragel_numeric.rl"
					
					ragel_type = TYPE_PHASE;
					ragel_cat = CAT_NUMBER;
				}
				
#line 1571 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_numeric.cpp"
				
				goto _st15;
				_ctr40:
				{
#line 58 "lex/ragel_numeric.rl"
					
					const auto frac = double(ragel_num.ratio.num) / ragel_num.ratio.den;
					ragel_num.vdouble = ragel_num.vint + ragel_num.sign * frac;
					ragel_type = TYPE_FLOAT;
					ragel_cat  = CAT_NUMBER;
				}
				
#line 1584 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_numeric.cpp"
				
				goto _st15;
				_ctr46:
				{
#line 44 "lex/ragel_numeric.rl"
					
					ragel_num.ratio.num *= ragel_num.sign;
					ragel_type = TYPE_RATIO;
					ragel_cat  = CAT_NUMBER;
				}
				
#line 1596 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_numeric.cpp"
				
				goto _st15;
				_ctr50:
				{
#line 65 "lex/ragel_numeric.rl"
					
					ragel_type = TYPE_INF;
					ragel_cat = CAT_NUMBER;
				}
				
#line 1607 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_numeric.cpp"
				
				goto _st15;
				_ctr53:
				{
#line 24 "lex/ragel_numeric.rl"
					
					ragel_type = TYPE_BIN;
					ragel_cat  = CAT_NUMBER;
				}
				
#line 1618 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_numeric.cpp"
				
				goto _st15;
				_ctr55:
				{
#line 37 "lex/ragel_numeric.rl"
					
					ragel_type = TYPE_HEX;
					ragel_cat  = CAT_NUMBER;
				}
				
#line 1629 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_numeric.cpp"
				
				goto _st15;
				_st15:
				if ( p == eof ) {
					if ( cs >= 14 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof15;
				st_case_15:
				{
#line 180 "lex/parser_numeric.rl"
					if((( (*( p))))!=' '&&(( (*( p))))!='\t'){{p = p - 1; }{p+= 1; cs = 15; goto _out;}}}
				
#line 1647 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_numeric.cpp"
				
				switch( ( (*( p))) ) {
					case 9: {
						goto _st15;
					}
					case 32: {
						goto _st15;
					}
				}
				{
					goto _st16;
				}
				_st16:
				if ( p == eof ) {
					if ( cs >= 14 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof16;
				st_case_16:
				{
					goto _st16;
				}
				_ctr28:
				{
#line 30 "lex/ragel_numeric.rl"
					
					ragel_num.vint *= ragel_num.sign;
					ragel_type = TYPE_INT;
					ragel_cat  = CAT_NUMBER;
				}
				
#line 1683 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_numeric.cpp"
				
				{
#line 108 "lex/ragel_numeric.rl"
					ragel_num.vdouble = ragel_num.vint;}
				
#line 1689 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_numeric.cpp"
				
				goto _st17;
				_ctr35:
				{
#line 70 "lex/ragel_numeric.rl"
					
					ragel_type = TYPE_PERCENT;
					ragel_cat = CAT_NUMBER;
				}
				
#line 1700 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_numeric.cpp"
				
				goto _st17;
				_ctr41:
				{
#line 58 "lex/ragel_numeric.rl"
					
					const auto frac = double(ragel_num.ratio.num) / ragel_num.ratio.den;
					ragel_num.vdouble = ragel_num.vint + ragel_num.sign * frac;
					ragel_type = TYPE_FLOAT;
					ragel_cat  = CAT_NUMBER;
				}
				
#line 1713 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_numeric.cpp"
				
				goto _st17;
				_st17:
				if ( p == eof ) {
					if ( cs >= 14 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof17;
				st_case_17:
				switch( ( (*( p))) ) {
					case 9: {
						goto _ctr36;
					}
					case 32: {
						goto _ctr36;
					}
				}
				{
					goto _st0;
				}
				_ctr29:
				{
#line 30 "lex/ragel_numeric.rl"
					
					ragel_num.vint *= ragel_num.sign;
					ragel_type = TYPE_INT;
					ragel_cat  = CAT_NUMBER;
				}
				
#line 1747 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_numeric.cpp"
				
				{
#line 113 "lex/ragel_numeric.rl"
					ragel_num.vdouble = ragel_num.vint;}
				
#line 1753 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_numeric.cpp"
				
				goto _st18;
				_ctr37:
				{
#line 75 "lex/ragel_numeric.rl"
					
					ragel_type = TYPE_PHASE;
					ragel_cat = CAT_NUMBER;
				}
				
#line 1764 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_numeric.cpp"
				
				goto _st18;
				_ctr42:
				{
#line 58 "lex/ragel_numeric.rl"
					
					const auto frac = double(ragel_num.ratio.num) / ragel_num.ratio.den;
					ragel_num.vdouble = ragel_num.vint + ragel_num.sign * frac;
					ragel_type = TYPE_FLOAT;
					ragel_cat  = CAT_NUMBER;
				}
				
#line 1777 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_numeric.cpp"
				
				goto _st18;
				_st18:
				if ( p == eof ) {
					if ( cs >= 14 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof18;
				st_case_18:
				switch( ( (*( p))) ) {
					case 9: {
						goto _ctr38;
					}
					case 32: {
						goto _ctr38;
					}
				}
				{
					goto _st0;
				}
				_ctr30:
				{
#line 30 "lex/ragel_numeric.rl"
					
					ragel_num.vint *= ragel_num.sign;
					ragel_type = TYPE_INT;
					ragel_cat  = CAT_NUMBER;
				}
				
#line 1811 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_numeric.cpp"
				
				{
#line 50 "lex/ragel_numeric.rl"
					
					ragel_num.ratio.num = 0;
					ragel_num.ratio.den = 1;
				}
				
#line 1820 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_numeric.cpp"
				
				goto _st3;
				_st3:
				if ( p == eof ) {
					if ( cs >= 14 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof3;
				st_case_3:
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr9;
				}
				{
					goto _st0;
				}
				_ctr9:
				{
#line 54 "lex/ragel_numeric.rl"
					
					(ragel_num.ratio.num *= 10) += ((( (*( p)))) - '0');
					ragel_num.ratio.den *= 10;
				}
				
#line 1848 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_numeric.cpp"
				
				goto _st19;
				_ctr39:
				{
#line 58 "lex/ragel_numeric.rl"
					
					const auto frac = double(ragel_num.ratio.num) / ragel_num.ratio.den;
					ragel_num.vdouble = ragel_num.vint + ragel_num.sign * frac;
					ragel_type = TYPE_FLOAT;
					ragel_cat  = CAT_NUMBER;
				}
				
#line 1861 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_numeric.cpp"
				
				goto _st19;
				_st19:
				if ( p == eof ) {
					if ( cs >= 14 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof19;
				st_case_19:
				switch( ( (*( p))) ) {
					case 9: {
						goto _ctr40;
					}
					case 32: {
						goto _ctr40;
					}
					case 37: {
						goto _ctr41;
					}
					case 42: {
						goto _ctr42;
					}
					case 95: {
						goto _ctr43;
					}
					case 112: {
						goto _ctr44;
					}
				}
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr9;
				}
				{
					goto _st0;
				}
				_ctr31:
				{
#line 30 "lex/ragel_numeric.rl"
					
					ragel_num.vint *= ragel_num.sign;
					ragel_type = TYPE_INT;
					ragel_cat  = CAT_NUMBER;
				}
				
#line 1910 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_numeric.cpp"
				
				{
#line 108 "lex/ragel_numeric.rl"
					ragel_num.vdouble = ragel_num.vint;}
				
#line 1916 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_numeric.cpp"
				
				{
#line 113 "lex/ragel_numeric.rl"
					ragel_num.vdouble = ragel_num.vint;}
				
#line 1922 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_numeric.cpp"
				
				goto _st4;
				_ctr43:
				{
#line 58 "lex/ragel_numeric.rl"
					
					const auto frac = double(ragel_num.ratio.num) / ragel_num.ratio.den;
					ragel_num.vdouble = ragel_num.vint + ragel_num.sign * frac;
					ragel_type = TYPE_FLOAT;
					ragel_cat  = CAT_NUMBER;
				}
				
#line 1935 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_numeric.cpp"
				
				goto _st4;
				_st4:
				if ( p == eof ) {
					if ( cs >= 14 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof4;
				st_case_4:
				switch( ( (*( p))) ) {
					case 37: {
						goto _st17;
					}
					case 112: {
						goto _st5;
					}
				}
				{
					goto _st0;
				}
				_ctr32:
				{
#line 30 "lex/ragel_numeric.rl"
					
					ragel_num.vint *= ragel_num.sign;
					ragel_type = TYPE_INT;
					ragel_cat  = CAT_NUMBER;
				}
				
#line 1969 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_numeric.cpp"
				
				{
#line 113 "lex/ragel_numeric.rl"
					ragel_num.vdouble = ragel_num.vint;}
				
#line 1975 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_numeric.cpp"
				
				goto _st5;
				_ctr44:
				{
#line 58 "lex/ragel_numeric.rl"
					
					const auto frac = double(ragel_num.ratio.num) / ragel_num.ratio.den;
					ragel_num.vdouble = ragel_num.vint + ragel_num.sign * frac;
					ragel_type = TYPE_FLOAT;
					ragel_cat  = CAT_NUMBER;
				}
				
#line 1988 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_numeric.cpp"
				
				goto _st5;
				_st5:
				if ( p == eof ) {
					if ( cs >= 14 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof5;
				st_case_5:
				if ( ( (*( p))) == 104 ) {
					goto _st6;
				}
				{
					goto _st0;
				}
				_st6:
				if ( p == eof ) {
					if ( cs >= 14 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof6;
				st_case_6:
				if ( ( (*( p))) == 97 ) {
					goto _st7;
				}
				{
					goto _st0;
				}
				_st7:
				if ( p == eof ) {
					if ( cs >= 14 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof7;
				st_case_7:
				if ( ( (*( p))) == 115 ) {
					goto _st8;
				}
				{
					goto _st0;
				}
				_st8:
				if ( p == eof ) {
					if ( cs >= 14 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof8;
				st_case_8:
				if ( ( (*( p))) == 101 ) {
					goto _st18;
				}
				{
					goto _st0;
				}
				_st9:
				if ( p == eof ) {
					if ( cs >= 14 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof9;
				st_case_9:
				if ( ( (*( p))) == 48 ) {
					goto _ctr18;
				}
				if ( 49 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr19;
				}
				{
					goto _st0;
				}
				_ctr18:
				{
#line 43 "lex/ragel_numeric.rl"
					(ragel_num.ratio.den *= 10) += ((( (*( p))))-'0'); }
				
#line 2084 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_numeric.cpp"
				
				goto _st20;
				_ctr45:
				{
#line 44 "lex/ragel_numeric.rl"
					
					ragel_num.ratio.num *= ragel_num.sign;
					ragel_type = TYPE_RATIO;
					ragel_cat  = CAT_NUMBER;
				}
				
#line 2096 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_numeric.cpp"
				
				goto _st20;
				_st20:
				if ( p == eof ) {
					if ( cs >= 14 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof20;
				st_case_20:
				switch( ( (*( p))) ) {
					case 9: {
						goto _ctr46;
					}
					case 32: {
						goto _ctr46;
					}
				}
				{
					goto _st0;
				}
				_ctr19:
				{
#line 43 "lex/ragel_numeric.rl"
					(ragel_num.ratio.den *= 10) += ((( (*( p))))-'0'); }
				
#line 2126 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_numeric.cpp"
				
				goto _st21;
				_ctr47:
				{
#line 44 "lex/ragel_numeric.rl"
					
					ragel_num.ratio.num *= ragel_num.sign;
					ragel_type = TYPE_RATIO;
					ragel_cat  = CAT_NUMBER;
				}
				
#line 2138 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_numeric.cpp"
				
				goto _st21;
				_st21:
				if ( p == eof ) {
					if ( cs >= 14 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof21;
				st_case_21:
				switch( ( (*( p))) ) {
					case 9: {
						goto _ctr46;
					}
					case 32: {
						goto _ctr46;
					}
				}
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr19;
				}
				{
					goto _st0;
				}
				_ctr4:
				{
#line 29 "lex/ragel_numeric.rl"
					(ragel_num.vint *= 10) += ((( (*( p))))-'0'); }
				
#line 2171 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_numeric.cpp"
				
				{
#line 42 "lex/ragel_numeric.rl"
					(ragel_num.ratio.num *= 10) += ((( (*( p))))-'0'); }
				
#line 2177 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_numeric.cpp"
				
				goto _st22;
				_ctr48:
				{
#line 30 "lex/ragel_numeric.rl"
					
					ragel_num.vint *= ragel_num.sign;
					ragel_type = TYPE_INT;
					ragel_cat  = CAT_NUMBER;
				}
				
#line 2189 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_numeric.cpp"
				
				goto _st22;
				_st22:
				if ( p == eof ) {
					if ( cs >= 14 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof22;
				st_case_22:
				switch( ( (*( p))) ) {
					case 9: {
						goto _ctr27;
					}
					case 32: {
						goto _ctr27;
					}
					case 37: {
						goto _ctr28;
					}
					case 42: {
						goto _ctr29;
					}
					case 46: {
						goto _ctr30;
					}
					case 47: {
						goto _st9;
					}
					case 95: {
						goto _ctr31;
					}
					case 112: {
						goto _ctr32;
					}
				}
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr4;
				}
				{
					goto _st0;
				}
				_st10:
				if ( p == eof ) {
					if ( cs >= 14 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof10;
				st_case_10:
				if ( ( (*( p))) == 110 ) {
					goto _st11;
				}
				{
					goto _st0;
				}
				_st11:
				if ( p == eof ) {
					if ( cs >= 14 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof11;
				st_case_11:
				if ( ( (*( p))) == 102 ) {
					goto _st23;
				}
				{
					goto _st0;
				}
				_ctr49:
				{
#line 65 "lex/ragel_numeric.rl"
					
					ragel_type = TYPE_INF;
					ragel_cat = CAT_NUMBER;
				}
				
#line 2277 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_numeric.cpp"
				
				goto _st23;
				_st23:
				if ( p == eof ) {
					if ( cs >= 14 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof23;
				st_case_23:
				switch( ( (*( p))) ) {
					case 9: {
						goto _ctr50;
					}
					case 32: {
						goto _ctr50;
					}
				}
				{
					goto _st0;
				}
				_ctr3:
				{
#line 29 "lex/ragel_numeric.rl"
					(ragel_num.vint *= 10) += ((( (*( p))))-'0'); }
				
#line 2307 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_numeric.cpp"
				
				{
#line 42 "lex/ragel_numeric.rl"
					(ragel_num.ratio.num *= 10) += ((( (*( p))))-'0'); }
				
#line 2313 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_numeric.cpp"
				
				goto _st24;
				_ctr51:
				{
#line 30 "lex/ragel_numeric.rl"
					
					ragel_num.vint *= ragel_num.sign;
					ragel_type = TYPE_INT;
					ragel_cat  = CAT_NUMBER;
				}
				
#line 2325 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_numeric.cpp"
				
				goto _st24;
				_st24:
				if ( p == eof ) {
					if ( cs >= 14 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof24;
				st_case_24:
				switch( ( (*( p))) ) {
					case 9: {
						goto _ctr27;
					}
					case 32: {
						goto _ctr27;
					}
					case 37: {
						goto _ctr28;
					}
					case 42: {
						goto _ctr29;
					}
					case 46: {
						goto _ctr30;
					}
					case 47: {
						goto _st9;
					}
					case 95: {
						goto _ctr31;
					}
					case 98: {
						goto _st12;
					}
					case 112: {
						goto _ctr32;
					}
					case 120: {
						goto _st13;
					}
				}
				{
					goto _st0;
				}
				_st12:
				if ( p == eof ) {
					if ( cs >= 14 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof12;
				st_case_12:
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 49 ) {
					goto _ctr23;
				}
				{
					goto _st0;
				}
				_ctr23:
				{
#line 23 "lex/ragel_numeric.rl"
					(ragel_num.vint <<= 1) |= ((( (*( p))))=='1'); }
				
#line 2396 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_numeric.cpp"
				
				goto _st25;
				_ctr52:
				{
#line 24 "lex/ragel_numeric.rl"
					
					ragel_type = TYPE_BIN;
					ragel_cat  = CAT_NUMBER;
				}
				
#line 2407 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_numeric.cpp"
				
				goto _st25;
				_st25:
				if ( p == eof ) {
					if ( cs >= 14 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof25;
				st_case_25:
				switch( ( (*( p))) ) {
					case 9: {
						goto _ctr53;
					}
					case 32: {
						goto _ctr53;
					}
				}
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 49 ) {
					goto _ctr23;
				}
				{
					goto _st0;
				}
				_st13:
				if ( p == eof ) {
					if ( cs >= 14 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof13;
				st_case_13:
				if ( ( (*( p))) < 65 ) {
					if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
						goto _ctr25;
					}
				} else if ( ( (*( p))) > 70 ) {
					if ( 97 <= ( (*( p))) && ( (*( p))) <= 102 ) {
						goto _ctr25;
					}
				} else {
					goto _ctr25;
				}
				{
					goto _st0;
				}
				_ctr25:
				{
#line 36 "lex/ragel_numeric.rl"
					(ragel_num.vint <<= 4) |= xchar2digit((( (*( p))))); }
				
#line 2465 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_numeric.cpp"
				
				goto _st26;
				_ctr54:
				{
#line 37 "lex/ragel_numeric.rl"
					
					ragel_type = TYPE_HEX;
					ragel_cat  = CAT_NUMBER;
				}
				
#line 2476 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_numeric.cpp"
				
				goto _st26;
				_st26:
				if ( p == eof ) {
					if ( cs >= 14 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof26;
				st_case_26:
				switch( ( (*( p))) ) {
					case 9: {
						goto _ctr55;
					}
					case 32: {
						goto _ctr55;
					}
				}
				if ( ( (*( p))) < 65 ) {
					if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
						goto _ctr25;
					}
				} else if ( ( (*( p))) > 70 ) {
					if ( 97 <= ( (*( p))) && ( (*( p))) <= 102 ) {
						goto _ctr25;
					}
				} else {
					goto _ctr25;
				}
				{
					goto _st0;
				}
				st_out:
				_test_eof1: cs = 1; goto _test_eof; 
				_test_eof2: cs = 2; goto _test_eof; 
				_test_eof14: cs = 14; goto _test_eof; 
				_test_eof15: cs = 15; goto _test_eof; 
				_test_eof16: cs = 16; goto _test_eof; 
				_test_eof17: cs = 17; goto _test_eof; 
				_test_eof18: cs = 18; goto _test_eof; 
				_test_eof3: cs = 3; goto _test_eof; 
				_test_eof19: cs = 19; goto _test_eof; 
				_test_eof4: cs = 4; goto _test_eof; 
				_test_eof5: cs = 5; goto _test_eof; 
				_test_eof6: cs = 6; goto _test_eof; 
				_test_eof7: cs = 7; goto _test_eof; 
				_test_eof8: cs = 8; goto _test_eof; 
				_test_eof9: cs = 9; goto _test_eof; 
				_test_eof20: cs = 20; goto _test_eof; 
				_test_eof21: cs = 21; goto _test_eof; 
				_test_eof22: cs = 22; goto _test_eof; 
				_test_eof10: cs = 10; goto _test_eof; 
				_test_eof11: cs = 11; goto _test_eof; 
				_test_eof23: cs = 23; goto _test_eof; 
				_test_eof24: cs = 24; goto _test_eof; 
				_test_eof12: cs = 12; goto _test_eof; 
				_test_eof25: cs = 25; goto _test_eof; 
				_test_eof13: cs = 13; goto _test_eof; 
				_test_eof26: cs = 26; goto _test_eof; 
				
				_test_eof: {}
				if ( p == eof ) {
					switch ( cs ) {
						case 1: {
							break;
						}
						case 0: {
							break;
						}
						case 2: {
							break;
						}
						case 14: {
							break;
						}
						case 15: {
							{
#line 180 "lex/parser_numeric.rl"
								if((( (*( p))))!=' '&&(( (*( p))))!='\t'){{p = p - 1; }{p+= 1; cs = 15; goto _out;}}}
							
#line 2560 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_numeric.cpp"
							
							break;
						}
						case 16: {
							break;
						}
						case 17: {
							break;
						}
						case 18: {
							break;
						}
						case 3: {
							break;
						}
						case 19: {
							break;
						}
						case 4: {
							break;
						}
						case 5: {
							break;
						}
						case 6: {
							break;
						}
						case 7: {
							break;
						}
						case 8: {
							break;
						}
						case 9: {
							break;
						}
						case 20: {
							break;
						}
						case 21: {
							break;
						}
						case 22: {
							break;
						}
						case 10: {
							break;
						}
						case 11: {
							break;
						}
						case 23: {
							break;
						}
						case 24: {
							break;
						}
						case 12: {
							break;
						}
						case 25: {
							break;
						}
						case 13: {
							break;
						}
						case 26: {
							break;
						}
					}
					switch ( cs ) {
					}
					switch ( cs ) {
						case 1:
						goto _st1;case 0:
						goto _st0;case 2:
						goto _st2;case 14:
						goto _ctr26;case 15:
						goto _st15;case 16:
						goto _st16;case 17:
						goto _ctr35;case 18:
						goto _ctr37;case 3:
						goto _st3;case 19:
						goto _ctr39;case 4:
						goto _st4;case 5:
						goto _st5;case 6:
						goto _st6;case 7:
						goto _st7;case 8:
						goto _st8;case 9:
						goto _st9;case 20:
						goto _ctr45;case 21:
						goto _ctr47;case 22:
						goto _ctr48;case 10:
						goto _st10;case 11:
						goto _st11;case 23:
						goto _ctr49;case 24:
						goto _ctr51;case 12:
						goto _st12;case 25:
						goto _ctr52;case 13:
						goto _st13;case 26:
						goto _ctr54;	}
				}
				
				if ( cs >= 14 )
					goto _out; _pop: {}
				_out: {}
			}
			
#line 216 "lex/parser_numeric.rl"
			
			
			ok = cs >= 
#line 2673 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_numeric.cpp"
			14
#line 218 "lex/parser_numeric.rl"
			;
			
			if(ok) {
				switch(ragel_type) {
					case TYPE_PHASE:
					case TYPE_PERCENT:
					case TYPE_FLOAT:
					res_.push_back({});
					res_.back().f = ragel_num.getFloat();
					break;
					case TYPE_INT:
					case TYPE_HEX:
					case TYPE_BIN:
					res_.push_back({});
					res_.back().i0 = ragel_num.getInteger();
					break;
					case TYPE_RATIO:
					res_.push_back({});
					res_.back().i0 = ragel_num.ratio.num;
					res_.back().i1 = ragel_num.ratio.den;
					break;
					case TYPE_INF:
					res_.push_back({});
					res_.back().f = std::numeric_limits<typeof(res_[0].f)>::infinity();
					break;
					default:
					return false;
					break;
				}
				
				res_.back().t = ragel_type;
			}
			
			endptr = p;
			return ok;
		}
		
		size_t NumericMatchSome::parseMany(const char *str, const char *&endptr)
		{
			reset();
			
			auto pstr = str;
			const char* end = nullptr;
			while(parseSingle(pstr, end))
			pstr = end;
			
			endptr = end;
			return res_.size();
		}
		
		bool NumericMatchSome::getBinAt(size_t idx, t_int& b) const
		{
			if(!isBinAt(idx)) return false;
			b = res_[idx].i0;
			return true;
		}
		
		bool NumericMatchSome::getIntAt(size_t idx, t_int& i) const
		{
			if(!isIntAt(idx)) return false;
			i = res_[idx].i0;
			return true;
		}
		
		bool NumericMatchSome::getHexAt(size_t idx, t_int& x) const
		{
			if(!isHexAt(idx)) return false;
			x = res_[idx].i0;
			return true;
		}
		
		bool NumericMatchSome::getFloatAt(size_t idx, t_float& f) const
		{
			if(!isFloatAt(idx)) return false;
			f = res_[idx].f;
			return true;
		}
		
		bool NumericMatchSome::getRatioAt(size_t idx, t_int& num, t_int& den) const
		{
			if(!isRatioAt(idx)) return false;
			num = res_[idx].i0;
			den = res_[idx].i1;
			return true;
		}
		
		double NumericMatchSome::asFloatAt(size_t idx, double def) const
		{
			if(idx >= res_.size())
			return def;
			
			auto& n = res_[idx];
			return n.asFloat(def);
		}
		
	}
}


