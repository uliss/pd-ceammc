/* Copyright (c) 1997-1999 Miller Puckette.
 * For information on usage and redistribution, and for a DISCLAIMER OF ALL
 * WARRANTIES, see the file, "LICENSE.txt," in this distribution. */

/* g_7_guis.c written by Thomas Musil (c) IEM KUG Graz Austria 2000-2001 */
/* thanks to Miller Puckette, Guenther Geiger and Krzystof Czaja */


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

#define HMARGIN 1
#define VMARGIN 2
#define PEAKHEIGHT 10

/* ----- vu  gui-peak- & rms- vu-meter-display ---------- */

t_widgetbehavior vu_widgetbehavior;
static t_class *vu_class;

/* widget helper functions */

static void vu_update_rms(t_vu *x, t_glist *glist)
{
    if(glist_isvisible(glist))
    {
        int w4 = x->x_gui.x_w / 4;
        int off = text_ypix(&x->x_gui.x_obj, glist) - IEMGUI_ZOOM(x);
        int xpos = text_xpix(&x->x_gui.x_obj, glist);
        int quad1 = xpos + w4 - IEMGUI_ZOOM(x);
        int quad3 = x->x_gui.x_w  + 2 * (IEMGUI_ZOOM(x) - w4);
        t_canvas *canvas = glist_getcanvas(glist);

        g_rect_move(canvas, x, "RCOVER",
                    quad1, off,
                    quad3, (x->x_led_size+1) * IEMGUI_ZOOM(x) * (IEM_VU_STEPS-x->x_rms));
    }
}

static void vu_update_peak(t_vu *x, t_glist *glist)
{
    t_canvas *canvas = glist_getcanvas(glist);

    if(glist_isvisible(glist))
    {
        int xpos = text_xpix(&x->x_gui.x_obj, glist);
        int ypos = text_ypix(&x->x_gui.x_obj, glist);

        if(x->x_peak)
        {
            int i = iemgui_vu_col[x->x_peak];
            int j = ypos +
                   (x->x_led_size+1)*IEMGUI_ZOOM(x)*(IEM_VU_STEPS+1-x->x_peak) -
                   (x->x_led_size+1)*IEMGUI_ZOOM(x)/2;

            g_line_move(canvas, x, "PLED",
                        xpos, j,
                        xpos + x->x_gui.x_w + IEMGUI_ZOOM(x), j);
            g_figure_fill(canvas, x, "PLED", iemgui_color_hex[i]);
        }
        else
        {
            int mid = xpos + x->x_gui.x_w/2;
            int pkh = PEAKHEIGHT * IEMGUI_ZOOM(x);

            g_figure_fill(canvas, x, "PLED", x->x_gui.x_bcol);
            g_line_move(canvas, x, "PLED",
                        mid, ypos + pkh, mid, ypos + pkh);
        }
    }
}

static void vu_draw_update(t_gobj *client, t_glist *glist)
{
    t_vu *x = (t_vu *)client;
    if (x->x_updaterms)
    {
        vu_update_rms(x, glist);
        x->x_updaterms = 0;
    }
    if (x->x_updatepeak)
    {
        vu_update_peak(x, glist);
        x->x_updatepeak = 0;
    }
}

