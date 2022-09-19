#line 1 "parser_whammy.rl"
# include "parser_whammy_common.h"

# include <cstring>

namespace ceammc {
	namespace proto {
		
		
#line 11 "parser_whammy.cpp"
		static const int whammy_ragel_start = 1;
		static const int whammy_ragel_first_final = 115;
		static const int whammy_ragel_error = 0;
		
		static const int whammy_ragel_en_main = 1;
		
		
#line 38 "parser_whammy.rl"
		
		
		bool nameToWhammyMode(const char* str, WhammyMode& mode) {
			int cs = 0;
			const char* p = str;
			WhammyMode mode_ = WHAMMY_MODE_UP_OCT;
			
			
#line 28 "parser_whammy.cpp"
			{
				cs = (int)whammy_ragel_start;
			}
			
#line 45 "parser_whammy.rl"
			
			
#line 36 "parser_whammy.cpp"
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
					case 115:
					goto st_case_115;
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
					case 37:
					goto st_case_37;
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
					case 47:
					goto st_case_47;
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
					case 55:
					goto st_case_55;
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
					case 63:
					goto st_case_63;
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
					case 76:
					goto st_case_76;
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
					case 84:
					goto st_case_84;
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
					case 93:
					goto st_case_93;
					case 94:
					goto st_case_94;
					case 95:
					goto st_case_95;
					case 96:
					goto st_case_96;
					case 97:
					goto st_case_97;
					case 98:
					goto st_case_98;
					case 99:
					goto st_case_99;
					case 100:
					goto st_case_100;
					case 101:
					goto st_case_101;
					case 102:
					goto st_case_102;
					case 103:
					goto st_case_103;
					case 104:
					goto st_case_104;
					case 105:
					goto st_case_105;
					case 106:
					goto st_case_106;
					case 107:
					goto st_case_107;
					case 108:
					goto st_case_108;
					case 109:
					goto st_case_109;
					case 110:
					goto st_case_110;
					case 111:
					goto st_case_111;
					case 112:
					goto st_case_112;
					case 113:
					goto st_case_113;
					case 114:
					goto st_case_114;
				}
				goto st_out;
				p += 1;
				st_case_1:
				switch( ( (*( p))) ) {
					case 43: {
						goto _st2;
					}
					case 45: {
						goto _st53;
					}
					case 49: {
						goto _st3;
					}
					case 50: {
						goto _st89;
					}
					case 52: {
						goto _st90;
					}
					case 53: {
						goto _st93;
					}
					case 98: {
						goto _st96;
					}
					case 100: {
						goto _st100;
					}
					case 111: {
						goto _st4;
					}
					case 115: {
						goto _st109;
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
				switch( ( (*( p))) ) {
					case 49: {
						goto _st3;
					}
					case 50: {
						goto _st7;
					}
					case 51: {
						goto _st18;
					}
					case 52: {
						goto _st34;
					}
					case 53: {
						goto _st42;
					}
					case 111: {
						goto _st4;
					}
				}
				{
					goto _st0;
				}
				_st3:
				p += 1;
				st_case_3:
				if ( ( (*( p))) == 111 ) {
					goto _st4;
				}
				{
					goto _st0;
				}
				_st4:
				p += 1;
				st_case_4:
				if ( ( (*( p))) == 99 ) {
					goto _st5;
				}
				{
					goto _st0;
				}
				_st5:
				p += 1;
				st_case_5:
				if ( ( (*( p))) == 116 ) {
					goto _st6;
				}
				{
					goto _st0;
				}
				_st6:
				p += 1;
				st_case_6:
				if ( ( (*( p))) == 0 ) {
					goto _ctr18;
				}
				{
					goto _st0;
				}
				_ctr18:
				{
#line 14 "parser_whammy.rl"
					mode_ = WHAMMY_MODE_UP_OCT; }
				
#line 381 "parser_whammy.cpp"
				
				{
#line 35 "parser_whammy.rl"
					{p+= 1; cs = 115; goto _out;} }
				
#line 387 "parser_whammy.cpp"
				
				goto _st115;
				_ctr23:
				{
#line 25 "parser_whammy.rl"
					mode_ = WHAMMY_MODE_UP_2ND_UP_3RD; }
				
#line 395 "parser_whammy.cpp"
				
				{
#line 35 "parser_whammy.rl"
					{p+= 1; cs = 115; goto _out;} }
				
#line 401 "parser_whammy.cpp"
				
				goto _st115;
				_ctr30:
				{
#line 13 "parser_whammy.rl"
					mode_ = WHAMMY_MODE_UP_2OCT; }
				
#line 409 "parser_whammy.cpp"
				
				{
#line 35 "parser_whammy.rl"
					{p+= 1; cs = 115; goto _out;} }
				
#line 415 "parser_whammy.cpp"
				
				goto _st115;
				_ctr35:
				{
#line 27 "parser_whammy.rl"
					mode_ = WHAMMY_MODE_UP_3RD_UP_4TH; }
				
#line 423 "parser_whammy.cpp"
				
				{
#line 35 "parser_whammy.rl"
					{p+= 1; cs = 115; goto _out;} }
				
#line 429 "parser_whammy.cpp"
				
				goto _st115;
				_ctr39:
				{
#line 26 "parser_whammy.rl"
					mode_ = WHAMMY_MODE_UP_3RD_UP_3RD; }
				
#line 437 "parser_whammy.cpp"
				
				{
#line 35 "parser_whammy.rl"
					{p+= 1; cs = 115; goto _out;} }
				
#line 443 "parser_whammy.cpp"
				
				goto _st115;
				_ctr51:
				{
#line 28 "parser_whammy.rl"
					mode_ = WHAMMY_MODE_UP_4TH_UP_5TH; }
				
#line 451 "parser_whammy.cpp"
				
				{
#line 35 "parser_whammy.rl"
					{p+= 1; cs = 115; goto _out;} }
				
#line 457 "parser_whammy.cpp"
				
				goto _st115;
				_ctr53:
				{
#line 16 "parser_whammy.rl"
					mode_ = WHAMMY_MODE_UP_4TH; }
				
#line 465 "parser_whammy.cpp"
				
				{
#line 35 "parser_whammy.rl"
					{p+= 1; cs = 115; goto _out;} }
				
#line 471 "parser_whammy.cpp"
				
				goto _st115;
				_ctr61:
				{
#line 29 "parser_whammy.rl"
					mode_ = WHAMMY_MODE_UP_5TH_UP_6TH; }
				
#line 479 "parser_whammy.cpp"
				
				{
#line 35 "parser_whammy.rl"
					{p+= 1; cs = 115; goto _out;} }
				
#line 485 "parser_whammy.cpp"
				
				goto _st115;
				_ctr62:
				{
#line 30 "parser_whammy.rl"
					mode_ = WHAMMY_MODE_UP_5TH_UP_7TH; }
				
#line 493 "parser_whammy.cpp"
				
				{
#line 35 "parser_whammy.rl"
					{p+= 1; cs = 115; goto _out;} }
				
#line 499 "parser_whammy.cpp"
				
				goto _st115;
				_ctr64:
				{
#line 15 "parser_whammy.rl"
					mode_ = WHAMMY_MODE_UP_5TH; }
				
#line 507 "parser_whammy.cpp"
				
				{
#line 35 "parser_whammy.rl"
					{p+= 1; cs = 115; goto _out;} }
				
#line 513 "parser_whammy.cpp"
				
				goto _st115;
				_ctr79:
				{
#line 20 "parser_whammy.rl"
					mode_ = WHAMMY_MODE_DOWN_OCT; }
				
#line 521 "parser_whammy.cpp"
				
				{
#line 35 "parser_whammy.rl"
					{p+= 1; cs = 115; goto _out;} }
				
#line 527 "parser_whammy.cpp"
				
				goto _st115;
				_ctr83:
				{
#line 17 "parser_whammy.rl"
					mode_ = WHAMMY_MODE_DOWN_2ND; }
				
#line 535 "parser_whammy.cpp"
				
				{
#line 35 "parser_whammy.rl"
					{p+= 1; cs = 115; goto _out;} }
				
#line 541 "parser_whammy.cpp"
				
				goto _st115;
				_ctr86:
				{
#line 21 "parser_whammy.rl"
					mode_ = WHAMMY_MODE_DOWN_2OCT; }
				
#line 549 "parser_whammy.cpp"
				
				{
#line 35 "parser_whammy.rl"
					{p+= 1; cs = 115; goto _out;} }
				
#line 555 "parser_whammy.cpp"
				
				goto _st115;
				_ctr90:
				{
#line 31 "parser_whammy.rl"
					mode_ = WHAMMY_MODE_DOWN_4TH_DOWN_3TH; }
				
#line 563 "parser_whammy.cpp"
				
				{
#line 35 "parser_whammy.rl"
					{p+= 1; cs = 115; goto _out;} }
				
#line 569 "parser_whammy.cpp"
				
				goto _st115;
				_ctr92:
				{
#line 18 "parser_whammy.rl"
					mode_ = WHAMMY_MODE_DOWN_4TH; }
				
#line 577 "parser_whammy.cpp"
				
				{
#line 35 "parser_whammy.rl"
					{p+= 1; cs = 115; goto _out;} }
				
#line 583 "parser_whammy.cpp"
				
				goto _st115;
				_ctr99:
				{
#line 32 "parser_whammy.rl"
					mode_ = WHAMMY_MODE_DOWN_5TH_DOWN_4TH; }
				
#line 591 "parser_whammy.cpp"
				
				{
#line 35 "parser_whammy.rl"
					{p+= 1; cs = 115; goto _out;} }
				
#line 597 "parser_whammy.cpp"
				
				goto _st115;
				_ctr101:
				{
#line 19 "parser_whammy.rl"
					mode_ = WHAMMY_MODE_DOWN_5TH; }
				
#line 605 "parser_whammy.cpp"
				
				{
#line 35 "parser_whammy.rl"
					{p+= 1; cs = 115; goto _out;} }
				
#line 611 "parser_whammy.cpp"
				
				goto _st115;
				_ctr107:
				{
#line 33 "parser_whammy.rl"
					mode_ = WHAMMY_MODE_DOWN_OCT_UP_OCT; }
				
#line 619 "parser_whammy.cpp"
				
				{
#line 35 "parser_whammy.rl"
					{p+= 1; cs = 115; goto _out;} }
				
#line 625 "parser_whammy.cpp"
				
				goto _st115;
				_ctr120:
				{
#line 22 "parser_whammy.rl"
					mode_ = WHAMMY_MODE_DIVE_BOMB; }
				
#line 633 "parser_whammy.cpp"
				
				{
#line 35 "parser_whammy.rl"
					{p+= 1; cs = 115; goto _out;} }
				
#line 639 "parser_whammy.cpp"
				
				goto _st115;
				_ctr125:
				{
#line 23 "parser_whammy.rl"
					mode_ = WHAMMY_MODE_DETUNE_DEEP; }
				
#line 647 "parser_whammy.cpp"
				
				{
#line 35 "parser_whammy.rl"
					{p+= 1; cs = 115; goto _out;} }
				
#line 653 "parser_whammy.cpp"
				
				goto _st115;
				_ctr130:
				{
#line 24 "parser_whammy.rl"
					mode_ = WHAMMY_MODE_DETUNE_SHALLOW; }
				
#line 661 "parser_whammy.cpp"
				
				{
#line 35 "parser_whammy.rl"
					{p+= 1; cs = 115; goto _out;} }
				
#line 667 "parser_whammy.cpp"
				
				goto _st115;
				_st115:
				p += 1;
				st_case_115:
				{
					goto _st0;
				}
				_st7:
				p += 1;
				st_case_7:
				switch( ( (*( p))) ) {
					case 43: {
						goto _st8;
					}
					case 110: {
						goto _st10;
					}
					case 111: {
						goto _st15;
					}
				}
				{
					goto _st0;
				}
				_st8:
				p += 1;
				st_case_8:
				if ( ( (*( p))) == 51 ) {
					goto _st9;
				}
				{
					goto _st0;
				}
				_st9:
				p += 1;
				st_case_9:
				if ( ( (*( p))) == 0 ) {
					goto _ctr23;
				}
				{
					goto _st0;
				}
				_st10:
				p += 1;
				st_case_10:
				if ( ( (*( p))) == 100 ) {
					goto _st11;
				}
				{
					goto _st0;
				}
				_st11:
				p += 1;
				st_case_11:
				if ( ( (*( p))) == 43 ) {
					goto _st12;
				}
				{
					goto _st0;
				}
				_st12:
				p += 1;
				st_case_12:
				if ( ( (*( p))) == 51 ) {
					goto _st13;
				}
				{
					goto _st0;
				}
				_st13:
				p += 1;
				st_case_13:
				if ( ( (*( p))) == 114 ) {
					goto _st14;
				}
				{
					goto _st0;
				}
				_st14:
				p += 1;
				st_case_14:
				if ( ( (*( p))) == 100 ) {
					goto _st9;
				}
				{
					goto _st0;
				}
				_st15:
				p += 1;
				st_case_15:
				if ( ( (*( p))) == 99 ) {
					goto _st16;
				}
				{
					goto _st0;
				}
				_st16:
				p += 1;
				st_case_16:
				if ( ( (*( p))) == 116 ) {
					goto _st17;
				}
				{
					goto _st0;
				}
				_st17:
				p += 1;
				st_case_17:
				if ( ( (*( p))) == 0 ) {
					goto _ctr30;
				}
				{
					goto _st0;
				}
				_st18:
				p += 1;
				st_case_18:
				switch( ( (*( p))) ) {
					case 43: {
						goto _st19;
					}
					case 98: {
						goto _st21;
					}
					case 114: {
						goto _st29;
					}
				}
				{
					goto _st0;
				}
				_st19:
				p += 1;
				st_case_19:
				if ( ( (*( p))) == 52 ) {
					goto _st20;
				}
				{
					goto _st0;
				}
				_st20:
				p += 1;
				st_case_20:
				if ( ( (*( p))) == 0 ) {
					goto _ctr35;
				}
				{
					goto _st0;
				}
				_st21:
				p += 1;
				st_case_21:
				switch( ( (*( p))) ) {
					case 43: {
						goto _st22;
					}
					case 114: {
						goto _st24;
					}
				}
				{
					goto _st0;
				}
				_st22:
				p += 1;
				st_case_22:
				if ( ( (*( p))) == 51 ) {
					goto _st23;
				}
				{
					goto _st0;
				}
				_st23:
				p += 1;
				st_case_23:
				if ( ( (*( p))) == 0 ) {
					goto _ctr39;
				}
				{
					goto _st0;
				}
				_st24:
				p += 1;
				st_case_24:
				if ( ( (*( p))) == 100 ) {
					goto _st25;
				}
				{
					goto _st0;
				}
				_st25:
				p += 1;
				st_case_25:
				if ( ( (*( p))) == 43 ) {
					goto _st26;
				}
				{
					goto _st0;
				}
				_st26:
				p += 1;
				st_case_26:
				if ( ( (*( p))) == 51 ) {
					goto _st27;
				}
				{
					goto _st0;
				}
				_st27:
				p += 1;
				st_case_27:
				if ( ( (*( p))) == 114 ) {
					goto _st28;
				}
				{
					goto _st0;
				}
				_st28:
				p += 1;
				st_case_28:
				if ( ( (*( p))) == 100 ) {
					goto _st23;
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
				if ( ( (*( p))) == 43 ) {
					goto _st31;
				}
				{
					goto _st0;
				}
				_st31:
				p += 1;
				st_case_31:
				if ( ( (*( p))) == 52 ) {
					goto _st32;
				}
				{
					goto _st0;
				}
				_st32:
				p += 1;
				st_case_32:
				if ( ( (*( p))) == 116 ) {
					goto _st33;
				}
				{
					goto _st0;
				}
				_st33:
				p += 1;
				st_case_33:
				if ( ( (*( p))) == 104 ) {
					goto _st20;
				}
				{
					goto _st0;
				}
				_st34:
				p += 1;
				st_case_34:
				switch( ( (*( p))) ) {
					case 43: {
						goto _st35;
					}
					case 116: {
						goto _st37;
					}
				}
				{
					goto _st0;
				}
				_st35:
				p += 1;
				st_case_35:
				if ( ( (*( p))) == 53 ) {
					goto _st36;
				}
				{
					goto _st0;
				}
				_st36:
				p += 1;
				st_case_36:
				if ( ( (*( p))) == 0 ) {
					goto _ctr51;
				}
				{
					goto _st0;
				}
				_st37:
				p += 1;
				st_case_37:
				if ( ( (*( p))) == 104 ) {
					goto _st38;
				}
				{
					goto _st0;
				}
				_st38:
				p += 1;
				st_case_38:
				switch( ( (*( p))) ) {
					case 0: {
						goto _ctr53;
					}
					case 43: {
						goto _st39;
					}
				}
				{
					goto _st0;
				}
				_st39:
				p += 1;
				st_case_39:
				if ( ( (*( p))) == 53 ) {
					goto _st40;
				}
				{
					goto _st0;
				}
				_st40:
				p += 1;
				st_case_40:
				if ( ( (*( p))) == 116 ) {
					goto _st41;
				}
				{
					goto _st0;
				}
				_st41:
				p += 1;
				st_case_41:
				if ( ( (*( p))) == 104 ) {
					goto _st36;
				}
				{
					goto _st0;
				}
				_st42:
				p += 1;
				st_case_42:
				switch( ( (*( p))) ) {
					case 43: {
						goto _st43;
					}
					case 116: {
						goto _st46;
					}
				}
				{
					goto _st0;
				}
				_st43:
				p += 1;
				st_case_43:
				switch( ( (*( p))) ) {
					case 54: {
						goto _st44;
					}
					case 55: {
						goto _st45;
					}
				}
				{
					goto _st0;
				}
				_st44:
				p += 1;
				st_case_44:
				if ( ( (*( p))) == 0 ) {
					goto _ctr61;
				}
				{
					goto _st0;
				}
				_st45:
				p += 1;
				st_case_45:
				if ( ( (*( p))) == 0 ) {
					goto _ctr62;
				}
				{
					goto _st0;
				}
				_st46:
				p += 1;
				st_case_46:
				if ( ( (*( p))) == 104 ) {
					goto _st47;
				}
				{
					goto _st0;
				}
				_st47:
				p += 1;
				st_case_47:
				switch( ( (*( p))) ) {
					case 0: {
						goto _ctr64;
					}
					case 43: {
						goto _st48;
					}
				}
				{
					goto _st0;
				}
				_st48:
				p += 1;
				st_case_48:
				switch( ( (*( p))) ) {
					case 54: {
						goto _st49;
					}
					case 55: {
						goto _st51;
					}
				}
				{
					goto _st0;
				}
				_st49:
				p += 1;
				st_case_49:
				if ( ( (*( p))) == 116 ) {
					goto _st50;
				}
				{
					goto _st0;
				}
				_st50:
				p += 1;
				st_case_50:
				if ( ( (*( p))) == 104 ) {
					goto _st44;
				}
				{
					goto _st0;
				}
				_st51:
				p += 1;
				st_case_51:
				if ( ( (*( p))) == 116 ) {
					goto _st52;
				}
				{
					goto _st0;
				}
				_st52:
				p += 1;
				st_case_52:
				if ( ( (*( p))) == 104 ) {
					goto _st45;
				}
				{
					goto _st0;
				}
				_st53:
				p += 1;
				st_case_53:
				switch( ( (*( p))) ) {
					case 49: {
						goto _st54;
					}
					case 50: {
						goto _st58;
					}
					case 52: {
						goto _st64;
					}
					case 53: {
						goto _st72;
					}
					case 56: {
						goto _st80;
					}
					case 111: {
						goto _st83;
					}
				}
				{
					goto _st0;
				}
				_st54:
				p += 1;
				st_case_54:
				if ( ( (*( p))) == 111 ) {
					goto _st55;
				}
				{
					goto _st0;
				}
				_st55:
				p += 1;
				st_case_55:
				if ( ( (*( p))) == 99 ) {
					goto _st56;
				}
				{
					goto _st0;
				}
				_st56:
				p += 1;
				st_case_56:
				if ( ( (*( p))) == 116 ) {
					goto _st57;
				}
				{
					goto _st0;
				}
				_st57:
				p += 1;
				st_case_57:
				if ( ( (*( p))) == 0 ) {
					goto _ctr79;
				}
				{
					goto _st0;
				}
				_st58:
				p += 1;
				st_case_58:
				switch( ( (*( p))) ) {
					case 110: {
						goto _st59;
					}
					case 111: {
						goto _st61;
					}
				}
				{
					goto _st0;
				}
				_st59:
				p += 1;
				st_case_59:
				if ( ( (*( p))) == 100 ) {
					goto _st60;
				}
				{
					goto _st0;
				}
				_st60:
				p += 1;
				st_case_60:
				if ( ( (*( p))) == 0 ) {
					goto _ctr83;
				}
				{
					goto _st0;
				}
				_st61:
				p += 1;
				st_case_61:
				if ( ( (*( p))) == 99 ) {
					goto _st62;
				}
				{
					goto _st0;
				}
				_st62:
				p += 1;
				st_case_62:
				if ( ( (*( p))) == 116 ) {
					goto _st63;
				}
				{
					goto _st0;
				}
				_st63:
				p += 1;
				st_case_63:
				if ( ( (*( p))) == 0 ) {
					goto _ctr86;
				}
				{
					goto _st0;
				}
				_st64:
				p += 1;
				st_case_64:
				switch( ( (*( p))) ) {
					case 45: {
						goto _st65;
					}
					case 116: {
						goto _st67;
					}
				}
				{
					goto _st0;
				}
				_st65:
				p += 1;
				st_case_65:
				if ( ( (*( p))) == 51 ) {
					goto _st66;
				}
				{
					goto _st0;
				}
				_st66:
				p += 1;
				st_case_66:
				if ( ( (*( p))) == 0 ) {
					goto _ctr90;
				}
				{
					goto _st0;
				}
				_st67:
				p += 1;
				st_case_67:
				if ( ( (*( p))) == 104 ) {
					goto _st68;
				}
				{
					goto _st0;
				}
				_st68:
				p += 1;
				st_case_68:
				switch( ( (*( p))) ) {
					case 0: {
						goto _ctr92;
					}
					case 45: {
						goto _st69;
					}
				}
				{
					goto _st0;
				}
				_st69:
				p += 1;
				st_case_69:
				if ( ( (*( p))) == 51 ) {
					goto _st70;
				}
				{
					goto _st0;
				}
				_st70:
				p += 1;
				st_case_70:
				if ( ( (*( p))) == 114 ) {
					goto _st71;
				}
				{
					goto _st0;
				}
				_st71:
				p += 1;
				st_case_71:
				if ( ( (*( p))) == 100 ) {
					goto _st66;
				}
				{
					goto _st0;
				}
				_st72:
				p += 1;
				st_case_72:
				switch( ( (*( p))) ) {
					case 45: {
						goto _st73;
					}
					case 116: {
						goto _st75;
					}
				}
				{
					goto _st0;
				}
				_st73:
				p += 1;
				st_case_73:
				if ( ( (*( p))) == 52 ) {
					goto _st74;
				}
				{
					goto _st0;
				}
				_st74:
				p += 1;
				st_case_74:
				if ( ( (*( p))) == 0 ) {
					goto _ctr99;
				}
				{
					goto _st0;
				}
				_st75:
				p += 1;
				st_case_75:
				if ( ( (*( p))) == 104 ) {
					goto _st76;
				}
				{
					goto _st0;
				}
				_st76:
				p += 1;
				st_case_76:
				switch( ( (*( p))) ) {
					case 0: {
						goto _ctr101;
					}
					case 45: {
						goto _st77;
					}
				}
				{
					goto _st0;
				}
				_st77:
				p += 1;
				st_case_77:
				if ( ( (*( p))) == 52 ) {
					goto _st78;
				}
				{
					goto _st0;
				}
				_st78:
				p += 1;
				st_case_78:
				if ( ( (*( p))) == 116 ) {
					goto _st79;
				}
				{
					goto _st0;
				}
				_st79:
				p += 1;
				st_case_79:
				if ( ( (*( p))) == 104 ) {
					goto _st74;
				}
				{
					goto _st0;
				}
				_st80:
				p += 1;
				st_case_80:
				if ( ( (*( p))) == 43 ) {
					goto _st81;
				}
				{
					goto _st0;
				}
				_st81:
				p += 1;
				st_case_81:
				if ( ( (*( p))) == 56 ) {
					goto _st82;
				}
				{
					goto _st0;
				}
				_st82:
				p += 1;
				st_case_82:
				if ( ( (*( p))) == 0 ) {
					goto _ctr107;
				}
				{
					goto _st0;
				}
				_st83:
				p += 1;
				st_case_83:
				if ( ( (*( p))) == 99 ) {
					goto _st84;
				}
				{
					goto _st0;
				}
				_st84:
				p += 1;
				st_case_84:
				if ( ( (*( p))) == 116 ) {
					goto _st85;
				}
				{
					goto _st0;
				}
				_st85:
				p += 1;
				st_case_85:
				switch( ( (*( p))) ) {
					case 0: {
						goto _ctr79;
					}
					case 43: {
						goto _st86;
					}
				}
				{
					goto _st0;
				}
				_st86:
				p += 1;
				st_case_86:
				if ( ( (*( p))) == 111 ) {
					goto _st87;
				}
				{
					goto _st0;
				}
				_st87:
				p += 1;
				st_case_87:
				if ( ( (*( p))) == 99 ) {
					goto _st88;
				}
				{
					goto _st0;
				}
				_st88:
				p += 1;
				st_case_88:
				if ( ( (*( p))) == 116 ) {
					goto _st82;
				}
				{
					goto _st0;
				}
				_st89:
				p += 1;
				st_case_89:
				if ( ( (*( p))) == 111 ) {
					goto _st15;
				}
				{
					goto _st0;
				}
				_st90:
				p += 1;
				st_case_90:
				if ( ( (*( p))) == 116 ) {
					goto _st91;
				}
				{
					goto _st0;
				}
				_st91:
				p += 1;
				st_case_91:
				if ( ( (*( p))) == 104 ) {
					goto _st92;
				}
				{
					goto _st0;
				}
				_st92:
				p += 1;
				st_case_92:
				if ( ( (*( p))) == 0 ) {
					goto _ctr53;
				}
				{
					goto _st0;
				}
				_st93:
				p += 1;
				st_case_93:
				if ( ( (*( p))) == 116 ) {
					goto _st94;
				}
				{
					goto _st0;
				}
				_st94:
				p += 1;
				st_case_94:
				if ( ( (*( p))) == 104 ) {
					goto _st95;
				}
				{
					goto _st0;
				}
				_st95:
				p += 1;
				st_case_95:
				if ( ( (*( p))) == 0 ) {
					goto _ctr64;
				}
				{
					goto _st0;
				}
				_st96:
				p += 1;
				st_case_96:
				if ( ( (*( p))) == 111 ) {
					goto _st97;
				}
				{
					goto _st0;
				}
				_st97:
				p += 1;
				st_case_97:
				if ( ( (*( p))) == 109 ) {
					goto _st98;
				}
				{
					goto _st0;
				}
				_st98:
				p += 1;
				st_case_98:
				if ( ( (*( p))) == 98 ) {
					goto _st99;
				}
				{
					goto _st0;
				}
				_st99:
				p += 1;
				st_case_99:
				if ( ( (*( p))) == 0 ) {
					goto _ctr120;
				}
				{
					goto _st0;
				}
				_st100:
				p += 1;
				st_case_100:
				if ( ( (*( p))) == 101 ) {
					goto _st101;
				}
				{
					goto _st0;
				}
				_st101:
				p += 1;
				st_case_101:
				switch( ( (*( p))) ) {
					case 101: {
						goto _st102;
					}
					case 116: {
						goto _st104;
					}
				}
				{
					goto _st0;
				}
				_st102:
				p += 1;
				st_case_102:
				if ( ( (*( p))) == 112 ) {
					goto _st103;
				}
				{
					goto _st0;
				}
				_st103:
				p += 1;
				st_case_103:
				if ( ( (*( p))) == 0 ) {
					goto _ctr125;
				}
				{
					goto _st0;
				}
				_st104:
				p += 1;
				st_case_104:
				if ( ( (*( p))) == 117 ) {
					goto _st105;
				}
				{
					goto _st0;
				}
				_st105:
				p += 1;
				st_case_105:
				if ( ( (*( p))) == 110 ) {
					goto _st106;
				}
				{
					goto _st0;
				}
				_st106:
				p += 1;
				st_case_106:
				if ( ( (*( p))) == 101 ) {
					goto _st107;
				}
				{
					goto _st0;
				}
				_st107:
				p += 1;
				st_case_107:
				switch( ( (*( p))) ) {
					case 49: {
						goto _st108;
					}
					case 50: {
						goto _st103;
					}
				}
				{
					goto _st0;
				}
				_st108:
				p += 1;
				st_case_108:
				if ( ( (*( p))) == 0 ) {
					goto _ctr130;
				}
				{
					goto _st0;
				}
				_st109:
				p += 1;
				st_case_109:
				if ( ( (*( p))) == 104 ) {
					goto _st110;
				}
				{
					goto _st0;
				}
				_st110:
				p += 1;
				st_case_110:
				if ( ( (*( p))) == 97 ) {
					goto _st111;
				}
				{
					goto _st0;
				}
				_st111:
				p += 1;
				st_case_111:
				if ( ( (*( p))) == 108 ) {
					goto _st112;
				}
				{
					goto _st0;
				}
				_st112:
				p += 1;
				st_case_112:
				if ( ( (*( p))) == 108 ) {
					goto _st113;
				}
				{
					goto _st0;
				}
				_st113:
				p += 1;
				st_case_113:
				if ( ( (*( p))) == 111 ) {
					goto _st114;
				}
				{
					goto _st0;
				}
				_st114:
				p += 1;
				st_case_114:
				if ( ( (*( p))) == 119 ) {
					goto _st108;
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
				_test_eof115: cs = 115; goto _test_eof; 
				_test_eof7: cs = 7; goto _test_eof; 
				_test_eof8: cs = 8; goto _test_eof; 
				_test_eof9: cs = 9; goto _test_eof; 
				_test_eof10: cs = 10; goto _test_eof; 
				_test_eof11: cs = 11; goto _test_eof; 
				_test_eof12: cs = 12; goto _test_eof; 
				_test_eof13: cs = 13; goto _test_eof; 
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
				_test_eof32: cs = 32; goto _test_eof; 
				_test_eof33: cs = 33; goto _test_eof; 
				_test_eof34: cs = 34; goto _test_eof; 
				_test_eof35: cs = 35; goto _test_eof; 
				_test_eof36: cs = 36; goto _test_eof; 
				_test_eof37: cs = 37; goto _test_eof; 
				_test_eof38: cs = 38; goto _test_eof; 
				_test_eof39: cs = 39; goto _test_eof; 
				_test_eof40: cs = 40; goto _test_eof; 
				_test_eof41: cs = 41; goto _test_eof; 
				_test_eof42: cs = 42; goto _test_eof; 
				_test_eof43: cs = 43; goto _test_eof; 
				_test_eof44: cs = 44; goto _test_eof; 
				_test_eof45: cs = 45; goto _test_eof; 
				_test_eof46: cs = 46; goto _test_eof; 
				_test_eof47: cs = 47; goto _test_eof; 
				_test_eof48: cs = 48; goto _test_eof; 
				_test_eof49: cs = 49; goto _test_eof; 
				_test_eof50: cs = 50; goto _test_eof; 
				_test_eof51: cs = 51; goto _test_eof; 
				_test_eof52: cs = 52; goto _test_eof; 
				_test_eof53: cs = 53; goto _test_eof; 
				_test_eof54: cs = 54; goto _test_eof; 
				_test_eof55: cs = 55; goto _test_eof; 
				_test_eof56: cs = 56; goto _test_eof; 
				_test_eof57: cs = 57; goto _test_eof; 
				_test_eof58: cs = 58; goto _test_eof; 
				_test_eof59: cs = 59; goto _test_eof; 
				_test_eof60: cs = 60; goto _test_eof; 
				_test_eof61: cs = 61; goto _test_eof; 
				_test_eof62: cs = 62; goto _test_eof; 
				_test_eof63: cs = 63; goto _test_eof; 
				_test_eof64: cs = 64; goto _test_eof; 
				_test_eof65: cs = 65; goto _test_eof; 
				_test_eof66: cs = 66; goto _test_eof; 
				_test_eof67: cs = 67; goto _test_eof; 
				_test_eof68: cs = 68; goto _test_eof; 
				_test_eof69: cs = 69; goto _test_eof; 
				_test_eof70: cs = 70; goto _test_eof; 
				_test_eof71: cs = 71; goto _test_eof; 
				_test_eof72: cs = 72; goto _test_eof; 
				_test_eof73: cs = 73; goto _test_eof; 
				_test_eof74: cs = 74; goto _test_eof; 
				_test_eof75: cs = 75; goto _test_eof; 
				_test_eof76: cs = 76; goto _test_eof; 
				_test_eof77: cs = 77; goto _test_eof; 
				_test_eof78: cs = 78; goto _test_eof; 
				_test_eof79: cs = 79; goto _test_eof; 
				_test_eof80: cs = 80; goto _test_eof; 
				_test_eof81: cs = 81; goto _test_eof; 
				_test_eof82: cs = 82; goto _test_eof; 
				_test_eof83: cs = 83; goto _test_eof; 
				_test_eof84: cs = 84; goto _test_eof; 
				_test_eof85: cs = 85; goto _test_eof; 
				_test_eof86: cs = 86; goto _test_eof; 
				_test_eof87: cs = 87; goto _test_eof; 
				_test_eof88: cs = 88; goto _test_eof; 
				_test_eof89: cs = 89; goto _test_eof; 
				_test_eof90: cs = 90; goto _test_eof; 
				_test_eof91: cs = 91; goto _test_eof; 
				_test_eof92: cs = 92; goto _test_eof; 
				_test_eof93: cs = 93; goto _test_eof; 
				_test_eof94: cs = 94; goto _test_eof; 
				_test_eof95: cs = 95; goto _test_eof; 
				_test_eof96: cs = 96; goto _test_eof; 
				_test_eof97: cs = 97; goto _test_eof; 
				_test_eof98: cs = 98; goto _test_eof; 
				_test_eof99: cs = 99; goto _test_eof; 
				_test_eof100: cs = 100; goto _test_eof; 
				_test_eof101: cs = 101; goto _test_eof; 
				_test_eof102: cs = 102; goto _test_eof; 
				_test_eof103: cs = 103; goto _test_eof; 
				_test_eof104: cs = 104; goto _test_eof; 
				_test_eof105: cs = 105; goto _test_eof; 
				_test_eof106: cs = 106; goto _test_eof; 
				_test_eof107: cs = 107; goto _test_eof; 
				_test_eof108: cs = 108; goto _test_eof; 
				_test_eof109: cs = 109; goto _test_eof; 
				_test_eof110: cs = 110; goto _test_eof; 
				_test_eof111: cs = 111; goto _test_eof; 
				_test_eof112: cs = 112; goto _test_eof; 
				_test_eof113: cs = 113; goto _test_eof; 
				_test_eof114: cs = 114; goto _test_eof; 
				
				_test_eof: {}
				if ( cs >= 115 )
					goto _out; _pop: {}
				_out: {}
			}
			
#line 46 "parser_whammy.rl"
			
			
			const auto ok = cs >= 
#line 1883 "parser_whammy.cpp"
			115
#line 48 "parser_whammy.rl"
			;
			if (ok)
				mode = mode_;
			
			return ok;
		}
		
	}
}
