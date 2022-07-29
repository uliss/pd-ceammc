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
	
	// keep in sync with CheckType values!
	const char* typeNames[] = {
		"atom",
		"bool",
		"byte",
		"int",
		"float",
		"symbol",
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
		
		inline std::string argName() const {
			if (name.empty())
				return typeNames[atom_type];
			else
				return { name.data(), name.size() };
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


#line 151 "args/argcheck2.cpp"
static const int arg_check2_start = 1;
static const int arg_check2_first_final = 57;
static const int arg_check2_error = 0;

static const int arg_check2_en_main = 1;


#line 298 "args/argcheck2.rl"


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
						pdError(x, fmt::format("invalid {} value at [{}]: '{}'", c.argName(), i, atom_to_string(a)));
						return false;
					}
					break;
					case CHECK_BYTE:
					if (!a.isInteger() || a.asT<int>() < 0 || a.asT<int>() > 255) {
						pdError(x, fmt::format("invalid {} value at [{}]: '{}'", c.argName(), i, atom_to_string(a)));
						return false;
					} else {
						debug("byte", "Ok");
						i++;
					}
					break;
					case CHECK_INT:
					if (!a.isInteger()) {
						pdError(x, fmt::format("invalid {} value at [{}]: '{}'", c.argName(), i, atom_to_string(a)));
						return false;
					} else {
						const int64_t val = a.asT<int>();
						const int64_t arg = (c.values.size() == 1 && boost::get<int64_t>(&c.values[0]))
						? *boost::strict_get<int64_t>(&c.values[0])
						: -999999999;
						
						switch (c.cmp_type) {
							case CMP_LESS:
							if (!(val < arg)) {
								pdError(x, fmt::format("{} at [{}] expected to be <{}, got: {}", c.argName(), i, arg, val));
								return false;
							}
							break;
							case CMP_LESS_EQ:
							if (!(val <= arg)) {
								pdError(x, fmt::format("{} at [{}] expected to be <={}, got: {}", c.argName(), i, arg, val));
								return false;
							}
							break;
							case CMP_GREATER:
							if (!(val > arg)) {
								pdError(x, fmt::format("{} at [{}] expected to be >{}, got: {}", c.argName(), i, arg, val));
								return false;
							}
							break;
							case CMP_GREATER_EQ:
							if (!(val >= arg)) {
								pdError(x, fmt::format("{} at [{}] expected to be >={}, got: {}", c.argName(), i, arg, val));
								return false;
							}
							break;
							case CMP_EQUAL:
							if (c.values.size() == 1) {
								if (val != arg) {
									pdError(x, fmt::format("{} at [{}] expected to be = {}, got: {}",
									c.argName(), i, arg_to_string(c.values[0]), atom_to_string(a)));
									return false;
								}
							} else {
								bool found = false;
								for (auto& v: c.values) {
									if (c.isEqual(v, val)) { found = true; break; }
								}
								if (!found) {
									pdError(x, fmt::format("{} at [{}] expected to be one of: {}, got: {}",
									c.argName(), i, arg_to_string(c.values), atom_to_string(a)));
									return false;
								}
							}
							break;
							case CMP_NOT_EQUAL:
							if (val == arg) {
								pdError(x, fmt::format("{} at [{}] expected to be !={}, got: {}", c.argName(), i, arg, val));
								return false;
							}
							break;
							case CMP_MODULE:
							fmt::print("val={}, arg={}\n", val, arg);
							if (val % arg != 0) {
								pdError(x, fmt::format("{} at [{}] expected to be multiple of {}, got: {}", c.argName(), i, arg, val));
								return false;
							}
							break;
							case CMP_POWER2: {
								bool rc = (val > 0 && ((val & (val - 1)) == 0));
								if (!rc) {
									pdError(x, fmt::format("{} at [{}] expected to be power of 2, got: {}", c.argName(), i, val));
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
									pdError(x, fmt::format("{} value at [{}] expected to be in [{},{}] range, got: {}", c.argName(), i, a, b, val));
									return false;
								}
								
								if (c.cmp_type == CMP_RANGE_SEMIOPEN && !(a <= val && val < b)) {
									pdError(x, fmt::format("{} at [{}] expected to be in [{},{}) range, got: {}", c.argName(), i, a, b, val));
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
							pdError(x, fmt::format("invalid {} value at [{}]: '{}'", c.argName(), i, atom_to_string(a)));
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
										pdError(x, fmt::format("{} at [{}] expected to be {}, got: '{}'",
									c.argName(), i, arg_to_string(c.values[0]), atom_to_string(a)));
									else
										pdError(x, fmt::format("{} at [{}] expected to be one of: {}, got: '{}'",
									c.argName(), i, arg_to_string(c.values), atom_to_string(a)));
									return false;
								}
							}
							break;
							default:
							break;
						}
						i++;
					}
					break;
					case CHECK_FLOAT: {
						if (!a.isFloat()) {
							pdError(x, fmt::format("invalid {} value at [{}]: '{}'", c.argName(), i, atom_to_string(a)));
							return false;
						}
						
						auto val = a.asT<t_float>();
						const t_float arg = (c.values.size() == 1 && boost::get<double>(&c.values[0]))
						? *boost::strict_get<double>(&c.values[0])
						: -999999999;
						
						switch (c.cmp_type) {
							case CMP_LESS:
							if (!(val < arg)) {
								pdError(x, fmt::format("{} at [{}] expected to be <{}, got: {}", c.argName(), i, arg, val));
								return false;
							}
							break;
							case CMP_LESS_EQ:
							if (!(val <= arg)) {
								pdError(x, fmt::format("{} at [{}] expected to be <={}, got: {}", c.argName(), i, arg, val));
								return false;
							}
							break;
							case CMP_GREATER:
							if (!(val > arg)) {
								pdError(x, fmt::format("{} at [{}] expected to be >{}, got: {}", c.argName(), i, arg, val));
								return false;
							}
							break;
							case CMP_GREATER_EQ:
							if (!(val >= arg)) {
								pdError(x, fmt::format("{} at [{}] expected to be >={}, got: {}", c.argName(), i, arg, val));
								return false;
							}
							break;
							default:
							break;
						}
						i++;
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
			
			
#line 455 "args/argcheck2.cpp"
			{
				cs = (int)arg_check2_start;
			}
			
#line 460 "args/argcheck2.cpp"
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
					case 57:
					goto st_case_57;
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
				}
				goto st_out;
				_ctr28:
				{
#line 289 "args/argcheck2.rl"
					rl_min = 1; rl_max = REPEAT_INF; }
				
#line 586 "args/argcheck2.cpp"
				
				{
#line 151 "args/argcheck2.rl"
					
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
				
#line 608 "args/argcheck2.cpp"
				
				goto _st1;
				_ctr35:
				{
#line 151 "args/argcheck2.rl"
					
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
				
#line 632 "args/argcheck2.cpp"
				
				goto _st1;
				_ctr62:
				{
#line 173 "args/argcheck2.rl"
					
					double real = rl_num;
					if (rl_den_cnt)
					real += rl_den / double(rl_den_cnt);
					
					rl_check.values.push_back(rl_sign * real);
				}
				
#line 646 "args/argcheck2.cpp"
				
				{
#line 289 "args/argcheck2.rl"
					rl_min = 1; rl_max = REPEAT_INF; }
				
#line 652 "args/argcheck2.cpp"
				
				{
#line 151 "args/argcheck2.rl"
					
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
				
#line 674 "args/argcheck2.cpp"
				
				goto _st1;
				_ctr93:
				{
#line 169 "args/argcheck2.rl"
					
					rl_check.values.push_back((int64_t)(rl_sign * rl_num));
				}
				
#line 684 "args/argcheck2.cpp"
				
				{
#line 289 "args/argcheck2.rl"
					rl_min = 1; rl_max = REPEAT_INF; }
				
#line 690 "args/argcheck2.cpp"
				
				{
#line 151 "args/argcheck2.rl"
					
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
				
#line 712 "args/argcheck2.cpp"
				
				goto _st1;
				_ctr138:
				{
#line 255 "args/argcheck2.rl"
					rl_sym_start = p; }
				
#line 720 "args/argcheck2.cpp"
				
				{
#line 181 "args/argcheck2.rl"
					
					try {
						ArgString str{ {}, 0 };
						str.first.assign(rl_sym_start, p - rl_sym_start);
						str.second = crc32_hash(str.first.data());
						rl_check.values.push_back(str);
					} catch(std::exception& e) {
						LIB_ERR << "exception: " << e.what();
					}
				}
				
#line 735 "args/argcheck2.cpp"
				
				{
#line 289 "args/argcheck2.rl"
					rl_min = 1; rl_max = REPEAT_INF; }
				
#line 741 "args/argcheck2.cpp"
				
				{
#line 151 "args/argcheck2.rl"
					
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
				
#line 763 "args/argcheck2.cpp"
				
				goto _st1;
				_ctr146:
				{
#line 181 "args/argcheck2.rl"
					
					try {
						ArgString str{ {}, 0 };
						str.first.assign(rl_sym_start, p - rl_sym_start);
						str.second = crc32_hash(str.first.data());
						rl_check.values.push_back(str);
					} catch(std::exception& e) {
						LIB_ERR << "exception: " << e.what();
					}
				}
				
#line 780 "args/argcheck2.cpp"
				
				{
#line 289 "args/argcheck2.rl"
					rl_min = 1; rl_max = REPEAT_INF; }
				
#line 786 "args/argcheck2.cpp"
				
				{
#line 151 "args/argcheck2.rl"
					
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
				
#line 808 "args/argcheck2.cpp"
				
				goto _st1;
				_st1:
				p += 1;
				st_case_1:
				switch( ( (*( p))) ) {
					case 66: {
						goto _ctr3;
					}
					case 95: {
						goto _ctr2;
					}
					case 97: {
						goto _ctr4;
					}
					case 98: {
						goto _ctr5;
					}
					case 102: {
						goto _ctr6;
					}
					case 105: {
						goto _ctr7;
					}
					case 115: {
						goto _ctr8;
					}
				}
				if ( ( (*( p))) > 57 ) {
					if ( 63 <= ( (*( p))) && ( (*( p))) <= 90 ) {
						goto _ctr2;
					}
				} else if ( ( (*( p))) >= 48 ) {
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
#line 280 "args/argcheck2.rl"
					rl_check.name.clear(); }
				
#line 856 "args/argcheck2.cpp"
				
				{
#line 280 "args/argcheck2.rl"
					rl_check.name.push_back((( (*( p))))); }
				
#line 862 "args/argcheck2.cpp"
				
				goto _st2;
				_st2:
				p += 1;
				st_case_2:
				switch( ( (*( p))) ) {
					case 58: {
						goto _st10;
					}
					case 95: {
						goto _ctr10;
					}
				}
				if ( ( (*( p))) > 57 ) {
					if ( 63 <= ( (*( p))) && ( (*( p))) <= 90 ) {
						goto _ctr10;
					}
				} else if ( ( (*( p))) >= 48 ) {
					goto _ctr10;
				}
				{
					goto _st0;
				}
				_ctr10:
				{
#line 280 "args/argcheck2.rl"
					rl_check.name.push_back((( (*( p))))); }
				
#line 891 "args/argcheck2.cpp"
				
				goto _st3;
				_st3:
				p += 1;
				st_case_3:
				switch( ( (*( p))) ) {
					case 58: {
						goto _st10;
					}
					case 95: {
						goto _ctr13;
					}
				}
				if ( ( (*( p))) > 57 ) {
					if ( 63 <= ( (*( p))) && ( (*( p))) <= 90 ) {
						goto _ctr13;
					}
				} else if ( ( (*( p))) >= 48 ) {
					goto _ctr13;
				}
				{
					goto _st0;
				}
				_ctr13:
				{
#line 280 "args/argcheck2.rl"
					rl_check.name.push_back((( (*( p))))); }
				
#line 920 "args/argcheck2.cpp"
				
				goto _st4;
				_st4:
				p += 1;
				st_case_4:
				switch( ( (*( p))) ) {
					case 58: {
						goto _st10;
					}
					case 95: {
						goto _ctr15;
					}
				}
				if ( ( (*( p))) > 57 ) {
					if ( 63 <= ( (*( p))) && ( (*( p))) <= 90 ) {
						goto _ctr15;
					}
				} else if ( ( (*( p))) >= 48 ) {
					goto _ctr15;
				}
				{
					goto _st0;
				}
				_ctr15:
				{
#line 280 "args/argcheck2.rl"
					rl_check.name.push_back((( (*( p))))); }
				
#line 949 "args/argcheck2.cpp"
				
				goto _st5;
				_st5:
				p += 1;
				st_case_5:
				switch( ( (*( p))) ) {
					case 58: {
						goto _st10;
					}
					case 95: {
						goto _ctr17;
					}
				}
				if ( ( (*( p))) > 57 ) {
					if ( 63 <= ( (*( p))) && ( (*( p))) <= 90 ) {
						goto _ctr17;
					}
				} else if ( ( (*( p))) >= 48 ) {
					goto _ctr17;
				}
				{
					goto _st0;
				}
				_ctr17:
				{
#line 280 "args/argcheck2.rl"
					rl_check.name.push_back((( (*( p))))); }
				
#line 978 "args/argcheck2.cpp"
				
				goto _st6;
				_st6:
				p += 1;
				st_case_6:
				switch( ( (*( p))) ) {
					case 58: {
						goto _st10;
					}
					case 95: {
						goto _ctr19;
					}
				}
				if ( ( (*( p))) > 57 ) {
					if ( 63 <= ( (*( p))) && ( (*( p))) <= 90 ) {
						goto _ctr19;
					}
				} else if ( ( (*( p))) >= 48 ) {
					goto _ctr19;
				}
				{
					goto _st0;
				}
				_ctr19:
				{
#line 280 "args/argcheck2.rl"
					rl_check.name.push_back((( (*( p))))); }
				
#line 1007 "args/argcheck2.cpp"
				
				goto _st7;
				_st7:
				p += 1;
				st_case_7:
				switch( ( (*( p))) ) {
					case 58: {
						goto _st10;
					}
					case 95: {
						goto _ctr21;
					}
				}
				if ( ( (*( p))) > 57 ) {
					if ( 63 <= ( (*( p))) && ( (*( p))) <= 90 ) {
						goto _ctr21;
					}
				} else if ( ( (*( p))) >= 48 ) {
					goto _ctr21;
				}
				{
					goto _st0;
				}
				_ctr21:
				{
#line 280 "args/argcheck2.rl"
					rl_check.name.push_back((( (*( p))))); }
				
#line 1036 "args/argcheck2.cpp"
				
				goto _st8;
				_st8:
				p += 1;
				st_case_8:
				switch( ( (*( p))) ) {
					case 58: {
						goto _st10;
					}
					case 95: {
						goto _ctr23;
					}
				}
				if ( ( (*( p))) > 57 ) {
					if ( 63 <= ( (*( p))) && ( (*( p))) <= 90 ) {
						goto _ctr23;
					}
				} else if ( ( (*( p))) >= 48 ) {
					goto _ctr23;
				}
				{
					goto _st0;
				}
				_ctr23:
				{
#line 280 "args/argcheck2.rl"
					rl_check.name.push_back((( (*( p))))); }
				
#line 1065 "args/argcheck2.cpp"
				
				goto _st9;
				_st9:
				p += 1;
				st_case_9:
				if ( ( (*( p))) == 58 ) {
					goto _st10;
				}
				{
					goto _st0;
				}
				_st10:
				p += 1;
				st_case_10:
				switch( ( (*( p))) ) {
					case 66: {
						goto _ctr25;
					}
					case 97: {
						goto _ctr4;
					}
					case 98: {
						goto _ctr5;
					}
					case 102: {
						goto _ctr6;
					}
					case 105: {
						goto _ctr7;
					}
					case 115: {
						goto _ctr8;
					}
				}
				{
					goto _st0;
				}
				_ctr4:
				{
#line 268 "args/argcheck2.rl"
					rl_type = CHECK_ATOM; }
				
#line 1108 "args/argcheck2.cpp"
				
				goto _st11;
				_ctr5:
				{
#line 270 "args/argcheck2.rl"
					rl_type = CHECK_BYTE; }
				
#line 1116 "args/argcheck2.cpp"
				
				goto _st11;
				_ctr25:
				{
#line 269 "args/argcheck2.rl"
					rl_type = CHECK_BOOL; }
				
#line 1124 "args/argcheck2.cpp"
				
				goto _st11;
				_ctr129:
				{
#line 221 "args/argcheck2.rl"
					rl_cmp = CMP_RANGE_SEMIOPEN; }
				
#line 1132 "args/argcheck2.cpp"
				
				goto _st11;
				_ctr131:
				{
#line 221 "args/argcheck2.rl"
					rl_cmp = CMP_RANGE_CLOSED; }
				
#line 1140 "args/argcheck2.cpp"
				
				goto _st11;
				_ctr132:
				{
#line 246 "args/argcheck2.rl"
					rl_cmp = CMP_POWER2; }
				
#line 1148 "args/argcheck2.cpp"
				
				goto _st11;
				_st11:
				p += 1;
				st_case_11:
				switch( ( (*( p))) ) {
					case 0: {
						goto _ctr27;
					}
					case 32: {
						goto _ctr28;
					}
					case 42: {
						goto _ctr29;
					}
					case 43: {
						goto _ctr30;
					}
					case 63: {
						goto _ctr31;
					}
					case 123: {
						goto _ctr32;
					}
				}
				{
					goto _st0;
				}
				_ctr27:
				{
#line 289 "args/argcheck2.rl"
					rl_min = 1; rl_max = REPEAT_INF; }
				
#line 1182 "args/argcheck2.cpp"
				
				{
#line 151 "args/argcheck2.rl"
					
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
				
#line 1204 "args/argcheck2.cpp"
				
				{
#line 294 "args/argcheck2.rl"
					{p+= 1; cs = 57; goto _out;} }
				
#line 1210 "args/argcheck2.cpp"
				
				goto _st57;
				_ctr34:
				{
#line 151 "args/argcheck2.rl"
					
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
				
#line 1234 "args/argcheck2.cpp"
				
				{
#line 294 "args/argcheck2.rl"
					{p+= 1; cs = 57; goto _out;} }
				
#line 1240 "args/argcheck2.cpp"
				
				goto _st57;
				_ctr61:
				{
#line 173 "args/argcheck2.rl"
					
					double real = rl_num;
					if (rl_den_cnt)
					real += rl_den / double(rl_den_cnt);
					
					rl_check.values.push_back(rl_sign * real);
				}
				
#line 1254 "args/argcheck2.cpp"
				
				{
#line 289 "args/argcheck2.rl"
					rl_min = 1; rl_max = REPEAT_INF; }
				
#line 1260 "args/argcheck2.cpp"
				
				{
#line 151 "args/argcheck2.rl"
					
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
				
#line 1282 "args/argcheck2.cpp"
				
				{
#line 294 "args/argcheck2.rl"
					{p+= 1; cs = 57; goto _out;} }
				
#line 1288 "args/argcheck2.cpp"
				
				goto _st57;
				_ctr92:
				{
#line 169 "args/argcheck2.rl"
					
					rl_check.values.push_back((int64_t)(rl_sign * rl_num));
				}
				
#line 1298 "args/argcheck2.cpp"
				
				{
#line 289 "args/argcheck2.rl"
					rl_min = 1; rl_max = REPEAT_INF; }
				
#line 1304 "args/argcheck2.cpp"
				
				{
#line 151 "args/argcheck2.rl"
					
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
				
#line 1326 "args/argcheck2.cpp"
				
				{
#line 294 "args/argcheck2.rl"
					{p+= 1; cs = 57; goto _out;} }
				
#line 1332 "args/argcheck2.cpp"
				
				goto _st57;
				_ctr137:
				{
#line 255 "args/argcheck2.rl"
					rl_sym_start = p; }
				
#line 1340 "args/argcheck2.cpp"
				
				{
#line 181 "args/argcheck2.rl"
					
					try {
						ArgString str{ {}, 0 };
						str.first.assign(rl_sym_start, p - rl_sym_start);
						str.second = crc32_hash(str.first.data());
						rl_check.values.push_back(str);
					} catch(std::exception& e) {
						LIB_ERR << "exception: " << e.what();
					}
				}
				
#line 1355 "args/argcheck2.cpp"
				
				{
#line 289 "args/argcheck2.rl"
					rl_min = 1; rl_max = REPEAT_INF; }
				
#line 1361 "args/argcheck2.cpp"
				
				{
#line 151 "args/argcheck2.rl"
					
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
				
#line 1383 "args/argcheck2.cpp"
				
				{
#line 294 "args/argcheck2.rl"
					{p+= 1; cs = 57; goto _out;} }
				
#line 1389 "args/argcheck2.cpp"
				
				goto _st57;
				_ctr145:
				{
#line 181 "args/argcheck2.rl"
					
					try {
						ArgString str{ {}, 0 };
						str.first.assign(rl_sym_start, p - rl_sym_start);
						str.second = crc32_hash(str.first.data());
						rl_check.values.push_back(str);
					} catch(std::exception& e) {
						LIB_ERR << "exception: " << e.what();
					}
				}
				
#line 1406 "args/argcheck2.cpp"
				
				{
#line 289 "args/argcheck2.rl"
					rl_min = 1; rl_max = REPEAT_INF; }
				
#line 1412 "args/argcheck2.cpp"
				
				{
#line 151 "args/argcheck2.rl"
					
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
				
#line 1434 "args/argcheck2.cpp"
				
				{
#line 294 "args/argcheck2.rl"
					{p+= 1; cs = 57; goto _out;} }
				
#line 1440 "args/argcheck2.cpp"
				
				goto _st57;
				_st57:
				p += 1;
				st_case_57:
				{
					goto _st0;
				}
				_ctr29:
				{
#line 289 "args/argcheck2.rl"
					rl_min = 1; rl_max = REPEAT_INF; }
				
#line 1454 "args/argcheck2.cpp"
				
				{
#line 276 "args/argcheck2.rl"
					rl_min = 0; rl_max = REPEAT_INF; }
				
#line 1460 "args/argcheck2.cpp"
				
				goto _st12;
				_ctr30:
				{
#line 289 "args/argcheck2.rl"
					rl_min = 1; rl_max = REPEAT_INF; }
				
#line 1468 "args/argcheck2.cpp"
				
				{
#line 275 "args/argcheck2.rl"
					rl_min = 1, rl_max = REPEAT_INF; }
				
#line 1474 "args/argcheck2.cpp"
				
				goto _st12;
				_ctr31:
				{
#line 289 "args/argcheck2.rl"
					rl_min = 1; rl_max = REPEAT_INF; }
				
#line 1482 "args/argcheck2.cpp"
				
				{
#line 274 "args/argcheck2.rl"
					rl_min = 0; rl_max = 1; }
				
#line 1488 "args/argcheck2.cpp"
				
				goto _st12;
				_ctr41:
				{
#line 197 "args/argcheck2.rl"
					rl_max = rl_min; }
				
#line 1496 "args/argcheck2.cpp"
				
				goto _st12;
				_ctr63:
				{
#line 173 "args/argcheck2.rl"
					
					double real = rl_num;
					if (rl_den_cnt)
					real += rl_den / double(rl_den_cnt);
					
					rl_check.values.push_back(rl_sign * real);
				}
				
#line 1510 "args/argcheck2.cpp"
				
				{
#line 289 "args/argcheck2.rl"
					rl_min = 1; rl_max = REPEAT_INF; }
				
#line 1516 "args/argcheck2.cpp"
				
				{
#line 276 "args/argcheck2.rl"
					rl_min = 0; rl_max = REPEAT_INF; }
				
#line 1522 "args/argcheck2.cpp"
				
				goto _st12;
				_ctr64:
				{
#line 173 "args/argcheck2.rl"
					
					double real = rl_num;
					if (rl_den_cnt)
					real += rl_den / double(rl_den_cnt);
					
					rl_check.values.push_back(rl_sign * real);
				}
				
#line 1536 "args/argcheck2.cpp"
				
				{
#line 289 "args/argcheck2.rl"
					rl_min = 1; rl_max = REPEAT_INF; }
				
#line 1542 "args/argcheck2.cpp"
				
				{
#line 275 "args/argcheck2.rl"
					rl_min = 1, rl_max = REPEAT_INF; }
				
#line 1548 "args/argcheck2.cpp"
				
				goto _st12;
				_ctr67:
				{
#line 173 "args/argcheck2.rl"
					
					double real = rl_num;
					if (rl_den_cnt)
					real += rl_den / double(rl_den_cnt);
					
					rl_check.values.push_back(rl_sign * real);
				}
				
#line 1562 "args/argcheck2.cpp"
				
				{
#line 289 "args/argcheck2.rl"
					rl_min = 1; rl_max = REPEAT_INF; }
				
#line 1568 "args/argcheck2.cpp"
				
				{
#line 274 "args/argcheck2.rl"
					rl_min = 0; rl_max = 1; }
				
#line 1574 "args/argcheck2.cpp"
				
				goto _st12;
				_ctr94:
				{
#line 169 "args/argcheck2.rl"
					
					rl_check.values.push_back((int64_t)(rl_sign * rl_num));
				}
				
#line 1584 "args/argcheck2.cpp"
				
				{
#line 289 "args/argcheck2.rl"
					rl_min = 1; rl_max = REPEAT_INF; }
				
#line 1590 "args/argcheck2.cpp"
				
				{
#line 276 "args/argcheck2.rl"
					rl_min = 0; rl_max = REPEAT_INF; }
				
#line 1596 "args/argcheck2.cpp"
				
				goto _st12;
				_ctr95:
				{
#line 169 "args/argcheck2.rl"
					
					rl_check.values.push_back((int64_t)(rl_sign * rl_num));
				}
				
#line 1606 "args/argcheck2.cpp"
				
				{
#line 289 "args/argcheck2.rl"
					rl_min = 1; rl_max = REPEAT_INF; }
				
#line 1612 "args/argcheck2.cpp"
				
				{
#line 275 "args/argcheck2.rl"
					rl_min = 1, rl_max = REPEAT_INF; }
				
#line 1618 "args/argcheck2.cpp"
				
				goto _st12;
				_ctr97:
				{
#line 169 "args/argcheck2.rl"
					
					rl_check.values.push_back((int64_t)(rl_sign * rl_num));
				}
				
#line 1628 "args/argcheck2.cpp"
				
				{
#line 289 "args/argcheck2.rl"
					rl_min = 1; rl_max = REPEAT_INF; }
				
#line 1634 "args/argcheck2.cpp"
				
				{
#line 274 "args/argcheck2.rl"
					rl_min = 0; rl_max = 1; }
				
#line 1640 "args/argcheck2.cpp"
				
				goto _st12;
				_st12:
				p += 1;
				st_case_12:
				switch( ( (*( p))) ) {
					case 0: {
						goto _ctr34;
					}
					case 32: {
						goto _ctr35;
					}
				}
				{
					goto _st0;
				}
				_ctr32:
				{
#line 289 "args/argcheck2.rl"
					rl_min = 1; rl_max = REPEAT_INF; }
				
#line 1662 "args/argcheck2.cpp"
				
				goto _st13;
				_ctr68:
				{
#line 173 "args/argcheck2.rl"
					
					double real = rl_num;
					if (rl_den_cnt)
					real += rl_den / double(rl_den_cnt);
					
					rl_check.values.push_back(rl_sign * real);
				}
				
#line 1676 "args/argcheck2.cpp"
				
				{
#line 289 "args/argcheck2.rl"
					rl_min = 1; rl_max = REPEAT_INF; }
				
#line 1682 "args/argcheck2.cpp"
				
				goto _st13;
				_ctr98:
				{
#line 169 "args/argcheck2.rl"
					
					rl_check.values.push_back((int64_t)(rl_sign * rl_num));
				}
				
#line 1692 "args/argcheck2.cpp"
				
				{
#line 289 "args/argcheck2.rl"
					rl_min = 1; rl_max = REPEAT_INF; }
				
#line 1698 "args/argcheck2.cpp"
				
				goto _st13;
				_st13:
				p += 1;
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
#line 195 "args/argcheck2.rl"
					rl_min = 0; }
				
#line 1718 "args/argcheck2.cpp"
				
				goto _st14;
				_st14:
				p += 1;
				st_case_14:
				switch( ( (*( p))) ) {
					case 44: {
						goto _st15;
					}
					case 125: {
						goto _ctr41;
					}
				}
				{
					goto _st0;
				}
				_st15:
				p += 1;
				st_case_15:
				switch( ( (*( p))) ) {
					case 48: {
						goto _ctr42;
					}
					case 125: {
						goto _st12;
					}
				}
				if ( 49 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr43;
				}
				{
					goto _st0;
				}
				_ctr42:
				{
#line 196 "args/argcheck2.rl"
					rl_max = 0; }
				
#line 1757 "args/argcheck2.cpp"
				
				goto _st16;
				_st16:
				p += 1;
				st_case_16:
				if ( ( (*( p))) == 125 ) {
					goto _st12;
				}
				{
					goto _st0;
				}
				_ctr43:
				{
#line 196 "args/argcheck2.rl"
					rl_max = (( (*( p))))-'0'; }
				
#line 1774 "args/argcheck2.cpp"
				
				goto _st17;
				_ctr46:
				{
#line 196 "args/argcheck2.rl"
					(rl_max *= 10) += ((( (*( p)))) - '0'); }
				
#line 1782 "args/argcheck2.cpp"
				
				goto _st17;
				_st17:
				p += 1;
				st_case_17:
				if ( ( (*( p))) == 125 ) {
					goto _st12;
				}
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr46;
				}
				{
					goto _st0;
				}
				_ctr38:
				{
#line 195 "args/argcheck2.rl"
					rl_min = (( (*( p))))-'0'; }
				
#line 1802 "args/argcheck2.cpp"
				
				goto _st18;
				_ctr48:
				{
#line 195 "args/argcheck2.rl"
					(rl_min *= 10) += ((( (*( p)))) - '0'); }
				
#line 1810 "args/argcheck2.cpp"
				
				goto _st18;
				_st18:
				p += 1;
				st_case_18:
				switch( ( (*( p))) ) {
					case 44: {
						goto _st15;
					}
					case 125: {
						goto _ctr41;
					}
				}
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr48;
				}
				{
					goto _st0;
				}
				_ctr6:
				{
#line 273 "args/argcheck2.rl"
					rl_type = CHECK_FLOAT; }
				
#line 1835 "args/argcheck2.cpp"
				
				goto _st19;
				_st19:
				p += 1;
				st_case_19:
				switch( ( (*( p))) ) {
					case 0: {
						goto _ctr27;
					}
					case 32: {
						goto _ctr28;
					}
					case 33: {
						goto _st20;
					}
					case 42: {
						goto _ctr29;
					}
					case 43: {
						goto _ctr30;
					}
					case 60: {
						goto _ctr51;
					}
					case 62: {
						goto _ctr52;
					}
					case 63: {
						goto _ctr31;
					}
					case 123: {
						goto _ctr32;
					}
				}
				{
					goto _st0;
				}
				_st20:
				p += 1;
				st_case_20:
				if ( ( (*( p))) == 61 ) {
					goto _ctr53;
				}
				{
					goto _st0;
				}
				_ctr53:
				{
#line 234 "args/argcheck2.rl"
					rl_cmp = CMP_NOT_EQUAL; }
				
#line 1887 "args/argcheck2.cpp"
				
				goto _st21;
				_ctr73:
				{
#line 233 "args/argcheck2.rl"
					rl_cmp = CMP_LESS_EQ; }
				
#line 1895 "args/argcheck2.cpp"
				
				goto _st21;
				_ctr75:
				{
#line 232 "args/argcheck2.rl"
					rl_cmp = CMP_GREATER_EQ; }
				
#line 1903 "args/argcheck2.cpp"
				
				goto _st21;
				_st21:
				p += 1;
				st_case_21:
				switch( ( (*( p))) ) {
					case 43: {
						goto _ctr55;
					}
					case 45: {
						goto _ctr56;
					}
				}
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr57;
				}
				{
					goto _st0;
				}
				_ctr55:
				{
#line 209 "args/argcheck2.rl"
					rl_sign = 1; }
				
#line 1928 "args/argcheck2.cpp"
				
				{
#line 200 "args/argcheck2.rl"
					rl_sign = 1; }
				
#line 1934 "args/argcheck2.cpp"
				
				goto _st22;
				_ctr56:
				{
#line 209 "args/argcheck2.rl"
					rl_sign = 1; }
				
#line 1942 "args/argcheck2.cpp"
				
				{
#line 201 "args/argcheck2.rl"
					rl_sign = -1; }
				
#line 1948 "args/argcheck2.cpp"
				
				goto _st22;
				_st22:
				p += 1;
				st_case_22:
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr59;
				}
				{
					goto _st0;
				}
				_ctr57:
				{
#line 209 "args/argcheck2.rl"
					rl_sign = 1; }
				
#line 1965 "args/argcheck2.cpp"
				
				{
#line 203 "args/argcheck2.rl"
					rl_num = 0; }
				
#line 1971 "args/argcheck2.cpp"
				
				{
#line 203 "args/argcheck2.rl"
					(rl_num *= 10) += ((( (*( p)))) - '0'); }
				
#line 1977 "args/argcheck2.cpp"
				
				goto _st23;
				_ctr59:
				{
#line 203 "args/argcheck2.rl"
					rl_num = 0; }
				
#line 1985 "args/argcheck2.cpp"
				
				{
#line 203 "args/argcheck2.rl"
					(rl_num *= 10) += ((( (*( p)))) - '0'); }
				
#line 1991 "args/argcheck2.cpp"
				
				goto _st23;
				_ctr66:
				{
#line 203 "args/argcheck2.rl"
					(rl_num *= 10) += ((( (*( p)))) - '0'); }
				
#line 1999 "args/argcheck2.cpp"
				
				goto _st23;
				_st23:
				p += 1;
				st_case_23:
				switch( ( (*( p))) ) {
					case 0: {
						goto _ctr61;
					}
					case 32: {
						goto _ctr62;
					}
					case 42: {
						goto _ctr63;
					}
					case 43: {
						goto _ctr64;
					}
					case 46: {
						goto _st24;
					}
					case 63: {
						goto _ctr67;
					}
					case 123: {
						goto _ctr68;
					}
				}
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr66;
				}
				{
					goto _st0;
				}
				_st24:
				p += 1;
				st_case_24:
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr69;
				}
				{
					goto _st0;
				}
				_ctr69:
				{
#line 204 "args/argcheck2.rl"
					rl_den = 0; rl_den_cnt = 1; }
				
#line 2048 "args/argcheck2.cpp"
				
				{
#line 204 "args/argcheck2.rl"
					(rl_den *= 10) += ((( (*( p)))) - '0'); rl_den_cnt *= 10; }
				
#line 2054 "args/argcheck2.cpp"
				
				goto _st25;
				_ctr71:
				{
#line 204 "args/argcheck2.rl"
					(rl_den *= 10) += ((( (*( p)))) - '0'); rl_den_cnt *= 10; }
				
#line 2062 "args/argcheck2.cpp"
				
				goto _st25;
				_st25:
				p += 1;
				st_case_25:
				switch( ( (*( p))) ) {
					case 0: {
						goto _ctr61;
					}
					case 32: {
						goto _ctr62;
					}
					case 42: {
						goto _ctr63;
					}
					case 43: {
						goto _ctr64;
					}
					case 63: {
						goto _ctr67;
					}
					case 123: {
						goto _ctr68;
					}
				}
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr71;
				}
				{
					goto _st0;
				}
				_ctr51:
				{
#line 233 "args/argcheck2.rl"
					rl_cmp = CMP_LESS; }
				
#line 2099 "args/argcheck2.cpp"
				
				goto _st26;
				_st26:
				p += 1;
				st_case_26:
				switch( ( (*( p))) ) {
					case 43: {
						goto _ctr55;
					}
					case 45: {
						goto _ctr56;
					}
					case 61: {
						goto _ctr73;
					}
				}
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr57;
				}
				{
					goto _st0;
				}
				_ctr52:
				{
#line 232 "args/argcheck2.rl"
					rl_cmp = CMP_GREATER; }
				
#line 2127 "args/argcheck2.cpp"
				
				goto _st27;
				_st27:
				p += 1;
				st_case_27:
				switch( ( (*( p))) ) {
					case 43: {
						goto _ctr55;
					}
					case 45: {
						goto _ctr56;
					}
					case 61: {
						goto _ctr75;
					}
				}
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr57;
				}
				{
					goto _st0;
				}
				_ctr7:
				{
#line 271 "args/argcheck2.rl"
					rl_type = CHECK_INT; }
				
#line 2155 "args/argcheck2.cpp"
				
				goto _st28;
				_st28:
				p += 1;
				st_case_28:
				switch( ( (*( p))) ) {
					case 0: {
						goto _ctr27;
					}
					case 32: {
						goto _ctr28;
					}
					case 33: {
						goto _st29;
					}
					case 37: {
						goto _ctr78;
					}
					case 42: {
						goto _ctr29;
					}
					case 43: {
						goto _ctr30;
					}
					case 60: {
						goto _ctr79;
					}
					case 61: {
						goto _ctr80;
					}
					case 62: {
						goto _ctr81;
					}
					case 63: {
						goto _ctr31;
					}
					case 91: {
						goto _st40;
					}
					case 94: {
						goto _st46;
					}
					case 123: {
						goto _ctr32;
					}
				}
				{
					goto _st0;
				}
				_st29:
				p += 1;
				st_case_29:
				if ( ( (*( p))) == 61 ) {
					goto _ctr84;
				}
				{
					goto _st0;
				}
				_ctr84:
				{
#line 234 "args/argcheck2.rl"
					rl_cmp = CMP_NOT_EQUAL; }
				
#line 2219 "args/argcheck2.cpp"
				
				goto _st30;
				_ctr104:
				{
#line 233 "args/argcheck2.rl"
					rl_cmp = CMP_LESS_EQ; }
				
#line 2227 "args/argcheck2.cpp"
				
				goto _st30;
				_ctr114:
				{
#line 232 "args/argcheck2.rl"
					rl_cmp = CMP_GREATER_EQ; }
				
#line 2235 "args/argcheck2.cpp"
				
				goto _st30;
				_st30:
				p += 1;
				st_case_30:
				switch( ( (*( p))) ) {
					case 43: {
						goto _ctr86;
					}
					case 45: {
						goto _ctr87;
					}
				}
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr88;
				}
				{
					goto _st0;
				}
				_ctr86:
				{
#line 209 "args/argcheck2.rl"
					rl_sign = 1; }
				
#line 2260 "args/argcheck2.cpp"
				
				{
#line 200 "args/argcheck2.rl"
					rl_sign = 1; }
				
#line 2266 "args/argcheck2.cpp"
				
				goto _st31;
				_ctr87:
				{
#line 209 "args/argcheck2.rl"
					rl_sign = 1; }
				
#line 2274 "args/argcheck2.cpp"
				
				{
#line 201 "args/argcheck2.rl"
					rl_sign = -1; }
				
#line 2280 "args/argcheck2.cpp"
				
				goto _st31;
				_st31:
				p += 1;
				st_case_31:
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr90;
				}
				{
					goto _st0;
				}
				_ctr88:
				{
#line 209 "args/argcheck2.rl"
					rl_sign = 1; }
				
#line 2297 "args/argcheck2.cpp"
				
				{
#line 203 "args/argcheck2.rl"
					rl_num = 0; }
				
#line 2303 "args/argcheck2.cpp"
				
				{
#line 203 "args/argcheck2.rl"
					(rl_num *= 10) += ((( (*( p)))) - '0'); }
				
#line 2309 "args/argcheck2.cpp"
				
				goto _st32;
				_ctr90:
				{
#line 203 "args/argcheck2.rl"
					rl_num = 0; }
				
#line 2317 "args/argcheck2.cpp"
				
				{
#line 203 "args/argcheck2.rl"
					(rl_num *= 10) += ((( (*( p)))) - '0'); }
				
#line 2323 "args/argcheck2.cpp"
				
				goto _st32;
				_ctr96:
				{
#line 203 "args/argcheck2.rl"
					(rl_num *= 10) += ((( (*( p)))) - '0'); }
				
#line 2331 "args/argcheck2.cpp"
				
				goto _st32;
				_st32:
				p += 1;
				st_case_32:
				switch( ( (*( p))) ) {
					case 0: {
						goto _ctr92;
					}
					case 32: {
						goto _ctr93;
					}
					case 42: {
						goto _ctr94;
					}
					case 43: {
						goto _ctr95;
					}
					case 63: {
						goto _ctr97;
					}
					case 123: {
						goto _ctr98;
					}
				}
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr96;
				}
				{
					goto _st0;
				}
				_ctr78:
				{
#line 239 "args/argcheck2.rl"
					rl_cmp = CMP_MODULE; }
				
