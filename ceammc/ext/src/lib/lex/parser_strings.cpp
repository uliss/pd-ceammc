#line 1 "lex/parser_strings.rl"
# include "parser_strings.h"
# include "ceammc_format.h"


#line 7 "lex/parser_strings.cpp"
static const int quoted_string_end_start = 1;
static const int quoted_string_end_first_final = 4;
static const int quoted_string_end_error = 0;

static const int quoted_string_end_en_main = 1;


#line 14 "lex/parser_strings.rl"


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
			
			
#line 47 "lex/parser_strings.cpp"
			{
				cs = (int)quoted_string_end_start;
			}
			
#line 44 "lex/parser_strings.rl"
			
			
#line 55 "lex/parser_strings.cpp"
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
#line 12 "lex/parser_strings.rl"
					{p+= 1; cs = 4; goto _out;} }
				
#line 105 "lex/parser_strings.cpp"
				
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
				if ( ( (*( p))) <= -1 ) {
					goto _st0;
				}
				{
					goto _st1;
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
			
#line 45 "lex/parser_strings.rl"
			
			
			return (cs >= 
#line 139 "lex/parser_strings.cpp"
			4
#line 47 "lex/parser_strings.rl"
			);
		}
		
		
#line 146 "lex/parser_strings.cpp"
		static const int is_quoted_string_start = 1;
		static const int is_quoted_string_first_final = 5;
		static const int is_quoted_string_error = 0;
		
		static const int is_quoted_string_en_main = 1;
		
		
#line 58 "lex/parser_strings.rl"
		
		
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
			
			
#line 190 "lex/parser_strings.cpp"
			{
				cs = (int)is_quoted_string_start;
			}
			
#line 92 "lex/parser_strings.rl"
			
			
#line 198 "lex/parser_strings.cpp"
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
#line 56 "lex/parser_strings.rl"
					{p+= 1; cs = 5; goto _out;} }
				
#line 258 "lex/parser_strings.cpp"
				
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
				if ( 0 <= ( (*( p))) ) {
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
			
#line 93 "lex/parser_strings.rl"
			
			
			return (cs >= 
#line 293 "lex/parser_strings.cpp"
			5
#line 95 "lex/parser_strings.rl"
			);
		}
		
		
#line 300 "lex/parser_strings.cpp"
		static const int need_quotes_start = 1;
		static const int need_quotes_first_final = 2;
		static const int need_quotes_error = 0;
		
		static const int need_quotes_en_main = 1;
		
		
#line 105 "lex/parser_strings.rl"
		
		
		bool string_need_quotes(const char* str) {
			if (str == nullptr || str[0] == '\0')
				return false;
			
			int cs = 0;
			const char* p = str;
			
			
#line 319 "lex/parser_strings.cpp"
			{
				cs = (int)need_quotes_start;
			}
			
#line 114 "lex/parser_strings.rl"
			
			
#line 327 "lex/parser_strings.cpp"
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
					case 44: {
						goto _st0;
					}
					case 59: {
						goto _st0;
					}
					case 92: {
						goto _st0;
					}
					case 96: {
						goto _st0;
					}
				}
				{
					goto _st1;
				}
				_ctr2:
				{
#line 103 "lex/parser_strings.rl"
					{p+= 1; cs = 2; goto _out;} }
				
#line 372 "lex/parser_strings.cpp"
				
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
			
#line 115 "lex/parser_strings.rl"
			
			
			return (cs < 
#line 399 "lex/parser_strings.cpp"
			2
#line 117 "lex/parser_strings.rl"
			);
		}
		
		
#line 406 "lex/parser_strings.cpp"
		static const int escape_and_quote_start = 1;
		static const int escape_and_quote_first_final = 3;
		static const int escape_and_quote_error = 0;
		
		static const int escape_and_quote_en_main = 1;
		
		
