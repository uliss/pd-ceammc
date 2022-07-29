#line 1 "args/argcheck2.rl"
# include "argcheck2.h"
# include "ceammc_string.h"
# include "ceammc_containers.h"
# include "fmt/core.h"

# include <cstdint>
# include <iostream>
# include <limits>

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
		CHECK_INT,
	};
	
	enum CompareType {
		CMP_NONE,
		CMP_LESS,
		CMP_LESS_EQ,
		CMP_GREATER,
		CMP_GREATER_EQ,
		CMP_EQUAL,
		CMP_NOT_EQUAL,
		CMP_MODULE,
		CMP_POWER2,
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


#line 60 "args/argcheck2.cpp"
static const int arg_check2_start = 1;
static const int arg_check2_first_final = 26;
static const int arg_check2_error = 0;

static const int arg_check2_en_main = 1;


#line 271 "args/argcheck2.rl"


namespace ceammc {
	namespace args {
		
		bool check_args(const char* arg_string, const AtomListView& lv, std::ostream& err)
		{
			const int N = lv.size();
			int ca = 0;
			int cs = 0;
			const char* p = arg_string;
			CheckType rl_type = CHECK_ATOM;
			CompareType rl_cmp = CMP_NONE;
			int64_t rl_num = 0;
			int64_t rl_den = 0;
			int64_t rl_den_cnt = 0;
			int rl_sign = 0;
			t_float rl_cmp_arg = 0;
			int rl_min = 0;
			int rl_max = 0;
			
			SmallAtomListN<16> rl_eq;
			
			
#line 93 "args/argcheck2.cpp"
			{
				cs = (int)arg_check2_start;
			}
			
#line 98 "args/argcheck2.cpp"
			{
				switch ( cs ) {
					case 1:
					goto st_case_1;
					case 0:
					goto st_case_0;
					case 2:
					goto st_case_2;
					case 26:
					goto st_case_26;
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
				}
				goto st_out;
				_ctr8:
				{
#line 262 "args/argcheck2.rl"
					rl_min = 1; rl_max = REPEAT_INF; }
				
#line 162 "args/argcheck2.cpp"
				
				{
#line 60 "args/argcheck2.rl"
					
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
							case CHECK_INT:
							err << fmt::format("int expected at position [{}]", cur);
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
							case CHECK_INT:
							if (!a.isInteger()) {
								err << fmt::format("not a int value at position [{}]: '{}'", ca, atom_to_string(a));
								return false;
							} else {
								const int64_t val = a.asT<int>();
								const int64_t arg = rl_cmp_arg;
								switch (rl_cmp) {
									case CMP_LESS:
									if (!(val < arg)) {
										err << fmt::format("int value at [{}] expected to be <{}, got: {}", ca, arg, val);
										return false;
									}
									break;
									case CMP_LESS_EQ:
									if (!(val <= arg)) {
										err << fmt::format("int value at [{}] expected to be <={}, got: {}", ca, arg, val);
										return false;
									}
									break;
									case CMP_GREATER:
									if (!(val > arg)) {
										err << fmt::format("int value at [{}] expected to be >{}, got: {}", ca, arg, val);
										return false;
									}
									break;
									case CMP_GREATER_EQ:
									if (!(val >= arg)) {
										err << fmt::format("int value at [{}] expected to be >={}, got: {}", ca, arg, val);
										return false;
									}
									break;
									case CMP_EQUAL:
									if (rl_eq.size() == 1) {
										if (a != rl_eq[0]) {
											err << fmt::format("int value at [{}] expected to be = {}, got: {}",
											ca, atom_to_string(rl_eq[0]), atom_to_string(a));
											return false;
										}
									} else {
										bool found = false;
										for (auto& x: rl_eq) {
											if (a == x) { found = true; break; }
										}
										if (!found) {
											err << fmt::format("int value at [{}] expected to be one of: {}, got: {}",
											ca, list_to_string(rl_eq.view()), atom_to_string(a));
											return false;
										}
									}
									break;
									case CMP_NOT_EQUAL:
									if (val == arg) {
										err << fmt::format("int value at [{}] expected to be !={}, got: {}", ca, arg, val);
										return false;
									}
									break;
									case CMP_MODULE:
									if (val % arg != 0) {
										err << fmt::format("int value at [{}] expected to be multiple of {}, got: {}", ca, arg, val);
										return false;
									}
									break;
									case CMP_POWER2: {
										bool rc = (val > 0 && ((val & (val - 1)) == 0));
										if (!rc) {
											err << fmt::format("int value at [{}] expected to be power of 2, got: {}", ca, val);
											return false;
										}
									}
									break;
									default:
									break;
								}
								debug("int", "Ok");
								ca++;
							}
							break;
						}
					}
					
					rl_num = 0;
					rl_den = 0;
					rl_sign = 0;
					rl_den_cnt = 0;
					rl_min = 0;
					rl_max = 0;
				}
				
#line 307 "args/argcheck2.cpp"
				
				goto _st1;
				_ctr15:
				{
#line 60 "args/argcheck2.rl"
					
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
							case CHECK_INT:
							err << fmt::format("int expected at position [{}]", cur);
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
							case CHECK_INT:
							if (!a.isInteger()) {
								err << fmt::format("not a int value at position [{}]: '{}'", ca, atom_to_string(a));
								return false;
							} else {
								const int64_t val = a.asT<int>();
								const int64_t arg = rl_cmp_arg;
								switch (rl_cmp) {
									case CMP_LESS:
									if (!(val < arg)) {
										err << fmt::format("int value at [{}] expected to be <{}, got: {}", ca, arg, val);
										return false;
									}
									break;
									case CMP_LESS_EQ:
									if (!(val <= arg)) {
										err << fmt::format("int value at [{}] expected to be <={}, got: {}", ca, arg, val);
										return false;
									}
									break;
									case CMP_GREATER:
									if (!(val > arg)) {
										err << fmt::format("int value at [{}] expected to be >{}, got: {}", ca, arg, val);
										return false;
									}
									break;
									case CMP_GREATER_EQ:
									if (!(val >= arg)) {
										err << fmt::format("int value at [{}] expected to be >={}, got: {}", ca, arg, val);
										return false;
									}
									break;
									case CMP_EQUAL:
									if (rl_eq.size() == 1) {
										if (a != rl_eq[0]) {
											err << fmt::format("int value at [{}] expected to be = {}, got: {}",
											ca, atom_to_string(rl_eq[0]), atom_to_string(a));
											return false;
										}
									} else {
										bool found = false;
										for (auto& x: rl_eq) {
											if (a == x) { found = true; break; }
										}
										if (!found) {
											err << fmt::format("int value at [{}] expected to be one of: {}, got: {}",
											ca, list_to_string(rl_eq.view()), atom_to_string(a));
											return false;
										}
									}
									break;
									case CMP_NOT_EQUAL:
									if (val == arg) {
										err << fmt::format("int value at [{}] expected to be !={}, got: {}", ca, arg, val);
										return false;
									}
									break;
									case CMP_MODULE:
									if (val % arg != 0) {
										err << fmt::format("int value at [{}] expected to be multiple of {}, got: {}", ca, arg, val);
										return false;
									}
									break;
									case CMP_POWER2: {
										bool rc = (val > 0 && ((val & (val - 1)) == 0));
										if (!rc) {
											err << fmt::format("int value at [{}] expected to be power of 2, got: {}", ca, val);
											return false;
										}
									}
									break;
									default:
									break;
								}
								debug("int", "Ok");
								ca++;
							}
							break;
						}
					}
					
					rl_num = 0;
					rl_den = 0;
					rl_sign = 0;
					rl_den_cnt = 0;
					rl_min = 0;
					rl_max = 0;
				}
				
#line 454 "args/argcheck2.cpp"
				
				goto _st1;
				_ctr64:
				{
#line 201 "args/argcheck2.rl"
					
					rl_eq.push_back(Atom(rl_sign * rl_num));
					debug("op size", rl_eq.size());
				}
				
#line 465 "args/argcheck2.cpp"
				
				{
#line 262 "args/argcheck2.rl"
					rl_min = 1; rl_max = REPEAT_INF; }
				
#line 471 "args/argcheck2.cpp"
				
				{
#line 60 "args/argcheck2.rl"
					
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
							case CHECK_INT:
							err << fmt::format("int expected at position [{}]", cur);
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
							case CHECK_INT:
							if (!a.isInteger()) {
								err << fmt::format("not a int value at position [{}]: '{}'", ca, atom_to_string(a));
								return false;
							} else {
								const int64_t val = a.asT<int>();
								const int64_t arg = rl_cmp_arg;
								switch (rl_cmp) {
									case CMP_LESS:
									if (!(val < arg)) {
										err << fmt::format("int value at [{}] expected to be <{}, got: {}", ca, arg, val);
										return false;
									}
									break;
									case CMP_LESS_EQ:
									if (!(val <= arg)) {
										err << fmt::format("int value at [{}] expected to be <={}, got: {}", ca, arg, val);
										return false;
									}
									break;
									case CMP_GREATER:
									if (!(val > arg)) {
										err << fmt::format("int value at [{}] expected to be >{}, got: {}", ca, arg, val);
										return false;
									}
									break;
									case CMP_GREATER_EQ:
									if (!(val >= arg)) {
										err << fmt::format("int value at [{}] expected to be >={}, got: {}", ca, arg, val);
										return false;
									}
									break;
									case CMP_EQUAL:
									if (rl_eq.size() == 1) {
										if (a != rl_eq[0]) {
											err << fmt::format("int value at [{}] expected to be = {}, got: {}",
											ca, atom_to_string(rl_eq[0]), atom_to_string(a));
											return false;
										}
									} else {
										bool found = false;
										for (auto& x: rl_eq) {
											if (a == x) { found = true; break; }
										}
										if (!found) {
											err << fmt::format("int value at [{}] expected to be one of: {}, got: {}",
											ca, list_to_string(rl_eq.view()), atom_to_string(a));
											return false;
										}
									}
									break;
									case CMP_NOT_EQUAL:
									if (val == arg) {
										err << fmt::format("int value at [{}] expected to be !={}, got: {}", ca, arg, val);
										return false;
									}
									break;
									case CMP_MODULE:
									if (val % arg != 0) {
										err << fmt::format("int value at [{}] expected to be multiple of {}, got: {}", ca, arg, val);
										return false;
									}
									break;
									case CMP_POWER2: {
										bool rc = (val > 0 && ((val & (val - 1)) == 0));
										if (!rc) {
											err << fmt::format("int value at [{}] expected to be power of 2, got: {}", ca, val);
											return false;
										}
									}
									break;
									default:
									break;
								}
								debug("int", "Ok");
								ca++;
							}
							break;
						}
					}
					
					rl_num = 0;
					rl_den = 0;
					rl_sign = 0;
					rl_den_cnt = 0;
					rl_min = 0;
					rl_max = 0;
				}
				
