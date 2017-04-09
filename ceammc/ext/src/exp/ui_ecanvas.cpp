//
//  exp_ecanvas.cpp
//  pd_ext
//
//  Created by Alex Nadzharov on 08/01/17.
//
//




#include "ceammc_gui.h"


using namespace ceammc_gui;



void ecanvas_d(t_glist *x)
{
    
    
    t_float xperpix, yperpix, x1, y1, x2, y2, xpix, ypix, xmargin, ymargin;
    int graphme, redraw = 0;
    
    xperpix = 1;//atom_getfloatarg(0, argc, argv);
    yperpix = 1;//atom_getfloatarg(1, argc, argv);
    graphme = 1;//(int)(atom_getfloatarg(2, argc, argv));
    x1 = 0;//atom_getfloatarg(3, argc, argv);
    y1 = 0;//atom_getfloatarg(4, argc, argv);
    x2 = 200;//atom_getfloatarg(5, argc, argv);
    y2 = 200;//atom_getfloatarg(6, argc, argv);
    xpix = 0;//atom_getfloatarg(7, argc, argv);
    ypix = 0;//atom_getfloatarg(8, argc, argv);
    xmargin = 0;//atom_getfloatarg(9, argc, argv);
    ymargin = 0;//atom_getfloatarg(10, argc, argv);
    /* hack - graphme is 0 for no, 1 for yes, and 3 for yes-and-hide-text*/
    //    if (!(graphme & 1))
    //        graphme = 0;
    x->gl_pixwidth = xpix;
    x->gl_pixheight = ypix;
    x->gl_xmargin = xmargin;
    x->gl_ymargin = ymargin;
    
    yperpix = -yperpix;
    if (xperpix == 0)
        xperpix = 1;
    if (yperpix == 0)
        yperpix = 1;
    
    if (graphme)
    {
        if (x1 != x2)
            x->gl_x1 = x1, x->gl_x2 = x2;
        else x->gl_x1 = 0, x->gl_x2 = 1;
        if (y1 != y2)
            x->gl_y1 = y1, x->gl_y2 = y2;
        else x->gl_y1 = 0, x->gl_y2 = 1;
    }
    else
    {
        if (xperpix != glist_dpixtodx(x, 1) || yperpix != glist_dpixtody(x, 1))
            redraw = 1;
        if (xperpix > 0)
        {
            x->gl_x1 = 0;
            x->gl_x2 = xperpix;
        }
        else
        {
            x->gl_x1 = -xperpix * (x->gl_screenx2 - x->gl_screenx1);
            x->gl_x2 = x->gl_x1 + xperpix;
        }
        if (yperpix > 0)
        {
            x->gl_y1 = 0;
            x->gl_y2 = yperpix;
        }
        else
        {
            x->gl_y1 = -yperpix * (x->gl_screeny2 - x->gl_screeny1);
            x->gl_y2 = x->gl_y1 + yperpix;
        }
    }
    
    
//    x->gl_name = sym;
    x->gl_x1 = x1;
    x->gl_x2 = x2;
    x->gl_y1 = y1;
    x->gl_y2 = y2;
//    x->gl_obj.te_xpix = px1;
//    x->gl_obj.te_ypix = py1;
//    x->gl_pixwidth = px2 - px1;
//    x->gl_pixheight = py2 - py1;
//    x->gl_font =  (canvas_getcurrent() ?
//                   canvas_getcurrent()->gl_font : sys_defaultfont);
    x->gl_zoom = 1;
    x->gl_screenx1 = 0;
//    x->gl_screeny1 = GLIST_DEFCANVASYLOC;
    x->gl_screenx2 = 450;
    x->gl_screeny2 = 300;
//    x->gl_owner = g;
//    canvas_bind(x);
    x->gl_isgraph = 1;
    x->gl_goprect = 0;
    x->gl_obj.te_binbuf = binbuf_new();
    binbuf_addv(x->gl_obj.te_binbuf, "s", gensym("graph"));
//    if (!menu)
//        pd_pushsym(&x->gl_pd);
    glist_add(x, &x->gl_gobj);
    
    
    /* LATER avoid doing 2 redraws here (possibly one inside setgraph) */
    //if (glist_isvisible(x->gl_owner))
    {
        canvas_setgraph(x, graphme, 0);
        canvas_dirty(x, 1);
        if (x->gl_havewindow)
            canvas_redraw(x);
        else if (glist_isvisible(x->gl_owner))
        {
            gobj_vis(&x->gl_gobj, x->gl_owner, 0);
            gobj_vis(&x->gl_gobj, x->gl_owner, 1);
        }
    }
}

