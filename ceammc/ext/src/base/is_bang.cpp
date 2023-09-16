#include "m_pd.h"

t_class* is_bang_class;
struct t_is_bang {
    t_object x_obj;
    t_outlet* out_value;
};

static void is_bang_anything(t_is_bang* x, t_symbol* /*s*/, int /*argc*/, t_atom* /*argv*/)
{
    outlet_float(x->x_obj.te_outlet, 0);
}

static void is_bang_bang(t_is_bang* x)
{
    outlet_bang(x->out_value);
    outlet_float(x->x_obj.te_outlet, 1);
}

static void is_bang_float(t_is_bang* x, t_floatarg /*f*/)
{
    outlet_float(x->x_obj.te_outlet, 0);
}

static void is_bang_list(t_is_bang* x, t_symbol* /*s*/, int /*argc*/, t_atom* /*argv*/)
{
    outlet_float(x->x_obj.te_outlet, 0);
}

static void is_bang_pointer(t_is_bang* x, t_gpointer* /*pt*/)
{
    outlet_float(x->x_obj.te_outlet, 0);
}

static void is_bang_symbol(t_is_bang* x, t_symbol* /*s*/)
{
    outlet_float(x->x_obj.te_outlet, 0);
}

static void* is_bang_new()
{
    t_is_bang* x = reinterpret_cast<t_is_bang*>(pd_new(is_bang_class));
    outlet_new(&x->x_obj, &s_float);
    x->out_value = outlet_new(&x->x_obj, &s_bang);
    return static_cast<void*>(x);
}

static void is_bang_free(t_is_bang* x)
{
    outlet_free(x->out_value);
}

extern "C" void is_bang_setup()
{
    is_bang_class = class_new(gensym("is_bang"),
        reinterpret_cast<t_newmethod>(is_bang_new),
        reinterpret_cast<t_method>(is_bang_free),
        sizeof(t_is_bang), 0, A_NULL);
    class_addanything(is_bang_class, reinterpret_cast<t_method>(is_bang_anything));
    class_addbang(is_bang_class, reinterpret_cast<t_method>(is_bang_bang));
    class_doaddfloat(is_bang_class, reinterpret_cast<t_method>(is_bang_float));
    class_addlist(is_bang_class, reinterpret_cast<t_method>(is_bang_list));
    class_addpointer(is_bang_class, reinterpret_cast<t_method>(is_bang_pointer));
    class_addsymbol(is_bang_class, reinterpret_cast<t_method>(is_bang_symbol));
}
