#line 1 "args/argcheck2.rl"
# include "argcheck2.h"
# include "ceammc_string.h"
# include "ceammc_object.h"
# include "ceammc_crc32.h"
# include "fmt/core.h"

# include <cstdint>
# include <iostream>
# include <limits>
# include <cmath>
# include <algorithm>
# include <boost/static_string.hpp>
# include <boost/container/static_vector.hpp>
# include <boost/variant.hpp>

// #define ARG_DEBUG

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
		CMP_APPROX,
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
	
	class ArgStringVisitor : public boost::static_visitor<std::string>
	{
		public:
		std::string operator()(const double& d) const { return fmt::format("{}", d); }
		std::string operator()(const int64_t& i) const { return fmt::format("{}", i); }
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
		return boost::apply_visitor(ArgStringVisitor(), v);
	}
	
	inline std::string arg_to_string(const ArgList& v, const char* delim = ", ")
	{
		ceammc::string::SmallString res;
		ArgStringVisitor visitor;
		for (auto& a: v) {
			if (&a != &v[0]) {
				res.insert(res.end(), delim, delim + strlen(delim));
			}
			fmt::format_to(std::back_inserter(res), boost::apply_visitor(visitor, a));
		}
		
		return { res.data(), res.size() };
	}
	
	inline bool approx_equal(double a, double b, double e = 0.00001)
	{
		return std::fabs(a - b) <= e;
	}
	
	struct Check {
		ArgList values;
		ArgName name;
		CheckType type;
		CompareType cmp;
		int8_t rmin;
		int8_t rmax;
		
		inline int repeatMin() const { return rmin; }
		inline int repeatMax() const { return (rmax == REPEAT_INF) ? std::numeric_limits<int>::max() : rmax; }
		inline void setRepeats(int min, int max) { rmin = min; rmax = max; }
		
		inline static bool isEqual(const ArgValue& v, int64_t i)
		{
			auto int_ptr = boost::get<int64_t>(&v);
			return (int_ptr && i == *int_ptr);
		}
		
		inline static bool isEqual(const ArgValue& v, double d)
		{
			auto dbl_ptr = boost::get<double>(&v);
			return (dbl_ptr && d == *dbl_ptr);
		}
		
		inline static bool isApproxEqual(const ArgValue& v, double d)
		{
			auto dbl_ptr = boost::get<double>(&v);
			return dbl_ptr && approx_equal(d, *dbl_ptr);
		}
		
		inline static bool isEqualHash(const ArgValue& v, uint32_t hash)
		{
			auto str_ptr = boost::get<ArgString>(&v);
			return (str_ptr && str_ptr->second == hash);
		}
		
		inline std::string argName() const {
			if (name.empty())
				return typeNames[type];
			else
				return { name.data(), name.size() };
		}
		
		inline std::string checkInfo() const {
			switch (cmp) {
				case CMP_MODULE:
				return fmt::format("check: %{}==0", arg_to_string(values));
				case CMP_LESS:
				return fmt::format("check: <{}", arg_to_string(values));
				case CMP_LESS_EQ:
				return fmt::format("check: <={}", arg_to_string(values));
				case CMP_GREATER:
				return fmt::format("check: >{}", arg_to_string(values));
				case CMP_GREATER_EQ:
				return fmt::format("check: >={}", arg_to_string(values));
				case CMP_RANGE_CLOSED:
				return fmt::format("range: [{}]", arg_to_string(values, ","));
				case CMP_RANGE_SEMIOPEN:
				return fmt::format("range: [{})", arg_to_string(values, ","));
				case CMP_EQUAL:
				if (values.size() == 1)
					return fmt::format("check: ={}", arg_to_string(values));
				else
					return fmt::format("enum: {}", arg_to_string(values, "|"));
				case CMP_APPROX:
				if (values.size() == 1)
					return fmt::format("check: ~{}", arg_to_string(values));
				else
					return fmt::format("enum: ~{}", arg_to_string(values, "|"));
				default:
				return {};
			}
		}
		
		inline std::string argInfo() const {
			if (name.empty())
				return fmt::format("{:10s} [{}]{}", typeNames[type], checkInfo(), helpRepeats());
			else
				return fmt::format("{:10s} [type: {} {}]{}", name.data(), typeNames[type], checkInfo(), helpRepeats());
		}
		
		inline std::string helpRepeats() const {
			if (rmin == 1 && rmax == 1) return {};
			if (rmin == 0 && rmax == 1) return { '?', 1 };
			if (rmin == 0 && rmax == REPEAT_INF) return { '*', 1 };
			if (rmin == 1 && rmax == REPEAT_INF) return { '+', 1 };
			if (rmin == rmax)
				return fmt::format("{{{}}}", (int)rmin);
			if (rmax != REPEAT_INF)
				return fmt::format("{{{},{}}}", (int)rmin, (int)rmax);
			else
				return fmt::format("{{{},}}", (int)rmin);
		}
	};
}


#line 221 "args/argcheck2.cpp"
static const int arg_check2_start = 1;
static const int arg_check2_first_final = 69;
static const int arg_check2_error = 0;

static const int arg_check2_en_main = 1;


#line 389 "args/argcheck2.rl"


namespace ceammc {
	namespace args {
		
		namespace {
			
			bool checkAtom(const Check& c, const Atom& a, int i, const void* x, bool pErr) {
				switch (c.type) {
					case CHECK_ATOM:
					debug("atom", "Ok");
					break;
					case CHECK_BOOL:
					if (a.isBool()) {
						debug("book", "Ok");
						i++;
					} else {
						if (pErr)
							pdError(x, fmt::format("invalid {} value at [{}]: '{}'", c.argName(), i, atom_to_string(a)));
						return false;
					}
					break;
					case CHECK_BYTE:
					if (!a.isInteger() || a.asT<int>() < 0 || a.asT<int>() > 255) {
						if (pErr)
							pdError(x, fmt::format("invalid {} value at [{}]: '{}'", c.argName(), i, atom_to_string(a)));
						return false;
					} else {
						debug("byte", "Ok");
					}
					break;
					case CHECK_INT:
					if (!a.isInteger()) {
						if (pErr)
							pdError(x, fmt::format("invalid {} value at [{}]: '{}'", c.argName(), i, atom_to_string(a)));
						return false;
					} else {
						const int64_t val = a.asT<int>();
						const int64_t arg = (c.values.size() == 1 && boost::get<int64_t>(&c.values[0]))
						? *boost::strict_get<int64_t>(&c.values[0])
						: -999999999;
						
						switch (c.cmp) {
							case CMP_LESS:
							if (!(val < arg)) {
								if (pErr)
									pdError(x, fmt::format("{} at [{}] expected to be <{}, got: {}", c.argName(), i, arg, val));
								return false;
							}
							break;
							case CMP_LESS_EQ:
							if (!(val <= arg)) {
								if (pErr)
									pdError(x, fmt::format("{} at [{}] expected to be <={}, got: {}", c.argName(), i, arg, val));
								return false;
							}
							break;
							case CMP_GREATER:
							if (!(val > arg)) {
								if (pErr)
									pdError(x, fmt::format("{} at [{}] expected to be >{}, got: {}", c.argName(), i, arg, val));
								return false;
							}
							break;
							case CMP_GREATER_EQ:
							if (!(val >= arg)) {
								if (pErr)
									pdError(x, fmt::format("{} at [{}] expected to be >={}, got: {}", c.argName(), i, arg, val));
								return false;
							}
							break;
							case CMP_EQUAL:
							if (c.values.size() == 1) {
								if (val != arg) {
									if (pErr)
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
									if (pErr)
										pdError(x, fmt::format("{} at [{}] expected to be one of: {}, got: {}",
									c.argName(), i, arg_to_string(c.values), atom_to_string(a)));
									return false;
								}
							}
							break;
							case CMP_NOT_EQUAL:
							if (val == arg) {
								if (pErr)
									pdError(x, fmt::format("{} at [{}] expected to be !={}, got: {}", c.argName(), i, arg, val));
								return false;
							}
							break;
							case CMP_MODULE:
							fmt::print("val={}, arg={}\n", val, arg);
							if (val % arg != 0) {
								if (pErr)
									pdError(x, fmt::format("{} at [{}] expected to be multiple of {}, got: {}", c.argName(), i, arg, val));
								return false;
							}
							break;
							case CMP_POWER2: {
								bool rc = (val > 0 && ((val & (val - 1)) == 0));
								if (!rc) {
									if (pErr)
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
								
								if (c.cmp == CMP_RANGE_CLOSED && !(a <= val && val <= b)) {
									if (pErr)
										pdError(x, fmt::format("{} value at [{}] expected to be in [{},{}] range, got: {}", c.argName(), i, a, b, val));
									return false;
								}
								
								if (c.cmp == CMP_RANGE_SEMIOPEN && !(a <= val && val < b)) {
									if (pErr)
										pdError(x, fmt::format("{} at [{}] expected to be in [{},{}) range, got: {}", c.argName(), i, a, b, val));
									return false;
								}
							}
							break;
							default:
							break;
						}
						debug("int", "Ok");
					}
					break;
					case CHECK_SYMBOL: {
						if (!a.isSymbol()) {
							if (pErr)
								pdError(x, fmt::format("invalid {} value at [{}]: '{}'", c.argName(), i, atom_to_string(a)));
							return false;
						}
						
						auto val = a.asT<t_symbol*>()->s_name;
						auto hash = crc32_hash(val);
						
						switch(c.cmp) {
							case CMP_EQUAL: {
								bool found = false;
								for (auto& v: c.values) {
									if (c.isEqualHash(v, hash)) { found = true; break; }
								}
								if (!found) {
									if (pErr) {
										if (c.values.size() == 1)
											pdError(x, fmt::format("{} at [{}] expected to be {}, got: '{}'",
										c.argName(), i, arg_to_string(c.values[0]), atom_to_string(a)));
										else
											pdError(x, fmt::format("{} at [{}] expected to be one of: {}, got: '{}'",
										c.argName(), i, arg_to_string(c.values), atom_to_string(a)));
									}
									return false;
								}
							}
							break;
							default:
							break;
						}
						debug("symbol", "Ok");
					}
					break;
					case CHECK_FLOAT: {
						if (!a.isFloat()) {
							if (pErr)
								pdError(x, fmt::format("invalid {} value at [{}]: '{}'", c.argName(), i, atom_to_string(a)));
							return false;
						}
						
						auto val = a.asT<t_float>();
						const t_float arg = (c.values.size() == 1 && boost::get<double>(&c.values[0]))
						? *boost::strict_get<double>(&c.values[0])
						: -999999999;
						
						switch (c.cmp) {
							case CMP_LESS:
							if (!(val < arg)) {
								if (pErr)
									pdError(x, fmt::format("{} at [{}] expected to be <{}, got: {}", c.argName(), i, arg, val));
								return false;
							}
							break;
							case CMP_LESS_EQ:
							if (!(val <= arg)) {
								if (pErr)
									pdError(x, fmt::format("{} at [{}] expected to be <={}, got: {}", c.argName(), i, arg, val));
								return false;
							}
							break;
							case CMP_GREATER:
							if (!(val > arg)) {
								if (pErr)
									pdError(x, fmt::format("{} at [{}] expected to be >{}, got: {}", c.argName(), i, arg, val));
								return false;
							}
							break;
							case CMP_GREATER_EQ:
							if (!(val >= arg)) {
								if (pErr)
									pdError(x, fmt::format("{} at [{}] expected to be >={}, got: {}", c.argName(), i, arg, val));
								return false;
							}
							break;
							case CMP_RANGE_SEMIOPEN:
							case CMP_RANGE_CLOSED: {
								if (c.values.size() != 2) {
									pdError(x, fmt::format("internal arg error, invalid arg count: {}", c.values.size()));
									return false;
								}
								
								if (!boost::get<double>(&c.values[0]) || !boost::get<double>(&c.values[1])) {
									pdError(x, "internal arg error");
									return false;
								}
								
								const auto a = *boost::get<double>(&c.values[0]);
								const auto b = *boost::get<double>(&c.values[1]);
								
								if (c.cmp == CMP_RANGE_CLOSED && !(a <= val && val <= b)) {
									if (pErr)
										pdError(x, fmt::format("{} value at [{}] expected to be in [{},{}] range, got: {}", c.argName(), i, a, b, val));
									return false;
								}
								
								if (c.cmp == CMP_RANGE_SEMIOPEN && !(a <= val && val < b)) {
									if (pErr)
										pdError(x, fmt::format("{} at [{}] expected to be in [{},{}) range, got: {}", c.argName(), i, a, b, val));
									return false;
								}
							}
							break;
							case CMP_EQUAL:
							if (c.values.size() == 1) {
								if (val != arg) {
									if (pErr)
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
									if (pErr)
										pdError(x, fmt::format("{} at [{}] expected to be one of: {}, got: {}",
									c.argName(), i, arg_to_string(c.values), atom_to_string(a)));
									return false;
								}
							}
							break;
							case CMP_NOT_EQUAL:
							if (val == arg) {
								if (pErr)
									pdError(x, fmt::format("{} at [{}] expected to be !={}, got: {}", c.argName(), i, arg, val));
								return false;
							}
							break;
							case CMP_APPROX:
							if (c.values.size() == 1) {
								if (!approx_equal(val, arg)) {
									if (pErr)
										pdError(x, fmt::format("{} at [{}] expected to be ~ {}, got: {}",
									c.argName(), i, arg_to_string(c.values[0]), atom_to_string(a)));
									return false;
								}
							} else {
								bool found = false;
								for (auto& v: c.values) {
									if (c.isApproxEqual(v, val)) { found = true; break; }
								}
								if (!found) {
									if (pErr)
										pdError(x, fmt::format("{} at [{}] expected to be aprrox of: {}, got: {}",
									c.argName(), i, arg_to_string(c.values), atom_to_string(a)));
									return false;
								}
							}
							break;
							default:
							break;
						}
						debug("float", "Ok");
					} break;
					default:
					break;
				}
				
				return true;
			}
		}
		
