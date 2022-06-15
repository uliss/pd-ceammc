#line 1 "lex/parser_color.rl"
# include "parser_color.h"

# include <cstdint>
# include <cstring>

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
		
		
#line 38 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_color.cpp"
		static const int color_start = 1;
		static const int color_first_final = 8;
		static const int color_error = 0;
		
		static const int color_en_main = 1;
		
		
#line 41 "lex/parser_color.rl"
		
		
		RgbHexFullMatch::RgbHexFullMatch()
		{
			reset();
		}
		
		void RgbHexFullMatch::reset()
		{
			color_ = {};
		}
		
		bool RgbHexFullMatch::parse(const Atom& a)
		{
			if (a.isSymbol())
				return parse(a.asT<t_symbol*>()->s_name);
			else
				return false;
		}
		
		bool RgbHexFullMatch::parse(const char* str)
		{
			const auto len = strlen(str);
			if (len == 0)
				return false;
			
			const char* p = str;
			const char* pe = p + len;
			const char* eof = pe;
			ColorRagelData color;
			AtomCategory cat_ {CAT_UNKNOWN};
			AtomType type_ = {TYPE_UNKNOWN};
			
			reset();
			
			
#line 83 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_color.cpp"
			{
				cs = (int)color_start;
			}
			
#line 76 "lex/parser_color.rl"
			
			
#line 91 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_color.cpp"
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
					case 3:
					goto st_case_3;
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
				}
				goto st_out;
				_st1:
				if ( p == eof ) {
					if ( cs >= 8 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof1;
				st_case_1:
				if ( ( (*( p))) == 35 ) {
					goto _st2;
				}
				{
					goto _st0;
				}
				st_case_0:
				_st0:
				cs = 0;
				goto _pop;
				_st2:
				if ( p == eof ) {
					if ( cs >= 8 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof2;
				st_case_2:
				if ( ( (*( p))) < 65 ) {
					if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
						goto _ctr3;
					}
				} else if ( ( (*( p))) > 70 ) {
					if ( 97 <= ( (*( p))) && ( (*( p))) <= 102 ) {
						goto _ctr3;
					}
				} else {
					goto _ctr3;
				}
				{
					goto _st0;
				}
				_ctr3:
				{
#line 5 "lex/ragel_color.rl"
					color.x = xchar2digit((( (*( p))))); }
				
#line 167 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_color.cpp"
				
				{
#line 6 "lex/ragel_color.rl"
					(color.x *= 16) += xchar2digit((( (*( p))))); }
				
#line 173 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_color.cpp"
				
				goto _st3;
				_st3:
				if ( p == eof ) {
					if ( cs >= 8 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof3;
				st_case_3:
				if ( ( (*( p))) < 65 ) {
					if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
						goto _ctr5;
					}
				} else if ( ( (*( p))) > 70 ) {
					if ( 97 <= ( (*( p))) && ( (*( p))) <= 102 ) {
						goto _ctr5;
					}
				} else {
					goto _ctr5;
				}
				{
					goto _st0;
				}
				_ctr5:
				{
#line 6 "lex/ragel_color.rl"
					(color.x *= 16) += xchar2digit((( (*( p))))); }
				
#line 206 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_color.cpp"
				
				goto _st4;
				_st4:
				if ( p == eof ) {
					if ( cs >= 8 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof4;
				st_case_4:
				if ( ( (*( p))) < 65 ) {
					if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
						goto _ctr7;
					}
				} else if ( ( (*( p))) > 70 ) {
					if ( 97 <= ( (*( p))) && ( (*( p))) <= 102 ) {
						goto _ctr7;
					}
				} else {
					goto _ctr7;
				}
				{
					goto _st0;
				}
				_ctr7:
				{
#line 11 "lex/ragel_color.rl"
					color.r = color.x;}
				
#line 239 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_color.cpp"
				
				{
#line 5 "lex/ragel_color.rl"
					color.x = xchar2digit((( (*( p))))); }
				
#line 245 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_color.cpp"
				
				{
#line 6 "lex/ragel_color.rl"
					(color.x *= 16) += xchar2digit((( (*( p))))); }
				
#line 251 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_color.cpp"
				
				goto _st5;
				_st5:
				if ( p == eof ) {
					if ( cs >= 8 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof5;
				st_case_5:
				if ( ( (*( p))) < 65 ) {
					if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
						goto _ctr9;
					}
				} else if ( ( (*( p))) > 70 ) {
					if ( 97 <= ( (*( p))) && ( (*( p))) <= 102 ) {
						goto _ctr9;
					}
				} else {
					goto _ctr9;
				}
				{
					goto _st0;
				}
				_ctr9:
				{
#line 6 "lex/ragel_color.rl"
					(color.x *= 16) += xchar2digit((( (*( p))))); }
				
#line 284 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_color.cpp"
				
				goto _st6;
				_st6:
				if ( p == eof ) {
					if ( cs >= 8 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof6;
				st_case_6:
				if ( ( (*( p))) < 65 ) {
					if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
						goto _ctr11;
					}
				} else if ( ( (*( p))) > 70 ) {
					if ( 97 <= ( (*( p))) && ( (*( p))) <= 102 ) {
						goto _ctr11;
					}
				} else {
					goto _ctr11;
				}
				{
					goto _st0;
				}
				_ctr11:
				{
#line 12 "lex/ragel_color.rl"
					color.g = color.x;}
				
#line 317 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_color.cpp"
				
				{
#line 5 "lex/ragel_color.rl"
					color.x = xchar2digit((( (*( p))))); }
				
#line 323 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_color.cpp"
				
				{
#line 6 "lex/ragel_color.rl"
					(color.x *= 16) += xchar2digit((( (*( p))))); }
				
#line 329 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_color.cpp"
				
				goto _st7;
				_st7:
				if ( p == eof ) {
					if ( cs >= 8 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof7;
				st_case_7:
				if ( ( (*( p))) < 65 ) {
					if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
						goto _ctr13;
					}
				} else if ( ( (*( p))) > 70 ) {
					if ( 97 <= ( (*( p))) && ( (*( p))) <= 102 ) {
						goto _ctr13;
					}
				} else {
					goto _ctr13;
				}
				{
					goto _st0;
				}
				_ctr13:
				{
#line 6 "lex/ragel_color.rl"
					(color.x *= 16) += xchar2digit((( (*( p))))); }
				
#line 362 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_color.cpp"
				
				goto _st8;
				_ctr14:
				{
#line 13 "lex/ragel_color.rl"
					color.b = color.x;}
				
#line 370 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_color.cpp"
				
				{
#line 14 "lex/ragel_color.rl"
					cat_ = CAT_COLOR; type_ = TYPE_HEX; }
				
#line 376 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_color.cpp"
				
				goto _st8;
				_st8:
				if ( p == eof ) {
					if ( cs >= 8 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof8;
				st_case_8:
				{
					goto _st0;
				}
				st_out:
				_test_eof1: cs = 1; goto _test_eof; 
				_test_eof2: cs = 2; goto _test_eof; 
				_test_eof3: cs = 3; goto _test_eof; 
				_test_eof4: cs = 4; goto _test_eof; 
				_test_eof5: cs = 5; goto _test_eof; 
				_test_eof6: cs = 6; goto _test_eof; 
				_test_eof7: cs = 7; goto _test_eof; 
				_test_eof8: cs = 8; goto _test_eof; 
				
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
						case 3: {
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
					}
					switch ( cs ) {
					}
					switch ( cs ) {
						case 1:
						goto _st1;case 0:
						goto _st0;case 2:
						goto _st2;case 3:
						goto _st3;case 4:
						goto _st4;case 5:
						goto _st5;case 6:
						goto _st6;case 7:
						goto _st7;case 8:
						goto _ctr14;	}
				}
				
				if ( cs >= 8 )
					goto _out; _pop: {}
				_out: {}
			}
			
#line 77 "lex/parser_color.rl"
			
			
			const auto ok = cs >= 
#line 458 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_color.cpp"
			8
#line 79 "lex/parser_color.rl"
			;
			if(ok) {
				color_.r = color.r;
				color_.g = color.g;
				color_.b = color.b;
			}
			
			return ok;
		}
		
		size_t RgbHexFullMatch::parse(const AtomListView& lv, SmallColorVec& out)
		{
			const size_t N = lv.size();
			
			for (size_t i = 0; i < N; i++) {
				const auto& a = lv[i];
				if (!parse(a))
					return i;
				
				out.push_back(asInt());
			}
			
			return N;
		}
		
	}
}


