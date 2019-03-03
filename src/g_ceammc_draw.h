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
#ifndef G_CEAMMC_DRAW_H
#define G_CEAMMC_DRAW_H

#if defined(_LANGUAGE_C_PLUS_PLUS) || defined(__cplusplus)
extern "C" {
#endif

#include "m_pd.h"

#include "g_all_guis.h"

// IEM
void g_iem_brect_draw(t_canvas* canvas, t_iemgui* x, int xpos, int ypos);
void g_iem_inlets_draw(t_canvas* canvas, t_iemgui* x, int xpos, int ypos);
void g_iem_outlets_draw(t_canvas* canvas, t_iemgui* x, int xpos, int ypos);
void g_iem_label_draw(t_canvas* canvas, t_iemgui* x, int xpos, int ypos);
void g_iem_label_config(t_canvas* canvas, t_iemgui* x);
void g_iem_label_move(t_canvas* canvas, t_iemgui* x, int xpos, int ypos);
void g_iem_box_draw(t_canvas* canvas, t_iemgui* x, int xpos, int ypos);
void g_iem_box_move(t_canvas* canvas, t_iemgui* x, int xpos, int ypos);
void g_iem_box_erase(t_canvas* canvas, t_iemgui* x);
void g_iem_io_draw(t_canvas* canvas, t_iemgui* x, int xpos, int ypos, int old_snd_rcv_flags);
void g_iem_io_move(t_canvas* canvas, t_iemgui* x, int xpos, int ypos);

// cirlce
void g_circle_draw_filled(t_canvas* canvas, void* x, const char* figure_id,
    int xpos, int ypos, int w, int h, int zoom, int color);
void g_circle_move(t_canvas* canvas, void* x, const char* figure_id, int xpos, int ypos, int w, int h);

// rect
void g_rect_draw_filled(t_canvas* canvas, void* x, const char* figure_id,
    int xpos, int ypos, int w, int h, int color);
void g_rect_move(t_canvas* canvas, void* x, const char* figure_id, int xpos, int ypos, int w, int h);

// text
void g_text_move(t_canvas* canvas, void* x, const char* figure_id, int xpos, int ypos);
void g_text_set(t_canvas* canvas, void* x, const char* figure_id, const char* txt);
void g_text_color(t_canvas* canvas, void* x, const char* figure_id, int color);
void g_text_font(t_canvas* canvas, void* x, const char* figure_id,
    const char* font, int size, const char* weight);

// line
void g_line_draw(t_canvas* canvas, void* x, const char* figure_id,
    int xpos0, int ypos0, int xpos1, int ypos1, int width, int color);
void g_line_move(t_canvas* canvas, void* x, const char* figure_id,
    int xpos0, int ypos0, int xpos1, int ypos1);

// all figures
void g_figure_set_linewidth(t_canvas* canvas, void* x, const char* figure_id, int width);
void g_figure_fill(t_canvas* canvas, void* x, const char* figure_id, int color);
void g_figure_outline(t_canvas* canvas, void* x, const char* figure_id, int color);
void g_figure_erase(t_canvas* canvas, void* x, const char* figure_id);
void g_figure_raise(t_canvas* canvas, void* x, const char* fig_upper, const char* fig_lower);

#if defined(_LANGUAGE_C_PLUS_PLUS) || defined(__cplusplus)
}
#endif

#endif // G_CEAMMC_DRAW_H
