/* Copyright (c) 1997-1999 Miller Puckette.
 * For information on usage and redistribution, and for a DISCLAIMER OF ALL
 * WARRANTIES, see the file, "LICENSE.txt," in this distribution. */

/* g_7_guis.c written by Thomas Musil (c) IEM KUG Graz Austria 2000-2001 */
/* thanks to Miller Puckette, Guenther Geiger and Krzystof Czaja */

/* name change to hradio by MSP and changed to
put out a "float" as in sliders, toggles, etc. */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include "m_pd.h"
#include "g_canvas.h"

#include "g_all_guis.h"
#include "g_ceammc_draw.h"
#include <math.h>

#ifdef _WIN32
#include <io.h>
#else
#include <unistd.h>
#endif

/* ------------- hdl     gui-horizontal dial ---------------------- */

t_widgetbehavior hradio_widgetbehavior;
static t_class *hradio_class, *hradio_old_class;

/* widget helper functions */

void hradio_draw_update(t_gobj *client, t_glist *glist)
{
    t_hradio *x = (t_hradio *)client;
    if(glist_isvisible(glist))
    {
        t_canvas *canvas = glist_getcanvas(glist);

        char buf_id[10] = { 0 };
        snprintf(buf_id, sizeof(buf_id), "BUT%d", x->x_drawn);
        g_figure_outfill(canvas, x, buf_id, x->x_gui.x_bcol);
        snprintf(buf_id, sizeof(buf_id), "BUT%d", x->x_on);
        g_figure_outfill(canvas, x, buf_id, x->x_gui.x_fcol);

        x->x_drawn = x->x_on;
    }
}

void hradio_draw_new(t_hradio *x, t_glist *glist)
{
    const int n = x->x_number;
    const int cell_size = x->x_gui.x_w;
    const unsigned int z = IEMGUI_ZOOM(x);
    const int zoom_fix = -1 * (z >> 1);
    const int knob_margin = cell_size / 4 + zoom_fix;
    const int knob_size = cell_size - 2 * knob_margin;
    const int xpos = text_xpix(&x->x_gui.x_obj, glist);
    const int ypos = text_ypix(&x->x_gui.x_obj, glist);
    char buf_id[10];

    t_canvas *canvas = glist_getcanvas(glist);

    for(int i = 0; i < n; i++)
    {
        const int cellx = xpos + i * cell_size;
        snprintf(buf_id, sizeof(buf_id), "BASE%d", i);

        g_rect_draw_filled(canvas, x, buf_id,
                           cellx, ypos, cell_size, cell_size,
                           x->x_gui.x_bcol);
        g_figure_set_linewidth(canvas, x, buf_id, IEMGUI_ZOOM(x));

        int color = (x->x_on == i) ? x->x_gui.x_fcol : x->x_gui.x_bcol;
        snprintf(buf_id, sizeof(buf_id), "BUT%d", i);
        g_rect_draw_outfilled(canvas, x, buf_id,
                              cellx + knob_margin, ypos + knob_margin,
                              knob_size, knob_size, color);
        x->x_drawn = x->x_on;
    }

    g_iem_inlets_draw(canvas, &x->x_gui, xpos, ypos);
    g_iem_outlets_draw(canvas, &x->x_gui, xpos, ypos);
    g_iem_label_draw(canvas, &x->x_gui, xpos, ypos);
}

void hradio_draw_move(t_hradio *x, t_glist *glist)
{
    const int n = x->x_number;
    const int cell_size = x->x_gui.x_w;
    const unsigned int z = IEMGUI_ZOOM(x);
    const int zoom_fix = -1 * (z >> 1);
    const int knob_margin = cell_size / 4 + zoom_fix;
    const int knob_size = cell_size - 2 * knob_margin;
    const int xpos = text_xpix(&x->x_gui.x_obj, glist);
    const int ypos = text_ypix(&x->x_gui.x_obj, glist);
    char buf_id[10];

    t_canvas *canvas = glist_getcanvas(glist);

    for(int i = 0; i < n; i++)
    {
        const int cellx = xpos + i * cell_size;
        snprintf(buf_id, sizeof(buf_id), "BASE%d", i);
        g_rect_move(canvas, x, buf_id, cellx, ypos, cell_size, cell_size);

        snprintf(buf_id, sizeof(buf_id), "BUT%d", i);
        g_rect_move(canvas, x, buf_id,
                    cellx + knob_margin, ypos + knob_margin, knob_size, knob_size);
    }

    g_iem_label_move(canvas, &x->x_gui, xpos, ypos);
    g_iem_io_move(canvas, &x->x_gui, xpos, ypos);
}

