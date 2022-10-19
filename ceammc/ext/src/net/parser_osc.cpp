#line 1 "parser_osc.rl"
# include "parser_osc.h"
# include "ceammc_string_types.h"


#line 7 "parser_osc.cpp"
static const int parser_osc_start = 1;
static const int parser_osc_first_final = 32;
static const int parser_osc_error = 0;

static const int parser_osc_en_main = 1;


#line 57 "parser_osc.rl"



namespace ceammc {
	namespace parser {
		
		OscAddress parse_osc_url(const char* str, OscProto& proto, t_symbol*& host, int& port, t_symbol*& unix_path) {
			int cs = 0;
			const char* p = str;
			string::SmallString rl_host;
			string::SmallString rl_path;
			int rl_port = 0;
			OscProto rl_proto = OSC_PROTO_UDP;
			OscAddress rl_addr = OSC_ADDR_INVALID;
			
			
#line 32 "parser_osc.cpp"
			{
				cs = (int)parser_osc_start;
			}
			
#line 72 "parser_osc.rl"
			
			
#line 40 "parser_osc.cpp"
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
					case 12:
					goto st_case_12;
					case 13:
					goto st_case_13;
					case 32:
					goto st_case_32;
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
					case 20:
					goto st_case_20;
					case 21:
					goto st_case_21;
					case 22:
					goto st_case_22;
					case 23:
					goto st_case_23;
					case 24:
					goto st_case_24;
					case 25:
					goto st_case_25;
					case 26:
					goto st_case_26;
					case 27:
					goto st_case_27;
					case 28:
					goto st_case_28;
					case 29:
					goto st_case_29;
					case 30:
					goto st_case_30;
					case 31:
					goto st_case_31;
				}
				goto st_out;
				p += 1;
				st_case_1:
				switch( ( (*( p))) ) {
					case 111: {
						goto _st2;
					}
					case 116: {
						goto _st24;
					}
					case 117: {
						goto _st29;
					}
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
					goto _ctr13;
				}
				{
					goto _st0;
				}
				_ctr13:
				{
#line 39 "parser_osc.rl"
					rl_proto = OSC_PROTO_TCP; }
				
#line 209 "parser_osc.cpp"
				
				goto _st9;
				_ctr24:
				{
#line 40 "parser_osc.rl"
					rl_proto = OSC_PROTO_UDP; }
				
#line 217 "parser_osc.cpp"
				
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
				_ctr16:
				{
#line 43 "parser_osc.rl"
					rl_host.push_back((( (*( p))))); }
				
#line 243 "parser_osc.cpp"
				
				goto _st11;
				_st11:
				p += 1;
				st_case_11:
				switch( ( (*( p))) ) {
					case 0: {
						goto _st0;
					}
					case 58: {
						goto _st12;
					}
					case 93: {
						goto _st0;
					}
				}
				{
					goto _ctr16;
				}
				_st12:
				p += 1;
				st_case_12:
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr18;
				}
				{
					goto _st0;
				}
				_ctr18:
				{
#line 44 "parser_osc.rl"
					(rl_port *= 10) += ((( (*( p)))) - '0'); }
				
#line 277 "parser_osc.cpp"
				
				{
#line 50 "parser_osc.rl"
					rl_addr = OSC_ADDR_URL; }
				
#line 283 "parser_osc.cpp"
				
				goto _st13;
				_st13:
				p += 1;
				st_case_13:
				if ( ( (*( p))) == 0 ) {
					goto _ctr20;
				}
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr18;
				}
				{
					goto _st0;
				}
				_ctr20:
				{
#line 54 "parser_osc.rl"
					{p+= 1; cs = 32; goto _out;} }
				
#line 303 "parser_osc.cpp"
				
				goto _st32;
				_st32:
				p += 1;
				st_case_32:
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
					goto _ctr24;
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
					goto _ctr27;
				}
				{
					goto _st0;
				}
				_ctr27:
				{
#line 42 "parser_osc.rl"
					rl_proto = OSC_PROTO_UNIX; }
				
#line 376 "parser_osc.cpp"
				
				goto _st20;
				_st20:
				p += 1;
				st_case_20:
				if ( ( (*( p))) == 47 ) {
					goto _st21;
				}
				{
					goto _st0;
				}
				_st21:
				p += 1;
				st_case_21:
				if ( ( (*( p))) == 47 ) {
					goto _st22;
				}
				{
					goto _st0;
				}
				_st22:
				p += 1;
				st_case_22:
				if ( ( (*( p))) == 0 ) {
					goto _st0;
				}
				{
					goto _ctr31;
				}
				_ctr31:
				{
#line 47 "parser_osc.rl"
					rl_path.push_back((( (*( p))))); }
				
