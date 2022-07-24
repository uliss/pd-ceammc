# include "new_file_pattern.h"
# include "ceammc_log.h"

# include <cmath>
# include <cstring>
# include <ctime>
# include <limits>

# include "fmt/core.h"

%%{

machine new_file_pattern;

action on_pattern_done {
    using IndexT = decltype(idx);
    constexpr auto NDIGITS10 = std::numeric_limits<IndexT>::digits10;
    if (rl_pat_cnum > NDIGITS10) {
        LIB_ERR << fmt::format("[{}] '#'-pattern is too long: {}, max length is {}", __FUNCTION__, rl_pat_cnum, NDIGITS10);
        return PATTERN_LENGTH_OVERFLOW;
    }

    char buf[NDIGITS10+1];
    buf[rl_pat_cnum] = '\0';
    for (IndexT i = 0, dig = 1; i < rl_pat_cnum; i++, dig *= 10)
        buf[rl_pat_cnum - (i+1)] = ('0' + (idx / dig) % 10);


    rl_fname.append(buf);
    rl_pattern_found = true;
}

action on_year {
    std::time_t t = std::time(nullptr);
    char buf[8];
    if (std::strftime(buf, sizeof(buf), "%Y", &rl_now))
        rl_fname.append(buf);
}

action on_month {
    std::time_t t = std::time(nullptr);
    char buf[8];
    if (std::strftime(buf, sizeof(buf), "%m", &rl_now))
        rl_fname.append(buf);
}

action on_day {
    char buf[8];
    if (std::strftime(buf, sizeof(buf), "%d", &rl_now))
        rl_fname.append(buf);
}

action on_hour {
    char buf[8];
    if (std::strftime(buf, sizeof(buf), "%H", &rl_now))
        rl_fname.append(buf);
}

action on_min {
    char buf[8];
    if (std::strftime(buf, sizeof(buf), "%M", &rl_now))
        rl_fname.append(buf);
}

action on_sec {
    char buf[8];
    if (std::strftime(buf, sizeof(buf), "%S", &rl_now))
        rl_fname.append(buf);
}

action on_date {
    char buf[32];
    if (std::strftime(buf, sizeof(buf), "%Y-%m-%d", &rl_now))
        rl_fname.append(buf);
}

action on_time {
    char buf[32];
    if (std::strftime(buf, sizeof(buf), "%H-%M-%S", &rl_now))
        rl_fname.append(buf);
}

action on_datetime {
    char buf[64];
    if (std::strftime(buf, sizeof(buf), "%Y-%m-%d_%H-%M-%S", &rl_now))
        rl_fname.append(buf);
}

date_pattern = (('%YEAR%' %on_year
             | '%MONTH%'  %on_month
             | '%DAY%'    %on_day
             | '%HOUR%'   %on_hour
             | '%MIN%'    %on_min
             | '%SEC%'    %on_sec
             | '%DATE%'   %on_date
             | '%TIME%'   %on_time
             | '%NOW%'    %on_datetime
             | '%%'       %{ rl_fname += '%'; })
             )  >{ rl_err = fpc; }
                %{ rl_date_found = true; }
                <err{ rl_fname += '%'; p = rl_err; fgoto main; }
             ;
not_hash     = [^#%];
token        = not_hash ${ rl_fname += fc; } | date_pattern;
prefix       = token*;
suffix       = prefix;
pattern      = '#'+ ${ rl_pat_cnum++; } %on_pattern_done;

main := prefix pattern? suffix;

write data;

}%%

namespace ceammc {
namespace path {

PatternStatus make_new_filename_by_pattern(const std::string& pattern, std::string& res, uint32_t idx)
{
    int cs = 0;
    auto* p = pattern.data();
    auto* pe = p + pattern.size();
    auto* eof = pe;

    auto tm = std::time(nullptr);
    auto rl_now = *std::localtime(&tm);
    bool rl_pattern_found = false;
    bool rl_date_found = false;
    const char* rl_err = 0;
    int rl_pat_cnum = 0;
    std::string rl_fname;
    rl_fname.reserve(pattern.size());

    %% write init;
    %% write exec;

    if ( cs >= %%{ write first_final; }%%) {
        if (rl_pattern_found || rl_date_found) {
            res = rl_fname;
            return PATTERN_DONE;
        } else {
            char suffix[8];
            snprintf(suffix, sizeof(suffix), "-%03d", idx % 1000);
            auto extpos = rl_fname.find_last_of(".");
            if (extpos == std::string::npos) {
                rl_fname += suffix;
            } else {
                rl_fname.insert(extpos, suffix);
            }
            res = rl_fname;
            return PATTERN_NOMATCH;
        }
    } else
        return PATTERN_ERROR;
}

}
}


