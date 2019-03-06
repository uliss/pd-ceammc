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
#include "g_style.h"

#include "g_canvas.h"
#include "m_imp.h"

#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

// from https://www.tcl-lang.org/man/tcl/TkCmd/canvas.htm#M26
static const char* dash_pattern_str[] = {
    "\"\"", // no dash
    ".",
    "-"
};

void g_delete_all(t_canvas* c)
{
    sys_vgui(".x%lx.c delete all\n", c);
}

void g_gop_draw(t_canvas* canvas, int x0, int y0, int w, int h)
{
    sys_vgui(".x%lx.c create line %d %d %d %d %d %d %d %d %d %d "
             "-fill #%6.6x -width %d -capstyle projecting -tags GOP\n",
        canvas,
        x0, y0,
        x0 + w, y0,
        x0 + w, y0 + h,
        x0, y0 + h,
        x0, y0,
        STYLE_CANVAS_GOP_RECT_COLOR, 1);
}

void g_gop_erase(t_canvas* canvas)
{
    sys_vgui(".x%lx.c delete GOP\n", canvas);
}

void g_cord_draw(t_canvas* canvas, t_object* src, int outno, t_outconnect* oc,
    int x0, int y0, int x1, int y1, int zoom)
{
    int cordw = (obj_issignaloutlet(src, outno)
            ? STYLE_CORD_AUDIO_WIDTH
            : STYLE_CORD_CONTROL_WIDTH);

    sys_vgui(
        ".x%lx.c create line %d %d %d %d -capstyle round "
        "-width %d -fill #%6.6x -tags [list l%lx cord]\n",
        canvas,
        x0, y0, x1, y1,
        cordw * zoom, style_cord_color(), oc);
}

void g_selection_draw(t_canvas* canvas, int x0, int y0, int x1, int y1)
{
    sys_vgui(".x%lx.c create rectangle %d %d %d %d -dash . -tags x\n", canvas, x0, y0, x1, y1);
}

void g_selection_move(t_canvas* canvas, int x0, int y0, int x1, int y1)
{
    sys_vgui(".x%lx.c coords x %d %d %d %d\n", canvas, x0, y0, x1, y1);
}

void g_selection_clear(t_canvas* canvas)
{
    sys_vgui(".x%lx.c delete x\n", canvas);
}

void g_cord_select(t_canvas* canvas, t_outconnect* oc)
{
    sys_vgui(".x%lx.c itemconfigure l%lx -fill #%6.6x\n", canvas, oc, STYLE_CORD_SELECTED);
}

void g_cord_deselect(t_canvas* canvas, t_outconnect* oc)
{
    sys_vgui(".x%lx.c itemconfigure l%lx -fill #%6.6x\n", canvas, oc, STYLE_CORD_NORMAL);
}

void g_cord_erase(t_canvas* canvas, t_outconnect* oc)
{
    sys_vgui(".x%lx.c delete l%lx\n", canvas, oc);
}

void g_cord_move(t_canvas* canvas, t_outconnect* oc, int x0, int y0, int x1, int y1)
{
    sys_vgui(".x%lx.c coords l%lx %d %d %d %d\n", canvas, oc, x0, y0, x1, y1);
}

void g_cord_raise_all(t_canvas* canvas)
{
    sys_vgui(".x%lx.c raise cord\n", canvas);
}

void g_connection_draw(t_canvas* canvas, int x0, int y0, int x1, int y1, int issignal)
{
    sys_vgui(".x%lx.c create line %d %d %d %d -width %d -tags x\n",
        canvas, x0, y0, x1, y1, style_cord_width_by_type(issignal) * canvas->gl_zoom);
}

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

void g_iem_inlets_move(t_canvas* canvas, t_iemgui* x, int xpos, int ypos)
{
    const int z = x->x_glist->gl_zoom;
    const int iow = IOWIDTH * z;
    const int ioh = IEM_GUI_IOHEIGHT * z;

    if (!x->x_fsf.x_rcv_able)
        g_rect_move(canvas, x, "IN0", xpos, ypos, iow, ioh - z);
}

