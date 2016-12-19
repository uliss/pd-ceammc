#include "ceammc.h"
#include <m_pd.h>
#include <stdlib.h>

t_class* list_change_class;
struct t_list_change {
    t_object x_obj;
    t_atom *stored_list;
    int stored_list_len;
};

static inline bool list_not_equal(int l1c, t_atom* l1, int l2c, t_atom* l2)
{
    if (l1c!=l2c) return 1;
    
    for (int i=0;i<l1c;i++)
    {
        if (l1[i].a_w.w_float != l2[i].a_w.w_float) return 1;
        if (l1[i].a_type!=l2[i].a_type) return 1;
        
    }
    
    return 0;
}

static void list_change_list(t_list_change* x, t_symbol* s, int argc, t_atom* argv)
{
    if (argc < 1)
        return;

    if (list_not_equal(argc, argv, x->stored_list_len, x->stored_list))
    {
        outlet_list(x->x_obj.te_outlet, s, argc, argv);
    }
    
    free(x->stored_list);
    x->stored_list = (t_atom*)malloc(argc * sizeof(t_atom));
    for (int i=0;i<argc;i++)
    {
        x->stored_list[i] = argv[i];
    }
    x->stored_list_len = argc;
    
}

static void* list_change_new()
{
    t_list_change* x = reinterpret_cast<t_list_change*>(pd_new(list_change_class));
    outlet_new(&x->x_obj, &s_float);
    
    x->stored_list = (t_atom*)malloc(0);     //dummy
    
    return static_cast<void*>(x);
}

extern "C" void setup_list0x2echange()
{
    list_change_class = class_new(gensym("list.change"),
        reinterpret_cast<t_newmethod>(list_change_new),
        reinterpret_cast<t_method>(0),
        sizeof(t_list_change), 0, A_NULL);
    class_addlist(list_change_class, list_change_list);
}
