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

%%{
machine arg_check2;

action do_check {
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

action append_opt_int {
    rl_check.values.push_back((int64_t)(rl_sign * rl_num));
}

action append_opt_real {
    double real = rl_sign * rl_num;
    if (rl_den_cnt)
        real += rl_den / double(rl_den_cnt);

    rl_check.values.push_back(real);
}

action append_opt_sym {
    try {
        ArgString str{ {}, 0 };
        str.first.assign(rl_sym_start, fpc - rl_sym_start);
        str.second = crc32_hash(str.first.data());
        rl_check.values.push_back(str);
    } catch(std::exception& e) {
        LIB_ERR << "exception: " << e.what();
    }
}

#####################
# repeats: {INT}, {INT,} or {INT,INT}
#####################
rep_min = '0' @{ rl_min = 0; } | ([1-9] @{ rl_min = fc-'0'; } [0-9]* ${ (rl_min *= 10) += (fc - '0'); });
rep_max = '0' @{ rl_max = 0; } | ([1-9] @{ rl_max = fc-'0'; } [0-9]* ${ (rl_max *= 10) += (fc - '0'); });
rep_num   = '{' rep_min '}' @{ rl_max = rl_min; };
rep_range = '{' rep_min ',' rep_max? '}';

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
                    num_int @append_opt_int ',' num_int @append_opt_int
                (']' @{ rl_cmp = CMP_RANGE_CLOSED; } | ')' @{ rl_cmp = CMP_RANGE_SEMIOPEN; })
                );

#####################
# equal: =INT|INT|INT...
#####################
cmp_eq_int = (('=' num_int @append_opt_int)
              ('|' num_int @append_opt_int)*
             ) >{ rl_cmp = CMP_EQUAL; }
           ;

cmp_op = ('>'  @{ rl_cmp = CMP_GREATER; } ('=' @{ rl_cmp = CMP_GREATER_EQ; })?)
       | ('<'  @{ rl_cmp = CMP_LESS; }    ('=' @{ rl_cmp = CMP_LESS_EQ; })?)
       |  '!=' @{ rl_cmp = CMP_NOT_EQUAL; };

#####################
# mod: %INT
#####################
cmp_mod = ( '%' @{ rl_cmp = CMP_MODULE; }
            ([1-9][0-9]*) >{ rl_sign = 1; rl_num = 0; } ${ (rl_num *= 10) += (fc - '0'); } @append_opt_int
          )
          ;
#####################
# power of 2: ^2
#####################
cmp_pow2 = '^2' @{ rl_cmp = CMP_POWER2; };

int_check = (cmp_op num_int %append_opt_int)
          | cmp_mod
          | cmp_pow2
          | cmp_eq_int
          | cmp_range_int
          ;

sym_opt = ([^ |]-0)+** >{ rl_sym_start = fpc; };

#####################
# symbol equal: =SYM|SYM...
#####################
cmp_eq_sym = ('=' sym_opt  %append_opt_sym
              ('|' sym_opt %append_opt_sym)*
             ) >{ rl_cmp = CMP_EQUAL; };

sym_check = cmp_eq_sym;

float_check = (cmp_op num_real %append_opt_real);

atom  = 'a' @{ rl_type = CHECK_ATOM; };
bool  = 'B' @{ rl_type = CHECK_BOOL; };
byte  = 'b' @{ rl_type = CHECK_BYTE; };
int   = 'i' @{ rl_type = CHECK_INT; } int_check?;
sym   = 's' @{ rl_type = CHECK_SYMBOL; } sym_check?;
float = 'f' @{ rl_type = CHECK_FLOAT; } float_check?;
nrep  = '?' @{ rl_min = 0; rl_max = 1; }
      | '+' @{ rl_min = 1, rl_max = REPEAT_INF; }
      | '*' @{ rl_min = 0; rl_max = REPEAT_INF; }
      | rep_range
      | rep_num;

check = (atom
      | bool
      | byte
      | int
      | sym
      | float
      ) nrep? >{ rl_min = 1; rl_max = REPEAT_INF; }
      ;

args = (check (' ' @do_check check)*);

main := args 0 >do_check @{ fbreak; };

write data;

}%%

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

    %%{
        write init;
        write exec noend;
    }%%

    if (cs < %%{ write first_final; }%%) {
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
