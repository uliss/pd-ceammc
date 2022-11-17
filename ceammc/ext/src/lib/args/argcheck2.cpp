#line 1 "args/argcheck2.rl"
# include "argcheck2.h"
# include "ceammc_string.h"
# include "ceammc_object.h"
# include "ceammc_crc32.h"
# include "fmt/core.h"
# include "ceammc_datatypes.h"

# include <cstdint>
# include <iostream>
# include <limits>
# include <cmath>
# include <algorithm>
# include <boost/container/static_vector.hpp>
# include <boost/variant.hpp>

// #define ARG_DEBUG

#ifdef ARG_DEBUG
#define debug(prefix, arg) std::cerr << " - " << prefix << ": " << arg << std::endl;
#else
#define debug(prefix, arg)
#endif

// keep in sync with typeNames values!
enum CheckType : int8_t {
	CHECK_NONE,
	CHECK_ATOM,
	CHECK_BOOL,
	CHECK_BYTE,
	CHECK_INT,
	CHECK_FLOAT,
	CHECK_SYMBOL,
	CHECK_TIME,
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

namespace {
	constexpr int16_t REPEAT_INF = -1;
	
	// keep in sync with CheckType values!
	const char* typeNames[] = {
		"",
		"atom",
		"bool",
		"byte",
		"int",
		"float",
		"symbol",
		"time",
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
	
	
	using ArgString = std::pair<ceammc::BoostStaticString<14>, uint32_t>;
	using ArgName = ceammc::BoostStaticString<10>;
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
}

struct Check {
	ArgList values;
	ArgName name;
	CheckType type { CHECK_NONE };
	CompareType cmp { CMP_NONE };
	int8_t rmin { 0 };
	int8_t rmax { 0 };
	
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


#line 226 "args/argcheck2.cpp"
static const int time_check_start = 1;
static const int time_check_first_final = 13;
static const int time_check_error = 0;

static const int time_check_en_main = 1;


#line 232 "args/argcheck2.rl"


namespace {
	
	bool check_time_unit(t_symbol* sym)
	{
		if (!sym) return false;
			
		int cs = 0;
		const char* p = sym->s_name;
		
		
#line 247 "args/argcheck2.cpp"
		{
			cs = (int)time_check_start;
		}
		
#line 243 "args/argcheck2.rl"
		
		
#line 255 "args/argcheck2.cpp"
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
				case 13:
				goto st_case_13;
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
			}
			p+= 1;
			st_case_1:
			if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
				goto _st2;
			}
			goto _st0;
			_st0:
			st_case_0:
			goto _out0;
			_st2:
			p+= 1;
			st_case_2:
			switch( ( (*( p))) ) {
				case 46: {
					goto _st3;
				}
				case 95: {
					goto _st5;
				}
				case 109: {
					goto _st6;
				}
				case 115: {
					goto _st10;
				}
			}
			if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
				goto _st2;
			}
			goto _st0;
			_st3:
			p+= 1;
			st_case_3:
			if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
				goto _st4;
			}
			goto _st0;
			_st4:
			p+= 1;
			st_case_4:
			switch( ( (*( p))) ) {
				case 95: {
					goto _st5;
				}
				case 109: {
					goto _st6;
				}
				case 115: {
					goto _st10;
				}
			}
			if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
				goto _st4;
			}
			goto _st0;
			_st5:
			p+= 1;
			st_case_5:
			switch( ( (*( p))) ) {
				case 109: {
					goto _st6;
				}
				case 115: {
					goto _st10;
				}
			}
			goto _st0;
			_st6:
			p+= 1;
			st_case_6:
			if ( ( (*( p))) == 115 ) {
				goto _st7;
			}
			goto _st0;
			_st7:
			p+= 1;
			st_case_7:
			switch( ( (*( p))) ) {
				case 0: {
					goto _ctr9;
				}
				case 101: {
					goto _st8;
				}
			}
			goto _st0;
			_ctr9:
			{
#line 229 "args/argcheck2.rl"
				{p+= 1; cs = 13; goto _out;} }
			
#line 378 "args/argcheck2.cpp"
			
			goto _st13;
			_st13:
			p+= 1;
			st_case_13:
			goto _st0;
			_st8:
			p+= 1;
			st_case_8:
			if ( ( (*( p))) == 99 ) {
				goto _st9;
			}
			goto _st0;
			_st9:
			p+= 1;
			st_case_9:
			if ( ( (*( p))) == 0 ) {
				goto _ctr9;
			}
			goto _st0;
			_st10:
			p+= 1;
			st_case_10:
			switch( ( (*( p))) ) {
				case 0: {
					goto _ctr9;
				}
				case 97: {
					goto _st11;
				}
				case 101: {
					goto _st8;
				}
			}
			goto _st0;
			_st11:
			p+= 1;
			st_case_11:
			if ( ( (*( p))) == 109 ) {
				goto _st12;
			}
			goto _st0;
			_st12:
			p+= 1;
			st_case_12:
			if ( ( (*( p))) == 112 ) {
				goto _st9;
			}
			goto _st0;
			_out1: cs = 1; goto _out; 
			_out0: cs = 0; goto _out; 
			_out2: cs = 2; goto _out; 
			_out3: cs = 3; goto _out; 
			_out4: cs = 4; goto _out; 
			_out5: cs = 5; goto _out; 
			_out6: cs = 6; goto _out; 
			_out7: cs = 7; goto _out; 
			_out13: cs = 13; goto _out; 
			_out8: cs = 8; goto _out; 
			_out9: cs = 9; goto _out; 
			_out10: cs = 10; goto _out; 
			_out11: cs = 11; goto _out; 
			_out12: cs = 12; goto _out; 
			_out: {}
		}
		
#line 244 "args/argcheck2.rl"
		
		
		return (cs >= 
#line 449 "args/argcheck2.cpp"
		13
#line 246 "args/argcheck2.rl"
		);
	}
	
}


#line 458 "args/argcheck2.cpp"
static const int arg_check2_start = 1;
static const int arg_check2_first_final = 69;
static const int arg_check2_error = 0;

static const int arg_check2_en_main = 1;


#line 424 "args/argcheck2.rl"


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
					case CHECK_TIME:
					if (a.isFloat()) {
						auto f = a.asT<t_float>();
						if (f < 0) {
							if (pErr)
								pdError(x, fmt::format("invalid {} value at [{}]: '{}', expected to be >= 0", c.argName(), i, f));
							return false;
						}
					} else if (a.isSymbol()) {
						if (!check_time_unit(a.asT<t_symbol*>())) {
							if (pErr)
								pdError(x, fmt::format("invalid {} value at [{}]: '{}', time value expected", c.argName(), i, atom_to_string(a)));
							return false;
						}
					} else {
						if (pErr)
							pdError(x, fmt::format("invalid {} value type at [{}]: '{}', expected float or symbol", c.argName(), i, atom_to_string(a)));
						return false;
					}
					debug("time", "Ok");
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
		
		bool ArgChecker::check(const AtomListView& lv, BaseObject* obj, ArgMatchList* matches) const
		{
			if (!chk_)
				return false;
			
			ArgMatchList m;
			const void* x = obj ? obj->owner() : nullptr;
			
			const int N = lv.size();
			int atom_idx = 0;
			
			for (int i = 0; i < chk_->size(); i++) {
				auto& check = (*chk_)[i];
				auto cur = atom_idx + (check.rmin - 1);
				auto match_pos = atom_idx;
				
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
				
				m.push_back(lv.subView(match_pos, atom_idx - match_pos));
			}
			
			if (atom_idx < N) {
				pdError(x, fmt::format("extra arguments left, starting from [{}]: {}", atom_idx, list_to_string(lv.subView(atom_idx))));
				return false;
			}
			
			if (matches)
				*matches = m;
			
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
			
			
#line 897 "args/argcheck2.cpp"
			{
				cs = (int)arg_check2_start;
			}
			
#line 902 "args/argcheck2.cpp"
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
				_ctr29:
				{
#line 415 "args/argcheck2.rl"
					rl_chk.rmin = 1; rl_chk.rmax = 1; }
				
#line 1051 "args/argcheck2.cpp"
				
				{
#line 254 "args/argcheck2.rl"
					
					if (chk_)
					chk_->push_back(rl_chk);
					
					rl_num = 0;
					rl_den = 0;
					rl_sign = 0;
					rl_den_cnt = 0;
					rl_chk = {};
				}
				
#line 1066 "args/argcheck2.cpp"
				
				goto _st1;
				_ctr36:
				{
#line 254 "args/argcheck2.rl"
					
					if (chk_)
					chk_->push_back(rl_chk);
					
					rl_num = 0;
					rl_den = 0;
					rl_sign = 0;
					rl_den_cnt = 0;
					rl_chk = {};
				}
				
#line 1083 "args/argcheck2.cpp"
				
				goto _st1;
				_ctr66:
				{
#line 269 "args/argcheck2.rl"
					
					double real = rl_num;
					if (rl_den_cnt)
					real += rl_den / double(rl_den_cnt);
					
					rl_chk.values.push_back(rl_sign * real);
				}
				
#line 1097 "args/argcheck2.cpp"
				
				{
#line 415 "args/argcheck2.rl"
					rl_chk.rmin = 1; rl_chk.rmax = 1; }
				
#line 1103 "args/argcheck2.cpp"
				
				{
#line 254 "args/argcheck2.rl"
					
					if (chk_)
					chk_->push_back(rl_chk);
					
					rl_num = 0;
					rl_den = 0;
					rl_sign = 0;
					rl_den_cnt = 0;
					rl_chk = {};
				}
				
#line 1118 "args/argcheck2.cpp"
				
				goto _st1;
				_ctr136:
				{
#line 265 "args/argcheck2.rl"
					
					rl_chk.values.push_back((int64_t)(rl_sign * rl_num));
				}
				
#line 1128 "args/argcheck2.cpp"
				
				{
#line 415 "args/argcheck2.rl"
					rl_chk.rmin = 1; rl_chk.rmax = 1; }
				
#line 1134 "args/argcheck2.cpp"
				
				{
#line 254 "args/argcheck2.rl"
					
					if (chk_)
					chk_->push_back(rl_chk);
					
					rl_num = 0;
					rl_den = 0;
					rl_sign = 0;
					rl_den_cnt = 0;
					rl_chk = {};
				}
				
#line 1149 "args/argcheck2.cpp"
				
				goto _st1;
				_ctr179:
				{
#line 317 "args/argcheck2.rl"
					rl_chk.cmp = CMP_RANGE_SEMIOPEN; }
				
#line 1157 "args/argcheck2.cpp"
				
				{
#line 415 "args/argcheck2.rl"
					rl_chk.rmin = 1; rl_chk.rmax = 1; }
				
#line 1163 "args/argcheck2.cpp"
				
				{
#line 254 "args/argcheck2.rl"
					
					if (chk_)
					chk_->push_back(rl_chk);
					
					rl_num = 0;
					rl_den = 0;
					rl_sign = 0;
					rl_den_cnt = 0;
					rl_chk = {};
				}
				
#line 1178 "args/argcheck2.cpp"
				
				goto _st1;
				_ctr186:
				{
#line 317 "args/argcheck2.rl"
					rl_chk.cmp = CMP_RANGE_CLOSED; }
				
#line 1186 "args/argcheck2.cpp"
				
				{
#line 415 "args/argcheck2.rl"
					rl_chk.rmin = 1; rl_chk.rmax = 1; }
				
#line 1192 "args/argcheck2.cpp"
				
				{
#line 254 "args/argcheck2.rl"
					
					if (chk_)
					chk_->push_back(rl_chk);
					
					rl_num = 0;
					rl_den = 0;
					rl_sign = 0;
					rl_den_cnt = 0;
					rl_chk = {};
				}
				
#line 1207 "args/argcheck2.cpp"
				
				goto _st1;
				_ctr198:
				{
#line 277 "args/argcheck2.rl"
					
					try {
						ArgString str{ {}, 0 };
						str.first.assign(rl_sym_start, p - rl_sym_start);
						str.second = crc32_hash(str.first.data());
						rl_chk.values.push_back(str);
					} catch(std::exception& e) {
						LIB_ERR << "exception: " << e.what();
					}
				}
				
#line 1224 "args/argcheck2.cpp"
				
				{
#line 415 "args/argcheck2.rl"
					rl_chk.rmin = 1; rl_chk.rmax = 1; }
				
#line 1230 "args/argcheck2.cpp"
				
				{
#line 254 "args/argcheck2.rl"
					
					if (chk_)
					chk_->push_back(rl_chk);
					
					rl_num = 0;
					rl_den = 0;
					rl_sign = 0;
					rl_den_cnt = 0;
					rl_chk = {};
				}
				
#line 1245 "args/argcheck2.cpp"
				
				goto _st1;
				_st1:
				p+= 1;
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
					case 116: {
						goto _ctr9;
					}
				}
				if ( ( (*( p))) > 57 ) {
					if ( 63 <= ( (*( p))) && ( (*( p))) <= 90 ) {
						goto _ctr2;
					}
				} else if ( ( (*( p))) >= 48 ) {
					goto _ctr2;
				}
				goto _st0;
				_st0:
				st_case_0:
				goto _out0;
				_ctr2:
				{
#line 405 "args/argcheck2.rl"
					rl_chk.name.clear(); }
				
