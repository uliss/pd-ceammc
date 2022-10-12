#line 1 "parser_osc.rl"
# include "parser_osc.h"
# include "ceammc_string_types.h"


#line 7 "parser_osc.cpp"
static const int parser_osc_start = 1;
static const int parser_osc_first_final = 20;
static const int parser_osc_error = 0;

static const int parser_osc_en_main = 1;


#line 50 "parser_osc.rl"



namespace ceammc {
	namespace parser {
		
		bool parse_osc_url(const char* str, t_symbol*& proto, t_symbol*& host, t_symbol*& port, int& iport) {
			int cs = 0;
			const char* p = str;
			string::SmallString rl_port;
			string::SmallString rl_host;
			int rl_int_port = 0;
			t_symbol* rl_proto = nullptr;
			
			
#line 31 "parser_osc.cpp"
			{
				cs = (int)parser_osc_start;
			}
			
#line 64 "parser_osc.rl"
			
			
#line 39 "parser_osc.cpp"
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
					case 20:
					goto st_case_20;
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
					case 17:
					goto st_case_17;
					case 18:
					goto st_case_18;
					case 19:
					goto st_case_19;
				}
				goto st_out;
				p += 1;
				st_case_1:
				if ( ( (*( p))) == 111 ) {
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
				if ( ( (*( p))) == 115 ) {
					goto _st3;
				}
				{
					goto _st0;
				}
				_st3:
				p += 1;
				st_case_3:
				if ( ( (*( p))) == 99 ) {
					goto _st4;
				}
				{
					goto _st0;
				}
				_st4:
				p += 1;
				st_case_4:
				switch( ( (*( p))) ) {
					case 46: {
						goto _st5;
					}
					case 58: {
						goto _st9;
					}
				}
				{
					goto _st0;
				}
				_st5:
				p += 1;
				st_case_5:
				switch( ( (*( p))) ) {
					case 116: {
						goto _st6;
					}
					case 117: {
						goto _st14;
					}
				}
				{
					goto _st0;
				}
				_st6:
				p += 1;
				st_case_6:
				if ( ( (*( p))) == 99 ) {
					goto _st7;
				}
				{
					goto _st0;
				}
				_st7:
				p += 1;
				st_case_7:
				if ( ( (*( p))) == 112 ) {
					goto _st8;
				}
				{
					goto _st0;
				}
				_st8:
				p += 1;
				st_case_8:
				if ( ( (*( p))) == 58 ) {
					goto _ctr11;
				}
				{
					goto _st0;
				}
				_ctr11:
				{
#line 39 "parser_osc.rl"
					rl_proto = gensym("tcp"); }
				
#line 176 "parser_osc.cpp"
				
				goto _st9;
				_ctr22:
				{
#line 40 "parser_osc.rl"
					rl_proto = gensym("udp"); }
				
#line 184 "parser_osc.cpp"
				
				goto _st9;
				_ctr25:
				{
#line 41 "parser_osc.rl"
					rl_proto = gensym("unix"); }
				
#line 192 "parser_osc.cpp"
				
				goto _st9;
				_st9:
				p += 1;
				st_case_9:
				if ( ( (*( p))) == 47 ) {
					goto _st10;
				}
				{
					goto _st0;
				}
				_st10:
				p += 1;
				st_case_10:
				if ( ( (*( p))) == 47 ) {
					goto _st11;
				}
				{
					goto _st0;
				}
				_ctr14:
				{
#line 43 "parser_osc.rl"
					rl_host.push_back((( (*( p))))); }
				
#line 218 "parser_osc.cpp"
				
				goto _st11;
				_st11:
				p += 1;
				st_case_11:
				switch( ( (*( p))) ) {
					case 0: {
						goto _ctr15;
					}
					case 58: {
						goto _st12;
					}
					case 93: {
						goto _st0;
					}
				}
				{
					goto _ctr14;
				}
				_ctr15:
				{
#line 47 "parser_osc.rl"
					{p+= 1; cs = 20; goto _out;} }
				
#line 243 "parser_osc.cpp"
				
				goto _st20;
				_st20:
				p += 1;
				st_case_20:
				{
					goto _st0;
				}
				_st12:
				p += 1;
				st_case_12:
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr17;
				}
				{
					goto _st0;
				}
				_ctr17:
				{
#line 44 "parser_osc.rl"
					rl_port.push_back((( (*( p))))); (rl_int_port *= 10) += ((( (*( p)))) - '0'); }
				
#line 266 "parser_osc.cpp"
				
				goto _st13;
				_st13:
				p += 1;
				st_case_13:
				if ( ( (*( p))) == 0 ) {
					goto _ctr15;
				}
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr17;
				}
				{
					goto _st0;
				}
				_st14:
				p += 1;
				st_case_14:
				switch( ( (*( p))) ) {
					case 100: {
						goto _st15;
					}
					case 110: {
						goto _st17;
					}
				}
				{
					goto _st0;
				}
				_st15:
				p += 1;
				st_case_15:
				if ( ( (*( p))) == 112 ) {
					goto _st16;
				}
				{
					goto _st0;
				}
				_st16:
				p += 1;
				st_case_16:
				if ( ( (*( p))) == 58 ) {
					goto _ctr22;
				}
				{
					goto _st0;
				}
				_st17:
				p += 1;
				st_case_17:
				if ( ( (*( p))) == 105 ) {
					goto _st18;
				}
				{
					goto _st0;
				}
				_st18:
				p += 1;
				st_case_18:
				if ( ( (*( p))) == 120 ) {
					goto _st19;
				}
				{
					goto _st0;
				}
				_st19:
				p += 1;
				st_case_19:
				if ( ( (*( p))) == 58 ) {
					goto _ctr25;
				}
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
				_test_eof9: cs = 9; goto _test_eof; 
				_test_eof10: cs = 10; goto _test_eof; 
				_test_eof11: cs = 11; goto _test_eof; 
				_test_eof20: cs = 20; goto _test_eof; 
				_test_eof12: cs = 12; goto _test_eof; 
				_test_eof13: cs = 13; goto _test_eof; 
				_test_eof14: cs = 14; goto _test_eof; 
				_test_eof15: cs = 15; goto _test_eof; 
				_test_eof16: cs = 16; goto _test_eof; 
				_test_eof17: cs = 17; goto _test_eof; 
				_test_eof18: cs = 18; goto _test_eof; 
				_test_eof19: cs = 19; goto _test_eof; 
				
				_test_eof: {}
				if ( cs >= 20 )
					goto _out; _pop: {}
				_out: {}
			}
			
#line 65 "parser_osc.rl"
			
			
			if (cs >= 
#line 372 "parser_osc.cpp"
			20
#line 67 "parser_osc.rl"
			) {
				rl_host.push_back('\0');
				rl_port.push_back('\0');
				proto = (rl_proto != nullptr) ? rl_proto : gensym("udp");
				host = gensym(rl_host.data());
				port = gensym(rl_port.data());
				iport = rl_int_port;
				return true;
			} else
			return false;
		}
	}
}

