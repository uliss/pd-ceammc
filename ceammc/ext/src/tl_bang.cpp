//
//  tl_bang.cpp
//  pd_ext
//
//  Created by Alex Nadzharov on 10/12/16.
//
//

//#include <stdio.h>

#include "ceammc.h"
#include "tl_lib.hpp"

#include "g_all_guis.h"

#include "ceammc_gui.hpp"

//GUI copied from pd source

int tlbang_iemgui_color_hex[]=
{
    16579836, 10526880, 4210752, 16572640, 16572608,
    16579784, 14220504, 14220540, 14476540, 16308476,
    14737632, 8158332, 2105376, 16525352, 16559172,
    15263784, 1370132, 2684148, 3952892, 16003312,
    12369084, 6316128, 0, 9177096, 5779456,
    7874580, 2641940, 17488, 5256, 5767248
};


t_widgetbehavior tl_bang_widgetbehavior;
t_class* tl_bang_class;

struct tl_bang_object {
    t_iemgui x_gui;
    
    t_outlet *out1;
    
};


#pragma mark widgetbehaviour

static void tl_bang_getrect(t_gobj *z, t_glist *glist, int *xp1, int *yp1, int *xp2, int *yp2)
{
    tl_bang_object *x = (tl_bang_object *)z;
    
    *xp1 = text_xpix(&x->x_gui.x_obj, glist);
    *yp1 = text_ypix(&x->x_gui.x_obj, glist);
    *xp2 = *xp1 + x->x_gui.x_w;
    *yp2 = *yp1 + x->x_gui.x_h;
}

static int tl_bang_newclick(t_gobj *z, struct _glist *glist, int xpix, int ypix, int shift, int alt, int dbl, int doit)
{
    //    if(doit)
    //        tl_bang_click((tl_bang_object *)z, (t_floatarg)xpix, (t_floatarg)ypix, (t_floatarg)shift, 0, (t_floatarg)alt);
    return (1);
}

#pragma mark plus

void tl_bang_displace(t_gobj *z, t_glist *glist, int dx, int dy)
{
    t_iemgui *x = &((tl_bang_object*)z)->x_gui;//(t_iemgui *)z;
    
    x->x_obj.te_xpix += dx;
    x->x_obj.te_ypix += dy;
    (*x->x_draw)((void *)z, glist, IEM_GUI_DRAW_MODE_MOVE);
    canvas_fixlinesfor(glist, (t_text *)z);
}

void tl_bang_select(t_gobj *z, t_glist *glist, int selected)
{
    t_iemgui *x = &((tl_bang_object*)z)->x_gui;//(t_iemgui *)z;    ////
    
    x->x_fsf.x_selected = selected;
    (*x->x_draw)((void *)z, glist, IEM_GUI_DRAW_MODE_SELECT);
}

void tl_bang_delete(t_gobj *z, t_glist *glist)
{
    canvas_deletelinesfor(glist, (t_text *)z);
}

void tl_bang_vis(t_gobj *z, t_glist *glist, int vis)
{
    t_iemgui *x = &((tl_bang_object*)z)->x_gui;//(t_iemgui *)z;
    
    printf ("tl_bang_vis %lu | %lu \n", (long)x->x_draw, (long)x);
    
    if (vis)
        (*x->x_draw)((void *)z, glist, IEM_GUI_DRAW_MODE_NEW);
    else
    {
        (*x->x_draw)((void *)z, glist, IEM_GUI_DRAW_MODE_ERASE);
        sys_unqueuegui(z);
    }
}

#pragma mark more iemgui copied here:
//iemgui
t_symbol *tlbang_new_dogetname(t_iemgui *iemgui, int indx, t_atom *argv)
{
    if (IS_A_SYMBOL(argv, indx))
        return (atom_getsymbolarg(indx, 100000, argv));
    else if (IS_A_FLOAT(argv, indx))
    {
        char str[80];
        sprintf(str, "%d", (int)atom_getintarg(indx, 100000, argv));
        return (gensym(str));
    }
    else return (gensym("empty"));
}

