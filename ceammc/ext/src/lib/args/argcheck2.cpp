#line 1 "args/argcheck2.rl"
# include "argcheck2.h"
# include "ceammc_string.h"
# include "ceammc_object.h"
# include "ceammc_crc32.h"
# include "fmt/core.h"

# include <cstdint>
# include <iostream>
# include <limits>
# include <algorithm>
# include <boost/static_string.hpp>
# include <boost/container/static_vector.hpp>
# include <boost/variant.hpp>

#define ARG_DEBUG

#ifdef ARG_DEBUG
#define debug(prefix, arg) std::cerr << " - " << prefix << ": " << arg << std::endl;
#else
#define debug(prefix, arg)
#endif

namespace {
	constexpr int16_t REPEAT_INF = -1;
	
	enum CheckType : int8_t {
		CHECK_ATOM,
		CHECK_BOOL,
		CHECK_BYTE,
		CHECK_INT,
		CHECK_FLOAT,
		CHECK_SYMBOL,
	};
	
	enum CompareType : int8_t {
		CMP_NONE,
		CMP_LESS,
		CMP_LESS_EQ,
		CMP_GREATER,
		CMP_GREATER_EQ,
		CMP_EQUAL,
		CMP_NOT_EQUAL,
		CMP_MODULE,
		CMP_POWER2,
		CMP_RANGE_CLOSED,
		CMP_RANGE_SEMIOPEN,
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
	
	
	using ArgString = std::pair<boost::static_string<14>, uint32_t>;
	using ArgName = boost::static_string<10>;
	using ArgValue = boost::variant<double, int64_t, ArgString>;
	using ArgList = boost::container::small_vector<ArgValue, 3>;
	
	struct Check {
		ArgList values;
		ArgName name;
		int8_t atom_type;
		int8_t cmp_type;
		int8_t repeat_min;
		int8_t repeat_max;
		
		inline int repeatMin() const { return repeat_min; }
		inline int repeatMax() const { return (repeat_max == REPEAT_INF) ? std::numeric_limits<int>::max() : repeat_max; }
		
		inline static bool isEqual(const ArgValue& v, int64_t i)
		{
			auto int_ptr = boost::get<int64_t>(&v);
			return (int_ptr && i == *int_ptr);
		}
		
		inline static bool isEqualHash(const ArgValue& v, uint32_t hash)
		{
			auto str_ptr = boost::get<ArgString>(&v);
			return (str_ptr && str_ptr->second == hash);
		}
	};
	
	class ArgStringVisitor : public boost::static_visitor<std::string>
	{
		public:
		std::string operator()(const double& d) const { return std::to_string(d); }
		std::string operator()(const int64_t& i) const { return std::to_string(i); }
		std::string operator()(const ArgString& s) const {
			std::string res;
			res.reserve(s.first.size() + 2);
			res += '\'';
			res.append(s.first.data(), s.first.size());
			res += '\'';
			return res;
		}
	};
	
	inline std::string arg_to_string(const ArgValue& v)
	{
		return boost::apply_visitor(ArgStringVisitor(), v );
	}
	
	inline std::string arg_to_string(const ArgList& v)
	{
		ceammc::string::SmallString res;
		ArgStringVisitor visitor;
		for (auto& a: v) {
			if (&a != &v[0]) {
				res.push_back(',');
				res.push_back(' ');
			}
			fmt::format_to(std::back_inserter(res), boost::apply_visitor(visitor, a));
		}
		
		return { res.data(), res.size() };
	}
}


#line 134 "args/argcheck2.cpp"
static const int arg_check2_start = 1;
static const int arg_check2_first_final = 46;
static const int arg_check2_error = 0;

static const int arg_check2_en_main = 1;


#line 278 "args/argcheck2.rl"


namespace ceammc {
	namespace args {
		
		namespace {
			
			bool checkAtom(const Check& c, const Atom& a, int& i, const void* x) {
				switch (c.atom_type) {
					case CHECK_ATOM:
					debug("atom", "Ok");
					i++;
					break;
					case CHECK_BOOL:
					if (a.isBool()) {
						debug("book", "Ok");
						i++;
					} else {
						pdError(x, fmt::format("bool expected at position [{}]: '{}'", i, atom_to_string(a)));
						return false;
					}
					break;
					case CHECK_BYTE:
					if (!a.isInteger() || a.asT<int>() < 0 || a.asT<int>() > 255) {
						pdError(x, fmt::format("not a byte value at position [{}]: '{}'", i, atom_to_string(a)));
						return false;
					} else {
						debug("byte", "Ok");
						i++;
					}
					break;
					case CHECK_INT:
					if (!a.isInteger()) {
						pdError(x, fmt::format("not a int value at position [{}]: '{}'", i, atom_to_string(a)));
						return false;
					} else {
						const int64_t val = a.asT<int>();
						const int64_t arg = (c.values.size() == 1 && boost::get<int64_t>(&c.values[0]))
						? *boost::strict_get<int64_t>(&c.values[0])
						: -999999999;
						
						switch (c.cmp_type) {
							case CMP_LESS:
							if (!(val < arg)) {
								pdError(x, fmt::format("int value at [{}] expected to be <{}, got: {}", i, arg, val));
								return false;
							}
							break;
							case CMP_LESS_EQ:
							if (!(val <= arg)) {
								pdError(x, fmt::format("int value at [{}] expected to be <={}, got: {}", i, arg, val));
								return false;
							}
							break;
							case CMP_GREATER:
							if (!(val > arg)) {
								pdError(x, fmt::format("int value at [{}] expected to be >{}, got: {}", i, arg, val));
								return false;
							}
							break;
							case CMP_GREATER_EQ:
							if (!(val >= arg)) {
								pdError(x, fmt::format("int value at [{}] expected to be >={}, got: {}", i, arg, val));
								return false;
							}
							break;
							case CMP_EQUAL:
							if (c.values.size() == 1) {
								if (val != arg) {
									pdError(x, fmt::format("int value at [{}] expected to be = {}, got: {}",
									i, arg_to_string(c.values[0]), atom_to_string(a)));
									return false;
								}
							} else {
								bool found = false;
								for (auto& v: c.values) {
									if (c.isEqual(v, val)) { found = true; break; }
								}
								if (!found) {
									pdError(x, fmt::format("int value at [{}] expected to be one of: {}, got: {}",
									i, arg_to_string(c.values), atom_to_string(a)));
									return false;
								}
							}
							break;
							case CMP_NOT_EQUAL:
							if (val == arg) {
								pdError(x, fmt::format("int value at [{}] expected to be !={}, got: {}", i, arg, val));
								return false;
							}
							break;
							case CMP_MODULE:
							fmt::print("val={}, arg={}\n", val, arg);
							if (val % arg != 0) {
								pdError(x, fmt::format("int value at [{}] expected to be multiple of {}, got: {}", i, arg, val));
								return false;
							}
							break;
							case CMP_POWER2: {
								bool rc = (val > 0 && ((val & (val - 1)) == 0));
								if (!rc) {
									pdError(x, fmt::format("int value at [{}] expected to be power of 2, got: {}", i, val));
									return false;
								}
							}
							break;
							case CMP_RANGE_SEMIOPEN:
							case CMP_RANGE_CLOSED: {
								if (c.values.size() != 2) {
									pdError(x, "internal arg error");
									return false;
								}
								
								if (!boost::get<int64_t>(&c.values[0]) || !boost::get<int64_t>(&c.values[1])) {
									pdError(x, "internal arg error");
									return false;
								}
								
								const auto a = *boost::get<int64_t>(&c.values[0]);
								const auto b = *boost::get<int64_t>(&c.values[1]);
								
								if (c.cmp_type == CMP_RANGE_CLOSED && !(a <= val && val <= b)) {
									pdError(x, fmt::format("int value at [{}] expected to be in [{},{}] range, got: {}", i, a, b, val));
									return false;
								}
								
								if (c.cmp_type == CMP_RANGE_SEMIOPEN && !(a <= val && val < b)) {
									pdError(x, fmt::format("int value at [{}] expected to be in [{},{}) range, got: {}", i, a, b, val));
									return false;
								}
							}
							break;
							default:
							break;
						}
						debug("int", "Ok");
						i++;
					}
					break;
					case CHECK_SYMBOL: {
						if (!a.isSymbol()) {
							pdError(x, fmt::format("not a symbol at position [{}]: '{}'", i, atom_to_string(a)));
							return false;
						}
						
						auto val = a.asT<t_symbol*>()->s_name;
						auto hash = crc32_hash(val);
						
						switch(c.cmp_type) {
							case CMP_EQUAL: {
								bool found = false;
								for (auto& v: c.values) {
									if (c.isEqualHash(v, hash)) { found = true; break; }
								}
								if (!found) {
									if (c.values.size() == 1)
										pdError(x, fmt::format("symbol value at [{}] expected to be {}, got: '{}'",
									i, arg_to_string(c.values[0]), atom_to_string(a)));
									else
										pdError(x, fmt::format("symbol value at [{}] expected to be one of: {}, got: '{}'",
									i, arg_to_string(c.values), atom_to_string(a)));
									return false;
								}
							}
							break;
							default:
							break;
						}
					}
					break;
					case CHECK_FLOAT: {
						if (!a.isFloat()) {
							pdError(x, fmt::format("not a float at position [{}]: '{}'", i, atom_to_string(a)));
							return false;
						}
						
						auto val = a.asT<t_float>();
						const t_float arg = (c.values.size() == 1 && boost::get<double>(&c.values[0]))
						? *boost::strict_get<double>(&c.values[0])
						: -999999999;
						
						switch (c.cmp_type) {
							case CMP_LESS:
							if (!(val < arg)) {
								pdError(x, fmt::format("float value at [{}] expected to be <{}, got: {}", i, arg, val));
								return false;
							}
							break;
							case CMP_LESS_EQ:
							if (!(val <= arg)) {
								pdError(x, fmt::format("float value at [{}] expected to be <={}, got: {}", i, arg, val));
								return false;
							}
							break;
							case CMP_GREATER:
							if (!(val > arg)) {
								pdError(x, fmt::format("float value at [{}] expected to be >{}, got: {}", i, arg, val));
								return false;
							}
							break;
							case CMP_GREATER_EQ:
							if (!(val >= arg)) {
								pdError(x, fmt::format("float value at [{}] expected to be >={}, got: {}", i, arg, val));
								return false;
							}
							break;
							default:
							break;
						}
						
					} break;
					default:
					break;
				}
				
				return true;
			}
		}
		