#line 2368 "args/argcheck2.cpp"
				
				goto _st33;
				_st33:
				p += 1;
				st_case_33:
				if ( 49 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr100;
				}
				{
					goto _st0;
				}
				_ctr100:
				{
#line 240 "args/argcheck2.rl"
					rl_sign = 1; rl_num = 0; }
				
#line 2385 "args/argcheck2.cpp"
				
				{
#line 240 "args/argcheck2.rl"
					(rl_num *= 10) += ((( (*( p)))) - '0'); }
				
#line 2391 "args/argcheck2.cpp"
				
				{
#line 169 "args/argcheck2.rl"
					
					rl_check.values.push_back((int64_t)(rl_sign * rl_num));
				}
				
#line 2399 "args/argcheck2.cpp"
				
				goto _st34;
				_ctr102:
				{
#line 240 "args/argcheck2.rl"
					(rl_num *= 10) += ((( (*( p)))) - '0'); }
				
#line 2407 "args/argcheck2.cpp"
				
				{
#line 169 "args/argcheck2.rl"
					
					rl_check.values.push_back((int64_t)(rl_sign * rl_num));
				}
				
#line 2415 "args/argcheck2.cpp"
				
				goto _st34;
				_st34:
				p += 1;
				st_case_34:
				switch( ( (*( p))) ) {
					case 0: {
						goto _ctr27;
					}
					case 32: {
						goto _ctr28;
					}
					case 42: {
						goto _ctr29;
					}
					case 43: {
						goto _ctr30;
					}
					case 63: {
						goto _ctr31;
					}
					case 123: {
						goto _ctr32;
					}
				}
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr102;
				}
				{
					goto _st0;
				}
				_ctr79:
				{
#line 233 "args/argcheck2.rl"
					rl_cmp = CMP_LESS; }
				
