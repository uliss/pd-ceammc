#include "env_ar.h"

static void* faust_new(t_symbol* s, int argc, t_atom* argv)
{
    t_faust* x = reinterpret_cast<t_faust*>(pd_new(faust_class));
    PdArgParser p(x, argc, argv);
    p.initFloatArg("attack", 1);
    p.initFloatArg("release", 2);
    return p.pd_obj();
}

extern "C" void setup_env0x2ear_tilde()
{
    internal_setup(gensym("env.ar~"));
}
