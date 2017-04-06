#include "dyn_limit2.h"

static void* faust_new(t_symbol* s, int argc, t_atom* argv)
{
    t_faust* x = reinterpret_cast<t_faust*>(pd_new(faust_class));
    PdArgParser p(x, argc, argv, false);
    return p.pd_obj();
}

extern "C" void setup_dyn0x2elimit2_tilde()
{
   internal_setup(gensym("dyn.limit2~"));
}