		struct ArgCheckImp : boost::container::small_vector<Check, 4> {
			public:
			std::string helpBrief() const {
				string::MediumString str;
				auto bs = std::back_inserter(str);
				fmt::format_to(bs, "usage: ");
				for (auto& c: *this) {
					fmt::format_to(bs, "{}{} ", c.argName(), c.helpRepeats());
				}
				return { str.data(), str.size() };
			}
			
			std::string help() const {
				string::MediumString str;
				auto bs = std::back_inserter(str);
				fmt::format_to(bs, "usage: ");
				for (auto& c: *this)
				fmt::format_to(bs, "{}{} ", c.argName(), c.helpRepeats());
				
				str.push_back('\n');
				for (auto& c: *this) {
					fmt::format_to(bs, " - {}\n", c.argInfo());
				}
				
				if (str.size() > 0 && str.back() == '\n')
					str.pop_back();
				
				return { str.data(), str.size() };
			}
		};
		
		ArgChecker::~ArgChecker()  = default;
		
		bool ArgChecker::check(const AtomListView& lv, BaseObject* obj) const
		{
			if (!chk_)
				return false;
			
			const void* x = obj ? obj->owner() : nullptr;
			
			// pdDebug(x, chk_->help());
			
			const int N = lv.size();
			int atom_idx = 0;
			
			for (int i = 0; i < chk_->size(); i++) {
				auto& check = (*chk_)[i];
				auto cur = atom_idx + (check.rmin - 1);
				
				for (int k = 0; k < check.rmin; k++, atom_idx++) {
					if (atom_idx >= N) {
						pdError(x, fmt::format("{} expected at [{}]", check.argName(), atom_idx));
						return false;
					}
					if (!checkAtom(check, lv[atom_idx], atom_idx, x, true))
						return false;
				}
				
				for (int k = check.rmin; k < check.repeatMax(); k++, atom_idx++) {
					if (atom_idx >= N)
						break;
					
					if (!checkAtom(check, lv[atom_idx], atom_idx, x, false))
						break;
				}
			}
			
			if (atom_idx < N) {
				pdError(x, fmt::format("extra arguments left, starting from [{}]: {}", atom_idx, list_to_string(lv.subView(atom_idx))));
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
			int64_t rl_num = 0;
			int64_t rl_den = 0;
			int64_t rl_den_cnt = 0;
			int rl_sign = 0;
			Check rl_chk;
			const char* rl_sym_start = 0;
			
			
#line 635 "args/argcheck2.cpp"
			{
				cs = (int)arg_check2_start;
			}
			
#line 640 "args/argcheck2.cpp"
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
					case 69:
					goto st_case_69;
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
				}
				goto st_out;
				_ctr28:
				{
#line 380 "args/argcheck2.rl"
					rl_chk.rmin = 1; rl_chk.rmax = 1; }
				
#line 790 "args/argcheck2.cpp"
				
				{
#line 221 "args/argcheck2.rl"
					
					if (chk_)
					chk_->push_back(rl_chk);
					
					rl_num = 0;
					rl_den = 0;
					rl_sign = 0;
					rl_den_cnt = 0;
					rl_chk = {};
				}
				
#line 805 "args/argcheck2.cpp"
				
				goto _st1;
				_ctr35:
				{
#line 221 "args/argcheck2.rl"
					
					if (chk_)
					chk_->push_back(rl_chk);
					
					rl_num = 0;
					rl_den = 0;
					rl_sign = 0;
					rl_den_cnt = 0;
					rl_chk = {};
				}
				
#line 822 "args/argcheck2.cpp"
				
				goto _st1;
				_ctr65:
				{
#line 236 "args/argcheck2.rl"
					
					double real = rl_num;
					if (rl_den_cnt)
					real += rl_den / double(rl_den_cnt);
					
					rl_chk.values.push_back(rl_sign * real);
				}
				
#line 836 "args/argcheck2.cpp"
				
				{
#line 380 "args/argcheck2.rl"
					rl_chk.rmin = 1; rl_chk.rmax = 1; }
				
#line 842 "args/argcheck2.cpp"
				
				{
#line 221 "args/argcheck2.rl"
					
					if (chk_)
					chk_->push_back(rl_chk);
					
					rl_num = 0;
					rl_den = 0;
					rl_sign = 0;
					rl_den_cnt = 0;
					rl_chk = {};
				}
				
#line 857 "args/argcheck2.cpp"
				
				goto _st1;
				_ctr135:
				{
#line 232 "args/argcheck2.rl"
					
					rl_chk.values.push_back((int64_t)(rl_sign * rl_num));
				}
				
#line 867 "args/argcheck2.cpp"
				
				{
#line 380 "args/argcheck2.rl"
					rl_chk.rmin = 1; rl_chk.rmax = 1; }
				
#line 873 "args/argcheck2.cpp"
				
				{
#line 221 "args/argcheck2.rl"
					
					if (chk_)
					chk_->push_back(rl_chk);
					
					rl_num = 0;
					rl_den = 0;
					rl_sign = 0;
					rl_den_cnt = 0;
					rl_chk = {};
				}
				
#line 888 "args/argcheck2.cpp"
				
				goto _st1;
				_ctr178:
				{
#line 284 "args/argcheck2.rl"
					rl_chk.cmp = CMP_RANGE_SEMIOPEN; }
				
#line 896 "args/argcheck2.cpp"
				
				{
#line 380 "args/argcheck2.rl"
					rl_chk.rmin = 1; rl_chk.rmax = 1; }
				
#line 902 "args/argcheck2.cpp"
				
				{
#line 221 "args/argcheck2.rl"
					
					if (chk_)
					chk_->push_back(rl_chk);
					
					rl_num = 0;
					rl_den = 0;
					rl_sign = 0;
					rl_den_cnt = 0;
					rl_chk = {};
				}
				
#line 917 "args/argcheck2.cpp"
				
				goto _st1;
				_ctr185:
				{
#line 284 "args/argcheck2.rl"
					rl_chk.cmp = CMP_RANGE_CLOSED; }
				
#line 925 "args/argcheck2.cpp"
				
				{
#line 380 "args/argcheck2.rl"
					rl_chk.rmin = 1; rl_chk.rmax = 1; }
				
#line 931 "args/argcheck2.cpp"
				
				{
#line 221 "args/argcheck2.rl"
					
					if (chk_)
					chk_->push_back(rl_chk);
					
					rl_num = 0;
					rl_den = 0;
					rl_sign = 0;
					rl_den_cnt = 0;
					rl_chk = {};
				}
				
#line 946 "args/argcheck2.cpp"
				
				goto _st1;
				_ctr197:
				{
#line 244 "args/argcheck2.rl"
					
					try {
						ArgString str{ {}, 0 };
						str.first.assign(rl_sym_start, p - rl_sym_start);
						str.second = crc32_hash(str.first.data());
						rl_chk.values.push_back(str);
					} catch(std::exception& e) {
						LIB_ERR << "exception: " << e.what();
					}
				}
				
#line 963 "args/argcheck2.cpp"
				
				{
#line 380 "args/argcheck2.rl"
					rl_chk.rmin = 1; rl_chk.rmax = 1; }
				
#line 969 "args/argcheck2.cpp"
				
				{
#line 221 "args/argcheck2.rl"
					
					if (chk_)
					chk_->push_back(rl_chk);
					
					rl_num = 0;
					rl_den = 0;
					rl_sign = 0;
					rl_den_cnt = 0;
					rl_chk = {};
				}
				
#line 984 "args/argcheck2.cpp"
				
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
#line 371 "args/argcheck2.rl"
					rl_chk.name.clear(); }
				
#line 1032 "args/argcheck2.cpp"
				