#line 616 "args/argcheck2.cpp"
				
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
					case 105: {
						goto _ctr5;
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
#line 249 "args/argcheck2.rl"
					rl_type = CHECK_BOOL; }
				
#line 648 "args/argcheck2.cpp"
				
				goto _st2;
				_ctr3:
				{
#line 248 "args/argcheck2.rl"
					rl_type = CHECK_ATOM; }
				
#line 656 "args/argcheck2.cpp"
				
				goto _st2;
				_ctr4:
				{
#line 250 "args/argcheck2.rl"
					rl_type = CHECK_BYTE; }
				
#line 664 "args/argcheck2.cpp"
				
				goto _st2;
				_ctr73:
				{
#line 240 "args/argcheck2.rl"
					rl_cmp = CMP_POWER2; }
				
#line 672 "args/argcheck2.cpp"
				
				goto _st2;
				_st2:
				p += 1;
				st_case_2:
				switch( ( (*( p))) ) {
					case 0: {
						goto _ctr7;
					}
					case 32: {
						goto _ctr8;
					}
					case 42: {
						goto _ctr9;
					}
					case 43: {
						goto _ctr10;
					}
					case 63: {
						goto _ctr11;
					}
					case 91: {
						goto _ctr12;
					}
				}
				{
					goto _st0;
				}
				_ctr7:
				{
#line 262 "args/argcheck2.rl"
					rl_min = 1; rl_max = REPEAT_INF; }
				
