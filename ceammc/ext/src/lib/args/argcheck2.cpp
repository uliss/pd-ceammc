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


#line 165 "args/argcheck2.cpp"
static const int arg_check2_start = 1;
static const int arg_check2_first_final = 52;
static const int arg_check2_error = 0;

static const int arg_check2_en_main = 1;


#line 305 "args/argcheck2.rl"


namespace ceammc {
	namespace args {
		
		namespace {
			
			bool checkAtom(const Check& c, const Atom& a, int i, const void* x) {
				switch (c.type) {
					case CHECK_ATOM:
					debug("atom", "Ok");
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
						
						switch (c.cmp) {
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
								
								if (c.cmp == CMP_RANGE_CLOSED && !(a <= val && val <= b)) {
									pdError(x, fmt::format("{} value at [{}] expected to be in [{},{}] range, got: {}", c.argName(), i, a, b, val));
									return false;
								}
								
								if (c.cmp == CMP_RANGE_SEMIOPEN && !(a <= val && val < b)) {
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
						debug("symbol", "Ok");
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
						
						switch (c.cmp) {
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
		};
		
		ArgChecker::~ArgChecker()  = default;
		
		bool ArgChecker::check(const AtomListView& lv, BaseObject* obj) const
		{
			if (!chk_)
				return false;
			
			const void* x = obj ? obj->owner() : nullptr;
			
			pdDebug(x, chk_->helpBrief());
			
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
					if (!checkAtom(check, lv[atom_idx], atom_idx, x))
						return false;
				}
				
				for (int k = check.rmin; k < check.repeatMax(); k++, atom_idx++) {
					if (atom_idx >= N)
						break;
					
					if (!checkAtom(check, lv[atom_idx], atom_idx, x))
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
			
			
#line 462 "args/argcheck2.cpp"
			{
				cs = (int)arg_check2_start;
			}
			
#line 467 "args/argcheck2.cpp"
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
					case 52:
					goto st_case_52;
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
				}
				goto st_out;
				_ctr28:
				{
#line 296 "args/argcheck2.rl"
					rl_chk.rmin = 1; rl_chk.rmax = 1; }
				
#line 583 "args/argcheck2.cpp"
				
				{
#line 165 "args/argcheck2.rl"
					
					if (chk_)
					chk_->push_back(rl_chk);
					
					rl_num = 0;
					rl_den = 0;
					rl_sign = 0;
					rl_den_cnt = 0;
					rl_chk = {};
				}
				
#line 598 "args/argcheck2.cpp"
				
				goto _st1;
				_ctr35:
				{
#line 165 "args/argcheck2.rl"
					
					if (chk_)
					chk_->push_back(rl_chk);
					
					rl_num = 0;
					rl_den = 0;
					rl_sign = 0;
					rl_den_cnt = 0;
					rl_chk = {};
				}
				
#line 615 "args/argcheck2.cpp"
				
				goto _st1;
				_ctr62:
				{
#line 180 "args/argcheck2.rl"
					
					double real = rl_num;
					if (rl_den_cnt)
					real += rl_den / double(rl_den_cnt);
					
					rl_chk.values.push_back(rl_sign * real);
				}
				
#line 629 "args/argcheck2.cpp"
				
				{
#line 296 "args/argcheck2.rl"
					rl_chk.rmin = 1; rl_chk.rmax = 1; }
				
#line 635 "args/argcheck2.cpp"
				
				{
#line 165 "args/argcheck2.rl"
					
					if (chk_)
					chk_->push_back(rl_chk);
					
					rl_num = 0;
					rl_den = 0;
					rl_sign = 0;
					rl_den_cnt = 0;
					rl_chk = {};
				}
				
#line 650 "args/argcheck2.cpp"
				
				goto _st1;
				_ctr93:
				{
#line 176 "args/argcheck2.rl"
					
					rl_chk.values.push_back((int64_t)(rl_sign * rl_num));
				}
				
#line 660 "args/argcheck2.cpp"
				
				{
#line 296 "args/argcheck2.rl"
					rl_chk.rmin = 1; rl_chk.rmax = 1; }
				
#line 666 "args/argcheck2.cpp"
				
				{
#line 165 "args/argcheck2.rl"
					
					if (chk_)
					chk_->push_back(rl_chk);
					
					rl_num = 0;
					rl_den = 0;
					rl_sign = 0;
					rl_den_cnt = 0;
					rl_chk = {};
				}
				
#line 681 "args/argcheck2.cpp"
				
				goto _st1;
				_ctr139:
				{
#line 188 "args/argcheck2.rl"
					
					try {
						ArgString str{ {}, 0 };
						str.first.assign(rl_sym_start, p - rl_sym_start);
						str.second = crc32_hash(str.first.data());
						rl_chk.values.push_back(str);
					} catch(std::exception& e) {
						LIB_ERR << "exception: " << e.what();
					}
				}
				
#line 698 "args/argcheck2.cpp"
				
				{
#line 296 "args/argcheck2.rl"
					rl_chk.rmin = 1; rl_chk.rmax = 1; }
				
#line 704 "args/argcheck2.cpp"
				
				{
#line 165 "args/argcheck2.rl"
					
					if (chk_)
					chk_->push_back(rl_chk);
					
					rl_num = 0;
					rl_den = 0;
					rl_sign = 0;
					rl_den_cnt = 0;
					rl_chk = {};
				}
				
#line 719 "args/argcheck2.cpp"
				
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
#line 287 "args/argcheck2.rl"
					rl_chk.name.clear(); }
				
#line 767 "args/argcheck2.cpp"
				
				{
#line 287 "args/argcheck2.rl"
					rl_chk.name.push_back((( (*( p))))); }
				
#line 773 "args/argcheck2.cpp"
				
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
#line 287 "args/argcheck2.rl"
					rl_chk.name.push_back((( (*( p))))); }
				
#line 802 "args/argcheck2.cpp"
				
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
#line 287 "args/argcheck2.rl"
					rl_chk.name.push_back((( (*( p))))); }
				
#line 831 "args/argcheck2.cpp"
				
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
#line 287 "args/argcheck2.rl"
					rl_chk.name.push_back((( (*( p))))); }
				
#line 860 "args/argcheck2.cpp"
				
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
#line 287 "args/argcheck2.rl"
					rl_chk.name.push_back((( (*( p))))); }
				
#line 889 "args/argcheck2.cpp"
				
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
#line 287 "args/argcheck2.rl"
					rl_chk.name.push_back((( (*( p))))); }
				
#line 918 "args/argcheck2.cpp"
				
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
#line 287 "args/argcheck2.rl"
					rl_chk.name.push_back((( (*( p))))); }
				