				{
#line 371 "args/argcheck2.rl"
					rl_chk.name.push_back((( (*( p))))); }
				
#line 1038 "args/argcheck2.cpp"
				
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
#line 371 "args/argcheck2.rl"
					rl_chk.name.push_back((( (*( p))))); }
				
#line 1067 "args/argcheck2.cpp"
				
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
#line 371 "args/argcheck2.rl"
					rl_chk.name.push_back((( (*( p))))); }
				
#line 1096 "args/argcheck2.cpp"
				
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
#line 371 "args/argcheck2.rl"
					rl_chk.name.push_back((( (*( p))))); }
				
#line 1125 "args/argcheck2.cpp"
				
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
#line 371 "args/argcheck2.rl"
					rl_chk.name.push_back((( (*( p))))); }
				
#line 1154 "args/argcheck2.cpp"
				
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
#line 371 "args/argcheck2.rl"
					rl_chk.name.push_back((( (*( p))))); }
				
#line 1183 "args/argcheck2.cpp"
				
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
#line 371 "args/argcheck2.rl"
					rl_chk.name.push_back((( (*( p))))); }
				
#line 1212 "args/argcheck2.cpp"
				
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
#line 371 "args/argcheck2.rl"
					rl_chk.name.push_back((( (*( p))))); }
				
#line 1241 "args/argcheck2.cpp"
				
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
#line 359 "args/argcheck2.rl"
					rl_chk.type = CHECK_ATOM; }
				
#line 1284 "args/argcheck2.cpp"
				
				goto _st11;
				_ctr5:
				{
#line 361 "args/argcheck2.rl"
					rl_chk.type = CHECK_BYTE; }
				
#line 1292 "args/argcheck2.cpp"
				
				goto _st11;
				_ctr25:
				{
#line 360 "args/argcheck2.rl"
					rl_chk.type = CHECK_BOOL; }
				
#line 1300 "args/argcheck2.cpp"
				
				goto _st11;
				_ctr108:
				{
#line 236 "args/argcheck2.rl"
					
					double real = rl_num;
					if (rl_den_cnt)
					real += rl_den / double(rl_den_cnt);
					
					rl_chk.values.push_back(rl_sign * real);
				}
				
#line 1314 "args/argcheck2.cpp"
				
				{
#line 334 "args/argcheck2.rl"
					rl_chk.cmp = CMP_RANGE_SEMIOPEN; }
				
#line 1320 "args/argcheck2.cpp"
				
				goto _st11;
				_ctr111:
				{
#line 236 "args/argcheck2.rl"
					
					double real = rl_num;
					if (rl_den_cnt)
					real += rl_den / double(rl_den_cnt);
					
					rl_chk.values.push_back(rl_sign * real);
				}
				
#line 1334 "args/argcheck2.cpp"
				
				{
#line 334 "args/argcheck2.rl"
					rl_chk.cmp = CMP_RANGE_CLOSED; }
				
#line 1340 "args/argcheck2.cpp"
				
				goto _st11;
				_ctr190:
				{
#line 309 "args/argcheck2.rl"
					rl_chk.cmp = CMP_POWER2; }
				
#line 1348 "args/argcheck2.cpp"
				
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
#line 380 "args/argcheck2.rl"
					rl_chk.rmin = 1; rl_chk.rmax = 1; }
				
#line 1382 "args/argcheck2.cpp"
				
				{
#line 221 "args/argcheck2.rl"
					
					if (chk_)
					chk_->push_back(rl_chk);
					
					rl_num = 0;
					rl_den = 0;
					rl_sign = 0;
					rl_den_cnt = 0;
					rl_chk = {};
				}
				
#line 1397 "args/argcheck2.cpp"
				
				{
#line 385 "args/argcheck2.rl"
					{p+= 1; cs = 69; goto _out;} }
				
#line 1403 "args/argcheck2.cpp"
				
				goto _st69;
				_ctr34:
				{
#line 221 "args/argcheck2.rl"
					
					if (chk_)
					chk_->push_back(rl_chk);
					
					rl_num = 0;
					rl_den = 0;
					rl_sign = 0;
					rl_den_cnt = 0;
					rl_chk = {};
				}
				
#line 1420 "args/argcheck2.cpp"
				
				{
#line 385 "args/argcheck2.rl"
					{p+= 1; cs = 69; goto _out;} }
				
#line 1426 "args/argcheck2.cpp"
				
				goto _st69;
				_ctr64:
				{
#line 236 "args/argcheck2.rl"
					
					double real = rl_num;
					if (rl_den_cnt)
					real += rl_den / double(rl_den_cnt);
					
					rl_chk.values.push_back(rl_sign * real);
				}
				
#line 1440 "args/argcheck2.cpp"
				
				{
#line 380 "args/argcheck2.rl"
					rl_chk.rmin = 1; rl_chk.rmax = 1; }
				
#line 1446 "args/argcheck2.cpp"
				
				{
#line 221 "args/argcheck2.rl"
					
					if (chk_)
					chk_->push_back(rl_chk);
					
					rl_num = 0;
					rl_den = 0;
					rl_sign = 0;
					rl_den_cnt = 0;
					rl_chk = {};
				}
				
#line 1461 "args/argcheck2.cpp"
				
				{
#line 385 "args/argcheck2.rl"
					{p+= 1; cs = 69; goto _out;} }
				
#line 1467 "args/argcheck2.cpp"
				
				goto _st69;
				_ctr134:
				{
#line 232 "args/argcheck2.rl"
					
					rl_chk.values.push_back((int64_t)(rl_sign * rl_num));
				}
				
#line 1477 "args/argcheck2.cpp"
				
				{
#line 380 "args/argcheck2.rl"
					rl_chk.rmin = 1; rl_chk.rmax = 1; }
				
#line 1483 "args/argcheck2.cpp"
				
				{
#line 221 "args/argcheck2.rl"
					
					if (chk_)
					chk_->push_back(rl_chk);
					
					rl_num = 0;
					rl_den = 0;
					rl_sign = 0;
					rl_den_cnt = 0;
					rl_chk = {};
				}
				
#line 1498 "args/argcheck2.cpp"
				
				{
#line 385 "args/argcheck2.rl"
					{p+= 1; cs = 69; goto _out;} }
				
#line 1504 "args/argcheck2.cpp"
				
				goto _st69;
				_ctr177:
				{
#line 284 "args/argcheck2.rl"
					rl_chk.cmp = CMP_RANGE_SEMIOPEN; }
				
#line 1512 "args/argcheck2.cpp"
				
				{
#line 380 "args/argcheck2.rl"
					rl_chk.rmin = 1; rl_chk.rmax = 1; }
				
#line 1518 "args/argcheck2.cpp"
				
				{
#line 221 "args/argcheck2.rl"
					
					if (chk_)
					chk_->push_back(rl_chk);
					
					rl_num = 0;
					rl_den = 0;
					rl_sign = 0;
					rl_den_cnt = 0;
					rl_chk = {};
				}
				
#line 1533 "args/argcheck2.cpp"
				
				{
#line 385 "args/argcheck2.rl"
					{p+= 1; cs = 69; goto _out;} }
				
#line 1539 "args/argcheck2.cpp"
				
				goto _st69;
				_ctr184:
				{
#line 284 "args/argcheck2.rl"
					rl_chk.cmp = CMP_RANGE_CLOSED; }
				
#line 1547 "args/argcheck2.cpp"
				
				{
#line 380 "args/argcheck2.rl"
					rl_chk.rmin = 1; rl_chk.rmax = 1; }
				
#line 1553 "args/argcheck2.cpp"
				
				{
#line 221 "args/argcheck2.rl"
					
					if (chk_)
					chk_->push_back(rl_chk);
					
					rl_num = 0;
					rl_den = 0;
					rl_sign = 0;
					rl_den_cnt = 0;
					rl_chk = {};
				}
				
#line 1568 "args/argcheck2.cpp"
				
				{
#line 385 "args/argcheck2.rl"
					{p+= 1; cs = 69; goto _out;} }
				
#line 1574 "args/argcheck2.cpp"
				
				goto _st69;
				_ctr196:
				{
#line 244 "args/argcheck2.rl"
					
					try {
						ArgString str{ {}, 0 };
						str.first.assign(rl_sym_start, p - rl_sym_start);
						str.second = crc32_hash(str.first.data());
						rl_chk.values.push_back(str);
					} catch(std::exception& e) {
						LIB_ERR << "exception: " << e.what();
					}
				}
				
#line 1591 "args/argcheck2.cpp"
				
				{
#line 380 "args/argcheck2.rl"
					rl_chk.rmin = 1; rl_chk.rmax = 1; }
				
#line 1597 "args/argcheck2.cpp"
				
				{
#line 221 "args/argcheck2.rl"
					
					if (chk_)
					chk_->push_back(rl_chk);
					
					rl_num = 0;
					rl_den = 0;
					rl_sign = 0;
					rl_den_cnt = 0;
					rl_chk = {};
				}
				
#line 1612 "args/argcheck2.cpp"
				
				{
#line 385 "args/argcheck2.rl"
					{p+= 1; cs = 69; goto _out;} }
				
#line 1618 "args/argcheck2.cpp"
				
				goto _st69;
				_st69:
				p += 1;
				st_case_69:
				{
					goto _st0;
				}
				_ctr29:
				{
#line 380 "args/argcheck2.rl"
					rl_chk.rmin = 1; rl_chk.rmax = 1; }
				
#line 1632 "args/argcheck2.cpp"
				
				{
#line 367 "args/argcheck2.rl"
					rl_chk.setRepeats(0, REPEAT_INF); }
				
#line 1638 "args/argcheck2.cpp"
				
				goto _st12;
				_ctr30:
				{
#line 380 "args/argcheck2.rl"
					rl_chk.rmin = 1; rl_chk.rmax = 1; }
				
#line 1646 "args/argcheck2.cpp"
				
				{
#line 366 "args/argcheck2.rl"
					rl_chk.setRepeats(1, REPEAT_INF); }
				
#line 1652 "args/argcheck2.cpp"
				
				goto _st12;
				_ctr31:
				{
#line 380 "args/argcheck2.rl"
					rl_chk.rmin = 1; rl_chk.rmax = 1; }
				
#line 1660 "args/argcheck2.cpp"
				
				{
#line 365 "args/argcheck2.rl"
					rl_chk.setRepeats(0, 1); }
				
