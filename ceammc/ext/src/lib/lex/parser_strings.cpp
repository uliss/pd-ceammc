#line 1 "lex/parser_strings.rl"
# include "parser_strings.h"
# include "ceammc_containers.h"
# include "ceammc_format.h"
# include "ceammc_log.h"
# include "ceammc_string.h"
# include "fmt/format.h"


#line 11 "lex/parser_strings.cpp"
static const int quoted_string_end_start = 1;
static const int quoted_string_end_first_final = 4;
static const int quoted_string_end_error = 0;

static const int quoted_string_end_en_main = 1;


#line 18 "lex/parser_strings.rl"


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
			
			
#line 51 "lex/parser_strings.cpp"
			{
				cs = (int)quoted_string_end_start;
			}
			
#line 48 "lex/parser_strings.rl"
			
			
#line 59 "lex/parser_strings.cpp"
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
#line 16 "lex/parser_strings.rl"
					{p+= 1; cs = 4; goto _out;} }
				
#line 109 "lex/parser_strings.cpp"
				
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
			
#line 49 "lex/parser_strings.rl"
			
			
			return (cs >= 
#line 151 "lex/parser_strings.cpp"
			4
#line 51 "lex/parser_strings.rl"
			);
		}
		
		
#line 158 "lex/parser_strings.cpp"
		static const int is_quoted_string_start = 1;
		static const int is_quoted_string_first_final = 5;
		static const int is_quoted_string_error = 0;
		
		static const int is_quoted_string_en_main = 1;
		
		
#line 62 "lex/parser_strings.rl"
		
		
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
			
			
#line 202 "lex/parser_strings.cpp"
			{
				cs = (int)is_quoted_string_start;
			}
			
#line 96 "lex/parser_strings.rl"
			
			
#line 210 "lex/parser_strings.cpp"
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
#line 60 "lex/parser_strings.rl"
					{p+= 1; cs = 5; goto _out;} }
				
#line 270 "lex/parser_strings.cpp"
				
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
			
#line 97 "lex/parser_strings.rl"
			
			
			return (cs >= 
#line 313 "lex/parser_strings.cpp"
			5
#line 99 "lex/parser_strings.rl"
			);
		}
		
		
#line 320 "lex/parser_strings.cpp"
		static const int need_quotes_start = 1;
		static const int need_quotes_first_final = 2;
		static const int need_quotes_error = 0;
		
		static const int need_quotes_en_main = 1;
		
		
#line 109 "lex/parser_strings.rl"
		
		
		bool string_need_quotes(const char* str) {
			if (str == nullptr || str[0] == '\0')
				return false;
			
			int cs = 0;
			const char* p = str;
			
			
#line 339 "lex/parser_strings.cpp"
			{
				cs = (int)need_quotes_start;
			}
			
#line 118 "lex/parser_strings.rl"
			
			
#line 347 "lex/parser_strings.cpp"
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
#line 107 "lex/parser_strings.rl"
					{p+= 1; cs = 2; goto _out;} }
				
#line 404 "lex/parser_strings.cpp"
				
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
			
#line 119 "lex/parser_strings.rl"
			
			
			return (cs < 
#line 431 "lex/parser_strings.cpp"
			2
#line 121 "lex/parser_strings.rl"
			);
		}
		
		
#line 438 "lex/parser_strings.cpp"
		static const int escape_and_quote_start = 1;
		static const int escape_and_quote_first_final = 3;
		static const int escape_and_quote_error = 0;
		
		static const int escape_and_quote_en_main = 1;
		
		
#line 152 "lex/parser_strings.rl"
		
		
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
				
				
#line 466 "lex/parser_strings.cpp"
				{
					cs = (int)escape_and_quote_start;
				}
				
#line 170 "lex/parser_strings.rl"
				
				
#line 474 "lex/parser_strings.cpp"
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
#line 148 "lex/parser_strings.rl"
						out.push_back((( (*( p))))); }
					
