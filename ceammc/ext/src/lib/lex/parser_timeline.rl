# include "ceammc_music_theory_tempo.h"
# include "fmt/core.h"
# include "parser_numeric.h"
# include "parser_timeline.h"
# include "parser_units.h"
# include "ragel_common.h"

# include <cstring>
# include <limits>
# include <string>

using namespace ceammc::music;
using namespace ceammc::parser;
using namespace ceammc;

namespace {

struct RagelTempo {
    int ival { 0 };
    int fnum { 0 };
    int fden { 1 };
    int dur_num { 1 };
    int dur_den { 4 };
};

Tempo fromRagel(const RagelTempo& t)
{
    float bpm = t.ival + t.fnum / float(t.fden);
    Tempo res { bpm, t.dur_den };
    res.setDuration(Duration(t.dur_num, t.dur_den));
    return res;
}

struct RagelEventTime {
    double time;
    std::vector<double> times;
    int sign {0};
    t_symbol* rel_event {&s_};
    int bar  {-1};
    int beat {0};
    bool relative {false};

    void reset() { *this = RagelEventTime{}; }
};

struct RagelBar {
    int count = 0;
    int num = 0;
    int div = 0;
    bool inf = false;
};

struct FSM {
    double time_unit {0};
    std::string symbol;
    t_symbol* event_id {&s_};
    TimeLineEventSend act_send;
    TimeLineEventOutput act_out;
    TimeLineEventPreset act_preset;
    double event_time {0};
    ceammc::AtomList args;
    int bpm_bar {0};

    void onTimeSmpteDone(const ceammc::parser::fsm::SmpteData& smpte) {
        time_unit = smpte.sec * 1000 + smpte.min * 60000 + smpte.hour * 3600000;
    }

    void onTimeInfDone() {
        time_unit = std::numeric_limits<t_float>::max();
    }

    void onEventIdDone() {
        event_id = ::gensym(symbol.c_str());
        symbol.clear();
    }

    void onSendTarget() {
        act_send.target = ::gensym(symbol.c_str());
        symbol.clear();
    }

    void onArgsDone() {
        args = ceammc::AtomList::parseString(symbol.c_str());
    }

    t_symbol* gensym() { return ::gensym(symbol.c_str()); }

    t_symbol* genAnonEventName() {
        static int id = 0;
        char buf[64];
        sprintf(buf, ".event%d", id++);
        return ::gensym(buf);
    }
};
}