#line 1293 "args/argcheck2.cpp"
				
				{
#line 405 "args/argcheck2.rl"
					rl_chk.name.push_back((( (*( p))))); }
				
#line 1299 "args/argcheck2.cpp"
				
				goto _st2;
				_st2:
				p+= 1;
				st_case_2:
				switch( ( (*( p))) ) {
					case 58: {
						goto _st10;
					}
					case 95: {
						goto _ctr11;
					}
				}
				if ( ( (*( p))) > 57 ) {
					if ( 63 <= ( (*( p))) && ( (*( p))) <= 90 ) {
						goto _ctr11;
					}
				} else if ( ( (*( p))) >= 48 ) {
					goto _ctr11;
				}
				goto _st0;
				_ctr11:
				{
#line 405 "args/argcheck2.rl"
					rl_chk.name.push_back((( (*( p))))); }
				
#line 1326 "args/argcheck2.cpp"
				
				goto _st3;
				_st3:
				p+= 1;
				st_case_3:
				switch( ( (*( p))) ) {
					case 58: {
						goto _st10;
					}
					case 95: {
						goto _ctr14;
					}
				}
				if ( ( (*( p))) > 57 ) {
					if ( 63 <= ( (*( p))) && ( (*( p))) <= 90 ) {
						goto _ctr14;
					}
				} else if ( ( (*( p))) >= 48 ) {
					goto _ctr14;
				}
				goto _st0;
				_ctr14:
				{
#line 405 "args/argcheck2.rl"
					rl_chk.name.push_back((( (*( p))))); }
				
#line 1353 "args/argcheck2.cpp"
				
				goto _st4;
				_st4:
				p+= 1;
				st_case_4:
				switch( ( (*( p))) ) {
					case 58: {
						goto _st10;
					}
					case 95: {
						goto _ctr16;
					}
				}
				if ( ( (*( p))) > 57 ) {
					if ( 63 <= ( (*( p))) && ( (*( p))) <= 90 ) {
						goto _ctr16;
					}
				} else if ( ( (*( p))) >= 48 ) {
					goto _ctr16;
				}
				goto _st0;
				_ctr16:
				{
#line 405 "args/argcheck2.rl"
					rl_chk.name.push_back((( (*( p))))); }
				
#line 1380 "args/argcheck2.cpp"
				
				goto _st5;
				_st5:
				p+= 1;
				st_case_5:
				switch( ( (*( p))) ) {
					case 58: {
						goto _st10;
					}
					case 95: {
						goto _ctr18;
					}
				}
				if ( ( (*( p))) > 57 ) {
					if ( 63 <= ( (*( p))) && ( (*( p))) <= 90 ) {
						goto _ctr18;
					}
				} else if ( ( (*( p))) >= 48 ) {
					goto _ctr18;
				}
				goto _st0;
				_ctr18:
				{
#line 405 "args/argcheck2.rl"
					rl_chk.name.push_back((( (*( p))))); }
				
#line 1407 "args/argcheck2.cpp"
				
				goto _st6;
				_st6:
				p+= 1;
				st_case_6:
				switch( ( (*( p))) ) {
					case 58: {
						goto _st10;
					}
					case 95: {
						goto _ctr20;
					}
				}
				if ( ( (*( p))) > 57 ) {
					if ( 63 <= ( (*( p))) && ( (*( p))) <= 90 ) {
						goto _ctr20;
					}
				} else if ( ( (*( p))) >= 48 ) {
					goto _ctr20;
				}
				goto _st0;
				_ctr20:
				{
#line 405 "args/argcheck2.rl"
					rl_chk.name.push_back((( (*( p))))); }
				
#line 1434 "args/argcheck2.cpp"
				
				goto _st7;
				_st7:
				p+= 1;
				st_case_7:
				switch( ( (*( p))) ) {
					case 58: {
						goto _st10;
					}
					case 95: {
						goto _ctr22;
					}
				}
				if ( ( (*( p))) > 57 ) {
					if ( 63 <= ( (*( p))) && ( (*( p))) <= 90 ) {
						goto _ctr22;
					}
				} else if ( ( (*( p))) >= 48 ) {
					goto _ctr22;
				}
				goto _st0;
				_ctr22:
				{
#line 405 "args/argcheck2.rl"
					rl_chk.name.push_back((( (*( p))))); }
				
#line 1461 "args/argcheck2.cpp"
				
				goto _st8;
				_st8:
				p+= 1;
				st_case_8:
				switch( ( (*( p))) ) {
					case 58: {
						goto _st10;
					}
					case 95: {
						goto _ctr24;
					}
				}
				if ( ( (*( p))) > 57 ) {
					if ( 63 <= ( (*( p))) && ( (*( p))) <= 90 ) {
						goto _ctr24;
					}
				} else if ( ( (*( p))) >= 48 ) {
					goto _ctr24;
				}
				goto _st0;
				_ctr24:
				{
#line 405 "args/argcheck2.rl"
					rl_chk.name.push_back((( (*( p))))); }
				
