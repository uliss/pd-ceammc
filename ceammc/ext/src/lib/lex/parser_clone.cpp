#line 1 "lex/parser_clone.rl"
# include "parser_clone.h"
# include <cstring>

namespace ceammc {
	namespace parser {
		
		static inline void set_args(TargetMessage& res, TargetType type, int16_t first, int16_t last, int16_t step, int8_t inlet) {
			res.first = first;
			res.last = last;
			res.step = step;
			res.target = type;
			res.inlet = inlet;
		}
		
		
#line 18 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
		static const int clone_target_start = 1;
		static const int clone_target_first_final = 21;
		static const int clone_target_error = 0;
		
		static const int clone_target_en_main = 1;
		
		
#line 51 "lex/parser_clone.rl"
		
		
		bool parse_clone_target(const char* str, TargetMessage& res)
		{
			const auto len = strlen(str);
			if (len == 0) {
				res.target = TARGET_TYPE_NONE;
				return false;
			}
			
			int cs = 0;
			const char* p = str;
			const char* pe = p + len;
			const char* eof = pe;
			
			int16_t id = 0, id0 = 1, id1 = -1, step = 1, inlet = -1;
			
			
#line 45 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
			{
				cs = (int)clone_target_start;
			}
			
#line 68 "lex/parser_clone.rl"
			
			
#line 53 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
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
					case 21:
					goto st_case_21;
					case 4:
					goto st_case_4;
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
					case 5:
					goto st_case_5;
					case 31:
					goto st_case_31;
					case 32:
					goto st_case_32;
					case 33:
					goto st_case_33;
					case 34:
					goto st_case_34;
					case 6:
					goto st_case_6;
					case 7:
					goto st_case_7;
					case 35:
					goto st_case_35;
					case 8:
					goto st_case_8;
					case 36:
					goto st_case_36;
					case 37:
					goto st_case_37;
					case 38:
					goto st_case_38;
					case 39:
					goto st_case_39;
					case 40:
					goto st_case_40;
					case 9:
					goto st_case_9;
					case 41:
					goto st_case_41;
					case 42:
					goto st_case_42;
					case 43:
					goto st_case_43;
					case 44:
					goto st_case_44;
					case 45:
					goto st_case_45;
					case 46:
					goto st_case_46;
					case 47:
					goto st_case_47;
					case 10:
					goto st_case_10;
					case 48:
					goto st_case_48;
					case 49:
					goto st_case_49;
					case 50:
					goto st_case_50;
					case 51:
					goto st_case_51;
					case 52:
					goto st_case_52;
					case 53:
					goto st_case_53;
					case 54:
					goto st_case_54;
					case 11:
					goto st_case_11;
					case 55:
					goto st_case_55;
					case 12:
					goto st_case_12;
					case 56:
					goto st_case_56;
					case 57:
					goto st_case_57;
					case 58:
					goto st_case_58;
					case 59:
					goto st_case_59;
					case 60:
					goto st_case_60;
					case 61:
					goto st_case_61;
					case 62:
					goto st_case_62;
					case 13:
					goto st_case_13;
					case 63:
					goto st_case_63;
					case 14:
					goto st_case_14;
					case 64:
					goto st_case_64;
					case 65:
					goto st_case_65;
					case 66:
					goto st_case_66;
					case 67:
					goto st_case_67;
					case 68:
					goto st_case_68;
					case 69:
					goto st_case_69;
					case 70:
					goto st_case_70;
					case 15:
					goto st_case_15;
					case 71:
					goto st_case_71;
					case 72:
					goto st_case_72;
					case 73:
					goto st_case_73;
					case 74:
					goto st_case_74;
					case 75:
					goto st_case_75;
					case 16:
					goto st_case_16;
					case 76:
					goto st_case_76;
					case 17:
					goto st_case_17;
					case 77:
					goto st_case_77;
					case 78:
					goto st_case_78;
					case 79:
					goto st_case_79;
					case 80:
					goto st_case_80;
					case 81:
					goto st_case_81;
					case 82:
					goto st_case_82;
					case 83:
					goto st_case_83;
					case 18:
					goto st_case_18;
					case 84:
					goto st_case_84;
					case 19:
					goto st_case_19;
					case 85:
					goto st_case_85;
					case 86:
					goto st_case_86;
					case 87:
					goto st_case_87;
					case 88:
					goto st_case_88;
					case 89:
					goto st_case_89;
					case 90:
					goto st_case_90;
					case 91:
					goto st_case_91;
					case 92:
					goto st_case_92;
					case 20:
					goto st_case_20;
					case 93:
					goto st_case_93;
					case 94:
					goto st_case_94;
					case 95:
					goto st_case_95;
				}
				goto st_out;
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
				p+= 1;
				if ( p == pe )
					goto _test_eof2;
				st_case_2:
				switch( ( (*( p))) ) {
					case 33: {
						goto _st3;
					}
					case 42: {
						goto _ctr4;
					}
					case 48: {
						goto _ctr5;
					}
					case 60: {
						goto _st11;
					}
					case 61: {
						goto _st15;
					}
					case 62: {
						goto _st16;
					}
					case 63: {
						goto _ctr10;
					}
				}
				if ( 49 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr6;
				}
				{
					goto _st0;
				}
				_st3:
				p+= 1;
				if ( p == pe )
					goto _test_eof3;
				st_case_3:
				if ( ( (*( p))) == 48 ) {
					goto _ctr11;
				}
				if ( 49 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr12;
				}
				{
					goto _st0;
				}
				_ctr11:
				{
#line 18 "lex/parser_clone.rl"
					id = 0; }
				
#line 319 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				{
#line 29 "lex/parser_clone.rl"
					set_args(res, TARGET_TYPE_EXCEPT, id, -1, 1,     inlet); }
				
#line 325 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				goto _st21;
				_ctr73:
				{
#line 19 "lex/parser_clone.rl"
					id = 10*id+((( (*( p))))-'0'); }
				
#line 333 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				{
#line 29 "lex/parser_clone.rl"
					set_args(res, TARGET_TYPE_EXCEPT, id, -1, 1,     inlet); }
				
#line 339 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				goto _st21;
				_st21:
				p+= 1;
				if ( p == pe )
					goto _test_eof21;
				st_case_21:
				if ( ( (*( p))) == 124 ) {
					goto _st4;
				}
				{
					goto _st0;
				}
				_st4:
				p+= 1;
				if ( p == pe )
					goto _test_eof4;
				st_case_4:
				if ( ( (*( p))) == 48 ) {
					goto _ctr14;
				}
				if ( 49 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr15;
				}
				{
					goto _st0;
				}
				_ctr14:
				{
#line 24 "lex/parser_clone.rl"
					inlet = 0; }
				
#line 372 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				{
#line 29 "lex/parser_clone.rl"
					set_args(res, TARGET_TYPE_EXCEPT, id, -1, 1,     inlet); }
				
#line 378 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				goto _st22;
				_ctr17:
				{
#line 24 "lex/parser_clone.rl"
					inlet = 0; }
				
#line 386 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				{
#line 27 "lex/parser_clone.rl"
					set_args(res, TARGET_TYPE_ALL,    -1, -1, 1,     inlet); }
				
#line 392 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				goto _st22;
				_ctr26:
				{
#line 24 "lex/parser_clone.rl"
					inlet = 0; }
				
#line 400 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				{
#line 36 "lex/parser_clone.rl"
					set_args(res, TARGET_TYPE_RANGE, id0, id1, step, inlet); }
				
#line 406 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				goto _st22;
				_ctr29:
				{
#line 24 "lex/parser_clone.rl"
					inlet = 0; }
				
#line 414 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				{
#line 30 "lex/parser_clone.rl"
					set_args(res, TARGET_TYPE_EQ,     id, -1, 1,     inlet); }
				
#line 420 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				goto _st22;
				_ctr35:
				{
#line 24 "lex/parser_clone.rl"
					inlet = 0; }
				
#line 428 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				{
#line 33 "lex/parser_clone.rl"
					set_args(res, TARGET_TYPE_LT,     id, -1, 1,     inlet); }
				
#line 434 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				goto _st22;
				_ctr40:
				{
#line 24 "lex/parser_clone.rl"
					inlet = 0; }
				
#line 442 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				{
#line 34 "lex/parser_clone.rl"
					set_args(res, TARGET_TYPE_LE,     id, -1, 1,     inlet); }
				
#line 448 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				goto _st22;
				_ctr48:
				{
#line 24 "lex/parser_clone.rl"
					inlet = 0; }
				
#line 456 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				{
#line 31 "lex/parser_clone.rl"
					set_args(res, TARGET_TYPE_GT,     id, -1, 1,     inlet); }
				
#line 462 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				goto _st22;
				_ctr53:
				{
#line 24 "lex/parser_clone.rl"
					inlet = 0; }
				
#line 470 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				{
#line 32 "lex/parser_clone.rl"
					set_args(res, TARGET_TYPE_GE,     id, -1, 1,     inlet); }
				
