//
//  ui_slider2d.cpp
//  pd_ext
//
//  Created by Alex on 18/12/16.
//
//

#include <stdio.h>

#include "ceammc_gui_cicm.h"

struct ui_slider2d : cm_gui_base_pd_object
{
    t_ebox x_gui;
    t_outlet *out1;
    
    t_atom out_list[2];
    
};


UI_fun(ui_slider2d)::wx_paint(t_object *z, t_object *view)
{
    UI_Prop
    
    t_symbol *bgl = gensym("background_layer");
    float size;
    t_rect rect;
    ebox_get_rect_for_view((t_ebox *)z, &rect);
    
    t_elayer *g = ebox_start_layer((t_ebox *)z, bgl, rect.width, rect.height);
    if(g)
        
    {
        
        float xx = UI_Pf("pos_x")*.5+.5;
        float yy = UI_Pf("pos_y")*.5+.5;
        xx *= rect.width;
        yy *= rect.height;
        
        egraphics_set_line_width(g, 0.5);
        egraphics_set_color_hex(g, gensym ("#00C0F0"));
        
        egraphics_line(g, xx, 0, xx, rect.height);
        egraphics_line(g, 0, yy, rect.width, yy);
        
        egraphics_stroke(g);
        
        egraphics_set_line_width(g, 1.);
        egraphics_set_color_hex(g, gensym ((UI_Pf("_mouse_dn")==1) ? "#00C0FF" : "#707070"));
        egraphics_rectangle(g, xx-10, yy-10, 20, 20);
        egraphics_stroke(g);
        
        egraphics_set_color_hex(g, gensym ((UI_Pf("_mouse_dn")==1) ? "#003070" : "#C0C0C0"));
        egraphics_rectangle(g, xx-9, yy-9, 19, 19);
        egraphics_fill(g);
        
        printf("draw\n");
        
    }
    
    ebox_end_layer((t_ebox*)z, bgl);
    ebox_paint_layer((t_ebox *)z, bgl, 0., 0.);
}


UI_fun(ui_slider2d)::wx_mousemove_ext(t_object* z, t_object *view, t_pt pt, long modifiers)
{
    UI_Prop
    
    UI_Pset("_mouse_x",pt.x);
    UI_Pset("_mouse_y",pt.y);
    
}

UI_fun(ui_slider2d)::wx_mousedrag_ext(t_object* z, t_object *view, t_pt pt, long modifiers)
{
    UI_Prop
    
    t_rect rect;
    ebox_get_rect_for_view((t_ebox *)z, &rect);
    
    float xx = (pt.x/rect.width)*2.-1.;
    float yy = (pt.y/rect.height)*2.-1.;
    
    UI_Pset("pos_x",xx);
    UI_Pset("pos_y",yy);
    
    printf("move %f %f\n", xx,yy);
    
    cm_gui_object<cm_gui_base_pd_object>::ws_redraw(z);
    
    atom_setfloat(&((ui_slider2d*)z)->out_list[0], UI_Pf("pos_x"));
    atom_setfloat(&((ui_slider2d*)z)->out_list[1], UI_Pf("pos_y"));
    
    outlet_list( ((ui_slider2d*)z)->out1, &s_list, 2, ((ui_slider2d*)z)->out_list );
    
}

UI_fun(ui_slider2d)::wx_mousedown_ext(t_object* z, t_object *view, t_pt pt, long modifiers)
{
    UI_Prop

    printf("n mouse dn\n");
    
    
    t_rect rect;
    ebox_get_rect_for_view((t_ebox *)z, &rect);
    
    float xx = (pt.x/rect.width)*2.-1.;
    float yy = (pt.y/rect.height)*2.-1.;
    
    UI_Pset("pos_x",xx);
    UI_Pset("pos_y",yy);
    
    cm_gui_object<cm_gui_base_pd_object>::ws_redraw(z);
    
    atom_setfloat(&((ui_slider2d*)z)->out_list[0], UI_Pf("pos_x"));
    atom_setfloat(&((ui_slider2d*)z)->out_list[1], UI_Pf("pos_y"));
    
    outlet_list( ((ui_slider2d*)z)->out1, &s_list, 2, ((ui_slider2d*)z)->out_list );
}

UI_fun(ui_slider2d)::wx_mouseup_ext(t_object* z, t_object *view, t_pt pt, long modifiers)
{
    UI_Prop

    printf("n mouse up\n");
    
    //cm_gui_object<cm_gui_base_pd_object>::wx_mouseup(z, view, pt, modifiers);
    
    t_rect rect;
    ebox_get_rect_for_view((t_ebox *)z, &rect);
    
    float xx = (pt.x/rect.width)*2.-1.;
    float yy = (pt.y/rect.height)*2.-1.;
    
    UI_Pset("pos_x",xx);
    UI_Pset("pos_y",yy);
    
    cm_gui_object<cm_gui_base_pd_object>::ws_redraw(z);
    
    //t_atom out_list[2];
    
    atom_setfloat(&((ui_slider2d*)z)->out_list[0], UI_Pf("pos_x"));
    atom_setfloat(&((ui_slider2d*)z)->out_list[1], UI_Pf("pos_y"));
    
    outlet_list( ((ui_slider2d*)z)->out1, &s_list, 2, ((ui_slider2d*)z)->out_list );
}


UI_fun(ui_slider2d)::ui_properties_init_ext(cm_gui_properties *def_p)
{
    
    def_p->ui_property_set("pos_x", 0);
    def_p->ui_property_set("pos_y", 0);
    def_p->ui_property_set("_mouse_down", 0);
    
    
}

UI_fun(ui_slider2d)::init_ext(t_eclass *z)
{
    CLASS_ATTR_DEFAULT (z, "size", 0, "100. 100.");
    
}

UI_fun(ui_slider2d)::new_ext(t_object *x, t_symbol *s, int argcl, t_atom *argv)
{
    ((ui_slider2d*)x)->out1 = outlet_new(x, &s_list);
}


extern "C" void setup_ui0x2eslider2d()
{
    cm_gui_object<ui_slider2d> class1;
    class1.setup("ui.slider2d");
}