static void vu_draw_new(t_vu *x, t_glist *glist)
{
    int xpos = text_xpix(&x->x_gui.x_obj, glist);
    int ypos = text_ypix(&x->x_gui.x_obj, glist);
    int hmargin = HMARGIN * IEMGUI_ZOOM(x), vmargin = VMARGIN * IEMGUI_ZOOM(x);
    int iow = IOWIDTH * IEMGUI_ZOOM(x), ioh = IEM_GUI_IOHEIGHT * IEMGUI_ZOOM(x);
    int w4 = x->x_gui.x_w/4, mid = xpos + x->x_gui.x_w/2,
        quad1 = xpos + w4 + IEMGUI_ZOOM(x);
    int quad3 = xpos + x->x_gui.x_w - w4,
        end = xpos + x->x_gui.x_w + 4*IEMGUI_ZOOM(x);
    int k1 = (x->x_led_size+1)*IEMGUI_ZOOM(x), k2 = IEM_VU_STEPS+1, k3 = k1/2;
    int led_col, yyy, i, k4 = ypos - k3;
    int ledw = x->x_led_size * IEMGUI_ZOOM(x);
    int fs = x->x_gui.x_fontsize * IEMGUI_ZOOM(x);
    t_canvas *canvas = glist_getcanvas(glist);

    g_rect_draw_filled(canvas, x, "BASE",
                       xpos - hmargin,
                       ypos - vmargin,
                       x->x_gui.x_w + 2* hmargin,
                       x->x_gui.x_h + 2 * vmargin,
                       x->x_gui.x_bcol);
    g_figure_set_linewidth(canvas, x, "BASE", IEMGUI_ZOOM(x));

    // leds
    for(int i = 1; i <= IEM_VU_STEPS; i++)
    {
        led_col = iemgui_vu_col[i];
        yyy = k4 + k1*(k2-i);

        char fig_id[10];
        snprintf(fig_id, sizeof(fig_id), "RLED%d", i);
        g_line_draw(canvas, x, fig_id, quad1, yyy, quad3, yyy, ledw, iemgui_color_hex[led_col]);
    }

    // scale
    if(x->x_scale)
    {
        char fig_id[10];
        const int step = 4;
        for(int i_ = 0; i_ <= IEM_VU_STEPS; i_ += step) {
            int idx = i_ + 1;
            yyy = k4 + k1*(k2 - idx);
            snprintf(fig_id, sizeof(fig_id), "SCALE%d", idx);
            g_text_draw(canvas, x, fig_id, end, yyy + k3, iemgui_vu_scale_str[idx]);
            g_text_anchor(canvas, x, fig_id, "w");
            g_text_font(canvas, x, fig_id, x->x_gui.x_font, fs, sys_fontweight);
            g_text_color(canvas, x, fig_id, x->x_gui.x_lcol);
        }
    }

    // led cover
    g_rect_draw_outfilled(canvas, x, "RCOVER",
                         quad1 - IEMGUI_ZOOM(x),
                         ypos - IEMGUI_ZOOM(x),
                         quad3 + IEMGUI_ZOOM(x) - quad1,
                         ypos - IEMGUI_ZOOM(x) + k1 * IEM_VU_STEPS - quad3,
                         x->x_gui.x_bcol);

    // peak led
    g_line_draw(canvas, x, "PLED",
                mid, ypos + PEAKHEIGHT * IEMGUI_ZOOM(x),
                mid, ypos + PEAKHEIGHT * IEMGUI_ZOOM(x),
                (x->x_led_size + 1) * IEMGUI_ZOOM(x), x->x_gui.x_bcol);

    // xlets
    if(!x->x_gui.x_fsf.x_snd_able)
    {
        int xlet_y = ypos + x->x_gui.x_h + vmargin - ioh;
        g_rect_draw_filled(canvas, x, "OUT0",
                           xpos - hmargin,
                           xlet_y,
                           iow,
                           ioh, IEM_GUI_COLOR_XLET);

        g_rect_draw_filled(canvas, x, "OUT1",
                           xpos + x->x_gui.x_w + hmargin - iow,
                           xlet_y,
                           iow, ioh,
                           IEM_GUI_COLOR_XLET);
    }
    if(!x->x_gui.x_fsf.x_rcv_able)
    {
        g_rect_draw_filled(canvas, x, "IN0",
                           xpos - hmargin,
                           ypos - vmargin,
                           iow, ioh,
                           IEM_GUI_COLOR_XLET);

        g_rect_draw_filled(canvas, x, "IN1",
                           xpos + x->x_gui.x_w + hmargin - iow,
                           ypos - vmargin,
                           iow, ioh,
                           IEM_GUI_COLOR_XLET);
    }

    // label
    g_iem_label_draw(canvas, &x->x_gui, xpos, ypos);
    x->x_updaterms = x->x_updatepeak = 1;

    sys_queuegui(x, x->x_gui.x_glist, vu_draw_update);
}

