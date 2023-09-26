# include "parser_timeline.h"
# include "ragel_common.h"
# include "parser_numeric.h"
# include "parser_units.h"
# include "fmt/core.h"

# include <cstring>
# include <limits>
# include <string>

%%{
    machine timeline_parser;
    include music_common "ragel_music.rl";
    include units_common "ragel_units.rl";

    action time_value_done {
        switch(ragel_type) {
        case TYPE_INT:   tl.duration = ragel_num.getInteger(); break;
        case TYPE_FLOAT: tl.duration = ragel_num.getFloat();   break;
        case TYPE_MSEC:  tl.duration = ragel_num.getFloat();   break;
        case TYPE_SEC:   tl.duration = ragel_num.getFloat() * 1000; break;
        case TYPE_MIN:   tl.duration = ragel_num.getFloat() * 60000; break;
        case TYPE_HOUR:  tl.duration = ragel_num.getFloat() * 3600000; break;
        default:
        break;
        }
    }

    action bar_unit_done {
        if (bar_inf) {
            tl.duration = std::numeric_limits<t_float>::max();
        } else {
            TimeLineBar bar;
            bar.count = bar_count;
            bar.div = bar_div;
            bar.num = bar_num;
            tl.bars.push_back(bar);
            std::cerr << fmt::format("bar_count={}, {}/{}\n", bar_count, bar_num, bar_div);
        }
    }

    action var_done {
        TimeLineVar var;
        var.name = gensym(var_name.c_str());
        tl.vars.push_back(var);
    }

    action var_char { var_name += *fpc; }

    sep = ' '+;
    time_suffix = unit_min | unit_msec | unit_sec | unit_hour;
    time_unit = (unit_float time_suffix) %time_value_done
        | units_smpte   %{ tl.duration = smpte.sec * 1000 + smpte.min * 60000 + smpte.hour * 3600000; }
        | ('inf' | '*') %{ tl.duration = std::numeric_limits<t_float>::max(); };

    tsig_div = '1'   %{ bar_div = 1; }
             | '2'   %{ bar_div = 2; }
             | '4'   %{ bar_div = 4; }
             | '8'   %{ bar_div = 8; }
             | '16'  %{ bar_div = 16; }
             | '32'  %{ bar_div = 32; }
             | '64'  %{ bar_div = 64; }
             | '128' %{ bar_div = 128; };
    bar_unit = ((num_uint %{ bar_inf = false; bar_count = ragel_num.vint; })?
                '*|' num_uint
                    >{ ragel_num.vint = 0; }
                    %{ bar_num = ragel_num.vint; ragel_num.vint = 0; }
                '/'  tsig_div
                '|') %bar_unit_done;
    bar_units = (bar_unit (sep bar_unit)*)
        >{ bar_inf = true; tl.bars.clear(); }
        %{ if(!bar_inf) tl.calcBarDuration(); };

    duration = 'duration' sep (time_unit | bar_units);

    #@preset  #id @signal #GET #SET @prop 324 @osc /isdf/wer/w555
    prop_default = ('@default' sep unit_float) %{ tl.vars.back().def = ragel_num.getFloat(); };
    var_prop = prop_default;
    var_props = var_prop (sep var_prop)*;
    var_name = ([a-zA-Z_] @var_char ([A-Z_a-z0-9] @var_char)*) >{ var_name.clear(); } %var_done;
    var = 'var' sep var_name (sep var_props)?;

    main := duration | var;
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
    bool bar_inf = false;
    int bar_count = 0;
    int bar_num = 0;
    int bar_div = 0;
    std::string var_name;

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
