# include "parser_clone.h"
# include <cstring>

namespace ceammc {
namespace parser {

static inline void set_args(CloneMessage& res, ArgumentType type, int16_t first, int16_t last, int16_t step, int8_t inlet) {
    res.first = first;
    res.last = last;
    res.step = step;
    res.type = type;
    res.inlet = inlet;
}

%%{
    machine clone_target;

    id = [1-9] ${ id = fc-'0'; } [0-9]{0,4} ${ id = 10*id+(fc-'0'); };
    id0 = id @{ id0 = id; };
    id1 = id @{ id1 = id; };
    step =  ':' [1-9] ${ step = fc-'0'; } [0-9]{0,4} ${ step = 10*step+(fc-'0'); };
    inlet = '|' [1-9] ${ inlet = fc-'0'; } [0-9]{0,3} ${ inlet = 10*inlet+(fc-'0'); };

    target_all =    ('*' inlet?)           @{ set_args(res, ARG_TYPE_ALL,    -1, -1, 1,    inlet); };
    target_random = ('?' inlet?)           @{ set_args(res, ARG_TYPE_RANDOM, -1, -1, 1,    inlet); };
    target_except = ('!' id inlet?)        @{ set_args(res, ARG_TYPE_EXCEPT, id, -1, 1,    inlet); };
    target_eq =     ('='? id step? inlet?) @{ set_args(res, ARG_TYPE_EQ,     id, -1, step, inlet); };
    target_gt =     ('>'  id inlet?)       @{ set_args(res, ARG_TYPE_GT,     id, -1, 1,    inlet); };
    target_ge =     ('>=' id inlet?)       @{ set_args(res, ARG_TYPE_GE,     id, -1, 1,    inlet); };
    target_lt =     ('<'  id inlet?)       @{ set_args(res, ARG_TYPE_LT,     id, -1, 1,    inlet); };
    target_le =     ('<=' id inlet?)       @{ set_args(res, ARG_TYPE_LE,     id, -1, 1,    inlet); };
    target_range =  (id0 '..' id1 step? inlet?)
                                           @{ set_args(res, ARG_TYPE_RANGE, id0, id1, step, inlet); };

    target =
        target_all
        | target_random
        | target_except
        | target_eq
        | target_gt
        | target_ge
        | target_lt
        | target_le
        | target_range;

    main := '#' target;
    write data;
}%%

bool parse_clone_action(const char* str, CloneMessage& res)
{
    const auto len = strlen(str);
    if (len == 0) {
        res.type = ARG_TYPE_NONE;
        return false;
    }

    int cs = 0;
    const char* p = str;
    const char* pe = p + len;
    const char* eof = pe;

    int16_t id = 0, id0 = 1, id1 = -1, step = 1, inlet = -1;

    %% write init;
    %% write exec;

    return cs >= %%{ write first_final; }%%;
}

%%{
    machine clone_action_name;

    action_name =
        'dsp'       @{ type = MSG_TYPE_DSP_SET; }
        | '~dsp'    @{ type = MSG_TYPE_DSP_TOGGLE; }
        | 'send'    @{ type = MSG_TYPE_SEND; }
        | 'spread'  @{ type = MSG_TYPE_SEND_SPREAD; };

    main := action_name;
    write data;
}%%

MessageType parse_clone_message_type(const char* str)
{
    const auto len = strlen(str);
    if (len == 0)
        return MSG_TYPE_NONE;

    int cs = 0;
    const char* p = str;
    const char* pe = p + len;
    const char* eof = pe;

    MessageType type = MSG_TYPE_NONE;

    %% write init;
    %% write exec;

    return (cs < %%{ write first_final; }%%) ? MSG_TYPE_NONE : type;
}

}
}
