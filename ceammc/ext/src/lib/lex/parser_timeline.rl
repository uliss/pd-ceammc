# include "parser_timeline.h"
# include "ragel_common.h"
# include "parser_numeric.h"
# include "parser_units.h"

# include <cstring>
# include <limits>

%%{
    machine timeline_parser;
    include music_common "ragel_music.rl";
    include units_common "ragel_units.rl";

    action time_value_done {
        switch(ragel_type) {
        case TYPE_SMPTE:
        break;
        case TYPE_INT:   tl.duration = ragel_num.getInteger(); break;
        case TYPE_FLOAT: tl.duration = ragel_num.getFloat();   break;
        case TYPE_MSEC:  tl.duration = ragel_num.getFloat();   break;
        case TYPE_SEC:   tl.duration = ragel_num.getFloat() * 1000; break;
        case TYPE_MIN:   tl.duration = ragel_num.getFloat() * 60000; break;
        case TYPE_HOUR:  tl.duration = ragel_num.getFloat() * 60 * 60000; break;
        default:
        break;
        }
    }

    sep = ' '+;
    time_suffix = unit_min | unit_msec | unit_sec | unit_hour;
    time_unit = (unit_float time_suffix?) %time_value_done
        | units_smpte   %{}
        | ('inf' | '*') %{ tl.duration = std::numeric_limits<t_float>::max(); };

    duration = 'duration' sep time_unit;

    main := duration;
    write data;
}%%

namespace ceammc {
namespace parser {

bool parse_timelime(const char* str, TimeLine& tl)
{
    const auto len = strlen(str);
    if (len == 0)
        return false;

    int cs = 0;
    const char* p = str;
    const char* pe = p + len;
    const char* eof = pe;

    DECLARE_RAGEL_COMMON_VARS;
    DECLARE_RAGEL_NUMERIC_VARS;
    fsm::SmpteData smpte;

    %% write init;
    %% write exec;

    const bool ok = cs >= %%{ write first_final; }%%;

    if (ok) {

     //   t = fromRagel(bpm);
    }

    return ok;
}

}
}
