//
//  ui_bpfunc.cpp
//  pd_ext
//
//  Created by Alex Nadzharov on 20/12/16.
//
//

#include <stdio.h>

#include "lib/ceammc_gui.h"

#include "ceammc_atomlist.h"
//#include "ceammc_format.h"

#include <map>
#include <algorithm>

#include <vector>

//new
#include "lib/ceammc_bpf.h"

//using namespace ceammc;

using namespace std;
using namespace ceammc;
using namespace ceammc_gui;


namespace ceammc_gui {
    
//    typedef struct t_bpt
//    {
//        int idx;
//        float x;
//        float y;
//        
//        float dist;         // distance from mouse. recalculated on hover
//        float ldist;        //  distance to line
//        bool selected;
//        
//        bool end_segment;
//        float range_x;
//        
//        bool lock_x;
//        bool lock_y;
//        
//        bool join_next;     //
//        
//    } t_bpt;
    
    typedef std::vector<t_bpt> bpf_points;
    
    struct ui_bpfunc : public BaseGuiObject
    {
        t_ebox x_gui;
        
        float mouse_x;
        float mouse_y;
        int mouse_dn;
        bool _selected;
        
        t_outlet *out1;
        t_atom *out_list;
        int out_list_count;
        
        //AtomList *output;
        
        bpf_points *points;
        
        float _px;
        float _py;
        
        int addidx;
        int delidx;
        
        float addpos;
        float addpos_y;
        bool del_mod;
        
        float range_x;
        float range_y;
        float shift_x;
        float shift_y;
        
        int select_idx;
        
        bool auto_send;
        bool drag_limit;    //move
        
        int seg_count;      //move
        
        t_etext *txt_min;
        t_etext *txt_max;
        t_etext *txt_val;   //stub
        
        t_efont *txt_font;
        
        vector<int> seg_idx;//move
        
        t_rgba b_color_background;
        t_rgba b_color_border;
        
        
    };
    
    const bool bpf_sort_pred(t_bpt const & a, t_bpt const & b)  {
        return a.x < b.x;
    }
    
    inline void bpf_point_sort(t_object *z)
    {
        
        bpf_points *ps = ((ui_bpfunc*)z)->points;
        
        std::sort(ps->begin(), ps->end(), bpf_sort_pred);
        
        
    }
    
    inline void bpf_point_add(t_object *z,int idx, float x, float y)
    {
        t_bpt pt1;
        pt1.idx = idx;
        pt1.x = x;
        pt1.y = y;
        
        pt1.lock_x = false;
        pt1.lock_y = false;
        
        pt1.end_segment = false;
        
        bpf_points *ps = ((ui_bpfunc*)z)->points;
        
        ps->push_back(pt1);
        
        bpf_point_sort(z);
        
        
    }
    
    inline void bpf_point_del(t_object *z,int idx)
    {
        //
        //printf("del pt: %i", idx);
        
        bpf_points *ps = ((ui_bpfunc*)z)->points;
        
        if (ps->size()>2)
        {
            ps->erase(ps->begin()+idx);
            bpf_point_sort(z);
        }
    }
    
    inline void bpf_point_split(t_object *z,int idx)
    {
        //
        //printf("del pt: %i", idx);
        
        bpf_points *ps = ((ui_bpfunc*)z)->points;
        
        ps->at(idx).end_segment = !ps->at(idx).end_segment;
        
        ui_bpfunc *zx = (ui_bpfunc*)z;
        
        if (ps->at(idx).end_segment)
        {
            zx->seg_count++;
            
            zx->seg_idx.push_back(idx);
            std::sort(zx->seg_idx.begin(), zx->seg_idx.end());
        }
        else
        {
            zx->seg_count--;
            
            vector<int>::iterator pos = find(zx->seg_idx.begin(),zx->seg_idx.end(), idx);
            if (pos != zx->seg_idx.end())
                zx->seg_idx.erase(pos);
            
        };
        
    }
    
    inline int bpf_size(t_object *z)
    {
        return (int)((ui_bpfunc*)z)->points->size();
    }
    
    inline void bpf_points_new(t_object *z)
    {
        //check
        if (((ui_bpfunc*)z)->points) delete (((ui_bpfunc*)z)->points);
        
        ((ui_bpfunc*)z)->points = new bpf_points;
    }
    
    inline void bpf_points_free(t_object *z)
    {
        //check
        if (((ui_bpfunc*)z)->points) delete (((ui_bpfunc*)z)->points);
        
    }
    
    
#pragma mark -
    