#line 1488 "args/argcheck2.cpp"
				
				goto _st9;
				_st9:
				p+= 1;
				st_case_9:
				if ( ( (*( p))) == 58 ) {
					goto _st10;
				}
				goto _st0;
				_st10:
				p+= 1;
				st_case_10:
				switch( ( (*( p))) ) {
					case 66: {
						goto _ctr26;
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
					case 116: {
						goto _ctr9;
					}
				}
				goto _st0;
				_ctr4:
				{
#line 392 "args/argcheck2.rl"
					rl_chk.type = CHECK_ATOM; }
				
#line 1530 "args/argcheck2.cpp"
				
				goto _st11;
				_ctr5:
				{
#line 394 "args/argcheck2.rl"
					rl_chk.type = CHECK_BYTE; }
				
#line 1538 "args/argcheck2.cpp"
				
				goto _st11;
				_ctr9:
				{
#line 398 "args/argcheck2.rl"
					rl_chk.type = CHECK_TIME; }
				
#line 1546 "args/argcheck2.cpp"
				
				goto _st11;
				_ctr26:
				{
#line 393 "args/argcheck2.rl"
					rl_chk.type = CHECK_BOOL; }
				
#line 1554 "args/argcheck2.cpp"
				
				goto _st11;
				_ctr109:
				{
#line 269 "args/argcheck2.rl"
					
					double real = rl_num;
					if (rl_den_cnt)
					real += rl_den / double(rl_den_cnt);
					
					rl_chk.values.push_back(rl_sign * real);
				}
				
#line 1568 "args/argcheck2.cpp"
				
				{
#line 367 "args/argcheck2.rl"
					rl_chk.cmp = CMP_RANGE_SEMIOPEN; }
				
#line 1574 "args/argcheck2.cpp"
				
				goto _st11;
				_ctr112:
				{
#line 269 "args/argcheck2.rl"
					
					double real = rl_num;
					if (rl_den_cnt)
					real += rl_den / double(rl_den_cnt);
					
					rl_chk.values.push_back(rl_sign * real);
				}
				
#line 1588 "args/argcheck2.cpp"
				
				{
#line 367 "args/argcheck2.rl"
					rl_chk.cmp = CMP_RANGE_CLOSED; }
				
#line 1594 "args/argcheck2.cpp"
				
				goto _st11;
				_ctr191:
				{
#line 342 "args/argcheck2.rl"
					rl_chk.cmp = CMP_POWER2; }
				
#line 1602 "args/argcheck2.cpp"
				
				goto _st11;
				_st11:
				p+= 1;
				st_case_11:
				switch( ( (*( p))) ) {
					case 0: {
						goto _ctr28;
					}
					case 32: {
						goto _ctr29;
					}
					case 42: {
						goto _ctr30;
					}
					case 43: {
						goto _ctr31;
					}
					case 63: {
						goto _ctr32;
					}
					case 123: {
						goto _ctr33;
					}
				}
				goto _st0;
				_ctr28:
				{
#line 415 "args/argcheck2.rl"
					rl_chk.rmin = 1; rl_chk.rmax = 1; }
				
#line 1634 "args/argcheck2.cpp"
				
				{
#line 254 "args/argcheck2.rl"
					
					if (chk_)
					chk_->push_back(rl_chk);
					
					rl_num = 0;
					rl_den = 0;
					rl_sign = 0;
					rl_den_cnt = 0;
					rl_chk = {};
				}
				
#line 1649 "args/argcheck2.cpp"
				
				{
#line 420 "args/argcheck2.rl"
					{p+= 1; cs = 69; goto _out;} }
				
#line 1655 "args/argcheck2.cpp"
				
				goto _st69;
				_ctr35:
				{
#line 254 "args/argcheck2.rl"
					
					if (chk_)
					chk_->push_back(rl_chk);
					
					rl_num = 0;
					rl_den = 0;
					rl_sign = 0;
					rl_den_cnt = 0;
					rl_chk = {};
				}
				
#line 1672 "args/argcheck2.cpp"
				
				{
#line 420 "args/argcheck2.rl"
					{p+= 1; cs = 69; goto _out;} }
				
#line 1678 "args/argcheck2.cpp"
				
				goto _st69;
				_ctr65:
				{
#line 269 "args/argcheck2.rl"
					
					double real = rl_num;
					if (rl_den_cnt)
					real += rl_den / double(rl_den_cnt);
					
					rl_chk.values.push_back(rl_sign * real);
				}
				
#line 1692 "args/argcheck2.cpp"
				
				{
#line 415 "args/argcheck2.rl"
					rl_chk.rmin = 1; rl_chk.rmax = 1; }
				
#line 1698 "args/argcheck2.cpp"
				
				{
#line 254 "args/argcheck2.rl"
					
					if (chk_)
					chk_->push_back(rl_chk);
					
					rl_num = 0;
					rl_den = 0;
					rl_sign = 0;
					rl_den_cnt = 0;
					rl_chk = {};
				}
				
#line 1713 "args/argcheck2.cpp"
				
				{
#line 420 "args/argcheck2.rl"
					{p+= 1; cs = 69; goto _out;} }
				
#line 1719 "args/argcheck2.cpp"
				
				goto _st69;
				_ctr135:
				{
#line 265 "args/argcheck2.rl"
					
					rl_chk.values.push_back((int64_t)(rl_sign * rl_num));
				}
				
#line 1729 "args/argcheck2.cpp"
				
				{
#line 415 "args/argcheck2.rl"
					rl_chk.rmin = 1; rl_chk.rmax = 1; }
				
#line 1735 "args/argcheck2.cpp"
				
				{
#line 254 "args/argcheck2.rl"
					
					if (chk_)
					chk_->push_back(rl_chk);
					
					rl_num = 0;
					rl_den = 0;
					rl_sign = 0;
					rl_den_cnt = 0;
					rl_chk = {};
				}
				
#line 1750 "args/argcheck2.cpp"
				
				{
#line 420 "args/argcheck2.rl"
					{p+= 1; cs = 69; goto _out;} }
				
#line 1756 "args/argcheck2.cpp"
				
				goto _st69;
				_ctr178:
				{
#line 317 "args/argcheck2.rl"
					rl_chk.cmp = CMP_RANGE_SEMIOPEN; }
				
#line 1764 "args/argcheck2.cpp"
				
				{
#line 415 "args/argcheck2.rl"
					rl_chk.rmin = 1; rl_chk.rmax = 1; }
				
#line 1770 "args/argcheck2.cpp"
				
				{
#line 254 "args/argcheck2.rl"
					
					if (chk_)
					chk_->push_back(rl_chk);
					
					rl_num = 0;
					rl_den = 0;
					rl_sign = 0;
					rl_den_cnt = 0;
					rl_chk = {};
				}
				
#line 1785 "args/argcheck2.cpp"
				
				{
#line 420 "args/argcheck2.rl"
					{p+= 1; cs = 69; goto _out;} }
				
#line 1791 "args/argcheck2.cpp"
				
				goto _st69;
				_ctr185:
				{
#line 317 "args/argcheck2.rl"
					rl_chk.cmp = CMP_RANGE_CLOSED; }
				
#line 1799 "args/argcheck2.cpp"
				
				{
#line 415 "args/argcheck2.rl"
					rl_chk.rmin = 1; rl_chk.rmax = 1; }
				
#line 1805 "args/argcheck2.cpp"
				
				{
#line 254 "args/argcheck2.rl"
					
					if (chk_)
					chk_->push_back(rl_chk);
					
					rl_num = 0;
					rl_den = 0;
					rl_sign = 0;
					rl_den_cnt = 0;
					rl_chk = {};
				}
				
#line 1820 "args/argcheck2.cpp"
				
				{
#line 420 "args/argcheck2.rl"
					{p+= 1; cs = 69; goto _out;} }
				
#line 1826 "args/argcheck2.cpp"
				
				goto _st69;
				_ctr197:
				{
#line 277 "args/argcheck2.rl"
					
					try {
						ArgString str{ {}, 0 };
						str.first.assign(rl_sym_start, p - rl_sym_start);
						str.second = crc32_hash(str.first.data());
						rl_chk.values.push_back(str);
					} catch(std::exception& e) {
						LIB_ERR << "exception: " << e.what();
					}
				}
				
#line 1843 "args/argcheck2.cpp"
				
				{
#line 415 "args/argcheck2.rl"
					rl_chk.rmin = 1; rl_chk.rmax = 1; }
				
#line 1849 "args/argcheck2.cpp"
				
				{
#line 254 "args/argcheck2.rl"
					
					if (chk_)
					chk_->push_back(rl_chk);
					
					rl_num = 0;
					rl_den = 0;
					rl_sign = 0;
					rl_den_cnt = 0;
					rl_chk = {};
				}
				
#line 1864 "args/argcheck2.cpp"
				
				{
#line 420 "args/argcheck2.rl"
					{p+= 1; cs = 69; goto _out;} }
				
#line 1870 "args/argcheck2.cpp"
				
				goto _st69;
				_st69:
				p+= 1;
				st_case_69:
				goto _st0;
				_ctr30:
				{
#line 415 "args/argcheck2.rl"
					rl_chk.rmin = 1; rl_chk.rmax = 1; }
				
#line 1882 "args/argcheck2.cpp"
				
				{
#line 401 "args/argcheck2.rl"
					rl_chk.setRepeats(0, REPEAT_INF); }
				
#line 1888 "args/argcheck2.cpp"
				
				goto _st12;
				_ctr31:
				{
#line 415 "args/argcheck2.rl"
					rl_chk.rmin = 1; rl_chk.rmax = 1; }
				
#line 1896 "args/argcheck2.cpp"
				
				{
#line 400 "args/argcheck2.rl"
					rl_chk.setRepeats(1, REPEAT_INF); }
				
#line 1902 "args/argcheck2.cpp"
				
				goto _st12;
				_ctr32:
				{
#line 415 "args/argcheck2.rl"
					rl_chk.rmin = 1; rl_chk.rmax = 1; }
				
#line 1910 "args/argcheck2.cpp"
				
				{
#line 399 "args/argcheck2.rl"
					rl_chk.setRepeats(0, 1); }
				
#line 1916 "args/argcheck2.cpp"
				
				goto _st12;
				_ctr42:
				{
#line 293 "args/argcheck2.rl"
					rl_chk.rmax = rl_chk.rmin; }
				
#line 1924 "args/argcheck2.cpp"
				
				goto _st12;
				_ctr67:
				{
#line 269 "args/argcheck2.rl"
					
					double real = rl_num;
					if (rl_den_cnt)
					real += rl_den / double(rl_den_cnt);
					
					rl_chk.values.push_back(rl_sign * real);
				}
				
#line 1938 "args/argcheck2.cpp"
				
				{
#line 415 "args/argcheck2.rl"
					rl_chk.rmin = 1; rl_chk.rmax = 1; }
				
#line 1944 "args/argcheck2.cpp"
				
				{
#line 401 "args/argcheck2.rl"
					rl_chk.setRepeats(0, REPEAT_INF); }
				
#line 1950 "args/argcheck2.cpp"
				
				goto _st12;
				_ctr68:
				{
#line 269 "args/argcheck2.rl"
					
					double real = rl_num;
					if (rl_den_cnt)
					real += rl_den / double(rl_den_cnt);
					
					rl_chk.values.push_back(rl_sign * real);
				}
				
#line 1964 "args/argcheck2.cpp"
				
				{
#line 415 "args/argcheck2.rl"
					rl_chk.rmin = 1; rl_chk.rmax = 1; }
				
#line 1970 "args/argcheck2.cpp"
				
				{
#line 400 "args/argcheck2.rl"
					rl_chk.setRepeats(1, REPEAT_INF); }
				
#line 1976 "args/argcheck2.cpp"
				
				goto _st12;
				_ctr71:
				{
#line 269 "args/argcheck2.rl"
					
					double real = rl_num;
					if (rl_den_cnt)
					real += rl_den / double(rl_den_cnt);
					
					rl_chk.values.push_back(rl_sign * real);
				}
				
#line 1990 "args/argcheck2.cpp"
				
				{
#line 415 "args/argcheck2.rl"
					rl_chk.rmin = 1; rl_chk.rmax = 1; }
				
#line 1996 "args/argcheck2.cpp"
				
				{
#line 399 "args/argcheck2.rl"
					rl_chk.setRepeats(0, 1); }
				
#line 2002 "args/argcheck2.cpp"
				
				goto _st12;
				_ctr137:
				{
#line 265 "args/argcheck2.rl"
					
					rl_chk.values.push_back((int64_t)(rl_sign * rl_num));
				}
				
#line 2012 "args/argcheck2.cpp"
				
				{
#line 415 "args/argcheck2.rl"
					rl_chk.rmin = 1; rl_chk.rmax = 1; }
				
#line 2018 "args/argcheck2.cpp"
				
				{
#line 401 "args/argcheck2.rl"
					rl_chk.setRepeats(0, REPEAT_INF); }
				
#line 2024 "args/argcheck2.cpp"
				
				goto _st12;
				_ctr138:
				{
#line 265 "args/argcheck2.rl"
					
					rl_chk.values.push_back((int64_t)(rl_sign * rl_num));
				}
				
#line 2034 "args/argcheck2.cpp"
				
				{
#line 415 "args/argcheck2.rl"
					rl_chk.rmin = 1; rl_chk.rmax = 1; }
				
#line 2040 "args/argcheck2.cpp"
				
				{
#line 400 "args/argcheck2.rl"
					rl_chk.setRepeats(1, REPEAT_INF); }
				
#line 2046 "args/argcheck2.cpp"
				
				goto _st12;
				_ctr140:
				{
#line 265 "args/argcheck2.rl"
					
					rl_chk.values.push_back((int64_t)(rl_sign * rl_num));
				}
				
#line 2056 "args/argcheck2.cpp"
				
				{
#line 415 "args/argcheck2.rl"
					rl_chk.rmin = 1; rl_chk.rmax = 1; }
				
#line 2062 "args/argcheck2.cpp"
				
				{
#line 399 "args/argcheck2.rl"
					rl_chk.setRepeats(0, 1); }
				
#line 2068 "args/argcheck2.cpp"
				
				goto _st12;
				_ctr180:
				{
#line 317 "args/argcheck2.rl"
					rl_chk.cmp = CMP_RANGE_SEMIOPEN; }
				
#line 2076 "args/argcheck2.cpp"
				
				{
#line 415 "args/argcheck2.rl"
					rl_chk.rmin = 1; rl_chk.rmax = 1; }
				
#line 2082 "args/argcheck2.cpp"
				
				{
#line 401 "args/argcheck2.rl"
					rl_chk.setRepeats(0, REPEAT_INF); }
				
#line 2088 "args/argcheck2.cpp"
				
				goto _st12;
				_ctr181:
				{
#line 317 "args/argcheck2.rl"
					rl_chk.cmp = CMP_RANGE_SEMIOPEN; }
				
#line 2096 "args/argcheck2.cpp"
				
				{
#line 415 "args/argcheck2.rl"
					rl_chk.rmin = 1; rl_chk.rmax = 1; }
				
#line 2102 "args/argcheck2.cpp"
				
				{
#line 400 "args/argcheck2.rl"
					rl_chk.setRepeats(1, REPEAT_INF); }
				
#line 2108 "args/argcheck2.cpp"
				
				goto _st12;
				_ctr182:
				{
#line 317 "args/argcheck2.rl"
					rl_chk.cmp = CMP_RANGE_SEMIOPEN; }
				
#line 2116 "args/argcheck2.cpp"
				
				{
#line 415 "args/argcheck2.rl"
					rl_chk.rmin = 1; rl_chk.rmax = 1; }
				
#line 2122 "args/argcheck2.cpp"
				
				{
#line 399 "args/argcheck2.rl"
					rl_chk.setRepeats(0, 1); }
				
#line 2128 "args/argcheck2.cpp"
				
				goto _st12;
				_ctr187:
				{
#line 317 "args/argcheck2.rl"
					rl_chk.cmp = CMP_RANGE_CLOSED; }
				
#line 2136 "args/argcheck2.cpp"
				
				{
#line 415 "args/argcheck2.rl"
					rl_chk.rmin = 1; rl_chk.rmax = 1; }
				
#line 2142 "args/argcheck2.cpp"
				
				{
#line 401 "args/argcheck2.rl"
					rl_chk.setRepeats(0, REPEAT_INF); }
				
#line 2148 "args/argcheck2.cpp"
				
				goto _st12;
				_ctr188:
				{
#line 317 "args/argcheck2.rl"
					rl_chk.cmp = CMP_RANGE_CLOSED; }
				
#line 2156 "args/argcheck2.cpp"
				
				{
#line 415 "args/argcheck2.rl"
					rl_chk.rmin = 1; rl_chk.rmax = 1; }
				
#line 2162 "args/argcheck2.cpp"
				
				{
#line 400 "args/argcheck2.rl"
					rl_chk.setRepeats(1, REPEAT_INF); }
				
#line 2168 "args/argcheck2.cpp"
				
				goto _st12;
				_ctr189:
				{
#line 317 "args/argcheck2.rl"
					rl_chk.cmp = CMP_RANGE_CLOSED; }
				
#line 2176 "args/argcheck2.cpp"
				
				{
#line 415 "args/argcheck2.rl"
					rl_chk.rmin = 1; rl_chk.rmax = 1; }
				
#line 2182 "args/argcheck2.cpp"
				
				{
#line 399 "args/argcheck2.rl"
					rl_chk.setRepeats(0, 1); }
				
#line 2188 "args/argcheck2.cpp"
				
				goto _st12;
				_ctr199:
				{
#line 277 "args/argcheck2.rl"
					
					try {
						ArgString str{ {}, 0 };
						str.first.assign(rl_sym_start, p - rl_sym_start);
						str.second = crc32_hash(str.first.data());
						rl_chk.values.push_back(str);
					} catch(std::exception& e) {
						LIB_ERR << "exception: " << e.what();
					}
				}
				
#line 2205 "args/argcheck2.cpp"
				
				{
#line 415 "args/argcheck2.rl"
					rl_chk.rmin = 1; rl_chk.rmax = 1; }
				
#line 2211 "args/argcheck2.cpp"
				
				{
#line 401 "args/argcheck2.rl"
					rl_chk.setRepeats(0, REPEAT_INF); }
				
#line 2217 "args/argcheck2.cpp"
				
				goto _st12;
				_ctr200:
				{
#line 277 "args/argcheck2.rl"
					
					try {
						ArgString str{ {}, 0 };
						str.first.assign(rl_sym_start, p - rl_sym_start);
						str.second = crc32_hash(str.first.data());
						rl_chk.values.push_back(str);
					} catch(std::exception& e) {
						LIB_ERR << "exception: " << e.what();
					}
				}
				
#line 2234 "args/argcheck2.cpp"
				
				{
#line 415 "args/argcheck2.rl"
					rl_chk.rmin = 1; rl_chk.rmax = 1; }
				
#line 2240 "args/argcheck2.cpp"
				
				{
#line 400 "args/argcheck2.rl"
					rl_chk.setRepeats(1, REPEAT_INF); }
				
#line 2246 "args/argcheck2.cpp"
				
				goto _st12;
				_ctr201:
				{
#line 277 "args/argcheck2.rl"
					
					try {
						ArgString str{ {}, 0 };
						str.first.assign(rl_sym_start, p - rl_sym_start);
						str.second = crc32_hash(str.first.data());
						rl_chk.values.push_back(str);
					} catch(std::exception& e) {
						LIB_ERR << "exception: " << e.what();
					}
				}
				
#line 2263 "args/argcheck2.cpp"
				
				{
#line 415 "args/argcheck2.rl"
					rl_chk.rmin = 1; rl_chk.rmax = 1; }
				
#line 2269 "args/argcheck2.cpp"
				
				{
#line 399 "args/argcheck2.rl"
					rl_chk.setRepeats(0, 1); }
				
#line 2275 "args/argcheck2.cpp"
				
				goto _st12;
				_st12:
				p+= 1;
				st_case_12:
				switch( ( (*( p))) ) {
					case 0: {
						goto _ctr35;
					}
					case 32: {
						goto _ctr36;
					}
				}
				goto _st0;
				_ctr33:
				{
#line 415 "args/argcheck2.rl"
					rl_chk.rmin = 1; rl_chk.rmax = 1; }
				
#line 2295 "args/argcheck2.cpp"
				
				{
#line 294 "args/argcheck2.rl"
					rl_chk.rmin = 0; rl_chk.rmax = REPEAT_INF; }
				
#line 2301 "args/argcheck2.cpp"
				
				goto _st13;
				_ctr72:
				{
#line 269 "args/argcheck2.rl"
					
					double real = rl_num;
					if (rl_den_cnt)
					real += rl_den / double(rl_den_cnt);
					
					rl_chk.values.push_back(rl_sign * real);
				}
				
#line 2315 "args/argcheck2.cpp"
				
				{
#line 415 "args/argcheck2.rl"
					rl_chk.rmin = 1; rl_chk.rmax = 1; }
				
#line 2321 "args/argcheck2.cpp"
				
				{
#line 294 "args/argcheck2.rl"
					rl_chk.rmin = 0; rl_chk.rmax = REPEAT_INF; }
				
#line 2327 "args/argcheck2.cpp"
				
				goto _st13;
				_ctr141:
				{
#line 265 "args/argcheck2.rl"
					
					rl_chk.values.push_back((int64_t)(rl_sign * rl_num));
				}
				
#line 2337 "args/argcheck2.cpp"
				
				{
#line 415 "args/argcheck2.rl"
					rl_chk.rmin = 1; rl_chk.rmax = 1; }
				
#line 2343 "args/argcheck2.cpp"
				
				{
#line 294 "args/argcheck2.rl"
					rl_chk.rmin = 0; rl_chk.rmax = REPEAT_INF; }
				
#line 2349 "args/argcheck2.cpp"
				
				goto _st13;
				_ctr183:
				{
#line 317 "args/argcheck2.rl"
					rl_chk.cmp = CMP_RANGE_SEMIOPEN; }
				
#line 2357 "args/argcheck2.cpp"
				
				{
#line 415 "args/argcheck2.rl"
					rl_chk.rmin = 1; rl_chk.rmax = 1; }
				
#line 2363 "args/argcheck2.cpp"
				
				{
#line 294 "args/argcheck2.rl"
					rl_chk.rmin = 0; rl_chk.rmax = REPEAT_INF; }
				
#line 2369 "args/argcheck2.cpp"
				
				goto _st13;
				_ctr190:
				{
#line 317 "args/argcheck2.rl"
					rl_chk.cmp = CMP_RANGE_CLOSED; }
				
#line 2377 "args/argcheck2.cpp"
				
				{
#line 415 "args/argcheck2.rl"
					rl_chk.rmin = 1; rl_chk.rmax = 1; }
				
#line 2383 "args/argcheck2.cpp"
				
				{
#line 294 "args/argcheck2.rl"
					rl_chk.rmin = 0; rl_chk.rmax = REPEAT_INF; }
				
#line 2389 "args/argcheck2.cpp"
				
				goto _st13;
				_ctr202:
				{
#line 277 "args/argcheck2.rl"
					
					try {
						ArgString str{ {}, 0 };
						str.first.assign(rl_sym_start, p - rl_sym_start);
						str.second = crc32_hash(str.first.data());
						rl_chk.values.push_back(str);
					} catch(std::exception& e) {
						LIB_ERR << "exception: " << e.what();
					}
				}
				
#line 2406 "args/argcheck2.cpp"
				
				{
#line 415 "args/argcheck2.rl"
					rl_chk.rmin = 1; rl_chk.rmax = 1; }
				
#line 2412 "args/argcheck2.cpp"
				
				{
#line 294 "args/argcheck2.rl"
					rl_chk.rmin = 0; rl_chk.rmax = REPEAT_INF; }
				
#line 2418 "args/argcheck2.cpp"
				
				goto _st13;
				_st13:
				p+= 1;
				st_case_13:
				if ( ( (*( p))) == 48 ) {
					goto _ctr38;
				}
				if ( 49 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr39;
				}
				goto _st0;
				_ctr38:
				{
#line 291 "args/argcheck2.rl"
					rl_chk.rmin = 0; }
				
#line 2436 "args/argcheck2.cpp"
				
				goto _st14;
				_st14:
				p+= 1;
				st_case_14:
				switch( ( (*( p))) ) {
					case 44: {
						goto _st15;
					}
					case 125: {
						goto _ctr42;
					}
				}
				goto _st0;
				_st15:
				p+= 1;
				st_case_15:
				switch( ( (*( p))) ) {
					case 48: {
						goto _ctr43;
					}
					case 125: {
						goto _st12;
					}
				}
				if ( 49 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr44;
				}
				goto _st0;
				_ctr43:
				{
#line 292 "args/argcheck2.rl"
					rl_chk.rmax = 0; }
				
#line 2471 "args/argcheck2.cpp"
				
				goto _st16;
				_st16:
				p+= 1;
				st_case_16:
				if ( ( (*( p))) == 125 ) {
					goto _st12;
				}
				goto _st0;
				_ctr44:
				{
#line 292 "args/argcheck2.rl"
					rl_chk.rmax = (( (*( p))))-'0'; }
				
#line 2486 "args/argcheck2.cpp"
				
				goto _st17;
				_ctr47:
				{
#line 292 "args/argcheck2.rl"
					(rl_chk.rmax *= 10) += ((( (*( p)))) - '0'); }
				
#line 2494 "args/argcheck2.cpp"
				
				goto _st17;
				_st17:
				p+= 1;
				st_case_17:
				if ( ( (*( p))) == 125 ) {
					goto _st12;
				}
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr47;
				}
				goto _st0;
				_ctr39:
				{
#line 291 "args/argcheck2.rl"
					rl_chk.rmin = (( (*( p))))-'0'; }
				