static void vu_draw_move(t_vu *x, t_glist *glist)
{
    int xpos = text_xpix(&x->x_gui.x_obj, glist);
    int ypos = text_ypix(&x->x_gui.x_obj, glist);
    int hmargin = HMARGIN * IEMGUI_ZOOM(x), vmargin = VMARGIN * IEMGUI_ZOOM(x);
    int iow = IOWIDTH * IEMGUI_ZOOM(x), ioh = IEM_GUI_IOHEIGHT * IEMGUI_ZOOM(x);
    int w4 = x->x_gui.x_w/4, quad1 = xpos + w4 + IEMGUI_ZOOM(x);
    int quad3 = xpos + x->x_gui.x_w - w4,
        end = xpos + x->x_gui.x_w + 4*IEMGUI_ZOOM(x);
    int k1 = (x->x_led_size+1)*IEMGUI_ZOOM(x), k2 = IEM_VU_STEPS+1, k3 = k1/2;
    int yyy, k4 = ypos - k3;
    t_canvas *canvas = glist_getcanvas(glist);

    g_rect_move(canvas, x, "BASE",
                xpos - hmargin,
                ypos - vmargin,
                x->x_gui.x_w + 2 * hmargin,
                x->x_gui.x_h + 2 * vmargin);

    // move leds
    for(int i = 1; i <= IEM_VU_STEPS; i++)
    {
        yyy = k4 + k1 * (k2 - i);

        char fig_id[10];
        snprintf(fig_id, sizeof(fig_id), "RLED%d", i);

        g_line_move(canvas, x, fig_id, quad1, yyy, quad3, yyy);
    }

    // move scale
    if(x->x_scale)
    {
        char fig_id[10];
        const int step = 4;
        for(int i_ = 0; i_ <= IEM_VU_STEPS; i_ += step) {
            int idx = i_ + 1;
            yyy = k4 + k1*(k2 - idx);
            snprintf(fig_id, sizeof(fig_id), "SCALE%d", idx);
            g_text_move(canvas, x, fig_id, end, yyy + k3);
        }
    }

    x->x_updaterms = x->x_updatepeak = 1;
    sys_queuegui(x, glist, vu_draw_update);

    // label move
    g_iem_label_move(canvas, &x->x_gui, xpos, ypos);

    // xlets move
    if(!x->x_gui.x_fsf.x_snd_able)
    {
        int xlet_y = ypos + x->x_gui.x_h + vmargin - ioh;
        g_rect_move(canvas, x, "OUT0",
                    xpos - hmargin, xlet_y,
                    iow, ioh);

        g_rect_move(canvas, x, "OUT1",
                    xpos + x->x_gui.x_w + hmargin - iow, xlet_y,
                    iow, ioh);
    }

    if(!x->x_gui.x_fsf.x_rcv_able)
    {
        g_rect_move(canvas, x, "IN0",
                    xpos - hmargin,
                    ypos - vmargin,
                    iow, ioh);

        g_rect_move(canvas, x, "IN1",
                    xpos + x->x_gui.x_w + hmargin - iow,
                    ypos - vmargin,
                    iow, ioh);
    }
}

static void vu_draw_erase(t_vu* x,t_glist* glist)
{
    t_canvas *canvas = glist_getcanvas(glist);

    g_figure_erase(canvas, x, "BASE");
    for(int i = 1; i <= IEM_VU_STEPS; i++)
    {
        char fig_id[10];
        snprintf(fig_id, sizeof(fig_id), "RLED%d", i);
        g_figure_erase(canvas, x, fig_id);
    }

    if(x->x_scale)
    {
        char fig_id[10];
        const int step = 4;
        for(int i_ = 0; i_ <= IEM_VU_STEPS; i_ += step) {
            int idx = i_ + 1;
            snprintf(fig_id, sizeof(fig_id), "SCALE%d", idx);
            g_figure_erase(canvas, x, fig_id);
        }
    }

    g_figure_erase(canvas, x, "PLED");
    g_figure_erase(canvas, x, "RCOVER");
    g_figure_erase(canvas, x, "LABEL");

    if(!x->x_gui.x_fsf.x_snd_able)
    {
        g_figure_erase_n(canvas, x, "OUT%d", 2);
    }
    if(!x->x_gui.x_fsf.x_rcv_able)
    {
        g_figure_erase_n(canvas, x, "IN%d", 2);
    }
}

static void vu_draw_config(t_vu* x, t_glist* glist)
{
    int ledw = x->x_led_size * IEMGUI_ZOOM(x);
    int fs = x->x_gui.x_fontsize * IEMGUI_ZOOM(x);
    t_canvas *canvas = glist_getcanvas(glist);

    g_figure_fill(canvas, x, "BASE", x->x_gui.x_bcol);
    for(int i = 1; i <= IEM_VU_STEPS; i++)
    {
        char fig_id[10];
        snprintf(fig_id, sizeof(fig_id), "RLED%d", i);
        g_figure_set_linewidth(canvas, x, fig_id, ledw);
    }

    if(x->x_scale)
    {
        char fig_id[10];
        const int step = 4;
        for(int i_ = 0; i_ <= IEM_VU_STEPS; i_ += step) {
            int idx = i_ + 1;
            snprintf(fig_id, sizeof(fig_id), "SCALE%d", idx);
            g_text_set(canvas, x, fig_id, iemgui_vu_scale_str[idx]);
            g_text_color(canvas, x, fig_id, x->x_gui.x_fsf.x_selected ? IEM_GUI_COLOR_SELECTED : x->x_gui.x_lcol);
            g_text_font(canvas, x, fig_id, x->x_gui.x_font, fs, sys_fontweight);
        }
    }

    g_iem_label_config(canvas, &x->x_gui);
    g_figure_outfill(canvas, x, "RCOVER", x->x_gui.x_bcol);
    g_figure_set_linewidth(canvas, x, "PLED", ledw);
}

