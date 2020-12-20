#include "ceammc.h"
#include <m_pd.h>

t_class* is_pointer_class;
struct t_is_pointer {
    t_object x_obj;
    t_outlet* out_value;
};

static void is_pointer_anything(t_is_pointer* x, t_symbol* /*s*/, int /*argc*/, t_atom* /*argv*/)
{
    outlet_float(x->x_obj.te_outlet, 0);
}

static void is_pointer_bang(t_is_pointer* x)
{
    outlet_float(x->x_obj.te_outlet, 0);
}

static void is_pointer_float(t_is_pointer* x, t_floatarg /*f*/)
{
    outlet_float(x->x_obj.te_outlet, 0);
}

static void is_pointer_list(t_is_pointer* x, t_symbol* /*s*/, int /*argc*/, t_atom* /*argv*/)
{
    outlet_float(x->x_obj.te_outlet, 0);
}

static void is_pointer_pointer(t_is_pointer* x, t_gpointer* pt)
{
    outlet_pointer(x->out_value, pt);
    outlet_float(x->x_obj.te_outlet, 1);
}

static void is_pointer_symbol(t_is_pointer* x, t_symbol* /*s*/)
{
    outlet_float(x->x_obj.te_outlet, 0);
}

static void* is_pointer_new()
{
    t_is_pointer* x = reinterpret_cast<t_is_pointer*>(pd_new(is_pointer_class));
    outlet_new(&x->x_obj, &s_float);
    x->out_value = outlet_new(&x->x_obj, &s_pointer);
    return static_cast<void*>(x);
}

static void is_pointer_free(t_is_pointer* x)
{
    outlet_free(x->out_value);
}

extern "C" void is_pointer_setup()
{
    is_pointer_class = class_new(gensym("is_pointer"),
        reinterpret_cast<t_newmethod>(is_pointer_new),
        reinterpret_cast<t_method>(is_pointer_free),
        sizeof(t_is_pointer), 0, A_NULL);
    class_addanything(is_pointer_class, reinterpret_cast<t_method>(is_pointer_anything));
    class_addbang(is_pointer_class, reinterpret_cast<t_method>(is_pointer_bang));
    class_doaddfloat(is_pointer_class, reinterpret_cast<t_method>(is_pointer_float));
    class_addlist(is_pointer_class, reinterpret_cast<t_method>(is_pointer_list));
    class_addpointer(is_pointer_class, reinterpret_cast<t_method>(is_pointer_pointer));
    class_addsymbol(is_pointer_class, reinterpret_cast<t_method>(is_pointer_symbol));
}
