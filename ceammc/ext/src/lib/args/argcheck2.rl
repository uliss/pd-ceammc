# include "argcheck2.h"
# include "ceammc_string.h"
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

%%{
machine arg_check2;

action do_check {
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
                    if (val != arg) {
                        err << fmt::format("int value at [{}] expected to be ={}, got: {}", ca, arg, val);
                        return false;
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

rep_min = '0' @{ rl_min = 0; } | ([1-9] @{ rl_min = fc-'0'; } [0-9]* ${ (rl_min *= 10) += (fc - '0'); });
rep_max = '0' @{ rl_max = 0; } | ([1-9] @{ rl_max = fc-'0'; } [0-9]* ${ (rl_max *= 10) += (fc - '0'); });
rep_num   = '[' rep_min ']' @{ rl_max = rl_min; };
rep_range = '[' rep_min '..' rep_max? ']';

num_sign = '+' @{ rl_sign = 1; }
         | '-' @{ rl_sign = -1; };

num_num  = [0-9]+ >{ rl_num = 0; } ${ (rl_num *= 10) += (fc - '0'); };
num_den  = [0-9]+ >{ rl_den = 0; rl_den_cnt = 1; } ${ (rl_den *= 10) += (fc - '0'); rl_den_cnt *= 10; };

cmp_farg   = (num_sign?
            num_num
            ('.' num_den)?
            ) >{ rl_sign = 1; }
              @{ rl_cmp_arg = rl_sign * rl_num;
                 if(rl_den_cnt)  rl_cmp_arg += (double(rl_den) / rl_den_cnt);
              };

cmp_op = ('>' @{ rl_cmp = CMP_GREATER; } ('=' @{ rl_cmp = CMP_GREATER_EQ; })?)
       | ('<'  @{ rl_cmp = CMP_LESS; } ('=' @{ rl_cmp = CMP_LESS_EQ; })?)
       |  '='  @{ rl_cmp = CMP_EQUAL; }
       |  '!=' @{ rl_cmp = CMP_NOT_EQUAL; };

cmp_mod = ( '%' @{ rl_cmp = CMP_MODULE; }
            ([1-9][0-9]*) >{ rl_num = 0; } ${ (rl_num *= 10) += (fc - '0'); }
          ) @{ rl_cmp_arg = rl_num; }
          ;

cmp_pow2 = '^2' @{ rl_cmp = CMP_POWER2; };

int_check = (cmp_op cmp_farg)
          | cmp_mod
          | cmp_pow2
          ;

atom = 'a' @{ rl_type = CHECK_ATOM; };
bool = 'B' @{ rl_type = CHECK_BOOL; };
byte = 'b' @{ rl_type = CHECK_BYTE; };
int  = 'i' @{ rl_type = CHECK_INT; } int_check?;
nrep = '?' @{ rl_min = 0; rl_max = 1; }
     | '+' @{ rl_min = 1, rl_max = REPEAT_INF; }
     | '*' @{ rl_min = 0; rl_max = REPEAT_INF; }
     | rep_range
     | rep_num;

check = (atom
      | bool
      | byte
      | int
      ) nrep? >{ rl_min = 1; rl_max = REPEAT_INF; }
      ;

args = (check (' ' @do_check check)*);

main := args 0 >do_check @{ fbreak; };

write data;

}%%

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

    %%{
        write init;
        write exec noend;
    }%%

    if (cs >= %%{ write first_final; }%%) {
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
