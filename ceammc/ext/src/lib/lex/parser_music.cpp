#line 1 "lex/parser_music.rl"
# include "parser_music.h"
# include "parser_numeric.h"

# include <cstdint>
# include <cstring>

namespace ceammc {
	namespace parser {
		
		
#line 13 "lex/parser_music.cpp"
		static const int bpm_start = 1;
		static const int bpm_first_final = 7;
		static const int bpm_error = 0;
		
		static const int bpm_en_main = 1;
		
		
#line 16 "lex/parser_music.rl"
		
		
		std::ostream& operator<<(std::ostream& os, const Duration& dur)
		{
			if (dur.repeats > 1)
				os << (int)dur.repeats << '*';
			
			os << (int)dur.num << '/' << (int)dur.den;
			return os;
		}
		
		BpmFullMatch::BpmFullMatch()
		{
			reset();
		}
		
		void BpmFullMatch::reset()
		{
			bpm_ = { 0, 0.25 };
		}
		
		bool BpmFullMatch::parse(const Atom& a)
		{
			reset();
			
			if (a.isSymbol())
				return parse(a.asT<t_symbol*>()->s_name);
			else if(a.isFloat()) {
				auto f = a.asT<t_float>();
				if(f < 0)
				return false;
				
				bpm_.bpm = a.asT<t_float>();
				bpm_.beatlen = 0.25;
				return true;
			} else
			return false;
		}
		
