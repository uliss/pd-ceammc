/*****************************************************************************
 * Copyright 2017 Serge Poltavsky. All rights reserved.
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

#ifndef PD_G_DRAW_H_
#define PD_G_DRAW_H_

#if defined(__cplusplus)
extern "C" {
#endif

typedef struct pd_point {
    int x;
    int y;
} t_pd_point;

struct pd_color {
    unsigned char r;
    unsigned char g;
    unsigned char b;
};

typedef struct pd_color t_pd_color;

extern const t_pd_color* PD_COLOR_BLACK;
extern const t_pd_color* PD_COLOR_WHITE;
extern const t_pd_color* PD_COLOR_BACKGROUND;
extern const t_pd_color* PD_COLOR_FOREGROUND;
extern const t_pd_color* PD_COLOR_ACTIVE;
extern const t_pd_color* PD_COLOR_SELECTION;

const char* pd_color_to_hex(t_pd_color* c);

/// LINE
typedef struct pd_line t_pd_line;
typedef void* t_pd_surface;

// constuct/destruct
t_pd_line* pd_line_create(t_pd_surface canvas, t_pd_point from, t_pd_point to);
void pd_line_destroy(t_pd_line* l);
// surface
t_pd_surface pd_line_get_surface(t_pd_line* l);
// width
float pd_line_get_width(t_pd_line* l);
void pd_line_set_width(t_pd_line* l, float w);
// color
t_pd_color pd_line_get_color(t_pd_line* l);
void pd_line_set_color(t_pd_line* l, t_pd_color c);
// position
void pd_line_get(t_pd_line* l, t_pd_point* p0, t_pd_point* p1);
void pd_line_set(t_pd_line* l, t_pd_point p0, t_pd_point p1);
void pd_line_move_x(t_pd_line* l, int offset);
void pd_line_move_y(t_pd_line* l, int offset);
void pd_line_move(t_pd_line* l, int x, int y);
// tag
const char* pd_line_get_tag(t_pd_line* l);
void pd_line_set_tag(t_pd_line* l, const char* tag);

// draw
void pd_line_draw(t_pd_line* l);
void pd_line_erase(t_pd_line* l);
void pd_line_update_pos(t_pd_line* l);
void pd_line_update_color(t_pd_line* l);
void pd_line_update_width(t_pd_line* l);

#if defined(__cplusplus)
}
#endif

#endif // PD_G_DRAW_H_
