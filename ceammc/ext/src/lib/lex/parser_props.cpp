#line 1 "lex/parser_props.rl"
# include "ceammc_atomlist_view.h"
# include "parser_props.h"

# include <cstdlib>
# include <cstring>

namespace ceammc {
	namespace parser {
		
		
#line 13 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_props.cpp"
		static const int bool_prop_start = 1;
		static const int bool_prop_first_final = 19;
		static const int bool_prop_error = 0;
		
		static const int bool_prop_en_main = 1;
		
		
#line 22 "lex/parser_props.rl"
		
		
		BoolPropOp parse_bool_prop(const char* str)
		{
			const auto len = std::strlen(str);
			if (len == 0)
				return BoolPropOp::UNKNOWN;
			
			int cs = 0;
			const char* p = str;
			const char* pe = p + len;
			const char* eof = pe;
			BoolPropOp type = BoolPropOp::UNKNOWN;
			
			
#line 37 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_props.cpp"
			{
				cs = (int)bool_prop_start;
			}
			
#line 36 "lex/parser_props.rl"
			
			
#line 45 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_props.cpp"
			{
				if ( p == pe )
					goto _test_eof;
				switch ( cs ) {
					case 1:
					goto st_case_1;
					case 0:
					goto st_case_0;
					case 19:
					goto st_case_19;
					case 2:
					goto st_case_2;
					case 3:
					goto st_case_3;
					case 4:
					goto st_case_4;
					case 5:
					goto st_case_5;
					case 20:
					goto st_case_20;
					case 6:
					goto st_case_6;
					case 7:
					goto st_case_7;
					case 8:
					goto st_case_8;
					case 21:
					goto st_case_21;
					case 9:
					goto st_case_9;
					case 10:
					goto st_case_10;
					case 22:
					goto st_case_22;
					case 11:
					goto st_case_11;
					case 12:
					goto st_case_12;
					case 13:
					goto st_case_13;
					case 23:
					goto st_case_23;
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
					case 24:
					goto st_case_24;
				}
				goto st_out;
				_st1:
				if ( p == eof ) {
					if ( cs >= 19 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof1;
				st_case_1:
				switch( ( (*( p))) ) {
					case 33: {
						goto _st19;
					}
					case 70: {
						goto _st2;
					}
					case 84: {
						goto _st6;
					}
					case 100: {
						goto _st9;
					}
					case 102: {
						goto _st2;
					}
					case 114: {
						goto _st14;
					}
					case 116: {
						goto _st6;
					}
					case 126: {
						goto _st19;
					}
				}
				{
					goto _st0;
				}
				st_case_0:
				_st0:
				cs = 0;
				goto _pop;
				_ctr25:
				{
#line 16 "lex/parser_props.rl"
					type = BoolPropOp::INVERT; }
				
#line 151 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_props.cpp"
				
				goto _st19;
				_st19:
				if ( p == eof ) {
					if ( cs >= 19 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof19;
				st_case_19:
				{
					goto _st0;
				}
				_st2:
				if ( p == eof ) {
					if ( cs >= 19 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof2;
				st_case_2:
				if ( ( (*( p))) == 97 ) {
					goto _st3;
				}
				{
					goto _st0;
				}
				_st3:
				if ( p == eof ) {
					if ( cs >= 19 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof3;
				st_case_3:
				if ( ( (*( p))) == 108 ) {
					goto _st4;
				}
				{
					goto _st0;
				}
				_st4:
				if ( p == eof ) {
					if ( cs >= 19 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof4;
				st_case_4:
				if ( ( (*( p))) == 115 ) {
					goto _st5;
				}
				{
					goto _st0;
				}
				_st5:
				if ( p == eof ) {
					if ( cs >= 19 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof5;
				st_case_5:
				if ( ( (*( p))) == 101 ) {
					goto _st20;
				}
				{
					goto _st0;
				}
				_ctr26:
				{
#line 14 "lex/parser_props.rl"
					type = BoolPropOp::FALSE; }
				
#line 241 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_props.cpp"
				
				goto _st20;
				_st20:
				if ( p == eof ) {
					if ( cs >= 19 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof20;
				st_case_20:
				{
					goto _st0;
				}
				_st6:
				if ( p == eof ) {
					if ( cs >= 19 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof6;
				st_case_6:
				if ( ( (*( p))) == 114 ) {
					goto _st7;
				}
				{
					goto _st0;
				}
				_st7:
				if ( p == eof ) {
					if ( cs >= 19 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof7;
				st_case_7:
				if ( ( (*( p))) == 117 ) {
					goto _st8;
				}
				{
					goto _st0;
				}
				_st8:
				if ( p == eof ) {
					if ( cs >= 19 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof8;
				st_case_8:
				if ( ( (*( p))) == 101 ) {
					goto _st21;
				}
				{
					goto _st0;
				}
				_ctr27:
				{
#line 13 "lex/parser_props.rl"
					type = BoolPropOp::TRUE; }
				
#line 314 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_props.cpp"
				
				goto _st21;
				_st21:
				if ( p == eof ) {
					if ( cs >= 19 )
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
				_st9:
				if ( p == eof ) {
					if ( cs >= 19 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof9;
				st_case_9:
				if ( ( (*( p))) == 101 ) {
					goto _st10;
				}
				{
					goto _st0;
				}
				_st10:
				if ( p == eof ) {
					if ( cs >= 19 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof10;
				st_case_10:
				if ( ( (*( p))) == 102 ) {
					goto _st22;
				}
				{
					goto _st0;
				}
				_ctr28:
				{
#line 17 "lex/parser_props.rl"
					type = BoolPropOp::DEFAULT; }
				
#line 370 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_props.cpp"
				
				goto _st22;
				_st22:
				if ( p == eof ) {
					if ( cs >= 19 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof22;
				st_case_22:
				if ( ( (*( p))) == 97 ) {
					goto _st11;
				}
				{
					goto _st0;
				}
				_st11:
				if ( p == eof ) {
					if ( cs >= 19 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof11;
				st_case_11:
				if ( ( (*( p))) == 117 ) {
					goto _st12;
				}
				{
					goto _st0;
				}
				_st12:
				if ( p == eof ) {
					if ( cs >= 19 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof12;
				st_case_12:
				if ( ( (*( p))) == 108 ) {
					goto _st13;
				}
				{
					goto _st0;
				}
				_st13:
				if ( p == eof ) {
					if ( cs >= 19 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof13;
				st_case_13:
				if ( ( (*( p))) == 116 ) {
					goto _st23;
				}
				{
					goto _st0;
				}
				_ctr29:
				{
#line 17 "lex/parser_props.rl"
					type = BoolPropOp::DEFAULT; }
				
#line 446 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_props.cpp"
				
				goto _st23;
				_st23:
				if ( p == eof ) {
					if ( cs >= 19 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof23;
				st_case_23:
				{
					goto _st0;
				}
				_st14:
				if ( p == eof ) {
					if ( cs >= 19 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof14;
				st_case_14:
				if ( ( (*( p))) == 97 ) {
					goto _st15;
				}
				{
					goto _st0;
				}
				_st15:
				if ( p == eof ) {
					if ( cs >= 19 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof15;
				st_case_15:
				if ( ( (*( p))) == 110 ) {
					goto _st16;
				}
				{
					goto _st0;
				}
				_st16:
				if ( p == eof ) {
					if ( cs >= 19 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof16;
				st_case_16:
				if ( ( (*( p))) == 100 ) {
					goto _st17;
				}
				{
					goto _st0;
				}
				_st17:
				if ( p == eof ) {
					if ( cs >= 19 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof17;
				st_case_17:
				if ( ( (*( p))) == 111 ) {
					goto _st18;
				}
				{
					goto _st0;
				}
				_st18:
				if ( p == eof ) {
					if ( cs >= 19 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof18;
				st_case_18:
				if ( ( (*( p))) == 109 ) {
					goto _st24;
				}
				{
					goto _st0;
				}
				_ctr30:
				{
#line 15 "lex/parser_props.rl"
					type = BoolPropOp::RANDOM; }
				
#line 553 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_props.cpp"
				
				goto _st24;
				_st24:
				if ( p == eof ) {
					if ( cs >= 19 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof24;
				st_case_24:
				{
					goto _st0;
				}
				st_out:
				_test_eof1: cs = 1; goto _test_eof; 
				_test_eof19: cs = 19; goto _test_eof; 
				_test_eof2: cs = 2; goto _test_eof; 
				_test_eof3: cs = 3; goto _test_eof; 
				_test_eof4: cs = 4; goto _test_eof; 
				_test_eof5: cs = 5; goto _test_eof; 
				_test_eof20: cs = 20; goto _test_eof; 
				_test_eof6: cs = 6; goto _test_eof; 
				_test_eof7: cs = 7; goto _test_eof; 
				_test_eof8: cs = 8; goto _test_eof; 
				_test_eof21: cs = 21; goto _test_eof; 
				_test_eof9: cs = 9; goto _test_eof; 
				_test_eof10: cs = 10; goto _test_eof; 
				_test_eof22: cs = 22; goto _test_eof; 
				_test_eof11: cs = 11; goto _test_eof; 
				_test_eof12: cs = 12; goto _test_eof; 
				_test_eof13: cs = 13; goto _test_eof; 
				_test_eof23: cs = 23; goto _test_eof; 
				_test_eof14: cs = 14; goto _test_eof; 
				_test_eof15: cs = 15; goto _test_eof; 
				_test_eof16: cs = 16; goto _test_eof; 
				_test_eof17: cs = 17; goto _test_eof; 
				_test_eof18: cs = 18; goto _test_eof; 
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
						case 19: {
							break;
						}
						case 2: {
							break;
						}
						case 3: {
							break;
						}
						case 4: {
							break;
						}
						case 5: {
							break;
						}
						case 20: {
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
						case 21: {
							break;
						}
						case 9: {
							break;
						}
						case 10: {
							break;
						}
						case 22: {
							break;
						}
						case 11: {
							break;
						}
						case 12: {
							break;
						}
						case 13: {
							break;
						}
						case 23: {
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
						case 17: {
							break;
						}
						case 18: {
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
						goto _st0;case 19:
						goto _ctr25;case 2:
						goto _st2;case 3:
						goto _st3;case 4:
						goto _st4;case 5:
						goto _st5;case 20:
						goto _ctr26;case 6:
						goto _st6;case 7:
						goto _st7;case 8:
						goto _st8;case 21:
						goto _ctr27;case 9:
						goto _st9;case 10:
						goto _st10;case 22:
						goto _ctr28;case 11:
						goto _st11;case 12:
						goto _st12;case 13:
						goto _st13;case 23:
						goto _ctr29;case 14:
						goto _st14;case 15:
						goto _st15;case 16:
						goto _st16;case 17:
						goto _st17;case 18:
						goto _st18;case 24:
						goto _ctr30;	}
				}
				
				if ( cs >= 19 )
					goto _out; _pop: {}
				_out: {}
			}
			
#line 37 "lex/parser_props.rl"
			
			
			const bool ok = cs >= 
#line 715 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_props.cpp"
			19
#line 39 "lex/parser_props.rl"
			;
			if (ok)
				return type;
			else
				return BoolPropOp::UNKNOWN;
		}
		
		
#line 726 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_props.cpp"
		static const int numeric_prop_start = 1;
		static const int numeric_prop_first_final = 12;
		static const int numeric_prop_error = 0;
		
		static const int numeric_prop_en_main = 1;
		
		
#line 60 "lex/parser_props.rl"
		
		
		NumericPropOp parse_numeric_prop_op(const char* str)
		{
			const auto len = std::strlen(str);
			if (len == 0)
				return NumericPropOp::UNKNOWN;
			
			int cs = 0;
			const char* p = str;
			const char* pe = p + len;
			const char* eof = pe;
			NumericPropOp type = NumericPropOp::UNKNOWN;
			
			
#line 750 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_props.cpp"
			{
				cs = (int)numeric_prop_start;
			}
			
#line 74 "lex/parser_props.rl"
			
			
#line 758 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_props.cpp"
			{
				if ( p == pe )
					goto _test_eof;
				switch ( cs ) {
					case 1:
					goto st_case_1;
					case 0:
					goto st_case_0;
					case 12:
					goto st_case_12;
					case 13:
					goto st_case_13;
					case 14:
					goto st_case_14;
					case 15:
					goto st_case_15;
					case 16:
					goto st_case_16;
					case 2:
					goto st_case_2;
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
					case 18:
					goto st_case_18;
					case 7:
					goto st_case_7;
					case 8:
					goto st_case_8;
					case 9:
					goto st_case_9;
					case 10:
					goto st_case_10;
					case 11:
					goto st_case_11;
					case 19:
					goto st_case_19;
				}
				goto st_out;
				_st1:
				if ( p == eof ) {
					if ( cs >= 12 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof1;
				st_case_1:
				switch( ( (*( p))) ) {
					case 37: {
						goto _st12;
					}
					case 42: {
						goto _st13;
					}
					case 43: {
						goto _st14;
					}
					case 45: {
						goto _st15;
					}
					case 47: {
						goto _st16;
					}
					case 100: {
						goto _st2;
					}
					case 114: {
						goto _st7;
					}
				}
				{
					goto _st0;
				}
				st_case_0:
				_st0:
				cs = 0;
				goto _pop;
				_ctr20:
				{
#line 53 "lex/parser_props.rl"
					type = NumericPropOp::MOD; }
				
#line 851 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_props.cpp"
				
				goto _st12;
				_st12:
				if ( p == eof ) {
					if ( cs >= 12 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof12;
				st_case_12:
				{
					goto _st0;
				}
				_ctr21:
				{
#line 51 "lex/parser_props.rl"
					type = NumericPropOp::MUL; }
				
#line 873 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_props.cpp"
				
				goto _st13;
				_st13:
				if ( p == eof ) {
					if ( cs >= 12 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof13;
				st_case_13:
				{
					goto _st0;
				}
				_ctr22:
				{
#line 49 "lex/parser_props.rl"
					type = NumericPropOp::ADD; }
				
#line 895 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_props.cpp"
				
				goto _st14;
				_st14:
				if ( p == eof ) {
					if ( cs >= 12 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof14;
				st_case_14:
				{
					goto _st0;
				}
				_ctr23:
				{
#line 50 "lex/parser_props.rl"
					type = NumericPropOp::SUB; }
				
#line 917 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_props.cpp"
				
				goto _st15;
				_st15:
				if ( p == eof ) {
					if ( cs >= 12 )
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
				_ctr24:
				{
#line 52 "lex/parser_props.rl"
					type = NumericPropOp::DIV; }
				
#line 939 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_props.cpp"
				
				goto _st16;
				_st16:
				if ( p == eof ) {
					if ( cs >= 12 )
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
				_st2:
				if ( p == eof ) {
					if ( cs >= 12 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof2;
				st_case_2:
				if ( ( (*( p))) == 101 ) {
					goto _st3;
				}
				{
					goto _st0;
				}
				_st3:
				if ( p == eof ) {
					if ( cs >= 12 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof3;
				st_case_3:
				if ( ( (*( p))) == 102 ) {
					goto _st17;
				}
				{
					goto _st0;
				}
				_ctr25:
				{
#line 55 "lex/parser_props.rl"
					type = NumericPropOp::DEFAULT; }
				
#line 995 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_props.cpp"
				
				goto _st17;
				_st17:
				if ( p == eof ) {
					if ( cs >= 12 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof17;
				st_case_17:
				if ( ( (*( p))) == 97 ) {
					goto _st4;
				}
				{
					goto _st0;
				}
				_st4:
				if ( p == eof ) {
					if ( cs >= 12 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof4;
				st_case_4:
				if ( ( (*( p))) == 117 ) {
					goto _st5;
				}
				{
					goto _st0;
				}
				_st5:
				if ( p == eof ) {
					if ( cs >= 12 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof5;
				st_case_5:
				if ( ( (*( p))) == 108 ) {
					goto _st6;
				}
				{
					goto _st0;
				}
				_st6:
				if ( p == eof ) {
					if ( cs >= 12 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof6;
				st_case_6:
				if ( ( (*( p))) == 116 ) {
					goto _st18;
				}
				{
					goto _st0;
				}
				_ctr26:
				{
#line 55 "lex/parser_props.rl"
					type = NumericPropOp::DEFAULT; }
				
#line 1071 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_props.cpp"
				
				goto _st18;
				_st18:
				if ( p == eof ) {
					if ( cs >= 12 )
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
				_st7:
				if ( p == eof ) {
					if ( cs >= 12 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof7;
				st_case_7:
				if ( ( (*( p))) == 97 ) {
					goto _st8;
				}
				{
					goto _st0;
				}
				_st8:
				if ( p == eof ) {
					if ( cs >= 12 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof8;
				st_case_8:
				if ( ( (*( p))) == 110 ) {
					goto _st9;
				}
				{
					goto _st0;
				}
				_st9:
				if ( p == eof ) {
					if ( cs >= 12 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof9;
				st_case_9:
				if ( ( (*( p))) == 100 ) {
					goto _st10;
				}
				{
					goto _st0;
				}
				_st10:
				if ( p == eof ) {
					if ( cs >= 12 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof10;
				st_case_10:
				if ( ( (*( p))) == 111 ) {
					goto _st11;
				}
				{
					goto _st0;
				}
				_st11:
				if ( p == eof ) {
					if ( cs >= 12 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof11;
				st_case_11:
				if ( ( (*( p))) == 109 ) {
					goto _st19;
				}
				{
					goto _st0;
				}
				_ctr27:
				{
#line 54 "lex/parser_props.rl"
					type = NumericPropOp::RANDOM; }
				
#line 1178 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_props.cpp"
				
				goto _st19;
				_st19:
				if ( p == eof ) {
					if ( cs >= 12 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof19;
				st_case_19:
				{
					goto _st0;
				}
				st_out:
				_test_eof1: cs = 1; goto _test_eof; 
				_test_eof12: cs = 12; goto _test_eof; 
				_test_eof13: cs = 13; goto _test_eof; 
				_test_eof14: cs = 14; goto _test_eof; 
				_test_eof15: cs = 15; goto _test_eof; 
				_test_eof16: cs = 16; goto _test_eof; 
				_test_eof2: cs = 2; goto _test_eof; 
				_test_eof3: cs = 3; goto _test_eof; 
				_test_eof17: cs = 17; goto _test_eof; 
				_test_eof4: cs = 4; goto _test_eof; 
				_test_eof5: cs = 5; goto _test_eof; 
				_test_eof6: cs = 6; goto _test_eof; 
				_test_eof18: cs = 18; goto _test_eof; 
				_test_eof7: cs = 7; goto _test_eof; 
				_test_eof8: cs = 8; goto _test_eof; 
				_test_eof9: cs = 9; goto _test_eof; 
				_test_eof10: cs = 10; goto _test_eof; 
				_test_eof11: cs = 11; goto _test_eof; 
				_test_eof19: cs = 19; goto _test_eof; 
				
				_test_eof: {}
				if ( p == eof ) {
					switch ( cs ) {
						case 1: {
							break;
						}
						case 0: {
							break;
						}
						case 12: {
							break;
						}
						case 13: {
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
						case 2: {
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
						case 18: {
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
						case 10: {
							break;
						}
						case 11: {
							break;
						}
						case 19: {
							break;
						}
					}
					switch ( cs ) {
					}
					switch ( cs ) {
						case 1:
						goto _st1;case 0:
						goto _st0;case 12:
						goto _ctr20;case 13:
						goto _ctr21;case 14:
						goto _ctr22;case 15:
						goto _ctr23;case 16:
						goto _ctr24;case 2:
						goto _st2;case 3:
						goto _st3;case 17:
						goto _ctr25;case 4:
						goto _st4;case 5:
						goto _st5;case 6:
						goto _st6;case 18:
						goto _ctr26;case 7:
						goto _st7;case 8:
						goto _st8;case 9:
						goto _st9;case 10:
						goto _st10;case 11:
						goto _st11;case 19:
						goto _ctr27;	}
				}
				
				if ( cs >= 12 )
					goto _out; _pop: {}
				_out: {}
			}
			
#line 75 "lex/parser_props.rl"
			
			
			const bool ok = cs >= 
#line 1315 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_props.cpp"
			12
#line 77 "lex/parser_props.rl"
			;
			if (ok)
				return type;
			else
				return NumericPropOp::UNKNOWN;
		}
		
	}
}