		bool BpmFullMatch::parse(const char* str)
		{
			const auto len = strlen(str);
			if (len == 0)
				return false;
			
			const char* p = str;
			const char* pe = p + len;
			const char* eof = pe;
			
			DECLARE_RAGEL_COMMON_VARS;
			fsm::BpmData bpm;
			
			reset();
			
			
#line 77 "lex/parser_music.cpp"
			{
				cs = (int)bpm_start;
			}
			
#line 70 "lex/parser_music.rl"
			
			
#line 85 "lex/parser_music.cpp"
			{
				if ( p == pe )
					goto _test_eof;
				switch ( cs ) {
					case 1:
					goto st_case_1;
					case 0:
					goto st_case_0;
					case 7:
					goto st_case_7;
					case 2:
					goto st_case_2;
					case 8:
					goto st_case_8;
					case 9:
					goto st_case_9;
					case 3:
					goto st_case_3;
					case 4:
					goto st_case_4;
					case 10:
					goto st_case_10;
					case 5:
					goto st_case_5;
					case 11:
					goto st_case_11;
					case 12:
					goto st_case_12;
					case 6:
					goto st_case_6;
					case 13:
					goto st_case_13;
					case 14:
					goto st_case_14;
				}
				goto st_out;
				_st1:
				if ( p == eof ) {
					if ( cs >= 7 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof1;
				st_case_1:
				if ( ( (*( p))) == 48 ) {
					goto _ctr2;
				}
				if ( 49 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr3;
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
#line 9 "lex/ragel_music.rl"
					bpm.dur_num = 1; bpm.dur_den = 4; }
				
#line 151 "lex/parser_music.cpp"
				
				{
#line 6 "lex/ragel_music.rl"
					(bpm.ival *= 10) += ((( (*( p)))) - '0'); }
				
#line 157 "lex/parser_music.cpp"
				
				goto _st7;
				_ctr13:
				{
#line 10 "lex/ragel_music.rl"
					ragel_cat = CAT_UNIT; ragel_type = TYPE_BPM; }
				
#line 165 "lex/parser_music.cpp"
				
				goto _st7;
				_st7:
				if ( p == eof ) {
					if ( cs >= 7 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof7;
				st_case_7:
				switch( ( (*( p))) ) {
					case 46: {
						goto _st2;
					}
					case 95: {
						goto _st9;
					}
					case 98: {
						goto _st3;
					}
					case 124: {
						goto _st5;
					}
				}
				{
					goto _st0;
				}
				_st2:
				if ( p == eof ) {
					if ( cs >= 7 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof2;
				st_case_2:
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr5;
				}
				{
					goto _st0;
				}
				_ctr5:
				{
#line 7 "lex/ragel_music.rl"
					(bpm.fnum *= 10) += ((( (*( p)))) - '0'); bpm.fden *= 10; }
				
#line 218 "lex/parser_music.cpp"
				
				goto _st8;
				_ctr15:
				{
#line 10 "lex/ragel_music.rl"
					ragel_cat = CAT_UNIT; ragel_type = TYPE_BPM; }
				
#line 226 "lex/parser_music.cpp"
				
				goto _st8;
				_st8:
				if ( p == eof ) {
					if ( cs >= 7 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof8;
				st_case_8:
				switch( ( (*( p))) ) {
					case 95: {
						goto _st9;
					}
					case 98: {
						goto _st3;
					}
					case 124: {
						goto _st5;
					}
				}
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr5;
				}
				{
					goto _st0;
				}
				_ctr16:
				{
#line 10 "lex/ragel_music.rl"
					ragel_cat = CAT_UNIT; ragel_type = TYPE_BPM; }
				
#line 262 "lex/parser_music.cpp"
				
				goto _st9;
				_ctr21:
				{
#line 24 "lex/ragel_music.rl"
					bpm.dur_num = 1;}
				
#line 270 "lex/parser_music.cpp"
				
				goto _st9;
				_st9:
				if ( p == eof ) {
					if ( cs >= 7 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof9;
				st_case_9:
				if ( ( (*( p))) == 98 ) {
					goto _st3;
				}
				{
					goto _st0;
				}
				_ctr22:
				{
#line 24 "lex/ragel_music.rl"
					bpm.dur_num = 1;}
				
#line 295 "lex/parser_music.cpp"
				
				goto _st3;
				_st3:
				if ( p == eof ) {
					if ( cs >= 7 )
						goto _out;
					else
						goto _pop;
				}
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
				if ( p == eof ) {
					if ( cs >= 7 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof4;
				st_case_4:
				if ( ( (*( p))) == 109 ) {
					goto _st10;
				}
				{
					goto _st0;
				}
				_ctr17:
				{
#line 10 "lex/ragel_music.rl"
					ragel_cat = CAT_UNIT; ragel_type = TYPE_BPM; }
				
#line 337 "lex/parser_music.cpp"
				
				goto _st10;
				_st10:
				if ( p == eof ) {
					if ( cs >= 7 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof10;
				st_case_10:
				{
					goto _st0;
				}
				_st5:
				if ( p == eof ) {
					if ( cs >= 7 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof5;
				st_case_5:
				if ( 49 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr10;
				}
				{
					goto _st0;
				}
				_ctr10:
				{
#line 11 "lex/ragel_music.rl"
					bpm.dur_num = 0; }
				
#line 376 "lex/parser_music.cpp"
				
				{
#line 12 "lex/ragel_music.rl"
					(bpm.dur_num *= 10) += ((( (*( p)))) - '0'); }
				
#line 382 "lex/parser_music.cpp"
				
				{
#line 13 "lex/ragel_music.rl"
					bpm.dur_den = 0;}
				
#line 388 "lex/parser_music.cpp"
				
				{
#line 14 "lex/ragel_music.rl"
					(bpm.dur_den *= 10) += ((( (*( p)))) - '0'); }
				
#line 394 "lex/parser_music.cpp"
				
				goto _st11;
				_ctr18:
				{
#line 24 "lex/ragel_music.rl"
					bpm.dur_num = 1;}
				
#line 402 "lex/parser_music.cpp"
				
				{
#line 10 "lex/ragel_music.rl"
					ragel_cat = CAT_UNIT; ragel_type = TYPE_BPM; }
				
#line 408 "lex/parser_music.cpp"
				
				goto _st11;
				_ctr20:
				{
#line 12 "lex/ragel_music.rl"
					(bpm.dur_num *= 10) += ((( (*( p)))) - '0'); }
				
#line 416 "lex/parser_music.cpp"
				
				{
#line 14 "lex/ragel_music.rl"
					(bpm.dur_den *= 10) += ((( (*( p)))) - '0'); }
				
#line 422 "lex/parser_music.cpp"
				
				goto _st11;
				_st11:
				if ( p == eof ) {
					if ( cs >= 7 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof11;
				st_case_11:
				switch( ( (*( p))) ) {
					case 46: {
						goto _ctr19;
					}
					case 47: {
						goto _st6;
					}
					case 95: {
						goto _ctr21;
					}
					case 98: {
						goto _ctr22;
					}
				}
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr20;
				}
				{
					goto _st0;
				}
				_ctr23:
				{
#line 10 "lex/ragel_music.rl"
					ragel_cat = CAT_UNIT; ragel_type = TYPE_BPM; }
				
#line 461 "lex/parser_music.cpp"
				
				goto _st12;
				_ctr19:
				{
#line 24 "lex/ragel_music.rl"
					bpm.dur_num = 1;}
				
#line 469 "lex/parser_music.cpp"
				
				{
#line 8 "lex/ragel_music.rl"
					bpm.dur_num *= 3; bpm.dur_den *= 2; }
				
#line 475 "lex/parser_music.cpp"
				
				goto _st12;
				_ctr25:
				{
#line 8 "lex/ragel_music.rl"
					bpm.dur_num *= 3; bpm.dur_den *= 2; }
				
#line 483 "lex/parser_music.cpp"
				
				goto _st12;
				_st12:
				if ( p == eof ) {
					if ( cs >= 7 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof12;
				st_case_12:
				switch( ( (*( p))) ) {
					case 95: {
						goto _st9;
					}
					case 98: {
						goto _st3;
					}
				}
				{
					goto _st0;
				}
				_st6:
				if ( p == eof ) {
					if ( cs >= 7 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof6;
				st_case_6:
				if ( 49 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr12;
				}
				{
					goto _st0;
				}
				_ctr12:
				{
#line 13 "lex/ragel_music.rl"
					bpm.dur_den = 0;}
				
#line 530 "lex/parser_music.cpp"
				
				{
#line 14 "lex/ragel_music.rl"
					(bpm.dur_den *= 10) += ((( (*( p)))) - '0'); }
				
#line 536 "lex/parser_music.cpp"
				
				goto _st13;
				_ctr24:
				{
#line 10 "lex/ragel_music.rl"
					ragel_cat = CAT_UNIT; ragel_type = TYPE_BPM; }
				
#line 544 "lex/parser_music.cpp"
				
				goto _st13;
				_ctr26:
				{
#line 14 "lex/ragel_music.rl"
					(bpm.dur_den *= 10) += ((( (*( p)))) - '0'); }
				
#line 552 "lex/parser_music.cpp"
				
				goto _st13;
				_st13:
				if ( p == eof ) {
					if ( cs >= 7 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof13;
				st_case_13:
				switch( ( (*( p))) ) {
					case 46: {
						goto _ctr25;
					}
					case 95: {
						goto _st9;
					}
					case 98: {
						goto _st3;
					}
				}
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr26;
				}
				{
					goto _st0;
				}
				_ctr3:
				{
#line 9 "lex/ragel_music.rl"
					bpm.dur_num = 1; bpm.dur_den = 4; }
				
#line 588 "lex/parser_music.cpp"
				
				{
#line 6 "lex/ragel_music.rl"
					(bpm.ival *= 10) += ((( (*( p)))) - '0'); }
				
#line 594 "lex/parser_music.cpp"
				
				goto _st14;
				_ctr27:
				{
#line 10 "lex/ragel_music.rl"
					ragel_cat = CAT_UNIT; ragel_type = TYPE_BPM; }
				
#line 602 "lex/parser_music.cpp"
				
				goto _st14;
				_ctr28:
				{
#line 6 "lex/ragel_music.rl"
					(bpm.ival *= 10) += ((( (*( p)))) - '0'); }
				
#line 610 "lex/parser_music.cpp"
				
				goto _st14;
				_st14:
				if ( p == eof ) {
					if ( cs >= 7 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof14;
				st_case_14:
				switch( ( (*( p))) ) {
					case 46: {
						goto _st2;
					}
					case 95: {
						goto _st9;
					}
					case 98: {
						goto _st3;
					}
					case 124: {
						goto _st5;
					}
				}
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr28;
				}
				{
					goto _st0;
				}
				st_out:
				_test_eof1: cs = 1; goto _test_eof; 
				_test_eof7: cs = 7; goto _test_eof; 
				_test_eof2: cs = 2; goto _test_eof; 
				_test_eof8: cs = 8; goto _test_eof; 
				_test_eof9: cs = 9; goto _test_eof; 
				_test_eof3: cs = 3; goto _test_eof; 
				_test_eof4: cs = 4; goto _test_eof; 
				_test_eof10: cs = 10; goto _test_eof; 
				_test_eof5: cs = 5; goto _test_eof; 
				_test_eof11: cs = 11; goto _test_eof; 
				_test_eof12: cs = 12; goto _test_eof; 
				_test_eof6: cs = 6; goto _test_eof; 
				_test_eof13: cs = 13; goto _test_eof; 
				_test_eof14: cs = 14; goto _test_eof; 
				
				_test_eof: {}
				if ( p == eof ) {
					switch ( cs ) {
						case 1: {
							break;
						}
						case 0: {
							break;
						}
						case 7: {
							break;
						}
						case 2: {
							break;
						}
						case 8: {
							break;
						}
						case 9: {
							break;
						}
						case 3: {
							break;
						}
						case 4: {
							break;
						}
						case 10: {
							break;
						}
						case 5: {
							break;
						}
						case 11: {
							break;
						}
						case 12: {
							break;
						}
						case 6: {
							break;
						}
						case 13: {
							break;
						}
						case 14: {
							break;
						}
					}
					switch ( cs ) {
					}
					switch ( cs ) {
						case 1:
						goto _st1;case 0:
						goto _st0;case 7:
						goto _ctr13;case 2:
						goto _st2;case 8:
						goto _ctr15;case 9:
						goto _ctr16;case 3:
						goto _st3;case 4:
						goto _st4;case 10:
						goto _ctr17;case 5:
						goto _st5;case 11:
						goto _ctr18;case 12:
						goto _ctr23;case 6:
						goto _st6;case 13:
						goto _ctr24;case 14:
						goto _ctr27;	}
				}
				
				if ( cs >= 7 )
					goto _out; _pop: {}
				_out: {}
			}
			
#line 71 "lex/parser_music.rl"
			
			
			const bool ok = cs >= 
#line 739 "lex/parser_music.cpp"
			7
#line 73 "lex/parser_music.rl"
			;
			
			if (ok)
				bpm_ = bpm;
			
			return ok;
		}
		
		size_t BpmFullMatch::parse(const AtomListView& lv, SmallBpmVec& out)
		{
			const size_t N = lv.size();
			
			for (size_t i = 0; i < N; i++) {
				const auto& a = lv[i];
				if (!parse(a))
					return i;
				
				out.push_back(bpm_);
			}
			
			return N;
		}
		
		
#line 766 "lex/parser_music.cpp"
		static const int spn_start = 1;
		static const int spn_first_final = 8;
		static const int spn_error = 0;
		
		static const int spn_en_main = 1;
		
		
#line 102 "lex/parser_music.rl"
		
		
		SpnFullMatch::SpnFullMatch()
		{
			reset();
		}
		
		void SpnFullMatch::reset()
		{
			spn_ = { };
		}
		
		bool SpnFullMatch::parse(const char* str)
		{
			const auto len = strlen(str);
			if (len == 0)
				return false;
			
			const char* p = str;
			const char* pe = p + len;
			const char* eof = pe;
			
			DECLARE_RAGEL_COMMON_VARS;
			fsm::SpnData spn;
			
			reset();
			
			
#line 803 "lex/parser_music.cpp"
			{
				cs = (int)spn_start;
			}
			
#line 129 "lex/parser_music.rl"
			
			
#line 811 "lex/parser_music.cpp"
			{
				if ( p == pe )
					goto _test_eof;
				switch ( cs ) {
					case 1:
					goto st_case_1;
					case 0:
					goto st_case_0;
					case 8:
					goto st_case_8;
					case 9:
					goto st_case_9;
					case 10:
					goto st_case_10;
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
					case 11:
					goto st_case_11;
					case 7:
					goto st_case_7;
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
				}
				goto st_out;
				_st1:
				if ( p == eof ) {
					if ( cs >= 8 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof1;
				st_case_1:
				switch( ( (*( p))) ) {
					case 65: {
						goto _ctr2;
					}
					case 66: {
						goto _ctr3;
					}
					case 72: {
						goto _ctr3;
					}
				}
				if ( 67 <= ( (*( p))) && ( (*( p))) <= 71 ) {
					goto _ctr4;
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
#line 77 "lex/ragel_music.rl"
					spn.alt = 0; spn.oct = 0; spn.octtype = OCTAVE_REL; spn.dev = 0;}
				
#line 901 "lex/parser_music.cpp"
				
				{
#line 41 "lex/ragel_music.rl"
					spn.note = 5;}
				
#line 907 "lex/parser_music.cpp"
				
				goto _st8;
				_ctr3:
				{
#line 77 "lex/ragel_music.rl"
					spn.alt = 0; spn.oct = 0; spn.octtype = OCTAVE_REL; spn.dev = 0;}
				
#line 915 "lex/parser_music.cpp"
				
				{
#line 42 "lex/ragel_music.rl"
					spn.note = 6;}
				
#line 921 "lex/parser_music.cpp"
				
				goto _st8;
				_ctr4:
				{
#line 77 "lex/ragel_music.rl"
					spn.alt = 0; spn.oct = 0; spn.octtype = OCTAVE_REL; spn.dev = 0;}
				
#line 929 "lex/parser_music.cpp"
				
				{
#line 43 "lex/ragel_music.rl"
					spn.note = ((( (*( p)))) - 'C');}
				
#line 935 "lex/parser_music.cpp"
				
				goto _st8;
				_ctr16:
				{
#line 44 "lex/ragel_music.rl"
					spn.pitch = (spn.note * 2) - (spn.note > 2);}
				
#line 943 "lex/parser_music.cpp"
				
				{
#line 60 "lex/ragel_music.rl"
					spn.octtype = OCTAVE_REL;}
				
#line 949 "lex/parser_music.cpp"
				
				{
#line 78 "lex/ragel_music.rl"
					ragel_cat = CAT_UNIT; ragel_type = TYPE_SPN;}
				
#line 955 "lex/parser_music.cpp"
				
				goto _st8;
				_st8:
				if ( p == eof ) {
					if ( cs >= 8 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof8;
				st_case_8:
				switch( ( (*( p))) ) {
					case 35: {
						goto _ctr17;
					}
					case 40: {
						goto _ctr18;
					}
					case 45: {
						goto _ctr19;
					}
					case 94: {
						goto _ctr21;
					}
					case 95: {
						goto _ctr22;
					}
					case 98: {
						goto _ctr23;
					}
				}
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr20;
				}
				{
					goto _st0;
				}
				_ctr17:
				{
#line 44 "lex/ragel_music.rl"
					spn.pitch = (spn.note * 2) - (spn.note > 2);}
				
#line 1000 "lex/parser_music.cpp"
				
				{
#line 37 "lex/ragel_music.rl"
					spn.alt = 1;}
				
#line 1006 "lex/parser_music.cpp"
				
				goto _st9;
				_ctr24:
				{
#line 60 "lex/ragel_music.rl"
					spn.octtype = OCTAVE_REL;}
				
#line 1014 "lex/parser_music.cpp"
				
				{
#line 78 "lex/ragel_music.rl"
					ragel_cat = CAT_UNIT; ragel_type = TYPE_SPN;}
				
#line 1020 "lex/parser_music.cpp"
				
				goto _st9;
				_st9:
				if ( p == eof ) {
					if ( cs >= 8 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof9;
				st_case_9:
				switch( ( (*( p))) ) {
					case 35: {
						goto _ctr25;
					}
					case 40: {
						goto _ctr26;
					}
					case 45: {
						goto _ctr27;
					}
					case 94: {
						goto _ctr29;
					}
					case 95: {
						goto _ctr30;
					}
				}
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr28;
				}
				{
					goto _st0;
				}
				_ctr31:
				{
#line 60 "lex/ragel_music.rl"
					spn.octtype = OCTAVE_REL;}
				
#line 1062 "lex/parser_music.cpp"
				
				{
#line 78 "lex/ragel_music.rl"
					ragel_cat = CAT_UNIT; ragel_type = TYPE_SPN;}
				
#line 1068 "lex/parser_music.cpp"
				
				goto _st10;
				_ctr25:
				{
#line 38 "lex/ragel_music.rl"
					spn.alt = 2;}
				
#line 1076 "lex/parser_music.cpp"
				
				goto _st10;
				_ctr53:
				{
#line 40 "lex/ragel_music.rl"
					spn.alt = -2;}
				
#line 1084 "lex/parser_music.cpp"
				
				goto _st10;
				_st10:
				if ( p == eof ) {
					if ( cs >= 8 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof10;
				st_case_10:
				switch( ( (*( p))) ) {
					case 40: {
						goto _ctr26;
					}
					case 45: {
						goto _ctr27;
					}
					case 94: {
						goto _ctr29;
					}
					case 95: {
						goto _ctr30;
					}
				}
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr28;
				}
				{
					goto _st0;
				}
				_ctr18:
				{
#line 44 "lex/ragel_music.rl"
					spn.pitch = (spn.note * 2) - (spn.note > 2);}
				
#line 1123 "lex/parser_music.cpp"
				
				{
#line 60 "lex/ragel_music.rl"
					spn.octtype = OCTAVE_REL;}
				
#line 1129 "lex/parser_music.cpp"
				
				goto _st2;
				_ctr26:
				{
#line 60 "lex/ragel_music.rl"
					spn.octtype = OCTAVE_REL;}
				
#line 1137 "lex/parser_music.cpp"
				
				goto _st2;
				_ctr34:
				{
#line 51 "lex/ragel_music.rl"
					spn.octtype = OCTAVE_ABS;}
				
#line 1145 "lex/parser_music.cpp"
				
				goto _st2;
				_st2:
				if ( p == eof ) {
					if ( cs >= 8 )
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
						goto _ctr6;
					}
					case 45: {
						goto _ctr6;
					}
				}
				{
					goto _st0;
				}
				_ctr6:
				{
#line 65 "lex/ragel_music.rl"
					spn.sign = ((( (*( p))))=='-') ? -1 : 1;}
				
#line 1175 "lex/parser_music.cpp"
				
				goto _st3;
				_st3:
				if ( p == eof ) {
					if ( cs >= 8 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof3;
				st_case_3:
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr8;
				}
				{
					goto _st0;
				}
				_ctr8:
				{
#line 66 "lex/ragel_music.rl"
					(spn.dev *= 10) += ((( (*( p)))) - '0');}
				
#line 1200 "lex/parser_music.cpp"
				
				goto _st4;
				_st4:
				if ( p == eof ) {
					if ( cs >= 8 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof4;
				st_case_4:
				if ( ( (*( p))) == 99 ) {
					goto _st6;
				}
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr10;
				}
				{
					goto _st0;
				}
				_ctr10:
				{
#line 66 "lex/ragel_music.rl"
					(spn.dev *= 10) += ((( (*( p)))) - '0');}
				
#line 1228 "lex/parser_music.cpp"
				
				goto _st5;
				_st5:
				if ( p == eof ) {
					if ( cs >= 8 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof5;
				st_case_5:
				if ( ( (*( p))) == 99 ) {
					goto _st6;
				}
				{
					goto _st0;
				}
				_st6:
				if ( p == eof ) {
					if ( cs >= 8 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof6;
				st_case_6:
				if ( ( (*( p))) == 41 ) {
					goto _ctr13;
				}
				{
					goto _st0;
				}
				_ctr13:
				{
#line 67 "lex/ragel_music.rl"
					spn.dev *= spn.sign;}
				
#line 1270 "lex/parser_music.cpp"
				
				goto _st11;
				_ctr32:
				{
#line 78 "lex/ragel_music.rl"
					ragel_cat = CAT_UNIT; ragel_type = TYPE_SPN;}
				
#line 1278 "lex/parser_music.cpp"
				
				goto _st11;
				_st11:
				if ( p == eof ) {
					if ( cs >= 8 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof11;
				st_case_11:
				{
					goto _st0;
				}
				_ctr19:
				{
#line 44 "lex/ragel_music.rl"
					spn.pitch = (spn.note * 2) - (spn.note > 2);}
				
#line 1300 "lex/parser_music.cpp"
				
				{
#line 60 "lex/ragel_music.rl"
					spn.octtype = OCTAVE_REL;}
				
#line 1306 "lex/parser_music.cpp"
				
				goto _st7;
				_ctr27:
				{
#line 60 "lex/ragel_music.rl"
					spn.octtype = OCTAVE_REL;}
				
#line 1314 "lex/parser_music.cpp"
				
				goto _st7;
				_st7:
				if ( p == eof ) {
					if ( cs >= 8 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof7;
				st_case_7:
				if ( ( (*( p))) == 49 ) {
					goto _ctr15;
				}
				{
					goto _st0;
				}
				_ctr15:
				{
#line 48 "lex/ragel_music.rl"
					spn.oct = -1;}
				
#line 1339 "lex/parser_music.cpp"
				
				goto _st12;
				_ctr20:
				{
#line 44 "lex/ragel_music.rl"
					spn.pitch = (spn.note * 2) - (spn.note > 2);}
				
#line 1347 "lex/parser_music.cpp"
				
				{
#line 60 "lex/ragel_music.rl"
					spn.octtype = OCTAVE_REL;}
				
#line 1353 "lex/parser_music.cpp"
				
				{
#line 49 "lex/ragel_music.rl"
					spn.oct = ((( (*( p)))) - '0');}
				
#line 1359 "lex/parser_music.cpp"
				
				goto _st12;
				_ctr28:
				{
#line 60 "lex/ragel_music.rl"
					spn.octtype = OCTAVE_REL;}
				
#line 1367 "lex/parser_music.cpp"
				
				{
#line 49 "lex/ragel_music.rl"
					spn.oct = ((( (*( p)))) - '0');}
				
#line 1373 "lex/parser_music.cpp"
				
				goto _st12;
				_ctr33:
				{
#line 51 "lex/ragel_music.rl"
					spn.octtype = OCTAVE_ABS;}
				
#line 1381 "lex/parser_music.cpp"
				
				{
#line 78 "lex/ragel_music.rl"
					ragel_cat = CAT_UNIT; ragel_type = TYPE_SPN;}
				
#line 1387 "lex/parser_music.cpp"
				
				goto _st12;
				_st12:
				if ( p == eof ) {
					if ( cs >= 8 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof12;
				st_case_12:
				if ( ( (*( p))) == 40 ) {
					goto _ctr34;
				}
				{
					goto _st0;
				}
				_ctr21:
				{
#line 44 "lex/ragel_music.rl"
					spn.pitch = (spn.note * 2) - (spn.note > 2);}
				
#line 1412 "lex/parser_music.cpp"
				
				{
#line 60 "lex/ragel_music.rl"
					spn.octtype = OCTAVE_REL;}
				
#line 1418 "lex/parser_music.cpp"
				
				{
#line 55 "lex/ragel_music.rl"
					spn.oct++;}
				
#line 1424 "lex/parser_music.cpp"
				
				{
#line 58 "lex/ragel_music.rl"
					spn.octtype = OCTAVE_REL;}
				
#line 1430 "lex/parser_music.cpp"
				
				goto _st13;
				_ctr29:
				{
#line 60 "lex/ragel_music.rl"
					spn.octtype = OCTAVE_REL;}
				
#line 1438 "lex/parser_music.cpp"
				
				{
#line 55 "lex/ragel_music.rl"
					spn.oct++;}
				
#line 1444 "lex/parser_music.cpp"
				
				{
#line 58 "lex/ragel_music.rl"
					spn.octtype = OCTAVE_REL;}
				
#line 1450 "lex/parser_music.cpp"
				
				goto _st13;
				_ctr35:
				{
#line 78 "lex/ragel_music.rl"
					ragel_cat = CAT_UNIT; ragel_type = TYPE_SPN;}
				
#line 1458 "lex/parser_music.cpp"
				
				goto _st13;
				_st13:
				if ( p == eof ) {
					if ( cs >= 8 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof13;
				st_case_13:
				switch( ( (*( p))) ) {
					case 40: {
						goto _st2;
					}
					case 94: {
						goto _ctr36;
					}
				}
				{
					goto _st0;
				}
				_ctr37:
				{
#line 78 "lex/ragel_music.rl"
					ragel_cat = CAT_UNIT; ragel_type = TYPE_SPN;}
				
#line 1488 "lex/parser_music.cpp"
				
				goto _st14;
				_ctr36:
				{
#line 55 "lex/ragel_music.rl"
					spn.oct++;}
				
#line 1496 "lex/parser_music.cpp"
				
				{
#line 58 "lex/ragel_music.rl"
					spn.octtype = OCTAVE_REL;}
				
#line 1502 "lex/parser_music.cpp"
				
				goto _st14;
				_st14:
				if ( p == eof ) {
					if ( cs >= 8 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof14;
				st_case_14:
				switch( ( (*( p))) ) {
					case 40: {
						goto _st2;
					}
					case 94: {
						goto _ctr38;
					}
				}
				{
					goto _st0;
				}
				_ctr39:
				{
#line 78 "lex/ragel_music.rl"
					ragel_cat = CAT_UNIT; ragel_type = TYPE_SPN;}
				
#line 1532 "lex/parser_music.cpp"
				
				goto _st15;
				_ctr38:
				{
#line 55 "lex/ragel_music.rl"
					spn.oct++;}
				
#line 1540 "lex/parser_music.cpp"
				
				{
#line 58 "lex/ragel_music.rl"
					spn.octtype = OCTAVE_REL;}
				
#line 1546 "lex/parser_music.cpp"
				
				goto _st15;
				_st15:
				if ( p == eof ) {
					if ( cs >= 8 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof15;
				st_case_15:
				switch( ( (*( p))) ) {
					case 40: {
						goto _st2;
					}
					case 94: {
						goto _ctr40;
					}
				}
				{
					goto _st0;
				}
				_ctr41:
				{
#line 78 "lex/ragel_music.rl"
					ragel_cat = CAT_UNIT; ragel_type = TYPE_SPN;}
				
#line 1576 "lex/parser_music.cpp"
				
				goto _st16;
				_ctr40:
				{
#line 55 "lex/ragel_music.rl"
					spn.oct++;}
				
#line 1584 "lex/parser_music.cpp"
				
				{
#line 58 "lex/ragel_music.rl"
					spn.octtype = OCTAVE_REL;}
				
#line 1590 "lex/parser_music.cpp"
				
				goto _st16;
				_st16:
				if ( p == eof ) {
					if ( cs >= 8 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof16;
				st_case_16:
				switch( ( (*( p))) ) {
					case 40: {
						goto _st2;
					}
					case 94: {
						goto _ctr42;
					}
				}
				{
					goto _st0;
				}
				_ctr43:
				{
#line 78 "lex/ragel_music.rl"
					ragel_cat = CAT_UNIT; ragel_type = TYPE_SPN;}
				
#line 1620 "lex/parser_music.cpp"
				
				goto _st17;
				_ctr42:
				{
#line 55 "lex/ragel_music.rl"
					spn.oct++;}
				
#line 1628 "lex/parser_music.cpp"
				
				{
#line 58 "lex/ragel_music.rl"
					spn.octtype = OCTAVE_REL;}
				
#line 1634 "lex/parser_music.cpp"
				
				goto _st17;
				_ctr51:
				{
#line 57 "lex/ragel_music.rl"
					spn.oct--;}
				
#line 1642 "lex/parser_music.cpp"
				
				{
#line 58 "lex/ragel_music.rl"
					spn.octtype = OCTAVE_REL;}
				
#line 1648 "lex/parser_music.cpp"
				
				goto _st17;
				_st17:
				if ( p == eof ) {
					if ( cs >= 8 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof17;
				st_case_17:
				if ( ( (*( p))) == 40 ) {
					goto _st2;
				}
				{
					goto _st0;
				}
				_ctr22:
				{
#line 44 "lex/ragel_music.rl"
					spn.pitch = (spn.note * 2) - (spn.note > 2);}
				
#line 1673 "lex/parser_music.cpp"
				
				{
#line 60 "lex/ragel_music.rl"
					spn.octtype = OCTAVE_REL;}
				
#line 1679 "lex/parser_music.cpp"
				
				{
#line 57 "lex/ragel_music.rl"
					spn.oct--;}
				
#line 1685 "lex/parser_music.cpp"
				
				{
#line 58 "lex/ragel_music.rl"
					spn.octtype = OCTAVE_REL;}
				
#line 1691 "lex/parser_music.cpp"
				
				goto _st18;
				_ctr30:
				{
#line 60 "lex/ragel_music.rl"
					spn.octtype = OCTAVE_REL;}
				
#line 1699 "lex/parser_music.cpp"
				
				{
#line 57 "lex/ragel_music.rl"
					spn.oct--;}
				
#line 1705 "lex/parser_music.cpp"
				
				{
#line 58 "lex/ragel_music.rl"
					spn.octtype = OCTAVE_REL;}
				
#line 1711 "lex/parser_music.cpp"
				
				goto _st18;
				_ctr44:
				{
#line 78 "lex/ragel_music.rl"
					ragel_cat = CAT_UNIT; ragel_type = TYPE_SPN;}
				
#line 1719 "lex/parser_music.cpp"
				
				goto _st18;
				_st18:
				if ( p == eof ) {
					if ( cs >= 8 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof18;
				st_case_18:
				switch( ( (*( p))) ) {
					case 40: {
						goto _st2;
					}
					case 95: {
						goto _ctr45;
					}
				}
				{
					goto _st0;
				}
				_ctr46:
				{
#line 78 "lex/ragel_music.rl"
					ragel_cat = CAT_UNIT; ragel_type = TYPE_SPN;}
				
#line 1749 "lex/parser_music.cpp"
				
				goto _st19;
				_ctr45:
				{
#line 57 "lex/ragel_music.rl"
					spn.oct--;}
				
#line 1757 "lex/parser_music.cpp"
				
				{
#line 58 "lex/ragel_music.rl"
					spn.octtype = OCTAVE_REL;}
				
#line 1763 "lex/parser_music.cpp"
				
				goto _st19;
				_st19:
				if ( p == eof ) {
					if ( cs >= 8 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof19;
				st_case_19:
				switch( ( (*( p))) ) {
					case 40: {
						goto _st2;
					}
					case 95: {
						goto _ctr47;
					}
				}
				{
					goto _st0;
				}
				_ctr48:
				{
#line 78 "lex/ragel_music.rl"
					ragel_cat = CAT_UNIT; ragel_type = TYPE_SPN;}
				
#line 1793 "lex/parser_music.cpp"
				
				goto _st20;
				_ctr47:
				{
#line 57 "lex/ragel_music.rl"
					spn.oct--;}
				
#line 1801 "lex/parser_music.cpp"
				
				{
#line 58 "lex/ragel_music.rl"
					spn.octtype = OCTAVE_REL;}
				
#line 1807 "lex/parser_music.cpp"
				
				goto _st20;
				_st20:
				if ( p == eof ) {
					if ( cs >= 8 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof20;
				st_case_20:
				switch( ( (*( p))) ) {
					case 40: {
						goto _st2;
					}
					case 95: {
						goto _ctr49;
					}
				}
				{
					goto _st0;
				}
				_ctr50:
				{
#line 78 "lex/ragel_music.rl"
					ragel_cat = CAT_UNIT; ragel_type = TYPE_SPN;}
				
#line 1837 "lex/parser_music.cpp"
				
				goto _st21;
				_ctr49:
				{
#line 57 "lex/ragel_music.rl"
					spn.oct--;}
				
#line 1845 "lex/parser_music.cpp"
				
				{
#line 58 "lex/ragel_music.rl"
					spn.octtype = OCTAVE_REL;}
				
#line 1851 "lex/parser_music.cpp"
				
				goto _st21;
				_st21:
				if ( p == eof ) {
					if ( cs >= 8 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof21;
				st_case_21:
				switch( ( (*( p))) ) {
					case 40: {
						goto _st2;
					}
					case 95: {
						goto _ctr51;
					}
				}
				{
					goto _st0;
				}
				_ctr23:
				{
#line 44 "lex/ragel_music.rl"
					spn.pitch = (spn.note * 2) - (spn.note > 2);}
				
#line 1881 "lex/parser_music.cpp"
				
				{
#line 39 "lex/ragel_music.rl"
					spn.alt = -1;}
				
#line 1887 "lex/parser_music.cpp"
				
				goto _st22;
				_ctr52:
				{
#line 60 "lex/ragel_music.rl"
					spn.octtype = OCTAVE_REL;}
				
#line 1895 "lex/parser_music.cpp"
				
				{
#line 78 "lex/ragel_music.rl"
					ragel_cat = CAT_UNIT; ragel_type = TYPE_SPN;}
				
#line 1901 "lex/parser_music.cpp"
				
				goto _st22;
				_st22:
				if ( p == eof ) {
					if ( cs >= 8 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof22;
				st_case_22:
				switch( ( (*( p))) ) {
					case 40: {
						goto _ctr26;
					}
					case 45: {
						goto _ctr27;
					}
					case 94: {
						goto _ctr29;
					}
					case 95: {
						goto _ctr30;
					}
					case 98: {
						goto _ctr53;
					}
				}
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr28;
				}
				{
					goto _st0;
				}
				st_out:
				_test_eof1: cs = 1; goto _test_eof; 
				_test_eof8: cs = 8; goto _test_eof; 
				_test_eof9: cs = 9; goto _test_eof; 
				_test_eof10: cs = 10; goto _test_eof; 
				_test_eof2: cs = 2; goto _test_eof; 
				_test_eof3: cs = 3; goto _test_eof; 
				_test_eof4: cs = 4; goto _test_eof; 
				_test_eof5: cs = 5; goto _test_eof; 
				_test_eof6: cs = 6; goto _test_eof; 
				_test_eof11: cs = 11; goto _test_eof; 
				_test_eof7: cs = 7; goto _test_eof; 
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
				
				_test_eof: {}
				if ( p == eof ) {
					switch ( cs ) {
						case 1: {
							break;
						}
						case 0: {
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
						case 6: {
							break;
						}
						case 11: {
							break;
						}
						case 7: {
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
						case 15: {
							break;
						}
						case 16: {
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
						case 20: {
							break;
						}
						case 21: {
							break;
						}
						case 22: {
							break;
						}
					}
					switch ( cs ) {
					}
					switch ( cs ) {
						case 1:
						goto _st1;case 0:
						goto _st0;case 8:
						goto _ctr16;case 9:
						goto _ctr24;case 10:
						goto _ctr31;case 2:
						goto _st2;case 3:
						goto _st3;case 4:
						goto _st4;case 5:
						goto _st5;case 6:
						goto _st6;case 11:
						goto _ctr32;case 7:
						goto _st7;case 12:
						goto _ctr33;case 13:
						goto _ctr35;case 14:
						goto _ctr37;case 15:
						goto _ctr39;case 16:
						goto _ctr41;case 17:
						goto _ctr43;case 18:
						goto _ctr44;case 19:
						goto _ctr46;case 20:
						goto _ctr48;case 21:
						goto _ctr50;case 22:
						goto _ctr52;	}
				}
				
				if ( cs >= 8 )
					goto _out; _pop: {}
				_out: {}
			}
			
#line 130 "lex/parser_music.rl"
			
			
			const bool ok = cs >= 
#line 2073 "lex/parser_music.cpp"
			8
#line 132 "lex/parser_music.rl"
			;
			
			if (ok)
				spn_ = spn;
			
			return ok;
		}
		
		bool SpnFullMatch::parse(const Atom& a)
		{
			if (a.isSymbol())
				return parse(a.asT<t_symbol*>()->s_name);
			else
				return false;
		}
		
		size_t SpnFullMatch::parse(const AtomListView& lv, SmallSpnVec& out)
		{
			const size_t N = lv.size();
			
			for (size_t i = 0; i < N; i++) {
				const auto& a = lv[i];
				if (!parse(a))
					return i;
				
				out.push_back(spn_);
			}
			
			return N;
		}
		
		
#line 2108 "lex/parser_music.cpp"
		static const int pitch_start = 1;
		static const int pitch_first_final = 2;
		static const int pitch_error = 0;
		
		static const int pitch_en_main = 1;
		
		
#line 169 "lex/parser_music.rl"
		
		
		PitchFullMatch::PitchFullMatch()
		{
			reset();
		}
		
		void PitchFullMatch::reset()
		{
			spn_ = { };
		}
		
		bool PitchFullMatch::parse(const char* str)
		{
			const auto len = strlen(str);
			if (len == 0)
				return false;
			
			const char* p = str;
			const char* pe = p + len;
			const char* eof = pe;
			
			DECLARE_RAGEL_COMMON_VARS;
			fsm::SpnData spn;
			
			reset();
			
			
#line 2145 "lex/parser_music.cpp"
			{
				cs = (int)pitch_start;
			}
			
#line 196 "lex/parser_music.rl"
			
			
#line 2153 "lex/parser_music.cpp"
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
				}
				goto st_out;
				_st1:
				if ( p == eof ) {
					if ( cs >= 2 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof1;
				st_case_1:
				switch( ( (*( p))) ) {
					case 65: {
						goto _ctr2;
					}
					case 66: {
						goto _ctr3;
					}
					case 72: {
						goto _ctr3;
					}
				}
				if ( 67 <= ( (*( p))) && ( (*( p))) <= 71 ) {
					goto _ctr4;
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
#line 81 "lex/ragel_music.rl"
					spn.alt = 0; spn.oct = 0; spn.octtype = OCTAVE_REL; spn.dev = 0;}
				
#line 2209 "lex/parser_music.cpp"
				
				{
#line 41 "lex/ragel_music.rl"
					spn.note = 5;}
				
#line 2215 "lex/parser_music.cpp"
				
				goto _st2;
				_ctr3:
				{
#line 81 "lex/ragel_music.rl"
					spn.alt = 0; spn.oct = 0; spn.octtype = OCTAVE_REL; spn.dev = 0;}
				
#line 2223 "lex/parser_music.cpp"
				
				{
#line 42 "lex/ragel_music.rl"
					spn.note = 6;}
				
#line 2229 "lex/parser_music.cpp"
				
				goto _st2;
				_ctr4:
				{
#line 81 "lex/ragel_music.rl"
					spn.alt = 0; spn.oct = 0; spn.octtype = OCTAVE_REL; spn.dev = 0;}
				
#line 2237 "lex/parser_music.cpp"
				
				{
#line 43 "lex/ragel_music.rl"
					spn.note = ((( (*( p)))) - 'C');}
				
#line 2243 "lex/parser_music.cpp"
				
				goto _st2;
				_ctr5:
				{
#line 44 "lex/ragel_music.rl"
					spn.pitch = (spn.note * 2) - (spn.note > 2);}
				
#line 2251 "lex/parser_music.cpp"
				
				{
#line 82 "lex/ragel_music.rl"
					ragel_cat = CAT_UNIT; ragel_type = TYPE_SPN;}
				
#line 2257 "lex/parser_music.cpp"
				
				goto _st2;
				_st2:
				if ( p == eof ) {
					if ( cs >= 2 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof2;
				st_case_2:
				switch( ( (*( p))) ) {
					case 35: {
						goto _ctr6;
					}
					case 98: {
						goto _ctr7;
					}
				}
				{
					goto _st0;
				}
				_ctr6:
				{
#line 44 "lex/ragel_music.rl"
					spn.pitch = (spn.note * 2) - (spn.note > 2);}
				
#line 2287 "lex/parser_music.cpp"
				
				{
#line 37 "lex/ragel_music.rl"
					spn.alt = 1;}
				
#line 2293 "lex/parser_music.cpp"
				
				goto _st3;
				_ctr8:
				{
#line 82 "lex/ragel_music.rl"
					ragel_cat = CAT_UNIT; ragel_type = TYPE_SPN;}
				
#line 2301 "lex/parser_music.cpp"
				
				goto _st3;
				_st3:
				if ( p == eof ) {
					if ( cs >= 2 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof3;
				st_case_3:
				if ( ( (*( p))) == 35 ) {
					goto _ctr9;
				}
				{
					goto _st0;
				}
				_ctr10:
				{
#line 82 "lex/ragel_music.rl"
					ragel_cat = CAT_UNIT; ragel_type = TYPE_SPN;}
				
#line 2326 "lex/parser_music.cpp"
				
				goto _st4;
				_ctr9:
				{
#line 38 "lex/ragel_music.rl"
					spn.alt = 2;}
				
#line 2334 "lex/parser_music.cpp"
				
				goto _st4;
				_ctr12:
				{
#line 40 "lex/ragel_music.rl"
					spn.alt = -2;}
				
#line 2342 "lex/parser_music.cpp"
				
				goto _st4;
				_st4:
				if ( p == eof ) {
					if ( cs >= 2 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof4;
				st_case_4:
				{
					goto _st0;
				}
				_ctr7:
				{
#line 44 "lex/ragel_music.rl"
					spn.pitch = (spn.note * 2) - (spn.note > 2);}
				
#line 2364 "lex/parser_music.cpp"
				
				{
#line 39 "lex/ragel_music.rl"
					spn.alt = -1;}
				
#line 2370 "lex/parser_music.cpp"
				
				goto _st5;
				_ctr11:
				{
#line 82 "lex/ragel_music.rl"
					ragel_cat = CAT_UNIT; ragel_type = TYPE_SPN;}
				
#line 2378 "lex/parser_music.cpp"
				
				goto _st5;
				_st5:
				if ( p == eof ) {
					if ( cs >= 2 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof5;
				st_case_5:
				if ( ( (*( p))) == 98 ) {
					goto _ctr12;
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
					}
					switch ( cs ) {
					}
					switch ( cs ) {
						case 1:
						goto _st1;case 0:
						goto _st0;case 2:
						goto _ctr5;case 3:
						goto _ctr8;case 4:
						goto _ctr10;case 5:
						goto _ctr11;	}
				}
				
				if ( cs >= 2 )
					goto _out; _pop: {}
				_out: {}
			}
			
#line 197 "lex/parser_music.rl"
			
			
			const bool ok = cs >= 
#line 2448 "lex/parser_music.cpp"
			2
#line 199 "lex/parser_music.rl"
			;
			
			if (ok)
				spn_ = spn;
			
			return ok;
		}
		
		bool PitchFullMatch::parse(const Atom& a)
		{
			if (a.isSymbol())
				return parse(a.asT<t_symbol*>()->s_name);
			else if(a.isInteger() && a.asT<int>() >= 0 && a.asT<int>() < 12) {
				spn_.setSemitones(a.asT<int>());
				return true;
			} else
			return false;
		}
		
		
#line 2471 "lex/parser_music.cpp"
		static const int notation_start = 1;
		static const int notation_first_final = 15;
		static const int notation_error = 0;
		
		static const int notation_en_main = 1;
		
		
#line 224 "lex/parser_music.rl"
		
		
		NotationSingle::NotationSingle()
		{
			reset();
		}
		
		void NotationSingle::reset()
		{
			note_.spn = {};
			note_.dur = {};
		}
		
		bool NotationSingle::parse(const char* str)
		{
			const auto len = strlen(str);
			if (len == 0)
				return false;
			
			const char* p = str;
			const char* pe = p + len;
			const char* eof = pe;
			
			DECLARE_RAGEL_COMMON_VARS;
			fsm::NotationData note;
			fsm::SpnData spn;
			
			reset();
			
			
#line 2510 "lex/parser_music.cpp"
			{
				cs = (int)notation_start;
			}
			
#line 253 "lex/parser_music.rl"
			
			
#line 2518 "lex/parser_music.cpp"
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
					case 15:
					goto st_case_15;
					case 16:
					goto st_case_16;
					case 17:
					goto st_case_17;
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
					case 18:
					goto st_case_18;
					case 9:
					goto st_case_9;
					case 10:
					goto st_case_10;
					case 19:
					goto st_case_19;
					case 11:
					goto st_case_11;
					case 20:
					goto st_case_20;
					case 12:
					goto st_case_12;
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
					case 13:
					goto st_case_13;
					case 26:
					goto st_case_26;
					case 27:
					goto st_case_27;
					case 14:
					goto st_case_14;
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
				}
				goto st_out;
				_st1:
				if ( p == eof ) {
					if ( cs >= 15 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof1;
				st_case_1:
				switch( ( (*( p))) ) {
					case 65: {
						goto _ctr3;
					}
					case 66: {
						goto _ctr4;
					}
					case 72: {
						goto _ctr4;
					}
					case 82: {
						goto _ctr6;
					}
				}
				if ( ( (*( p))) > 57 ) {
					if ( 67 <= ( (*( p))) && ( (*( p))) <= 71 ) {
						goto _ctr5;
					}
				} else if ( ( (*( p))) >= 49 ) {
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
#line 138 "lex/ragel_music.rl"
					note.repeats = 0; }
				
#line 2649 "lex/parser_music.cpp"
				
				{
#line 139 "lex/ragel_music.rl"
					(note.repeats *= 10) += ((( (*( p)))) - '0'); }
				
#line 2655 "lex/parser_music.cpp"
				
				goto _st2;
				_ctr9:
				{
#line 139 "lex/ragel_music.rl"
					(note.repeats *= 10) += ((( (*( p)))) - '0'); }
				
#line 2663 "lex/parser_music.cpp"
				
				goto _st2;
				_st2:
				if ( p == eof ) {
					if ( cs >= 15 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof2;
				st_case_2:
				if ( ( (*( p))) == 42 ) {
					goto _st3;
				}
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr9;
				}
				{
					goto _st0;
				}
				_st3:
				if ( p == eof ) {
					if ( cs >= 15 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof3;
				st_case_3:
				switch( ( (*( p))) ) {
					case 65: {
						goto _ctr3;
					}
					case 66: {
						goto _ctr4;
					}
					case 72: {
						goto _ctr4;
					}
					case 82: {
						goto _ctr6;
					}
				}
				if ( 67 <= ( (*( p))) && ( (*( p))) <= 71 ) {
					goto _ctr5;
				}
				{
					goto _st0;
				}
				_ctr3:
				{
#line 77 "lex/ragel_music.rl"
					spn.alt = 0; spn.oct = 0; spn.octtype = OCTAVE_REL; spn.dev = 0;}
				
#line 2722 "lex/parser_music.cpp"
				
				{
#line 41 "lex/ragel_music.rl"
					spn.note = 5;}
				
#line 2728 "lex/parser_music.cpp"
				
				goto _st15;
				_ctr4:
				{
#line 77 "lex/ragel_music.rl"
					spn.alt = 0; spn.oct = 0; spn.octtype = OCTAVE_REL; spn.dev = 0;}
				
#line 2736 "lex/parser_music.cpp"
				
				{
#line 42 "lex/ragel_music.rl"
					spn.note = 6;}
				
#line 2742 "lex/parser_music.cpp"
				
				goto _st15;
				_ctr5:
				{
#line 77 "lex/ragel_music.rl"
					spn.alt = 0; spn.oct = 0; spn.octtype = OCTAVE_REL; spn.dev = 0;}
				
#line 2750 "lex/parser_music.cpp"
				
				{
#line 43 "lex/ragel_music.rl"
					spn.note = ((( (*( p)))) - 'C');}
				
#line 2756 "lex/parser_music.cpp"
				
				goto _st15;
				_ctr31:
				{
#line 44 "lex/ragel_music.rl"
					spn.pitch = (spn.note * 2) - (spn.note > 2);}
				
#line 2764 "lex/parser_music.cpp"
				
				{
#line 60 "lex/ragel_music.rl"
					spn.octtype = OCTAVE_REL;}
				
#line 2770 "lex/parser_music.cpp"
				
				{
#line 78 "lex/ragel_music.rl"
					ragel_cat = CAT_UNIT; ragel_type = TYPE_SPN;}
				
#line 2776 "lex/parser_music.cpp"
				
				{
#line 134 "lex/ragel_music.rl"
					note.num = 1; note.den = 4; note.dots = 0; note.durtype = DURATION_REL;}
				
#line 2782 "lex/parser_music.cpp"
				
				{
#line 135 "lex/ragel_music.rl"
					ragel_cat = CAT_UNIT; ragel_type = TYPE_SPN;}
				
#line 2788 "lex/parser_music.cpp"
				
				goto _st15;
				_st15:
				if ( p == eof ) {
					if ( cs >= 15 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof15;
				st_case_15:
				switch( ( (*( p))) ) {
					case 35: {
						goto _ctr32;
					}
					case 40: {
						goto _ctr33;
					}
					case 45: {
						goto _ctr34;
					}
					case 94: {
						goto _ctr36;
					}
					case 95: {
						goto _ctr37;
					}
					case 98: {
						goto _ctr38;
					}
					case 124: {
						goto _ctr39;
					}
				}
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr35;
				}
				{
					goto _st0;
				}
				_ctr32:
				{
#line 44 "lex/ragel_music.rl"
					spn.pitch = (spn.note * 2) - (spn.note > 2);}
				
#line 2836 "lex/parser_music.cpp"
				
				{
#line 37 "lex/ragel_music.rl"
					spn.alt = 1;}
				
#line 2842 "lex/parser_music.cpp"
				
				goto _st16;
				_ctr40:
				{
#line 60 "lex/ragel_music.rl"
					spn.octtype = OCTAVE_REL;}
				
#line 2850 "lex/parser_music.cpp"
				
				{
#line 78 "lex/ragel_music.rl"
					ragel_cat = CAT_UNIT; ragel_type = TYPE_SPN;}
				
#line 2856 "lex/parser_music.cpp"
				
				{
#line 134 "lex/ragel_music.rl"
					note.num = 1; note.den = 4; note.dots = 0; note.durtype = DURATION_REL;}
				
#line 2862 "lex/parser_music.cpp"
				
				{
#line 135 "lex/ragel_music.rl"
					ragel_cat = CAT_UNIT; ragel_type = TYPE_SPN;}
				
#line 2868 "lex/parser_music.cpp"
				
				goto _st16;
				_st16:
				if ( p == eof ) {
					if ( cs >= 15 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof16;
				st_case_16:
				switch( ( (*( p))) ) {
					case 35: {
						goto _ctr41;
					}
					case 40: {
						goto _ctr42;
					}
					case 45: {
						goto _ctr43;
					}
					case 94: {
						goto _ctr45;
					}
					case 95: {
						goto _ctr46;
					}
					case 124: {
						goto _ctr47;
					}
				}
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr44;
				}
				{
					goto _st0;
				}
				_ctr48:
				{
#line 60 "lex/ragel_music.rl"
					spn.octtype = OCTAVE_REL;}
				
#line 2913 "lex/parser_music.cpp"
				
				{
#line 78 "lex/ragel_music.rl"
					ragel_cat = CAT_UNIT; ragel_type = TYPE_SPN;}
				
#line 2919 "lex/parser_music.cpp"
				
				{
#line 134 "lex/ragel_music.rl"
					note.num = 1; note.den = 4; note.dots = 0; note.durtype = DURATION_REL;}
				
#line 2925 "lex/parser_music.cpp"
				
				{
#line 135 "lex/ragel_music.rl"
					ragel_cat = CAT_UNIT; ragel_type = TYPE_SPN;}
				
#line 2931 "lex/parser_music.cpp"
				
				goto _st17;
				_ctr41:
				{
#line 38 "lex/ragel_music.rl"
					spn.alt = 2;}
				
#line 2939 "lex/parser_music.cpp"
				
				goto _st17;
				_ctr92:
				{
#line 40 "lex/ragel_music.rl"
					spn.alt = -2;}
				
#line 2947 "lex/parser_music.cpp"
				
				goto _st17;
				_st17:
				if ( p == eof ) {
					if ( cs >= 15 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof17;
				st_case_17:
				switch( ( (*( p))) ) {
					case 40: {
						goto _ctr42;
					}
					case 45: {
						goto _ctr43;
					}
					case 94: {
						goto _ctr45;
					}
					case 95: {
						goto _ctr46;
					}
					case 124: {
						goto _ctr47;
					}
				}
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr44;
				}
				{
					goto _st0;
				}
				_ctr33:
				{
#line 44 "lex/ragel_music.rl"
					spn.pitch = (spn.note * 2) - (spn.note > 2);}
				
#line 2989 "lex/parser_music.cpp"
				
				{
#line 60 "lex/ragel_music.rl"
					spn.octtype = OCTAVE_REL;}
				
#line 2995 "lex/parser_music.cpp"
				
				goto _st4;
				_ctr42:
				{
#line 60 "lex/ragel_music.rl"
					spn.octtype = OCTAVE_REL;}
				
#line 3003 "lex/parser_music.cpp"
				
				goto _st4;
				_ctr72:
				{
#line 51 "lex/ragel_music.rl"
					spn.octtype = OCTAVE_ABS;}
				
#line 3011 "lex/parser_music.cpp"
				
				goto _st4;
				_st4:
				if ( p == eof ) {
					if ( cs >= 15 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof4;
				st_case_4:
				switch( ( (*( p))) ) {
					case 43: {
						goto _ctr11;
					}
					case 45: {
						goto _ctr11;
					}
				}
				{
					goto _st0;
				}
				_ctr11:
				{
#line 65 "lex/ragel_music.rl"
					spn.sign = ((( (*( p))))=='-') ? -1 : 1;}
				
#line 3041 "lex/parser_music.cpp"
				
				goto _st5;
				_st5:
				if ( p == eof ) {
					if ( cs >= 15 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof5;
				st_case_5:
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr13;
				}
				{
					goto _st0;
				}
				_ctr13:
				{
#line 66 "lex/ragel_music.rl"
					(spn.dev *= 10) += ((( (*( p)))) - '0');}
				
#line 3066 "lex/parser_music.cpp"
				
				goto _st6;
				_st6:
				if ( p == eof ) {
					if ( cs >= 15 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof6;
				st_case_6:
				if ( ( (*( p))) == 99 ) {
					goto _st8;
				}
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr15;
				}
				{
					goto _st0;
				}
				_ctr15:
				{
#line 66 "lex/ragel_music.rl"
					(spn.dev *= 10) += ((( (*( p)))) - '0');}
				
#line 3094 "lex/parser_music.cpp"
				
				goto _st7;
				_st7:
				if ( p == eof ) {
					if ( cs >= 15 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof7;
				st_case_7:
				if ( ( (*( p))) == 99 ) {
					goto _st8;
				}
				{
					goto _st0;
				}
				_st8:
				if ( p == eof ) {
					if ( cs >= 15 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof8;
				st_case_8:
				if ( ( (*( p))) == 41 ) {
					goto _ctr18;
				}
				{
					goto _st0;
				}
				_ctr18:
				{
#line 67 "lex/ragel_music.rl"
					spn.dev *= spn.sign;}
				
#line 3136 "lex/parser_music.cpp"
				
				goto _st18;
				_ctr49:
				{
#line 78 "lex/ragel_music.rl"
					ragel_cat = CAT_UNIT; ragel_type = TYPE_SPN;}
				
#line 3144 "lex/parser_music.cpp"
				
				{
#line 134 "lex/ragel_music.rl"
					note.num = 1; note.den = 4; note.dots = 0; note.durtype = DURATION_REL;}
				
#line 3150 "lex/parser_music.cpp"
				
				{
#line 135 "lex/ragel_music.rl"
					ragel_cat = CAT_UNIT; ragel_type = TYPE_SPN;}
				
#line 3156 "lex/parser_music.cpp"
				
				goto _st18;
				_st18:
				if ( p == eof ) {
					if ( cs >= 15 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof18;
				st_case_18:
				if ( ( (*( p))) == 124 ) {
					goto _ctr50;
				}
				{
					goto _st0;
				}
				_ctr39:
				{
#line 44 "lex/ragel_music.rl"
					spn.pitch = (spn.note * 2) - (spn.note > 2);}
				
#line 3181 "lex/parser_music.cpp"
				
				{
#line 60 "lex/ragel_music.rl"
					spn.octtype = OCTAVE_REL;}
				
#line 3187 "lex/parser_music.cpp"
				
				{
#line 78 "lex/ragel_music.rl"
					ragel_cat = CAT_UNIT; ragel_type = TYPE_SPN;}
				
#line 3193 "lex/parser_music.cpp"
				
				{
#line 134 "lex/ragel_music.rl"
					note.num = 1; note.den = 4; note.dots = 0; note.durtype = DURATION_REL;}
				
#line 3199 "lex/parser_music.cpp"
				
				goto _st9;
				_ctr47:
				{
#line 60 "lex/ragel_music.rl"
					spn.octtype = OCTAVE_REL;}
				
#line 3207 "lex/parser_music.cpp"
				
				{
#line 78 "lex/ragel_music.rl"
					ragel_cat = CAT_UNIT; ragel_type = TYPE_SPN;}
				
#line 3213 "lex/parser_music.cpp"
				
				{
#line 134 "lex/ragel_music.rl"
					note.num = 1; note.den = 4; note.dots = 0; note.durtype = DURATION_REL;}
				
#line 3219 "lex/parser_music.cpp"
				
				goto _st9;
				_ctr50:
				{
#line 78 "lex/ragel_music.rl"
					ragel_cat = CAT_UNIT; ragel_type = TYPE_SPN;}
				
#line 3227 "lex/parser_music.cpp"
				
				{
#line 134 "lex/ragel_music.rl"
					note.num = 1; note.den = 4; note.dots = 0; note.durtype = DURATION_REL;}
				
#line 3233 "lex/parser_music.cpp"
				
				goto _st9;
				_ctr73:
				{
#line 51 "lex/ragel_music.rl"
					spn.octtype = OCTAVE_ABS;}
				
#line 3241 "lex/parser_music.cpp"
				
				{
#line 78 "lex/ragel_music.rl"
					ragel_cat = CAT_UNIT; ragel_type = TYPE_SPN;}
				
#line 3247 "lex/parser_music.cpp"
				
				{
#line 134 "lex/ragel_music.rl"
					note.num = 1; note.den = 4; note.dots = 0; note.durtype = DURATION_REL;}
				
#line 3253 "lex/parser_music.cpp"
				
				goto _st9;
				_ctr94:
				{
#line 134 "lex/ragel_music.rl"
					note.num = 1; note.den = 4; note.dots = 0; note.durtype = DURATION_REL;}
				
#line 3261 "lex/parser_music.cpp"
				
				goto _st9;
				_st9:
				if ( p == eof ) {
					if ( cs >= 15 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof9;
				st_case_9:
				switch( ( (*( p))) ) {
					case 42: {
						goto _st10;
					}
					case 47: {
						goto _st12;
					}
				}
				if ( 49 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr22;
				}
				{
					goto _st0;
				}
				_st10:
				if ( p == eof ) {
					if ( cs >= 15 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof10;
				st_case_10:
				if ( 49 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr23;
				}
				{
					goto _st0;
				}
				_ctr23:
				{
#line 105 "lex/ragel_music.rl"
					note.num = 0; }
				
#line 3311 "lex/parser_music.cpp"
				
				{
#line 106 "lex/ragel_music.rl"
					(note.num *= 10) += ((( (*( p)))) - '0'); }
				
#line 3317 "lex/parser_music.cpp"
				
				goto _st19;
				_ctr51:
				{
#line 103 "lex/ragel_music.rl"
					note.den = 1; }
				
#line 3325 "lex/parser_music.cpp"
				
				{
#line 101 "lex/ragel_music.rl"
					note.durtype = DURATION_REL; }
				
#line 3331 "lex/parser_music.cpp"
				
				{
#line 135 "lex/ragel_music.rl"
					ragel_cat = CAT_UNIT; ragel_type = TYPE_SPN;}
				
#line 3337 "lex/parser_music.cpp"
				
				goto _st19;
				_ctr52:
				{
#line 106 "lex/ragel_music.rl"
					(note.num *= 10) += ((( (*( p)))) - '0'); }
				
#line 3345 "lex/parser_music.cpp"
				
				goto _st19;
				_st19:
				if ( p == eof ) {
					if ( cs >= 15 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof19;
				st_case_19:
				if ( ( (*( p))) == 47 ) {
					goto _st11;
				}
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr52;
				}
				{
					goto _st0;
				}
				_st11:
				if ( p == eof ) {
					if ( cs >= 15 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof11;
				st_case_11:
				if ( 49 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr25;
				}
				{
					goto _st0;
				}
				_ctr25:
				{
#line 107 "lex/ragel_music.rl"
					note.den = 0; }
				
#line 3390 "lex/parser_music.cpp"
				
				{
#line 108 "lex/ragel_music.rl"
					(note.den *= 10) += ((( (*( p)))) - '0'); }
				
#line 3396 "lex/parser_music.cpp"
				
				goto _st20;
				_ctr53:
				{
#line 101 "lex/ragel_music.rl"
					note.durtype = DURATION_REL; }
				
#line 3404 "lex/parser_music.cpp"
				
				{
#line 135 "lex/ragel_music.rl"
					ragel_cat = CAT_UNIT; ragel_type = TYPE_SPN;}
				
#line 3410 "lex/parser_music.cpp"
				
				goto _st20;
				_ctr54:
				{
#line 108 "lex/ragel_music.rl"
					(note.den *= 10) += ((( (*( p)))) - '0'); }
				
#line 3418 "lex/parser_music.cpp"
				
				goto _st20;
				_st20:
				if ( p == eof ) {
					if ( cs >= 15 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof20;
				st_case_20:
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr54;
				}
				{
					goto _st0;
				}
				_st12:
				if ( p == eof ) {
					if ( cs >= 15 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof12;
				st_case_12:
				if ( 49 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr26;
				}
				{
					goto _st0;
				}
				_ctr26:
				{
#line 107 "lex/ragel_music.rl"
					note.den = 0; }
				
#line 3460 "lex/parser_music.cpp"
				
				{
#line 108 "lex/ragel_music.rl"
					(note.den *= 10) += ((( (*( p)))) - '0'); }
				
#line 3466 "lex/parser_music.cpp"
				
				goto _st21;
				_ctr56:
				{
#line 108 "lex/ragel_music.rl"
					(note.den *= 10) += ((( (*( p)))) - '0'); }
				
#line 3474 "lex/parser_music.cpp"
				
				goto _st21;
				_ctr55:
				{
#line 102 "lex/ragel_music.rl"
					note.num = 1; }
				
#line 3482 "lex/parser_music.cpp"
				
				{
#line 101 "lex/ragel_music.rl"
					note.durtype = DURATION_REL; }
				
#line 3488 "lex/parser_music.cpp"
				
				{
#line 135 "lex/ragel_music.rl"
					ragel_cat = CAT_UNIT; ragel_type = TYPE_SPN;}
				
#line 3494 "lex/parser_music.cpp"
				
				goto _st21;
				_st21:
				if ( p == eof ) {
					if ( cs >= 15 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof21;
				st_case_21:
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr56;
				}
				{
					goto _st0;
				}
				_ctr22:
				{
#line 105 "lex/ragel_music.rl"
					note.num = 0; }
				
#line 3519 "lex/parser_music.cpp"
				
				{
#line 106 "lex/ragel_music.rl"
					(note.num *= 10) += ((( (*( p)))) - '0'); }
				
#line 3525 "lex/parser_music.cpp"
				
				{
#line 107 "lex/ragel_music.rl"
					note.den = 0; }
				
#line 3531 "lex/parser_music.cpp"
				
				{
#line 108 "lex/ragel_music.rl"
					(note.den *= 10) += ((( (*( p)))) - '0'); }
				
#line 3537 "lex/parser_music.cpp"
				
				goto _st22;
				_ctr57:
				{
#line 102 "lex/ragel_music.rl"
					note.num = 1; }
				
#line 3545 "lex/parser_music.cpp"
				
				{
#line 100 "lex/ragel_music.rl"
					note.durtype = DURATION_ABS; }
				
#line 3551 "lex/parser_music.cpp"
				
				{
#line 135 "lex/ragel_music.rl"
					ragel_cat = CAT_UNIT; ragel_type = TYPE_SPN;}
				
#line 3557 "lex/parser_music.cpp"
				
				goto _st22;
				_ctr59:
				{
#line 106 "lex/ragel_music.rl"
					(note.num *= 10) += ((( (*( p)))) - '0'); }
				
#line 3565 "lex/parser_music.cpp"
				
				{
#line 108 "lex/ragel_music.rl"
					(note.den *= 10) += ((( (*( p)))) - '0'); }
				
#line 3571 "lex/parser_music.cpp"
				
				goto _st22;
				_st22:
				if ( p == eof ) {
					if ( cs >= 15 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof22;
				st_case_22:
				switch( ( (*( p))) ) {
					case 46: {
						goto _ctr58;
					}
					case 47: {
						goto _st13;
					}
					case 95: {
						goto _ctr60;
					}
				}
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr59;
				}
				{
					goto _st0;
				}
				_ctr58:
				{
#line 102 "lex/ragel_music.rl"
					note.num = 1; }
				
#line 3607 "lex/parser_music.cpp"
				
				{
#line 85 "lex/ragel_music.rl"
					
					note.dots++;
					if(note.dots == 1) {
						note.num *= 3;
						note.den *= 2;
					} else if(note.dots == 2) {
						note.num = note.num / 3 * 7;
						note.den *= 2;
					} else if(note.dots == 3) {
						note.num = note.num / 7 * 15;
						note.den *= 2;
					}
				}
				
#line 3625 "lex/parser_music.cpp"
				
				goto _st23;
				_ctr61:
				{
#line 100 "lex/ragel_music.rl"
					note.durtype = DURATION_ABS; }
				
#line 3633 "lex/parser_music.cpp"
				
				{
#line 135 "lex/ragel_music.rl"
					ragel_cat = CAT_UNIT; ragel_type = TYPE_SPN;}
				
#line 3639 "lex/parser_music.cpp"
				
				goto _st23;
				_ctr67:
				{
#line 85 "lex/ragel_music.rl"
					
					note.dots++;
					if(note.dots == 1) {
						note.num *= 3;
						note.den *= 2;
					} else if(note.dots == 2) {
						note.num = note.num / 3 * 7;
						note.den *= 2;
					} else if(note.dots == 3) {
						note.num = note.num / 7 * 15;
						note.den *= 2;
					}
				}
				
#line 3659 "lex/parser_music.cpp"
				
				goto _st23;
				_st23:
				if ( p == eof ) {
					if ( cs >= 15 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof23;
				st_case_23:
				if ( ( (*( p))) == 46 ) {
					goto _ctr62;
				}
				{
					goto _st0;
				}
				_ctr63:
				{
#line 100 "lex/ragel_music.rl"
					note.durtype = DURATION_ABS; }
				
#line 3684 "lex/parser_music.cpp"
				
				{
#line 135 "lex/ragel_music.rl"
					ragel_cat = CAT_UNIT; ragel_type = TYPE_SPN;}
				
#line 3690 "lex/parser_music.cpp"
				
				goto _st24;
				_ctr62:
				{
#line 85 "lex/ragel_music.rl"
					
					note.dots++;
					if(note.dots == 1) {
						note.num *= 3;
						note.den *= 2;
					} else if(note.dots == 2) {
						note.num = note.num / 3 * 7;
						note.den *= 2;
					} else if(note.dots == 3) {
						note.num = note.num / 7 * 15;
						note.den *= 2;
					}
				}
				
#line 3710 "lex/parser_music.cpp"
				
				goto _st24;
				_st24:
				if ( p == eof ) {
					if ( cs >= 15 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof24;
				st_case_24:
				if ( ( (*( p))) == 46 ) {
					goto _ctr64;
				}
				{
					goto _st0;
				}
				_ctr65:
				{
#line 100 "lex/ragel_music.rl"
					note.durtype = DURATION_ABS; }
				
#line 3735 "lex/parser_music.cpp"
				
				{
#line 135 "lex/ragel_music.rl"
					ragel_cat = CAT_UNIT; ragel_type = TYPE_SPN;}
				
#line 3741 "lex/parser_music.cpp"
				
				goto _st25;
				_ctr64:
				{
#line 85 "lex/ragel_music.rl"
					
					note.dots++;
					if(note.dots == 1) {
						note.num *= 3;
						note.den *= 2;
					} else if(note.dots == 2) {
						note.num = note.num / 3 * 7;
						note.den *= 2;
					} else if(note.dots == 3) {
						note.num = note.num / 7 * 15;
						note.den *= 2;
					}
				}
				
#line 3761 "lex/parser_music.cpp"
				
				goto _st25;
				_st25:
				if ( p == eof ) {
					if ( cs >= 15 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof25;
				st_case_25:
				{
					goto _st0;
				}
				_st13:
				if ( p == eof ) {
					if ( cs >= 15 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof13;
				st_case_13:
				if ( 49 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr28;
				}
				{
					goto _st0;
				}
				_ctr28:
				{
#line 107 "lex/ragel_music.rl"
					note.den = 0; }
				
#line 3800 "lex/parser_music.cpp"
				
				{
#line 108 "lex/ragel_music.rl"
					(note.den *= 10) += ((( (*( p)))) - '0'); }
				
#line 3806 "lex/parser_music.cpp"
				
				goto _st26;
				_ctr68:
				{
#line 108 "lex/ragel_music.rl"
					(note.den *= 10) += ((( (*( p)))) - '0'); }
				
#line 3814 "lex/parser_music.cpp"
				
				goto _st26;
				_ctr66:
				{
#line 100 "lex/ragel_music.rl"
					note.durtype = DURATION_ABS; }
				
#line 3822 "lex/parser_music.cpp"
				
				{
#line 135 "lex/ragel_music.rl"
					ragel_cat = CAT_UNIT; ragel_type = TYPE_SPN;}
				
#line 3828 "lex/parser_music.cpp"
				
				goto _st26;
				_st26:
				if ( p == eof ) {
					if ( cs >= 15 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof26;
				st_case_26:
				switch( ( (*( p))) ) {
					case 46: {
						goto _ctr67;
					}
					case 95: {
						goto _st27;
					}
				}
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr68;
				}
				{
					goto _st0;
				}
				_ctr60:
				{
#line 102 "lex/ragel_music.rl"
					note.num = 1; }
				
#line 3861 "lex/parser_music.cpp"
				
				goto _st27;
				_ctr70:
				{
#line 100 "lex/ragel_music.rl"
					note.durtype = DURATION_ABS; }
				
#line 3869 "lex/parser_music.cpp"
				
				{
#line 135 "lex/ragel_music.rl"
					ragel_cat = CAT_UNIT; ragel_type = TYPE_SPN;}
				
#line 3875 "lex/parser_music.cpp"
				
				goto _st27;
				_st27:
				if ( p == eof ) {
					if ( cs >= 15 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof27;
				st_case_27:
				if ( ( (*( p))) == 46 ) {
					goto _ctr67;
				}
				{
					goto _st0;
				}
				_ctr34:
				{
#line 44 "lex/ragel_music.rl"
					spn.pitch = (spn.note * 2) - (spn.note > 2);}
				
#line 3900 "lex/parser_music.cpp"
				
				{
#line 60 "lex/ragel_music.rl"
					spn.octtype = OCTAVE_REL;}
				
#line 3906 "lex/parser_music.cpp"
				
				goto _st14;
				_ctr43:
				{
#line 60 "lex/ragel_music.rl"
					spn.octtype = OCTAVE_REL;}
				
#line 3914 "lex/parser_music.cpp"
				
				goto _st14;
				_st14:
				if ( p == eof ) {
					if ( cs >= 15 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof14;
				st_case_14:
				if ( ( (*( p))) == 49 ) {
					goto _ctr30;
				}
				{
					goto _st0;
				}
				_ctr30:
				{
#line 48 "lex/ragel_music.rl"
					spn.oct = -1;}
				
#line 3939 "lex/parser_music.cpp"
				
				goto _st28;
				_ctr35:
				{
#line 44 "lex/ragel_music.rl"
					spn.pitch = (spn.note * 2) - (spn.note > 2);}
				
#line 3947 "lex/parser_music.cpp"
				
				{
#line 60 "lex/ragel_music.rl"
					spn.octtype = OCTAVE_REL;}
				
#line 3953 "lex/parser_music.cpp"
				
				{
#line 49 "lex/ragel_music.rl"
					spn.oct = ((( (*( p)))) - '0');}
				
#line 3959 "lex/parser_music.cpp"
				
				goto _st28;
				_ctr44:
				{
#line 60 "lex/ragel_music.rl"
					spn.octtype = OCTAVE_REL;}
				
#line 3967 "lex/parser_music.cpp"
				
				{
#line 49 "lex/ragel_music.rl"
					spn.oct = ((( (*( p)))) - '0');}
				
#line 3973 "lex/parser_music.cpp"
				
				goto _st28;
				_ctr71:
				{
#line 51 "lex/ragel_music.rl"
					spn.octtype = OCTAVE_ABS;}
				
#line 3981 "lex/parser_music.cpp"
				
				{
#line 78 "lex/ragel_music.rl"
					ragel_cat = CAT_UNIT; ragel_type = TYPE_SPN;}
				
#line 3987 "lex/parser_music.cpp"
				
				{
#line 134 "lex/ragel_music.rl"
					note.num = 1; note.den = 4; note.dots = 0; note.durtype = DURATION_REL;}
				
#line 3993 "lex/parser_music.cpp"
				
				{
#line 135 "lex/ragel_music.rl"
					ragel_cat = CAT_UNIT; ragel_type = TYPE_SPN;}
				
#line 3999 "lex/parser_music.cpp"
				
				goto _st28;
				_st28:
				if ( p == eof ) {
					if ( cs >= 15 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof28;
				st_case_28:
				switch( ( (*( p))) ) {
					case 40: {
						goto _ctr72;
					}
					case 124: {
						goto _ctr73;
					}
				}
				{
					goto _st0;
				}
				_ctr36:
				{
#line 44 "lex/ragel_music.rl"
					spn.pitch = (spn.note * 2) - (spn.note > 2);}
				
#line 4029 "lex/parser_music.cpp"
				
				{
#line 60 "lex/ragel_music.rl"
					spn.octtype = OCTAVE_REL;}
				
#line 4035 "lex/parser_music.cpp"
				
				{
#line 55 "lex/ragel_music.rl"
					spn.oct++;}
				
#line 4041 "lex/parser_music.cpp"
				
				{
#line 58 "lex/ragel_music.rl"
					spn.octtype = OCTAVE_REL;}
				
#line 4047 "lex/parser_music.cpp"
				
				goto _st29;
				_ctr45:
				{
#line 60 "lex/ragel_music.rl"
					spn.octtype = OCTAVE_REL;}
				
#line 4055 "lex/parser_music.cpp"
				
				{
#line 55 "lex/ragel_music.rl"
					spn.oct++;}
				
#line 4061 "lex/parser_music.cpp"
				
				{
#line 58 "lex/ragel_music.rl"
					spn.octtype = OCTAVE_REL;}
				
#line 4067 "lex/parser_music.cpp"
				
				goto _st29;
				_ctr74:
				{
#line 78 "lex/ragel_music.rl"
					ragel_cat = CAT_UNIT; ragel_type = TYPE_SPN;}
				
#line 4075 "lex/parser_music.cpp"
				
				{
#line 134 "lex/ragel_music.rl"
					note.num = 1; note.den = 4; note.dots = 0; note.durtype = DURATION_REL;}
				
#line 4081 "lex/parser_music.cpp"
				
				{
#line 135 "lex/ragel_music.rl"
					ragel_cat = CAT_UNIT; ragel_type = TYPE_SPN;}
				
#line 4087 "lex/parser_music.cpp"
				
				goto _st29;
				_st29:
				if ( p == eof ) {
					if ( cs >= 15 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof29;
				st_case_29:
				switch( ( (*( p))) ) {
					case 40: {
						goto _st4;
					}
					case 94: {
						goto _ctr75;
					}
					case 124: {
						goto _ctr50;
					}
				}
				{
					goto _st0;
				}
				_ctr76:
				{
#line 78 "lex/ragel_music.rl"
					ragel_cat = CAT_UNIT; ragel_type = TYPE_SPN;}
				
#line 4120 "lex/parser_music.cpp"
				
				{
#line 134 "lex/ragel_music.rl"
					note.num = 1; note.den = 4; note.dots = 0; note.durtype = DURATION_REL;}
				
#line 4126 "lex/parser_music.cpp"
				
				{
#line 135 "lex/ragel_music.rl"
					ragel_cat = CAT_UNIT; ragel_type = TYPE_SPN;}
				
#line 4132 "lex/parser_music.cpp"
				
				goto _st30;
				_ctr75:
				{
#line 55 "lex/ragel_music.rl"
					spn.oct++;}
				
#line 4140 "lex/parser_music.cpp"
				
				{
#line 58 "lex/ragel_music.rl"
					spn.octtype = OCTAVE_REL;}
				
#line 4146 "lex/parser_music.cpp"
				
				goto _st30;
				_st30:
				if ( p == eof ) {
					if ( cs >= 15 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof30;
				st_case_30:
				switch( ( (*( p))) ) {
					case 40: {
						goto _st4;
					}
					case 94: {
						goto _ctr77;
					}
					case 124: {
						goto _ctr50;
					}
				}
				{
					goto _st0;
				}
				_ctr78:
				{
#line 78 "lex/ragel_music.rl"
					ragel_cat = CAT_UNIT; ragel_type = TYPE_SPN;}
				
#line 4179 "lex/parser_music.cpp"
				
				{
#line 134 "lex/ragel_music.rl"
					note.num = 1; note.den = 4; note.dots = 0; note.durtype = DURATION_REL;}
				
#line 4185 "lex/parser_music.cpp"
				
				{
#line 135 "lex/ragel_music.rl"
					ragel_cat = CAT_UNIT; ragel_type = TYPE_SPN;}
				
#line 4191 "lex/parser_music.cpp"
				
				goto _st31;
				_ctr77:
				{
#line 55 "lex/ragel_music.rl"
					spn.oct++;}
				
#line 4199 "lex/parser_music.cpp"
				
				{
#line 58 "lex/ragel_music.rl"
					spn.octtype = OCTAVE_REL;}
				
#line 4205 "lex/parser_music.cpp"
				
				goto _st31;
				_st31:
				if ( p == eof ) {
					if ( cs >= 15 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof31;
				st_case_31:
				switch( ( (*( p))) ) {
					case 40: {
						goto _st4;
					}
					case 94: {
						goto _ctr79;
					}
					case 124: {
						goto _ctr50;
					}
				}
				{
					goto _st0;
				}
				_ctr80:
				{
#line 78 "lex/ragel_music.rl"
					ragel_cat = CAT_UNIT; ragel_type = TYPE_SPN;}
				
#line 4238 "lex/parser_music.cpp"
				
				{
#line 134 "lex/ragel_music.rl"
					note.num = 1; note.den = 4; note.dots = 0; note.durtype = DURATION_REL;}
				
#line 4244 "lex/parser_music.cpp"
				
				{
#line 135 "lex/ragel_music.rl"
					ragel_cat = CAT_UNIT; ragel_type = TYPE_SPN;}
				
#line 4250 "lex/parser_music.cpp"
				
				goto _st32;
				_ctr79:
				{
#line 55 "lex/ragel_music.rl"
					spn.oct++;}
				
#line 4258 "lex/parser_music.cpp"
				
				{
#line 58 "lex/ragel_music.rl"
					spn.octtype = OCTAVE_REL;}
				
#line 4264 "lex/parser_music.cpp"
				
				goto _st32;
				_st32:
				if ( p == eof ) {
					if ( cs >= 15 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof32;
				st_case_32:
				switch( ( (*( p))) ) {
					case 40: {
						goto _st4;
					}
					case 94: {
						goto _ctr81;
					}
					case 124: {
						goto _ctr50;
					}
				}
				{
					goto _st0;
				}
				_ctr82:
				{
#line 78 "lex/ragel_music.rl"
					ragel_cat = CAT_UNIT; ragel_type = TYPE_SPN;}
				
#line 4297 "lex/parser_music.cpp"
				
				{
#line 134 "lex/ragel_music.rl"
					note.num = 1; note.den = 4; note.dots = 0; note.durtype = DURATION_REL;}
				
#line 4303 "lex/parser_music.cpp"
				
				{
#line 135 "lex/ragel_music.rl"
					ragel_cat = CAT_UNIT; ragel_type = TYPE_SPN;}
				
#line 4309 "lex/parser_music.cpp"
				
				goto _st33;
				_ctr81:
				{
#line 55 "lex/ragel_music.rl"
					spn.oct++;}
				
#line 4317 "lex/parser_music.cpp"
				
				{
#line 58 "lex/ragel_music.rl"
					spn.octtype = OCTAVE_REL;}
				
#line 4323 "lex/parser_music.cpp"
				
				goto _st33;
				_ctr90:
				{
#line 57 "lex/ragel_music.rl"
					spn.oct--;}
				
#line 4331 "lex/parser_music.cpp"
				
				{
#line 58 "lex/ragel_music.rl"
					spn.octtype = OCTAVE_REL;}
				
#line 4337 "lex/parser_music.cpp"
				
				goto _st33;
				_st33:
				if ( p == eof ) {
					if ( cs >= 15 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof33;
				st_case_33:
				switch( ( (*( p))) ) {
					case 40: {
						goto _st4;
					}
					case 124: {
						goto _ctr50;
					}
				}
				{
					goto _st0;
				}
				_ctr37:
				{
#line 44 "lex/ragel_music.rl"
					spn.pitch = (spn.note * 2) - (spn.note > 2);}
				
#line 4367 "lex/parser_music.cpp"
				
				{
#line 60 "lex/ragel_music.rl"
					spn.octtype = OCTAVE_REL;}
				
#line 4373 "lex/parser_music.cpp"
				
				{
#line 57 "lex/ragel_music.rl"
					spn.oct--;}
				
#line 4379 "lex/parser_music.cpp"
				
				{
#line 58 "lex/ragel_music.rl"
					spn.octtype = OCTAVE_REL;}
				
#line 4385 "lex/parser_music.cpp"
				
				goto _st34;
				_ctr46:
				{
#line 60 "lex/ragel_music.rl"
					spn.octtype = OCTAVE_REL;}
				
#line 4393 "lex/parser_music.cpp"
				
				{
#line 57 "lex/ragel_music.rl"
					spn.oct--;}
				
#line 4399 "lex/parser_music.cpp"
				
				{
#line 58 "lex/ragel_music.rl"
					spn.octtype = OCTAVE_REL;}
				
#line 4405 "lex/parser_music.cpp"
				
				goto _st34;
				_ctr83:
				{
#line 78 "lex/ragel_music.rl"
					ragel_cat = CAT_UNIT; ragel_type = TYPE_SPN;}
				
#line 4413 "lex/parser_music.cpp"
				
				{
#line 134 "lex/ragel_music.rl"
					note.num = 1; note.den = 4; note.dots = 0; note.durtype = DURATION_REL;}
				
#line 4419 "lex/parser_music.cpp"
				
				{
#line 135 "lex/ragel_music.rl"
					ragel_cat = CAT_UNIT; ragel_type = TYPE_SPN;}
				
#line 4425 "lex/parser_music.cpp"
				
				goto _st34;
				_st34:
				if ( p == eof ) {
					if ( cs >= 15 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof34;
				st_case_34:
				switch( ( (*( p))) ) {
					case 40: {
						goto _st4;
					}
					case 95: {
						goto _ctr84;
					}
					case 124: {
						goto _ctr50;
					}
				}
				{
					goto _st0;
				}
				_ctr85:
				{
#line 78 "lex/ragel_music.rl"
					ragel_cat = CAT_UNIT; ragel_type = TYPE_SPN;}
				
#line 4458 "lex/parser_music.cpp"
				
				{
#line 134 "lex/ragel_music.rl"
					note.num = 1; note.den = 4; note.dots = 0; note.durtype = DURATION_REL;}
				
#line 4464 "lex/parser_music.cpp"
				
				{
#line 135 "lex/ragel_music.rl"
					ragel_cat = CAT_UNIT; ragel_type = TYPE_SPN;}
				
#line 4470 "lex/parser_music.cpp"
				
				goto _st35;
				_ctr84:
				{
#line 57 "lex/ragel_music.rl"
					spn.oct--;}
				
#line 4478 "lex/parser_music.cpp"
				
				{
#line 58 "lex/ragel_music.rl"
					spn.octtype = OCTAVE_REL;}
				
#line 4484 "lex/parser_music.cpp"
				
				goto _st35;
				_st35:
				if ( p == eof ) {
					if ( cs >= 15 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof35;
				st_case_35:
				switch( ( (*( p))) ) {
					case 40: {
						goto _st4;
					}
					case 95: {
						goto _ctr86;
					}
					case 124: {
						goto _ctr50;
					}
				}
				{
					goto _st0;
				}
				_ctr87:
				{
#line 78 "lex/ragel_music.rl"
					ragel_cat = CAT_UNIT; ragel_type = TYPE_SPN;}
				
#line 4517 "lex/parser_music.cpp"
				
				{
#line 134 "lex/ragel_music.rl"
					note.num = 1; note.den = 4; note.dots = 0; note.durtype = DURATION_REL;}
				
#line 4523 "lex/parser_music.cpp"
				
				{
#line 135 "lex/ragel_music.rl"
					ragel_cat = CAT_UNIT; ragel_type = TYPE_SPN;}
				
#line 4529 "lex/parser_music.cpp"
				
				goto _st36;
				_ctr86:
				{
#line 57 "lex/ragel_music.rl"
					spn.oct--;}
				
#line 4537 "lex/parser_music.cpp"
				
				{
#line 58 "lex/ragel_music.rl"
					spn.octtype = OCTAVE_REL;}
				
#line 4543 "lex/parser_music.cpp"
				
				goto _st36;
				_st36:
				if ( p == eof ) {
					if ( cs >= 15 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof36;
				st_case_36:
				switch( ( (*( p))) ) {
					case 40: {
						goto _st4;
					}
					case 95: {
						goto _ctr88;
					}
					case 124: {
						goto _ctr50;
					}
				}
				{
					goto _st0;
				}
				_ctr89:
				{
#line 78 "lex/ragel_music.rl"
					ragel_cat = CAT_UNIT; ragel_type = TYPE_SPN;}
				
#line 4576 "lex/parser_music.cpp"
				
				{
#line 134 "lex/ragel_music.rl"
					note.num = 1; note.den = 4; note.dots = 0; note.durtype = DURATION_REL;}
				
#line 4582 "lex/parser_music.cpp"
				
				{
#line 135 "lex/ragel_music.rl"
					ragel_cat = CAT_UNIT; ragel_type = TYPE_SPN;}
				
#line 4588 "lex/parser_music.cpp"
				
				goto _st37;
				_ctr88:
				{
#line 57 "lex/ragel_music.rl"
					spn.oct--;}
				
#line 4596 "lex/parser_music.cpp"
				
				{
#line 58 "lex/ragel_music.rl"
					spn.octtype = OCTAVE_REL;}
				
#line 4602 "lex/parser_music.cpp"
				
				goto _st37;
				_st37:
				if ( p == eof ) {
					if ( cs >= 15 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof37;
				st_case_37:
				switch( ( (*( p))) ) {
					case 40: {
						goto _st4;
					}
					case 95: {
						goto _ctr90;
					}
					case 124: {
						goto _ctr50;
					}
				}
				{
					goto _st0;
				}
				_ctr38:
				{
#line 44 "lex/ragel_music.rl"
					spn.pitch = (spn.note * 2) - (spn.note > 2);}
				
#line 4635 "lex/parser_music.cpp"
				
				{
#line 39 "lex/ragel_music.rl"
					spn.alt = -1;}
				
#line 4641 "lex/parser_music.cpp"
				
				goto _st38;
				_ctr91:
				{
#line 60 "lex/ragel_music.rl"
					spn.octtype = OCTAVE_REL;}
				
#line 4649 "lex/parser_music.cpp"
				
				{
#line 78 "lex/ragel_music.rl"
					ragel_cat = CAT_UNIT; ragel_type = TYPE_SPN;}
				
#line 4655 "lex/parser_music.cpp"
				
				{
#line 134 "lex/ragel_music.rl"
					note.num = 1; note.den = 4; note.dots = 0; note.durtype = DURATION_REL;}
				
#line 4661 "lex/parser_music.cpp"
				
				{
#line 135 "lex/ragel_music.rl"
					ragel_cat = CAT_UNIT; ragel_type = TYPE_SPN;}
				
#line 4667 "lex/parser_music.cpp"
				
				goto _st38;
				_st38:
				if ( p == eof ) {
					if ( cs >= 15 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof38;
				st_case_38:
				switch( ( (*( p))) ) {
					case 40: {
						goto _ctr42;
					}
					case 45: {
						goto _ctr43;
					}
					case 94: {
						goto _ctr45;
					}
					case 95: {
						goto _ctr46;
					}
					case 98: {
						goto _ctr92;
					}
					case 124: {
						goto _ctr47;
					}
				}
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr44;
				}
				{
					goto _st0;
				}
				_ctr6:
				{
#line 131 "lex/ragel_music.rl"
					note.rest = 1;}
				
#line 4712 "lex/parser_music.cpp"
				
				goto _st39;
				_ctr93:
				{
#line 134 "lex/ragel_music.rl"
					note.num = 1; note.den = 4; note.dots = 0; note.durtype = DURATION_REL;}
				
#line 4720 "lex/parser_music.cpp"
				
				{
#line 135 "lex/ragel_music.rl"
					ragel_cat = CAT_UNIT; ragel_type = TYPE_SPN;}
				
#line 4726 "lex/parser_music.cpp"
				
				goto _st39;
				_st39:
				if ( p == eof ) {
					if ( cs >= 15 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof39;
				st_case_39:
				if ( ( (*( p))) == 124 ) {
					goto _ctr94;
				}
				{
					goto _st0;
				}
				st_out:
				_test_eof1: cs = 1; goto _test_eof; 
				_test_eof2: cs = 2; goto _test_eof; 
				_test_eof3: cs = 3; goto _test_eof; 
				_test_eof15: cs = 15; goto _test_eof; 
				_test_eof16: cs = 16; goto _test_eof; 
				_test_eof17: cs = 17; goto _test_eof; 
				_test_eof4: cs = 4; goto _test_eof; 
				_test_eof5: cs = 5; goto _test_eof; 
				_test_eof6: cs = 6; goto _test_eof; 
				_test_eof7: cs = 7; goto _test_eof; 
				_test_eof8: cs = 8; goto _test_eof; 
				_test_eof18: cs = 18; goto _test_eof; 
				_test_eof9: cs = 9; goto _test_eof; 
				_test_eof10: cs = 10; goto _test_eof; 
				_test_eof19: cs = 19; goto _test_eof; 
				_test_eof11: cs = 11; goto _test_eof; 
				_test_eof20: cs = 20; goto _test_eof; 
				_test_eof12: cs = 12; goto _test_eof; 
				_test_eof21: cs = 21; goto _test_eof; 
				_test_eof22: cs = 22; goto _test_eof; 
				_test_eof23: cs = 23; goto _test_eof; 
				_test_eof24: cs = 24; goto _test_eof; 
				_test_eof25: cs = 25; goto _test_eof; 
				_test_eof13: cs = 13; goto _test_eof; 
				_test_eof26: cs = 26; goto _test_eof; 
				_test_eof27: cs = 27; goto _test_eof; 
				_test_eof14: cs = 14; goto _test_eof; 
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
						case 15: {
							break;
						}
						case 16: {
							break;
						}
						case 17: {
							break;
						}
						case 4: {
							break;
						}
						case 5: {
							break;
						}
						case 6: {
							break;
						}
						case 7: {
							break;
						}
						case 8: {
							break;
						}
						case 18: {
							break;
						}
						case 9: {
							break;
						}
						case 10: {
							break;
						}
						case 19: {
							break;
						}
						case 11: {
							break;
						}
						case 20: {
							break;
						}
						case 12: {
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
						case 13: {
							break;
						}
						case 26: {
							break;
						}
						case 27: {
							break;
						}
						case 14: {
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
						case 31: {
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
						case 37: {
							break;
						}
						case 38: {
							break;
						}
						case 39: {
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
						goto _st3;case 15:
						goto _ctr31;case 16:
						goto _ctr40;case 17:
						goto _ctr48;case 4:
						goto _st4;case 5:
						goto _st5;case 6:
						goto _st6;case 7:
						goto _st7;case 8:
						goto _st8;case 18:
						goto _ctr49;case 9:
						goto _st9;case 10:
						goto _st10;case 19:
						goto _ctr51;case 11:
						goto _st11;case 20:
						goto _ctr53;case 12:
						goto _st12;case 21:
						goto _ctr55;case 22:
						goto _ctr57;case 23:
						goto _ctr61;case 24:
						goto _ctr63;case 25:
						goto _ctr65;case 13:
						goto _st13;case 26:
						goto _ctr66;case 27:
						goto _ctr70;case 14:
						goto _st14;case 28:
						goto _ctr71;case 29:
						goto _ctr74;case 30:
						goto _ctr76;case 31:
						goto _ctr78;case 32:
						goto _ctr80;case 33:
						goto _ctr82;case 34:
						goto _ctr83;case 35:
						goto _ctr85;case 36:
						goto _ctr87;case 37:
						goto _ctr89;case 38:
						goto _ctr91;case 39:
						goto _ctr93;	}
				}
				
				if ( cs >= 15 )
					goto _out; _pop: {}
				_out: {}
			}
			
#line 254 "lex/parser_music.rl"
			
			
			const bool ok = cs >= 
#line 4966 "lex/parser_music.cpp"
			15
#line 256 "lex/parser_music.rl"
			;
			
			if (ok)
				note_ = Notation(spn, note);
			
			
			return ok;
		}
		
		bool NotationSingle::parse(const Atom& a)
		{
			if (a.isSymbol())
				return parse(a.asT<t_symbol*>()->s_name);
			else
				return false;
		}
		
		size_t NotationSingle::parse(const AtomListView& lv, NoteVec& out)
		{
			const size_t N = lv.size();
			
			for (size_t i = 0; i < N; i++) {
				if (!parse(lv[i]))
					return i;
				
				out.push_back(note_);
				if (note_.dur.repeats > 1) {
					std::cerr << note_.dur.repeats << "\n";
					out.insert(out.end(), note_.dur.repeats - 1, out.back());
					note_.dur.repeats = 1;
				}
			}
			
			return N;
		}
		
		
#line 5006 "lex/parser_music.cpp"
		static const int duration_start = 1;
		static const int duration_first_final = 4;
		static const int duration_error = 0;
		
		static const int duration_en_main = 1;
		
		
#line 298 "lex/parser_music.rl"
		
		
		DurationFullMatch::DurationFullMatch()
		{
			reset();
		}
		
		void DurationFullMatch::reset()
		{
			dur_ = {};
		}
		
		bool DurationFullMatch::parse(const char* str)
		{
			const auto len = strlen(str);
			if (len == 0)
				return false;
			
			const char* p = str;
			const char* pe = p + len;
			const char* eof = pe;
			int cat_ = 0;
			AtomType type_;
			fsm::NotationData note;
			
			reset();
			
			
#line 5043 "lex/parser_music.cpp"
			{
				cs = (int)duration_start;
			}
			
#line 325 "lex/parser_music.rl"
			
			
#line 5051 "lex/parser_music.cpp"
			{
				if ( p == pe )
					goto _test_eof;
				switch ( cs ) {
					case 1:
					goto st_case_1;
					case 0:
					goto st_case_0;
					case 4:
					goto st_case_4;
					case 2:
					goto st_case_2;
					case 5:
					goto st_case_5;
					case 6:
					goto st_case_6;
					case 7:
					goto st_case_7;
					case 8:
					goto st_case_8;
					case 3:
					goto st_case_3;
					case 9:
					goto st_case_9;
					case 10:
					goto st_case_10;
				}
				goto st_out;
				_st1:
				if ( p == eof ) {
					if ( cs >= 4 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof1;
				st_case_1:
				if ( 49 <= ( (*( p))) && ( (*( p))) <= 57 ) {
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
#line 105 "lex/ragel_music.rl"
					note.num = 0; }
				
#line 5106 "lex/parser_music.cpp"
				
				{
#line 106 "lex/ragel_music.rl"
					(note.num *= 10) += ((( (*( p)))) - '0'); }
				
#line 5112 "lex/parser_music.cpp"
				
				{
#line 107 "lex/ragel_music.rl"
					note.den = 0; }
				
#line 5118 "lex/parser_music.cpp"
				
				{
#line 108 "lex/ragel_music.rl"
					(note.den *= 10) += ((( (*( p)))) - '0'); }
				
#line 5124 "lex/parser_music.cpp"
				
				goto _st4;
				_ctr7:
				{
#line 102 "lex/ragel_music.rl"
					note.num = 1; }
				
#line 5132 "lex/parser_music.cpp"
				
				{
#line 100 "lex/ragel_music.rl"
					note.durtype = DURATION_ABS; }
				
#line 5138 "lex/parser_music.cpp"
				
				goto _st4;
				_ctr9:
				{
#line 106 "lex/ragel_music.rl"
					(note.num *= 10) += ((( (*( p)))) - '0'); }
				
#line 5146 "lex/parser_music.cpp"
				
				{
#line 108 "lex/ragel_music.rl"
					(note.den *= 10) += ((( (*( p)))) - '0'); }
				
#line 5152 "lex/parser_music.cpp"
				
				goto _st4;
				_st4:
				if ( p == eof ) {
					if ( cs >= 4 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof4;
				st_case_4:
				switch( ( (*( p))) ) {
					case 42: {
						goto _st2;
					}
					case 46: {
						goto _ctr8;
					}
					case 47: {
						goto _st3;
					}
					case 95: {
						goto _ctr10;
					}
				}
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr9;
				}
				{
					goto _st0;
				}
				_st2:
				if ( p == eof ) {
					if ( cs >= 4 )
						goto _out;
					else
						goto _pop;
				}
				{
#line 104 "lex/ragel_music.rl"
					note.repeats = note.num; }
				
#line 5197 "lex/parser_music.cpp"
				
				p+= 1;
				if ( p == pe )
					goto _test_eof2;
				st_case_2:
				if ( 49 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr4;
				}
				{
					goto _st0;
				}
				_ctr4:
				{
#line 105 "lex/ragel_music.rl"
					note.num = 0; }
				
#line 5214 "lex/parser_music.cpp"
				
				{
#line 106 "lex/ragel_music.rl"
					(note.num *= 10) += ((( (*( p)))) - '0'); }
				
#line 5220 "lex/parser_music.cpp"
				
				{
#line 107 "lex/ragel_music.rl"
					note.den = 0; }
				
#line 5226 "lex/parser_music.cpp"
				
				{
#line 108 "lex/ragel_music.rl"
					(note.den *= 10) += ((( (*( p)))) - '0'); }
				
#line 5232 "lex/parser_music.cpp"
				
				goto _st5;
				_ctr11:
				{
#line 102 "lex/ragel_music.rl"
					note.num = 1; }
				
#line 5240 "lex/parser_music.cpp"
				
				{
#line 100 "lex/ragel_music.rl"
					note.durtype = DURATION_ABS; }
				
#line 5246 "lex/parser_music.cpp"
				
				goto _st5;
				_ctr12:
				{
#line 106 "lex/ragel_music.rl"
					(note.num *= 10) += ((( (*( p)))) - '0'); }
				
#line 5254 "lex/parser_music.cpp"
				
				{
#line 108 "lex/ragel_music.rl"
					(note.den *= 10) += ((( (*( p)))) - '0'); }
				
#line 5260 "lex/parser_music.cpp"
				
				goto _st5;
				_st5:
				if ( p == eof ) {
					if ( cs >= 4 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof5;
				st_case_5:
				switch( ( (*( p))) ) {
					case 46: {
						goto _ctr8;
					}
					case 47: {
						goto _st3;
					}
					case 95: {
						goto _ctr10;
					}
				}
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr12;
				}
				{
					goto _st0;
				}
				_ctr8:
				{
#line 102 "lex/ragel_music.rl"
					note.num = 1; }
				
#line 5296 "lex/parser_music.cpp"
				
				{
#line 85 "lex/ragel_music.rl"
					
					note.dots++;
					if(note.dots == 1) {
						note.num *= 3;
						note.den *= 2;
					} else if(note.dots == 2) {
						note.num = note.num / 3 * 7;
						note.den *= 2;
					} else if(note.dots == 3) {
						note.num = note.num / 7 * 15;
						note.den *= 2;
					}
				}
				
#line 5314 "lex/parser_music.cpp"
				
				goto _st6;
				_ctr13:
				{
#line 100 "lex/ragel_music.rl"
					note.durtype = DURATION_ABS; }
				
#line 5322 "lex/parser_music.cpp"
				
				goto _st6;
				_ctr19:
				{
#line 85 "lex/ragel_music.rl"
					
					note.dots++;
					if(note.dots == 1) {
						note.num *= 3;
						note.den *= 2;
					} else if(note.dots == 2) {
						note.num = note.num / 3 * 7;
						note.den *= 2;
					} else if(note.dots == 3) {
						note.num = note.num / 7 * 15;
						note.den *= 2;
					}
				}
				
#line 5342 "lex/parser_music.cpp"
				
				goto _st6;
				_st6:
				if ( p == eof ) {
					if ( cs >= 4 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof6;
				st_case_6:
				if ( ( (*( p))) == 46 ) {
					goto _ctr14;
				}
				{
					goto _st0;
				}
				_ctr15:
				{
#line 100 "lex/ragel_music.rl"
					note.durtype = DURATION_ABS; }
				
#line 5367 "lex/parser_music.cpp"
				
				goto _st7;
				_ctr14:
				{
#line 85 "lex/ragel_music.rl"
					
					note.dots++;
					if(note.dots == 1) {
						note.num *= 3;
						note.den *= 2;
					} else if(note.dots == 2) {
						note.num = note.num / 3 * 7;
						note.den *= 2;
					} else if(note.dots == 3) {
						note.num = note.num / 7 * 15;
						note.den *= 2;
					}
				}
				
#line 5387 "lex/parser_music.cpp"
				
				goto _st7;
				_st7:
				if ( p == eof ) {
					if ( cs >= 4 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof7;
				st_case_7:
				if ( ( (*( p))) == 46 ) {
					goto _ctr16;
				}
				{
					goto _st0;
				}
				_ctr17:
				{
#line 100 "lex/ragel_music.rl"
					note.durtype = DURATION_ABS; }
				
#line 5412 "lex/parser_music.cpp"
				
				goto _st8;
				_ctr16:
				{
#line 85 "lex/ragel_music.rl"
					
					note.dots++;
					if(note.dots == 1) {
						note.num *= 3;
						note.den *= 2;
					} else if(note.dots == 2) {
						note.num = note.num / 3 * 7;
						note.den *= 2;
					} else if(note.dots == 3) {
						note.num = note.num / 7 * 15;
						note.den *= 2;
					}
				}
				
#line 5432 "lex/parser_music.cpp"
				
				goto _st8;
				_st8:
				if ( p == eof ) {
					if ( cs >= 4 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof8;
				st_case_8:
				{
					goto _st0;
				}
				_st3:
				if ( p == eof ) {
					if ( cs >= 4 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof3;
				st_case_3:
				if ( 49 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr6;
				}
				{
					goto _st0;
				}
				_ctr6:
				{
#line 107 "lex/ragel_music.rl"
					note.den = 0; }
				
#line 5471 "lex/parser_music.cpp"
				
				{
#line 108 "lex/ragel_music.rl"
					(note.den *= 10) += ((( (*( p)))) - '0'); }
				
#line 5477 "lex/parser_music.cpp"
				
				goto _st9;
				_ctr18:
				{
#line 100 "lex/ragel_music.rl"
					note.durtype = DURATION_ABS; }
				
#line 5485 "lex/parser_music.cpp"
				
				goto _st9;
				_ctr20:
				{
#line 108 "lex/ragel_music.rl"
					(note.den *= 10) += ((( (*( p)))) - '0'); }
				
#line 5493 "lex/parser_music.cpp"
				
				goto _st9;
				_st9:
				if ( p == eof ) {
					if ( cs >= 4 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof9;
				st_case_9:
				switch( ( (*( p))) ) {
					case 46: {
						goto _ctr19;
					}
					case 95: {
						goto _st10;
					}
				}
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr20;
				}
				{
					goto _st0;
				}
				_ctr10:
				{
#line 102 "lex/ragel_music.rl"
					note.num = 1; }
				
#line 5526 "lex/parser_music.cpp"
				
				goto _st10;
				_ctr22:
				{
#line 100 "lex/ragel_music.rl"
					note.durtype = DURATION_ABS; }
				
#line 5534 "lex/parser_music.cpp"
				
				goto _st10;
				_st10:
				if ( p == eof ) {
					if ( cs >= 4 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof10;
				st_case_10:
				if ( ( (*( p))) == 46 ) {
					goto _ctr19;
				}
				{
					goto _st0;
				}
				st_out:
				_test_eof1: cs = 1; goto _test_eof; 
				_test_eof4: cs = 4; goto _test_eof; 
				_test_eof2: cs = 2; goto _test_eof; 
				_test_eof5: cs = 5; goto _test_eof; 
				_test_eof6: cs = 6; goto _test_eof; 
				_test_eof7: cs = 7; goto _test_eof; 
				_test_eof8: cs = 8; goto _test_eof; 
				_test_eof3: cs = 3; goto _test_eof; 
				_test_eof9: cs = 9; goto _test_eof; 
				_test_eof10: cs = 10; goto _test_eof; 
				
				_test_eof: {}
				if ( p == eof ) {
					switch ( cs ) {
						case 1: {
							break;
						}
						case 0: {
							break;
						}
						case 4: {
							break;
						}
						case 2: {
							break;
						}
						case 5: {
							break;
						}
						case 6: {
							break;
						}
						case 7: {
							break;
						}
						case 8: {
							break;
						}
						case 3: {
							break;
						}
						case 9: {
							break;
						}
						case 10: {
							break;
						}
					}
					switch ( cs ) {
					}
					switch ( cs ) {
						case 1:
						goto _st1;case 0:
						goto _st0;case 4:
						goto _ctr7;case 2:
						goto _st2;case 5:
						goto _ctr11;case 6:
						goto _ctr13;case 7:
						goto _ctr15;case 8:
						goto _ctr17;case 3:
						goto _st3;case 9:
						goto _ctr18;case 10:
						goto _ctr22;	}
				}
				
				if ( cs >= 4 )
					goto _out; _pop: {}
				_out: {}
			}
			
#line 326 "lex/parser_music.rl"
			
			
			const bool ok = cs >= 
#line 5629 "lex/parser_music.cpp"
			4
#line 328 "lex/parser_music.rl"
			;
			
			if (ok)
				dur_ = note;
			
			return ok;
		}
		
		bool DurationFullMatch::parse(const Atom& a)
		{
			if (a.isSymbol())
				return parse(a.asT<t_symbol*>()->s_name);
			else if(a.isFloat()) {
				const auto i = a.asT<int>();
				if (i >= 0)
					dur_ = Duration(1, i);
				
				return i >= 0;
			}
			else
				return false;
		}
		
		size_t DurationFullMatch::parse(const AtomListView& lv, DurationVec& out)
		{
			const size_t N = lv.size();
			
			for (size_t i = 0; i < N; i++) {
				if (!parse(lv[i]))
					return i;
				
				out.push_back(dur_);
				if(dur_.repeats > 1) {
					out.insert(out.end(), dur_.repeats - 1, out.back());
					dur_.repeats = 1;
				}
			}
			
			return N;
		}
		
		
	}
}