#line 2512 "args/argcheck2.cpp"
				
				goto _st18;
				_ctr49:
				{
#line 291 "args/argcheck2.rl"
					(rl_chk.rmin *= 10) += ((( (*( p)))) - '0'); }
				
#line 2520 "args/argcheck2.cpp"
				
				goto _st18;
				_st18:
				p+= 1;
				st_case_18:
				switch( ( (*( p))) ) {
					case 44: {
						goto _st15;
					}
					case 125: {
						goto _ctr42;
					}
				}
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr49;
				}
				goto _st0;
				_ctr6:
				{
#line 397 "args/argcheck2.rl"
					rl_chk.type = CHECK_FLOAT; }
				
#line 2543 "args/argcheck2.cpp"
				
				goto _st19;
				_st19:
				p+= 1;
				st_case_19:
				switch( ( (*( p))) ) {
					case 0: {
						goto _ctr28;
					}
					case 32: {
						goto _ctr29;
					}
					case 33: {
						goto _st20;
					}
					case 42: {
						goto _ctr30;
					}
					case 43: {
						goto _ctr31;
					}
					case 60: {
						goto _ctr52;
					}
					case 61: {
						goto _ctr53;
					}
					case 62: {
						goto _ctr54;
					}
					case 63: {
						goto _ctr32;
					}
					case 91: {
						goto _st33;
					}
					case 123: {
						goto _ctr33;
					}
					case 126: {
						goto _ctr56;
					}
				}
				goto _st0;
				_st20:
				p+= 1;
				st_case_20:
				if ( ( (*( p))) == 61 ) {
					goto _ctr57;
				}
				goto _st0;
				_ctr57:
				{
#line 330 "args/argcheck2.rl"
					rl_chk.cmp = CMP_NOT_EQUAL; }
				
