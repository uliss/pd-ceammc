//
//  ui_sliders.cpp
//  pd_ext
//
//  Created by Alex Nadzharov on 19/12/16.
//
//


#include "ceammc_gui_cicm.h"

struct ui_sliders : cm_gui_base_pd_object
{
    t_ebox x_gui;
    t_outlet *out1;
    
    t_atom *val_list;
    int val_list_size;
    
};

UI_fun(ui_sliders)::wx_paint(t_object *z, t_object *view)
{
    UI_Prop
    
    t_symbol *bgl = gensym("background_layer");
    float size;
    t_rect rect;
    ebox_get_rect_for_view((t_ebox *)z, &rect);
    
    t_elayer *g = ebox_start_layer((t_ebox *)z, bgl, rect.width, rect.height);
    
    ui_sliders *zx = (ui_sliders*)z;
    
    if(g)
    {
        
        if (UI_Pf("count")<1) UI_Pset("count",1);
        if (UI_Pf("count")>128) UI_Pset("count",128);   //TEMP
        
        
        for (int i =0; i<UI_Pf("count"); i++)
        {
            float v = (zx->val_list)[i].a_w.w_float;
            
            float xx,yy,w,h;
            
            if (UI_Pf("_is_vertical")==1.)
            {
                xx = 0;
                yy = i*rect.height / UI_Pf("count");
                w = v*rect.width;
                h = rect.height / UI_Pf("count")-1;
            }
            else
            {
                yy = rect.height - v*rect.height;
                xx = i*rect.width / UI_Pf("count");
                h = rect.height;
                w = rect.width / UI_Pf("count")-1;
            }
            
            egraphics_rectangle(g, xx, yy, w, h);
            egraphics_set_color_hex(g, gensym("#C0C0C0"));
            egraphics_fill(g);
            
        }
    }
    
    ebox_end_layer((t_ebox*)z, bgl);
    ebox_paint_layer((t_ebox *)z, bgl, 0., 0.);

}

UI_fun(ui_sliders)::m_list(t_object *z, t_symbol *s, int argc, t_atom *argv)
{
    UI_Prop
    
    ui_sliders *zx = (ui_sliders*)z;
    
    if (zx->val_list) free (zx->val_list);
    
    zx->val_list = (t_atom*)malloc(sizeof(t_atom)*argc);
    zx->val_list_size = argc;
    
    UI_Pset("count", argc);
    
    for (int i=0;i<argc;i++) { zx->val_list[i] = argv[i]; }
    
    cm_gui_object<cm_gui_base_pd_object>::ws_redraw(z);
    
    outlet_list( ((ui_sliders*)z)->out1, &s_list, ((ui_sliders*)z)->val_list_size, ((ui_sliders*)z)->val_list );
    
    
}

UI_fun(ui_sliders)::wx_oksize(t_object *z, t_rect *newrect)
{
    UI_Prop
    
    UI_Pset("_is_vertical", newrect->width<newrect->height);
}



UI_fun(ui_sliders)::wx_mousedrag_ext(t_object* z, t_object *view, t_pt pt, long modifiers)
{
    UI_Prop
    
    t_rect rect;
    ebox_get_rect_for_view((t_ebox *)z, &rect);
    
    int numslider; float val;
    
    if (UI_Pf("_is_vertical")==1)
    {
        numslider = floor(pt.y/rect.height * ((ui_sliders*)z)->val_list_size);
        val = pt.x/rect.width;
    }
    else
    {
       numslider = floor(pt.x/rect.width * ((ui_sliders*)z)->val_list_size);
        val = 1. - pt.y/rect.height;
    }
    
    ((ui_sliders*)z)->val_list[numslider].a_w.w_float = val;
    
    cm_gui_object<cm_gui_base_pd_object>::ws_redraw(z);
    
    outlet_list( ((ui_sliders*)z)->out1, &s_list, ((ui_sliders*)z)->val_list_size, ((ui_sliders*)z)->val_list );
    
}

UI_fun(ui_sliders)::wx_mousedown_ext(t_object *z, t_object *view, t_pt pt, long modifiers)
{
    cm_gui_object<ui_sliders>::wx_mousedrag_ext(z, view, pt, modifiers);
}

UI_fun(ui_sliders)::ui_properties_init_ext(cm_gui_properties *def_p)
{
    
    def_p->ui_property_set("count", 8);
    def_p->ui_property_set("_is_vertical", 0);
    
    
}

UI_fun(ui_sliders)::new_ext(t_object *x, t_symbol *s, int argcl, t_atom *argv)
{
    ((ui_sliders*)x)->out1 = outlet_new(x, &s_list);
    
    ((ui_sliders*)x)->val_list_size = 8;
    ((ui_sliders*)x)->val_list = (t_atom*)malloc(sizeof(t_atom)*8);
    for (int i=0;i<8;i++)
    {
    ((ui_sliders*)x)->val_list[i].a_type = A_FLOAT;
    ((ui_sliders*)x)->val_list[i].a_w.w_float = 0.;
    }
}

extern "C" void setup_ui0x2esliders()
{
    cm_gui_object<ui_sliders> class1;
    class1.setup("ui.sliders");
}