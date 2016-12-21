//
//  tl_test.cpp
//  
//
//  Created by Alex Nadzharov on 15/12/16.
//
//



#include "ceammc_gui_cicm.h"


struct test_cicm : cm_gui_base_pd_object
{
    
};



UI_fun(test_cicm)::new_ext(t_object *x, t_symbol *s, int argc, t_atom *argv)
{
    
    printf("additional init\n");

    
}

extern "C" void setup_ui0x2etest_cicm()
{
    cm_gui_object<test_cicm> class1;
    class1.setup("ui.test_cicm");
}

