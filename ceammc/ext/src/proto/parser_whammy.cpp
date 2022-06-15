#line 1 "parser_whammy.rl"
# include "parser_whammy_common.h"

# include <cstring>

namespace ceammc {
	namespace proto {
		
		
#line 11 "/Users/serge/work/music/pure-data/ceammc/ext/src/proto/parser_whammy.cpp"
		static const int whammy_ragel_start = 1;
		static const int whammy_ragel_first_final = 91;
		static const int whammy_ragel_error = 0;
		
		static const int whammy_ragel_en_main = 1;
		
		
#line 38 "parser_whammy.rl"
		
		
		bool nameToWhammyMode(const char* str, WhammyMode& mode) {
			const auto len = strlen(str);
			if (len == 0)
				return false;
			
			int cs = 0;
			const char* p = str;
			const char* pe = p + len;
			const char* eof = pe;
			WhammyMode mode_ = WHAMMY_MODE_UP_OCT;
			
			
#line 34 "/Users/serge/work/music/pure-data/ceammc/ext/src/proto/parser_whammy.cpp"
			{
				cs = (int)whammy_ragel_start;
			}
			
#line 51 "parser_whammy.rl"
			
			
#line 42 "/Users/serge/work/music/pure-data/ceammc/ext/src/proto/parser_whammy.cpp"
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
					case 5:
					goto st_case_5;
					case 91:
					goto st_case_91;
					case 6:
					goto st_case_6;
					case 7:
					goto st_case_7;
					case 92:
					goto st_case_92;
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
					case 14:
					goto st_case_14;
					case 93:
					goto st_case_93;
					case 15:
					goto st_case_15;
					case 16:
					goto st_case_16;
					case 94:
					goto st_case_94;
					case 17:
					goto st_case_17;
					case 18:
					goto st_case_18;
					case 95:
					goto st_case_95;
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
					case 96:
					goto st_case_96;
					case 31:
					goto st_case_31;
					case 97:
					goto st_case_97;
					case 32:
					goto st_case_32;
					case 33:
					goto st_case_33;
					case 34:
					goto st_case_34;
					case 35:
					goto st_case_35;
					case 36:
					goto st_case_36;
					case 98:
					goto st_case_98;
					case 99:
					goto st_case_99;
					case 37:
					goto st_case_37;
					case 100:
					goto st_case_100;
					case 38:
					goto st_case_38;
					case 39:
					goto st_case_39;
					case 40:
					goto st_case_40;
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
					case 101:
					goto st_case_101;
					case 47:
					goto st_case_47;
					case 48:
					goto st_case_48;
					case 102:
					goto st_case_102;
					case 49:
					goto st_case_49;
					case 50:
					goto st_case_50;
					case 103:
					goto st_case_103;
					case 51:
					goto st_case_51;
					case 52:
					goto st_case_52;
					case 104:
					goto st_case_104;
					case 53:
					goto st_case_53;
					case 105:
					goto st_case_105;
					case 54:
					goto st_case_54;
					case 55:
					goto st_case_55;
					case 56:
					goto st_case_56;
					case 57:
					goto st_case_57;
					case 58:
					goto st_case_58;
					case 106:
					goto st_case_106;
					case 59:
					goto st_case_59;
					case 107:
					goto st_case_107;
					case 60:
					goto st_case_60;
					case 61:
					goto st_case_61;
					case 62:
					goto st_case_62;
					case 63:
					goto st_case_63;
					case 64:
					goto st_case_64;
					case 108:
					goto st_case_108;
					case 65:
					goto st_case_65;
					case 66:
					goto st_case_66;
					case 109:
					goto st_case_109;
					case 67:
					goto st_case_67;
					case 68:
					goto st_case_68;
					case 69:
					goto st_case_69;
					case 70:
					goto st_case_70;
					case 71:
					goto st_case_71;
					case 72:
					goto st_case_72;
					case 110:
					goto st_case_110;
					case 73:
					goto st_case_73;
					case 74:
					goto st_case_74;
					case 111:
					goto st_case_111;
					case 75:
					goto st_case_75;
					case 76:
					goto st_case_76;
					case 77:
					goto st_case_77;
					case 112:
					goto st_case_112;
					case 78:
					goto st_case_78;
					case 79:
					goto st_case_79;
					case 80:
					goto st_case_80;
					case 113:
					goto st_case_113;
					case 81:
					goto st_case_81;
					case 82:
					goto st_case_82;
					case 83:
					goto st_case_83;
					case 84:
					goto st_case_84;
					case 114:
					goto st_case_114;
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
				}
				goto st_out;
				_st1:
				if ( p == eof ) {
					if ( cs >= 91 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof1;
				st_case_1:
				switch( ( (*( p))) ) {
					case 43: {
						goto _st2;
					}
					case 45: {
						goto _st43;
					}
					case 49: {
						goto _st3;
					}
					case 50: {
						goto _st70;
					}
					case 52: {
						goto _st71;
					}
					case 53: {
						goto _st73;
					}
					case 98: {
						goto _st75;
					}
					case 100: {
						goto _st78;
					}
					case 111: {
						goto _st4;
					}
					case 115: {
						goto _st85;
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
				if ( p == eof ) {
					if ( cs >= 91 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof2;
				st_case_2:
				switch( ( (*( p))) ) {
					case 49: {
						goto _st3;
					}
					case 50: {
						goto _st6;
					}
					case 51: {
						goto _st15;
					}
					case 52: {
						goto _st29;
					}
					case 53: {
						goto _st35;
					}
					case 111: {
						goto _st4;
					}
				}
				{
					goto _st0;
				}
				_st3:
				if ( p == eof ) {
					if ( cs >= 91 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof3;
				st_case_3:
				if ( ( (*( p))) == 111 ) {
					goto _st4;
				}
				{
					goto _st0;
				}
				_st4:
				if ( p == eof ) {
					if ( cs >= 91 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof4;
				st_case_4:
				if ( ( (*( p))) == 99 ) {
					goto _st5;
				}
				{
					goto _st0;
				}
				_st5:
				if ( p == eof ) {
					if ( cs >= 91 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof5;
				st_case_5:
				if ( ( (*( p))) == 116 ) {
					goto _st91;
				}
				{
					goto _st0;
				}
				_ctr115:
				{
#line 14 "parser_whammy.rl"
					mode_ = WHAMMY_MODE_UP_OCT; }
				
#line 419 "/Users/serge/work/music/pure-data/ceammc/ext/src/proto/parser_whammy.cpp"
				
				goto _st91;
				_st91:
				if ( p == eof ) {
					if ( cs >= 91 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof91;
				st_case_91:
				{
					goto _st0;
				}
				_st6:
				if ( p == eof ) {
					if ( cs >= 91 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof6;
				st_case_6:
				switch( ( (*( p))) ) {
					case 43: {
						goto _st7;
					}
					case 110: {
						goto _st8;
					}
					case 111: {
						goto _st13;
					}
				}
				{
					goto _st0;
				}
				_st7:
				if ( p == eof ) {
					if ( cs >= 91 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof7;
				st_case_7:
				if ( ( (*( p))) == 51 ) {
					goto _st92;
				}
				{
					goto _st0;
				}
				_ctr116:
				{
#line 25 "parser_whammy.rl"
					mode_ = WHAMMY_MODE_UP_2ND_UP_3RD; }
				
#line 483 "/Users/serge/work/music/pure-data/ceammc/ext/src/proto/parser_whammy.cpp"
				
				goto _st92;
				_st92:
				if ( p == eof ) {
					if ( cs >= 91 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof92;
				st_case_92:
				{
					goto _st0;
				}
				_st8:
				if ( p == eof ) {
					if ( cs >= 91 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof8;
				st_case_8:
				if ( ( (*( p))) == 100 ) {
					goto _st9;
				}
				{
					goto _st0;
				}
				_st9:
				if ( p == eof ) {
					if ( cs >= 91 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof9;
				st_case_9:
				if ( ( (*( p))) == 43 ) {
					goto _st10;
				}
				{
					goto _st0;
				}
				_st10:
				if ( p == eof ) {
					if ( cs >= 91 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof10;
				st_case_10:
				if ( ( (*( p))) == 51 ) {
					goto _st11;
				}
				{
					goto _st0;
				}
				_st11:
				if ( p == eof ) {
					if ( cs >= 91 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof11;
				st_case_11:
				if ( ( (*( p))) == 114 ) {
					goto _st12;
				}
				{
					goto _st0;
				}
				_st12:
				if ( p == eof ) {
					if ( cs >= 91 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof12;
				st_case_12:
				if ( ( (*( p))) == 100 ) {
					goto _st92;
				}
				{
					goto _st0;
				}
				_st13:
				if ( p == eof ) {
					if ( cs >= 91 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof13;
				st_case_13:
				if ( ( (*( p))) == 99 ) {
					goto _st14;
				}
				{
					goto _st0;
				}
				_st14:
				if ( p == eof ) {
					if ( cs >= 91 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof14;
				st_case_14:
				if ( ( (*( p))) == 116 ) {
					goto _st93;
				}
				{
					goto _st0;
				}
				_ctr117:
				{
#line 13 "parser_whammy.rl"
					mode_ = WHAMMY_MODE_UP_2OCT; }
				
#line 624 "/Users/serge/work/music/pure-data/ceammc/ext/src/proto/parser_whammy.cpp"
				
				goto _st93;
				_st93:
				if ( p == eof ) {
					if ( cs >= 91 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof93;
				st_case_93:
				{
					goto _st0;
				}
				_st15:
				if ( p == eof ) {
					if ( cs >= 91 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof15;
				st_case_15:
				switch( ( (*( p))) ) {
					case 43: {
						goto _st16;
					}
					case 98: {
						goto _st17;
					}
					case 114: {
						goto _st24;
					}
				}
				{
					goto _st0;
				}
				_st16:
				if ( p == eof ) {
					if ( cs >= 91 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof16;
				st_case_16:
				if ( ( (*( p))) == 52 ) {
					goto _st94;
				}
				{
					goto _st0;
				}
				_ctr118:
				{
#line 27 "parser_whammy.rl"
					mode_ = WHAMMY_MODE_UP_3RD_UP_4TH; }
				
#line 688 "/Users/serge/work/music/pure-data/ceammc/ext/src/proto/parser_whammy.cpp"
				
				goto _st94;
				_st94:
				if ( p == eof ) {
					if ( cs >= 91 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof94;
				st_case_94:
				{
					goto _st0;
				}
				_st17:
				if ( p == eof ) {
					if ( cs >= 91 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof17;
				st_case_17:
				switch( ( (*( p))) ) {
					case 43: {
						goto _st18;
					}
					case 114: {
						goto _st19;
					}
				}
				{
					goto _st0;
				}
				_st18:
				if ( p == eof ) {
					if ( cs >= 91 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof18;
				st_case_18:
				if ( ( (*( p))) == 51 ) {
					goto _st95;
				}
				{
					goto _st0;
				}
				_ctr119:
				{
#line 26 "parser_whammy.rl"
					mode_ = WHAMMY_MODE_UP_3RD_UP_3RD; }
				
#line 749 "/Users/serge/work/music/pure-data/ceammc/ext/src/proto/parser_whammy.cpp"
				
				goto _st95;
				_st95:
				if ( p == eof ) {
					if ( cs >= 91 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof95;
				st_case_95:
				{
					goto _st0;
				}
				_st19:
				if ( p == eof ) {
					if ( cs >= 91 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof19;
				st_case_19:
				if ( ( (*( p))) == 100 ) {
					goto _st20;
				}
				{
					goto _st0;
				}
				_st20:
				if ( p == eof ) {
					if ( cs >= 91 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof20;
				st_case_20:
				if ( ( (*( p))) == 43 ) {
					goto _st21;
				}
				{
					goto _st0;
				}
				_st21:
				if ( p == eof ) {
					if ( cs >= 91 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof21;
				st_case_21:
				if ( ( (*( p))) == 51 ) {
					goto _st22;
				}
				{
					goto _st0;
				}
				_st22:
				if ( p == eof ) {
					if ( cs >= 91 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof22;
				st_case_22:
				if ( ( (*( p))) == 114 ) {
					goto _st23;
				}
				{
					goto _st0;
				}
				_st23:
				if ( p == eof ) {
					if ( cs >= 91 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof23;
				st_case_23:
				if ( ( (*( p))) == 100 ) {
					goto _st95;
				}
				{
					goto _st0;
				}
				_st24:
				if ( p == eof ) {
					if ( cs >= 91 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof24;
				st_case_24:
				if ( ( (*( p))) == 100 ) {
					goto _st25;
				}
				{
					goto _st0;
				}
				_st25:
				if ( p == eof ) {
					if ( cs >= 91 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof25;
				st_case_25:
				if ( ( (*( p))) == 43 ) {
					goto _st26;
				}
				{
					goto _st0;
				}
				_st26:
				if ( p == eof ) {
					if ( cs >= 91 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof26;
				st_case_26:
				if ( ( (*( p))) == 52 ) {
					goto _st27;
				}
				{
					goto _st0;
				}
				_st27:
				if ( p == eof ) {
					if ( cs >= 91 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof27;
				st_case_27:
				if ( ( (*( p))) == 116 ) {
					goto _st28;
				}
				{
					goto _st0;
				}
				_st28:
				if ( p == eof ) {
					if ( cs >= 91 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof28;
				st_case_28:
				if ( ( (*( p))) == 104 ) {
					goto _st94;
				}
				{
					goto _st0;
				}
				_st29:
				if ( p == eof ) {
					if ( cs >= 91 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof29;
				st_case_29:
				switch( ( (*( p))) ) {
					case 43: {
						goto _st30;
					}
					case 116: {
						goto _st31;
					}
				}
				{
					goto _st0;
				}
				_st30:
				if ( p == eof ) {
					if ( cs >= 91 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof30;
				st_case_30:
				if ( ( (*( p))) == 53 ) {
					goto _st96;
				}
				{
					goto _st0;
				}
				_ctr120:
				{
#line 28 "parser_whammy.rl"
					mode_ = WHAMMY_MODE_UP_4TH_UP_5TH; }
				
#line 980 "/Users/serge/work/music/pure-data/ceammc/ext/src/proto/parser_whammy.cpp"
				
				goto _st96;
				_st96:
				if ( p == eof ) {
					if ( cs >= 91 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof96;
				st_case_96:
				{
					goto _st0;
				}
				_st31:
				if ( p == eof ) {
					if ( cs >= 91 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof31;
				st_case_31:
				if ( ( (*( p))) == 104 ) {
					goto _st97;
				}
				{
					goto _st0;
				}
				_ctr121:
				{
#line 16 "parser_whammy.rl"
					mode_ = WHAMMY_MODE_UP_4TH; }
				
#line 1019 "/Users/serge/work/music/pure-data/ceammc/ext/src/proto/parser_whammy.cpp"
				
				goto _st97;
				_st97:
				if ( p == eof ) {
					if ( cs >= 91 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof97;
				st_case_97:
				if ( ( (*( p))) == 43 ) {
					goto _st32;
				}
				{
					goto _st0;
				}
				_st32:
				if ( p == eof ) {
					if ( cs >= 91 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof32;
				st_case_32:
				if ( ( (*( p))) == 53 ) {
					goto _st33;
				}
				{
					goto _st0;
				}
				_st33:
				if ( p == eof ) {
					if ( cs >= 91 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof33;
				st_case_33:
				if ( ( (*( p))) == 116 ) {
					goto _st34;
				}
				{
					goto _st0;
				}
				_st34:
				if ( p == eof ) {
					if ( cs >= 91 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof34;
				st_case_34:
				if ( ( (*( p))) == 104 ) {
					goto _st96;
				}
				{
					goto _st0;
				}
				_st35:
				if ( p == eof ) {
					if ( cs >= 91 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof35;
				st_case_35:
				switch( ( (*( p))) ) {
					case 43: {
						goto _st36;
					}
					case 116: {
						goto _st37;
					}
				}
				{
					goto _st0;
				}
				_st36:
				if ( p == eof ) {
					if ( cs >= 91 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof36;
				st_case_36:
				switch( ( (*( p))) ) {
					case 54: {
						goto _st98;
					}
					case 55: {
						goto _st99;
					}
				}
				{
					goto _st0;
				}
				_ctr122:
				{
#line 29 "parser_whammy.rl"
					mode_ = WHAMMY_MODE_UP_5TH_UP_6TH; }
				
#line 1139 "/Users/serge/work/music/pure-data/ceammc/ext/src/proto/parser_whammy.cpp"
				
				goto _st98;
				_st98:
				if ( p == eof ) {
					if ( cs >= 91 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof98;
				st_case_98:
				{
					goto _st0;
				}
				_ctr123:
				{
#line 30 "parser_whammy.rl"
					mode_ = WHAMMY_MODE_UP_5TH_UP_7TH; }
				
#line 1161 "/Users/serge/work/music/pure-data/ceammc/ext/src/proto/parser_whammy.cpp"
				
				goto _st99;
				_st99:
				if ( p == eof ) {
					if ( cs >= 91 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof99;
				st_case_99:
				{
					goto _st0;
				}
				_st37:
				if ( p == eof ) {
					if ( cs >= 91 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof37;
				st_case_37:
				if ( ( (*( p))) == 104 ) {
					goto _st100;
				}
				{
					goto _st0;
				}
				_ctr124:
				{
#line 15 "parser_whammy.rl"
					mode_ = WHAMMY_MODE_UP_5TH; }
				
#line 1200 "/Users/serge/work/music/pure-data/ceammc/ext/src/proto/parser_whammy.cpp"
				
				goto _st100;
				_st100:
				if ( p == eof ) {
					if ( cs >= 91 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof100;
				st_case_100:
				if ( ( (*( p))) == 43 ) {
					goto _st38;
				}
				{
					goto _st0;
				}
				_st38:
				if ( p == eof ) {
					if ( cs >= 91 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof38;
				st_case_38:
				switch( ( (*( p))) ) {
					case 54: {
						goto _st39;
					}
					case 55: {
						goto _st41;
					}
				}
				{
					goto _st0;
				}
				_st39:
				if ( p == eof ) {
					if ( cs >= 91 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof39;
				st_case_39:
				if ( ( (*( p))) == 116 ) {
					goto _st40;
				}
				{
					goto _st0;
				}
				_st40:
				if ( p == eof ) {
					if ( cs >= 91 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof40;
				st_case_40:
				if ( ( (*( p))) == 104 ) {
					goto _st98;
				}
				{
					goto _st0;
				}
				_st41:
				if ( p == eof ) {
					if ( cs >= 91 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof41;
				st_case_41:
				if ( ( (*( p))) == 116 ) {
					goto _st42;
				}
				{
					goto _st0;
				}
				_st42:
				if ( p == eof ) {
					if ( cs >= 91 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof42;
				st_case_42:
				if ( ( (*( p))) == 104 ) {
					goto _st99;
				}
				{
					goto _st0;
				}
				_st43:
				if ( p == eof ) {
					if ( cs >= 91 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof43;
				st_case_43:
				switch( ( (*( p))) ) {
					case 49: {
						goto _st44;
					}
					case 50: {
						goto _st47;
					}
					case 52: {
						goto _st51;
					}
					case 53: {
						goto _st57;
					}
					case 56: {
						goto _st63;
					}
					case 111: {
						goto _st65;
					}
				}
				{
					goto _st0;
				}
				_st44:
				if ( p == eof ) {
					if ( cs >= 91 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof44;
				st_case_44:
				if ( ( (*( p))) == 111 ) {
					goto _st45;
				}
				{
					goto _st0;
				}
				_st45:
				if ( p == eof ) {
					if ( cs >= 91 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof45;
				st_case_45:
				if ( ( (*( p))) == 99 ) {
					goto _st46;
				}
				{
					goto _st0;
				}
				_st46:
				if ( p == eof ) {
					if ( cs >= 91 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof46;
				st_case_46:
				if ( ( (*( p))) == 116 ) {
					goto _st101;
				}
				{
					goto _st0;
				}
				_ctr125:
				{
#line 20 "parser_whammy.rl"
					mode_ = WHAMMY_MODE_DOWN_OCT; }
				
#line 1400 "/Users/serge/work/music/pure-data/ceammc/ext/src/proto/parser_whammy.cpp"
				
				goto _st101;
				_st101:
				if ( p == eof ) {
					if ( cs >= 91 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof101;
				st_case_101:
				{
					goto _st0;
				}
				_st47:
				if ( p == eof ) {
					if ( cs >= 91 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof47;
				st_case_47:
				switch( ( (*( p))) ) {
					case 110: {
						goto _st48;
					}
					case 111: {
						goto _st49;
					}
				}
				{
					goto _st0;
				}
				_st48:
				if ( p == eof ) {
					if ( cs >= 91 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof48;
				st_case_48:
				if ( ( (*( p))) == 100 ) {
					goto _st102;
				}
				{
					goto _st0;
				}
				_ctr126:
				{
#line 17 "parser_whammy.rl"
					mode_ = WHAMMY_MODE_DOWN_2ND; }
				
#line 1461 "/Users/serge/work/music/pure-data/ceammc/ext/src/proto/parser_whammy.cpp"
				
				goto _st102;
				_st102:
				if ( p == eof ) {
					if ( cs >= 91 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof102;
				st_case_102:
				{
					goto _st0;
				}
				_st49:
				if ( p == eof ) {
					if ( cs >= 91 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof49;
				st_case_49:
				if ( ( (*( p))) == 99 ) {
					goto _st50;
				}
				{
					goto _st0;
				}
				_st50:
				if ( p == eof ) {
					if ( cs >= 91 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof50;
				st_case_50:
				if ( ( (*( p))) == 116 ) {
					goto _st103;
				}
				{
					goto _st0;
				}
				_ctr127:
				{
#line 21 "parser_whammy.rl"
					mode_ = WHAMMY_MODE_DOWN_2OCT; }
				
#line 1517 "/Users/serge/work/music/pure-data/ceammc/ext/src/proto/parser_whammy.cpp"
				
				goto _st103;
				_st103:
				if ( p == eof ) {
					if ( cs >= 91 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof103;
				st_case_103:
				{
					goto _st0;
				}
				_st51:
				if ( p == eof ) {
					if ( cs >= 91 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof51;
				st_case_51:
				switch( ( (*( p))) ) {
					case 45: {
						goto _st52;
					}
					case 116: {
						goto _st53;
					}
				}
				{
					goto _st0;
				}
				_st52:
				if ( p == eof ) {
					if ( cs >= 91 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof52;
				st_case_52:
				if ( ( (*( p))) == 51 ) {
					goto _st104;
				}
				{
					goto _st0;
				}
				_ctr128:
				{
#line 31 "parser_whammy.rl"
					mode_ = WHAMMY_MODE_DOWN_4TH_DOWN_3TH; }
				
#line 1578 "/Users/serge/work/music/pure-data/ceammc/ext/src/proto/parser_whammy.cpp"
				
				goto _st104;
				_st104:
				if ( p == eof ) {
					if ( cs >= 91 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof104;
				st_case_104:
				{
					goto _st0;
				}
				_st53:
				if ( p == eof ) {
					if ( cs >= 91 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof53;
				st_case_53:
				if ( ( (*( p))) == 104 ) {
					goto _st105;
				}
				{
					goto _st0;
				}
				_ctr129:
				{
#line 18 "parser_whammy.rl"
					mode_ = WHAMMY_MODE_DOWN_4TH; }
				
#line 1617 "/Users/serge/work/music/pure-data/ceammc/ext/src/proto/parser_whammy.cpp"
				
				goto _st105;
				_st105:
				if ( p == eof ) {
					if ( cs >= 91 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof105;
				st_case_105:
				if ( ( (*( p))) == 45 ) {
					goto _st54;
				}
				{
					goto _st0;
				}
				_st54:
				if ( p == eof ) {
					if ( cs >= 91 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof54;
				st_case_54:
				if ( ( (*( p))) == 51 ) {
					goto _st55;
				}
				{
					goto _st0;
				}
				_st55:
				if ( p == eof ) {
					if ( cs >= 91 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof55;
				st_case_55:
				if ( ( (*( p))) == 114 ) {
					goto _st56;
				}
				{
					goto _st0;
				}
				_st56:
				if ( p == eof ) {
					if ( cs >= 91 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof56;
				st_case_56:
				if ( ( (*( p))) == 100 ) {
					goto _st104;
				}
				{
					goto _st0;
				}
				_st57:
				if ( p == eof ) {
					if ( cs >= 91 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof57;
				st_case_57:
				switch( ( (*( p))) ) {
					case 45: {
						goto _st58;
					}
					case 116: {
						goto _st59;
					}
				}
				{
					goto _st0;
				}
				_st58:
				if ( p == eof ) {
					if ( cs >= 91 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof58;
				st_case_58:
				if ( ( (*( p))) == 52 ) {
					goto _st106;
				}
				{
					goto _st0;
				}
				_ctr130:
				{
#line 32 "parser_whammy.rl"
					mode_ = WHAMMY_MODE_DOWN_5TH_DOWN_4TH; }
				
#line 1732 "/Users/serge/work/music/pure-data/ceammc/ext/src/proto/parser_whammy.cpp"
				
				goto _st106;
				_st106:
				if ( p == eof ) {
					if ( cs >= 91 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof106;
				st_case_106:
				{
					goto _st0;
				}
				_st59:
				if ( p == eof ) {
					if ( cs >= 91 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof59;
				st_case_59:
				if ( ( (*( p))) == 104 ) {
					goto _st107;
				}
				{
					goto _st0;
				}
				_ctr131:
				{
#line 19 "parser_whammy.rl"
					mode_ = WHAMMY_MODE_DOWN_5TH; }
				
#line 1771 "/Users/serge/work/music/pure-data/ceammc/ext/src/proto/parser_whammy.cpp"
				
				goto _st107;
				_st107:
				if ( p == eof ) {
					if ( cs >= 91 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof107;
				st_case_107:
				if ( ( (*( p))) == 45 ) {
					goto _st60;
				}
				{
					goto _st0;
				}
				_st60:
				if ( p == eof ) {
					if ( cs >= 91 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof60;
				st_case_60:
				if ( ( (*( p))) == 52 ) {
					goto _st61;
				}
				{
					goto _st0;
				}
				_st61:
				if ( p == eof ) {
					if ( cs >= 91 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof61;
				st_case_61:
				if ( ( (*( p))) == 116 ) {
					goto _st62;
				}
				{
					goto _st0;
				}
				_st62:
				if ( p == eof ) {
					if ( cs >= 91 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof62;
				st_case_62:
				if ( ( (*( p))) == 104 ) {
					goto _st106;
				}
				{
					goto _st0;
				}
				_st63:
				if ( p == eof ) {
					if ( cs >= 91 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof63;
				st_case_63:
				if ( ( (*( p))) == 43 ) {
					goto _st64;
				}
				{
					goto _st0;
				}
				_st64:
				if ( p == eof ) {
					if ( cs >= 91 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof64;
				st_case_64:
				if ( ( (*( p))) == 56 ) {
					goto _st108;
				}
				{
					goto _st0;
				}
				_ctr132:
				{
#line 33 "parser_whammy.rl"
					mode_ = WHAMMY_MODE_DOWN_OCT_UP_OCT; }
				
#line 1881 "/Users/serge/work/music/pure-data/ceammc/ext/src/proto/parser_whammy.cpp"
				
				goto _st108;
				_st108:
				if ( p == eof ) {
					if ( cs >= 91 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof108;
				st_case_108:
				{
					goto _st0;
				}
				_st65:
				if ( p == eof ) {
					if ( cs >= 91 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof65;
				st_case_65:
				if ( ( (*( p))) == 99 ) {
					goto _st66;
				}
				{
					goto _st0;
				}
				_st66:
				if ( p == eof ) {
					if ( cs >= 91 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof66;
				st_case_66:
				if ( ( (*( p))) == 116 ) {
					goto _st109;
				}
				{
					goto _st0;
				}
				_ctr133:
				{
#line 20 "parser_whammy.rl"
					mode_ = WHAMMY_MODE_DOWN_OCT; }
				
#line 1937 "/Users/serge/work/music/pure-data/ceammc/ext/src/proto/parser_whammy.cpp"
				
				goto _st109;
				_st109:
				if ( p == eof ) {
					if ( cs >= 91 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof109;
				st_case_109:
				if ( ( (*( p))) == 43 ) {
					goto _st67;
				}
				{
					goto _st0;
				}
				_st67:
				if ( p == eof ) {
					if ( cs >= 91 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof67;
				st_case_67:
				if ( ( (*( p))) == 111 ) {
					goto _st68;
				}
				{
					goto _st0;
				}
				_st68:
				if ( p == eof ) {
					if ( cs >= 91 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof68;
				st_case_68:
				if ( ( (*( p))) == 99 ) {
					goto _st69;
				}
				{
					goto _st0;
				}
				_st69:
				if ( p == eof ) {
					if ( cs >= 91 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof69;
				st_case_69:
				if ( ( (*( p))) == 116 ) {
					goto _st108;
				}
				{
					goto _st0;
				}
				_st70:
				if ( p == eof ) {
					if ( cs >= 91 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof70;
				st_case_70:
				if ( ( (*( p))) == 111 ) {
					goto _st13;
				}
				{
					goto _st0;
				}
				_st71:
				if ( p == eof ) {
					if ( cs >= 91 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof71;
				st_case_71:
				if ( ( (*( p))) == 116 ) {
					goto _st72;
				}
				{
					goto _st0;
				}
				_st72:
				if ( p == eof ) {
					if ( cs >= 91 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof72;
				st_case_72:
				if ( ( (*( p))) == 104 ) {
					goto _st110;
				}
				{
					goto _st0;
				}
				_ctr134:
				{
#line 16 "parser_whammy.rl"
					mode_ = WHAMMY_MODE_UP_4TH; }
				
#line 2064 "/Users/serge/work/music/pure-data/ceammc/ext/src/proto/parser_whammy.cpp"
				
				goto _st110;
				_st110:
				if ( p == eof ) {
					if ( cs >= 91 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof110;
				st_case_110:
				{
					goto _st0;
				}
				_st73:
				if ( p == eof ) {
					if ( cs >= 91 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof73;
				st_case_73:
				if ( ( (*( p))) == 116 ) {
					goto _st74;
				}
				{
					goto _st0;
				}
				_st74:
				if ( p == eof ) {
					if ( cs >= 91 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof74;
				st_case_74:
				if ( ( (*( p))) == 104 ) {
					goto _st111;
				}
				{
					goto _st0;
				}
				_ctr135:
				{
#line 15 "parser_whammy.rl"
					mode_ = WHAMMY_MODE_UP_5TH; }
				
#line 2120 "/Users/serge/work/music/pure-data/ceammc/ext/src/proto/parser_whammy.cpp"
				
				goto _st111;
				_st111:
				if ( p == eof ) {
					if ( cs >= 91 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof111;
				st_case_111:
				{
					goto _st0;
				}
				_st75:
				if ( p == eof ) {
					if ( cs >= 91 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof75;
				st_case_75:
				if ( ( (*( p))) == 111 ) {
					goto _st76;
				}
				{
					goto _st0;
				}
				_st76:
				if ( p == eof ) {
					if ( cs >= 91 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof76;
				st_case_76:
				if ( ( (*( p))) == 109 ) {
					goto _st77;
				}
				{
					goto _st0;
				}
				_st77:
				if ( p == eof ) {
					if ( cs >= 91 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof77;
				st_case_77:
				if ( ( (*( p))) == 98 ) {
					goto _st112;
				}
				{
					goto _st0;
				}
				_ctr136:
				{
#line 22 "parser_whammy.rl"
					mode_ = WHAMMY_MODE_DIVE_BOMB; }
				
#line 2193 "/Users/serge/work/music/pure-data/ceammc/ext/src/proto/parser_whammy.cpp"
				
				goto _st112;
				_st112:
				if ( p == eof ) {
					if ( cs >= 91 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof112;
				st_case_112:
				{
					goto _st0;
				}
				_st78:
				if ( p == eof ) {
					if ( cs >= 91 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof78;
				st_case_78:
				if ( ( (*( p))) == 101 ) {
					goto _st79;
				}
				{
					goto _st0;
				}
				_st79:
				if ( p == eof ) {
					if ( cs >= 91 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof79;
				st_case_79:
				switch( ( (*( p))) ) {
					case 101: {
						goto _st80;
					}
					case 116: {
						goto _st81;
					}
				}
				{
					goto _st0;
				}
				_st80:
				if ( p == eof ) {
					if ( cs >= 91 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof80;
				st_case_80:
				if ( ( (*( p))) == 112 ) {
					goto _st113;
				}
				{
					goto _st0;
				}
				_ctr137:
				{
#line 23 "parser_whammy.rl"
					mode_ = WHAMMY_MODE_DETUNE_DEEP; }
				
#line 2271 "/Users/serge/work/music/pure-data/ceammc/ext/src/proto/parser_whammy.cpp"
				
				goto _st113;
				_st113:
				if ( p == eof ) {
					if ( cs >= 91 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof113;
				st_case_113:
				{
					goto _st0;
				}
				_st81:
				if ( p == eof ) {
					if ( cs >= 91 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof81;
				st_case_81:
				if ( ( (*( p))) == 117 ) {
					goto _st82;
				}
				{
					goto _st0;
				}
				_st82:
				if ( p == eof ) {
					if ( cs >= 91 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof82;
				st_case_82:
				if ( ( (*( p))) == 110 ) {
					goto _st83;
				}
				{
					goto _st0;
				}
				_st83:
				if ( p == eof ) {
					if ( cs >= 91 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof83;
				st_case_83:
				if ( ( (*( p))) == 101 ) {
					goto _st84;
				}
				{
					goto _st0;
				}
				_st84:
				if ( p == eof ) {
					if ( cs >= 91 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof84;
				st_case_84:
				switch( ( (*( p))) ) {
					case 49: {
						goto _st114;
					}
					case 50: {
						goto _st113;
					}
				}
				{
					goto _st0;
				}
				_ctr138:
				{
#line 24 "parser_whammy.rl"
					mode_ = WHAMMY_MODE_DETUNE_SHALLOW; }
				
#line 2366 "/Users/serge/work/music/pure-data/ceammc/ext/src/proto/parser_whammy.cpp"
				
				goto _st114;
				_st114:
				if ( p == eof ) {
					if ( cs >= 91 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof114;
				st_case_114:
				{
					goto _st0;
				}
				_st85:
				if ( p == eof ) {
					if ( cs >= 91 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof85;
				st_case_85:
				if ( ( (*( p))) == 104 ) {
					goto _st86;
				}
				{
					goto _st0;
				}
				_st86:
				if ( p == eof ) {
					if ( cs >= 91 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof86;
				st_case_86:
				if ( ( (*( p))) == 97 ) {
					goto _st87;
				}
				{
					goto _st0;
				}
				_st87:
				if ( p == eof ) {
					if ( cs >= 91 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof87;
				st_case_87:
				if ( ( (*( p))) == 108 ) {
					goto _st88;
				}
				{
					goto _st0;
				}
				_st88:
				if ( p == eof ) {
					if ( cs >= 91 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof88;
				st_case_88:
				if ( ( (*( p))) == 108 ) {
					goto _st89;
				}
				{
					goto _st0;
				}
				_st89:
				if ( p == eof ) {
					if ( cs >= 91 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof89;
				st_case_89:
				if ( ( (*( p))) == 111 ) {
					goto _st90;
				}
				{
					goto _st0;
				}
				_st90:
				if ( p == eof ) {
					if ( cs >= 91 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof90;
				st_case_90:
				if ( ( (*( p))) == 119 ) {
					goto _st114;
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
				_test_eof91: cs = 91; goto _test_eof; 
				_test_eof6: cs = 6; goto _test_eof; 
				_test_eof7: cs = 7; goto _test_eof; 
				_test_eof92: cs = 92; goto _test_eof; 
				_test_eof8: cs = 8; goto _test_eof; 
				_test_eof9: cs = 9; goto _test_eof; 
				_test_eof10: cs = 10; goto _test_eof; 
				_test_eof11: cs = 11; goto _test_eof; 
				_test_eof12: cs = 12; goto _test_eof; 
				_test_eof13: cs = 13; goto _test_eof; 
				_test_eof14: cs = 14; goto _test_eof; 
				_test_eof93: cs = 93; goto _test_eof; 
				_test_eof15: cs = 15; goto _test_eof; 
				_test_eof16: cs = 16; goto _test_eof; 
				_test_eof94: cs = 94; goto _test_eof; 
				_test_eof17: cs = 17; goto _test_eof; 
				_test_eof18: cs = 18; goto _test_eof; 
				_test_eof95: cs = 95; goto _test_eof; 
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
				_test_eof96: cs = 96; goto _test_eof; 
				_test_eof31: cs = 31; goto _test_eof; 
				_test_eof97: cs = 97; goto _test_eof; 
				_test_eof32: cs = 32; goto _test_eof; 
				_test_eof33: cs = 33; goto _test_eof; 
				_test_eof34: cs = 34; goto _test_eof; 
				_test_eof35: cs = 35; goto _test_eof; 
				_test_eof36: cs = 36; goto _test_eof; 
				_test_eof98: cs = 98; goto _test_eof; 
				_test_eof99: cs = 99; goto _test_eof; 
				_test_eof37: cs = 37; goto _test_eof; 
				_test_eof100: cs = 100; goto _test_eof; 
				_test_eof38: cs = 38; goto _test_eof; 
				_test_eof39: cs = 39; goto _test_eof; 
				_test_eof40: cs = 40; goto _test_eof; 
				_test_eof41: cs = 41; goto _test_eof; 
				_test_eof42: cs = 42; goto _test_eof; 
				_test_eof43: cs = 43; goto _test_eof; 
				_test_eof44: cs = 44; goto _test_eof; 
				_test_eof45: cs = 45; goto _test_eof; 
				_test_eof46: cs = 46; goto _test_eof; 
				_test_eof101: cs = 101; goto _test_eof; 
				_test_eof47: cs = 47; goto _test_eof; 
				_test_eof48: cs = 48; goto _test_eof; 
				_test_eof102: cs = 102; goto _test_eof; 
				_test_eof49: cs = 49; goto _test_eof; 
				_test_eof50: cs = 50; goto _test_eof; 
				_test_eof103: cs = 103; goto _test_eof; 
				_test_eof51: cs = 51; goto _test_eof; 
				_test_eof52: cs = 52; goto _test_eof; 
				_test_eof104: cs = 104; goto _test_eof; 
				_test_eof53: cs = 53; goto _test_eof; 
				_test_eof105: cs = 105; goto _test_eof; 
				_test_eof54: cs = 54; goto _test_eof; 
				_test_eof55: cs = 55; goto _test_eof; 
				_test_eof56: cs = 56; goto _test_eof; 
				_test_eof57: cs = 57; goto _test_eof; 
				_test_eof58: cs = 58; goto _test_eof; 
				_test_eof106: cs = 106; goto _test_eof; 
				_test_eof59: cs = 59; goto _test_eof; 
				_test_eof107: cs = 107; goto _test_eof; 
				_test_eof60: cs = 60; goto _test_eof; 
				_test_eof61: cs = 61; goto _test_eof; 
				_test_eof62: cs = 62; goto _test_eof; 
				_test_eof63: cs = 63; goto _test_eof; 
				_test_eof64: cs = 64; goto _test_eof; 
				_test_eof108: cs = 108; goto _test_eof; 
				_test_eof65: cs = 65; goto _test_eof; 
				_test_eof66: cs = 66; goto _test_eof; 
				_test_eof109: cs = 109; goto _test_eof; 
				_test_eof67: cs = 67; goto _test_eof; 
				_test_eof68: cs = 68; goto _test_eof; 
				_test_eof69: cs = 69; goto _test_eof; 
				_test_eof70: cs = 70; goto _test_eof; 
				_test_eof71: cs = 71; goto _test_eof; 
				_test_eof72: cs = 72; goto _test_eof; 
				_test_eof110: cs = 110; goto _test_eof; 
				_test_eof73: cs = 73; goto _test_eof; 
				_test_eof74: cs = 74; goto _test_eof; 
				_test_eof111: cs = 111; goto _test_eof; 
				_test_eof75: cs = 75; goto _test_eof; 
				_test_eof76: cs = 76; goto _test_eof; 
				_test_eof77: cs = 77; goto _test_eof; 
				_test_eof112: cs = 112; goto _test_eof; 
				_test_eof78: cs = 78; goto _test_eof; 
				_test_eof79: cs = 79; goto _test_eof; 
				_test_eof80: cs = 80; goto _test_eof; 
				_test_eof113: cs = 113; goto _test_eof; 
				_test_eof81: cs = 81; goto _test_eof; 
				_test_eof82: cs = 82; goto _test_eof; 
				_test_eof83: cs = 83; goto _test_eof; 
				_test_eof84: cs = 84; goto _test_eof; 
				_test_eof114: cs = 114; goto _test_eof; 
				_test_eof85: cs = 85; goto _test_eof; 
				_test_eof86: cs = 86; goto _test_eof; 
				_test_eof87: cs = 87; goto _test_eof; 
				_test_eof88: cs = 88; goto _test_eof; 
				_test_eof89: cs = 89; goto _test_eof; 
				_test_eof90: cs = 90; goto _test_eof; 
				
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
						case 4: {
							break;
						}
						case 5: {
							break;
						}
						case 91: {
							break;
						}
						case 6: {
							break;
						}
						case 7: {
							break;
						}
						case 92: {
							break;
						}
						case 8: {
							break;
						}
						case 9: {
							break;
						}
						case 10: {
							break;
						}
						case 11: {
							break;
						}
						case 12: {
							break;
						}
						case 13: {
							break;
						}
						case 14: {
							break;
						}
						case 93: {
							break;
						}
						case 15: {
							break;
						}
						case 16: {
							break;
						}
						case 94: {
							break;
						}
						case 17: {
							break;
						}
						case 18: {
							break;
						}
						case 95: {
							break;
						}
						case 19: {
							break;
						}
						case 20: {
							break;
						}
						case 21: {
							break;
						}
						case 22: {
							break;
						}
						case 23: {
							break;
						}
						case 24: {
							break;
						}
						case 25: {
							break;
						}
						case 26: {
							break;
						}
						case 27: {
							break;
						}
						case 28: {
							break;
						}
						case 29: {
							break;
						}
						case 30: {
							break;
						}
						case 96: {
							break;
						}
						case 31: {
							break;
						}
						case 97: {
							break;
						}
						case 32: {
							break;
						}
						case 33: {
							break;
						}
						case 34: {
							break;
						}
						case 35: {
							break;
						}
						case 36: {
							break;
						}
						case 98: {
							break;
						}
						case 99: {
							break;
						}
						case 37: {
							break;
						}
						case 100: {
							break;
						}
						case 38: {
							break;
						}
						case 39: {
							break;
						}
						case 40: {
							break;
						}
						case 41: {
							break;
						}
						case 42: {
							break;
						}
						case 43: {
							break;
						}
						case 44: {
							break;
						}
						case 45: {
							break;
						}
						case 46: {
							break;
						}
						case 101: {
							break;
						}
						case 47: {
							break;
						}
						case 48: {
							break;
						}
						case 102: {
							break;
						}
						case 49: {
							break;
						}
						case 50: {
							break;
						}
						case 103: {
							break;
						}
						case 51: {
							break;
						}
						case 52: {
							break;
						}
						case 104: {
							break;
						}
						case 53: {
							break;
						}
						case 105: {
							break;
						}
						case 54: {
							break;
						}
						case 55: {
							break;
						}
						case 56: {
							break;
						}
						case 57: {
							break;
						}
						case 58: {
							break;
						}
						case 106: {
							break;
						}
						case 59: {
							break;
						}
						case 107: {
							break;
						}
						case 60: {
							break;
						}
						case 61: {
							break;
						}
						case 62: {
							break;
						}
						case 63: {
							break;
						}
						case 64: {
							break;
						}
						case 108: {
							break;
						}
						case 65: {
							break;
						}
						case 66: {
							break;
						}
						case 109: {
							break;
						}
						case 67: {
							break;
						}
						case 68: {
							break;
						}
						case 69: {
							break;
						}
						case 70: {
							break;
						}
						case 71: {
							break;
						}
						case 72: {
							break;
						}
						case 110: {
							break;
						}
						case 73: {
							break;
						}
						case 74: {
							break;
						}
						case 111: {
							break;
						}
						case 75: {
							break;
						}
						case 76: {
							break;
						}
						case 77: {
							break;
						}
						case 112: {
							break;
						}
						case 78: {
							break;
						}
						case 79: {
							break;
						}
						case 80: {
							break;
						}
						case 113: {
							break;
						}
						case 81: {
							break;
						}
						case 82: {
							break;
						}
						case 83: {
							break;
						}
						case 84: {
							break;
						}
						case 114: {
							break;
						}
						case 85: {
							break;
						}
						case 86: {
							break;
						}
						case 87: {
							break;
						}
						case 88: {
							break;
						}
						case 89: {
							break;
						}
						case 90: {
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
						goto _st3;case 4:
						goto _st4;case 5:
						goto _st5;case 91:
						goto _ctr115;case 6:
						goto _st6;case 7:
						goto _st7;case 92:
						goto _ctr116;case 8:
						goto _st8;case 9:
						goto _st9;case 10:
						goto _st10;case 11:
						goto _st11;case 12:
						goto _st12;case 13:
						goto _st13;case 14:
						goto _st14;case 93:
						goto _ctr117;case 15:
						goto _st15;case 16:
						goto _st16;case 94:
						goto _ctr118;case 17:
						goto _st17;case 18:
						goto _st18;case 95:
						goto _ctr119;case 19:
						goto _st19;case 20:
						goto _st20;case 21:
						goto _st21;case 22:
						goto _st22;case 23:
						goto _st23;case 24:
						goto _st24;case 25:
						goto _st25;case 26:
						goto _st26;case 27:
						goto _st27;case 28:
						goto _st28;case 29:
						goto _st29;case 30:
						goto _st30;case 96:
						goto _ctr120;case 31:
						goto _st31;case 97:
						goto _ctr121;case 32:
						goto _st32;case 33:
						goto _st33;case 34:
						goto _st34;case 35:
						goto _st35;case 36:
						goto _st36;case 98:
						goto _ctr122;case 99:
						goto _ctr123;case 37:
						goto _st37;case 100:
						goto _ctr124;case 38:
						goto _st38;case 39:
						goto _st39;case 40:
						goto _st40;case 41:
						goto _st41;case 42:
						goto _st42;case 43:
						goto _st43;case 44:
						goto _st44;case 45:
						goto _st45;case 46:
						goto _st46;case 101:
						goto _ctr125;case 47:
						goto _st47;case 48:
						goto _st48;case 102:
						goto _ctr126;case 49:
						goto _st49;case 50:
						goto _st50;case 103:
						goto _ctr127;case 51:
						goto _st51;case 52:
						goto _st52;case 104:
						goto _ctr128;case 53:
						goto _st53;case 105:
						goto _ctr129;case 54:
						goto _st54;case 55:
						goto _st55;case 56:
						goto _st56;case 57:
						goto _st57;case 58:
						goto _st58;case 106:
						goto _ctr130;case 59:
						goto _st59;case 107:
						goto _ctr131;case 60:
						goto _st60;case 61:
						goto _st61;case 62:
						goto _st62;case 63:
						goto _st63;case 64:
						goto _st64;case 108:
						goto _ctr132;case 65:
						goto _st65;case 66:
						goto _st66;case 109:
						goto _ctr133;case 67:
						goto _st67;case 68:
						goto _st68;case 69:
						goto _st69;case 70:
						goto _st70;case 71:
						goto _st71;case 72:
						goto _st72;case 110:
						goto _ctr134;case 73:
						goto _st73;case 74:
						goto _st74;case 111:
						goto _ctr135;case 75:
						goto _st75;case 76:
						goto _st76;case 77:
						goto _st77;case 112:
						goto _ctr136;case 78:
						goto _st78;case 79:
						goto _st79;case 80:
						goto _st80;case 113:
						goto _ctr137;case 81:
						goto _st81;case 82:
						goto _st82;case 83:
						goto _st83;case 84:
						goto _st84;case 114:
						goto _ctr138;case 85:
						goto _st85;case 86:
						goto _st86;case 87:
						goto _st87;case 88:
						goto _st88;case 89:
						goto _st89;case 90:
						goto _st90;	}
				}
				
				if ( cs >= 91 )
					goto _out; _pop: {}
				_out: {}
			}
			
#line 52 "parser_whammy.rl"
			
			
			const auto ok = cs >= 
#line 3080 "/Users/serge/work/music/pure-data/ceammc/ext/src/proto/parser_whammy.cpp"
			91
#line 54 "parser_whammy.rl"
			;
			if (ok)
				mode = mode_;
			
			return ok;
		}
		
	}
}