void hradio_draw_erase(t_hradio* x, t_glist* glist)
{
    int n = x->x_number;

    t_canvas *canvas = glist_getcanvas(glist);

    g_figure_erase_n(canvas, x, "BASE%d", n);
    g_figure_erase_n(canvas, x, "BUT%d", n);
    g_figure_erase(canvas, x, "LABEL");
    g_iem_io_erase(canvas, &x->x_gui);
}

void hradio_draw_config(t_hradio* x, t_glist* glist)
{
    int n = x->x_number;
    char buf_id[10] = { 0 };

    t_canvas *canvas = glist_getcanvas(glist);

    g_iem_label_config(canvas, &x->x_gui);
    g_figure_fill_n(canvas, x, "BASE%d", n, x->x_gui.x_bcol);

    for(int i = 0; i < n; i++)
    {
        snprintf(buf_id, sizeof(buf_id), "BUT%d", i);
        int color = (x->x_on == i) ? x->x_gui.x_fcol : x->x_gui.x_bcol;
        g_figure_outfill(canvas, x, buf_id, color);
    }
}

void hradio_draw_io(t_hradio* x, t_glist* glist, int old_snd_rcv_flags)
{
    int xpos = text_xpix(&x->x_gui.x_obj, glist);
    int ypos = text_ypix(&x->x_gui.x_obj, glist);
    t_canvas *canvas = glist_getcanvas(glist);

    g_iem_io_draw(canvas, &x->x_gui, xpos, ypos, old_snd_rcv_flags);

    if((old_snd_rcv_flags & IEM_GUI_OLD_SND_FLAG) && !x->x_gui.x_fsf.x_snd_able)
    {
        /* keep these above outlet */
        if(x->x_on == 0) {
            g_figure_raise(canvas, x, "BUT0", "OUT0");
            g_figure_raise(canvas, x, "LABEL", "BUT0");
        }
    }
    if((old_snd_rcv_flags & IEM_GUI_OLD_RCV_FLAG) && !x->x_gui.x_fsf.x_rcv_able)
    {
        /* keep these above inlet */
        if(x->x_on == 0) {
            g_figure_raise(canvas, x, "BUT0", "IN0");
            g_figure_raise(canvas, x, "LABEL", "BUT0");
        }
    }
}

void hradio_draw_select(t_hradio* x, t_glist* glist)
{
    t_canvas *canvas = glist_getcanvas(glist);

    if(x->x_gui.x_fsf.x_selected)
    {
        g_figure_outline_n(canvas, x, "BASE%d", x->x_number, IEM_GUI_COLOR_SELECTED);
        g_iem_label_select(canvas, &x->x_gui);
    }
    else
    {
        g_figure_outline_n(canvas, x, "BASE%d", x->x_number, IEM_GUI_COLOR_NORMAL);
        g_iem_label_unselect(canvas, &x->x_gui);
    }
}

void hradio_draw(t_hradio *x, t_glist *glist, int mode)
{
    if(mode == IEM_GUI_DRAW_MODE_UPDATE)
        sys_queuegui(x, glist, hradio_draw_update);
    else if(mode == IEM_GUI_DRAW_MODE_MOVE)
        hradio_draw_move(x, glist);
    else if(mode == IEM_GUI_DRAW_MODE_NEW)
        hradio_draw_new(x, glist);
    else if(mode == IEM_GUI_DRAW_MODE_SELECT)
        hradio_draw_select(x, glist);
    else if(mode == IEM_GUI_DRAW_MODE_ERASE)
        hradio_draw_erase(x, glist);
    else if(mode == IEM_GUI_DRAW_MODE_CONFIG)
        hradio_draw_config(x, glist);
    else if(mode >= IEM_GUI_DRAW_MODE_IO)
        hradio_draw_io(x, glist, mode - IEM_GUI_DRAW_MODE_IO);
}

/* ------------------------ hdl widgetbehaviour----------------------------- */

static void hradio_getrect(t_gobj *z, t_glist *glist, int *xp1, int *yp1, int *xp2, int *yp2)
{
    t_hradio *x = (t_hradio *)z;

    *xp1 = text_xpix(&x->x_gui.x_obj, glist);
    *yp1 = text_ypix(&x->x_gui.x_obj, glist);
    *xp2 = *xp1 + x->x_gui.x_w * x->x_number;
    *yp2 = *yp1 + x->x_gui.x_h;
}

