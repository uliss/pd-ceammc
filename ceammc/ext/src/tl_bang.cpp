////
////  tl_bang2.cpp
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

struct tl_bang : public base_pd_object
{
    
    t_etext *txt;
    t_efont *fnt;
    
    t_rgba border_color;
    t_rgba bg_color;
    
    t_object x_gui;
    t_outlet *out1;
};


UI_fun(tl_bang)::wx_paint(t_object *z, t_object *view)
{
    
    t_symbol *bgl = gensym("background_layer");
    //float size;
    
    t_rect rect;
    ebox_get_rect_for_view((t_ebox *)z, &rect);
    
    t_elayer *g = ebox_start_layer((t_ebox *)z, bgl, rect.width, rect.height);
    if(g)
    {
        tl_bang* zx = (tl_bang*)z;
        
        egraphics_rectangle(g, 0, 0, rect.width, rect.height);
        egraphics_set_color_hex(g, gensym("#F0F0F0"));
        egraphics_fill(g);
        
        etext_layout_set(zx->txt, "tl.bang", zx->fnt, 2, 15, rect.width, rect.height/2, ETEXT_DOWN_LEFT, ETEXT_JLEFT, ETEXT_NOWRAP);
        etext_layout_draw(zx->txt, g);  //zx->cue_name.c_str()
        
        tll_ui_update_pos((t_object *)z, zx->b_box.b_rect.x);
        
    }
    
    ebox_end_layer((t_ebox*)z, bgl);
    ebox_paint_layer((t_ebox *)z, bgl, 0., 0.);
    
}



#pragma mark -

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
        x->b_rect.y += dy;
        x->b_obj.o_obj.te_xpix += dx;
        x->b_obj.o_obj.te_ypix += dy;
        tl_cue_ebox_move(x);
    }
#else
    t_ebox* x = (t_ebox *)z;
    
    x->b_rect.x += dx;
    x->b_rect.y += dy;
    x->b_obj.o_obj.te_xpix += dx;
    x->b_obj.o_obj.te_ypix += dy;
    tl_cue_ebox_move(x);
#endif
    
    t_rect rect;
    ebox_get_rect_for_view((t_ebox *)x, &rect);
    
    tl_bang* zx = (tl_bang*)z;
    
    object<tl_bang>::ws_redraw((t_object*)z);
    
    tll_ui_update_pos((t_object *)z, zx->b_box.b_rect.x);
}


UI_fun(tl_bang)::wx_oksize(t_object *z, t_rect *newrect)
{
    newrect->width=60;
    newrect->height=15;
}

#pragma mark

static void tl_bang_getdrawparams(tl_bang *x, t_object *view, t_edrawparams *params)
{
    params->d_borderthickness   = 2;
    params->d_cornersize        = 2;
    params->d_bordercolor       = x->border_color;
    params->d_boxfillcolor      = x->bg_color;
}

#pragma mark -


UI_fun(tl_bang)::new_ext(t_object* z, t_symbol *s, int argc, t_atom *argv)
{
    tl_bang* zx = (tl_bang*)z;
    
    tll_ui_add((t_object*)z, zx->b_box.b_rect.x);
    ((tl_bang*)z)->out1 =outlet_new(z, &s_bang);
    tll_ui_update_pos((t_object *)z, zx->b_box.b_obj.o_obj.te_xpix);
    
    zx->txt = etext_layout_create();
    zx->fnt = efont_create(gensym("Monaco"), gensym(""), gensym("normal"), 12);
    
    
}


UI_fun(tl_bang)::init_ext(t_eclass *z)
{
    z->c_widget.w_displacefn = tl_cue_displace;
    
    eclass_addmethod(z, (method) tl_bang_getdrawparams,   "getdrawparams",    A_NULL, 0);
   
    
    CLASS_ATTR_RGBA                 (z, "brcolor", 0, tl_bang, border_color);
    CLASS_ATTR_LABEL                (z, "brcolor", 0, "Border Color");
    CLASS_ATTR_ORDER                (z, "brcolor", 0, "3");
    CLASS_ATTR_DEFAULT_SAVE_PAINT   (z, "brcolor", 0, "0. 0.7 1. 1.");
    CLASS_ATTR_STYLE                (z, "brcolor", 0, "color");
    
    CLASS_ATTR_RGBA                 (z, "bgcolor", 0, tl_bang, bg_color);
    CLASS_ATTR_LABEL                (z, "bgcolor", 0, "Background Color");
    CLASS_ATTR_ORDER                (z, "bgcolor", 0, "3");
    CLASS_ATTR_DEFAULT_SAVE_PAINT   (z, "bgcolor", 0, ".7 .7 .7 1.");
    CLASS_ATTR_STYLE                (z, "bgcolor", 0, "color");
}



UI_fun(tl_bang)::free_ext(t_object *x)
{
    tll_ui_delete(x);
    
}


#pragma mark -
void tl_bang_action(t_object *x)
{
//    printf("bang action");
    outlet_bang(((tl_bang*)x)->out1);
}


extern "C" void setup_tl0x2ebang()
{
    object<tl_bang> class1;
    tll_set_ui_action((tl_bang_action));    //reinterpret_cast<tl_ui_action>
    class1.setup("tl.bang");
    
}



