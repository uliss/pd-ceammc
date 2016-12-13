////
////  tl_cue.cpp
////  pd_ext
////
////  Created by Alex Nadzharov on 10/12/16.
////
////
//




#include "tl_cue2.hpp"





//};
//
//
//#pragma mark widgetbehaviour
//
//static void tl_cue_getrect(t_gobj *z, t_glist *glist, int *xp1, int *yp1, int *xp2, int *yp2)
//{
//    tl_cue_object *x = (tl_cue_object *)z;
//
//    *xp1 = text_xpix(&x->x_gui.x_obj, glist);
//    *yp1 = text_ypix(&x->x_gui.x_obj, glist);
//    *xp2 = *xp1 + x->x_gui.x_w;
//    *yp2 = *yp1 + x->x_gui.x_h;
//}
//
//static int tl_cue_newclick(t_gobj *z, struct _glist *glist, int xpix, int ypix, int shift, int alt, int dbl, int doit)
//{
////    if(doit)
////        tl_cue_click((tl_cue_object *)z, (t_floatarg)xpix, (t_floatarg)ypix, (t_floatarg)shift, 0, (t_floatarg)alt);
//    return (1);
//}
//
//#pragma mark plus
//
//void tl_cue_displace(t_gobj *z, t_glist *glist, int dx, int dy)
//{
//    t_iemgui *x = &((tl_cue_object*)z)->x_gui;//(t_iemgui *)z;
//
//    x->x_obj.te_xpix += dx;
//    x->x_obj.te_ypix += dy;
//    (*x->x_draw)((void *)z, glist, IEM_GUI_DRAW_MODE_MOVE);
//    canvas_fixlinesfor(glist, (t_text *)z);
//}
//
//void tl_cue_select(t_gobj *z, t_glist *glist, int selected)
//{
//    t_iemgui *x = &((tl_cue_object*)z)->x_gui;//(t_iemgui *)z;    ////
//
//    x->x_fsf.x_selected = selected;
//    (*x->x_draw)((void *)z, glist, IEM_GUI_DRAW_MODE_SELECT);
//}
//
//void tl_cue_delete(t_gobj *z, t_glist *glist)
//{
//    canvas_deletelinesfor(glist, (t_text *)z);
//}
//
//void tl_cue_vis(t_gobj *z, t_glist *glist, int vis)
//{
//    t_iemgui *x = &((tl_cue_object*)z)->x_gui;//(t_iemgui *)z;
//
//    printf ("tl_cue_vis %lu | %lu \n", (long)x->x_draw, (long)x);
//
//    if (vis)
//        (*x->x_draw)((void *)z, glist, IEM_GUI_DRAW_MODE_NEW);
//    else
//    {
//        (*x->x_draw)((void *)z, glist, IEM_GUI_DRAW_MODE_ERASE);
//        sys_unqueuegui(z);
//    }
//}
//
//#pragma mark more iemgui copied here:
////iemgui
//t_symbol *tlcue_new_dogetname(t_iemgui *iemgui, int indx, t_atom *argv)
//{
//    if (IS_A_SYMBOL(argv, indx))
//        return (atom_getsymbolarg(indx, 100000, argv));
//    else if (IS_A_FLOAT(argv, indx))
//    {
//        char str[80];
//        sprintf(str, "%d", (int)atom_getintarg(indx, 100000, argv));
//        return (gensym(str));
//    }
//    else return (gensym("empty"));
//}
//
//void tlcue_new_getnames(t_iemgui *iemgui, int indx, t_atom *argv)
//{
//    if (argv)
//    {
//        iemgui->x_snd = tlcue_new_dogetname(iemgui, indx, argv);
//        iemgui->x_rcv = tlcue_new_dogetname(iemgui, indx+1, argv);
//        iemgui->x_lab = tlcue_new_dogetname(iemgui, indx+2, argv);
//    }
//    else iemgui->x_snd = iemgui->x_rcv = iemgui->x_lab = gensym("empty");
//    iemgui->x_snd_unexpanded = iemgui->x_rcv_unexpanded =
//    iemgui->x_lab_unexpanded = 0;
//    iemgui->x_binbufindex = indx;
//    iemgui->x_labelbindex = indx + 3;
//}
//
///* inform GUIs that glist's zoom is about to change.  The glist will
// take care of x,y locations but we have to adjust width and height */
//void tlcue_zoom(t_iemgui *iemgui, t_floatarg zoom)
//{
//    int oldzoom = iemgui->x_glist->gl_zoom;
//    if (oldzoom < 1)
//        oldzoom = 1;
//    iemgui->x_w = (int)(iemgui->x_w)/oldzoom*(int)zoom;
//    iemgui->x_h = (int)(iemgui->x_h)/oldzoom*(int)zoom;
//}
//
///* when creating a new GUI from menu onto a zoomed canvas, pretend to
// change the canvas's zoom so we'll get properly sized */
//void tlcue_newzoom(t_iemgui *iemgui)
//{
//    if (iemgui->x_glist->gl_zoom != 1)
//    {
//        int newzoom = iemgui->x_glist->gl_zoom;
//        iemgui->x_glist->gl_zoom = 1;
//        tlcue_zoom(iemgui, (t_floatarg)newzoom);
//        iemgui->x_glist->gl_zoom = newzoom;
//    }
//}
//
////iem
//
//#define LOADINIT 1
//#define SCALE 2
//#define SCALEBIS (1<<20)
//
//void tlcue_inttosymargs(t_iem_init_symargs *symargp, int n)
//{
//    memset(symargp, 0, sizeof(*symargp));
//    symargp->x_loadinit = ((n & LOADINIT) != 0);
//    symargp->x_scale = ((n & SCALE) || (n & SCALEBIS)) ;
//    symargp->x_flashed = 0;
//    symargp->x_locked = 0;
//}
//
//
//
//void tlcue_inttofstyle(t_iem_fstyle_flags *fstylep, int n)
//{
//    memset(fstylep, 0, sizeof(*fstylep));
//    fstylep->x_font_style = (n >> 0);
//    fstylep->x_shiftdown = 0;
//    fstylep->x_selected = 0;
//    fstylep->x_finemoved = 0;
//    fstylep->x_put_in2out = 0;
//    fstylep->x_change = 0;
//    fstylep->x_thick = 0;
//    fstylep->x_lin0_log1 = 0;
//    fstylep->x_steady = 0;
//}
//
//static int tl_cue_getcolorarg(int index, int argc, t_atom*argv)
//{
//    if(index < 0 || index >= argc)
//        return 0;
//    if(IS_A_FLOAT(argv,index))
//        return atom_getintarg(index, argc, argv);
//    if(IS_A_SYMBOL(argv,index))
//    {
//        t_symbol*s=atom_getsymbolarg(index, argc, argv);
//        if ('#' == s->s_name[0])
//            return strtol(s->s_name+1, 0, 16);
//    }
//    return 0;
//}
//
//int tlcue_iemgui_modulo_color(int col)
//{
//    while(col >= IEM_GUI_MAX_COLOR)
//        col -= IEM_GUI_MAX_COLOR;
//    while(col < 0)
//        col += IEM_GUI_MAX_COLOR;
//    return(col);
//}
//
//static int tl_cue_colfromatomload(t_atom*colatom)
//{
//    int color;
//    /* old-fashioned color arguement, either a number or symbol
//     evaluating to an integer */
//    if (colatom->a_type == A_FLOAT)
//        color = atom_getint(colatom);
//    else if (colatom->a_type == A_SYMBOL &&
//             (isdigit(colatom->a_w.w_symbol->s_name[0]) ||
//              colatom->a_w.w_symbol->s_name[0] == '-'))
//        color = atoi(colatom->a_w.w_symbol->s_name);
//
//    /* symbolic color */
//    else return (tl_cue_getcolorarg(0, 1, colatom));
//    if (color < 0)
//    {
//        color = -1 - color;
//        color = ((color & 0x3f000) << 6)|((color & 0xfc0) << 4)|
//        ((color & 0x3f) << 2);
//    }
//    else
//    {
//        color = tlcue_iemgui_modulo_color(color);
//        color = tlcue_iemgui_color_hex[color];
//    }
//    return (color);
//}
//
//void tlcue_all_loadcolors(t_iemgui *iemgui, t_atom*bcol, t_atom*fcol, t_atom*lcol)
//{
//    if(bcol)iemgui->x_bcol = tl_cue_colfromatomload(bcol);
//    if(fcol)iemgui->x_fcol = tl_cue_colfromatomload(fcol);
//    if(lcol)iemgui->x_lcol = tl_cue_colfromatomload(lcol);
//}
//#pragma mark draw
//
//void tl_cue_draw_update(tl_cue_object *x, t_glist *glist)
//{
////    if(glist_isvisible(glist))
////    {
////        t_canvas *canvas=glist_getcanvas(glist);
////
////    }
//}
//


