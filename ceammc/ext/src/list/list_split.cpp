//
//  list_split.cpp
//  pd_ext
//
//  Created by Alex Nadzharov on 23/12/16.
//
//

#include "ceammc_atomlist.h"
//#include <m_pd.h>
#include <stdlib.h>
#include "cicm_wrapper.h"

using namespace ceammc;

static t_eclass* list_split_class;
struct t_list_split {
    t_ebox x_gui;
    
    int list_len;
    int list_split;
    
    t_outlet *out1;
    t_outlet *out2;

};

static void list_split_l1(t_object *z, _symbol *s, int argc, t_atom *argv)
{
    AtomList list1;
    list1.fromPdData(argc, argv);
    
    t_list_split* zx = (t_list_split*)z;
    zx->list_len = argc;
    
    if (zx->list_split>zx->list_len) zx->list_split = zx->list_len;
    
    int n_split = zx->list_split ;
    
    AtomList out1 = list1.slice(0, n_split);
    AtomList out2 = list1.slice(n_split, zx->list_len);
    
    out1.output(zx->out1);
    out2.output(zx->out2);
    
}

static void list_split_l2(t_object *z, _symbol *s, int argc, t_atom *argv)
{
    if (argc<1) return;
    Atom a1;
    a1 = argv[0];
    
    t_list_split* zx = (t_list_split*)z;
    
    zx->list_split = (int)a1.asFloat();
    
}

static void* list_split_new(t_symbol *s, int argc, t_atom *argv)
{
    t_list_split* x = (t_list_split*)eobj_new(list_split_class);
    
    x->out1 = outlet_new((t_object*)&x->x_gui, &s_list);
    x->out2 = outlet_new((t_object*)&x->x_gui, &s_list);
    
    inlet_new((t_object*)&x->x_gui, (t_pd*)&x->x_gui.b_obj.o_obj.te_g, gensym("list"), gensym("in1"));
    inlet_new((t_object*)&x->x_gui, (t_pd*)&x->x_gui.b_obj.o_obj.te_g, gensym("list"), gensym("in2"));
    
    if (argc>0)
    {
        Atom a1;
        a1 = argv[0];
        x->list_split = a1.asFloat();
    }
    
    printf("ptr %lu\n",(long)x);
    
    return static_cast<void*>(x);
}

static void list_split_free(t_list_split* x)
{
    
}

#pragma mark -
extern "C" void setup_list0x2esplit()
{
    list_split_class = eclass_new("list.split",
                                       (method)(list_split_new),
                                       (method)(list_split_free),
                                       sizeof(t_list_split), CLASS_NOINLET, A_GIMME, 0);
    
    eclass_addmethod(list_split_class, (method)list_split_l1, "in1", A_GIMME, 0);
    eclass_addmethod(list_split_class, (method)list_split_l2, "in2", A_GIMME, 0);
    
    eclass_register(CLASS_OBJ, list_split_class);
    
    
}
