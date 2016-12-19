#include "ceammc.h"
#include <m_pd.h>
#include <stdlib.h>
#include <string.h>

#define MIN(a, b)  (((a) < (b)) ? (a) : (b))

t_class* vector_dot_class;
struct t_vector_dot {
    t_object x_obj;
    t_atom *vec2;
    int vec2_size;
    
    t_inlet *in2;
};


static void vector_dot_list2(t_vector_dot* x, t_symbol* s, int argc, t_atom* argv)
{
    if (strcmp(s->s_name,"list2")==0)
    {
        x->vec2_size = argc;
        
        free(x->vec2);
        
        x->vec2 = (t_atom*)malloc(sizeof(t_atom)*argc);
        
        for (int i=0;i<argc;i++)
        {
            x->vec2[i] = argv[i];
        }
    }
}

static void vector_dot_list(t_vector_dot* x, t_symbol* s, int argc, t_atom* argv)
{
    if (argc < 1 || x->vec2_size < 1)
        return;
    
    float dot1 = 0;
    
    int min_size = MIN(argc, x->vec2_size);
    
    for (int i=0;i<min_size;i++)
    {
        dot1 += argv[i].a_w.w_float * x->vec2[i].a_w.w_float;
        
    }
    
    outlet_float(x->x_obj.te_outlet, dot1);
    
}

static void* vector_dot_new()
{
    t_vector_dot* x = reinterpret_cast<t_vector_dot*>(pd_new(vector_dot_class));
    outlet_new(&x->x_obj, &s_float);
    
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, gensym("list"), gensym("list2"));
    
    x->vec2 = (t_atom*)malloc(0);    //dummy
    x->vec2_size=0;
    
    return static_cast<void*>(x);
}

extern "C" void setup_vector0x2edot()
{
    vector_dot_class = class_new(gensym("vector.dot"),
                                      reinterpret_cast<t_newmethod>(vector_dot_new),
                                      reinterpret_cast<t_method>(0),
                                      sizeof(t_vector_dot), 0, A_NULL);
    class_addlist(vector_dot_class, vector_dot_list);
    
    class_addanything(vector_dot_class, vector_dot_list2);
    
    
}