#line 1666 "args/argcheck2.cpp"
				
				goto _st12;
				_ctr41:
				{
#line 260 "args/argcheck2.rl"
					rl_chk.rmax = rl_chk.rmin; }
				
#line 1674 "args/argcheck2.cpp"
				
				goto _st12;
				_ctr66:
				{
#line 236 "args/argcheck2.rl"
					
					double real = rl_num;
					if (rl_den_cnt)
					real += rl_den / double(rl_den_cnt);
					
					rl_chk.values.push_back(rl_sign * real);
				}
				
#line 1688 "args/argcheck2.cpp"
				
				{
#line 380 "args/argcheck2.rl"
					rl_chk.rmin = 1; rl_chk.rmax = 1; }
				
#line 1694 "args/argcheck2.cpp"
				
				{
#line 367 "args/argcheck2.rl"
					rl_chk.setRepeats(0, REPEAT_INF); }
				
#line 1700 "args/argcheck2.cpp"
				
				goto _st12;
				_ctr67:
				{
#line 236 "args/argcheck2.rl"
					
					double real = rl_num;
					if (rl_den_cnt)
					real += rl_den / double(rl_den_cnt);
					
					rl_chk.values.push_back(rl_sign * real);
				}
				
#line 1714 "args/argcheck2.cpp"
				
				{
#line 380 "args/argcheck2.rl"
					rl_chk.rmin = 1; rl_chk.rmax = 1; }
				
#line 1720 "args/argcheck2.cpp"
				
				{
#line 366 "args/argcheck2.rl"
					rl_chk.setRepeats(1, REPEAT_INF); }
				
#line 1726 "args/argcheck2.cpp"
				
				goto _st12;
				_ctr70:
				{
#line 236 "args/argcheck2.rl"
					
					double real = rl_num;
					if (rl_den_cnt)
					real += rl_den / double(rl_den_cnt);
					
					rl_chk.values.push_back(rl_sign * real);
				}
				
#line 1740 "args/argcheck2.cpp"
				
				{
#line 380 "args/argcheck2.rl"
					rl_chk.rmin = 1; rl_chk.rmax = 1; }
				
#line 1746 "args/argcheck2.cpp"
				
				{
#line 365 "args/argcheck2.rl"
					rl_chk.setRepeats(0, 1); }
				
#line 1752 "args/argcheck2.cpp"
				
				goto _st12;
				_ctr136:
				{
#line 232 "args/argcheck2.rl"
					
					rl_chk.values.push_back((int64_t)(rl_sign * rl_num));
				}
				
#line 1762 "args/argcheck2.cpp"
				
				{
#line 380 "args/argcheck2.rl"
					rl_chk.rmin = 1; rl_chk.rmax = 1; }
				
#line 1768 "args/argcheck2.cpp"
				
				{
#line 367 "args/argcheck2.rl"
					rl_chk.setRepeats(0, REPEAT_INF); }
				
#line 1774 "args/argcheck2.cpp"
				
				goto _st12;
				_ctr137:
				{
#line 232 "args/argcheck2.rl"
					
					rl_chk.values.push_back((int64_t)(rl_sign * rl_num));
				}
				
#line 1784 "args/argcheck2.cpp"
				
				{
#line 380 "args/argcheck2.rl"
					rl_chk.rmin = 1; rl_chk.rmax = 1; }
				
#line 1790 "args/argcheck2.cpp"
				
				{
#line 366 "args/argcheck2.rl"
					rl_chk.setRepeats(1, REPEAT_INF); }
				
#line 1796 "args/argcheck2.cpp"
				
				goto _st12;
				_ctr139:
				{
#line 232 "args/argcheck2.rl"
					
					rl_chk.values.push_back((int64_t)(rl_sign * rl_num));
				}
				
#line 1806 "args/argcheck2.cpp"
				
				{
#line 380 "args/argcheck2.rl"
					rl_chk.rmin = 1; rl_chk.rmax = 1; }
				
#line 1812 "args/argcheck2.cpp"
				
				{
#line 365 "args/argcheck2.rl"
					rl_chk.setRepeats(0, 1); }
				
#line 1818 "args/argcheck2.cpp"
				
				goto _st12;
				_ctr179:
				{
#line 284 "args/argcheck2.rl"
					rl_chk.cmp = CMP_RANGE_SEMIOPEN; }
				
#line 1826 "args/argcheck2.cpp"
				
				{
#line 380 "args/argcheck2.rl"
					rl_chk.rmin = 1; rl_chk.rmax = 1; }
				
#line 1832 "args/argcheck2.cpp"
				
				{
#line 367 "args/argcheck2.rl"
					rl_chk.setRepeats(0, REPEAT_INF); }
				
#line 1838 "args/argcheck2.cpp"
				
				goto _st12;
				_ctr180:
				{
#line 284 "args/argcheck2.rl"
					rl_chk.cmp = CMP_RANGE_SEMIOPEN; }
				
#line 1846 "args/argcheck2.cpp"
				
				{
#line 380 "args/argcheck2.rl"
					rl_chk.rmin = 1; rl_chk.rmax = 1; }
				
#line 1852 "args/argcheck2.cpp"
				
				{
#line 366 "args/argcheck2.rl"
					rl_chk.setRepeats(1, REPEAT_INF); }
				
#line 1858 "args/argcheck2.cpp"
				
				goto _st12;
				_ctr181:
				{
#line 284 "args/argcheck2.rl"
					rl_chk.cmp = CMP_RANGE_SEMIOPEN; }
				
#line 1866 "args/argcheck2.cpp"
				
				{
#line 380 "args/argcheck2.rl"
					rl_chk.rmin = 1; rl_chk.rmax = 1; }
				
#line 1872 "args/argcheck2.cpp"
				
				{
#line 365 "args/argcheck2.rl"
					rl_chk.setRepeats(0, 1); }
				
#line 1878 "args/argcheck2.cpp"
				
				goto _st12;
				_ctr186:
				{
#line 284 "args/argcheck2.rl"
					rl_chk.cmp = CMP_RANGE_CLOSED; }
				
#line 1886 "args/argcheck2.cpp"
				
				{
#line 380 "args/argcheck2.rl"
					rl_chk.rmin = 1; rl_chk.rmax = 1; }
				
#line 1892 "args/argcheck2.cpp"
				
				{
#line 367 "args/argcheck2.rl"
					rl_chk.setRepeats(0, REPEAT_INF); }
				
#line 1898 "args/argcheck2.cpp"
				
				goto _st12;
				_ctr187:
				{
#line 284 "args/argcheck2.rl"
					rl_chk.cmp = CMP_RANGE_CLOSED; }
				
#line 1906 "args/argcheck2.cpp"
				
				{
#line 380 "args/argcheck2.rl"
					rl_chk.rmin = 1; rl_chk.rmax = 1; }
				
#line 1912 "args/argcheck2.cpp"
				
				{
#line 366 "args/argcheck2.rl"
					rl_chk.setRepeats(1, REPEAT_INF); }
				
#line 1918 "args/argcheck2.cpp"
				
				goto _st12;
				_ctr188:
				{
#line 284 "args/argcheck2.rl"
					rl_chk.cmp = CMP_RANGE_CLOSED; }
				
#line 1926 "args/argcheck2.cpp"
				
				{
#line 380 "args/argcheck2.rl"
					rl_chk.rmin = 1; rl_chk.rmax = 1; }
				
#line 1932 "args/argcheck2.cpp"
				
				{
#line 365 "args/argcheck2.rl"
					rl_chk.setRepeats(0, 1); }
				
#line 1938 "args/argcheck2.cpp"
				
				goto _st12;
				_ctr198:
				{
#line 244 "args/argcheck2.rl"
					
					try {
						ArgString str{ {}, 0 };
						str.first.assign(rl_sym_start, p - rl_sym_start);
						str.second = crc32_hash(str.first.data());
						rl_chk.values.push_back(str);
					} catch(std::exception& e) {
						LIB_ERR << "exception: " << e.what();
					}
				}
				
#line 1955 "args/argcheck2.cpp"
				
				{
#line 380 "args/argcheck2.rl"
					rl_chk.rmin = 1; rl_chk.rmax = 1; }
				
#line 1961 "args/argcheck2.cpp"
				
				{
#line 367 "args/argcheck2.rl"
					rl_chk.setRepeats(0, REPEAT_INF); }
				
#line 1967 "args/argcheck2.cpp"
				
				goto _st12;
				_ctr199:
				{
#line 244 "args/argcheck2.rl"
					
					try {
						ArgString str{ {}, 0 };
						str.first.assign(rl_sym_start, p - rl_sym_start);
						str.second = crc32_hash(str.first.data());
						rl_chk.values.push_back(str);
					} catch(std::exception& e) {
						LIB_ERR << "exception: " << e.what();
					}
				}
				
#line 1984 "args/argcheck2.cpp"
				
				{
#line 380 "args/argcheck2.rl"
					rl_chk.rmin = 1; rl_chk.rmax = 1; }
				
#line 1990 "args/argcheck2.cpp"
				
				{
#line 366 "args/argcheck2.rl"
					rl_chk.setRepeats(1, REPEAT_INF); }
				
#line 1996 "args/argcheck2.cpp"
				
				goto _st12;
				_ctr200:
				{
#line 244 "args/argcheck2.rl"
					
					try {
						ArgString str{ {}, 0 };
						str.first.assign(rl_sym_start, p - rl_sym_start);
						str.second = crc32_hash(str.first.data());
						rl_chk.values.push_back(str);
					} catch(std::exception& e) {
						LIB_ERR << "exception: " << e.what();
					}
				}
				
#line 2013 "args/argcheck2.cpp"
				
				{
#line 380 "args/argcheck2.rl"
					rl_chk.rmin = 1; rl_chk.rmax = 1; }
				
#line 2019 "args/argcheck2.cpp"
				
				{
#line 365 "args/argcheck2.rl"
					rl_chk.setRepeats(0, 1); }
				
#line 2025 "args/argcheck2.cpp"
				
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
#line 380 "args/argcheck2.rl"
					rl_chk.rmin = 1; rl_chk.rmax = 1; }
				
#line 2047 "args/argcheck2.cpp"
				
				{
#line 261 "args/argcheck2.rl"
					rl_chk.rmin = 0; rl_chk.rmax = REPEAT_INF; }
				
#line 2053 "args/argcheck2.cpp"
				
				goto _st13;
				_ctr71:
				{
#line 236 "args/argcheck2.rl"
					
					double real = rl_num;
					if (rl_den_cnt)
					real += rl_den / double(rl_den_cnt);
					
					rl_chk.values.push_back(rl_sign * real);
				}
				
#line 2067 "args/argcheck2.cpp"
				
				{
#line 380 "args/argcheck2.rl"
					rl_chk.rmin = 1; rl_chk.rmax = 1; }
				