		struct ArgCheckImp : boost::container::small_vector<Check, 4> {};
		
		ArgChecker::~ArgChecker()  = default;
		
		bool ArgChecker::check(const AtomListView& lv, BaseObject* obj) const
		{
			if (!chk_)
				return false;
			
			const void* x = obj ? obj->owner() : nullptr;
			
			const int N = lv.size();
			int ca = 0;
			for (int i = 0; i < chk_->size(); i++) {
				auto& c = (*chk_)[i];
				auto cur = i + (c.repeat_min - 1);
				if (cur >= N) {
					switch (c.atom_type) {
						case CHECK_ATOM:
						pdError(x, fmt::format("atom expected at position [{}]", cur));
						return false;
						case CHECK_BOOL:
						pdError(x, fmt::format("bool expected at position [{}]", cur));
						return false;
						case CHECK_BYTE:
						pdError(x, fmt::format("byte[0..255] expected at position [{}]", cur));
						return false;
						case CHECK_INT:
						pdError(x, fmt::format("int expected at position [{}]", cur));
						return false;
						case CHECK_FLOAT:
						pdError(x, fmt::format("float expected at position [{}]", cur));
						return false;
						case CHECK_SYMBOL:
						pdError(x, fmt::format("symbol expected at position [{}]", cur));
						return false;
						default:
						pdError(x, fmt::format("error at position [{}]", cur));
						return false;
					}
				}
				
				const auto TOTAL = std::min<int>(N, i + c.repeatMax());
				for (ca = i; ca < TOTAL; ca++) {
					if (!checkAtom(c, lv[ca], ca, x))
						return false;
				}
			}
			
			if (ca < N) {
				pdError(x, fmt::format("extra arguments left, starting from [{}]", ca));
				return false;
			}
			
			return true;
		}
		
