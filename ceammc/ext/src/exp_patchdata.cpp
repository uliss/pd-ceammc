#include "ceammc.h"
#include <g_canvas.h>
#include <m_pd.h>
#include <stdlib.h>
//

t_class* exp_patchdata_class;
struct t_exp_patchdata {
    t_object x_obj;

    t_canvas* c1;
    t_outlet* out1;
};

static void exp_patchdata_dump(t_exp_patchdata* x, t_symbol* s, int argc, t_atom* argv)
{
    t_canvas* c1 = x->c1;

    if (c1) {
        printf("canvas ptr: %lu\n", (long)c1);

        t_binbuf* b1 = binbuf_new();

        canvas_saveto(x->c1, b1);

        int blen = 0;
        char* bchar;
        binbuf_gettext(b1, &bchar, &blen);
        t_atom* outlist = binbuf_getvec(b1);
        int outcount = binbuf_getnatom(b1);

        outlet_list(x->out1, &s_list, outcount, outlist);
    }
}

static void* exp_patchdata_new()
{

    t_exp_patchdata* x = reinterpret_cast<t_exp_patchdata*>(pd_new(exp_patchdata_class));
    x->c1 = canvas_getcurrent();
    x->out1 = outlet_new(&x->x_obj, &s_list);
    return static_cast<void*>(x);
}

extern "C" void setup_exp0x2epatchdata()
{

    exp_patchdata_class = class_new(gensym("exp.patchdata"),
        reinterpret_cast<t_newmethod>(exp_patchdata_new),
        reinterpret_cast<t_method>(0),
        sizeof(t_exp_patchdata), 0, A_NULL);
    class_addmethod(exp_patchdata_class, reinterpret_cast<t_method>(exp_patchdata_dump), gensym("dump"), A_NULL);
}