#line 706 "args/argcheck2.cpp"
				
				{
#line 60 "args/argcheck2.rl"
					
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
							case CHECK_INT:
							err << fmt::format("int expected at position [{}]", cur);
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
							case CHECK_INT:
							if (!a.isInteger()) {
								err << fmt::format("not a int value at position [{}]: '{}'", ca, atom_to_string(a));
								return false;
							} else {
								const int64_t val = a.asT<int>();
								const int64_t arg = rl_cmp_arg;
								switch (rl_cmp) {
									case CMP_LESS:
									if (!(val < arg)) {
										err << fmt::format("int value at [{}] expected to be <{}, got: {}", ca, arg, val);
										return false;
									}
									break;
									case CMP_LESS_EQ:
									if (!(val <= arg)) {
										err << fmt::format("int value at [{}] expected to be <={}, got: {}", ca, arg, val);
										return false;
									}
									break;
									case CMP_GREATER:
									if (!(val > arg)) {
										err << fmt::format("int value at [{}] expected to be >{}, got: {}", ca, arg, val);
										return false;
									}
									break;
									case CMP_GREATER_EQ:
									if (!(val >= arg)) {
										err << fmt::format("int value at [{}] expected to be >={}, got: {}", ca, arg, val);
										return false;
									}
									break;
									case CMP_EQUAL:
									if (rl_eq.size() == 1) {
										if (a != rl_eq[0]) {
											err << fmt::format("int value at [{}] expected to be = {}, got: {}",
											ca, atom_to_string(rl_eq[0]), atom_to_string(a));
											return false;
										}
									} else {
										bool found = false;
										for (auto& x: rl_eq) {
											if (a == x) { found = true; break; }
										}
										if (!found) {
											err << fmt::format("int value at [{}] expected to be one of: {}, got: {}",
											ca, list_to_string(rl_eq.view()), atom_to_string(a));
											return false;
										}
									}
									break;
									case CMP_NOT_EQUAL:
									if (val == arg) {
										err << fmt::format("int value at [{}] expected to be !={}, got: {}", ca, arg, val);
										return false;
									}
									break;
									case CMP_MODULE:
									if (val % arg != 0) {
										err << fmt::format("int value at [{}] expected to be multiple of {}, got: {}", ca, arg, val);
										return false;
									}
									break;
									case CMP_POWER2: {
										bool rc = (val > 0 && ((val & (val - 1)) == 0));
										if (!rc) {
											err << fmt::format("int value at [{}] expected to be power of 2, got: {}", ca, val);
											return false;
										}
									}
									break;
									default:
									break;
								}
								debug("int", "Ok");
								ca++;
							}
							break;
						}
					}
					
					rl_num = 0;
					rl_den = 0;
					rl_sign = 0;
					rl_den_cnt = 0;
					rl_min = 0;
					rl_max = 0;
				}
				
#line 851 "args/argcheck2.cpp"
				
				{
#line 267 "args/argcheck2.rl"
					{p+= 1; cs = 26; goto _out;} }
				
