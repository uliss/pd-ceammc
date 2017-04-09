#include "ceammc.h"
#include <glib.h>
#include <m_pd.h>

t_class* string_compare_class;
struct t_string_compare {
    t_object x_obj;
    t_symbol *str2;
    t_float f_out;
    t_outlet *out1;
};

static void string_compare_symbol(t_string_compare* x, t_symbol* s)
{
    gunichar ch = g_utf8_substring(x->str2->s_name, 0, 1)[0];
    gchar *char1 = g_utf8_strchr(s->s_name, 1024, ch);
    if (char1)
    {
        glong l_out = g_utf8_pointer_to_offset(s->s_name, char1);
        x->f_out = float(l_out);
        outlet_float(x->out1, x->f_out);
    }
    else
    {
        outlet_float(x->out1, -1.);
    }
    
}

static void* string_compare_new()
{
    t_string_compare* x = reinterpret_cast<t_string_compare*>(pd_new(string_compare_class));
    
    symbolinlet_new(&x->x_obj, &x->str2);
    
    x->out1 = outlet_new(&x->x_obj, &s_float);
    
    x->str2 = gensym("");
    
    return static_cast<void*>(x);
}

extern "C" void setup_string0x2ecompare()
{
    string_compare_class = class_new(gensym("string.compare"),
        reinterpret_cast<t_newmethod>(string_compare_new),
        reinterpret_cast<t_method>(0),
        sizeof(t_string_compare), 0, A_NULL);
    
    class_addsymbol(string_compare_class, string_compare_symbol);
}