#line 2600 "args/argcheck2.cpp"
				
				goto _st21;
				_ctr77:
				{
#line 329 "args/argcheck2.rl"
					rl_chk.cmp = CMP_LESS_EQ; }
				
#line 2608 "args/argcheck2.cpp"
				
				goto _st21;
				_ctr92:
				{
#line 328 "args/argcheck2.rl"
					rl_chk.cmp = CMP_GREATER_EQ; }
				
#line 2616 "args/argcheck2.cpp"
				
				goto _st21;
				_st21:
				p+= 1;
				st_case_21:
				switch( ( (*( p))) ) {
					case 43: {
						goto _ctr59;
					}
					case 45: {
						goto _ctr60;
					}
				}
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr61;
				}
				goto _st0;
				_ctr59:
				{
#line 305 "args/argcheck2.rl"
					rl_sign = 1; }
				
#line 2639 "args/argcheck2.cpp"
				
				{
#line 296 "args/argcheck2.rl"
					rl_sign = 1; }
				
#line 2645 "args/argcheck2.cpp"
				
				goto _st22;
				_ctr60:
				{
#line 305 "args/argcheck2.rl"
					rl_sign = 1; }
				
#line 2653 "args/argcheck2.cpp"
				
				{
#line 297 "args/argcheck2.rl"
					rl_sign = -1; }
				
#line 2659 "args/argcheck2.cpp"
				
				goto _st22;
				_st22:
				p+= 1;
				st_case_22:
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr63;
				}
				goto _st0;
				_ctr61:
				{
#line 305 "args/argcheck2.rl"
					rl_sign = 1; }
				
#line 2674 "args/argcheck2.cpp"
				
				{
#line 299 "args/argcheck2.rl"
					rl_num = 0; }
				
#line 2680 "args/argcheck2.cpp"
				
				{
#line 299 "args/argcheck2.rl"
					(rl_num *= 10) += ((( (*( p)))) - '0'); }
				
#line 2686 "args/argcheck2.cpp"
				
				goto _st23;
				_ctr63:
				{
#line 299 "args/argcheck2.rl"
					rl_num = 0; }
				
#line 2694 "args/argcheck2.cpp"
				
				{
#line 299 "args/argcheck2.rl"
					(rl_num *= 10) += ((( (*( p)))) - '0'); }
				
#line 2700 "args/argcheck2.cpp"
				
				goto _st23;
				_ctr70:
				{
#line 299 "args/argcheck2.rl"
					(rl_num *= 10) += ((( (*( p)))) - '0'); }
				
#line 2708 "args/argcheck2.cpp"
				
				goto _st23;
				_st23:
				p+= 1;
				st_case_23:
				switch( ( (*( p))) ) {
					case 0: {
						goto _ctr65;
					}
					case 32: {
						goto _ctr66;
					}
					case 42: {
						goto _ctr67;
					}
					case 43: {
						goto _ctr68;
					}
					case 46: {
						goto _st24;
					}
					case 63: {
						goto _ctr71;
					}
					case 123: {
						goto _ctr72;
					}
				}
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr70;
				}
				goto _st0;
				_st24:
				p+= 1;
				st_case_24:
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr73;
				}
				goto _st0;
				_ctr73:
				{
#line 300 "args/argcheck2.rl"
					rl_den = 0; rl_den_cnt = 1; }
				
#line 2753 "args/argcheck2.cpp"
				
				{
#line 300 "args/argcheck2.rl"
					(rl_den *= 10) += ((( (*( p)))) - '0'); rl_den_cnt *= 10; }
				
#line 2759 "args/argcheck2.cpp"
				
				goto _st25;
				_ctr75:
				{
#line 300 "args/argcheck2.rl"
					(rl_den *= 10) += ((( (*( p)))) - '0'); rl_den_cnt *= 10; }
				
#line 2767 "args/argcheck2.cpp"
				
				goto _st25;
				_st25:
				p+= 1;
				st_case_25:
				switch( ( (*( p))) ) {
					case 0: {
						goto _ctr65;
					}
					case 32: {
						goto _ctr66;
					}
					case 42: {
						goto _ctr67;
					}
					case 43: {
						goto _ctr68;
					}
					case 63: {
						goto _ctr71;
					}
					case 123: {
						goto _ctr72;
					}
				}
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr75;
				}
				goto _st0;
				_ctr52:
				{
#line 329 "args/argcheck2.rl"
					rl_chk.cmp = CMP_LESS; }
				
#line 2802 "args/argcheck2.cpp"
				
				goto _st26;
				_st26:
				p+= 1;
				st_case_26:
				switch( ( (*( p))) ) {
					case 43: {
						goto _ctr59;
					}
					case 45: {
						goto _ctr60;
					}
					case 61: {
						goto _ctr77;
					}
				}
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr61;
				}
				goto _st0;
				_ctr53:
				{
#line 375 "args/argcheck2.rl"
					rl_chk.cmp = CMP_EQUAL; }
				
#line 2828 "args/argcheck2.cpp"
				
				goto _st27;
				_ctr56:
				{
#line 383 "args/argcheck2.rl"
					rl_chk.cmp = CMP_APPROX; }
				
#line 2836 "args/argcheck2.cpp"
				
				goto _st27;
				_ctr87:
				{
#line 269 "args/argcheck2.rl"
					
					double real = rl_num;
					if (rl_den_cnt)
					real += rl_den / double(rl_den_cnt);
					
					rl_chk.values.push_back(rl_sign * real);
				}
				
#line 2850 "args/argcheck2.cpp"
				
				goto _st27;
				_st27:
				p+= 1;
				st_case_27:
				switch( ( (*( p))) ) {
					case 43: {
						goto _ctr79;
					}
					case 45: {
						goto _ctr80;
					}
				}
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr81;
				}
				goto _st0;
				_ctr79:
				{
#line 305 "args/argcheck2.rl"
					rl_sign = 1; }
				
#line 2873 "args/argcheck2.cpp"
				
				{
#line 296 "args/argcheck2.rl"
					rl_sign = 1; }
				
#line 2879 "args/argcheck2.cpp"
				
				goto _st28;
				_ctr80:
				{
#line 305 "args/argcheck2.rl"
					rl_sign = 1; }
				
#line 2887 "args/argcheck2.cpp"
				
				{
#line 297 "args/argcheck2.rl"
					rl_sign = -1; }
				
#line 2893 "args/argcheck2.cpp"
				
				goto _st28;
				_st28:
				p+= 1;
				st_case_28:
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr83;
				}
				goto _st0;
				_ctr81:
				{
#line 305 "args/argcheck2.rl"
					rl_sign = 1; }
				
#line 2908 "args/argcheck2.cpp"
				
				{
#line 299 "args/argcheck2.rl"
					rl_num = 0; }
				
#line 2914 "args/argcheck2.cpp"
				
				{
#line 299 "args/argcheck2.rl"
					(rl_num *= 10) += ((( (*( p)))) - '0'); }
				
#line 2920 "args/argcheck2.cpp"
				
				goto _st29;
				_ctr83:
				{
#line 299 "args/argcheck2.rl"
					rl_num = 0; }
				
#line 2928 "args/argcheck2.cpp"
				
				{
#line 299 "args/argcheck2.rl"
					(rl_num *= 10) += ((( (*( p)))) - '0'); }
				
#line 2934 "args/argcheck2.cpp"
				
				goto _st29;
				_ctr86:
				{
#line 299 "args/argcheck2.rl"
					(rl_num *= 10) += ((( (*( p)))) - '0'); }
				
#line 2942 "args/argcheck2.cpp"
				
				goto _st29;
				_st29:
				p+= 1;
				st_case_29:
				switch( ( (*( p))) ) {
					case 0: {
						goto _ctr65;
					}
					case 32: {
						goto _ctr66;
					}
					case 42: {
						goto _ctr67;
					}
					case 43: {
						goto _ctr68;
					}
					case 46: {
						goto _st30;
					}
					case 63: {
						goto _ctr71;
					}
					case 123: {
						goto _ctr72;
					}
					case 124: {
						goto _ctr87;
					}
				}
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr86;
				}
				goto _st0;
				_st30:
				p+= 1;
				st_case_30:
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr88;
				}
				goto _st0;
				_ctr88:
				{
#line 300 "args/argcheck2.rl"
					rl_den = 0; rl_den_cnt = 1; }
				
#line 2990 "args/argcheck2.cpp"
				
				{
#line 300 "args/argcheck2.rl"
					(rl_den *= 10) += ((( (*( p)))) - '0'); rl_den_cnt *= 10; }
				
#line 2996 "args/argcheck2.cpp"
				
				goto _st31;
				_ctr90:
				{
#line 300 "args/argcheck2.rl"
					(rl_den *= 10) += ((( (*( p)))) - '0'); rl_den_cnt *= 10; }
				
