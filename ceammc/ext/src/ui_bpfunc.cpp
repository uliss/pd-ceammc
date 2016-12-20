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
    
    float dist;      // distance from mouse. recalculated on hover
    float ldist;    //  distance to line
    bool selected;
    
} t_bpt;

typedef std::vector<t_bpt> bpf_points;


struct ui_bpfunc : cm_gui_base_pd_object
{
    t_ebox x_gui;
    
    bpf_points *points;
    
    float _px;
    float _py;
    
    int addidx;
    float addpos;
    float addpos_y;
    
};

inline void bpf_point_add(t_object *z,int idx, float x, float y)
{
    t_bpt pt1;
    pt1.idx = idx;
    pt1.x = x;
    pt1.y = y;
    
    bpf_points *ps = ((ui_bpfunc*)z)->points;
    
    ps->push_back(pt1);
    
    
    struct pred {
        bool operator()(t_bpt const & a, t_bpt const & b) const {
            return a.idx < b.idx;
        }
    };
    std::sort(ps->begin(), ps->end(), pred());
    
    
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
        
        egraphics_set_color_hex(g, gensym("#C0C0C0"));
        
        egraphics_line(g, 0, .25*rect.height, rect.width, .25*rect.height);
        egraphics_line(g, 0, .5*rect.height, rect.width, .5*rect.height);
        egraphics_line(g, 0, .75*rect.height, rect.width, .75*rect.height);
        
        egraphics_stroke(g);
        
        egraphics_set_color_hex(g, gensym("#505050"));
        
        
        bpf_points::iterator it = zx->points->begin();
        
        //        egraphics_stroke(g);
        
        //
        if (zx->addpos>0)
        {
            egraphics_set_color_hex(g, gensym("#00C0FF"));
            
            //egraphics_rectangle(g, zx->addpos-7, zx->addpos_y-7, 15, 15);
            egraphics_line(g,zx->addpos , 0, zx->addpos, rect.height);
            egraphics_line(g,0 , zx->addpos_y, rect.width, zx->addpos_y);
            
            egraphics_stroke(g);
            
        }
        
        //for (; it != zx->points->end(); ++it)
        {
            
            //egraphics_circle(g, it->x * rect.width, (1-it->y) * rect.height, 6);
            egraphics_rectangle(g, it->x * rect.width -3, (1-it->y) * rect.height -3, 6, 6);
            egraphics_fill(g);
            
            egraphics_set_color_hex(g, gensym("#505050"));
            
            float px = it->x * rect.width;
            float py = (1-it->y) * rect.height;
            
            //egraphics_move_to(g, it->x * rect.width, (1-it->y) * rect.height);
            
            if (it->dist < .1)
            {
                egraphics_rectangle(g, it->x * rect.width -6, (1-it->y) * rect.height -6, 11, 11);
                egraphics_stroke(g);
                //egraphics_circle(g, it->x * rect.width, (1-it->y) * rect.height, 10. * it->dist);
            }
            
            
            
            float lw = (it->ldist<.4);// && (it->ldist>.2);
            if (lw<0) lw = 0;
            if (lw>1) lw=1;
            
            egraphics_set_line_width(g, 1+2*lw);
            
            ++it;
            
            for ( ; it != zx->points->end(); ++it)
            {
                egraphics_set_line_width(g, 1+2*lw);
                egraphics_line(g, px, py, it->x * rect.width, (1-it->y) * rect.height);
                egraphics_stroke(g);
                
                px = it->x * rect.width;
                py = (1-it->y) * rect.height;
                
                lw = (it->ldist<.4);// && (it->ldist>.2);
                if (lw<0) lw = 0;
                if (lw>1) lw=1;
                
                
                
                
                egraphics_set_line_width(g, 1);
                egraphics_rectangle(g, it->x * rect.width -3, (1-it->y) * rect.height -3, 6, 6);
                egraphics_fill(g);
                
                if (it->dist < .1)
                {
                    egraphics_rectangle(g, it->x * rect.width -6, (1-it->y) * rect.height -6, 11, 11);
                    egraphics_stroke(g);
                    
                }
                
                
            }
        }
        
    }
    
    ebox_end_layer((t_ebox*)z, bgl);
    ebox_paint_layer((t_ebox *)z, bgl, 0., 0.);
    
}

