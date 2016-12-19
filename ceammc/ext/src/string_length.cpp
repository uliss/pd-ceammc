#include "ceammc.h"
#include <glib.h>
#include <m_pd.h>

t_class* string_length_class;
struct t_string_length {
    t_object x_obj;
};

static void string_length_symbol(t_string_length* x, t_symbol* s)
{
    glong len = g_utf8_strlen(s->s_name, -1);
    outlet_float(x->x_obj.te_outlet, len);
}

static void* string_length_new()
{
    t_string_length* x = reinterpret_cast<t_string_length*>(pd_new(string_length_class));
    outlet_new(&x->x_obj, &s_float);
    return static_cast<void*>(x);
}

extern "C" void setup_string0x2elength()
{
    string_length_class = class_new(gensym("string.length"),
        reinterpret_cast<t_newmethod>(string_length_new),
        reinterpret_cast<t_method>(0),
        sizeof(t_string_length), 0, A_NULL);
    class_addsymbol(string_length_class, string_length_symbol);
}
