#include "ceammc.h"
#include <glib.h>
#include <m_pd.h>

t_class* is_file_class;
struct t_is_file {
    t_object x_obj;
    t_outlet* out_pass;
};

static void is_file_symbol(t_is_file* x, t_symbol* s)
{
    gboolean r = g_file_test(s->s_name, G_FILE_TEST_IS_REGULAR);
    outlet_float(x->x_obj.ob_outlet, r ? 1 : 0);
    if (r) {
        outlet_symbol(x->out_pass, s);
    }
}

static void* is_file_new()
{
    t_is_file* x = reinterpret_cast<t_is_file*>(pd_new(is_file_class));
    outlet_new(&x->x_obj, &s_float);
    x->out_pass = outlet_new(&x->x_obj, &s_symbol);
    return static_cast<void*>(x);
}

static void is_file_free(t_is_file* x)
{
    outlet_free(x->out_pass);
}

extern "C" void is_file_setup()
{
    is_file_class = class_new(gensym("is_file"),
        reinterpret_cast<t_newmethod>(is_file_new),
        reinterpret_cast<t_method>(is_file_free),
        sizeof(t_is_file), 0, A_NULL);
    class_addsymbol(is_file_class, is_file_symbol);
}
