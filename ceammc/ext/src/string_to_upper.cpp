#include <glib.h>
#include <m_pd.h>

t_class* string_to_upper_class;
struct t_string_to_upper {
    t_object x_obj;
};

static void symbol_to_upper_symbol(t_string_to_upper* x, t_symbol* s)
{
    gchar* res = g_utf8_strup(s->s_name, -1);
    outlet_symbol(x->x_obj.te_outlet, gensym(res));
    g_free(res);
}

static void* symbol_to_upper_new()
{
    t_string_to_upper* x = reinterpret_cast<t_string_to_upper*>(pd_new(string_to_upper_class));
    outlet_new(&x->x_obj, &s_float);
    return static_cast<void*>(x);
}

extern "C" void setup_string0x2eto_upper()
{
    string_to_upper_class = class_new(gensym("string.to_upper"),
        reinterpret_cast<t_newmethod>(symbol_to_upper_new),
        reinterpret_cast<t_method>(0),
        sizeof(t_string_to_upper), 0, A_NULL);
    class_addsymbol(string_to_upper_class, symbol_to_upper_symbol);
}