#line 2073 "args/argcheck2.cpp"
				
				{
#line 261 "args/argcheck2.rl"
					rl_chk.rmin = 0; rl_chk.rmax = REPEAT_INF; }
				
#line 2079 "args/argcheck2.cpp"
				
				goto _st13;
				_ctr140:
				{
#line 232 "args/argcheck2.rl"
					
					rl_chk.values.push_back((int64_t)(rl_sign * rl_num));
				}
				
#line 2089 "args/argcheck2.cpp"
				
				{
#line 380 "args/argcheck2.rl"
					rl_chk.rmin = 1; rl_chk.rmax = 1; }
				
#line 2095 "args/argcheck2.cpp"
				
				{
#line 261 "args/argcheck2.rl"
					rl_chk.rmin = 0; rl_chk.rmax = REPEAT_INF; }
				
#line 2101 "args/argcheck2.cpp"
				
				goto _st13;
				_ctr182:
				{
#line 284 "args/argcheck2.rl"
					rl_chk.cmp = CMP_RANGE_SEMIOPEN; }
				
#line 2109 "args/argcheck2.cpp"
				
				{
#line 380 "args/argcheck2.rl"
					rl_chk.rmin = 1; rl_chk.rmax = 1; }
				
#line 2115 "args/argcheck2.cpp"
				
				{
#line 261 "args/argcheck2.rl"
					rl_chk.rmin = 0; rl_chk.rmax = REPEAT_INF; }
				
#line 2121 "args/argcheck2.cpp"
				
				goto _st13;
				_ctr189:
				{
#line 284 "args/argcheck2.rl"
					rl_chk.cmp = CMP_RANGE_CLOSED; }
				
#line 2129 "args/argcheck2.cpp"
				
				{
#line 380 "args/argcheck2.rl"
					rl_chk.rmin = 1; rl_chk.rmax = 1; }
				
#line 2135 "args/argcheck2.cpp"
				
				{
#line 261 "args/argcheck2.rl"
					rl_chk.rmin = 0; rl_chk.rmax = REPEAT_INF; }
				
#line 2141 "args/argcheck2.cpp"
				
				goto _st13;
				_ctr201:
				{
#line 244 "args/argcheck2.rl"
					
					try {
						ArgString str{ {}, 0 };
						str.first.assign(rl_sym_start, p - rl_sym_start);
						str.second = crc32_hash(str.first.data());
						rl_chk.values.push_back(str);
					} catch(std::exception& e) {
						LIB_ERR << "exception: " << e.what();
					}
				}
				
#line 2158 "args/argcheck2.cpp"
				
				{
#line 380 "args/argcheck2.rl"
					rl_chk.rmin = 1; rl_chk.rmax = 1; }
				
#line 2164 "args/argcheck2.cpp"
				
				{
#line 261 "args/argcheck2.rl"
					rl_chk.rmin = 0; rl_chk.rmax = REPEAT_INF; }
				
#line 2170 "args/argcheck2.cpp"
				
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
#line 258 "args/argcheck2.rl"
					rl_chk.rmin = 0; }
				
#line 2190 "args/argcheck2.cpp"
				
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
#line 259 "args/argcheck2.rl"
					rl_chk.rmax = 0; }
				
#line 2229 "args/argcheck2.cpp"
				
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
#line 259 "args/argcheck2.rl"
					rl_chk.rmax = (( (*( p))))-'0'; }
				
#line 2246 "args/argcheck2.cpp"
				
				goto _st17;
				_ctr46:
				{
#line 259 "args/argcheck2.rl"
					(rl_chk.rmax *= 10) += ((( (*( p)))) - '0'); }
				
#line 2254 "args/argcheck2.cpp"
				
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
#line 258 "args/argcheck2.rl"
					rl_chk.rmin = (( (*( p))))-'0'; }
				
#line 2274 "args/argcheck2.cpp"
				
				goto _st18;
				_ctr48:
				{
#line 258 "args/argcheck2.rl"
					(rl_chk.rmin *= 10) += ((( (*( p)))) - '0'); }
				
#line 2282 "args/argcheck2.cpp"
				
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
#line 364 "args/argcheck2.rl"
					rl_chk.type = CHECK_FLOAT; }
				
#line 2307 "args/argcheck2.cpp"
				
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
					case 61: {
						goto _ctr52;
					}
					case 62: {
						goto _ctr53;
					}
					case 63: {
						goto _ctr31;
					}
					case 91: {
						goto _st33;
					}
					case 123: {
						goto _ctr32;
					}
					case 126: {
						goto _ctr55;
					}
				}
				{
					goto _st0;
				}
				_st20:
				p += 1;
				st_case_20:
				if ( ( (*( p))) == 61 ) {
					goto _ctr56;
				}
				{
					goto _st0;
				}
				_ctr56:
				{
#line 297 "args/argcheck2.rl"
					rl_chk.cmp = CMP_NOT_EQUAL; }
				
#line 2368 "args/argcheck2.cpp"
				
				goto _st21;
				_ctr76:
				{
#line 296 "args/argcheck2.rl"
					rl_chk.cmp = CMP_LESS_EQ; }
				
#line 2376 "args/argcheck2.cpp"
				
				goto _st21;
				_ctr91:
				{
#line 295 "args/argcheck2.rl"
					rl_chk.cmp = CMP_GREATER_EQ; }
				
#line 2384 "args/argcheck2.cpp"
				
				goto _st21;
				_st21:
				p += 1;
				st_case_21:
				switch( ( (*( p))) ) {
					case 43: {
						goto _ctr58;
					}
					case 45: {
						goto _ctr59;
					}
				}
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr60;
				}
				{
					goto _st0;
				}
				_ctr58:
				{
#line 272 "args/argcheck2.rl"
					rl_sign = 1; }
				
#line 2409 "args/argcheck2.cpp"
				
				{
#line 263 "args/argcheck2.rl"
					rl_sign = 1; }
				
#line 2415 "args/argcheck2.cpp"
				
				goto _st22;
				_ctr59:
				{
#line 272 "args/argcheck2.rl"
					rl_sign = 1; }
				
#line 2423 "args/argcheck2.cpp"
				
				{
#line 264 "args/argcheck2.rl"
					rl_sign = -1; }
				
#line 2429 "args/argcheck2.cpp"
				
				goto _st22;
				_st22:
				p += 1;
				st_case_22:
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr62;
				}
				{
					goto _st0;
				}
				_ctr60:
				{
#line 272 "args/argcheck2.rl"
					rl_sign = 1; }
				
#line 2446 "args/argcheck2.cpp"
				
				{
#line 266 "args/argcheck2.rl"
					rl_num = 0; }
				
#line 2452 "args/argcheck2.cpp"
				
				{
#line 266 "args/argcheck2.rl"
					(rl_num *= 10) += ((( (*( p)))) - '0'); }
				
#line 2458 "args/argcheck2.cpp"
				
				goto _st23;
				_ctr62:
				{
#line 266 "args/argcheck2.rl"
					rl_num = 0; }
				
#line 2466 "args/argcheck2.cpp"
				
				{
#line 266 "args/argcheck2.rl"
					(rl_num *= 10) += ((( (*( p)))) - '0'); }
				
#line 2472 "args/argcheck2.cpp"
				
				goto _st23;
				_ctr69:
				{
#line 266 "args/argcheck2.rl"
					(rl_num *= 10) += ((( (*( p)))) - '0'); }
				
#line 2480 "args/argcheck2.cpp"
				
				goto _st23;
				_st23:
				p += 1;
				st_case_23:
				switch( ( (*( p))) ) {
					case 0: {
						goto _ctr64;
					}
					case 32: {
						goto _ctr65;
					}
					case 42: {
						goto _ctr66;
					}
					case 43: {
						goto _ctr67;
					}
					case 46: {
						goto _st24;
					}
					case 63: {
						goto _ctr70;
					}
					case 123: {
						goto _ctr71;
					}
				}
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr69;
				}
				{
					goto _st0;
				}
				_st24:
				p += 1;
				st_case_24:
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr72;
				}
				{
					goto _st0;
				}
				_ctr72:
				{
#line 267 "args/argcheck2.rl"
					rl_den = 0; rl_den_cnt = 1; }
				
#line 2529 "args/argcheck2.cpp"
				
				{
#line 267 "args/argcheck2.rl"
					(rl_den *= 10) += ((( (*( p)))) - '0'); rl_den_cnt *= 10; }
				
#line 2535 "args/argcheck2.cpp"
				
				goto _st25;
				_ctr74:
				{
#line 267 "args/argcheck2.rl"
					(rl_den *= 10) += ((( (*( p)))) - '0'); rl_den_cnt *= 10; }
				
#line 2543 "args/argcheck2.cpp"
				
				goto _st25;
				_st25:
				p += 1;
				st_case_25:
				switch( ( (*( p))) ) {
					case 0: {
						goto _ctr64;
					}
					case 32: {
						goto _ctr65;
					}
					case 42: {
						goto _ctr66;
					}
					case 43: {
						goto _ctr67;
					}
					case 63: {
						goto _ctr70;
					}
					case 123: {
						goto _ctr71;
					}
				}
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr74;
				}
				{
					goto _st0;
				}
				_ctr51:
				{
#line 296 "args/argcheck2.rl"
					rl_chk.cmp = CMP_LESS; }
				
#line 2580 "args/argcheck2.cpp"
				
				goto _st26;
				_st26:
				p += 1;
				st_case_26:
				switch( ( (*( p))) ) {
					case 43: {
						goto _ctr58;
					}
					case 45: {
						goto _ctr59;
					}
					case 61: {
						goto _ctr76;
					}
				}
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr60;
				}
				{
					goto _st0;
				}
				_ctr52:
				{
#line 342 "args/argcheck2.rl"
					rl_chk.cmp = CMP_EQUAL; }
				
#line 2608 "args/argcheck2.cpp"
				
				goto _st27;
				_ctr55:
				{
#line 350 "args/argcheck2.rl"
					rl_chk.cmp = CMP_APPROX; }
				
#line 2616 "args/argcheck2.cpp"
				
				goto _st27;
				_ctr86:
				{
#line 236 "args/argcheck2.rl"
					
					double real = rl_num;
					if (rl_den_cnt)
					real += rl_den / double(rl_den_cnt);
					
					rl_chk.values.push_back(rl_sign * real);
				}
				
#line 2630 "args/argcheck2.cpp"
				
				goto _st27;
				_st27:
				p += 1;
				st_case_27:
				switch( ( (*( p))) ) {
					case 43: {
						goto _ctr78;
					}
					case 45: {
						goto _ctr79;
					}
				}
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr80;
				}
				{
					goto _st0;
				}
				_ctr78:
				{
#line 272 "args/argcheck2.rl"
					rl_sign = 1; }
				