#line 947 "args/argcheck2.cpp"
				
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
#line 287 "args/argcheck2.rl"
					rl_chk.name.push_back((( (*( p))))); }
				
#line 976 "args/argcheck2.cpp"
				
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
#line 275 "args/argcheck2.rl"
					rl_chk.type = CHECK_ATOM; }
				
#line 1019 "args/argcheck2.cpp"
				
				goto _st11;
				_ctr5:
				{
#line 277 "args/argcheck2.rl"
					rl_chk.type = CHECK_BYTE; }
				
#line 1027 "args/argcheck2.cpp"
				
				goto _st11;
				_ctr25:
				{
#line 276 "args/argcheck2.rl"
					rl_chk.type = CHECK_BOOL; }
				
#line 1035 "args/argcheck2.cpp"
				
				goto _st11;
				_ctr129:
				{
#line 228 "args/argcheck2.rl"
					rl_chk.cmp = CMP_RANGE_SEMIOPEN; }
				
#line 1043 "args/argcheck2.cpp"
				
				goto _st11;
				_ctr131:
				{
#line 228 "args/argcheck2.rl"
					rl_chk.cmp = CMP_RANGE_CLOSED; }
				
#line 1051 "args/argcheck2.cpp"
				
				goto _st11;
				_ctr132:
				{
#line 253 "args/argcheck2.rl"
					rl_chk.cmp = CMP_POWER2; }
				
#line 1059 "args/argcheck2.cpp"
				
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
#line 296 "args/argcheck2.rl"
					rl_chk.rmin = 1; rl_chk.rmax = 1; }
				
#line 1093 "args/argcheck2.cpp"
				
				{
#line 165 "args/argcheck2.rl"
					
					if (chk_)
					chk_->push_back(rl_chk);
					
					rl_num = 0;
					rl_den = 0;
					rl_sign = 0;
					rl_den_cnt = 0;
					rl_chk = {};
				}
				
#line 1108 "args/argcheck2.cpp"
				
				{
#line 301 "args/argcheck2.rl"
					{p+= 1; cs = 52; goto _out;} }
				
#line 1114 "args/argcheck2.cpp"
				
				goto _st52;
				_ctr34:
				{
#line 165 "args/argcheck2.rl"
					
					if (chk_)
					chk_->push_back(rl_chk);
					
					rl_num = 0;
					rl_den = 0;
					rl_sign = 0;
					rl_den_cnt = 0;
					rl_chk = {};
				}
				
#line 1131 "args/argcheck2.cpp"
				
				{
#line 301 "args/argcheck2.rl"
					{p+= 1; cs = 52; goto _out;} }
				
#line 1137 "args/argcheck2.cpp"
				
				goto _st52;
				_ctr61:
				{
#line 180 "args/argcheck2.rl"
					
					double real = rl_num;
					if (rl_den_cnt)
					real += rl_den / double(rl_den_cnt);
					
					rl_chk.values.push_back(rl_sign * real);
				}
				
#line 1151 "args/argcheck2.cpp"
				
				{
#line 296 "args/argcheck2.rl"
					rl_chk.rmin = 1; rl_chk.rmax = 1; }
				
#line 1157 "args/argcheck2.cpp"
				
				{
#line 165 "args/argcheck2.rl"
					
					if (chk_)
					chk_->push_back(rl_chk);
					
					rl_num = 0;
					rl_den = 0;
					rl_sign = 0;
					rl_den_cnt = 0;
					rl_chk = {};
				}
				
#line 1172 "args/argcheck2.cpp"
				
				{
#line 301 "args/argcheck2.rl"
					{p+= 1; cs = 52; goto _out;} }
				
#line 1178 "args/argcheck2.cpp"
				
				goto _st52;
				_ctr92:
				{
#line 176 "args/argcheck2.rl"
					
					rl_chk.values.push_back((int64_t)(rl_sign * rl_num));
				}
				