#line 2452 "args/argcheck2.cpp"
				
				goto _st35;
				_st35:
				p += 1;
				st_case_35:
				switch( ( (*( p))) ) {
					case 43: {
						goto _ctr86;
					}
					case 45: {
						goto _ctr87;
					}
					case 61: {
						goto _ctr104;
					}
				}
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr88;
				}
				{
					goto _st0;
				}
				_ctr80:
				{
#line 229 "args/argcheck2.rl"
					rl_cmp = CMP_EQUAL; }
				
#line 2480 "args/argcheck2.cpp"
				
				goto _st36;
				_st36:
				p += 1;
				st_case_36:
				switch( ( (*( p))) ) {
					case 43: {
						goto _ctr106;
					}
					case 45: {
						goto _ctr107;
					}
				}
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr108;
				}
				{
					goto _st0;
				}
				_ctr106:
				{
#line 209 "args/argcheck2.rl"
					rl_sign = 1; }
				
#line 2505 "args/argcheck2.cpp"
				
				{
#line 200 "args/argcheck2.rl"
					rl_sign = 1; }
				
#line 2511 "args/argcheck2.cpp"
				
				goto _st37;
				_ctr107:
				{
#line 209 "args/argcheck2.rl"
					rl_sign = 1; }
				
#line 2519 "args/argcheck2.cpp"
				
				{
#line 201 "args/argcheck2.rl"
					rl_sign = -1; }
				