#pragma mark draw


//

//
//void tl_cue_draw_config(tl_cue_object* x, t_glist* glist)
//{
//    t_canvas *canvas=glist_getcanvas(glist);
//
//    sys_vgui(".x%lx.c itemconfigure %lxLABEL -font {{%s} -%d %s} -fill #%06x -text {%s} \n",
//             canvas, x, x->x_gui.x_font, x->x_gui.x_fontsize, sys_fontweight,
//             x->x_gui.x_fsf.x_selected?IEM_GUI_COLOR_SELECTED:x->x_gui.x_lcol,
//             strcmp(x->x_gui.x_lab->s_name, "empty")?x->x_gui.x_lab->s_name:"");
//    sys_vgui(".x%lx.c itemconfigure %lxBASE -fill #%06x\n", canvas, x,
//             x->x_gui.x_bcol);
////    sys_vgui(".x%lx.c itemconfigure %lxX1 -fill #%06x\n", canvas, x,
////             x->x_on?x->x_gui.x_fcol:x->x_gui.x_bcol);
////    sys_vgui(".x%lx.c itemconfigure %lxX2 -fill #%06x\n", canvas, x,
////             x->x_on?x->x_gui.x_fcol:x->x_gui.x_bcol);
//}
//
//void tl_cue_draw_io(tl_cue_object* x, t_glist* glist, int old_snd_rcv_flags)
//{
//    int xpos=text_xpix(&x->x_gui.x_obj, glist);
//    int ypos=text_ypix(&x->x_gui.x_obj, glist);
//    t_canvas *canvas=glist_getcanvas(glist);
//
//    if((old_snd_rcv_flags & IEM_GUI_OLD_SND_FLAG) && !x->x_gui.x_fsf.x_snd_able)
//        sys_vgui(".x%lx.c create rectangle %d %d %d %d -fill %s -tags %lxOUT%d\n",
//                 canvas, xpos,
//                 ypos + x->x_gui.x_h-1, xpos + IOWIDTH,
//                 ypos + x->x_gui.x_h, "#FF00FF", x, 0);
//    if(!(old_snd_rcv_flags & IEM_GUI_OLD_SND_FLAG) && x->x_gui.x_fsf.x_snd_able)
//        sys_vgui(".x%lx.c delete %lxOUT%d\n", canvas, x, 0);
//    if((old_snd_rcv_flags & IEM_GUI_OLD_RCV_FLAG) && !x->x_gui.x_fsf.x_rcv_able)
//        sys_vgui(".x%lx.c create rectangle %d %d %d %d -fill %s -tags %lxIN%d\n",
//                 canvas, xpos, ypos,
//                 xpos + IOWIDTH, ypos+1, "#FFFF00", x, 0);
//    if(!(old_snd_rcv_flags & IEM_GUI_OLD_RCV_FLAG) && x->x_gui.x_fsf.x_rcv_able)
//        sys_vgui(".x%lx.c delete %lxIN%d\n", canvas, x, 0);
//}
//
//void tl_cue_draw_select(tl_cue_object* x, t_glist* glist)
//{
//    t_canvas *canvas=glist_getcanvas(glist);
//
//    if(x->x_gui.x_fsf.x_selected)
//    {
//        sys_vgui(".x%lx.c itemconfigure %lxBASE -outline %s\n", canvas, x, "#0000FF");
//        sys_vgui(".x%lx.c itemconfigure %lxLABEL -fill %s\n", canvas, x, "#0000FF");
//    }
//    else
//    {
//        sys_vgui(".x%lx.c itemconfigure %lxBASE -outline %s\n", canvas, x, "#00C0FF");
//        sys_vgui(".x%lx.c itemconfigure %lxLABEL -fill #%06x\n", canvas, x, x->x_gui.x_lcol);
//    }
//}
//