#line 1188 "args/argcheck2.cpp"
				
				{
#line 296 "args/argcheck2.rl"
					rl_chk.rmin = 1; rl_chk.rmax = 1; }
				
#line 1194 "args/argcheck2.cpp"
				
				{
#line 165 "args/argcheck2.rl"
					
					if (chk_)
					chk_->push_back(rl_chk);
					
					rl_num = 0;
					rl_den = 0;
					rl_sign = 0;
					rl_den_cnt = 0;
					rl_chk = {};
				}
				
#line 1209 "args/argcheck2.cpp"
				
				{
#line 301 "args/argcheck2.rl"
					{p+= 1; cs = 52; goto _out;} }
				
#line 1215 "args/argcheck2.cpp"
				
				goto _st52;
				_ctr138:
				{
#line 188 "args/argcheck2.rl"
					
					try {
						ArgString str{ {}, 0 };
						str.first.assign(rl_sym_start, p - rl_sym_start);
						str.second = crc32_hash(str.first.data());
						rl_chk.values.push_back(str);
					} catch(std::exception& e) {
						LIB_ERR << "exception: " << e.what();
					}
				}
				
#line 1232 "args/argcheck2.cpp"
				
				{
#line 296 "args/argcheck2.rl"
					rl_chk.rmin = 1; rl_chk.rmax = 1; }
				
#line 1238 "args/argcheck2.cpp"
				
				{
#line 165 "args/argcheck2.rl"
					
					if (chk_)
					chk_->push_back(rl_chk);
					
					rl_num = 0;
					rl_den = 0;
					rl_sign = 0;
					rl_den_cnt = 0;
					rl_chk = {};
				}
				
#line 1253 "args/argcheck2.cpp"
				
				{
#line 301 "args/argcheck2.rl"
					{p+= 1; cs = 52; goto _out;} }
				
#line 1259 "args/argcheck2.cpp"
				
				goto _st52;
				_st52:
				p += 1;
				st_case_52:
				{
					goto _st0;
				}
				_ctr29:
				{
#line 296 "args/argcheck2.rl"
					rl_chk.rmin = 1; rl_chk.rmax = 1; }
				
#line 1273 "args/argcheck2.cpp"
				
				{
#line 283 "args/argcheck2.rl"
					rl_chk.setRepeats(0, REPEAT_INF); }
				
#line 1279 "args/argcheck2.cpp"
				
				goto _st12;
				_ctr30:
				{
#line 296 "args/argcheck2.rl"
					rl_chk.rmin = 1; rl_chk.rmax = 1; }
				
#line 1287 "args/argcheck2.cpp"
				
				{
#line 282 "args/argcheck2.rl"
					rl_chk.setRepeats(1, REPEAT_INF); }
				
#line 1293 "args/argcheck2.cpp"
				
				goto _st12;
				_ctr31:
				{
#line 296 "args/argcheck2.rl"
					rl_chk.rmin = 1; rl_chk.rmax = 1; }
				
#line 1301 "args/argcheck2.cpp"
				
				{
#line 281 "args/argcheck2.rl"
					rl_chk.setRepeats(0, 1); }
				
#line 1307 "args/argcheck2.cpp"
				
				goto _st12;
				_ctr41:
				{
#line 204 "args/argcheck2.rl"
					rl_chk.rmax = rl_chk.rmin; }
				
#line 1315 "args/argcheck2.cpp"
				
				goto _st12;
				_ctr63:
				{
#line 180 "args/argcheck2.rl"
					
					double real = rl_num;
					if (rl_den_cnt)
					real += rl_den / double(rl_den_cnt);
					
					rl_chk.values.push_back(rl_sign * real);
				}
				
#line 1329 "args/argcheck2.cpp"
				
				{
#line 296 "args/argcheck2.rl"
					rl_chk.rmin = 1; rl_chk.rmax = 1; }
				
#line 1335 "args/argcheck2.cpp"
				
				{
#line 283 "args/argcheck2.rl"
					rl_chk.setRepeats(0, REPEAT_INF); }
				
#line 1341 "args/argcheck2.cpp"
				
				goto _st12;
				_ctr64:
				{
#line 180 "args/argcheck2.rl"
					
					double real = rl_num;
					if (rl_den_cnt)
					real += rl_den / double(rl_den_cnt);
					
					rl_chk.values.push_back(rl_sign * real);
				}
				
#line 1355 "args/argcheck2.cpp"
				
				{
#line 296 "args/argcheck2.rl"
					rl_chk.rmin = 1; rl_chk.rmax = 1; }
				
#line 1361 "args/argcheck2.cpp"
				
				{
#line 282 "args/argcheck2.rl"
					rl_chk.setRepeats(1, REPEAT_INF); }
				
#line 1367 "args/argcheck2.cpp"
				
				goto _st12;
				_ctr67:
				{
#line 180 "args/argcheck2.rl"
					
					double real = rl_num;
					if (rl_den_cnt)
					real += rl_den / double(rl_den_cnt);
					
					rl_chk.values.push_back(rl_sign * real);
				}
				
#line 1381 "args/argcheck2.cpp"
				
				{
#line 296 "args/argcheck2.rl"
					rl_chk.rmin = 1; rl_chk.rmax = 1; }
				
#line 1387 "args/argcheck2.cpp"
				
				{
#line 281 "args/argcheck2.rl"
					rl_chk.setRepeats(0, 1); }
				
