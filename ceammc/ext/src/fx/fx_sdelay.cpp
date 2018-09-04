#include "fx_sdelay.h"

EXTERNAL_NEW
{
    FAUST_EXT* x = reinterpret_cast<FAUST_EXT*>(pd_new(FAUST_EXT_CLASS));
    PdArgParser p(x, argc, argv);
    p.initFloatArg("delay", 1);
    p.initFloatArg("feedback", 2);
    return p.pd_obj();
}

static void sdelay_clear(FAUST_EXT* x)
{
    x->dsp->instanceClear();
}

extern "C" void setup_fx0x2esdelay_tilde()
{
    internal_setup(gensym("fx.sdelay~"));
    class_addmethod(FAUST_EXT_CLASS, (t_method)sdelay_clear, gensym("clear"), A_NULL);
}