static void hradio_save(t_gobj *z, t_binbuf *b)
{
    t_hradio *x = (t_hradio *)z;
    t_symbol *bflcol[3];
    t_symbol *srl[3];

    iemgui_save(&x->x_gui, srl, bflcol);
    binbuf_addv(b, "ssiisiiiisssiiiisssf", gensym("#X"),gensym("obj"),
                (int)x->x_gui.x_obj.te_xpix, (int)x->x_gui.x_obj.te_ypix,
                (pd_class(&x->x_gui.x_obj.ob_pd) == hradio_old_class ?
                    gensym("hdl") : gensym("hradio")),
                x->x_gui.x_w/IEMGUI_ZOOM(x),
                x->x_change, iem_symargstoint(&x->x_gui.x_isa), x->x_number,
                srl[0], srl[1], srl[2],
                x->x_gui.x_ldx, x->x_gui.x_ldy,
                iem_fstyletoint(&x->x_gui.x_fsf), x->x_gui.x_fontsize,
                bflcol[0], bflcol[1], bflcol[2], x->x_fval);
    binbuf_addv(b, ";");
}

static void hradio_properties(t_gobj *z, t_glist *owner)
{
    t_hradio *x = (t_hradio *)z;
    char buf[800];
    t_symbol *srl[3];
    int hchange = -1;

    iemgui_properties(&x->x_gui, srl);
    if(pd_class(&x->x_gui.x_obj.ob_pd) == hradio_old_class)
        hchange = x->x_change;
    sprintf(buf, "pdtk_iemgui_dialog %%s |hradio| \
            ----------dimensions(pix):----------- %d %d size: 0 0 empty \
            empty 0.0 empty 0.0 empty %d \
            %d new-only new&old %d %d number: %d \
            %s %s \
            %s %d %d \
            %d %d \
            #%06x #%06x #%06x\n",
            x->x_gui.x_w/IEMGUI_ZOOM(x), IEM_GUI_MINSIZE,
            0,/*no_schedule*/
            hchange, x->x_gui.x_isa.x_loadinit, -1, x->x_number,
            srl[0]->s_name, srl[1]->s_name,
            srl[2]->s_name, x->x_gui.x_ldx, x->x_gui.x_ldy,
            x->x_gui.x_fsf.x_font_style, x->x_gui.x_fontsize,
            0xffffff & x->x_gui.x_bcol, 0xffffff & x->x_gui.x_fcol, 0xffffff & x->x_gui.x_lcol);
    gfxstub_new(&x->x_gui.x_obj.ob_pd, x, buf);
}

static void hradio_dialog(t_hradio *x, t_symbol *s, int argc, t_atom *argv)
{
    t_symbol *srl[3];
    int a = (int)atom_getfloatarg(0, argc, argv);
    int chg = (int)atom_getfloatarg(4, argc, argv);
    int num = (int)atom_getfloatarg(6, argc, argv);
    int sr_flags;

    if(chg != 0) chg = 1;
    x->x_change = chg;
    sr_flags = iemgui_dialog(&x->x_gui, srl, argc, argv);
    x->x_gui.x_w = iemgui_clip_size(a) * IEMGUI_ZOOM(x);
    x->x_gui.x_h = x->x_gui.x_w;
    if(x->x_number != num)
    {
        (*x->x_gui.x_draw)(x, x->x_gui.x_glist, IEM_GUI_DRAW_MODE_ERASE);
        x->x_number = num;
        if(x->x_on >= x->x_number)
        {
            x->x_on = x->x_number - 1;
            x->x_on_old = x->x_on;
        }
        (*x->x_gui.x_draw)(x, x->x_gui.x_glist, IEM_GUI_DRAW_MODE_NEW);
    }
    else
    {
        (*x->x_gui.x_draw)(x, x->x_gui.x_glist, IEM_GUI_DRAW_MODE_CONFIG);
        (*x->x_gui.x_draw)(x, x->x_gui.x_glist, IEM_GUI_DRAW_MODE_IO + sr_flags);
        (*x->x_gui.x_draw)(x, x->x_gui.x_glist, IEM_GUI_DRAW_MODE_MOVE);
        canvas_fixlinesfor(x->x_gui.x_glist, (t_text*)x);
    }

}