#line 1393 "args/argcheck2.cpp"
				
				goto _st12;
				_ctr94:
				{
#line 176 "args/argcheck2.rl"
					
					rl_chk.values.push_back((int64_t)(rl_sign * rl_num));
				}
				
#line 1403 "args/argcheck2.cpp"
				
				{
#line 296 "args/argcheck2.rl"
					rl_chk.rmin = 1; rl_chk.rmax = 1; }
				
#line 1409 "args/argcheck2.cpp"
				
				{
#line 283 "args/argcheck2.rl"
					rl_chk.setRepeats(0, REPEAT_INF); }
				
#line 1415 "args/argcheck2.cpp"
				
				goto _st12;
				_ctr95:
				{
#line 176 "args/argcheck2.rl"
					
					rl_chk.values.push_back((int64_t)(rl_sign * rl_num));
				}
				
#line 1425 "args/argcheck2.cpp"
				
				{
#line 296 "args/argcheck2.rl"
					rl_chk.rmin = 1; rl_chk.rmax = 1; }
				
#line 1431 "args/argcheck2.cpp"
				
				{
#line 282 "args/argcheck2.rl"
					rl_chk.setRepeats(1, REPEAT_INF); }
				
#line 1437 "args/argcheck2.cpp"
				
				goto _st12;
				_ctr97:
				{
#line 176 "args/argcheck2.rl"
					
					rl_chk.values.push_back((int64_t)(rl_sign * rl_num));
				}
				
#line 1447 "args/argcheck2.cpp"
				
				{
#line 296 "args/argcheck2.rl"
					rl_chk.rmin = 1; rl_chk.rmax = 1; }
				
#line 1453 "args/argcheck2.cpp"
				
				{
#line 281 "args/argcheck2.rl"
					rl_chk.setRepeats(0, 1); }
				
#line 1459 "args/argcheck2.cpp"
				
				goto _st12;
				_ctr140:
				{
#line 188 "args/argcheck2.rl"
					
					try {
						ArgString str{ {}, 0 };
						str.first.assign(rl_sym_start, p - rl_sym_start);
						str.second = crc32_hash(str.first.data());
						rl_chk.values.push_back(str);
					} catch(std::exception& e) {
						LIB_ERR << "exception: " << e.what();
					}
				}
				
#line 1476 "args/argcheck2.cpp"
				
				{
#line 296 "args/argcheck2.rl"
					rl_chk.rmin = 1; rl_chk.rmax = 1; }
				
#line 1482 "args/argcheck2.cpp"
				
				{
#line 283 "args/argcheck2.rl"
					rl_chk.setRepeats(0, REPEAT_INF); }
				
#line 1488 "args/argcheck2.cpp"
				
				goto _st12;
				_ctr141:
				{
#line 188 "args/argcheck2.rl"
					
					try {
						ArgString str{ {}, 0 };
						str.first.assign(rl_sym_start, p - rl_sym_start);
						str.second = crc32_hash(str.first.data());
						rl_chk.values.push_back(str);
					} catch(std::exception& e) {
						LIB_ERR << "exception: " << e.what();
					}
				}
				
#line 1505 "args/argcheck2.cpp"
				
				{
#line 296 "args/argcheck2.rl"
					rl_chk.rmin = 1; rl_chk.rmax = 1; }
				
#line 1511 "args/argcheck2.cpp"
				
				{
#line 282 "args/argcheck2.rl"
					rl_chk.setRepeats(1, REPEAT_INF); }
				
#line 1517 "args/argcheck2.cpp"
				
				goto _st12;
				_ctr142:
				{
#line 188 "args/argcheck2.rl"
					
					try {
						ArgString str{ {}, 0 };
						str.first.assign(rl_sym_start, p - rl_sym_start);
						str.second = crc32_hash(str.first.data());
						rl_chk.values.push_back(str);
					} catch(std::exception& e) {
						LIB_ERR << "exception: " << e.what();
					}
				}
				
#line 1534 "args/argcheck2.cpp"
				
				{
#line 296 "args/argcheck2.rl"
					rl_chk.rmin = 1; rl_chk.rmax = 1; }
				
#line 1540 "args/argcheck2.cpp"
				
				{
#line 281 "args/argcheck2.rl"
					rl_chk.setRepeats(0, 1); }
				
#line 1546 "args/argcheck2.cpp"
				
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
#line 296 "args/argcheck2.rl"
					rl_chk.rmin = 1; rl_chk.rmax = 1; }
				
#line 1568 "args/argcheck2.cpp"
				
				{
#line 205 "args/argcheck2.rl"
					rl_chk.rmin = 0; rl_chk.rmax = REPEAT_INF; }
				
#line 1574 "args/argcheck2.cpp"
				
				goto _st13;
				_ctr68:
				{
#line 180 "args/argcheck2.rl"
					
					double real = rl_num;
					if (rl_den_cnt)
					real += rl_den / double(rl_den_cnt);
					
					rl_chk.values.push_back(rl_sign * real);
				}
				
#line 1588 "args/argcheck2.cpp"
				
				{
#line 296 "args/argcheck2.rl"
					rl_chk.rmin = 1; rl_chk.rmax = 1; }
				