		ArgChecker::ArgChecker(const char* str)
		: chk_(new ArgCheckImp)
		{
			int ca = 0;
			int cs = 0;
			const char* p = str;
			CheckType rl_type = CHECK_ATOM;
			CompareType rl_cmp = CMP_NONE;
			int64_t rl_num = 0;
			int64_t rl_den = 0;
			int64_t rl_den_cnt = 0;
			int rl_sign = 0;
			int rl_min = 0;
			int rl_max = 0;
			Check rl_check;
			const char* rl_sym_start = 0;
			
			
#line 437 "args/argcheck2.cpp"
			{
				cs = (int)arg_check2_start;
			}
			
#line 442 "args/argcheck2.cpp"
			{
				switch ( cs ) {
					case 1:
					goto st_case_1;
					case 0:
					goto st_case_0;
					case 2:
					goto st_case_2;
					case 46:
					goto st_case_46;
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
				}
				goto st_out;
				_ctr10:
				{
#line 269 "args/argcheck2.rl"
					rl_min = 1; rl_max = REPEAT_INF; }
				
#line 546 "args/argcheck2.cpp"
				
				{
#line 134 "args/argcheck2.rl"
					
					if (chk_) {
						rl_check.atom_type = rl_type;
						rl_check.cmp_type = rl_cmp;
						rl_check.repeat_min = rl_min;
						rl_check.repeat_max = rl_max;
						chk_->push_back(rl_check);
					}
					
					rl_num = 0;
					rl_den = 0;
					rl_sign = 0;
					rl_den_cnt = 0;
					rl_min = 0;
					rl_max = 0;
					rl_check = {};
				}
				
#line 568 "args/argcheck2.cpp"
				
				goto _st1;
				_ctr17:
				{
#line 134 "args/argcheck2.rl"
					
					if (chk_) {
						rl_check.atom_type = rl_type;
						rl_check.cmp_type = rl_cmp;
						rl_check.repeat_min = rl_min;
						rl_check.repeat_max = rl_max;
						chk_->push_back(rl_check);
					}
					
					rl_num = 0;
					rl_den = 0;
					rl_sign = 0;
					rl_den_cnt = 0;
					rl_min = 0;
					rl_max = 0;
					rl_check = {};
				}
				
#line 592 "args/argcheck2.cpp"
				
				goto _st1;
				_ctr44:
				{
#line 156 "args/argcheck2.rl"
					
					double real = rl_num;
					if (rl_den_cnt)
					real += rl_den / double(rl_den_cnt);
					
					rl_check.values.push_back(rl_sign * real);
				}
				
#line 606 "args/argcheck2.cpp"
				
				{
#line 269 "args/argcheck2.rl"
					rl_min = 1; rl_max = REPEAT_INF; }
				
#line 612 "args/argcheck2.cpp"
				
				{
#line 134 "args/argcheck2.rl"
					
					if (chk_) {
						rl_check.atom_type = rl_type;
						rl_check.cmp_type = rl_cmp;
						rl_check.repeat_min = rl_min;
						rl_check.repeat_max = rl_max;
						chk_->push_back(rl_check);
					}
					
					rl_num = 0;
					rl_den = 0;
					rl_sign = 0;
					rl_den_cnt = 0;
					rl_min = 0;
					rl_max = 0;
					rl_check = {};
				}
				
#line 634 "args/argcheck2.cpp"
				
				goto _st1;
				_ctr75:
				{
#line 152 "args/argcheck2.rl"
					
					rl_check.values.push_back((int64_t)(rl_sign * rl_num));
				}
				
#line 644 "args/argcheck2.cpp"
				
				{
#line 269 "args/argcheck2.rl"
					rl_min = 1; rl_max = REPEAT_INF; }
				
#line 650 "args/argcheck2.cpp"
				
				{
#line 134 "args/argcheck2.rl"
					
					if (chk_) {
						rl_check.atom_type = rl_type;
						rl_check.cmp_type = rl_cmp;
						rl_check.repeat_min = rl_min;
						rl_check.repeat_max = rl_max;
						chk_->push_back(rl_check);
					}
					
					rl_num = 0;
					rl_den = 0;
					rl_sign = 0;
					rl_den_cnt = 0;
					rl_min = 0;
					rl_max = 0;
					rl_check = {};
				}
				
#line 672 "args/argcheck2.cpp"
				
				goto _st1;
				_ctr120:
				{
#line 238 "args/argcheck2.rl"
					rl_sym_start = p; }
				
#line 680 "args/argcheck2.cpp"
				
				{
#line 164 "args/argcheck2.rl"
					
					try {
						ArgString str{ {}, 0 };
						str.first.assign(rl_sym_start, p - rl_sym_start);
						str.second = crc32_hash(str.first.data());
						rl_check.values.push_back(str);
					} catch(std::exception& e) {
						LIB_ERR << "exception: " << e.what();
					}
				}
				
#line 695 "args/argcheck2.cpp"
				
				{
#line 269 "args/argcheck2.rl"
					rl_min = 1; rl_max = REPEAT_INF; }
				
#line 701 "args/argcheck2.cpp"
				
				{
#line 134 "args/argcheck2.rl"
					
					if (chk_) {
						rl_check.atom_type = rl_type;
						rl_check.cmp_type = rl_cmp;
						rl_check.repeat_min = rl_min;
						rl_check.repeat_max = rl_max;
						chk_->push_back(rl_check);
					}
					
					rl_num = 0;
					rl_den = 0;
					rl_sign = 0;
					rl_den_cnt = 0;
					rl_min = 0;
					rl_max = 0;
					rl_check = {};
				}
				
#line 723 "args/argcheck2.cpp"
				
				goto _st1;
				_ctr128:
				{
#line 164 "args/argcheck2.rl"
					
					try {
						ArgString str{ {}, 0 };
						str.first.assign(rl_sym_start, p - rl_sym_start);
						str.second = crc32_hash(str.first.data());
						rl_check.values.push_back(str);
					} catch(std::exception& e) {
						LIB_ERR << "exception: " << e.what();
					}
				}
				
#line 740 "args/argcheck2.cpp"
				
				{
#line 269 "args/argcheck2.rl"
					rl_min = 1; rl_max = REPEAT_INF; }
				
#line 746 "args/argcheck2.cpp"
				
				{
#line 134 "args/argcheck2.rl"
					
					if (chk_) {
						rl_check.atom_type = rl_type;
						rl_check.cmp_type = rl_cmp;
						rl_check.repeat_min = rl_min;
						rl_check.repeat_max = rl_max;
						chk_->push_back(rl_check);
					}
					
					rl_num = 0;
					rl_den = 0;
					rl_sign = 0;
					rl_den_cnt = 0;
					rl_min = 0;
					rl_max = 0;
					rl_check = {};
				}
				
#line 768 "args/argcheck2.cpp"
				
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
					case 102: {
						goto _ctr5;
					}
					case 105: {
						goto _ctr6;
					}
					case 115: {
						goto _ctr7;
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
#line 252 "args/argcheck2.rl"
					rl_type = CHECK_BOOL; }
				
#line 806 "args/argcheck2.cpp"
				
				goto _st2;
				_ctr3:
				{
#line 251 "args/argcheck2.rl"
					rl_type = CHECK_ATOM; }
				
#line 814 "args/argcheck2.cpp"
				
				goto _st2;
				_ctr4:
				{
#line 253 "args/argcheck2.rl"
					rl_type = CHECK_BYTE; }
				
#line 822 "args/argcheck2.cpp"
				
				goto _st2;
				_ctr111:
				{
#line 204 "args/argcheck2.rl"
					rl_cmp = CMP_RANGE_SEMIOPEN; }
				
#line 830 "args/argcheck2.cpp"
				
				goto _st2;
				_ctr113:
				{
#line 204 "args/argcheck2.rl"
					rl_cmp = CMP_RANGE_CLOSED; }
				
#line 838 "args/argcheck2.cpp"
				
				goto _st2;
				_ctr114:
				{
#line 229 "args/argcheck2.rl"
					rl_cmp = CMP_POWER2; }
				
#line 846 "args/argcheck2.cpp"
				
				goto _st2;
				_st2:
				p += 1;
				st_case_2:
				switch( ( (*( p))) ) {
					case 0: {
						goto _ctr9;
					}
					case 32: {
						goto _ctr10;
					}
					case 42: {
						goto _ctr11;
					}
					case 43: {
						goto _ctr12;
					}
					case 63: {
						goto _ctr13;
					}
					case 123: {
						goto _ctr14;
					}
				}
				{
					goto _st0;
				}
				_ctr9:
				{
#line 269 "args/argcheck2.rl"
					rl_min = 1; rl_max = REPEAT_INF; }
				
#line 880 "args/argcheck2.cpp"
				
				{
#line 134 "args/argcheck2.rl"
					
					if (chk_) {
						rl_check.atom_type = rl_type;
						rl_check.cmp_type = rl_cmp;
						rl_check.repeat_min = rl_min;
						rl_check.repeat_max = rl_max;
						chk_->push_back(rl_check);
					}
					
					rl_num = 0;
					rl_den = 0;
					rl_sign = 0;
					rl_den_cnt = 0;
					rl_min = 0;
					rl_max = 0;
					rl_check = {};
				}
				
#line 902 "args/argcheck2.cpp"
				
				{
#line 274 "args/argcheck2.rl"
					{p+= 1; cs = 46; goto _out;} }
				
#line 908 "args/argcheck2.cpp"
				
				goto _st46;
				_ctr16:
				{
#line 134 "args/argcheck2.rl"
					
					if (chk_) {
						rl_check.atom_type = rl_type;
						rl_check.cmp_type = rl_cmp;
						rl_check.repeat_min = rl_min;
						rl_check.repeat_max = rl_max;
						chk_->push_back(rl_check);
					}
					
					rl_num = 0;
					rl_den = 0;
					rl_sign = 0;
					rl_den_cnt = 0;
					rl_min = 0;
					rl_max = 0;
					rl_check = {};
				}
				
#line 932 "args/argcheck2.cpp"
				
				{
#line 274 "args/argcheck2.rl"
					{p+= 1; cs = 46; goto _out;} }
				
#line 938 "args/argcheck2.cpp"
				
				goto _st46;
				_ctr43:
				{
#line 156 "args/argcheck2.rl"
					
					double real = rl_num;
					if (rl_den_cnt)
					real += rl_den / double(rl_den_cnt);
					
					rl_check.values.push_back(rl_sign * real);
				}
				
#line 952 "args/argcheck2.cpp"
				
				{
#line 269 "args/argcheck2.rl"
					rl_min = 1; rl_max = REPEAT_INF; }
				
#line 958 "args/argcheck2.cpp"
				
				{
#line 134 "args/argcheck2.rl"
					
					if (chk_) {
						rl_check.atom_type = rl_type;
						rl_check.cmp_type = rl_cmp;
						rl_check.repeat_min = rl_min;
						rl_check.repeat_max = rl_max;
						chk_->push_back(rl_check);
					}
					
					rl_num = 0;
					rl_den = 0;
					rl_sign = 0;
					rl_den_cnt = 0;
					rl_min = 0;
					rl_max = 0;
					rl_check = {};
				}
				
#line 980 "args/argcheck2.cpp"
				
				{
#line 274 "args/argcheck2.rl"
					{p+= 1; cs = 46; goto _out;} }
				
#line 986 "args/argcheck2.cpp"
				
				goto _st46;
				_ctr74:
				{
#line 152 "args/argcheck2.rl"
					
					rl_check.values.push_back((int64_t)(rl_sign * rl_num));
				}
				
#line 996 "args/argcheck2.cpp"
				
				{
#line 269 "args/argcheck2.rl"
					rl_min = 1; rl_max = REPEAT_INF; }
				
#line 1002 "args/argcheck2.cpp"
				
				{
#line 134 "args/argcheck2.rl"
					
					if (chk_) {
						rl_check.atom_type = rl_type;
						rl_check.cmp_type = rl_cmp;
						rl_check.repeat_min = rl_min;
						rl_check.repeat_max = rl_max;
						chk_->push_back(rl_check);
					}
					
					rl_num = 0;
					rl_den = 0;
					rl_sign = 0;
					rl_den_cnt = 0;
					rl_min = 0;
					rl_max = 0;
					rl_check = {};
				}
				
#line 1024 "args/argcheck2.cpp"
				
				{
#line 274 "args/argcheck2.rl"
					{p+= 1; cs = 46; goto _out;} }
				
#line 1030 "args/argcheck2.cpp"
				
				goto _st46;
				_ctr119:
				{
#line 238 "args/argcheck2.rl"
					rl_sym_start = p; }
				
#line 1038 "args/argcheck2.cpp"
				
				{
#line 164 "args/argcheck2.rl"
					
					try {
						ArgString str{ {}, 0 };
						str.first.assign(rl_sym_start, p - rl_sym_start);
						str.second = crc32_hash(str.first.data());
						rl_check.values.push_back(str);
					} catch(std::exception& e) {
						LIB_ERR << "exception: " << e.what();
					}
				}
				
#line 1053 "args/argcheck2.cpp"
				
				{
#line 269 "args/argcheck2.rl"
					rl_min = 1; rl_max = REPEAT_INF; }
				
#line 1059 "args/argcheck2.cpp"
				
				{
#line 134 "args/argcheck2.rl"
					
					if (chk_) {
						rl_check.atom_type = rl_type;
						rl_check.cmp_type = rl_cmp;
						rl_check.repeat_min = rl_min;
						rl_check.repeat_max = rl_max;
						chk_->push_back(rl_check);
					}
					
					rl_num = 0;
					rl_den = 0;
					rl_sign = 0;
					rl_den_cnt = 0;
					rl_min = 0;
					rl_max = 0;
					rl_check = {};
				}
				
#line 1081 "args/argcheck2.cpp"
				
				{
#line 274 "args/argcheck2.rl"
					{p+= 1; cs = 46; goto _out;} }
				
#line 1087 "args/argcheck2.cpp"
				
				goto _st46;
				_ctr127:
				{
#line 164 "args/argcheck2.rl"
					
					try {
						ArgString str{ {}, 0 };
						str.first.assign(rl_sym_start, p - rl_sym_start);
						str.second = crc32_hash(str.first.data());
						rl_check.values.push_back(str);
					} catch(std::exception& e) {
						LIB_ERR << "exception: " << e.what();
					}
				}
				
#line 1104 "args/argcheck2.cpp"
				
				{
#line 269 "args/argcheck2.rl"
					rl_min = 1; rl_max = REPEAT_INF; }
				
#line 1110 "args/argcheck2.cpp"
				
				{
#line 134 "args/argcheck2.rl"
					
					if (chk_) {
						rl_check.atom_type = rl_type;
						rl_check.cmp_type = rl_cmp;
						rl_check.repeat_min = rl_min;
						rl_check.repeat_max = rl_max;
						chk_->push_back(rl_check);
					}
					
					rl_num = 0;
					rl_den = 0;
					rl_sign = 0;
					rl_den_cnt = 0;
					rl_min = 0;
					rl_max = 0;
					rl_check = {};
				}
				
#line 1132 "args/argcheck2.cpp"
				
				{
#line 274 "args/argcheck2.rl"
					{p+= 1; cs = 46; goto _out;} }
				
#line 1138 "args/argcheck2.cpp"
				
				goto _st46;
				_st46:
				p += 1;
				st_case_46:
				{
					goto _st0;
				}
				_ctr11:
				{
#line 269 "args/argcheck2.rl"
					rl_min = 1; rl_max = REPEAT_INF; }
				
#line 1152 "args/argcheck2.cpp"
				
				{
#line 259 "args/argcheck2.rl"
					rl_min = 0; rl_max = REPEAT_INF; }
				
#line 1158 "args/argcheck2.cpp"
				
				goto _st3;
				_ctr12:
				{
#line 269 "args/argcheck2.rl"
					rl_min = 1; rl_max = REPEAT_INF; }
				
#line 1166 "args/argcheck2.cpp"
				
				{
#line 258 "args/argcheck2.rl"
					rl_min = 1, rl_max = REPEAT_INF; }
				
#line 1172 "args/argcheck2.cpp"
				
				goto _st3;
				_ctr13:
				{
#line 269 "args/argcheck2.rl"
					rl_min = 1; rl_max = REPEAT_INF; }
				
#line 1180 "args/argcheck2.cpp"
				
				{
#line 257 "args/argcheck2.rl"
					rl_min = 0; rl_max = 1; }
				
#line 1186 "args/argcheck2.cpp"
				
				goto _st3;
				_ctr23:
				{
#line 180 "args/argcheck2.rl"
					rl_max = rl_min; }
				
#line 1194 "args/argcheck2.cpp"
				
				goto _st3;
				_ctr45:
				{
#line 156 "args/argcheck2.rl"
					
					double real = rl_num;
					if (rl_den_cnt)
					real += rl_den / double(rl_den_cnt);
					
					rl_check.values.push_back(rl_sign * real);
				}
				
#line 1208 "args/argcheck2.cpp"
				
				{
#line 269 "args/argcheck2.rl"
					rl_min = 1; rl_max = REPEAT_INF; }
				
#line 1214 "args/argcheck2.cpp"
				
				{
#line 259 "args/argcheck2.rl"
					rl_min = 0; rl_max = REPEAT_INF; }
				
#line 1220 "args/argcheck2.cpp"
				
				goto _st3;
				_ctr46:
				{
#line 156 "args/argcheck2.rl"
					
					double real = rl_num;
					if (rl_den_cnt)
					real += rl_den / double(rl_den_cnt);
					
					rl_check.values.push_back(rl_sign * real);
				}
				
#line 1234 "args/argcheck2.cpp"
				
				{
#line 269 "args/argcheck2.rl"
					rl_min = 1; rl_max = REPEAT_INF; }
				
#line 1240 "args/argcheck2.cpp"
				
				{
#line 258 "args/argcheck2.rl"
					rl_min = 1, rl_max = REPEAT_INF; }
				
#line 1246 "args/argcheck2.cpp"
				
				goto _st3;
				_ctr49:
				{
#line 156 "args/argcheck2.rl"
					
					double real = rl_num;
					if (rl_den_cnt)
					real += rl_den / double(rl_den_cnt);
					
					rl_check.values.push_back(rl_sign * real);
				}
				
#line 1260 "args/argcheck2.cpp"
				
				{
#line 269 "args/argcheck2.rl"
					rl_min = 1; rl_max = REPEAT_INF; }
				
#line 1266 "args/argcheck2.cpp"
				
				{
#line 257 "args/argcheck2.rl"
					rl_min = 0; rl_max = 1; }
				
#line 1272 "args/argcheck2.cpp"
				
				goto _st3;
				_ctr76:
				{
#line 152 "args/argcheck2.rl"
					
					rl_check.values.push_back((int64_t)(rl_sign * rl_num));
				}
				
#line 1282 "args/argcheck2.cpp"
				
				{
#line 269 "args/argcheck2.rl"
					rl_min = 1; rl_max = REPEAT_INF; }
				
#line 1288 "args/argcheck2.cpp"
				
				{
#line 259 "args/argcheck2.rl"
					rl_min = 0; rl_max = REPEAT_INF; }
				
#line 1294 "args/argcheck2.cpp"
				
				goto _st3;
				_ctr77:
				{
#line 152 "args/argcheck2.rl"
					
					rl_check.values.push_back((int64_t)(rl_sign * rl_num));
				}
				
#line 1304 "args/argcheck2.cpp"
				
				{
#line 269 "args/argcheck2.rl"
					rl_min = 1; rl_max = REPEAT_INF; }
				
#line 1310 "args/argcheck2.cpp"
				
				{
#line 258 "args/argcheck2.rl"
					rl_min = 1, rl_max = REPEAT_INF; }
				
#line 1316 "args/argcheck2.cpp"
				
				goto _st3;
				_ctr79:
				{
#line 152 "args/argcheck2.rl"
					
					rl_check.values.push_back((int64_t)(rl_sign * rl_num));
				}
				
#line 1326 "args/argcheck2.cpp"
				
				{
#line 269 "args/argcheck2.rl"
					rl_min = 1; rl_max = REPEAT_INF; }
				
#line 1332 "args/argcheck2.cpp"
				
				{
#line 257 "args/argcheck2.rl"
					rl_min = 0; rl_max = 1; }
				
#line 1338 "args/argcheck2.cpp"
				
				goto _st3;
				_st3:
				p += 1;
				st_case_3:
				switch( ( (*( p))) ) {
					case 0: {
						goto _ctr16;
					}
					case 32: {
						goto _ctr17;
					}
				}
				{
					goto _st0;
				}
				_ctr14:
				{
#line 269 "args/argcheck2.rl"
					rl_min = 1; rl_max = REPEAT_INF; }
				
#line 1360 "args/argcheck2.cpp"
				
				goto _st4;
				_ctr50:
				{
#line 156 "args/argcheck2.rl"
					
					double real = rl_num;
					if (rl_den_cnt)
					real += rl_den / double(rl_den_cnt);
					
					rl_check.values.push_back(rl_sign * real);
				}
				
#line 1374 "args/argcheck2.cpp"
				
				{
#line 269 "args/argcheck2.rl"
					rl_min = 1; rl_max = REPEAT_INF; }
				
#line 1380 "args/argcheck2.cpp"
				
				goto _st4;
				_ctr80:
				{
#line 152 "args/argcheck2.rl"
					
					rl_check.values.push_back((int64_t)(rl_sign * rl_num));
				}
				
#line 1390 "args/argcheck2.cpp"
				
				{
#line 269 "args/argcheck2.rl"
					rl_min = 1; rl_max = REPEAT_INF; }
				
#line 1396 "args/argcheck2.cpp"
				
				goto _st4;
				_st4:
				p += 1;
				st_case_4:
				if ( ( (*( p))) == 48 ) {
					goto _ctr19;
				}
				if ( 49 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr20;
				}
				{
					goto _st0;
				}
				_ctr19:
				{
#line 178 "args/argcheck2.rl"
					rl_min = 0; }
				
#line 1416 "args/argcheck2.cpp"
				
				goto _st5;
				_st5:
				p += 1;
				st_case_5:
				switch( ( (*( p))) ) {
					case 44: {
						goto _st6;
					}
					case 125: {
						goto _ctr23;
					}
				}
				{
					goto _st0;
				}
				_st6:
				p += 1;
				st_case_6:
				switch( ( (*( p))) ) {
					case 48: {
						goto _ctr24;
					}
					case 125: {
						goto _st3;
					}
				}
				if ( 49 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr25;
				}
				{
					goto _st0;
				}
				_ctr24:
				{
#line 179 "args/argcheck2.rl"
					rl_max = 0; }
				
#line 1455 "args/argcheck2.cpp"
				
				goto _st7;
				_st7:
				p += 1;
				st_case_7:
				if ( ( (*( p))) == 125 ) {
					goto _st3;
				}
				{
					goto _st0;
				}
				_ctr25:
				{
#line 179 "args/argcheck2.rl"
					rl_max = (( (*( p))))-'0'; }
				
#line 1472 "args/argcheck2.cpp"
				
				goto _st8;
				_ctr28:
				{
#line 179 "args/argcheck2.rl"
					(rl_max *= 10) += ((( (*( p)))) - '0'); }
				
#line 1480 "args/argcheck2.cpp"
				
				goto _st8;
				_st8:
				p += 1;
				st_case_8:
				if ( ( (*( p))) == 125 ) {
					goto _st3;
				}
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr28;
				}
				{
					goto _st0;
				}
				_ctr20:
				{
#line 178 "args/argcheck2.rl"
					rl_min = (( (*( p))))-'0'; }
				
#line 1500 "args/argcheck2.cpp"
				
				goto _st9;
				_ctr30:
				{
#line 178 "args/argcheck2.rl"
					(rl_min *= 10) += ((( (*( p)))) - '0'); }
				
#line 1508 "args/argcheck2.cpp"
				
				goto _st9;
				_st9:
				p += 1;
				st_case_9:
				switch( ( (*( p))) ) {
					case 44: {
						goto _st6;
					}
					case 125: {
						goto _ctr23;
					}
				}
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr30;
				}
				{
					goto _st0;
				}
				_ctr5:
				{
#line 256 "args/argcheck2.rl"
					rl_type = CHECK_FLOAT; }
				
#line 1533 "args/argcheck2.cpp"
				