#line 2525 "args/argcheck2.cpp"
				
				goto _st37;
				_st37:
				p += 1;
				st_case_37:
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr110;
				}
				{
					goto _st0;
				}
				_ctr108:
				{
#line 209 "args/argcheck2.rl"
					rl_sign = 1; }
				
#line 2542 "args/argcheck2.cpp"
				
				{
#line 203 "args/argcheck2.rl"
					rl_num = 0; }
				
#line 2548 "args/argcheck2.cpp"
				
				{
#line 203 "args/argcheck2.rl"
					(rl_num *= 10) += ((( (*( p)))) - '0'); }
				
#line 2554 "args/argcheck2.cpp"
				
				{
#line 169 "args/argcheck2.rl"
					
					rl_check.values.push_back((int64_t)(rl_sign * rl_num));
				}
				
#line 2562 "args/argcheck2.cpp"
				
				goto _st38;
				_ctr110:
				{
#line 203 "args/argcheck2.rl"
					rl_num = 0; }
				
#line 2570 "args/argcheck2.cpp"
				
				{
#line 203 "args/argcheck2.rl"
					(rl_num *= 10) += ((( (*( p)))) - '0'); }
				
#line 2576 "args/argcheck2.cpp"
				
				{
#line 169 "args/argcheck2.rl"
					
					rl_check.values.push_back((int64_t)(rl_sign * rl_num));
				}
				
