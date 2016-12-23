//
//  tl_test.cpp
//  
//
//  Created by Alex Nadzharov on 15/12/16.
//
//



#include "lib/ceammc_gui.h"


struct test_cicm : ceammc_gui::base_pd_object
{
    
};



UI_fun(test_cicm)::new_ext(t_object *x, t_symbol *s, int argc, t_atom *argv)
{
    
    printf("additional init\n");

    
}

extern "C" void setup_ui0x2etest_cicm()
{
    ceammc_gui::object<test_cicm> class1;
    class1.setup("ui.test_cicm");
}