				goto _st10;
				_st10:
				p += 1;
				st_case_10:
				switch( ( (*( p))) ) {
					case 0: {
						goto _ctr9;
					}
					case 32: {
						goto _ctr10;
					}
					case 33: {
						goto _st11;
					}
					case 42: {
						goto _ctr11;
					}
					case 43: {
						goto _ctr12;
					}
					case 60: {
						goto _ctr33;
					}
					case 62: {
						goto _ctr34;
					}
					case 63: {
						goto _ctr13;
					}
					case 123: {
						goto _ctr14;
					}
				}
				{
					goto _st0;
				}
				_st11:
				p += 1;
				st_case_11:
				if ( ( (*( p))) == 61 ) {
					goto _ctr35;
				}
				{
					goto _st0;
				}
				_ctr35:
				{
#line 217 "args/argcheck2.rl"
					rl_cmp = CMP_NOT_EQUAL; }
				
#line 1585 "args/argcheck2.cpp"
				
				goto _st12;
				_ctr55:
				{
#line 216 "args/argcheck2.rl"
					rl_cmp = CMP_LESS_EQ; }
				
#line 1593 "args/argcheck2.cpp"
				
				goto _st12;
				_ctr57:
				{
#line 215 "args/argcheck2.rl"
					rl_cmp = CMP_GREATER_EQ; }
				
#line 1601 "args/argcheck2.cpp"
				
