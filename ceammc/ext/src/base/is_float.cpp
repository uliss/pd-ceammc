#include "m_pd.h"

t_class* is_float_class;
struct t_is_float {
    t_object x_obj;
    t_outlet* out_value;
};

static void is_float_anything(t_is_float* x, t_symbol* /*s*/, int /*argc*/, t_atom* /*argv*/)
{
    outlet_float(x->x_obj.te_outlet, 0);
}

static void is_float_bang(t_is_float* x)
{
    outlet_float(x->x_obj.te_outlet, 0);
}

static void is_float_float(t_is_float* x, t_floatarg f)
{
    outlet_float(x->out_value, f);
    outlet_float(x->x_obj.te_outlet, 1);
}

static void is_float_list(t_is_float* x, t_symbol* /*s*/, int /*argc*/, t_atom* /*argv*/)
{
    outlet_float(x->x_obj.te_outlet, 0);
}

static void is_float_pointer(t_is_float* x, t_gpointer* /*pt*/)
{
    outlet_float(x->x_obj.te_outlet, 0);
}

static void is_float_symbol(t_is_float* x, t_symbol* /*s*/)
{
    outlet_float(x->x_obj.te_outlet, 0);
}

static void* is_float_new()
{
    t_is_float* x = reinterpret_cast<t_is_float*>(pd_new(is_float_class));
    outlet_new(&x->x_obj, &s_float);
    x->out_value = outlet_new(&x->x_obj, &s_float);
    return static_cast<void*>(x);
}

static void is_float_free(t_is_float* x)
{
    outlet_free(x->out_value);
}

extern "C" void is_float_setup()
{
    is_float_class = class_new(gensym("is_float"),
        reinterpret_cast<t_newmethod>(is_float_new),
        reinterpret_cast<t_method>(is_float_free),
        sizeof(t_is_float), 0, A_NULL);
    class_addanything(is_float_class, reinterpret_cast<t_method>(is_float_anything));
    class_addbang(is_float_class, reinterpret_cast<t_method>(is_float_bang));
    class_doaddfloat(is_float_class, reinterpret_cast<t_method>(is_float_float));
    class_addlist(is_float_class, reinterpret_cast<t_method>(is_float_list));
    class_addpointer(is_float_class, reinterpret_cast<t_method>(is_float_pointer));
    class_addsymbol(is_float_class, reinterpret_cast<t_method>(is_float_symbol));
}