#line 1594 "args/argcheck2.cpp"
				
				{
#line 205 "args/argcheck2.rl"
					rl_chk.rmin = 0; rl_chk.rmax = REPEAT_INF; }
				
#line 1600 "args/argcheck2.cpp"
				
				goto _st13;
				_ctr98:
				{
#line 176 "args/argcheck2.rl"
					
					rl_chk.values.push_back((int64_t)(rl_sign * rl_num));
				}
				
#line 1610 "args/argcheck2.cpp"
				
				{
#line 296 "args/argcheck2.rl"
					rl_chk.rmin = 1; rl_chk.rmax = 1; }
				
#line 1616 "args/argcheck2.cpp"
				
				{
#line 205 "args/argcheck2.rl"
					rl_chk.rmin = 0; rl_chk.rmax = REPEAT_INF; }
				
#line 1622 "args/argcheck2.cpp"
				
				goto _st13;
				_ctr143:
				{
#line 188 "args/argcheck2.rl"
					
					try {
						ArgString str{ {}, 0 };
						str.first.assign(rl_sym_start, p - rl_sym_start);
						str.second = crc32_hash(str.first.data());
						rl_chk.values.push_back(str);
					} catch(std::exception& e) {
						LIB_ERR << "exception: " << e.what();
					}
				}
				
#line 1639 "args/argcheck2.cpp"
				
				{
#line 296 "args/argcheck2.rl"
					rl_chk.rmin = 1; rl_chk.rmax = 1; }
				
#line 1645 "args/argcheck2.cpp"
				
				{
#line 205 "args/argcheck2.rl"
					rl_chk.rmin = 0; rl_chk.rmax = REPEAT_INF; }
				
#line 1651 "args/argcheck2.cpp"
				
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
#line 202 "args/argcheck2.rl"
					rl_chk.rmin = 0; }
				
#line 1671 "args/argcheck2.cpp"
				
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
#line 203 "args/argcheck2.rl"
					rl_chk.rmax = 0; }
				
#line 1710 "args/argcheck2.cpp"
				
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
#line 203 "args/argcheck2.rl"
					rl_chk.rmax = (( (*( p))))-'0'; }
				
#line 1727 "args/argcheck2.cpp"
				
				goto _st17;
				_ctr46:
				{
#line 203 "args/argcheck2.rl"
					(rl_chk.rmax *= 10) += ((( (*( p)))) - '0'); }
				
#line 1735 "args/argcheck2.cpp"
				
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
#line 202 "args/argcheck2.rl"
					rl_chk.rmin = (( (*( p))))-'0'; }
				
#line 1755 "args/argcheck2.cpp"
				
				goto _st18;
				_ctr48:
				{
#line 202 "args/argcheck2.rl"
					(rl_chk.rmin *= 10) += ((( (*( p)))) - '0'); }
				
#line 1763 "args/argcheck2.cpp"
				
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
#line 280 "args/argcheck2.rl"
					rl_chk.type = CHECK_FLOAT; }
				
#line 1788 "args/argcheck2.cpp"
				
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
#line 241 "args/argcheck2.rl"
					rl_chk.cmp = CMP_NOT_EQUAL; }
				
#line 1840 "args/argcheck2.cpp"
				
				goto _st21;
				_ctr73:
				{
#line 240 "args/argcheck2.rl"
					rl_chk.cmp = CMP_LESS_EQ; }
				
#line 1848 "args/argcheck2.cpp"
				
				goto _st21;
				_ctr75:
				{
#line 239 "args/argcheck2.rl"
					rl_chk.cmp = CMP_GREATER_EQ; }
				
#line 1856 "args/argcheck2.cpp"
				
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
#line 216 "args/argcheck2.rl"
					rl_sign = 1; }
				
#line 1881 "args/argcheck2.cpp"
				
				{
#line 207 "args/argcheck2.rl"
					rl_sign = 1; }
				
#line 1887 "args/argcheck2.cpp"
				
				goto _st22;
				_ctr56:
				{
#line 216 "args/argcheck2.rl"
					rl_sign = 1; }
				
#line 1895 "args/argcheck2.cpp"
				
				{
#line 208 "args/argcheck2.rl"
					rl_sign = -1; }
				
#line 1901 "args/argcheck2.cpp"
				
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
#line 216 "args/argcheck2.rl"
					rl_sign = 1; }
				
#line 1918 "args/argcheck2.cpp"
				
				{
#line 210 "args/argcheck2.rl"
					rl_num = 0; }
				
#line 1924 "args/argcheck2.cpp"
				
				{
#line 210 "args/argcheck2.rl"
					(rl_num *= 10) += ((( (*( p)))) - '0'); }
				
#line 1930 "args/argcheck2.cpp"
				
				goto _st23;
				_ctr59:
				{
#line 210 "args/argcheck2.rl"
					rl_num = 0; }
				
#line 1938 "args/argcheck2.cpp"
				
				{
#line 210 "args/argcheck2.rl"
					(rl_num *= 10) += ((( (*( p)))) - '0'); }
				
#line 1944 "args/argcheck2.cpp"
				
				goto _st23;
				_ctr66:
				{
#line 210 "args/argcheck2.rl"
					(rl_num *= 10) += ((( (*( p)))) - '0'); }
				
#line 1952 "args/argcheck2.cpp"
				
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
#line 211 "args/argcheck2.rl"
					rl_den = 0; rl_den_cnt = 1; }
				
