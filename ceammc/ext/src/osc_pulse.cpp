#include "osc_pulse.h"

static void* faust_new(t_symbol* s, int argc, t_atom* argv)
{
    t_faust* x = reinterpret_cast<t_faust*>(pd_new(faust_class));
    PdArgParser p(x, argc, argv);
    p.signalFloatArg("freq", 1);
    p.initFloatArg("duty", 2);
    return p.pd_obj();
}

extern "C" void setup_osc0x2epulse_tilde()
{
    internal_setup(gensym("osc.pulse~"));
}