#line 2655 "args/argcheck2.cpp"
				
				{
#line 263 "args/argcheck2.rl"
					rl_sign = 1; }
				
#line 2661 "args/argcheck2.cpp"
				
				goto _st28;
				_ctr79:
				{
#line 272 "args/argcheck2.rl"
					rl_sign = 1; }
				
#line 2669 "args/argcheck2.cpp"
				
				{
#line 264 "args/argcheck2.rl"
					rl_sign = -1; }
				
#line 2675 "args/argcheck2.cpp"
				
				goto _st28;
				_st28:
				p += 1;
				st_case_28:
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr82;
				}
				{
					goto _st0;
				}
				_ctr80:
				{
#line 272 "args/argcheck2.rl"
					rl_sign = 1; }
				
#line 2692 "args/argcheck2.cpp"
				
				{
#line 266 "args/argcheck2.rl"
					rl_num = 0; }
				
#line 2698 "args/argcheck2.cpp"
				
				{
#line 266 "args/argcheck2.rl"
					(rl_num *= 10) += ((( (*( p)))) - '0'); }
				
#line 2704 "args/argcheck2.cpp"
				
				goto _st29;
				_ctr82:
				{
#line 266 "args/argcheck2.rl"
					rl_num = 0; }
				
#line 2712 "args/argcheck2.cpp"
				
				{
#line 266 "args/argcheck2.rl"
					(rl_num *= 10) += ((( (*( p)))) - '0'); }
				
#line 2718 "args/argcheck2.cpp"
				
				goto _st29;
				_ctr85:
				{
#line 266 "args/argcheck2.rl"
					(rl_num *= 10) += ((( (*( p)))) - '0'); }
				
#line 2726 "args/argcheck2.cpp"
				
				goto _st29;
				_st29:
				p += 1;
				st_case_29:
				switch( ( (*( p))) ) {
					case 0: {
						goto _ctr64;
					}
					case 32: {
						goto _ctr65;
					}
					case 42: {
						goto _ctr66;
					}
					case 43: {
						goto _ctr67;
					}
					case 46: {
						goto _st30;
					}
					case 63: {
						goto _ctr70;
					}
					case 123: {
						goto _ctr71;
					}
					case 124: {
						goto _ctr86;
					}
				}
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr85;
				}
				{
					goto _st0;
				}
				_st30:
				p += 1;
				st_case_30:
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr87;
				}
				{
					goto _st0;
				}
				_ctr87:
				{
#line 267 "args/argcheck2.rl"
					rl_den = 0; rl_den_cnt = 1; }
				
#line 2778 "args/argcheck2.cpp"
				
				{
#line 267 "args/argcheck2.rl"
					(rl_den *= 10) += ((( (*( p)))) - '0'); rl_den_cnt *= 10; }
				
#line 2784 "args/argcheck2.cpp"
				
				goto _st31;
				_ctr89:
				{
#line 267 "args/argcheck2.rl"
					(rl_den *= 10) += ((( (*( p)))) - '0'); rl_den_cnt *= 10; }
				
#line 2792 "args/argcheck2.cpp"
				
				goto _st31;
				_st31:
				p += 1;
				st_case_31:
				switch( ( (*( p))) ) {
					case 0: {
						goto _ctr64;
					}
					case 32: {
						goto _ctr65;
					}
					case 42: {
						goto _ctr66;
					}
					case 43: {
						goto _ctr67;
					}
					case 63: {
						goto _ctr70;
					}
					case 123: {
						goto _ctr71;
					}
					case 124: {
						goto _ctr86;
					}
				}
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr89;
				}
				{
					goto _st0;
				}
				_ctr53:
				{
#line 295 "args/argcheck2.rl"
					rl_chk.cmp = CMP_GREATER; }
				
#line 2832 "args/argcheck2.cpp"
				
				goto _st32;
				_st32:
				p += 1;
				st_case_32:
				switch( ( (*( p))) ) {
					case 43: {
						goto _ctr58;
					}
					case 45: {
						goto _ctr59;
					}
					case 61: {
						goto _ctr91;
					}
				}
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr60;
				}
				{
					goto _st0;
				}
				_st33:
				p += 1;
				st_case_33:
				switch( ( (*( p))) ) {
					case 43: {
						goto _ctr92;
					}
					case 45: {
						goto _ctr93;
					}
				}
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr94;
				}
				{
					goto _st0;
				}
				_ctr92:
				{
#line 272 "args/argcheck2.rl"
					rl_sign = 1; }
				
#line 2877 "args/argcheck2.cpp"
				
				{
#line 263 "args/argcheck2.rl"
					rl_sign = 1; }
				
#line 2883 "args/argcheck2.cpp"
				
				goto _st34;
				_ctr93:
				{
#line 272 "args/argcheck2.rl"
					rl_sign = 1; }
				
#line 2891 "args/argcheck2.cpp"
				
				{
#line 264 "args/argcheck2.rl"
					rl_sign = -1; }
				
#line 2897 "args/argcheck2.cpp"
				
				goto _st34;
				_st34:
				p += 1;
				st_case_34:
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr96;
				}
				{
					goto _st0;
				}
				_ctr94:
				{
#line 272 "args/argcheck2.rl"
					rl_sign = 1; }
				
#line 2914 "args/argcheck2.cpp"
				
				{
#line 266 "args/argcheck2.rl"
					rl_num = 0; }
				
#line 2920 "args/argcheck2.cpp"
				
				{
#line 266 "args/argcheck2.rl"
					(rl_num *= 10) += ((( (*( p)))) - '0'); }
				
#line 2926 "args/argcheck2.cpp"
				
				goto _st35;
				_ctr96:
				{
#line 266 "args/argcheck2.rl"
					rl_num = 0; }
				
#line 2934 "args/argcheck2.cpp"
				
				{
#line 266 "args/argcheck2.rl"
					(rl_num *= 10) += ((( (*( p)))) - '0'); }
				
#line 2940 "args/argcheck2.cpp"
				
				goto _st35;
				_ctr100:
				{
#line 266 "args/argcheck2.rl"
					(rl_num *= 10) += ((( (*( p)))) - '0'); }
				
#line 2948 "args/argcheck2.cpp"
				
				goto _st35;
				_st35:
				p += 1;
				st_case_35:
				switch( ( (*( p))) ) {
					case 44: {
						goto _ctr98;
					}
					case 46: {
						goto _st41;
					}
				}
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr100;
				}
				{
					goto _st0;
				}
				_ctr98:
				{
#line 236 "args/argcheck2.rl"
					
					double real = rl_num;
					if (rl_den_cnt)
					real += rl_den / double(rl_den_cnt);
					
					rl_chk.values.push_back(rl_sign * real);
				}
				
#line 2979 "args/argcheck2.cpp"
				
				goto _st36;
				_st36:
				p += 1;
				st_case_36:
				switch( ( (*( p))) ) {
					case 43: {
						goto _ctr102;
					}
					case 45: {
						goto _ctr103;
					}
				}
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr104;
				}
				{
					goto _st0;
				}
				_ctr102:
				{
#line 272 "args/argcheck2.rl"
					rl_sign = 1; }
				
#line 3004 "args/argcheck2.cpp"
				
				{
#line 263 "args/argcheck2.rl"
					rl_sign = 1; }
				
#line 3010 "args/argcheck2.cpp"
				
				goto _st37;
				_ctr103:
				{
#line 272 "args/argcheck2.rl"
					rl_sign = 1; }
				
#line 3018 "args/argcheck2.cpp"
				
				{
#line 264 "args/argcheck2.rl"
					rl_sign = -1; }
				
#line 3024 "args/argcheck2.cpp"
				
				goto _st37;
				_st37:
				p += 1;
				st_case_37:
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr106;
				}
				{
					goto _st0;
				}
				_ctr104:
				{
#line 272 "args/argcheck2.rl"
					rl_sign = 1; }
				
#line 3041 "args/argcheck2.cpp"
				
				{
#line 266 "args/argcheck2.rl"
					rl_num = 0; }
				
#line 3047 "args/argcheck2.cpp"
				
				{
#line 266 "args/argcheck2.rl"
					(rl_num *= 10) += ((( (*( p)))) - '0'); }
				
#line 3053 "args/argcheck2.cpp"
				
				goto _st38;
				_ctr106:
				{
#line 266 "args/argcheck2.rl"
					rl_num = 0; }
				
#line 3061 "args/argcheck2.cpp"
				
				{
#line 266 "args/argcheck2.rl"
					(rl_num *= 10) += ((( (*( p)))) - '0'); }
				
#line 3067 "args/argcheck2.cpp"
				
				goto _st38;
				_ctr110:
				{
#line 266 "args/argcheck2.rl"
					(rl_num *= 10) += ((( (*( p)))) - '0'); }
				
#line 3075 "args/argcheck2.cpp"
				
				goto _st38;
				_st38:
				p += 1;
				st_case_38:
				switch( ( (*( p))) ) {
					case 41: {
						goto _ctr108;
					}
					case 46: {
						goto _st39;
					}
					case 93: {
						goto _ctr111;
					}
				}
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr110;
				}
				{
					goto _st0;
				}
				_st39:
				p += 1;
				st_case_39:
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr112;
				}
				{
					goto _st0;
				}
				_ctr112:
				{
#line 267 "args/argcheck2.rl"
					rl_den = 0; rl_den_cnt = 1; }
				
#line 3112 "args/argcheck2.cpp"
				
				{
#line 267 "args/argcheck2.rl"
					(rl_den *= 10) += ((( (*( p)))) - '0'); rl_den_cnt *= 10; }
				
#line 3118 "args/argcheck2.cpp"
				
				goto _st40;
				_ctr114:
				{
#line 267 "args/argcheck2.rl"
					(rl_den *= 10) += ((( (*( p)))) - '0'); rl_den_cnt *= 10; }
				
#line 3126 "args/argcheck2.cpp"
				
				goto _st40;
				_st40:
				p += 1;
				st_case_40:
				switch( ( (*( p))) ) {
					case 41: {
						goto _ctr108;
					}
					case 93: {
						goto _ctr111;
					}
				}
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr114;
				}
				{
					goto _st0;
				}
				_st41:
				p += 1;
				st_case_41:
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr115;
				}
				{
					goto _st0;
				}
				_ctr115:
				{
#line 267 "args/argcheck2.rl"
					rl_den = 0; rl_den_cnt = 1; }
				
#line 3160 "args/argcheck2.cpp"
				
				{
#line 267 "args/argcheck2.rl"
					(rl_den *= 10) += ((( (*( p)))) - '0'); rl_den_cnt *= 10; }
				
