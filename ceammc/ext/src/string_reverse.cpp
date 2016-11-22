#include "ceammc.h"
#include <glib.h>
#include <m_pd.h>

t_class* string_reverse_class;
struct t_string_reverse {
    t_object x_obj;
};

static void string_reverse_symbol(t_string_reverse* x, t_symbol* s)
{
    gchar* rev = g_utf8_strreverse(s->s_name, -1);
    outlet_symbol(x->x_obj.te_outlet, gensym(rev));
    g_free(rev);
}

static void* string_reverse_new()
{
    t_string_reverse* x = reinterpret_cast<t_string_reverse*>(pd_new(string_reverse_class));
    outlet_new(&x->x_obj, &s_symbol);
    return static_cast<void*>(x);
}

extern "C" void setup_string0x2ereverse()
{
    string_reverse_class = class_new(gensym("string.reverse"),
        reinterpret_cast<t_newmethod>(string_reverse_new),
        reinterpret_cast<t_method>(0),
        sizeof(t_string_reverse), 0, A_NULL);
    class_addsymbol(string_reverse_class, string_reverse_symbol);
}