//  g_editor.c
/* set a canvas up as a graph-on-parent.  Set reasonable defaults for
 any missing paramters and redraw things if necessary. */
void ecanvas_setgraph(t_glist *x, int flag, int nogoprect)
{
    if (!flag && glist_isgraph(x))
    {
        if (x->gl_owner && !x->gl_loading && glist_isvisible(x->gl_owner))
            gobj_vis(&x->gl_gobj, x->gl_owner, 0);
        x->gl_isgraph = x->gl_hidetext = 0;
        if (x->gl_owner && !x->gl_loading && glist_isvisible(x->gl_owner))
        {
            gobj_vis(&x->gl_gobj, x->gl_owner, 1);
            canvas_fixlinesfor(x->gl_owner, &x->gl_obj);
        }
    }
    else if (flag)
    {
        if (x->gl_pixwidth <= 0)
            x->gl_pixwidth = GLIST_DEFGRAPHWIDTH;
        
        if (x->gl_pixheight <= 0)
            x->gl_pixheight = GLIST_DEFGRAPHHEIGHT;
        
        if (x->gl_owner && !x->gl_loading && glist_isvisible(x->gl_owner))
            gobj_vis(&x->gl_gobj, x->gl_owner, 0);
        x->gl_isgraph = 1;
        x->gl_hidetext = !(!(flag&2));
        x->gl_goprect = !nogoprect;
        if (glist_isvisible(x) && x->gl_goprect)
            glist_redraw(x);
        if (x->gl_owner && !x->gl_loading && glist_isvisible(x->gl_owner))
        {
            gobj_vis(&x->gl_gobj, x->gl_owner, 1);
            canvas_fixlinesfor(x->gl_owner, &x->gl_obj);
        }
    }
}

namespace ceammc_gui {
    
    
    struct eCanvas : public BaseGuiObject
    {
        t_ebox b_box;
        t_canvas *canvas;
        t_canvas *root_canvas;
        
        int openWindow;
    };
    