    UI_fun(ui_bpfunc)::wx_paint(t_object *z, t_object *view)
    {
        
        
        t_symbol *bgl = gensym("background_layer");
        //float size;
        t_rect rect;
        ebox_get_rect_for_view((t_ebox *)z, &rect);
        
        t_elayer *g = ebox_start_layer((t_ebox *)z, bgl, rect.width, rect.height);
        
        ui_bpfunc *zx = (ui_bpfunc*)z;
        
        if(g)
        {
            //egraphics_set_color_hex(g, gensym("#E0E0E0"));
            egraphics_set_color_rgba(g, &zx->b_color_background);
            
            egraphics_rectangle(g, 0, 0, rect.width, rect.height);
            egraphics_fill(g);
            
            egraphics_set_color_hex(g, gensym("#C0C0C0"));
            
            egraphics_line(g, 0, .25*rect.height, rect.width, .25*rect.height);
            egraphics_line(g, 0, .5*rect.height, rect.width, .5*rect.height);
            egraphics_line(g, 0, .75*rect.height, rect.width, .75*rect.height);
            
            egraphics_stroke(g);
            
            egraphics_set_color_hex(g, gensym("#505050"));
            
            
            bpf_points::iterator it = zx->points->begin();
            
            char c_val[16];
            sprintf(c_val, "—");
            
            
            if (zx->addpos>0)
            {
                egraphics_set_color_hex(g, gensym("#00C0FF"));
                
                egraphics_line(g,zx->addpos , 0, zx->addpos, rect.height);
                egraphics_line(g,0 , zx->addpos_y, rect.width, zx->addpos_y);
                
                sprintf(c_val, "%.2f : %.2f", zx->addpos / rect.width * zx->range_x+zx->shift_x, (1- zx->addpos_y / rect.height) * zx->range_y+zx->shift_y);
                
                egraphics_stroke(g);
                
            }
            
            if (zx->select_idx == 0)
            {
                egraphics_set_color_hex(g, gensym("#00C0FF"));
                
                egraphics_rectangle(g, it->x * rect.width -8, (1-it->y) * rect.height -8, 15, 15);
                egraphics_stroke(g);
                
            }
            
            int i=1;
            
            
            //for (; it != zx->points->end(); ++it)
            {
                
                //egraphics_circle(g, it->x * rect.width, (1-it->y) * rect.height, 6);
                
                
                int pw1 = 6-4*it->lock_x;
                int ph1 = 6-4*it->lock_y;
                //egraphics_set_color_rgba(g, &zx->b_color_background);
                
                egraphics_rectangle(g, it->x * rect.width -.5*pw1, (1-it->y) * rect.height -.5*ph1, pw1, ph1);
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
                    
                    sprintf(c_val, "%.2f : %.2f", zx->points->at(i).x * zx->range_x+zx->shift_x, zx->points->at(i).x * zx->range_y+zx->shift_y);
                }
                
                
                //yet disabled
                //            float lw = (it->ldist<.4);// && (it->ldist>.2);
                //            if (lw<0) lw = 0;
                //            if (lw>1) lw=1;
                
                egraphics_set_line_width(g, 1);    //yet disabled
                
                ++it;
                
                for ( ; it != zx->points->end(); ++it)
                {
                    egraphics_set_color_hex(g, gensym("#505050"));
                    
                    egraphics_set_line_width(g, 1);
                    egraphics_line(g, px, py, it->x * rect.width, (1-it->y) * rect.height);
                    egraphics_stroke(g);
                    
                    px = it->x * rect.width;
                    py = (1-it->y) * rect.height;
                    
                    //yet disabled
                    //                lw = (it->ldist<.4);
                    //                if (lw<0) lw = 0;
                    //                if (lw>1) lw=1;
                    
                    egraphics_set_line_width(g, 1);
                    int pw1 = 6-4*it->lock_x;
                    int ph1 = 6-4*it->lock_y;
                    egraphics_rectangle(g, it->x * rect.width -.5*pw1, (1-it->y) * rect.height -.5*ph1, pw1, ph1);
                    egraphics_fill(g);
                    
                    if (it->dist < .1)
                    {
                        egraphics_set_color_hex(g, gensym((zx->del_mod)?"#FF0000":"#505050"));
                        
                        egraphics_rectangle(g, it->x * rect.width -6, (1-it->y) * rect.height -6, 11, 11);
                        egraphics_stroke(g);
                        
                        sprintf(c_val, "%.2f : %.2f", zx->points->at(i).x * zx->range_x+zx->shift_x, zx->points->at(i).x * zx->range_y+zx->shift_y);
                        
                    }
                    
                    if (zx->select_idx == i)
                    {
                        egraphics_set_color_hex(g, gensym("#00C0FF"));
                        
                        egraphics_rectangle(g, it->x * rect.width -8, (1-it->y) * rect.height -8, 15, 15);
                        egraphics_stroke(g);
                        
                    }
                    
                    if (it->end_segment)
                    {
                        for (int sy=0;sy<rect.height; sy+=5)
                        {
                            egraphics_line(g, it->x * rect.width, sy, it->x * rect.width, sy+2.5);
                        }
                        egraphics_stroke(g);
                        
                    }
                    
                    i++;
                    
                }
            }
            
            char c_min[10];
            sprintf(c_min, "%.2f", zx->shift_y);
            
            char c_max[10];
            sprintf(c_max, "%.2f", zx->range_y+zx->shift_y);
            
            
            etext_layout_set(zx->txt_min, c_min, zx->txt_font, 3, rect.height-12, rect.width*2, rect.height/2, ETEXT_UP_LEFT, ETEXT_JLEFT, ETEXT_WRAP);
            etext_layout_draw(zx->txt_min, g);
            
            etext_layout_set(zx->txt_max, c_max, zx->txt_font, 3, 12, rect.width*2, rect.height/2, ETEXT_DOWN_LEFT, ETEXT_JLEFT, ETEXT_WRAP);
            etext_layout_draw(zx->txt_max, g);
            
            etext_layout_set(zx->txt_val, c_val, zx->txt_font, rect.width*.45, 12, rect.width*.1, rect.height/2, ETEXT_DOWN_LEFT, ETEXT_JLEFT, ETEXT_NOWRAP);
            etext_layout_draw(zx->txt_val, g);
            
        }
        
        ebox_end_layer((t_ebox*)z, bgl);
        ebox_paint_layer((t_ebox *)z, bgl, 0., 0.);
        
    }
    