static void vu_draw_io(t_vu* x, t_glist* glist, int old_snd_rcv_flags)
{
    int xpos = text_xpix(&x->x_gui.x_obj, glist);
    int ypos = text_ypix(&x->x_gui.x_obj, glist);
    int hmargin = HMARGIN * IEMGUI_ZOOM(x), vmargin = VMARGIN * IEMGUI_ZOOM(x);
    int iow = IOWIDTH * IEMGUI_ZOOM(x), ioh = IEM_GUI_IOHEIGHT * IEMGUI_ZOOM(x);
    t_canvas *canvas = glist_getcanvas(glist);

    if((old_snd_rcv_flags & IEM_GUI_OLD_SND_FLAG) && !x->x_gui.x_fsf.x_snd_able)
    {
        g_rect_draw_filled(canvas, x, "OUT0",
                           xpos - hmargin, ypos + x->x_gui.x_h + vmargin + IEMGUI_ZOOM(x) - ioh,
                           iow, ioh - IEMGUI_ZOOM(x),
                           IEM_GUI_COLOR_XLET);

        g_rect_draw_filled(canvas, x, "OUT1",
                           xpos + x->x_gui.x_w + hmargin - iow,
                           ypos + x->x_gui.x_h + vmargin + IEMGUI_ZOOM(x) - ioh,
                           iow, ioh - IEMGUI_ZOOM(x),
                           IEM_GUI_COLOR_XLET);

        /* keep above outlets */
        g_figure_raise(canvas, x, "LABEL", "OUT1");
    }
    if(!(old_snd_rcv_flags & IEM_GUI_OLD_SND_FLAG) && x->x_gui.x_fsf.x_snd_able)
    {
        g_figure_erase(canvas, x, "OUT0");
        g_figure_erase(canvas, x, "OUT1");
    }
    if((old_snd_rcv_flags & IEM_GUI_OLD_RCV_FLAG) && !x->x_gui.x_fsf.x_rcv_able)
    {
        g_rect_draw_filled(canvas, x, "IN0",
                           xpos - hmargin, ypos - vmargin,
                           iow, ioh - IEMGUI_ZOOM(x),
                           IEM_GUI_COLOR_XLET);

        g_rect_draw_filled(canvas, x, "IN1",
                           xpos + x->x_gui.x_w + hmargin - iow, ypos - vmargin,
                           iow, ioh - IEMGUI_ZOOM(x),
                           IEM_GUI_COLOR_XLET);

        /* keep above inlets */
        g_figure_raise(canvas, x, "LABEL", "IN1");
    }
    if(!(old_snd_rcv_flags & IEM_GUI_OLD_RCV_FLAG) && x->x_gui.x_fsf.x_rcv_able)
    {
        g_figure_erase(canvas, x, "IN0");
        g_figure_erase(canvas, x, "IN1");
    }
}

static void vu_draw_select(t_vu* x,t_glist* glist)
{
    t_canvas *canvas = glist_getcanvas(glist);

    if(x->x_gui.x_fsf.x_selected)
    {
        g_figure_outline(canvas, x, "BASE", IEM_GUI_COLOR_SELECTED);

        if(x->x_scale)
        {
            char fig_id[10];
            const int step = 4;
            for(int i_ = 0; i_ <= IEM_VU_STEPS; i_ += step) {
                int idx = i_ + 1;
                snprintf(fig_id, sizeof(fig_id), "SCALE%d", idx);
                g_text_color(canvas, x, fig_id, IEM_GUI_COLOR_SELECTED);
            }
        }

        g_figure_fill(canvas, x, "LABEL", IEM_GUI_COLOR_SELECTED);
    }
    else
    {
        g_figure_outline(canvas, x, "BASE", IEM_GUI_COLOR_NORMAL);

        if(x->x_scale)
        {
            char fig_id[10];
            const int step = 4;
            for(int i_ = 0; i_ <= IEM_VU_STEPS; i_ += step) {
                int idx = i_ + 1;
                snprintf(fig_id, sizeof(fig_id), "SCALE%d", idx);
                g_text_color(canvas, x, fig_id, x->x_gui.x_lcol);
            }
        }

         g_figure_fill(canvas, x, "LABEL", x->x_gui.x_lcol);
    }
}