#line 857 "args/argcheck2.cpp"
				
				goto _st26;
				_ctr14:
				{
#line 60 "args/argcheck2.rl"
					
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
							case CHECK_INT:
							err << fmt::format("int expected at position [{}]", cur);
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
							case CHECK_INT:
							if (!a.isInteger()) {
								err << fmt::format("not a int value at position [{}]: '{}'", ca, atom_to_string(a));
								return false;
							} else {
								const int64_t val = a.asT<int>();
								const int64_t arg = rl_cmp_arg;
								switch (rl_cmp) {
									case CMP_LESS:
									if (!(val < arg)) {
										err << fmt::format("int value at [{}] expected to be <{}, got: {}", ca, arg, val);
										return false;
									}
									break;
									case CMP_LESS_EQ:
									if (!(val <= arg)) {
										err << fmt::format("int value at [{}] expected to be <={}, got: {}", ca, arg, val);
										return false;
									}
									break;
									case CMP_GREATER:
									if (!(val > arg)) {
										err << fmt::format("int value at [{}] expected to be >{}, got: {}", ca, arg, val);
										return false;
									}
									break;
									case CMP_GREATER_EQ:
									if (!(val >= arg)) {
										err << fmt::format("int value at [{}] expected to be >={}, got: {}", ca, arg, val);
										return false;
									}
									break;
									case CMP_EQUAL:
									if (rl_eq.size() == 1) {
										if (a != rl_eq[0]) {
											err << fmt::format("int value at [{}] expected to be = {}, got: {}",
											ca, atom_to_string(rl_eq[0]), atom_to_string(a));
											return false;
										}
									} else {
										bool found = false;
										for (auto& x: rl_eq) {
											if (a == x) { found = true; break; }
										}
										if (!found) {
											err << fmt::format("int value at [{}] expected to be one of: {}, got: {}",
											ca, list_to_string(rl_eq.view()), atom_to_string(a));
											return false;
										}
									}
									break;
									case CMP_NOT_EQUAL:
									if (val == arg) {
										err << fmt::format("int value at [{}] expected to be !={}, got: {}", ca, arg, val);
										return false;
									}
									break;
									case CMP_MODULE:
									if (val % arg != 0) {
										err << fmt::format("int value at [{}] expected to be multiple of {}, got: {}", ca, arg, val);
										return false;
									}
									break;
									case CMP_POWER2: {
										bool rc = (val > 0 && ((val & (val - 1)) == 0));
										if (!rc) {
											err << fmt::format("int value at [{}] expected to be power of 2, got: {}", ca, val);
											return false;
										}
									}
									break;
									default:
									break;
								}
								debug("int", "Ok");
								ca++;
							}
							break;
						}
					}
					
					rl_num = 0;
					rl_den = 0;
					rl_sign = 0;
					rl_den_cnt = 0;
					rl_min = 0;
					rl_max = 0;
				}
				
#line 1004 "args/argcheck2.cpp"
				
				{
#line 267 "args/argcheck2.rl"
					{p+= 1; cs = 26; goto _out;} }
				
#line 1010 "args/argcheck2.cpp"
				
				goto _st26;
				_ctr63:
				{
#line 201 "args/argcheck2.rl"
					
					rl_eq.push_back(Atom(rl_sign * rl_num));
					debug("op size", rl_eq.size());
				}
				
#line 1021 "args/argcheck2.cpp"
				
				{
#line 262 "args/argcheck2.rl"
					rl_min = 1; rl_max = REPEAT_INF; }
				
#line 1027 "args/argcheck2.cpp"
				
				{
#line 60 "args/argcheck2.rl"
					
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
							case CHECK_INT:
							err << fmt::format("int expected at position [{}]", cur);
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
							case CHECK_INT:
							if (!a.isInteger()) {
								err << fmt::format("not a int value at position [{}]: '{}'", ca, atom_to_string(a));
								return false;
							} else {
								const int64_t val = a.asT<int>();
								const int64_t arg = rl_cmp_arg;
								switch (rl_cmp) {
									case CMP_LESS:
									if (!(val < arg)) {
										err << fmt::format("int value at [{}] expected to be <{}, got: {}", ca, arg, val);
										return false;
									}
									break;
									case CMP_LESS_EQ:
									if (!(val <= arg)) {
										err << fmt::format("int value at [{}] expected to be <={}, got: {}", ca, arg, val);
										return false;
									}
									break;
									case CMP_GREATER:
									if (!(val > arg)) {
										err << fmt::format("int value at [{}] expected to be >{}, got: {}", ca, arg, val);
										return false;
									}
									break;
									case CMP_GREATER_EQ:
									if (!(val >= arg)) {
										err << fmt::format("int value at [{}] expected to be >={}, got: {}", ca, arg, val);
										return false;
									}
									break;
									case CMP_EQUAL:
									if (rl_eq.size() == 1) {
										if (a != rl_eq[0]) {
											err << fmt::format("int value at [{}] expected to be = {}, got: {}",
											ca, atom_to_string(rl_eq[0]), atom_to_string(a));
											return false;
										}
									} else {
										bool found = false;
										for (auto& x: rl_eq) {
											if (a == x) { found = true; break; }
										}
										if (!found) {
											err << fmt::format("int value at [{}] expected to be one of: {}, got: {}",
											ca, list_to_string(rl_eq.view()), atom_to_string(a));
											return false;
										}
									}
									break;
									case CMP_NOT_EQUAL:
									if (val == arg) {
										err << fmt::format("int value at [{}] expected to be !={}, got: {}", ca, arg, val);
										return false;
									}
									break;
									case CMP_MODULE:
									if (val % arg != 0) {
										err << fmt::format("int value at [{}] expected to be multiple of {}, got: {}", ca, arg, val);
										return false;
									}
									break;
									case CMP_POWER2: {
										bool rc = (val > 0 && ((val & (val - 1)) == 0));
										if (!rc) {
											err << fmt::format("int value at [{}] expected to be power of 2, got: {}", ca, val);
											return false;
										}
									}
									break;
									default:
									break;
								}
								debug("int", "Ok");
								ca++;
							}
							break;
						}
					}
					
					rl_num = 0;
					rl_den = 0;
					rl_sign = 0;
					rl_den_cnt = 0;
					rl_min = 0;
					rl_max = 0;
				}
				