%%{
    machine timeline_parser;
    include music_common "ragel_music.rl";
    include units_common "ragel_units.rl";
    include music_common "ragel_music.rl";

    action time_unit_done {
        switch(ragel_type) {
        case TYPE_MSEC:  fsm.time_unit = ragel_num.getFloat();   break;
        case TYPE_SEC:   fsm.time_unit = ragel_num.getFloat() * 1000; break;
        case TYPE_MIN:   fsm.time_unit = ragel_num.getFloat() * 60000; break;
        case TYPE_HOUR:  fsm.time_unit = ragel_num.getFloat() * 3600000; break;
        default:
        break;
        }
    }

    action time_smpte_done { fsm.onTimeSmpteDone(smpte); }

    action time_inf_done { fsm.onTimeInfDone(); }

    action sym_add_char  { fsm.symbol += *fpc; }

    action event_declare_done { fsm.onEventIdDone(); tl.addEvent(fsm.event_id); }
    action event_send_done    { fsm.act_send.args = fsm.args; tl.addEventAction(fsm.event_id, fsm.act_send); }
    action event_out_done     { fsm.act_out.args = fsm.args;  tl.addEventAction(fsm.event_id, fsm.act_out); }
    action event_send_target  { fsm.onSendTarget(); }
    action event_args_done    { fsm.onArgsDone(); }
    action event_preset_done  { tl.addEventAction(fsm.event_id, fsm.act_preset); }

    action event_add_call     {
        fsm.onEventIdDone();
        for (auto& t: event_time.times)
            tl.addEventAt(fsm.event_id, t, event_time.relative);

        event_time.reset();
    }

    action event_add_anon_send {
        auto x = fsm.genAnonEventName();
        tl.addEvent(x);
        fsm.act_send.args = fsm.args;
        tl.addEventAction(x, fsm.act_send);
        tl.addEventAt(x, event_time.time, event_time.relative);
        event_time.reset();
    }

    action event_add_anon_out {
        auto x = fsm.genAnonEventName();
        tl.addEvent(x);
        fsm.act_out.args = fsm.args;
        tl.addEventAction(x, fsm.act_out);
        tl.addEventAt(x, event_time.time, event_time.relative);
        event_time.reset();
    }

    action event_add_anon_preset {
        auto x = fsm.genAnonEventName();
        tl.addEvent(x);
        tl.addEventAction(x, fsm.act_preset);
        tl.addEventAt(x, event_time.time, event_time.relative);
        event_time.reset();
    }

    action event_time_rel_event_id {
       event_time.rel_event = fsm.gensym();
       fsm.symbol.clear();
    }

    action event_time_abs_event_done {
        event_time.time = fsm.time_unit;
        event_time.times.assign(1, fsm.time_unit);
        fsm.time_unit = 0;
    }

    action event_time_rel_time_done {
        event_time.time = fsm.time_unit;
        fsm.time_unit = 0;
    }

    action event_time_bar_idx  {
        event_time.bar = ragel_num.vint;
        ragel_num.vint = 0;
    }

    action event_time_beat_idx  {
        event_time.beat = ragel_num.vint;
        ragel_num.vint = 0;
    }

    action event_time_rel_event_done {
        //fmt::println("#{}{}{}", event_time.rel_event->s_name, event_time.sign > 0 ? '+' : '-', event_time.time);

        bool found = false;
        for (auto t: tl.findEventTime(event_time.rel_event)) {
            found = true;
            event_time.times.push_back(t + event_time.time * event_time.sign);
        }

        if (!found)
            fmt::println("can't find event: '{}'", event_time.rel_event->s_name);

        fsm.time_unit = 0;
        fsm.symbol.clear();
    }

    action event_time_rel_bar_done {
        fmt::println("#{}.{}{}{}", event_time.bar, event_time.beat, event_time.sign > 0 ? '+' : '-', event_time.time);

        auto t = tl.findBarTime(event_time.bar, event_time.beat);
        event_time.times.clear();

        if (t >= 0) {
            event_time.times.push_back(t + (event_time.time * event_time.sign));
        } else {
            fmt::println("can't find bar #{}.{}", event_time.bar, event_time.beat);
            event_time.time = -1;
        }

        fsm.time_unit = 0;
    }

    action bar_unit_done {
        if (bar.inf) {
            tl.duration = std::numeric_limits<t_float>::max();
        } else {
            TimeLineBar b;
            b.count = bar.count;
            b.sig.set(bar.num, bar.div);
            tl.bars.push_back(b);
        }
    }

    action var_done {
        TimeLineVarDef var;
        var.name = gensym(fsm.symbol.c_str());
        tl.var_defs.push_back(var);
    }

    WS  = ' '+;

    # comments
    comment = WS ? '//' (any-'\n')*;

    # time values
    time_suffix = unit_min | unit_msec | unit_sec | unit_hour;
    time_unit   = (unit_ufloat time_suffix) %time_unit_done;
    time_smpte  = units_smpte               %time_smpte_done;
    time_inf    = '*'                       @time_inf_done;
    time_value  = time_unit | time_smpte;

    # values
    value_atom  = ((any - WS ) $sym_add_char)+;
    atom_float  = unit_float;
    atom_symbol = (value_atom - atom_float);
    atom_list   = (WS  (atom_float | atom_symbol))*;

    # MIDI
    midi3 = num_uint WS  num_uint (WS  num_uint)?;
    midi2 = num_uint (WS  num_uint)?;

    timeline_id       = [a-zA-Z_] $sym_add_char ([a-zA-Z_0-9] $sym_add_char)*;

    # event definitions
    event_id          = timeline_id;
    event_declare     = ('event' WS  event_id)     %event_declare_done;
    event_action_args = WS  ((any $sym_add_char)*) %event_args_done;
    event_out         = '!out'        event_action_args?;
    event_send        = '!send'       WS  atom_symbol %event_send_target event_action_args?;
    event_preset      = '!preset'     WS  num_uint >{ragel_num.vint = 0;} @{ fsm.act_preset.idx = ragel_num.vint; };
    event_osc         = '!osc'        event_action_args?;
    event_clock       = '!clock'      event_action_args?;
    event_midi_note   = '!midi:note'  WS  midi3;
    event_midi_ctl    = '!midi:ctl'   WS  midi3;
    event_midi_pgm    = '!midi:pgm'   WS  midi2;
    event_midi_sysex  = '!midi:sysex' event_action_args?;
    event_midi        = event_midi_note | event_midi_ctl | event_midi_pgm | event_midi_sysex;
    event_action      = event_out    %event_out_done
                      | event_send   %event_send_done
                      | event_preset %event_preset_done
                      | event_osc
                      | event_midi
                      ;
    event_def         = event_declare (WS  event_action)*;

    # var definitions
    var_id      = timeline_id;
    var_declare = 'var' WS  var_id >{ fsm.symbol.clear(); } %var_done;
    var_default = '@default' WS  unit_float %{ tl.var_defs.back().def = ragel_num.getFloat(); };
    var_preset  = '@preset'  WS  unit_float %{ tl.var_defs.back().preset = ragel_num.getFloat(); };
    var_method  = var_preset;
    var_prop    = var_default | var_method;
    var_def     = var_declare (WS  var_prop)*;

    # timeline event
    event_time_rel_sign  = ('+' | '-') @{ event_time.sign = (*fpc == '+') ? +1 : -1; };

    event_time_rel_bar   = '#' num_uint   %event_time_bar_idx
                            ('.' num_uint %event_time_beat_idx)?
                            (event_time_rel_sign time_unit %event_time_rel_time_done)?;

    event_time_rel_event = '#' event_id  %event_time_rel_event_id
                            (event_time_rel_sign time_unit %event_time_rel_time_done)?;

    event_time_rel_last = '+' @{ event_time.relative = true; };
    event_time           = event_time_rel_last? time_value      %event_time_abs_event_done
                         | event_time_rel_bar   %event_time_rel_bar_done
                         | event_time_rel_event %event_time_rel_event_done
                         ;
    event                = event_time
                           WS
                           'event'
                           WS  ( event_id       %event_add_call
                               | event_send     %event_add_anon_send
                               | event_out      %event_add_anon_out
                               | event_preset   %event_add_anon_preset
                               );

    # timeline var
    var_line             = '!line'   WS  unit_float WS  unit_float WS  time_unit;
    var_lineto           = '!lineto' WS  unit_float WS  time_unit;
    var_set              = '!set'    WS  unit_float;
    var_get              = '!get';
    var_action           = var_line | var_lineto | var_set;
    var                  = event_time WS  'var' WS  '$' var_id WS  var_action;

    # bar defs
    # COUNT? *|NUM/DIV|
    time_sig_num   = num_uint >{ ragel_num.vint = 0; } %{ bar.num = ragel_num.vint; ragel_num.vint = 0; };
    time_sig_div   = '1'   @{ bar.div = 1; }
                   | '2'   @{ bar.div = 2; }
                   | '4'   @{ bar.div = 4; }
                   | '8'   @{ bar.div = 8; }
                   | '16'  @{ bar.div = 16; }
                   | '32'  @{ bar.div = 32; }
                   | '64'  @{ bar.div = 64; }
                   | '128' @{ bar.div = 128; };
    time_sig       = time_sig_num '/'  time_sig_div;

    bar_count      = num_uint %{ bar.inf = false; bar.count = ragel_num.vint; };
    bar_definition = (bar_count? '*' '|' time_sig '|') %bar_unit_done;
    bar_defs = (bar_definition (WS  bar_definition)*)
        >{ bar.inf = true; tl.bars.clear(); }
        %{ tl.calcBarDuration(bar.inf); };

    duration_time = (time_value | time_inf) %{ tl.duration = fsm.time_unit; };
    duration      = 'tl' WS  (duration_time | bar_defs);

    # BPM defs
    bar_index     = '#' num_uint %{ fsm.bpm_bar = ragel_num.vint; ragel_num.vint = 0; };
    bpm_def       = 'bpm' WS  bar_index WS  bpm
                  %{ tl.setBarBpm(fsm.bpm_bar, fromRagel(bpm)); tl.calcBarDuration(bar.inf); };

    main := comment | duration | bpm_def | var_def | event_def | event | var;
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
    FSM fsm;
    fsm::SmpteData smpte;
    RagelEventTime event_time;
    RagelBar bar;
    RagelTempo bpm;

    %% write init;
    %% write exec;

    const bool ok = cs >= %%{ write first_final; }%%;
    return ok;
}

}
}