void tlbang_new_getnames(t_iemgui *iemgui, int indx, t_atom *argv)
{
    if (argv)
    {
        iemgui->x_snd = tlbang_new_dogetname(iemgui, indx, argv);
        iemgui->x_rcv = tlbang_new_dogetname(iemgui, indx+1, argv);
        iemgui->x_lab = tlbang_new_dogetname(iemgui, indx+2, argv);
    }
    else iemgui->x_snd = iemgui->x_rcv = iemgui->x_lab = gensym("empty");
    iemgui->x_snd_unexpanded = iemgui->x_rcv_unexpanded =
    iemgui->x_lab_unexpanded = 0;
    iemgui->x_binbufindex = indx;
    iemgui->x_labelbindex = indx + 3;
}

/* inform GUIs that glist's zoom is about to change.  The glist will
 take care of x,y locations but we have to adjust width and height */
void tlbang_zoom(t_iemgui *iemgui, t_floatarg zoom)
{
    int oldzoom = iemgui->x_glist->gl_zoom;
    if (oldzoom < 1)
        oldzoom = 1;
    iemgui->x_w = (int)(iemgui->x_w)/oldzoom*(int)zoom;
    iemgui->x_h = (int)(iemgui->x_h)/oldzoom*(int)zoom;
}

/* when creating a new GUI from menu onto a zoomed canvas, pretend to
 change the canvas's zoom so we'll get properly sized */
void tlbang_newzoom(t_iemgui *iemgui)
{
    if (iemgui->x_glist->gl_zoom != 1)
    {
        int newzoom = iemgui->x_glist->gl_zoom;
        iemgui->x_glist->gl_zoom = 1;
        tlbang_zoom(iemgui, (t_floatarg)newzoom);
        iemgui->x_glist->gl_zoom = newzoom;
    }
}

//iem

#define LOADINIT 1
#define SCALE 2
#define SCALEBIS (1<<20)

void tlbang_inttosymargs(t_iem_init_symargs *symargp, int n)
{
    memset(symargp, 0, sizeof(*symargp));
    symargp->x_loadinit = ((n & LOADINIT) != 0);
    symargp->x_scale = ((n & SCALE) || (n & SCALEBIS)) ;
    symargp->x_flashed = 0;
    symargp->x_locked = 0;
}



void tlbang_inttofstyle(t_iem_fstyle_flags *fstylep, int n)
{
    memset(fstylep, 0, sizeof(*fstylep));
    fstylep->x_font_style = (n >> 0);
    fstylep->x_shiftdown = 0;
    fstylep->x_selected = 0;
    fstylep->x_finemoved = 0;
    fstylep->x_put_in2out = 0;
    fstylep->x_change = 0;
    fstylep->x_thick = 0;
    fstylep->x_lin0_log1 = 0;
    fstylep->x_steady = 0;
}

static int tl_bang_getcolorarg(int index, int argc, t_atom*argv)
{
    if(index < 0 || index >= argc)
        return 0;
    if(IS_A_FLOAT(argv,index))
        return atom_getintarg(index, argc, argv);
    if(IS_A_SYMBOL(argv,index))
    {
        t_symbol*s=atom_getsymbolarg(index, argc, argv);
        if ('#' == s->s_name[0])
            return strtol(s->s_name+1, 0, 16);
    }
    return 0;
}

int tlbang_iemgui_modulo_color(int col)
{
    while(col >= IEM_GUI_MAX_COLOR)
        col -= IEM_GUI_MAX_COLOR;
    while(col < 0)
        col += IEM_GUI_MAX_COLOR;
    return(col);
}

static int tl_bang_colfromatomload(t_atom*colatom)
{
    int color;
    /* old-fashioned color arguement, either a number or symbol
     evaluating to an integer */
    if (colatom->a_type == A_FLOAT)
        color = atom_getint(colatom);
    else if (colatom->a_type == A_SYMBOL &&
             (isdigit(colatom->a_w.w_symbol->s_name[0]) ||
              colatom->a_w.w_symbol->s_name[0] == '-'))
        color = atoi(colatom->a_w.w_symbol->s_name);
    
    /* symbolic color */
    else return (tl_bang_getcolorarg(0, 1, colatom));
    if (color < 0)
    {
        color = -1 - color;
        color = ((color & 0x3f000) << 6)|((color & 0xfc0) << 4)|
        ((color & 0x3f) << 2);
    }
    else
    {
        color = tlbang_iemgui_modulo_color(color);
        color = tlbang_iemgui_color_hex[color];
    }
    return (color);
}

