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
				if ( 0 <= ( (*( p))) ) {
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
#line 141 "lex/parser_strings.cpp"
			4
#line 49 "lex/parser_strings.rl"
			);
		}
		
		
#line 148 "lex/parser_strings.cpp"
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
			
			
#line 192 "lex/parser_strings.cpp"
			{
				cs = (int)is_quoted_string_start;
			}
			
#line 94 "lex/parser_strings.rl"
			
			
#line 200 "lex/parser_strings.cpp"
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
				
#line 260 "lex/parser_strings.cpp"
				
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
				if ( ( (*( p))) <= -1 ) {
					goto _st0;
				}
				{
					goto _st2;
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
#line 295 "lex/parser_strings.cpp"
			5
#line 97 "lex/parser_strings.rl"
			);
		}
		
		
#line 302 "lex/parser_strings.cpp"
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
			
			
#line 321 "lex/parser_strings.cpp"
			{
				cs = (int)need_quotes_start;
			}
			
#line 116 "lex/parser_strings.rl"
			
			
#line 329 "lex/parser_strings.cpp"
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
					case 34: {
						goto _st0;
					}
					case 40: {
						goto _st0;
					}
					case 44: {
						goto _st0;
					}
					case 59: {
						goto _st0;
					}
					case 96: {
						goto _st0;
					}
				}
				if ( 91 <= ( (*( p))) && ( (*( p))) <= 92 ) {
					goto _st0;
				}
				{
					goto _st1;
				}
				_ctr2:
				{
#line 105 "lex/parser_strings.rl"
					{p+= 1; cs = 2; goto _out;} }
				
#line 377 "lex/parser_strings.cpp"
				
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
#line 404 "lex/parser_strings.cpp"
			2
#line 119 "lex/parser_strings.rl"
			);
		}
		
		
#line 411 "lex/parser_strings.cpp"
		static const int escape_and_quote_start = 1;
		static const int escape_and_quote_first_final = 3;
		static const int escape_and_quote_error = 0;
		
		static const int escape_and_quote_en_main = 1;
		
		
#line 148 "lex/parser_strings.rl"
		
		
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
				
				
#line 439 "lex/parser_strings.cpp"
				{
					cs = (int)escape_and_quote_start;
				}
				
#line 166 "lex/parser_strings.rl"
				
				
#line 447 "lex/parser_strings.cpp"
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
#line 144 "lex/parser_strings.rl"
						out.push_back((( (*( p))))); }
					
#line 465 "lex/parser_strings.cpp"
					
					goto _st1;
					_ctr19:
					{
#line 142 "lex/parser_strings.rl"
						rl_esc_count++; }
					
#line 473 "lex/parser_strings.cpp"
					
					{
#line 144 "lex/parser_strings.rl"
						out.push_back((( (*( p))))); }
					
#line 479 "lex/parser_strings.cpp"
					
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
					}
					{
						goto _ctr2;
					}
					_ctr3:
					{
#line 146 "lex/parser_strings.rl"
						{p+= 1; cs = 3; goto _out;} }
					
#line 540 "lex/parser_strings.cpp"
					
					goto _st3;
					_ctr20:
					{
#line 142 "lex/parser_strings.rl"
						rl_esc_count++; }
					
#line 548 "lex/parser_strings.cpp"
					
					{
#line 146 "lex/parser_strings.rl"
						{p+= 1; cs = 3; goto _out;} }
					
#line 554 "lex/parser_strings.cpp"
					
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
#line 131 "lex/parser_strings.rl"
						out.push_back(' '); }
					
#line 572 "lex/parser_strings.cpp"
					
					goto _st2;
					_ctr5:
					{
#line 129 "lex/parser_strings.rl"
						out.push_back('`');   out.push_back('"'); }
					
#line 580 "lex/parser_strings.cpp"
					
					goto _st2;
					_ctr6:
					{
#line 141 "lex/parser_strings.rl"
						out.push_back('#'); }
					