#line 2584 "args/argcheck2.cpp"
				
				goto _st38;
				_ctr112:
				{
#line 203 "args/argcheck2.rl"
					(rl_num *= 10) += ((( (*( p)))) - '0'); }
				
#line 2592 "args/argcheck2.cpp"
				
				{
#line 169 "args/argcheck2.rl"
					
					rl_check.values.push_back((int64_t)(rl_sign * rl_num));
				}
				
#line 2600 "args/argcheck2.cpp"
				
				goto _st38;
				_st38:
				p += 1;
				st_case_38:
				switch( ( (*( p))) ) {
					case 0: {
						goto _ctr27;
					}
					case 32: {
						goto _ctr28;
					}
					case 42: {
						goto _ctr29;
					}
					case 43: {
						goto _ctr30;
					}
					case 63: {
						goto _ctr31;
					}
					case 123: {
						goto _ctr32;
					}
					case 124: {
						goto _st36;
					}
				}
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr112;
				}
				{
					goto _st0;
				}
				_ctr81:
				{
#line 232 "args/argcheck2.rl"
					rl_cmp = CMP_GREATER; }
				
#line 2640 "args/argcheck2.cpp"
				
				goto _st39;
				_st39:
				p += 1;
				st_case_39:
				switch( ( (*( p))) ) {
					case 43: {
						goto _ctr86;
					}
					case 45: {
						goto _ctr87;
					}
					case 61: {
						goto _ctr114;
					}
				}
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr88;
				}
				{
					goto _st0;
				}
				_st40:
				p += 1;
				st_case_40:
				switch( ( (*( p))) ) {
					case 43: {
						goto _ctr115;
					}
					case 45: {
						goto _ctr116;
					}
				}
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr117;
				}
				{
					goto _st0;
				}
				_ctr115:
				{
#line 209 "args/argcheck2.rl"
					rl_sign = 1; }
				
