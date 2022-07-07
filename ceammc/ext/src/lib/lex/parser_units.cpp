#line 1 "lex/parser_units.rl"
# include "parser_units.h"

# include <cstdint>
# include <cstring>
# include <cstdio>

namespace ceammc {
	namespace parser {
		
		
#line 13 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
		static const int units_full_start = 1;
		static const int units_full_first_final = 48;
		static const int units_full_error = 0;
		
		static const int units_full_en_main = 1;
		
		
#line 16 "lex/parser_units.rl"
		
		
		UnitsFullMatch::UnitsFullMatch()
		{
			reset();
		}
		
		void UnitsFullMatch::reset()
		{
			unit_ = { };
		}
		
		bool UnitsFullMatch::parse(const Atom& a)
		{
			reset();
			
			if (a.isSymbol())
				return parse(a.asT<t_symbol*>()->s_name);
			else if(a.isFloat()) {
				unit_.value = a.asT<t_float>();
				if(a.isInteger())
				unit_.type = TYPE_INT;
				else
					unit_.type = TYPE_FLOAT;
				
				return true;
			} else
			return false;
		}
		
		bool UnitsFullMatch::parseAs(const Atom& a, AtomType t)
		{
			const bool ok = parse(a);
			return ok && (unit_.type == t || unit_.type == TYPE_INT || unit_.type == TYPE_FLOAT);
		}
		
		bool UnitsFullMatch::parse(const char* str)
		{
			const auto len = strlen(str);
			if (len == 0)
				return false;
			
			const char* p = str;
			const char* pe = p + len;
			const char* eof = pe;
			
			PositionType pos_ = POSITION_ABS;
			
			DECLARE_RAGEL_COMMON_VARS;
			DECLARE_RAGEL_NUMERIC_VARS;
			
			fsm::BpmData bpm;
			fsm::SmpteData smpte;
			
			reset();
			
			
#line 79 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
			{
				cs = (int)units_full_start;
			}
			
#line 72 "lex/parser_units.rl"
			
			
#line 87 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
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
					case 48:
					goto st_case_48;
					case 49:
					goto st_case_49;
					case 50:
					goto st_case_50;
					case 4:
					goto st_case_4;
					case 51:
					goto st_case_51;
					case 5:
					goto st_case_5;
					case 52:
					goto st_case_52;
					case 53:
					goto st_case_53;
					case 54:
					goto st_case_54;
					case 55:
					goto st_case_55;
					case 6:
					goto st_case_6;
					case 7:
					goto st_case_7;
					case 56:
					goto st_case_56;
					case 57:
					goto st_case_57;
					case 8:
					goto st_case_8;
					case 58:
					goto st_case_58;
					case 59:
					goto st_case_59;
					case 9:
					goto st_case_9;
					case 60:
					goto st_case_60;
					case 10:
					goto st_case_10;
					case 61:
					goto st_case_61;
					case 62:
					goto st_case_62;
					case 11:
					goto st_case_11;
					case 12:
					goto st_case_12;
					case 63:
					goto st_case_63;
					case 64:
					goto st_case_64;
					case 13:
					goto st_case_13;
					case 65:
					goto st_case_65;
					case 66:
					goto st_case_66;
					case 14:
					goto st_case_14;
					case 67:
					goto st_case_67;
					case 15:
					goto st_case_15;
					case 16:
					goto st_case_16;
					case 17:
					goto st_case_17;
					case 68:
					goto st_case_68;
					case 18:
					goto st_case_18;
					case 19:
					goto st_case_19;
					case 69:
					goto st_case_69;
					case 20:
					goto st_case_20;
					case 21:
					goto st_case_21;
					case 22:
					goto st_case_22;
					case 70:
					goto st_case_70;
					case 23:
					goto st_case_23;
					case 24:
					goto st_case_24;
					case 71:
					goto st_case_71;
					case 72:
					goto st_case_72;
					case 73:
					goto st_case_73;
					case 25:
					goto st_case_25;
					case 74:
					goto st_case_74;
					case 26:
					goto st_case_26;
					case 75:
					goto st_case_75;
					case 27:
					goto st_case_27;
					case 76:
					goto st_case_76;
					case 28:
					goto st_case_28;
					case 29:
					goto st_case_29;
					case 30:
					goto st_case_30;
					case 77:
					goto st_case_77;
					case 31:
					goto st_case_31;
					case 78:
					goto st_case_78;
					case 79:
					goto st_case_79;
					case 32:
					goto st_case_32;
					case 33:
					goto st_case_33;
					case 34:
					goto st_case_34;
					case 80:
					goto st_case_80;
					case 35:
					goto st_case_35;
					case 81:
					goto st_case_81;
					case 82:
					goto st_case_82;
					case 36:
					goto st_case_36;
					case 37:
					goto st_case_37;
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
					case 38:
					goto st_case_38;
					case 88:
					goto st_case_88;
					case 89:
					goto st_case_89;
					case 39:
					goto st_case_39;
					case 40:
					goto st_case_40;
					case 90:
					goto st_case_90;
					case 41:
					goto st_case_41;
					case 91:
					goto st_case_91;
					case 92:
					goto st_case_92;
					case 93:
					goto st_case_93;
					case 42:
					goto st_case_42;
					case 94:
					goto st_case_94;
					case 95:
					goto st_case_95;
					case 96:
					goto st_case_96;
					case 97:
					goto st_case_97;
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
				}
				goto st_out;
				_st1:
				if ( p == eof ) {
					if ( cs >= 48 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof1;
				st_case_1:
				switch( ( (*( p))) ) {
					case 36: {
						goto _ctr2;
					}
					case 43: {
						goto _ctr3;
					}
					case 45: {
						goto _ctr3;
					}
					case 48: {
						goto _ctr4;
					}
					case 99: {
						goto _ctr7;
					}
					case 101: {
						goto _ctr8;
					}
				}
				if ( ( (*( p))) > 53 ) {
					if ( ( (*( p))) <= 57 ) {
						goto _ctr6;
					}
				} else if ( ( (*( p))) >= 49 ) {
					goto _ctr5;
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
#line 8 "lex/ragel_units.rl"
					ragel_type = TYPE_UNKNOWN; ragel_cat = CAT_UNKNOWN; }
				
#line 340 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				{
#line 70 "lex/ragel_units.rl"
					pos_ = POSITION_ABS;}
				
#line 346 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				goto _st2;
				_st2:
				if ( p == eof ) {
					if ( cs >= 48 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof2;
				st_case_2:
				switch( ( (*( p))) ) {
					case 43: {
						goto _ctr10;
					}
					case 45: {
						goto _ctr10;
					}
					case 48: {
						goto _ctr11;
					}
				}
				if ( ( (*( p))) > 53 ) {
					if ( ( (*( p))) <= 57 ) {
						goto _ctr13;
					}
				} else if ( ( (*( p))) >= 49 ) {
					goto _ctr12;
				}
				{
					goto _st0;
				}
				_ctr3:
				{
#line 8 "lex/ragel_units.rl"
					ragel_type = TYPE_UNKNOWN; ragel_cat = CAT_UNKNOWN; }
				
#line 386 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				{
#line 70 "lex/ragel_units.rl"
					pos_ = POSITION_ABS;}
				
#line 392 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				{
#line 21 "lex/ragel_numeric.rl"
					ragel_num.sign = ((( (*( p))))=='-') ? -1 : 1; }
				
#line 398 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				{
#line 61 "lex/ragel_units.rl"
					smpte.np = 0;}
				
#line 404 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				{
#line 56 "lex/ragel_units.rl"
					smpte.sign = ((( (*( p))))=='-')?-1:1;}
				
#line 410 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				goto _st3;
				_ctr10:
				{
#line 64 "lex/ragel_units.rl"
					pos_ = POSITION_END;}
				
#line 418 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				{
#line 21 "lex/ragel_numeric.rl"
					ragel_num.sign = ((( (*( p))))=='-') ? -1 : 1; }
				
#line 424 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				{
#line 61 "lex/ragel_units.rl"
					smpte.np = 0;}
				
#line 430 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				{
#line 56 "lex/ragel_units.rl"
					smpte.sign = ((( (*( p))))=='-')?-1:1;}
				
#line 436 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				goto _st3;
				_ctr88:
				{
#line 65 "lex/ragel_units.rl"
					pos_ = POSITION_CURRENT;}
				
#line 444 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				{
#line 21 "lex/ragel_numeric.rl"
					ragel_num.sign = ((( (*( p))))=='-') ? -1 : 1; }
				
#line 450 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				{
#line 61 "lex/ragel_units.rl"
					smpte.np = 0;}
				
#line 456 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				{
#line 56 "lex/ragel_units.rl"
					smpte.sign = ((( (*( p))))=='-')?-1:1;}
				
#line 462 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				goto _st3;
				_st3:
				if ( p == eof ) {
					if ( cs >= 48 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof3;
				st_case_3:
				if ( ( (*( p))) == 48 ) {
					goto _ctr15;
				}
				if ( ( (*( p))) > 53 ) {
					if ( ( (*( p))) <= 57 ) {
						goto _ctr17;
					}
				} else if ( ( (*( p))) >= 49 ) {
					goto _ctr16;
				}
				{
					goto _st0;
				}
				_ctr11:
				{
#line 64 "lex/ragel_units.rl"
					pos_ = POSITION_END;}
				
#line 494 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				{
#line 29 "lex/ragel_numeric.rl"
					(ragel_num.vint *= 10) += ((( (*( p))))-'0'); }
				
#line 500 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				{
#line 42 "lex/ragel_numeric.rl"
					(ragel_num.ratio.num *= 10) += ((( (*( p))))-'0'); }
				
#line 506 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				{
#line 61 "lex/ragel_units.rl"
					smpte.np = 0;}
				
#line 512 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				{
#line 49 "lex/ragel_units.rl"
					smpte.hour = 0; smpte.np++;}
				
#line 518 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				{
#line 49 "lex/ragel_units.rl"
					(smpte.hour *= 10) += ((( (*( p)))) - '0');}
				
#line 524 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				goto _st48;
				_ctr15:
				{
#line 29 "lex/ragel_numeric.rl"
					(ragel_num.vint *= 10) += ((( (*( p))))-'0'); }
				
#line 532 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				{
#line 42 "lex/ragel_numeric.rl"
					(ragel_num.ratio.num *= 10) += ((( (*( p))))-'0'); }
				
#line 538 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				{
#line 49 "lex/ragel_units.rl"
					smpte.hour = 0; smpte.np++;}
				
#line 544 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				{
#line 49 "lex/ragel_units.rl"
					(smpte.hour *= 10) += ((( (*( p)))) - '0');}
				
#line 550 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				goto _st48;
				_ctr89:
				{
#line 65 "lex/ragel_units.rl"
					pos_ = POSITION_CURRENT;}
				
#line 558 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				{
#line 29 "lex/ragel_numeric.rl"
					(ragel_num.vint *= 10) += ((( (*( p))))-'0'); }
				
#line 564 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				{
#line 42 "lex/ragel_numeric.rl"
					(ragel_num.ratio.num *= 10) += ((( (*( p))))-'0'); }
				
#line 570 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				{
#line 61 "lex/ragel_units.rl"
					smpte.np = 0;}
				
#line 576 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				{
#line 49 "lex/ragel_units.rl"
					smpte.hour = 0; smpte.np++;}
				
#line 582 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				{
#line 49 "lex/ragel_units.rl"
					(smpte.hour *= 10) += ((( (*( p)))) - '0');}
				
#line 588 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				goto _st48;
				_ctr94:
				{
#line 30 "lex/ragel_numeric.rl"
					
					ragel_num.vint *= ragel_num.sign;
					ragel_type = TYPE_INT;
					ragel_cat  = CAT_NUMBER;
				}
				
#line 600 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				{
#line 7 "lex/ragel_units.rl"
					ragel_num.vdouble = ragel_num.vint; }
				
#line 606 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				{
#line 9 "lex/ragel_units.rl"
					ragel_cat = CAT_UNIT; }
				
#line 612 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				goto _st48;
				_st48:
				if ( p == eof ) {
					if ( cs >= 48 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof48;
				st_case_48:
				switch( ( (*( p))) ) {
					case 37: {
						goto _ctr95;
					}
					case 42: {
						goto _ctr96;
					}
					case 46: {
						goto _ctr97;
					}
					case 47: {
						goto _st31;
					}
					case 72: {
						goto _ctr99;
					}
					case 95: {
						goto _ctr100;
					}
					case 99: {
						goto _ctr101;
					}
					case 100: {
						goto _ctr102;
					}
					case 104: {
						goto _ctr103;
					}
					case 109: {
						goto _ctr104;
					}
					case 112: {
						goto _ctr105;
					}
					case 114: {
						goto _ctr106;
					}
					case 115: {
						goto _ctr107;
					}
				}
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr98;
				}
				{
					goto _st0;
				}
				_ctr95:
				{
#line 30 "lex/ragel_numeric.rl"
					
					ragel_num.vint *= ragel_num.sign;
					ragel_type = TYPE_INT;
					ragel_cat  = CAT_NUMBER;
				}
				
#line 682 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				{
#line 7 "lex/ragel_units.rl"
					ragel_num.vdouble = ragel_num.vint; }
				
#line 688 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				{
#line 108 "lex/ragel_numeric.rl"
					ragel_num.vdouble = ragel_num.vint;}
				
#line 694 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				goto _st49;
				_ctr108:
				{
#line 25 "lex/ragel_units.rl"
					ragel_type = TYPE_PERCENT;}
				
#line 702 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				{
#line 70 "lex/ragel_numeric.rl"
					
					ragel_type = TYPE_PERCENT;
					ragel_cat = CAT_NUMBER;
				}
				
#line 711 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				{
#line 72 "lex/ragel_units.rl"
					ragel_type = TYPE_PERCENT;}
				
#line 717 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				{
#line 9 "lex/ragel_units.rl"
					ragel_cat = CAT_UNIT; }
				
#line 723 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				goto _st49;
				_ctr111:
				{
#line 58 "lex/ragel_numeric.rl"
					
					const auto frac = double(ragel_num.ratio.num) / ragel_num.ratio.den;
					ragel_num.vdouble = ragel_num.vint + ragel_num.sign * frac;
					ragel_type = TYPE_FLOAT;
					ragel_cat  = CAT_NUMBER;
				}
				
#line 736 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				goto _st49;
				_st49:
				if ( p == eof ) {
					if ( cs >= 48 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof49;
				st_case_49:
				{
					goto _st0;
				}
				_ctr96:
				{
#line 30 "lex/ragel_numeric.rl"
					
					ragel_num.vint *= ragel_num.sign;
					ragel_type = TYPE_INT;
					ragel_cat  = CAT_NUMBER;
				}
				
#line 762 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				{
#line 7 "lex/ragel_units.rl"
					ragel_num.vdouble = ragel_num.vint; }
				
#line 768 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				{
#line 113 "lex/ragel_numeric.rl"
					ragel_num.vdouble = ragel_num.vint;}
				
#line 774 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				goto _st50;
				_ctr109:
				{
#line 27 "lex/ragel_units.rl"
					ragel_type = TYPE_PHASE;}
				
#line 782 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				{
#line 75 "lex/ragel_numeric.rl"
					
					ragel_type = TYPE_PHASE;
					ragel_cat = CAT_NUMBER;
				}
				