#line 1172 "args/argcheck2.cpp"
				
				{
#line 267 "args/argcheck2.rl"
					{p+= 1; cs = 26; goto _out;} }
				
#line 1178 "args/argcheck2.cpp"
				
				goto _st26;
				_st26:
				p += 1;
				st_case_26:
				{
					goto _st0;
				}
				_ctr9:
				{
#line 262 "args/argcheck2.rl"
					rl_min = 1; rl_max = REPEAT_INF; }
				
#line 1192 "args/argcheck2.cpp"
				
				{
#line 254 "args/argcheck2.rl"
					rl_min = 0; rl_max = REPEAT_INF; }
				
#line 1198 "args/argcheck2.cpp"
				
				goto _st3;
				_ctr10:
				{
#line 262 "args/argcheck2.rl"
					rl_min = 1; rl_max = REPEAT_INF; }
				
#line 1206 "args/argcheck2.cpp"
				
				{
#line 253 "args/argcheck2.rl"
					rl_min = 1, rl_max = REPEAT_INF; }
				
#line 1212 "args/argcheck2.cpp"
				
				goto _st3;
				_ctr11:
				{
#line 262 "args/argcheck2.rl"
					rl_min = 1; rl_max = REPEAT_INF; }
				
#line 1220 "args/argcheck2.cpp"
				
				{
#line 252 "args/argcheck2.rl"
					rl_min = 0; rl_max = 1; }
				
#line 1226 "args/argcheck2.cpp"
				
				goto _st3;
				_ctr21:
				{
#line 208 "args/argcheck2.rl"
					rl_max = rl_min; }
				
#line 1234 "args/argcheck2.cpp"
				
				goto _st3;
				_ctr65:
				{
#line 201 "args/argcheck2.rl"
					
					rl_eq.push_back(Atom(rl_sign * rl_num));
					debug("op size", rl_eq.size());
				}
				
#line 1245 "args/argcheck2.cpp"
				
				{
#line 262 "args/argcheck2.rl"
					rl_min = 1; rl_max = REPEAT_INF; }
				
#line 1251 "args/argcheck2.cpp"
				
				{
#line 254 "args/argcheck2.rl"
					rl_min = 0; rl_max = REPEAT_INF; }
				
#line 1257 "args/argcheck2.cpp"
				
				goto _st3;
				_ctr66:
				{
#line 201 "args/argcheck2.rl"
					
					rl_eq.push_back(Atom(rl_sign * rl_num));
					debug("op size", rl_eq.size());
				}
				
#line 1268 "args/argcheck2.cpp"
				
				{
#line 262 "args/argcheck2.rl"
					rl_min = 1; rl_max = REPEAT_INF; }
				
#line 1274 "args/argcheck2.cpp"
				
				{
#line 253 "args/argcheck2.rl"
					rl_min = 1, rl_max = REPEAT_INF; }
				
#line 1280 "args/argcheck2.cpp"
				
				goto _st3;
				_ctr68:
				{
#line 201 "args/argcheck2.rl"
					
					rl_eq.push_back(Atom(rl_sign * rl_num));
					debug("op size", rl_eq.size());
				}
				
#line 1291 "args/argcheck2.cpp"
				
				{
#line 262 "args/argcheck2.rl"
					rl_min = 1; rl_max = REPEAT_INF; }
				
#line 1297 "args/argcheck2.cpp"
				
				{
#line 252 "args/argcheck2.rl"
					rl_min = 0; rl_max = 1; }
				
#line 1303 "args/argcheck2.cpp"
				
				goto _st3;
				_st3:
				p += 1;
				st_case_3:
				switch( ( (*( p))) ) {
					case 0: {
						goto _ctr14;
					}
					case 32: {
						goto _ctr15;
					}
				}
				{
					goto _st0;
				}
				_ctr12:
				{
#line 262 "args/argcheck2.rl"
					rl_min = 1; rl_max = REPEAT_INF; }
				
#line 1325 "args/argcheck2.cpp"
				
				goto _st4;
				_ctr69:
				{
#line 201 "args/argcheck2.rl"
					
					rl_eq.push_back(Atom(rl_sign * rl_num));
					debug("op size", rl_eq.size());
				}
				
#line 1336 "args/argcheck2.cpp"
				
				{
#line 262 "args/argcheck2.rl"
					rl_min = 1; rl_max = REPEAT_INF; }
				