static void hradio_set(t_hradio *x, t_floatarg f)
{
    int i = (int)f;
    int old = x->x_on_old;

    x->x_fval = f;
    if(i < 0)
        i = 0;
    if(i >= x->x_number)
        i = x->x_number - 1;
    if(x->x_on != x->x_on_old)
    {
        old = x->x_on_old;
        x->x_on_old = x->x_on;
        x->x_on = i;
        (*x->x_gui.x_draw)(x, x->x_gui.x_glist, IEM_GUI_DRAW_MODE_UPDATE);
        x->x_on_old = old;
    }
    else
    {
        x->x_on = i;
        (*x->x_gui.x_draw)(x, x->x_gui.x_glist, IEM_GUI_DRAW_MODE_UPDATE);
    }
}

static void hradio_bang(t_hradio *x)
{
        /* compatibility with earlier "hdial" behavior */
    if(pd_class(&x->x_gui.x_obj.ob_pd) == hradio_old_class)
    {
        if((x->x_change) && (x->x_on != x->x_on_old))
        {
            SETFLOAT(x->x_at, (t_float)x->x_on_old);
            SETFLOAT(x->x_at+1, 0.0);
            outlet_list(x->x_gui.x_obj.ob_outlet, &s_list, 2, x->x_at);
            if(x->x_gui.x_fsf.x_snd_able && x->x_gui.x_snd->s_thing)
                pd_list(x->x_gui.x_snd->s_thing, &s_list, 2, x->x_at);
        }
        x->x_on_old = x->x_on;
        SETFLOAT(x->x_at, (t_float)x->x_on);
        SETFLOAT(x->x_at+1, 1.0);
        outlet_list(x->x_gui.x_obj.ob_outlet, &s_list, 2, x->x_at);
        if(x->x_gui.x_fsf.x_snd_able && x->x_gui.x_snd->s_thing)
            pd_list(x->x_gui.x_snd->s_thing, &s_list, 2, x->x_at);
    }
    else
    {
        float outval = (pd_compatibilitylevel < 46 ? x->x_on : x->x_fval);
        outlet_float(x->x_gui.x_obj.ob_outlet, outval);
        if(x->x_gui.x_fsf.x_snd_able && x->x_gui.x_snd->s_thing)
            pd_float(x->x_gui.x_snd->s_thing, outval);
    }
}

static void hradio_fout(t_hradio *x, t_floatarg f)
{
    int i = (int)f;

    x->x_fval = f;
    if(i < 0)
        i = 0;
    if(i >= x->x_number)
        i = x->x_number - 1;

    if(pd_class(&x->x_gui.x_obj.ob_pd) == hradio_old_class)
    {
        if((x->x_change) && (i != x->x_on_old))
        {
            SETFLOAT(x->x_at, (t_float)x->x_on_old);
            SETFLOAT(x->x_at+1, 0.0);
            outlet_list(x->x_gui.x_obj.ob_outlet, &s_list, 2, x->x_at);
            if(x->x_gui.x_fsf.x_snd_able && x->x_gui.x_snd->s_thing)
                pd_list(x->x_gui.x_snd->s_thing, &s_list, 2, x->x_at);
        }
        if(x->x_on != x->x_on_old)
            x->x_on_old = x->x_on;
        x->x_on = i;
        (*x->x_gui.x_draw)(x, x->x_gui.x_glist, IEM_GUI_DRAW_MODE_UPDATE);
        x->x_on_old = x->x_on;
        SETFLOAT(x->x_at, (t_float)x->x_on);
        SETFLOAT(x->x_at+1, 1.0);
        outlet_list(x->x_gui.x_obj.ob_outlet, &s_list, 2, x->x_at);
        if(x->x_gui.x_fsf.x_snd_able && x->x_gui.x_snd->s_thing)
            pd_list(x->x_gui.x_snd->s_thing, &s_list, 2, x->x_at);
    }
    else
    {
        float outval = (pd_compatibilitylevel < 46 ? i : x->x_fval);
        x->x_on_old = x->x_on;
        x->x_on = i;
        (*x->x_gui.x_draw)(x, x->x_gui.x_glist, IEM_GUI_DRAW_MODE_UPDATE);
        outlet_float(x->x_gui.x_obj.ob_outlet, outval);
        if(x->x_gui.x_fsf.x_snd_able && x->x_gui.x_snd->s_thing)
            pd_float(x->x_gui.x_snd->s_thing, outval);
    }
}

