# include "parser_clone.h"
# include <cstring>

namespace ceammc {
namespace parser {

static inline void set_args(CloneMessage& res, ArgumentType type, int16_t first, int16_t last, int16_t step) {
    res.args.first = first;
    res.args.last = last;
    res.args.step = step;
    res.args.type = type;
}

%%{
    machine clone;

    id = [1-9] ${ id = fc-'0'; } [0-9]{0,4} ${ id = 10*id+(fc-'0'); };
    id0 = id @{ id0 = id; };
    id1 = id @{ id1 = id; };
    step = '|' [1-9] ${ step = fc-'0'; } [0-9]{0,4} ${ step = 10*step+(fc-'0'); };

    action set_all {}

    args_all = '*'            @{ set_args(res, ARG_TYPE_ALL, -1, -1, 1); };
    args_random = '%'         @{ set_args(res, ARG_TYPE_RANDOM, -1, -1, 1); };
    args_except = ('!' id)    @{ set_args(res, ARG_TYPE_EXCEPT, id, -1, 1); };
    args_eq = ('='? id)       @{ set_args(res, ARG_TYPE_EQ, id, -1, 1); };
    args_gt = ('>'  id step?) @{ set_args(res, ARG_TYPE_GT, id, -1, step); };
    args_ge = ('>=' id step?) @{ set_args(res, ARG_TYPE_GE, id, -1, step); };
    args_lt = ('<'  id step?) @{ set_args(res, ARG_TYPE_LT, id, -1, step); };
    args_le = ('<=' id step?) @{ set_args(res, ARG_TYPE_LE, id, -1, step); };
    args_range =   (id0 '-' id1 step?)      @{ set_args(res, ARG_TYPE_RANGE, id0, id1, step); };
    args_spread0 = ('<:' id0? step?)        @{ set_args(res, ARG_TYPE_SPREAD, id0, -1, step); };
    args_spread1 = ('<:' id0 '-' id1 step?) @{ set_args(res, ARG_TYPE_SPREAD, id0, id1, step); };
    args_spread = args_spread0 | args_spread1;

    msg_type = '#' ${ res.msg_type = MSG_TYPE_SEND; }
        | '~'      ${ res.msg_type = MSG_TYPE_DSP_SET; }
        | '^~'     ${ res.msg_type = MSG_TYPE_DSP_TOGGLE; };

    args =
        args_all
        | args_random
        | args_except
        | args_eq
        | args_gt
        | args_ge
        | args_lt
        | args_le
        | args_range
        | args_spread;

    act = msg_type args;

    main := act;
    write data;
}%%

bool parse_clone_action(const char* str, CloneMessage& res)
{
    const auto len = strlen(str);
    if (len == 0) {
        res.msg_type = MSG_TYPE_NONE;
        return false;
    }

    int cs = 0;
    const char* p = str;
    const char* pe = p + len;
    const char* eof = pe;

    int16_t id = 0, id0 = 1, id1 = -1, step = 1;

    %% write init;
    %% write exec;

    return cs >= %%{ write first_final; }%%;
}

}
}