#line 1342 "args/argcheck2.cpp"
				
				goto _st4;
				_st4:
				p += 1;
				st_case_4:
				if ( ( (*( p))) == 48 ) {
					goto _ctr17;
				}
				if ( 49 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr18;
				}
				{
					goto _st0;
				}
				_ctr17:
				{
#line 206 "args/argcheck2.rl"
					rl_min = 0; }
				
#line 1362 "args/argcheck2.cpp"
				
				goto _st5;
				_st5:
				p += 1;
				st_case_5:
				switch( ( (*( p))) ) {
					case 46: {
						goto _st6;
					}
					case 93: {
						goto _ctr21;
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
						goto _ctr23;
					}
					case 93: {
						goto _st3;
					}
				}
				if ( 49 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr24;
				}
				{
					goto _st0;
				}
				_ctr23:
				{
#line 207 "args/argcheck2.rl"
					rl_max = 0; }
				
#line 1410 "args/argcheck2.cpp"
				
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
				_ctr24:
				{
#line 207 "args/argcheck2.rl"
					rl_max = (( (*( p))))-'0'; }
				
#line 1427 "args/argcheck2.cpp"
				
				goto _st9;
				_ctr27:
				{
#line 207 "args/argcheck2.rl"
					(rl_max *= 10) += ((( (*( p)))) - '0'); }
				
#line 1435 "args/argcheck2.cpp"
				
				goto _st9;
				_st9:
				p += 1;
				st_case_9:
				if ( ( (*( p))) == 93 ) {
					goto _st3;
				}
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr27;
				}
				{
					goto _st0;
				}
				_ctr18:
				{
#line 206 "args/argcheck2.rl"
					rl_min = (( (*( p))))-'0'; }
				
#line 1455 "args/argcheck2.cpp"
				
				goto _st10;
				_ctr29:
				{
#line 206 "args/argcheck2.rl"
					(rl_min *= 10) += ((( (*( p)))) - '0'); }
				
#line 1463 "args/argcheck2.cpp"
				
				goto _st10;
				_st10:
				p += 1;
				st_case_10:
				switch( ( (*( p))) ) {
					case 46: {
						goto _st6;
					}
					case 93: {
						goto _ctr21;
					}
				}
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr29;
				}
				{
					goto _st0;
				}
				_ctr5:
				{
#line 251 "args/argcheck2.rl"
					rl_type = CHECK_INT; }
				
#line 1488 "args/argcheck2.cpp"
				
				goto _st11;
				_st11:
				p += 1;
				st_case_11:
				switch( ( (*( p))) ) {
					case 0: {
						goto _ctr7;
					}
					case 32: {
						goto _ctr8;
					}
					case 33: {
						goto _st12;
					}
					case 37: {
						goto _ctr32;
					}
					case 42: {
						goto _ctr9;
					}
					case 43: {
						goto _ctr10;
					}
					case 60: {
						goto _ctr33;
					}
					case 61: {
						goto _ctr34;
					}
					case 62: {
						goto _ctr35;
					}
					case 63: {
						goto _ctr11;
					}
					case 91: {
						goto _ctr12;
					}
					case 94: {
						goto _st25;
					}
				}
				{
					goto _st0;
				}
				_st12:
				p += 1;
				st_case_12:
				if ( ( (*( p))) == 61 ) {
					goto _ctr37;
				}
				{
					goto _st0;
				}
				_ctr37:
				{
#line 233 "args/argcheck2.rl"
					rl_cmp = CMP_NOT_EQUAL; }
				
#line 1549 "args/argcheck2.cpp"
				
				goto _st13;
				_ctr55:
				{
#line 232 "args/argcheck2.rl"
					rl_cmp = CMP_LESS_EQ; }
				
#line 1557 "args/argcheck2.cpp"
				
				goto _st13;
				_ctr72:
				{
#line 231 "args/argcheck2.rl"
					rl_cmp = CMP_GREATER_EQ; }
				
#line 1565 "args/argcheck2.cpp"
				
				goto _st13;
				_st13:
				p += 1;
				st_case_13:
				switch( ( (*( p))) ) {
					case 43: {
						goto _ctr39;
					}
					case 45: {
						goto _ctr40;
					}
				}
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr41;
				}
				{
					goto _st0;
				}
				_ctr39:
				{
#line 216 "args/argcheck2.rl"
					rl_sign = 1; }
				
#line 1590 "args/argcheck2.cpp"
				
				{
#line 211 "args/argcheck2.rl"
					rl_sign = 1; }
				
#line 1596 "args/argcheck2.cpp"
				
				goto _st14;
				_ctr40:
				{
#line 216 "args/argcheck2.rl"
					rl_sign = 1; }
				
#line 1604 "args/argcheck2.cpp"
				
				{
#line 212 "args/argcheck2.rl"
					rl_sign = -1; }
				
#line 1610 "args/argcheck2.cpp"
				
				goto _st14;
				_st14:
				p += 1;
				st_case_14:
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr43;
				}
				{
					goto _st0;
				}
				_ctr41:
				{
#line 216 "args/argcheck2.rl"
					rl_sign = 1; }
				
#line 1627 "args/argcheck2.cpp"
				
				{
#line 214 "args/argcheck2.rl"
					rl_num = 0; }
				
