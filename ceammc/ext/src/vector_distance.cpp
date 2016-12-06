#include "ceammc.h"
#include <m_pd.h>
#include <stdlib.h>
#include <string.h>

#define MIN(a, b)  (((a) < (b)) ? (a) : (b))

t_class* vector_distance_class;
struct t_vector_distance {
    t_object x_obj;
    t_atom *vec2;
    int vec2_size;
    
    t_inlet *in2;
};


static void vector_distance_list2(t_vector_distance* x, t_symbol* s, int argc, t_atom* argv)
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

static void vector_distance_list(t_vector_distance* x, t_symbol* s, int argc, t_atom* argv)
{
    if (argc < 1 || x->vec2_size < 1)
        return;
    
    float len1 = 0;
    
    int min_size = MIN(argc, x->vec2_size);
    
    for (int i=0;i<min_size;i++)
    {
        len1 += argv[i].a_w.w_float*argv[i].a_w.w_float;
        len1 -= x->vec2->a_w.w_float*x->vec2[i].a_w.w_float;
        
    }
    
    len1 = qsqrt(len1);
    
    
    outlet_float(x->x_obj.te_outlet, len1);
    
}

static void* vector_distance_new()
{
    t_vector_distance* x = reinterpret_cast<t_vector_distance*>(pd_new(vector_distance_class));
    outlet_new(&x->x_obj, &s_float);
    
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, gensym("list"), gensym("list2"));
    
    x->vec2 = (t_atom*)malloc(0);    //dummy
    x->vec2_size=0;
    
    return static_cast<void*>(x);
}

extern "C" void setup_vector0x2edistance()
{
    vector_distance_class = class_new(gensym("vector.distance"),
                                  reinterpret_cast<t_newmethod>(vector_distance_new),
                                  reinterpret_cast<t_method>(0),
                                  sizeof(t_vector_distance), 0, A_NULL);
    class_addlist(vector_distance_class, vector_distance_list);
    
    class_addanything(vector_distance_class, vector_distance_list2);
    
    
}