#line 588 "lex/parser_strings.cpp"
					
					goto _st2;
					_ctr7:
					{
#line 140 "lex/parser_strings.rl"
						out.push_back('%'); }
					
#line 596 "lex/parser_strings.cpp"
					
					goto _st2;
					_ctr8:
					{
#line 134 "lex/parser_strings.rl"
						out.push_back('('); }
					
#line 604 "lex/parser_strings.cpp"
					
					goto _st2;
					_ctr9:
					{
#line 135 "lex/parser_strings.rl"
						out.push_back(')'); }
					
#line 612 "lex/parser_strings.cpp"
					
					goto _st2;
					_ctr10:
					{
#line 132 "lex/parser_strings.rl"
						out.push_back(','); }
					
#line 620 "lex/parser_strings.cpp"
					
					goto _st2;
					_ctr11:
					{
#line 138 "lex/parser_strings.rl"
						out.push_back(':'); }
					
#line 628 "lex/parser_strings.cpp"
					
					goto _st2;
					_ctr12:
					{
#line 133 "lex/parser_strings.rl"
						out.push_back(';'); }
					
#line 636 "lex/parser_strings.cpp"
					
					goto _st2;
					_ctr13:
					{
#line 139 "lex/parser_strings.rl"
						out.push_back('@'); }
					
#line 644 "lex/parser_strings.cpp"
					
					goto _st2;
					_ctr14:
					{
#line 136 "lex/parser_strings.rl"
						out.push_back('['); }
					
#line 652 "lex/parser_strings.cpp"
					
					goto _st2;
					_ctr15:
					{
#line 130 "lex/parser_strings.rl"
						out.push_back('\\'); }
					
#line 660 "lex/parser_strings.cpp"
					
					goto _st2;
					_ctr16:
					{
#line 137 "lex/parser_strings.rl"
						out.push_back(']'); }
					
#line 668 "lex/parser_strings.cpp"
					
					goto _st2;
					_ctr17:
					{
#line 128 "lex/parser_strings.rl"
						out.push_back('`');   out.push_back('`'); }
					
#line 676 "lex/parser_strings.cpp"
					
					goto _st2;
					_ctr21:
					{
#line 142 "lex/parser_strings.rl"
						rl_esc_count++; }
					
#line 684 "lex/parser_strings.cpp"
					
					{
#line 131 "lex/parser_strings.rl"
						out.push_back(' '); }
					
#line 690 "lex/parser_strings.cpp"
					
					goto _st2;
					_ctr22:
					{
#line 142 "lex/parser_strings.rl"
						rl_esc_count++; }
					
#line 698 "lex/parser_strings.cpp"
					
					{
#line 129 "lex/parser_strings.rl"
						out.push_back('`');   out.push_back('"'); }
					
#line 704 "lex/parser_strings.cpp"
					
					goto _st2;
					_ctr23:
					{
#line 142 "lex/parser_strings.rl"
						rl_esc_count++; }
					
#line 712 "lex/parser_strings.cpp"
					
					{
#line 141 "lex/parser_strings.rl"
						out.push_back('#'); }
					
#line 718 "lex/parser_strings.cpp"
					
					goto _st2;
					_ctr24:
					{
#line 142 "lex/parser_strings.rl"
						rl_esc_count++; }
					
#line 726 "lex/parser_strings.cpp"
					
					{
#line 140 "lex/parser_strings.rl"
						out.push_back('%'); }
					
#line 732 "lex/parser_strings.cpp"
					
					goto _st2;
					_ctr25:
					{
#line 142 "lex/parser_strings.rl"
						rl_esc_count++; }
					
#line 740 "lex/parser_strings.cpp"
					
					{
#line 134 "lex/parser_strings.rl"
						out.push_back('('); }
					
#line 746 "lex/parser_strings.cpp"
					
					goto _st2;
					_ctr26:
					{
#line 142 "lex/parser_strings.rl"
						rl_esc_count++; }
					