void vu_draw(t_vu *x, t_glist *glist, int mode)
{
    if(mode == IEM_GUI_DRAW_MODE_MOVE)
        vu_draw_move(x, glist);
    else if(mode == IEM_GUI_DRAW_MODE_NEW)
        vu_draw_new(x, glist);
    else if(mode == IEM_GUI_DRAW_MODE_SELECT)
        vu_draw_select(x, glist);
    else if(mode == IEM_GUI_DRAW_MODE_ERASE)
        vu_draw_erase(x, glist);
    else if(mode == IEM_GUI_DRAW_MODE_CONFIG)
        vu_draw_config(x, glist);
    else if(mode >= IEM_GUI_DRAW_MODE_IO)
        vu_draw_io(x, glist, mode - IEM_GUI_DRAW_MODE_IO);
}

/* ------------------------ vu widgetbehaviour----------------------------- */

static void vu_getrect(t_gobj *z, t_glist *glist,
                       int *xp1, int *yp1, int *xp2, int *yp2)
{
    t_vu* x = (t_vu*)z;
    int hmargin = HMARGIN * IEMGUI_ZOOM(x), vmargin = VMARGIN * IEMGUI_ZOOM(x);

    *xp1 = text_xpix(&x->x_gui.x_obj, glist) - hmargin;
    *yp1 = text_ypix(&x->x_gui.x_obj, glist) - vmargin;
    *xp2 = *xp1 + x->x_gui.x_w + hmargin*2;
    *yp2 = *yp1 + x->x_gui.x_h + vmargin*2;
}

static void vu_save(t_gobj *z, t_binbuf *b)
{
    t_vu *x = (t_vu *)z;
    t_symbol *bflcol[3];
    t_symbol *srl[3];

    iemgui_save(&x->x_gui, srl, bflcol);
    binbuf_addv(b, "ssiisiissiiiissii", gensym("#X"), gensym("obj"),
                (int)x->x_gui.x_obj.te_xpix, (int)x->x_gui.x_obj.te_ypix,
                gensym("vu"), x->x_gui.x_w/IEMGUI_ZOOM(x), x->x_gui.x_h/IEMGUI_ZOOM(x),
                srl[1], srl[2],
                x->x_gui.x_ldx, x->x_gui.x_ldy,
                iem_fstyletoint(&x->x_gui.x_fsf), x->x_gui.x_fontsize,
                bflcol[0], bflcol[2], x->x_scale,
                iem_symargstoint(&x->x_gui.x_isa));
    binbuf_addv(b, ";");
}

void vu_check_height(t_vu *x, int h)
{
    int n;

    n = h / IEM_VU_STEPS;
    if(n < IEM_VU_MINSIZE)
        n = IEM_VU_MINSIZE;
    x->x_led_size = n - 1;
    x->x_gui.x_h = (IEM_VU_STEPS * n) * IEMGUI_ZOOM(x);
}

static void vu_scale(t_vu *x, t_floatarg fscale)
{
    int i, scale = (int)fscale;

    if(scale != 0) scale = 1;
    // hide scale
    if(x->x_scale && !scale)
    {
        t_canvas *canvas = glist_getcanvas(x->x_gui.x_glist);

        x->x_scale = (int)scale;
        if(glist_isvisible(x->x_gui.x_glist))
        {
            char fig_id[10];
            const int step = 4;
            for(int i_ = 0; i_ <= IEM_VU_STEPS; i_ += step) {
                int idx = i_ + 1;
                snprintf(fig_id, sizeof(fig_id), "SCALE%d", idx);
                g_figure_erase(canvas, x, fig_id);
            }
        }
    }
    // show scale
    if(!x->x_scale && scale)
    {
        int end = text_xpix(&x->x_gui.x_obj, x->x_gui.x_glist) + x->x_gui.x_w + 4*IEMGUI_ZOOM(x);
        int k1 = (x->x_led_size+1)*IEMGUI_ZOOM(x), k2 = IEM_VU_STEPS+1, k3 = k1/2;
        int yyy, k4 = text_ypix(&x->x_gui.x_obj, x->x_gui.x_glist) - k3;
        int fs = x->x_gui.x_fontsize * IEMGUI_ZOOM(x);
        t_canvas *canvas = glist_getcanvas(x->x_gui.x_glist);

        x->x_scale = (int)scale;
        if(glist_isvisible(x->x_gui.x_glist))
        {
            char fig_id[10];
            const int step = 4;
            for(int i_ = 0; i_ <= IEM_VU_STEPS; i_ += step) {
                int idx = i_ + 1;
                yyy = k4 + k1*(k2 - idx);
                snprintf(fig_id, sizeof(fig_id), "SCALE%d", idx);
                g_text_draw(canvas, x, fig_id, end, yyy + k3, iemgui_vu_scale_str[idx]);
                g_text_anchor(canvas, x, fig_id, "w");
                g_text_font(canvas, x, fig_id, x->x_gui.x_font, fs, sys_fontweight);
                g_text_color(canvas, x, fig_id, x->x_gui.x_lcol);
            }
        }
    }
}