#line 476 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				goto _st22;
				_ctr56:
				{
#line 24 "lex/parser_clone.rl"
					inlet = 0; }
				
#line 484 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				{
#line 28 "lex/parser_clone.rl"
					set_args(res, TARGET_TYPE_RANDOM, -1, -1, 1,     inlet); }
				
#line 490 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				goto _st22;
				_ctr65:
				{
#line 25 "lex/parser_clone.rl"
					inlet = 10*inlet+((( (*( p))))-'0'); }
				
#line 498 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				{
#line 29 "lex/parser_clone.rl"
					set_args(res, TARGET_TYPE_EXCEPT, id, -1, 1,     inlet); }
				
#line 504 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				goto _st22;
				_ctr80:
				{
#line 25 "lex/parser_clone.rl"
					inlet = 10*inlet+((( (*( p))))-'0'); }
				
#line 512 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				{
#line 27 "lex/parser_clone.rl"
					set_args(res, TARGET_TYPE_ALL,    -1, -1, 1,     inlet); }
				
#line 518 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				goto _st22;
				_ctr97:
				{
#line 25 "lex/parser_clone.rl"
					inlet = 10*inlet+((( (*( p))))-'0'); }
				
#line 526 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				{
#line 36 "lex/parser_clone.rl"
					set_args(res, TARGET_TYPE_RANGE, id0, id1, step, inlet); }
				
#line 532 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				goto _st22;
				_ctr111:
				{
#line 25 "lex/parser_clone.rl"
					inlet = 10*inlet+((( (*( p))))-'0'); }
				
#line 540 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				{
#line 30 "lex/parser_clone.rl"
					set_args(res, TARGET_TYPE_EQ,     id, -1, 1,     inlet); }
				
#line 546 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				goto _st22;
				_ctr126:
				{
#line 25 "lex/parser_clone.rl"
					inlet = 10*inlet+((( (*( p))))-'0'); }
				
#line 554 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				{
#line 33 "lex/parser_clone.rl"
					set_args(res, TARGET_TYPE_LT,     id, -1, 1,     inlet); }
				
#line 560 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				goto _st22;
				_ctr141:
				{
#line 25 "lex/parser_clone.rl"
					inlet = 10*inlet+((( (*( p))))-'0'); }
				
#line 568 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				{
#line 34 "lex/parser_clone.rl"
					set_args(res, TARGET_TYPE_LE,     id, -1, 1,     inlet); }
				
#line 574 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				goto _st22;
				_ctr165:
				{
#line 25 "lex/parser_clone.rl"
					inlet = 10*inlet+((( (*( p))))-'0'); }
				
#line 582 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				{
#line 31 "lex/parser_clone.rl"
					set_args(res, TARGET_TYPE_GT,     id, -1, 1,     inlet); }
				
#line 588 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				goto _st22;
				_ctr180:
				{
#line 25 "lex/parser_clone.rl"
					inlet = 10*inlet+((( (*( p))))-'0'); }
				
#line 596 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				{
#line 32 "lex/parser_clone.rl"
					set_args(res, TARGET_TYPE_GE,     id, -1, 1,     inlet); }
				
#line 602 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				goto _st22;
				_ctr195:
				{
#line 25 "lex/parser_clone.rl"
					inlet = 10*inlet+((( (*( p))))-'0'); }
				
#line 610 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				{
#line 28 "lex/parser_clone.rl"
					set_args(res, TARGET_TYPE_RANDOM, -1, -1, 1,     inlet); }
				
#line 616 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				goto _st22;
				_st22:
				p+= 1;
				if ( p == pe )
					goto _test_eof22;
				st_case_22:
				{
					goto _st0;
				}
				_ctr15:
				{
#line 25 "lex/parser_clone.rl"
					inlet = (( (*( p))))-'0'; }
				
#line 632 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				{
#line 29 "lex/parser_clone.rl"
					set_args(res, TARGET_TYPE_EXCEPT, id, -1, 1,     inlet); }
				
#line 638 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				goto _st23;
				_st23:
				p+= 1;
				if ( p == pe )
					goto _test_eof23;
				st_case_23:
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr61;
				}
				{
					goto _st0;
				}
				_ctr61:
				{
#line 25 "lex/parser_clone.rl"
					inlet = 10*inlet+((( (*( p))))-'0'); }
				
#line 657 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				{
#line 29 "lex/parser_clone.rl"
					set_args(res, TARGET_TYPE_EXCEPT, id, -1, 1,     inlet); }
				
#line 663 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				goto _st24;
				_st24:
				p+= 1;
				if ( p == pe )
					goto _test_eof24;
				st_case_24:
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr63;
				}
				{
					goto _st0;
				}
				_ctr63:
				{
#line 25 "lex/parser_clone.rl"
					inlet = 10*inlet+((( (*( p))))-'0'); }
				
#line 682 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				{
#line 29 "lex/parser_clone.rl"
					set_args(res, TARGET_TYPE_EXCEPT, id, -1, 1,     inlet); }
				
#line 688 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				goto _st25;
				_st25:
				p+= 1;
				if ( p == pe )
					goto _test_eof25;
				st_case_25:
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr65;
				}
				{
					goto _st0;
				}
				_ctr12:
				{
#line 19 "lex/parser_clone.rl"
					id = (( (*( p))))-'0'; }
				
#line 707 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				{
#line 29 "lex/parser_clone.rl"
					set_args(res, TARGET_TYPE_EXCEPT, id, -1, 1,     inlet); }
				
#line 713 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				goto _st26;
				_st26:
				p+= 1;
				if ( p == pe )
					goto _test_eof26;
				st_case_26:
				if ( ( (*( p))) == 124 ) {
					goto _st4;
				}
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr67;
				}
				{
					goto _st0;
				}
				_ctr67:
				{
#line 19 "lex/parser_clone.rl"
					id = 10*id+((( (*( p))))-'0'); }
				
#line 735 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				{
#line 29 "lex/parser_clone.rl"
					set_args(res, TARGET_TYPE_EXCEPT, id, -1, 1,     inlet); }
				
#line 741 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				goto _st27;
				_st27:
				p+= 1;
				if ( p == pe )
					goto _test_eof27;
				st_case_27:
				if ( ( (*( p))) == 124 ) {
					goto _st4;
				}
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr69;
				}
				{
					goto _st0;
				}
				_ctr69:
				{
#line 19 "lex/parser_clone.rl"
					id = 10*id+((( (*( p))))-'0'); }
				
#line 763 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				{
#line 29 "lex/parser_clone.rl"
					set_args(res, TARGET_TYPE_EXCEPT, id, -1, 1,     inlet); }
				
#line 769 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				goto _st28;
				_st28:
				p+= 1;
				if ( p == pe )
					goto _test_eof28;
				st_case_28:
				if ( ( (*( p))) == 124 ) {
					goto _st4;
				}
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr71;
				}
				{
					goto _st0;
				}
				_ctr71:
				{
#line 19 "lex/parser_clone.rl"
					id = 10*id+((( (*( p))))-'0'); }
				
#line 791 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				{
#line 29 "lex/parser_clone.rl"
					set_args(res, TARGET_TYPE_EXCEPT, id, -1, 1,     inlet); }
				
#line 797 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				goto _st29;
				_st29:
				p+= 1;
				if ( p == pe )
					goto _test_eof29;
				st_case_29:
				if ( ( (*( p))) == 124 ) {
					goto _st4;
				}
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr73;
				}
				{
					goto _st0;
				}
				_ctr4:
				{
#line 27 "lex/parser_clone.rl"
					set_args(res, TARGET_TYPE_ALL,    -1, -1, 1,     inlet); }
				
#line 819 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				goto _st30;
				_st30:
				p+= 1;
				if ( p == pe )
					goto _test_eof30;
				st_case_30:
				if ( ( (*( p))) == 124 ) {
					goto _st5;
				}
				{
					goto _st0;
				}
				_st5:
				p+= 1;
				if ( p == pe )
					goto _test_eof5;
				st_case_5:
				if ( ( (*( p))) == 48 ) {
					goto _ctr17;
				}
				if ( 49 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr18;
				}
				{
					goto _st0;
				}
				_ctr18:
				{
#line 25 "lex/parser_clone.rl"
					inlet = (( (*( p))))-'0'; }
				
#line 852 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				{
#line 27 "lex/parser_clone.rl"
					set_args(res, TARGET_TYPE_ALL,    -1, -1, 1,     inlet); }
				
#line 858 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				goto _st31;
				_st31:
				p+= 1;
				if ( p == pe )
					goto _test_eof31;
				st_case_31:
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr76;
				}
				{
					goto _st0;
				}
				_ctr76:
				{
#line 25 "lex/parser_clone.rl"
					inlet = 10*inlet+((( (*( p))))-'0'); }
				
#line 877 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				{
#line 27 "lex/parser_clone.rl"
					set_args(res, TARGET_TYPE_ALL,    -1, -1, 1,     inlet); }
				