#line 1633 "args/argcheck2.cpp"
				
				{
#line 214 "args/argcheck2.rl"
					(rl_num *= 10) += ((( (*( p)))) - '0'); }
				
#line 1639 "args/argcheck2.cpp"
				
				{
#line 220 "args/argcheck2.rl"
					
					rl_cmp_arg = rl_sign * rl_num;
					if (rl_den_cnt)
					rl_cmp_arg += (double(rl_den) / rl_den_cnt);
				}
				
#line 1649 "args/argcheck2.cpp"
				
				goto _st15;
				_ctr43:
				{
#line 214 "args/argcheck2.rl"
					rl_num = 0; }
				
#line 1657 "args/argcheck2.cpp"
				
				{
#line 214 "args/argcheck2.rl"
					(rl_num *= 10) += ((( (*( p)))) - '0'); }
				
#line 1663 "args/argcheck2.cpp"
				
				{
#line 220 "args/argcheck2.rl"
					
					rl_cmp_arg = rl_sign * rl_num;
					if (rl_den_cnt)
					rl_cmp_arg += (double(rl_den) / rl_den_cnt);
				}
				
#line 1673 "args/argcheck2.cpp"
				
				goto _st15;
				_ctr46:
				{
#line 214 "args/argcheck2.rl"
					(rl_num *= 10) += ((( (*( p)))) - '0'); }
				
#line 1681 "args/argcheck2.cpp"
				
				{
#line 220 "args/argcheck2.rl"
					
					rl_cmp_arg = rl_sign * rl_num;
					if (rl_den_cnt)
					rl_cmp_arg += (double(rl_den) / rl_den_cnt);
				}
				
#line 1691 "args/argcheck2.cpp"
				
				goto _st15;
				_st15:
				p += 1;
				st_case_15:
				switch( ( (*( p))) ) {
					case 0: {
						goto _ctr7;
					}
					case 32: {
						goto _ctr8;
					}
					case 42: {
						goto _ctr9;
					}
					case 43: {
						goto _ctr10;
					}
					case 46: {
						goto _st16;
					}
					case 63: {
						goto _ctr11;
					}
					case 91: {
						goto _ctr12;
					}
				}
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr46;
				}
				{
					goto _st0;
				}
				_st16:
				p += 1;
				st_case_16:
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr47;
				}
				{
					goto _st0;
				}
				_ctr47:
				{
#line 215 "args/argcheck2.rl"
					rl_den = 0; rl_den_cnt = 1; }
				
#line 1740 "args/argcheck2.cpp"
				
				{
#line 215 "args/argcheck2.rl"
					(rl_den *= 10) += ((( (*( p)))) - '0'); rl_den_cnt *= 10; }
				
#line 1746 "args/argcheck2.cpp"
				
				{
#line 220 "args/argcheck2.rl"
					
					rl_cmp_arg = rl_sign * rl_num;
					if (rl_den_cnt)
					rl_cmp_arg += (double(rl_den) / rl_den_cnt);
				}
				
#line 1756 "args/argcheck2.cpp"
				
				goto _st17;
				_ctr49:
				{
#line 215 "args/argcheck2.rl"
					(rl_den *= 10) += ((( (*( p)))) - '0'); rl_den_cnt *= 10; }
				
#line 1764 "args/argcheck2.cpp"
				
				{
#line 220 "args/argcheck2.rl"
					
					rl_cmp_arg = rl_sign * rl_num;
					if (rl_den_cnt)
					rl_cmp_arg += (double(rl_den) / rl_den_cnt);
				}
				
#line 1774 "args/argcheck2.cpp"
				
				goto _st17;
				_st17:
				p += 1;
				st_case_17:
				switch( ( (*( p))) ) {
					case 0: {
						goto _ctr7;
					}
					case 32: {
						goto _ctr8;
					}
					case 42: {
						goto _ctr9;
					}
					case 43: {
						goto _ctr10;
					}
					case 63: {
						goto _ctr11;
					}
					case 91: {
						goto _ctr12;
					}
				}
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr49;
				}
				{
					goto _st0;
				}
				_ctr32:
				{
#line 235 "args/argcheck2.rl"
					rl_cmp = CMP_MODULE; }
				
#line 1811 "args/argcheck2.cpp"
				
				goto _st18;
				_st18:
				p += 1;
				st_case_18:
				if ( 49 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr51;
				}
				{
					goto _st0;
				}
				_ctr51:
				{
#line 236 "args/argcheck2.rl"
					rl_num = 0; }
				
#line 1828 "args/argcheck2.cpp"
				
				{
#line 236 "args/argcheck2.rl"
					(rl_num *= 10) += ((( (*( p)))) - '0'); }
				
#line 1834 "args/argcheck2.cpp"
				
				{
#line 237 "args/argcheck2.rl"
					rl_cmp_arg = rl_num; }
				
#line 1840 "args/argcheck2.cpp"
				
				goto _st19;
				_ctr53:
				{
#line 236 "args/argcheck2.rl"
					(rl_num *= 10) += ((( (*( p)))) - '0'); }
				
#line 1848 "args/argcheck2.cpp"
				
				{
#line 237 "args/argcheck2.rl"
					rl_cmp_arg = rl_num; }
				
