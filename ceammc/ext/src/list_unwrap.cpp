#include "ceammc.h"
#include <m_pd.h>
#include <stdlib.h>
#include <string.h>

#define MIN(a, b)  (((a) < (b)) ? (a) : (b))

t_class* list_unwrap_class;
struct t_list_unwrap {
    t_object x_obj;
    t_atom *list2;
    int list2_size;
    
    t_atom *outlist;
    int outlist_size;
    
    t_inlet *in2;
};


static void list_unwrap_list2(t_list_unwrap* x, t_symbol* s, int argc, t_atom* argv)
{
    if (strcmp(s->s_name,"list2")==0)
    {
        x->list2_size = argc;
        
        free(x->list2);
        x->list2 = (t_atom*)malloc(sizeof(t_atom)*argc);
        
        for (int i=0;i<argc;i++)
        {
            x->list2[i] = argv[i];
        }
    }
}

static void list_unwrap_list(t_list_unwrap* x, t_symbol* s, int argc, t_atom* argv)
{
    if (argc < 1 || x->list2_size < 1)
        return;
    
    int new_len=0;
    for (int i=0;i<x->list2_size;i++)
    {
        new_len += x->list2[i].a_w.w_float;
    }
    
    free(x->outlist);
    x->outlist = (t_atom*)malloc(sizeof(t_atom)*new_len);
    x->outlist_size = new_len;
    
    int i=0, j=0;
    while(i<new_len)
    {
        int c = x->list2[j].a_w.w_float;
        for (int k=0;k<c;k++)
        {
            x->outlist[i] = argv[j];
            i++;
        }
        j++;
    }
    
    outlet_list(x->x_obj.te_outlet, s, x->outlist_size, x->outlist);

    
}

static void* list_unwrap_new()
{
    t_list_unwrap* x = reinterpret_cast<t_list_unwrap*>(pd_new(list_unwrap_class));
    outlet_new(&x->x_obj, &s_float);
    
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, gensym("list"), gensym("list2"));
    
    x->list2 = (t_atom*)malloc(0);    //dummy
    x->list2_size=0;
    
    x->outlist = (t_atom*)malloc(0);    //dummy
    x->outlist_size=0;
    
    return static_cast<void*>(x);
}

extern "C" void setup_list0x2eunwrap()
{
    list_unwrap_class = class_new(gensym("list.unwrap"),
                                      reinterpret_cast<t_newmethod>(list_unwrap_new),
                                      reinterpret_cast<t_method>(0),
                                      sizeof(t_list_unwrap), 0, A_NULL);
    class_addlist(list_unwrap_class, list_unwrap_list);
    
    class_addanything(list_unwrap_class, list_unwrap_list2);
    
    
}