#line 883 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				goto _st32;
				_st32:
				p+= 1;
				if ( p == pe )
					goto _test_eof32;
				st_case_32:
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr78;
				}
				{
					goto _st0;
				}
				_ctr78:
				{
#line 25 "lex/parser_clone.rl"
					inlet = 10*inlet+((( (*( p))))-'0'); }
				
#line 902 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				{
#line 27 "lex/parser_clone.rl"
					set_args(res, TARGET_TYPE_ALL,    -1, -1, 1,     inlet); }
				
#line 908 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				goto _st33;
				_st33:
				p+= 1;
				if ( p == pe )
					goto _test_eof33;
				st_case_33:
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr80;
				}
				{
					goto _st0;
				}
				_ctr5:
				{
#line 18 "lex/parser_clone.rl"
					id = 0; }
				
#line 927 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				{
#line 30 "lex/parser_clone.rl"
					set_args(res, TARGET_TYPE_EQ,     id, -1, 1,     inlet); }
				
#line 933 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				{
#line 20 "lex/parser_clone.rl"
					id0 = id; }
				
#line 939 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				goto _st34;
				_ctr119:
				{
#line 19 "lex/parser_clone.rl"
					id = 10*id+((( (*( p))))-'0'); }
				
#line 947 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				{
#line 30 "lex/parser_clone.rl"
					set_args(res, TARGET_TYPE_EQ,     id, -1, 1,     inlet); }
				
#line 953 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				{
#line 20 "lex/parser_clone.rl"
					id0 = id; }
				
#line 959 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				goto _st34;
				_st34:
				p+= 1;
				if ( p == pe )
					goto _test_eof34;
				st_case_34:
				switch( ( (*( p))) ) {
					case 46: {
						goto _st6;
					}
					case 124: {
						goto _st10;
					}
				}
				{
					goto _st0;
				}
				_st6:
				p+= 1;
				if ( p == pe )
					goto _test_eof6;
				st_case_6:
				if ( ( (*( p))) == 46 ) {
					goto _st7;
				}
				{
					goto _st0;
				}
				_st7:
				p+= 1;
				if ( p == pe )
					goto _test_eof7;
				st_case_7:
				if ( ( (*( p))) == 48 ) {
					goto _ctr21;
				}
				if ( 49 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr22;
				}
				{
					goto _st0;
				}
				_ctr21:
				{
#line 18 "lex/parser_clone.rl"
					id = 0; }
				
#line 1008 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				{
#line 21 "lex/parser_clone.rl"
					id1 = id; }
				
#line 1014 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				{
#line 36 "lex/parser_clone.rl"
					set_args(res, TARGET_TYPE_RANGE, id0, id1, step, inlet); }
				
#line 1020 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				goto _st35;
				_ctr105:
				{
#line 19 "lex/parser_clone.rl"
					id = 10*id+((( (*( p))))-'0'); }
				
#line 1028 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				{
#line 21 "lex/parser_clone.rl"
					id1 = id; }
				
#line 1034 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				{
#line 36 "lex/parser_clone.rl"
					set_args(res, TARGET_TYPE_RANGE, id0, id1, step, inlet); }
				
#line 1040 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				goto _st35;
				_st35:
				p+= 1;
				if ( p == pe )
					goto _test_eof35;
				st_case_35:
				switch( ( (*( p))) ) {
					case 58: {
						goto _st8;
					}
					case 124: {
						goto _st9;
					}
				}
				{
					goto _st0;
				}
				_st8:
				p+= 1;
				if ( p == pe )
					goto _test_eof8;
				st_case_8:
				if ( 49 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr24;
				}
				{
					goto _st0;
				}
				_ctr24:
				{
#line 22 "lex/parser_clone.rl"
					step = (( (*( p))))-'0'; }
				
#line 1075 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				{
#line 36 "lex/parser_clone.rl"
					set_args(res, TARGET_TYPE_RANGE, id0, id1, step, inlet); }
				
#line 1081 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				goto _st36;
				_st36:
				p+= 1;
				if ( p == pe )
					goto _test_eof36;
				st_case_36:
				if ( ( (*( p))) == 124 ) {
					goto _st9;
				}
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr84;
				}
				{
					goto _st0;
				}
				_ctr84:
				{
#line 22 "lex/parser_clone.rl"
					step = 10*step+((( (*( p))))-'0'); }
				
#line 1103 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				{
#line 36 "lex/parser_clone.rl"
					set_args(res, TARGET_TYPE_RANGE, id0, id1, step, inlet); }
				
#line 1109 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				goto _st37;
				_st37:
				p+= 1;
				if ( p == pe )
					goto _test_eof37;
				st_case_37:
				if ( ( (*( p))) == 124 ) {
					goto _st9;
				}
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr86;
				}
				{
					goto _st0;
				}
				_ctr86:
				{
#line 22 "lex/parser_clone.rl"
					step = 10*step+((( (*( p))))-'0'); }
				
#line 1131 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				{
#line 36 "lex/parser_clone.rl"
					set_args(res, TARGET_TYPE_RANGE, id0, id1, step, inlet); }
				
#line 1137 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				goto _st38;
				_st38:
				p+= 1;
				if ( p == pe )
					goto _test_eof38;
				st_case_38:
				if ( ( (*( p))) == 124 ) {
					goto _st9;
				}
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr88;
				}
				{
					goto _st0;
				}
				_ctr88:
				{
#line 22 "lex/parser_clone.rl"
					step = 10*step+((( (*( p))))-'0'); }
				
#line 1159 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				{
#line 36 "lex/parser_clone.rl"
					set_args(res, TARGET_TYPE_RANGE, id0, id1, step, inlet); }
				
#line 1165 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				goto _st39;
				_st39:
				p+= 1;
				if ( p == pe )
					goto _test_eof39;
				st_case_39:
				if ( ( (*( p))) == 124 ) {
					goto _st9;
				}
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr90;
				}
				{
					goto _st0;
				}
				_ctr90:
				{
#line 22 "lex/parser_clone.rl"
					step = 10*step+((( (*( p))))-'0'); }
				
#line 1187 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				{
#line 36 "lex/parser_clone.rl"
					set_args(res, TARGET_TYPE_RANGE, id0, id1, step, inlet); }
				
#line 1193 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				goto _st40;
				_st40:
				p+= 1;
				if ( p == pe )
					goto _test_eof40;
				st_case_40:
				if ( ( (*( p))) == 124 ) {
					goto _st9;
				}
				{
					goto _st0;
				}
				_st9:
				p+= 1;
				if ( p == pe )
					goto _test_eof9;
				st_case_9:
				if ( ( (*( p))) == 48 ) {
					goto _ctr26;
				}
				if ( 49 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr27;
				}
				{
					goto _st0;
				}
				_ctr27:
				{
#line 25 "lex/parser_clone.rl"
					inlet = (( (*( p))))-'0'; }
				
#line 1226 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				{
#line 36 "lex/parser_clone.rl"
					set_args(res, TARGET_TYPE_RANGE, id0, id1, step, inlet); }
				
#line 1232 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				goto _st41;
				_st41:
				p+= 1;
				if ( p == pe )
					goto _test_eof41;
				st_case_41:
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr93;
				}
				{
					goto _st0;
				}
				_ctr93:
				{
#line 25 "lex/parser_clone.rl"
					inlet = 10*inlet+((( (*( p))))-'0'); }
				
#line 1251 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				{
#line 36 "lex/parser_clone.rl"
					set_args(res, TARGET_TYPE_RANGE, id0, id1, step, inlet); }
				
#line 1257 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				goto _st42;
				_st42:
				p+= 1;
				if ( p == pe )
					goto _test_eof42;
				st_case_42:
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr95;
				}
				{
					goto _st0;
				}
				_ctr95:
				{
#line 25 "lex/parser_clone.rl"
					inlet = 10*inlet+((( (*( p))))-'0'); }
				
#line 1276 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				{
#line 36 "lex/parser_clone.rl"
					set_args(res, TARGET_TYPE_RANGE, id0, id1, step, inlet); }
				
#line 1282 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				goto _st43;
				_st43:
				p+= 1;
				if ( p == pe )
					goto _test_eof43;
				st_case_43:
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr97;
				}
				{
					goto _st0;
				}
				_ctr22:
				{
#line 19 "lex/parser_clone.rl"
					id = (( (*( p))))-'0'; }
				
#line 1301 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				{
#line 21 "lex/parser_clone.rl"
					id1 = id; }
				
#line 1307 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				{
#line 36 "lex/parser_clone.rl"
					set_args(res, TARGET_TYPE_RANGE, id0, id1, step, inlet); }
				
#line 1313 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				goto _st44;
				_st44:
				p+= 1;
				if ( p == pe )
					goto _test_eof44;
				st_case_44:
				switch( ( (*( p))) ) {
					case 58: {
						goto _st8;
					}
					case 124: {
						goto _st9;
					}
				}
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr99;
				}
				{
					goto _st0;
				}
				_ctr99:
				{
#line 19 "lex/parser_clone.rl"
					id = 10*id+((( (*( p))))-'0'); }
				