#line 2685 "args/argcheck2.cpp"
				
				{
#line 200 "args/argcheck2.rl"
					rl_sign = 1; }
				
#line 2691 "args/argcheck2.cpp"
				
				goto _st41;
				_ctr116:
				{
#line 209 "args/argcheck2.rl"
					rl_sign = 1; }
				
#line 2699 "args/argcheck2.cpp"
				
				{
#line 201 "args/argcheck2.rl"
					rl_sign = -1; }
				
#line 2705 "args/argcheck2.cpp"
				
				goto _st41;
				_st41:
				p += 1;
				st_case_41:
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr119;
				}
				{
					goto _st0;
				}
				_ctr117:
				{
#line 209 "args/argcheck2.rl"
					rl_sign = 1; }
				
#line 2722 "args/argcheck2.cpp"
				
				{
#line 203 "args/argcheck2.rl"
					rl_num = 0; }
				
#line 2728 "args/argcheck2.cpp"
				
				{
#line 203 "args/argcheck2.rl"
					(rl_num *= 10) += ((( (*( p)))) - '0'); }
				
#line 2734 "args/argcheck2.cpp"
				
				{
#line 169 "args/argcheck2.rl"
					
					rl_check.values.push_back((int64_t)(rl_sign * rl_num));
				}
				
#line 2742 "args/argcheck2.cpp"
				
				goto _st42;
				_ctr119:
				{
#line 203 "args/argcheck2.rl"
					rl_num = 0; }
				