static void vu_properties(t_gobj *z, t_glist *owner)
{
    t_vu *x = (t_vu *)z;
    char buf[800];
    t_symbol *srl[3];

    iemgui_properties(&x->x_gui, srl);
    sprintf(buf, "pdtk_iemgui_dialog %%s |vu| \
            --------dimensions(pix)(pix):-------- %d %d width: %d %d height: \
            empty 0.0 empty 0.0 empty %d \
            %d no_scale scale %d %d empty %d \
            %s %s \
            %s %d %d \
            %d %d \
            #%06x none #%06x\n",
            x->x_gui.x_w/IEMGUI_ZOOM(x), IEM_GUI_MINSIZE,
            x->x_gui.x_h/IEMGUI_ZOOM(x), IEM_VU_STEPS*IEM_VU_MINSIZE,
            0,/*no_schedule*/
            x->x_scale, -1, -1, -1,/*no linlog, no init, no multi*/
            "nosndno", srl[1]->s_name,/*no send*/
            srl[2]->s_name, x->x_gui.x_ldx, x->x_gui.x_ldy,
            x->x_gui.x_fsf.x_font_style, x->x_gui.x_fontsize,
            0xffffff & x->x_gui.x_bcol, 0xffffff & x->x_gui.x_lcol);
    gfxstub_new(&x->x_gui.x_obj.ob_pd, x, buf);
}

static void vu_dialog(t_vu *x, t_symbol *s, int argc, t_atom *argv)
{
    t_symbol *srl[3];
    int w = (int)atom_getfloatarg(0, argc, argv);
    int h = (int)atom_getfloatarg(1, argc, argv);
    int scale = (int)atom_getfloatarg(4, argc, argv);
    int sr_flags;

    srl[0] = gensym("empty");
    sr_flags = iemgui_dialog(&x->x_gui, srl, argc, argv);
    x->x_gui.x_fsf.x_snd_able = 0;
    x->x_gui.x_isa.x_loadinit = 0;
    x->x_gui.x_w = iemgui_clip_size(w) * IEMGUI_ZOOM(x);
    vu_check_height(x, h);
    if(scale != 0)
        scale = 1;
    vu_scale(x, (t_float)scale);
    (*x->x_gui.x_draw)(x, x->x_gui.x_glist, IEM_GUI_DRAW_MODE_CONFIG);
    (*x->x_gui.x_draw)(x, x->x_gui.x_glist, IEM_GUI_DRAW_MODE_IO + sr_flags);
    (*x->x_gui.x_draw)(x, x->x_gui.x_glist, IEM_GUI_DRAW_MODE_MOVE);
    canvas_fixlinesfor(x->x_gui.x_glist, (t_text*)x);
}

static void vu_size(t_vu *x, t_symbol *s, int ac, t_atom *av)
{
    x->x_gui.x_w = iemgui_clip_size((int)atom_getfloatarg(0, ac, av)) * IEMGUI_ZOOM(x);
    if(ac > 1)
        vu_check_height(x, (int)atom_getfloatarg(1, ac, av));
    if(glist_isvisible(x->x_gui.x_glist))
    {
        (*x->x_gui.x_draw)(x, x->x_gui.x_glist, IEM_GUI_DRAW_MODE_MOVE);
        (*x->x_gui.x_draw)(x, x->x_gui.x_glist, IEM_GUI_DRAW_MODE_CONFIG);
        canvas_fixlinesfor(x->x_gui.x_glist, (t_text*)x);
    }
}

static void vu_delta(t_vu *x, t_symbol *s, int ac, t_atom *av)
{iemgui_delta((void *)x, &x->x_gui, s, ac, av);}

static void vu_pos(t_vu *x, t_symbol *s, int ac, t_atom *av)
{iemgui_pos((void *)x, &x->x_gui, s, ac, av);}

static void vu_color(t_vu *x, t_symbol *s, int ac, t_atom *av)
{iemgui_color((void *)x, &x->x_gui, s, ac, av);}

static void vu_receive(t_vu *x, t_symbol *s)
{iemgui_receive(x, &x->x_gui, s);}

static void vu_label(t_vu *x, t_symbol *s)
{iemgui_label((void *)x, &x->x_gui, s);}