#line 1340 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				{
#line 21 "lex/parser_clone.rl"
					id1 = id; }
				
#line 1346 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				{
#line 36 "lex/parser_clone.rl"
					set_args(res, TARGET_TYPE_RANGE, id0, id1, step, inlet); }
				
#line 1352 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				goto _st45;
				_st45:
				p+= 1;
				if ( p == pe )
					goto _test_eof45;
				st_case_45:
				switch( ( (*( p))) ) {
					case 58: {
						goto _st8;
					}
					case 124: {
						goto _st9;
					}
				}
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr101;
				}
				{
					goto _st0;
				}
				_ctr101:
				{
#line 19 "lex/parser_clone.rl"
					id = 10*id+((( (*( p))))-'0'); }
				
#line 1379 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				{
#line 21 "lex/parser_clone.rl"
					id1 = id; }
				
#line 1385 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				{
#line 36 "lex/parser_clone.rl"
					set_args(res, TARGET_TYPE_RANGE, id0, id1, step, inlet); }
				
#line 1391 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				goto _st46;
				_st46:
				p+= 1;
				if ( p == pe )
					goto _test_eof46;
				st_case_46:
				switch( ( (*( p))) ) {
					case 58: {
						goto _st8;
					}
					case 124: {
						goto _st9;
					}
				}
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr103;
				}
				{
					goto _st0;
				}
				_ctr103:
				{
#line 19 "lex/parser_clone.rl"
					id = 10*id+((( (*( p))))-'0'); }
				
#line 1418 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				{
#line 21 "lex/parser_clone.rl"
					id1 = id; }
				
#line 1424 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				{
#line 36 "lex/parser_clone.rl"
					set_args(res, TARGET_TYPE_RANGE, id0, id1, step, inlet); }
				
#line 1430 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				goto _st47;
				_st47:
				p+= 1;
				if ( p == pe )
					goto _test_eof47;
				st_case_47:
				switch( ( (*( p))) ) {
					case 58: {
						goto _st8;
					}
					case 124: {
						goto _st9;
					}
				}
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr105;
				}
				{
					goto _st0;
				}
				_st10:
				p+= 1;
				if ( p == pe )
					goto _test_eof10;
				st_case_10:
				if ( ( (*( p))) == 48 ) {
					goto _ctr29;
				}
				if ( 49 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr30;
				}
				{
					goto _st0;
				}
				_ctr30:
				{
#line 25 "lex/parser_clone.rl"
					inlet = (( (*( p))))-'0'; }
				
#line 1471 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				{
#line 30 "lex/parser_clone.rl"
					set_args(res, TARGET_TYPE_EQ,     id, -1, 1,     inlet); }
				
#line 1477 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				goto _st48;
				_st48:
				p+= 1;
				if ( p == pe )
					goto _test_eof48;
				st_case_48:
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr107;
				}
				{
					goto _st0;
				}
				_ctr107:
				{
#line 25 "lex/parser_clone.rl"
					inlet = 10*inlet+((( (*( p))))-'0'); }
				
#line 1496 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				{
#line 30 "lex/parser_clone.rl"
					set_args(res, TARGET_TYPE_EQ,     id, -1, 1,     inlet); }
				
#line 1502 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				goto _st49;
				_st49:
				p+= 1;
				if ( p == pe )
					goto _test_eof49;
				st_case_49:
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr109;
				}
				{
					goto _st0;
				}
				_ctr109:
				{
#line 25 "lex/parser_clone.rl"
					inlet = 10*inlet+((( (*( p))))-'0'); }
				
#line 1521 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				{
#line 30 "lex/parser_clone.rl"
					set_args(res, TARGET_TYPE_EQ,     id, -1, 1,     inlet); }
				
#line 1527 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				goto _st50;
				_st50:
				p+= 1;
				if ( p == pe )
					goto _test_eof50;
				st_case_50:
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr111;
				}
				{
					goto _st0;
				}
				_ctr6:
				{
#line 19 "lex/parser_clone.rl"
					id = (( (*( p))))-'0'; }
				
#line 1546 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				{
#line 30 "lex/parser_clone.rl"
					set_args(res, TARGET_TYPE_EQ,     id, -1, 1,     inlet); }
				
#line 1552 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				{
#line 20 "lex/parser_clone.rl"
					id0 = id; }
				
#line 1558 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				goto _st51;
				_st51:
				p+= 1;
				if ( p == pe )
					goto _test_eof51;
				st_case_51:
				switch( ( (*( p))) ) {
					case 46: {
						goto _st6;
					}
					case 124: {
						goto _st10;
					}
				}
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr113;
				}
				{
					goto _st0;
				}
				_ctr113:
				{
#line 19 "lex/parser_clone.rl"
					id = 10*id+((( (*( p))))-'0'); }
				
#line 1585 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				{
#line 30 "lex/parser_clone.rl"
					set_args(res, TARGET_TYPE_EQ,     id, -1, 1,     inlet); }
				
#line 1591 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				{
#line 20 "lex/parser_clone.rl"
					id0 = id; }
				
#line 1597 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				goto _st52;
				_st52:
				p+= 1;
				if ( p == pe )
					goto _test_eof52;
				st_case_52:
				switch( ( (*( p))) ) {
					case 46: {
						goto _st6;
					}
					case 124: {
						goto _st10;
					}
				}
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr115;
				}
				{
					goto _st0;
				}
				_ctr115:
				{
#line 19 "lex/parser_clone.rl"
					id = 10*id+((( (*( p))))-'0'); }
				
#line 1624 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				{
#line 30 "lex/parser_clone.rl"
					set_args(res, TARGET_TYPE_EQ,     id, -1, 1,     inlet); }
				
#line 1630 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				{
#line 20 "lex/parser_clone.rl"
					id0 = id; }
				
#line 1636 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				goto _st53;
				_st53:
				p+= 1;
				if ( p == pe )
					goto _test_eof53;
				st_case_53:
				switch( ( (*( p))) ) {
					case 46: {
						goto _st6;
					}
					case 124: {
						goto _st10;
					}
				}
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr117;
				}
				{
					goto _st0;
				}
				_ctr117:
				{
#line 19 "lex/parser_clone.rl"
					id = 10*id+((( (*( p))))-'0'); }
				
#line 1663 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				{
#line 30 "lex/parser_clone.rl"
					set_args(res, TARGET_TYPE_EQ,     id, -1, 1,     inlet); }
				
#line 1669 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				{
#line 20 "lex/parser_clone.rl"
					id0 = id; }
				
#line 1675 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				goto _st54;
				_st54:
				p+= 1;
				if ( p == pe )
					goto _test_eof54;
				st_case_54:
				switch( ( (*( p))) ) {
					case 46: {
						goto _st6;
					}
					case 124: {
						goto _st10;
					}
				}
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr119;
				}
				{
					goto _st0;
				}
				_st11:
				p+= 1;
				if ( p == pe )
					goto _test_eof11;
				st_case_11:
				switch( ( (*( p))) ) {
					case 48: {
						goto _ctr31;
					}
					case 61: {
						goto _st13;
					}
				}
				if ( 49 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr32;
				}
				{
					goto _st0;
				}
				_ctr31:
				{
#line 18 "lex/parser_clone.rl"
					id = 0; }
				
#line 1721 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				{
#line 33 "lex/parser_clone.rl"
					set_args(res, TARGET_TYPE_LT,     id, -1, 1,     inlet); }
				
#line 1727 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				goto _st55;
				_ctr134:
				{
#line 19 "lex/parser_clone.rl"
					id = 10*id+((( (*( p))))-'0'); }
				
#line 1735 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				{
#line 33 "lex/parser_clone.rl"
					set_args(res, TARGET_TYPE_LT,     id, -1, 1,     inlet); }
				
#line 1741 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				goto _st55;
				_st55:
				p+= 1;
				if ( p == pe )
					goto _test_eof55;
				st_case_55:
				if ( ( (*( p))) == 124 ) {
					goto _st12;
				}
				{
					goto _st0;
				}
				_st12:
				p+= 1;
				if ( p == pe )
					goto _test_eof12;
				st_case_12:
				if ( ( (*( p))) == 48 ) {
					goto _ctr35;
				}
				if ( 49 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr36;
				}
				{
					goto _st0;
				}
				_ctr36:
				{
#line 25 "lex/parser_clone.rl"
					inlet = (( (*( p))))-'0'; }
				
#line 1774 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				{
#line 33 "lex/parser_clone.rl"
					set_args(res, TARGET_TYPE_LT,     id, -1, 1,     inlet); }
				
