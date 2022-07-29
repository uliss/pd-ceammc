# include "argcheck2.h"
# include "ceammc_string.h"
# include "fmt/core.h"

# include <cstdint>
# include <iostream>

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
        }

    }
}

rep_min = '0' @{ rl_min = 0; } | ([1-9] @{ rl_min = fc-'0'; } [0-9]* ${ (rl_min *= 10) += (fc - '0'); });
rep_max = '0' @{ rl_max = 0; } | ([1-9] @{ rl_max = fc-'0'; } [0-9]* ${ (rl_max *= 10) += (fc - '0'); });
rep_num   = '[' rep_min ']' @{ rl_max = rl_min; };
rep_range = '[' rep_min '..' rep_max? ']';


atom = 'a' @{ rl_type = CHECK_ATOM; };
bool = 'B' @{ rl_type = CHECK_BOOL; };
byte = 'b' @{ rl_type = CHECK_BYTE; };
nrep = '?' @{ rl_min = 0; rl_max = 1; }
     | '+' @{ rl_min = 1, rl_max = REPEAT_INF; }
     | '*' @{ rl_min = 0; rl_max = REPEAT_INF; }
     | rep_range
     | rep_num;

check = (atom
      | bool
      | byte
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