#line 3166 "args/argcheck2.cpp"
				
				goto _st42;
				_ctr117:
				{
#line 267 "args/argcheck2.rl"
					(rl_den *= 10) += ((( (*( p)))) - '0'); rl_den_cnt *= 10; }
				
#line 3174 "args/argcheck2.cpp"
				
				goto _st42;
				_st42:
				p += 1;
				st_case_42:
				if ( ( (*( p))) == 44 ) {
					goto _ctr98;
				}
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr117;
				}
				{
					goto _st0;
				}
				_ctr7:
				{
#line 362 "args/argcheck2.rl"
					rl_chk.type = CHECK_INT; }
				
#line 3194 "args/argcheck2.cpp"
				
				goto _st43;
				_st43:
				p += 1;
				st_case_43:
				switch( ( (*( p))) ) {
					case 0: {
						goto _ctr27;
					}
					case 32: {
						goto _ctr28;
					}
					case 33: {
						goto _st44;
					}
					case 37: {
						goto _ctr120;
					}
					case 42: {
						goto _ctr29;
					}
					case 43: {
						goto _ctr30;
					}
					case 60: {
						goto _ctr121;
					}
					case 61: {
						goto _ctr122;
					}
					case 62: {
						goto _ctr123;
					}
					case 63: {
						goto _ctr31;
					}
					case 91: {
						goto _st55;
					}
					case 94: {
						goto _st63;
					}
					case 123: {
						goto _ctr32;
					}
				}
				{
					goto _st0;
				}
				_st44:
				p += 1;
				st_case_44:
				if ( ( (*( p))) == 61 ) {
					goto _ctr126;
				}
				{
					goto _st0;
				}
				_ctr126:
				{
#line 297 "args/argcheck2.rl"
					rl_chk.cmp = CMP_NOT_EQUAL; }
				
#line 3258 "args/argcheck2.cpp"
				
				goto _st45;
				_ctr146:
				{
#line 296 "args/argcheck2.rl"
					rl_chk.cmp = CMP_LESS_EQ; }
				
#line 3266 "args/argcheck2.cpp"
				
				goto _st45;
				_ctr157:
				{
#line 295 "args/argcheck2.rl"
					rl_chk.cmp = CMP_GREATER_EQ; }
				
#line 3274 "args/argcheck2.cpp"
				
				goto _st45;
				_st45:
				p += 1;
				st_case_45:
				switch( ( (*( p))) ) {
					case 43: {
						goto _ctr128;
					}
					case 45: {
						goto _ctr129;
					}
				}
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr130;
				}
				{
					goto _st0;
				}
				_ctr128:
				{
#line 272 "args/argcheck2.rl"
					rl_sign = 1; }
				
#line 3299 "args/argcheck2.cpp"
				
				{
#line 263 "args/argcheck2.rl"
					rl_sign = 1; }
				
#line 3305 "args/argcheck2.cpp"
				
				goto _st46;
				_ctr129:
				{
#line 272 "args/argcheck2.rl"
					rl_sign = 1; }
				
#line 3313 "args/argcheck2.cpp"
				
				{
#line 264 "args/argcheck2.rl"
					rl_sign = -1; }
				
#line 3319 "args/argcheck2.cpp"
				
				goto _st46;
				_st46:
				p += 1;
				st_case_46:
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr132;
				}
				{
					goto _st0;
				}
				_ctr130:
				{
#line 272 "args/argcheck2.rl"
					rl_sign = 1; }
				
#line 3336 "args/argcheck2.cpp"
				
				{
#line 266 "args/argcheck2.rl"
					rl_num = 0; }
				
#line 3342 "args/argcheck2.cpp"
				
				{
#line 266 "args/argcheck2.rl"
					(rl_num *= 10) += ((( (*( p)))) - '0'); }
				
#line 3348 "args/argcheck2.cpp"
				
				goto _st47;
				_ctr132:
				{
#line 266 "args/argcheck2.rl"
					rl_num = 0; }
				
#line 3356 "args/argcheck2.cpp"
				
				{
#line 266 "args/argcheck2.rl"
					(rl_num *= 10) += ((( (*( p)))) - '0'); }
				
#line 3362 "args/argcheck2.cpp"
				
				goto _st47;
				_ctr138:
				{
#line 266 "args/argcheck2.rl"
					(rl_num *= 10) += ((( (*( p)))) - '0'); }
				
#line 3370 "args/argcheck2.cpp"
				
				goto _st47;
				_st47:
				p += 1;
				st_case_47:
				switch( ( (*( p))) ) {
					case 0: {
						goto _ctr134;
					}
					case 32: {
						goto _ctr135;
					}
					case 42: {
						goto _ctr136;
					}
					case 43: {
						goto _ctr137;
					}
					case 63: {
						goto _ctr139;
					}
					case 123: {
						goto _ctr140;
					}
				}
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr138;
				}
				{
					goto _st0;
				}
				_ctr120:
				{
#line 302 "args/argcheck2.rl"
					rl_chk.cmp = CMP_MODULE; }
				
#line 3407 "args/argcheck2.cpp"
				
				goto _st48;
				_st48:
				p += 1;
				st_case_48:
				if ( 49 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr142;
				}
				{
					goto _st0;
				}
				_ctr142:
				{
#line 303 "args/argcheck2.rl"
					rl_sign = 1; rl_num = 0; }
				
#line 3424 "args/argcheck2.cpp"
				
				{
#line 303 "args/argcheck2.rl"
					(rl_num *= 10) += ((( (*( p)))) - '0'); }
				
#line 3430 "args/argcheck2.cpp"
				
				goto _st49;
				_ctr144:
				{
#line 303 "args/argcheck2.rl"
					(rl_num *= 10) += ((( (*( p)))) - '0'); }
				
#line 3438 "args/argcheck2.cpp"
				
				goto _st49;
				_st49:
				p += 1;
				st_case_49:
				switch( ( (*( p))) ) {
					case 0: {
						goto _ctr134;
					}
					case 32: {
						goto _ctr135;
					}
					case 42: {
						goto _ctr136;
					}
					case 43: {
						goto _ctr137;
					}
					case 63: {
						goto _ctr139;
					}
					case 123: {
						goto _ctr140;
					}
				}
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr144;
				}
				{
					goto _st0;
				}
				_ctr121:
				{
#line 296 "args/argcheck2.rl"
					rl_chk.cmp = CMP_LESS; }
				
#line 3475 "args/argcheck2.cpp"
				
				goto _st50;
				_st50:
				p += 1;
				st_case_50:
				switch( ( (*( p))) ) {
					case 43: {
						goto _ctr128;
					}
					case 45: {
						goto _ctr129;
					}
					case 61: {
						goto _ctr146;
					}
				}
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr130;
				}
				{
					goto _st0;
				}
				_ctr122:
				{
#line 292 "args/argcheck2.rl"
					rl_chk.cmp = CMP_EQUAL; }
				
#line 3503 "args/argcheck2.cpp"
				
				goto _st51;
				_ctr155:
				{
#line 232 "args/argcheck2.rl"
					
					rl_chk.values.push_back((int64_t)(rl_sign * rl_num));
				}
				
#line 3513 "args/argcheck2.cpp"
				
				goto _st51;
				_st51:
				p += 1;
				st_case_51:
				switch( ( (*( p))) ) {
					case 43: {
						goto _ctr148;
					}
					case 45: {
						goto _ctr149;
					}
				}
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr150;
				}
				{
					goto _st0;
				}
				_ctr148:
				{
#line 272 "args/argcheck2.rl"
					rl_sign = 1; }
				
#line 3538 "args/argcheck2.cpp"
				
				{
#line 263 "args/argcheck2.rl"
					rl_sign = 1; }
				
#line 3544 "args/argcheck2.cpp"
				
				goto _st52;
				_ctr149:
				{
#line 272 "args/argcheck2.rl"
					rl_sign = 1; }
				
#line 3552 "args/argcheck2.cpp"
				
				{
#line 264 "args/argcheck2.rl"
					rl_sign = -1; }
				
#line 3558 "args/argcheck2.cpp"
				
				goto _st52;
				_st52:
				p += 1;
				st_case_52:
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr152;
				}
				{
					goto _st0;
				}
				_ctr150:
				{
#line 272 "args/argcheck2.rl"
					rl_sign = 1; }
				
#line 3575 "args/argcheck2.cpp"
				
				{
#line 266 "args/argcheck2.rl"
					rl_num = 0; }
				
#line 3581 "args/argcheck2.cpp"
				
				{
#line 266 "args/argcheck2.rl"
					(rl_num *= 10) += ((( (*( p)))) - '0'); }
				
#line 3587 "args/argcheck2.cpp"
				
				goto _st53;
				_ctr152:
				{
#line 266 "args/argcheck2.rl"
					rl_num = 0; }
				
#line 3595 "args/argcheck2.cpp"
				
				{
#line 266 "args/argcheck2.rl"
					(rl_num *= 10) += ((( (*( p)))) - '0'); }
				
#line 3601 "args/argcheck2.cpp"
				
				goto _st53;
				_ctr154:
				{
#line 266 "args/argcheck2.rl"
					(rl_num *= 10) += ((( (*( p)))) - '0'); }
				
#line 3609 "args/argcheck2.cpp"
				
				goto _st53;
				_st53:
				p += 1;
				st_case_53:
				switch( ( (*( p))) ) {
					case 0: {
						goto _ctr134;
					}
					case 32: {
						goto _ctr135;
					}
					case 42: {
						goto _ctr136;
					}
					case 43: {
						goto _ctr137;
					}
					case 63: {
						goto _ctr139;
					}
					case 123: {
						goto _ctr140;
					}
					case 124: {
						goto _ctr155;
					}
				}
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr154;
				}
				{
					goto _st0;
				}
				_ctr123:
				{
#line 295 "args/argcheck2.rl"
					rl_chk.cmp = CMP_GREATER; }
				
#line 3649 "args/argcheck2.cpp"
				
				goto _st54;
				_st54:
				p += 1;
				st_case_54:
				switch( ( (*( p))) ) {
					case 43: {
						goto _ctr128;
					}
					case 45: {
						goto _ctr129;
					}
					case 61: {
						goto _ctr157;
					}
				}
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr130;
				}
				{
					goto _st0;
				}
				_st55:
				p += 1;
				st_case_55:
				switch( ( (*( p))) ) {
					case 43: {
						goto _ctr158;
					}
					case 45: {
						goto _ctr159;
					}
				}
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr160;
				}
				{
					goto _st0;
				}
				_ctr158:
				{
#line 272 "args/argcheck2.rl"
					rl_sign = 1; }
				
