# include "argcheck.h"
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
    int16_t rmin { 0 };
    int16_t rmax { 0 };

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

%%{
machine time_check;

num    = [0-9]+ ('.' [0-9]+)?;
suffix = '_'? ('samp'|'msec'|'ms'|'sec'|'s');

main := num suffix 0 @{ fbreak; };

write data;
}%%

namespace {

bool check_time_unit(t_symbol* sym)
{
    if (!sym) return false;

    int cs = 0;
    const char* p = sym->s_name;

    %% write init;
    %% write exec noend;

    return (cs >= %%{ write first_final; }%%);
}

}

%%{
machine arg_check2;

action do_check {
    if (chk_)
        chk_->push_back(rl_chk);

    rl_num = 0;
    rl_den = 0;
    rl_sign = 0;
    rl_den_cnt = 0;
    rl_chk = {};
}

action append_opt_int {
    rl_chk.values.push_back((int64_t)(rl_sign * rl_num));
}

action append_opt_real {
    double real = rl_num;
    if (rl_den_cnt)
        real += rl_den / double(rl_den_cnt);

    rl_chk.values.push_back(rl_sign * real);
}

action append_opt_sym {
    try {
        ArgString str{ {}, 0 };
        const auto LEN = fpc - rl_sym_start;
        const auto MAXLEN = str.first.capacity();
        char guard = 0;

        if (*rl_sym_start == '"')
            guard = '"';
        else if (*rl_sym_start == '\'')
            guard = '\'';

        for (int i = 0; i < LEN; i++) {
            auto c = rl_sym_start[i];
            if (c == guard) continue;
            if (str.first.size() == MAXLEN) {
                LIB_ERR << fmt::format(
                    "[devel] ArgChecker max symbol length exceeded for '{}'"
                    ", max length is: {}, using trimmed symbol: '{}'",
                    std::string(rl_sym_start, LEN),
                    MAXLEN,
                    str.first.data());
                break;
            } else
                str.first.push_back(c);
        }

        str.second = crc32_hash(str.first.data());
        rl_chk.values.push_back(str);
    } catch(std::exception& e) {
        LIB_ERR << "exception: " << e.what();
    }
}

#####################
# repeats: {INT}, {INT,} or {INT,INT}
#####################
rep_min = '0' @{ rl_chk.rmin = 0; } | ([1-9] @{ rl_chk.rmin = fc-'0'; } [0-9]* ${ (rl_chk.rmin *= 10) += (fc - '0'); });
rep_max = '0' @{ rl_chk.rmax = 0; } | ([1-9] @{ rl_chk.rmax = fc-'0'; } [0-9]* ${ (rl_chk.rmax *= 10) += (fc - '0'); });
rep_num   = '{' rep_min '}' @{ rl_chk.rmax = rl_chk.rmin; };
rep_range = ('{' rep_min ',' rep_max? '}') >{ rl_chk.rmin = 0; rl_chk.rmax = REPEAT_INF; };

num_sign = '+' @{ rl_sign = 1; }
         | '-' @{ rl_sign = -1; };

num_num  = [0-9]+ >{ rl_num = 0; } ${ (rl_num *= 10) += (fc - '0'); };
num_den  = [0-9]+ >{ rl_den = 0; rl_den_cnt = 1; } ${ (rl_den *= 10) += (fc - '0'); rl_den_cnt *= 10; };

#####################
# int: (+-)?INT
#####################
num_int  = num_sign? >{ rl_sign = 1; } num_num;

#####################
# float: (+-)?INT.FRAC
#####################
num_real = num_int ('.' num_den)?;

#####################
# closed int range: [INT,INT] or [INT,INT)
#####################
cmp_range_int = ('['
                    num_int %append_opt_int ',' num_int %append_opt_int
                (']' %{ rl_chk.cmp = CMP_RANGE_CLOSED; } | ')' %{ rl_chk.cmp = CMP_RANGE_SEMIOPEN; })
                );

#####################
# equal: =INT|INT|INT...
#####################
cmp_eq_int = (('=' num_int %append_opt_int)
              ('|' num_int %append_opt_int)*
             ) >{ rl_chk.cmp = CMP_EQUAL; }
           ;

cmp_op = ('>'  @{ rl_chk.cmp = CMP_GREATER; } ('=' @{ rl_chk.cmp = CMP_GREATER_EQ; })?)
       | ('<'  @{ rl_chk.cmp = CMP_LESS; }    ('=' @{ rl_chk.cmp = CMP_LESS_EQ; })?)
       |  '!=' @{ rl_chk.cmp = CMP_NOT_EQUAL; };

#####################
# mod: %INT
#####################
cmp_mod = ( '%' @{ rl_chk.cmp = CMP_MODULE; }
            ([1-9][0-9]*) >{ rl_sign = 1; rl_num = 0; } ${ (rl_num *= 10) += (fc - '0'); } %append_opt_int
          )
          ;
#####################
# power of 2: ^2
#####################
cmp_pow2 = '^2' @{ rl_chk.cmp = CMP_POWER2; };

int_check = (cmp_op num_int %append_opt_int)
          | cmp_mod
          | cmp_pow2
          | cmp_eq_int
          | cmp_range_int
          ;

sym_opt_simple  = [a-zA-Z_\-0-9@#:]+;
sym_opt_dquoted = '"' [^"]+ '"';
sym_opt_squoted = "'" [^']+ "'";
sym_opt = (sym_opt_simple | sym_opt_squoted | sym_opt_dquoted) >{ rl_sym_start = fpc; };

#####################
# symbol equal: =SYM|SYM...
#####################
cmp_eq_sym = ('=' sym_opt  %append_opt_sym
              ('|' sym_opt %append_opt_sym)*
             ) >{ rl_chk.cmp = CMP_EQUAL; };

sym_check = cmp_eq_sym;

#####################
# closed float range: [FLOAT,FLOAT] or [FLOAT,FLOAT)
#####################
cmp_range_float = ('['
                    num_real %append_opt_real ',' num_real %append_opt_real
                (']' @{ rl_chk.cmp = CMP_RANGE_CLOSED; } | ')' @{ rl_chk.cmp = CMP_RANGE_SEMIOPEN; })
                );

#####################
# equal: =FLOAT|FLOAT|FLOAT...
#####################
cmp_eq_float = (('=' num_real %append_opt_real)
               ('|' num_real  %append_opt_real)*
               ) >{ rl_chk.cmp = CMP_EQUAL; }
               ;

#####################
# approx: ~FLOAT|FLOAT|FLOAT...
#####################
cmp_approx_float = (('~' num_real %append_opt_real)
               ('|' num_real  %append_opt_real)*
               ) >{ rl_chk.cmp = CMP_APPROX; }
               ;

float_check = (cmp_op num_real %append_opt_real)
            | cmp_eq_float
            | cmp_approx_float
            | cmp_range_float
            ;

atom  = 'a' @{ rl_chk.type = CHECK_ATOM; };
bool  = 'B' @{ rl_chk.type = CHECK_BOOL; };
byte  = 'b' @{ rl_chk.type = CHECK_BYTE; };
int   = 'i' @{ rl_chk.type = CHECK_INT; } int_check?;
sym   = 's' @{ rl_chk.type = CHECK_SYMBOL; } sym_check?;
float = 'f' @{ rl_chk.type = CHECK_FLOAT; } float_check?;
time  = 't' @{ rl_chk.type = CHECK_TIME; };
nrep  = '?' @{ rl_chk.setRepeats(0, 1); }
      | '+' @{ rl_chk.setRepeats(1, REPEAT_INF); }
      | '*' @{ rl_chk.setRepeats(0, REPEAT_INF); }
      | rep_range
      | rep_num;

arg_name = [A-Z0-9_?@]{1,8} >{ rl_chk.name.clear(); } ${ rl_chk.name.push_back(fc); };

check = (arg_name ':')?
      (atom
      | bool
      | byte
      | int
      | sym
      | float
      | time
      ) nrep? >{ rl_chk.rmin = 1; rl_chk.rmax = 1; }
      ;

args = (check (' ' @do_check check)*);

main := args 0 >do_check @{ fbreak; };

write data;

}%%

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
        if (!a.isInteger() || a.asT<t_int>() < 0 || a.asT<t_int>() > 255) {
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
            const int64_t val = a.asT<t_int>();
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

    %%{
        write init;
        write exec noend;
    }%%

    if (cs < %%{ write first_final; }%%) {
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