static void vu_label_pos(t_vu *x, t_symbol *s, int ac, t_atom *av)
{iemgui_label_pos((void *)x, &x->x_gui, s, ac, av);}

static void vu_label_font(t_vu *x, t_symbol *s, int ac, t_atom *av)
{iemgui_label_font((void *)x, &x->x_gui, s, ac, av);}

static void vu_float(t_vu *x, t_floatarg rms)
{
    int i;
    int old = x->x_rms;
    if(rms <= IEM_VU_MINDB)
        x->x_rms = 0;
    else if(rms >= IEM_VU_MAXDB)
        x->x_rms = IEM_VU_STEPS;
    else
    {
        int i = (int)(2.0*(rms + IEM_VU_OFFSET));
        x->x_rms = iemgui_vu_db2i[i];
    }
    i = (int)(100.0*rms + 10000.5);
    rms = 0.01*(t_float)(i - 10000);
    x->x_fr = rms;
    outlet_float(x->x_out_rms, rms);
    x->x_updaterms = 1;
    if(x->x_rms != old)
        sys_queuegui(x, x->x_gui.x_glist, vu_draw_update);
}

static void vu_ft1(t_vu *x, t_floatarg peak)
{
    int i;
    int old = x->x_peak;
    if(peak <= IEM_VU_MINDB)
        x->x_peak = 0;
    else if(peak >= IEM_VU_MAXDB)
        x->x_peak = IEM_VU_STEPS;
    else
    {
        int i = (int)(2.0*(peak + IEM_VU_OFFSET));
        x->x_peak = iemgui_vu_db2i[i];
    }
    i = (int)(100.0*peak + 10000.5);
    peak = 0.01*(t_float)(i - 10000);
    x->x_fp = peak;
    x->x_updatepeak = 1;
    if(x->x_peak != old)
        sys_queuegui(x, x->x_gui.x_glist, vu_draw_update);
    outlet_float(x->x_out_peak, peak);
}

static void vu_bang(t_vu *x)
{
    outlet_float(x->x_out_peak, x->x_fp);
    outlet_float(x->x_out_rms, x->x_fr);
    x->x_updaterms = x->x_updatepeak = 1;
    sys_queuegui(x, x->x_gui.x_glist, vu_draw_update);
}

static void *vu_new(t_symbol *s, int argc, t_atom *argv)
{
    t_vu *x = (t_vu *)pd_new(vu_class);
    int w = IEM_GUI_DEFAULTSIZE, h = IEM_VU_STEPS*IEM_VU_DEFAULTSIZE;
    int ldx = -1, ldy = -8, f = 0, fs = 10, scale  =1;
    int ftbreak = IEM_BNG_DEFAULTBREAKFLASHTIME, fthold = IEM_BNG_DEFAULTHOLDFLASHTIME;
    char str[144];

    iem_inttosymargs(&x->x_gui.x_isa, 0);
    iem_inttofstyle(&x->x_gui.x_fsf, 0);

    x->x_gui.x_bcol = 0x404040;
    x->x_gui.x_fcol = 0x00;
    x->x_gui.x_lcol = 0x00;

    if((argc >= 11)&&IS_A_FLOAT(argv,0)&&IS_A_FLOAT(argv,1)
       &&(IS_A_SYMBOL(argv,2)||IS_A_FLOAT(argv,2))
       &&(IS_A_SYMBOL(argv,3)||IS_A_FLOAT(argv,3))
       &&IS_A_FLOAT(argv,4)&&IS_A_FLOAT(argv,5)
       &&IS_A_FLOAT(argv,6)&&IS_A_FLOAT(argv,7)
       &&IS_A_FLOAT(argv,10))
    {
        w = (int)atom_getfloatarg(0, argc, argv);
        h = (int)atom_getfloatarg(1, argc, argv);
        iemgui_new_getnames(&x->x_gui, 1, argv);
        ldx = (int)atom_getfloatarg(4, argc, argv);
        ldy = (int)atom_getfloatarg(5, argc, argv);
        iem_inttofstyle(&x->x_gui.x_fsf, atom_getfloatarg(6, argc, argv));
        fs = (int)atom_getfloatarg(7, argc, argv);
        iemgui_all_loadcolors(&x->x_gui, argv+8, NULL, argv+9);
        scale = (int)atom_getfloatarg(10, argc, argv);
    }
    else iemgui_new_getnames(&x->x_gui, 1, 0);
    if((argc == 12)&&IS_A_FLOAT(argv,11))
        iem_inttosymargs(&x->x_gui.x_isa, atom_getfloatarg(11, argc, argv));
    x->x_gui.x_draw = (t_iemfunptr)vu_draw;

    x->x_gui.x_fsf.x_snd_able = 0;
    x->x_gui.x_fsf.x_rcv_able = 1;
    x->x_gui.x_glist = (t_glist *)canvas_getcurrent();
    if (!strcmp(x->x_gui.x_rcv->s_name, "empty"))
        x->x_gui.x_fsf.x_rcv_able = 0;
    if (x->x_gui.x_fsf.x_font_style == 1)
        strcpy(x->x_gui.x_font, "helvetica");
    else if(x->x_gui.x_fsf.x_font_style == 2)
        strcpy(x->x_gui.x_font, "times");
    else { x->x_gui.x_fsf.x_font_style = 0;
        strcpy(x->x_gui.x_font, sys_font); }
    if(x->x_gui.x_fsf.x_rcv_able)
        pd_bind(&x->x_gui.x_obj.ob_pd, x->x_gui.x_rcv);
    x->x_gui.x_ldx = ldx;
    x->x_gui.x_ldy = ldy;
    if(fs < 4)
        fs = 4;
    x->x_gui.x_fontsize = fs;
    x->x_gui.x_w = iemgui_clip_size(w);
    vu_check_height(x, h);
    if(scale != 0)
        scale = 1;
    x->x_scale = scale;
    x->x_peak = 0;
    x->x_rms = 0;
    x->x_fp = -101.0;
    x->x_fr = -101.0;
    iemgui_verify_snd_ne_rcv(&x->x_gui);
    inlet_new(&x->x_gui.x_obj, &x->x_gui.x_obj.ob_pd, &s_float, gensym("ft1"));
    x->x_out_rms = outlet_new(&x->x_gui.x_obj, &s_float);
    x->x_out_peak = outlet_new(&x->x_gui.x_obj, &s_float);
    iemgui_newzoom(&x->x_gui);
    return (x);
}

