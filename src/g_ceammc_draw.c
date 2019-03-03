/*****************************************************************************
 * Copyright 2019 Serge Poltavsky. All rights reserved.
 *
 * This file may be distributed under the terms of GNU Public License version
 * 3 (GPL v3) as defined by the Free Software Foundation (FSF). A copy of the
 * license should have been included with this file, or the project in which
 * this file belongs to. You may also find the details of GPL v3 at:
 * http://www.gnu.org/licenses/gpl-3.0.txt
 *
 * If you have any questions regarding the use of this file, feel free to
 * contact the author of this file, or the owner of the project in which
 * this file belongs to.
 *****************************************************************************/
#ifndef G_CEAMMC_DRAW_C
#define G_CEAMMC_DRAW_C

#include "g_ceammc_draw.h"

#include "g_canvas.h"

#include <string.h>

void g_iem_brect_draw(t_canvas* canvas, t_iemgui* x, int xpos, int ypos)
{
    const int z = x->x_glist->gl_zoom;

    g_rect_draw_filled(canvas, x, "BASE", xpos, ypos, x->x_w, x->x_h, x->x_bcol);
    g_figure_set_linewidth(canvas, x, "BASE", z);
}

void g_iem_inlets_draw(t_canvas* canvas, t_iemgui* x, int xpos, int ypos)
{
    const int z = x->x_glist->gl_zoom;
    const int iow = IOWIDTH * z;
    const int ioh = IEM_GUI_IOHEIGHT * z;

    if (!x->x_fsf.x_rcv_able)
        sys_vgui(".x%lx.c create rectangle %d %d %d %d -fill #%6.6x -tags [list %lx_IN0 inlet]\n",
            canvas,
            xpos, ypos,
            xpos + iow, ypos - z + ioh, IEM_GUI_COLOR_XLET, x);
}

void g_iem_outlets_draw(t_canvas* canvas, t_iemgui* x, int xpos, int ypos)
{
    const int z = x->x_glist->gl_zoom;
    const int iow = IOWIDTH * z;
    const int ioh = IEM_GUI_IOHEIGHT * z;

    if (!x->x_fsf.x_snd_able)
        sys_vgui(".x%lx.c create rectangle %d %d %d %d -fill #%6.6x -tags [list %lx_OUT0 outlet]\n",
            canvas,
            xpos, ypos + x->x_h - ioh,
            xpos + iow, ypos + x->x_h, IEM_GUI_COLOR_XLET, x);
}

void g_iem_label_draw(t_canvas* canvas, t_iemgui* x, int xpos, int ypos)
{
    const int z = x->x_glist->gl_zoom;
    sys_vgui(".x%lx.c create text %d %d -text {%s} -anchor w \
             -font {{%s} -%d %s} -fill #%6.6x -tags [list %lx_LABEL label text]\n",
        canvas, xpos + x->x_ldx * z,
        ypos + x->x_ldy * z, (strcmp(x->x_lab->s_name, "empty") ? x->x_lab->s_name : ""),
        x->x_font, x->x_fontsize * z, sys_fontweight,
        x->x_lcol, x);
}

void g_iem_label_config(t_canvas* canvas, t_iemgui* x)
{
    const int z = x->x_glist->gl_zoom;

    g_text_set(canvas, x, "LABEL", (strcmp(x->x_lab->s_name, "empty") ? x->x_lab->s_name : ""));
    g_text_font(canvas, x, "LABEL", x->x_font, x->x_fontsize * z, sys_fontweight);
    g_text_color(canvas, x, "LABEL", (x->x_fsf.x_selected ? IEM_GUI_COLOR_SELECTED : x->x_lcol));
}

void g_iem_label_move(t_canvas* canvas, t_iemgui* x, int xpos, int ypos)
{
    const int z = x->x_glist->gl_zoom;

    g_text_move(canvas, x, "LABEL",
        xpos + x->x_ldx * z,
        ypos + x->x_ldy * z);
}

void g_iem_box_draw(t_canvas* canvas, t_iemgui* x, int xpos, int ypos)
{
    g_iem_brect_draw(canvas, x, xpos, ypos);
    g_iem_inlets_draw(canvas, x, xpos, ypos);
    g_iem_outlets_draw(canvas, x, xpos, ypos);
}

void g_iem_box_move(t_canvas* canvas, t_iemgui* x, int xpos, int ypos)
{
    const int z = x->x_glist->gl_zoom;
    const int iow = IOWIDTH * z;
    const int ioh = IEM_GUI_IOHEIGHT * z;

    g_rect_move(canvas, x, "BASE", xpos, ypos, x->x_w, x->x_h);

    if (!x->x_fsf.x_snd_able)
        g_rect_move(canvas, x, "OUT0", xpos, ypos + x->x_h, iow, z - ioh);

    if (!x->x_fsf.x_rcv_able)
        g_rect_move(canvas, x, "IN0", xpos, ypos, iow, ioh - z);
}

