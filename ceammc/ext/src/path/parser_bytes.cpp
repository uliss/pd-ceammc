#line 1 "parser_bytes.rl"
# include "parser_bytes.h"
# include "fmt/core.h"
# include "ceammc_format.h"


#line 8 "parser_bytes.cpp"
static const int bytes_start = 1;
static const int bytes_first_final = 6;
static const int bytes_error = 0;

static const int bytes_en_main = 1;


#line 18 "parser_bytes.rl"


namespace ceammc {
	
	int parse_byte_string(const char* str)
	{
		int cs = 0;
		int rl_val = 0;
		
		const char* p = str;
		
		
#line 29 "parser_bytes.cpp"
		{
			cs = (int)bytes_start;
		}
		
#line 29 "parser_bytes.rl"
		
		
#line 37 "parser_bytes.cpp"
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
				case 4:
				goto st_case_4;
				case 5:
				goto st_case_5;
				case 6:
				goto st_case_6;
			}
			goto st_out;
			p += 1;
			st_case_1:
			if ( ( (*( p))) == 48 ) {
				goto _ctr2;
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
#line 8 "parser_bytes.rl"
				rl_val = 0; }
			
#line 73 "parser_bytes.cpp"
			
			goto _st2;
			_st2:
			p += 1;
			st_case_2:
			if ( ( (*( p))) == 120 ) {
				goto _st3;
			}
			{
				goto _st0;
			}
			_st3:
			p += 1;
			st_case_3:
			if ( ( (*( p))) < 65 ) {
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr5;
				}
			} else if ( ( (*( p))) > 70 ) {
				if ( 97 <= ( (*( p))) && ( (*( p))) <= 102 ) {
					goto _ctr7;
				}
			} else {
				goto _ctr6;
			}
			{
				goto _st0;
			}
			_ctr5:
			{
#line 9 "parser_bytes.rl"
				(rl_val <<= 4) |= ((( (*( p)))) - '0'); }
			
#line 107 "parser_bytes.cpp"
			
			goto _st4;
			_ctr6:
			{
#line 11 "parser_bytes.rl"
				(rl_val <<= 4) |= ((( (*( p)))) - 'A' + 10); }
			
#line 115 "parser_bytes.cpp"
			
			goto _st4;
			_ctr7:
			{
#line 10 "parser_bytes.rl"
				(rl_val <<= 4) |= ((( (*( p)))) - 'a' + 10); }
			
#line 123 "parser_bytes.cpp"
			
			goto _st4;
			_st4:
			p += 1;
			st_case_4:
			if ( ( (*( p))) < 65 ) {
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr9;
				}
			} else if ( ( (*( p))) > 70 ) {
				if ( 97 <= ( (*( p))) && ( (*( p))) <= 102 ) {
					goto _ctr11;
				}
			} else {
				goto _ctr10;
			}
			{
				goto _st0;
			}
			_ctr9:
			{
#line 9 "parser_bytes.rl"
				(rl_val <<= 4) |= ((( (*( p)))) - '0'); }
			
#line 148 "parser_bytes.cpp"
			
			goto _st5;
			_ctr10:
			{
#line 11 "parser_bytes.rl"
				(rl_val <<= 4) |= ((( (*( p)))) - 'A' + 10); }
			
#line 156 "parser_bytes.cpp"
			
			goto _st5;
			_ctr11:
			{
#line 10 "parser_bytes.rl"
				(rl_val <<= 4) |= ((( (*( p)))) - 'a' + 10); }
			
#line 164 "parser_bytes.cpp"
			
			goto _st5;
			_st5:
			p += 1;
			st_case_5:
			if ( ( (*( p))) == 0 ) {
				goto _ctr13;
			}
			{
				goto _st0;
			}
			_ctr13:
			{
#line 15 "parser_bytes.rl"
				{p+= 1; cs = 6; goto _out;} }
			
#line 181 "parser_bytes.cpp"
			
			goto _st6;
			_st6:
			p += 1;
			st_case_6:
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
			
			_test_eof: {}
			if ( cs >= 6 )
				goto _out; _pop: {}
			_out: {}
		}
		
