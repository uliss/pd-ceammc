#include <glib.h>
#include <m_pd.h>

t_class* string_to_lower_class;
struct t_string_to_lower {
    t_object x_obj;
};

static void symbol_to_lower_symbol(t_string_to_lower* x, t_symbol* s)
{
    gchar* res = g_utf8_strdown(s->s_name, -1);
    outlet_symbol(x->x_obj.te_outlet, gensym(res));
    g_free(res);
}

static void* symbol_to_lower_new()
{
    t_string_to_lower* x = reinterpret_cast<t_string_to_lower*>(pd_new(string_to_lower_class));
    outlet_new(&x->x_obj, &s_float);
    return static_cast<void*>(x);
}

extern "C" void setup_string0x2eto_lower()
{
    string_to_lower_class = class_new(gensym("string.to_lower"),
        reinterpret_cast<t_newmethod>(symbol_to_lower_new),
        reinterpret_cast<t_method>(0),
        sizeof(t_string_to_lower), 0, A_NULL);
    class_addsymbol(string_to_lower_class, symbol_to_lower_symbol);
}