#pragma mark -
#pragma mark mouse
    
    UI_fun(ui_bpfunc)::wx_mousemove_ext(t_object *z, t_object *view, t_pt pt, long modifiers)
    {
        //calc distances
        
        ui_bpfunc *zx = (ui_bpfunc*)z;
        
        t_rect rect;
        ebox_get_rect_for_view((t_ebox *)z, &rect);
        
        float nx = pt.x / rect.width;
        float ny = pt.y / rect.height;
        
//        float nlen = sqrtf(nx*nx+ny*ny);
//        float nnx = (nlen!=0) ? nx/ nlen : 0;
//        float nny = (nlen!=0) ? ny/ nlen : 0;
        
        
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
            
            it->ldist = 1;//abs(dot1);
            
            if ( (it->x < (pt.x / rect.width)) && ((pt.x / rect.width )< itn->x) )
            {
                zx->addidx = i;
            }
            else
            {
                zx->addidx = (int)zx->points->size();
                
            }
            
            if (modifiers==EMOD_SHIFT)
            {
                zx->addpos = pt.x;
                zx->addpos_y = pt.y;
            }
            else
            {
                zx->addpos = -1;
                zx->del_mod = false;
            }
            
            if (it->selected) zx->delidx = i;
            
            if (modifiers == EMOD_ALT)
            {
                zx->del_mod = true;
            }
            
            //printf ("(%f,%f) %f : ", dx,dy, it->dist);
            //printf("%f : ", it->ldist);
        }
        //printf("\n");
        
        GuiFactory<BaseGuiObject>::ws_redraw(z);
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
            GuiFactory<BaseGuiObject>::ws_redraw(z);
            
        }
        
        if (modifiers==EMOD_ALT)
        {
            bpf_point_del(z, zx->delidx);
            printf("del %i\n", zx->delidx);
            GuiFactory<BaseGuiObject>::ws_redraw(z);
            
        }
        
        if (modifiers==EMOD_CTRL)
        {
            bpf_point_split(z, zx->delidx);
            
            GuiFactory<BaseGuiObject>::ws_redraw(z);
            
        }
    }
    
    UI_fun(ui_bpfunc)::wx_mouseup_ext(t_object *z, t_object *view, t_pt pt, long modifiers)
    {
        ui_bpfunc *zx = (ui_bpfunc*)z;
        
        zx->addpos = -1;
    }
    
    
    
    
    
