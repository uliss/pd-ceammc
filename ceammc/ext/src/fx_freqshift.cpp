#include "fx_freqshift.h"

static void* faust_new(t_symbol* s, int argc, t_atom* argv)
{
    t_faust* x = reinterpret_cast<t_faust*>(pd_new(faust_class));
    PdArgParser p(x, argc, argv);
    return p.pd_obj();
}

extern "C" void setup_fx0x2efreqshift_tilde()
{
    internal_setup(gensym("fx.freqshift~"));
}