void tlbang_all_loadcolors(t_iemgui *iemgui, t_atom*bcol, t_atom*fcol, t_atom*lcol)
{
    if(bcol)iemgui->x_bcol = tl_bang_colfromatomload(bcol);
    if(fcol)iemgui->x_fcol = tl_bang_colfromatomload(fcol);
    if(lcol)iemgui->x_lcol = tl_bang_colfromatomload(lcol);
}
#pragma mark draw

void tl_bang_draw_update(tl_bang_object *x, t_glist *glist)
{
    //    if(glist_isvisible(glist))
    //    {
    //        t_canvas *canvas=glist_getcanvas(glist);
    //
    //    }
}

void tl_bang_draw_new(tl_bang_object *x, t_glist *glist)
{
    t_canvas *canvas=glist_getcanvas(glist);
    int w=1, xx=text_xpix(&x->x_gui.x_obj, glist), yy=text_ypix(&x->x_gui.x_obj, glist);
    
    tll_ui_add((t_object*)&x->x_gui.x_obj,x->x_gui.x_obj.te_xpix);
    
    int stroke_width = 1;
    int zoomlabel =
    1 + (IEMGUI_ZOOM(x)-1) * (x->x_gui.x_ldx >= 0 && x->x_gui.x_ldy >= 0);
    if(x->x_gui.x_w >= 30)
        w = 2;
    if(x->x_gui.x_w >= 60)
        w = 3;
    
    sys_vgui(".x%lx.c create rectangle %d %d %d %d -width %d "
             "-outline %s "
             "-fill #%06x -tags %lxBASE\n",
             canvas, xx, yy, xx + x->x_gui.x_w, yy + x->x_gui.x_h,
             stroke_width,
             "#00C0FF",
             x->x_gui.x_bcol, x);
//    
//    sys_vgui(".x%lx.c create rectangle %d %d %d %d -width %d "
//             "-outline %s "
//             "-fill #%06x -tags %lxBASE2\n",
//             canvas, xx, 0, xx + 1, 1000,
//             stroke_width,
//             "#00C0FF",
//             x->x_gui.x_bcol, x);
    
    sys_vgui(".x%lx.c create text %d %d -text {%s} -anchor w \
             -font {{%s} -%d %s} -fill #%06x -tags [list %lxLABEL label text]\n",
             canvas, xx+x->x_gui.x_ldx * zoomlabel,
             yy+x->x_gui.x_ldy * zoomlabel,
             "tl.bang",
             x->x_gui.x_font, x->x_gui.x_fontsize, sys_fontweight,
             x->x_gui.x_lcol, x);
    
    sys_vgui(".x%lx.c create rectangle %d %d %d %d -fill #FFFFFF -outline %s -tags [list %lxOUT%d outlet]\n",
             canvas, xx, yy + x->x_gui.x_h - 2,
             xx + IOWIDTH, yy + x->x_gui.x_h, "#000000", x, 0); //
    
}

void tl_bang_draw_move(tl_bang_object *x, t_glist *glist)
{
    t_canvas *canvas=glist_getcanvas(glist);
    int w=1, xx=text_xpix(&x->x_gui.x_obj, glist);
    
    float x_pos = xx;
    tll_ui_update_pos((t_object *)x, x_pos);
    
    int yy =text_ypix(&x->x_gui.x_obj, glist);
    int zoomlabel =
    1 + (IEMGUI_ZOOM(x)-1) * (x->x_gui.x_ldx >= 0 && x->x_gui.x_ldy >= 0);
    
    if(x->x_gui.x_w >= 30)
        w = 2;
    
    if(x->x_gui.x_w >= 60)
        w = 3;
    sys_vgui(".x%lx.c coords %lxBASE %d %d %d %d\n",
             canvas, x, xx, yy, xx + x->x_gui.x_w, yy + x->x_gui.x_h);
//    sys_vgui(".x%lx.c coords %lxBASE2 %d %d %d %d\n",
//             canvas, x, xx, 0, xx +1, 1000);
    sys_vgui(".x%lx.c itemconfigure %lxX1 -width %d\n", canvas, x, w);
    sys_vgui(".x%lx.c coords %lxX1 %d %d %d %d\n",
             canvas, x, xx+w+1, yy+w+1, xx + x->x_gui.x_w-w, yy + x->x_gui.x_h-w);
    sys_vgui(".x%lx.c itemconfigure %lxX2 -width %d\n", canvas, x, w);
    sys_vgui(".x%lx.c coords %lxX2 %d %d %d %d\n",
             canvas, x, xx+w+1, yy + x->x_gui.x_h-w-1, xx + x->x_gui.x_w-w, yy+w);
    sys_vgui(".x%lx.c coords %lxLABEL %d %d\n",
             canvas, x, xx+x->x_gui.x_ldx * zoomlabel,
             yy+x->x_gui.x_ldy * zoomlabel);
    //    if(!x->x_gui.x_fsf.x_snd_able)
            sys_vgui(".x%lx.c coords %lxOUT%d %d %d %d %d\n",
                     canvas, x, 0, xx, yy + x->x_gui.x_h - 2, xx + IOWIDTH, yy + x->x_gui.x_h); //2 = xlet_height(x)
    //    if(!x->x_gui.x_fsf.x_rcv_able)
    //        sys_vgui(".x%lx.c coords %lxIN%d %d %d %d %d\n",
    //                 canvas, x, 0, xx, yy, xx + IOWIDTH, yy + xlet_height(x));
}