static void vu_free(t_vu *x)
{
    if(x->x_gui.x_fsf.x_rcv_able)
        pd_unbind(&x->x_gui.x_obj.ob_pd, x->x_gui.x_rcv);
    gfxstub_deleteforkey(x);
}

void g_vumeter_setup(void)
{
    vu_class = class_new(gensym("vu"), (t_newmethod)vu_new,
        (t_method)vu_free, sizeof(t_vu), 0, A_GIMME, 0);
    class_addbang(vu_class,vu_bang);
    class_addfloat(vu_class,vu_float);
    class_addmethod(vu_class, (t_method)vu_ft1,
        gensym("ft1"), A_FLOAT, 0);
    class_addmethod(vu_class, (t_method)vu_dialog,
        gensym("dialog"), A_GIMME, 0);
    class_addmethod(vu_class, (t_method)vu_size,
        gensym("size"), A_GIMME, 0);
    class_addmethod(vu_class, (t_method)vu_scale,
        gensym("scale"), A_DEFFLOAT, 0);
    class_addmethod(vu_class, (t_method)vu_delta,
        gensym("delta"), A_GIMME, 0);
    class_addmethod(vu_class, (t_method)vu_pos,
        gensym("pos"), A_GIMME, 0);
    class_addmethod(vu_class, (t_method)vu_color,
        gensym("color"), A_GIMME, 0);
    class_addmethod(vu_class, (t_method)vu_receive,
        gensym("receive"), A_DEFSYM, 0);
    class_addmethod(vu_class, (t_method)vu_label,
        gensym("label"), A_DEFSYM, 0);
    class_addmethod(vu_class, (t_method)vu_label_pos,
        gensym("label_pos"), A_GIMME, 0);
    class_addmethod(vu_class, (t_method)vu_label_font,
        gensym("label_font"), A_GIMME, 0);
    class_addmethod(vu_class, (t_method)iemgui_zoom,
        gensym("zoom"), A_CANT, 0);
    vu_widgetbehavior.w_getrectfn =    vu_getrect;
    vu_widgetbehavior.w_displacefn =   iemgui_displace;
    vu_widgetbehavior.w_selectfn =     iemgui_select;
    vu_widgetbehavior.w_activatefn =   NULL;
    vu_widgetbehavior.w_deletefn =     iemgui_delete;
    vu_widgetbehavior.w_visfn =        iemgui_vis;
    vu_widgetbehavior.w_clickfn =      NULL;
    class_setwidget(vu_class,&vu_widgetbehavior);
    class_sethelpsymbol(vu_class, gensym("vu"));
    class_setsavefn(vu_class, vu_save);
    class_setpropertiesfn(vu_class, vu_properties);
}