void g_iem_outlets_draw(t_canvas* canvas, t_iemgui* x, int xpos, int ypos)
{
    const int z = x->x_glist->gl_zoom;
    const int iow = IOWIDTH * z;
    const int ioh = IEM_GUI_IOHEIGHT * z;

    if (!x->x_fsf.x_snd_able)
        sys_vgui(".x%lx.c create rectangle %d %d %d %d -fill #%6.6x -tags [list %lx_OUT0 outlet]\n",
            canvas,
            xpos, ypos + x->x_h - ioh + z,
            xpos + iow, ypos + x->x_h, IEM_GUI_COLOR_XLET, x);
}

void g_iem_outlets_move(t_canvas* canvas, t_iemgui* x, int xpos, int ypos)
{
    const int z = x->x_glist->gl_zoom;
    const int iow = IOWIDTH * z;
    const int ioh = IEM_GUI_IOHEIGHT * z;

    if (!x->x_fsf.x_snd_able)
        g_rect_move(canvas, x, "OUT0", xpos, ypos + x->x_h, iow, z - ioh);
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

void g_iem_label_select(t_canvas* canvas, t_iemgui* x)
{
    g_figure_fill(canvas, x, "LABEL", IEM_GUI_COLOR_SELECTED);
}

void g_iem_label_unselect(t_canvas* canvas, t_iemgui* x)
{
    g_figure_fill(canvas, x, "LABEL", x->x_lcol);
}

void g_iem_box_draw(t_canvas* canvas, t_iemgui* x, int xpos, int ypos)
{
    g_iem_brect_draw(canvas, x, xpos, ypos);
    g_iem_inlets_draw(canvas, x, xpos, ypos);
    g_iem_outlets_draw(canvas, x, xpos, ypos);
}

void g_iem_box_move(t_canvas* canvas, t_iemgui* x, int xpos, int ypos)
{
    g_rect_move(canvas, x, "BASE", xpos, ypos, x->x_w, x->x_h);
    g_iem_io_move(canvas, x, xpos, ypos);
}

void g_iem_box_erase(t_canvas* canvas, t_iemgui* x)
{
    g_figure_erase(canvas, x, "BASE");
    g_figure_erase(canvas, x, "LABEL");
    g_iem_io_erase(canvas, x);
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

void g_iem_io_move(t_canvas* canvas, t_iemgui* x, int xpos, int ypos)
{
    g_iem_inlets_move(canvas, x, xpos, ypos);
    g_iem_outlets_move(canvas, x, xpos, ypos);
}

void g_figure_outfill(t_canvas* canvas, void* x, const char* figure_id, int color)
{
    sys_vgui(".x%lx.c itemconfigure %lx_%s -fill #%6.6x -outline #%6.6x \n",
        canvas, x, figure_id, color, color);
}

void g_iem_io_erase(t_canvas* canvas, t_iemgui* x)
{
    if (!x->x_fsf.x_snd_able)
        g_figure_erase(canvas, x, "OUT0");

    if (!x->x_fsf.x_rcv_able)
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

void g_rect_draw_filled(t_canvas* canvas, void* x, const char* figure_id,
    int xpos, int ypos, int w, int h, int color)
{
    sys_vgui(".x%lx.c create rectangle %d %d %d %d -fill #%6.6x -tags %lx_%s\n",
        canvas, xpos, ypos, xpos + w, ypos + h, color, x, figure_id);
}

void g_rect_draw_outlined(t_canvas* canvas, void* x, const char* figure_id,
    int xpos, int ypos, int w, int h, int line_wd, int color)
{
    sys_vgui(".x%lx.c create rectangle %d %d %d %d -width %d -outline #%6.6x -tags %lx_%s\n",
        canvas, xpos, ypos, xpos + w, ypos + h, line_wd, color, x, figure_id);
}

void g_rect_draw_outfilled(t_canvas* canvas, void* x, const char* figure_id,
    int xpos, int ypos, int w, int h, int color)
{
    sys_vgui(".x%lx.c create rectangle %d %d %d %d -fill #%6.6x -outline #%6.6x -tags %lx_%s\n",
        canvas, xpos, ypos, xpos + w, ypos + h, color, color, x, figure_id);
}

void g_rect_move(t_canvas* canvas, void* x, const char* figure_id, int xpos, int ypos, int w, int h)
{
    sys_vgui(".x%lx.c coords %lx_%s %d %d %d %d\n",
        canvas, x, figure_id,
        xpos, ypos, xpos + w, ypos + h);
}

void g_text_draw(t_canvas* canvas, void* x, const char* figure_id, int xpos, int ypos, const char* txt)
{
    sys_vgui(".x%lx.c create text %d %d -text {%s} -tags %lx_%s\n",
        canvas,
        xpos, ypos,
        txt,
        x, figure_id);
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

void g_text_anchor(t_canvas* canvas, void* x, const char* figure_id, const char* anchor)
{
    sys_vgui(".x%lx.c itemconfigure %lx_%s -anchor %s \n", canvas, x, figure_id, anchor);
}

void g_figure_raise(t_canvas* canvas, void* x, const char* fig_upper, const char* fig_lower)
{
    sys_vgui(".x%lx.c raise %lx_%s %lx_%s\n", canvas, x, fig_upper, x, fig_lower);
}

void g_figure_fill_n(t_canvas* canvas, void* x, const char* figure_tmp, int n, int color)
{
    char buf_id[64];
    for (int i = 0; i < n; i++) {
        snprintf(buf_id, sizeof(buf_id), figure_tmp, i);
        g_figure_fill(canvas, x, buf_id, color);
    }
}

void g_figure_outline_n(t_canvas* canvas, void* x, const char* figure_tmp, int n, int color)
{
    char buf_id[64];
    for (int i = 0; i < n; i++) {
        snprintf(buf_id, sizeof(buf_id), figure_tmp, i);
        g_figure_outline(canvas, x, buf_id, color);
    }
}

void g_figure_outfill_n(t_canvas* canvas, void* x, const char* figure_tmp, int n, int color)
{
    char buf_id[64];
    for (int i = 0; i < n; i++) {
        snprintf(buf_id, sizeof(buf_id), figure_tmp, i);
        g_figure_outfill(canvas, x, buf_id, color);
    }
}

void g_figure_erase_n(t_canvas* canvas, void* x, const char* figure_tmp, int n)
{
    char buf_id[64];
    for (int i = 0; i < n; i++) {
        snprintf(buf_id, sizeof(buf_id), figure_tmp, i);
        g_figure_erase(canvas, x, buf_id);
    }
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

static const char digit_pairs[201] = {
    "00010203040506070809"
    "10111213141516171819"
    "20212223242526272829"
    "30313233343536373839"
    "40414243444546474849"
    "50515253545556575859"
    "60616263646566676869"
    "70717273747576777879"
    "80818283848586878889"
    "90919293949596979899"
};

static const int INT_BUFFER_SIZE = 11;
static const int MAX_VARS = 70;
static const int VAR_INT_BUFFER_SIZE = (INT_BUFFER_SIZE + 1) * MAX_VARS + 1;

size_t g_int2str(int val, char* dest)
{
    if (!dest)
        return 0;

    char buf[INT_BUFFER_SIZE];
    char* it = &buf[INT_BUFFER_SIZE - 3];

    if (val >= 0) {
        int div = val / 100;
        while (div) {
            memcpy(it, &digit_pairs[2 * (val - div * 100)], 2);
            val = div;
            it -= 2;
            div = val / 100;
        }
        memcpy(it, &digit_pairs[2 * val], 2);
        if (val < 10)
            it++;
    } else {
        int div = val / 100;
        while (div) {
            memcpy(it, &digit_pairs[-2 * (val - div * 100)], 2);
            val = div;
            it -= 2;
            div = val / 100;
        }
        memcpy(it, &digit_pairs[-2 * val], 2);
        if (val <= -10)
            it--;
        *it = '-';
    }

    buf[INT_BUFFER_SIZE - 1] = '\0';
    strcpy(dest, it);
    return &buf[INT_BUFFER_SIZE - 1] - it;
}

static int g_va_int2str_helper(char* dest, size_t n, unsigned argc, va_list args)
{
    if (n == 0)
        return -1;

    char buf[INT_BUFFER_SIZE];
    size_t total_sz = 0;
    int rc = 0;

    for (int i = 0; i < argc; i++) {
        int ival = va_arg(args, int);
        size_t nchars = g_int2str(ival, buf);
        if (total_sz + nchars >= (n - 1)) {
            fputs("buffer size is too small\n", stderr);
            rc = -1;
            break;
        }

        strcpy(dest + total_sz, buf);
        total_sz += nchars;
        dest[total_sz++] = ' ';
        dest[total_sz] = '\0';
    }

    return rc;
}

int g_va_int2str(char* dest, size_t n, unsigned argc, ...)
{
    va_list args;
    va_start(args, argc);
    int rc = g_va_int2str_helper(dest, n, argc, args);
    va_end(args);
    return rc;
}

void g_figure_move_var(t_canvas* canvas, void* x, const char* figure_id, unsigned int ncoords, ...)
{
    va_list args;
    va_start(args, ncoords);

    char buf_int[VAR_INT_BUFFER_SIZE] = { 0 };
    int rc = g_va_int2str_helper(buf_int, VAR_INT_BUFFER_SIZE, ncoords, args);
    va_end(args);

    if (rc != 0)
        return;

    sys_vgui(".x%lx.c coords %lx_%s %s\n", canvas, x, figure_id, buf_int);
}

void g_line_draw_var(t_canvas* canvas, void* x, const char* figure_id, unsigned ncoords, ...)
{
    va_list args;
    va_start(args, ncoords);

    char buf_int[VAR_INT_BUFFER_SIZE] = { 0 };
    int rc = g_va_int2str_helper(buf_int, VAR_INT_BUFFER_SIZE, ncoords, args);

    va_end(args);

    if (rc != 0)
        return;

    sys_vgui(".x%lx.c create line %s -tags %lx_%s\n",
        canvas, buf_int, x, figure_id);
}

void g_polygon_draw(t_canvas* canvas, void* x, const char* figure_id, unsigned ncoords, ...)
{
    va_list args;
    va_start(args, ncoords);

    char buf_int[VAR_INT_BUFFER_SIZE] = { 0 };
    int rc = g_va_int2str_helper(buf_int, VAR_INT_BUFFER_SIZE, ncoords, args);

    va_end(args);

    if (rc != 0)
        return;

    sys_vgui(".x%lx.c create polygon %s -tags %lx_%s\n",
        canvas, buf_int, x, figure_id);
}

void g_message_click(t_canvas* canvas, const char* tag)
{
    sys_vgui(".x%lx.c itemconfigure %sR -width %d\n", canvas, tag, STYLE_BORDER_WIDTH_CLICKED);
    sys_vgui(".x%lx.c itemconfigure %sR -outline #%6.6x\n", canvas, tag, STYLE_BORDER_COLOR_CLICKED);
}

void g_message_normal(t_canvas* canvas, const char* tag)
{
    sys_vgui(".x%lx.c itemconfigure %sR -width %d\n", canvas, tag, STYLE_BORDER_WIDTH * canvas->gl_zoom);
    sys_vgui(".x%lx.c itemconfigure %sR -outline #%6.6x\n", canvas, tag, STYLE_BORDER_COLOR);
}

void g_message_draw(t_canvas* canvas, const char* tag, int x1, int y1, int x2, int y2, int corner)
{
    sys_vgui(".x%lx.c create polygon %d %d %d %d %d %d %d %d %d %d %d %d "
             "-width %d -outline #%6.6x -fill #%6.6x -tags [list %sR msg]\n",
        canvas,
        x1, y1, x2 + corner, y1, x2, y1 + corner, x2, y2 - corner, x2 + corner, y2,
        x1, y2,
        canvas->gl_zoom, STYLE_BORDER_COLOR, STYLE_FILL_COLOR, tag);
}

void g_message_move(t_canvas* canvas, const char* tag, int x1, int y1, int x2, int y2, int corner)
{
    sys_vgui(".x%lx.c coords %sR %d %d %d %d %d %d %d %d %d %d %d %d %d %d\n",
        canvas, tag,
        x1, y1, x2 + corner, y1, x2, y1 + corner, x2, y2 - corner, x2 + corner, y2,
        x1, y2, x1, y1);
}

void g_atom_select(t_canvas* canvas, const char* tag, int state)
{
    sys_vgui(".x%lx.c itemconfigure %sR -outline #%6.6x\n", canvas, tag,
        (state ? STYLE_SELECT_COLOR : STYLE_BORDER_COLOR));
}

void g_outlet_draw(t_canvas* canvas, t_object* obj, const char* tag, int idx, int x, int y, int zoom)
{
    int iow = IOWIDTH * zoom;
    int oh = (OHEIGHT - 1) * zoom;

    if (obj_issignaloutlet(obj, idx)) {
        sys_vgui(".x%lx.c create rectangle %d %d %d %d "
                 "-tags [list %so%d outlet] -fill #%6.6x "
                 "-outline #%6.6x -width %d\n",
            canvas,
            x, y - oh,
            x + iow, y,
            tag, idx, STYLE_AUDIO_XLET_COLOR, STYLE_AUDIO_XLET_COLOR, zoom);
    } else {
        sys_vgui(".x%lx.c create rectangle %d %d %d %d "
                 "-tags [list %so%d outlet] -fill #%6.6x -outline #%6.6x -width %d\n",
            canvas,
            x, y - oh,
            x + iow, y,
            tag, idx, STYLE_FILL_COLOR, STYLE_CONTROL_XLET_COLOR, zoom);
    }
}

void g_outlet_move(t_canvas* canvas, const char* tag, int idx, int x, int y, int zoom)
{
    int iow = IOWIDTH * zoom;
    int oh = (OHEIGHT - 1) * zoom;

    sys_vgui(".x%lx.c coords %so%d %d %d %d %d\n",
        canvas, tag, idx,
        x, y - oh, x + iow, y);
}

void g_inlet_draw(t_canvas* canvas, t_object* obj, const char* tag, int idx, int x, int y, int zoom)
{
    int iow = IOWIDTH * zoom;
    int ih = IHEIGHT * zoom;

    if (obj_issignalinlet(obj, idx)) {
        sys_vgui(".x%lx.c create rectangle %d %d %d %d "
                 "-tags [list %si%d inlet] -fill #%6.6x "
                 "-outline #%6.6x -width %d\n",
            canvas, x, y, x + iow, y + ih - zoom, tag, idx,
            STYLE_AUDIO_XLET_COLOR, STYLE_AUDIO_XLET_COLOR, zoom);
    } else {
        sys_vgui(".x%lx.c create rectangle %d %d %d %d "
                 "-tags [list %si%d inlet] -fill #%6.6x "
                 "-outline #%6.6x -width %d\n",
            canvas, x, y, x + iow, y + ih - zoom, tag, idx,
            STYLE_FILL_COLOR, STYLE_CONTROL_XLET_COLOR, zoom);
    }
}

void g_inlet_move(t_canvas* canvas, const char* tag, int idx, int x, int y, int zoom)
{
    int iow = IOWIDTH * zoom;
    int ih = OHEIGHT * zoom;

    sys_vgui(".x%lx.c coords %si%d %d %d %d %d\n",
        canvas, tag, idx,
        x, y, x + iow, y + ih - zoom);
}

void g_object_draw(t_canvas* canvas, const char* tag,
    int x1, int y1, int x2, int y2, int zoom, t_dash_pattern p)
{
    sys_vgui(".x%lx.c create rectangle %d %d %d %d "
             " -dash %s -width %d -fill #%6.6x -outline #%6.6x -tags [list %sR obj]\n",
        canvas, x1, y1, x2, y2, dash_pattern_str[p], zoom,
        STYLE_FILL_COLOR, STYLE_BORDER_COLOR, tag);
}

void g_object_move(t_canvas* canvas, const char* tag,
    int x1, int y1, int x2, int y2)
{
    sys_vgui(".x%lx.c coords %sR %d %d %d %d\n", canvas, tag, x1, y1, x2, y2);
}

void g_object_dash(t_canvas* canvas, const char* tag, t_dash_pattern p)
{
    sys_vgui(".x%lx.c itemconfigure %sR -dash %s\n", canvas, tag, dash_pattern_str[p]);
}

void g_xatom_draw(t_canvas* canvas, const char* tag,
    int x1, int y1, int x2, int y2, int corner, int zoom)
{
    sys_vgui(".x%lx.c create polygon %d %d %d %d %d %d %d %d %d %d "
             "-width %d -fill #%6.6x -outline #%6.6x -tags [list %sR atom]\n",
        canvas, x1, y1, x2 - corner, y1, x2, y1 + corner, x2, y2, x1, y2,
        zoom, STYLE_FILL_COLOR, STYLE_BORDER_COLOR, tag);
}

void g_xatom_move(t_canvas* canvas, const char* tag, int x1, int y1, int x2, int y2, int corner)
{
    sys_vgui(".x%lx.c coords %sR %d %d %d %d %d %d %d %d %d %d\n",
        canvas, tag,
        x1, y1, x2 - corner, y1, x2, y1 + corner, x2, y2, x1, y2);
}

#endif // G_CEAMMC_DRAW_C
