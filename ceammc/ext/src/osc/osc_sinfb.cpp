#include "osc_sinfb.h"

EXTERNAL_NEW
{
    FAUST_EXT* x = reinterpret_cast<FAUST_EXT*>(pd_new(FAUST_EXT_CLASS));

    PdArgParser p(x, argc, argv);
    t_float freq = 0;
    get_nth_float_arg(argc, argv, 1, &freq);
    pd_float((t_pd*)x, freq);

    p.initFloatArg("feedback", 2);
    return p.pd_obj();
}

EXTERNAL_SETUP(osc);
