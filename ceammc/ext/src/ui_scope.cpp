//
//  ui_scope~.cpp
//  pd_ext
//
//  Created by Alex on 19/12/16.
//
//

#include <stdio.h>

#include "ceammc_gui_cicm.h"

struct ui_scope : cm_gui_base_pd_object
{
    //t_ebox x_gui;
    t_edspobj d_dsp;
    
    t_inlet *in1;
    t_sample input;
    t_sample buf[8192];
    
    int counter;
    
} _ui_scope;

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
            float yy = (val *.5 + .5)*rect.height;
            
            egraphics_line_to(g, xx, yy);
        }
        
        //printf ("val %f\n", ((ui_scope*)z)->buf[0]);
        
        egraphics_stroke(g);
    }


    ebox_end_layer((t_ebox*)z, bgl);
    ebox_paint_layer((t_ebox *)z, bgl, 0., 0.);
    
    //printf(" * ");
    
}



static void ui_scope_perform(ui_scope *x, t_object *dsp64, t_sample **ins, long numins, t_sample **outs, long numouts, long sampleframes, long flags, void *userparam)
{
    //ui_scope *x = (ui_scope *)(w[1]);
    t_sample  *in1 =   ins[0]; //(t_sample *)(w[2]);
//    t_sample  *in2 =    (t_sample *)(w[3]);
    //t_sample  *out =    (t_sample *)(w[4]);
    int          n =           sampleframes;
    //t_sample f_pan = (x->f_pan<0)?0.0:(x->f_pan>1)?1.0:x->f_pan;
    
    //t_sample * out = (x)->buf;


    t_sample * out = &(x)->buf [x->counter * sampleframes];
    
    while (n--){*out++ = *in1++;} //

    x->counter++; if (x->counter==32) {x->counter=0; cm_gui_object<ui_scope>::ws_redraw(((t_object *)x));}
 
    //printf( " buf %f ", x->buf[0]);
}
//
//
void ui_scope_dsp(ui_scope *x, t_object *dsp, short *count, double samplerate, long maxvectorsize, long flags)
{
    //dsp_add(ui_scope_perform, 5, x,
    //        sp[0]->s_vec, sp[1]->s_vec, sp[2]->s_vec, sp[0]->s_n);
    
    
    object_method(dsp, gensym("dsp_add"), x, (method)ui_scope_perform, 0, NULL);
    
   // dsp_add(ui_scope_perform, 4, sp[0]->s_vec, sp[0]->s_n);
}

UI_fun(ui_scope)::new_ext(t_object *z, t_symbol *s, int argcl, t_atom *argv)
{
    //((ui_scope*)z)->in1 = inlet_new(&((ui_scope*)z)->x_gui, &((ui_scope*)z)->x_gui.ob_pd, &s_signal, &s_signal);
    eobj_dspsetup(z, 1, 0);
    printf ("new ext \n");
}

UI_fun(ui_scope)::free_ext(t_object *x)
{
    //eobj_dspfree(x);
}

UI_fun(ui_scope)::init_ext(t_eclass *z)
{
    //eclass
    eclass_addmethod(z, (method)ui_scope_dsp, "dsp", A_CANT, 0);
    
    //CLASS_MAINSIGNALIN(z, ui_scope, f);
}

extern "C" void setup_ui0x2escope()
{
    cm_gui_object<ui_scope> class1;
    class1.setup_dsp("ui.scope");
}