void tl_bang_draw_erase(tl_bang_object* x, t_glist* glist)
{
    t_canvas *canvas=glist_getcanvas(glist);
    
    sys_vgui(".x%lx.c delete %lxBASE\n", canvas, x);
//    sys_vgui(".x%lx.c delete %lxBASE2\n", canvas, x);
    sys_vgui(".x%lx.c delete %lxX1\n", canvas, x);
    sys_vgui(".x%lx.c delete %lxX2\n", canvas, x);
    sys_vgui(".x%lx.c delete %lxLABEL\n", canvas, x);
    if(!x->x_gui.x_fsf.x_snd_able)
        sys_vgui(".x%lx.c delete %lxOUT%d\n", canvas, x, 0);
    if(!x->x_gui.x_fsf.x_rcv_able)
        sys_vgui(".x%lx.c delete %lxIN%d\n", canvas, x, 0);
}

void tl_bang_draw_config(tl_bang_object* x, t_glist* glist)
{
    t_canvas *canvas=glist_getcanvas(glist);
    
    sys_vgui(".x%lx.c itemconfigure %lxLABEL -font {{%s} -%d %s} -fill #%06x -text {%s} \n",
             canvas, x, x->x_gui.x_font, x->x_gui.x_fontsize, sys_fontweight,
             x->x_gui.x_fsf.x_selected?IEM_GUI_COLOR_SELECTED:x->x_gui.x_lcol,
             strcmp(x->x_gui.x_lab->s_name, "empty")?x->x_gui.x_lab->s_name:"");
    sys_vgui(".x%lx.c itemconfigure %lxBASE -fill #%06x\n", canvas, x,
             x->x_gui.x_bcol);
    //    sys_vgui(".x%lx.c itemconfigure %lxX1 -fill #%06x\n", canvas, x,
    //             x->x_on?x->x_gui.x_fcol:x->x_gui.x_bcol);
    //    sys_vgui(".x%lx.c itemconfigure %lxX2 -fill #%06x\n", canvas, x,
    //             x->x_on?x->x_gui.x_fcol:x->x_gui.x_bcol);
}

void tl_bang_draw_io(tl_bang_object* x, t_glist* glist, int old_snd_rcv_flags)
{
    int xpos=text_xpix(&x->x_gui.x_obj, glist);
    int ypos=text_ypix(&x->x_gui.x_obj, glist);
    t_canvas *canvas=glist_getcanvas(glist);
    
    //if((old_snd_rcv_flags & IEM_GUI_OLD_SND_FLAG) && !x->x_gui.x_fsf.x_snd_able)
        sys_vgui(".x%lx.c create rectangle %d %d %d %d -fill %s -tags %lxOUT%d\n",
                 canvas, xpos,
                 ypos + x->x_gui.x_h-1, xpos + IOWIDTH,
                 ypos + x->x_gui.x_h, "#FF00FF", x, 0);
    
    printf("draw io\n");
    
    if(!(old_snd_rcv_flags & IEM_GUI_OLD_SND_FLAG) && x->x_gui.x_fsf.x_snd_able)
        sys_vgui(".x%lx.c delete %lxOUT%d\n", canvas, x, 0);
    if((old_snd_rcv_flags & IEM_GUI_OLD_RCV_FLAG) && !x->x_gui.x_fsf.x_rcv_able)
        sys_vgui(".x%lx.c create rectangle %d %d %d %d -fill %s -tags %lxIN%d\n",
                 canvas, xpos, ypos,
                 xpos + IOWIDTH, ypos+1, "#FFFF00", x, 0);
    if(!(old_snd_rcv_flags & IEM_GUI_OLD_RCV_FLAG) && x->x_gui.x_fsf.x_rcv_able)
        sys_vgui(".x%lx.c delete %lxIN%d\n", canvas, x, 0);
}

