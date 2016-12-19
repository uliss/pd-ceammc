#include "fx_pitchshift.h"

static void* faust_new(t_symbol* s, int argc, t_atom* argv)
{
    t_faust* x = reinterpret_cast<t_faust*>(pd_new(faust_class));
    PdArgParser p(x, argc, argv);
    p.initFloatArg("window", 1);
    p.initFloatArg("fade", 2);
    return p.pd_obj();
}

extern "C" void setup_fx0x2epitchshift_tilde()
{
    internal_setup(gensym("fx.pitchshift~"));
}

