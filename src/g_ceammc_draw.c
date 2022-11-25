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
        "-width %d -fill $::pd_colors::cord_normal -tags [list l%lx cord]\n",
        canvas,
        x0, y0, x1, y1,
        cordw * zoom, oc);
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
    sys_vgui(".x%lx.c itemconfigure l%lx -fill $::pd_colors::cord_selected\n", canvas, oc);
}

void g_cord_deselect(t_canvas* canvas, t_outconnect* oc)
{
    sys_vgui(".x%lx.c itemconfigure l%lx -fill $::pd_colors::cord_normal\n", canvas, oc);
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

enum {
    INT_BUFFER_SIZE = 11,
    MAX_VARS = 70,
    VAR_INT_BUFFER_SIZE = (INT_BUFFER_SIZE + 1) * MAX_VARS + 1
};

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

void g_message_click(t_canvas* canvas, const char* tag)
{
    sys_vgui(".x%lx.c itemconfigure %sR -width %d\n", canvas, tag, STYLE_BORDER_WIDTH_CLICKED);
    sys_vgui(".x%lx.c itemconfigure %sR -outline $::pd_colors::msg_clicked\n", canvas, tag);
}

void g_message_normal(t_canvas* canvas, const char* tag)
{
    sys_vgui(".x%lx.c itemconfigure %sR -width %d\n", canvas, tag, STYLE_BORDER_WIDTH * canvas->gl_zoom);
    sys_vgui(".x%lx.c itemconfigure %sR -outline $::pd_colors::obj_border\n", canvas, tag);
}

void g_message_draw(t_canvas* canvas, const char* tag, int x1, int y1, int x2, int y2, int corner)
{
    sys_vgui(".x%lx.c create polygon %d %d %d %d %d %d %d %d %d %d %d %d "
             "-width %d -fill $::pd_colors::obj_fill -outline $::pd_colors::obj_border  -tags [list %sR msg]\n",
        canvas,
        x1, y1, x2 + corner, y1, x2, y1 + corner, x2, y2 - corner, x2 + corner, y2, x1, y2,
        canvas->gl_zoom, tag);
}

void g_message_move(t_canvas* canvas, const char* tag, int x1, int y1, int x2, int y2, int corner)
{
    sys_vgui(".x%lx.c coords %sR %d %d %d %d %d %d %d %d %d %d %d %d %d %d\n",
        canvas, tag,
        x1, y1, x2 + corner, y1, x2, y1 + corner, x2, y2 - corner, x2 + corner, y2, x1, y2, x1, y1);
}

void g_listentry_draw(t_canvas* canvas, const char* tag, int x1, int y1, int x2, int y2, int corner, int grabbed)
{
    sys_vgui(".x%lx.c create polygon %d %d %d %d %d %d %d %d %d %d %d %d %d %d "
             "-width %d -fill $::pd_colors::obj_fill -outline $::pd_colors::obj_border -tags [list %sR msg]\n",
        canvas,
        x1, y1, x2 - corner, y1, x2, y1 + corner, x2, y2 - corner, x2 - corner, y2, x1, y2, x1, y1,
        canvas->gl_zoom, tag);
}

void g_listentry_move(t_canvas* canvas, const char* tag, int x1, int y1, int x2, int y2, int corner, int grabbed)
{
    sys_vgui(".x%lx.c coords %sR %d %d %d %d %d %d %d %d %d %d %d %d %d %d\n",
        canvas, tag,
        x1, y1, x2 - corner, y1, x2, y1 + corner, x2, y2 - corner, x2 - corner, y2, x1, y2, x1, y1);
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

#if defined(__APPLE__)
    if (obj_issignaloutlet(obj, idx)) {
        sys_vgui(".x%lx.c create rectangle %d %d %d %d "
                 "-tags [list %so%d outlet] -fill $::pd_colors::xlet_signal "
                 "-outline $::pd_colors::xlet_signal -width %d\n",
            canvas,
            x, y - oh,
            x + iow, y,
            tag, idx, zoom);
    } else {
        sys_vgui(".x%lx.c create rectangle %d %d %d %d "
                 "-tags [list %so%d outlet] -outline $::pd_colors::xlet_control -width %d\n",
            canvas,
            x, y - oh,
            x + iow, y,
            tag, idx, zoom);
    }
#else
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
#endif
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

#if defined(__APPLE__)
    if (obj_issignalinlet(obj, idx)) {
        sys_vgui(".x%lx.c create rectangle %d %d %d %d "
                 "-tags [list %si%d inlet] -fill $::pd_colors::xlet_signal "
                 "-outline $::pd_colors::xlet_signal -width %d\n",
            canvas, x, y, x + iow, y + ih - zoom, tag, idx, zoom);
    } else {
        sys_vgui(".x%lx.c create rectangle %d %d %d %d "
                 "-tags [list %si%d inlet] -outline $::pd_colors::xlet_control -width %d\n",
            canvas, x, y, x + iow, y + ih - zoom, tag, idx, zoom);
    }
#else
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
#endif
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
             " -dash %s -width %d -fill $::pd_colors::obj_fill -outline $::pd_colors::obj_border -tags [list %sR obj]\n",
        canvas, x1, y1, x2, y2, dash_pattern_str[p], zoom, tag);
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
    int x1, int y1, int x2, int y2, int corner, int grabbed)
{
    sys_vgui(".x%lx.c create polygon %d %d %d %d %d %d %d %d %d %d "
             "-width %d -fill $::pd_colors::obj_fill -outline $::pd_colors::obj_border -tags [list %sR atom]\n",
        canvas, x1, y1, x2 - corner, y1, x2, y1 + corner, x2, y2, x1, y2,
        canvas->gl_zoom, tag);
}