#pragma mark -
#pragma mark messages
    
    //get list; scaled
    UI_fun(ui_bpfunc)::m_bang(t_object *z, t_symbol *s, int argc, t_atom *argv)
    {
        
        ui_bpfunc *zx = (ui_bpfunc*)z;
        
        zx->out_list_count = (int)zx->points->size() * 2;
        
        if (zx->out_list) {free(zx->out_list);}
        
        zx->out_list = (t_atom*)malloc(sizeof(t_atom)*zx->out_list_count);
        
        int j=0;
        for (int i=0;i<zx->out_list_count;i+=2)
        {
            zx->out_list[i].a_type = A_FLOAT;
            zx->out_list[i].a_w.w_float = zx->points->at(j).x * zx->range_x + zx->shift_x;
            
            zx->out_list[i+1].a_type = A_FLOAT;
            zx->out_list[i+1].a_w.w_float = zx->points->at(j).y * zx->range_y + zx->shift_y;
            
            j++;
        }
        
        outlet_list(zx->out1, &s_list, zx->out_list_count, zx->out_list);
    }
    
    void bpf_m_raw(t_object *z, t_symbol *s, int argc, t_atom *argv)
    {
        
        ui_bpfunc *zx = (ui_bpfunc*)z;
        
        zx->out_list_count = (int)zx->points->size() * 2;
        
        if (zx->out_list) {free(zx->out_list);}
        
        zx->out_list = (t_atom*)malloc(sizeof(t_atom)*zx->out_list_count);
        
        int j=0;
        for (int i=0;i<zx->out_list_count;i+=2)
        {
            zx->out_list[i].a_type = A_FLOAT;
            zx->out_list[i].a_w.w_float = zx->points->at(j).x ;
            
            zx->out_list[i+1].a_type = A_FLOAT;
            zx->out_list[i+1].a_w.w_float = zx->points->at(j).y ;
            
            j++;
        }
        
        outlet_list(zx->out1, &s_list, zx->out_list_count, zx->out_list);
    }
    
    void bpf_m_range_x(t_object *z, t_symbol *s, int argc, t_atom *argv)
    {
        ui_bpfunc *zx = (ui_bpfunc*)z;
        zx->range_x = argv[0].a_w.w_float;
    }
    
    void bpf_m_range_y(t_object *z, t_symbol *s, int argc, t_atom *argv)
    {
        ui_bpfunc *zx = (ui_bpfunc*)z;
        zx->range_y = argv[0].a_w.w_float;
    }
    
    void bpf_m_shift_x(t_object *z, t_symbol *s, int argc, t_atom *argv)
    {
        ui_bpfunc *zx = (ui_bpfunc*)z;
        zx->shift_x = argv[0].a_w.w_float;
    }
    
    void bpf_m_shift_y(t_object *z, t_symbol *s, int argc, t_atom *argv)
    {
        ui_bpfunc *zx = (ui_bpfunc*)z;
        zx->shift_y = argv[0].a_w.w_float;
    }
    
    void bpf_m_select(t_object *z, t_symbol *s, int argc, t_atom *argv)
    {
        if (argc<1) return;
        
        if (argv->a_type == A_FLOAT)
        {
            ui_bpfunc *zx = (ui_bpfunc*)z;
            zx->select_idx = (int)(argv[0].a_w.w_float);
        }
        
        GuiFactory<BaseGuiObject>::ws_redraw(z);
    }
    
    void bpf_m_drag_limit(t_object *z, t_symbol *s, int argc, t_atom *argv)
    {
        if (argc<1) return;
        
        if (argv->a_type == A_FLOAT)
        {
            ui_bpfunc *zx = (ui_bpfunc*)z;
            zx->drag_limit = (int)(argv[0].a_w.w_float!=0);
        }
        
        //GuiFactory<BaseGuiObject>::ws_redraw(z);
    }
    
    void bpf_m_get(t_object *z, t_symbol *s, int argc, t_atom *argv)
    {
        if (argc<1) return;
        if (argv[0].a_type != A_FLOAT) return;
        
        ui_bpfunc *zx = (ui_bpfunc*)z;
        
        int i = int(argv[0].a_w.w_float);
        if (i>zx->points->size()-1) return;//i = (int)zx->points->size()-1;
        if (i<0) return; //i=0;
        zx->out_list_count = 2;
        
        if (zx->out_list) {free(zx->out_list);}
        
        zx->out_list = (t_atom*)malloc(sizeof(t_atom)*zx->out_list_count);
        
        zx->out_list[0].a_type = A_FLOAT;
        zx->out_list[0].a_w.w_float = zx->points->at(i).x * zx->range_x + zx->shift_x;
            
        zx->out_list[1].a_type = A_FLOAT;
        zx->out_list[1].a_w.w_float = zx->points->at(i).y * zx->range_y + zx->shift_y;
        
        outlet_list(zx->out1, &s_list, zx->out_list_count, zx->out_list);
    }
    
    void bpf_m_getraw(t_object *z, t_symbol *s, int argc, t_atom *argv)
    {
        if (argc<0) return;
        if (argv[0].a_type != A_FLOAT) return;
        
        ui_bpfunc *zx = (ui_bpfunc*)z;
        
        int i = int(argv[0].a_w.w_float);
        if (i>zx->points->size()-1) return;//i = (int)zx->points->size()-1;
        if (i<0) return; //i=0;
        zx->out_list_count = 2;
        
        if (zx->out_list) {free(zx->out_list);}
        
        zx->out_list = (t_atom*)malloc(sizeof(t_atom)*zx->out_list_count);
        
        zx->out_list[0].a_type = A_FLOAT;
        zx->out_list[0].a_w.w_float = zx->points->at(i).x;
        
        zx->out_list[1].a_type = A_FLOAT;
        zx->out_list[1].a_w.w_float = zx->points->at(i).y;
        
        outlet_list(zx->out1, &s_list, zx->out_list_count, zx->out_list);
    }
    
    void bpf_m_set(t_object *z, t_symbol *s, int argc, t_atom *argv)
    {
        if (argc<3) return;
        if (argv[0].a_type != A_FLOAT) return;
        
        ui_bpfunc *zx = (ui_bpfunc*)z;
        
        int i = int(argv[0].a_w.w_float);
        if (i>zx->points->size()-1) return;//i = (int)zx->points->size()-1;
        if (i<0) return; //i=0;
        
        float x = argv[1].a_w.w_float;
        float y = argv[2].a_w.w_float;
        
        zx->points->at(i).x = (x-zx->shift_x)/zx->range_x;
        zx->points->at(i).y = (y-zx->shift_y)/zx->range_y;
        
        GuiFactory<BaseGuiObject>::ws_redraw(z);
    }
    
    void bpf_m_set_raw(t_object *z, t_symbol *s, int argc, t_atom *argv)
    {
        if (argc<3) return;
        if (argv[0].a_type != A_FLOAT) return;
        
        ui_bpfunc *zx = (ui_bpfunc*)z;
        
        int i = int(argv[0].a_w.w_float);
        if (i>zx->points->size()-1) return;//i = (int)zx->points->size()-1;
        if (i<0) return; //i=0;
        
        float x = argv[1].a_w.w_float;
        float y = argv[2].a_w.w_float;
        
        zx->points->at(i).x = x;
        zx->points->at(i).y = y;
        
        GuiFactory<BaseGuiObject>::ws_redraw(z);
    }
    
    
    void bpf_m_end_seg(t_object *z, t_symbol *s, int argc, t_atom *argv)
    {
        if (argc<2) return;
        if (argv[0].a_type != A_FLOAT) return;
        
        ui_bpfunc *zx = (ui_bpfunc*)z;
        
        int idx = int(argv[0].a_w.w_float);
        if (idx>zx->points->size()-1) return;//idx = (int)zx->points->size()-1;
        if (idx<0) return; //idx=0;
        
        bool b = argv[1].a_w.w_float != 0;
        
        zx->points->at(idx).end_segment = b;
        
        if (b)
        {
            zx->seg_count ++;
            
            zx->seg_idx.push_back(idx);
            std::sort(zx->seg_idx.begin(), zx->seg_idx.end());
        }
        else
        {
            zx->seg_count--;
            
            vector<int>::iterator pos = find(zx->seg_idx.begin(),zx->seg_idx.end(), idx);
            if (pos != zx->seg_idx.end())
                zx->seg_idx.erase(pos);
            
        };
        
        GuiFactory<BaseGuiObject>::ws_redraw(z);
        
    }
    
    
    
    void bpf_m_seg_count(t_object *z, t_symbol *s, int argc, t_atom *argv)
    {
        
        ui_bpfunc *zx = (ui_bpfunc*)z;
        
        if (zx->out_list) {free(zx->out_list);}
        zx->out_list = (t_atom*)malloc(sizeof(t_atom));
        
        zx->out_list[0].a_type = A_FLOAT;
        zx->out_list[0].a_w.w_float = zx->seg_count;
        
        outlet_list(zx->out1, &s_list, 1, zx->out_list);
    }
    
    void bpf_m_lock_x(t_object *z, t_symbol *s, int argc, t_atom *argv)
    {
        if (argc<2) return;
        
        if (argv[0].a_type == A_FLOAT)
        {
            ui_bpfunc *zx = (ui_bpfunc*)z;
            int idx = (int)(argv[0].a_w.w_float);
            zx->points->at(idx).lock_x = (argv[1].a_w.w_float!=0);
        }
        
        GuiFactory<BaseGuiObject>::ws_redraw(z);
    }
    
    void bpf_m_lock_y(t_object *z, t_symbol *s, int argc, t_atom *argv)
    {
        if (argc<2) return;
        
        if (argv[0].a_type == A_FLOAT)
        {
            ui_bpfunc *zx = (ui_bpfunc*)z;
            int idx = (int)(argv[0].a_w.w_float);
            zx->points->at(idx).lock_y = (argv[1].a_w.w_float!=0);
        }
        
        GuiFactory<BaseGuiObject>::ws_redraw(z);
    }
    
    void bpf_m_join_next(t_object *z, t_symbol *s, int argc, t_atom *argv)
    {
        if (argc<2) return;
        
        if (argv[0].a_type == A_FLOAT)
        {
            ui_bpfunc *zx = (ui_bpfunc*)z;
            int idx = (int)(argv[0].a_w.w_float);
            zx->points->at(idx).join_next = (argv[1].a_w.w_float!=0);
        }
        
        GuiFactory<BaseGuiObject>::ws_redraw(z);
    }
    
    
    
    void bpf_m_clear(t_object *z, t_symbol *s, int argc, t_atom *argv)
    {
        
        ui_bpfunc *zx = (ui_bpfunc*)z;
        
        bpf_points_new(z);
        
        bpf_point_add(z, 0, 0, 0);
        bpf_point_add(z, 1, .5, .75);
        bpf_point_add(z, 2, 1, 1);
        
        zx->seg_count = 1;
        
        zx->seg_idx.clear();
        zx->seg_idx.push_back(0);
        
        GuiFactory<BaseGuiObject>::ws_redraw(z);
        
    }
    
    void bpf_m_add(t_object *z, t_symbol *s, int argc, t_atom *argv)
    {
        ui_bpfunc *zx = (ui_bpfunc*)z;
        
        if (argc <2) return;
        if ( (argv[0].a_type != A_FLOAT) && (argv[1].a_type != A_FLOAT) ) return;
        
        float xx = argv[0].a_w.w_float;
        float yy = argv[1].a_w.w_float;
        
        t_rect rect;
        ebox_get_rect_for_view((t_ebox *)z, &rect);
        
        bpf_point_add(z, zx->addidx, (xx-zx->shift_x)/zx->range_x, (yy-zx->shift_y)/zx->range_y);
        GuiFactory<BaseGuiObject>::ws_redraw(z);
        
    }
    
    void bpf_m_add_raw(t_object *z, t_symbol *s, int argc, t_atom *argv)
    {
        ui_bpfunc *zx = (ui_bpfunc*)z;
        
        if (argc <2) return;
        if ( (argv[0].a_type != A_FLOAT) && (argv[1].a_type != A_FLOAT) ) return;
        
        float xx = argv[0].a_w.w_float;
        float yy = argv[1].a_w.w_float;
        
        t_rect rect;
        ebox_get_rect_for_view((t_ebox *)z, &rect);
        
        bpf_point_add(z, zx->addidx, xx, yy );
        GuiFactory<BaseGuiObject>::ws_redraw(z);
        
    }
    
    void bpf_m_del(t_object *z, t_symbol *s, int argc, t_atom *argv)
    {
        if (argc <1) return;
        if (argv[0].a_type != A_FLOAT) return;
        
        int del_i = (int)argv[0].a_w.w_float;
        
        bpf_point_del(z, del_i);
        GuiFactory<BaseGuiObject>::ws_redraw(z);
        
        
    }
    
    void bpf_m_vline(t_object *z, t_symbol *s, int argc, t_atom *argv)
    {
        
        ui_bpfunc *zx = (ui_bpfunc*)z;
        
        
        float last_time = 0;
        
        
        //?replace with AtomList
        for (int j=0;j<zx->points->size();j++)      //i is on vacation
        {
            t_atom *out_list = (t_atom*)malloc(sizeof(t_atom)*3);
            
            float this_time = zx->points->at(j).x * zx->range_x + zx->shift_x;
            
            out_list[1].a_type = A_FLOAT;
            out_list[1].a_w.w_float = this_time;
            
            out_list[0].a_type = A_FLOAT;
            out_list[0].a_w.w_float = zx->points->at(j).y * zx->range_y + zx->shift_y;
            
            out_list[2].a_type = A_FLOAT;
            out_list[2].a_w.w_float = last_time;
            
            last_time += this_time;
            
            outlet_list(zx->out1, &s_list, 3, out_list);
            
        }
        
        
    }
    
    void bpf_m_vline_seg(t_object *z, t_symbol *s, int argc, t_atom *argv)
    {
        if (argc<1) return;
        if (argv[0].a_type!=A_FLOAT) {return;}
        
        int seg_number = int(argv[0].a_w.w_float);
        
        ui_bpfunc *zx = (ui_bpfunc*)z;
        
        if (seg_number<0) {error("segment number should be greater than zero"); return;}
        if (seg_number>(zx->seg_count-1)) {error("segment number out of range"); return;}
        
        vector<int>::iterator it = zx->seg_idx.begin();
        //printf("advance %d to %d\n", *it, seg_number);
        advance(it, seg_number);
        
        int seg_index_start = *it ;
        int seg_index_end = (int)zx->points->size()-1;
        
        if (seg_number<(zx->seg_count-1))
        {
            advance(it,1);
            seg_index_end = *it;
        }
        
        //printf("from %d to %d\n", seg_index_start, seg_index_end);
        
        
        float last_time = 0;
        
        
        //?replace with AtomList
        for (int j=seg_index_start;j<=seg_index_end;j++)      //i is on vacation
        {
            
            t_atom *out_list = (t_atom*)malloc(sizeof(t_atom)*3);
            
            float this_time = zx->points->at(j).x * zx->range_x + zx->shift_x;
            
            out_list[1].a_type = A_FLOAT;
            out_list[1].a_w.w_float = this_time;
            
            out_list[0].a_type = A_FLOAT;
            out_list[0].a_w.w_float = zx->points->at(j).y * zx->range_y + zx->shift_y;
            
            out_list[2].a_type = A_FLOAT;
            out_list[2].a_w.w_float = last_time;
            
            last_time += this_time;
            
            outlet_list(zx->out1, &s_list, 3, out_list);
            
            
        }
        
        
    }
    
    void bpf_m_vline_tgl(t_object *z, t_symbol *s, int argc, t_atom *argv)
    {
        if (argc<1) return;
        if (argv[0].a_type!=A_FLOAT) {return;}
        
        int seg_number = int(argv[0].a_w.w_float);
        
        //ui_bpfunc *zx = (ui_bpfunc*)z;
        
        if (seg_number<0) {error("segment number should be 0 or 1"); return;}
        if (seg_number>(1)) {error("segment number should be 0 or 1"); return;}
        
        t_atom tgl;
        tgl.a_type = A_FLOAT;
        tgl.a_w.w_float = 1 - seg_number;
        
        bpf_m_vline_seg(z, s, 1, &tgl);
        
    }
    
    void bpf_m_auto_send(t_object *z, t_symbol *s, int argc, t_atom *argv)
    {
        if (argc<1) return;
        
        if (argv->a_type == A_FLOAT)
        {
            ui_bpfunc *zx = (ui_bpfunc*)z;
            zx->auto_send = (argv[0].a_w.w_float>0);
        }
        
        
    }
    
    void bpf_m_setbpf(t_object *z, t_symbol *s, int argc, t_atom *argv)
    {
        if (argc<2) return;
        
        //ui_bpfunc *zx = (ui_bpfunc*)z;
        
        AtomList list = AtomList(argc,argv);
        
        int num_points = floor(list.size()/2);
        
        
        bpf_points_new(z);
        
        for (int i=0; i<num_points; i++)
        {
            bpf_point_add(z, i, list.at(i*2).asFloat(), list.at(i*2+1).asFloat());
        }
        
        GuiFactory<BaseGuiObject>::ws_redraw(z);
    
    }
    
    
