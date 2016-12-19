#include "ceammc.h"
#include <m_pd.h>
#include <stdlib.h>
#include <math.h>

#define MIN(a, b)  (((a) < (b)) ? (a) : (b))
#define CLAMP(x, low, high)  (((x) > (high)) ? (high) : (((x) < (low)) ? (low) : (x)))

t_class* list_shift_class;
struct t_list_shift {
    t_object x_obj;
    t_float idx_shift;
    
    t_atom *output;

};


static void list_shift_list(t_list_shift* x, t_symbol* s, int argc, t_atom* argv)
{
    if (argc < 1 )
        return;
    
    if (x->output) free(x->output);
    x->output = (t_atom*)malloc(sizeof(t_atom)*argc);
    
    
    for (int i=0;i<argc;i++)
    {
        float new_index = (i + x->idx_shift) ;
        
        new_index = CLAMP(new_index, -1, argc);     // temporary
        
        int idx1 = int(new_index);
        int idx2 = ((idx1+1)<argc)? (idx1+1) : (idx1);
        float mix1 = fmod(new_index,1) ;
        float inrange = float( (new_index>=0) && (new_index<argc) );
        
        float a1 = argv[idx1].a_w.w_float;
        float a2 = argv[idx2].a_w.w_float;
        
        x->output[i].a_type = A_FLOAT;      //?
        x->output[i].a_w.w_float = inrange * ( a1 * (1-mix1) + a2 * mix1 );
        
    }
    
    outlet_list(x->x_obj.te_outlet, s, argc, x->output);
    
}

static void* list_shift_new()
{
    t_list_shift* x = reinterpret_cast<t_list_shift*>(pd_new(list_shift_class));
    outlet_new(&x->x_obj, &s_list);
    
    floatinlet_new(&x->x_obj, &x->idx_shift);
    
    x->output = (t_atom*)malloc(0);         //dummy
    
    return static_cast<void*>(x);
}

extern "C" void setup_list0x2eshift()
{
    list_shift_class = class_new(gensym("list.shift"),
                                  reinterpret_cast<t_newmethod>(list_shift_new),
                                  reinterpret_cast<t_method>(0),
                                  sizeof(t_list_shift), 0, A_NULL);
    class_addlist(list_shift_class, list_shift_list);
    
    
    
}
