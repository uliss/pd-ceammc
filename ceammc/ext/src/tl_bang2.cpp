////
////  tl_bang2.cpp
////  pd_ext
////
////  Created by Alex Nadzharov on 10/12/16.
////
////
//




#include "tl_bang2.hpp"

#include <iostream>

#include "tl_lib.hpp"

t_class *tl_bang2::ceammc_gui_pd_class;

t_class *tl_bang2::get_pd_class(){return tl_bang2::ceammc_gui_pd_class;}
void tl_bang2::set_pd_class(t_class* c1){tl_bang2::ceammc_gui_pd_class=c1;}


#pragma mark -

#define UI_Pf x->ui_property_get_float
#define UI_Ps x->ui_property_get_c_str
#define UI_ draw_wrappers::
#define UI_Set x->ui_property_set

#define UI_Setup  ceammc_gui_object *x  = (ceammc_gui_object*)z;UI_ dw_set_canvas(glist); UI_ dw_set_object((t_object*)x);

void tl_bang2::w_draw(t_gobj *z, t_glist *glist)
{
    UI_Setup
    
    std::string obj_color = (UI_Pf("_selected")==0)? "#00C0FF" : "#0000C0";
    
    printf ("coords %f %f", UI_Pf("x"), UI_Pf("y"));
    UI_ dw_rect("BASE", UI_Pf("x"), UI_Pf("y"), UI_Pf("width"), UI_Pf("height"),
                obj_color, "#C0C0C0", 1.0f);
//    UI_ dw_rect("VLINE", UI_Pf("x"), 0.0f, 1, 1000.0f,
//                obj_color, obj_color, 1.0f);
    UI_ dw_text("LABEL", UI_Pf("x")+UI_Pf("label_x"), UI_Pf("y")+UI_Pf("label_y"),
                "tl.bang", "#000000");
    
    //outlet test
    UI_ dw_rect("OUT", UI_Pf("x"), UI_Pf("y")+UI_Pf("height")-2,IOWIDTH, 2,
                "#000000", "#F0F0F0", 1.0f);

    
}
//
void tl_bang2::w_erase(t_gobj *z, t_glist *glist)
{
    UI_Setup
    
    UI_ dw_delete("BASE");
//    UI_ dw_delete("VLINE");
    UI_ dw_delete("LABEL");
    
    //outlet test
    UI_ dw_delete("OUT");
    
}

void tl_bang2::w_vis(t_gobj *z, t_glist *glist,int vis)
{
    if (vis)
        tl_bang2::w_draw(z, glist);
    else
        tl_bang2::w_erase(z, glist);
}

void tl_bang2::w_displace(t_gobj *z, t_glist *glist, int dx, int dy)
{
    ceammc_gui_object::w_displace(z,glist,dx,dy);
    
    UI_Setup
    
    float x_pos = UI_Pf("x");
    tll_ui_update_pos((t_object *)x, x_pos);
    
    printf("xpos [%lu] : %f\n", (long)x, x_pos);
    
//    int cue_idx = tll_cue_getnumber((t_object *)x);
//    char cuename[10];
//    sprintf(cuename, "cue %d", cue_idx);
//    UI_Set("cue_name",cuename);
    
    //UI_Set ("y",0);
    
    //TODO MOVE
    UI_ dw_delete("LABEL");
    UI_ dw_text("LABEL", UI_Pf("x")+UI_Pf("label_x"), UI_Pf("y")+UI_Pf("label_y"),
                "tl.bang", "#000000");
    
    UI_ dw_move("BASE", UI_Pf("x"), UI_Pf("y"), UI_Pf("width"), UI_Pf("height"));
    
    
    
//    UI_ dw_move("VLINE", UI_Pf("x"), 0, 1, 1000);
    
    //outlet test
    UI_ dw_move("OUT", UI_Pf("x"), UI_Pf("y")+UI_Pf("height")-2,IOWIDTH, 2);
    
    canvas_fixlinesfor(glist, (t_text *)z);
    
    tll_cue_dump();
}

void tl_bang2::w_select(t_gobj *z, t_glist *glist, int selected)
{
    UI_Setup
    
    UI_Set ("_selected", selected);
    
    tl_bang2::w_erase(z,glist);
    tl_bang2::w_draw(z, glist);
}

//void tl_bang2_object::w_delete(t_gobj *z, t_glist *glist)
//{
//    printf("bang2 delete!\n");
//
//}

////////////////////


#pragma mark -

t_newmethod tl_bang2_object::get_pd_class_new()
{
    printf("\n**tl_bang2::get_pd_class_new \n");
    return (t_newmethod)(tl_bang2::pd_class_new1);
}

t_method tl_bang2_object::get_pd_class_free()
{
    printf("\n**tl_bang2::get_pd_class_free \n");
    return (t_method)(tl_bang2::pd_class_free1);
}

void *tl_bang2::pd_class_new1(t_symbol *s, int argc, t_atom *argv)
{
    printf("\n**tl_bang2::pd_class_new1 \n");
    
    tl_bang2_object *obj1 = new tl_bang2_object();
    obj1->ui_properties_init();
    
    t_object *obj = (t_object*)ceammc_gui::pd_class_new_common((t_class*)tl_bang2::ceammc_gui_pd_class,(t_object*)obj1,s,argc,argv);
    
    printf("*** bang2 instance init base\n");
    tll_ui_add((t_object*)obj,((tl_bang2_object*)obj)->ui_property_get_float("x"));
    
    //outlet test
    ((tl_bang2_object*)obj)->outlet1 = outlet_new(obj, &s_list);
    
    return obj;
}

void tl_bang2::pd_class_free1(t_object *x)
{
    
    ceammc_gui::pd_class_free_common(x);
}

//void tl_bang2_object::pd_instance_init(t_object *obj)
//{
//    
//}



void tl_bang2_object::ui_properties_init()
{
    printf("bang2 property init!\n");
    
    ceammc_gui_object::ui_properties_init();
    
    //this->ui_property_set("cue_name", "cue x");
    
    this->ui_property_set("object_name", "tl.bang2");
    
}

#pragma mark -
void tl_bang2::tl_bang2_action(t_object *x) //, tl_t_list list)
{
    printf("bang action");
    outlet_bang(((tl_bang2_object*)x)->outlet1);
}

extern "C" {
    
    void setup_tl0x2ebang2()    //_tl0x2ebang2
    {
        tl_bang2 *gui = new tl_bang2();
        
        tl_bang2_object *obj1 = new tl_bang2_object(); //(tl_bang2_object*)malloc(16384); //
        
        obj1->ui_properties_init();
        
        tl_bang2::ceammc_gui_pd_class = gui->pd_setup((t_object*)obj1, "tl.bang2", tl_bang2::ceammc_gui_pd_class);
        
        gui->w_ = new t_widgetbehavior;
        
        //gui->w_->w_clickfn = w_proxy::pw_click;
        //gui->w_->w_deletefn = w_proxy::pw_delete;
        gui->w_->w_displacefn = tl_bang2::w_displace;
        gui->w_->w_getrectfn = ceammc_gui_object::w_getrect;
        gui->w_->w_selectfn = tl_bang2::w_select;
        gui->w_->w_visfn = tl_bang2::w_vis;
        
        tll_set_ui_action(reinterpret_cast<tl_ui_action>(&tl_bang2::tl_bang2_action));
        
        class_setwidget(tl_bang2::ceammc_gui_pd_class, gui->w_);
    }
    
}



