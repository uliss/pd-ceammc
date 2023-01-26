#include <m_pd.h>

t_class* is_list_class;
struct t_is_list {
    t_object x_obj;
    t_outlet* out_value;
};

static void is_list_anything(t_is_list* x, t_symbol* /*s*/, int /*argc*/, t_atom* /*argv*/)
{
    outlet_float(x->x_obj.te_outlet, 0);
}

static void is_list_bang(t_is_list* x)
{
    outlet_float(x->x_obj.te_outlet, 0);
}

static void is_list_float(t_is_list* x, t_floatarg /*f*/)
{
    outlet_float(x->x_obj.te_outlet, 0);
}

static void is_list_list(t_is_list* x, t_symbol* s, int argc, t_atom* argv)
{
    outlet_list(x->out_value, s, argc, argv);
    outlet_float(x->x_obj.te_outlet, 1);
}

static void is_list_pointer(t_is_list* x, t_gpointer* /*pt*/)
{
    outlet_float(x->x_obj.te_outlet, 0);
}

static void is_list_symbol(t_is_list* x, t_symbol* /*s*/)
{
    outlet_float(x->x_obj.te_outlet, 0);
}

static void* is_list_new()
{
    t_is_list* x = reinterpret_cast<t_is_list*>(pd_new(is_list_class));
    outlet_new(&x->x_obj, &s_float);
    x->out_value = outlet_new(&x->x_obj, &s_list);
    return static_cast<void*>(x);
}

static void is_list_free(t_is_list* x)
{
    outlet_free(x->out_value);
}

extern "C" void is_list_setup()
{
    is_list_class = class_new(gensym("is_list"),
        reinterpret_cast<t_newmethod>(is_list_new),
        reinterpret_cast<t_method>(is_list_free),
        sizeof(t_is_list), 0, A_NULL);
    class_addanything(is_list_class, reinterpret_cast<t_method>(is_list_anything));
    class_addbang(is_list_class, reinterpret_cast<t_method>(is_list_bang));
    class_doaddfloat(is_list_class, reinterpret_cast<t_method>(is_list_float));
    class_addlist(is_list_class, reinterpret_cast<t_method>(is_list_list));
    class_addpointer(is_list_class, reinterpret_cast<t_method>(is_list_pointer));
    class_addsymbol(is_list_class, reinterpret_cast<t_method>(is_list_symbol));
}