				goto _st12;
				_st12:
				p += 1;
				st_case_12:
				switch( ( (*( p))) ) {
					case 43: {
						goto _ctr37;
					}
					case 45: {
						goto _ctr38;
					}
				}
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr39;
				}
				{
					goto _st0;
				}
				_ctr37:
				{
#line 192 "args/argcheck2.rl"
					rl_sign = 1; }
				
#line 1626 "args/argcheck2.cpp"
				
				{
#line 183 "args/argcheck2.rl"
					rl_sign = 1; }
				
#line 1632 "args/argcheck2.cpp"
				
				goto _st13;
				_ctr38:
				{
#line 192 "args/argcheck2.rl"
					rl_sign = 1; }
				
#line 1640 "args/argcheck2.cpp"
				
				{
#line 184 "args/argcheck2.rl"
					rl_sign = -1; }
				
#line 1646 "args/argcheck2.cpp"
				
				goto _st13;
				_st13:
				p += 1;
				st_case_13:
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr41;
				}
				{
					goto _st0;
				}
				_ctr39:
				{
#line 192 "args/argcheck2.rl"
					rl_sign = 1; }
				
#line 1663 "args/argcheck2.cpp"
				
				{
#line 186 "args/argcheck2.rl"
					rl_num = 0; }
				
#line 1669 "args/argcheck2.cpp"
				
				{
#line 186 "args/argcheck2.rl"
					(rl_num *= 10) += ((( (*( p)))) - '0'); }
				
#line 1675 "args/argcheck2.cpp"
				
				goto _st14;
				_ctr41:
				{
#line 186 "args/argcheck2.rl"
					rl_num = 0; }
				
#line 1683 "args/argcheck2.cpp"
				
				{
#line 186 "args/argcheck2.rl"
					(rl_num *= 10) += ((( (*( p)))) - '0'); }
				
#line 1689 "args/argcheck2.cpp"
				
				goto _st14;
				_ctr48:
				{
#line 186 "args/argcheck2.rl"
					(rl_num *= 10) += ((( (*( p)))) - '0'); }
				
#line 1697 "args/argcheck2.cpp"
				
				goto _st14;
				_st14:
				p += 1;
				st_case_14:
				switch( ( (*( p))) ) {
					case 0: {
						goto _ctr43;
					}
					case 32: {
						goto _ctr44;
					}
					case 42: {
						goto _ctr45;
					}
					case 43: {
						goto _ctr46;
					}
					case 46: {
						goto _st15;
					}
					case 63: {
						goto _ctr49;
					}
					case 123: {
						goto _ctr50;
					}
				}
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr48;
				}
				{
					goto _st0;
				}
				_st15:
				p += 1;
				st_case_15:
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr51;
				}
				{
					goto _st0;
				}
				_ctr51:
				{
#line 187 "args/argcheck2.rl"
					rl_den = 0; rl_den_cnt = 1; }
				
#line 1746 "args/argcheck2.cpp"
				
				{
#line 187 "args/argcheck2.rl"
					(rl_den *= 10) += ((( (*( p)))) - '0'); rl_den_cnt *= 10; }
				
#line 1752 "args/argcheck2.cpp"
				
				goto _st16;
				_ctr53:
				{
#line 187 "args/argcheck2.rl"
					(rl_den *= 10) += ((( (*( p)))) - '0'); rl_den_cnt *= 10; }
				
#line 1760 "args/argcheck2.cpp"
				
				goto _st16;
				_st16:
				p += 1;
				st_case_16:
				switch( ( (*( p))) ) {
					case 0: {
						goto _ctr43;
					}
					case 32: {
						goto _ctr44;
					}
					case 42: {
						goto _ctr45;
					}
					case 43: {
						goto _ctr46;
					}
					case 63: {
						goto _ctr49;
					}
					case 123: {
						goto _ctr50;
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
#line 216 "args/argcheck2.rl"
					rl_cmp = CMP_LESS; }
				
#line 1797 "args/argcheck2.cpp"
				
				goto _st17;
				_st17:
				p += 1;
				st_case_17:
				switch( ( (*( p))) ) {
					case 43: {
						goto _ctr37;
					}
					case 45: {
						goto _ctr38;
					}
					case 61: {
						goto _ctr55;
					}
				}
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr39;
				}
				{
					goto _st0;
				}
				_ctr34:
				{
#line 215 "args/argcheck2.rl"
					rl_cmp = CMP_GREATER; }
				
#line 1825 "args/argcheck2.cpp"
				
				goto _st18;
				_st18:
				p += 1;
				st_case_18:
				switch( ( (*( p))) ) {
					case 43: {
						goto _ctr37;
					}
					case 45: {
						goto _ctr38;
					}
					case 61: {
						goto _ctr57;
					}
				}
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr39;
				}
				{
					goto _st0;
				}
				_ctr6:
				{
#line 254 "args/argcheck2.rl"
					rl_type = CHECK_INT; }
				