#line 492 "lex/parser_strings.cpp"
					
					goto _st1;
					_ctr21:
					{
#line 146 "lex/parser_strings.rl"
						rl_esc_count++; }
					
#line 500 "lex/parser_strings.cpp"
					
					{
#line 148 "lex/parser_strings.rl"
						out.push_back((( (*( p))))); }
					
#line 506 "lex/parser_strings.cpp"
					
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
#line 150 "lex/parser_strings.rl"
						{p+= 1; cs = 3; goto _out;} }
					
#line 573 "lex/parser_strings.cpp"
					
					goto _st3;
					_ctr22:
					{
#line 146 "lex/parser_strings.rl"
						rl_esc_count++; }
					
#line 581 "lex/parser_strings.cpp"
					
					{
#line 150 "lex/parser_strings.rl"
						{p+= 1; cs = 3; goto _out;} }
					
#line 587 "lex/parser_strings.cpp"
					
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
#line 135 "lex/parser_strings.rl"
						out.push_back(' '); }
					
#line 605 "lex/parser_strings.cpp"
					
					goto _st2;
					_ctr5:
					{
#line 131 "lex/parser_strings.rl"
						out.push_back('`');   out.push_back('"'); }
					
#line 613 "lex/parser_strings.cpp"
					
					goto _st2;
					_ctr6:
					{
#line 145 "lex/parser_strings.rl"
						out.push_back('#'); }
					
#line 621 "lex/parser_strings.cpp"
					
					goto _st2;
					_ctr7:
					{
#line 144 "lex/parser_strings.rl"
						out.push_back('%'); }
					
#line 629 "lex/parser_strings.cpp"
					
					goto _st2;
					_ctr8:
					{
#line 138 "lex/parser_strings.rl"
						out.push_back('('); }
					
#line 637 "lex/parser_strings.cpp"
					
					goto _st2;
					_ctr9:
					{
#line 139 "lex/parser_strings.rl"
						out.push_back(')'); }
					
#line 645 "lex/parser_strings.cpp"
					
					goto _st2;
					_ctr10:
					{
#line 136 "lex/parser_strings.rl"
						out.push_back(','); }
					
#line 653 "lex/parser_strings.cpp"
					
					goto _st2;
					_ctr11:
					{
#line 142 "lex/parser_strings.rl"
						out.push_back(':'); }
					
#line 661 "lex/parser_strings.cpp"
					
					goto _st2;
					_ctr12:
					{
#line 137 "lex/parser_strings.rl"
						out.push_back(';'); }
					
#line 669 "lex/parser_strings.cpp"
					
					goto _st2;
					_ctr13:
					{
#line 143 "lex/parser_strings.rl"
						out.push_back('@'); }
					
#line 677 "lex/parser_strings.cpp"
					
					goto _st2;
					_ctr14:
					{
#line 140 "lex/parser_strings.rl"
						out.push_back('['); }
					
#line 685 "lex/parser_strings.cpp"
					
					goto _st2;
					_ctr15:
					{
#line 134 "lex/parser_strings.rl"
						out.push_back('\\'); }
					
#line 693 "lex/parser_strings.cpp"
					
					goto _st2;
					_ctr16:
					{
#line 141 "lex/parser_strings.rl"
						out.push_back(']'); }
					
#line 701 "lex/parser_strings.cpp"
					
					goto _st2;
					_ctr17:
					{
#line 130 "lex/parser_strings.rl"
						out.push_back('`');   out.push_back('`'); }
					
#line 709 "lex/parser_strings.cpp"
					
					goto _st2;
					_ctr18:
					{
#line 132 "lex/parser_strings.rl"
						out.push_back('`');   out.push_back('('); }
					
#line 717 "lex/parser_strings.cpp"
					
					goto _st2;
					_ctr19:
					{
#line 133 "lex/parser_strings.rl"
						out.push_back('`');   out.push_back(')'); }
					
#line 725 "lex/parser_strings.cpp"
					
					goto _st2;
					_ctr23:
					{
#line 146 "lex/parser_strings.rl"
						rl_esc_count++; }
					