#line 1780 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				goto _st56;
				_st56:
				p+= 1;
				if ( p == pe )
					goto _test_eof56;
				st_case_56:
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr122;
				}
				{
					goto _st0;
				}
				_ctr122:
				{
#line 25 "lex/parser_clone.rl"
					inlet = 10*inlet+((( (*( p))))-'0'); }
				
#line 1799 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				{
#line 33 "lex/parser_clone.rl"
					set_args(res, TARGET_TYPE_LT,     id, -1, 1,     inlet); }
				
#line 1805 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				goto _st57;
				_st57:
				p+= 1;
				if ( p == pe )
					goto _test_eof57;
				st_case_57:
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr124;
				}
				{
					goto _st0;
				}
				_ctr124:
				{
#line 25 "lex/parser_clone.rl"
					inlet = 10*inlet+((( (*( p))))-'0'); }
				
#line 1824 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				{
#line 33 "lex/parser_clone.rl"
					set_args(res, TARGET_TYPE_LT,     id, -1, 1,     inlet); }
				
#line 1830 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				goto _st58;
				_st58:
				p+= 1;
				if ( p == pe )
					goto _test_eof58;
				st_case_58:
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr126;
				}
				{
					goto _st0;
				}
				_ctr32:
				{
#line 19 "lex/parser_clone.rl"
					id = (( (*( p))))-'0'; }
				
#line 1849 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				{
#line 33 "lex/parser_clone.rl"
					set_args(res, TARGET_TYPE_LT,     id, -1, 1,     inlet); }
				
#line 1855 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				goto _st59;
				_st59:
				p+= 1;
				if ( p == pe )
					goto _test_eof59;
				st_case_59:
				if ( ( (*( p))) == 124 ) {
					goto _st12;
				}
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr128;
				}
				{
					goto _st0;
				}
				_ctr128:
				{
#line 19 "lex/parser_clone.rl"
					id = 10*id+((( (*( p))))-'0'); }
				
#line 1877 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				{
#line 33 "lex/parser_clone.rl"
					set_args(res, TARGET_TYPE_LT,     id, -1, 1,     inlet); }
				
#line 1883 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				goto _st60;
				_st60:
				p+= 1;
				if ( p == pe )
					goto _test_eof60;
				st_case_60:
				if ( ( (*( p))) == 124 ) {
					goto _st12;
				}
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr130;
				}
				{
					goto _st0;
				}
				_ctr130:
				{
#line 19 "lex/parser_clone.rl"
					id = 10*id+((( (*( p))))-'0'); }
				
#line 1905 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				{
#line 33 "lex/parser_clone.rl"
					set_args(res, TARGET_TYPE_LT,     id, -1, 1,     inlet); }
				
#line 1911 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				goto _st61;
				_st61:
				p+= 1;
				if ( p == pe )
					goto _test_eof61;
				st_case_61:
				if ( ( (*( p))) == 124 ) {
					goto _st12;
				}
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr132;
				}
				{
					goto _st0;
				}
				_ctr132:
				{
#line 19 "lex/parser_clone.rl"
					id = 10*id+((( (*( p))))-'0'); }
				
#line 1933 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				{
#line 33 "lex/parser_clone.rl"
					set_args(res, TARGET_TYPE_LT,     id, -1, 1,     inlet); }
				
#line 1939 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				goto _st62;
				_st62:
				p+= 1;
				if ( p == pe )
					goto _test_eof62;
				st_case_62:
				if ( ( (*( p))) == 124 ) {
					goto _st12;
				}
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr134;
				}
				{
					goto _st0;
				}
				_st13:
				p+= 1;
				if ( p == pe )
					goto _test_eof13;
				st_case_13:
				if ( ( (*( p))) == 48 ) {
					goto _ctr37;
				}
				if ( 49 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr38;
				}
				{
					goto _st0;
				}
				_ctr37:
				{
#line 18 "lex/parser_clone.rl"
					id = 0; }
				
#line 1975 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				{
#line 34 "lex/parser_clone.rl"
					set_args(res, TARGET_TYPE_LE,     id, -1, 1,     inlet); }
				
#line 1981 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				goto _st63;
				_ctr149:
				{
#line 19 "lex/parser_clone.rl"
					id = 10*id+((( (*( p))))-'0'); }
				
#line 1989 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				{
#line 34 "lex/parser_clone.rl"
					set_args(res, TARGET_TYPE_LE,     id, -1, 1,     inlet); }
				
#line 1995 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				goto _st63;
				_st63:
				p+= 1;
				if ( p == pe )
					goto _test_eof63;
				st_case_63:
				if ( ( (*( p))) == 124 ) {
					goto _st14;
				}
				{
					goto _st0;
				}
				_st14:
				p+= 1;
				if ( p == pe )
					goto _test_eof14;
				st_case_14:
				if ( ( (*( p))) == 48 ) {
					goto _ctr40;
				}
				if ( 49 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr41;
				}
				{
					goto _st0;
				}
				_ctr41:
				{
#line 25 "lex/parser_clone.rl"
					inlet = (( (*( p))))-'0'; }
				
#line 2028 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				{
#line 34 "lex/parser_clone.rl"
					set_args(res, TARGET_TYPE_LE,     id, -1, 1,     inlet); }
				
#line 2034 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				goto _st64;
				_st64:
				p+= 1;
				if ( p == pe )
					goto _test_eof64;
				st_case_64:
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr137;
				}
				{
					goto _st0;
				}
				_ctr137:
				{
#line 25 "lex/parser_clone.rl"
					inlet = 10*inlet+((( (*( p))))-'0'); }
				
#line 2053 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				{
#line 34 "lex/parser_clone.rl"
					set_args(res, TARGET_TYPE_LE,     id, -1, 1,     inlet); }
				
#line 2059 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				goto _st65;
				_st65:
				p+= 1;
				if ( p == pe )
					goto _test_eof65;
				st_case_65:
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr139;
				}
				{
					goto _st0;
				}
				_ctr139:
				{
#line 25 "lex/parser_clone.rl"
					inlet = 10*inlet+((( (*( p))))-'0'); }
				
#line 2078 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				{
#line 34 "lex/parser_clone.rl"
					set_args(res, TARGET_TYPE_LE,     id, -1, 1,     inlet); }
				
#line 2084 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				goto _st66;
				_st66:
				p+= 1;
				if ( p == pe )
					goto _test_eof66;
				st_case_66:
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr141;
				}
				{
					goto _st0;
				}
				_ctr38:
				{
#line 19 "lex/parser_clone.rl"
					id = (( (*( p))))-'0'; }
				
#line 2103 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				{
#line 34 "lex/parser_clone.rl"
					set_args(res, TARGET_TYPE_LE,     id, -1, 1,     inlet); }
				
#line 2109 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				goto _st67;
				_st67:
				p+= 1;
				if ( p == pe )
					goto _test_eof67;
				st_case_67:
				if ( ( (*( p))) == 124 ) {
					goto _st14;
				}
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr143;
				}
				{
					goto _st0;
				}
				_ctr143:
				{
#line 19 "lex/parser_clone.rl"
					id = 10*id+((( (*( p))))-'0'); }
				
#line 2131 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				{
#line 34 "lex/parser_clone.rl"
					set_args(res, TARGET_TYPE_LE,     id, -1, 1,     inlet); }
				
#line 2137 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				goto _st68;
				_st68:
				p+= 1;
				if ( p == pe )
					goto _test_eof68;
				st_case_68:
				if ( ( (*( p))) == 124 ) {
					goto _st14;
				}
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr145;
				}
				{
					goto _st0;
				}
				_ctr145:
				{
#line 19 "lex/parser_clone.rl"
					id = 10*id+((( (*( p))))-'0'); }
				
#line 2159 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				{
#line 34 "lex/parser_clone.rl"
					set_args(res, TARGET_TYPE_LE,     id, -1, 1,     inlet); }
				
#line 2165 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				goto _st69;
				_st69:
				p+= 1;
				if ( p == pe )
					goto _test_eof69;
				st_case_69:
				if ( ( (*( p))) == 124 ) {
					goto _st14;
				}
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr147;
				}
				{
					goto _st0;
				}
				_ctr147:
				{
#line 19 "lex/parser_clone.rl"
					id = 10*id+((( (*( p))))-'0'); }
				
#line 2187 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				{
#line 34 "lex/parser_clone.rl"
					set_args(res, TARGET_TYPE_LE,     id, -1, 1,     inlet); }
				
#line 2193 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				goto _st70;
				_st70:
				p+= 1;
				if ( p == pe )
					goto _test_eof70;
				st_case_70:
				if ( ( (*( p))) == 124 ) {
					goto _st14;
				}
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr149;
				}
				{
					goto _st0;
				}
				_st15:
				p+= 1;
				if ( p == pe )
					goto _test_eof15;
				st_case_15:
				if ( ( (*( p))) == 48 ) {
					goto _ctr42;
				}
				if ( 49 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr43;
				}
				{
					goto _st0;
				}
				_ctr42:
				{
#line 18 "lex/parser_clone.rl"
					id = 0; }
				