#line 1853 "args/argcheck2.cpp"
				
				goto _st19;
				_st19:
				p += 1;
				st_case_19:
				switch( ( (*( p))) ) {
					case 0: {
						goto _ctr9;
					}
					case 32: {
						goto _ctr10;
					}
					case 33: {
						goto _st20;
					}
					case 37: {
						goto _ctr60;
					}
					case 42: {
						goto _ctr11;
					}
					case 43: {
						goto _ctr12;
					}
					case 60: {
						goto _ctr61;
					}
					case 61: {
						goto _ctr62;
					}
					case 62: {
						goto _ctr63;
					}
					case 63: {
						goto _ctr13;
					}
					case 91: {
						goto _st31;
					}
					case 94: {
						goto _st37;
					}
					case 123: {
						goto _ctr14;
					}
				}
				{
					goto _st0;
				}
				_st20:
				p += 1;
				st_case_20:
				if ( ( (*( p))) == 61 ) {
					goto _ctr66;
				}
				{
					goto _st0;
				}
				_ctr66:
				{
#line 217 "args/argcheck2.rl"
					rl_cmp = CMP_NOT_EQUAL; }
				
#line 1917 "args/argcheck2.cpp"
				
				goto _st21;
				_ctr86:
				{
#line 216 "args/argcheck2.rl"
					rl_cmp = CMP_LESS_EQ; }
				
#line 1925 "args/argcheck2.cpp"
				
				goto _st21;
				_ctr96:
				{
#line 215 "args/argcheck2.rl"
					rl_cmp = CMP_GREATER_EQ; }
				
#line 1933 "args/argcheck2.cpp"
				
				goto _st21;
				_st21:
				p += 1;
				st_case_21:
				switch( ( (*( p))) ) {
					case 43: {
						goto _ctr68;
					}
					case 45: {
						goto _ctr69;
					}
				}
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr70;
				}
				{
					goto _st0;
				}
				_ctr68:
				{
#line 192 "args/argcheck2.rl"
					rl_sign = 1; }
				
#line 1958 "args/argcheck2.cpp"
				
				{
#line 183 "args/argcheck2.rl"
					rl_sign = 1; }
				
#line 1964 "args/argcheck2.cpp"
				
				goto _st22;
				_ctr69:
				{
#line 192 "args/argcheck2.rl"
					rl_sign = 1; }
				
#line 1972 "args/argcheck2.cpp"
				
				{
#line 184 "args/argcheck2.rl"
					rl_sign = -1; }
				
#line 1978 "args/argcheck2.cpp"
				
				goto _st22;
				_st22:
				p += 1;
				st_case_22:
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr72;
				}
				{
					goto _st0;
				}
				_ctr70:
				{
#line 192 "args/argcheck2.rl"
					rl_sign = 1; }
				
#line 1995 "args/argcheck2.cpp"
				
				{
#line 186 "args/argcheck2.rl"
					rl_num = 0; }
				
#line 2001 "args/argcheck2.cpp"
				
				{
#line 186 "args/argcheck2.rl"
					(rl_num *= 10) += ((( (*( p)))) - '0'); }
				
#line 2007 "args/argcheck2.cpp"
				
				goto _st23;
				_ctr72:
				{
#line 186 "args/argcheck2.rl"
					rl_num = 0; }
				
#line 2015 "args/argcheck2.cpp"
				
				{
#line 186 "args/argcheck2.rl"
					(rl_num *= 10) += ((( (*( p)))) - '0'); }
				
#line 2021 "args/argcheck2.cpp"
				
				goto _st23;
				_ctr78:
				{
#line 186 "args/argcheck2.rl"
					(rl_num *= 10) += ((( (*( p)))) - '0'); }
				
#line 2029 "args/argcheck2.cpp"
				
				goto _st23;
				_st23:
				p += 1;
				st_case_23:
				switch( ( (*( p))) ) {
					case 0: {
						goto _ctr74;
					}
					case 32: {
						goto _ctr75;
					}
					case 42: {
						goto _ctr76;
					}
					case 43: {
						goto _ctr77;
					}
					case 63: {
						goto _ctr79;
					}
					case 123: {
						goto _ctr80;
					}
				}
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr78;
				}
				{
					goto _st0;
				}
				_ctr60:
				{
#line 222 "args/argcheck2.rl"
					rl_cmp = CMP_MODULE; }
				
#line 2066 "args/argcheck2.cpp"
				
				goto _st24;
				_st24:
				p += 1;
				st_case_24:
				if ( 49 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr82;
				}
				{
					goto _st0;
				}
				_ctr82:
				{
#line 223 "args/argcheck2.rl"
					rl_sign = 1; rl_num = 0; }
				
#line 2083 "args/argcheck2.cpp"
				
				{
#line 223 "args/argcheck2.rl"
					(rl_num *= 10) += ((( (*( p)))) - '0'); }
				
#line 2089 "args/argcheck2.cpp"
				
				{
#line 152 "args/argcheck2.rl"
					
					rl_check.values.push_back((int64_t)(rl_sign * rl_num));
				}
				
#line 2097 "args/argcheck2.cpp"
				
				goto _st25;
				_ctr84:
				{
#line 223 "args/argcheck2.rl"
					(rl_num *= 10) += ((( (*( p)))) - '0'); }
				
#line 2105 "args/argcheck2.cpp"
				
				{
#line 152 "args/argcheck2.rl"
					
					rl_check.values.push_back((int64_t)(rl_sign * rl_num));
				}
				
#line 2113 "args/argcheck2.cpp"
				
				goto _st25;
				_st25:
				p += 1;
				st_case_25:
				switch( ( (*( p))) ) {
					case 0: {
						goto _ctr9;
					}
					case 32: {
						goto _ctr10;
					}
					case 42: {
						goto _ctr11;
					}
					case 43: {
						goto _ctr12;
					}
					case 63: {
						goto _ctr13;
					}
					case 123: {
						goto _ctr14;
					}
				}
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr84;
				}
				{
					goto _st0;
				}
				_ctr61:
				{
#line 216 "args/argcheck2.rl"
					rl_cmp = CMP_LESS; }
				
#line 2150 "args/argcheck2.cpp"
				
				goto _st26;
				_st26:
				p += 1;
				st_case_26:
				switch( ( (*( p))) ) {
					case 43: {
						goto _ctr68;
					}
					case 45: {
						goto _ctr69;
					}
					case 61: {
						goto _ctr86;
					}
				}
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr70;
				}
				{
					goto _st0;
				}
				_ctr62:
				{
#line 212 "args/argcheck2.rl"
					rl_cmp = CMP_EQUAL; }
				
#line 2178 "args/argcheck2.cpp"
				
				goto _st27;
				_st27:
				p += 1;
				st_case_27:
				switch( ( (*( p))) ) {
					case 43: {
						goto _ctr88;
					}
					case 45: {
						goto _ctr89;
					}
				}
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr90;
				}
				{
					goto _st0;
				}
				_ctr88:
				{
#line 192 "args/argcheck2.rl"
					rl_sign = 1; }
				
#line 2203 "args/argcheck2.cpp"
				
				{
#line 183 "args/argcheck2.rl"
					rl_sign = 1; }
				
#line 2209 "args/argcheck2.cpp"
				
				goto _st28;
				_ctr89:
				{
#line 192 "args/argcheck2.rl"
					rl_sign = 1; }
				
#line 2217 "args/argcheck2.cpp"
				
				{
#line 184 "args/argcheck2.rl"
					rl_sign = -1; }
				
#line 2223 "args/argcheck2.cpp"
				
				goto _st28;
				_st28:
				p += 1;
				st_case_28:
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr92;
				}
				{
					goto _st0;
				}
				_ctr90:
				{
#line 192 "args/argcheck2.rl"
					rl_sign = 1; }
				
#line 2240 "args/argcheck2.cpp"
				
				{
#line 186 "args/argcheck2.rl"
					rl_num = 0; }
				
#line 2246 "args/argcheck2.cpp"
				
				{
#line 186 "args/argcheck2.rl"
					(rl_num *= 10) += ((( (*( p)))) - '0'); }
				
#line 2252 "args/argcheck2.cpp"
				
				{
#line 152 "args/argcheck2.rl"
					
					rl_check.values.push_back((int64_t)(rl_sign * rl_num));
				}
				
#line 2260 "args/argcheck2.cpp"
				
				goto _st29;
				_ctr92:
				{
#line 186 "args/argcheck2.rl"
					rl_num = 0; }
				
#line 2268 "args/argcheck2.cpp"
				
				{
#line 186 "args/argcheck2.rl"
					(rl_num *= 10) += ((( (*( p)))) - '0'); }
				
#line 2274 "args/argcheck2.cpp"
				
				{
#line 152 "args/argcheck2.rl"
					
					rl_check.values.push_back((int64_t)(rl_sign * rl_num));
				}
				
#line 2282 "args/argcheck2.cpp"
				
				goto _st29;
				_ctr94:
				{
#line 186 "args/argcheck2.rl"
					(rl_num *= 10) += ((( (*( p)))) - '0'); }
				
#line 2290 "args/argcheck2.cpp"
				
				{
#line 152 "args/argcheck2.rl"
					
					rl_check.values.push_back((int64_t)(rl_sign * rl_num));
				}
				