#line 30 "parser_bytes.rl"
		
		
		if (cs >= 
#line 208 "parser_bytes.cpp"
		6
#line 32 "parser_bytes.rl"
		)
		return rl_val;
		
		return BYTE_PARSE_RESULT_INVALID_STRING;
	}
	
	std::pair<int, size_t> parse_bytes(const AtomListView& lv, std::vector<char>& out)
	{
		out.clear();
		out.reserve(lv.size());
		
		for (size_t i = 0; i < lv.size(); i++) {
			auto& a = lv[i];
			if (a.isInteger()) {
				auto val = a.asT<int>();
				if (val >= 0 && val < 255) {
					out.push_back(static_cast<char>(val));
				} else {
					return { BYTE_PARSE_RESULT_INVALID_RANGE, i };
				}
			} else if (a.isSymbol()) {
				auto val = parse_byte_string(a.asT<t_symbol*>()->s_name);
				if (val < 0)
					return { val, i };
				
				out.push_back(static_cast<char>(val));
			} else {
				return { BYTE_PARSE_RESULT_INVALID_ATOM, i };
			}
		}
		
		return { 0, 0 };
	}
	
	std::string parser_byte_error(const AtomListView& lv, const std::pair<int, size_t>& res)
	{
		switch(res.first) {
			case BYTE_PARSE_RESULT_INVALID_RANGE:
			return fmt::format("byte value out of range at [{}]: {}", res.second, to_string(lv.at(res.second)));
			case BYTE_PARSE_RESULT_INVALID_STRING:
			return fmt::format("invalid string at [{}]: {}", res.second, to_string(lv.at(res.second)));
			case BYTE_PARSE_RESULT_INVALID_ATOM:
			return fmt::format("invalid atom at [{}]: {}", res.second, to_string(lv.at(res.second)));
			default:
			return {};
		}
	}
	
	
#line 260 "parser_bytes.cpp"
	static const int open_mode_start = 1;
	static const int open_mode_first_final = 4;
	static const int open_mode_error = 0;
	
	static const int open_mode_en_main = 1;
	
	
#line 91 "parser_bytes.rl"
	
	
	std::ios::open_mode parse_mode(const char* str)
	{
		int cs = 0;
		int rl_mode = 0;
		const char* p = str;
		
		if (!p || p[0] == '\0')
			return std::ios::in | std::ios::out;
		
		
#line 281 "parser_bytes.cpp"
		{
			cs = (int)open_mode_start;
		}
		
#line 102 "parser_bytes.rl"
		
		
#line 289 "parser_bytes.cpp"
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
			p += 1;
			st_case_1:
			switch( ( (*( p))) ) {
				case 97: {
					goto _ctr2;
				}
				case 114: {
					goto _ctr3;
				}
				case 119: {
					goto _ctr4;
				}
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
#line 85 "parser_bytes.rl"
				rl_mode = std::ios::out | std::ios::app; }
			
#line 329 "parser_bytes.cpp"
			
			goto _st2;
			_ctr3:
			{
#line 83 "parser_bytes.rl"
				rl_mode = std::ios::in; }
			
#line 337 "parser_bytes.cpp"
			
			goto _st2;
			_ctr4:
			{
#line 84 "parser_bytes.rl"
				rl_mode = std::ios::out | std::ios::trunc; }
			
#line 345 "parser_bytes.cpp"
			
			goto _st2;
			_st2:
			p += 1;
			st_case_2:
			switch( ( (*( p))) ) {
				case 0: {
					goto _ctr6;
				}
				case 43: {
					goto _ctr7;
				}
			}
			{
				goto _st0;
			}
			_ctr6:
			{
#line 88 "parser_bytes.rl"
				{p+= 1; cs = 4; goto _out;} }
			
#line 367 "parser_bytes.cpp"
			
			goto _st4;
			_st4:
			p += 1;
			st_case_4:
			{
				goto _st0;
			}
			_ctr7:
			{
#line 88 "parser_bytes.rl"
				rl_mode |= std::ios::in; }
			
#line 381 "parser_bytes.cpp"
			
			goto _st3;
			_st3:
			p += 1;
			st_case_3:
			if ( ( (*( p))) == 0 ) {
				goto _ctr6;
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
		
#line 103 "parser_bytes.rl"
		
		
		if (cs >= 
#line 409 "parser_bytes.cpp"
		4
#line 105 "parser_bytes.rl"
		)
		return rl_mode;
		
		return 0;
	}
	
}