#line 791 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				{
#line 73 "lex/ragel_units.rl"
					ragel_type = TYPE_PHASE;}
				
#line 797 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				{
#line 9 "lex/ragel_units.rl"
					ragel_cat = CAT_UNIT; }
				
#line 803 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				goto _st50;
				_ctr112:
				{
#line 58 "lex/ragel_numeric.rl"
					
					const auto frac = double(ragel_num.ratio.num) / ragel_num.ratio.den;
					ragel_num.vdouble = ragel_num.vint + ragel_num.sign * frac;
					ragel_type = TYPE_FLOAT;
					ragel_cat  = CAT_NUMBER;
				}
				
#line 816 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				goto _st50;
				_st50:
				if ( p == eof ) {
					if ( cs >= 48 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof50;
				st_case_50:
				{
					goto _st0;
				}
				_ctr97:
				{
#line 30 "lex/ragel_numeric.rl"
					
					ragel_num.vint *= ragel_num.sign;
					ragel_type = TYPE_INT;
					ragel_cat  = CAT_NUMBER;
				}
				
#line 842 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				{
#line 50 "lex/ragel_numeric.rl"
					
					ragel_num.ratio.num = 0;
					ragel_num.ratio.den = 1;
				}
				
#line 851 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				goto _st4;
				_st4:
				if ( p == eof ) {
					if ( cs >= 48 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof4;
				st_case_4:
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr19;
				}
				{
					goto _st0;
				}
				_ctr19:
				{
#line 54 "lex/ragel_numeric.rl"
					
					(ragel_num.ratio.num *= 10) += ((( (*( p)))) - '0');
					ragel_num.ratio.den *= 10;
				}
				
#line 879 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				goto _st51;
				_ctr110:
				{
#line 58 "lex/ragel_numeric.rl"
					
					const auto frac = double(ragel_num.ratio.num) / ragel_num.ratio.den;
					ragel_num.vdouble = ragel_num.vint + ragel_num.sign * frac;
					ragel_type = TYPE_FLOAT;
					ragel_cat  = CAT_NUMBER;
				}
				
#line 892 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				{
#line 9 "lex/ragel_units.rl"
					ragel_cat = CAT_UNIT; }
				
#line 898 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				goto _st51;
				_st51:
				if ( p == eof ) {
					if ( cs >= 48 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof51;
				st_case_51:
				switch( ( (*( p))) ) {
					case 37: {
						goto _ctr111;
					}
					case 42: {
						goto _ctr112;
					}
					case 72: {
						goto _ctr113;
					}
					case 95: {
						goto _ctr114;
					}
					case 99: {
						goto _ctr115;
					}
					case 100: {
						goto _ctr116;
					}
					case 104: {
						goto _ctr117;
					}
					case 109: {
						goto _ctr118;
					}
					case 112: {
						goto _ctr119;
					}
					case 114: {
						goto _ctr120;
					}
					case 115: {
						goto _ctr121;
					}
				}
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr19;
				}
				{
					goto _st0;
				}
				_ctr99:
				{
#line 30 "lex/ragel_numeric.rl"
					
					ragel_num.vint *= ragel_num.sign;
					ragel_type = TYPE_INT;
					ragel_cat  = CAT_NUMBER;
				}
				
#line 962 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				{
#line 7 "lex/ragel_units.rl"
					ragel_num.vdouble = ragel_num.vint; }
				
#line 968 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				goto _st5;
				_ctr113:
				{
#line 58 "lex/ragel_numeric.rl"
					
					const auto frac = double(ragel_num.ratio.num) / ragel_num.ratio.den;
					ragel_num.vdouble = ragel_num.vint + ragel_num.sign * frac;
					ragel_type = TYPE_FLOAT;
					ragel_cat  = CAT_NUMBER;
				}
				
#line 981 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				goto _st5;
				_st5:
				if ( p == eof ) {
					if ( cs >= 48 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof5;
				st_case_5:
				if ( ( (*( p))) == 122 ) {
					goto _st52;
				}
				{
					goto _st0;
				}
				_ctr122:
				{
#line 13 "lex/ragel_units.rl"
					ragel_type = TYPE_HZ;}
				
#line 1006 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				{
#line 9 "lex/ragel_units.rl"
					ragel_cat = CAT_UNIT; }
				
#line 1012 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				goto _st52;
				_st52:
				if ( p == eof ) {
					if ( cs >= 48 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof52;
				st_case_52:
				{
					goto _st0;
				}
				_ctr100:
				{
#line 30 "lex/ragel_numeric.rl"
					
					ragel_num.vint *= ragel_num.sign;
					ragel_type = TYPE_INT;
					ragel_cat  = CAT_NUMBER;
				}
				
#line 1038 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				{
#line 7 "lex/ragel_units.rl"
					ragel_num.vdouble = ragel_num.vint; }
				
#line 1044 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				{
#line 108 "lex/ragel_numeric.rl"
					ragel_num.vdouble = ragel_num.vint;}
				
#line 1050 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				{
#line 113 "lex/ragel_numeric.rl"
					ragel_num.vdouble = ragel_num.vint;}
				
#line 1056 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				goto _st53;
				_ctr114:
				{
#line 58 "lex/ragel_numeric.rl"
					
					const auto frac = double(ragel_num.ratio.num) / ragel_num.ratio.den;
					ragel_num.vdouble = ragel_num.vint + ragel_num.sign * frac;
					ragel_type = TYPE_FLOAT;
					ragel_cat  = CAT_NUMBER;
				}
				
#line 1069 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				goto _st53;
				_ctr123:
				{
#line 9 "lex/ragel_units.rl"
					ragel_cat = CAT_UNIT; }
				
#line 1077 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				goto _st53;
				_st53:
				if ( p == eof ) {
					if ( cs >= 48 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof53;
				st_case_53:
				switch( ( (*( p))) ) {
					case 37: {
						goto _st49;
					}
					case 42: {
						goto _st54;
					}
					case 72: {
						goto _st5;
					}
					case 99: {
						goto _st55;
					}
					case 100: {
						goto _st57;
					}
					case 104: {
						goto _st62;
					}
					case 109: {
						goto _st64;
					}
					case 112: {
						goto _st15;
					}
					case 114: {
						goto _st23;
					}
					case 115: {
						goto _st72;
					}
				}
				{
					goto _st0;
				}
				_ctr131:
				{
#line 27 "lex/ragel_units.rl"
					ragel_type = TYPE_PHASE;}
				
#line 1131 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				{
#line 9 "lex/ragel_units.rl"
					ragel_cat = CAT_UNIT; }
				
#line 1137 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				goto _st54;
				_st54:
				if ( p == eof ) {
					if ( cs >= 48 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof54;
				st_case_54:
				{
					goto _st0;
				}
				_ctr101:
				{
#line 30 "lex/ragel_numeric.rl"
					
					ragel_num.vint *= ragel_num.sign;
					ragel_type = TYPE_INT;
					ragel_cat  = CAT_NUMBER;
				}
				
#line 1163 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				{
#line 7 "lex/ragel_units.rl"
					ragel_num.vdouble = ragel_num.vint; }
				
#line 1169 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				goto _st55;
				_ctr115:
				{
#line 58 "lex/ragel_numeric.rl"
					
					const auto frac = double(ragel_num.ratio.num) / ragel_num.ratio.den;
					ragel_num.vdouble = ragel_num.vint + ragel_num.sign * frac;
					ragel_type = TYPE_FLOAT;
					ragel_cat  = CAT_NUMBER;
				}
				
#line 1182 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				goto _st55;
				_ctr132:
				{
#line 23 "lex/ragel_units.rl"
					ragel_type = TYPE_CENT;}
				
#line 1190 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				{
#line 9 "lex/ragel_units.rl"
					ragel_cat = CAT_UNIT; }
				
#line 1196 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				goto _st55;
				_st55:
				if ( p == eof ) {
					if ( cs >= 48 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof55;
				st_case_55:
				if ( ( (*( p))) == 101 ) {
					goto _st6;
				}
				{
					goto _st0;
				}
				_st6:
				if ( p == eof ) {
					if ( cs >= 48 )
						goto _out;
					else
						goto _pop;
				}
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
				if ( p == eof ) {
					if ( cs >= 48 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof7;
				st_case_7:
				if ( ( (*( p))) == 116 ) {
					goto _st56;
				}
				{
					goto _st0;
				}
				_ctr133:
				{
#line 23 "lex/ragel_units.rl"
					ragel_type = TYPE_CENT;}
				
#line 1255 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				{
#line 9 "lex/ragel_units.rl"
					ragel_cat = CAT_UNIT; }
				
#line 1261 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				goto _st56;
				_st56:
				if ( p == eof ) {
					if ( cs >= 48 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof56;
				st_case_56:
				{
					goto _st0;
				}
				_ctr102:
				{
#line 30 "lex/ragel_numeric.rl"
					
					ragel_num.vint *= ragel_num.sign;
					ragel_type = TYPE_INT;
					ragel_cat  = CAT_NUMBER;
				}
				
#line 1287 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				{
#line 7 "lex/ragel_units.rl"
					ragel_num.vdouble = ragel_num.vint; }
				
#line 1293 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				goto _st57;
				_ctr116:
				{
#line 58 "lex/ragel_numeric.rl"
					
					const auto frac = double(ragel_num.ratio.num) / ragel_num.ratio.den;
					ragel_num.vdouble = ragel_num.vint + ragel_num.sign * frac;
					ragel_type = TYPE_FLOAT;
					ragel_cat  = CAT_NUMBER;
				}
				
#line 1306 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				goto _st57;
				_ctr134:
				{
#line 18 "lex/ragel_units.rl"
					ragel_type = TYPE_DAY;}
				
#line 1314 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				{
#line 9 "lex/ragel_units.rl"
					ragel_cat = CAT_UNIT; }
				
#line 1320 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				goto _st57;
				_st57:
				if ( p == eof ) {
					if ( cs >= 48 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof57;
				st_case_57:
				switch( ( (*( p))) ) {
					case 97: {
						goto _st8;
					}
					case 98: {
						goto _st59;
					}
					case 101: {
						goto _st10;
					}
				}
				{
					goto _st0;
				}
				_st8:
				if ( p == eof ) {
					if ( cs >= 48 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof8;
				st_case_8:
				if ( ( (*( p))) == 121 ) {
					goto _st58;
				}
				{
					goto _st0;
				}
				_ctr136:
				{
#line 18 "lex/ragel_units.rl"
					ragel_type = TYPE_DAY;}
				
#line 1370 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				{
#line 9 "lex/ragel_units.rl"
					ragel_cat = CAT_UNIT; }
				
#line 1376 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				goto _st58;
				_st58:
				if ( p == eof ) {
					if ( cs >= 48 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof58;
				st_case_58:
				{
					goto _st0;
				}
				_ctr137:
				{
#line 17 "lex/ragel_units.rl"
					ragel_type = TYPE_DB;}
				
#line 1398 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				{
#line 9 "lex/ragel_units.rl"
					ragel_cat = CAT_UNIT; }
				
#line 1404 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				goto _st59;
				_st59:
				if ( p == eof ) {
					if ( cs >= 48 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof59;
				st_case_59:
				if ( ( (*( p))) == 102 ) {
					goto _st9;
				}
				{
					goto _st0;
				}
				_st9:
				if ( p == eof ) {
					if ( cs >= 48 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof9;
				st_case_9:
				if ( ( (*( p))) == 115 ) {
					goto _st60;
				}
				{
					goto _st0;
				}
				_ctr138:
				{
#line 17 "lex/ragel_units.rl"
					ragel_type = TYPE_DB;}
				
#line 1446 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				{
#line 9 "lex/ragel_units.rl"
					ragel_cat = CAT_UNIT; }
				
#line 1452 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				goto _st60;
				_st60:
				if ( p == eof ) {
					if ( cs >= 48 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof60;
				st_case_60:
				{
					goto _st0;
				}
				_st10:
				if ( p == eof ) {
					if ( cs >= 48 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof10;
				st_case_10:
				if ( ( (*( p))) == 103 ) {
					goto _st61;
				}
				{
					goto _st0;
				}
				_ctr139:
				{
#line 16 "lex/ragel_units.rl"
					ragel_type = TYPE_DEGREE;}
				
#line 1491 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				{
#line 9 "lex/ragel_units.rl"
					ragel_cat = CAT_UNIT; }
				
#line 1497 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				goto _st61;
				_st61:
				if ( p == eof ) {
					if ( cs >= 48 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof61;
				st_case_61:
				{
					goto _st0;
				}
				_ctr103:
				{
#line 30 "lex/ragel_numeric.rl"
					
					ragel_num.vint *= ragel_num.sign;
					ragel_type = TYPE_INT;
					ragel_cat  = CAT_NUMBER;
				}
				
#line 1523 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				{
#line 7 "lex/ragel_units.rl"
					ragel_num.vdouble = ragel_num.vint; }
				
#line 1529 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				goto _st62;
				_ctr117:
				{
#line 58 "lex/ragel_numeric.rl"
					
					const auto frac = double(ragel_num.ratio.num) / ragel_num.ratio.den;
					ragel_num.vdouble = ragel_num.vint + ragel_num.sign * frac;
					ragel_type = TYPE_FLOAT;
					ragel_cat  = CAT_NUMBER;
				}
				
#line 1542 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				goto _st62;
				_ctr140:
				{
#line 19 "lex/ragel_units.rl"
					ragel_type = TYPE_HOUR;}
				
#line 1550 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				{
#line 9 "lex/ragel_units.rl"
					ragel_cat = CAT_UNIT; }
				
#line 1556 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				goto _st62;
				_st62:
				if ( p == eof ) {
					if ( cs >= 48 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof62;
				st_case_62:
				switch( ( (*( p))) ) {
					case 111: {
						goto _st11;
					}
					case 122: {
						goto _st52;
					}
				}
				{
					goto _st0;
				}
				_st11:
				if ( p == eof ) {
					if ( cs >= 48 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof11;
				st_case_11:
				if ( ( (*( p))) == 117 ) {
					goto _st12;
				}
				{
					goto _st0;
				}
				_st12:
				if ( p == eof ) {
					if ( cs >= 48 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof12;
				st_case_12:
				if ( ( (*( p))) == 114 ) {
					goto _st63;
				}
				{
					goto _st0;
				}
				_ctr141:
				{
#line 19 "lex/ragel_units.rl"
					ragel_type = TYPE_HOUR;}
				
#line 1620 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				{
#line 9 "lex/ragel_units.rl"
					ragel_cat = CAT_UNIT; }
				
#line 1626 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				goto _st63;
				_st63:
				if ( p == eof ) {
					if ( cs >= 48 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof63;
				st_case_63:
				{
					goto _st0;
				}
				_ctr104:
				{
#line 30 "lex/ragel_numeric.rl"
					
					ragel_num.vint *= ragel_num.sign;
					ragel_type = TYPE_INT;
					ragel_cat  = CAT_NUMBER;
				}
				
#line 1652 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				{
#line 7 "lex/ragel_units.rl"
					ragel_num.vdouble = ragel_num.vint; }
				
#line 1658 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				goto _st64;
				_ctr118:
				{
#line 58 "lex/ragel_numeric.rl"
					
					const auto frac = double(ragel_num.ratio.num) / ragel_num.ratio.den;
					ragel_num.vdouble = ragel_num.vint + ragel_num.sign * frac;
					ragel_type = TYPE_FLOAT;
					ragel_cat  = CAT_NUMBER;
				}
				
#line 1671 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				goto _st64;
				_ctr142:
				{
#line 20 "lex/ragel_units.rl"
					ragel_type = TYPE_MIN;}
				
#line 1679 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				{
#line 9 "lex/ragel_units.rl"
					ragel_cat = CAT_UNIT; }
				
#line 1685 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				goto _st64;
				_st64:
				if ( p == eof ) {
					if ( cs >= 48 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof64;
				st_case_64:
				switch( ( (*( p))) ) {
					case 105: {
						goto _st13;
					}
					case 115: {
						goto _st66;
					}
				}
				{
					goto _st0;
				}
				_st13:
				if ( p == eof ) {
					if ( cs >= 48 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof13;
				st_case_13:
				if ( ( (*( p))) == 110 ) {
					goto _st65;
				}
				{
					goto _st0;
				}
				_ctr144:
				{
#line 20 "lex/ragel_units.rl"
					ragel_type = TYPE_MIN;}
				
#line 1732 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				{
#line 9 "lex/ragel_units.rl"
					ragel_cat = CAT_UNIT; }
				
#line 1738 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				goto _st65;
				_st65:
				if ( p == eof ) {
					if ( cs >= 48 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof65;
				st_case_65:
				{
					goto _st0;
				}
				_ctr145:
				{
#line 22 "lex/ragel_units.rl"
					ragel_type = TYPE_MSEC;}
				
#line 1760 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				{
#line 9 "lex/ragel_units.rl"
					ragel_cat = CAT_UNIT; }
				
#line 1766 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				goto _st66;
				_st66:
				if ( p == eof ) {
					if ( cs >= 48 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof66;
				st_case_66:
				if ( ( (*( p))) == 101 ) {
					goto _st14;
				}
				{
					goto _st0;
				}
				_st14:
				if ( p == eof ) {
					if ( cs >= 48 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof14;
				st_case_14:
				if ( ( (*( p))) == 99 ) {
					goto _st67;
				}
				{
					goto _st0;
				}
				_ctr146:
				{
#line 22 "lex/ragel_units.rl"
					ragel_type = TYPE_MSEC;}
				
#line 1808 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				{
#line 9 "lex/ragel_units.rl"
					ragel_cat = CAT_UNIT; }
				
#line 1814 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				goto _st67;
				_st67:
				if ( p == eof ) {
					if ( cs >= 48 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof67;
				st_case_67:
				{
					goto _st0;
				}
				_ctr105:
				{
#line 30 "lex/ragel_numeric.rl"
					
					ragel_num.vint *= ragel_num.sign;
					ragel_type = TYPE_INT;
					ragel_cat  = CAT_NUMBER;
				}
				
#line 1840 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				{
#line 7 "lex/ragel_units.rl"
					ragel_num.vdouble = ragel_num.vint; }
				
#line 1846 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				{
#line 113 "lex/ragel_numeric.rl"
					ragel_num.vdouble = ragel_num.vint;}
				
#line 1852 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				goto _st15;
				_ctr119:
				{
#line 58 "lex/ragel_numeric.rl"
					
					const auto frac = double(ragel_num.ratio.num) / ragel_num.ratio.den;
					ragel_num.vdouble = ragel_num.vint + ragel_num.sign * frac;
					ragel_type = TYPE_FLOAT;
					ragel_cat  = CAT_NUMBER;
				}
				
#line 1865 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				goto _st15;
				_st15:
				if ( p == eof ) {
					if ( cs >= 48 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof15;
				st_case_15:
				switch( ( (*( p))) ) {
					case 101: {
						goto _st16;
					}
					case 104: {
						goto _st20;
					}
				}
				{
					goto _st0;
				}
				_st16:
				if ( p == eof ) {
					if ( cs >= 48 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof16;
				st_case_16:
				if ( ( (*( p))) == 114 ) {
					goto _st17;
				}
				{
					goto _st0;
				}
				_st17:
				if ( p == eof ) {
					if ( cs >= 48 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof17;
				st_case_17:
				if ( ( (*( p))) == 99 ) {
					goto _st68;
				}
				{
					goto _st0;
				}
				_ctr147:
				{
#line 26 "lex/ragel_units.rl"
					ragel_type = TYPE_PERCENT;}
				
#line 1929 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				{
#line 9 "lex/ragel_units.rl"
					ragel_cat = CAT_UNIT; }
				
#line 1935 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				goto _st68;
				_st68:
				if ( p == eof ) {
					if ( cs >= 48 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof68;
				st_case_68:
				if ( ( (*( p))) == 101 ) {
					goto _st18;
				}
				{
					goto _st0;
				}
				_st18:
				if ( p == eof ) {
					if ( cs >= 48 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof18;
				st_case_18:
				if ( ( (*( p))) == 110 ) {
					goto _st19;
				}
				{
					goto _st0;
				}
				_st19:
				if ( p == eof ) {
					if ( cs >= 48 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof19;
				st_case_19:
				if ( ( (*( p))) == 116 ) {
					goto _st69;
				}
				{
					goto _st0;
				}
				_ctr148:
				{
#line 26 "lex/ragel_units.rl"
					ragel_type = TYPE_PERCENT;}
				
#line 1994 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				{
#line 9 "lex/ragel_units.rl"
					ragel_cat = CAT_UNIT; }
				
#line 2000 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				goto _st69;
				_st69:
				if ( p == eof ) {
					if ( cs >= 48 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof69;
				st_case_69:
				{
					goto _st0;
				}
				_st20:
				if ( p == eof ) {
					if ( cs >= 48 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof20;
				st_case_20:
				if ( ( (*( p))) == 97 ) {
					goto _st21;
				}
				{
					goto _st0;
				}
				_st21:
				if ( p == eof ) {
					if ( cs >= 48 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof21;
				st_case_21:
				if ( ( (*( p))) == 115 ) {
					goto _st22;
				}
				{
					goto _st0;
				}
				_st22:
				if ( p == eof ) {
					if ( cs >= 48 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof22;
				st_case_22:
				if ( ( (*( p))) == 101 ) {
					goto _st70;
				}
				{
					goto _st0;
				}
				_ctr149:
				{
#line 28 "lex/ragel_units.rl"
					ragel_type = TYPE_PHASE;}
				
#line 2073 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				{
#line 75 "lex/ragel_numeric.rl"
					
					ragel_type = TYPE_PHASE;
					ragel_cat = CAT_NUMBER;
				}
				
#line 2082 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				{
#line 73 "lex/ragel_units.rl"
					ragel_type = TYPE_PHASE;}
				
#line 2088 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				{
#line 9 "lex/ragel_units.rl"
					ragel_cat = CAT_UNIT; }
				
#line 2094 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				goto _st70;
				_st70:
				if ( p == eof ) {
					if ( cs >= 48 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof70;
				st_case_70:
				{
					goto _st0;
				}
				_ctr106:
				{
#line 30 "lex/ragel_numeric.rl"
					
					ragel_num.vint *= ragel_num.sign;
					ragel_type = TYPE_INT;
					ragel_cat  = CAT_NUMBER;
				}
				
#line 2120 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				{
#line 7 "lex/ragel_units.rl"
					ragel_num.vdouble = ragel_num.vint; }
				
#line 2126 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				goto _st23;
				_ctr120:
				{
#line 58 "lex/ragel_numeric.rl"
					
					const auto frac = double(ragel_num.ratio.num) / ragel_num.ratio.den;
					ragel_num.vdouble = ragel_num.vint + ragel_num.sign * frac;
					ragel_type = TYPE_FLOAT;
					ragel_cat  = CAT_NUMBER;
				}
				
#line 2139 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				goto _st23;
				_st23:
				if ( p == eof ) {
					if ( cs >= 48 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof23;
				st_case_23:
				if ( ( (*( p))) == 97 ) {
					goto _st24;
				}
				{
					goto _st0;
				}
				_st24:
				if ( p == eof ) {
					if ( cs >= 48 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof24;
				st_case_24:
				if ( ( (*( p))) == 100 ) {
					goto _st71;
				}
				{
					goto _st0;
				}
				_ctr150:
				{
#line 15 "lex/ragel_units.rl"
					ragel_type = TYPE_RADIAN;}
				
#line 2181 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				{
#line 9 "lex/ragel_units.rl"
					ragel_cat = CAT_UNIT; }
				
#line 2187 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				goto _st71;
				_st71:
				if ( p == eof ) {
					if ( cs >= 48 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof71;
				st_case_71:
				{
					goto _st0;
				}
				_ctr107:
				{
#line 30 "lex/ragel_numeric.rl"
					
					ragel_num.vint *= ragel_num.sign;
					ragel_type = TYPE_INT;
					ragel_cat  = CAT_NUMBER;
				}
				
#line 2213 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				{
#line 7 "lex/ragel_units.rl"
					ragel_num.vdouble = ragel_num.vint; }
				
#line 2219 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				goto _st72;
				_ctr121:
				{
#line 58 "lex/ragel_numeric.rl"
					
					const auto frac = double(ragel_num.ratio.num) / ragel_num.ratio.den;
					ragel_num.vdouble = ragel_num.vint + ragel_num.sign * frac;
					ragel_type = TYPE_FLOAT;
					ragel_cat  = CAT_NUMBER;
				}
				
#line 2232 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				goto _st72;
				_ctr151:
				{
#line 21 "lex/ragel_units.rl"
					ragel_type = TYPE_SEC;}
				
#line 2240 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				{
#line 9 "lex/ragel_units.rl"
					ragel_cat = CAT_UNIT; }
				
#line 2246 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				goto _st72;
				_st72:
				if ( p == eof ) {
					if ( cs >= 48 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof72;
				st_case_72:
				switch( ( (*( p))) ) {
					case 97: {
						goto _st73;
					}
					case 101: {
						goto _st26;
					}
				}
				{
					goto _st0;
				}
				_ctr153:
				{
#line 14 "lex/ragel_units.rl"
					ragel_type = TYPE_SAMP;}
				
#line 2276 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				{
#line 9 "lex/ragel_units.rl"
					ragel_cat = CAT_UNIT; }
				
#line 2282 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				goto _st73;
				_st73:
				if ( p == eof ) {
					if ( cs >= 48 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof73;
				st_case_73:
				if ( ( (*( p))) == 109 ) {
					goto _st25;
				}
				{
					goto _st0;
				}
				_st25:
				if ( p == eof ) {
					if ( cs >= 48 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof25;
				st_case_25:
				if ( ( (*( p))) == 112 ) {
					goto _st74;
				}
				{
					goto _st0;
				}
				_ctr154:
				{
#line 14 "lex/ragel_units.rl"
					ragel_type = TYPE_SAMP;}
				
#line 2324 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				{
#line 9 "lex/ragel_units.rl"
					ragel_cat = CAT_UNIT; }
				
#line 2330 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				goto _st74;
				_st74:
				if ( p == eof ) {
					if ( cs >= 48 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof74;
				st_case_74:
				{
					goto _st0;
				}
				_st26:
				if ( p == eof ) {
					if ( cs >= 48 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof26;
				st_case_26:
				switch( ( (*( p))) ) {
					case 99: {
						goto _st75;
					}
					case 109: {
						goto _st27;
					}
				}
				{
					goto _st0;
				}
				_ctr155:
				{
#line 21 "lex/ragel_units.rl"
					ragel_type = TYPE_SEC;}
				
#line 2374 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				{
#line 9 "lex/ragel_units.rl"
					ragel_cat = CAT_UNIT; }
				
#line 2380 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				goto _st75;
				_st75:
				if ( p == eof ) {
					if ( cs >= 48 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof75;
				st_case_75:
				{
					goto _st0;
				}
				_st27:
				if ( p == eof ) {
					if ( cs >= 48 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof27;
				st_case_27:
				if ( ( (*( p))) == 105 ) {
					goto _st76;
				}
				{
					goto _st0;
				}
				_ctr156:
				{
#line 24 "lex/ragel_units.rl"
					ragel_type = TYPE_SEMITONE;}
				
#line 2419 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				{
#line 9 "lex/ragel_units.rl"
					ragel_cat = CAT_UNIT; }
				
#line 2425 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				goto _st76;
				_st76:
				if ( p == eof ) {
					if ( cs >= 48 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof76;
				st_case_76:
				if ( ( (*( p))) == 116 ) {
					goto _st28;
				}
				{
					goto _st0;
				}
				_st28:
				if ( p == eof ) {
					if ( cs >= 48 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof28;
				st_case_28:
				if ( ( (*( p))) == 111 ) {
					goto _st29;
				}
				{
					goto _st0;
				}
				_st29:
				if ( p == eof ) {
					if ( cs >= 48 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof29;
				st_case_29:
				if ( ( (*( p))) == 110 ) {
					goto _st30;
				}
				{
					goto _st0;
				}
				_st30:
				if ( p == eof ) {
					if ( cs >= 48 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof30;
				st_case_30:
				if ( ( (*( p))) == 101 ) {
					goto _st77;
				}
				{
					goto _st0;
				}
				_ctr157:
				{
#line 24 "lex/ragel_units.rl"
					ragel_type = TYPE_SEMITONE;}
				
#line 2501 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				{
#line 9 "lex/ragel_units.rl"
					ragel_cat = CAT_UNIT; }
				
#line 2507 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				goto _st77;
				_st77:
				if ( p == eof ) {
					if ( cs >= 48 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof77;
				st_case_77:
				{
					goto _st0;
				}
				_st31:
				if ( p == eof ) {
					if ( cs >= 48 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof31;
				st_case_31:
				if ( ( (*( p))) == 48 ) {
					goto _ctr63;
				}
				if ( 49 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr64;
				}
				{
					goto _st0;
				}
				_ctr63:
				{
#line 43 "lex/ragel_numeric.rl"
					(ragel_num.ratio.den *= 10) += ((( (*( p))))-'0'); }
				
#line 2549 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				goto _st78;
				_ctr158:
				{
#line 44 "lex/ragel_numeric.rl"
					
					ragel_num.ratio.num *= ragel_num.sign;
					ragel_type = TYPE_RATIO;
					ragel_cat  = CAT_NUMBER;
				}
				
#line 2561 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				{
#line 74 "lex/ragel_units.rl"
					ragel_type = TYPE_RATIO;}
				
#line 2567 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				{
#line 9 "lex/ragel_units.rl"
					ragel_cat = CAT_UNIT; }
				
#line 2573 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				goto _st78;
				_st78:
				if ( p == eof ) {
					if ( cs >= 48 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof78;
				st_case_78:
				{
					goto _st0;
				}
				_ctr64:
				{
#line 43 "lex/ragel_numeric.rl"
					(ragel_num.ratio.den *= 10) += ((( (*( p))))-'0'); }
				
#line 2595 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				goto _st79;
				_ctr159:
				{
#line 44 "lex/ragel_numeric.rl"
					
					ragel_num.ratio.num *= ragel_num.sign;
					ragel_type = TYPE_RATIO;
					ragel_cat  = CAT_NUMBER;
				}
				
#line 2607 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				{
#line 74 "lex/ragel_units.rl"
					ragel_type = TYPE_RATIO;}
				
#line 2613 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				{
#line 9 "lex/ragel_units.rl"
					ragel_cat = CAT_UNIT; }
				
#line 2619 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				goto _st79;
				_st79:
				if ( p == eof ) {
					if ( cs >= 48 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof79;
				st_case_79:
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr64;
				}
				{
					goto _st0;
				}
				_ctr98:
				{
#line 49 "lex/ragel_units.rl"
					(smpte.hour *= 10) += ((( (*( p)))) - '0');}
				
#line 2644 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				goto _st32;
				_st32:
				if ( p == eof ) {
					if ( cs >= 48 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof32;
				st_case_32:
				if ( ( (*( p))) == 58 ) {
					goto _st33;
				}
				{
					goto _st0;
				}
				_st33:
				if ( p == eof ) {
					if ( cs >= 48 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof33;
				st_case_33:
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 53 ) {
					goto _ctr67;
				}
				{
					goto _st0;
				}
				_ctr67:
				{
#line 50 "lex/ragel_units.rl"
					smpte.min = 0; smpte.np++;}
				
#line 2686 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				{
#line 50 "lex/ragel_units.rl"
					(smpte.min *= 10) += ((( (*( p)))) - '0');}
				
#line 2692 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				goto _st34;
				_st34:
				if ( p == eof ) {
					if ( cs >= 48 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof34;
				st_case_34:
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr69;
				}
				{
					goto _st0;
				}
				_ctr69:
				{
#line 50 "lex/ragel_units.rl"
					(smpte.min *= 10) += ((( (*( p)))) - '0');}
				
#line 2717 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				goto _st80;
				_ctr160:
				{
#line 35 "lex/ragel_units.rl"
					
					ragel_type = TYPE_SMPTE;
					smpte.hour *= smpte.sign;
					smpte.min *= smpte.sign;
					smpte.sec *= smpte.sign;
					smpte.frame *= smpte.sign;
					
					if (smpte.np == 2) {
						smpte.sec = smpte.min;
						smpte.min = smpte.hour;
						smpte.hour = 0;
					}
				}
				
#line 2737 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				{
#line 9 "lex/ragel_units.rl"
					ragel_cat = CAT_UNIT; }
				
#line 2743 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				goto _st80;
				_st80:
				if ( p == eof ) {
					if ( cs >= 48 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof80;
				st_case_80:
				switch( ( (*( p))) ) {
					case 46: {
						goto _st35;
					}
					case 58: {
						goto _st36;
					}
				}
				{
					goto _st0;
				}
				_st35:
				if ( p == eof ) {
					if ( cs >= 48 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof35;
				st_case_35:
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr71;
				}
				{
					goto _st0;
				}
				_ctr71:
				{
#line 52 "lex/ragel_units.rl"
					smpte.frame = 0;}
				
#line 2790 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				{
#line 52 "lex/ragel_units.rl"
					(smpte.frame *= 10) += ((( (*( p)))) - '0');}
				
#line 2796 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				goto _st81;
				_ctr161:
				{
#line 35 "lex/ragel_units.rl"
					
					ragel_type = TYPE_SMPTE;
					smpte.hour *= smpte.sign;
					smpte.min *= smpte.sign;
					smpte.sec *= smpte.sign;
					smpte.frame *= smpte.sign;
					
					if (smpte.np == 2) {
						smpte.sec = smpte.min;
						smpte.min = smpte.hour;
						smpte.hour = 0;
					}
				}
				
#line 2816 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				{
#line 9 "lex/ragel_units.rl"
					ragel_cat = CAT_UNIT; }
				
#line 2822 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				goto _st81;
				_st81:
				if ( p == eof ) {
					if ( cs >= 48 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof81;
				st_case_81:
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr162;
				}
				{
					goto _st0;
				}
				_ctr163:
				{
#line 35 "lex/ragel_units.rl"
					
					ragel_type = TYPE_SMPTE;
					smpte.hour *= smpte.sign;
					smpte.min *= smpte.sign;
					smpte.sec *= smpte.sign;
					smpte.frame *= smpte.sign;
					
					if (smpte.np == 2) {
						smpte.sec = smpte.min;
						smpte.min = smpte.hour;
						smpte.hour = 0;
					}
				}
				
#line 2859 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				{
#line 9 "lex/ragel_units.rl"
					ragel_cat = CAT_UNIT; }
				
#line 2865 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				goto _st82;
				_ctr162:
				{
#line 52 "lex/ragel_units.rl"
					(smpte.frame *= 10) += ((( (*( p)))) - '0');}
				
#line 2873 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				goto _st82;
				_st82:
				if ( p == eof ) {
					if ( cs >= 48 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof82;
				st_case_82:
				{
					goto _st0;
				}
				_st36:
				if ( p == eof ) {
					if ( cs >= 48 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof36;
				st_case_36:
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 53 ) {
					goto _ctr73;
				}
				{
					goto _st0;
				}
				_ctr73:
				{
#line 51 "lex/ragel_units.rl"
					smpte.sec = 0; smpte.np++;}
				
#line 2912 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				{
#line 51 "lex/ragel_units.rl"
					(smpte.sec *= 10) += ((( (*( p)))) - '0');}
				
#line 2918 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				goto _st37;
				_st37:
				if ( p == eof ) {
					if ( cs >= 48 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof37;
				st_case_37:
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr75;
				}
				{
					goto _st0;
				}
				_ctr75:
				{
#line 51 "lex/ragel_units.rl"
					(smpte.sec *= 10) += ((( (*( p)))) - '0');}
				
#line 2943 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				goto _st83;
				_ctr164:
				{
#line 35 "lex/ragel_units.rl"
					
					ragel_type = TYPE_SMPTE;
					smpte.hour *= smpte.sign;
					smpte.min *= smpte.sign;
					smpte.sec *= smpte.sign;
					smpte.frame *= smpte.sign;
					
					if (smpte.np == 2) {
						smpte.sec = smpte.min;
						smpte.min = smpte.hour;
						smpte.hour = 0;
					}
				}
				
#line 2963 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				{
#line 9 "lex/ragel_units.rl"
					ragel_cat = CAT_UNIT; }
				
#line 2969 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				goto _st83;
				_st83:
				if ( p == eof ) {
					if ( cs >= 48 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof83;
				st_case_83:
				if ( ( (*( p))) == 46 ) {
					goto _st35;
				}
				{
					goto _st0;
				}
				_ctr12:
				{
#line 64 "lex/ragel_units.rl"
					pos_ = POSITION_END;}
				
#line 2994 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				{
#line 29 "lex/ragel_numeric.rl"
					(ragel_num.vint *= 10) += ((( (*( p))))-'0'); }
				
#line 3000 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				{
#line 42 "lex/ragel_numeric.rl"
					(ragel_num.ratio.num *= 10) += ((( (*( p))))-'0'); }
				
#line 3006 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				{
#line 61 "lex/ragel_units.rl"
					smpte.np = 0;}
				
#line 3012 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				{
#line 49 "lex/ragel_units.rl"
					smpte.hour = 0; smpte.np++;}
				
#line 3018 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				{
#line 49 "lex/ragel_units.rl"
					(smpte.hour *= 10) += ((( (*( p)))) - '0');}
				
#line 3024 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				goto _st84;
				_ctr16:
				{
#line 29 "lex/ragel_numeric.rl"
					(ragel_num.vint *= 10) += ((( (*( p))))-'0'); }
				
#line 3032 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				{
#line 42 "lex/ragel_numeric.rl"
					(ragel_num.ratio.num *= 10) += ((( (*( p))))-'0'); }
				
#line 3038 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				{
#line 49 "lex/ragel_units.rl"
					smpte.hour = 0; smpte.np++;}
				
#line 3044 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				{
#line 49 "lex/ragel_units.rl"
					(smpte.hour *= 10) += ((( (*( p)))) - '0');}
				
#line 3050 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				goto _st84;
				_ctr90:
				{
#line 65 "lex/ragel_units.rl"
					pos_ = POSITION_CURRENT;}
				
#line 3058 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				{
#line 29 "lex/ragel_numeric.rl"
					(ragel_num.vint *= 10) += ((( (*( p))))-'0'); }
				
#line 3064 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				{
#line 42 "lex/ragel_numeric.rl"
					(ragel_num.ratio.num *= 10) += ((( (*( p))))-'0'); }
				
#line 3070 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				{
#line 61 "lex/ragel_units.rl"
					smpte.np = 0;}
				
#line 3076 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				{
#line 49 "lex/ragel_units.rl"
					smpte.hour = 0; smpte.np++;}
				
#line 3082 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				{
#line 49 "lex/ragel_units.rl"
					(smpte.hour *= 10) += ((( (*( p)))) - '0');}
				
#line 3088 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				goto _st84;
				_ctr165:
				{
#line 30 "lex/ragel_numeric.rl"
					
					ragel_num.vint *= ragel_num.sign;
					ragel_type = TYPE_INT;
					ragel_cat  = CAT_NUMBER;
				}
				
#line 3100 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				{
#line 7 "lex/ragel_units.rl"
					ragel_num.vdouble = ragel_num.vint; }
				
#line 3106 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				{
#line 9 "lex/ragel_units.rl"
					ragel_cat = CAT_UNIT; }
				
#line 3112 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				goto _st84;
				_st84:
				if ( p == eof ) {
					if ( cs >= 48 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof84;
				st_case_84:
				switch( ( (*( p))) ) {
					case 37: {
						goto _ctr95;
					}
					case 42: {
						goto _ctr96;
					}
					case 46: {
						goto _ctr97;
					}
					case 47: {
						goto _st31;
					}
					case 72: {
						goto _ctr99;
					}
					case 95: {
						goto _ctr100;
					}
					case 99: {
						goto _ctr101;
					}
					case 100: {
						goto _ctr102;
					}
					case 104: {
						goto _ctr103;
					}
					case 109: {
						goto _ctr104;
					}
					case 112: {
						goto _ctr105;
					}
					case 114: {
						goto _ctr106;
					}
					case 115: {
						goto _ctr107;
					}
				}
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr166;
				}
				{
					goto _st0;
				}
				_ctr167:
				{
#line 30 "lex/ragel_numeric.rl"
					
					ragel_num.vint *= ragel_num.sign;
					ragel_type = TYPE_INT;
					ragel_cat  = CAT_NUMBER;
				}
				
#line 3182 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				{
#line 7 "lex/ragel_units.rl"
					ragel_num.vdouble = ragel_num.vint; }
				
#line 3188 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				{
#line 9 "lex/ragel_units.rl"
					ragel_cat = CAT_UNIT; }
				
#line 3194 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				goto _st85;
				_ctr166:
				{
#line 29 "lex/ragel_numeric.rl"
					(ragel_num.vint *= 10) += ((( (*( p))))-'0'); }
				
#line 3202 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				{
#line 42 "lex/ragel_numeric.rl"
					(ragel_num.ratio.num *= 10) += ((( (*( p))))-'0'); }
				
#line 3208 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				{
#line 49 "lex/ragel_units.rl"
					(smpte.hour *= 10) += ((( (*( p)))) - '0');}
				
#line 3214 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				goto _st85;
				_st85:
				if ( p == eof ) {
					if ( cs >= 48 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof85;
				st_case_85:
				switch( ( (*( p))) ) {
					case 37: {
						goto _ctr95;
					}
					case 42: {
						goto _ctr96;
					}
					case 46: {
						goto _ctr97;
					}
					case 47: {
						goto _st31;
					}
					case 58: {
						goto _st33;
					}
					case 72: {
						goto _ctr99;
					}
					case 95: {
						goto _ctr100;
					}
					case 99: {
						goto _ctr101;
					}
					case 100: {
						goto _ctr102;
					}
					case 104: {
						goto _ctr103;
					}
					case 109: {
						goto _ctr104;
					}
					case 112: {
						goto _ctr105;
					}
					case 114: {
						goto _ctr106;
					}
					case 115: {
						goto _ctr107;
					}
				}
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr17;
				}
				{
					goto _st0;
				}
				_ctr13:
				{
#line 64 "lex/ragel_units.rl"
					pos_ = POSITION_END;}
				
#line 3283 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				{
#line 29 "lex/ragel_numeric.rl"
					(ragel_num.vint *= 10) += ((( (*( p))))-'0'); }
				
#line 3289 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				{
#line 42 "lex/ragel_numeric.rl"
					(ragel_num.ratio.num *= 10) += ((( (*( p))))-'0'); }
				
#line 3295 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				goto _st86;
				_ctr17:
				{
#line 29 "lex/ragel_numeric.rl"
					(ragel_num.vint *= 10) += ((( (*( p))))-'0'); }
				
#line 3303 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				{
#line 42 "lex/ragel_numeric.rl"
					(ragel_num.ratio.num *= 10) += ((( (*( p))))-'0'); }
				
#line 3309 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				goto _st86;
				_ctr91:
				{
#line 65 "lex/ragel_units.rl"
					pos_ = POSITION_CURRENT;}
				
#line 3317 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				{
#line 29 "lex/ragel_numeric.rl"
					(ragel_num.vint *= 10) += ((( (*( p))))-'0'); }
				
#line 3323 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				{
#line 42 "lex/ragel_numeric.rl"
					(ragel_num.ratio.num *= 10) += ((( (*( p))))-'0'); }
				
#line 3329 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				goto _st86;
				_ctr168:
				{
#line 30 "lex/ragel_numeric.rl"
					
					ragel_num.vint *= ragel_num.sign;
					ragel_type = TYPE_INT;
					ragel_cat  = CAT_NUMBER;
				}
				
#line 3341 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				{
#line 7 "lex/ragel_units.rl"
					ragel_num.vdouble = ragel_num.vint; }
				
#line 3347 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				{
#line 9 "lex/ragel_units.rl"
					ragel_cat = CAT_UNIT; }
				
#line 3353 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				goto _st86;
				_st86:
				if ( p == eof ) {
					if ( cs >= 48 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof86;
				st_case_86:
				switch( ( (*( p))) ) {
					case 37: {
						goto _ctr95;
					}
					case 42: {
						goto _ctr96;
					}
					case 46: {
						goto _ctr97;
					}
					case 47: {
						goto _st31;
					}
					case 72: {
						goto _ctr99;
					}
					case 95: {
						goto _ctr100;
					}
					case 99: {
						goto _ctr101;
					}
					case 100: {
						goto _ctr102;
					}
					case 104: {
						goto _ctr103;
					}
					case 109: {
						goto _ctr104;
					}
					case 112: {
						goto _ctr105;
					}
					case 114: {
						goto _ctr106;
					}
					case 115: {
						goto _ctr107;
					}
				}
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr17;
				}
				{
					goto _st0;
				}
				_ctr4:
				{
#line 8 "lex/ragel_units.rl"
					ragel_type = TYPE_UNKNOWN; ragel_cat = CAT_UNKNOWN; }
				
#line 3419 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				{
#line 70 "lex/ragel_units.rl"
					pos_ = POSITION_ABS;}
				
#line 3425 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				{
#line 29 "lex/ragel_numeric.rl"
					(ragel_num.vint *= 10) += ((( (*( p))))-'0'); }
				
#line 3431 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				{
#line 42 "lex/ragel_numeric.rl"
					(ragel_num.ratio.num *= 10) += ((( (*( p))))-'0'); }
				
#line 3437 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				{
#line 61 "lex/ragel_units.rl"
					smpte.np = 0;}
				
#line 3443 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				{
#line 49 "lex/ragel_units.rl"
					smpte.hour = 0; smpte.np++;}
				
#line 3449 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				{
#line 49 "lex/ragel_units.rl"
					(smpte.hour *= 10) += ((( (*( p)))) - '0');}
				
#line 3455 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				{
#line 9 "lex/ragel_music.rl"
					bpm.dur_num = 1; bpm.dur_den = 4; }
				
#line 3461 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				{
#line 6 "lex/ragel_music.rl"
					(bpm.ival *= 10) += ((( (*( p)))) - '0'); }
				
#line 3467 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				goto _st87;
				_ctr169:
				{
#line 30 "lex/ragel_numeric.rl"
					
					ragel_num.vint *= ragel_num.sign;
					ragel_type = TYPE_INT;
					ragel_cat  = CAT_NUMBER;
				}
				
#line 3479 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				{
#line 7 "lex/ragel_units.rl"
					ragel_num.vdouble = ragel_num.vint; }
				
#line 3485 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				{
#line 10 "lex/ragel_music.rl"
					ragel_cat = CAT_UNIT; ragel_type = TYPE_BPM; }
				
#line 3491 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				{
#line 9 "lex/ragel_units.rl"
					ragel_cat = CAT_UNIT; }
				
#line 3497 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				goto _st87;
				_st87:
				if ( p == eof ) {
					if ( cs >= 48 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof87;
				st_case_87:
				switch( ( (*( p))) ) {
					case 37: {
						goto _ctr95;
					}
					case 42: {
						goto _ctr96;
					}
					case 46: {
						goto _ctr170;
					}
					case 47: {
						goto _st31;
					}
					case 72: {
						goto _ctr99;
					}
					case 95: {
						goto _ctr171;
					}
					case 98: {
						goto _st39;
					}
					case 99: {
						goto _ctr101;
					}
					case 100: {
						goto _ctr102;
					}
					case 104: {
						goto _ctr103;
					}
					case 109: {
						goto _ctr104;
					}
					case 112: {
						goto _ctr105;
					}
					case 114: {
						goto _ctr106;
					}
					case 115: {
						goto _ctr107;
					}
					case 124: {
						goto _st41;
					}
				}
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr98;
				}
				{
					goto _st0;
				}
				_ctr170:
				{
#line 30 "lex/ragel_numeric.rl"
					
					ragel_num.vint *= ragel_num.sign;
					ragel_type = TYPE_INT;
					ragel_cat  = CAT_NUMBER;
				}
				
#line 3573 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				{
#line 50 "lex/ragel_numeric.rl"
					
					ragel_num.ratio.num = 0;
					ragel_num.ratio.den = 1;
				}
				
#line 3582 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				goto _st38;
				_st38:
				if ( p == eof ) {
					if ( cs >= 48 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof38;
				st_case_38:
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr77;
				}
				{
					goto _st0;
				}
				_ctr77:
				{
#line 54 "lex/ragel_numeric.rl"
					
					(ragel_num.ratio.num *= 10) += ((( (*( p)))) - '0');
					ragel_num.ratio.den *= 10;
				}
				
#line 3610 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				{
#line 7 "lex/ragel_music.rl"
					(bpm.fnum *= 10) += ((( (*( p)))) - '0'); bpm.fden *= 10; }
				
#line 3616 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				goto _st88;
				_ctr172:
				{
#line 58 "lex/ragel_numeric.rl"
					
					const auto frac = double(ragel_num.ratio.num) / ragel_num.ratio.den;
					ragel_num.vdouble = ragel_num.vint + ragel_num.sign * frac;
					ragel_type = TYPE_FLOAT;
					ragel_cat  = CAT_NUMBER;
				}
				
#line 3629 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				{
#line 10 "lex/ragel_music.rl"
					ragel_cat = CAT_UNIT; ragel_type = TYPE_BPM; }
				
#line 3635 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				{
#line 9 "lex/ragel_units.rl"
					ragel_cat = CAT_UNIT; }
				
#line 3641 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				goto _st88;
				_st88:
				if ( p == eof ) {
					if ( cs >= 48 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof88;
				st_case_88:
				switch( ( (*( p))) ) {
					case 37: {
						goto _ctr111;
					}
					case 42: {
						goto _ctr112;
					}
					case 72: {
						goto _ctr113;
					}
					case 95: {
						goto _ctr173;
					}
					case 98: {
						goto _st39;
					}
					case 99: {
						goto _ctr115;
					}
					case 100: {
						goto _ctr116;
					}
					case 104: {
						goto _ctr117;
					}
					case 109: {
						goto _ctr118;
					}
					case 112: {
						goto _ctr119;
					}
					case 114: {
						goto _ctr120;
					}
					case 115: {
						goto _ctr121;
					}
					case 124: {
						goto _st41;
					}
				}
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr77;
				}
				{
					goto _st0;
				}
				_ctr171:
				{
#line 30 "lex/ragel_numeric.rl"
					
					ragel_num.vint *= ragel_num.sign;
					ragel_type = TYPE_INT;
					ragel_cat  = CAT_NUMBER;
				}
				
#line 3711 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				{
#line 7 "lex/ragel_units.rl"
					ragel_num.vdouble = ragel_num.vint; }
				
#line 3717 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				{
#line 108 "lex/ragel_numeric.rl"
					ragel_num.vdouble = ragel_num.vint;}
				
#line 3723 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				{
#line 113 "lex/ragel_numeric.rl"
					ragel_num.vdouble = ragel_num.vint;}
				
#line 3729 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				goto _st89;
				_ctr173:
				{
#line 58 "lex/ragel_numeric.rl"
					
					const auto frac = double(ragel_num.ratio.num) / ragel_num.ratio.den;
					ragel_num.vdouble = ragel_num.vint + ragel_num.sign * frac;
					ragel_type = TYPE_FLOAT;
					ragel_cat  = CAT_NUMBER;
				}
				
#line 3742 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				goto _st89;
				_ctr174:
				{
#line 10 "lex/ragel_music.rl"
					ragel_cat = CAT_UNIT; ragel_type = TYPE_BPM; }
				
#line 3750 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				{
#line 9 "lex/ragel_units.rl"
					ragel_cat = CAT_UNIT; }
				
#line 3756 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				goto _st89;
				_st89:
				if ( p == eof ) {
					if ( cs >= 48 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof89;
				st_case_89:
				switch( ( (*( p))) ) {
					case 37: {
						goto _st49;
					}
					case 42: {
						goto _st54;
					}
					case 72: {
						goto _st5;
					}
					case 98: {
						goto _st39;
					}
					case 99: {
						goto _st55;
					}
					case 100: {
						goto _st57;
					}
					case 104: {
						goto _st62;
					}
					case 109: {
						goto _st64;
					}
					case 112: {
						goto _st15;
					}
					case 114: {
						goto _st23;
					}
					case 115: {
						goto _st72;
					}
				}
				{
					goto _st0;
				}
				_ctr180:
				{
#line 24 "lex/ragel_music.rl"
					bpm.dur_num = 1;}
				
#line 3813 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				goto _st39;
				_st39:
				if ( p == eof ) {
					if ( cs >= 48 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof39;
				st_case_39:
				if ( ( (*( p))) == 112 ) {
					goto _st40;
				}
				{
					goto _st0;
				}
				_st40:
				if ( p == eof ) {
					if ( cs >= 48 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof40;
				st_case_40:
				if ( ( (*( p))) == 109 ) {
					goto _st90;
				}
				{
					goto _st0;
				}
				_ctr175:
				{
#line 10 "lex/ragel_music.rl"
					ragel_cat = CAT_UNIT; ragel_type = TYPE_BPM; }
				
#line 3855 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				{
#line 9 "lex/ragel_units.rl"
					ragel_cat = CAT_UNIT; }
				
#line 3861 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				goto _st90;
				_st90:
				if ( p == eof ) {
					if ( cs >= 48 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof90;
				st_case_90:
				{
					goto _st0;
				}
				_st41:
				if ( p == eof ) {
					if ( cs >= 48 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof41;
				st_case_41:
				if ( 49 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr82;
				}
				{
					goto _st0;
				}
				_ctr82:
				{
#line 11 "lex/ragel_music.rl"
					bpm.dur_num = 0; }
				
#line 3900 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				{
#line 12 "lex/ragel_music.rl"
					(bpm.dur_num *= 10) += ((( (*( p)))) - '0'); }
				
#line 3906 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				{
#line 13 "lex/ragel_music.rl"
					bpm.dur_den = 0;}
				
#line 3912 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				{
#line 14 "lex/ragel_music.rl"
					(bpm.dur_den *= 10) += ((( (*( p)))) - '0'); }
				
#line 3918 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				goto _st91;
				_ctr176:
				{
#line 24 "lex/ragel_music.rl"
					bpm.dur_num = 1;}
				
#line 3926 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				{
#line 10 "lex/ragel_music.rl"
					ragel_cat = CAT_UNIT; ragel_type = TYPE_BPM; }
				
#line 3932 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				{
#line 9 "lex/ragel_units.rl"
					ragel_cat = CAT_UNIT; }
				
#line 3938 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				goto _st91;
				_ctr178:
				{
#line 12 "lex/ragel_music.rl"
					(bpm.dur_num *= 10) += ((( (*( p)))) - '0'); }
				
#line 3946 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				{
#line 14 "lex/ragel_music.rl"
					(bpm.dur_den *= 10) += ((( (*( p)))) - '0'); }
				
#line 3952 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				goto _st91;
				_st91:
				if ( p == eof ) {
					if ( cs >= 48 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof91;
				st_case_91:
				switch( ( (*( p))) ) {
					case 46: {
						goto _ctr177;
					}
					case 47: {
						goto _st42;
					}
					case 95: {
						goto _ctr179;
					}
					case 98: {
						goto _ctr180;
					}
				}
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr178;
				}
				{
					goto _st0;
				}
				_ctr181:
				{
#line 10 "lex/ragel_music.rl"
					ragel_cat = CAT_UNIT; ragel_type = TYPE_BPM; }
				
#line 3991 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				{
#line 9 "lex/ragel_units.rl"
					ragel_cat = CAT_UNIT; }
				
#line 3997 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				goto _st92;
				_ctr177:
				{
#line 24 "lex/ragel_music.rl"
					bpm.dur_num = 1;}
				
#line 4005 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				{
#line 8 "lex/ragel_music.rl"
					bpm.dur_num *= 3; bpm.dur_den *= 2; }
				
#line 4011 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				goto _st92;
				_ctr185:
				{
#line 8 "lex/ragel_music.rl"
					bpm.dur_num *= 3; bpm.dur_den *= 2; }
				
#line 4019 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				goto _st92;
				_st92:
				if ( p == eof ) {
					if ( cs >= 48 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof92;
				st_case_92:
				switch( ( (*( p))) ) {
					case 95: {
						goto _st93;
					}
					case 98: {
						goto _st39;
					}
				}
				{
					goto _st0;
				}
				_ctr183:
				{
#line 10 "lex/ragel_music.rl"
					ragel_cat = CAT_UNIT; ragel_type = TYPE_BPM; }
				
#line 4049 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				{
#line 9 "lex/ragel_units.rl"
					ragel_cat = CAT_UNIT; }
				
#line 4055 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				goto _st93;
				_ctr179:
				{
#line 24 "lex/ragel_music.rl"
					bpm.dur_num = 1;}
				
#line 4063 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				goto _st93;
				_st93:
				if ( p == eof ) {
					if ( cs >= 48 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof93;
				st_case_93:
				if ( ( (*( p))) == 98 ) {
					goto _st39;
				}
				{
					goto _st0;
				}
				_st42:
				if ( p == eof ) {
					if ( cs >= 48 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof42;
				st_case_42:
				if ( 49 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr84;
				}
				{
					goto _st0;
				}
				_ctr84:
				{
#line 13 "lex/ragel_music.rl"
					bpm.dur_den = 0;}
				
#line 4105 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				{
#line 14 "lex/ragel_music.rl"
					(bpm.dur_den *= 10) += ((( (*( p)))) - '0'); }
				
#line 4111 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				goto _st94;
				_ctr184:
				{
#line 10 "lex/ragel_music.rl"
					ragel_cat = CAT_UNIT; ragel_type = TYPE_BPM; }
				
#line 4119 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				{
#line 9 "lex/ragel_units.rl"
					ragel_cat = CAT_UNIT; }
				
#line 4125 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				goto _st94;
				_ctr186:
				{
#line 14 "lex/ragel_music.rl"
					(bpm.dur_den *= 10) += ((( (*( p)))) - '0'); }
				
#line 4133 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				goto _st94;
				_st94:
				if ( p == eof ) {
					if ( cs >= 48 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof94;
				st_case_94:
				switch( ( (*( p))) ) {
					case 46: {
						goto _ctr185;
					}
					case 95: {
						goto _st93;
					}
					case 98: {
						goto _st39;
					}
				}
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr186;
				}
				{
					goto _st0;
				}
				_ctr5:
				{
#line 8 "lex/ragel_units.rl"
					ragel_type = TYPE_UNKNOWN; ragel_cat = CAT_UNKNOWN; }
				
#line 4169 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				{
#line 70 "lex/ragel_units.rl"
					pos_ = POSITION_ABS;}
				
#line 4175 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				{
#line 29 "lex/ragel_numeric.rl"
					(ragel_num.vint *= 10) += ((( (*( p))))-'0'); }
				
#line 4181 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				{
#line 42 "lex/ragel_numeric.rl"
					(ragel_num.ratio.num *= 10) += ((( (*( p))))-'0'); }
				
#line 4187 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				{
#line 61 "lex/ragel_units.rl"
					smpte.np = 0;}
				
#line 4193 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				{
#line 49 "lex/ragel_units.rl"
					smpte.hour = 0; smpte.np++;}
				
#line 4199 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				{
#line 49 "lex/ragel_units.rl"
					(smpte.hour *= 10) += ((( (*( p)))) - '0');}
				
#line 4205 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				{
#line 9 "lex/ragel_music.rl"
					bpm.dur_num = 1; bpm.dur_den = 4; }
				
#line 4211 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				{
#line 6 "lex/ragel_music.rl"
					(bpm.ival *= 10) += ((( (*( p)))) - '0'); }
				
#line 4217 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				goto _st95;
				_ctr187:
				{
#line 30 "lex/ragel_numeric.rl"
					
					ragel_num.vint *= ragel_num.sign;
					ragel_type = TYPE_INT;
					ragel_cat  = CAT_NUMBER;
				}
				
#line 4229 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				{
#line 7 "lex/ragel_units.rl"
					ragel_num.vdouble = ragel_num.vint; }
				
#line 4235 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				{
#line 10 "lex/ragel_music.rl"
					ragel_cat = CAT_UNIT; ragel_type = TYPE_BPM; }
				
#line 4241 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				{
#line 9 "lex/ragel_units.rl"
					ragel_cat = CAT_UNIT; }
				
#line 4247 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				goto _st95;
				_st95:
				if ( p == eof ) {
					if ( cs >= 48 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof95;
				st_case_95:
				switch( ( (*( p))) ) {
					case 37: {
						goto _ctr95;
					}
					case 42: {
						goto _ctr96;
					}
					case 46: {
						goto _ctr170;
					}
					case 47: {
						goto _st31;
					}
					case 72: {
						goto _ctr99;
					}
					case 95: {
						goto _ctr171;
					}
					case 98: {
						goto _st39;
					}
					case 99: {
						goto _ctr101;
					}
					case 100: {
						goto _ctr102;
					}
					case 104: {
						goto _ctr103;
					}
					case 109: {
						goto _ctr104;
					}
					case 112: {
						goto _ctr105;
					}
					case 114: {
						goto _ctr106;
					}
					case 115: {
						goto _ctr107;
					}
					case 124: {
						goto _st41;
					}
				}
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr188;
				}
				{
					goto _st0;
				}
				_ctr189:
				{
#line 30 "lex/ragel_numeric.rl"
					
					ragel_num.vint *= ragel_num.sign;
					ragel_type = TYPE_INT;
					ragel_cat  = CAT_NUMBER;
				}
				
#line 4323 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				{
#line 7 "lex/ragel_units.rl"
					ragel_num.vdouble = ragel_num.vint; }
				
#line 4329 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				{
#line 10 "lex/ragel_music.rl"
					ragel_cat = CAT_UNIT; ragel_type = TYPE_BPM; }
				
#line 4335 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				{
#line 9 "lex/ragel_units.rl"
					ragel_cat = CAT_UNIT; }
				
#line 4341 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				goto _st96;
				_ctr188:
				{
#line 29 "lex/ragel_numeric.rl"
					(ragel_num.vint *= 10) += ((( (*( p))))-'0'); }
				
#line 4349 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				{
#line 42 "lex/ragel_numeric.rl"
					(ragel_num.ratio.num *= 10) += ((( (*( p))))-'0'); }
				
#line 4355 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				{
#line 49 "lex/ragel_units.rl"
					(smpte.hour *= 10) += ((( (*( p)))) - '0');}
				
#line 4361 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				{
#line 6 "lex/ragel_music.rl"
					(bpm.ival *= 10) += ((( (*( p)))) - '0'); }
				
#line 4367 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				goto _st96;
				_st96:
				if ( p == eof ) {
					if ( cs >= 48 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof96;
				st_case_96:
				switch( ( (*( p))) ) {
					case 37: {
						goto _ctr95;
					}
					case 42: {
						goto _ctr96;
					}
					case 46: {
						goto _ctr170;
					}
					case 47: {
						goto _st31;
					}
					case 58: {
						goto _st33;
					}
					case 72: {
						goto _ctr99;
					}
					case 95: {
						goto _ctr171;
					}
					case 98: {
						goto _st39;
					}
					case 99: {
						goto _ctr101;
					}
					case 100: {
						goto _ctr102;
					}
					case 104: {
						goto _ctr103;
					}
					case 109: {
						goto _ctr104;
					}
					case 112: {
						goto _ctr105;
					}
					case 114: {
						goto _ctr106;
					}
					case 115: {
						goto _ctr107;
					}
					case 124: {
						goto _st41;
					}
				}
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr190;
				}
				{
					goto _st0;
				}
				_ctr6:
				{
#line 8 "lex/ragel_units.rl"
					ragel_type = TYPE_UNKNOWN; ragel_cat = CAT_UNKNOWN; }
				
#line 4442 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				{
#line 70 "lex/ragel_units.rl"
					pos_ = POSITION_ABS;}
				
#line 4448 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				{
#line 29 "lex/ragel_numeric.rl"
					(ragel_num.vint *= 10) += ((( (*( p))))-'0'); }
				
#line 4454 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				{
#line 42 "lex/ragel_numeric.rl"
					(ragel_num.ratio.num *= 10) += ((( (*( p))))-'0'); }
				
#line 4460 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				{
#line 9 "lex/ragel_music.rl"
					bpm.dur_num = 1; bpm.dur_den = 4; }
				
#line 4466 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				{
#line 6 "lex/ragel_music.rl"
					(bpm.ival *= 10) += ((( (*( p)))) - '0'); }
				
#line 4472 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				goto _st97;
				_ctr191:
				{
#line 30 "lex/ragel_numeric.rl"
					
					ragel_num.vint *= ragel_num.sign;
					ragel_type = TYPE_INT;
					ragel_cat  = CAT_NUMBER;
				}
				
#line 4484 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				{
#line 7 "lex/ragel_units.rl"
					ragel_num.vdouble = ragel_num.vint; }
				
#line 4490 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				{
#line 10 "lex/ragel_music.rl"
					ragel_cat = CAT_UNIT; ragel_type = TYPE_BPM; }
				
#line 4496 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				{
#line 9 "lex/ragel_units.rl"
					ragel_cat = CAT_UNIT; }
				
#line 4502 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				goto _st97;
				_ctr190:
				{
#line 29 "lex/ragel_numeric.rl"
					(ragel_num.vint *= 10) += ((( (*( p))))-'0'); }
				
#line 4510 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				{
#line 42 "lex/ragel_numeric.rl"
					(ragel_num.ratio.num *= 10) += ((( (*( p))))-'0'); }
				
#line 4516 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				{
#line 6 "lex/ragel_music.rl"
					(bpm.ival *= 10) += ((( (*( p)))) - '0'); }
				
#line 4522 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				goto _st97;
				_st97:
				if ( p == eof ) {
					if ( cs >= 48 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof97;
				st_case_97:
				switch( ( (*( p))) ) {
					case 37: {
						goto _ctr95;
					}
					case 42: {
						goto _ctr96;
					}
					case 46: {
						goto _ctr170;
					}
					case 47: {
						goto _st31;
					}
					case 72: {
						goto _ctr99;
					}
					case 95: {
						goto _ctr171;
					}
					case 98: {
						goto _st39;
					}
					case 99: {
						goto _ctr101;
					}
					case 100: {
						goto _ctr102;
					}
					case 104: {
						goto _ctr103;
					}
					case 109: {
						goto _ctr104;
					}
					case 112: {
						goto _ctr105;
					}
					case 114: {
						goto _ctr106;
					}
					case 115: {
						goto _ctr107;
					}
					case 124: {
						goto _st41;
					}
				}
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr190;
				}
				{
					goto _st0;
				}
				_ctr7:
				{
#line 8 "lex/ragel_units.rl"
					ragel_type = TYPE_UNKNOWN; ragel_cat = CAT_UNKNOWN; }
				
#line 4594 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				{
#line 70 "lex/ragel_units.rl"
					pos_ = POSITION_ABS;}
				
#line 4600 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				goto _st43;
				_st43:
				if ( p == eof ) {
					if ( cs >= 48 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof43;
				st_case_43:
				if ( ( (*( p))) == 117 ) {
					goto _st44;
				}
				{
					goto _st0;
				}
				_st44:
				if ( p == eof ) {
					if ( cs >= 48 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof44;
				st_case_44:
				if ( ( (*( p))) == 114 ) {
					goto _st45;
				}
				{
					goto _st0;
				}
				_st45:
				if ( p == eof ) {
					if ( cs >= 48 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof45;
				st_case_45:
				switch( ( (*( p))) ) {
					case 43: {
						goto _ctr88;
					}
					case 45: {
						goto _ctr88;
					}
					case 48: {
						goto _ctr89;
					}
				}
				if ( ( (*( p))) > 53 ) {
					if ( ( (*( p))) <= 57 ) {
						goto _ctr91;
					}
				} else if ( ( (*( p))) >= 49 ) {
					goto _ctr90;
				}
				{
					goto _st0;
				}
				_ctr8:
				{
#line 8 "lex/ragel_units.rl"
					ragel_type = TYPE_UNKNOWN; ragel_cat = CAT_UNKNOWN; }
				
#line 4674 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				{
#line 70 "lex/ragel_units.rl"
					pos_ = POSITION_ABS;}
				
#line 4680 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				goto _st46;
				_st46:
				if ( p == eof ) {
					if ( cs >= 48 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof46;
				st_case_46:
				if ( ( (*( p))) == 110 ) {
					goto _st47;
				}
				{
					goto _st0;
				}
				_st47:
				if ( p == eof ) {
					if ( cs >= 48 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof47;
				st_case_47:
				if ( ( (*( p))) == 100 ) {
					goto _st2;
				}
				{
					goto _st0;
				}
				st_out:
				_test_eof1: cs = 1; goto _test_eof; 
				_test_eof2: cs = 2; goto _test_eof; 
				_test_eof3: cs = 3; goto _test_eof; 
				_test_eof48: cs = 48; goto _test_eof; 
				_test_eof49: cs = 49; goto _test_eof; 
				_test_eof50: cs = 50; goto _test_eof; 
				_test_eof4: cs = 4; goto _test_eof; 
				_test_eof51: cs = 51; goto _test_eof; 
				_test_eof5: cs = 5; goto _test_eof; 
				_test_eof52: cs = 52; goto _test_eof; 
				_test_eof53: cs = 53; goto _test_eof; 
				_test_eof54: cs = 54; goto _test_eof; 
				_test_eof55: cs = 55; goto _test_eof; 
				_test_eof6: cs = 6; goto _test_eof; 
				_test_eof7: cs = 7; goto _test_eof; 
				_test_eof56: cs = 56; goto _test_eof; 
				_test_eof57: cs = 57; goto _test_eof; 
				_test_eof8: cs = 8; goto _test_eof; 
				_test_eof58: cs = 58; goto _test_eof; 
				_test_eof59: cs = 59; goto _test_eof; 
				_test_eof9: cs = 9; goto _test_eof; 
				_test_eof60: cs = 60; goto _test_eof; 
				_test_eof10: cs = 10; goto _test_eof; 
				_test_eof61: cs = 61; goto _test_eof; 
				_test_eof62: cs = 62; goto _test_eof; 
				_test_eof11: cs = 11; goto _test_eof; 
				_test_eof12: cs = 12; goto _test_eof; 
				_test_eof63: cs = 63; goto _test_eof; 
				_test_eof64: cs = 64; goto _test_eof; 
				_test_eof13: cs = 13; goto _test_eof; 
				_test_eof65: cs = 65; goto _test_eof; 
				_test_eof66: cs = 66; goto _test_eof; 
				_test_eof14: cs = 14; goto _test_eof; 
				_test_eof67: cs = 67; goto _test_eof; 
				_test_eof15: cs = 15; goto _test_eof; 
				_test_eof16: cs = 16; goto _test_eof; 
				_test_eof17: cs = 17; goto _test_eof; 
				_test_eof68: cs = 68; goto _test_eof; 
				_test_eof18: cs = 18; goto _test_eof; 
				_test_eof19: cs = 19; goto _test_eof; 
				_test_eof69: cs = 69; goto _test_eof; 
				_test_eof20: cs = 20; goto _test_eof; 
				_test_eof21: cs = 21; goto _test_eof; 
				_test_eof22: cs = 22; goto _test_eof; 
				_test_eof70: cs = 70; goto _test_eof; 
				_test_eof23: cs = 23; goto _test_eof; 
				_test_eof24: cs = 24; goto _test_eof; 
				_test_eof71: cs = 71; goto _test_eof; 
				_test_eof72: cs = 72; goto _test_eof; 
				_test_eof73: cs = 73; goto _test_eof; 
				_test_eof25: cs = 25; goto _test_eof; 
				_test_eof74: cs = 74; goto _test_eof; 
				_test_eof26: cs = 26; goto _test_eof; 
				_test_eof75: cs = 75; goto _test_eof; 
				_test_eof27: cs = 27; goto _test_eof; 
				_test_eof76: cs = 76; goto _test_eof; 
				_test_eof28: cs = 28; goto _test_eof; 
				_test_eof29: cs = 29; goto _test_eof; 
				_test_eof30: cs = 30; goto _test_eof; 
				_test_eof77: cs = 77; goto _test_eof; 
				_test_eof31: cs = 31; goto _test_eof; 
				_test_eof78: cs = 78; goto _test_eof; 
				_test_eof79: cs = 79; goto _test_eof; 
				_test_eof32: cs = 32; goto _test_eof; 
				_test_eof33: cs = 33; goto _test_eof; 
				_test_eof34: cs = 34; goto _test_eof; 
				_test_eof80: cs = 80; goto _test_eof; 
				_test_eof35: cs = 35; goto _test_eof; 
				_test_eof81: cs = 81; goto _test_eof; 
				_test_eof82: cs = 82; goto _test_eof; 
				_test_eof36: cs = 36; goto _test_eof; 
				_test_eof37: cs = 37; goto _test_eof; 
				_test_eof83: cs = 83; goto _test_eof; 
				_test_eof84: cs = 84; goto _test_eof; 
				_test_eof85: cs = 85; goto _test_eof; 
				_test_eof86: cs = 86; goto _test_eof; 
				_test_eof87: cs = 87; goto _test_eof; 
				_test_eof38: cs = 38; goto _test_eof; 
				_test_eof88: cs = 88; goto _test_eof; 
				_test_eof89: cs = 89; goto _test_eof; 
				_test_eof39: cs = 39; goto _test_eof; 
				_test_eof40: cs = 40; goto _test_eof; 
				_test_eof90: cs = 90; goto _test_eof; 
				_test_eof41: cs = 41; goto _test_eof; 
				_test_eof91: cs = 91; goto _test_eof; 
				_test_eof92: cs = 92; goto _test_eof; 
				_test_eof93: cs = 93; goto _test_eof; 
				_test_eof42: cs = 42; goto _test_eof; 
				_test_eof94: cs = 94; goto _test_eof; 
				_test_eof95: cs = 95; goto _test_eof; 
				_test_eof96: cs = 96; goto _test_eof; 
				_test_eof97: cs = 97; goto _test_eof; 
				_test_eof43: cs = 43; goto _test_eof; 
				_test_eof44: cs = 44; goto _test_eof; 
				_test_eof45: cs = 45; goto _test_eof; 
				_test_eof46: cs = 46; goto _test_eof; 
				_test_eof47: cs = 47; goto _test_eof; 
				
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
						case 48: {
							break;
						}
						case 49: {
							break;
						}
						case 50: {
							break;
						}
						case 4: {
							break;
						}
						case 51: {
							break;
						}
						case 5: {
							break;
						}
						case 52: {
							break;
						}
						case 53: {
							break;
						}
						case 54: {
							break;
						}
						case 55: {
							break;
						}
						case 6: {
							break;
						}
						case 7: {
							break;
						}
						case 56: {
							break;
						}
						case 57: {
							break;
						}
						case 8: {
							break;
						}
						case 58: {
							break;
						}
						case 59: {
							break;
						}
						case 9: {
							break;
						}
						case 60: {
							break;
						}
						case 10: {
							break;
						}
						case 61: {
							break;
						}
						case 62: {
							break;
						}
						case 11: {
							break;
						}
						case 12: {
							break;
						}
						case 63: {
							break;
						}
						case 64: {
							break;
						}
						case 13: {
							break;
						}
						case 65: {
							break;
						}
						case 66: {
							break;
						}
						case 14: {
							break;
						}
						case 67: {
							break;
						}
						case 15: {
							break;
						}
						case 16: {
							break;
						}
						case 17: {
							break;
						}
						case 68: {
							break;
						}
						case 18: {
							break;
						}
						case 19: {
							break;
						}
						case 69: {
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
						case 70: {
							break;
						}
						case 23: {
							break;
						}
						case 24: {
							break;
						}
						case 71: {
							break;
						}
						case 72: {
							break;
						}
						case 73: {
							break;
						}
						case 25: {
							break;
						}
						case 74: {
							break;
						}
						case 26: {
							break;
						}
						case 75: {
							break;
						}
						case 27: {
							break;
						}
						case 76: {
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
						case 77: {
							break;
						}
						case 31: {
							break;
						}
						case 78: {
							break;
						}
						case 79: {
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
						case 80: {
							break;
						}
						case 35: {
							break;
						}
						case 81: {
							break;
						}
						case 82: {
							break;
						}
						case 36: {
							break;
						}
						case 37: {
							break;
						}
						case 83: {
							break;
						}
						case 84: {
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
						case 38: {
							break;
						}
						case 88: {
							break;
						}
						case 89: {
							break;
						}
						case 39: {
							break;
						}
						case 40: {
							break;
						}
						case 90: {
							break;
						}
						case 41: {
							break;
						}
						case 91: {
							break;
						}
						case 92: {
							break;
						}
						case 93: {
							break;
						}
						case 42: {
							break;
						}
						case 94: {
							break;
						}
						case 95: {
							break;
						}
						case 96: {
							break;
						}
						case 97: {
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
						case 47: {
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
						goto _st3;case 48:
						goto _ctr94;case 49:
						goto _ctr108;case 50:
						goto _ctr109;case 4:
						goto _st4;case 51:
						goto _ctr110;case 5:
						goto _st5;case 52:
						goto _ctr122;case 53:
						goto _ctr123;case 54:
						goto _ctr131;case 55:
						goto _ctr132;case 6:
						goto _st6;case 7:
						goto _st7;case 56:
						goto _ctr133;case 57:
						goto _ctr134;case 8:
						goto _st8;case 58:
						goto _ctr136;case 59:
						goto _ctr137;case 9:
						goto _st9;case 60:
						goto _ctr138;case 10:
						goto _st10;case 61:
						goto _ctr139;case 62:
						goto _ctr140;case 11:
						goto _st11;case 12:
						goto _st12;case 63:
						goto _ctr141;case 64:
						goto _ctr142;case 13:
						goto _st13;case 65:
						goto _ctr144;case 66:
						goto _ctr145;case 14:
						goto _st14;case 67:
						goto _ctr146;case 15:
						goto _st15;case 16:
						goto _st16;case 17:
						goto _st17;case 68:
						goto _ctr147;case 18:
						goto _st18;case 19:
						goto _st19;case 69:
						goto _ctr148;case 20:
						goto _st20;case 21:
						goto _st21;case 22:
						goto _st22;case 70:
						goto _ctr149;case 23:
						goto _st23;case 24:
						goto _st24;case 71:
						goto _ctr150;case 72:
						goto _ctr151;case 73:
						goto _ctr153;case 25:
						goto _st25;case 74:
						goto _ctr154;case 26:
						goto _st26;case 75:
						goto _ctr155;case 27:
						goto _st27;case 76:
						goto _ctr156;case 28:
						goto _st28;case 29:
						goto _st29;case 30:
						goto _st30;case 77:
						goto _ctr157;case 31:
						goto _st31;case 78:
						goto _ctr158;case 79:
						goto _ctr159;case 32:
						goto _st32;case 33:
						goto _st33;case 34:
						goto _st34;case 80:
						goto _ctr160;case 35:
						goto _st35;case 81:
						goto _ctr161;case 82:
						goto _ctr163;case 36:
						goto _st36;case 37:
						goto _st37;case 83:
						goto _ctr164;case 84:
						goto _ctr165;case 85:
						goto _ctr167;case 86:
						goto _ctr168;case 87:
						goto _ctr169;case 38:
						goto _st38;case 88:
						goto _ctr172;case 89:
						goto _ctr174;case 39:
						goto _st39;case 40:
						goto _st40;case 90:
						goto _ctr175;case 41:
						goto _st41;case 91:
						goto _ctr176;case 92:
						goto _ctr181;case 93:
						goto _ctr183;case 42:
						goto _st42;case 94:
						goto _ctr184;case 95:
						goto _ctr187;case 96:
						goto _ctr189;case 97:
						goto _ctr191;case 43:
						goto _st43;case 44:
						goto _st44;case 45:
						goto _st45;case 46:
						goto _st46;case 47:
						goto _st47;	}
				}
				
				if ( cs >= 48 )
					goto _out; _pop: {}
				_out: {}
			}
			
#line 73 "lex/parser_units.rl"
			
			
			const bool ok = cs >= 
#line 5227 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
			48
#line 75 "lex/parser_units.rl"
			;
			if (ok) {
				unit_.value = ragel_num.vdouble;
				
				switch(ragel_type) {
					case TYPE_RATIO:
					if (ragel_num.ratio.den == 0) {
						fprintf(stderr, "division by zero: %s\n", str);
						return false;
					}
					
					unit_.value = ragel_num.getRatioAsFloat();
					break;
					case TYPE_INT:
					unit_.value = ragel_num.getInteger();
					break;
					default:
					break;
				}
				
				unit_.type = ragel_type;
				unit_.pos = pos_;
				unit_.smpte.hour = smpte.hour;
				unit_.smpte.min = smpte.min;
				unit_.smpte.sec = smpte.sec;
				unit_.smpte.frame = smpte.frame;
				unit_.bpm = bpm;
			}
			
			return ok;
		}
		
		size_t UnitsFullMatch::parse(const AtomListView& lv, UnitVec& out)
		{
			const size_t N = lv.size();
			
			for (size_t i = 0; i < N; i++) {
				const auto& a = lv[i];
				if (!parse(a))
					return i;
				
				out.push_back(unit_);
			}
			
			return N;
		}
		
		
#line 5278 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
		static const int units_type_start = 1;
		static const int units_type_first_final = 28;
		static const int units_type_error = 0;
		
		static const int units_type_en_main = 1;
		
		
#line 128 "lex/parser_units.rl"
		
		
		bool UnitTypeFullMatch::parse(const char* str)
		{
			reset();
			
			const auto len = strlen(str);
			if (len == 0)
				return false;
			
			const char* p = str;
			const char* pe = p + len;
			const char* eof = pe;
			
			
#line 5302 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
			{
				cs = (int)units_type_start;
			}
			
#line 142 "lex/parser_units.rl"
			
			
#line 5310 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
			{
				if ( p == pe )
					goto _test_eof;
				switch ( cs ) {
					case 1:
					goto st_case_1;
					case 0:
					goto st_case_0;
					case 28:
					goto st_case_28;
					case 29:
					goto st_case_29;
					case 2:
					goto st_case_2;
					case 30:
					goto st_case_30;
					case 31:
					goto st_case_31;
					case 3:
					goto st_case_3;
					case 4:
					goto st_case_4;
					case 32:
					goto st_case_32;
					case 33:
					goto st_case_33;
					case 5:
					goto st_case_5;
					case 34:
					goto st_case_34;
					case 35:
					goto st_case_35;
					case 6:
					goto st_case_6;
					case 36:
					goto st_case_36;
					case 7:
					goto st_case_7;
					case 37:
					goto st_case_37;
					case 38:
					goto st_case_38;
					case 8:
					goto st_case_8;
					case 9:
					goto st_case_9;
					case 39:
					goto st_case_39;
					case 40:
					goto st_case_40;
					case 10:
					goto st_case_10;
					case 41:
					goto st_case_41;
					case 42:
					goto st_case_42;
					case 11:
					goto st_case_11;
					case 43:
					goto st_case_43;
					case 12:
					goto st_case_12;
					case 13:
					goto st_case_13;
					case 14:
					goto st_case_14;
					case 44:
					goto st_case_44;
					case 15:
					goto st_case_15;
					case 16:
					goto st_case_16;
					case 45:
					goto st_case_45;
					case 17:
					goto st_case_17;
					case 18:
					goto st_case_18;
					case 19:
					goto st_case_19;
					case 46:
					goto st_case_46;
					case 20:
					goto st_case_20;
					case 21:
					goto st_case_21;
					case 47:
					goto st_case_47;
					case 48:
					goto st_case_48;
					case 49:
					goto st_case_49;
					case 22:
					goto st_case_22;
					case 50:
					goto st_case_50;
					case 23:
					goto st_case_23;
					case 51:
					goto st_case_51;
					case 24:
					goto st_case_24;
					case 52:
					goto st_case_52;
					case 25:
					goto st_case_25;
					case 26:
					goto st_case_26;
					case 27:
					goto st_case_27;
					case 53:
					goto st_case_53;
				}
				goto st_out;
				_st1:
				if ( p == eof ) {
					if ( cs >= 28 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof1;
				st_case_1:
				switch( ( (*( p))) ) {
					case 37: {
						goto _st28;
					}
					case 42: {
						goto _st29;
					}
					case 72: {
						goto _st2;
					}
					case 99: {
						goto _st31;
					}
					case 100: {
						goto _st33;
					}
					case 104: {
						goto _st38;
					}
					case 109: {
						goto _st40;
					}
					case 112: {
						goto _st12;
					}
					case 114: {
						goto _st20;
					}
					case 115: {
						goto _st48;
					}
				}
				{
					goto _st0;
				}
				st_case_0:
				_st0:
				cs = 0;
				goto _pop;
				_ctr51:
				{
#line 25 "lex/ragel_units.rl"
					ragel_type = TYPE_PERCENT;}
				
#line 5480 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				goto _st28;
				_st28:
				if ( p == eof ) {
					if ( cs >= 28 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof28;
				st_case_28:
				{
					goto _st0;
				}
				_ctr52:
				{
#line 27 "lex/ragel_units.rl"
					ragel_type = TYPE_PHASE;}
				
#line 5502 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				goto _st29;
				_st29:
				if ( p == eof ) {
					if ( cs >= 28 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof29;
				st_case_29:
				{
					goto _st0;
				}
				_st2:
				if ( p == eof ) {
					if ( cs >= 28 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof2;
				st_case_2:
				if ( ( (*( p))) == 122 ) {
					goto _st30;
				}
				{
					goto _st0;
				}
				_ctr53:
				{
#line 13 "lex/ragel_units.rl"
					ragel_type = TYPE_HZ;}
				
#line 5541 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				goto _st30;
				_st30:
				if ( p == eof ) {
					if ( cs >= 28 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof30;
				st_case_30:
				{
					goto _st0;
				}
				_ctr54:
				{
#line 23 "lex/ragel_units.rl"
					ragel_type = TYPE_CENT;}
				
#line 5563 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				goto _st31;
				_st31:
				if ( p == eof ) {
					if ( cs >= 28 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof31;
				st_case_31:
				if ( ( (*( p))) == 101 ) {
					goto _st3;
				}
				{
					goto _st0;
				}
				_st3:
				if ( p == eof ) {
					if ( cs >= 28 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof3;
				st_case_3:
				if ( ( (*( p))) == 110 ) {
					goto _st4;
				}
				{
					goto _st0;
				}
				_st4:
				if ( p == eof ) {
					if ( cs >= 28 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof4;
				st_case_4:
				if ( ( (*( p))) == 116 ) {
					goto _st32;
				}
				{
					goto _st0;
				}
				_ctr55:
				{
#line 23 "lex/ragel_units.rl"
					ragel_type = TYPE_CENT;}
				
#line 5622 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				goto _st32;
				_st32:
				if ( p == eof ) {
					if ( cs >= 28 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof32;
				st_case_32:
				{
					goto _st0;
				}
				_ctr56:
				{
#line 18 "lex/ragel_units.rl"
					ragel_type = TYPE_DAY;}
				
#line 5644 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				goto _st33;
				_st33:
				if ( p == eof ) {
					if ( cs >= 28 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof33;
				st_case_33:
				switch( ( (*( p))) ) {
					case 97: {
						goto _st5;
					}
					case 98: {
						goto _st35;
					}
					case 101: {
						goto _st7;
					}
				}
				{
					goto _st0;
				}
				_st5:
				if ( p == eof ) {
					if ( cs >= 28 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof5;
				st_case_5:
				if ( ( (*( p))) == 121 ) {
					goto _st34;
				}
				{
					goto _st0;
				}
				_ctr58:
				{
#line 18 "lex/ragel_units.rl"
					ragel_type = TYPE_DAY;}
				
#line 5694 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				goto _st34;
				_st34:
				if ( p == eof ) {
					if ( cs >= 28 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof34;
				st_case_34:
				{
					goto _st0;
				}
				_ctr59:
				{
#line 17 "lex/ragel_units.rl"
					ragel_type = TYPE_DB;}
				
#line 5716 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				goto _st35;
				_st35:
				if ( p == eof ) {
					if ( cs >= 28 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof35;
				st_case_35:
				if ( ( (*( p))) == 102 ) {
					goto _st6;
				}
				{
					goto _st0;
				}
				_st6:
				if ( p == eof ) {
					if ( cs >= 28 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof6;
				st_case_6:
				if ( ( (*( p))) == 115 ) {
					goto _st36;
				}
				{
					goto _st0;
				}
				_ctr60:
				{
#line 17 "lex/ragel_units.rl"
					ragel_type = TYPE_DB;}
				
#line 5758 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				goto _st36;
				_st36:
				if ( p == eof ) {
					if ( cs >= 28 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof36;
				st_case_36:
				{
					goto _st0;
				}
				_st7:
				if ( p == eof ) {
					if ( cs >= 28 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof7;
				st_case_7:
				if ( ( (*( p))) == 103 ) {
					goto _st37;
				}
				{
					goto _st0;
				}
				_ctr61:
				{
#line 16 "lex/ragel_units.rl"
					ragel_type = TYPE_DEGREE;}
				
#line 5797 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				goto _st37;
				_st37:
				if ( p == eof ) {
					if ( cs >= 28 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof37;
				st_case_37:
				{
					goto _st0;
				}
				_ctr62:
				{
#line 19 "lex/ragel_units.rl"
					ragel_type = TYPE_HOUR;}
				
#line 5819 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				goto _st38;
				_st38:
				if ( p == eof ) {
					if ( cs >= 28 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof38;
				st_case_38:
				switch( ( (*( p))) ) {
					case 111: {
						goto _st8;
					}
					case 122: {
						goto _st30;
					}
				}
				{
					goto _st0;
				}
				_st8:
				if ( p == eof ) {
					if ( cs >= 28 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof8;
				st_case_8:
				if ( ( (*( p))) == 117 ) {
					goto _st9;
				}
				{
					goto _st0;
				}
				_st9:
				if ( p == eof ) {
					if ( cs >= 28 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof9;
				st_case_9:
				if ( ( (*( p))) == 114 ) {
					goto _st39;
				}
				{
					goto _st0;
				}
				_ctr63:
				{
#line 19 "lex/ragel_units.rl"
					ragel_type = TYPE_HOUR;}
				
#line 5883 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				goto _st39;
				_st39:
				if ( p == eof ) {
					if ( cs >= 28 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof39;
				st_case_39:
				{
					goto _st0;
				}
				_ctr64:
				{
#line 20 "lex/ragel_units.rl"
					ragel_type = TYPE_MIN;}
				
#line 5905 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				goto _st40;
				_st40:
				if ( p == eof ) {
					if ( cs >= 28 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof40;
				st_case_40:
				switch( ( (*( p))) ) {
					case 105: {
						goto _st10;
					}
					case 115: {
						goto _st42;
					}
				}
				{
					goto _st0;
				}
				_st10:
				if ( p == eof ) {
					if ( cs >= 28 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof10;
				st_case_10:
				if ( ( (*( p))) == 110 ) {
					goto _st41;
				}
				{
					goto _st0;
				}
				_ctr66:
				{
#line 20 "lex/ragel_units.rl"
					ragel_type = TYPE_MIN;}
				
#line 5952 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				goto _st41;
				_st41:
				if ( p == eof ) {
					if ( cs >= 28 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof41;
				st_case_41:
				{
					goto _st0;
				}
				_ctr67:
				{
#line 22 "lex/ragel_units.rl"
					ragel_type = TYPE_MSEC;}
				
#line 5974 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				goto _st42;
				_st42:
				if ( p == eof ) {
					if ( cs >= 28 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof42;
				st_case_42:
				if ( ( (*( p))) == 101 ) {
					goto _st11;
				}
				{
					goto _st0;
				}
				_st11:
				if ( p == eof ) {
					if ( cs >= 28 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof11;
				st_case_11:
				if ( ( (*( p))) == 99 ) {
					goto _st43;
				}
				{
					goto _st0;
				}
				_ctr68:
				{
#line 22 "lex/ragel_units.rl"
					ragel_type = TYPE_MSEC;}
				
#line 6016 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				goto _st43;
				_st43:
				if ( p == eof ) {
					if ( cs >= 28 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof43;
				st_case_43:
				{
					goto _st0;
				}
				_st12:
				if ( p == eof ) {
					if ( cs >= 28 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof12;
				st_case_12:
				switch( ( (*( p))) ) {
					case 101: {
						goto _st13;
					}
					case 104: {
						goto _st17;
					}
				}
				{
					goto _st0;
				}
				_st13:
				if ( p == eof ) {
					if ( cs >= 28 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof13;
				st_case_13:
				if ( ( (*( p))) == 114 ) {
					goto _st14;
				}
				{
					goto _st0;
				}
				_st14:
				if ( p == eof ) {
					if ( cs >= 28 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof14;
				st_case_14:
				if ( ( (*( p))) == 99 ) {
					goto _st44;
				}
				{
					goto _st0;
				}
				_ctr69:
				{
#line 26 "lex/ragel_units.rl"
					ragel_type = TYPE_PERCENT;}
				
#line 6094 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				goto _st44;
				_st44:
				if ( p == eof ) {
					if ( cs >= 28 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof44;
				st_case_44:
				if ( ( (*( p))) == 101 ) {
					goto _st15;
				}
				{
					goto _st0;
				}
				_st15:
				if ( p == eof ) {
					if ( cs >= 28 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof15;
				st_case_15:
				if ( ( (*( p))) == 110 ) {
					goto _st16;
				}
				{
					goto _st0;
				}
				_st16:
				if ( p == eof ) {
					if ( cs >= 28 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof16;
				st_case_16:
				if ( ( (*( p))) == 116 ) {
					goto _st45;
				}
				{
					goto _st0;
				}
				_ctr70:
				{
#line 26 "lex/ragel_units.rl"
					ragel_type = TYPE_PERCENT;}
				
#line 6153 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				goto _st45;
				_st45:
				if ( p == eof ) {
					if ( cs >= 28 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof45;
				st_case_45:
				{
					goto _st0;
				}
				_st17:
				if ( p == eof ) {
					if ( cs >= 28 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof17;
				st_case_17:
				if ( ( (*( p))) == 97 ) {
					goto _st18;
				}
				{
					goto _st0;
				}
				_st18:
				if ( p == eof ) {
					if ( cs >= 28 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof18;
				st_case_18:
				if ( ( (*( p))) == 115 ) {
					goto _st19;
				}
				{
					goto _st0;
				}
				_st19:
				if ( p == eof ) {
					if ( cs >= 28 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof19;
				st_case_19:
				if ( ( (*( p))) == 101 ) {
					goto _st46;
				}
				{
					goto _st0;
				}
				_ctr71:
				{
#line 28 "lex/ragel_units.rl"
					ragel_type = TYPE_PHASE;}
				
#line 6226 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				goto _st46;
				_st46:
				if ( p == eof ) {
					if ( cs >= 28 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof46;
				st_case_46:
				{
					goto _st0;
				}
				_st20:
				if ( p == eof ) {
					if ( cs >= 28 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof20;
				st_case_20:
				if ( ( (*( p))) == 97 ) {
					goto _st21;
				}
				{
					goto _st0;
				}
				_st21:
				if ( p == eof ) {
					if ( cs >= 28 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof21;
				st_case_21:
				if ( ( (*( p))) == 100 ) {
					goto _st47;
				}
				{
					goto _st0;
				}
				_ctr72:
				{
#line 15 "lex/ragel_units.rl"
					ragel_type = TYPE_RADIAN;}
				
#line 6282 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				goto _st47;
				_st47:
				if ( p == eof ) {
					if ( cs >= 28 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof47;
				st_case_47:
				{
					goto _st0;
				}
				_ctr73:
				{
#line 21 "lex/ragel_units.rl"
					ragel_type = TYPE_SEC;}
				
#line 6304 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				goto _st48;
				_st48:
				if ( p == eof ) {
					if ( cs >= 28 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof48;
				st_case_48:
				switch( ( (*( p))) ) {
					case 97: {
						goto _st49;
					}
					case 101: {
						goto _st23;
					}
				}
				{
					goto _st0;
				}
				_ctr75:
				{
#line 14 "lex/ragel_units.rl"
					ragel_type = TYPE_SAMP;}
				
#line 6334 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				goto _st49;
				_st49:
				if ( p == eof ) {
					if ( cs >= 28 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof49;
				st_case_49:
				if ( ( (*( p))) == 109 ) {
					goto _st22;
				}
				{
					goto _st0;
				}
				_st22:
				if ( p == eof ) {
					if ( cs >= 28 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof22;
				st_case_22:
				if ( ( (*( p))) == 112 ) {
					goto _st50;
				}
				{
					goto _st0;
				}
				_ctr76:
				{
#line 14 "lex/ragel_units.rl"
					ragel_type = TYPE_SAMP;}
				
#line 6376 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				goto _st50;
				_st50:
				if ( p == eof ) {
					if ( cs >= 28 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof50;
				st_case_50:
				{
					goto _st0;
				}
				_st23:
				if ( p == eof ) {
					if ( cs >= 28 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof23;
				st_case_23:
				switch( ( (*( p))) ) {
					case 99: {
						goto _st51;
					}
					case 109: {
						goto _st24;
					}
				}
				{
					goto _st0;
				}
				_ctr77:
				{
#line 21 "lex/ragel_units.rl"
					ragel_type = TYPE_SEC;}
				
#line 6420 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				goto _st51;
				_st51:
				if ( p == eof ) {
					if ( cs >= 28 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof51;
				st_case_51:
				{
					goto _st0;
				}
				_st24:
				if ( p == eof ) {
					if ( cs >= 28 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof24;
				st_case_24:
				if ( ( (*( p))) == 105 ) {
					goto _st52;
				}
				{
					goto _st0;
				}
				_ctr78:
				{
#line 24 "lex/ragel_units.rl"
					ragel_type = TYPE_SEMITONE;}
				
#line 6459 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				goto _st52;
				_st52:
				if ( p == eof ) {
					if ( cs >= 28 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof52;
				st_case_52:
				if ( ( (*( p))) == 116 ) {
					goto _st25;
				}
				{
					goto _st0;
				}
				_st25:
				if ( p == eof ) {
					if ( cs >= 28 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof25;
				st_case_25:
				if ( ( (*( p))) == 111 ) {
					goto _st26;
				}
				{
					goto _st0;
				}
				_st26:
				if ( p == eof ) {
					if ( cs >= 28 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof26;
				st_case_26:
				if ( ( (*( p))) == 110 ) {
					goto _st27;
				}
				{
					goto _st0;
				}
				_st27:
				if ( p == eof ) {
					if ( cs >= 28 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof27;
				st_case_27:
				if ( ( (*( p))) == 101 ) {
					goto _st53;
				}
				{
					goto _st0;
				}
				_ctr79:
				{
#line 24 "lex/ragel_units.rl"
					ragel_type = TYPE_SEMITONE;}
				
#line 6535 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
				
				goto _st53;
				_st53:
				if ( p == eof ) {
					if ( cs >= 28 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof53;
				st_case_53:
				{
					goto _st0;
				}
				st_out:
				_test_eof1: cs = 1; goto _test_eof; 
				_test_eof28: cs = 28; goto _test_eof; 
				_test_eof29: cs = 29; goto _test_eof; 
				_test_eof2: cs = 2; goto _test_eof; 
				_test_eof30: cs = 30; goto _test_eof; 
				_test_eof31: cs = 31; goto _test_eof; 
				_test_eof3: cs = 3; goto _test_eof; 
				_test_eof4: cs = 4; goto _test_eof; 
				_test_eof32: cs = 32; goto _test_eof; 
				_test_eof33: cs = 33; goto _test_eof; 
				_test_eof5: cs = 5; goto _test_eof; 
				_test_eof34: cs = 34; goto _test_eof; 
				_test_eof35: cs = 35; goto _test_eof; 
				_test_eof6: cs = 6; goto _test_eof; 
				_test_eof36: cs = 36; goto _test_eof; 
				_test_eof7: cs = 7; goto _test_eof; 
				_test_eof37: cs = 37; goto _test_eof; 
				_test_eof38: cs = 38; goto _test_eof; 
				_test_eof8: cs = 8; goto _test_eof; 
				_test_eof9: cs = 9; goto _test_eof; 
				_test_eof39: cs = 39; goto _test_eof; 
				_test_eof40: cs = 40; goto _test_eof; 
				_test_eof10: cs = 10; goto _test_eof; 
				_test_eof41: cs = 41; goto _test_eof; 
				_test_eof42: cs = 42; goto _test_eof; 
				_test_eof11: cs = 11; goto _test_eof; 
				_test_eof43: cs = 43; goto _test_eof; 
				_test_eof12: cs = 12; goto _test_eof; 
				_test_eof13: cs = 13; goto _test_eof; 
				_test_eof14: cs = 14; goto _test_eof; 
				_test_eof44: cs = 44; goto _test_eof; 
				_test_eof15: cs = 15; goto _test_eof; 
				_test_eof16: cs = 16; goto _test_eof; 
				_test_eof45: cs = 45; goto _test_eof; 
				_test_eof17: cs = 17; goto _test_eof; 
				_test_eof18: cs = 18; goto _test_eof; 
				_test_eof19: cs = 19; goto _test_eof; 
				_test_eof46: cs = 46; goto _test_eof; 
				_test_eof20: cs = 20; goto _test_eof; 
				_test_eof21: cs = 21; goto _test_eof; 
				_test_eof47: cs = 47; goto _test_eof; 
				_test_eof48: cs = 48; goto _test_eof; 
				_test_eof49: cs = 49; goto _test_eof; 
				_test_eof22: cs = 22; goto _test_eof; 
				_test_eof50: cs = 50; goto _test_eof; 
				_test_eof23: cs = 23; goto _test_eof; 
				_test_eof51: cs = 51; goto _test_eof; 
				_test_eof24: cs = 24; goto _test_eof; 
				_test_eof52: cs = 52; goto _test_eof; 
				_test_eof25: cs = 25; goto _test_eof; 
				_test_eof26: cs = 26; goto _test_eof; 
				_test_eof27: cs = 27; goto _test_eof; 
				_test_eof53: cs = 53; goto _test_eof; 
				
				_test_eof: {}
				if ( p == eof ) {
					switch ( cs ) {
						case 1: {
							break;
						}
						case 0: {
							break;
						}
						case 28: {
							break;
						}
						case 29: {
							break;
						}
						case 2: {
							break;
						}
						case 30: {
							break;
						}
						case 31: {
							break;
						}
						case 3: {
							break;
						}
						case 4: {
							break;
						}
						case 32: {
							break;
						}
						case 33: {
							break;
						}
						case 5: {
							break;
						}
						case 34: {
							break;
						}
						case 35: {
							break;
						}
						case 6: {
							break;
						}
						case 36: {
							break;
						}
						case 7: {
							break;
						}
						case 37: {
							break;
						}
						case 38: {
							break;
						}
						case 8: {
							break;
						}
						case 9: {
							break;
						}
						case 39: {
							break;
						}
						case 40: {
							break;
						}
						case 10: {
							break;
						}
						case 41: {
							break;
						}
						case 42: {
							break;
						}
						case 11: {
							break;
						}
						case 43: {
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
						case 44: {
							break;
						}
						case 15: {
							break;
						}
						case 16: {
							break;
						}
						case 45: {
							break;
						}
						case 17: {
							break;
						}
						case 18: {
							break;
						}
						case 19: {
							break;
						}
						case 46: {
							break;
						}
						case 20: {
							break;
						}
						case 21: {
							break;
						}
						case 47: {
							break;
						}
						case 48: {
							break;
						}
						case 49: {
							break;
						}
						case 22: {
							break;
						}
						case 50: {
							break;
						}
						case 23: {
							break;
						}
						case 51: {
							break;
						}
						case 24: {
							break;
						}
						case 52: {
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
						case 53: {
							break;
						}
					}
					switch ( cs ) {
					}
					switch ( cs ) {
						case 1:
						goto _st1;case 0:
						goto _st0;case 28:
						goto _ctr51;case 29:
						goto _ctr52;case 2:
						goto _st2;case 30:
						goto _ctr53;case 31:
						goto _ctr54;case 3:
						goto _st3;case 4:
						goto _st4;case 32:
						goto _ctr55;case 33:
						goto _ctr56;case 5:
						goto _st5;case 34:
						goto _ctr58;case 35:
						goto _ctr59;case 6:
						goto _st6;case 36:
						goto _ctr60;case 7:
						goto _st7;case 37:
						goto _ctr61;case 38:
						goto _ctr62;case 8:
						goto _st8;case 9:
						goto _st9;case 39:
						goto _ctr63;case 40:
						goto _ctr64;case 10:
						goto _st10;case 41:
						goto _ctr66;case 42:
						goto _ctr67;case 11:
						goto _st11;case 43:
						goto _ctr68;case 12:
						goto _st12;case 13:
						goto _st13;case 14:
						goto _st14;case 44:
						goto _ctr69;case 15:
						goto _st15;case 16:
						goto _st16;case 45:
						goto _ctr70;case 17:
						goto _st17;case 18:
						goto _st18;case 19:
						goto _st19;case 46:
						goto _ctr71;case 20:
						goto _st20;case 21:
						goto _st21;case 47:
						goto _ctr72;case 48:
						goto _ctr73;case 49:
						goto _ctr75;case 22:
						goto _st22;case 50:
						goto _ctr76;case 23:
						goto _st23;case 51:
						goto _ctr77;case 24:
						goto _st24;case 52:
						goto _ctr78;case 25:
						goto _st25;case 26:
						goto _st26;case 27:
						goto _st27;case 53:
						goto _ctr79;	}
				}
				
				if ( cs >= 28 )
					goto _out; _pop: {}
				_out: {}
			}
			
#line 143 "lex/parser_units.rl"
			
			
			return cs >= 
#line 6842 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_units.cpp"
			28
#line 145 "lex/parser_units.rl"
			;
		}
		
		bool UnitTypeFullMatch::parse(const Atom& a)
		{
			if (a.isSymbol()) {
				return parse(a.asT<t_symbol*>()->s_name);
			} else if(a.isFloat()) {
				reset();
				return true;
			} else
			return false;
		}
		
	}
}