#pragma mark env
    
    //list sending defines
#define e_list_clear list.clear();
#define e_list_addf(x) a = Atom(x); list.append(a);
    
    void bpf_m_env(t_object *z, t_symbol *s, int argc, t_atom *argv)
    {
        if (argc<1) {error("bad argument");return;}
        if (argv->a_type!= A_SYMBOL) {error("bad argument");return;}
        
        if (Atom(argv[0]).asString() == "adsr")
        {
//            ;
//            bpf1 clear;
//            bpf1 set_raw 1 0.25 1;
//            bpf1 set_raw 2 0.5 0.6;
//            bpf1 add_raw 1 0;
//            bpf1 lock_y 0 1;
//            bpf1 lock_y 1 1;
//            bpf1 lock_y 3 1;
//            bpf1 end_seg 2 1;
//            bpf1 auto_send 1;
//            bpf1 drag_limit 1
            
            AtomList list;
            Atom a;
            
            //todo cleanup
            
            bpf_m_clear(z, 0, 0, 0);
            
            e_list_clear e_list_addf(1) e_list_addf(.25) e_list_addf(1)
            bpf_m_set_raw(z, 0, (int)list.size(), list.toPdData());
            e_list_clear e_list_addf(2) e_list_addf(.5) e_list_addf(.6)
            bpf_m_set_raw(z, 0, (int)list.size(), list.toPdData());
            e_list_clear e_list_addf(1) e_list_addf(0.)
            bpf_m_add_raw(z, 0, (int)list.size(), list.toPdData());
            e_list_clear e_list_addf(0.) e_list_addf(1)
            bpf_m_lock_y(z, 0, (int)list.size(), list.toPdData());
            e_list_clear e_list_addf(1.) e_list_addf(1)
            bpf_m_lock_y(z, 0, (int)list.size(), list.toPdData());
            e_list_clear e_list_addf(3.) e_list_addf(1)
            bpf_m_lock_y(z, 0, (int)list.size(), list.toPdData());
            e_list_clear e_list_addf(2) e_list_addf(1)
            bpf_m_end_seg(z, 0, (int)list.size(), list.toPdData());
            e_list_clear e_list_addf(1)
            bpf_m_auto_send(z, 0, (int)list.size(), list.toPdData());
            e_list_clear e_list_addf(1)
            bpf_m_drag_limit(z, 0, (int)list.size(), list.toPdData());
            
        }
            else if(Atom(argv[0]).asString() == "ahr")
        {
//            ;
//            bpf2 clear;
//            bpf2 set_raw 1 0.25 0.7;
//            bpf2 set_raw 2 0.5 0.7;
//            bpf2 add_raw 1 0;
//            bpf2 lock_y 0 1;
//            bpf2 lock_y 2 1;
//            bpf2 join_next 1 1;
//            bpf2 lock_y 3 1;
//            bpf2 auto_send 1;
//            bpf2 drag_limit 1
            
            AtomList list;
            Atom a;
            
            //todo cleanup
            bpf_m_clear(z, 0, 0, 0);
            
            e_list_clear e_list_addf(1) e_list_addf(.25) e_list_addf(.7)
            bpf_m_set_raw(z, 0, (int)list.size(), list.toPdData());
            e_list_clear e_list_addf(2) e_list_addf(.5) e_list_addf(.7)
            bpf_m_set_raw(z, 0, (int)list.size(), list.toPdData());
            e_list_clear e_list_addf(1) e_list_addf(0.)
            bpf_m_add_raw(z, 0, (int)list.size(), list.toPdData());
            e_list_clear e_list_addf(0.) e_list_addf(1)
            bpf_m_lock_y(z, 0, (int)list.size(), list.toPdData());
            e_list_clear e_list_addf(2.) e_list_addf(1)
            bpf_m_lock_y(z, 0, (int)list.size(), list.toPdData());
            e_list_clear e_list_addf(3.) e_list_addf(1)
            bpf_m_lock_y(z, 0, (int)list.size(), list.toPdData());
            e_list_clear e_list_addf(1.) e_list_addf(1)
            bpf_m_join_next(z, 0, (int)list.size(), list.toPdData());
            e_list_clear e_list_addf(1)
            bpf_m_auto_send(z, 0, (int)list.size(), list.toPdData());
            e_list_clear e_list_addf(1)
            bpf_m_drag_limit(z, 0, (int)list.size(), list.toPdData());
        }
        else
        {
            error("bad envelope type specified: %s", ceammc::Atom(argv[0]).asString().c_str());
        }
        
//        if (argv->a_type == A_FLOAT)
//        {
//            ui_bpfunc *zx = (ui_bpfunc*)z;
//            zx->auto_send = (argv[0].a_w.w_float>0);
//        }
        
        
    }
    
