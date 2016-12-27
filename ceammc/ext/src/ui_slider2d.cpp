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

struct ui_slider2d : public ceammc_gui::base_pd_object
{
    t_ebox x_gui;
    
    float mouse_x;
    float mouse_y;
    int mouse_dn;
    bool _selected;
    
    t_outlet *out1;
    
    t_atom out_list[2];
    
    float shift_x;
    float range_x;
    float shift_y;
    float range_y;
    
    float _posx;
    float _posy;
    
    t_etext *txt_min;
    t_etext *txt_max;
    t_efont *txt_font;
    
    
};

namespace ceammc_gui {
    
    UI_fun(ui_slider2d)::wx_paint(t_object *z, t_object *view)
    {
//        UI_Prop
        
        t_symbol *bgl = gensym("background_layer");
        t_rect rect;
        ebox_get_rect_for_view((t_ebox *)z, &rect);
        
        t_elayer *g = ebox_start_layer((t_ebox *)z, bgl, rect.width, rect.height);
        
        ui_slider2d *zx = (ui_slider2d*)z;
        if(g)
        {
            ui_slider2d *zx = (ui_slider2d*) z;
            
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
            egraphics_set_color_hex(g, gensym (zx->mouse_dn ? "#00C0FF" : "#707070"));      //(UI_Pf("_mouse_dn")==1)
            egraphics_rectangle(g, xx-0.5*knobsize, yy-0.5*knobsize, knobsize, knobsize);
            egraphics_stroke(g);
            
            egraphics_set_color_hex(g, gensym (zx->mouse_dn ? "#003070" : "#C0C0C0"));
            egraphics_rectangle(g, xx-0.5*knobsize+1, yy-0.5*knobsize+1, knobsize-1, knobsize-1);
            egraphics_fill(g);
            
            
        }
        
        char c_min[30];
        sprintf(c_min, "[%.2f..%.2f]", zx->shift_x,zx->range_x+zx->shift_x );
    
        char c_max[30];
        sprintf(c_max, "[%.2f..%.2f]", zx->shift_y,zx->range_y+zx->shift_y);
        
        etext_layout_set(zx->txt_min, c_min, zx->txt_font, rect.width-3, rect.height-12, rect.width, rect.height/2, ETEXT_UP_RIGHT, ETEXT_JRIGHT, ETEXT_WRAP);
        etext_layout_draw(zx->txt_min, g);
        
        etext_layout_set(zx->txt_max, c_max, zx->txt_font, 3, 12, rect.width*2, rect.height/2, ETEXT_DOWN_LEFT, ETEXT_JLEFT, ETEXT_WRAP);
        etext_layout_draw(zx->txt_max, g);
        
        ebox_end_layer((t_ebox*)z, bgl);
        ebox_paint_layer((t_ebox *)z, bgl, 0., 0.);
    }
    
    UI_fun(ui_slider2d)::wx_mousedrag_ext(t_object* z, t_object *view, t_pt pt, long modifiers)
    {
        ui_slider2d *zx = (ui_slider2d*) z;
        
        t_rect rect;
        ebox_get_rect_for_view((t_ebox *)z, &rect);
        
        zx->_posx = (pt.x/rect.width)*2.-1.;
        zx->_posy = (pt.y/rect.height)*2.-1.;
        
        if ( (zx->_posx) > (zx->shift_x+zx->range_x) ) zx->_posx = zx->shift_x+zx->range_x;
        if ( (zx->_posx) < (zx->shift_x) ) zx->_posx = zx->shift_x;
        if ( (zx->_posy) > (zx->shift_y+zx->range_y) ) zx->_posy = zx->shift_y+zx->range_y;
        if ( (zx->_posy) < (zx->shift_y) ) zx->_posy = zx->shift_y;

        ceammc_gui::object<ceammc_gui::base_pd_object>::ws_redraw(z);
        
        atom_setfloat(&((ui_slider2d*)z)->out_list[0], zx->_posx);
        atom_setfloat(&((ui_slider2d*)z)->out_list[1], zx->_posy);
        
        outlet_list( ((ui_slider2d*)z)->out1, &s_list, 2, ((ui_slider2d*)z)->out_list );
        
    }
    
    UI_fun(ui_slider2d)::wx_mousedown_ext(t_object* z, t_object *view, t_pt pt, long modifiers)
    {
        ui_slider2d *zx = (ui_slider2d*) z;
        
        t_rect rect;
        ebox_get_rect_for_view((t_ebox *)z, &rect);
        
        zx->_posx = (pt.x/rect.width)*2.-1.;
        zx->_posy = (pt.y/rect.height)*2.-1.;
        
        if ( (zx->_posx) > (zx->shift_x+zx->range_x) ) zx->_posx = zx->shift_x+zx->range_x;
        if ( (zx->_posx) < (zx->shift_x) ) zx->_posx = zx->shift_x;
        if ( (zx->_posy) > (zx->shift_y+zx->range_y) ) zx->_posy = zx->shift_y+zx->range_y;
        if ( (zx->_posy) < (zx->shift_y) ) zx->_posy = zx->shift_y;
        
        
        ceammc_gui::object<ceammc_gui::base_pd_object>::ws_redraw(z);
        
        atom_setfloat(&((ui_slider2d*)z)->out_list[0], zx->_posx);
        atom_setfloat(&((ui_slider2d*)z)->out_list[1], zx->_posy);
        
        outlet_list( ((ui_slider2d*)z)->out1, &s_list, 2, ((ui_slider2d*)z)->out_list );
    }
    
    UI_fun(ui_slider2d)::wx_mouseup_ext(t_object* z, t_object *view, t_pt pt, long modifiers)
    {
        
        ui_slider2d *zx = (ui_slider2d*) z;
        
        t_rect rect;
        ebox_get_rect_for_view((t_ebox *)z, &rect);
        
        zx->_posx = (pt.x/rect.width)*2.-1.;
        zx->_posy = (pt.y/rect.height)*2.-1.;
        
        if ( (zx->_posx) > (zx->shift_x+zx->range_x) ) zx->_posx = zx->shift_x+zx->range_x;
        if ( (zx->_posx) < (zx->shift_x) ) zx->_posx = zx->shift_x;
        if ( (zx->_posy) > (zx->shift_y+zx->range_y) ) zx->_posy = zx->shift_y+zx->range_y;
        if ( (zx->_posy) < (zx->shift_y) ) zx->_posy = zx->shift_y;
        
        ceammc_gui::object<ceammc_gui::base_pd_object>::ws_redraw(z);
        
        atom_setfloat(&((ui_slider2d*)z)->out_list[0], zx->_posx);
        atom_setfloat(&((ui_slider2d*)z)->out_list[1], zx->_posy);
        
        outlet_list( zx->out1, &s_list, 2, zx->out_list );
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
        ui_slider2d *zx = (ui_slider2d*)x;
        zx->out1 = outlet_new(x, &s_list);
        
        zx->txt_max = etext_layout_create();
        zx->txt_min = etext_layout_create();
        zx->txt_font = efont_create(gensym("Helvetica"), gensym("light"), gensym("normal"), 8);
    }
    
}

extern "C" void setup_ui0x2eslider2d()
{
    ceammc_gui::object<ui_slider2d> class1;
    class1.setup("ui.slider2d");
}