//void tl_cue_all_sym2dollararg(t_iemgui *iemgui, t_symbol **srlsym)
//{
//    printf("sym2dollararg\n");
//    tl_cue_init_sym2dollararg(iemgui, &iemgui->x_snd_unexpanded,
//                              iemgui->x_binbufindex+1, iemgui->x_snd);
//    tl_cue_init_sym2dollararg(iemgui, &iemgui->x_rcv_unexpanded,
//                              iemgui->x_binbufindex+2, iemgui->x_rcv);
//    tl_cue_init_sym2dollararg(iemgui, &iemgui->x_lab_unexpanded,
//                              iemgui->x_labelbindex, iemgui->x_lab);
//    srlsym[0] = iemgui->x_snd_unexpanded;
//    srlsym[1] = iemgui->x_rcv_unexpanded;
//    srlsym[2] = iemgui->x_lab_unexpanded;
//}
//
//static t_symbol* tl_color2symbol(int col) {
//    printf("color2symbol\n");
//    const int  compat = (pd_compatibilitylevel < 48)?1:
//    /* FIXXME: for Pd>=0.48, the default compatibility mode should be OFF */
//    1;
//
//    char colname[MAXPDSTRING];
//    colname[0] = colname[MAXPDSTRING-1] = 0;
//
//    if (compat)
//    {
//        /* compatibility with Pd<=0.47: saves colors as numbers with limited resolution */
//        int col2 = -1 - (((0xfc0000 & col) >> 6)|((0xfc00 & col) >> 4)|((0xfc & col) >> 2));
//        snprintf(colname, MAXPDSTRING-1, "%d", col2);
//    } else {
//        snprintf(colname, MAXPDSTRING-1, "#%06x", col);
//    }
//    return gensym(colname);
//}
//
//void tl_cue_all_col2save(t_iemgui *iemgui, t_symbol**bflcol)
//{
//    printf("col2save\n");
//    bflcol[0] = tl_color2symbol(iemgui->x_bcol);
//    bflcol[1] = tl_color2symbol(iemgui->x_fcol);
//    bflcol[2] = tl_color2symbol(iemgui->x_lcol);
//}
//
//void tl_cue_gui_save(t_iemgui *iemgui, t_symbol **srl, t_symbol**bflcol)
//{
//    srl[0] = iemgui->x_snd;
//    srl[1] = iemgui->x_rcv;
//    srl[2] = iemgui->x_lab;
//    tl_cue_all_sym2dollararg(iemgui, srl);  //iemgui_all_sym2dollararg
//    tl_cue_all_col2save(iemgui, bflcol);
//}
//
//int tl_cue_symargstoint(t_iem_init_symargs *symargp)
//{
//    return ((symargp->x_loadinit ? LOADINIT : 0) |
//            (symargp->x_scale ? (SCALE | SCALEBIS) : 0));
//}
//
//int tl_cue_fstyletoint(t_iem_fstyle_flags *fstylep)
//{
//    return ((fstylep->x_font_style << 0) & 63);
//}
//


