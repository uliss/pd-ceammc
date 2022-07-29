#line 1 "args/argcheck2.rl"
# include "argcheck2.h"
# include "ceammc_string.h"
# include "ceammc_object.h"
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
	};
	
	class ArgStringVisitor : public boost::static_visitor<std::string>
	{
		public:
		std::string operator()(const double& d) const { return std::to_string(d); }
		std::string operator()(const int64_t& i) const { return std::to_string(i); }
		std::string operator()(const ArgString& s) const { return s.first.data(); }
	};
	
	inline std::string arg_to_string(const ArgValue& v)
	{
		return boost::apply_visitor(ArgStringVisitor(), v );
	}
	
	inline std::string arg_to_string(const ArgList& v)
	{
		std::string res;
		ArgStringVisitor visitor;
		for (auto& a: v) {
			if (&a != &v[0]) {
				res += ',';
				res += ' ';
			}
			res += boost::apply_visitor(visitor, a);
		}
		
		return res;
	}
}


#line 118 "args/argcheck2.cpp"
static const int arg_check2_start = 1;
static const int arg_check2_first_final = 24;
static const int arg_check2_error = 0;

static const int arg_check2_en_main = 1;


#line 198 "args/argcheck2.rl"


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
						const int64_t arg = (c.values.size() >= 1 && boost::get<int64_t>(&c.values[0]))
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
							default:
							break;
						}
						debug("int", "Ok");
						i++;
					}
					break;
					case CHECK_SYMBOL:
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
			
			
#line 316 "args/argcheck2.cpp"
			{
				cs = (int)arg_check2_start;
			}
			
