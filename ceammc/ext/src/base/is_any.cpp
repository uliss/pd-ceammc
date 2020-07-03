#include "ceammc.h"
#include <m_pd.h>

t_class* is_any_class;
struct t_is_any {
    t_object x_obj;
    t_outlet* out_value;
};

static void is_any_anything(t_is_any* x, t_symbol* s, int argc, t_atom* argv)
{
    outlet_anything(x->out_value, s, argc, argv);
    outlet_float(x->x_obj.te_outlet, 1);
}

static void is_any_bang(t_is_any* x)
{
    outlet_float(x->x_obj.te_outlet, 0);
}

static void is_any_float(t_is_any* x, t_floatarg /*f*/)
{
    outlet_float(x->x_obj.te_outlet, 0);
}

static void is_any_list(t_is_any* x, t_symbol* /*s*/, int /*argc*/, t_atom* /*argv*/)
{
    outlet_float(x->x_obj.te_outlet, 0);
}

static void is_any_pointer(t_is_any* x, t_gpointer* /*pt*/)
{
    outlet_float(x->x_obj.te_outlet, 0);
}

static void is_any_symbol(t_is_any* x, t_symbol* /*s*/)
{
    outlet_float(x->x_obj.te_outlet, 0);
}

static void* is_any_new()
{
    t_is_any* x = reinterpret_cast<t_is_any*>(pd_new(is_any_class));
    outlet_new(&x->x_obj, &s_float);
    x->out_value = outlet_new(&x->x_obj, &s_anything);
    return static_cast<void*>(x);
}

static void is_any_free(t_is_any* x)
{
    outlet_free(x->out_value);
}

extern "C" void is_any_setup()
{
    is_any_class = class_new(gensym("is_any"),
        reinterpret_cast<t_newmethod>(is_any_new),
        reinterpret_cast<t_method>(is_any_free),
        sizeof(t_is_any), CLASS_DEFAULT, A_NULL);

    class_addanything(is_any_class, reinterpret_cast<t_method>(is_any_anything));
    class_addbang(is_any_class, reinterpret_cast<t_method>(is_any_bang));
    class_doaddfloat(is_any_class, reinterpret_cast<t_method>(is_any_float));
    class_addlist(is_any_class, reinterpret_cast<t_method>(is_any_list));
    class_addpointer(is_any_class, reinterpret_cast<t_method>(is_any_pointer));
    class_addsymbol(is_any_class, reinterpret_cast<t_method>(is_any_symbol));
}