#line 138 "lex/parser_strings.rl"
		
		
		bool escape_and_quote(const char* str, std::string& out)
		{
			if (str == nullptr || str[0] == '\0') {
				out = "\"\"";
				return true;
			}
			
			int cs = 0;
			const char* p = str;
			std::string& rl_str = out;
			int rl_esc_count = 0;
			
			rl_str += '"';
			
			
#line 432 "lex/parser_strings.cpp"
			{
				cs = (int)escape_and_quote_start;
			}
			
#line 154 "lex/parser_strings.rl"
			
			
#line 440 "lex/parser_strings.cpp"
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
#line 134 "lex/parser_strings.rl"
					rl_str += (( (*( p)))); }
				
#line 458 "lex/parser_strings.cpp"
				
				goto _st1;
				_ctr11:
				{
#line 132 "lex/parser_strings.rl"
					rl_esc_count++; }
				
#line 466 "lex/parser_strings.cpp"
				
				{
#line 134 "lex/parser_strings.rl"
					rl_str += (( (*( p)))); }
				
#line 472 "lex/parser_strings.cpp"
				
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
					case 44: {
						goto _ctr6;
					}
					case 59: {
						goto _ctr7;
					}
					case 92: {
						goto _ctr8;
					}
					case 96: {
						goto _ctr9;
					}
				}
				{
					goto _ctr2;
				}
				_ctr3:
				{
#line 136 "lex/parser_strings.rl"
					{p+= 1; cs = 3; goto _out;} }
				
#line 509 "lex/parser_strings.cpp"
				
				goto _st3;
				_ctr12:
				{
#line 132 "lex/parser_strings.rl"
					rl_esc_count++; }
				
#line 517 "lex/parser_strings.cpp"
				
				{
#line 136 "lex/parser_strings.rl"
					{p+= 1; cs = 3; goto _out;} }
				
#line 523 "lex/parser_strings.cpp"
				
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
#line 128 "lex/parser_strings.rl"
					rl_str += '\\';  rl_str += ' '; }
				
#line 541 "lex/parser_strings.cpp"
				
				goto _st2;
				_ctr5:
				{
#line 127 "lex/parser_strings.rl"
					rl_str += '`';   rl_str += '"'; }
				
#line 549 "lex/parser_strings.cpp"
				
				goto _st2;
				_ctr6:
				{
#line 129 "lex/parser_strings.rl"
					rl_str += '\\';  rl_str += ','; }
				
#line 557 "lex/parser_strings.cpp"
				
				goto _st2;
				_ctr7:
				{
#line 130 "lex/parser_strings.rl"
					rl_str += '\\';  rl_str += ';'; }
				
#line 565 "lex/parser_strings.cpp"
				
				goto _st2;
				_ctr8:
				{
#line 131 "lex/parser_strings.rl"
					rl_str += '`';   rl_str += '/'; }
				
#line 573 "lex/parser_strings.cpp"
				
				goto _st2;
				_ctr9:
				{
#line 126 "lex/parser_strings.rl"
					rl_str += '`';   rl_str += '`'; }
				
#line 581 "lex/parser_strings.cpp"
				
				goto _st2;
				_ctr13:
				{
#line 132 "lex/parser_strings.rl"
					rl_esc_count++; }
				
#line 589 "lex/parser_strings.cpp"
				
				{
#line 128 "lex/parser_strings.rl"
					rl_str += '\\';  rl_str += ' '; }
				
#line 595 "lex/parser_strings.cpp"
				
				goto _st2;
				_ctr14:
				{
#line 132 "lex/parser_strings.rl"
					rl_esc_count++; }
				
#line 603 "lex/parser_strings.cpp"
				
				{
#line 127 "lex/parser_strings.rl"
					rl_str += '`';   rl_str += '"'; }
				
#line 609 "lex/parser_strings.cpp"
				
				goto _st2;
				_ctr15:
				{
#line 132 "lex/parser_strings.rl"
					rl_esc_count++; }
				
#line 617 "lex/parser_strings.cpp"
				
				{
#line 129 "lex/parser_strings.rl"
					rl_str += '\\';  rl_str += ','; }
				
#line 623 "lex/parser_strings.cpp"
				
				goto _st2;
				_ctr16:
				{
#line 132 "lex/parser_strings.rl"
					rl_esc_count++; }
				
