
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


#line 227 "args/argcheck2.cpp"
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

    
#line 248 "args/argcheck2.cpp"
	{
	cs = time_check_start;
	}

#line 244 "args/argcheck2.rl"
    
#line 255 "args/argcheck2.cpp"
	{
	switch ( cs )
	{
case 1:
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st2;
	goto st0;
st0:
cs = 0;
	goto _out;
st2:
	p += 1;
case 2:
	switch( (*p) ) {
		case 46: goto st3;
		case 95: goto st5;
		case 109: goto st6;
		case 115: goto st10;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st2;
	goto st0;
st3:
	p += 1;
case 3:
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st4;
	goto st0;
st4:
	p += 1;
case 4:
	switch( (*p) ) {
		case 95: goto st5;
		case 109: goto st6;
		case 115: goto st10;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st4;
	goto st0;
st5:
	p += 1;
case 5:
	switch( (*p) ) {
		case 109: goto st6;
		case 115: goto st10;
	}
	goto st0;
st6:
	p += 1;
case 6:
	if ( (*p) == 115 )
		goto st7;
	goto st0;
st7:
	p += 1;
case 7:
	switch( (*p) ) {
		case 0: goto tr8;
		case 101: goto st8;
	}
	goto st0;
tr8:
#line 229 "args/argcheck2.rl"
	{ {p++; cs = 13; goto _out;} }
	goto st13;
st13:
	p += 1;
case 13:
#line 324 "args/argcheck2.cpp"
	goto st0;
st8:
	p += 1;
case 8:
	if ( (*p) == 99 )
		goto st9;
	goto st0;
st9:
	p += 1;
case 9:
	if ( (*p) == 0 )
		goto tr8;
	goto st0;
st10:
	p += 1;
case 10:
	switch( (*p) ) {
		case 0: goto tr8;
		case 97: goto st11;
		case 101: goto st8;
	}
	goto st0;
st11:
	p += 1;
case 11:
	if ( (*p) == 109 )
		goto st12;
	goto st0;
st12:
	p += 1;
case 12:
	if ( (*p) == 112 )
		goto st9;
	goto st0;
	}

	_out: {}
	}

#line 245 "args/argcheck2.rl"

    return (cs >= 13);
}

}


#line 372 "args/argcheck2.cpp"
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
            const int64_t arg = (c.values.size() >= 1 && boost::get<int64_t>(&c.values[0]))
                ? *boost::get<int64_t>(&c.values[0])
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

bool ArgChecker::check(const AtomListView& lv, BaseObject* obj, ArgMatchList* matches, bool printErr) const
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
                if (printErr)
                    pdError(x, fmt::format("{} expected at [{}]", check.argName(), atom_idx));

                return false;
            }
            if (!checkAtom(check, lv[atom_idx], atom_idx, x, printErr))
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
        if (printErr)
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

    
#line 815 "args/argcheck2.cpp"
	{
	cs = arg_check2_start;
	}

