//
//  list_interleave.cpp
//  pd_ext
//
//  Created by Alex Nadzharov on 23/12/16.
//
//

#include "ceammc_atomlist.h"
//#include <m_pd.h>
#include <stdlib.h>
#include "../../extra/CicmWrapper/Sources/cicm_wrapper_c.h"


using namespace ceammc;

static t_eclass* list_interleave_class;
struct t_list_interleave {
    t_ebox x_gui;
    
    t_outlet *out1;
    //AtomList* stored_list;
};

static void* list_interleave_new()
{
    t_list_interleave* x = (t_list_interleave*)eobj_new(list_interleave_class);
    x->out1 = outlet_new((t_object*)&x->x_gui, &s_list);
    
    inlet_new((t_object*)&x->x_gui, (t_pd*)&x, gensym("list"), gensym("list"));
    inlet_new((t_object*)&x->x_gui, (t_pd*)&x, gensym("list"), gensym("list2"));
    
    //x->stored_list = new AtomList;
    return static_cast<void*>(x);
}

static void list_interleave_free(t_list_interleave* x)
{
//    delete x->stored_list;
}

#pragma mark -
extern "C" void setup_list0x2einterleave()
{
    printf("init\n");
    
    list_interleave_class = eclass_new("list.interleave",
                                 (method)(&list_interleave_new),
                                 (method)(&list_interleave_free),
                                 sizeof(t_list_interleave), CLASS_NOINLET, A_GIMME, 0);
    
//    class_addlist(list_delta_class, list_delta_list);
//    class_addanything(list_delta_class, list_delta_clear);
    
    eclass_register(CLASS_OBJ, list_interleave_class);
}