#line 2001 "args/argcheck2.cpp"
				
				{
#line 211 "args/argcheck2.rl"
					(rl_den *= 10) += ((( (*( p)))) - '0'); rl_den_cnt *= 10; }
				
#line 2007 "args/argcheck2.cpp"
				
				goto _st25;
				_ctr71:
				{
#line 211 "args/argcheck2.rl"
					(rl_den *= 10) += ((( (*( p)))) - '0'); rl_den_cnt *= 10; }
				
#line 2015 "args/argcheck2.cpp"
				
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
#line 240 "args/argcheck2.rl"
					rl_chk.cmp = CMP_LESS; }
				
#line 2052 "args/argcheck2.cpp"
				
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
#line 239 "args/argcheck2.rl"
					rl_chk.cmp = CMP_GREATER; }
				
#line 2080 "args/argcheck2.cpp"
				
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
#line 278 "args/argcheck2.rl"
					rl_chk.type = CHECK_INT; }
				
#line 2108 "args/argcheck2.cpp"
				
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
#line 241 "args/argcheck2.rl"
					rl_chk.cmp = CMP_NOT_EQUAL; }
				
#line 2172 "args/argcheck2.cpp"
				
				goto _st30;
				_ctr104:
				{
#line 240 "args/argcheck2.rl"
					rl_chk.cmp = CMP_LESS_EQ; }
				
#line 2180 "args/argcheck2.cpp"
				
				goto _st30;
				_ctr114:
				{
#line 239 "args/argcheck2.rl"
					rl_chk.cmp = CMP_GREATER_EQ; }
				
#line 2188 "args/argcheck2.cpp"
				
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
#line 216 "args/argcheck2.rl"
					rl_sign = 1; }
				
#line 2213 "args/argcheck2.cpp"
				
				{
#line 207 "args/argcheck2.rl"
					rl_sign = 1; }
				
#line 2219 "args/argcheck2.cpp"
				
				goto _st31;
				_ctr87:
				{
#line 216 "args/argcheck2.rl"
					rl_sign = 1; }
				
#line 2227 "args/argcheck2.cpp"
				
				{
#line 208 "args/argcheck2.rl"
					rl_sign = -1; }
				
#line 2233 "args/argcheck2.cpp"
				
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
#line 216 "args/argcheck2.rl"
					rl_sign = 1; }
				
#line 2250 "args/argcheck2.cpp"
				
				{
#line 210 "args/argcheck2.rl"
					rl_num = 0; }
				
#line 2256 "args/argcheck2.cpp"
				
				{
#line 210 "args/argcheck2.rl"
					(rl_num *= 10) += ((( (*( p)))) - '0'); }
				
#line 2262 "args/argcheck2.cpp"
				
				goto _st32;
				_ctr90:
				{
#line 210 "args/argcheck2.rl"
					rl_num = 0; }
				
#line 2270 "args/argcheck2.cpp"
				
				{
#line 210 "args/argcheck2.rl"
					(rl_num *= 10) += ((( (*( p)))) - '0'); }
				
#line 2276 "args/argcheck2.cpp"
				
				goto _st32;
				_ctr96:
				{
#line 210 "args/argcheck2.rl"
					(rl_num *= 10) += ((( (*( p)))) - '0'); }
				
#line 2284 "args/argcheck2.cpp"
				
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
#line 246 "args/argcheck2.rl"
					rl_chk.cmp = CMP_MODULE; }
				
#line 2321 "args/argcheck2.cpp"
				
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
#line 247 "args/argcheck2.rl"
					rl_sign = 1; rl_num = 0; }
				
#line 2338 "args/argcheck2.cpp"
				
				{
#line 247 "args/argcheck2.rl"
					(rl_num *= 10) += ((( (*( p)))) - '0'); }
				
#line 2344 "args/argcheck2.cpp"
				
				{
#line 176 "args/argcheck2.rl"
					
					rl_chk.values.push_back((int64_t)(rl_sign * rl_num));
				}
				
#line 2352 "args/argcheck2.cpp"
				
				goto _st34;
				_ctr102:
				{
#line 247 "args/argcheck2.rl"
					(rl_num *= 10) += ((( (*( p)))) - '0'); }
				
#line 2360 "args/argcheck2.cpp"
				
				{
#line 176 "args/argcheck2.rl"
					
					rl_chk.values.push_back((int64_t)(rl_sign * rl_num));
				}
				
#line 2368 "args/argcheck2.cpp"
				
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
#line 240 "args/argcheck2.rl"
					rl_chk.cmp = CMP_LESS; }
				
#line 2405 "args/argcheck2.cpp"
				
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
#line 236 "args/argcheck2.rl"
					rl_chk.cmp = CMP_EQUAL; }
				
#line 2433 "args/argcheck2.cpp"
				
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
#line 216 "args/argcheck2.rl"
					rl_sign = 1; }
				
#line 2458 "args/argcheck2.cpp"
				
				{
#line 207 "args/argcheck2.rl"
					rl_sign = 1; }
				
#line 2464 "args/argcheck2.cpp"
				
				goto _st37;
				_ctr107:
				{
#line 216 "args/argcheck2.rl"
					rl_sign = 1; }
				
