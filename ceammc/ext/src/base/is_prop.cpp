#include "m_pd.h"

t_class* is_prop_class;
struct t_is_prop {
    t_object x_obj;
    t_outlet* out_value;
};

static void is_prop_anything(t_is_prop* x, t_symbol* s, int argc, t_atom* argv)
{
    outlet_anything(x->out_value, s, argc, argv);
    outlet_float(x->x_obj.te_outlet, s->s_name[0] == '@');
}

static void is_prop_bang(t_is_prop* x)
{
    outlet_float(x->x_obj.te_outlet, 0);
}

static void is_prop_float(t_is_prop* x, t_floatarg /*f*/)
{
    outlet_float(x->x_obj.te_outlet, 0);
}

static void is_prop_list(t_is_prop* x, t_symbol* /*s*/, int /*argc*/, t_atom* /*argv*/)
{
    outlet_float(x->x_obj.te_outlet, 0);
}

static void is_prop_pointer(t_is_prop* x, t_gpointer* /*pt*/)
{
    outlet_float(x->x_obj.te_outlet, 0);
}

static void is_prop_symbol(t_is_prop* x, t_symbol* /*s*/)
{
    outlet_float(x->x_obj.te_outlet, 0);
}

static void* is_prop_new()
{
    t_is_prop* x = reinterpret_cast<t_is_prop*>(pd_new(is_prop_class));
    outlet_new(&x->x_obj, &s_float);
    x->out_value = outlet_new(&x->x_obj, &s_anything);
    return static_cast<void*>(x);
}

static void is_prop_free(t_is_prop* x)
{
    outlet_free(x->out_value);
}

extern "C" void is_prop_setup()
{
    is_prop_class = class_new(gensym("is_prop"),
        reinterpret_cast<t_newmethod>(is_prop_new),
        reinterpret_cast<t_method>(is_prop_free),
        sizeof(t_is_prop), CLASS_DEFAULT, A_NULL);

    class_addanything(is_prop_class, reinterpret_cast<t_method>(is_prop_anything));
    class_addbang(is_prop_class, reinterpret_cast<t_method>(is_prop_bang));
    class_doaddfloat(is_prop_class, reinterpret_cast<t_method>(is_prop_float));
    class_addlist(is_prop_class, reinterpret_cast<t_method>(is_prop_list));
    class_addpointer(is_prop_class, reinterpret_cast<t_method>(is_prop_pointer));
    class_addsymbol(is_prop_class, reinterpret_cast<t_method>(is_prop_symbol));
}