#line 631 "lex/parser_strings.cpp"
				
				{
#line 130 "lex/parser_strings.rl"
					rl_str += '\\';  rl_str += ';'; }
				
#line 637 "lex/parser_strings.cpp"
				
				goto _st2;
				_ctr17:
				{
#line 132 "lex/parser_strings.rl"
					rl_esc_count++; }
				
#line 645 "lex/parser_strings.cpp"
				
				{
#line 131 "lex/parser_strings.rl"
					rl_str += '`';   rl_str += '/'; }
				
#line 651 "lex/parser_strings.cpp"
				
				goto _st2;
				_ctr18:
				{
#line 132 "lex/parser_strings.rl"
					rl_esc_count++; }
				
#line 659 "lex/parser_strings.cpp"
				
				{
#line 126 "lex/parser_strings.rl"
					rl_str += '`';   rl_str += '`'; }
				
#line 665 "lex/parser_strings.cpp"
				
				goto _st2;
				_st2:
				p += 1;
				st_case_2:
				switch( ( (*( p))) ) {
					case 0: {
						goto _ctr12;
					}
					case 32: {
						goto _ctr13;
					}
					case 34: {
						goto _ctr14;
					}
					case 44: {
						goto _ctr15;
					}
					case 59: {
						goto _ctr16;
					}
					case 92: {
						goto _ctr17;
					}
					case 96: {
						goto _ctr18;
					}
				}
				{
					goto _ctr11;
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
			
#line 155 "lex/parser_strings.rl"
			
			
			rl_str += '"';
			return rl_esc_count > 0;
		}
		
		
#line 716 "lex/parser_strings.cpp"
		static const int escape_and_quote_buf_start = 1;
		static const int escape_and_quote_buf_first_final = 3;
		static const int escape_and_quote_buf_error = 0;
		
		static const int escape_and_quote_buf_en_main = 1;
		
		
#line 179 "lex/parser_strings.rl"
		
		
		int escape_and_quote(const char* str, char* buf, size_t buf_len)
		{
#define append_buf(c) { if (rl_n-- > 1) *(rl_buf++) = c; else rl_overflow = true; }
			
			if (!buf || buf_len < 3)
				return -1;
			
			if (str == nullptr || str[0] == '\0') {
				buf[0] = '"';
				buf[1] = '"';
				buf[2] = 0;
				return 2;
			}
			
			int cs = 0;
			const char* p = str;
			char* rl_buf = buf;
			size_t rl_n = buf_len;
			size_t rl_esc_n = 0;
			bool rl_overflow = false;
			
			append_buf('"');
			
			
#line 751 "lex/parser_strings.cpp"
			{
				cs = (int)escape_and_quote_buf_start;
			}
			
#line 204 "lex/parser_strings.rl"
			
			
#line 759 "lex/parser_strings.cpp"
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
#line 175 "lex/parser_strings.rl"
					append_buf((( (*( p))))); }
				
#line 777 "lex/parser_strings.cpp"
				
				goto _st1;
				_ctr11:
				{
#line 173 "lex/parser_strings.rl"
					rl_esc_n++; }
				
#line 785 "lex/parser_strings.cpp"
				
				{
#line 175 "lex/parser_strings.rl"
					append_buf((( (*( p))))); }
				
#line 791 "lex/parser_strings.cpp"
				
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
					case 44: {
						goto _ctr6;
					}
					case 59: {
						goto _ctr7;
					}
					case 92: {
						goto _ctr8;
					}
					case 96: {
						goto _ctr9;
					}
				}
				{
					goto _ctr2;
				}
				_ctr3:
				{
#line 177 "lex/parser_strings.rl"
					{p+= 1; cs = 3; goto _out;} }
				
#line 828 "lex/parser_strings.cpp"
				
				goto _st3;
				_ctr12:
				{
#line 173 "lex/parser_strings.rl"
					rl_esc_n++; }
				
#line 836 "lex/parser_strings.cpp"
				
				{
#line 177 "lex/parser_strings.rl"
					{p+= 1; cs = 3; goto _out;} }
				