void tl_bang_draw_select(tl_bang_object* x, t_glist* glist)
{
    t_canvas *canvas=glist_getcanvas(glist);
    
    if(x->x_gui.x_fsf.x_selected)
    {
        sys_vgui(".x%lx.c itemconfigure %lxBASE -outline %s\n", canvas, x, "#0000FF");
        sys_vgui(".x%lx.c itemconfigure %lxLABEL -fill %s\n", canvas, x, "#0000FF");
    }
    else
    {
        sys_vgui(".x%lx.c itemconfigure %lxBASE -outline %s\n", canvas, x, "#00C0FF");
        sys_vgui(".x%lx.c itemconfigure %lxLABEL -fill #%06x\n", canvas, x, x->x_gui.x_lcol);
    }
}

void tl_bang_draw(tl_bang_object *x, t_glist *glist, int mode)
{
    printf("cue draw");
    
    if(mode == IEM_GUI_DRAW_MODE_UPDATE)
        tl_bang_draw_update(x, glist);
    else if(mode == IEM_GUI_DRAW_MODE_MOVE)
        tl_bang_draw_move(x, glist);
    else if(mode == IEM_GUI_DRAW_MODE_NEW)
        tl_bang_draw_new(x, glist);
    else if(mode == IEM_GUI_DRAW_MODE_SELECT)
        tl_bang_draw_select(x, glist);
    else if(mode == IEM_GUI_DRAW_MODE_ERASE)
        tl_bang_draw_erase(x, glist);
    else if(mode == IEM_GUI_DRAW_MODE_CONFIG)
        tl_bang_draw_config(x, glist);
    else if(mode >= IEM_GUI_DRAW_MODE_IO)
        tl_bang_draw_io(x, glist, mode - IEM_GUI_DRAW_MODE_IO);
}

#pragma mark save

static void tl_bang_init_sym2dollararg(t_iemgui *iemgui, t_symbol **symp,
                                      int indx, t_symbol *fallback)
{
    printf("initsym2dollararg\n");
    if (!*symp)
    {
        //        t_binbuf *b = iemgui->x_obj.ob_binbuf;
        //        if (binbuf_getnatom(b) > indx)
        //        {
        //            char buf[80];
        //            atom_string(binbuf_getvec(b) + indx, buf, 80);
        //            *symp = gensym(buf);
        //        }
        //        else if (fallback)
        //            *symp = fallback;
        //        else
        *symp = gensym("empty");
    }
}

void tl_bang_all_sym2dollararg(t_iemgui *iemgui, t_symbol **srlsym)
{
    printf("sym2dollararg\n");
    tl_bang_init_sym2dollararg(iemgui, &iemgui->x_snd_unexpanded,
                              iemgui->x_binbufindex+1, iemgui->x_snd);
    tl_bang_init_sym2dollararg(iemgui, &iemgui->x_rcv_unexpanded,
                              iemgui->x_binbufindex+2, iemgui->x_rcv);
    tl_bang_init_sym2dollararg(iemgui, &iemgui->x_lab_unexpanded,
                              iemgui->x_labelbindex, iemgui->x_lab);
    srlsym[0] = iemgui->x_snd_unexpanded;
    srlsym[1] = iemgui->x_rcv_unexpanded;
    srlsym[2] = iemgui->x_lab_unexpanded;
}

static t_symbol* tl_color2symbol(int col) {
    printf("color2symbol\n");
    const int  compat = (pd_compatibilitylevel < 48)?1:
    /* FIXXME: for Pd>=0.48, the default compatibility mode should be OFF */
    1;
    
    char colname[MAXPDSTRING];
    colname[0] = colname[MAXPDSTRING-1] = 0;
    
    if (compat)
    {
        /* compatibility with Pd<=0.47: saves colors as numbers with limited resolution */
        int col2 = -1 - (((0xfc0000 & col) >> 6)|((0xfc00 & col) >> 4)|((0xfc & col) >> 2));
        snprintf(colname, MAXPDSTRING-1, "%d", col2);
    } else {
        snprintf(colname, MAXPDSTRING-1, "#%06x", col);
    }
    return gensym(colname);
}

