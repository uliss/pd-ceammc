//
//  ui_bpfunc.cpp
//  pd_ext
//
//  Created by Alex on 20/12/16.
//
//

#include <stdio.h>

#include "ceammc_gui_cicm.h"

#include <map>

typedef struct t_bpt
{
    int idx;
    float x;
    float y;
} t_bpt;

typedef std::vector<t_bpt> bpf_points;


struct ui_bpfunc : cm_gui_base_pd_object
{
    t_ebox x_gui;
    
    bpf_points *points;
    
};

inline void bpf_point_add(t_object *z,int idx, float x, float y)
{
    t_bpt pt1;
    pt1.x = x;
    pt1.y = y;
    
    bpf_points *ps = ((ui_bpfunc*)z)->points;
    
    ps->push_back(pt1);
    
    
}

inline void bpf_point_del(t_object *z,int idx)
{
    //
}

inline int bpf_size(t_object *z)
{
    return ((ui_bpfunc*)z)->points->size();
}

inline void bpf_points_new(t_object *z)
{
    //check
    if (((ui_bpfunc*)z)->points) free (((ui_bpfunc*)z)->points);
    
    ((ui_bpfunc*)z)->points = new bpf_points;
}


#pragma mark -

UI_fun(ui_bpfunc)::wx_paint(t_object *z, t_object *view)
{
    //UI_Prop
    
    t_symbol *bgl = gensym("background_layer");
    float size;
    t_rect rect;
    ebox_get_rect_for_view((t_ebox *)z, &rect);
    
    t_elayer *g = ebox_start_layer((t_ebox *)z, bgl, rect.width, rect.height);
    
    ui_bpfunc *zx = (ui_bpfunc*)z;
    
    if(g)
    {
        egraphics_set_color_hex(g, gensym("#E0E0E0"));
        
        egraphics_rectangle(g, 0, 0, rect.width, rect.height);
        egraphics_fill(g);
        
        egraphics_set_color_hex(g, gensym("#505050"));
        
        
        bpf_points::iterator it = zx->points->begin();
        
        //        egraphics_stroke(g);
        
        egraphics_circle(g, it->x * rect.width, (1-it->y) * rect.height, 6);
        egraphics_stroke(g);
        
        egraphics_move_to(g, it->x * rect.width, (1-it->y) * rect.height);
        ++it;
        
                for ( ; it != zx->points->end(); ++it)
                {
                    egraphics_line_to(g, it->x * rect.width, (1-it->y) * rect.height);
                    egraphics_stroke(g);
                    egraphics_circle(g, it->x * rect.width, (1-it->y) * rect.height, 6);
                    egraphics_stroke(g);
        
                }
        
    }
    
    ebox_end_layer((t_ebox*)z, bgl);
    ebox_paint_layer((t_ebox *)z, bgl, 0., 0.);
    
}

UI_fun(ui_bpfunc)::new_ext(t_object *z, t_symbol *s, int argcl, t_atom *argv)
{
    ((t_ebox*)z)->b_boxparameters.d_boxfillcolor.red = 0.75;
    ((t_ebox*)z)->b_boxparameters.d_boxfillcolor.green = 0.75;
    ((t_ebox*)z)->b_boxparameters.d_boxfillcolor.blue   = 0.75;
    ((t_ebox*)z)->b_boxparameters.d_boxfillcolor.alpha = 1.;
    
    bpf_points_new(z);
    
    bpf_point_add(z, 0, 0, 0);
    bpf_point_add(z, 1, 1, 1);
}

UI_fun(ui_bpfunc)::init_ext(t_eclass *z)
{
    CLASS_ATTR_DEFAULT (z, "size", 0, "200. 150.");
    
}

extern "C" void setup_ui0x2ebpfunc()
{
    cm_gui_object<ui_bpfunc> class1;
    class1.setup("ui.bpfunc");
}