#line 2472 "args/argcheck2.cpp"
				
				{
#line 208 "args/argcheck2.rl"
					rl_sign = -1; }
				
#line 2478 "args/argcheck2.cpp"
				
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
#line 216 "args/argcheck2.rl"
					rl_sign = 1; }
				
#line 2495 "args/argcheck2.cpp"
				
				{
#line 210 "args/argcheck2.rl"
					rl_num = 0; }
				
#line 2501 "args/argcheck2.cpp"
				
				{
#line 210 "args/argcheck2.rl"
					(rl_num *= 10) += ((( (*( p)))) - '0'); }
				
#line 2507 "args/argcheck2.cpp"
				
				{
#line 176 "args/argcheck2.rl"
					
					rl_chk.values.push_back((int64_t)(rl_sign * rl_num));
				}
				
#line 2515 "args/argcheck2.cpp"
				
				goto _st38;
				_ctr110:
				{
#line 210 "args/argcheck2.rl"
					rl_num = 0; }
				
#line 2523 "args/argcheck2.cpp"
				
				{
#line 210 "args/argcheck2.rl"
					(rl_num *= 10) += ((( (*( p)))) - '0'); }
				
#line 2529 "args/argcheck2.cpp"
				
				{
#line 176 "args/argcheck2.rl"
					
					rl_chk.values.push_back((int64_t)(rl_sign * rl_num));
				}
				
#line 2537 "args/argcheck2.cpp"
				
				goto _st38;
				_ctr112:
				{
#line 210 "args/argcheck2.rl"
					(rl_num *= 10) += ((( (*( p)))) - '0'); }
				
#line 2545 "args/argcheck2.cpp"
				
				{
#line 176 "args/argcheck2.rl"
					
					rl_chk.values.push_back((int64_t)(rl_sign * rl_num));
				}
				
#line 2553 "args/argcheck2.cpp"
				
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
#line 239 "args/argcheck2.rl"
					rl_chk.cmp = CMP_GREATER; }
				
#line 2593 "args/argcheck2.cpp"
				
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
#line 216 "args/argcheck2.rl"
					rl_sign = 1; }
				
#line 2638 "args/argcheck2.cpp"
				
				{
#line 207 "args/argcheck2.rl"
					rl_sign = 1; }
				
#line 2644 "args/argcheck2.cpp"
				
				goto _st41;
				_ctr116:
				{
#line 216 "args/argcheck2.rl"
					rl_sign = 1; }
				
#line 2652 "args/argcheck2.cpp"
				
				{
#line 208 "args/argcheck2.rl"
					rl_sign = -1; }
				
#line 2658 "args/argcheck2.cpp"
				
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
#line 216 "args/argcheck2.rl"
					rl_sign = 1; }
				
#line 2675 "args/argcheck2.cpp"
				
				{
#line 210 "args/argcheck2.rl"
					rl_num = 0; }
				
#line 2681 "args/argcheck2.cpp"
				
				{
#line 210 "args/argcheck2.rl"
					(rl_num *= 10) += ((( (*( p)))) - '0'); }
				
#line 2687 "args/argcheck2.cpp"
				
				{
#line 176 "args/argcheck2.rl"
					
					rl_chk.values.push_back((int64_t)(rl_sign * rl_num));
				}
				
#line 2695 "args/argcheck2.cpp"
				
				goto _st42;
				_ctr119:
				{
#line 210 "args/argcheck2.rl"
					rl_num = 0; }
				
#line 2703 "args/argcheck2.cpp"
				
				{
#line 210 "args/argcheck2.rl"
					(rl_num *= 10) += ((( (*( p)))) - '0'); }
				
#line 2709 "args/argcheck2.cpp"
				
				{
#line 176 "args/argcheck2.rl"
					
					rl_chk.values.push_back((int64_t)(rl_sign * rl_num));
				}
				
#line 2717 "args/argcheck2.cpp"
				
				goto _st42;
				_ctr122:
				{
#line 210 "args/argcheck2.rl"
					(rl_num *= 10) += ((( (*( p)))) - '0'); }
				
#line 2725 "args/argcheck2.cpp"
				
				{
#line 176 "args/argcheck2.rl"
					
					rl_chk.values.push_back((int64_t)(rl_sign * rl_num));
				}
				
#line 2733 "args/argcheck2.cpp"
				
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
#line 216 "args/argcheck2.rl"
					rl_sign = 1; }
				
#line 2770 "args/argcheck2.cpp"
				
				{
#line 207 "args/argcheck2.rl"
					rl_sign = 1; }
				
#line 2776 "args/argcheck2.cpp"
				
				goto _st44;
				_ctr124:
				{
#line 216 "args/argcheck2.rl"
					rl_sign = 1; }
				
#line 2784 "args/argcheck2.cpp"
				
				{
#line 208 "args/argcheck2.rl"
					rl_sign = -1; }
				
#line 2790 "args/argcheck2.cpp"
				
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
#line 216 "args/argcheck2.rl"
					rl_sign = 1; }
				
#line 2807 "args/argcheck2.cpp"
				
				{
#line 210 "args/argcheck2.rl"
					rl_num = 0; }
				
#line 2813 "args/argcheck2.cpp"
				
				{
#line 210 "args/argcheck2.rl"
					(rl_num *= 10) += ((( (*( p)))) - '0'); }
				