#line 754 "lex/parser_strings.cpp"
					
					{
#line 135 "lex/parser_strings.rl"
						out.push_back(')'); }
					
#line 760 "lex/parser_strings.cpp"
					
					goto _st2;
					_ctr27:
					{
#line 142 "lex/parser_strings.rl"
						rl_esc_count++; }
					
#line 768 "lex/parser_strings.cpp"
					
					{
#line 132 "lex/parser_strings.rl"
						out.push_back(','); }
					
#line 774 "lex/parser_strings.cpp"
					
					goto _st2;
					_ctr28:
					{
#line 142 "lex/parser_strings.rl"
						rl_esc_count++; }
					
#line 782 "lex/parser_strings.cpp"
					
					{
#line 138 "lex/parser_strings.rl"
						out.push_back(':'); }
					
#line 788 "lex/parser_strings.cpp"
					
					goto _st2;
					_ctr29:
					{
#line 142 "lex/parser_strings.rl"
						rl_esc_count++; }
					
#line 796 "lex/parser_strings.cpp"
					
					{
#line 133 "lex/parser_strings.rl"
						out.push_back(';'); }
					
#line 802 "lex/parser_strings.cpp"
					
					goto _st2;
					_ctr30:
					{
#line 142 "lex/parser_strings.rl"
						rl_esc_count++; }
					
#line 810 "lex/parser_strings.cpp"
					
					{
#line 139 "lex/parser_strings.rl"
						out.push_back('@'); }
					
#line 816 "lex/parser_strings.cpp"
					
					goto _st2;
					_ctr31:
					{
#line 142 "lex/parser_strings.rl"
						rl_esc_count++; }
					
#line 824 "lex/parser_strings.cpp"
					
					{
#line 136 "lex/parser_strings.rl"
						out.push_back('['); }
					
#line 830 "lex/parser_strings.cpp"
					
					goto _st2;
					_ctr32:
					{
#line 142 "lex/parser_strings.rl"
						rl_esc_count++; }
					
#line 838 "lex/parser_strings.cpp"
					
					{
#line 130 "lex/parser_strings.rl"
						out.push_back('\\'); }
					
#line 844 "lex/parser_strings.cpp"
					
					goto _st2;
					_ctr33:
					{
#line 142 "lex/parser_strings.rl"
						rl_esc_count++; }
					
#line 852 "lex/parser_strings.cpp"
					
					{
#line 137 "lex/parser_strings.rl"
						out.push_back(']'); }
					
#line 858 "lex/parser_strings.cpp"
					
					goto _st2;
					_ctr34:
					{
#line 142 "lex/parser_strings.rl"
						rl_esc_count++; }
					
#line 866 "lex/parser_strings.cpp"
					
					{
#line 128 "lex/parser_strings.rl"
						out.push_back('`');   out.push_back('`'); }
					
#line 872 "lex/parser_strings.cpp"
					
					goto _st2;
					_st2:
					p += 1;
					st_case_2:
					switch( ( (*( p))) ) {
						case 0: {
							goto _ctr20;
						}
						case 32: {
							goto _ctr21;
						}
						case 34: {
							goto _ctr22;
						}
						case 35: {
							goto _ctr23;
						}
						case 37: {
							goto _ctr24;
						}
						case 40: {
							goto _ctr25;
						}
						case 41: {
							goto _ctr26;
						}
						case 44: {
							goto _ctr27;
						}
						case 58: {
							goto _ctr28;
						}
						case 59: {
							goto _ctr29;
						}
						case 64: {
							goto _ctr30;
						}
						case 91: {
							goto _ctr31;
						}
						case 92: {
							goto _ctr32;
						}
						case 93: {
							goto _ctr33;
						}
						case 96: {
							goto _ctr34;
						}
					}
					{
						goto _ctr19;
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
				
#line 167 "lex/parser_strings.rl"
				
				
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
		
	}
}