#line 411 "parser_osc.cpp"
				
				{
#line 51 "parser_osc.rl"
					rl_addr = OSC_ADDR_UNIX; }
				
#line 417 "parser_osc.cpp"
				
				goto _st23;
				_st23:
				p += 1;
				st_case_23:
				if ( ( (*( p))) == 0 ) {
					goto _ctr20;
				}
				{
					goto _ctr31;
				}
				_st24:
				p += 1;
				st_case_24:
				if ( ( (*( p))) == 99 ) {
					goto _st25;
				}
				{
					goto _st0;
				}
				_st25:
				p += 1;
				st_case_25:
				if ( ( (*( p))) == 112 ) {
					goto _st26;
				}
				{
					goto _st0;
				}
				_st26:
				p += 1;
				st_case_26:
				if ( ( (*( p))) == 58 ) {
					goto _ctr35;
				}
				{
					goto _st0;
				}
				_ctr35:
				{
#line 39 "parser_osc.rl"
					rl_proto = OSC_PROTO_TCP; }
				
#line 461 "parser_osc.cpp"
				
				goto _st27;
				_ctr41:
				{
#line 40 "parser_osc.rl"
					rl_proto = OSC_PROTO_UDP; }
				
#line 469 "parser_osc.cpp"
				
				goto _st27;
				_st27:
				p += 1;
				st_case_27:
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr37;
				}
				{
					goto _st0;
				}
				_ctr37:
				{
#line 44 "parser_osc.rl"
					(rl_port *= 10) += ((( (*( p)))) - '0'); }
				
#line 486 "parser_osc.cpp"
				
				{
#line 52 "parser_osc.rl"
					rl_addr = OSC_ADDR_PROTO_PORT; }
				
#line 492 "parser_osc.cpp"
				
				goto _st28;
				_st28:
				p += 1;
				st_case_28:
				if ( ( (*( p))) == 0 ) {
					goto _ctr20;
				}
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr37;
				}
				{
					goto _st0;
				}
				_st29:
				p += 1;
				st_case_29:
				if ( ( (*( p))) == 100 ) {
					goto _st30;
				}
				{
					goto _st0;
				}
				_st30:
				p += 1;
				st_case_30:
				if ( ( (*( p))) == 112 ) {
					goto _st31;
				}
				{
					goto _st0;
				}
				_st31:
				p += 1;
				st_case_31:
				if ( ( (*( p))) == 58 ) {
					goto _ctr41;
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
				_test_eof12: cs = 12; goto _test_eof; 
				_test_eof13: cs = 13; goto _test_eof; 
				_test_eof32: cs = 32; goto _test_eof; 
				_test_eof14: cs = 14; goto _test_eof; 
				_test_eof15: cs = 15; goto _test_eof; 
				_test_eof16: cs = 16; goto _test_eof; 
				_test_eof17: cs = 17; goto _test_eof; 
				_test_eof18: cs = 18; goto _test_eof; 
				_test_eof19: cs = 19; goto _test_eof; 
				_test_eof20: cs = 20; goto _test_eof; 
				_test_eof21: cs = 21; goto _test_eof; 
				_test_eof22: cs = 22; goto _test_eof; 
				_test_eof23: cs = 23; goto _test_eof; 
				_test_eof24: cs = 24; goto _test_eof; 
				_test_eof25: cs = 25; goto _test_eof; 
				_test_eof26: cs = 26; goto _test_eof; 
				_test_eof27: cs = 27; goto _test_eof; 
				_test_eof28: cs = 28; goto _test_eof; 
				_test_eof29: cs = 29; goto _test_eof; 
				_test_eof30: cs = 30; goto _test_eof; 
				_test_eof31: cs = 31; goto _test_eof; 
				
				_test_eof: {}
				if ( cs >= 32 )
					goto _out; _pop: {}
				_out: {}
			}
			
#line 73 "parser_osc.rl"
			
			
			if (cs >= 
#line 578 "parser_osc.cpp"
			32
#line 75 "parser_osc.rl"
			) {
				rl_host.push_back('\0');
				proto = rl_proto;
				host = gensym(rl_host.data());
				port = rl_port;
				unix_path = gensym(rl_path.data());
			}
			
			return rl_addr;
		}
	}
}

