#include "flt_lowshelf.h"

static void* faust_new(t_symbol* s, int argc, t_atom* argv)
{
    t_faust* x = reinterpret_cast<t_faust*>(pd_new(faust_class));
    PdArgParser p(x, argc, argv);
    p.initFloatArg("freq", 1);
    p.initFloatArg("gain", 2);
    return p.pd_obj();
}

extern "C" void setup_flt0x2elowshelf_tilde()
{
    internal_setup(gensym("flt.lowshelf~"));
}

