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

typedef enum dash_pattern {
    DASH_PATTERN_SOLID = 0, // no dash
    DASH_PATTERN_2_4, // two dots, four spaces: -  -  -
    DASH_PATTERN_6_4
} t_dash_pattern;

void g_delete_all(t_canvas* c);

// cords
void g_cord_draw(t_canvas* canvas, t_object* src, int outno, t_outconnect* oc,
    int x0, int y0, int x1, int y1, int zoom);
void g_cord_select(t_canvas* canvas, t_outconnect* oc);
void g_cord_deselect(t_canvas* canvas, t_outconnect* oc);
void g_cord_erase(t_canvas* canvas, t_outconnect* oc);
void g_cord_move(t_canvas* canvas, t_outconnect* oc, int x0, int y0, int x1, int y1);
void g_cord_raise_all(t_canvas* canvas);

// connection
void g_connection_draw(t_canvas* canvas, int x0, int y0, int x1, int y1, int issignal);
void g_selection_draw(t_canvas* canvas, int x0, int y0, int x1, int y1);
void g_selection_move(t_canvas* canvas, int x0, int y0, int x1, int y1);
void g_selection_clear(t_canvas* canvas);

// GOP (graph on parent)
void g_gop_draw(t_canvas* canvas, int x0, int y0, int x1, int y1);
void g_gop_erase(t_canvas* canvas);

// atoms
void g_atom_select(t_canvas* canvas, const char* tag, int state);
void g_atom_label_draw(t_canvas* canvas, t_gobj* obj, int x, int y, const char* txt, int fontsize);
void g_atom_label_move(t_canvas* canvas, t_gobj* obj, int x, int y);
void g_atom_label_erase(t_canvas* canvas, t_gobj* obj);
void g_atom_erase(t_canvas* canvas, const char* tag);
// messages
void g_message_click(t_canvas* canvas, const char* tag);
void g_message_normal(t_canvas* canvas, const char* tag);
void g_message_draw(t_canvas* canvas, const char* tag, int x1, int y1, int x2, int y2, int corner);
void g_message_move(t_canvas* canvas, const char* tag, int x1, int y1, int x2, int y2, int corner);
// list entry
void g_listentry_draw(t_canvas* canvas, const char* tag, int x1, int y1, int x2, int y2, int corner, int grabbed);
void g_listentry_move(t_canvas* canvas, const char* tag, int x1, int y1, int x2, int y2, int corner, int grabbed);
// objects
void g_object_draw(t_canvas* canvas, const char* tag,
    int x1, int y1, int x2, int y2, int zoom, t_dash_pattern p);
void g_object_move(t_canvas* canvas, const char* tag, int x1, int y1, int x2, int y2);
void g_object_dash(t_canvas* canvas, const char* tag, t_dash_pattern p);
// xlets
void g_outlet_draw(t_canvas* canvas, t_object* obj, const char* tag, int idx, int x, int y, int zoom);
void g_outlet_move(t_canvas* canvas, const char* tag, int idx, int x, int y, int zoom);
void g_inlet_draw(t_canvas* canvas, t_object* obj, const char* tag, int idx, int x, int y, int zoom);
void g_inlet_move(t_canvas* canvas, const char* tag, int idx, int x, int y, int zoom);
void g_outlets_erase(t_canvas* canvas, const char* tag, int n);
void g_inlets_erase(t_canvas* canvas, const char* tag, int n);
// float and symbol atoms
void g_xatom_draw(t_canvas* canvas, const char* tag, int x1, int y1, int x2, int y2, int corner, int grabbed);
void g_xatom_move(t_canvas* canvas, const char* tag, int x1, int y1, int x2, int y2, int corner, int grabbed);
// comment bar in edit mode
void g_commentbar_draw(t_canvas* canvas, const char* tag, int x1, int y1, int x2, int y2);
void g_commentbar_move(t_canvas* canvas, const char* tag, int x1, int y1, int x2, int y2);
void g_commentbar_erase_all(t_canvas* canvas);

size_t g_int2str(int v, char* dest);
int g_va_int2str(char* dest, size_t n, unsigned argc, ...);

#if defined(_LANGUAGE_C_PLUS_PLUS) || defined(__cplusplus)
}
#endif

#endif // G_CEAMMC_DRAW_H