#line 733 "lex/parser_strings.cpp"
					
					{
#line 135 "lex/parser_strings.rl"
						out.push_back(' '); }
					
#line 739 "lex/parser_strings.cpp"
					
					goto _st2;
					_ctr24:
					{
#line 146 "lex/parser_strings.rl"
						rl_esc_count++; }
					
#line 747 "lex/parser_strings.cpp"
					
					{
#line 131 "lex/parser_strings.rl"
						out.push_back('`');   out.push_back('"'); }
					
#line 753 "lex/parser_strings.cpp"
					
					goto _st2;
					_ctr25:
					{
#line 146 "lex/parser_strings.rl"
						rl_esc_count++; }
					
#line 761 "lex/parser_strings.cpp"
					
					{
#line 145 "lex/parser_strings.rl"
						out.push_back('#'); }
					
#line 767 "lex/parser_strings.cpp"
					
					goto _st2;
					_ctr26:
					{
#line 146 "lex/parser_strings.rl"
						rl_esc_count++; }
					
#line 775 "lex/parser_strings.cpp"
					
					{
#line 144 "lex/parser_strings.rl"
						out.push_back('%'); }
					
#line 781 "lex/parser_strings.cpp"
					
					goto _st2;
					_ctr27:
					{
#line 146 "lex/parser_strings.rl"
						rl_esc_count++; }
					
#line 789 "lex/parser_strings.cpp"
					
					{
#line 138 "lex/parser_strings.rl"
						out.push_back('('); }
					
#line 795 "lex/parser_strings.cpp"
					
					goto _st2;
					_ctr28:
					{
#line 146 "lex/parser_strings.rl"
						rl_esc_count++; }
					
#line 803 "lex/parser_strings.cpp"
					
					{
#line 139 "lex/parser_strings.rl"
						out.push_back(')'); }
					
#line 809 "lex/parser_strings.cpp"
					
					goto _st2;
					_ctr29:
					{
#line 146 "lex/parser_strings.rl"
						rl_esc_count++; }
					
#line 817 "lex/parser_strings.cpp"
					
					{
#line 136 "lex/parser_strings.rl"
						out.push_back(','); }
					
#line 823 "lex/parser_strings.cpp"
					
					goto _st2;
					_ctr30:
					{
#line 146 "lex/parser_strings.rl"
						rl_esc_count++; }
					
#line 831 "lex/parser_strings.cpp"
					
					{
#line 142 "lex/parser_strings.rl"
						out.push_back(':'); }
					
#line 837 "lex/parser_strings.cpp"
					
					goto _st2;
					_ctr31:
					{
#line 146 "lex/parser_strings.rl"
						rl_esc_count++; }
					
#line 845 "lex/parser_strings.cpp"
					
					{
#line 137 "lex/parser_strings.rl"
						out.push_back(';'); }
					
#line 851 "lex/parser_strings.cpp"
					
					goto _st2;
					_ctr32:
					{
#line 146 "lex/parser_strings.rl"
						rl_esc_count++; }
					
#line 859 "lex/parser_strings.cpp"
					
					{
#line 143 "lex/parser_strings.rl"
						out.push_back('@'); }
					
#line 865 "lex/parser_strings.cpp"
					
					goto _st2;
					_ctr33:
					{
#line 146 "lex/parser_strings.rl"
						rl_esc_count++; }
					
#line 873 "lex/parser_strings.cpp"
					
					{
#line 140 "lex/parser_strings.rl"
						out.push_back('['); }
					
#line 879 "lex/parser_strings.cpp"
					
					goto _st2;
					_ctr34:
					{
#line 146 "lex/parser_strings.rl"
						rl_esc_count++; }
					
#line 887 "lex/parser_strings.cpp"
					
					{
#line 134 "lex/parser_strings.rl"
						out.push_back('\\'); }
					
