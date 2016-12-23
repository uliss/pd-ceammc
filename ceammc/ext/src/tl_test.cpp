//
//  tl_test.cpp
//  
//
//  Created by Alex Nadzharov on 15/12/16.
//
//



#include "ceammc_gui_old.h"


struct test_obj1 : cm_gui_base_pd_object
{
    
};

UI_fun(test_obj1)::w_draw(t_gobj *z, struct _glist *glist)
{
    UI_Setup
    UI_Prop
    
    UI_Pset("width",150);
    UI_Pset("height",15);
    
    std::string color1 = (UI_Pf("_selected")==0) ?  "#00C0F0" :  "#0000C0";
    
    gui_rect("BASE", UI_x, UI_y, UI_Pf("width"), UI_Pf("height"), color1, "#F0F0F0", 1.0f);
    
}

UI_fun(test_obj1)::w_erase(t_gobj *z, struct _glist *glist)
{
    UI_Setup
    
    gui_delete("BASE");
}


UI_fun(test_obj1)::w_displace(t_gobj *z, struct _glist *glist, int dx, int dy)
{
    UI_Setup
    UI_Prop
    
    UI_basefun::w_displace(z, glist, dx, dy);
    
    gui_move("BASE", UI_x, UI_y, UI_Pf("width"), UI_Pf("height"));
    
    canvas_fixlinesfor(glist, (t_text *)z);
    
}


UI_fun(test_obj1)::new_ext(t_object *x, t_symbol *s, int argc, t_atom *argv)
{
    
    printf("additional init\n");

    
}

extern "C" void setup_tl0x2etest()
{
    cm_gui_object<test_obj1> class1;
    class1.setup("tl.test");
}