void tl_bang_all_col2save(t_iemgui *iemgui, t_symbol**bflcol)
{
    printf("col2save\n");
    bflcol[0] = tl_color2symbol(iemgui->x_bcol);
    bflcol[1] = tl_color2symbol(iemgui->x_fcol);
    bflcol[2] = tl_color2symbol(iemgui->x_lcol);
}

void tl_bang_gui_save(t_iemgui *iemgui, t_symbol **srl, t_symbol**bflcol)
{
    srl[0] = iemgui->x_snd;
    srl[1] = iemgui->x_rcv;
    srl[2] = iemgui->x_lab;
    tl_bang_all_sym2dollararg(iemgui, srl);  //iemgui_all_sym2dollararg
    tl_bang_all_col2save(iemgui, bflcol);
}

int tl_bang_symargstoint(t_iem_init_symargs *symargp)
{
    return ((symargp->x_loadinit ? LOADINIT : 0) |
            (symargp->x_scale ? (SCALE | SCALEBIS) : 0));
}

int tl_bang_fstyletoint(t_iem_fstyle_flags *fstylep)
{
    return ((fstylep->x_font_style << 0) & 63);
}

#pragma mark _

static void tl_bang_save(t_gobj *z, t_binbuf *b)
{
    tl_bang_object *x = (tl_bang_object *)z;//&((tl_bang_object*)z)->x_gui;//(t_toggle *)z;
    t_symbol *bflcol[3];
    t_symbol *srl[3];
    
    printf ("gui ptr %lu\n", (long)x);
    
    tl_bang_gui_save(&x->x_gui, srl, bflcol);
    printf("gui save\n");
    binbuf_addv(b, "ssiisiisssiiiisss", gensym("#X"),gensym("obj"),
                (int)x->x_gui.x_obj.te_xpix,
                (int)x->x_gui.x_obj.te_ypix,
                gensym("tl.bang"), x->x_gui.x_w,
                tl_bang_symargstoint(&x->x_gui.x_isa),
                srl[0], srl[1], srl[2],
                x->x_gui.x_ldx, x->x_gui.x_ldy,
                tl_bang_fstyletoint(&x->x_gui.x_fsf), x->x_gui.x_fontsize,
                bflcol[0], bflcol[1], bflcol[2]);
    printf("addv\n");
    binbuf_addv(b, ";");
}
//iem_symargstoint
#pragma mark -

void tl_bang_update_pos(tl_bang_object *x)
{
    float x_pos = x->x_gui.x_obj.te_xpix;
    
}

#pragma mark -
void tl_bang_action(t_object *x) //, tl_t_list list)
{
    printf("bang action");
    outlet_bang(((tl_bang_object*)x)->out1);
}

#pragma mark -

