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
		static const int escape_and_quote_first_final = 2;
		static const int escape_and_quote_error = 0;
		
		static const int escape_and_quote_en_main = 1;
		
		
#line 139 "lex/parser_strings.rl"
		
		
		template <typename T>
		static bool escape_and_quote_t(const char* str, T& out) noexcept
		{
			try {
				if (str == nullptr || str[0] == '\0') {
					out.push_back('"');
					out.push_back('"');
					return true;
				}
				
				int cs = 0;
				const char* p = str;
				
				out.push_back('"');
				
				
#line 438 "lex/parser_strings.cpp"
				{
					cs = (int)escape_and_quote_start;
				}
				
#line 156 "lex/parser_strings.rl"
				
				
#line 446 "lex/parser_strings.cpp"
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
					_ctr2:
					{
#line 135 "lex/parser_strings.rl"
						out.push_back((( (*( p))))); }
					
#line 462 "lex/parser_strings.cpp"
					
					goto _st1;
					_ctr4:
					{
#line 130 "lex/parser_strings.rl"
						out.push_back('\\');  out.push_back(' '); }
					
#line 470 "lex/parser_strings.cpp"
					
					goto _st1;
					_ctr5:
					{
#line 129 "lex/parser_strings.rl"
						out.push_back('`');   out.push_back('"'); }
					
#line 478 "lex/parser_strings.cpp"
					
					goto _st1;
					_ctr6:
					{
#line 131 "lex/parser_strings.rl"
						out.push_back('\\');  out.push_back(','); }
					
#line 486 "lex/parser_strings.cpp"
					
					goto _st1;
					_ctr7:
					{
#line 132 "lex/parser_strings.rl"
						out.push_back('\\');  out.push_back(';'); }
					
#line 494 "lex/parser_strings.cpp"
					
					goto _st1;
					_ctr8:
					{
#line 133 "lex/parser_strings.rl"
						out.push_back('`');   out.push_back('/'); }
					
#line 502 "lex/parser_strings.cpp"
					
					goto _st1;
					_ctr9:
					{
#line 128 "lex/parser_strings.rl"
						out.push_back('`');   out.push_back('`'); }
					
#line 510 "lex/parser_strings.cpp"
					
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
#line 137 "lex/parser_strings.rl"
						{p+= 1; cs = 2; goto _out;} }
					
#line 547 "lex/parser_strings.cpp"
					
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
				
#line 157 "lex/parser_strings.rl"
				
				
				out.push_back('"');
				return true;
			} catch(std::exception& e) {
				LIB_ERR << fmt::format("[{}] error '{}'", __FUNCTION__, e.what());
				return false;
			}
		}
		
		void escape_and_quote(const char* str, std::string& out)
		{
			MediumString buf;
			escape_and_quote_t(str, buf);
			out.assign(buf.data(), buf.size());
		}
		
		void escape_and_quote(const char* str, SmallString& out)
		{
			escape_and_quote_t(str, out);
		}
		
		void escape_and_quote(const char* str, MediumString& out)
		{
			escape_and_quote_t(str, out);
		}
		
		bool escape_and_quote(const char* str, StaticString& out)
		{
			return escape_and_quote_t(str, out);
		}
		
	}
}