#line 842 "lex/parser_strings.cpp"
				
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
#line 169 "lex/parser_strings.rl"
					append_buf('\\');  append_buf(' '); }
				
#line 860 "lex/parser_strings.cpp"
				
				goto _st2;
				_ctr5:
				{
#line 168 "lex/parser_strings.rl"
					append_buf('`');   append_buf('"'); }
				
#line 868 "lex/parser_strings.cpp"
				
				goto _st2;
				_ctr6:
				{
#line 170 "lex/parser_strings.rl"
					append_buf('\\');  append_buf(','); }
				
#line 876 "lex/parser_strings.cpp"
				
				goto _st2;
				_ctr7:
				{
#line 171 "lex/parser_strings.rl"
					append_buf('\\');  append_buf(';'); }
				
#line 884 "lex/parser_strings.cpp"
				
				goto _st2;
				_ctr8:
				{
#line 172 "lex/parser_strings.rl"
					append_buf('`');   append_buf('/'); }
				
#line 892 "lex/parser_strings.cpp"
				
				goto _st2;
				_ctr9:
				{
#line 167 "lex/parser_strings.rl"
					append_buf('`');   append_buf('`'); }
				
#line 900 "lex/parser_strings.cpp"
				
				goto _st2;
				_ctr13:
				{
#line 173 "lex/parser_strings.rl"
					rl_esc_n++; }
				
#line 908 "lex/parser_strings.cpp"
				
				{
#line 169 "lex/parser_strings.rl"
					append_buf('\\');  append_buf(' '); }
				
#line 914 "lex/parser_strings.cpp"
				
				goto _st2;
				_ctr14:
				{
#line 173 "lex/parser_strings.rl"
					rl_esc_n++; }
				
#line 922 "lex/parser_strings.cpp"
				
				{
#line 168 "lex/parser_strings.rl"
					append_buf('`');   append_buf('"'); }
				
#line 928 "lex/parser_strings.cpp"
				
				goto _st2;
				_ctr15:
				{
#line 173 "lex/parser_strings.rl"
					rl_esc_n++; }
				
#line 936 "lex/parser_strings.cpp"
				
				{
#line 170 "lex/parser_strings.rl"
					append_buf('\\');  append_buf(','); }
				
#line 942 "lex/parser_strings.cpp"
				
				goto _st2;
				_ctr16:
				{
#line 173 "lex/parser_strings.rl"
					rl_esc_n++; }
				
#line 950 "lex/parser_strings.cpp"
				
				{
#line 171 "lex/parser_strings.rl"
					append_buf('\\');  append_buf(';'); }
				
#line 956 "lex/parser_strings.cpp"
				
				goto _st2;
				_ctr17:
				{
#line 173 "lex/parser_strings.rl"
					rl_esc_n++; }
				
#line 964 "lex/parser_strings.cpp"
				
				{
#line 172 "lex/parser_strings.rl"
					append_buf('`');   append_buf('/'); }
				
#line 970 "lex/parser_strings.cpp"
				
				goto _st2;
				_ctr18:
				{
#line 173 "lex/parser_strings.rl"
					rl_esc_n++; }
				
#line 978 "lex/parser_strings.cpp"
				
				{
#line 167 "lex/parser_strings.rl"
					append_buf('`');   append_buf('`'); }
				
#line 984 "lex/parser_strings.cpp"
				
				goto _st2;
				_st2:
				p += 1;
				st_case_2:
				switch( ( (*( p))) ) {
					case 0: {
						goto _ctr12;
					}
					case 32: {
						goto _ctr13;
					}
					case 34: {
						goto _ctr14;
					}
					case 44: {
						goto _ctr15;
					}
					case 59: {
						goto _ctr16;
					}
					case 92: {
						goto _ctr17;
					}
					case 96: {
						goto _ctr18;
					}
				}
				{
					goto _ctr11;
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
			
#line 205 "lex/parser_strings.rl"
			
			
			append_buf('"');
			
			if (rl_overflow) {
				buf[0] = 0;
				return -2;
			} else {
				*rl_buf = 0;
				return buf_len - rl_n;
			}
		}
		
	}
}
