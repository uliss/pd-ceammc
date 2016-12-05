#include "ceammc.h"
#include <glib.h>
#include <m_pd.h>

t_class* string_join_class;
struct t_string_join {
    t_object x_obj;
    t_symbol *str2;
    t_symbol *str_out;
    t_outlet *out1;
};

static void string_join_symbol(t_string_join* x, t_symbol* s)
{

    gchar *str1 = s->s_name;    //?
    g_strlcat(str1, x->str2->s_name, 1024);
  
    x->str_out = gensym(str1);
    outlet_symbol(x->out1, x->str_out);
}

static void* string_join_new()
{
    t_string_join* x = reinterpret_cast<t_string_join*>(pd_new(string_join_class));
    
    symbolinlet_new(&x->x_obj, &x->str2);
    
    x->out1 = outlet_new(&x->x_obj, &s_symbol);
    
    return static_cast<void*>(x);
}

extern "C" void setup_string0x2ejoin()
{
    string_join_class = class_new(gensym("string.join"),
        reinterpret_cast<t_newmethod>(string_join_new),
        reinterpret_cast<t_method>(0),
        sizeof(t_string_join), 0, A_NULL);
    
    class_addsymbol(string_join_class, string_join_symbol);
}