#pragma mark -



#pragma mark -

#define UI_Pf x->ui_property_get_float
#define UI_Ps x->ui_property_get_c_str
#define UI_ this->

void tl_cue2_object::w_draw(t_gobj *z, t_glist *glist)
{
    printf("\ncue2 draw!  %lu | %lu\n", z, glist);
    
    ceammc_gui_object *x  = (ceammc_gui_object*)z;
    
    UI_ dw_set_canvas(glist);
    
    UI_ dw_rect("BASE", UI_Pf("x"), UI_Pf("y"), UI_Pf("width"), UI_Pf("height"),
                "#00C0FF", "#F0F0F0", 1.);
//    UI_ dw_rect("VLINE", UI_Pf("x"), 0, UI_Pf("x")+1, 1000,
//                "#00C0FF", "#00C0FF", 1.);
//    UI_ dw_text("LABEL", UI_Pf("x")+UI_Pf("label_x"), UI_Pf("y")+UI_Pf("label_y"),
//                UI_Ps("cue_name"), "#000000");
}
//
//void tl_cue2_object::w_erase(t_gobj *z, t_glist *glist)
//{
//    ceammc_gui_object *x  = (ceammc_gui_object*)z;
//    
//    UI_ dw_set_canvas(glist);
//    
//    UI_ dw_delete("BASE");
//    UI_ dw_delete("VLINE");
//    UI_ dw_delete("LABEL");
//    
//}
//
//
//
//void tl_cue2_object::w_displace(t_gobj *z, t_glist *glist, int dx, int dy)
//{
//    printf("cue2 displace!\n");
//    
////    this->w_displace(z,glist,dx,dy);
////    
////    ceammc_gui_object *x  = (ceammc_gui_object*)z;
////    
////    float x_pos=text_xpix(&x->x_obj, glist);
//////    tll_cue_update_pos((t_object *)x, x_pos);
////    
//////    int cue_idx = tll_cue_getnumber((t_object *)x);
////    char cuename[10];
//////    sprintf(cuename, "cue %d", cue_idx);
////    
////    UI_ dw_delete("LABEL");
////    UI_ dw_text("LABEL", UI_Pf("x")+UI_Pf("label_x"), UI_Pf("y")+UI_Pf("label_y"),
////                UI_Ps(cuename), "#000000");
////    
////    UI_ dw_move("BASE", UI_Pf("x"), UI_Pf("y"), UI_Pf("width"), UI_Pf("height"));
////    UI_ dw_move("BASE2", UI_Pf("x"), 0, UI_Pf("x")+1, 1000);
//    
//}


////////////////////

void tl_cue2_object::ui_property_init()
{
    ceammc_gui_object::ui_property_init();
    
    this->ui_property_set("cue_name", "cue x");
 
    this->ui_property_set("object_name", "tl.cue2");
}


//void tl_cue2::pd_setup(t_object* obj)
//{
//    ceammc_gui::pd_setup(obj);
//    printf("nope\n");
//}


void cpp__() 
{

    //printf("extern cue2\n");
    
    tl_cue2 *gui = new tl_cue2();
    
    tl_cue2_object *obj1 = new tl_cue2_object();
    obj1->ui_property_init();
    
    gui->pd_setup((t_object*)obj1);

}
    


extern "C" {
    
void setup()    //_tl0x2ecue2
{
    printf("extern C\n");
    
    cpp__();
}
    
}



