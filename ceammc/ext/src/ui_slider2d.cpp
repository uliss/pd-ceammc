//
//  ui_slider2d.cpp
//  pd_ext
//
//  Created by Alex on 18/12/16.
//
//

#include <stdio.h>

#include "ceammc_atomlist.h"
#include "ceammc_format.h"

#include "lib/ceammc_gui.h"

struct ui_slider2d : ceammc_gui::base_pd_object
{
    t_ebox x_gui;
    t_outlet *out1;
    
    t_atom out_list[2];
    
    float shift_x;
    float range_x;
    float shift_y;
    float range_y;
    
    float _posx;
    float _posy;
    
    
};

namespace ceammc_gui {
    
    UI_fun(ui_slider2d)::wx_paint(t_object *z, t_object *view)
    {
        UI_Prop
        
        t_symbol *bgl = gensym("background_layer");
        //float size;
        t_rect rect;
        ebox_get_rect_for_view((t_ebox *)z, &rect);
        
        t_elayer *g = ebox_start_layer((t_ebox *)z, bgl, rect.width, rect.height);
        if(g)
        {
            ui_slider2d *zx = (ui_slider2d*) z;
            
//            float xx = UI_Pf("pos_x")*.5+.5;
//            float yy = UI_Pf("pos_y")*.5+.5;
            
            float xx = zx->_posx*.5+.5;
            float yy = zx->_posy*.5+.5;
            xx *= rect.width;
            yy *= rect.height;
            
            float knobsize = (rect.width*0.1);
            if (knobsize>(rect.height*0.1)) knobsize = rect.height*0.1;
            if (knobsize<5) knobsize = 5;
            if (knobsize>20) knobsize = 20;
            
            egraphics_set_line_width(g, 0.5);
            egraphics_set_color_hex(g, gensym ("#00C0F0"));
            
            egraphics_line(g, xx, 0, xx, rect.height);
            egraphics_line(g, 0, yy, rect.width, yy);
            
            egraphics_stroke(g);
            
            egraphics_set_line_width(g, 1.);
            egraphics_set_color_hex(g, gensym ((UI_Pf("_mouse_dn")==1) ? "#00C0FF" : "#707070"));
            egraphics_rectangle(g, xx-0.5*knobsize, yy-0.5*knobsize, knobsize, knobsize);
            egraphics_stroke(g);
            
            egraphics_set_color_hex(g, gensym ((UI_Pf("_mouse_dn")==1) ? "#003070" : "#C0C0C0"));
            egraphics_rectangle(g, xx-0.5*knobsize+1, yy-0.5*knobsize+1, knobsize-1, knobsize-1);
            egraphics_fill(g);
            
//            printf("draw\n");
            
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
//        UI_Prop
        
        ui_slider2d *zx = (ui_slider2d*) z;
        
        
        t_rect rect;
        ebox_get_rect_for_view((t_ebox *)z, &rect);
        
        zx->_posx = (pt.x/rect.width)*2.-1.;
        zx->_posy = (pt.y/rect.height)*2.-1.;
        
//        UI_Pset("pos_x",xx);
//        UI_Pset("pos_y",yy);
        
//        printf("move %f %f\n", xx,yy);
        
        ceammc_gui::object<ceammc_gui::base_pd_object>::ws_redraw(z);
        
        atom_setfloat(&((ui_slider2d*)z)->out_list[0], zx->_posx);
        atom_setfloat(&((ui_slider2d*)z)->out_list[1], zx->_posy);
        
        outlet_list( ((ui_slider2d*)z)->out1, &s_list, 2, ((ui_slider2d*)z)->out_list );
        
    }
    
    UI_fun(ui_slider2d)::wx_mousedown_ext(t_object* z, t_object *view, t_pt pt, long modifiers)
    {
//        UI_Prop
        
        ui_slider2d *zx = (ui_slider2d*) z;
        
        //printf("n mouse dn\n");
        
        t_rect rect;
        ebox_get_rect_for_view((t_ebox *)z, &rect);
        
        zx->_posx = (pt.x/rect.width)*2.-1.;
        zx->_posy = (pt.y/rect.height)*2.-1.;
        
//        UI_Pset("pos_x",xx);
//        UI_Pset("pos_y",yy);
        
        ceammc_gui::object<ceammc_gui::base_pd_object>::ws_redraw(z);
        
        atom_setfloat(&((ui_slider2d*)z)->out_list[0], zx->_posx);
        atom_setfloat(&((ui_slider2d*)z)->out_list[1], zx->_posy);
        
        outlet_list( ((ui_slider2d*)z)->out1, &s_list, 2, ((ui_slider2d*)z)->out_list );
    }
    
    UI_fun(ui_slider2d)::wx_mouseup_ext(t_object* z, t_object *view, t_pt pt, long modifiers)
    {
//        UI_Prop
        
        ui_slider2d *zx = (ui_slider2d*) z;
        
        //printf("n mouse up\n");
        
        //ceammc_gui::object<ceammc_gui::base_pd_object>::wx_mouseup(z, view, pt, modifiers);
        
        t_rect rect;
        ebox_get_rect_for_view((t_ebox *)z, &rect);
        
        zx->_posx = (pt.x/rect.width)*2.-1.;
        zx->_posy = (pt.y/rect.height)*2.-1.;
        
//        UI_Pset("pos_x",xx);
//        UI_Pset("pos_y",yy);
        
        ceammc_gui::object<ceammc_gui::base_pd_object>::ws_redraw(z);
        
        //t_atom out_list[2];
        
        atom_setfloat(&((ui_slider2d*)z)->out_list[0], zx->_posx);
        atom_setfloat(&((ui_slider2d*)z)->out_list[1], zx->_posy);
        
        outlet_list( zx->out1, &s_list, 2, zx->out_list );
    }
    
    
    UI_fun(ui_slider2d)::ui_properties_init_ext(ceammc_gui::properties *def_p)
    {
        
//        def_p->ui_property_set("pos_x", 0);
//        def_p->ui_property_set("pos_y", 0);
        def_p->ui_property_set("_mouse_down", 0);
        
        
    }
    
    UI_fun(ui_slider2d)::init_ext(t_eclass *z)
    {
        CLASS_ATTR_DEFAULT (z, "size", 0, "100. 100.");
        
        CLASS_ATTR_FLOAT(z, "shift_x", 0, ui_slider2d, shift_x);
        CLASS_ATTR_DEFAULT(z, "shift_x", 0, "-1");
        CLASS_ATTR_LABEL(z, "shift_x", 0, "shift_x");
        CLASS_ATTR_DEFAULT_SAVE_PAINT(z, "shift_x", 0, "-1");
        
        CLASS_ATTR_FLOAT(z, "range_x", 0, ui_slider2d, range_x);
        CLASS_ATTR_DEFAULT(z, "range_x", 0, "2");
        CLASS_ATTR_LABEL(z, "range_x", 0, "range_x");
        CLASS_ATTR_DEFAULT_SAVE_PAINT(z, "range_x", 0, "2");
        
        CLASS_ATTR_FLOAT(z, "shift_y", 0, ui_slider2d, shift_y);
        CLASS_ATTR_DEFAULT(z, "shift_y", 0, "-1");
        CLASS_ATTR_LABEL(z, "shift_y", 0, "shift_y");
        CLASS_ATTR_DEFAULT_SAVE_PAINT(z, "shift_y", 0, "-1");
        
        CLASS_ATTR_FLOAT(z, "range_y", 0, ui_slider2d, range_y);
        CLASS_ATTR_DEFAULT(z, "range_y", 0, "2");
        CLASS_ATTR_LABEL(z, "range_y", 0, "range_y");
        CLASS_ATTR_DEFAULT_SAVE_PAINT(z, "range_y", 0, "2");
        
    }
    
    UI_fun(ui_slider2d)::new_ext(t_object *x, t_symbol *s, int argcl, t_atom *argv)
    {
        ((ui_slider2d*)x)->out1 = outlet_new(x, &s_list);
    }
    
}

extern "C" void setup_ui0x2eslider2d()
{
    ceammc_gui::object<ui_slider2d> class1;
    class1.setup("ui.slider2d");
}