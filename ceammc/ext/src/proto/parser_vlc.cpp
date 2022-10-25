#line 1 "parser_vlc.rl"
# include "parser_vlc.h"


#line 6 "parser_vlc.cpp"
static const int vlc_seek_parser_start = 1;
static const int vlc_seek_parser_first_final = 11;
static const int vlc_seek_parser_error = 0;

static const int vlc_seek_parser_en_main = 1;


#line 15 "parser_vlc.rl"


namespace ceammc {
	namespace parser {
		
		bool check_vlc_seek(const Atom& a)
		{
			if (a.isInteger()) return true;
				if (a.isSymbol()) return check_vlc_seek(a.asT<t_symbol*>()->s_name);
				return false;
		}
		
		bool check_vlc_seek(const char* str)
		{
			int cs = 0;
			auto* p = str;
			
			
#line 33 "parser_vlc.cpp"
			{
				cs = (int)vlc_seek_parser_start;
			}
			
#line 32 "parser_vlc.rl"
			
			
#line 41 "parser_vlc.cpp"
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
					case 11:
					goto st_case_11;
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
					case 10:
					goto st_case_10;
				}
				goto st_out;
				p += 1;
				st_case_1:
				switch( ( (*( p))) ) {
					case 43: {
						goto _st2;
					}
					case 45: {
						goto _st2;
					}
				}
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _st3;
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
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _st3;
				}
				{
					goto _st0;
				}
				_st3:
				p += 1;
				st_case_3:
				switch( ( (*( p))) ) {
					case 0: {
						goto _ctr4;
					}
					case 34: {
						goto _st4;
					}
					case 37: {
						goto _st4;
					}
					case 39: {
						goto _st5;
					}
					case 58: {
						goto _st6;
					}
					case 72: {
						goto _st8;
					}
					case 77: {
						goto _st5;
					}
					case 83: {
						goto _st4;
					}
					case 104: {
						goto _st8;
					}
					case 109: {
						goto _st5;
					}
					case 115: {
						goto _st4;
					}
				}
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _st3;
				}
				{
					goto _st0;
				}
				_ctr4:
				{
#line 12 "parser_vlc.rl"
					{p+= 1; cs = 11; goto _out;} }
				
#line 148 "parser_vlc.cpp"
				
				goto _st11;
				_st11:
				p += 1;
				st_case_11:
				{
					goto _st0;
				}
				_st4:
				p += 1;
				st_case_4:
				if ( ( (*( p))) == 0 ) {
					goto _ctr4;
				}
				{
					goto _st0;
				}
				_st5:
				p += 1;
				st_case_5:
				if ( ( (*( p))) == 58 ) {
					goto _st6;
				}
				{
					goto _st0;
				}
				_st6:
				p += 1;
				st_case_6:
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _st7;
				}
				{
					goto _st0;
				}
				_st7:
				p += 1;
				st_case_7:
				switch( ( (*( p))) ) {
					case 34: {
						goto _st4;
					}
					case 83: {
						goto _st4;
					}
					case 115: {
						goto _st4;
					}
				}
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _st7;
				}
				{
					goto _st0;
				}
				_st8:
				p += 1;
				st_case_8:
				if ( ( (*( p))) == 58 ) {
					goto _st9;
				}
				{
					goto _st0;
				}
				_st9:
				p += 1;
				st_case_9:
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _st10;
				}
				{
					goto _st0;
				}
				_st10:
				p += 1;
				st_case_10:
				switch( ( (*( p))) ) {
					case 34: {
						goto _st4;
					}
					case 39: {
						goto _st5;
					}
					case 58: {
						goto _st6;
					}
					case 77: {
						goto _st5;
					}
					case 83: {
						goto _st4;
					}
					case 109: {
						goto _st5;
					}
					case 115: {
						goto _st4;
					}
				}
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _st10;
				}
				{
					goto _st0;
				}
				st_out:
				_test_eof1: cs = 1; goto _test_eof; 
				_test_eof2: cs = 2; goto _test_eof; 
				_test_eof3: cs = 3; goto _test_eof; 
				_test_eof11: cs = 11; goto _test_eof; 
				_test_eof4: cs = 4; goto _test_eof; 
				_test_eof5: cs = 5; goto _test_eof; 
				_test_eof6: cs = 6; goto _test_eof; 
				_test_eof7: cs = 7; goto _test_eof; 
				_test_eof8: cs = 8; goto _test_eof; 
				_test_eof9: cs = 9; goto _test_eof; 
				_test_eof10: cs = 10; goto _test_eof; 
				
				_test_eof: {}
				if ( cs >= 11 )
					goto _out; _pop: {}
				_out: {}
			}
			
#line 33 "parser_vlc.rl"
			
			
			return cs >= 
#line 277 "parser_vlc.cpp"
			11
#line 35 "parser_vlc.rl"
			;
		}
		
	}
}