#line 2750 "args/argcheck2.cpp"
				
				{
#line 203 "args/argcheck2.rl"
					(rl_num *= 10) += ((( (*( p)))) - '0'); }
				
#line 2756 "args/argcheck2.cpp"
				
				{
#line 169 "args/argcheck2.rl"
					
					rl_check.values.push_back((int64_t)(rl_sign * rl_num));
				}
				
#line 2764 "args/argcheck2.cpp"
				
				goto _st42;
				_ctr122:
				{
#line 203 "args/argcheck2.rl"
					(rl_num *= 10) += ((( (*( p)))) - '0'); }
				
#line 2772 "args/argcheck2.cpp"
				
				{
#line 169 "args/argcheck2.rl"
					
					rl_check.values.push_back((int64_t)(rl_sign * rl_num));
				}
				
#line 2780 "args/argcheck2.cpp"
				
				goto _st42;
				_st42:
				p += 1;
				st_case_42:
				if ( ( (*( p))) == 44 ) {
					goto _st43;
				}
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr122;
				}
				{
					goto _st0;
				}
				_st43:
				p += 1;
				st_case_43:
				switch( ( (*( p))) ) {
					case 43: {
						goto _ctr123;
					}
					case 45: {
						goto _ctr124;
					}
				}
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr125;
				}
				{
					goto _st0;
				}
				_ctr123:
				{
#line 209 "args/argcheck2.rl"
					rl_sign = 1; }
				
#line 2817 "args/argcheck2.cpp"
				
				{
#line 200 "args/argcheck2.rl"
					rl_sign = 1; }
				
#line 2823 "args/argcheck2.cpp"
				
				goto _st44;
				_ctr124:
				{
#line 209 "args/argcheck2.rl"
					rl_sign = 1; }
				
#line 2831 "args/argcheck2.cpp"
				
				{
#line 201 "args/argcheck2.rl"
					rl_sign = -1; }
				
#line 2837 "args/argcheck2.cpp"
				
				goto _st44;
				_st44:
				p += 1;
				st_case_44:
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr127;
				}
				{
					goto _st0;
				}
				_ctr125:
				{
#line 209 "args/argcheck2.rl"
					rl_sign = 1; }
				
#line 2854 "args/argcheck2.cpp"
				
				{
#line 203 "args/argcheck2.rl"
					rl_num = 0; }
				
#line 2860 "args/argcheck2.cpp"
				
				{
#line 203 "args/argcheck2.rl"
					(rl_num *= 10) += ((( (*( p)))) - '0'); }
				
#line 2866 "args/argcheck2.cpp"
				
				{
#line 169 "args/argcheck2.rl"
					
					rl_check.values.push_back((int64_t)(rl_sign * rl_num));
				}
				
#line 2874 "args/argcheck2.cpp"
				
				goto _st45;
				_ctr127:
				{
#line 203 "args/argcheck2.rl"
					rl_num = 0; }
				
#line 2882 "args/argcheck2.cpp"
				
				{
#line 203 "args/argcheck2.rl"
					(rl_num *= 10) += ((( (*( p)))) - '0'); }
				
#line 2888 "args/argcheck2.cpp"
				
				{
#line 169 "args/argcheck2.rl"
					
					rl_check.values.push_back((int64_t)(rl_sign * rl_num));
				}
				
#line 2896 "args/argcheck2.cpp"
				
				goto _st45;
				_ctr130:
				{
#line 203 "args/argcheck2.rl"
					(rl_num *= 10) += ((( (*( p)))) - '0'); }
				
#line 2904 "args/argcheck2.cpp"
				
				{
#line 169 "args/argcheck2.rl"
					
					rl_check.values.push_back((int64_t)(rl_sign * rl_num));
				}
				
#line 2912 "args/argcheck2.cpp"
				
				goto _st45;
				_st45:
				p += 1;
				st_case_45:
				switch( ( (*( p))) ) {
					case 41: {
						goto _ctr129;
					}
					case 93: {
						goto _ctr131;
					}
				}
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr130;
				}
				{
					goto _st0;
				}
				_st46:
				p += 1;
				st_case_46:
				if ( ( (*( p))) == 50 ) {
					goto _ctr132;
				}
				{
					goto _st0;
				}
				_ctr8:
				{
#line 272 "args/argcheck2.rl"
					rl_type = CHECK_SYMBOL; }
				
#line 2946 "args/argcheck2.cpp"
				
				goto _st47;
				_st47:
				p += 1;
				st_case_47:
				switch( ( (*( p))) ) {
					case 0: {
						goto _ctr27;
					}
					case 32: {
						goto _ctr28;
					}
					case 42: {
						goto _ctr29;
					}
					case 43: {
						goto _ctr30;
					}
					case 61: {
						goto _ctr134;
					}
					case 63: {
						goto _ctr31;
					}
					case 123: {
						goto _ctr32;
					}
				}
				{
					goto _st0;
				}
				_ctr134:
				{
#line 262 "args/argcheck2.rl"
					rl_cmp = CMP_EQUAL; }
				
#line 2983 "args/argcheck2.cpp"
				
				goto _st48;
				_ctr143:
				{
#line 255 "args/argcheck2.rl"
					rl_sym_start = p; }
				
#line 2991 "args/argcheck2.cpp"
				
				{
#line 181 "args/argcheck2.rl"
					
					try {
						ArgString str{ {}, 0 };
						str.first.assign(rl_sym_start, p - rl_sym_start);
						str.second = crc32_hash(str.first.data());
						rl_check.values.push_back(str);
					} catch(std::exception& e) {
						LIB_ERR << "exception: " << e.what();
					}
				}
				
#line 3006 "args/argcheck2.cpp"
				
				goto _st48;
				_ctr147:
				{
#line 181 "args/argcheck2.rl"
					
					try {
						ArgString str{ {}, 0 };
						str.first.assign(rl_sym_start, p - rl_sym_start);
						str.second = crc32_hash(str.first.data());
						rl_check.values.push_back(str);
					} catch(std::exception& e) {
						LIB_ERR << "exception: " << e.what();
					}
				}
				
#line 3023 "args/argcheck2.cpp"
				
				goto _st48;
				_st48:
				p += 1;
				st_case_48:
				switch( ( (*( p))) ) {
					case 0: {
						goto _ctr137;
					}
					case 32: {
						goto _ctr138;
					}
					case 42: {
						goto _ctr139;
					}
					case 43: {
						goto _ctr140;
					}
					case 63: {
						goto _ctr141;
					}
					case 123: {
						goto _ctr142;
					}
					case 124: {
						goto _ctr143;
					}
				}
				{
					goto _ctr136;
				}
				_ctr153:
				{
#line 197 "args/argcheck2.rl"
					rl_max = rl_min; }
				
#line 3060 "args/argcheck2.cpp"
				
				goto _st49;
				_ctr154:
				{
#line 196 "args/argcheck2.rl"
					rl_max = 0; }
				
#line 3068 "args/argcheck2.cpp"
				
				goto _st49;
				_ctr136:
				{
#line 255 "args/argcheck2.rl"
					rl_sym_start = p; }
				
#line 3076 "args/argcheck2.cpp"
				
				goto _st49;
				_ctr139:
				{
#line 255 "args/argcheck2.rl"
					rl_sym_start = p; }
				
#line 3084 "args/argcheck2.cpp"
				
				{
#line 181 "args/argcheck2.rl"
					
					try {
						ArgString str{ {}, 0 };
						str.first.assign(rl_sym_start, p - rl_sym_start);
						str.second = crc32_hash(str.first.data());
						rl_check.values.push_back(str);
					} catch(std::exception& e) {
						LIB_ERR << "exception: " << e.what();
					}
				}
				
#line 3099 "args/argcheck2.cpp"
				
				{
#line 289 "args/argcheck2.rl"
					rl_min = 1; rl_max = REPEAT_INF; }
				
#line 3105 "args/argcheck2.cpp"
				
				{
#line 276 "args/argcheck2.rl"
					rl_min = 0; rl_max = REPEAT_INF; }
				
#line 3111 "args/argcheck2.cpp"
				
				goto _st49;
				_ctr140:
				{
#line 255 "args/argcheck2.rl"
					rl_sym_start = p; }
				
