#include "ceammc.h"
#include <glib.h>
#include <m_pd.h>

t_class* string_slice_class;
struct t_string_slice {
    t_object x_obj;
    t_float slice;
    t_outlet *out1, *out2;
};

static void string_slice_symbol(t_string_slice* x, t_symbol* s)
{
    if (x->slice < 0)
    {
        outlet_symbol(x->out1, gensym(s->s_name));
    }
    else
    {
        glong l2 = g_utf8_strlen(s->s_name, -1);
        int i_slice = CLAMP(int(x->slice), 0, 1024);     //?
        glong l1 =  MIN(i_slice, l2);
        
        gchar *str1 = g_utf8_substring(s->s_name, 0, l1);
        gchar *str2 = g_utf8_substring(s->s_name, l1, l2);
        
        outlet_symbol(x->out1, gensym(str1));
        outlet_symbol(x->out2, gensym(str2));
        
        g_free(str1);
        g_free(str2);
    }
}

static void* string_slice_new()
{
    t_string_slice* x = reinterpret_cast<t_string_slice*>(pd_new(string_slice_class));
    
    x->out1 = outlet_new(&x->x_obj, &s_symbol);
    x->out2 = outlet_new(&x->x_obj, &s_symbol);
    
    floatinlet_new(&x->x_obj, &x->slice);
    
    x->slice = -1;
    
    return static_cast<void*>(x);
}

extern "C" void setup_string0x2eslice()
{
    string_slice_class = class_new(gensym("string.slice"),
        reinterpret_cast<t_newmethod>(string_slice_new),
        reinterpret_cast<t_method>(0),
        sizeof(t_string_slice), 0, A_NULL);
    class_addsymbol(string_slice_class, string_slice_symbol);
}
