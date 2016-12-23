//
//  list_split.cpp
//  pd_ext
//
//  Created by Alex on 23/12/16.
//
//

#include "ceammc_atomlist.h"
//#include <m_pd.h>
#include <stdlib.h>
#include "../../extra/CicmWrapper/Sources/cicm_wrapper_c.h"

using namespace ceammc;

static t_eclass* list_split_class;
struct t_list_split {
    t_ebox x_gui;
    
    t_outlet *out1;
    t_outlet *out2;
    
    //AtomList* stored_list;
};

static void* list_split_new(t_symbol *s, int argc, t_atom *argv)
{
    t_list_split* x = (t_list_split*)eobj_new(list_split_class);
    
    x->out1 = outlet_new((t_object*)&x->x_gui, &s_list);
    x->out2 = outlet_new((t_object*)&x->x_gui, &s_list);
    
    inlet_new((t_object*)&x->x_gui, (t_pd*)&x, gensym("list"), gensym("list"));
    inlet_new((t_object*)&x->x_gui, (t_pd*)&x, gensym("list"), gensym("list2"));
    
    //    x->stored_list = new AtomList;
    
    return static_cast<void*>(x);
}

static void list_split_free(t_list_split* x)
{
    //    delete x->stored_list;
}

#pragma mark -
extern "C" void setup_list0x2esplit()
{
    list_split_class = eclass_new("list.split",
                                       (method)(list_split_new),
                                       (method)(list_split_free),
                                       sizeof(t_list_split), CLASS_NOINLET, A_GIMME, 0);
    
    //eclass_init(list_split_class, 0);
    
    //    class_addlist(list_delta_class, list_delta_list);
    //    class_addanything(list_delta_class, list_delta_clear);
    
    eclass_register(CLASS_OBJ, list_split_class);
    
    
}