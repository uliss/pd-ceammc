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

#include "ceammc_gui_cicm.h"

struct tl_cue : cm_gui_base_pd_object
{
    t_canvas *canvas;
};

UI_fun(tl_cue)::wx_paint(t_object *z, t_object *view)
{
    //    UI_Setup
    UI_Prop
    
    //
    
    t_symbol *bgl = gensym("background_layer");
    float size;
    t_rect rect;
    ebox_get_rect_for_view((t_ebox *)z, &rect);
    
    t_elayer *g = ebox_start_layer((t_ebox *)z, bgl, rect.width, rect.height);
    if(g)
    {
        
        t_symbol *obj_color = gensym ( 1 ? "#00C0FF" : "#0000C0" ); //(UI_Pf("_selected")==0)
        
        egraphics_rectangle(g, 0, 0, rect.width, rect.height);
        egraphics_set_color_hex(g, gensym("#F0F0F0"));
        egraphics_fill(g);
        
        egraphics_set_line_width(g, 1);
        
        egraphics_rectangle(g, 0, 0, rect.width, rect.height);
        egraphics_set_color_hex(g, obj_color);
        egraphics_stroke(g);
        
        egraphics_line(g, 0, 0, 0, 800);
        egraphics_set_color_hex(g, obj_color);
        egraphics_stroke(g);
        
//        t_etext *te = etext_layout_create();
//        t_efont *tf = efont_create(gensym("Monaco"), gensym(""), gensym("normal"), 12);
//        
//        etext_layout_set(te, UI_Ps("cue_name"), tf, rect.x, 0, rect.width, rect.height, ETEXT_LEFT, ETEXT_JLEFT, ETEXT_NOWRAP);
//        etext_layout_draw(te, g);
    }
    
    ebox_end_layer((t_ebox*)z, bgl);
    ebox_paint_layer((t_ebox *)z, bgl, 0., 0.);
    
    printf("paint\n");
}

//UI_fun(tl_cue)::w_draw(t_gobj *z, t_glist *glist)
//{
//    printf("cue2 draw\n");
//
//    UI_Setup
//    UI_Prop
//
//    rect.y = 0;
//
//    std::string obj_color = (UI_Pf("_selected")==0)? "#00C0FF" : "#0000C0";
//
//    gui_rect("BASE", rect.x, rect.y, UI_Pf("width"), UI_Pf("height"),
//             obj_color, "#F0F0F0", 1.0f);
//    gui_rect("VLINE", rect.x, 0.0f, 1, 1000.0f,
//             obj_color, obj_color, 1.0f);
//    gui_text("LABEL", rect.x+UI_Pf("label_x"), rect.y+UI_Pf("label_y"),
//             UI_Ps("cue_name"), "#000000");
//}
//
////
//UI_fun(tl_cue)::w_erase(t_gobj *z, t_glist *glist)
//{
//    UI_Setup
//
//    gui_delete("BASE");
//    gui_delete("VLINE");
//    gui_delete("LABEL");
//
//
//}
//
//UI_fun(tl_cue)::w_displace(t_gobj *z, t_glist *glist, int dx, int dy)
//{
//
//    UI_Setup
//    UI_Prop
//
//    UI_basefun::w_displace(z,glist,dx,dy);
//
//    tll_cue_update_pos((t_object *)z, rect.x);
//
//    printf("move cue %lu \n", (long)z);
//
//    int cue_idx = tll_cue_getnumber((t_object *)z);
//    char cuename[10];
//    sprintf(cuename, "cue_%d", cue_idx);
//    UI_Pset("cue_name",cuename);
//
//    rect.y = 0;
//
//    gui_delete("LABEL");
//    gui_text("LABEL", rect.x+UI_Pf("label_x"), rect.y+UI_Pf("label_y"),
//             cuename, "#000000");
//
//    gui_move("BASE", rect.x, rect.y, UI_Pf("width"), UI_Pf("height"));
//    gui_move("VLINE", rect.x, 0, 1, 1000);
//
//    //outlet test
//    //    gui_move("OUT", rect.x, rect.y+UI_Pf("height")-2,IOWIDTH, 2);
//
//    canvas_fixlinesfor(glist, (t_text *)z);
//
//    //tll_update_cue_guis((t_object*)z);
//
//    //tll_cue_dump();
//}

void tl_cue_displace(t_gobj *z, t_glist *glist, int dx, int dy)
{
    //ebox src
#ifdef _WINDOWS
    t_ebox* x = (t_ebox *)z;
    if(x->b_selected_box)
    {
        x->b_rect.x += dx;
        x->b_rect.y = 0;
        x->b_obj.o_obj.te_xpix += dx;
        x->b_obj.o_obj.te_ypix = 0;
        ebox_move(x);
    }
#else
    t_ebox* x = (t_ebox *)z;
    
    x->b_rect.x += dx;
    x->b_rect.y = 0;
    x->b_obj.o_obj.te_xpix += dx;
    x->b_obj.o_obj.te_ypix = 0;
    ebox_move(x);
#endif
}

UI_fun(tl_cue)::wx_oksize(t_object *z, t_rect *newrect)
{
    newrect->width=45;
    newrect->height=15;
    newrect->y = 0;
}




#pragma mark -

UI_fun(tl_cue)::init_ext(t_eclass *z)
{
    z->c_widget.w_displacefn = tl_cue_displace;
    
    //CLASS_ATTR_DEFAULT_SAVE_PAINT   (z, "bdcolor", 0, "0.5 0.5 0.5 0.");
    
    

    
}


UI_fun(tl_cue)::new_ext(t_object* z, t_symbol *s, int argc, t_atom *argv)
{
    //cm_gui_properties *x  = instances[(t_object *)z];
    
//    tll_cue_add((t_object*)z, rect.x);
//    
//    ((tl_cue*)z)->canvas = canvas_getcurrent();
//    
//    //printf("add cue %lu (gui %lu)\n", (long*)z,(long)& ((cm_gui_base_pd_object*)z)->x_gui);
//    
//    tll_cue_update_pos((t_object *)z, rect.x);
//    tll_update_cue_guis((t_object*)z);
    
    
#ifdef _WINDOWS
    t_ebox* x = (t_ebox *)z;
    if(x->b_selected_box)
    {
        x->b_rect.y = 0;
        x->b_obj.o_obj.te_ypix = 0;
        ebox_move(x);
    }
#else
    t_ebox* x = (t_ebox *)z;
    
    x->b_rect.y = 0;
    x->b_obj.o_obj.te_ypix = 0;
    ebox_move(x);
#endif
    
    x->b_boxparameters.d_borderthickness=0;
    x->b_flags = EBOX_GROWNO;
    
//
    
}

UI_fun(tl_cue)::free_ext(t_object *x)
{
    printf("*cue2 pd class free1\n");
    
//    tll_cue_delete(x);
//    
//    printf("del cue %lu \n", (long)x);
    
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
    
   // cm_gui_object<tl_cue>::w_displace((t_gobj*)z, ((tl_cue*)z)->canvas,0,0);
}




extern "C" void setup_tl0x2ecue()
{
    cm_gui_object<tl_cue> class1;
    class1.setup("tl.cue");
}





