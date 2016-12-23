////
////  tl_bang2.cpp
////  pd_ext
////
////  Created by Alex Nadzharov on 10/12/16.
////
////
//


#include <iostream>

#include "tl_lib.hpp"

#include "ceammc_gui_old.h"

struct tl_bang : cm_gui_base_pd_object
{
    t_object x_gui;
    t_outlet *out1;
};


UI_fun(tl_bang)::w_draw(t_gobj *z, t_glist *glist)
{
    printf("cue2 draw\n");
    
    UI_Setup
    UI_Prop
    
    std::string obj_color = (UI_Pf("_selected")==0)? "#00C0FF" : "#0000C0";
    
    printf ("coords %f %f", UI_x, UI_y);
    gui_rect("BASE", UI_x, UI_y, UI_Pf("width"), UI_Pf("height"),
                obj_color, "#C0C0C0", 1.0f);
    gui_text("LABEL", UI_x+UI_Pf("label_x"), UI_y+UI_Pf("label_y"),
                "tl.bang", "#000000");
    
    gui_rect("OUT", UI_x, UI_y+UI_Pf("height")-2,IOWIDTH, 2,
                "#000000", "#F0F0F0", 1.0f);
    
}

//
UI_fun(tl_bang)::w_erase(t_gobj *z, t_glist *glist)
{
    UI_Setup
    
    gui_delete("BASE");
    gui_delete("LABEL");
    gui_delete("OUT");
    
}


UI_fun(tl_bang)::w_displace(t_gobj *z, t_glist *glist, int dx, int dy)
{
    UI_Setup
    UI_Prop
    
    UI_basefun::w_displace(z,glist,dx,dy);
    
    tll_ui_update_pos((t_object *)z, UI_x);
    
    printf("xpos [%lu] : %d\n", (long)x, UI_x);
    
//    int cue_idx = tll_cue_getnumber((t_object *)x);
//    char cuename[10];
//    sprintf(cuename, "cue %d", cue_idx);
//    UI_Set("cue_name",cuename);
    
    //TODO MOVE
    gui_delete("LABEL");
    gui_text("LABEL", UI_x+UI_Pf("label_x"), UI_y+UI_Pf("label_y"),
                "tl.bang", "#000000");
    
    gui_move("BASE", UI_x, UI_y, UI_Pf("width"), UI_Pf("height"));
    
    gui_move("OUT", UI_x, UI_y+UI_Pf("height")-2,IOWIDTH, 2);
    
    canvas_fixlinesfor(glist, (t_text *)z);
    
    //tll_cue_dump();
}


#pragma mark -


UI_fun(tl_bang)::new_ext(t_object* z, t_symbol *s, int argc, t_atom *argv)
{
    
    tll_ui_add((t_object*)z, UI_x);
    
    ((tl_bang*)z)->out1 =outlet_new(z, &s_bang);
    
    tll_ui_update_pos((t_object *)z, UI_x);
    
    
}



UI_fun(tl_bang)::free_ext(t_object *x)
{
    printf("*cue2 pd class free1\n");
    
    
    tll_ui_delete(x);
    
    printf("del cue %lu \n", (long)x);
    
}

UI_fun(tl_bang)::ui_properties_init_ext(cm_gui_properties *def_p)
{

    def_p->ui_property_set("label_x", 3);
    def_p->ui_property_set("width", 45);
    
    
}


#pragma mark -
void tl_bang_action(t_object *x)
{
    printf("bang action");
    outlet_bang(((tl_bang*)x)->out1);
}


extern "C" void setup_tl0x2ebang()
{
    cm_gui_object<tl_bang> class1;
    tll_set_ui_action((tl_bang_action));    //reinterpret_cast<tl_ui_action>
    class1.setup("tl.bang");
    
    
    
}



