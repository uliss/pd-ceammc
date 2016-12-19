#include "flt_c_peak.h"

static void* faust_new(t_symbol* s, int argc, t_atom* argv)
{
    t_faust* x = reinterpret_cast<t_faust*>(pd_new(faust_class));

    const char* id = NULL;
    get_nth_symbol_arg(argc, argv, 1, &id);
    faust_new_internal(x, id);

    t_float param1 = 0;
    get_nth_float_arg(argc, argv, 1, &param1);
    pd_float((t_pd*)x, param1);

    t_float param2 = 0;
    get_nth_float_arg(argc, argv, 2, &param2);
    x->ui->setElementValue("param2", param2);

    return x;
}

extern "C" void setup_flt0x2ec_peak_tilde()
{
    t_symbol* s = gensym("flt.c_peak~");
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