#line 2229 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				{
#line 30 "lex/parser_clone.rl"
					set_args(res, TARGET_TYPE_EQ,     id, -1, 1,     inlet); }
				
#line 2235 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				goto _st71;
				_ctr158:
				{
#line 19 "lex/parser_clone.rl"
					id = 10*id+((( (*( p))))-'0'); }
				
#line 2243 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				{
#line 30 "lex/parser_clone.rl"
					set_args(res, TARGET_TYPE_EQ,     id, -1, 1,     inlet); }
				
#line 2249 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				goto _st71;
				_st71:
				p+= 1;
				if ( p == pe )
					goto _test_eof71;
				st_case_71:
				if ( ( (*( p))) == 124 ) {
					goto _st10;
				}
				{
					goto _st0;
				}
				_ctr43:
				{
#line 19 "lex/parser_clone.rl"
					id = (( (*( p))))-'0'; }
				
#line 2268 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				{
#line 30 "lex/parser_clone.rl"
					set_args(res, TARGET_TYPE_EQ,     id, -1, 1,     inlet); }
				
#line 2274 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				goto _st72;
				_st72:
				p+= 1;
				if ( p == pe )
					goto _test_eof72;
				st_case_72:
				if ( ( (*( p))) == 124 ) {
					goto _st10;
				}
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr152;
				}
				{
					goto _st0;
				}
				_ctr152:
				{
#line 19 "lex/parser_clone.rl"
					id = 10*id+((( (*( p))))-'0'); }
				
#line 2296 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				{
#line 30 "lex/parser_clone.rl"
					set_args(res, TARGET_TYPE_EQ,     id, -1, 1,     inlet); }
				
#line 2302 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				goto _st73;
				_st73:
				p+= 1;
				if ( p == pe )
					goto _test_eof73;
				st_case_73:
				if ( ( (*( p))) == 124 ) {
					goto _st10;
				}
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr154;
				}
				{
					goto _st0;
				}
				_ctr154:
				{
#line 19 "lex/parser_clone.rl"
					id = 10*id+((( (*( p))))-'0'); }
				
#line 2324 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				{
#line 30 "lex/parser_clone.rl"
					set_args(res, TARGET_TYPE_EQ,     id, -1, 1,     inlet); }
				
#line 2330 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				goto _st74;
				_st74:
				p+= 1;
				if ( p == pe )
					goto _test_eof74;
				st_case_74:
				if ( ( (*( p))) == 124 ) {
					goto _st10;
				}
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr156;
				}
				{
					goto _st0;
				}
				_ctr156:
				{
#line 19 "lex/parser_clone.rl"
					id = 10*id+((( (*( p))))-'0'); }
				
#line 2352 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				{
#line 30 "lex/parser_clone.rl"
					set_args(res, TARGET_TYPE_EQ,     id, -1, 1,     inlet); }
				
#line 2358 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				goto _st75;
				_st75:
				p+= 1;
				if ( p == pe )
					goto _test_eof75;
				st_case_75:
				if ( ( (*( p))) == 124 ) {
					goto _st10;
				}
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr158;
				}
				{
					goto _st0;
				}
				_st16:
				p+= 1;
				if ( p == pe )
					goto _test_eof16;
				st_case_16:
				switch( ( (*( p))) ) {
					case 48: {
						goto _ctr44;
					}
					case 61: {
						goto _st18;
					}
				}
				if ( 49 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr45;
				}
				{
					goto _st0;
				}
				_ctr44:
				{
#line 18 "lex/parser_clone.rl"
					id = 0; }
				
#line 2399 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				{
#line 31 "lex/parser_clone.rl"
					set_args(res, TARGET_TYPE_GT,     id, -1, 1,     inlet); }
				
#line 2405 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				goto _st76;
				_ctr173:
				{
#line 19 "lex/parser_clone.rl"
					id = 10*id+((( (*( p))))-'0'); }
				
#line 2413 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				{
#line 31 "lex/parser_clone.rl"
					set_args(res, TARGET_TYPE_GT,     id, -1, 1,     inlet); }
				
#line 2419 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				goto _st76;
				_st76:
				p+= 1;
				if ( p == pe )
					goto _test_eof76;
				st_case_76:
				if ( ( (*( p))) == 124 ) {
					goto _st17;
				}
				{
					goto _st0;
				}
				_st17:
				p+= 1;
				if ( p == pe )
					goto _test_eof17;
				st_case_17:
				if ( ( (*( p))) == 48 ) {
					goto _ctr48;
				}
				if ( 49 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr49;
				}
				{
					goto _st0;
				}
				_ctr49:
				{
#line 25 "lex/parser_clone.rl"
					inlet = (( (*( p))))-'0'; }
				
#line 2452 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				{
#line 31 "lex/parser_clone.rl"
					set_args(res, TARGET_TYPE_GT,     id, -1, 1,     inlet); }
				
#line 2458 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				goto _st77;
				_st77:
				p+= 1;
				if ( p == pe )
					goto _test_eof77;
				st_case_77:
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr161;
				}
				{
					goto _st0;
				}
				_ctr161:
				{
#line 25 "lex/parser_clone.rl"
					inlet = 10*inlet+((( (*( p))))-'0'); }
				
#line 2477 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				{
#line 31 "lex/parser_clone.rl"
					set_args(res, TARGET_TYPE_GT,     id, -1, 1,     inlet); }
				
#line 2483 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				goto _st78;
				_st78:
				p+= 1;
				if ( p == pe )
					goto _test_eof78;
				st_case_78:
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr163;
				}
				{
					goto _st0;
				}
				_ctr163:
				{
#line 25 "lex/parser_clone.rl"
					inlet = 10*inlet+((( (*( p))))-'0'); }
				
#line 2502 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				{
#line 31 "lex/parser_clone.rl"
					set_args(res, TARGET_TYPE_GT,     id, -1, 1,     inlet); }
				
#line 2508 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				goto _st79;
				_st79:
				p+= 1;
				if ( p == pe )
					goto _test_eof79;
				st_case_79:
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr165;
				}
				{
					goto _st0;
				}
				_ctr45:
				{
#line 19 "lex/parser_clone.rl"
					id = (( (*( p))))-'0'; }
				
#line 2527 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				{
#line 31 "lex/parser_clone.rl"
					set_args(res, TARGET_TYPE_GT,     id, -1, 1,     inlet); }
				
#line 2533 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				goto _st80;
				_st80:
				p+= 1;
				if ( p == pe )
					goto _test_eof80;
				st_case_80:
				if ( ( (*( p))) == 124 ) {
					goto _st17;
				}
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr167;
				}
				{
					goto _st0;
				}
				_ctr167:
				{
#line 19 "lex/parser_clone.rl"
					id = 10*id+((( (*( p))))-'0'); }
				
#line 2555 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				{
#line 31 "lex/parser_clone.rl"
					set_args(res, TARGET_TYPE_GT,     id, -1, 1,     inlet); }
				
#line 2561 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				goto _st81;
				_st81:
				p+= 1;
				if ( p == pe )
					goto _test_eof81;
				st_case_81:
				if ( ( (*( p))) == 124 ) {
					goto _st17;
				}
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr169;
				}
				{
					goto _st0;
				}
				_ctr169:
				{
#line 19 "lex/parser_clone.rl"
					id = 10*id+((( (*( p))))-'0'); }
				
#line 2583 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				{
#line 31 "lex/parser_clone.rl"
					set_args(res, TARGET_TYPE_GT,     id, -1, 1,     inlet); }
				
#line 2589 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				goto _st82;
				_st82:
				p+= 1;
				if ( p == pe )
					goto _test_eof82;
				st_case_82:
				if ( ( (*( p))) == 124 ) {
					goto _st17;
				}
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr171;
				}
				{
					goto _st0;
				}
				_ctr171:
				{
#line 19 "lex/parser_clone.rl"
					id = 10*id+((( (*( p))))-'0'); }
				
#line 2611 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				{
#line 31 "lex/parser_clone.rl"
					set_args(res, TARGET_TYPE_GT,     id, -1, 1,     inlet); }
				
#line 2617 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				goto _st83;
				_st83:
				p+= 1;
				if ( p == pe )
					goto _test_eof83;
				st_case_83:
				if ( ( (*( p))) == 124 ) {
					goto _st17;
				}
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr173;
				}
				{
					goto _st0;
				}
				_st18:
				p+= 1;
				if ( p == pe )
					goto _test_eof18;
				st_case_18:
				if ( ( (*( p))) == 48 ) {
					goto _ctr50;
				}
				if ( 49 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr51;
				}
				{
					goto _st0;
				}
				_ctr50:
				{
#line 18 "lex/parser_clone.rl"
					id = 0; }
				
#line 2653 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				{
#line 32 "lex/parser_clone.rl"
					set_args(res, TARGET_TYPE_GE,     id, -1, 1,     inlet); }
				
