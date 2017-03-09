#include "osc_saw.h"

static void* faust_new(t_symbol* s, int argc, t_atom* argv)
{
    t_faust* x = reinterpret_cast<t_faust*>(pd_new(faust_class));
    PdArgParser p(x, argc, argv, false);
    p.signalFloatArg("freq", 1);
    return p.pd_obj();
}

extern "C" void setup_osc0x2esaw_tilde()
{
    internal_setup(gensym("osc.saw~"));
}