void g_xatom_move(t_canvas* canvas, const char* tag, int x1, int y1, int x2, int y2, int corner, int grabbed)
{
    sys_vgui(".x%lx.c coords %sR %d %d %d %d %d %d %d %d %d %d\n",
        canvas, tag,
        x1, y1, x2 - corner, y1, x2, y1 + corner, x2, y2, x1, y2);
}

void g_commentbar_draw(t_canvas* canvas, const char* tag, int x1, int y1, int x2, int y2)
{
    sys_vgui(".x%lx.c create rectangle %d %d %d %d "
             "-outline $::pd_colors::comment_border -width 1 -tags [list %sR commentbar]\n",
        canvas,
        x1, y1, x2, y2, tag);
}

void g_commentbar_move(t_canvas* canvas, const char* tag, int x1, int y1, int x2, int y2)
{
    sys_vgui(".x%lx.c coords %sR %d %d %d %d\n", canvas, tag, x1, y1, x2, y2);
}

void g_outlets_erase(t_canvas* canvas, const char* tag, int n)
{
    for (int i = 0; i < n; i++)
        sys_vgui(".x%lx.c delete %so%d\n", canvas, tag, i);
}

void g_inlets_erase(t_canvas* canvas, const char* tag, int n)
{
    for (int i = 0; i < n; i++)
        sys_vgui(".x%lx.c delete %si%d\n", canvas, tag, i);
}

void g_atom_label_move(t_canvas* canvas, t_gobj* obj, int x, int y)
{
    sys_vgui(".x%lx.c move %lx.l %d %d\n", canvas, obj, x, y);
}

void g_atom_label_erase(t_canvas* canvas, t_gobj* obj)
{
    sys_vgui(".x%lx.c delete %lx.l\n", canvas, obj);
}

void g_atom_label_draw(t_canvas* canvas, t_gobj* obj, int x, int y, const char* txt, int fontsize)
{
    sys_vgui("pdtk_text_new .x%lx.c {%lx.l label text} %f %f {%s } %d %s\n",
        canvas, obj, (double)x, (double)y, txt, fontsize,
        "$::pd_colors::obj_text");
}

void g_atom_erase(t_canvas* canvas, const char* tag)
{
    sys_vgui(".x%lx.c delete %sR\n", canvas, tag);
}

void g_commentbar_erase_all(t_canvas* canvas)
{
    sys_vgui(".x%lx.c delete commentbar\n", canvas);
}

#endif // G_CEAMMC_DRAW_C