#line 820 "args/argcheck2.cpp"
	{
	switch ( cs )
	{
tr19:
#line 415 "args/argcheck2.rl"
	{ rl_chk.rmin = 1; rl_chk.rmax = 1; }
#line 254 "args/argcheck2.rl"
	{
    if (chk_)
        chk_->push_back(rl_chk);

    rl_num = 0;
    rl_den = 0;
    rl_sign = 0;
    rl_den_cnt = 0;
    rl_chk = {};
}
	goto st1;
tr25:
#line 254 "args/argcheck2.rl"
	{
    if (chk_)
        chk_->push_back(rl_chk);

    rl_num = 0;
    rl_den = 0;
    rl_sign = 0;
    rl_den_cnt = 0;
    rl_chk = {};
}
	goto st1;
tr47:
#line 269 "args/argcheck2.rl"
	{
    double real = rl_num;
    if (rl_den_cnt)
        real += rl_den / double(rl_den_cnt);

    rl_chk.values.push_back(rl_sign * real);
}
#line 415 "args/argcheck2.rl"
	{ rl_chk.rmin = 1; rl_chk.rmax = 1; }
#line 254 "args/argcheck2.rl"
	{
    if (chk_)
        chk_->push_back(rl_chk);

    rl_num = 0;
    rl_den = 0;
    rl_sign = 0;
    rl_den_cnt = 0;
    rl_chk = {};
}
	goto st1;
tr99:
#line 265 "args/argcheck2.rl"
	{
    rl_chk.values.push_back((int64_t)(rl_sign * rl_num));
}
#line 415 "args/argcheck2.rl"
	{ rl_chk.rmin = 1; rl_chk.rmax = 1; }
#line 254 "args/argcheck2.rl"
	{
    if (chk_)
        chk_->push_back(rl_chk);

    rl_num = 0;
    rl_den = 0;
    rl_sign = 0;
    rl_den_cnt = 0;
    rl_chk = {};
}
	goto st1;
tr129:
#line 317 "args/argcheck2.rl"
	{ rl_chk.cmp = CMP_RANGE_SEMIOPEN; }
#line 415 "args/argcheck2.rl"
	{ rl_chk.rmin = 1; rl_chk.rmax = 1; }
#line 254 "args/argcheck2.rl"
	{
    if (chk_)
        chk_->push_back(rl_chk);

    rl_num = 0;
    rl_den = 0;
    rl_sign = 0;
    rl_den_cnt = 0;
    rl_chk = {};
}
	goto st1;
tr135:
#line 317 "args/argcheck2.rl"
	{ rl_chk.cmp = CMP_RANGE_CLOSED; }
#line 415 "args/argcheck2.rl"
	{ rl_chk.rmin = 1; rl_chk.rmax = 1; }
#line 254 "args/argcheck2.rl"
	{
    if (chk_)
        chk_->push_back(rl_chk);

    rl_num = 0;
    rl_den = 0;
    rl_sign = 0;
    rl_den_cnt = 0;
    rl_chk = {};
}
	goto st1;
tr144:
#line 277 "args/argcheck2.rl"
	{
    try {
        ArgString str{ {}, 0 };
        str.first.assign(rl_sym_start, p - rl_sym_start);
        str.second = crc32_hash(str.first.data());
        rl_chk.values.push_back(str);
    } catch(std::exception& e) {
        LIB_ERR << "exception: " << e.what();
    }
}
#line 415 "args/argcheck2.rl"
	{ rl_chk.rmin = 1; rl_chk.rmax = 1; }
#line 254 "args/argcheck2.rl"
	{
    if (chk_)
        chk_->push_back(rl_chk);

    rl_num = 0;
    rl_den = 0;
    rl_sign = 0;
    rl_den_cnt = 0;
    rl_chk = {};
}
	goto st1;
st1:
	p += 1;
case 1:
#line 957 "args/argcheck2.cpp"
	switch( (*p) ) {
		case 66: goto tr2;
		case 95: goto tr0;
		case 97: goto tr3;
		case 98: goto tr4;
		case 102: goto tr5;
		case 105: goto tr6;
		case 115: goto tr7;
		case 116: goto tr8;
	}
	if ( (*p) > 57 ) {
		if ( 63 <= (*p) && (*p) <= 90 )
			goto tr0;
	} else if ( (*p) >= 48 )
		goto tr0;
	goto st0;
st0:
cs = 0;
	goto _out;
tr0:
#line 405 "args/argcheck2.rl"
	{ rl_chk.name.clear(); }
#line 405 "args/argcheck2.rl"
	{ rl_chk.name.push_back((*p)); }
	goto st2;
st2:
	p += 1;
case 2:
#line 986 "args/argcheck2.cpp"
	switch( (*p) ) {
		case 58: goto st10;
		case 95: goto tr9;
	}
	if ( (*p) > 57 ) {
		if ( 63 <= (*p) && (*p) <= 90 )
			goto tr9;
	} else if ( (*p) >= 48 )
		goto tr9;
	goto st0;
tr9:
#line 405 "args/argcheck2.rl"
	{ rl_chk.name.push_back((*p)); }
	goto st3;
st3:
	p += 1;
case 3:
#line 1004 "args/argcheck2.cpp"
	switch( (*p) ) {
		case 58: goto st10;
		case 95: goto tr11;
	}
	if ( (*p) > 57 ) {
		if ( 63 <= (*p) && (*p) <= 90 )
			goto tr11;
	} else if ( (*p) >= 48 )
		goto tr11;
	goto st0;
tr11:
#line 405 "args/argcheck2.rl"
	{ rl_chk.name.push_back((*p)); }
	goto st4;
st4:
	p += 1;
case 4:
#line 1022 "args/argcheck2.cpp"
	switch( (*p) ) {
		case 58: goto st10;
		case 95: goto tr12;
	}
	if ( (*p) > 57 ) {
		if ( 63 <= (*p) && (*p) <= 90 )
			goto tr12;
	} else if ( (*p) >= 48 )
		goto tr12;
	goto st0;
tr12:
#line 405 "args/argcheck2.rl"
	{ rl_chk.name.push_back((*p)); }
	goto st5;
st5:
	p += 1;
case 5:
#line 1040 "args/argcheck2.cpp"
	switch( (*p) ) {
		case 58: goto st10;
		case 95: goto tr13;
	}
	if ( (*p) > 57 ) {
		if ( 63 <= (*p) && (*p) <= 90 )
			goto tr13;
	} else if ( (*p) >= 48 )
		goto tr13;
	goto st0;
tr13:
#line 405 "args/argcheck2.rl"
	{ rl_chk.name.push_back((*p)); }
	goto st6;
st6:
	p += 1;
case 6:
#line 1058 "args/argcheck2.cpp"
	switch( (*p) ) {
		case 58: goto st10;
		case 95: goto tr14;
	}
	if ( (*p) > 57 ) {
		if ( 63 <= (*p) && (*p) <= 90 )
			goto tr14;
	} else if ( (*p) >= 48 )
		goto tr14;
	goto st0;
tr14:
#line 405 "args/argcheck2.rl"
	{ rl_chk.name.push_back((*p)); }
	goto st7;
st7:
	p += 1;
case 7:
#line 1076 "args/argcheck2.cpp"
	switch( (*p) ) {
		case 58: goto st10;
		case 95: goto tr15;
	}
	if ( (*p) > 57 ) {
		if ( 63 <= (*p) && (*p) <= 90 )
			goto tr15;
	} else if ( (*p) >= 48 )
		goto tr15;
	goto st0;
tr15:
#line 405 "args/argcheck2.rl"
	{ rl_chk.name.push_back((*p)); }
	goto st8;
st8:
	p += 1;
case 8:
#line 1094 "args/argcheck2.cpp"
	switch( (*p) ) {
		case 58: goto st10;
		case 95: goto tr16;
	}
	if ( (*p) > 57 ) {
		if ( 63 <= (*p) && (*p) <= 90 )
			goto tr16;
	} else if ( (*p) >= 48 )
		goto tr16;
	goto st0;
tr16:
#line 405 "args/argcheck2.rl"
	{ rl_chk.name.push_back((*p)); }
	goto st9;
st9:
	p += 1;
case 9:
#line 1112 "args/argcheck2.cpp"
	if ( (*p) == 58 )
		goto st10;
	goto st0;
st10:
	p += 1;
case 10:
	switch( (*p) ) {
		case 66: goto tr17;
		case 97: goto tr3;
		case 98: goto tr4;
		case 102: goto tr5;
		case 105: goto tr6;
		case 115: goto tr7;
		case 116: goto tr8;
	}
	goto st0;
tr3:
#line 392 "args/argcheck2.rl"
	{ rl_chk.type = CHECK_ATOM; }
	goto st11;
tr4:
#line 394 "args/argcheck2.rl"
	{ rl_chk.type = CHECK_BYTE; }
	goto st11;
tr8:
#line 398 "args/argcheck2.rl"
	{ rl_chk.type = CHECK_TIME; }
	goto st11;
tr17:
#line 393 "args/argcheck2.rl"
	{ rl_chk.type = CHECK_BOOL; }
	goto st11;
tr78:
#line 269 "args/argcheck2.rl"
	{
    double real = rl_num;
    if (rl_den_cnt)
        real += rl_den / double(rl_den_cnt);

    rl_chk.values.push_back(rl_sign * real);
}
#line 367 "args/argcheck2.rl"
	{ rl_chk.cmp = CMP_RANGE_SEMIOPEN; }
	goto st11;
tr81:
#line 269 "args/argcheck2.rl"
	{
    double real = rl_num;
    if (rl_den_cnt)
        real += rl_den / double(rl_den_cnt);

    rl_chk.values.push_back(rl_sign * real);
}
#line 367 "args/argcheck2.rl"
	{ rl_chk.cmp = CMP_RANGE_CLOSED; }
	goto st11;
tr140:
#line 342 "args/argcheck2.rl"
	{ rl_chk.cmp = CMP_POWER2; }
	goto st11;
st11:
	p += 1;
case 11:
#line 1176 "args/argcheck2.cpp"
	switch( (*p) ) {
		case 0: goto tr18;
		case 32: goto tr19;
		case 42: goto tr20;
		case 43: goto tr21;
		case 63: goto tr22;
		case 123: goto tr23;
	}
	goto st0;
tr18:
#line 415 "args/argcheck2.rl"
	{ rl_chk.rmin = 1; rl_chk.rmax = 1; }
#line 254 "args/argcheck2.rl"
	{
    if (chk_)
        chk_->push_back(rl_chk);

    rl_num = 0;
    rl_den = 0;
    rl_sign = 0;
    rl_den_cnt = 0;
    rl_chk = {};
}
#line 420 "args/argcheck2.rl"
	{ {p++; cs = 69; goto _out;} }
	goto st69;
tr24:
#line 254 "args/argcheck2.rl"
	{
    if (chk_)
        chk_->push_back(rl_chk);

    rl_num = 0;
    rl_den = 0;
    rl_sign = 0;
    rl_den_cnt = 0;
    rl_chk = {};
}
#line 420 "args/argcheck2.rl"
	{ {p++; cs = 69; goto _out;} }
	goto st69;
tr46:
#line 269 "args/argcheck2.rl"
	{
    double real = rl_num;
    if (rl_den_cnt)
        real += rl_den / double(rl_den_cnt);

    rl_chk.values.push_back(rl_sign * real);
}
#line 415 "args/argcheck2.rl"
	{ rl_chk.rmin = 1; rl_chk.rmax = 1; }
#line 254 "args/argcheck2.rl"
	{
    if (chk_)
        chk_->push_back(rl_chk);

    rl_num = 0;
    rl_den = 0;
    rl_sign = 0;
    rl_den_cnt = 0;
    rl_chk = {};
}
#line 420 "args/argcheck2.rl"
	{ {p++; cs = 69; goto _out;} }
	goto st69;
tr98:
#line 265 "args/argcheck2.rl"
	{
    rl_chk.values.push_back((int64_t)(rl_sign * rl_num));
}
#line 415 "args/argcheck2.rl"
	{ rl_chk.rmin = 1; rl_chk.rmax = 1; }
#line 254 "args/argcheck2.rl"
	{
    if (chk_)
        chk_->push_back(rl_chk);

    rl_num = 0;
    rl_den = 0;
    rl_sign = 0;
    rl_den_cnt = 0;
    rl_chk = {};
}
#line 420 "args/argcheck2.rl"
	{ {p++; cs = 69; goto _out;} }
	goto st69;
tr128:
#line 317 "args/argcheck2.rl"
	{ rl_chk.cmp = CMP_RANGE_SEMIOPEN; }
#line 415 "args/argcheck2.rl"
	{ rl_chk.rmin = 1; rl_chk.rmax = 1; }
#line 254 "args/argcheck2.rl"
	{
    if (chk_)
        chk_->push_back(rl_chk);

    rl_num = 0;
    rl_den = 0;
    rl_sign = 0;
    rl_den_cnt = 0;
    rl_chk = {};
}
#line 420 "args/argcheck2.rl"
	{ {p++; cs = 69; goto _out;} }
	goto st69;
tr134:
#line 317 "args/argcheck2.rl"
	{ rl_chk.cmp = CMP_RANGE_CLOSED; }
#line 415 "args/argcheck2.rl"
	{ rl_chk.rmin = 1; rl_chk.rmax = 1; }
#line 254 "args/argcheck2.rl"
	{
    if (chk_)
        chk_->push_back(rl_chk);

    rl_num = 0;
    rl_den = 0;
    rl_sign = 0;
    rl_den_cnt = 0;
    rl_chk = {};
}
#line 420 "args/argcheck2.rl"
	{ {p++; cs = 69; goto _out;} }
	goto st69;
tr143:
#line 277 "args/argcheck2.rl"
	{
    try {
        ArgString str{ {}, 0 };
        str.first.assign(rl_sym_start, p - rl_sym_start);
        str.second = crc32_hash(str.first.data());
        rl_chk.values.push_back(str);
    } catch(std::exception& e) {
        LIB_ERR << "exception: " << e.what();
    }
}
#line 415 "args/argcheck2.rl"
	{ rl_chk.rmin = 1; rl_chk.rmax = 1; }
#line 254 "args/argcheck2.rl"
	{
    if (chk_)
        chk_->push_back(rl_chk);

    rl_num = 0;
    rl_den = 0;
    rl_sign = 0;
    rl_den_cnt = 0;
    rl_chk = {};
}
#line 420 "args/argcheck2.rl"
	{ {p++; cs = 69; goto _out;} }
	goto st69;
st69:
	p += 1;
case 69:
#line 1333 "args/argcheck2.cpp"
	goto st0;
tr20:
#line 415 "args/argcheck2.rl"
	{ rl_chk.rmin = 1; rl_chk.rmax = 1; }
#line 401 "args/argcheck2.rl"
	{ rl_chk.setRepeats(0, REPEAT_INF); }
	goto st12;
tr21:
#line 415 "args/argcheck2.rl"
	{ rl_chk.rmin = 1; rl_chk.rmax = 1; }
#line 400 "args/argcheck2.rl"
	{ rl_chk.setRepeats(1, REPEAT_INF); }
	goto st12;
tr22:
#line 415 "args/argcheck2.rl"
	{ rl_chk.rmin = 1; rl_chk.rmax = 1; }
#line 399 "args/argcheck2.rl"
	{ rl_chk.setRepeats(0, 1); }
	goto st12;
tr29:
#line 293 "args/argcheck2.rl"
	{ rl_chk.rmax = rl_chk.rmin; }
	goto st12;
tr48:
#line 269 "args/argcheck2.rl"
	{
    double real = rl_num;
    if (rl_den_cnt)
        real += rl_den / double(rl_den_cnt);

    rl_chk.values.push_back(rl_sign * real);
}
#line 415 "args/argcheck2.rl"
	{ rl_chk.rmin = 1; rl_chk.rmax = 1; }
#line 401 "args/argcheck2.rl"
	{ rl_chk.setRepeats(0, REPEAT_INF); }
	goto st12;
tr49:
#line 269 "args/argcheck2.rl"
	{
    double real = rl_num;
    if (rl_den_cnt)
        real += rl_den / double(rl_den_cnt);

    rl_chk.values.push_back(rl_sign * real);
}
#line 415 "args/argcheck2.rl"
	{ rl_chk.rmin = 1; rl_chk.rmax = 1; }
#line 400 "args/argcheck2.rl"
	{ rl_chk.setRepeats(1, REPEAT_INF); }
	goto st12;
tr52:
#line 269 "args/argcheck2.rl"
	{
    double real = rl_num;
    if (rl_den_cnt)
        real += rl_den / double(rl_den_cnt);

    rl_chk.values.push_back(rl_sign * real);
}
#line 415 "args/argcheck2.rl"
	{ rl_chk.rmin = 1; rl_chk.rmax = 1; }
#line 399 "args/argcheck2.rl"
	{ rl_chk.setRepeats(0, 1); }
	goto st12;
tr100:
#line 265 "args/argcheck2.rl"
	{
    rl_chk.values.push_back((int64_t)(rl_sign * rl_num));
}
#line 415 "args/argcheck2.rl"
	{ rl_chk.rmin = 1; rl_chk.rmax = 1; }
#line 401 "args/argcheck2.rl"
	{ rl_chk.setRepeats(0, REPEAT_INF); }
	goto st12;
tr101:
#line 265 "args/argcheck2.rl"
	{
    rl_chk.values.push_back((int64_t)(rl_sign * rl_num));
}
#line 415 "args/argcheck2.rl"
	{ rl_chk.rmin = 1; rl_chk.rmax = 1; }
#line 400 "args/argcheck2.rl"
	{ rl_chk.setRepeats(1, REPEAT_INF); }
	goto st12;
tr103:
#line 265 "args/argcheck2.rl"
	{
    rl_chk.values.push_back((int64_t)(rl_sign * rl_num));
}
#line 415 "args/argcheck2.rl"
	{ rl_chk.rmin = 1; rl_chk.rmax = 1; }
#line 399 "args/argcheck2.rl"
	{ rl_chk.setRepeats(0, 1); }
	goto st12;
tr130:
#line 317 "args/argcheck2.rl"
	{ rl_chk.cmp = CMP_RANGE_SEMIOPEN; }
#line 415 "args/argcheck2.rl"
	{ rl_chk.rmin = 1; rl_chk.rmax = 1; }
#line 401 "args/argcheck2.rl"
	{ rl_chk.setRepeats(0, REPEAT_INF); }
	goto st12;
tr131:
#line 317 "args/argcheck2.rl"
	{ rl_chk.cmp = CMP_RANGE_SEMIOPEN; }
#line 415 "args/argcheck2.rl"
	{ rl_chk.rmin = 1; rl_chk.rmax = 1; }
#line 400 "args/argcheck2.rl"
	{ rl_chk.setRepeats(1, REPEAT_INF); }
	goto st12;
tr132:
#line 317 "args/argcheck2.rl"
	{ rl_chk.cmp = CMP_RANGE_SEMIOPEN; }
#line 415 "args/argcheck2.rl"
	{ rl_chk.rmin = 1; rl_chk.rmax = 1; }
#line 399 "args/argcheck2.rl"
	{ rl_chk.setRepeats(0, 1); }
	goto st12;
tr136:
#line 317 "args/argcheck2.rl"
	{ rl_chk.cmp = CMP_RANGE_CLOSED; }
#line 415 "args/argcheck2.rl"
	{ rl_chk.rmin = 1; rl_chk.rmax = 1; }
#line 401 "args/argcheck2.rl"
	{ rl_chk.setRepeats(0, REPEAT_INF); }
	goto st12;
tr137:
#line 317 "args/argcheck2.rl"
	{ rl_chk.cmp = CMP_RANGE_CLOSED; }
#line 415 "args/argcheck2.rl"
	{ rl_chk.rmin = 1; rl_chk.rmax = 1; }
#line 400 "args/argcheck2.rl"
	{ rl_chk.setRepeats(1, REPEAT_INF); }
	goto st12;
tr138:
#line 317 "args/argcheck2.rl"
	{ rl_chk.cmp = CMP_RANGE_CLOSED; }
#line 415 "args/argcheck2.rl"
	{ rl_chk.rmin = 1; rl_chk.rmax = 1; }
#line 399 "args/argcheck2.rl"
	{ rl_chk.setRepeats(0, 1); }
	goto st12;
tr146:
#line 277 "args/argcheck2.rl"
	{
    try {
        ArgString str{ {}, 0 };
        str.first.assign(rl_sym_start, p - rl_sym_start);
        str.second = crc32_hash(str.first.data());
        rl_chk.values.push_back(str);
    } catch(std::exception& e) {
        LIB_ERR << "exception: " << e.what();
    }
}
#line 415 "args/argcheck2.rl"
	{ rl_chk.rmin = 1; rl_chk.rmax = 1; }
#line 401 "args/argcheck2.rl"
	{ rl_chk.setRepeats(0, REPEAT_INF); }
	goto st12;
tr147:
#line 277 "args/argcheck2.rl"
	{
    try {
        ArgString str{ {}, 0 };
        str.first.assign(rl_sym_start, p - rl_sym_start);
        str.second = crc32_hash(str.first.data());
        rl_chk.values.push_back(str);
    } catch(std::exception& e) {
        LIB_ERR << "exception: " << e.what();
    }
}
#line 415 "args/argcheck2.rl"
	{ rl_chk.rmin = 1; rl_chk.rmax = 1; }
#line 400 "args/argcheck2.rl"
	{ rl_chk.setRepeats(1, REPEAT_INF); }
	goto st12;
tr148:
#line 277 "args/argcheck2.rl"
	{
    try {
        ArgString str{ {}, 0 };
        str.first.assign(rl_sym_start, p - rl_sym_start);
        str.second = crc32_hash(str.first.data());
        rl_chk.values.push_back(str);
    } catch(std::exception& e) {
        LIB_ERR << "exception: " << e.what();
    }
}
#line 415 "args/argcheck2.rl"
	{ rl_chk.rmin = 1; rl_chk.rmax = 1; }
#line 399 "args/argcheck2.rl"
	{ rl_chk.setRepeats(0, 1); }
	goto st12;
st12:
	p += 1;
case 12:
#line 1531 "args/argcheck2.cpp"
	switch( (*p) ) {
		case 0: goto tr24;
		case 32: goto tr25;
	}
	goto st0;
tr23:
#line 415 "args/argcheck2.rl"
	{ rl_chk.rmin = 1; rl_chk.rmax = 1; }
#line 294 "args/argcheck2.rl"
	{ rl_chk.rmin = 0; rl_chk.rmax = REPEAT_INF; }
	goto st13;
tr53:
#line 269 "args/argcheck2.rl"
	{
    double real = rl_num;
    if (rl_den_cnt)
        real += rl_den / double(rl_den_cnt);

    rl_chk.values.push_back(rl_sign * real);
}
#line 415 "args/argcheck2.rl"
	{ rl_chk.rmin = 1; rl_chk.rmax = 1; }
#line 294 "args/argcheck2.rl"
	{ rl_chk.rmin = 0; rl_chk.rmax = REPEAT_INF; }
	goto st13;
tr104:
#line 265 "args/argcheck2.rl"
	{
    rl_chk.values.push_back((int64_t)(rl_sign * rl_num));
}
#line 415 "args/argcheck2.rl"
	{ rl_chk.rmin = 1; rl_chk.rmax = 1; }
#line 294 "args/argcheck2.rl"
	{ rl_chk.rmin = 0; rl_chk.rmax = REPEAT_INF; }
	goto st13;
tr133:
#line 317 "args/argcheck2.rl"
	{ rl_chk.cmp = CMP_RANGE_SEMIOPEN; }
#line 415 "args/argcheck2.rl"
	{ rl_chk.rmin = 1; rl_chk.rmax = 1; }
#line 294 "args/argcheck2.rl"
	{ rl_chk.rmin = 0; rl_chk.rmax = REPEAT_INF; }
	goto st13;
tr139:
#line 317 "args/argcheck2.rl"
	{ rl_chk.cmp = CMP_RANGE_CLOSED; }
#line 415 "args/argcheck2.rl"
	{ rl_chk.rmin = 1; rl_chk.rmax = 1; }
#line 294 "args/argcheck2.rl"
	{ rl_chk.rmin = 0; rl_chk.rmax = REPEAT_INF; }
	goto st13;
tr149:
#line 277 "args/argcheck2.rl"
	{
    try {
        ArgString str{ {}, 0 };
        str.first.assign(rl_sym_start, p - rl_sym_start);
        str.second = crc32_hash(str.first.data());
        rl_chk.values.push_back(str);
    } catch(std::exception& e) {
        LIB_ERR << "exception: " << e.what();
    }
}
#line 415 "args/argcheck2.rl"
	{ rl_chk.rmin = 1; rl_chk.rmax = 1; }
#line 294 "args/argcheck2.rl"
	{ rl_chk.rmin = 0; rl_chk.rmax = REPEAT_INF; }
	goto st13;
st13:
	p += 1;
case 13:
#line 1603 "args/argcheck2.cpp"
	if ( (*p) == 48 )
		goto tr26;
	if ( 49 <= (*p) && (*p) <= 57 )
		goto tr27;
	goto st0;
tr26:
#line 291 "args/argcheck2.rl"
	{ rl_chk.rmin = 0; }
	goto st14;
st14:
	p += 1;
case 14:
#line 1616 "args/argcheck2.cpp"
	switch( (*p) ) {
		case 44: goto st15;
		case 125: goto tr29;
	}
	goto st0;
st15:
	p += 1;
case 15:
	switch( (*p) ) {
		case 48: goto tr30;
		case 125: goto st12;
	}
	if ( 49 <= (*p) && (*p) <= 57 )
		goto tr31;
	goto st0;
tr30:
#line 292 "args/argcheck2.rl"
	{ rl_chk.rmax = 0; }
	goto st16;
st16:
	p += 1;
case 16:
#line 1639 "args/argcheck2.cpp"
	if ( (*p) == 125 )
		goto st12;
	goto st0;
tr31:
#line 292 "args/argcheck2.rl"
	{ rl_chk.rmax = (*p)-'0'; }
	goto st17;
tr33:
#line 292 "args/argcheck2.rl"
	{ (rl_chk.rmax *= 10) += ((*p) - '0'); }
	goto st17;
st17:
	p += 1;
case 17:
#line 1654 "args/argcheck2.cpp"
	if ( (*p) == 125 )
		goto st12;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr33;
	goto st0;
tr27:
#line 291 "args/argcheck2.rl"
	{ rl_chk.rmin = (*p)-'0'; }
	goto st18;
tr34:
#line 291 "args/argcheck2.rl"
	{ (rl_chk.rmin *= 10) += ((*p) - '0'); }
	goto st18;
st18:
	p += 1;
case 18:
#line 1671 "args/argcheck2.cpp"
	switch( (*p) ) {
		case 44: goto st15;
		case 125: goto tr29;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr34;
	goto st0;
tr5:
#line 397 "args/argcheck2.rl"
	{ rl_chk.type = CHECK_FLOAT; }
	goto st19;
st19:
	p += 1;
case 19:
#line 1686 "args/argcheck2.cpp"
	switch( (*p) ) {
		case 0: goto tr18;
		case 32: goto tr19;
		case 33: goto st20;
		case 42: goto tr20;
		case 43: goto tr21;
		case 60: goto tr36;
		case 61: goto tr37;
		case 62: goto tr38;
		case 63: goto tr22;
		case 91: goto st33;
		case 123: goto tr23;
		case 126: goto tr40;
	}
	goto st0;
st20:
	p += 1;
case 20:
	if ( (*p) == 61 )
		goto tr41;
	goto st0;
tr41:
#line 330 "args/argcheck2.rl"
	{ rl_chk.cmp = CMP_NOT_EQUAL; }
	goto st21;
tr56:
#line 329 "args/argcheck2.rl"
	{ rl_chk.cmp = CMP_LESS_EQ; }
	goto st21;
tr66:
#line 328 "args/argcheck2.rl"
	{ rl_chk.cmp = CMP_GREATER_EQ; }
	goto st21;
st21:
	p += 1;
case 21:
#line 1723 "args/argcheck2.cpp"
	switch( (*p) ) {
		case 43: goto tr42;
		case 45: goto tr43;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr44;
	goto st0;
tr42:
#line 305 "args/argcheck2.rl"
	{ rl_sign = 1; }
#line 296 "args/argcheck2.rl"
	{ rl_sign = 1; }
	goto st22;
tr43:
#line 305 "args/argcheck2.rl"
	{ rl_sign = 1; }
#line 297 "args/argcheck2.rl"
	{ rl_sign = -1; }
	goto st22;
st22:
	p += 1;
case 22:
#line 1746 "args/argcheck2.cpp"
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr45;
	goto st0;
tr44:
#line 305 "args/argcheck2.rl"
	{ rl_sign = 1; }
#line 299 "args/argcheck2.rl"
	{ rl_num = 0; }
#line 299 "args/argcheck2.rl"
	{ (rl_num *= 10) += ((*p) - '0'); }
	goto st23;
tr45:
#line 299 "args/argcheck2.rl"
	{ rl_num = 0; }
#line 299 "args/argcheck2.rl"
	{ (rl_num *= 10) += ((*p) - '0'); }
	goto st23;
tr51:
#line 299 "args/argcheck2.rl"
	{ (rl_num *= 10) += ((*p) - '0'); }
	goto st23;
st23:
	p += 1;
case 23:
#line 1771 "args/argcheck2.cpp"
	switch( (*p) ) {
		case 0: goto tr46;
		case 32: goto tr47;
		case 42: goto tr48;
		case 43: goto tr49;
		case 46: goto st24;
		case 63: goto tr52;
		case 123: goto tr53;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr51;
	goto st0;
st24:
	p += 1;
case 24:
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr54;
	goto st0;
tr54:
#line 300 "args/argcheck2.rl"
	{ rl_den = 0; rl_den_cnt = 1; }
#line 300 "args/argcheck2.rl"
	{ (rl_den *= 10) += ((*p) - '0'); rl_den_cnt *= 10; }
	goto st25;
tr55:
#line 300 "args/argcheck2.rl"
	{ (rl_den *= 10) += ((*p) - '0'); rl_den_cnt *= 10; }
	goto st25;
st25:
	p += 1;
case 25:
#line 1803 "args/argcheck2.cpp"
	switch( (*p) ) {
		case 0: goto tr46;
		case 32: goto tr47;
		case 42: goto tr48;
		case 43: goto tr49;
		case 63: goto tr52;
		case 123: goto tr53;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr55;
	goto st0;
tr36:
#line 329 "args/argcheck2.rl"
	{ rl_chk.cmp = CMP_LESS; }
	goto st26;
st26:
	p += 1;
case 26:
#line 1822 "args/argcheck2.cpp"
	switch( (*p) ) {
		case 43: goto tr42;
		case 45: goto tr43;
		case 61: goto tr56;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr44;
	goto st0;
tr37:
#line 375 "args/argcheck2.rl"
	{ rl_chk.cmp = CMP_EQUAL; }
	goto st27;
tr40:
#line 383 "args/argcheck2.rl"
	{ rl_chk.cmp = CMP_APPROX; }
	goto st27;
tr63:
#line 269 "args/argcheck2.rl"
	{
    double real = rl_num;
    if (rl_den_cnt)
        real += rl_den / double(rl_den_cnt);

    rl_chk.values.push_back(rl_sign * real);
}
	goto st27;
st27:
	p += 1;
case 27:
#line 1852 "args/argcheck2.cpp"
	switch( (*p) ) {
		case 43: goto tr57;
		case 45: goto tr58;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr59;
	goto st0;
tr57:
#line 305 "args/argcheck2.rl"
	{ rl_sign = 1; }
#line 296 "args/argcheck2.rl"
	{ rl_sign = 1; }
	goto st28;
tr58:
#line 305 "args/argcheck2.rl"
	{ rl_sign = 1; }
#line 297 "args/argcheck2.rl"
	{ rl_sign = -1; }
	goto st28;
st28:
	p += 1;
case 28:
#line 1875 "args/argcheck2.cpp"
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr60;
	goto st0;
tr59:
#line 305 "args/argcheck2.rl"
	{ rl_sign = 1; }
#line 299 "args/argcheck2.rl"
	{ rl_num = 0; }
#line 299 "args/argcheck2.rl"
	{ (rl_num *= 10) += ((*p) - '0'); }
	goto st29;
tr60:
#line 299 "args/argcheck2.rl"
	{ rl_num = 0; }
#line 299 "args/argcheck2.rl"
	{ (rl_num *= 10) += ((*p) - '0'); }
	goto st29;
tr62:
#line 299 "args/argcheck2.rl"
	{ (rl_num *= 10) += ((*p) - '0'); }
	goto st29;
st29:
	p += 1;
case 29:
#line 1900 "args/argcheck2.cpp"
	switch( (*p) ) {
		case 0: goto tr46;
		case 32: goto tr47;
		case 42: goto tr48;
		case 43: goto tr49;
		case 46: goto st30;
		case 63: goto tr52;
		case 123: goto tr53;
		case 124: goto tr63;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr62;
	goto st0;
st30:
	p += 1;
case 30:
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr64;
	goto st0;
tr64:
#line 300 "args/argcheck2.rl"
	{ rl_den = 0; rl_den_cnt = 1; }
#line 300 "args/argcheck2.rl"
	{ (rl_den *= 10) += ((*p) - '0'); rl_den_cnt *= 10; }
	goto st31;
tr65:
#line 300 "args/argcheck2.rl"
	{ (rl_den *= 10) += ((*p) - '0'); rl_den_cnt *= 10; }
	goto st31;
st31:
	p += 1;
case 31:
#line 1933 "args/argcheck2.cpp"
	switch( (*p) ) {
		case 0: goto tr46;
		case 32: goto tr47;
		case 42: goto tr48;
		case 43: goto tr49;
		case 63: goto tr52;
		case 123: goto tr53;
		case 124: goto tr63;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr65;
	goto st0;
tr38:
#line 328 "args/argcheck2.rl"
	{ rl_chk.cmp = CMP_GREATER; }
	goto st32;
st32:
	p += 1;
case 32:
#line 1953 "args/argcheck2.cpp"
	switch( (*p) ) {
		case 43: goto tr42;
		case 45: goto tr43;
		case 61: goto tr66;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr44;
	goto st0;
st33:
	p += 1;
case 33:
	switch( (*p) ) {
		case 43: goto tr67;
		case 45: goto tr68;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr69;
	goto st0;
tr67:
#line 305 "args/argcheck2.rl"
	{ rl_sign = 1; }
#line 296 "args/argcheck2.rl"
	{ rl_sign = 1; }
	goto st34;
tr68:
#line 305 "args/argcheck2.rl"
	{ rl_sign = 1; }
#line 297 "args/argcheck2.rl"
	{ rl_sign = -1; }
	goto st34;
st34:
	p += 1;
case 34:
#line 1987 "args/argcheck2.cpp"
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr70;
	goto st0;
tr69:
#line 305 "args/argcheck2.rl"
	{ rl_sign = 1; }
#line 299 "args/argcheck2.rl"
	{ rl_num = 0; }
#line 299 "args/argcheck2.rl"
	{ (rl_num *= 10) += ((*p) - '0'); }
	goto st35;
tr70:
#line 299 "args/argcheck2.rl"
	{ rl_num = 0; }
#line 299 "args/argcheck2.rl"
	{ (rl_num *= 10) += ((*p) - '0'); }
	goto st35;
tr73:
#line 299 "args/argcheck2.rl"
	{ (rl_num *= 10) += ((*p) - '0'); }
	goto st35;
st35:
	p += 1;
case 35:
#line 2012 "args/argcheck2.cpp"
	switch( (*p) ) {
		case 44: goto tr71;
		case 46: goto st41;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr73;
	goto st0;
tr71:
#line 269 "args/argcheck2.rl"
	{
    double real = rl_num;
    if (rl_den_cnt)
        real += rl_den / double(rl_den_cnt);

    rl_chk.values.push_back(rl_sign * real);
}
	goto st36;
st36:
	p += 1;
case 36:
#line 2033 "args/argcheck2.cpp"
	switch( (*p) ) {
		case 43: goto tr74;
		case 45: goto tr75;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr76;
	goto st0;
tr74:
#line 305 "args/argcheck2.rl"
	{ rl_sign = 1; }
#line 296 "args/argcheck2.rl"
	{ rl_sign = 1; }
	goto st37;
tr75:
#line 305 "args/argcheck2.rl"
	{ rl_sign = 1; }
#line 297 "args/argcheck2.rl"
	{ rl_sign = -1; }
	goto st37;
st37:
	p += 1;
case 37:
#line 2056 "args/argcheck2.cpp"
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr77;
	goto st0;
tr76:
#line 305 "args/argcheck2.rl"
	{ rl_sign = 1; }
#line 299 "args/argcheck2.rl"
	{ rl_num = 0; }
#line 299 "args/argcheck2.rl"
	{ (rl_num *= 10) += ((*p) - '0'); }
	goto st38;
tr77:
#line 299 "args/argcheck2.rl"
	{ rl_num = 0; }
#line 299 "args/argcheck2.rl"
	{ (rl_num *= 10) += ((*p) - '0'); }
	goto st38;
tr80:
#line 299 "args/argcheck2.rl"
	{ (rl_num *= 10) += ((*p) - '0'); }
	goto st38;
st38:
	p += 1;
case 38:
#line 2081 "args/argcheck2.cpp"
	switch( (*p) ) {
		case 41: goto tr78;
		case 46: goto st39;
		case 93: goto tr81;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr80;
	goto st0;
st39:
	p += 1;
case 39:
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr82;
	goto st0;
tr82:
#line 300 "args/argcheck2.rl"
	{ rl_den = 0; rl_den_cnt = 1; }
#line 300 "args/argcheck2.rl"
	{ (rl_den *= 10) += ((*p) - '0'); rl_den_cnt *= 10; }
	goto st40;
tr83:
#line 300 "args/argcheck2.rl"
	{ (rl_den *= 10) += ((*p) - '0'); rl_den_cnt *= 10; }
	goto st40;
st40:
	p += 1;
case 40:
#line 2109 "args/argcheck2.cpp"
	switch( (*p) ) {
		case 41: goto tr78;
		case 93: goto tr81;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr83;
	goto st0;
st41:
	p += 1;
case 41:
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr84;
	goto st0;
tr84:
#line 300 "args/argcheck2.rl"
	{ rl_den = 0; rl_den_cnt = 1; }
#line 300 "args/argcheck2.rl"
	{ (rl_den *= 10) += ((*p) - '0'); rl_den_cnt *= 10; }
	goto st42;
tr85:
#line 300 "args/argcheck2.rl"
	{ (rl_den *= 10) += ((*p) - '0'); rl_den_cnt *= 10; }
	goto st42;
st42:
	p += 1;
case 42:
#line 2136 "args/argcheck2.cpp"
	if ( (*p) == 44 )
		goto tr71;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr85;
	goto st0;
tr6:
#line 395 "args/argcheck2.rl"
	{ rl_chk.type = CHECK_INT; }
	goto st43;
st43:
	p += 1;
case 43:
#line 2149 "args/argcheck2.cpp"
	switch( (*p) ) {
		case 0: goto tr18;
		case 32: goto tr19;
		case 33: goto st44;
		case 37: goto tr87;
		case 42: goto tr20;
		case 43: goto tr21;
		case 60: goto tr88;
		case 61: goto tr89;
		case 62: goto tr90;
		case 63: goto tr22;
		case 91: goto st55;
		case 94: goto st63;
		case 123: goto tr23;
	}
	goto st0;
st44:
	p += 1;
case 44:
	if ( (*p) == 61 )
		goto tr93;
	goto st0;
tr93:
#line 330 "args/argcheck2.rl"
	{ rl_chk.cmp = CMP_NOT_EQUAL; }
	goto st45;
tr107:
#line 329 "args/argcheck2.rl"
	{ rl_chk.cmp = CMP_LESS_EQ; }
	goto st45;
tr114:
#line 328 "args/argcheck2.rl"
	{ rl_chk.cmp = CMP_GREATER_EQ; }
	goto st45;
st45:
	p += 1;
case 45:
#line 2187 "args/argcheck2.cpp"
	switch( (*p) ) {
		case 43: goto tr94;
		case 45: goto tr95;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr96;
	goto st0;
tr94:
#line 305 "args/argcheck2.rl"
	{ rl_sign = 1; }
#line 296 "args/argcheck2.rl"
	{ rl_sign = 1; }
	goto st46;
tr95:
#line 305 "args/argcheck2.rl"
	{ rl_sign = 1; }
#line 297 "args/argcheck2.rl"
	{ rl_sign = -1; }
	goto st46;
st46:
	p += 1;
case 46:
#line 2210 "args/argcheck2.cpp"
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr97;
	goto st0;
tr96:
#line 305 "args/argcheck2.rl"
	{ rl_sign = 1; }
#line 299 "args/argcheck2.rl"
	{ rl_num = 0; }
#line 299 "args/argcheck2.rl"
	{ (rl_num *= 10) += ((*p) - '0'); }
	goto st47;
tr97:
#line 299 "args/argcheck2.rl"
	{ rl_num = 0; }
#line 299 "args/argcheck2.rl"
	{ (rl_num *= 10) += ((*p) - '0'); }
	goto st47;
tr102:
#line 299 "args/argcheck2.rl"
	{ (rl_num *= 10) += ((*p) - '0'); }
	goto st47;
st47:
	p += 1;
case 47:
#line 2235 "args/argcheck2.cpp"
	switch( (*p) ) {
		case 0: goto tr98;
		case 32: goto tr99;
		case 42: goto tr100;
		case 43: goto tr101;
		case 63: goto tr103;
		case 123: goto tr104;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr102;
	goto st0;
tr87:
#line 335 "args/argcheck2.rl"
	{ rl_chk.cmp = CMP_MODULE; }
	goto st48;
st48:
	p += 1;
case 48:
#line 2254 "args/argcheck2.cpp"
	if ( 49 <= (*p) && (*p) <= 57 )
		goto tr105;
	goto st0;
tr105:
#line 336 "args/argcheck2.rl"
	{ rl_sign = 1; rl_num = 0; }
#line 336 "args/argcheck2.rl"
	{ (rl_num *= 10) += ((*p) - '0'); }
	goto st49;
tr106:
#line 336 "args/argcheck2.rl"
	{ (rl_num *= 10) += ((*p) - '0'); }
	goto st49;
st49:
	p += 1;
case 49:
#line 2271 "args/argcheck2.cpp"
	switch( (*p) ) {
		case 0: goto tr98;
		case 32: goto tr99;
		case 42: goto tr100;
		case 43: goto tr101;
		case 63: goto tr103;
		case 123: goto tr104;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr106;
	goto st0;
tr88:
#line 329 "args/argcheck2.rl"
	{ rl_chk.cmp = CMP_LESS; }
	goto st50;
st50:
	p += 1;
case 50:
#line 2290 "args/argcheck2.cpp"
	switch( (*p) ) {
		case 43: goto tr94;
		case 45: goto tr95;
		case 61: goto tr107;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr96;
	goto st0;
tr89:
#line 325 "args/argcheck2.rl"
	{ rl_chk.cmp = CMP_EQUAL; }
	goto st51;
tr113:
#line 265 "args/argcheck2.rl"
	{
    rl_chk.values.push_back((int64_t)(rl_sign * rl_num));
}
	goto st51;
st51:
	p += 1;
case 51:
#line 2312 "args/argcheck2.cpp"
	switch( (*p) ) {
		case 43: goto tr108;
		case 45: goto tr109;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr110;
	goto st0;
tr108:
#line 305 "args/argcheck2.rl"
	{ rl_sign = 1; }
#line 296 "args/argcheck2.rl"
	{ rl_sign = 1; }
	goto st52;
tr109:
#line 305 "args/argcheck2.rl"
	{ rl_sign = 1; }
#line 297 "args/argcheck2.rl"
	{ rl_sign = -1; }
	goto st52;
st52:
	p += 1;
case 52:
#line 2335 "args/argcheck2.cpp"
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr111;
	goto st0;
tr110:
#line 305 "args/argcheck2.rl"
	{ rl_sign = 1; }
#line 299 "args/argcheck2.rl"
	{ rl_num = 0; }
#line 299 "args/argcheck2.rl"
	{ (rl_num *= 10) += ((*p) - '0'); }
	goto st53;
tr111:
#line 299 "args/argcheck2.rl"
	{ rl_num = 0; }
#line 299 "args/argcheck2.rl"
	{ (rl_num *= 10) += ((*p) - '0'); }
	goto st53;
tr112:
#line 299 "args/argcheck2.rl"
	{ (rl_num *= 10) += ((*p) - '0'); }
	goto st53;
st53:
	p += 1;
case 53:
#line 2360 "args/argcheck2.cpp"
	switch( (*p) ) {
		case 0: goto tr98;
		case 32: goto tr99;
		case 42: goto tr100;
		case 43: goto tr101;
		case 63: goto tr103;
		case 123: goto tr104;
		case 124: goto tr113;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr112;
	goto st0;
tr90:
#line 328 "args/argcheck2.rl"
	{ rl_chk.cmp = CMP_GREATER; }
	goto st54;
st54:
	p += 1;
case 54:
#line 2380 "args/argcheck2.cpp"
	switch( (*p) ) {
		case 43: goto tr94;
		case 45: goto tr95;
		case 61: goto tr114;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr96;
	goto st0;
st55:
	p += 1;
case 55:
	switch( (*p) ) {
		case 43: goto tr115;
		case 45: goto tr116;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr117;
	goto st0;
tr115:
#line 305 "args/argcheck2.rl"
	{ rl_sign = 1; }
#line 296 "args/argcheck2.rl"
	{ rl_sign = 1; }
	goto st56;
tr116:
#line 305 "args/argcheck2.rl"
	{ rl_sign = 1; }
#line 297 "args/argcheck2.rl"
	{ rl_sign = -1; }
	goto st56;
st56:
	p += 1;
case 56:
#line 2414 "args/argcheck2.cpp"
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr118;
	goto st0;
tr117:
#line 305 "args/argcheck2.rl"
	{ rl_sign = 1; }
#line 299 "args/argcheck2.rl"
	{ rl_num = 0; }
#line 299 "args/argcheck2.rl"
	{ (rl_num *= 10) += ((*p) - '0'); }
	goto st57;
tr118:
#line 299 "args/argcheck2.rl"
	{ rl_num = 0; }
#line 299 "args/argcheck2.rl"
	{ (rl_num *= 10) += ((*p) - '0'); }
	goto st57;
tr120:
#line 299 "args/argcheck2.rl"
	{ (rl_num *= 10) += ((*p) - '0'); }
	goto st57;
st57:
	p += 1;
case 57:
#line 2439 "args/argcheck2.cpp"
	if ( (*p) == 44 )
		goto tr119;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr120;
	goto st0;
tr119:
#line 265 "args/argcheck2.rl"
	{
    rl_chk.values.push_back((int64_t)(rl_sign * rl_num));
}
	goto st58;
st58:
	p += 1;
case 58:
#line 2454 "args/argcheck2.cpp"
	switch( (*p) ) {
		case 43: goto tr121;
		case 45: goto tr122;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr123;
	goto st0;
tr121:
#line 305 "args/argcheck2.rl"
	{ rl_sign = 1; }
#line 296 "args/argcheck2.rl"
	{ rl_sign = 1; }
	goto st59;
tr122:
#line 305 "args/argcheck2.rl"
	{ rl_sign = 1; }
#line 297 "args/argcheck2.rl"
	{ rl_sign = -1; }
	goto st59;
st59:
	p += 1;
case 59:
#line 2477 "args/argcheck2.cpp"
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr124;
	goto st0;
tr123:
#line 305 "args/argcheck2.rl"
	{ rl_sign = 1; }
#line 299 "args/argcheck2.rl"
	{ rl_num = 0; }
#line 299 "args/argcheck2.rl"
	{ (rl_num *= 10) += ((*p) - '0'); }
	goto st60;
tr124:
#line 299 "args/argcheck2.rl"
	{ rl_num = 0; }
#line 299 "args/argcheck2.rl"
	{ (rl_num *= 10) += ((*p) - '0'); }
	goto st60;
tr126:
#line 299 "args/argcheck2.rl"
	{ (rl_num *= 10) += ((*p) - '0'); }
	goto st60;
st60:
	p += 1;
case 60:
#line 2502 "args/argcheck2.cpp"
	switch( (*p) ) {
		case 41: goto tr125;
		case 93: goto tr127;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr126;
	goto st0;
tr125:
#line 265 "args/argcheck2.rl"
	{
    rl_chk.values.push_back((int64_t)(rl_sign * rl_num));
}
	goto st61;
st61:
	p += 1;
case 61:
#line 2519 "args/argcheck2.cpp"
	switch( (*p) ) {
		case 0: goto tr128;
		case 32: goto tr129;
		case 42: goto tr130;
		case 43: goto tr131;
		case 63: goto tr132;
		case 123: goto tr133;
	}
	goto st0;
tr127:
#line 265 "args/argcheck2.rl"
	{
    rl_chk.values.push_back((int64_t)(rl_sign * rl_num));
}
	goto st62;
st62:
	p += 1;
case 62:
#line 2538 "args/argcheck2.cpp"
	switch( (*p) ) {
		case 0: goto tr134;
		case 32: goto tr135;
		case 42: goto tr136;
		case 43: goto tr137;
		case 63: goto tr138;
		case 123: goto tr139;
	}
	goto st0;
st63:
	p += 1;
case 63:
	if ( (*p) == 50 )
		goto tr140;
	goto st0;
tr7:
#line 396 "args/argcheck2.rl"
	{ rl_chk.type = CHECK_SYMBOL; }
	goto st64;
st64:
	p += 1;
case 64:
#line 2561 "args/argcheck2.cpp"
	switch( (*p) ) {
		case 0: goto tr18;
		case 32: goto tr19;
		case 42: goto tr20;
		case 43: goto tr21;
		case 61: goto tr141;
		case 63: goto tr22;
		case 123: goto tr23;
	}
	goto st0;
tr141:
#line 358 "args/argcheck2.rl"
	{ rl_chk.cmp = CMP_EQUAL; }
	goto st65;
tr150:
#line 277 "args/argcheck2.rl"
	{
    try {
        ArgString str{ {}, 0 };
        str.first.assign(rl_sym_start, p - rl_sym_start);
        str.second = crc32_hash(str.first.data());
        rl_chk.values.push_back(str);
    } catch(std::exception& e) {
        LIB_ERR << "exception: " << e.what();
    }
}
	goto st65;
st65:
	p += 1;
case 65:
#line 2592 "args/argcheck2.cpp"
	switch( (*p) ) {
		case 35: goto tr142;
		case 45: goto tr142;
		case 95: goto tr142;
	}
	if ( (*p) < 64 ) {
		if ( 48 <= (*p) && (*p) <= 58 )
			goto tr142;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr142;
	} else
		goto tr142;
	goto st0;
tr142:
#line 351 "args/argcheck2.rl"
	{ rl_sym_start = p; }
	goto st66;
st66:
	p += 1;
case 66:
#line 2614 "args/argcheck2.cpp"
	switch( (*p) ) {
		case 0: goto tr143;
		case 32: goto tr144;
		case 35: goto st66;
		case 42: goto tr146;
		case 43: goto tr147;
		case 45: goto st66;
		case 63: goto tr148;
		case 95: goto st66;
		case 123: goto tr149;
		case 124: goto tr150;
	}
	if ( (*p) < 64 ) {
		if ( 48 <= (*p) && (*p) <= 58 )
			goto st66;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st66;
	} else
		goto st66;
	goto st0;
tr2:
#line 405 "args/argcheck2.rl"
	{ rl_chk.name.clear(); }
#line 405 "args/argcheck2.rl"
	{ rl_chk.name.push_back((*p)); }
#line 393 "args/argcheck2.rl"
	{ rl_chk.type = CHECK_BOOL; }
	goto st67;
st67:
	p += 1;
case 67:
#line 2647 "args/argcheck2.cpp"
	switch( (*p) ) {
		case 0: goto tr18;
		case 32: goto tr19;
		case 42: goto tr20;
		case 43: goto tr21;
		case 58: goto st10;
		case 63: goto tr151;
		case 95: goto tr9;
		case 123: goto tr23;
	}
	if ( (*p) > 57 ) {
		if ( 64 <= (*p) && (*p) <= 90 )
			goto tr9;
	} else if ( (*p) >= 48 )
		goto tr9;
	goto st0;
tr151:
#line 405 "args/argcheck2.rl"
	{ rl_chk.name.push_back((*p)); }
#line 415 "args/argcheck2.rl"
	{ rl_chk.rmin = 1; rl_chk.rmax = 1; }
#line 399 "args/argcheck2.rl"
	{ rl_chk.setRepeats(0, 1); }
	goto st68;
st68:
	p += 1;
case 68:
#line 2675 "args/argcheck2.cpp"
	switch( (*p) ) {
		case 0: goto tr24;
		case 32: goto tr25;
		case 58: goto st10;
		case 95: goto tr11;
	}
	if ( (*p) > 57 ) {
		if ( 63 <= (*p) && (*p) <= 90 )
			goto tr11;
	} else if ( (*p) >= 48 )
		goto tr11;
	goto st0;
	}

	_out: {}
	}

#line 860 "args/argcheck2.rl"


    if (cs < 69) {
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