#pragma mark -
#pragma mark mouse+
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
                bpf_points::iterator it_prev = it-1;
                bpf_points::iterator it_next = it+1;
                
                float prev_x = -1.;
                if (it_prev>= zx->points->begin())
                    prev_x = it_prev->x;
                
                float next_x = 2.;
                if (it_next< zx->points->end())
                    next_x = it_next->x;
                
                float dx =(pt.x - zx->_px) / rect.width;
                
                if ( !((zx->drag_limit) && ( (prev_x>(it->x+dx)) || (next_x<(it->x+dx)) ) ))
                {
                    if (!it->lock_x)
                        it->x += dx;
                    if (!it->lock_y)
                        it->y += ((1-pt.y) - (1-zx->_py)) / rect.height;
                    
                    if (it->x>1) it->x=1;
                    if (it->x<0) it->x=0;
                    
                    if (it->y>1) it->y=1;
                    if (it->y<0) it->y=0;
                    
                    if (it->join_next)
                    {
                        it_next->y = it->y;
                    }
                    
                }
                
            }
        }
        
        zx->_px = pt.x;
        zx->_py = pt.y;
        
        bpf_point_sort(z);
        
        GuiFactory<BaseGuiObject>::ws_redraw(z);
        
        if (zx->auto_send)
        {
            GuiFactory<ui_bpfunc>::m_bang(z, &s_bang, 0, NULL);
        }
        
    }
    
