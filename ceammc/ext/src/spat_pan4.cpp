#include "spat_pan4.h"

static void* faust_new(t_symbol* s, int argc, t_atom* argv)
{
    t_faust* x = reinterpret_cast<t_faust*>(pd_new(faust_class));
    PdArgParser p(x, argc, argv);
    p.initFloatArg("angle", 1);
    return p.pd_obj();
}

extern "C" void setup_spat0x2epan4_tilde()
{
    internal_setup(gensym("spat.pan4~"));
}