#line 321 "args/argcheck2.cpp"
			{
				switch ( cs ) {
					case 1:
					goto st_case_1;
					case 0:
					goto st_case_0;
					case 2:
					goto st_case_2;
					case 24:
					goto st_case_24;
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
				}
				goto st_out;
				_ctr8:
				{
#line 189 "args/argcheck2.rl"
					rl_min = 1; rl_max = REPEAT_INF; }
				
#line 381 "args/argcheck2.cpp"
				
				{
#line 118 "args/argcheck2.rl"
					
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
				
#line 403 "args/argcheck2.cpp"
				
				goto _st1;
				_ctr15:
				{
#line 118 "args/argcheck2.rl"
					
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
				
#line 427 "args/argcheck2.cpp"
				
				goto _st1;
				_ctr46:
				{
#line 136 "args/argcheck2.rl"
					
					rl_check.values.push_back((int64_t)(rl_sign * rl_num));
				}
				
#line 437 "args/argcheck2.cpp"
				
				{
#line 189 "args/argcheck2.rl"
					rl_min = 1; rl_max = REPEAT_INF; }
				
#line 443 "args/argcheck2.cpp"
				
				{
#line 118 "args/argcheck2.rl"
					
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
				
#line 465 "args/argcheck2.cpp"
				
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
#line 176 "args/argcheck2.rl"
					rl_type = CHECK_BOOL; }
				
#line 497 "args/argcheck2.cpp"
				
				goto _st2;
				_ctr3:
				{
#line 175 "args/argcheck2.rl"
					rl_type = CHECK_ATOM; }
				
#line 505 "args/argcheck2.cpp"
				
				goto _st2;
				_ctr4:
				{
#line 177 "args/argcheck2.rl"
					rl_type = CHECK_BYTE; }
				
#line 513 "args/argcheck2.cpp"
				
				goto _st2;
				_ctr68:
				{
#line 167 "args/argcheck2.rl"
					rl_cmp = CMP_POWER2; }
				
#line 521 "args/argcheck2.cpp"
				
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
#line 189 "args/argcheck2.rl"
					rl_min = 1; rl_max = REPEAT_INF; }
				
#line 555 "args/argcheck2.cpp"
				
				{
#line 118 "args/argcheck2.rl"
					
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
				
#line 577 "args/argcheck2.cpp"
				
				{
#line 194 "args/argcheck2.rl"
					{p+= 1; cs = 24; goto _out;} }
				
#line 583 "args/argcheck2.cpp"
				
				goto _st24;
				_ctr14:
				{
#line 118 "args/argcheck2.rl"
					
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
				
#line 607 "args/argcheck2.cpp"
				
				{
#line 194 "args/argcheck2.rl"
					{p+= 1; cs = 24; goto _out;} }
				
#line 613 "args/argcheck2.cpp"
				
				goto _st24;
				_ctr45:
				{
#line 136 "args/argcheck2.rl"
					
					rl_check.values.push_back((int64_t)(rl_sign * rl_num));
				}
				
#line 623 "args/argcheck2.cpp"
				
				{
#line 189 "args/argcheck2.rl"
					rl_min = 1; rl_max = REPEAT_INF; }
				
#line 629 "args/argcheck2.cpp"
				
				{
#line 118 "args/argcheck2.rl"
					
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
				
#line 651 "args/argcheck2.cpp"
				
				{
#line 194 "args/argcheck2.rl"
					{p+= 1; cs = 24; goto _out;} }
				
#line 657 "args/argcheck2.cpp"
				
				goto _st24;
				_st24:
				p += 1;
				st_case_24:
				{
					goto _st0;
				}
				_ctr9:
				{
#line 189 "args/argcheck2.rl"
					rl_min = 1; rl_max = REPEAT_INF; }
				
#line 671 "args/argcheck2.cpp"
				
				{
#line 181 "args/argcheck2.rl"
					rl_min = 0; rl_max = REPEAT_INF; }
				
#line 677 "args/argcheck2.cpp"
				
				goto _st3;
				_ctr10:
				{
#line 189 "args/argcheck2.rl"
					rl_min = 1; rl_max = REPEAT_INF; }
				
#line 685 "args/argcheck2.cpp"
				
				{
#line 180 "args/argcheck2.rl"
					rl_min = 1, rl_max = REPEAT_INF; }
				
#line 691 "args/argcheck2.cpp"
				
				goto _st3;
				_ctr11:
				{
#line 189 "args/argcheck2.rl"
					rl_min = 1; rl_max = REPEAT_INF; }
				
#line 699 "args/argcheck2.cpp"
				
				{
#line 179 "args/argcheck2.rl"
					rl_min = 0; rl_max = 1; }
				
#line 705 "args/argcheck2.cpp"
				
				goto _st3;
				_ctr21:
				{
#line 142 "args/argcheck2.rl"
					rl_max = rl_min; }
				
#line 713 "args/argcheck2.cpp"
				
				goto _st3;
				_ctr47:
				{
#line 136 "args/argcheck2.rl"
					
					rl_check.values.push_back((int64_t)(rl_sign * rl_num));
				}
				
#line 723 "args/argcheck2.cpp"
				
				{
#line 189 "args/argcheck2.rl"
					rl_min = 1; rl_max = REPEAT_INF; }
				
#line 729 "args/argcheck2.cpp"
				
				{
#line 181 "args/argcheck2.rl"
					rl_min = 0; rl_max = REPEAT_INF; }
				
#line 735 "args/argcheck2.cpp"
				
				goto _st3;
				_ctr48:
				{
#line 136 "args/argcheck2.rl"
					
					rl_check.values.push_back((int64_t)(rl_sign * rl_num));
				}
				
#line 745 "args/argcheck2.cpp"
				
				{
#line 189 "args/argcheck2.rl"
					rl_min = 1; rl_max = REPEAT_INF; }
				
#line 751 "args/argcheck2.cpp"
				
				{
#line 180 "args/argcheck2.rl"
					rl_min = 1, rl_max = REPEAT_INF; }
				
#line 757 "args/argcheck2.cpp"
				
				goto _st3;
				_ctr50:
				{
#line 136 "args/argcheck2.rl"
					
					rl_check.values.push_back((int64_t)(rl_sign * rl_num));
				}
				
#line 767 "args/argcheck2.cpp"
				
				{
#line 189 "args/argcheck2.rl"
					rl_min = 1; rl_max = REPEAT_INF; }
				
#line 773 "args/argcheck2.cpp"
				
				{
#line 179 "args/argcheck2.rl"
					rl_min = 0; rl_max = 1; }
				
#line 779 "args/argcheck2.cpp"
				
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
#line 189 "args/argcheck2.rl"
					rl_min = 1; rl_max = REPEAT_INF; }
				
#line 801 "args/argcheck2.cpp"
				
				goto _st4;
				_ctr51:
				{
#line 136 "args/argcheck2.rl"
					
					rl_check.values.push_back((int64_t)(rl_sign * rl_num));
				}
				
#line 811 "args/argcheck2.cpp"
				
				{
#line 189 "args/argcheck2.rl"
					rl_min = 1; rl_max = REPEAT_INF; }
				
#line 817 "args/argcheck2.cpp"
				
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
#line 140 "args/argcheck2.rl"
					rl_min = 0; }
				
#line 837 "args/argcheck2.cpp"
				
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
#line 141 "args/argcheck2.rl"
					rl_max = 0; }
				
#line 885 "args/argcheck2.cpp"
				
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
#line 141 "args/argcheck2.rl"
					rl_max = (( (*( p))))-'0'; }
				
#line 902 "args/argcheck2.cpp"
				
				goto _st9;
				_ctr27:
				{
#line 141 "args/argcheck2.rl"
					(rl_max *= 10) += ((( (*( p)))) - '0'); }
				
#line 910 "args/argcheck2.cpp"
				
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
#line 140 "args/argcheck2.rl"
					rl_min = (( (*( p))))-'0'; }
				