#line 1854 "args/argcheck2.cpp"
				
				goto _st19;
				_st19:
				p += 1;
				st_case_19:
				switch( ( (*( p))) ) {
					case 0: {
						goto _ctr7;
					}
					case 32: {
						goto _ctr8;
					}
					case 42: {
						goto _ctr9;
					}
					case 43: {
						goto _ctr10;
					}
					case 63: {
						goto _ctr11;
					}
					case 91: {
						goto _ctr12;
					}
				}
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr53;
				}
				{
					goto _st0;
				}
				_ctr33:
				{
#line 232 "args/argcheck2.rl"
					rl_cmp = CMP_LESS; }
				
#line 1891 "args/argcheck2.cpp"
				
				goto _st20;
				_st20:
				p += 1;
				st_case_20:
				switch( ( (*( p))) ) {
					case 43: {
						goto _ctr39;
					}
					case 45: {
						goto _ctr40;
					}
					case 61: {
						goto _ctr55;
					}
				}
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr41;
				}
				{
					goto _st0;
				}
				_ctr34:
				{
#line 228 "args/argcheck2.rl"
					rl_cmp = CMP_EQUAL; }
				
#line 1919 "args/argcheck2.cpp"
				
				goto _st21;
				_ctr70:
				{
#line 201 "args/argcheck2.rl"
					
					rl_eq.push_back(Atom(rl_sign * rl_num));
					debug("op size", rl_eq.size());
				}
				
#line 1930 "args/argcheck2.cpp"
				
				goto _st21;
				_st21:
				p += 1;
				st_case_21:
				switch( ( (*( p))) ) {
					case 43: {
						goto _ctr57;
					}
					case 45: {
						goto _ctr58;
					}
				}
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr59;
				}
				{
					goto _st0;
				}
				_ctr57:
				{
#line 216 "args/argcheck2.rl"
					rl_sign = 1; }
				
#line 1955 "args/argcheck2.cpp"
				
				{
#line 211 "args/argcheck2.rl"
					rl_sign = 1; }
				
#line 1961 "args/argcheck2.cpp"
				
				goto _st22;
				_ctr58:
				{
#line 216 "args/argcheck2.rl"
					rl_sign = 1; }
				
#line 1969 "args/argcheck2.cpp"
				
				{
#line 212 "args/argcheck2.rl"
					rl_sign = -1; }
				
#line 1975 "args/argcheck2.cpp"
				
				goto _st22;
				_st22:
				p += 1;
				st_case_22:
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr61;
				}
				{
					goto _st0;
				}
				_ctr59:
				{
#line 216 "args/argcheck2.rl"
					rl_sign = 1; }
				
#line 1992 "args/argcheck2.cpp"
				
				{
#line 214 "args/argcheck2.rl"
					rl_num = 0; }
				
#line 1998 "args/argcheck2.cpp"
				
				{
#line 214 "args/argcheck2.rl"
					(rl_num *= 10) += ((( (*( p)))) - '0'); }
				
#line 2004 "args/argcheck2.cpp"
				
				goto _st23;
				_ctr61:
				{
#line 214 "args/argcheck2.rl"
					rl_num = 0; }
				
#line 2012 "args/argcheck2.cpp"
				
				{
#line 214 "args/argcheck2.rl"
					(rl_num *= 10) += ((( (*( p)))) - '0'); }
				
#line 2018 "args/argcheck2.cpp"
				
				goto _st23;
				_ctr67:
				{
#line 214 "args/argcheck2.rl"
					(rl_num *= 10) += ((( (*( p)))) - '0'); }
				
#line 2026 "args/argcheck2.cpp"
				
				goto _st23;
				_st23:
				p += 1;
				st_case_23:
				switch( ( (*( p))) ) {
					case 0: {
						goto _ctr63;
					}
					case 32: {
						goto _ctr64;
					}
					case 42: {
						goto _ctr65;
					}
					case 43: {
						goto _ctr66;
					}
					case 63: {
						goto _ctr68;
					}
					case 91: {
						goto _ctr69;
					}
					case 124: {
						goto _ctr70;
					}
				}
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr67;
				}
				{
					goto _st0;
				}
				_ctr35:
				{
#line 231 "args/argcheck2.rl"
					rl_cmp = CMP_GREATER; }
				
#line 2066 "args/argcheck2.cpp"
				
				goto _st24;
				_st24:
				p += 1;
				st_case_24:
				switch( ( (*( p))) ) {
					case 43: {
						goto _ctr39;
					}
					case 45: {
						goto _ctr40;
					}
					case 61: {
						goto _ctr72;
					}
				}
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr41;
				}
				{
					goto _st0;
				}
				_st25:
				p += 1;
				st_case_25:
				if ( ( (*( p))) == 50 ) {
					goto _ctr73;
				}
				{
					goto _st0;
				}
				st_out:
				_test_eof1: cs = 1; goto _test_eof; 
				_test_eof2: cs = 2; goto _test_eof; 
				_test_eof26: cs = 26; goto _test_eof; 
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
				
				_test_eof: {}
				if ( cs >= 26 )
					goto _out; _pop: {}
				_out: {}
			}
			
#line 297 "args/argcheck2.rl"
			
			
			if (cs >= 
#line 2136 "args/argcheck2.cpp"
			26
#line 299 "args/argcheck2.rl"
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