#line 2819 "args/argcheck2.cpp"
				
				{
#line 176 "args/argcheck2.rl"
					
					rl_chk.values.push_back((int64_t)(rl_sign * rl_num));
				}
				
#line 2827 "args/argcheck2.cpp"
				
				goto _st45;
				_ctr127:
				{
#line 210 "args/argcheck2.rl"
					rl_num = 0; }
				
#line 2835 "args/argcheck2.cpp"
				
				{
#line 210 "args/argcheck2.rl"
					(rl_num *= 10) += ((( (*( p)))) - '0'); }
				
#line 2841 "args/argcheck2.cpp"
				
				{
#line 176 "args/argcheck2.rl"
					
					rl_chk.values.push_back((int64_t)(rl_sign * rl_num));
				}
				
#line 2849 "args/argcheck2.cpp"
				
				goto _st45;
				_ctr130:
				{
#line 210 "args/argcheck2.rl"
					(rl_num *= 10) += ((( (*( p)))) - '0'); }
				
#line 2857 "args/argcheck2.cpp"
				
				{
#line 176 "args/argcheck2.rl"
					
					rl_chk.values.push_back((int64_t)(rl_sign * rl_num));
				}
				
#line 2865 "args/argcheck2.cpp"
				
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
#line 279 "args/argcheck2.rl"
					rl_chk.type = CHECK_SYMBOL; }
				
#line 2899 "args/argcheck2.cpp"
				
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
#line 269 "args/argcheck2.rl"
					rl_chk.cmp = CMP_EQUAL; }
				
#line 2936 "args/argcheck2.cpp"
				
				goto _st48;
				_ctr144:
				{
#line 188 "args/argcheck2.rl"
					
					try {
						ArgString str{ {}, 0 };
						str.first.assign(rl_sym_start, p - rl_sym_start);
						str.second = crc32_hash(str.first.data());
						rl_chk.values.push_back(str);
					} catch(std::exception& e) {
						LIB_ERR << "exception: " << e.what();
					}
				}
				
#line 2953 "args/argcheck2.cpp"
				
				goto _st48;
				_st48:
				p += 1;
				st_case_48:
				switch( ( (*( p))) ) {
					case 35: {
						goto _ctr136;
					}
					case 45: {
						goto _ctr136;
					}
					case 95: {
						goto _ctr136;
					}
				}
				if ( ( (*( p))) < 64 ) {
					if ( 48 <= ( (*( p))) && ( (*( p))) <= 58 ) {
						goto _ctr136;
					}
				} else if ( ( (*( p))) > 90 ) {
					if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
						goto _ctr136;
					}
				} else {
					goto _ctr136;
				}
				{
					goto _st0;
				}
				_ctr136:
				{
#line 262 "args/argcheck2.rl"
					rl_sym_start = p; }
				
#line 2989 "args/argcheck2.cpp"
				
				goto _st49;
				_st49:
				p += 1;
				st_case_49:
				switch( ( (*( p))) ) {
					case 0: {
						goto _ctr138;
					}
					case 32: {
						goto _ctr139;
					}
					case 35: {
						goto _st49;
					}
					case 42: {
						goto _ctr140;
					}
					case 43: {
						goto _ctr141;
					}
					case 45: {
						goto _st49;
					}
					case 63: {
						goto _ctr142;
					}
					case 95: {
						goto _st49;
					}
					case 123: {
						goto _ctr143;
					}
					case 124: {
						goto _ctr144;
					}
				}
				if ( ( (*( p))) < 64 ) {
					if ( 48 <= ( (*( p))) && ( (*( p))) <= 58 ) {
						goto _st49;
					}
				} else if ( ( (*( p))) > 90 ) {
					if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
						goto _st49;
					}
				} else {
					goto _st49;
				}
				{
					goto _st0;
				}
				_ctr3:
				{
#line 287 "args/argcheck2.rl"
					rl_chk.name.clear(); }
				
#line 3046 "args/argcheck2.cpp"
				
				{
#line 287 "args/argcheck2.rl"
					rl_chk.name.push_back((( (*( p))))); }
				
#line 3052 "args/argcheck2.cpp"
				
				{
#line 276 "args/argcheck2.rl"
					rl_chk.type = CHECK_BOOL; }
				
#line 3058 "args/argcheck2.cpp"
				
				goto _st50;
				_st50:
				p += 1;
				st_case_50:
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
						goto _ctr146;
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
				_ctr146:
				{
#line 287 "args/argcheck2.rl"
					rl_chk.name.push_back((( (*( p))))); }
				
#line 3105 "args/argcheck2.cpp"
				
				{
#line 296 "args/argcheck2.rl"
					rl_chk.rmin = 1; rl_chk.rmax = 1; }
				
#line 3111 "args/argcheck2.cpp"
				
				{
#line 281 "args/argcheck2.rl"
					rl_chk.setRepeats(0, 1); }
				
#line 3117 "args/argcheck2.cpp"
				
				goto _st51;
				_st51:
				p += 1;
				st_case_51:
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
				_test_eof52: cs = 52; goto _test_eof; 
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
				
				_test_eof: {}
				if ( cs >= 52 )
					goto _out; _pop: {}
				_out: {}
			}
			
#line 595 "args/argcheck2.rl"
			
			
			if (cs < 
#line 3211 "args/argcheck2.cpp"
			52
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
