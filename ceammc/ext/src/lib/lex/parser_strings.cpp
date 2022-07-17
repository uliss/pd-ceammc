#line 1 "lex/parser_strings.rl"
# include "parser_strings.h"
# include "ceammc_format.h"
# include "ceammc_log.h"
# include "fmt/format.h"


#line 9 "lex/parser_strings.cpp"
static const int quoted_string_end_start = 1;
static const int quoted_string_end_first_final = 4;
static const int quoted_string_end_error = 0;

static const int quoted_string_end_en_main = 1;


#line 16 "lex/parser_strings.rl"


namespace ceammc {
	namespace string {
		
		bool quoted_string_end(const Atom& a) {
			if (a.isSymbol())
				return quoted_string_end(a.asT<t_symbol*>());
			else
				return false;
		}
		
		bool quoted_string_end(const std::string& str) {
			return quoted_string_end(str.c_str());
		}
		
		bool quoted_string_end(t_symbol* sym) {
			if (!sym)
				return false;
			else
				return quoted_string_end(sym->s_name);
		}
		
		bool quoted_string_end(const char* str) {
			if (str == nullptr)
				return false;
			
			int cs = 0;
			const char* p = str;
			
			
#line 49 "lex/parser_strings.cpp"
			{
				cs = (int)quoted_string_end_start;
			}
			
#line 46 "lex/parser_strings.rl"
			
			
#line 57 "lex/parser_strings.cpp"
			{
				switch ( cs ) {
					case 1:
					goto st_case_1;
					case 0:
					goto st_case_0;
					case 2:
					goto st_case_2;
					case 4:
					goto st_case_4;
					case 3:
					goto st_case_3;
				}
				goto st_out;
				_st1:
				p += 1;
				st_case_1:
				switch( ( (*( p))) ) {
					case 0: {
						goto _st0;
					}
					case 34: {
						goto _st2;
					}
					case 96: {
						goto _st3;
					}
				}
				{
					goto _st1;
				}
				st_case_0:
				_st0:
				cs = 0;
				goto _pop;
				_st2:
				p += 1;
				st_case_2:
				if ( ( (*( p))) == 0 ) {
					goto _ctr4;
				}
				{
					goto _st0;
				}
				_ctr4:
				{
#line 14 "lex/parser_strings.rl"
					{p+= 1; cs = 4; goto _out;} }
				
#line 107 "lex/parser_strings.cpp"
				
				goto _st4;
				_st4:
				p += 1;
				st_case_4:
				{
					goto _st0;
				}
				_st3:
				p += 1;
				st_case_3:
				switch( ( (*( p))) ) {
					case 34: {
						goto _st1;
					}
					case 96: {
						goto _st1;
					}
				}
				if ( 40 <= ( (*( p))) && ( (*( p))) <= 41 ) {
					goto _st1;
				}
				{
					goto _st0;
				}
				st_out:
				_test_eof1: cs = 1; goto _test_eof; 
				_test_eof2: cs = 2; goto _test_eof; 
				_test_eof4: cs = 4; goto _test_eof; 
				_test_eof3: cs = 3; goto _test_eof; 
				
				_test_eof: {}
				if ( cs >= 4 )
					goto _out; _pop: {}
				_out: {}
			}
			
#line 47 "lex/parser_strings.rl"
			
			
			return (cs >= 
#line 149 "lex/parser_strings.cpp"
			4
#line 49 "lex/parser_strings.rl"
			);
		}
		
		
#line 156 "lex/parser_strings.cpp"
		static const int is_quoted_string_start = 1;
		static const int is_quoted_string_first_final = 5;
		static const int is_quoted_string_error = 0;
		
		static const int is_quoted_string_en_main = 1;
		
		
#line 60 "lex/parser_strings.rl"
		
		
		bool is_quoted_string(const AtomListView& lv) {
			auto N = lv.size();
			if (N == 1 && lv[0].isSymbol())
				return is_quoted_string(lv[0].asT<t_symbol*>()->s_name);
			else if(N > 1
				&& lv[0].isSymbol()
			&& lv[0].asT<t_symbol*>()
			&& lv[0].asT<t_symbol*>()->s_name
			&& lv[0].asT<t_symbol*>()->s_name[0] == '"'
			&& lv[N-1].isSymbol())
			return is_quoted_string(to_string(lv));
			return false;
		}
		
		bool is_quoted_string(t_symbol* s) {
			if (!s)
				return false;
			else
				return is_quoted_string(s->s_name);
		}
		
