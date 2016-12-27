//
//  ui_knob.cpp
//  pd_ext
//
//  Created by Alex Nadzharov on 19/12/16.
//
//


#include "lib/ceammc_gui.h"

#include "ceammc_atomlist.h"
#include "ceammc_format.h"

struct ui_knob : public ceammc_gui::base_pd_object
{
    t_ebox x_gui;
    
    float mouse_x;
    float mouse_y;
    int mouse_dn;
    bool _selected;
    
    t_outlet *out1;
    
    t_atom *val_list;
    int val_list_size;
    
    float _value;
    
};

namespace ceammc_gui {
    
    void e_polyarc(t_elayer *g, float xc, float yc, float radius, float angle1, float angle2, float lod)
    {
        float d_angle = angle2-angle1;
        for (int i=0; i<lod; i++)
        {
            float x1,x2,y1,y2;
            
            x1 = xc + cos(angle1 + d_angle* float(i/lod)) *radius;
            x2 = xc + cos(angle1 + d_angle* float((i+1)/lod)) *radius;
            
            y1 = yc - sin(angle1 + d_angle* float(i/lod)) *radius;
            y2 = yc - sin(angle1 + d_angle* float((i+1)/lod)) *radius;
            
            egraphics_line(g, x1, y1, x2, y2);
            
        }
    }
    
    void e_marc(t_elayer *g, float xc, float yc, float radius, float angle1, float angle2, float anglem, float lod)
    {
        float x1,x2,y1,y2;
        float d_angle = angle2-angle1;
        
        x1 = xc + cos(angle1)  *radius;// + d_angle* float(i/lod)) *radius;
        x2 = xc + cos(angle1 + d_angle ) *radius;
        
        y1 = yc - sin(angle1) *radius;
        y2 = yc - sin(angle1 + d_angle) *radius;
        
        float angle_l = 1. - 0.99*((angle1+d_angle)/d_angle)*anglem;
        
        egraphics_move_to(g, x1, y1);
        egraphics_arc_to(g, xc, yc, angle_l);
    }
    
    UI_fun(ui_knob)::wx_paint(t_object *z, t_object *view)
    {
//        UI_Prop
        
        t_symbol *bgl = gensym("background_layer");
        //float size;
        t_rect rect;
        ebox_get_rect_for_view((t_ebox *)z, &rect);
        
        t_elayer *g = ebox_start_layer((t_ebox *)z, bgl, rect.width, rect.height);
        
        ui_knob *zx = (ui_knob*)z;
        
        if(g)
        {
            
            float rmin = .5*rect.width*.7 -2;
            float rmid = .5*rect.width -2;
            float rmax = .5*rect.width -2;
            float amin = 5/8.*M_PI*2.;
            float amax = -1/8.*M_PI*2.;
            float aval = M_PI*2.*6/8.* zx->_value;//UI_Pf("_value");
            
            float cx = rect.width*.5;
            float cy = rect.height*.5;
            
            egraphics_set_line_width(g, 1);
            egraphics_set_color_hex(g, gensym("#C0C0C0"));
            //e_marc(g, cx, cy, rmax, amin, amin+ 2*M_PI, EPD_2PI*6/8., 20);
            e_polyarc(g, cx, cy, rmin, amax + aval, amax, 30);
            e_polyarc(g, cx, cy, rmax, amin, amax, 30);
            //egraphics_circle(g, cx, cy, rmax);
            
            egraphics_line(g, cx + cos(amin)*0, cy - sin(amin)*0, cx + cos(amin)*rmax, cy - sin(amin)*rmax );
            egraphics_line(g, cx + cos(amax)*0, cy - sin(amax)*0, cx + cos(amax)*rmax, cy - sin(amax)*rmax );
            
            egraphics_stroke(g);
            
            egraphics_set_line_width(g, 2);
            egraphics_set_color_hex(g, gensym("#00C0FF"));
            //e_marc(g, cx, cy, rmid, amax + aval, amin, EPD_2PI*6/8., 20);
            e_polyarc(g, cx, cy, rmid, amax + aval, amin,  30 - floor(29 * zx->_value));    //UI_Pf("_value")
            //egraphics_arc(g, cx, cy, rmid, amax+aval, amin);
            egraphics_line(g, cx , cy , cx + cos(amax + aval)*rmid, cy - sin(amax + aval)*rmid );
            egraphics_line(g, cx , cy , cx + cos(amin)*rmid, cy - sin(amin)*rmid );
            egraphics_stroke(g);
            
            
        }
        
        ebox_end_layer((t_ebox*)z, bgl);
        ebox_paint_layer((t_ebox *)z, bgl, 0., 0.);
        
    }
    
    
    UI_fun(ui_knob)::wx_oksize(t_object *z, t_rect *newrect)
    {
        newrect->width = floorf(newrect->width/5.)*5;
        newrect->height = newrect->width;
        
    }
    
    
    
    UI_fun(ui_knob)::wx_mousedrag_ext(t_object* z, t_object *view, t_pt pt, long modifiers)
    {
//        UI_Prop
        
        t_rect rect;
        ebox_get_rect_for_view((t_ebox *)z, &rect);
        
        float val;
        val = 1-pt.y/rect.height;
        
        if (val>1) val=1;
        if (val<0) val=0;
        
//        UI_Pset("_value",val);
        
        ui_knob *zx = (ui_knob*)z;
        zx->_value = val;
        
        ceammc_gui::object<ceammc_gui::base_pd_object>::ws_redraw(z);
        
        outlet_float(((ui_knob*)z)->out1, (1-val)*127);
        
        
    }
    
    UI_fun(ui_knob)::wx_mousedown_ext(t_object *z, t_object *view, t_pt pt, long modifiers)
    {
        ceammc_gui::object<ui_knob>::wx_mousedrag_ext(z, view, pt, modifiers);
    }
    
    UI_fun(ui_knob)::m_float(t_object *z,  t_float f)
    {
//        UI_Prop
        
        
//        UI_Pset("_value",1.-f/127.);
//        float val =UI_Pf("_value");
        
        ui_knob *zx = (ui_knob*)z;
        zx->_value = 1.-f/127.;
        
        ceammc_gui::object<ceammc_gui::base_pd_object>::ws_redraw(z);
        
        outlet_float(((ui_knob*)z)->out1, zx->_value*127.);
        
    }
    
    UI_fun(ui_knob)::m_bang(t_object *z, t_symbol *s, int argc, t_atom *argv)
    {
//        UI_Prop
        
        ui_knob *zx = (ui_knob*)z;
        
//        float val =UI_Pf("_value");
        outlet_float(((ui_knob*)z)->out1, zx->_value*127.);
    }
    
//    UI_fun(ui_knob)::ui_properties_init_ext(ceammc_gui::properties *def_p)
//    {
//        
//        
////        def_p->ui_property_set("width", 45.);
////        def_p->ui_property_set("height", 45.);
////        
////        def_p->ui_property_set("_value", 1.);
//        
//        
//    }
    UI_fun(ui_knob)::init_ext(t_eclass *z)
    {
        CLASS_ATTR_DEFAULT (z, "size", 0, "60. 60.");
        
    }
    
    UI_fun(ui_knob)::new_ext(t_object *x, t_symbol *s, int argcl, t_atom *argv)
    {
        ((ui_knob*)x)->out1 = outlet_new(x, &s_float);
        
    }
    
}

extern "C" void setup_ui0x2eknob()
{
    ceammc_gui::object<ui_knob> class1;
    class1.setup("ui.knob");
}