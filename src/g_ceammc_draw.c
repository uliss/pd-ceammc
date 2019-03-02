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

void g_ceammc_draw_brect(t_bng* x, int xpos, int ypos, t_canvas* canvas)
{
    g_rect_draw_filled(canvas, x, "BASE", xpos, ypos, x->x_gui.x_w, x->x_gui.x_h, x->x_gui.x_bcol);
    g_figure_set_linewidth(canvas, x, "BASE", IEMGUI_ZOOM(x));
}

void g_ceammc_draw_inlets(t_bng* x, int xpos, int ypos, t_canvas* canvas)
{
    int iow = IOWIDTH * IEMGUI_ZOOM(x);
    int ioh = IEM_GUI_IOHEIGHT * IEMGUI_ZOOM(x);

    if (!x->x_gui.x_fsf.x_rcv_able)
        sys_vgui(".x%lx.c create rectangle %d %d %d %d -fill black -tags [list %lx_IN0 inlet]\n",
            canvas,
            xpos, ypos,
            xpos + iow, ypos - IEMGUI_ZOOM(x) + ioh, x);
}

void g_ceammc_draw_outlets(t_bng* x, int xpos, int ypos, t_canvas* canvas)
{
    int iow = IOWIDTH * IEMGUI_ZOOM(x);
    int ioh = IEM_GUI_IOHEIGHT * IEMGUI_ZOOM(x);

    if (!x->x_gui.x_fsf.x_snd_able)
        sys_vgui(".x%lx.c create rectangle %d %d %d %d -fill black -tags [list %lx_OUT0 outlet]\n",
            canvas,
            xpos, ypos + x->x_gui.x_h + IEMGUI_ZOOM(x) - ioh,
            xpos + iow, ypos + x->x_gui.x_h,
            x);
}

void g_ceammc_draw_label(t_bng* x, int xpos, int ypos, const char* str, t_canvas* canvas)
{
    sys_vgui(".x%lx.c create text %d %d -text {%s} -anchor w \
             -font {{%s} -%d %s} -fill #%6.6x -tags [list %lx_LABEL label text]\n",
        canvas, xpos + x->x_gui.x_ldx * IEMGUI_ZOOM(x),
        ypos + x->x_gui.x_ldy * IEMGUI_ZOOM(x), str,
        x->x_gui.x_font, x->x_gui.x_fontsize * IEMGUI_ZOOM(x), sys_fontweight,
        x->x_gui.x_lcol, x);
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

#endif // G_CEAMMC_DRAW_C