    UI_fun(eCanvas)::wx_paint(eCanvas* z, t_object* view)
    {
        t_symbol *bgl = gensym("background_layer");
        //float size;
        t_rect rect;
        ebox_get_rect_for_view((t_ebox *)z, &rect);
        
        t_elayer *g = ebox_start_layer((t_ebox *)z, bgl, rect.width, rect.height);
        
        eCanvas *zx = (eCanvas*)z;
        
        if(g)
        {
            // EXAMPLE
            float size =20;
            egraphics_set_color_hex(g, gensym("#00C0FF"));
            egraphics_circle(g, floor(size + 0.5), floor(size + 0.5), size * 0.9);
            egraphics_fill(g);
            ebox_end_layer((t_ebox*)z, bgl);
            
            
            
            //canvas_vis(zx->canvas, 1);
            
            if (zx->canvas->gl_owner && !zx->canvas->gl_loading && glist_isvisible(zx->canvas->gl_owner) )
            {
                printf("\npaint 1\n");
                //                printf("canvas ptr %lu\n", (long)zx->canvas);
                //                printf("globj ptr %lu\n", (long)&zx->canvas->gl_obj);
                //                printf("te_g ptr %lu\n", (long)&zx->canvas->gl_obj.te_g);
                //
                //                printf("***\n");
                //                printf("root canvas ptr %lu\n", (long)zx->root_canvas);
                //                printf("root globj ptr %lu\n", (long)&zx->root_canvas->gl_obj);
                //                printf("root te_g ptr %lu\n", (long)&zx->root_canvas->gl_obj.te_g);
                //
                //                //if (x->g_pd->c_wb && x->g_pd->c_wb->w_visfn && gobj_shouldvis(x, glist))
                //                //    (*x->g_pd->c_wb->w_visfn)(x, glist, flag);
                //
                //                                printf("g_pd ptr %lu\n", (long)zx->canvas->gl_obj.te_g.g_pd);
                //                                printf("c_wb ptr %lu\n", (long)zx->canvas->gl_obj.te_g.g_pd->c_wb);
                //                                printf("vis_fn ptr %lu\n", (long)zx->canvas->gl_obj.te_g.g_pd->c_wb->w_visfn);
                //
                //                printf("root g_pd ptr %lu\n", (long)zx->root_canvas->gl_obj.te_g.g_pd);
                //                printf("root c_wb ptr %lu\n", (long)zx->root_canvas->gl_obj.te_g.g_pd->c_wb);
                //                printf("root vis_fn ptr %lu\n", (long)zx->root_canvas->gl_obj.te_g.g_pd->c_wb->w_visfn);
                
                //->gl_obj
                
                zx->canvas->gl_name = gensym("eCanvas");
                
                //                gobj_vis(&(zx->canvas->gl_gobj), zx->root_canvas, 0);
                //                gobj_vis(&(zx->canvas->gl_gobj), zx->root_canvas, 1);
                //                canvas_fixlinesfor(zx->root_canvas, &(zx->canvas->gl_obj));
                
                //gobj_vis(&(zx->canvas->gl_gobj), zx->root_canvas, 0);
                
                //gobj_vis(&(zx->canvas->gl_gobj), zx->root_canvas, 1);
                
                //                gobj_vis((zx->canvas->gl_gobj.g_next), zx->root_canvas, 1);
                //                canvas_fixlinesfor(zx->root_canvas, &(zx->canvas->gl_obj));
                
                
                
                //
                t_binbuf *b1 = binbuf_new();
                if (zx->canvas)
                    canvas_saveto(zx->canvas, b1);
                if (b1){
                    binbuf_print(b1);
                    binbuf_free(b1);}
                
                //                glist_delete(zx->root_canvas, &zx->canvas->gl_gobj);
                //                glist_add(zx->root_canvas, &zx->canvas->gl_gobj);
                //glist_redraw(zx->root_canvas);
                
                
                
                
                //                t_glist *x = zx->canvas;
                //
                //                printf("gl_gobj ptr %lu\n", (long)&x->gl_gobj);
                //
                //                if (x->gl_owner && !x->gl_loading && glist_isvisible(x->gl_owner))
                //                    gobj_vis(&x->gl_gobj, x->gl_owner, 0);
                //                x->gl_isgraph = 1;
                //                //x->gl_hidetext = !(!(flag&2));
                //                //x->gl_goprect = !nogoprect;
                //                if (glist_isvisible(x) && x->gl_goprect)
                //                    glist_redraw(x);
                //                if (x->gl_owner && !x->gl_loading && glist_isvisible(x->gl_owner))
                //                {
                //                    printf("setgraph 1\n");
                //                    gobj_vis(&x->gl_gobj, x->gl_owner, 1);
                //                    canvas_fixlinesfor(x->gl_owner, &x->gl_obj);
                //                }
                
                //(zx->root_canvas->gl_obj.te_g.g_pd->c_wb->w_visfn)(&zx->root_canvas->gl_obj.te_g, zx->root_canvas, 1);
                
                //canvas_fixlinesfor(zx->canvas->gl_owner, &zx->canvas->gl_obj);
                
                
                t_canvas *x = zx->canvas;
                
                if (x)
                {
                    canvas_dirty(x, 1);
                    if (x->gl_havewindow)
                        canvas_redraw(x);
                    else if (glist_isvisible(x->gl_owner))
                    {
                        gobj_vis(&x->gl_gobj, x->gl_owner, 0);
                        gobj_vis(&x->gl_gobj, x->gl_owner, 1);
                        canvas_fixlinesfor(zx->root_canvas, &(zx->canvas->gl_obj));
                    }
                }
            }
            
        }
        else
        {
            //canvas_vis(zx->canvas, 0);
            if (zx->canvas->gl_owner && !zx->canvas->gl_loading && glist_isvisible(zx->canvas->gl_owner) )
            {
                printf("\npaint 0\n");
                // gobj_vis(&zx->canvas->gl_gobj, zx->root_canvas, 0);
                
            }
            //            canvas_fixlinesfor(zx->canvas->gl_owner, &zx->canvas->gl_obj);
        }
        
        ebox_paint_layer((t_ebox *)z, bgl, 0., 0.);
        
    }
    
