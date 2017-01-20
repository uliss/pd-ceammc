////
////  tl_cue.cpp
////  pd_ext
////
////  Created by Alex Nadzharov on 10/12/16.
////
////
//

//  CICM Wrapper version

#include <iostream>
#include "tl_lib.hpp"

#include "ceammc_gui.h"

using namespace ceammc_gui;

namespace ceammc_gui {
    
    struct tl_cue : public BaseGuiObject
    {
        t_canvas *canvas;
        
        t_etext *txt;
        t_efont *fnt;
        
        t_rgba border_color;
        t_rgba bg_color;
        
        std::string* cue_name;
    };
    
#pragma mark -
    
    inline void gui_rect (t_canvas * _gui_canvas, t_object * _gui_obj, std::string obj, int x, int y, int w, int h, std::string stroke_color, std::string fill_color, float line_width)
    {
        char s1[] = ".x%lx.c create rectangle %d %d %d %d -width %d -outline %s -fill %s -tags %lx%s\n";
        sys_vgui(s1,
                 _gui_canvas, x, y,x+w, y+h,
                 int(line_width),
                 stroke_color.c_str(),
                 fill_color.c_str(), _gui_obj, obj.c_str());
    }
    
    inline void gui_delete(t_canvas * _gui_canvas, t_object * _gui_obj, std::string obj )
    {
        char s1[] = ".x%lx.c delete %lx%s\n";
        sys_vgui(s1, _gui_canvas, _gui_obj, obj.c_str());
    }
    
    inline void gui_move (t_canvas * _gui_canvas, t_object * _gui_obj, std::string obj, int x, int y, int w, int h)
    {
        char s1[] = ".x%lx.c coords %lx%s %d %d %d %d\n";
        sys_vgui(s1,
                 _gui_canvas, _gui_obj, obj.c_str(), x, y, x+w, y+h);
    }
    
#pragma mark
    
    static void tl_cue_getdrawparams(tl_cue *x, t_object *view, t_edrawparams *params)
    {
        params->d_borderthickness   = 2;
        params->d_cornersize        = 2;
        params->d_bordercolor       = x->border_color;
        params->d_boxfillcolor      = x->bg_color;
    }
    
    
    UI_fun(tl_cue)::wx_paint(t_object *z, t_object *view)
    {
        
        t_symbol *bgl = gensym("background_layer");
        //float size;
        
        t_rect rect;
        ebox_get_rect_for_view((t_ebox *)z, &rect);
        
        t_elayer *g = ebox_start_layer((t_ebox *)z, bgl, rect.width, rect.height);
        if(g)
        {
            tl_cue* zx = (tl_cue*)z;
            
            egraphics_rectangle(g, 0, 0, rect.width, rect.height);
            egraphics_set_color_hex(g, gensym("#F0F0F0"));
            egraphics_fill(g);
            
            //
            tll_cue_update_pos((t_object *)z, zx->b_box.b_rect.x);
            
            int cue_idx = tll_cue_getnumber((t_object *)z);
            char cuename[10];
            sprintf(cuename, "cue_%d", cue_idx);
            *zx->cue_name = cuename;
            
            //
            
            etext_layout_set(zx->txt, zx->cue_name->c_str(), zx->fnt, 2, 15, rect.width, rect.height/2, ETEXT_DOWN_LEFT, ETEXT_JLEFT, ETEXT_NOWRAP);
            etext_layout_draw(zx->txt, g);
            
            //get layer / egraphics?
            gui_delete(zx->canvas, z, "VLINE");
            gui_rect(zx->canvas, z, "VLINE", rect.x-2 , 0.0f, 1, 1000.0f,
                     rgba_to_hex(zx->border_color), rgba_to_hex(zx->border_color), 1.0f);
            
            
        }
        
        ebox_end_layer((t_ebox*)z, bgl);
        ebox_paint_layer((t_ebox *)z, bgl, 0., 0.);
        
    }
    
    
    static void tl_cue_ebox_move(t_ebox* x)
    {
        if(glist_isvisible(x->b_obj.o_canvas))
        {
            sys_vgui("%s coords %s %d %d\n", x->b_canvas_id->s_name, x->b_window_id->s_name, (int)(x->b_rect.x - x->b_boxparameters.d_borderthickness), (int)(x->b_rect.y - x->b_boxparameters.d_borderthickness));
        }
        canvas_fixlinesfor(glist_getcanvas(x->b_obj.o_canvas), (t_text*)x);
    }
    
