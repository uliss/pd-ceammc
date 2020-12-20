#include "ceammc.h"
#include <m_pd.h>

t_class* is_symbol_class;
struct t_is_symbol {
    t_object x_obj;
    t_outlet* out_value;
};

static void is_symbol_anything(t_is_symbol* x, t_symbol* /*s*/, int /*argc*/, t_atom* /*argv*/)
{
    outlet_float(x->x_obj.te_outlet, 0);
}

static void is_symbol_bang(t_is_symbol* x)
{
    outlet_float(x->x_obj.te_outlet, 0);
}

static void is_symbol_float(t_is_symbol* x, t_floatarg /*f*/)
{
    outlet_float(x->x_obj.te_outlet, 0);
}

static void is_symbol_list(t_is_symbol* x, t_symbol* /*s*/, int /*argc*/, t_atom* /*argv*/)
{
    outlet_float(x->x_obj.te_outlet, 0);
}

static void is_symbol_pointer(t_is_symbol* x, t_gpointer* /*pt*/)
{
    outlet_float(x->x_obj.te_outlet, 0);
}

static void is_symbol_symbol(t_is_symbol* x, t_symbol* s)
{
    outlet_symbol(x->out_value, s);
    outlet_float(x->x_obj.te_outlet, 1);
}

static void* is_symbol_new()
{
    t_is_symbol* x = reinterpret_cast<t_is_symbol*>(pd_new(is_symbol_class));
    outlet_new(&x->x_obj, &s_float);
    x->out_value = outlet_new(&x->x_obj, &s_symbol);
    return static_cast<void*>(x);
}

static void is_symbol_free(t_is_symbol* x)
{
    outlet_free(x->out_value);
}

extern "C" void is_symbol_setup()
{
    is_symbol_class = class_new(gensym("is_symbol"),
        reinterpret_cast<t_newmethod>(is_symbol_new),
        reinterpret_cast<t_method>(is_symbol_free),
        sizeof(t_is_symbol), 0, A_NULL);
    class_addanything(is_symbol_class, reinterpret_cast<t_method>(is_symbol_anything));
    class_addbang(is_symbol_class, reinterpret_cast<t_method>(is_symbol_bang));
    class_doaddfloat(is_symbol_class, reinterpret_cast<t_method>(is_symbol_float));
    class_addlist(is_symbol_class, reinterpret_cast<t_method>(is_symbol_list));
    class_addpointer(is_symbol_class, reinterpret_cast<t_method>(is_symbol_pointer));
    class_addsymbol(is_symbol_class, reinterpret_cast<t_method>(is_symbol_symbol));
}
