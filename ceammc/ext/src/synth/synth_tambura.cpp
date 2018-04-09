#include "synth_tambura.h"

EXTERNAL_NEW
{
    FAUST_EXT* x = reinterpret_cast<FAUST_EXT*>(pd_new(FAUST_EXT_CLASS));

    PdArgParser p(x, argc, argv);
    return p.pd_obj();
}

static void synth_tambura(t_faust_tambura* x, t_float f)
{
    t_atom a;
    SETFLOAT(&a, f);
    pd_typedmess((t_pd*)x, gensym("pluck3"), 1, &a);
}

extern "C" void setup_synth0x2etambura_tilde()
{
    internal_setup(gensym("synth.tambura~"), false);
    class_addmethod(tambura_faust_class, reinterpret_cast<t_method>(synth_tambura), gensym("trigger"), A_DEFFLOAT);
}
