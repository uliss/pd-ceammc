//
//  ui_scope~.cpp
//  pd_ext
//
//  Created by Alex on 19/12/16.
//
//

#include <stdio.h>

#include "lib/ceammc_gui.h"

struct ui_scope : ceammc_gui::base_pd_object
{
    //t_ebox x_gui;
    t_edspobj d_dsp;
    
    t_inlet *in1;
    t_sample input;
    t_sample buf[8192];
    
    int counter;
    
} _ui_scope;

namespace ceammc_gui {
    
    UI_fun(ui_scope)::wx_paint(t_object *z, t_object *view)
    {
        UI_Prop
        
        t_symbol *bgl = gensym("background_layer");
        float size;
        t_rect rect;
        ebox_get_rect_for_view((t_ebox *)z, &rect);
        
        t_elayer *g = ebox_start_layer((t_ebox *)z, bgl, rect.width, rect.height);
        
        ui_scope *zx = (ui_scope*)z;
        
        if(g)
        {
            egraphics_set_line_width(g, 1);
            
            egraphics_set_color_hex(g, gensym("#A0A0A0"));
            egraphics_line(g, 0, rect.height/2, rect.width, rect.height/2);
            egraphics_line(g, 0, rect.height*.25, rect.width, rect.height*.25);
            egraphics_line(g, 0, rect.height*.75, rect.width, rect.height*.75);
            egraphics_stroke(g);
            
            
            
            egraphics_set_line_width(g, 2);
            
            egraphics_set_color_hex(g, gensym("#00C0FF"));
            egraphics_move_to(g, 0, (((ui_scope*)z)->buf[0]*.5 + .5 )* rect.height);
            
            for (int i=0; i<128;i++)
            {
                float xx = float(i+1)/128.*rect.width;
                float val = ( ((ui_scope*)z)->buf[i] ==  ((ui_scope*)z)->buf[i] ) ? ((ui_scope*)z)->buf[i] : 0.;
                float yy = (-val *.5 + .5)*rect.height;
                
                egraphics_line_to(g, xx, yy);
            }
            
            egraphics_stroke(g);
        }
        
        
        ebox_end_layer((t_ebox*)z, bgl);
        ebox_paint_layer((t_ebox *)z, bgl, 0., 0.);
        
        
    }
    
    
    
    
    static void ui_scope_perform(ui_scope *x, t_object *dsp64, t_sample **ins, long numins, t_sample **outs, long numouts, long sampleframes, long flags, void *userparam)
    {
        t_sample  *in1 =   ins[0];
        int          n =           sampleframes;
        
        t_sample * out = &(x)->buf [x->counter * sampleframes];
        
        while (n--){*out++ = *in1++;} //
        
        x->counter++; if (x->counter==32) {x->counter=0; ceammc_gui::object<ui_scope>::ws_redraw(((t_object *)x));}
        
    }
    //
    //
    void ui_scope_dsp(ui_scope *x, t_object *dsp, short *count, double samplerate, long maxvectorsize, long flags)
    {
        object_method(dsp, gensym("dsp_add"), x, (method)ui_scope_perform, 0, NULL);
        
        
    }
    
    UI_fun(ui_scope)::new_ext(t_object *z, t_symbol *s, int argcl, t_atom *argv)
    {
        eobj_dspsetup(z, 1, 0);
        printf ("new ext \n");
    }
    
    
    
    UI_fun(ui_scope)::init_ext(t_eclass *z)
    {
        eclass_addmethod(z, (method)ui_scope_dsp, "dsp", A_CANT, 0);
        CLASS_ATTR_DEFAULT (z, "size", 0, "150. 100.");
        
    }
    
}

extern "C" void setup_ui0x2escope()
{
    ceammc_gui::object<ui_scope> class1;
    class1.setup_dsp("ui.scope");
}