    //    UI_fun(eCanvas)::wx_mouseenter_ext(t_object *z, t_object *view, t_pt pt, long modifiers)
    //    {
    //        post("subcanvas");
    //
    //        eCanvas *zx = (eCanvas*)z;
    //
    //
    //        canvas_setcurrent(zx->canvas);
    //
    //        GuiFactory<eCanvas>::ws_redraw(z);
    //    }
    //
    //    UI_fun(eCanvas)::wx_mouseleave_ext(t_object *z, t_object *view, t_pt pt, long modifiers)
    //    {
    //        post("main canvas");
    //
    //        eCanvas *zx = (eCanvas*)z;
    //
    //        //zx->canvas->gl_edit = 0;
    //
    //        canvas_setcurrent(zx->root_canvas);
    //
    //        GuiFactory<eCanvas>::ws_redraw(z);
    //
    //    }
    
    
    
    UI_fun(eCanvas)::wx_mousedown_ext(eCanvas *z, t_object *view, t_pt pt, long modifiers)
    {
        printf("window\n");
        eCanvas *zx = (eCanvas*)z;
        
        if (!zx->canvas)
        {
            t_canvas *x = (t_canvas*)subcanvas_new(gensym("eCanvas"));
            
            x->gl_owner = zx->root_canvas;
            if (!x->gl_obj.te_binbuf) x->gl_obj.te_binbuf = binbuf_new();
            
            ecanvas_d(x);
            
            zx->canvas = x;
            
        }
        canvas_vis(zx->canvas,1);
    }
    
    
    UI_fun(eCanvas)::new_ext(eCanvas *zx, t_symbol *s, int argc, t_atom *argv)
    {   
        t_canvas *zz = canvas_getcurrent();
        zx->root_canvas = zz;
        
        
        
        
        //if (!zx->root_canvas->gl_loading)
        {
            t_canvas *x = (t_canvas*)subcanvas_new(gensym("eCanvas"));
            
            x->gl_owner = zx->root_canvas;
            if (!x->gl_obj.te_binbuf) x->gl_obj.te_binbuf = binbuf_new();
            
            ecanvas_d(x);
            
            zx->canvas = x;
            
        }
    }
    
    UI_fun(eCanvas)::wx_oksize(eCanvas *zx, t_rect *newrect)
    {
        if (zx->canvas)
        {
            zx->canvas->gl_pixwidth = newrect->width;
            zx->canvas->gl_pixheight = newrect->height;
        }
        
    }
    
    UI_fun(eCanvas)::init_ext(t_eclass *z)
    {
        CLASS_ATTR_DEFAULT (z, "size", 0, "200. 200.");
        
        
    }
    
}

extern "C" void setup_ui0x2eecanvas()
{
    GuiFactory<eCanvas> class1;
    class1.setup("ui.ecanvas");
}