#pragma mark -
    
    UI_fun(ui_bpfunc)::new_ext(t_object *z, t_symbol *s, int argcl, t_atom *argv)
    {
//        ((t_ebox*)z)->b_boxparameters.d_boxfillcolor.red = 0.75;
//        ((t_ebox*)z)->b_boxparameters.d_boxfillcolor.green = 0.75;
//        ((t_ebox*)z)->b_boxparameters.d_boxfillcolor.blue   = 0.75;
//        ((t_ebox*)z)->b_boxparameters.d_boxfillcolor.alpha = 1.;
        
        bpf_points_new(z);
        
        bpf_point_add(z, 0, 0, 0);
        bpf_point_add(z, 1, .5, .75);
        bpf_point_add(z, 2, 1, 1);
        
        ui_bpfunc *zx = (ui_bpfunc*)z;
        
        zx->_px = 0;
        zx->_py = 0;
        
        zx->out1 = outlet_new(z, &s_list);
        
        zx->range_x = 1000;
        zx->range_y = 1;
        zx->shift_x = 0;
        zx->shift_y = 0;
        
        zx->select_idx = -1;
        
        zx->auto_send = 0;
        
        zx->seg_count = 1;
        
        //zx->output = new AtomList;
        
        zx->out_list = (t_atom*) malloc(sizeof(t_atom));
        
        zx->txt_max = etext_layout_create();
        zx->txt_min = etext_layout_create();
        zx->txt_val = etext_layout_create();
        
        zx->seg_idx.clear();
        zx->seg_idx.push_back(0);
        
        zx->txt_font = efont_create(gensym("Helvetica"), gensym("light"), gensym("normal"), 8);
        
        
    }
    
    static void ui_bpf_getdrawparams(ui_bpfunc *x, t_object *patcherview, t_edrawparams *params)
    {
        params->d_borderthickness   = 1;
        params->d_cornersize        = 2;
        params->d_bordercolor       = x->b_color_border;
        params->d_boxfillcolor      = x->b_color_background;
    }
    
    UI_fun(ui_bpfunc)::init_ext(t_eclass *z)
    {
        CLASS_ATTR_DEFAULT (z, "size", 0, "200. 150.");
        
        CLASS_ATTR_FLOAT(z, "shift_x", 0, ui_bpfunc, shift_x);
        CLASS_ATTR_DEFAULT(z, "shift_x", 0, "0");
        CLASS_ATTR_LABEL(z, "shift_x", 0, "shift_x");
        CLASS_ATTR_DEFAULT_SAVE_PAINT(z, "shift_x", 0, "0");
        
        CLASS_ATTR_FLOAT(z, "range_x", 0, ui_bpfunc, range_x);
        CLASS_ATTR_DEFAULT(z, "range_x", 0, "1");
        CLASS_ATTR_LABEL(z, "range_x", 0, "range_x");
        CLASS_ATTR_DEFAULT_SAVE_PAINT(z, "range_x", 0, "1");
        
        CLASS_ATTR_FLOAT(z, "shift_y", 0, ui_bpfunc, shift_y);
        CLASS_ATTR_DEFAULT(z, "shift_y", 0, "0");
        CLASS_ATTR_LABEL(z, "shift_y", 0, "shift_y");
        CLASS_ATTR_DEFAULT_SAVE_PAINT(z, "shift_y", 0, "0");
        
        CLASS_ATTR_FLOAT(z, "range_y", 0, ui_bpfunc, range_y);
        CLASS_ATTR_DEFAULT(z, "range_y", 0, "1");
        CLASS_ATTR_LABEL(z, "range_y", 0, "range_y");
        CLASS_ATTR_DEFAULT_SAVE_PAINT(z, "range_y", 0, "1");
        
        CLASS_ATTR_INT(z, "drag_limit", 0, ui_bpfunc, drag_limit);
        CLASS_ATTR_DEFAULT(z, "drag_limit", 0, "1");
        CLASS_ATTR_LABEL(z, "drag_limit", 0, "drag_limit");
        CLASS_ATTR_DEFAULT_SAVE_PAINT(z, "drag_limit", 0, "0");
        CLASS_ATTR_STYLE(z, "drag_limit", 0, "onoff");
        
        CLASS_ATTR_INT(z, "auto_send", 0, ui_bpfunc, auto_send);
        CLASS_ATTR_DEFAULT(z, "auto_send", 0, "1");
        CLASS_ATTR_LABEL(z, "auto_send", 0, "auto_send");
        CLASS_ATTR_DEFAULT_SAVE_PAINT(z, "auto_send", 0, "0");
        CLASS_ATTR_STYLE(z, "auto_send", 0, "onoff");
        
        CLASS_ATTR_RGBA                 (z, "bgcolor", 0, ui_bpfunc, b_color_background);
        CLASS_ATTR_LABEL                (z, "bgcolor", 0, "Background Color");
        CLASS_ATTR_ORDER                (z, "bgcolor", 0, "1");
        CLASS_ATTR_DEFAULT_SAVE_PAINT   (z, "bgcolor", 0, "0.93 0.93 0.93 1.");
        CLASS_ATTR_STYLE                (z, "bgcolor", 0, "color");
        
        CLASS_ATTR_RGBA                 (z, "bdcolor", 0, ui_bpfunc, b_color_border);
        CLASS_ATTR_LABEL                (z, "bdcolor", 0, "Border Color");
        CLASS_ATTR_ORDER                (z, "bdcolor", 0, "2");
        CLASS_ATTR_DEFAULT_SAVE_PAINT   (z, "bdcolor", 0, "0. 0. 0. 1.");
        CLASS_ATTR_STYLE                (z, "bdcolor", 0, "color");
        
        //todo readonly or disable?
//        CLASS_ATTR_INT(z, "seg_count", 0, ui_bpfunc, seg_count);
//        CLASS_ATTR_DEFAULT(z, "seg_count", 0, "1");
//        CLASS_ATTR_LABEL(z, "seg_count", 0, "seg_count");
//        CLASS_ATTR_DEFAULT_SAVE_PAINT(z, "seg_count", 0, "1");
        
        
        eclass_addmethod(z, (method)(bpf_m_range_x), ("range_x"), A_GIMME,0);
        eclass_addmethod(z, (method)(bpf_m_range_y), ("range_y"), A_GIMME,0);
        eclass_addmethod(z, (method)(bpf_m_shift_x), ("shift_x"), A_GIMME,0);
        eclass_addmethod(z, (method)(bpf_m_shift_y), ("shift_y"), A_GIMME,0);
        
        eclass_addmethod(z, (method)(bpf_m_select), ("sel"), A_GIMME,0);
        
        eclass_addmethod(z, (method)(bpf_m_set), ("set"), A_GIMME,0);
        eclass_addmethod(z, (method)(bpf_m_set_raw), ("set_raw"), A_GIMME,0);
        
        eclass_addmethod(z, (method)(bpf_m_get), ("get"), A_GIMME,0);
        eclass_addmethod(z, (method)(bpf_m_getraw), ("getraw"), A_GIMME,0);
        
        eclass_addmethod(z, (method)(bpf_m_add), ("add"), A_GIMME,0);
        eclass_addmethod(z, (method)(bpf_m_add_raw), ("add_raw"), A_GIMME,0);
        
        eclass_addmethod(z, (method)(bpf_m_del), ("del"), A_GIMME,0);
        
        eclass_addmethod(z, (method)(bpf_m_drag_limit), ("drag_limit"), A_GIMME,0);
        
        
        //stubs
        
        //eclass_addmethod(z, (method)(bpf_m_set_seg), ("set_seg"), A_GIMME,0);
        eclass_addmethod(z, (method)(bpf_m_setbpf), ("set_rawbpf"), A_GIMME,0);
        
        eclass_addmethod(z, (method)(bpf_m_env), ("env"), A_GIMME,0);
        
        eclass_addmethod(z, (method)(bpf_m_seg_count), ("seg_count"), A_GIMME,0);
        
        eclass_addmethod(z, (method)(bpf_m_end_seg), ("end_seg"), A_GIMME,0);
        
        //bang for getting the whole envelope
        eclass_addmethod(z, (method)(bpf_m_raw), ("raw"), A_GIMME,0);
        
        eclass_addmethod(z, (method)(bpf_m_auto_send), ("auto_send"), A_GIMME,0);
        
        eclass_addmethod(z, (method)(bpf_m_lock_x), ("lock_x"), A_GIMME,0);
        eclass_addmethod(z, (method)(bpf_m_lock_y), ("lock_y"), A_GIMME,0);
        eclass_addmethod(z, (method)(bpf_m_join_next), ("join_next"), A_GIMME,0);
        
        eclass_addmethod(z, (method)(bpf_m_clear), ("clear"), A_GIMME,0);
        
        eclass_addmethod(z, (method)(bpf_m_vline), ("vline"), A_GIMME,0);
        eclass_addmethod(z, (method)(bpf_m_vline_seg), ("vline_seg"), A_GIMME,0);
        eclass_addmethod(z, (method)(bpf_m_vline_tgl), ("vline_tgl"), A_GIMME,0);
        
        eclass_addmethod(z, (method)(ui_bpf_getdrawparams), "getdrawparams", A_NULL, 0);
        
    }
    
}

extern "C" void setup_ui0x2ebpfunc()
{
    GuiFactory<ui_bpfunc> class1;
    class1.setup("ui.bpfunc");
}