void g_iem_io_draw(t_canvas* canvas, t_iemgui* x, int xpos, int ypos, int old_snd_rcv_flags)
{
    const int z = x->x_glist->gl_zoom;
    const int iow = IOWIDTH * z;
    const int ioh = IEM_GUI_IOHEIGHT * z;

    if ((old_snd_rcv_flags & IEM_GUI_OLD_SND_FLAG) && !x->x_fsf.x_snd_able) {
        g_rect_draw_filled(canvas, x, "OUT0",
            xpos, ypos + x->x_h, iow, z - ioh,
            IEM_GUI_COLOR_XLET);
        /* keep above outlet */
        g_figure_raise(canvas, x, "LABEL", "OUT0");
    }

    if (!(old_snd_rcv_flags & IEM_GUI_OLD_SND_FLAG) && x->x_fsf.x_snd_able)
        g_figure_erase(canvas, x, "OUT0");

    if ((old_snd_rcv_flags & IEM_GUI_OLD_RCV_FLAG) && !x->x_fsf.x_rcv_able) {
        g_rect_draw_filled(canvas, x, "IN0",
            xpos, ypos, iow, ioh,
            IEM_GUI_COLOR_XLET);
        /* keep above inlet */
        g_figure_raise(canvas, x, "LABEL", "IN0");
    }

    if (!(old_snd_rcv_flags & IEM_GUI_OLD_RCV_FLAG) && x->x_fsf.x_rcv_able)
        g_figure_erase(canvas, x, "IN0");
}

void g_circle_draw_filled(t_canvas* canvas, void* x, const char* figure_id,
    int xpos, int ypos, int w, int h, int zoom, int color)
{
    sys_vgui(".x%lx.c create oval %d %d %d %d -width %d -fill #%6.6x -tags %lx_%s\n",
        canvas, xpos, ypos,
        xpos + w, ypos + h, zoom,
        color, x, figure_id);
}

void g_circle_move(t_canvas* canvas, void* x, const char* figure_id, int xpos, int ypos, int w, int h)
{
    sys_vgui(".x%lx.c coords %lx_%s %d %d %d %d\n",
        canvas, x, figure_id,
        xpos, ypos, xpos + w, ypos + h);
}

void g_figure_outline(t_canvas* canvas, void* x, const char* figure_id, int color)
{
    sys_vgui(".x%lx.c itemconfigure %lx_%s -outline #%6.6x\n", canvas, x, figure_id, color);
}

void g_figure_fill(t_canvas* canvas, void* x, const char* figure_id, int color)
{
    sys_vgui(".x%lx.c itemconfigure %lx_%s -fill #%6.6x\n",
        canvas, x, figure_id, color);
}

void g_figure_set_linewidth(t_canvas* canvas, void* x, const char* figure_id, int width)
{
    sys_vgui(".x%lx.c itemconfigure %lx_%s -width %d\n", canvas, x, figure_id, width);
}

void g_figure_erase(t_canvas* canvas, void* x, const char* figure_id)
{
    sys_vgui(".x%lx.c delete %lx_%s\n", canvas, x, figure_id);
}

void g_rect_draw_filled(t_canvas* canvas, void* x, const char* figure_id, int xpos, int ypos, int w, int h, int color)
{
    sys_vgui(".x%lx.c create rectangle %d %d %d %d -fill #%6.6x -tags %lx_%s\n",
        canvas, xpos, ypos, xpos + w, ypos + h, color, x, figure_id);
}

void g_rect_move(t_canvas* canvas, void* x, const char* figure_id, int xpos, int ypos, int w, int h)
{
    sys_vgui(".x%lx.c coords %lx_%s %d %d %d %d\n",
        canvas, x, figure_id,
        xpos, ypos, xpos + w, ypos + h);
}

void g_text_move(t_canvas* canvas, void* x, const char* figure_id, int xpos, int ypos)
{
    sys_vgui(".x%lx.c coords %lx_%s %d %d\n", canvas, x, figure_id, xpos, ypos);
}

void g_text_set(t_canvas* canvas, void* x, const char* figure_id, const char* txt)
{
    sys_vgui(".x%lx.c itemconfigure %lx_%s -text {%s} \n", canvas, x, figure_id, txt);
}

void g_text_color(t_canvas* canvas, void* x, const char* figure_id, int color)
{
    g_figure_fill(canvas, x, figure_id, color);
}

void g_text_font(t_canvas* canvas, void* x, const char* figure_id,
    const char* font, int size, const char* weight)
{
    sys_vgui(".x%lx.c itemconfigure %lx_%s -font {{%s} -%d %s} \n",
        canvas, x, figure_id, font, size, weight);
}

void g_figure_raise(t_canvas* canvas, void* x, const char* fig_upper, const char* fig_lower)
{
    sys_vgui(".x%lx.c raise %lx_%s %lx_%s\n", canvas, x, fig_upper, x, fig_lower);
}

void g_line_draw(t_canvas* canvas, void* x, const char* figure_id,
    int xpos0, int ypos0, int xpos1, int ypos1, int width, int color)
{
    sys_vgui(".x%lx.c create line %d %d %d %d -width %d -fill #%6.6x -tags %lx_%s\n",
        canvas,
        xpos0, ypos0, xpos1, ypos1,
        width, color, x, figure_id);
}

void g_line_move(t_canvas* canvas, void* x, const char* figure_id,
    int xpos0, int ypos0, int xpos1, int ypos1)
{
    sys_vgui(".x%lx.c coords %lx_%s %d %d %d %d\n",
        canvas, x, figure_id, xpos0, ypos0, xpos1, ypos1);
}

#endif // G_CEAMMC_DRAW_C