#line 3694 "args/argcheck2.cpp"
				
				{
#line 263 "args/argcheck2.rl"
					rl_sign = 1; }
				
#line 3700 "args/argcheck2.cpp"
				
				goto _st56;
				_ctr159:
				{
#line 272 "args/argcheck2.rl"
					rl_sign = 1; }
				
#line 3708 "args/argcheck2.cpp"
				
				{
#line 264 "args/argcheck2.rl"
					rl_sign = -1; }
				
#line 3714 "args/argcheck2.cpp"
				
				goto _st56;
				_st56:
				p += 1;
				st_case_56:
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr162;
				}
				{
					goto _st0;
				}
				_ctr160:
				{
#line 272 "args/argcheck2.rl"
					rl_sign = 1; }
				
#line 3731 "args/argcheck2.cpp"
				
				{
#line 266 "args/argcheck2.rl"
					rl_num = 0; }
				
#line 3737 "args/argcheck2.cpp"
				
				{
#line 266 "args/argcheck2.rl"
					(rl_num *= 10) += ((( (*( p)))) - '0'); }
				
#line 3743 "args/argcheck2.cpp"
				
				goto _st57;
				_ctr162:
				{
#line 266 "args/argcheck2.rl"
					rl_num = 0; }
				
#line 3751 "args/argcheck2.cpp"
				
				{
#line 266 "args/argcheck2.rl"
					(rl_num *= 10) += ((( (*( p)))) - '0'); }
				
#line 3757 "args/argcheck2.cpp"
				
				goto _st57;
				_ctr165:
				{
#line 266 "args/argcheck2.rl"
					(rl_num *= 10) += ((( (*( p)))) - '0'); }
				
#line 3765 "args/argcheck2.cpp"
				
				goto _st57;
				_st57:
				p += 1;
				st_case_57:
				if ( ( (*( p))) == 44 ) {
					goto _ctr164;
				}
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr165;
				}
				{
					goto _st0;
				}
				_ctr164:
				{
#line 232 "args/argcheck2.rl"
					
					rl_chk.values.push_back((int64_t)(rl_sign * rl_num));
				}
				
#line 3787 "args/argcheck2.cpp"
				
				goto _st58;
				_st58:
				p += 1;
				st_case_58:
				switch( ( (*( p))) ) {
					case 43: {
						goto _ctr167;
					}
					case 45: {
						goto _ctr168;
					}
				}
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr169;
				}
				{
					goto _st0;
				}
				_ctr167:
				{
#line 272 "args/argcheck2.rl"
					rl_sign = 1; }
				
#line 3812 "args/argcheck2.cpp"
				
				{
#line 263 "args/argcheck2.rl"
					rl_sign = 1; }
				
#line 3818 "args/argcheck2.cpp"
				
				goto _st59;
				_ctr168:
				{
#line 272 "args/argcheck2.rl"
					rl_sign = 1; }
				
#line 3826 "args/argcheck2.cpp"
				
				{
#line 264 "args/argcheck2.rl"
					rl_sign = -1; }
				
#line 3832 "args/argcheck2.cpp"
				
				goto _st59;
				_st59:
				p += 1;
				st_case_59:
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr171;
				}
				{
					goto _st0;
				}
				_ctr169:
				{
#line 272 "args/argcheck2.rl"
					rl_sign = 1; }
				
#line 3849 "args/argcheck2.cpp"
				
				{
#line 266 "args/argcheck2.rl"
					rl_num = 0; }
				
#line 3855 "args/argcheck2.cpp"
				
				{
#line 266 "args/argcheck2.rl"
					(rl_num *= 10) += ((( (*( p)))) - '0'); }
				
#line 3861 "args/argcheck2.cpp"
				
				goto _st60;
				_ctr171:
				{
#line 266 "args/argcheck2.rl"
					rl_num = 0; }
				
#line 3869 "args/argcheck2.cpp"
				
				{
#line 266 "args/argcheck2.rl"
					(rl_num *= 10) += ((( (*( p)))) - '0'); }
				
#line 3875 "args/argcheck2.cpp"
				
				goto _st60;
				_ctr174:
				{
#line 266 "args/argcheck2.rl"
					(rl_num *= 10) += ((( (*( p)))) - '0'); }
				
#line 3883 "args/argcheck2.cpp"
				
				goto _st60;
				_st60:
				p += 1;
				st_case_60:
				switch( ( (*( p))) ) {
					case 41: {
						goto _ctr173;
					}
					case 93: {
						goto _ctr175;
					}
				}
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr174;
				}
				{
					goto _st0;
				}
				_ctr173:
				{
#line 232 "args/argcheck2.rl"
					
					rl_chk.values.push_back((int64_t)(rl_sign * rl_num));
				}
				
#line 3910 "args/argcheck2.cpp"
				
				goto _st61;
				_st61:
				p += 1;
				st_case_61:
				switch( ( (*( p))) ) {
					case 0: {
						goto _ctr177;
					}
					case 32: {
						goto _ctr178;
					}
					case 42: {
						goto _ctr179;
					}
					case 43: {
						goto _ctr180;
					}
					case 63: {
						goto _ctr181;
					}
					case 123: {
						goto _ctr182;
					}
				}
				{
					goto _st0;
				}
				_ctr175:
				{
#line 232 "args/argcheck2.rl"
					
					rl_chk.values.push_back((int64_t)(rl_sign * rl_num));
				}
				
#line 3946 "args/argcheck2.cpp"
				
				goto _st62;
				_st62:
				p += 1;
				st_case_62:
				switch( ( (*( p))) ) {
					case 0: {
						goto _ctr184;
					}
					case 32: {
						goto _ctr185;
					}
					case 42: {
						goto _ctr186;
					}
					case 43: {
						goto _ctr187;
					}
					case 63: {
						goto _ctr188;
					}
					case 123: {
						goto _ctr189;
					}
				}
				{
					goto _st0;
				}
				_st63:
				p += 1;
				st_case_63:
				if ( ( (*( p))) == 50 ) {
					goto _ctr190;
				}
				{
					goto _st0;
				}
				_ctr8:
				{
#line 363 "args/argcheck2.rl"
					rl_chk.type = CHECK_SYMBOL; }
				
#line 3989 "args/argcheck2.cpp"
				
				goto _st64;
				_st64:
				p += 1;
				st_case_64:
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
						goto _ctr192;
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
				_ctr192:
				{
#line 325 "args/argcheck2.rl"
					rl_chk.cmp = CMP_EQUAL; }
				
#line 4026 "args/argcheck2.cpp"
				
				goto _st65;
				_ctr202:
				{
#line 244 "args/argcheck2.rl"
					
					try {
						ArgString str{ {}, 0 };
						str.first.assign(rl_sym_start, p - rl_sym_start);
						str.second = crc32_hash(str.first.data());
						rl_chk.values.push_back(str);
					} catch(std::exception& e) {
						LIB_ERR << "exception: " << e.what();
					}
				}
				
#line 4043 "args/argcheck2.cpp"
				
				goto _st65;
				_st65:
				p += 1;
				st_case_65:
				switch( ( (*( p))) ) {
					case 35: {
						goto _ctr194;
					}
					case 45: {
						goto _ctr194;
					}
					case 95: {
						goto _ctr194;
					}
				}
				if ( ( (*( p))) < 64 ) {
					if ( 48 <= ( (*( p))) && ( (*( p))) <= 58 ) {
						goto _ctr194;
					}
				} else if ( ( (*( p))) > 90 ) {
					if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
						goto _ctr194;
					}
				} else {
					goto _ctr194;
				}
				{
					goto _st0;
				}
				_ctr194:
				{
#line 318 "args/argcheck2.rl"
					rl_sym_start = p; }
				
#line 4079 "args/argcheck2.cpp"
				
				goto _st66;
				_st66:
				p += 1;
				st_case_66:
				switch( ( (*( p))) ) {
					case 0: {
						goto _ctr196;
					}
					case 32: {
						goto _ctr197;
					}
					case 35: {
						goto _st66;
					}
					case 42: {
						goto _ctr198;
					}
					case 43: {
						goto _ctr199;
					}
					case 45: {
						goto _st66;
					}
					case 63: {
						goto _ctr200;
					}
					case 95: {
						goto _st66;
					}
					case 123: {
						goto _ctr201;
					}
					case 124: {
						goto _ctr202;
					}
				}
				if ( ( (*( p))) < 64 ) {
					if ( 48 <= ( (*( p))) && ( (*( p))) <= 58 ) {
						goto _st66;
					}
				} else if ( ( (*( p))) > 90 ) {
					if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
						goto _st66;
					}
				} else {
					goto _st66;
				}
				{
					goto _st0;
				}
				_ctr3:
				{
#line 371 "args/argcheck2.rl"
					rl_chk.name.clear(); }
				
#line 4136 "args/argcheck2.cpp"
				
				{
#line 371 "args/argcheck2.rl"
					rl_chk.name.push_back((( (*( p))))); }
				
#line 4142 "args/argcheck2.cpp"
				
				{
#line 360 "args/argcheck2.rl"
					rl_chk.type = CHECK_BOOL; }
				
#line 4148 "args/argcheck2.cpp"
				
				goto _st67;
				_st67:
				p += 1;
				st_case_67:
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
						goto _ctr204;
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
				_ctr204:
				{
#line 371 "args/argcheck2.rl"
					rl_chk.name.push_back((( (*( p))))); }
				
#line 4195 "args/argcheck2.cpp"
				
				{
#line 380 "args/argcheck2.rl"
					rl_chk.rmin = 1; rl_chk.rmax = 1; }
				
#line 4201 "args/argcheck2.cpp"
				
				{
#line 365 "args/argcheck2.rl"
					rl_chk.setRepeats(0, 1); }
				
#line 4207 "args/argcheck2.cpp"
				
				goto _st68;
				_st68:
				p += 1;
				st_case_68:
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
				_test_eof69: cs = 69; goto _test_eof; 
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
				
				_test_eof: {}
				if ( cs >= 69 )
					goto _out; _pop: {}
				_out: {}
			}
			
#line 796 "args/argcheck2.rl"
			
			
			if (cs < 
#line 4318 "args/argcheck2.cpp"
			69
#line 798 "args/argcheck2.rl"
			) {
				LIB_ERR << fmt::format("invalid format string: '{}'", str);
				chk_.reset();
			}
		}
		
		void ArgChecker::usage(BaseObject* obj, t_symbol* m)
		{
			if (!chk_)
				return;
			
			Error err(obj);
			if (m)
				err << '[' << m->s_name << "( ";
			
			err << chk_->help();
		}
		
		bool check_args(const char* arg_string, const AtomListView& lv, BaseObject* obj)
		{
			ArgChecker chk(arg_string);
			return chk.check(lv, obj);
		}
		
	}
}
