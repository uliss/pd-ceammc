#line 1 "args/argcheck2.rl"
# include "argcheck2.h"
# include "ceammc_string.h"
# include "fmt/core.h"

# include <cstdint>
# include <iostream>

#define ARG_DEBUG

#ifdef ARG_DEBUG
#define debug(prefix, arg) std::cerr << " - " << prefix << ": " << arg << std::endl;
#else
#define debug(prefix, arg)
#endif

namespace {
	constexpr int16_t REPEAT_INF = -1;
	
	enum CheckType {
		CHECK_ATOM,
		CHECK_BOOL,
		CHECK_BYTE,
	};
	
	
	std::string atom_to_string(const ceammc::Atom& a) {
		ceammc::string::SmallString buf;
		if (ceammc::string::atom_to_string(a, buf))
			return { buf.data(), buf.size() };
		else
			return "?";
	}
	
	std::string list_to_string(const ceammc::AtomListView& lv) {
		ceammc::string::MediumString buf;
		if (ceammc::string::list_to_string(lv, buf))
			return { buf.data(), buf.size() };
		else
			return "?";
	}
}


#line 46 "args/argcheck2.cpp"
static const int arg_check2_start = 1;
static const int arg_check2_first_final = 11;
static const int arg_check2_error = 0;

static const int arg_check2_en_main = 1;


#line 127 "args/argcheck2.rl"


namespace ceammc {
	namespace args {
		
