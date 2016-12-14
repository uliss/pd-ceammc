////
////  tl_cue.cpp
////  pd_ext
////
////  Created by Alex Nadzharov on 10/12/16.
////
////
//




#include "tl_cue2.hpp"

#include <iostream>

#include "tl_lib.hpp"

#pragma mark -

#define UI_Pf x->ui_property_get_float
#define UI_Ps x->ui_property_get_c_str
#define UI_ draw_wrappers::
#define UI_Set x->ui_property_set

#define UI_Setup  ceammc_gui_object *x  = (ceammc_gui_object*)z;UI_ dw_set_canvas(glist); UI_ dw_set_object((t_object*)x);

void tl_cue2_object::w_draw(t_gobj *z, t_glist *glist)
{
    UI_Setup
    
    std::string obj_color = (UI_Pf("_selected")==0)? "#00C0FF" : "#0000C0";
    
    printf ("coords %f %f", UI_Pf("x"), UI_Pf("y"));
    UI_ dw_rect("BASE", UI_Pf("x"), UI_Pf("y"), UI_Pf("width"), UI_Pf("height"),
                obj_color, "#F0F0F0", 1.0f);
    UI_ dw_rect("VLINE", UI_Pf("x"), 0.0f, 1, 1000.0f,
                obj_color, obj_color, 1.0f);
    UI_ dw_text("LABEL", UI_Pf("x")+UI_Pf("label_x"), UI_Pf("y")+UI_Pf("label_y"),
                UI_Ps("cue_name"), "#000000");
    
    //outlet test
    UI_ dw_rect("OUT", UI_Pf("x"), UI_Pf("y")+UI_Pf("height")-2,IOWIDTH, 2,
                "#000000", "#F0F0F0", 1.0f);

    
}
//
void tl_cue2_object::w_erase(t_gobj *z, t_glist *glist)
{
    UI_Setup
    
    UI_ dw_delete("BASE");
    UI_ dw_delete("VLINE");
    UI_ dw_delete("LABEL");
    
    //outlet test
    UI_ dw_delete("OUT");
    
}

void tl_cue2_object::w_displace(t_gobj *z, t_glist *glist, int dx, int dy)
{
    this->ceammc_gui_object::w_displace(z,glist,dx,dy);
    
    UI_Setup
    
    float x_pos = UI_Pf("x");
    tll_cue_update_pos((t_object *)x, x_pos);
    
    printf("xpos [%lu] : %f\n", (long)x, x_pos);
    
    int cue_idx = tll_cue_getnumber((t_object *)x);
    char cuename[10];
    sprintf(cuename, "cue %d", cue_idx);
    UI_Set("cue_name",cuename);
    
    UI_Set ("y",0);
    
    UI_ dw_delete("LABEL");
    UI_ dw_text("LABEL", UI_Pf("x")+UI_Pf("label_x"), UI_Pf("y")+UI_Pf("label_y"),
                cuename, "#000000");
    
    UI_ dw_move("BASE", UI_Pf("x"), UI_Pf("y"), UI_Pf("width"), UI_Pf("height"));
    UI_ dw_move("VLINE", UI_Pf("x"), 0, 1, 1000);
    
    //outlet test
    UI_ dw_move("OUT", UI_Pf("x"), UI_Pf("y")+UI_Pf("height")-2,IOWIDTH, 2);
    
    canvas_fixlinesfor(glist, (t_text *)z);
    
    tll_cue_dump();
}

void tl_cue2_object::w_select(t_gobj *z, t_glist *glist, int selected)
{
    UI_Setup
    
    UI_Set ("_selected", selected);
    
    this->w_erase(z,glist);
    this->w_draw(z, glist);
}

//void tl_cue2_object::w_delete(t_gobj *z, t_glist *glist)
//{
//    printf("cue2 delete!\n");
//
//}

////////////////////

void tl_cue2_object::ui_property_init()
{
    printf("cue2 property init!\n");
    
    ceammc_gui_object::ui_property_init();
    
    this->ui_property_set("cue_name", "cue x");
    
    this->ui_property_set("object_name", "tl.cue2");

}


//void tl_cue2::pd_setup(t_object* obj)
//{
//    ceammc_gui::pd_setup(obj);
//    printf("nope\n");
//}

void tl_cue2_object::pd_instance_init(t_object *obj)
{
    printf("*** CUE2 instance init base\n");
    tll_cue_add((t_object*)obj,((tl_cue2_object*)obj)->ui_property_get_float("x"));
    
    //outlet test
    ((tl_cue2_object*)obj)->outlet1 = outlet_new(obj, &s_list);
}

void cpp__()
{
    
    //printf("extern cue2\n");
    
    ceammc_gui *gui = new ceammc_gui();
    
    tl_cue2_object *obj1 = new tl_cue2_object(); //(tl_cue2_object*)malloc(16384); //
    
    //printf ("obj %lu \n", obj1);
    obj1->ui_property_init();
    
    gui->pd_setup((t_object*)obj1);
    
}



extern "C" {
    
    void setup()    //_tl0x2ecue2
    {
        printf("extern C\n");
        
        cpp__();
    }
    
}



