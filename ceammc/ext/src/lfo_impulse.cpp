#include "lfo_impulse.h"

static void* faust_new(t_symbol* s, int argc, t_atom* argv)
{
    t_faust* x = reinterpret_cast<t_faust*>(pd_new(faust_class));
    PdArgParser p(x, argc, argv, false);
    p.signalFloatArg("freq", 1);
    return p.pd_obj();
}

extern "C" void setup_lfo0x2eimpulse_tilde()
{
    internal_setup(gensym("lfo.impulse~"));
}