static void hradio_float(t_hradio *x, t_floatarg f)
{
    int i = (int)f;
    x->x_fval = f;
    if(i < 0)
        i = 0;
    if(i >= x->x_number)
        i = x->x_number - 1;

    if(pd_class(&x->x_gui.x_obj.ob_pd) == hradio_old_class)
    {
            /* compatibility with earlier "hdial" behavior */
        if((x->x_change) && (i != x->x_on_old))
        {
            if(x->x_gui.x_fsf.x_put_in2out)
            {
                SETFLOAT(x->x_at, (t_float)x->x_on_old);
                SETFLOAT(x->x_at+1, 0.0);
                outlet_list(x->x_gui.x_obj.ob_outlet, &s_list, 2, x->x_at);
                if(x->x_gui.x_fsf.x_snd_able && x->x_gui.x_snd->s_thing)
                    pd_list(x->x_gui.x_snd->s_thing, &s_list, 2, x->x_at);
            }
        }
        if(x->x_on != x->x_on_old)
            x->x_on_old = x->x_on;
        x->x_on = i;
        (*x->x_gui.x_draw)(x, x->x_gui.x_glist, IEM_GUI_DRAW_MODE_UPDATE);
        x->x_on_old = x->x_on;
        if(x->x_gui.x_fsf.x_put_in2out)
        {
            SETFLOAT(x->x_at, (t_float)x->x_on);
            SETFLOAT(x->x_at+1, 1.0);
            outlet_list(x->x_gui.x_obj.ob_outlet, &s_list, 2, x->x_at);
            if(x->x_gui.x_fsf.x_snd_able && x->x_gui.x_snd->s_thing)
                pd_list(x->x_gui.x_snd->s_thing, &s_list, 2, x->x_at);
        }
    }
    else
    {
        float outval = (pd_compatibilitylevel < 46 ? i : x->x_fval);
        x->x_on_old = x->x_on;
        x->x_on = i;
        (*x->x_gui.x_draw)(x, x->x_gui.x_glist, IEM_GUI_DRAW_MODE_UPDATE);
        if(x->x_gui.x_fsf.x_put_in2out)
        {
            outlet_float(x->x_gui.x_obj.ob_outlet, outval);
            if(x->x_gui.x_fsf.x_snd_able && x->x_gui.x_snd->s_thing)
                pd_float(x->x_gui.x_snd->s_thing, outval);
        }
    }
}

static void hradio_click(t_hradio *x, t_floatarg xpos, t_floatarg ypos, t_floatarg shift, t_floatarg ctrl, t_floatarg alt)
{
    int xx = (int)xpos - (int)text_xpix(&x->x_gui.x_obj, x->x_gui.x_glist);

    hradio_fout(x, (t_float)(xx / x->x_gui.x_w));
}

static int hradio_newclick(t_gobj *z, struct _glist *glist, int xpix, int ypix, int shift, int alt, int dbl, int doit)
{
    if(doit)
        hradio_click((t_hradio *)z, (t_floatarg)xpix, (t_floatarg)ypix, (t_floatarg)shift, 0, (t_floatarg)alt);
    return (1);
}

static void hradio_loadbang(t_hradio *x, t_floatarg action)
{
    if(action == LB_LOAD && x->x_gui.x_isa.x_loadinit)
        hradio_bang(x);
}

static void hradio_number(t_hradio *x, t_floatarg num)
{
    int n = (int)num;

    if(n < 1)
        n = 1;
    if(n > IEM_RADIO_MAX)
        n = IEM_RADIO_MAX;
    if(n != x->x_number)
    {
        (*x->x_gui.x_draw)(x, x->x_gui.x_glist, IEM_GUI_DRAW_MODE_ERASE);
        x->x_number = n;
        if(x->x_on >= x->x_number)
            x->x_on = x->x_number - 1;
        x->x_on_old = x->x_on;
        (*x->x_gui.x_draw)(x, x->x_gui.x_glist, IEM_GUI_DRAW_MODE_NEW);
        canvas_fixlinesfor(x->x_gui.x_glist, (t_text*)x);
    }
}