#line 2659 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				goto _st84;
				_ctr188:
				{
#line 19 "lex/parser_clone.rl"
					id = 10*id+((( (*( p))))-'0'); }
				
#line 2667 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				{
#line 32 "lex/parser_clone.rl"
					set_args(res, TARGET_TYPE_GE,     id, -1, 1,     inlet); }
				
#line 2673 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				goto _st84;
				_st84:
				p+= 1;
				if ( p == pe )
					goto _test_eof84;
				st_case_84:
				if ( ( (*( p))) == 124 ) {
					goto _st19;
				}
				{
					goto _st0;
				}
				_st19:
				p+= 1;
				if ( p == pe )
					goto _test_eof19;
				st_case_19:
				if ( ( (*( p))) == 48 ) {
					goto _ctr53;
				}
				if ( 49 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr54;
				}
				{
					goto _st0;
				}
				_ctr54:
				{
#line 25 "lex/parser_clone.rl"
					inlet = (( (*( p))))-'0'; }
				
#line 2706 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				{
#line 32 "lex/parser_clone.rl"
					set_args(res, TARGET_TYPE_GE,     id, -1, 1,     inlet); }
				
#line 2712 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				goto _st85;
				_st85:
				p+= 1;
				if ( p == pe )
					goto _test_eof85;
				st_case_85:
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr176;
				}
				{
					goto _st0;
				}
				_ctr176:
				{
#line 25 "lex/parser_clone.rl"
					inlet = 10*inlet+((( (*( p))))-'0'); }
				
#line 2731 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				{
#line 32 "lex/parser_clone.rl"
					set_args(res, TARGET_TYPE_GE,     id, -1, 1,     inlet); }
				
#line 2737 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				goto _st86;
				_st86:
				p+= 1;
				if ( p == pe )
					goto _test_eof86;
				st_case_86:
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr178;
				}
				{
					goto _st0;
				}
				_ctr178:
				{
#line 25 "lex/parser_clone.rl"
					inlet = 10*inlet+((( (*( p))))-'0'); }
				
#line 2756 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				{
#line 32 "lex/parser_clone.rl"
					set_args(res, TARGET_TYPE_GE,     id, -1, 1,     inlet); }
				
#line 2762 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				goto _st87;
				_st87:
				p+= 1;
				if ( p == pe )
					goto _test_eof87;
				st_case_87:
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr180;
				}
				{
					goto _st0;
				}
				_ctr51:
				{
#line 19 "lex/parser_clone.rl"
					id = (( (*( p))))-'0'; }
				
#line 2781 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				{
#line 32 "lex/parser_clone.rl"
					set_args(res, TARGET_TYPE_GE,     id, -1, 1,     inlet); }
				
#line 2787 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				goto _st88;
				_st88:
				p+= 1;
				if ( p == pe )
					goto _test_eof88;
				st_case_88:
				if ( ( (*( p))) == 124 ) {
					goto _st19;
				}
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr182;
				}
				{
					goto _st0;
				}
				_ctr182:
				{
#line 19 "lex/parser_clone.rl"
					id = 10*id+((( (*( p))))-'0'); }
				
#line 2809 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				{
#line 32 "lex/parser_clone.rl"
					set_args(res, TARGET_TYPE_GE,     id, -1, 1,     inlet); }
				
#line 2815 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				goto _st89;
				_st89:
				p+= 1;
				if ( p == pe )
					goto _test_eof89;
				st_case_89:
				if ( ( (*( p))) == 124 ) {
					goto _st19;
				}
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr184;
				}
				{
					goto _st0;
				}
				_ctr184:
				{
#line 19 "lex/parser_clone.rl"
					id = 10*id+((( (*( p))))-'0'); }
				
#line 2837 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				{
#line 32 "lex/parser_clone.rl"
					set_args(res, TARGET_TYPE_GE,     id, -1, 1,     inlet); }
				
#line 2843 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				goto _st90;
				_st90:
				p+= 1;
				if ( p == pe )
					goto _test_eof90;
				st_case_90:
				if ( ( (*( p))) == 124 ) {
					goto _st19;
				}
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr186;
				}
				{
					goto _st0;
				}
				_ctr186:
				{
#line 19 "lex/parser_clone.rl"
					id = 10*id+((( (*( p))))-'0'); }
				
#line 2865 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				{
#line 32 "lex/parser_clone.rl"
					set_args(res, TARGET_TYPE_GE,     id, -1, 1,     inlet); }
				
#line 2871 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				goto _st91;
				_st91:
				p+= 1;
				if ( p == pe )
					goto _test_eof91;
				st_case_91:
				if ( ( (*( p))) == 124 ) {
					goto _st19;
				}
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr188;
				}
				{
					goto _st0;
				}
				_ctr10:
				{
#line 28 "lex/parser_clone.rl"
					set_args(res, TARGET_TYPE_RANDOM, -1, -1, 1,     inlet); }
				
#line 2893 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				goto _st92;
				_st92:
				p+= 1;
				if ( p == pe )
					goto _test_eof92;
				st_case_92:
				if ( ( (*( p))) == 124 ) {
					goto _st20;
				}
				{
					goto _st0;
				}
				_st20:
				p+= 1;
				if ( p == pe )
					goto _test_eof20;
				st_case_20:
				if ( ( (*( p))) == 48 ) {
					goto _ctr56;
				}
				if ( 49 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr57;
				}
				{
					goto _st0;
				}
				_ctr57:
				{
#line 25 "lex/parser_clone.rl"
					inlet = (( (*( p))))-'0'; }
				
#line 2926 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				{
#line 28 "lex/parser_clone.rl"
					set_args(res, TARGET_TYPE_RANDOM, -1, -1, 1,     inlet); }
				
#line 2932 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				goto _st93;
				_st93:
				p+= 1;
				if ( p == pe )
					goto _test_eof93;
				st_case_93:
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr191;
				}
				{
					goto _st0;
				}
				_ctr191:
				{
#line 25 "lex/parser_clone.rl"
					inlet = 10*inlet+((( (*( p))))-'0'); }
				
#line 2951 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				{
#line 28 "lex/parser_clone.rl"
					set_args(res, TARGET_TYPE_RANDOM, -1, -1, 1,     inlet); }
				
#line 2957 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				goto _st94;
				_st94:
				p+= 1;
				if ( p == pe )
					goto _test_eof94;
				st_case_94:
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr193;
				}
				{
					goto _st0;
				}
				_ctr193:
				{
#line 25 "lex/parser_clone.rl"
					inlet = 10*inlet+((( (*( p))))-'0'); }
				
#line 2976 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				{
#line 28 "lex/parser_clone.rl"
					set_args(res, TARGET_TYPE_RANDOM, -1, -1, 1,     inlet); }
				