#line 893 "lex/parser_strings.cpp"
					
					goto _st2;
					_ctr35:
					{
#line 146 "lex/parser_strings.rl"
						rl_esc_count++; }
					
#line 901 "lex/parser_strings.cpp"
					
					{
#line 141 "lex/parser_strings.rl"
						out.push_back(']'); }
					
#line 907 "lex/parser_strings.cpp"
					
					goto _st2;
					_ctr36:
					{
#line 146 "lex/parser_strings.rl"
						rl_esc_count++; }
					
#line 915 "lex/parser_strings.cpp"
					
					{
#line 130 "lex/parser_strings.rl"
						out.push_back('`');   out.push_back('`'); }
					
#line 921 "lex/parser_strings.cpp"
					
					goto _st2;
					_ctr37:
					{
#line 146 "lex/parser_strings.rl"
						rl_esc_count++; }
					
#line 929 "lex/parser_strings.cpp"
					
					{
#line 132 "lex/parser_strings.rl"
						out.push_back('`');   out.push_back('('); }
					
#line 935 "lex/parser_strings.cpp"
					
					goto _st2;
					_ctr38:
					{
#line 146 "lex/parser_strings.rl"
						rl_esc_count++; }
					
#line 943 "lex/parser_strings.cpp"
					
					{
#line 133 "lex/parser_strings.rl"
						out.push_back('`');   out.push_back(')'); }
					
#line 949 "lex/parser_strings.cpp"
					
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
				
#line 171 "lex/parser_strings.rl"
				
				
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
		
		
#line 1090 "lex/parser_strings.cpp"
		static const int unquote_and_unescape_start = 1;
		static const int unquote_and_unescape_first_final = 8;
		static const int unquote_and_unescape_error = 0;
		
		static const int unquote_and_unescape_en_envvar = 6;
		static const int unquote_and_unescape_en_main = 1;
		
		
#line 275 "lex/parser_strings.rl"
		
		
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
				
				
#line 1120 "lex/parser_strings.cpp"
				{
					cs = (int)unquote_and_unescape_start;
					top = 0;
				}
				
#line 294 "lex/parser_strings.rl"
				
				
#line 1129 "lex/parser_strings.cpp"
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
#line 251 "lex/parser_strings.rl"
						
						rl_string.push_back('%');
						rl_string.insert(rl_string.end(), rl_envvar.begin(), rl_envvar.end());
						{p = p - 1; }
						{top -= 1;cs = stack[top];goto _again;}
					}
					
#line 1201 "lex/parser_strings.cpp"
					
					goto _st0;
					st_case_0:
					_st0:
					cs = 0;
					goto _pop;
					_ctr4:
					{
#line 268 "lex/parser_strings.rl"
						rl_string.push_back((( (*( p))))); }
					
#line 1213 "lex/parser_strings.cpp"
					
					goto _st2;
					_ctr6:
					{
#line 265 "lex/parser_strings.rl"
						{stack[top] = 2; top+= 1; goto _st6;}}
					
#line 1221 "lex/parser_strings.cpp"
					
					goto _st2;
					_ctr9:
					{
#line 263 "lex/parser_strings.rl"
						rl_string.push_back('"'); }
					
#line 1229 "lex/parser_strings.cpp"
					
					goto _st2;
					_ctr10:
					{
#line 264 "lex/parser_strings.rl"
						rl_string.push_back('%'); }
					
#line 1237 "lex/parser_strings.cpp"
					
					goto _st2;
					_ctr11:
					{
#line 266 "lex/parser_strings.rl"
						rl_string.push_back('{'); }
						
#line 1245 "lex/parser_strings.cpp"
						
						goto _st2;
						_ctr12:
						{
#line 267 "lex/parser_strings.rl"
							rl_string.push_back('}'); }
					
#line 1253 "lex/parser_strings.cpp"
					
					goto _st2;
					_ctr13:
					{
#line 262 "lex/parser_strings.rl"
						rl_string.push_back('`'); }
					
#line 1261 "lex/parser_strings.cpp"
					
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
#line 273 "lex/parser_strings.rl"
						{p+= 1; cs = 8; goto _out;} }
					