#line 2298 "args/argcheck2.cpp"
				
				goto _st29;
				_st29:
				p += 1;
				st_case_29:
				switch( ( (*( p))) ) {
					case 0: {
						goto _ctr9;
					}
					case 32: {
						goto _ctr10;
					}
					case 42: {
						goto _ctr11;
					}
					case 43: {
						goto _ctr12;
					}
					case 63: {
						goto _ctr13;
					}
					case 123: {
						goto _ctr14;
					}
					case 124: {
						goto _st27;
					}
				}
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr94;
				}
				{
					goto _st0;
				}
				_ctr63:
				{
#line 215 "args/argcheck2.rl"
					rl_cmp = CMP_GREATER; }
				
#line 2338 "args/argcheck2.cpp"
				
				goto _st30;
				_st30:
				p += 1;
				st_case_30:
				switch( ( (*( p))) ) {
					case 43: {
						goto _ctr68;
					}
					case 45: {
						goto _ctr69;
					}
					case 61: {
						goto _ctr96;
					}
				}
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr70;
				}
				{
					goto _st0;
				}
				_st31:
				p += 1;
				st_case_31:
				switch( ( (*( p))) ) {
					case 43: {
						goto _ctr97;
					}
					case 45: {
						goto _ctr98;
					}
				}
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr99;
				}
				{
					goto _st0;
				}
				_ctr97:
				{
#line 192 "args/argcheck2.rl"
					rl_sign = 1; }
				
#line 2383 "args/argcheck2.cpp"
				
				{
#line 183 "args/argcheck2.rl"
					rl_sign = 1; }
				
#line 2389 "args/argcheck2.cpp"
				
				goto _st32;
				_ctr98:
				{
#line 192 "args/argcheck2.rl"
					rl_sign = 1; }
				
#line 2397 "args/argcheck2.cpp"
				
				{
#line 184 "args/argcheck2.rl"
					rl_sign = -1; }
				
#line 2403 "args/argcheck2.cpp"
				
				goto _st32;
				_st32:
				p += 1;
				st_case_32:
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr101;
				}
				{
					goto _st0;
				}
				_ctr99:
				{
#line 192 "args/argcheck2.rl"
					rl_sign = 1; }
				
#line 2420 "args/argcheck2.cpp"
				
				{
#line 186 "args/argcheck2.rl"
					rl_num = 0; }
				
#line 2426 "args/argcheck2.cpp"
				
				{
#line 186 "args/argcheck2.rl"
					(rl_num *= 10) += ((( (*( p)))) - '0'); }
				
#line 2432 "args/argcheck2.cpp"
				
				{
#line 152 "args/argcheck2.rl"
					
					rl_check.values.push_back((int64_t)(rl_sign * rl_num));
				}
				
#line 2440 "args/argcheck2.cpp"
				
				goto _st33;
				_ctr101:
				{
#line 186 "args/argcheck2.rl"
					rl_num = 0; }
				
#line 2448 "args/argcheck2.cpp"
				
				{
#line 186 "args/argcheck2.rl"
					(rl_num *= 10) += ((( (*( p)))) - '0'); }
				
#line 2454 "args/argcheck2.cpp"
				
				{
#line 152 "args/argcheck2.rl"
					
					rl_check.values.push_back((int64_t)(rl_sign * rl_num));
				}
				
#line 2462 "args/argcheck2.cpp"
				
				goto _st33;
				_ctr104:
				{
#line 186 "args/argcheck2.rl"
					(rl_num *= 10) += ((( (*( p)))) - '0'); }
				
#line 2470 "args/argcheck2.cpp"
				
				{
#line 152 "args/argcheck2.rl"
					
					rl_check.values.push_back((int64_t)(rl_sign * rl_num));
				}
				
#line 2478 "args/argcheck2.cpp"
				
				goto _st33;
				_st33:
				p += 1;
				st_case_33:
				if ( ( (*( p))) == 44 ) {
					goto _st34;
				}
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr104;
				}
				{
					goto _st0;
				}
				_st34:
				p += 1;
				st_case_34:
				switch( ( (*( p))) ) {
					case 43: {
						goto _ctr105;
					}
					case 45: {
						goto _ctr106;
					}
				}
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr107;
				}
				{
					goto _st0;
				}
				_ctr105:
				{
#line 192 "args/argcheck2.rl"
					rl_sign = 1; }
				
#line 2515 "args/argcheck2.cpp"
				
				{
#line 183 "args/argcheck2.rl"
					rl_sign = 1; }
				
#line 2521 "args/argcheck2.cpp"
				
				goto _st35;
				_ctr106:
				{
#line 192 "args/argcheck2.rl"
					rl_sign = 1; }
				
#line 2529 "args/argcheck2.cpp"
				
				{
#line 184 "args/argcheck2.rl"
					rl_sign = -1; }
				
#line 2535 "args/argcheck2.cpp"
				
				goto _st35;
				_st35:
				p += 1;
				st_case_35:
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr109;
				}
				{
					goto _st0;
				}
				_ctr107:
				{
#line 192 "args/argcheck2.rl"
					rl_sign = 1; }
				
#line 2552 "args/argcheck2.cpp"
				
				{
#line 186 "args/argcheck2.rl"
					rl_num = 0; }
				
#line 2558 "args/argcheck2.cpp"
				
				{
#line 186 "args/argcheck2.rl"
					(rl_num *= 10) += ((( (*( p)))) - '0'); }
				
#line 2564 "args/argcheck2.cpp"
				
				{
#line 152 "args/argcheck2.rl"
					
					rl_check.values.push_back((int64_t)(rl_sign * rl_num));
				}
				
#line 2572 "args/argcheck2.cpp"
				
				goto _st36;
				_ctr109:
				{
#line 186 "args/argcheck2.rl"
					rl_num = 0; }
				
#line 2580 "args/argcheck2.cpp"
				
				{
#line 186 "args/argcheck2.rl"
					(rl_num *= 10) += ((( (*( p)))) - '0'); }
				
#line 2586 "args/argcheck2.cpp"
				
				{
#line 152 "args/argcheck2.rl"
					
					rl_check.values.push_back((int64_t)(rl_sign * rl_num));
				}
				
#line 2594 "args/argcheck2.cpp"
				
				goto _st36;
				_ctr112:
				{
#line 186 "args/argcheck2.rl"
					(rl_num *= 10) += ((( (*( p)))) - '0'); }
				
#line 2602 "args/argcheck2.cpp"
				
				{
#line 152 "args/argcheck2.rl"
					
					rl_check.values.push_back((int64_t)(rl_sign * rl_num));
				}
				
#line 2610 "args/argcheck2.cpp"
				
				goto _st36;
				_st36:
				p += 1;
				st_case_36:
				switch( ( (*( p))) ) {
					case 41: {
						goto _ctr111;
					}
					case 93: {
						goto _ctr113;
					}
				}
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr112;
				}
				{
					goto _st0;
				}
				_st37:
				p += 1;
				st_case_37:
				if ( ( (*( p))) == 50 ) {
					goto _ctr114;
				}
				{
					goto _st0;
				}
				_ctr7:
				{
#line 255 "args/argcheck2.rl"
					rl_type = CHECK_SYMBOL; }
				
#line 2644 "args/argcheck2.cpp"
				
				goto _st38;
				_st38:
				p += 1;
				st_case_38:
				switch( ( (*( p))) ) {
					case 0: {
						goto _ctr9;
					}
					case 32: {
						goto _ctr10;
					}
					case 42: {
						goto _ctr11;
					}
					case 43: {
						goto _ctr12;
					}
					case 61: {
						goto _ctr116;
					}
					case 63: {
						goto _ctr13;
					}
					case 123: {
						goto _ctr14;
					}
				}
				{
					goto _st0;
				}
				_ctr116:
				{
#line 245 "args/argcheck2.rl"
					rl_cmp = CMP_EQUAL; }
				
#line 2681 "args/argcheck2.cpp"
				
				goto _st39;
				_ctr125:
				{
#line 238 "args/argcheck2.rl"
					rl_sym_start = p; }
				
#line 2689 "args/argcheck2.cpp"
				
				{
#line 164 "args/argcheck2.rl"
					
					try {
						ArgString str{ {}, 0 };
						str.first.assign(rl_sym_start, p - rl_sym_start);
						str.second = crc32_hash(str.first.data());
						rl_check.values.push_back(str);
					} catch(std::exception& e) {
						LIB_ERR << "exception: " << e.what();
					}
				}
				
#line 2704 "args/argcheck2.cpp"
				
				goto _st39;
				_ctr129:
				{
#line 164 "args/argcheck2.rl"
					
					try {
						ArgString str{ {}, 0 };
						str.first.assign(rl_sym_start, p - rl_sym_start);
						str.second = crc32_hash(str.first.data());
						rl_check.values.push_back(str);
					} catch(std::exception& e) {
						LIB_ERR << "exception: " << e.what();
					}
				}
				