#line 930 "args/argcheck2.cpp"
				
				goto _st10;
				_ctr29:
				{
#line 140 "args/argcheck2.rl"
					(rl_min *= 10) += ((( (*( p)))) - '0'); }
				
#line 938 "args/argcheck2.cpp"
				
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
#line 178 "args/argcheck2.rl"
					rl_type = CHECK_INT; }
				
#line 963 "args/argcheck2.cpp"
				
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
						goto _st23;
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
#line 160 "args/argcheck2.rl"
					rl_cmp = CMP_NOT_EQUAL; }
				
#line 1024 "args/argcheck2.cpp"
				
				goto _st13;
				_ctr57:
				{
#line 159 "args/argcheck2.rl"
					rl_cmp = CMP_LESS_EQ; }
				
#line 1032 "args/argcheck2.cpp"
				
				goto _st13;
				_ctr67:
				{
#line 158 "args/argcheck2.rl"
					rl_cmp = CMP_GREATER_EQ; }
				
#line 1040 "args/argcheck2.cpp"
				
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
#line 150 "args/argcheck2.rl"
					rl_sign = 1; }
				
#line 1065 "args/argcheck2.cpp"
				
				{
#line 145 "args/argcheck2.rl"
					rl_sign = 1; }
				
#line 1071 "args/argcheck2.cpp"
				
				goto _st14;
				_ctr40:
				{
#line 150 "args/argcheck2.rl"
					rl_sign = 1; }
				
#line 1079 "args/argcheck2.cpp"
				
				{
#line 146 "args/argcheck2.rl"
					rl_sign = -1; }
				
#line 1085 "args/argcheck2.cpp"
				
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
#line 150 "args/argcheck2.rl"
					rl_sign = 1; }
				
#line 1102 "args/argcheck2.cpp"
				
				{
#line 148 "args/argcheck2.rl"
					rl_num = 0; }
				
#line 1108 "args/argcheck2.cpp"
				
				{
#line 148 "args/argcheck2.rl"
					(rl_num *= 10) += ((( (*( p)))) - '0'); }
				
#line 1114 "args/argcheck2.cpp"
				
				goto _st15;
				_ctr43:
				{
#line 148 "args/argcheck2.rl"
					rl_num = 0; }
				
#line 1122 "args/argcheck2.cpp"
				
				{
#line 148 "args/argcheck2.rl"
					(rl_num *= 10) += ((( (*( p)))) - '0'); }
				
#line 1128 "args/argcheck2.cpp"
				
				goto _st15;
				_ctr49:
				{
#line 148 "args/argcheck2.rl"
					(rl_num *= 10) += ((( (*( p)))) - '0'); }
				
#line 1136 "args/argcheck2.cpp"
				
				goto _st15;
				_st15:
				p += 1;
				st_case_15:
				switch( ( (*( p))) ) {
					case 0: {
						goto _ctr45;
					}
					case 32: {
						goto _ctr46;
					}
					case 42: {
						goto _ctr47;
					}
					case 43: {
						goto _ctr48;
					}
					case 63: {
						goto _ctr50;
					}
					case 91: {
						goto _ctr51;
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
#line 162 "args/argcheck2.rl"
					rl_cmp = CMP_MODULE; }
				
#line 1173 "args/argcheck2.cpp"
				
				goto _st16;
				_st16:
				p += 1;
				st_case_16:
				if ( 49 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr53;
				}
				{
					goto _st0;
				}
				_ctr53:
				{
#line 163 "args/argcheck2.rl"
					rl_sign = 1; rl_num = 0; }
				
#line 1190 "args/argcheck2.cpp"
				
				{
#line 163 "args/argcheck2.rl"
					(rl_num *= 10) += ((( (*( p)))) - '0'); }
				
#line 1196 "args/argcheck2.cpp"
				
				{
#line 136 "args/argcheck2.rl"
					
					rl_check.values.push_back((int64_t)(rl_sign * rl_num));
				}
				
#line 1204 "args/argcheck2.cpp"
				
				goto _st17;
				_ctr55:
				{
#line 163 "args/argcheck2.rl"
					(rl_num *= 10) += ((( (*( p)))) - '0'); }
				
#line 1212 "args/argcheck2.cpp"
				
				{
#line 136 "args/argcheck2.rl"
					
					rl_check.values.push_back((int64_t)(rl_sign * rl_num));
				}
				
#line 1220 "args/argcheck2.cpp"
				
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
					goto _ctr55;
				}
				{
					goto _st0;
				}
				_ctr33:
				{
#line 159 "args/argcheck2.rl"
					rl_cmp = CMP_LESS; }
				