#line 3119 "args/argcheck2.cpp"
				
				{
#line 181 "args/argcheck2.rl"
					
					try {
						ArgString str{ {}, 0 };
						str.first.assign(rl_sym_start, p - rl_sym_start);
						str.second = crc32_hash(str.first.data());
						rl_check.values.push_back(str);
					} catch(std::exception& e) {
						LIB_ERR << "exception: " << e.what();
					}
				}
				
#line 3134 "args/argcheck2.cpp"
				
				{
#line 289 "args/argcheck2.rl"
					rl_min = 1; rl_max = REPEAT_INF; }
				
#line 3140 "args/argcheck2.cpp"
				
				{
#line 275 "args/argcheck2.rl"
					rl_min = 1, rl_max = REPEAT_INF; }
				
#line 3146 "args/argcheck2.cpp"
				
				goto _st49;
				_ctr141:
				{
#line 255 "args/argcheck2.rl"
					rl_sym_start = p; }
				
#line 3154 "args/argcheck2.cpp"
				
				{
#line 181 "args/argcheck2.rl"
					
					try {
						ArgString str{ {}, 0 };
						str.first.assign(rl_sym_start, p - rl_sym_start);
						str.second = crc32_hash(str.first.data());
						rl_check.values.push_back(str);
					} catch(std::exception& e) {
						LIB_ERR << "exception: " << e.what();
					}
				}
				
#line 3169 "args/argcheck2.cpp"
				
				{
#line 289 "args/argcheck2.rl"
					rl_min = 1; rl_max = REPEAT_INF; }
				
#line 3175 "args/argcheck2.cpp"
				
				{
#line 274 "args/argcheck2.rl"
					rl_min = 0; rl_max = 1; }
				
#line 3181 "args/argcheck2.cpp"
				
				goto _st49;
				_st49:
				p += 1;
				st_case_49:
				switch( ( (*( p))) ) {
					case 0: {
						goto _ctr145;
					}
					case 32: {
						goto _ctr146;
					}
					case 124: {
						goto _ctr147;
					}
				}
				{
					goto _st49;
				}
				_ctr142:
				{
#line 255 "args/argcheck2.rl"
					rl_sym_start = p; }
				
#line 3206 "args/argcheck2.cpp"
				
				{
#line 181 "args/argcheck2.rl"
					
					try {
						ArgString str{ {}, 0 };
						str.first.assign(rl_sym_start, p - rl_sym_start);
						str.second = crc32_hash(str.first.data());
						rl_check.values.push_back(str);
					} catch(std::exception& e) {
						LIB_ERR << "exception: " << e.what();
					}
				}
				
#line 3221 "args/argcheck2.cpp"
				
				{
#line 289 "args/argcheck2.rl"
					rl_min = 1; rl_max = REPEAT_INF; }
				
#line 3227 "args/argcheck2.cpp"
				
				goto _st50;
				_st50:
				p += 1;
				st_case_50:
				switch( ( (*( p))) ) {
					case 0: {
						goto _ctr145;
					}
					case 32: {
						goto _ctr146;
					}
					case 48: {
						goto _ctr149;
					}
					case 124: {
						goto _ctr147;
					}
				}
				if ( 49 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr150;
				}
				{
					goto _st49;
				}
				_ctr149:
				{
#line 195 "args/argcheck2.rl"
					rl_min = 0; }
				
#line 3258 "args/argcheck2.cpp"
				
				goto _st51;
				_st51:
				p += 1;
				st_case_51:
				switch( ( (*( p))) ) {
					case 0: {
						goto _ctr145;
					}
					case 32: {
						goto _ctr146;
					}
					case 44: {
						goto _st52;
					}
					case 124: {
						goto _ctr147;
					}
					case 125: {
						goto _ctr153;
					}
				}
				{
					goto _st49;
				}
				_st52:
				p += 1;
				st_case_52:
				switch( ( (*( p))) ) {
					case 0: {
						goto _ctr145;
					}
					case 32: {
						goto _ctr146;
					}
					case 48: {
						goto _ctr154;
					}
					case 124: {
						goto _ctr147;
					}
				}
				if ( 49 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr155;
				}
				{
					goto _st49;
				}
				_ctr155:
				{
#line 196 "args/argcheck2.rl"
					rl_max = (( (*( p))))-'0'; }
				
#line 3312 "args/argcheck2.cpp"
				
				goto _st53;
				_ctr157:
				{
#line 196 "args/argcheck2.rl"
					(rl_max *= 10) += ((( (*( p)))) - '0'); }
				
#line 3320 "args/argcheck2.cpp"
				
				goto _st53;
				_st53:
				p += 1;
				st_case_53:
				switch( ( (*( p))) ) {
					case 0: {
						goto _ctr145;
					}
					case 32: {
						goto _ctr146;
					}
					case 124: {
						goto _ctr147;
					}
				}
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr157;
				}
				{
					goto _st49;
				}
				_ctr150:
				{
#line 195 "args/argcheck2.rl"
					rl_min = (( (*( p))))-'0'; }
				
#line 3348 "args/argcheck2.cpp"
				
				goto _st54;
				_ctr159:
				{
#line 195 "args/argcheck2.rl"
					(rl_min *= 10) += ((( (*( p)))) - '0'); }
				
#line 3356 "args/argcheck2.cpp"
				
				goto _st54;
				_st54:
				p += 1;
				st_case_54:
				switch( ( (*( p))) ) {
					case 0: {
						goto _ctr145;
					}
					case 32: {
						goto _ctr146;
					}
					case 44: {
						goto _st52;
					}
					case 124: {
						goto _ctr147;
					}
					case 125: {
						goto _ctr153;
					}
				}
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr159;
				}
				{
					goto _st49;
				}
				_ctr3:
				{
#line 280 "args/argcheck2.rl"
					rl_check.name.clear(); }
				
#line 3390 "args/argcheck2.cpp"
				
				{
#line 280 "args/argcheck2.rl"
					rl_check.name.push_back((( (*( p))))); }
				
#line 3396 "args/argcheck2.cpp"
				
				{
#line 269 "args/argcheck2.rl"
					rl_type = CHECK_BOOL; }
				
#line 3402 "args/argcheck2.cpp"
				
				goto _st55;
				_st55:
				p += 1;
				st_case_55:
				switch( ( (*( p))) ) {
					case 0: {
						goto _ctr27;
					}
					case 32: {
						goto _ctr28;
					}
					case 42: {
						goto _ctr29;
					}
					case 43: {
						goto _ctr30;
					}
					case 58: {
						goto _st10;
					}
					case 63: {
						goto _ctr161;
					}
					case 95: {
						goto _ctr10;
					}
					case 123: {
						goto _ctr32;
					}
				}
				if ( ( (*( p))) > 57 ) {
					if ( 64 <= ( (*( p))) && ( (*( p))) <= 90 ) {
						goto _ctr10;
					}
				} else if ( ( (*( p))) >= 48 ) {
					goto _ctr10;
				}
				{
					goto _st0;
				}
				_ctr161:
				{
#line 280 "args/argcheck2.rl"
					rl_check.name.push_back((( (*( p))))); }
				
#line 3449 "args/argcheck2.cpp"
				
				{
#line 289 "args/argcheck2.rl"
					rl_min = 1; rl_max = REPEAT_INF; }
				
#line 3455 "args/argcheck2.cpp"
				
				{
#line 274 "args/argcheck2.rl"
					rl_min = 0; rl_max = 1; }
				
#line 3461 "args/argcheck2.cpp"
				
				goto _st56;
				_st56:
				p += 1;
				st_case_56:
				switch( ( (*( p))) ) {
					case 0: {
						goto _ctr34;
					}
					case 32: {
						goto _ctr35;
					}
					case 58: {
						goto _st10;
					}
					case 95: {
						goto _ctr13;
					}
				}
				if ( ( (*( p))) > 57 ) {
					if ( 63 <= ( (*( p))) && ( (*( p))) <= 90 ) {
						goto _ctr13;
					}
				} else if ( ( (*( p))) >= 48 ) {
					goto _ctr13;
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
				_test_eof57: cs = 57; goto _test_eof; 
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
				
				_test_eof: {}
				if ( cs >= 57 )
					goto _out; _pop: {}
				_out: {}
			}
			
#line 595 "args/argcheck2.rl"
			
			
			if (cs < 
#line 3560 "args/argcheck2.cpp"
			57
#line 597 "args/argcheck2.rl"
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