#line 1310 "lex/parser_strings.cpp"
					
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
#line 251 "lex/parser_strings.rl"
						
						rl_string.push_back('%');
						rl_string.insert(rl_string.end(), rl_envvar.begin(), rl_envvar.end());
						{p = p - 1; }
						{top -= 1;cs = stack[top];goto _again;}
					}
					
#line 1379 "lex/parser_strings.cpp"
					
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
#line 251 "lex/parser_strings.rl"
						
						rl_string.push_back('%');
						rl_string.insert(rl_string.end(), rl_envvar.begin(), rl_envvar.end());
						{p = p - 1; }
						{top -= 1;cs = stack[top];goto _again;}
					}
					
#line 1414 "lex/parser_strings.cpp"
					
					goto _st7;
					_ctr16:
					{
#line 258 "lex/parser_strings.rl"
						rl_envvar += (( (*( p)))); }
					
#line 1422 "lex/parser_strings.cpp"
					
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
#line 241 "lex/parser_strings.rl"
						
						auto env = std::getenv(rl_envvar.c_str());
						if (env)
						rl_string.insert(rl_string.end(), env, env + strlen(env));
						else
						rl_string.insert(rl_string.end(), rl_envvar.begin(), rl_envvar.end());
						
						{top -= 1;cs = stack[top];goto _again;}
					}
					
#line 1465 "lex/parser_strings.cpp"
					
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
				
#line 295 "lex/parser_strings.rl"
				
				
				if (cs < 
#line 1550 "lex/parser_strings.cpp"
				8
#line 297 "lex/parser_strings.rl"
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
		
		t_symbol* parse_ceammc_quoted_string(t_symbol* s)
		{
			if (s == nullptr) return &s_;
				SmallString str;
			if (unquote_and_unescape(s->s_name, str) > 0) {
				str.push_back(0);
				return gensym(str.data());
			} else
			return s;
		}
		
		Atom parse_ceammc_quoted_string(const Atom& a)
		{
			switch(a.atom().a_type) {
				case A_SYMBOL:
				case A_DOLLSYM:
				return parse_ceammc_quoted_string(a.asT<t_symbol*>());
				default:
				return a;
			}
		}
		
		AtomList parse_ceammc_quoted_string(const AtomListView& lv)
		{
			AtomList res;
			res.reserve(lv.size());
			
			bool in_string = false;
			
			SmallAtomListN<8> str_atoms;
			
			for (auto& a: lv) {
				if (a.isQuoted() && !in_string)
					res.append(parse_ceammc_quoted_string(a));
				else if (a.beginQuote() && !in_string) {
					in_string = true;
					str_atoms.push_back(a);
				} else if (a.endQuote()) {
					str_atoms.push_back(a);
					MediumString buf;
					for (auto& a: str_atoms) {
						atom_to_string(a, buf);
						buf.push_back(' ');
					}
					
					// remove trailing space
					if (buf.size() > 0 && buf.back() == ' ')
						buf.pop_back();
					
					// make zero-terminated string
					buf.push_back('\0');
					
					// unquoted and unescape
					MediumString str;
					if (unquote_and_unescape(buf.data(), str)) {
						str.push_back('\0');
						auto sym = gensym(str.data());
						res.append(sym);
					}
					
					str_atoms.clear();
					in_string = false;
				} else if (in_string) {
					str_atoms.push_back(a);
				} else {
					res.append(a);
				}
			}
			
			// non-balanced quotes
			if (str_atoms.size() > 0)  {
				LIB_LOG << "non-balanced quotes: " << lv;
				for (auto& a: str_atoms)
				res.append(a);
			}
			
			return res;
		}
		
		bool maybe_ceammc_quoted_string(const AtomListView& lv)
		{
			for (auto& a: lv) {
				if (a.beginQuote() || a.endQuote() || a.isQuoted())
					return true;
			}
			
			return false;
		}
		
	}
}