static void hradio_size(t_hradio *x, t_symbol *s, int ac, t_atom *av)
{
    x->x_gui.x_w = iemgui_clip_size((int)atom_getfloatarg(0, ac, av)) * IEMGUI_ZOOM(x);
    x->x_gui.x_h = x->x_gui.x_w;
    iemgui_size((void *)x, &x->x_gui);
}

static void hradio_delta(t_hradio *x, t_symbol *s, int ac, t_atom *av)
{iemgui_delta((void *)x, &x->x_gui, s, ac, av);}

static void hradio_pos(t_hradio *x, t_symbol *s, int ac, t_atom *av)
{iemgui_pos((void *)x, &x->x_gui, s, ac, av);}

static void hradio_color(t_hradio *x, t_symbol *s, int ac, t_atom *av)
{iemgui_color((void *)x, &x->x_gui, s, ac, av);}

static void hradio_send(t_hradio *x, t_symbol *s)
{iemgui_send(x, &x->x_gui, s);}

static void hradio_receive(t_hradio *x, t_symbol *s)
{iemgui_receive(x, &x->x_gui, s);}

static void hradio_label(t_hradio *x, t_symbol *s)
{iemgui_label((void *)x, &x->x_gui, s);}

static void hradio_label_pos(t_hradio *x, t_symbol *s, int ac, t_atom *av)
{iemgui_label_pos((void *)x, &x->x_gui, s, ac, av);}

static void hradio_label_font(t_hradio *x, t_symbol *s, int ac, t_atom *av)
{iemgui_label_font((void *)x, &x->x_gui, s, ac, av);}

static void hradio_init(t_hradio *x, t_floatarg f)
{x->x_gui.x_isa.x_loadinit = (f == 0.0) ? 0 : 1;}

static void hradio_double_change(t_hradio *x)
{x->x_change = 1;}

static void hradio_single_change(t_hradio *x)
{x->x_change = 0;}

static void *hradio_donew(t_symbol *s, int argc, t_atom *argv, int old)
{
    t_hradio *x = (t_hradio *)pd_new(old ? hradio_old_class : hradio_class);
    int a = IEM_GUI_DEFAULTSIZE, on = 0;
    int ldx = 0, ldy = -8, chg = 1, num = 8;
    int fs = 10;
    float fval = 0;

    iem_inttosymargs(&x->x_gui.x_isa, 0);
    iem_inttofstyle(&x->x_gui.x_fsf, 0);

    x->x_gui.x_bcol = 0xFCFCFC;
    x->x_gui.x_fcol = 0x00;
    x->x_gui.x_lcol = 0x00;

    if((argc == 15)&&IS_A_FLOAT(argv,0)&&IS_A_FLOAT(argv,1)&&IS_A_FLOAT(argv,2)
       &&IS_A_FLOAT(argv,3)
       &&(IS_A_SYMBOL(argv,4)||IS_A_FLOAT(argv,4))
       &&(IS_A_SYMBOL(argv,5)||IS_A_FLOAT(argv,5))
       &&(IS_A_SYMBOL(argv,6)||IS_A_FLOAT(argv,6))
       &&IS_A_FLOAT(argv,7)&&IS_A_FLOAT(argv,8)
       &&IS_A_FLOAT(argv,9)&&IS_A_FLOAT(argv,10)&&IS_A_FLOAT(argv,14))
    {
        a = (int)atom_getfloatarg(0, argc, argv);
        chg = (int)atom_getfloatarg(1, argc, argv);
        iem_inttosymargs(&x->x_gui.x_isa, atom_getfloatarg(2, argc, argv));
        num = (int)atom_getfloatarg(3, argc, argv);
        iemgui_new_getnames(&x->x_gui, 4, argv);
        ldx = (int)atom_getfloatarg(7, argc, argv);
        ldy = (int)atom_getfloatarg(8, argc, argv);
        iem_inttofstyle(&x->x_gui.x_fsf, atom_getfloatarg(9, argc, argv));
        fs = (int)atom_getfloatarg(10, argc, argv);
        iemgui_all_loadcolors(&x->x_gui, argv+11, argv+12, argv+13);
        fval = atom_getfloatarg(14, argc, argv);
    }
    else iemgui_new_getnames(&x->x_gui, 4, 0);
    x->x_gui.x_draw = (t_iemfunptr)hradio_draw;
    x->x_gui.x_fsf.x_snd_able = 1;
    x->x_gui.x_fsf.x_rcv_able = 1;
    x->x_gui.x_glist = (t_glist *)canvas_getcurrent();
    if(!strcmp(x->x_gui.x_snd->s_name, "empty"))
        x->x_gui.x_fsf.x_snd_able = 0;
    if(!strcmp(x->x_gui.x_rcv->s_name, "empty"))
        x->x_gui.x_fsf.x_rcv_able = 0;
    if(x->x_gui.x_fsf.x_font_style == 1) strcpy(x->x_gui.x_font, "helvetica");
    else if(x->x_gui.x_fsf.x_font_style == 2) strcpy(x->x_gui.x_font, "times");
    else { x->x_gui.x_fsf.x_font_style = 0;
        strcpy(x->x_gui.x_font, sys_font); }
    if(num < 1)
        num = 1;
    if(num > IEM_RADIO_MAX)
        num = IEM_RADIO_MAX;
    x->x_number = num;
    x->x_fval = fval;
    on = fval;
    if(on < 0)
        on = 0;
    if(on >= x->x_number)
        on = x->x_number - 1;
    if(x->x_gui.x_isa.x_loadinit)
        x->x_on = on;
    else
        x->x_on = 0;
    x->x_on_old = x->x_on;
    x->x_change = (chg == 0) ? 0 : 1;
    if(x->x_gui.x_fsf.x_rcv_able)
        pd_bind(&x->x_gui.x_obj.ob_pd, x->x_gui.x_rcv);
    x->x_gui.x_ldx = ldx;
    x->x_gui.x_ldy = ldy;
    if(fs < 4)
        fs = 4;
    x->x_gui.x_fontsize = fs;
    x->x_gui.x_w = iemgui_clip_size(a);
    x->x_gui.x_h = x->x_gui.x_w;
    iemgui_verify_snd_ne_rcv(&x->x_gui);
    iemgui_newzoom(&x->x_gui);
    outlet_new(&x->x_gui.x_obj, &s_list);
    return (x);
}