#line 1257 "args/argcheck2.cpp"
				
				goto _st18;
				_st18:
				p += 1;
				st_case_18:
				switch( ( (*( p))) ) {
					case 43: {
						goto _ctr39;
					}
					case 45: {
						goto _ctr40;
					}
					case 61: {
						goto _ctr57;
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
#line 155 "args/argcheck2.rl"
					rl_cmp = CMP_EQUAL; }
				
#line 1285 "args/argcheck2.cpp"
				
				goto _st19;
				_st19:
				p += 1;
				st_case_19:
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
				{
					goto _st0;
				}
				_ctr59:
				{
#line 150 "args/argcheck2.rl"
					rl_sign = 1; }
				
#line 1310 "args/argcheck2.cpp"
				
				{
#line 145 "args/argcheck2.rl"
					rl_sign = 1; }
				
#line 1316 "args/argcheck2.cpp"
				
				goto _st20;
				_ctr60:
				{
#line 150 "args/argcheck2.rl"
					rl_sign = 1; }
				
#line 1324 "args/argcheck2.cpp"
				
				{
#line 146 "args/argcheck2.rl"
					rl_sign = -1; }
				
#line 1330 "args/argcheck2.cpp"
				
				goto _st20;
				_st20:
				p += 1;
				st_case_20:
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr63;
				}
				{
					goto _st0;
				}
				_ctr61:
				{
#line 150 "args/argcheck2.rl"
					rl_sign = 1; }
				
#line 1347 "args/argcheck2.cpp"
				
				{
#line 148 "args/argcheck2.rl"
					rl_num = 0; }
				
#line 1353 "args/argcheck2.cpp"
				
				{
#line 148 "args/argcheck2.rl"
					(rl_num *= 10) += ((( (*( p)))) - '0'); }
				
#line 1359 "args/argcheck2.cpp"
				
				{
#line 136 "args/argcheck2.rl"
					
					rl_check.values.push_back((int64_t)(rl_sign * rl_num));
				}
				
#line 1367 "args/argcheck2.cpp"
				
				goto _st21;
				_ctr63:
				{
#line 148 "args/argcheck2.rl"
					rl_num = 0; }
				
#line 1375 "args/argcheck2.cpp"
				
				{
#line 148 "args/argcheck2.rl"
					(rl_num *= 10) += ((( (*( p)))) - '0'); }
				
#line 1381 "args/argcheck2.cpp"
				
				{
#line 136 "args/argcheck2.rl"
					
					rl_check.values.push_back((int64_t)(rl_sign * rl_num));
				}
				
#line 1389 "args/argcheck2.cpp"
				
				goto _st21;
				_ctr65:
				{
#line 148 "args/argcheck2.rl"
					(rl_num *= 10) += ((( (*( p)))) - '0'); }
				
#line 1397 "args/argcheck2.cpp"
				
				{
#line 136 "args/argcheck2.rl"
					
					rl_check.values.push_back((int64_t)(rl_sign * rl_num));
				}
				
#line 1405 "args/argcheck2.cpp"
				
				goto _st21;
				_st21:
				p += 1;
				st_case_21:
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
					case 124: {
						goto _st19;
					}
				}
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr65;
				}
				{
					goto _st0;
				}
				_ctr35:
				{
#line 158 "args/argcheck2.rl"
					rl_cmp = CMP_GREATER; }
				
#line 1445 "args/argcheck2.cpp"
				
				goto _st22;
				_st22:
				p += 1;
				st_case_22:
				switch( ( (*( p))) ) {
					case 43: {
						goto _ctr39;
					}
					case 45: {
						goto _ctr40;
					}
					case 61: {
						goto _ctr67;
					}
				}
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr41;
				}
				{
					goto _st0;
				}
				_st23:
				p += 1;
				st_case_23:
				if ( ( (*( p))) == 50 ) {
					goto _ctr68;
				}
				{
					goto _st0;
				}
				st_out:
				_test_eof1: cs = 1; goto _test_eof; 
				_test_eof2: cs = 2; goto _test_eof; 
				_test_eof24: cs = 24; goto _test_eof; 
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
				
				_test_eof: {}
				if ( cs >= 24 )
					goto _out; _pop: {}
				_out: {}
			}
			
#line 389 "args/argcheck2.rl"
			
			
			if (cs < 
#line 1513 "args/argcheck2.cpp"
			24
#line 391 "args/argcheck2.rl"
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