#line 3004 "args/argcheck2.cpp"
				
				goto _st31;
				_st31:
				p+= 1;
				st_case_31:
				switch( ( (*( p))) ) {
					case 0: {
						goto _ctr65;
					}
					case 32: {
						goto _ctr66;
					}
					case 42: {
						goto _ctr67;
					}
					case 43: {
						goto _ctr68;
					}
					case 63: {
						goto _ctr71;
					}
					case 123: {
						goto _ctr72;
					}
					case 124: {
						goto _ctr87;
					}
				}
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr90;
				}
				goto _st0;
				_ctr54:
				{
#line 328 "args/argcheck2.rl"
					rl_chk.cmp = CMP_GREATER; }
				
#line 3042 "args/argcheck2.cpp"
				
				goto _st32;
				_st32:
				p+= 1;
				st_case_32:
				switch( ( (*( p))) ) {
					case 43: {
						goto _ctr59;
					}
					case 45: {
						goto _ctr60;
					}
					case 61: {
						goto _ctr92;
					}
				}
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr61;
				}
				goto _st0;
				_st33:
				p+= 1;
				st_case_33:
				switch( ( (*( p))) ) {
					case 43: {
						goto _ctr93;
					}
					case 45: {
						goto _ctr94;
					}
				}
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr95;
				}
				goto _st0;
				_ctr93:
				{
#line 305 "args/argcheck2.rl"
					rl_sign = 1; }
				
#line 3083 "args/argcheck2.cpp"
				
				{
#line 296 "args/argcheck2.rl"
					rl_sign = 1; }
				
#line 3089 "args/argcheck2.cpp"
				
				goto _st34;
				_ctr94:
				{
#line 305 "args/argcheck2.rl"
					rl_sign = 1; }
				
#line 3097 "args/argcheck2.cpp"
				
				{
#line 297 "args/argcheck2.rl"
					rl_sign = -1; }
				
#line 3103 "args/argcheck2.cpp"
				
				goto _st34;
				_st34:
				p+= 1;
				st_case_34:
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr97;
				}
				goto _st0;
				_ctr95:
				{
#line 305 "args/argcheck2.rl"
					rl_sign = 1; }
				
#line 3118 "args/argcheck2.cpp"
				
				{
#line 299 "args/argcheck2.rl"
					rl_num = 0; }
				
#line 3124 "args/argcheck2.cpp"
				
				{
#line 299 "args/argcheck2.rl"
					(rl_num *= 10) += ((( (*( p)))) - '0'); }
				
#line 3130 "args/argcheck2.cpp"
				
				goto _st35;
				_ctr97:
				{
#line 299 "args/argcheck2.rl"
					rl_num = 0; }
				
#line 3138 "args/argcheck2.cpp"
				
				{
#line 299 "args/argcheck2.rl"
					(rl_num *= 10) += ((( (*( p)))) - '0'); }
				
#line 3144 "args/argcheck2.cpp"
				
				goto _st35;
				_ctr101:
				{
#line 299 "args/argcheck2.rl"
					(rl_num *= 10) += ((( (*( p)))) - '0'); }
				
#line 3152 "args/argcheck2.cpp"
				
				goto _st35;
				_st35:
				p+= 1;
				st_case_35:
				switch( ( (*( p))) ) {
					case 44: {
						goto _ctr99;
					}
					case 46: {
						goto _st41;
					}
				}
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr101;
				}
				goto _st0;
				_ctr99:
				{
#line 269 "args/argcheck2.rl"
					
					double real = rl_num;
					if (rl_den_cnt)
					real += rl_den / double(rl_den_cnt);
					
					rl_chk.values.push_back(rl_sign * real);
				}
				
#line 3181 "args/argcheck2.cpp"
				
				goto _st36;
				_st36:
				p+= 1;
				st_case_36:
				switch( ( (*( p))) ) {
					case 43: {
						goto _ctr103;
					}
					case 45: {
						goto _ctr104;
					}
				}
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr105;
				}
				goto _st0;
				_ctr103:
				{
#line 305 "args/argcheck2.rl"
					rl_sign = 1; }
				
#line 3204 "args/argcheck2.cpp"
				
				{
#line 296 "args/argcheck2.rl"
					rl_sign = 1; }
				
#line 3210 "args/argcheck2.cpp"
				
				goto _st37;
				_ctr104:
				{
#line 305 "args/argcheck2.rl"
					rl_sign = 1; }
				
#line 3218 "args/argcheck2.cpp"
				
				{
#line 297 "args/argcheck2.rl"
					rl_sign = -1; }
				
#line 3224 "args/argcheck2.cpp"
				
				goto _st37;
				_st37:
				p+= 1;
				st_case_37:
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr107;
				}
				goto _st0;
				_ctr105:
				{
#line 305 "args/argcheck2.rl"
					rl_sign = 1; }
				
#line 3239 "args/argcheck2.cpp"
				
				{
#line 299 "args/argcheck2.rl"
					rl_num = 0; }
				
#line 3245 "args/argcheck2.cpp"
				
				{
#line 299 "args/argcheck2.rl"
					(rl_num *= 10) += ((( (*( p)))) - '0'); }
				
#line 3251 "args/argcheck2.cpp"
				
				goto _st38;
				_ctr107:
				{
#line 299 "args/argcheck2.rl"
					rl_num = 0; }
				
#line 3259 "args/argcheck2.cpp"
				
				{
#line 299 "args/argcheck2.rl"
					(rl_num *= 10) += ((( (*( p)))) - '0'); }
				
#line 3265 "args/argcheck2.cpp"
				
				goto _st38;
				_ctr111:
				{
#line 299 "args/argcheck2.rl"
					(rl_num *= 10) += ((( (*( p)))) - '0'); }
				
#line 3273 "args/argcheck2.cpp"
				
				goto _st38;
				_st38:
				p+= 1;
				st_case_38:
				switch( ( (*( p))) ) {
					case 41: {
						goto _ctr109;
					}
					case 46: {
						goto _st39;
					}
					case 93: {
						goto _ctr112;
					}
				}
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr111;
				}
				goto _st0;
				_st39:
				p+= 1;
				st_case_39:
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr113;
				}
				goto _st0;
				_ctr113:
				{
#line 300 "args/argcheck2.rl"
					rl_den = 0; rl_den_cnt = 1; }
				
#line 3306 "args/argcheck2.cpp"
				
				{
#line 300 "args/argcheck2.rl"
					(rl_den *= 10) += ((( (*( p)))) - '0'); rl_den_cnt *= 10; }
				
#line 3312 "args/argcheck2.cpp"
				
				goto _st40;
				_ctr115:
				{
#line 300 "args/argcheck2.rl"
					(rl_den *= 10) += ((( (*( p)))) - '0'); rl_den_cnt *= 10; }
				
#line 3320 "args/argcheck2.cpp"
				
				goto _st40;
				_st40:
				p+= 1;
				st_case_40:
				switch( ( (*( p))) ) {
					case 41: {
						goto _ctr109;
					}
					case 93: {
						goto _ctr112;
					}
				}
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr115;
				}
				goto _st0;
				_st41:
				p+= 1;
				st_case_41:
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr116;
				}
				goto _st0;
				_ctr116:
				{
#line 300 "args/argcheck2.rl"
					rl_den = 0; rl_den_cnt = 1; }
				
#line 3350 "args/argcheck2.cpp"
				
				{
#line 300 "args/argcheck2.rl"
					(rl_den *= 10) += ((( (*( p)))) - '0'); rl_den_cnt *= 10; }
				
#line 3356 "args/argcheck2.cpp"
				
				goto _st42;
				_ctr118:
				{
#line 300 "args/argcheck2.rl"
					(rl_den *= 10) += ((( (*( p)))) - '0'); rl_den_cnt *= 10; }
				
#line 3364 "args/argcheck2.cpp"
				
				goto _st42;
				_st42:
				p+= 1;
				st_case_42:
				if ( ( (*( p))) == 44 ) {
					goto _ctr99;
				}
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr118;
				}
				goto _st0;
				_ctr7:
				{
#line 395 "args/argcheck2.rl"
					rl_chk.type = CHECK_INT; }
				
#line 3382 "args/argcheck2.cpp"
				
				goto _st43;
				_st43:
				p+= 1;
				st_case_43:
				switch( ( (*( p))) ) {
					case 0: {
						goto _ctr28;
					}
					case 32: {
						goto _ctr29;
					}
					case 33: {
						goto _st44;
					}
					case 37: {
						goto _ctr121;
					}
					case 42: {
						goto _ctr30;
					}
					case 43: {
						goto _ctr31;
					}
					case 60: {
						goto _ctr122;
					}
					case 61: {
						goto _ctr123;
					}
					case 62: {
						goto _ctr124;
					}
					case 63: {
						goto _ctr32;
					}
					case 91: {
						goto _st55;
					}
					case 94: {
						goto _st63;
					}
					case 123: {
						goto _ctr33;
					}
				}
				goto _st0;
				_st44:
				p+= 1;
				st_case_44:
				if ( ( (*( p))) == 61 ) {
					goto _ctr127;
				}
				goto _st0;
				_ctr127:
				{
#line 330 "args/argcheck2.rl"
					rl_chk.cmp = CMP_NOT_EQUAL; }
				
#line 3442 "args/argcheck2.cpp"
				
				goto _st45;
				_ctr147:
				{
#line 329 "args/argcheck2.rl"
					rl_chk.cmp = CMP_LESS_EQ; }
				
#line 3450 "args/argcheck2.cpp"
				
				goto _st45;
				_ctr158:
				{
#line 328 "args/argcheck2.rl"
					rl_chk.cmp = CMP_GREATER_EQ; }
				
#line 3458 "args/argcheck2.cpp"
				
				goto _st45;
				_st45:
				p+= 1;
				st_case_45:
				switch( ( (*( p))) ) {
					case 43: {
						goto _ctr129;
					}
					case 45: {
						goto _ctr130;
					}
				}
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr131;
				}
				goto _st0;
				_ctr129:
				{
#line 305 "args/argcheck2.rl"
					rl_sign = 1; }
				
#line 3481 "args/argcheck2.cpp"
				
				{
#line 296 "args/argcheck2.rl"
					rl_sign = 1; }
				
#line 3487 "args/argcheck2.cpp"
				
				goto _st46;
				_ctr130:
				{
#line 305 "args/argcheck2.rl"
					rl_sign = 1; }
				
#line 3495 "args/argcheck2.cpp"
				
				{
#line 297 "args/argcheck2.rl"
					rl_sign = -1; }
				
#line 3501 "args/argcheck2.cpp"
				
				goto _st46;
				_st46:
				p+= 1;
				st_case_46:
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr133;
				}
				goto _st0;
				_ctr131:
				{
#line 305 "args/argcheck2.rl"
					rl_sign = 1; }
				
#line 3516 "args/argcheck2.cpp"
				
				{
#line 299 "args/argcheck2.rl"
					rl_num = 0; }
				