static void *hradio_new(t_symbol *s, int argc, t_atom *argv)
{
    return (hradio_donew(s, argc, argv, 0));
}

static void *hdial_new(t_symbol *s, int argc, t_atom *argv)
{
    return (hradio_donew(s, argc, argv, 1));
}

static void hradio_ff(t_hradio *x)
{
    if(x->x_gui.x_fsf.x_rcv_able)
        pd_unbind(&x->x_gui.x_obj.ob_pd, x->x_gui.x_rcv);
    gfxstub_deleteforkey(x);
}

void g_hradio_setup(void)
{
    hradio_class = class_new(gensym("hradio"), (t_newmethod)hradio_new,
        (t_method)hradio_ff, sizeof(t_hradio), 0, A_GIMME, 0);
    class_addbang(hradio_class, hradio_bang);
    class_addfloat(hradio_class, hradio_float);
    class_addmethod(hradio_class, (t_method)hradio_click,
        gensym("click"), A_FLOAT, A_FLOAT, A_FLOAT, A_FLOAT, A_FLOAT, 0);
    class_addmethod(hradio_class, (t_method)hradio_dialog,
        gensym("dialog"), A_GIMME, 0);
    class_addmethod(hradio_class, (t_method)hradio_loadbang,
        gensym("loadbang"), A_DEFFLOAT, 0);
    class_addmethod(hradio_class, (t_method)hradio_set,
        gensym("set"), A_FLOAT, 0);
    class_addmethod(hradio_class, (t_method)hradio_size,
        gensym("size"), A_GIMME, 0);
    class_addmethod(hradio_class, (t_method)hradio_delta,
        gensym("delta"), A_GIMME, 0);
    class_addmethod(hradio_class, (t_method)hradio_pos,
        gensym("pos"), A_GIMME, 0);
    class_addmethod(hradio_class, (t_method)hradio_color,
        gensym("color"), A_GIMME, 0);
    class_addmethod(hradio_class, (t_method)hradio_send,
        gensym("send"), A_DEFSYM, 0);
    class_addmethod(hradio_class, (t_method)hradio_receive,
        gensym("receive"), A_DEFSYM, 0);
    class_addmethod(hradio_class, (t_method)hradio_label,
        gensym("label"), A_DEFSYM, 0);
    class_addmethod(hradio_class, (t_method)hradio_label_pos,
        gensym("label_pos"), A_GIMME, 0);
    class_addmethod(hradio_class, (t_method)hradio_label_font,
        gensym("label_font"), A_GIMME, 0);
    class_addmethod(hradio_class, (t_method)hradio_init,
        gensym("init"), A_FLOAT, 0);
    class_addmethod(hradio_class, (t_method)hradio_number,
        gensym("number"), A_FLOAT, 0);
    class_addmethod(hradio_class, (t_method)hradio_single_change,
        gensym("single_change"), 0);
    class_addmethod(hradio_class, (t_method)hradio_double_change,
        gensym("double_change"), 0);
    class_addmethod(hradio_class, (t_method)iemgui_zoom,
        gensym("zoom"), A_CANT, 0);
    hradio_widgetbehavior.w_getrectfn = hradio_getrect;
    hradio_widgetbehavior.w_displacefn = iemgui_displace;
    hradio_widgetbehavior.w_selectfn = iemgui_select;
    hradio_widgetbehavior.w_activatefn = NULL;
    hradio_widgetbehavior.w_deletefn = iemgui_delete;
    hradio_widgetbehavior.w_visfn = iemgui_vis;
    hradio_widgetbehavior.w_clickfn = hradio_newclick;
    class_setwidget(hradio_class, &hradio_widgetbehavior);
    class_sethelpsymbol(hradio_class, gensym("hradio"));
    class_setsavefn(hradio_class, hradio_save);
    class_setpropertiesfn(hradio_class, hradio_properties);

        /*obsolete version (0.34-0.35) */
    hradio_old_class = class_new(gensym("hdl"), (t_newmethod)hdial_new,
        (t_method)hradio_ff, sizeof(t_hradio), 0, A_GIMME, 0);
    class_addcreator((t_newmethod)hradio_new, gensym("rdb"), A_GIMME, 0);
    class_addcreator((t_newmethod)hradio_new, gensym("radiobut"), A_GIMME, 0);
    class_addcreator((t_newmethod)hradio_new, gensym("radiobutton"),
        A_GIMME, 0);
    class_addbang(hradio_old_class, hradio_bang);
    class_addfloat(hradio_old_class, hradio_float);
    class_addmethod(hradio_old_class, (t_method)hradio_click,
        gensym("click"), A_FLOAT, A_FLOAT, A_FLOAT, A_FLOAT, A_FLOAT, 0);
    class_addmethod(hradio_old_class, (t_method)hradio_dialog,
        gensym("dialog"), A_GIMME, 0);
    class_addmethod(hradio_old_class, (t_method)hradio_loadbang,
        gensym("loadbang"), 0);
    class_addmethod(hradio_old_class, (t_method)hradio_set,
        gensym("set"), A_FLOAT, 0);
    class_addmethod(hradio_old_class, (t_method)hradio_size,
        gensym("size"), A_GIMME, 0);
    class_addmethod(hradio_old_class, (t_method)hradio_delta,
        gensym("delta"), A_GIMME, 0);
    class_addmethod(hradio_old_class, (t_method)hradio_pos,
        gensym("pos"), A_GIMME, 0);
    class_addmethod(hradio_old_class, (t_method)hradio_color,
        gensym("color"), A_GIMME, 0);
    class_addmethod(hradio_old_class, (t_method)hradio_send,
        gensym("send"), A_DEFSYM, 0);
    class_addmethod(hradio_old_class, (t_method)hradio_receive,
        gensym("receive"), A_DEFSYM, 0);
    class_addmethod(hradio_old_class, (t_method)hradio_label,
        gensym("label"), A_DEFSYM, 0);
    class_addmethod(hradio_old_class, (t_method)hradio_label_pos,
        gensym("label_pos"), A_GIMME, 0);
    class_addmethod(hradio_old_class, (t_method)hradio_label_font,
        gensym("label_font"), A_GIMME, 0);
    class_addmethod(hradio_old_class, (t_method)hradio_init,
        gensym("init"), A_FLOAT, 0);
    class_addmethod(hradio_old_class, (t_method)hradio_number,
        gensym("number"), A_FLOAT, 0);
    class_addmethod(hradio_old_class, (t_method)hradio_single_change,
        gensym("single_change"), 0);
    class_addmethod(hradio_old_class, (t_method)hradio_double_change,
        gensym("double_change"), 0);
    class_addmethod(hradio_old_class, (t_method)iemgui_zoom,
        gensym("zoom"), A_CANT, 0);
    class_setwidget(hradio_old_class, &hradio_widgetbehavior);
    class_sethelpsymbol(hradio_old_class, gensym("hradio"));
}
