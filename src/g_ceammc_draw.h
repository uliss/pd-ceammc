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

void g_ceammc_draw_brect(t_bng* x, int xpos, int ypos, t_canvas* canvas);
void g_ceammc_draw_inlets(t_bng* x, int xpos, int ypos, t_canvas* canvas);
void g_ceammc_draw_outlets(t_bng* x, int xpos, int ypos, t_canvas* canvas);
void g_ceammc_draw_label(t_bng* x, int xpos, int ypos, const char* str, t_canvas* canvas);

void g_circle_draw_filled(t_canvas* canvas, void* x, const char* figure_id,
    int xpos, int ypos, int w, int h, int zoom, int color);
void g_circle_move(t_canvas* canvas, void* x, const char* figure_id, int xpos, int ypos, int w, int h);

void g_rect_draw_filled(t_canvas* canvas, void* x, const char* figure_id,
    int xpos, int ypos, int w, int h, int color);
void g_rect_move(t_canvas* canvas, void* x, const char* figure_id, int xpos, int ypos, int w, int h);

void g_figure_set_linewidth(t_canvas* canvas, void* x, const char* figure_id, int width);
void g_figure_fill(t_canvas* canvas, void* x, const char* figure_id, int color);
void g_figure_outline(t_canvas* canvas, void* x, const char* figure_id, int color);
void g_figure_erase(t_canvas* canvas, void* x, const char* figure_id);

#if defined(_LANGUAGE_C_PLUS_PLUS) || defined(__cplusplus)
}
#endif

#endif // G_CEAMMC_DRAW_H
