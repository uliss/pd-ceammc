#include "ceammc.h"
#include <m_pd.h>

t_class* is_any_class;
struct t_is_any {
    t_object x_obj;
};

static void is_any_anything(t_is_any* x, t_symbol* s, int argc, t_atom* argv)
{
    outlet_float(x->x_obj.te_outlet, 1);
}

static void is_any_bang(t_is_any* x)
{
    outlet_float(x->x_obj.te_outlet, 0);
}

static void is_any_float(t_is_any* x, t_floatarg f)
{
    outlet_float(x->x_obj.te_outlet, 0);
}

static void is_any_list(t_is_any* x, t_symbol* s, int argc, t_atom* argv)
{
    outlet_float(x->x_obj.te_outlet, 0);
}

static void is_any_pointer(t_is_any* x, t_gpointer* pt)
{
    outlet_float(x->x_obj.te_outlet, 0);
}

static void is_any_symbol(t_is_any* x, t_symbol* s)
{
    outlet_float(x->x_obj.te_outlet, 0);
}

static void* is_any_new()
{
    t_is_any* x = reinterpret_cast<t_is_any*>(pd_new(is_any_class));
    outlet_new(&x->x_obj, &s_float);

    return static_cast<void*>(x);
}

extern "C" void setup_is0x2eany()
{
    is_any_class = class_new(gensym("is.any"),
        reinterpret_cast<t_newmethod>(is_any_new),
        reinterpret_cast<t_method>(0),
        sizeof(t_is_any), 0, A_NULL);
    class_addanything(is_any_class, is_any_anything);
    class_addbang(is_any_class, is_any_bang);
    class_addfloat(is_any_class, is_any_float);
    class_addlist(is_any_class, is_any_list);
    class_addpointer(is_any_class, is_any_pointer);
    class_addsymbol(is_any_class, is_any_symbol);
}