UI_fun(ui_bpfunc)::wx_mousemove_ext(t_object *z, t_object *view, t_pt pt, long modifiers)
{
    //calc distances
    
    ui_bpfunc *zx = (ui_bpfunc*)z;
    
    t_rect rect;
    ebox_get_rect_for_view((t_ebox *)z, &rect);
    
    float nx = pt.x / rect.width;
    float ny = pt.y / rect.height;
    
    float nlen = sqrtf(nx*nx+ny*ny);
    float nnx = (nlen!=0) ? nx/ nlen : 0;
    float nny = (nlen!=0) ? ny/ nlen : 0;
    
    
    //bpf_points::iterator it;
    //bpf_points::iterator itn;
    
    for (int i = 0; i<zx->points->size();i++)   //it = zx->points->begin(); it != zx->points->end(); ++it
    {
        t_bpt * it = &(zx->points)->at(i);
        t_bpt * itn = ((i-1) == zx->points->size()) ? &(zx->points)->at(i+1) : &(zx->points)->at(i);
        
        float dx = nx - it->x;
        float dy =  ( ny - (1 -it->y) );
        
        it->dist = sqrtf(dx*dx + dy*dy);
        
        it->selected = (it->dist<.1);
        
        //////
        
        //if (itn != zx->points->end())
        
        float dx2 = itn->x - it->x;
        float dy2 = (1-itn->y) - (1-it->y);
        
        float len2 = sqrtf(dx2*dx2+dy2*dy2);
        float ndx2 = (len2!=0) ? dx2/ len2 : 0;
        float ndy2 = (len2!=0) ? dy2/ len2 : 0;
        
        float dot1 = nnx*ndx2 + nny*ndy2;
        //if (dot1<0) dot1 = 0;
        
        it->ldist = abs(dot1);
        
        
        //float d2 = .5 * sqrtf(dx2*dx2 + dy2*dy2);
        
        //it->ldist = sqrtf(it->dist * it->dist - d2 * d2) + sqrtf(itn->dist * itn->dist - d2 * d2);
        
        if ( (it->x < (pt.x / rect.width)) && ((pt.x / rect.width )< itn->x) )
        {
            zx->addidx = i;
        }
        else
        {
            zx->addidx = zx->points->size();
            
        }
        
        if (modifiers==EMOD_SHIFT)
        {
            zx->addpos = pt.x;
            zx->addpos_y = pt.y;
        }
        else
        {
            zx->addpos = -1;
        }
        
        //printf ("(%f,%f) %f : ", dx,dy, it->dist);
        //printf("%f : ", it->ldist);
    }
    //printf("\n");
    cm_gui_object<cm_gui_base_pd_object>::ws_redraw(z);
}


UI_fun(ui_bpfunc)::wx_mouseleave_ext(t_object *z, t_object *view, t_pt pt, long modifiers)
{
    ui_bpfunc *zx = (ui_bpfunc*)z;
    
    zx->addpos = -1;
}

UI_fun(ui_bpfunc)::wx_mousedown_ext(t_object *z, t_object *view, t_pt pt, long modifiers)
{
    ui_bpfunc *zx = (ui_bpfunc*)z;
    
    zx->_px = pt.x;
    zx->_py = pt.y;
    
    t_rect rect;
    ebox_get_rect_for_view((t_ebox *)z, &rect);
    
    if (modifiers==EMOD_SHIFT)
    {
        bpf_point_add(z, zx->addidx, pt.x/rect.width, 1-(pt.y/rect.height));
        cm_gui_object<cm_gui_base_pd_object>::ws_redraw(z);
        
    }
}

UI_fun(ui_bpfunc)::wx_mousedrag_ext(t_object *z, t_object *view, t_pt pt, long modifiers)
{
    ui_bpfunc *zx = (ui_bpfunc*)z;
    
    bpf_points::iterator it = zx->points->begin();
    
    t_rect rect;
    ebox_get_rect_for_view((t_ebox *)z, &rect);
    
    for ( ; it != zx->points->end(); ++it)
    {
        if (it->selected)
        {
            it->x += (pt.x - zx->_px) / rect.width;
            it->y += ((1-pt.y) - (1-zx->_py)) / rect.height;
            
            if (it->x>1) it->x=1;
            if (it->x<0) it->x=0;
            
            if (it->y>1) it->y=1;
            if (it->y<0) it->y=0;
            
            
        }
    }
    
    
    zx->_px = pt.x;
    zx->_py = pt.y;
    
    cm_gui_object<cm_gui_base_pd_object>::ws_redraw(z);
    
}

#pragma mark -

UI_fun(ui_bpfunc)::new_ext(t_object *z, t_symbol *s, int argcl, t_atom *argv)
{
    ((t_ebox*)z)->b_boxparameters.d_boxfillcolor.red = 0.75;
    ((t_ebox*)z)->b_boxparameters.d_boxfillcolor.green = 0.75;
    ((t_ebox*)z)->b_boxparameters.d_boxfillcolor.blue   = 0.75;
    ((t_ebox*)z)->b_boxparameters.d_boxfillcolor.alpha = 1.;
    
    bpf_points_new(z);
    
    bpf_point_add(z, 0, 0, 0);
    bpf_point_add(z, 1, .5, .75);
    bpf_point_add(z, 2, 1, 1);
    
    ui_bpfunc *zx = (ui_bpfunc*)z;
    
    zx->_px = 0;
    zx->_py = 0;
    
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