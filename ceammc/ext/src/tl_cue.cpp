////
////  tl_cue.cpp
////  pd_ext
////
////  Created by Alex Nadzharov on 10/12/16.
////
////
//


#include <iostream>
#include "tl_lib.hpp"

#include "ceammc_gui_old.h"

struct tl_cue : cm_gui_base_pd_object
{
    t_canvas *canvas;
};

UI_fun(tl_cue)::w_draw(t_gobj *z, t_glist *glist)
{
    printf("cue2 draw\n");
    
    UI_Setup
    UI_Prop
    
    UI_y = 0;
    
    std::string obj_color = (UI_Pf("_selected")==0)? "#00C0FF" : "#0000C0";

    gui_rect("BASE", UI_x, UI_y, UI_Pf("width"), UI_Pf("height"),
             obj_color, "#F0F0F0", 1.0f);
    gui_rect("VLINE", UI_x, 0.0f, 1, 1000.0f,
             obj_color, obj_color, 1.0f);
    gui_text("LABEL", UI_x+UI_Pf("label_x"), UI_y+UI_Pf("label_y"),
             UI_Ps("cue_name"), "#000000");
}

//
UI_fun(tl_cue)::w_erase(t_gobj *z, t_glist *glist)
{
    UI_Setup
    
    gui_delete("BASE");
    gui_delete("VLINE");
    gui_delete("LABEL");

    
}

UI_fun(tl_cue)::w_displace(t_gobj *z, t_glist *glist, int dx, int dy)
{
    
    UI_Setup
    UI_Prop
    
    UI_basefun::w_displace(z,glist,dx,dy);
    
    tll_cue_update_pos((t_object *)z, UI_x);
    
    printf("move cue %lu \n", (long)z);
    
    int cue_idx = tll_cue_getnumber((t_object *)z);
    char cuename[10];
    sprintf(cuename, "cue_%d", cue_idx);
    UI_Pset("cue_name",cuename);
    
    UI_y = 0;
    
    gui_delete("LABEL");
    gui_text("LABEL", UI_x+UI_Pf("label_x"), UI_y+UI_Pf("label_y"),
             cuename, "#000000");
    
    gui_move("BASE", UI_x, UI_y, UI_Pf("width"), UI_Pf("height"));
    gui_move("VLINE", UI_x, 0, 1, 1000);
    
    //outlet test
    //    gui_move("OUT", UI_x, UI_y+UI_Pf("height")-2,IOWIDTH, 2);
    
    canvas_fixlinesfor(glist, (t_text *)z);
    
    //tll_update_cue_guis((t_object*)z);
    
    //tll_cue_dump();
}






#pragma mark -



UI_fun(tl_cue)::new_ext(t_object* z, t_symbol *s, int argc, t_atom *argv)
{
    //cm_gui_properties *x  = instances[(t_object *)z];
    
    tll_cue_add((t_object*)z, UI_x);
    
    ((tl_cue*)z)->canvas = canvas_getcurrent();
    
    //printf("add cue %lu (gui %lu)\n", (long*)z,(long)& ((cm_gui_base_pd_object*)z)->x_gui);
    
    tll_cue_update_pos((t_object *)z, UI_x);
    tll_update_cue_guis((t_object*)z);

}

UI_fun(tl_cue)::free_ext(t_object *x)
{
    printf("*cue2 pd class free1\n");
    
    tll_cue_delete(x);
    
    printf("del cue %lu \n", (long)x);
    
}


UI_fun(tl_cue)::ui_properties_init_ext(cm_gui_properties *def_p)
{
    
    printf("cue2 property init!\n");
    def_p->ui_property_set("cue_name", "cue_x");
    
    
}

#pragma mark -

void tl_cue_drawcue(t_object *z)
{
    printf("cue redraw\n");
    
    cm_gui_object<tl_cue>::w_displace((t_gobj*)z, ((tl_cue*)z)->canvas,0,0);
}




extern "C" void setup_tl0x2ecue()
{
    cm_gui_object<tl_cue> class1;
    class1.setup("tl.cue");
}





