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

    action time_unit_done {
        switch(ragel_type) {
        case TYPE_MSEC:  tl.duration = ragel_num.getFloat();   break;
        case TYPE_SEC:   tl.duration = ragel_num.getFloat() * 1000; break;
        case TYPE_MIN:   tl.duration = ragel_num.getFloat() * 60000; break;
        case TYPE_HOUR:  tl.duration = ragel_num.getFloat() * 3600000; break;
        default:
        break;
        }
    }

    action time_smpte_done { tl.duration = smpte.sec * 1000 + smpte.min * 60000 + smpte.hour * 3600000; }

    action time_inf_done { tl.duration = std::numeric_limits<t_float>::max(); }

    action event_declare_done {
        TimeLineEvent ev;
        tl.events.push_back(ev);
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

    # comments
    comment = '//' (any-'\n')*;

    # time values
    time_suffix = unit_min | unit_msec | unit_sec | unit_hour;
    time_unit   = (unit_float time_suffix) %time_unit_done;
    time_smpte  = units_smpte              %time_smpte_done;
    time_inf    = '*'                      %time_inf_done;
    time_value  = time_unit | time_smpte;

    time_number = unit_float;
    time_symbol = any+ - time_number;
    symbol_name = [\-a-zA-Z_$] @var_char ([\-a-zA-Z_$0-9] @var_char)*;

    # event definitions
    event_id          = [a-zA-Z_] @var_char ([a-zA-Z_0-9\-] @var_char)*;
    event_declare     = 'event' sep event_id %event_declare_done;
    event_action_args = (sep (any* -- sep))*;
    event_out         = '!out'  event_action_args;
    event_send        = '!send' event_action_args;
    event_preset      = '!preset' event_action_args;
    event_osc         = '!osc' event_action_args;
    event_midi_note   = '!midi:note' event_action_args;
    event_midi_ctl    = '!midi:ctl' event_action_args;
    event_midi_pgm    = '!midi:pgm' event_action_args;
    event_midi_sysex  = '!midi:sysex' event_action_args;
    event_midi        = event_midi_note | event_midi_ctl | event_midi_pgm | event_midi_sysex;
    event_action      = event_out | event_send | event_preset | event_osc | event_midi;
    event_def         = event_declare (sep event_action)*;

    # var definitions
    var_id      = [a-zA-Z_] @var_char ([a-zA-Z_0-9\-] @var_char)*;
    var_declare = 'var' sep var_id >{ var_name.clear(); } %var_done;
    var_default = '@default' sep unit_float %{ tl.vars.back().def = ragel_num.getFloat(); };
    var_preset  = '@preset' sep unit_float  %{ tl.vars.back().preset = ragel_num.getFloat(); };
    var_prop    = var_default | var_preset;
    var_def     = var_declare (sep var_prop)*;

    # timeline event
    event_time_rel       = '+' | '-';
    event_time_rel_bar   = '#' num_uint ('.' num_uint)? (event_time_rel time_unit)?;
    event_time_rel_event = '#' event_id (event_time_rel time_unit)?;
    event_time           = time_value | event_time_rel_bar | event_time_rel_event;
    event                = event_time sep 'event' sep (event_id | event_action);

    # timeline var
    var_line             = 'line' sep unit_float sep unit_float sep time_unit;
    var_lineto           = 'lineto' sep unit_float sep time_unit;
    var_set              = 'set' sep unit_float;
    var_action           = var_line | var_lineto | var_set;
    var                  = event_time sep 'var' sep '$' var_id sep var_action;

    tsig_div = '1'   %{ bar_div = 1; }
             | '2'   %{ bar_div = 2; }
             | '4'   %{ bar_div = 4; }
             | '8'   %{ bar_div = 8; }
             | '16'  %{ bar_div = 16; }
             | '32'  %{ bar_div = 32; }
             | '64'  %{ bar_div = 64; }
             | '128' %{ bar_div = 128; };
    # COUNT? *|NUM/DIV|
    bar_unit = ((num_uint %{ bar_inf = false; bar_count = ragel_num.vint; })?
                '*|' num_uint
                    >{ ragel_num.vint = 0; }
                    %{ bar_num = ragel_num.vint; ragel_num.vint = 0; }
                '/'  tsig_div
                '|') %bar_unit_done;
    bar_units = (bar_unit (sep bar_unit)*)
        >{ bar_inf = true; tl.bars.clear(); }
        %{ if(!bar_inf) tl.calcBarDuration(); };

    duration = 'duration' sep (time_value | time_inf | bar_units);

    main := comment | duration | var_def | event_def | event | var;
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