		bool is_quoted_string(const std::string& str) {
			return is_quoted_string(str.c_str());
		}
		
		bool is_quoted_string(const char* str) {
			if (str == nullptr)
				return false;
			
			int cs = 0;
			const char* p = str;
			
			
#line 200 "lex/parser_strings.cpp"
			{
				cs = (int)is_quoted_string_start;
			}
			
#line 94 "lex/parser_strings.rl"
			
			
#line 208 "lex/parser_strings.cpp"
			{
				switch ( cs ) {
					case 1:
					goto st_case_1;
					case 0:
					goto st_case_0;
					case 2:
					goto st_case_2;
					case 3:
					goto st_case_3;
					case 5:
					goto st_case_5;
					case 4:
					goto st_case_4;
				}
				goto st_out;
				p += 1;
				st_case_1:
				if ( ( (*( p))) == 34 ) {
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
				p += 1;
				st_case_2:
				switch( ( (*( p))) ) {
					case 0: {
						goto _st0;
					}
					case 34: {
						goto _st3;
					}
					case 96: {
						goto _st4;
					}
				}
				{
					goto _st2;
				}
				_st3:
				p += 1;
				st_case_3:
				if ( ( (*( p))) == 0 ) {
					goto _ctr5;
				}
				{
					goto _st0;
				}
				_ctr5:
				{
#line 58 "lex/parser_strings.rl"
					{p+= 1; cs = 5; goto _out;} }
				
#line 268 "lex/parser_strings.cpp"
				
				goto _st5;
				_st5:
				p += 1;
				st_case_5:
				{
					goto _st0;
				}
				_st4:
				p += 1;
				st_case_4:
				switch( ( (*( p))) ) {
					case 34: {
						goto _st2;
					}
					case 96: {
						goto _st2;
					}
				}
				if ( 40 <= ( (*( p))) && ( (*( p))) <= 41 ) {
					goto _st2;
				}
				{
					goto _st0;
				}
				st_out:
				_test_eof1: cs = 1; goto _test_eof; 
				_test_eof2: cs = 2; goto _test_eof; 
				_test_eof3: cs = 3; goto _test_eof; 
				_test_eof5: cs = 5; goto _test_eof; 
				_test_eof4: cs = 4; goto _test_eof; 
				
				_test_eof: {}
				if ( cs >= 5 )
					goto _out; _pop: {}
				_out: {}
			}
			
#line 95 "lex/parser_strings.rl"
			
			
			return (cs >= 
#line 311 "lex/parser_strings.cpp"
			5
#line 97 "lex/parser_strings.rl"
			);
		}
		
		
#line 318 "lex/parser_strings.cpp"
		static const int need_quotes_start = 1;
		static const int need_quotes_first_final = 2;
		static const int need_quotes_error = 0;
		
		static const int need_quotes_en_main = 1;
		
		
#line 107 "lex/parser_strings.rl"
		
		
		bool string_need_quotes(const char* str) {
			if (str == nullptr || str[0] == '\0')
				return false;
			
			int cs = 0;
			const char* p = str;
			
			
#line 337 "lex/parser_strings.cpp"
			{
				cs = (int)need_quotes_start;
			}
			
#line 116 "lex/parser_strings.rl"
			
			
#line 345 "lex/parser_strings.cpp"
			{
				switch ( cs ) {
					case 1:
					goto st_case_1;
					case 2:
					goto st_case_2;
					case 0:
					goto st_case_0;
				}
				goto st_out;
				_st1:
				p += 1;
				st_case_1:
				switch( ( (*( p))) ) {
					case 0: {
						goto _ctr2;
					}
					case 32: {
						goto _st0;
					}
					case 37: {
						goto _st0;
					}
					case 44: {
						goto _st0;
					}
					case 64: {
						goto _st0;
					}
					case 96: {
						goto _st0;
					}
				}
				if ( ( (*( p))) < 40 ) {
					if ( 34 <= ( (*( p))) && ( (*( p))) <= 35 ) {
						goto _st0;
					}
				} else if ( ( (*( p))) > 41 ) {
					if ( ( (*( p))) > 59 ) {
						if ( 91 <= ( (*( p))) && ( (*( p))) <= 93 ) {
							goto _st0;
						}
					} else if ( ( (*( p))) >= 58 ) {
						goto _st0;
					}
				} else {
					goto _st0;
				}
				{
					goto _st1;
				}
				_ctr2:
				{
#line 105 "lex/parser_strings.rl"
					{p+= 1; cs = 2; goto _out;} }
				
#line 402 "lex/parser_strings.cpp"
				
				goto _st2;
				_st2:
				p += 1;
				st_case_2:
				{
					goto _st0;
				}
				st_case_0:
				_st0:
				cs = 0;
				goto _pop;
				st_out:
				_test_eof1: cs = 1; goto _test_eof; 
				_test_eof2: cs = 2; goto _test_eof; 
				
				_test_eof: {}
				if ( cs >= 2 )
					goto _out; _pop: {}
				_out: {}
			}
			
#line 117 "lex/parser_strings.rl"
			
			
			return (cs < 
#line 429 "lex/parser_strings.cpp"
			2
#line 119 "lex/parser_strings.rl"
			);
		}
		
		
#line 436 "lex/parser_strings.cpp"
		static const int escape_and_quote_start = 1;
		static const int escape_and_quote_first_final = 3;
		static const int escape_and_quote_error = 0;
		
		static const int escape_and_quote_en_main = 1;
		
		
#line 150 "lex/parser_strings.rl"
		
		
		template <typename T>
		static int escape_and_quote_t(const char* str, T& out) noexcept
		{
			try {
				if (str == nullptr || str[0] == '\0') {
					out.push_back('"');
					out.push_back('"');
					return 0;
				}
				
				int cs = 0;
				const char* p = str;
				int rl_esc_count = 0;
				
				out.push_back('"');
				
				
#line 464 "lex/parser_strings.cpp"
				{
					cs = (int)escape_and_quote_start;
				}
				
#line 168 "lex/parser_strings.rl"
				
				
#line 472 "lex/parser_strings.cpp"
				{
					switch ( cs ) {
						case 1:
						goto st_case_1;
						case 3:
						goto st_case_3;
						case 0:
						goto st_case_0;
						case 2:
						goto st_case_2;
					}
					goto st_out;
					_ctr2:
					{
#line 146 "lex/parser_strings.rl"
						out.push_back((( (*( p))))); }
					
#line 490 "lex/parser_strings.cpp"
					
					goto _st1;
					_ctr21:
					{
#line 144 "lex/parser_strings.rl"
						rl_esc_count++; }
					
#line 498 "lex/parser_strings.cpp"
					
					{
#line 146 "lex/parser_strings.rl"
						out.push_back((( (*( p))))); }
					
#line 504 "lex/parser_strings.cpp"
					
					goto _st1;
					_st1:
					p += 1;
					st_case_1:
					switch( ( (*( p))) ) {
						case 0: {
							goto _ctr3;
						}
						case 32: {
							goto _ctr4;
						}
						case 34: {
							goto _ctr5;
						}
						case 35: {
							goto _ctr6;
						}
						case 37: {
							goto _ctr7;
						}
						case 40: {
							goto _ctr8;
						}
						case 41: {
							goto _ctr9;
						}
						case 44: {
							goto _ctr10;
						}
						case 58: {
							goto _ctr11;
						}
						case 59: {
							goto _ctr12;
						}
						case 64: {
							goto _ctr13;
						}
						case 91: {
							goto _ctr14;
						}
						case 92: {
							goto _ctr15;
						}
						case 93: {
							goto _ctr16;
						}
						case 96: {
							goto _ctr17;
						}
						case 123: {
							goto _ctr18;
						}
						case 125: {
							goto _ctr19;
						}
					}
					{
						goto _ctr2;
					}
					_ctr3:
					{
#line 148 "lex/parser_strings.rl"
						{p+= 1; cs = 3; goto _out;} }
					
#line 571 "lex/parser_strings.cpp"
					
					goto _st3;
					_ctr22:
					{
#line 144 "lex/parser_strings.rl"
						rl_esc_count++; }
					
#line 579 "lex/parser_strings.cpp"
					
					{
#line 148 "lex/parser_strings.rl"
						{p+= 1; cs = 3; goto _out;} }
					
#line 585 "lex/parser_strings.cpp"
					
					goto _st3;
					_st3:
					p += 1;
					st_case_3:
					{
						goto _st0;
					}
					st_case_0:
					_st0:
					cs = 0;
					goto _pop;
					_ctr4:
					{
#line 133 "lex/parser_strings.rl"
						out.push_back(' '); }
					
#line 603 "lex/parser_strings.cpp"
					
					goto _st2;
					_ctr5:
					{
#line 129 "lex/parser_strings.rl"
						out.push_back('`');   out.push_back('"'); }
					
#line 611 "lex/parser_strings.cpp"
					
					goto _st2;
					_ctr6:
					{
#line 143 "lex/parser_strings.rl"
						out.push_back('#'); }
					
#line 619 "lex/parser_strings.cpp"
					
					goto _st2;
					_ctr7:
					{
#line 142 "lex/parser_strings.rl"
						out.push_back('%'); }
					
#line 627 "lex/parser_strings.cpp"
					
					goto _st2;
					_ctr8:
					{
#line 136 "lex/parser_strings.rl"
						out.push_back('('); }
					
#line 635 "lex/parser_strings.cpp"
					
					goto _st2;
					_ctr9:
					{
#line 137 "lex/parser_strings.rl"
						out.push_back(')'); }
					
#line 643 "lex/parser_strings.cpp"
					
					goto _st2;
					_ctr10:
					{
#line 134 "lex/parser_strings.rl"
						out.push_back(','); }
					
#line 651 "lex/parser_strings.cpp"
					
					goto _st2;
					_ctr11:
					{
#line 140 "lex/parser_strings.rl"
						out.push_back(':'); }
					
#line 659 "lex/parser_strings.cpp"
					
					goto _st2;
					_ctr12:
					{
#line 135 "lex/parser_strings.rl"
						out.push_back(';'); }
					
#line 667 "lex/parser_strings.cpp"
					
					goto _st2;
					_ctr13:
					{
#line 141 "lex/parser_strings.rl"
						out.push_back('@'); }
					
#line 675 "lex/parser_strings.cpp"
					
					goto _st2;
					_ctr14:
					{
#line 138 "lex/parser_strings.rl"
						out.push_back('['); }
					
#line 683 "lex/parser_strings.cpp"
					
					goto _st2;
					_ctr15:
					{
#line 132 "lex/parser_strings.rl"
						out.push_back('\\'); }
					
#line 691 "lex/parser_strings.cpp"
					
					goto _st2;
					_ctr16:
					{
#line 139 "lex/parser_strings.rl"
						out.push_back(']'); }
					
#line 699 "lex/parser_strings.cpp"
					
					goto _st2;
					_ctr17:
					{
#line 128 "lex/parser_strings.rl"
						out.push_back('`');   out.push_back('`'); }
					
#line 707 "lex/parser_strings.cpp"
					
					goto _st2;
					_ctr18:
					{
#line 130 "lex/parser_strings.rl"
						out.push_back('`');   out.push_back('('); }
					
#line 715 "lex/parser_strings.cpp"
					
					goto _st2;
					_ctr19:
					{
#line 131 "lex/parser_strings.rl"
						out.push_back('`');   out.push_back(')'); }
					
#line 723 "lex/parser_strings.cpp"
					
					goto _st2;
					_ctr23:
					{
#line 144 "lex/parser_strings.rl"
						rl_esc_count++; }
					
#line 731 "lex/parser_strings.cpp"
					
					{
#line 133 "lex/parser_strings.rl"
						out.push_back(' '); }
					
#line 737 "lex/parser_strings.cpp"
					
					goto _st2;
					_ctr24:
					{
#line 144 "lex/parser_strings.rl"
						rl_esc_count++; }
					
#line 745 "lex/parser_strings.cpp"
					
					{
#line 129 "lex/parser_strings.rl"
						out.push_back('`');   out.push_back('"'); }
					
#line 751 "lex/parser_strings.cpp"
					
					goto _st2;
					_ctr25:
					{
#line 144 "lex/parser_strings.rl"
						rl_esc_count++; }
					
#line 759 "lex/parser_strings.cpp"
					
					{
#line 143 "lex/parser_strings.rl"
						out.push_back('#'); }
					
#line 765 "lex/parser_strings.cpp"
					
					goto _st2;
					_ctr26:
					{
#line 144 "lex/parser_strings.rl"
						rl_esc_count++; }
					
#line 773 "lex/parser_strings.cpp"
					
					{
#line 142 "lex/parser_strings.rl"
						out.push_back('%'); }
					
#line 779 "lex/parser_strings.cpp"
					
					goto _st2;
					_ctr27:
					{
#line 144 "lex/parser_strings.rl"
						rl_esc_count++; }
					
#line 787 "lex/parser_strings.cpp"
					
					{
#line 136 "lex/parser_strings.rl"
						out.push_back('('); }
					
#line 793 "lex/parser_strings.cpp"
					
					goto _st2;
					_ctr28:
					{
#line 144 "lex/parser_strings.rl"
						rl_esc_count++; }
					
#line 801 "lex/parser_strings.cpp"
					
					{
#line 137 "lex/parser_strings.rl"
						out.push_back(')'); }
					
#line 807 "lex/parser_strings.cpp"
					
					goto _st2;
					_ctr29:
					{
#line 144 "lex/parser_strings.rl"
						rl_esc_count++; }
					
#line 815 "lex/parser_strings.cpp"
					
					{
#line 134 "lex/parser_strings.rl"
						out.push_back(','); }
					
#line 821 "lex/parser_strings.cpp"
					
					goto _st2;
					_ctr30:
					{
#line 144 "lex/parser_strings.rl"
						rl_esc_count++; }
					
#line 829 "lex/parser_strings.cpp"
					
					{
#line 140 "lex/parser_strings.rl"
						out.push_back(':'); }
					
#line 835 "lex/parser_strings.cpp"
					
					goto _st2;
					_ctr31:
					{
#line 144 "lex/parser_strings.rl"
						rl_esc_count++; }
					
#line 843 "lex/parser_strings.cpp"
					
					{
#line 135 "lex/parser_strings.rl"
						out.push_back(';'); }
					
#line 849 "lex/parser_strings.cpp"
					
					goto _st2;
					_ctr32:
					{
#line 144 "lex/parser_strings.rl"
						rl_esc_count++; }
					
#line 857 "lex/parser_strings.cpp"
					
					{
#line 141 "lex/parser_strings.rl"
						out.push_back('@'); }
					
#line 863 "lex/parser_strings.cpp"
					
					goto _st2;
					_ctr33:
					{
#line 144 "lex/parser_strings.rl"
						rl_esc_count++; }
					
#line 871 "lex/parser_strings.cpp"
					
					{
#line 138 "lex/parser_strings.rl"
						out.push_back('['); }
					
#line 877 "lex/parser_strings.cpp"
					
					goto _st2;
					_ctr34:
					{
#line 144 "lex/parser_strings.rl"
						rl_esc_count++; }
					
#line 885 "lex/parser_strings.cpp"
					
					{
#line 132 "lex/parser_strings.rl"
						out.push_back('\\'); }
					
#line 891 "lex/parser_strings.cpp"
					
					goto _st2;
					_ctr35:
					{
#line 144 "lex/parser_strings.rl"
						rl_esc_count++; }
					
#line 899 "lex/parser_strings.cpp"
					
					{
#line 139 "lex/parser_strings.rl"
						out.push_back(']'); }
					
#line 905 "lex/parser_strings.cpp"
					
					goto _st2;
					_ctr36:
					{
#line 144 "lex/parser_strings.rl"
						rl_esc_count++; }
					
#line 913 "lex/parser_strings.cpp"
					
					{
#line 128 "lex/parser_strings.rl"
						out.push_back('`');   out.push_back('`'); }
					
#line 919 "lex/parser_strings.cpp"
					
					goto _st2;
					_ctr37:
					{
#line 144 "lex/parser_strings.rl"
						rl_esc_count++; }
					
#line 927 "lex/parser_strings.cpp"
					
					{
#line 130 "lex/parser_strings.rl"
						out.push_back('`');   out.push_back('('); }
					
#line 933 "lex/parser_strings.cpp"
					
					goto _st2;
					_ctr38:
					{
#line 144 "lex/parser_strings.rl"
						rl_esc_count++; }
					
#line 941 "lex/parser_strings.cpp"
					
					{
#line 131 "lex/parser_strings.rl"
						out.push_back('`');   out.push_back(')'); }
					
#line 947 "lex/parser_strings.cpp"
					
					goto _st2;
					_st2:
					p += 1;
					st_case_2:
					switch( ( (*( p))) ) {
						case 0: {
							goto _ctr22;
						}
						case 32: {
							goto _ctr23;
						}
						case 34: {
							goto _ctr24;
						}
						case 35: {
							goto _ctr25;
						}
						case 37: {
							goto _ctr26;
						}
						case 40: {
							goto _ctr27;
						}
						case 41: {
							goto _ctr28;
						}
						case 44: {
							goto _ctr29;
						}
						case 58: {
							goto _ctr30;
						}
						case 59: {
							goto _ctr31;
						}
						case 64: {
							goto _ctr32;
						}
						case 91: {
							goto _ctr33;
						}
						case 92: {
							goto _ctr34;
						}
						case 93: {
							goto _ctr35;
						}
						case 96: {
							goto _ctr36;
						}
						case 123: {
							goto _ctr37;
						}
						case 125: {
							goto _ctr38;
						}
					}
					{
						goto _ctr21;
					}
					st_out:
					_test_eof1: cs = 1; goto _test_eof; 
					_test_eof3: cs = 3; goto _test_eof; 
					_test_eof2: cs = 2; goto _test_eof; 
					
					_test_eof: {}
					if ( cs >= 3 )
						goto _out; _pop: {}
					_out: {}
				}
				
#line 169 "lex/parser_strings.rl"
				
				
				out.push_back('"');
				
				return rl_esc_count;
			} catch(std::exception& e) {
				LIB_ERR << fmt::format("[{}] error '{}'", __FUNCTION__, e.what());
				return -1;
			}
		}
		
		int escape_and_quote(const char* str, SmallString& out)
		{
			return escape_and_quote_t(str, out);
		}
		
		int  escape_and_quote(const char* str, MediumString& out)
		{
			return escape_and_quote_t(str, out);
		}
		
		int escape_and_quote(const char* str, StaticString& out)
		{
			return escape_and_quote_t(str, out);
		}
		
		void escape_and_quote(Atom& a)
		{
			if (a.isSymbol() || a.isComma() || a.isSemicolon()) {
				SmallString out;
				if (escape_and_quote(a, out)) {
					out.push_back(0);
					a = gensym(out.data());
				}
			}
		}
		
		Atom escape_and_quote(const Atom& a)
		{
			if (a.isSymbol() || a.isComma() || a.isSemicolon()) {
				SmallString out;
				if (escape_and_quote(a, out)) {
					out.push_back(0);
					return gensym(out.data());
				}
			}
			
			return a;
		}
		
		void escape_and_quote(AtomList& lst)
		{
			for (auto& a: lst)
			escape_and_quote(a);
		}
		
		AtomList escape_and_quote(const AtomListView& lv)
		{
			AtomList res;
			res.reserve(lv.size());
			for (auto& a: lv)
			res.append(escape_and_quote(a));
			
			return res;
		}
		
		
#line 1088 "lex/parser_strings.cpp"
		static const int unquote_and_unescape_start = 1;
		static const int unquote_and_unescape_first_final = 8;
		static const int unquote_and_unescape_error = 0;
		
		static const int unquote_and_unescape_en_envvar = 6;
		static const int unquote_and_unescape_en_main = 1;
		
		
#line 273 "lex/parser_strings.rl"
		
		
		template <typename T>
		static int unquote_and_unescape_t(const char* str, T& out) noexcept
		{
			try {
				if (str == nullptr)
					return -1;
				else if(str[0] == '\0')
					return 0;
				
				int cs = 0;
				int top = 0;
				int stack[2];
				const char* p = str;
				const char* eof = p + strlen(p);
				T& rl_string = out;
				std::string rl_envvar;
				
				
#line 1118 "lex/parser_strings.cpp"
				{
					cs = (int)unquote_and_unescape_start;
					top = 0;
				}
				
#line 292 "lex/parser_strings.rl"
				
				
#line 1127 "lex/parser_strings.cpp"
				{
					goto _resume;
					
					_again: {}
					switch ( cs ) {
						case 1: goto _st1;
						case 0: goto _st0;
						case 2: goto _st2;
						case 3: goto _st3;
						case 8: goto _st8;
						case 4: goto _st4;
						case 5: goto _st5;
						case 6: goto _st6;
						case 7: goto _st7;
						case 9: goto _st9;
					}
					
					_resume: {}
					switch ( cs ) {
						case 1:
						goto st_case_1;
						case 0:
						goto st_case_0;
						case 2:
						goto st_case_2;
						case 3:
						goto st_case_3;
						case 8:
						goto st_case_8;
						case 4:
						goto st_case_4;
						case 5:
						goto st_case_5;
						case 6:
						goto st_case_6;
						case 7:
						goto st_case_7;
						case 9:
						goto st_case_9;
					}
					goto st_out;
					_st1:
					if ( p == eof ) {
						if ( cs >= 8 )
							goto _out;
						else
							goto _pop;
					}
					p += 1;
					st_case_1:
					switch( ( (*( p))) ) {
						case 34: {
							goto _st2;
						}
						case 83: {
							goto _st5;
						}
					}
					{
						goto _st0;
					}
					_ctr15:
					{
#line 249 "lex/parser_strings.rl"
						
						rl_string.push_back('%');
						rl_string.insert(rl_string.end(), rl_envvar.begin(), rl_envvar.end());
						{p = p - 1; }
						{top -= 1;cs = stack[top];goto _again;}
					}
					
#line 1199 "lex/parser_strings.cpp"
					
					goto _st0;
					st_case_0:
					_st0:
					cs = 0;
					goto _pop;
					_ctr4:
					{
#line 266 "lex/parser_strings.rl"
						rl_string.push_back((( (*( p))))); }
					
#line 1211 "lex/parser_strings.cpp"
					
					goto _st2;
					_ctr6:
					{
#line 263 "lex/parser_strings.rl"
						{stack[top] = 2; top+= 1; goto _st6;}}
					
#line 1219 "lex/parser_strings.cpp"
					
					goto _st2;
					_ctr9:
					{
#line 261 "lex/parser_strings.rl"
						rl_string.push_back('"'); }
					
#line 1227 "lex/parser_strings.cpp"
					
					goto _st2;
					_ctr10:
					{
#line 262 "lex/parser_strings.rl"
						rl_string.push_back('%'); }
					
#line 1235 "lex/parser_strings.cpp"
					
					goto _st2;
					_ctr11:
					{
#line 264 "lex/parser_strings.rl"
						rl_string.push_back('{'); }
						
#line 1243 "lex/parser_strings.cpp"
						
						goto _st2;
						_ctr12:
						{
#line 265 "lex/parser_strings.rl"
							rl_string.push_back('}'); }
					
#line 1251 "lex/parser_strings.cpp"
					
					goto _st2;
					_ctr13:
					{
#line 260 "lex/parser_strings.rl"
						rl_string.push_back('`'); }
					
#line 1259 "lex/parser_strings.cpp"
					
					goto _st2;
					_st2:
					if ( p == eof ) {
						if ( cs >= 8 )
							goto _out;
						else
							goto _pop;
					}
					p += 1;
					st_case_2:
					switch( ( (*( p))) ) {
						case 0: {
							goto _st0;
						}
						case 34: {
							goto _st3;
						}
						case 37: {
							goto _ctr6;
						}
						case 96: {
							goto _st4;
						}
					}
					{
						goto _ctr4;
					}
					_st3:
					if ( p == eof ) {
						if ( cs >= 8 )
							goto _out;
						else
							goto _pop;
					}
					p += 1;
					st_case_3:
					if ( ( (*( p))) == 0 ) {
						goto _ctr8;
					}
					{
						goto _st0;
					}
					_ctr8:
					{
#line 271 "lex/parser_strings.rl"
						{p+= 1; cs = 8; goto _out;} }
					
#line 1308 "lex/parser_strings.cpp"
					
					goto _st8;
					_st8:
					if ( p == eof ) {
						if ( cs >= 8 )
							goto _out;
						else
							goto _pop;
					}
					p += 1;
					st_case_8:
					{
						goto _st0;
					}
					_st4:
					if ( p == eof ) {
						if ( cs >= 8 )
							goto _out;
						else
							goto _pop;
					}
					p += 1;
					st_case_4:
					switch( ( (*( p))) ) {
						case 34: {
							goto _ctr9;
						}
						case 37: {
							goto _ctr10;
						}
						case 40: {
							goto _ctr11;
						}
						case 41: {
							goto _ctr12;
						}
						case 96: {
							goto _ctr13;
						}
					}
					{
						goto _st0;
					}
					_st5:
					if ( p == eof ) {
						if ( cs >= 8 )
							goto _out;
						else
							goto _pop;
					}
					p += 1;
					st_case_5:
					if ( ( (*( p))) == 34 ) {
						goto _st2;
					}
					{
						goto _st0;
					}
					_ctr14:
					{
#line 249 "lex/parser_strings.rl"
						
						rl_string.push_back('%');
						rl_string.insert(rl_string.end(), rl_envvar.begin(), rl_envvar.end());
						{p = p - 1; }
						{top -= 1;cs = stack[top];goto _again;}
					}
					
#line 1377 "lex/parser_strings.cpp"
					
					goto _st6;
					_st6:
					if ( p == eof ) {
						if ( cs >= 8 )
							goto _out;
						else
							goto _pop;
					}
					p += 1;
					st_case_6:
					if ( ( (*( p))) == 95 ) {
						goto _ctr16;
					}
					if ( ( (*( p))) > 57 ) {
						if ( 65 <= ( (*( p))) && ( (*( p))) <= 90 ) {
							goto _ctr16;
						}
					} else if ( ( (*( p))) >= 48 ) {
						goto _ctr16;
					}
					{
						goto _ctr15;
					}
					_ctr17:
					{
#line 249 "lex/parser_strings.rl"
						
						rl_string.push_back('%');
						rl_string.insert(rl_string.end(), rl_envvar.begin(), rl_envvar.end());
						{p = p - 1; }
						{top -= 1;cs = stack[top];goto _again;}
					}
					
#line 1412 "lex/parser_strings.cpp"
					
					goto _st7;
					_ctr16:
					{
#line 256 "lex/parser_strings.rl"
						rl_envvar += (( (*( p)))); }
					
#line 1420 "lex/parser_strings.cpp"
					
					goto _st7;
					_st7:
					if ( p == eof ) {
						if ( cs >= 8 )
							goto _out;
						else
							goto _pop;
					}
					p += 1;
					st_case_7:
					switch( ( (*( p))) ) {
						case 37: {
							goto _ctr18;
						}
						case 95: {
							goto _ctr16;
						}
					}
					if ( ( (*( p))) > 57 ) {
						if ( 65 <= ( (*( p))) && ( (*( p))) <= 90 ) {
							goto _ctr16;
						}
					} else if ( ( (*( p))) >= 48 ) {
						goto _ctr16;
					}
					{
						goto _ctr15;
					}
					_ctr18:
					{
#line 239 "lex/parser_strings.rl"
						
						auto env = std::getenv(rl_envvar.c_str());
						if (env)
						rl_string.insert(rl_string.end(), env, env + strlen(env));
						else
						rl_string.insert(rl_string.end(), rl_envvar.begin(), rl_envvar.end());
						
						{top -= 1;cs = stack[top];goto _again;}
					}
					
#line 1463 "lex/parser_strings.cpp"
					
					goto _st9;
					_st9:
					if ( p == eof ) {
						if ( cs >= 8 )
							goto _out;
						else
							goto _pop;
					}
					p += 1;
					st_case_9:
					{
						goto _st0;
					}
					st_out:
					_test_eof1: cs = 1; goto _test_eof; 
					_test_eof2: cs = 2; goto _test_eof; 
					_test_eof3: cs = 3; goto _test_eof; 
					_test_eof8: cs = 8; goto _test_eof; 
					_test_eof4: cs = 4; goto _test_eof; 
					_test_eof5: cs = 5; goto _test_eof; 
					_test_eof6: cs = 6; goto _test_eof; 
					_test_eof7: cs = 7; goto _test_eof; 
					_test_eof9: cs = 9; goto _test_eof; 
					
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
							case 8: {
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
							case 9: {
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
							goto _st3;case 8:
							goto _st8;case 4:
							goto _st4;case 5:
							goto _st5;case 6:
							goto _ctr14;case 7:
							goto _ctr17;case 9:
							goto _st9;	}
					}
					
					if ( cs >= 8 )
						goto _out; _pop: {}
					_out: {}
				}
				
#line 293 "lex/parser_strings.rl"
				
				
				if (cs < 
#line 1548 "lex/parser_strings.cpp"
				8
#line 295 "lex/parser_strings.rl"
				)
				return 0;
				else
					return 1;
				
			} catch(std::exception& e) {
				LIB_ERR << fmt::format("[{}] error '{}'", __FUNCTION__, e.what());
				return -1;
			}
		}
		
		int unquote_and_unescape(const char* str, StaticString& out)
		{
			return unquote_and_unescape_t(str, out);
		}
		
		int unquote_and_unescape(const char* str, SmallString& out)
		{
			return unquote_and_unescape_t(str, out);
		}
		
		int unquote_and_unescape(const char* str, MediumString& out)
		{
			return unquote_and_unescape_t(str, out);
		}
		
		bool unquote_and_unescape(Atom& a)
		{
			if (!a.isSymbol())
				return false;
			else {
				SmallString str;
				auto rc = unquote_and_unescape_t(a.asT<t_symbol*>()->s_name, str);
				if (rc > 0) {
					str.push_back(0);
					a = gensym(str.data());
					return true;
				} else if (rc == 0) {
					return false;
				} else {
					a = &s_;
					return true;
				}
			}
		}
		
		Atom unquote_and_unescape(const Atom& a)
		{
			Atom res = a;
			unquote_and_unescape(res);
			return res;
		}
		
		t_symbol* unquote_and_unescape(t_symbol* s)
		{
			if (s == nullptr) return &s_;
				SmallString str;
			if (unquote_and_unescape(s->s_name, str) > 0) {
				str.push_back(0);
				return gensym(str.data());
			} else
			return s;
		}
		
	}
}
