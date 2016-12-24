//
//  list_deinterleave.cpp
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

static t_eclass* list_deinterleave_class;
struct t_list_deinterleave {
    t_ebox x_gui;
    
    t_outlet *out1;
    t_outlet *out2;
    //AtomList* stored_list;
};

static void* list_deinterleave_new()
{
    t_list_deinterleave* x = (t_list_deinterleave*)eobj_new(list_deinterleave_class);
    
    x->out1 = outlet_new((t_object*)&x->x_gui, &s_list);
    x->out2 = outlet_new((t_object*)&x->x_gui, &s_list);
    //    outlet_new(&x->x_obj, &s_float);
    //    x->stored_list = new AtomList;
    
    inlet_new((t_object*)&x->x_gui, (t_pd*)&x, gensym("list"),gensym("list"));
    
    return static_cast<void*>(x);
}

static void list_deinterleave_free(t_list_deinterleave* x)
{
    //    delete x->stored_list;
}

#pragma mark -
extern "C" void setup_list0x2edeinterleave()
{
    list_deinterleave_class = eclass_new("list.deinterleave",
                                       (method)(&list_deinterleave_new),
                                       (method)(&list_deinterleave_free),
                                       sizeof(t_list_deinterleave), CLASS_NOINLET, A_GIMME, 0);
    //    class_addlist(list_delta_class, list_delta_list);
    //    class_addanything(list_delta_class, list_delta_clear);
    
    eclass_register(CLASS_OBJ, list_deinterleave_class);
}