#include "dyn_gate.h"

static void* faust_new(t_symbol* s, int argc, t_atom* argv)
{
    t_faust* x = reinterpret_cast<t_faust*>(pd_new(faust_class));
    PdArgParser p(x, argc, argv);
    p.initFloatArg("threshold", 1);
    p.initFloatArg("attack", 2);
    p.initFloatArg("hold", 3);
    p.initFloatArg("release", 4);
    return p.pd_obj();
}

extern "C" void setup_dyn0x2egate_tilde()
{
    internal_setup(gensym("dyn.gate~"));
}