#line 2721 "args/argcheck2.cpp"
				
				goto _st39;
				_st39:
				p += 1;
				st_case_39:
				switch( ( (*( p))) ) {
					case 0: {
						goto _ctr119;
					}
					case 32: {
						goto _ctr120;
					}
					case 42: {
						goto _ctr121;
					}
					case 43: {
						goto _ctr122;
					}
					case 63: {
						goto _ctr123;
					}
					case 123: {
						goto _ctr124;
					}
					case 124: {
						goto _ctr125;
					}
				}
				{
					goto _ctr118;
				}
				_ctr135:
				{
#line 180 "args/argcheck2.rl"
					rl_max = rl_min; }
				
#line 2758 "args/argcheck2.cpp"
				
				goto _st40;
				_ctr136:
				{
#line 179 "args/argcheck2.rl"
					rl_max = 0; }
				
#line 2766 "args/argcheck2.cpp"
				
				goto _st40;
				_ctr118:
				{
#line 238 "args/argcheck2.rl"
					rl_sym_start = p; }
				
#line 2774 "args/argcheck2.cpp"
				
				goto _st40;
				_ctr121:
				{
#line 238 "args/argcheck2.rl"
					rl_sym_start = p; }
				
#line 2782 "args/argcheck2.cpp"
				
				{
#line 164 "args/argcheck2.rl"
					
					try {
						ArgString str{ {}, 0 };
						str.first.assign(rl_sym_start, p - rl_sym_start);
						str.second = crc32_hash(str.first.data());
						rl_check.values.push_back(str);
					} catch(std::exception& e) {
						LIB_ERR << "exception: " << e.what();
					}
				}
				
#line 2797 "args/argcheck2.cpp"
				
				{
#line 269 "args/argcheck2.rl"
					rl_min = 1; rl_max = REPEAT_INF; }
				
#line 2803 "args/argcheck2.cpp"
				
				{
#line 259 "args/argcheck2.rl"
					rl_min = 0; rl_max = REPEAT_INF; }
				
#line 2809 "args/argcheck2.cpp"
				
				goto _st40;
				_ctr122:
				{
#line 238 "args/argcheck2.rl"
					rl_sym_start = p; }
				
#line 2817 "args/argcheck2.cpp"
				
				{
#line 164 "args/argcheck2.rl"
					
					try {
						ArgString str{ {}, 0 };
						str.first.assign(rl_sym_start, p - rl_sym_start);
						str.second = crc32_hash(str.first.data());
						rl_check.values.push_back(str);
					} catch(std::exception& e) {
						LIB_ERR << "exception: " << e.what();
					}
				}
				
#line 2832 "args/argcheck2.cpp"
				
				{
#line 269 "args/argcheck2.rl"
					rl_min = 1; rl_max = REPEAT_INF; }
				
#line 2838 "args/argcheck2.cpp"
				
				{
#line 258 "args/argcheck2.rl"
					rl_min = 1, rl_max = REPEAT_INF; }
				
#line 2844 "args/argcheck2.cpp"
				
				goto _st40;
				_ctr123:
				{
#line 238 "args/argcheck2.rl"
					rl_sym_start = p; }
				
#line 2852 "args/argcheck2.cpp"
				
				{
#line 164 "args/argcheck2.rl"
					
					try {
						ArgString str{ {}, 0 };
						str.first.assign(rl_sym_start, p - rl_sym_start);
						str.second = crc32_hash(str.first.data());
						rl_check.values.push_back(str);
					} catch(std::exception& e) {
						LIB_ERR << "exception: " << e.what();
					}
				}
				
#line 2867 "args/argcheck2.cpp"
				
				{
#line 269 "args/argcheck2.rl"
					rl_min = 1; rl_max = REPEAT_INF; }
				
#line 2873 "args/argcheck2.cpp"
				
				{
#line 257 "args/argcheck2.rl"
					rl_min = 0; rl_max = 1; }
				
#line 2879 "args/argcheck2.cpp"
				
				goto _st40;
				_st40:
				p += 1;
				st_case_40:
				switch( ( (*( p))) ) {
					case 0: {
						goto _ctr127;
					}
					case 32: {
						goto _ctr128;
					}
					case 124: {
						goto _ctr129;
					}
				}
				{
					goto _st40;
				}
				_ctr124:
				{
#line 238 "args/argcheck2.rl"
					rl_sym_start = p; }
				
#line 2904 "args/argcheck2.cpp"
				
				{
#line 164 "args/argcheck2.rl"
					
					try {
						ArgString str{ {}, 0 };
						str.first.assign(rl_sym_start, p - rl_sym_start);
						str.second = crc32_hash(str.first.data());
						rl_check.values.push_back(str);
					} catch(std::exception& e) {
						LIB_ERR << "exception: " << e.what();
					}
				}
				
#line 2919 "args/argcheck2.cpp"
				
				{
#line 269 "args/argcheck2.rl"
					rl_min = 1; rl_max = REPEAT_INF; }
				
#line 2925 "args/argcheck2.cpp"
				
				goto _st41;
				_st41:
				p += 1;
				st_case_41:
				switch( ( (*( p))) ) {
					case 0: {
						goto _ctr127;
					}
					case 32: {
						goto _ctr128;
					}
					case 48: {
						goto _ctr131;
					}
					case 124: {
						goto _ctr129;
					}
				}
				if ( 49 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr132;
				}
				{
					goto _st40;
				}
				_ctr131:
				{
#line 178 "args/argcheck2.rl"
					rl_min = 0; }
				
#line 2956 "args/argcheck2.cpp"
				
				goto _st42;
				_st42:
				p += 1;
				st_case_42:
				switch( ( (*( p))) ) {
					case 0: {
						goto _ctr127;
					}
					case 32: {
						goto _ctr128;
					}
					case 44: {
						goto _st43;
					}
					case 124: {
						goto _ctr129;
					}
					case 125: {
						goto _ctr135;
					}
				}
				{
					goto _st40;
				}
				_st43:
				p += 1;
				st_case_43:
				switch( ( (*( p))) ) {
					case 0: {
						goto _ctr127;
					}
					case 32: {
						goto _ctr128;
					}
					case 48: {
						goto _ctr136;
					}
					case 124: {
						goto _ctr129;
					}
				}
				if ( 49 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr137;
				}
				{
					goto _st40;
				}
				_ctr137:
				{
#line 179 "args/argcheck2.rl"
					rl_max = (( (*( p))))-'0'; }
				
#line 3010 "args/argcheck2.cpp"
				
				goto _st44;
				_ctr139:
				{
#line 179 "args/argcheck2.rl"
					(rl_max *= 10) += ((( (*( p)))) - '0'); }
				
#line 3018 "args/argcheck2.cpp"
				
				goto _st44;
				_st44:
				p += 1;
				st_case_44:
				switch( ( (*( p))) ) {
					case 0: {
						goto _ctr127;
					}
					case 32: {
						goto _ctr128;
					}
					case 124: {
						goto _ctr129;
					}
				}
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr139;
				}
				{
					goto _st40;
				}
				_ctr132:
				{
#line 178 "args/argcheck2.rl"
					rl_min = (( (*( p))))-'0'; }
				
#line 3046 "args/argcheck2.cpp"
				
				goto _st45;
				_ctr141:
				{
#line 178 "args/argcheck2.rl"
					(rl_min *= 10) += ((( (*( p)))) - '0'); }
				
#line 3054 "args/argcheck2.cpp"
				
				goto _st45;
				_st45:
				p += 1;
				st_case_45:
				switch( ( (*( p))) ) {
					case 0: {
						goto _ctr127;
					}
					case 32: {
						goto _ctr128;
					}
					case 44: {
						goto _st43;
					}
					case 124: {
						goto _ctr129;
					}
					case 125: {
						goto _ctr135;
					}
				}
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr141;
				}
				{
					goto _st40;
				}
				st_out:
				_test_eof1: cs = 1; goto _test_eof; 
				_test_eof2: cs = 2; goto _test_eof; 
				_test_eof46: cs = 46; goto _test_eof; 
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
				
				_test_eof: {}
				if ( cs >= 46 )
					goto _out; _pop: {}
				_out: {}
			}
			
#line 574 "args/argcheck2.rl"
			
			
			if (cs < 
#line 3141 "args/argcheck2.cpp"
			46
#line 576 "args/argcheck2.rl"
			) {
				LIB_ERR << fmt::format("invalid format string: '{}'", str);
				chk_.reset();
			}
		}
		
		bool check_args(const char* arg_string, const AtomListView& lv, BaseObject* obj)
		{
			ArgChecker chk(arg_string);
			return chk.check(lv, obj);
		}
		
	}
}
