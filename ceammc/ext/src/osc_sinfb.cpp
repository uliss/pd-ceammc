#include "osc_sinfb.h"

static void* faust_new(t_symbol* s, int argc, t_atom* argv)
{
    t_faust* x = reinterpret_cast<t_faust*>(pd_new(faust_class));
    PdArgParser p(x, argc, argv);
    t_float freq = 0;
    get_nth_float_arg(argc, argv, 1, &freq);
    pd_float((t_pd*)x, freq);

    p.initFloatArg("feedback", 2);
    return p.pd_obj();
}

extern "C" void setup_osc0x2esinfb_tilde()
{
    internal_setup(gensym("osc.sinfb~"));
}