#line 2982 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				goto _st95;
				_st95:
				p+= 1;
				if ( p == pe )
					goto _test_eof95;
				st_case_95:
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr195;
				}
				{
					goto _st0;
				}
				st_out:
				_test_eof1: cs = 1; goto _test_eof; 
				_test_eof2: cs = 2; goto _test_eof; 
				_test_eof3: cs = 3; goto _test_eof; 
				_test_eof21: cs = 21; goto _test_eof; 
				_test_eof4: cs = 4; goto _test_eof; 
				_test_eof22: cs = 22; goto _test_eof; 
				_test_eof23: cs = 23; goto _test_eof; 
				_test_eof24: cs = 24; goto _test_eof; 
				_test_eof25: cs = 25; goto _test_eof; 
				_test_eof26: cs = 26; goto _test_eof; 
				_test_eof27: cs = 27; goto _test_eof; 
				_test_eof28: cs = 28; goto _test_eof; 
				_test_eof29: cs = 29; goto _test_eof; 
				_test_eof30: cs = 30; goto _test_eof; 
				_test_eof5: cs = 5; goto _test_eof; 
				_test_eof31: cs = 31; goto _test_eof; 
				_test_eof32: cs = 32; goto _test_eof; 
				_test_eof33: cs = 33; goto _test_eof; 
				_test_eof34: cs = 34; goto _test_eof; 
				_test_eof6: cs = 6; goto _test_eof; 
				_test_eof7: cs = 7; goto _test_eof; 
				_test_eof35: cs = 35; goto _test_eof; 
				_test_eof8: cs = 8; goto _test_eof; 
				_test_eof36: cs = 36; goto _test_eof; 
				_test_eof37: cs = 37; goto _test_eof; 
				_test_eof38: cs = 38; goto _test_eof; 
				_test_eof39: cs = 39; goto _test_eof; 
				_test_eof40: cs = 40; goto _test_eof; 
				_test_eof9: cs = 9; goto _test_eof; 
				_test_eof41: cs = 41; goto _test_eof; 
				_test_eof42: cs = 42; goto _test_eof; 
				_test_eof43: cs = 43; goto _test_eof; 
				_test_eof44: cs = 44; goto _test_eof; 
				_test_eof45: cs = 45; goto _test_eof; 
				_test_eof46: cs = 46; goto _test_eof; 
				_test_eof47: cs = 47; goto _test_eof; 
				_test_eof10: cs = 10; goto _test_eof; 
				_test_eof48: cs = 48; goto _test_eof; 
				_test_eof49: cs = 49; goto _test_eof; 
				_test_eof50: cs = 50; goto _test_eof; 
				_test_eof51: cs = 51; goto _test_eof; 
				_test_eof52: cs = 52; goto _test_eof; 
				_test_eof53: cs = 53; goto _test_eof; 
				_test_eof54: cs = 54; goto _test_eof; 
				_test_eof11: cs = 11; goto _test_eof; 
				_test_eof55: cs = 55; goto _test_eof; 
				_test_eof12: cs = 12; goto _test_eof; 
				_test_eof56: cs = 56; goto _test_eof; 
				_test_eof57: cs = 57; goto _test_eof; 
				_test_eof58: cs = 58; goto _test_eof; 
				_test_eof59: cs = 59; goto _test_eof; 
				_test_eof60: cs = 60; goto _test_eof; 
				_test_eof61: cs = 61; goto _test_eof; 
				_test_eof62: cs = 62; goto _test_eof; 
				_test_eof13: cs = 13; goto _test_eof; 
				_test_eof63: cs = 63; goto _test_eof; 
				_test_eof14: cs = 14; goto _test_eof; 
				_test_eof64: cs = 64; goto _test_eof; 
				_test_eof65: cs = 65; goto _test_eof; 
				_test_eof66: cs = 66; goto _test_eof; 
				_test_eof67: cs = 67; goto _test_eof; 
				_test_eof68: cs = 68; goto _test_eof; 
				_test_eof69: cs = 69; goto _test_eof; 
				_test_eof70: cs = 70; goto _test_eof; 
				_test_eof15: cs = 15; goto _test_eof; 
				_test_eof71: cs = 71; goto _test_eof; 
				_test_eof72: cs = 72; goto _test_eof; 
				_test_eof73: cs = 73; goto _test_eof; 
				_test_eof74: cs = 74; goto _test_eof; 
				_test_eof75: cs = 75; goto _test_eof; 
				_test_eof16: cs = 16; goto _test_eof; 
				_test_eof76: cs = 76; goto _test_eof; 
				_test_eof17: cs = 17; goto _test_eof; 
				_test_eof77: cs = 77; goto _test_eof; 
				_test_eof78: cs = 78; goto _test_eof; 
				_test_eof79: cs = 79; goto _test_eof; 
				_test_eof80: cs = 80; goto _test_eof; 
				_test_eof81: cs = 81; goto _test_eof; 
				_test_eof82: cs = 82; goto _test_eof; 
				_test_eof83: cs = 83; goto _test_eof; 
				_test_eof18: cs = 18; goto _test_eof; 
				_test_eof84: cs = 84; goto _test_eof; 
				_test_eof19: cs = 19; goto _test_eof; 
				_test_eof85: cs = 85; goto _test_eof; 
				_test_eof86: cs = 86; goto _test_eof; 
				_test_eof87: cs = 87; goto _test_eof; 
				_test_eof88: cs = 88; goto _test_eof; 
				_test_eof89: cs = 89; goto _test_eof; 
				_test_eof90: cs = 90; goto _test_eof; 
				_test_eof91: cs = 91; goto _test_eof; 
				_test_eof92: cs = 92; goto _test_eof; 
				_test_eof20: cs = 20; goto _test_eof; 
				_test_eof93: cs = 93; goto _test_eof; 
				_test_eof94: cs = 94; goto _test_eof; 
				_test_eof95: cs = 95; goto _test_eof; 
				
				_test_eof: {}
				if ( cs >= 21 )
					goto _out; _pop: {}
				_out: {}
			}
			
#line 69 "lex/parser_clone.rl"
			
			
			return cs >= 
#line 3103 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
			21
#line 71 "lex/parser_clone.rl"
			;
		}
		
		const char *clone_message_to_string(CloneMessageType msg)
		{
			switch(msg) {
				case MSG_TYPE_DSP_SET:      return "dsp=";
				case MSG_TYPE_DSP_SPREAD:   return "dsp:";
				case MSG_TYPE_DSP_TOGGLE:   return "dsp~";
				case MSG_TYPE_SEND:         return "send";
				case MSG_TYPE_SEND_SPREAD:  return "send:";
				default: return "";
			}
		}
		
		
#line 3122 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
		static const int clone_message_type_start = 1;
		static const int clone_message_type_first_final = 8;
		static const int clone_message_type_error = 0;
		
		static const int clone_message_type_en_main = 1;
		
		
#line 98 "lex/parser_clone.rl"
		
		
		CloneMessageType parse_clone_message_type(const char* str)
		{
			const auto len = strlen(str);
			if (len == 0)
				return MSG_TYPE_NONE;
			
			int cs = 0;
			const char* p = str;
			const char* pe = p + len;
			const char* eof = pe;
			
			auto type = MSG_TYPE_NONE;
			
			
#line 3147 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
			{
				cs = (int)clone_message_type_start;
			}
			
#line 113 "lex/parser_clone.rl"
			
			
#line 3155 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
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
					case 8:
					goto st_case_8;
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
				p+= 1;
				if ( p == pe )
					goto _test_eof1;
				st_case_1:
				switch( ( (*( p))) ) {
					case 100: {
						goto _st2;
					}
					case 115: {
						goto _st5;
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
				p+= 1;
				if ( p == pe )
					goto _test_eof2;
				st_case_2:
				if ( ( (*( p))) == 115 ) {
					goto _st3;
				}
				{
					goto _st0;
				}
				_st3:
				p+= 1;
				if ( p == pe )
					goto _test_eof3;
				st_case_3:
				if ( ( (*( p))) == 112 ) {
					goto _st4;
				}
				{
					goto _st0;
				}
				_st4:
				p+= 1;
				if ( p == pe )
					goto _test_eof4;
				st_case_4:
				switch( ( (*( p))) ) {
					case 58: {
						goto _ctr6;
					}
					case 61: {
						goto _ctr7;
					}
					case 126: {
						goto _ctr8;
					}
				}
				{
					goto _st0;
				}
				_ctr6:
				{
#line 92 "lex/parser_clone.rl"
					type = MSG_TYPE_DSP_SPREAD; }
				
#line 3247 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				goto _st8;
				_ctr7:
				{
#line 90 "lex/parser_clone.rl"
					type = MSG_TYPE_DSP_SET; }
				
#line 3255 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				goto _st8;
				_ctr8:
				{
#line 91 "lex/parser_clone.rl"
					type = MSG_TYPE_DSP_TOGGLE; }
				
#line 3263 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				goto _st8;
				_ctr14:
				{
#line 94 "lex/parser_clone.rl"
					type = MSG_TYPE_SEND_SPREAD; }
				
#line 3271 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				goto _st8;
				_st8:
				p+= 1;
				if ( p == pe )
					goto _test_eof8;
				st_case_8:
				{
					goto _st0;
				}
				_st5:
				p+= 1;
				if ( p == pe )
					goto _test_eof5;
				st_case_5:
				if ( ( (*( p))) == 101 ) {
					goto _st6;
				}
				{
					goto _st0;
				}
				_st6:
				p+= 1;
				if ( p == pe )
					goto _test_eof6;
				st_case_6:
				if ( ( (*( p))) == 110 ) {
					goto _st7;
				}
				{
					goto _st0;
				}
				_st7:
				p+= 1;
				if ( p == pe )
					goto _test_eof7;
				st_case_7:
				if ( ( (*( p))) == 100 ) {
					goto _ctr11;
				}
				{
					goto _st0;
				}
				_ctr11:
				{
#line 93 "lex/parser_clone.rl"
					type = MSG_TYPE_SEND; }
				
#line 3320 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
				
				goto _st9;
				_st9:
				p+= 1;
				if ( p == pe )
					goto _test_eof9;
				st_case_9:
				if ( ( (*( p))) == 58 ) {
					goto _ctr14;
				}
				{
					goto _st0;
				}
				st_out:
				_test_eof1: cs = 1; goto _test_eof; 
				_test_eof2: cs = 2; goto _test_eof; 
				_test_eof3: cs = 3; goto _test_eof; 
				_test_eof4: cs = 4; goto _test_eof; 
				_test_eof8: cs = 8; goto _test_eof; 
				_test_eof5: cs = 5; goto _test_eof; 
				_test_eof6: cs = 6; goto _test_eof; 
				_test_eof7: cs = 7; goto _test_eof; 
				_test_eof9: cs = 9; goto _test_eof; 
				
				_test_eof: {}
				if ( cs >= 8 )
					goto _out; _pop: {}
				_out: {}
			}
			
#line 114 "lex/parser_clone.rl"
			
			
			return (cs < 
#line 3355 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_clone.cpp"
			8
#line 116 "lex/parser_clone.rl"
			) ? MSG_TYPE_NONE : type;
		}
		
	}
}
