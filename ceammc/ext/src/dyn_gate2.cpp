#include "dyn_gate2.h"

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

extern "C" void setup_dyn0x2egate2_tilde()
{
    t_symbol* s = gensym("dyn.gate2~");
    faust_class = class_new(s, reinterpret_cast<t_newmethod>(faust_new),
        reinterpret_cast<t_method>(faust_free),
        sizeof(t_faust),
        CLASS_DEFAULT,
        A_GIMME, A_NULL);
    class_addmethod(faust_class, nullfn, &s_signal, A_NULL);
    class_addmethod(faust_class, reinterpret_cast<t_method>(faust_dsp), gensym("dsp"), A_NULL);
    CLASS_MAINSIGNALIN(faust_class, t_faust, f);
    class_addanything(faust_class, faust_any);

    s_button = gensym("button");
    s_checkbox = gensym("checkbox");
    s_vslider = gensym("vslider");
    s_hslider = gensym("hslider");
    s_nentry = gensym("nentry");
    s_vbargraph = gensym("vbargraph");
    s_hbargraph = gensym("hbargraph");
}