    void tl_cue_displace(t_gobj *z, t_glist *glist, int dx, int dy)
    {
        //todo set arg - see ui.display
        
        //ebox src
#ifdef _WINDOWS
        t_ebox* x = (t_ebox *)z;
        if(x->b_selected_box)
        {
            x->b_rect.x += dx;
            x->b_rect.y = 2;
            x->b_obj.o_obj.te_xpix += dx;
            x->b_obj.o_obj.te_ypix = 0;
            tl_cue_ebox_move(x);
        }
#else
        t_ebox* x = (t_ebox *)z;
        
        x->b_rect.x += dx;
        x->b_rect.y = 2;
        x->b_obj.o_obj.te_xpix += dx;
        x->b_obj.o_obj.te_ypix = 2;
        tl_cue_ebox_move(x);
#endif
        
        t_rect rect;
        ebox_get_rect_for_view((t_ebox *)x, &rect);
        
        tl_cue* zx = (tl_cue*)z;
        
        //    gui_move(glist, (t_object*)z, "VLINE", rect.x-2, 0, 1, 1000);
        
        //    int cue_idx = tll_cue_getnumber((t_object *)z);
        //    char cuename[10];
        //    sprintf(cuename, "cue_%d", cue_idx);
        //    zx->cue_name = cuename;
        
        //
        
        tll_cue_update_pos((t_object *)z, zx->b_box.b_rect.x);
        tll_update_cue_guis(NULL);  //(t_object *)z
        
    }
    
    
    
    UI_fun(tl_cue)::wx_oksize(t_object *z, t_rect *newrect)
    {
        newrect->width=45;
        newrect->height=15;
    }
    
    UI_fun(tl_cue)::free_ext(t_object *z)
    {
        tll_cue_delete(z);
    }
    
    void tl_cue_draw(t_object *z)
    {
        GuiFactory<tl_cue>::ws_redraw((t_object*)z);
    }
    
#pragma mark -
    
    
    UI_fun(tl_cue)::new_ext(t_object *z, t_symbol *s, int argc, t_atom *argv)
    {
        tl_cue* zx = (tl_cue*)z;
        
        zx->txt = etext_layout_create();
        zx->fnt = efont_create(gensym("Monaco"), gensym(""), gensym("normal"), 12);
        
        zx->canvas = canvas_getcurrent();
        
        zx->cue_name = new std::string("cue_x");
        
        //
        tll_cue_add((t_object*)z, zx->b_box.b_rect.x);
        tll_cue_update_pos((t_object *)z, zx->b_box.b_rect.x);
        tll_update_cue_guis(NULL);
    }
    
    
    UI_fun(tl_cue)::init_ext(t_eclass *z)
    {
        CLASS_ATTR_DEFAULT (z, "size", 0, "200. 150.");
        
        CLASS_ATTR_RGBA                 (z, "brcolor", 0, tl_cue, border_color);
        CLASS_ATTR_LABEL                (z, "brcolor", 0, "Border Color");
        CLASS_ATTR_ORDER                (z, "brcolor", 0, "3");
        CLASS_ATTR_DEFAULT_SAVE_PAINT   (z, "brcolor", 0, "0. 0.7 1. 1.");
        CLASS_ATTR_STYLE                (z, "brcolor", 0, "color");
        
        CLASS_ATTR_RGBA                 (z, "bgcolor", 0, tl_cue, bg_color);
        CLASS_ATTR_LABEL                (z, "bgcolor", 0, "Background Color");
        CLASS_ATTR_ORDER                (z, "bgcolor", 0, "3");
        CLASS_ATTR_DEFAULT_SAVE_PAINT   (z, "bgcolor", 0, ".7 .7 .7 1.");
        CLASS_ATTR_STYLE                (z, "bgcolor", 0, "color");
        
        z->c_widget.w_displacefn = tl_cue_displace;
        
        eclass_addmethod(z, (method) tl_cue_getdrawparams,   "getdrawparams",    A_NULL, 0);
        
        
    }
}

#pragma mark -

extern "C" void setup_tl0x2ecue()
{
    GuiFactory<tl_cue> class1;
    tll_set_ui_drawcue(tl_cue_draw);
    class1.setup_noin("tl.cue");
}
