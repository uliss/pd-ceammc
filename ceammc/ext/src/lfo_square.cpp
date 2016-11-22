#include "lfo_square.h"

extern "C" void setup_lfo0x2esquare_tilde()
{
    t_symbol* s = gensym("lfo.square~");
    faust_class = class_new(s, reinterpret_cast<t_newmethod>(faust_new),
        reinterpret_cast<t_method>(faust_free),
        sizeof(t_faust),
        CLASS_DEFAULT,
        A_GIMME, A_NULL);
    class_addmethod(faust_class, nullfn, &s_signal, A_NULL);
    class_addmethod(faust_class, reinterpret_cast<t_method>(faust_dsp), gensym("dsp"), A_NULL);
    CLASS_MAINSIGNALIN(faust_class, t_faust, f);

    s_button = gensym("button");
    s_checkbox = gensym("checkbox");
    s_vslider = gensym("vslider");
    s_hslider = gensym("hslider");
    s_nentry = gensym("nentry");
    s_vbargraph = gensym("vbargraph");
    s_hbargraph = gensym("hbargraph");
}