		bool check_args(const char* arg_string, const AtomListView& lv, std::ostream& err)
		{
			const int N = lv.size();
			int ca = 0;
			int cs = 0;
			const char* p = arg_string;
			CheckType rl_type = CHECK_ATOM;
			int rl_min = 0;
			int rl_max = 0;
			
			
#line 71 "args/argcheck2.cpp"
			{
				cs = (int)arg_check2_start;
			}
			
#line 76 "args/argcheck2.cpp"
			{
				switch ( cs ) {
					case 1:
					goto st_case_1;
					case 0:
					goto st_case_0;
					case 2:
					goto st_case_2;
					case 11:
					goto st_case_11;
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
				}
				goto st_out;
				_ctr7:
				{
#line 118 "args/argcheck2.rl"
					rl_min = 1; rl_max = REPEAT_INF; }
				
#line 110 "args/argcheck2.cpp"
				
				{
#line 46 "args/argcheck2.rl"
					
					debug("min", rl_min);
					debug("max", rl_max);
					
					auto cur = ca + (rl_min-1);
					if (cur >= N) {
						switch (rl_type) {
							case CHECK_ATOM:
							err << fmt::format("atom expected at position [{}]", cur);
							return false;
							case CHECK_BOOL:
							err << fmt::format("bool expected at position [{}]", cur);
							return false;
							case CHECK_BYTE:
							err << fmt::format("int[0..255] expected at position [{}]", cur);
							return false;
							default:
							err << fmt::format("error at position [{}]", cur);
							return false;
						}
					}
					
					const auto rep_max = (rl_max == REPEAT_INF) ? 1000000 : rl_max;
					const auto TOTAL = std::min<int>(N, ca + rep_max);
					for (int i = ca; i < TOTAL; i++) {
						const auto& a = lv[i];
						switch (rl_type) {
							case CHECK_ATOM:
							debug("atom", "Ok");
							ca++;
							break;
							case CHECK_BOOL:
							if (a.isBool()) {
								debug("book", "Ok");
								ca++;
							} else {
								err << fmt::format("bool expected at position [{}]: '{}'", ca, atom_to_string(a));
								return false;
							}
							break;
							case CHECK_BYTE:
							if (!a.isInteger() || a.asT<int>() < 0 || a.asT<int>() > 255) {
								err << fmt::format("not a byte value at position [{}]: '{}'", ca, atom_to_string(a));
								return false;
							} else {
								debug("byte", "Ok");
								ca++;
							}
							break;
						}
						
					}
				}
				
#line 168 "args/argcheck2.cpp"
				
				goto _st1;
				_ctr14:
				{
#line 46 "args/argcheck2.rl"
					
					debug("min", rl_min);
					debug("max", rl_max);
					
					auto cur = ca + (rl_min-1);
					if (cur >= N) {
						switch (rl_type) {
							case CHECK_ATOM:
							err << fmt::format("atom expected at position [{}]", cur);
							return false;
							case CHECK_BOOL:
							err << fmt::format("bool expected at position [{}]", cur);
							return false;
							case CHECK_BYTE:
							err << fmt::format("int[0..255] expected at position [{}]", cur);
							return false;
							default:
							err << fmt::format("error at position [{}]", cur);
							return false;
						}
					}
					
					const auto rep_max = (rl_max == REPEAT_INF) ? 1000000 : rl_max;
					const auto TOTAL = std::min<int>(N, ca + rep_max);
					for (int i = ca; i < TOTAL; i++) {
						const auto& a = lv[i];
						switch (rl_type) {
							case CHECK_ATOM:
							debug("atom", "Ok");
							ca++;
							break;
							case CHECK_BOOL:
							if (a.isBool()) {
								debug("book", "Ok");
								ca++;
							} else {
								err << fmt::format("bool expected at position [{}]: '{}'", ca, atom_to_string(a));
								return false;
							}
							break;
							case CHECK_BYTE:
							if (!a.isInteger() || a.asT<int>() < 0 || a.asT<int>() > 255) {
								err << fmt::format("not a byte value at position [{}]: '{}'", ca, atom_to_string(a));
								return false;
							} else {
								debug("byte", "Ok");
								ca++;
							}
							break;
						}
						
					}
				}
				
#line 228 "args/argcheck2.cpp"
				
				goto _st1;
				_st1:
				p += 1;
				st_case_1:
				switch( ( (*( p))) ) {
					case 66: {
						goto _ctr2;
					}
					case 97: {
						goto _ctr3;
					}
					case 98: {
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
#line 107 "args/argcheck2.rl"
					rl_type = CHECK_BOOL; }
				
#line 257 "args/argcheck2.cpp"
				
				goto _st2;
				_ctr3:
				{
#line 106 "args/argcheck2.rl"
					rl_type = CHECK_ATOM; }
				
#line 265 "args/argcheck2.cpp"
				
				goto _st2;
				_ctr4:
				{
#line 108 "args/argcheck2.rl"
					rl_type = CHECK_BYTE; }
				
#line 273 "args/argcheck2.cpp"
				
				goto _st2;
				_st2:
				p += 1;
				st_case_2:
				switch( ( (*( p))) ) {
					case 0: {
						goto _ctr6;
					}
					case 32: {
						goto _ctr7;
					}
					case 42: {
						goto _ctr8;
					}
					case 43: {
						goto _ctr9;
					}
					case 63: {
						goto _ctr10;
					}
					case 91: {
						goto _ctr11;
					}
				}
				{
					goto _st0;
				}
				_ctr6:
				{
#line 118 "args/argcheck2.rl"
					rl_min = 1; rl_max = REPEAT_INF; }
				
#line 307 "args/argcheck2.cpp"
				
				{
#line 46 "args/argcheck2.rl"
					
					debug("min", rl_min);
					debug("max", rl_max);
					
					auto cur = ca + (rl_min-1);
					if (cur >= N) {
						switch (rl_type) {
							case CHECK_ATOM:
							err << fmt::format("atom expected at position [{}]", cur);
							return false;
							case CHECK_BOOL:
							err << fmt::format("bool expected at position [{}]", cur);
							return false;
							case CHECK_BYTE:
							err << fmt::format("int[0..255] expected at position [{}]", cur);
							return false;
							default:
							err << fmt::format("error at position [{}]", cur);
							return false;
						}
					}
					
					const auto rep_max = (rl_max == REPEAT_INF) ? 1000000 : rl_max;
					const auto TOTAL = std::min<int>(N, ca + rep_max);
					for (int i = ca; i < TOTAL; i++) {
						const auto& a = lv[i];
						switch (rl_type) {
							case CHECK_ATOM:
							debug("atom", "Ok");
							ca++;
							break;
							case CHECK_BOOL:
							if (a.isBool()) {
								debug("book", "Ok");
								ca++;
							} else {
								err << fmt::format("bool expected at position [{}]: '{}'", ca, atom_to_string(a));
								return false;
							}
							break;
							case CHECK_BYTE:
							if (!a.isInteger() || a.asT<int>() < 0 || a.asT<int>() > 255) {
								err << fmt::format("not a byte value at position [{}]: '{}'", ca, atom_to_string(a));
								return false;
							} else {
								debug("byte", "Ok");
								ca++;
							}
							break;
						}
						
					}
				}
				
#line 365 "args/argcheck2.cpp"
				
				{
#line 123 "args/argcheck2.rl"
					{p+= 1; cs = 11; goto _out;} }
				
#line 371 "args/argcheck2.cpp"
				
				goto _st11;
				_ctr13:
				{
#line 46 "args/argcheck2.rl"
					
					debug("min", rl_min);
					debug("max", rl_max);
					
					auto cur = ca + (rl_min-1);
					if (cur >= N) {
						switch (rl_type) {
							case CHECK_ATOM:
							err << fmt::format("atom expected at position [{}]", cur);
							return false;
							case CHECK_BOOL:
							err << fmt::format("bool expected at position [{}]", cur);
							return false;
							case CHECK_BYTE:
							err << fmt::format("int[0..255] expected at position [{}]", cur);
							return false;
							default:
							err << fmt::format("error at position [{}]", cur);
							return false;
						}
					}
					
					const auto rep_max = (rl_max == REPEAT_INF) ? 1000000 : rl_max;
					const auto TOTAL = std::min<int>(N, ca + rep_max);
					for (int i = ca; i < TOTAL; i++) {
						const auto& a = lv[i];
						switch (rl_type) {
							case CHECK_ATOM:
							debug("atom", "Ok");
							ca++;
							break;
							case CHECK_BOOL:
							if (a.isBool()) {
								debug("book", "Ok");
								ca++;
							} else {
								err << fmt::format("bool expected at position [{}]: '{}'", ca, atom_to_string(a));
								return false;
							}
							break;
							case CHECK_BYTE:
							if (!a.isInteger() || a.asT<int>() < 0 || a.asT<int>() > 255) {
								err << fmt::format("not a byte value at position [{}]: '{}'", ca, atom_to_string(a));
								return false;
							} else {
								debug("byte", "Ok");
								ca++;
							}
							break;
						}
						
					}
				}
				
#line 431 "args/argcheck2.cpp"
				
				{
#line 123 "args/argcheck2.rl"
					{p+= 1; cs = 11; goto _out;} }
				
#line 437 "args/argcheck2.cpp"
				
				goto _st11;
				_st11:
				p += 1;
				st_case_11:
				{
					goto _st0;
				}
				_ctr8:
				{
#line 118 "args/argcheck2.rl"
					rl_min = 1; rl_max = REPEAT_INF; }
				
#line 451 "args/argcheck2.cpp"
				
				{
#line 111 "args/argcheck2.rl"
					rl_min = 0; rl_max = REPEAT_INF; }
				
#line 457 "args/argcheck2.cpp"
				
				goto _st3;
				_ctr9:
				{
#line 118 "args/argcheck2.rl"
					rl_min = 1; rl_max = REPEAT_INF; }
				
#line 465 "args/argcheck2.cpp"
				
				{
#line 110 "args/argcheck2.rl"
					rl_min = 1, rl_max = REPEAT_INF; }
				
#line 471 "args/argcheck2.cpp"
				
				goto _st3;
				_ctr10:
				{
#line 118 "args/argcheck2.rl"
					rl_min = 1; rl_max = REPEAT_INF; }
				
#line 479 "args/argcheck2.cpp"
				
				{
#line 109 "args/argcheck2.rl"
					rl_min = 0; rl_max = 1; }
				
#line 485 "args/argcheck2.cpp"
				
				goto _st3;
				_ctr20:
				{
#line 102 "args/argcheck2.rl"
					rl_max = rl_min; }
				
#line 493 "args/argcheck2.cpp"
				
				goto _st3;
				_st3:
				p += 1;
				st_case_3:
				switch( ( (*( p))) ) {
					case 0: {
						goto _ctr13;
					}
					case 32: {
						goto _ctr14;
					}
				}
				{
					goto _st0;
				}
				_ctr11:
				{
#line 118 "args/argcheck2.rl"
					rl_min = 1; rl_max = REPEAT_INF; }
				
#line 515 "args/argcheck2.cpp"
				
				goto _st4;
				_st4:
				p += 1;
				st_case_4:
				if ( ( (*( p))) == 48 ) {
					goto _ctr16;
				}
				if ( 49 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr17;
				}
				{
					goto _st0;
				}
				_ctr16:
				{
#line 100 "args/argcheck2.rl"
					rl_min = 0; }
				
#line 535 "args/argcheck2.cpp"
				
				goto _st5;
				_st5:
				p += 1;
				st_case_5:
				switch( ( (*( p))) ) {
					case 46: {
						goto _st6;
					}
					case 93: {
						goto _ctr20;
					}
				}
				{
					goto _st0;
				}
				_st6:
				p += 1;
				st_case_6:
				if ( ( (*( p))) == 46 ) {
					goto _st7;
				}
				{
					goto _st0;
				}
				_st7:
				p += 1;
				st_case_7:
				switch( ( (*( p))) ) {
					case 48: {
						goto _ctr22;
					}
					case 93: {
						goto _st3;
					}
				}
				if ( 49 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr23;
				}
				{
					goto _st0;
				}
				_ctr22:
				{
#line 101 "args/argcheck2.rl"
					rl_max = 0; }
				
#line 583 "args/argcheck2.cpp"
				
				goto _st8;
				_st8:
				p += 1;
				st_case_8:
				if ( ( (*( p))) == 93 ) {
					goto _st3;
				}
				{
					goto _st0;
				}
				_ctr23:
				{
#line 101 "args/argcheck2.rl"
					rl_max = (( (*( p))))-'0'; }
				
#line 600 "args/argcheck2.cpp"
				
				goto _st9;
				_ctr26:
				{
#line 101 "args/argcheck2.rl"
					(rl_max *= 10) += ((( (*( p)))) - '0'); }
				
#line 608 "args/argcheck2.cpp"
				
				goto _st9;
				_st9:
				p += 1;
				st_case_9:
				if ( ( (*( p))) == 93 ) {
					goto _st3;
				}
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr26;
				}
				{
					goto _st0;
				}
				_ctr17:
				{
#line 100 "args/argcheck2.rl"
					rl_min = (( (*( p))))-'0'; }
				
#line 628 "args/argcheck2.cpp"
				
				goto _st10;
				_ctr28:
				{
#line 100 "args/argcheck2.rl"
					(rl_min *= 10) += ((( (*( p)))) - '0'); }
				
#line 636 "args/argcheck2.cpp"
				
				goto _st10;
				_st10:
				p += 1;
				st_case_10:
				switch( ( (*( p))) ) {
					case 46: {
						goto _st6;
					}
					case 93: {
						goto _ctr20;
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
				_test_eof2: cs = 2; goto _test_eof; 
				_test_eof11: cs = 11; goto _test_eof; 
				_test_eof3: cs = 3; goto _test_eof; 
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
			
#line 145 "args/argcheck2.rl"
			
			
			if (cs >= 
#line 679 "args/argcheck2.cpp"
			11
#line 147 "args/argcheck2.rl"
			) {
				if (ca < N) {
					err << fmt::format("unexpected extra atoms: {} at [{}]", list_to_string(lv.subView(ca)), ca);
					return false;
				} else
				return true;
			} else {
				err << fmt::format("invalid format string: '{}'", arg_string);
				return false;
			}
		}
		
	}
}