void* tl_bang_new(t_symbol *s, int argc, t_atom *argv)
{
    tl_bang_object* x = reinterpret_cast<tl_bang_object*>(pd_new(tl_bang_class));
    
    printf ("tl cue init\n");

    //
    //GUI
    
    int ldx=5, ldy=7;
    int fs=10;
    
    printf ("tl cue gui init\n");
    
    tlbang_inttosymargs(&x->x_gui.x_isa, 0);
    tlbang_inttofstyle(&x->x_gui.x_fsf, 0);
    
    x->x_gui.x_snd = gensym("tl_bang");
    x->x_gui.x_rcv = gensym("");
    x->x_gui.x_lab = gensym("");
    
    printf ("1\n");
    
    x->x_gui.x_bcol = 0xC0C0C0;
    x->x_gui.x_fcol = 0x00;
    x->x_gui.x_lcol = 0x00;
    
    int a=0;
    //restore data
    
    printf("argc %i\n",argc);
    
    if(((argc == 12)||(argc == 14))&&IS_A_FLOAT(argv,0)
       &&IS_A_FLOAT(argv,1)
       &&(IS_A_SYMBOL(argv,2)||IS_A_FLOAT(argv,2))
       &&(IS_A_SYMBOL(argv,3)||IS_A_FLOAT(argv,3))
       &&(IS_A_SYMBOL(argv,4)||IS_A_FLOAT(argv,4))
       &&IS_A_FLOAT(argv,5)&&IS_A_FLOAT(argv,6)
       &&IS_A_FLOAT(argv,7)&&IS_A_FLOAT(argv,8))
    {
        printf("loading obj\n");
        a = (int)atom_getintarg(0, argc, argv);
        tlbang_inttosymargs(&x->x_gui.x_isa, atom_getintarg(1, argc, argv));
        tlbang_new_getnames(&x->x_gui, 2, argv);
        ldx = (int)atom_getintarg(5, argc, argv);
        ldy = (int)atom_getintarg(6, argc, argv);
        tlbang_inttofstyle(&x->x_gui.x_fsf, atom_getintarg(7, argc, argv));
        fs = (int)atom_getintarg(8, argc, argv);
        tlbang_all_loadcolors(&x->x_gui, argv+9, argv+10, argv+11);
        
    }
    else
    {
        tlbang_new_getnames(&x->x_gui, 2, 0);
    }
    
    printf ("2\n");
    
    
    x->x_gui.x_draw = (t_iemfunptr)&tl_bang_draw;
    
    x->x_gui.x_fsf.x_snd_able = 1;
    x->x_gui.x_fsf.x_rcv_able = 1;
    x->x_gui.x_glist = (t_glist *)canvas_getcurrent();
//    if (!strcmp(x->x_gui.x_snd->s_name, "empty"))
//        x->x_gui.x_fsf.x_snd_able = 0;
    if (!strcmp(x->x_gui.x_rcv->s_name, "empty"))
        x->x_gui.x_fsf.x_rcv_able = 0;
    if(x->x_gui.x_fsf.x_font_style == 1) strcpy(x->x_gui.x_font, "helvetica");
    else if(x->x_gui.x_fsf.x_font_style == 2) strcpy(x->x_gui.x_font, "times");
    else { x->x_gui.x_fsf.x_font_style = 0;
        strcpy(x->x_gui.x_font, sys_font); }
    
    if (x->x_gui.x_fsf.x_rcv_able)
        pd_bind(&x->x_gui.x_obj.ob_pd, x->x_gui.x_rcv);
    x->x_gui.x_ldx = ldx;
    x->x_gui.x_ldy = ldy;
    
    if(fs < 4)
        fs = 4;
    x->x_gui.x_fontsize = fs;
    x->x_gui.x_w = 50;
    x->x_gui.x_h = 15;
    tlbang_newzoom(&x->x_gui);
    //    iemgui_verify_snd_ne_rcv(&x->x_gui);
    
    
    printf ("3. pointers x xgui %lu %lu\n", (long)x, (long)&x->x_gui);
    
    
    
    x->out1 = outlet_new(&x->x_gui.x_obj, &s_bang);
    
    return static_cast<void*>(x);
}

static void tl_bang_free(tl_bang_object *x)
{
    if(x->x_gui.x_fsf.x_rcv_able)
        pd_unbind(&x->x_gui.x_obj.ob_pd, x->x_gui.x_rcv);
    gfxstub_deleteforkey(x);
}
extern "C" void setup_tl0x2ebang()
{
    printf ("tlbang setup\n");
    
    tl_bang_class = class_new(gensym("tl.bang"),
                             reinterpret_cast<t_newmethod>(tl_bang_new),
                             reinterpret_cast<t_method>(tl_bang_free),
                             sizeof(tl_bang_object), 0, A_GIMME,0);
    
    //tll_set_ui_drawcue(reinterpret_cast<tl_ui_drawcue>(tl_bang_update_pos));
    
    tll_set_ui_action(reinterpret_cast<tl_ui_action>(&tl_bang_action));
    
    printf("*bang action ptr %lu", (long)&tl_bang_action);
    
    //GUI
    tl_bang_widgetbehavior.w_getrectfn = tl_bang_getrect;
    tl_bang_widgetbehavior.w_displacefn = tl_bang_displace;
    tl_bang_widgetbehavior.w_selectfn = tl_bang_select;
    tl_bang_widgetbehavior.w_activatefn = NULL;
    tl_bang_widgetbehavior.w_deletefn = tl_bang_delete;
    tl_bang_widgetbehavior.w_visfn = tl_bang_vis;
    tl_bang_widgetbehavior.w_clickfn = tl_bang_newclick;
    class_setwidget(tl_bang_class, &tl_bang_widgetbehavior);
    
    class_setsavefn(tl_bang_class, tl_bang_save);
    
}