#line 3522 "args/argcheck2.cpp"
				
				{
#line 299 "args/argcheck2.rl"
					(rl_num *= 10) += ((( (*( p)))) - '0'); }
				
#line 3528 "args/argcheck2.cpp"
				
				goto _st47;
				_ctr133:
				{
#line 299 "args/argcheck2.rl"
					rl_num = 0; }
				
#line 3536 "args/argcheck2.cpp"
				
				{
#line 299 "args/argcheck2.rl"
					(rl_num *= 10) += ((( (*( p)))) - '0'); }
				
#line 3542 "args/argcheck2.cpp"
				
				goto _st47;
				_ctr139:
				{
#line 299 "args/argcheck2.rl"
					(rl_num *= 10) += ((( (*( p)))) - '0'); }
				
#line 3550 "args/argcheck2.cpp"
				
				goto _st47;
				_st47:
				p+= 1;
				st_case_47:
				switch( ( (*( p))) ) {
					case 0: {
						goto _ctr135;
					}
					case 32: {
						goto _ctr136;
					}
					case 42: {
						goto _ctr137;
					}
					case 43: {
						goto _ctr138;
					}
					case 63: {
						goto _ctr140;
					}
					case 123: {
						goto _ctr141;
					}
				}
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr139;
				}
				goto _st0;
				_ctr121:
				{
#line 335 "args/argcheck2.rl"
					rl_chk.cmp = CMP_MODULE; }
				
#line 3585 "args/argcheck2.cpp"
				
				goto _st48;
				_st48:
				p+= 1;
				st_case_48:
				if ( 49 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr143;
				}
				goto _st0;
				_ctr143:
				{
#line 336 "args/argcheck2.rl"
					rl_sign = 1; rl_num = 0; }
				
#line 3600 "args/argcheck2.cpp"
				
				{
#line 336 "args/argcheck2.rl"
					(rl_num *= 10) += ((( (*( p)))) - '0'); }
				
#line 3606 "args/argcheck2.cpp"
				
				goto _st49;
				_ctr145:
				{
#line 336 "args/argcheck2.rl"
					(rl_num *= 10) += ((( (*( p)))) - '0'); }
				
#line 3614 "args/argcheck2.cpp"
				
				goto _st49;
				_st49:
				p+= 1;
				st_case_49:
				switch( ( (*( p))) ) {
					case 0: {
						goto _ctr135;
					}
					case 32: {
						goto _ctr136;
					}
					case 42: {
						goto _ctr137;
					}
					case 43: {
						goto _ctr138;
					}
					case 63: {
						goto _ctr140;
					}
					case 123: {
						goto _ctr141;
					}
				}
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr145;
				}
				goto _st0;
				_ctr122:
				{
#line 329 "args/argcheck2.rl"
					rl_chk.cmp = CMP_LESS; }
				
#line 3649 "args/argcheck2.cpp"
				
				goto _st50;
				_st50:
				p+= 1;
				st_case_50:
				switch( ( (*( p))) ) {
					case 43: {
						goto _ctr129;
					}
					case 45: {
						goto _ctr130;
					}
					case 61: {
						goto _ctr147;
					}
				}
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr131;
				}
				goto _st0;
				_ctr123:
				{
#line 325 "args/argcheck2.rl"
					rl_chk.cmp = CMP_EQUAL; }
				
#line 3675 "args/argcheck2.cpp"
				
				goto _st51;
				_ctr156:
				{
#line 265 "args/argcheck2.rl"
					
					rl_chk.values.push_back((int64_t)(rl_sign * rl_num));
				}
				
#line 3685 "args/argcheck2.cpp"
				
				goto _st51;
				_st51:
				p+= 1;
				st_case_51:
				switch( ( (*( p))) ) {
					case 43: {
						goto _ctr149;
					}
					case 45: {
						goto _ctr150;
					}
				}
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr151;
				}
				goto _st0;
				_ctr149:
				{
#line 305 "args/argcheck2.rl"
					rl_sign = 1; }
				
#line 3708 "args/argcheck2.cpp"
				
				{
#line 296 "args/argcheck2.rl"
					rl_sign = 1; }
				
#line 3714 "args/argcheck2.cpp"
				
				goto _st52;
				_ctr150:
				{
#line 305 "args/argcheck2.rl"
					rl_sign = 1; }
				
#line 3722 "args/argcheck2.cpp"
				
				{
#line 297 "args/argcheck2.rl"
					rl_sign = -1; }
				
#line 3728 "args/argcheck2.cpp"
				
				goto _st52;
				_st52:
				p+= 1;
				st_case_52:
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr153;
				}
				goto _st0;
				_ctr151:
				{
#line 305 "args/argcheck2.rl"
					rl_sign = 1; }
				
#line 3743 "args/argcheck2.cpp"
				
				{
#line 299 "args/argcheck2.rl"
					rl_num = 0; }
				
#line 3749 "args/argcheck2.cpp"
				
				{
#line 299 "args/argcheck2.rl"
					(rl_num *= 10) += ((( (*( p)))) - '0'); }
				
#line 3755 "args/argcheck2.cpp"
				
				goto _st53;
				_ctr153:
				{
#line 299 "args/argcheck2.rl"
					rl_num = 0; }
				
#line 3763 "args/argcheck2.cpp"
				
				{
#line 299 "args/argcheck2.rl"
					(rl_num *= 10) += ((( (*( p)))) - '0'); }
				
#line 3769 "args/argcheck2.cpp"
				
				goto _st53;
				_ctr155:
				{
#line 299 "args/argcheck2.rl"
					(rl_num *= 10) += ((( (*( p)))) - '0'); }
				
#line 3777 "args/argcheck2.cpp"
				
				goto _st53;
				_st53:
				p+= 1;
				st_case_53:
				switch( ( (*( p))) ) {
					case 0: {
						goto _ctr135;
					}
					case 32: {
						goto _ctr136;
					}
					case 42: {
						goto _ctr137;
					}
					case 43: {
						goto _ctr138;
					}
					case 63: {
						goto _ctr140;
					}
					case 123: {
						goto _ctr141;
					}
					case 124: {
						goto _ctr156;
					}
				}
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr155;
				}
				goto _st0;
				_ctr124:
				{
#line 328 "args/argcheck2.rl"
					rl_chk.cmp = CMP_GREATER; }
				
#line 3815 "args/argcheck2.cpp"
				
				goto _st54;
				_st54:
				p+= 1;
				st_case_54:
				switch( ( (*( p))) ) {
					case 43: {
						goto _ctr129;
					}
					case 45: {
						goto _ctr130;
					}
					case 61: {
						goto _ctr158;
					}
				}
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr131;
				}
				goto _st0;
				_st55:
				p+= 1;
				st_case_55:
				switch( ( (*( p))) ) {
					case 43: {
						goto _ctr159;
					}
					case 45: {
						goto _ctr160;
					}
				}
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr161;
				}
				goto _st0;
				_ctr159:
				{
#line 305 "args/argcheck2.rl"
					rl_sign = 1; }
				
#line 3856 "args/argcheck2.cpp"
				
				{
#line 296 "args/argcheck2.rl"
					rl_sign = 1; }
				
#line 3862 "args/argcheck2.cpp"
				
				goto _st56;
				_ctr160:
				{
#line 305 "args/argcheck2.rl"
					rl_sign = 1; }
				
#line 3870 "args/argcheck2.cpp"
				
				{
#line 297 "args/argcheck2.rl"
					rl_sign = -1; }
				
#line 3876 "args/argcheck2.cpp"
				
				goto _st56;
				_st56:
				p+= 1;
				st_case_56:
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr163;
				}
				goto _st0;
				_ctr161:
				{
#line 305 "args/argcheck2.rl"
					rl_sign = 1; }
				
#line 3891 "args/argcheck2.cpp"
				
				{
#line 299 "args/argcheck2.rl"
					rl_num = 0; }
				
#line 3897 "args/argcheck2.cpp"
				
				{
#line 299 "args/argcheck2.rl"
					(rl_num *= 10) += ((( (*( p)))) - '0'); }
				
#line 3903 "args/argcheck2.cpp"
				
				goto _st57;
				_ctr163:
				{
#line 299 "args/argcheck2.rl"
					rl_num = 0; }
				
#line 3911 "args/argcheck2.cpp"
				
				{
#line 299 "args/argcheck2.rl"
					(rl_num *= 10) += ((( (*( p)))) - '0'); }
				
#line 3917 "args/argcheck2.cpp"
				
				goto _st57;
				_ctr166:
				{
#line 299 "args/argcheck2.rl"
					(rl_num *= 10) += ((( (*( p)))) - '0'); }
				
#line 3925 "args/argcheck2.cpp"
				
				goto _st57;
				_st57:
				p+= 1;
				st_case_57:
				if ( ( (*( p))) == 44 ) {
					goto _ctr165;
				}
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr166;
				}
				goto _st0;
				_ctr165:
				{
#line 265 "args/argcheck2.rl"
					
					rl_chk.values.push_back((int64_t)(rl_sign * rl_num));
				}
				
#line 3945 "args/argcheck2.cpp"
				
				goto _st58;
				_st58:
				p+= 1;
				st_case_58:
				switch( ( (*( p))) ) {
					case 43: {
						goto _ctr168;
					}
					case 45: {
						goto _ctr169;
					}
				}
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr170;
				}
				goto _st0;
				_ctr168:
				{
#line 305 "args/argcheck2.rl"
					rl_sign = 1; }
				
#line 3968 "args/argcheck2.cpp"
				
				{
#line 296 "args/argcheck2.rl"
					rl_sign = 1; }
				
#line 3974 "args/argcheck2.cpp"
				
				goto _st59;
				_ctr169:
				{
#line 305 "args/argcheck2.rl"
					rl_sign = 1; }
				
#line 3982 "args/argcheck2.cpp"
				
				{
#line 297 "args/argcheck2.rl"
					rl_sign = -1; }
				
#line 3988 "args/argcheck2.cpp"
				
				goto _st59;
				_st59:
				p+= 1;
				st_case_59:
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr172;
				}
				goto _st0;
				_ctr170:
				{
#line 305 "args/argcheck2.rl"
					rl_sign = 1; }
				
#line 4003 "args/argcheck2.cpp"
				
				{
#line 299 "args/argcheck2.rl"
					rl_num = 0; }
				
#line 4009 "args/argcheck2.cpp"
				
				{
#line 299 "args/argcheck2.rl"
					(rl_num *= 10) += ((( (*( p)))) - '0'); }
				
#line 4015 "args/argcheck2.cpp"
				
				goto _st60;
				_ctr172:
				{
#line 299 "args/argcheck2.rl"
					rl_num = 0; }
				
#line 4023 "args/argcheck2.cpp"
				
				{
#line 299 "args/argcheck2.rl"
					(rl_num *= 10) += ((( (*( p)))) - '0'); }
				
#line 4029 "args/argcheck2.cpp"
				
				goto _st60;
				_ctr175:
				{
#line 299 "args/argcheck2.rl"
					(rl_num *= 10) += ((( (*( p)))) - '0'); }
				
#line 4037 "args/argcheck2.cpp"
				
				goto _st60;
				_st60:
				p+= 1;
				st_case_60:
				switch( ( (*( p))) ) {
					case 41: {
						goto _ctr174;
					}
					case 93: {
						goto _ctr176;
					}
				}
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr175;
				}
				goto _st0;
				_ctr174:
				{
#line 265 "args/argcheck2.rl"
					
					rl_chk.values.push_back((int64_t)(rl_sign * rl_num));
				}
				
#line 4062 "args/argcheck2.cpp"
				
				goto _st61;
				_st61:
				p+= 1;
				st_case_61:
				switch( ( (*( p))) ) {
					case 0: {
						goto _ctr178;
					}
					case 32: {
						goto _ctr179;
					}
					case 42: {
						goto _ctr180;
					}
					case 43: {
						goto _ctr181;
					}
					case 63: {
						goto _ctr182;
					}
					case 123: {
						goto _ctr183;
					}
				}
				goto _st0;
				_ctr176:
				{
#line 265 "args/argcheck2.rl"
					
					rl_chk.values.push_back((int64_t)(rl_sign * rl_num));
				}
				
#line 4096 "args/argcheck2.cpp"
				
				goto _st62;
				_st62:
				p+= 1;
				st_case_62:
				switch( ( (*( p))) ) {
					case 0: {
						goto _ctr185;
					}
					case 32: {
						goto _ctr186;
					}
					case 42: {
						goto _ctr187;
					}
					case 43: {
						goto _ctr188;
					}
					case 63: {
						goto _ctr189;
					}
					case 123: {
						goto _ctr190;
					}
				}
				goto _st0;
				_st63:
				p+= 1;
				st_case_63:
				if ( ( (*( p))) == 50 ) {
					goto _ctr191;
				}
				goto _st0;
				_ctr8:
				{
#line 396 "args/argcheck2.rl"
					rl_chk.type = CHECK_SYMBOL; }
				
#line 4135 "args/argcheck2.cpp"
				
				goto _st64;
				_st64:
				p+= 1;
				st_case_64:
				switch( ( (*( p))) ) {
					case 0: {
						goto _ctr28;
					}
					case 32: {
						goto _ctr29;
					}
					case 42: {
						goto _ctr30;
					}
					case 43: {
						goto _ctr31;
					}
					case 61: {
						goto _ctr193;
					}
					case 63: {
						goto _ctr32;
					}
					case 123: {
						goto _ctr33;
					}
				}
				goto _st0;
				_ctr193:
				{
#line 358 "args/argcheck2.rl"
					rl_chk.cmp = CMP_EQUAL; }
				
#line 4170 "args/argcheck2.cpp"
				
				goto _st65;
				_ctr203:
				{
#line 277 "args/argcheck2.rl"
					
					try {
						ArgString str{ {}, 0 };
						str.first.assign(rl_sym_start, p - rl_sym_start);
						str.second = crc32_hash(str.first.data());
						rl_chk.values.push_back(str);
					} catch(std::exception& e) {
						LIB_ERR << "exception: " << e.what();
					}
				}
				
#line 4187 "args/argcheck2.cpp"
				
				goto _st65;
				_st65:
				p+= 1;
				st_case_65:
				switch( ( (*( p))) ) {
					case 35: {
						goto _ctr195;
					}
					case 45: {
						goto _ctr195;
					}
					case 95: {
						goto _ctr195;
					}
				}
				if ( ( (*( p))) < 64 ) {
					if ( 48 <= ( (*( p))) && ( (*( p))) <= 58 ) {
						goto _ctr195;
					}
				} else if ( ( (*( p))) > 90 ) {
					if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
						goto _ctr195;
					}
				} else {
					goto _ctr195;
				}
				goto _st0;
				_ctr195:
				{
#line 351 "args/argcheck2.rl"
					rl_sym_start = p; }
				
#line 4221 "args/argcheck2.cpp"
				
				goto _st66;
				_st66:
				p+= 1;
				st_case_66:
				switch( ( (*( p))) ) {
					case 0: {
						goto _ctr197;
					}
					case 32: {
						goto _ctr198;
					}
					case 35: {
						goto _st66;
					}
					case 42: {
						goto _ctr199;
					}
					case 43: {
						goto _ctr200;
					}
					case 45: {
						goto _st66;
					}
					case 63: {
						goto _ctr201;
					}
					case 95: {
						goto _st66;
					}
					case 123: {
						goto _ctr202;
					}
					case 124: {
						goto _ctr203;
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
				goto _st0;
				_ctr3:
				{
#line 405 "args/argcheck2.rl"
					rl_chk.name.clear(); }
				
#line 4276 "args/argcheck2.cpp"
				
				{
#line 405 "args/argcheck2.rl"
					rl_chk.name.push_back((( (*( p))))); }
				
#line 4282 "args/argcheck2.cpp"
				
				{
#line 393 "args/argcheck2.rl"
					rl_chk.type = CHECK_BOOL; }
				
#line 4288 "args/argcheck2.cpp"
				
				goto _st67;
				_st67:
				p+= 1;
				st_case_67:
				switch( ( (*( p))) ) {
					case 0: {
						goto _ctr28;
					}
					case 32: {
						goto _ctr29;
					}
					case 42: {
						goto _ctr30;
					}
					case 43: {
						goto _ctr31;
					}
					case 58: {
						goto _st10;
					}
					case 63: {
						goto _ctr205;
					}
					case 95: {
						goto _ctr11;
					}
					case 123: {
						goto _ctr33;
					}
				}
				if ( ( (*( p))) > 57 ) {
					if ( 64 <= ( (*( p))) && ( (*( p))) <= 90 ) {
						goto _ctr11;
					}
				} else if ( ( (*( p))) >= 48 ) {
					goto _ctr11;
				}
				goto _st0;
				_ctr205:
				{
#line 405 "args/argcheck2.rl"
					rl_chk.name.push_back((( (*( p))))); }
				
#line 4333 "args/argcheck2.cpp"
				
				{
#line 415 "args/argcheck2.rl"
					rl_chk.rmin = 1; rl_chk.rmax = 1; }
				
#line 4339 "args/argcheck2.cpp"
				
				{
#line 399 "args/argcheck2.rl"
					rl_chk.setRepeats(0, 1); }
				
#line 4345 "args/argcheck2.cpp"
				
				goto _st68;
				_st68:
				p+= 1;
				st_case_68:
				switch( ( (*( p))) ) {
					case 0: {
						goto _ctr35;
					}
					case 32: {
						goto _ctr36;
					}
					case 58: {
						goto _st10;
					}
					case 95: {
						goto _ctr14;
					}
				}
				if ( ( (*( p))) > 57 ) {
					if ( 63 <= ( (*( p))) && ( (*( p))) <= 90 ) {
						goto _ctr14;
					}
				} else if ( ( (*( p))) >= 48 ) {
					goto _ctr14;
				}
				goto _st0;
				_out1: cs = 1; goto _out; 
				_out0: cs = 0; goto _out; 
				_out2: cs = 2; goto _out; 
				_out3: cs = 3; goto _out; 
				_out4: cs = 4; goto _out; 
				_out5: cs = 5; goto _out; 
				_out6: cs = 6; goto _out; 
				_out7: cs = 7; goto _out; 
				_out8: cs = 8; goto _out; 
				_out9: cs = 9; goto _out; 
				_out10: cs = 10; goto _out; 
				_out11: cs = 11; goto _out; 
				_out69: cs = 69; goto _out; 
				_out12: cs = 12; goto _out; 
				_out13: cs = 13; goto _out; 
				_out14: cs = 14; goto _out; 
				_out15: cs = 15; goto _out; 
				_out16: cs = 16; goto _out; 
				_out17: cs = 17; goto _out; 
				_out18: cs = 18; goto _out; 
				_out19: cs = 19; goto _out; 
				_out20: cs = 20; goto _out; 
				_out21: cs = 21; goto _out; 
				_out22: cs = 22; goto _out; 
				_out23: cs = 23; goto _out; 
				_out24: cs = 24; goto _out; 
				_out25: cs = 25; goto _out; 
				_out26: cs = 26; goto _out; 
				_out27: cs = 27; goto _out; 
				_out28: cs = 28; goto _out; 
				_out29: cs = 29; goto _out; 
				_out30: cs = 30; goto _out; 
				_out31: cs = 31; goto _out; 
				_out32: cs = 32; goto _out; 
				_out33: cs = 33; goto _out; 
				_out34: cs = 34; goto _out; 
				_out35: cs = 35; goto _out; 
				_out36: cs = 36; goto _out; 
				_out37: cs = 37; goto _out; 
				_out38: cs = 38; goto _out; 
				_out39: cs = 39; goto _out; 
				_out40: cs = 40; goto _out; 
				_out41: cs = 41; goto _out; 
				_out42: cs = 42; goto _out; 
				_out43: cs = 43; goto _out; 
				_out44: cs = 44; goto _out; 
				_out45: cs = 45; goto _out; 
				_out46: cs = 46; goto _out; 
				_out47: cs = 47; goto _out; 
				_out48: cs = 48; goto _out; 
				_out49: cs = 49; goto _out; 
				_out50: cs = 50; goto _out; 
				_out51: cs = 51; goto _out; 
				_out52: cs = 52; goto _out; 
				_out53: cs = 53; goto _out; 
				_out54: cs = 54; goto _out; 
				_out55: cs = 55; goto _out; 
				_out56: cs = 56; goto _out; 
				_out57: cs = 57; goto _out; 
				_out58: cs = 58; goto _out; 
				_out59: cs = 59; goto _out; 
				_out60: cs = 60; goto _out; 
				_out61: cs = 61; goto _out; 
				_out62: cs = 62; goto _out; 
				_out63: cs = 63; goto _out; 
				_out64: cs = 64; goto _out; 
				_out65: cs = 65; goto _out; 
				_out66: cs = 66; goto _out; 
				_out67: cs = 67; goto _out; 
				_out68: cs = 68; goto _out; 
				_out: {}
			}
			
#line 856 "args/argcheck2.rl"
			
			
			if (cs < 
#line 4450 "args/argcheck2.cpp"
			69
#line 858 "args/argcheck2.rl"
			) {
				LIB_ERR << fmt::format("invalid format string: '{}'", str);
				chk_.reset();
			}
		}
		
		void ArgChecker::usage(BaseObject* obj, t_symbol* m) const
		{
			if (!chk_)
				return;
			
			Error err(obj);
			if (m)
				err << '[' << m->s_name << "( ";
			
			err << chk_->help();
		}
		
		bool check_args(const char* arg_string, const AtomListView& lv, BaseObject* obj, ArgMatchList* matches)
		{
			ArgChecker chk(arg_string);
			return chk.check(lv, obj, matches);
		}
		
	}
}
