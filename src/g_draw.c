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

#include "g_draw.h"
#include "g_draw_tcl.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define TAG_MAX_SIZE 32

struct pd_line {
    void* surface;
    t_pd_point p0;
    t_pd_point p1;
    t_pd_color color;
    char tag[TAG_MAX_SIZE];
    char hidden;
    float width;
};

static t_pd_color _PD_COLOR_BLACK = { 0, 0, 0 };
static t_pd_color _PD_COLOR_WHITE = { 255, 255, 255 };
static t_pd_color _PD_COLOR_BACKGROUND = { 200, 200, 200 };
static t_pd_color _PD_COLOR_FOREGROUND = { 250, 250, 250 };
static t_pd_color _PD_COLOR_ACTIVE = { 0, 200, 255 };
static t_pd_color _PD_COLOR_SELECTION = { 50, 50, 255 };

const t_pd_color* PD_COLOR_BLACK = &_PD_COLOR_BLACK;
const t_pd_color* PD_COLOR_WHITE = &_PD_COLOR_WHITE;
const t_pd_color* PD_COLOR_BACKGROUND = &_PD_COLOR_BACKGROUND;
const t_pd_color* PD_COLOR_FOREGROUND = &_PD_COLOR_FOREGROUND;
const t_pd_color* PD_COLOR_ACTIVE = &_PD_COLOR_ACTIVE;
const t_pd_color* PD_COLOR_SELECTION = &_PD_COLOR_SELECTION;

const char* pd_color_to_hex(t_pd_color* c)
{
    static const char* DIGITS = "0123456789ABCDEF";
    static char buf[8] = {};
    buf[0] = '#';
    buf[1] = DIGITS[c->r >> 0x4];
    buf[2] = DIGITS[c->r & 0xF];
    buf[3] = DIGITS[c->g >> 0x4];
    buf[4] = DIGITS[c->g & 0xF];
    buf[5] = DIGITS[c->b >> 0x4];
    buf[6] = DIGITS[c->b & 0xF];
    buf[6] = '\0';
    return buf;
}

t_pd_line* pd_line_create(t_pd_surface canvas, t_pd_point from, t_pd_point to)
{
    t_pd_line* l = (t_pd_line*)calloc(1, sizeof(t_pd_line));
    l->surface = canvas;
    l->p0 = from;
    l->p1 = to;
    l->width = 1;
    l->hidden = 1;
    return l;
}

void pd_line_destroy(t_pd_line* l)
{
    pd_line_erase(l);
    free(l);
}

void pd_line_set_width(t_pd_line* l, float w)
{
    l->width = w;
    pd_line_update_width(l);
}

float pd_line_get_width(t_pd_line* l)
{
    return l->width;
}

t_pd_color pd_line_get_color(t_pd_line* l)
{
    return l->color;
}

void pd_line_set_color(t_pd_line* l, t_pd_color c)
{
    l->color = c;
    pd_line_update_color(l);
}

void pd_line_set(t_pd_line* l, t_pd_point p0, t_pd_point p1)
{
    l->p0 = p0;
    l->p1 = p1;
    pd_line_update_pos(l);
}

void pd_line_get(t_pd_line* l, t_pd_point* p0, t_pd_point* p1)
{
    *p0 = l->p0;
    *p1 = l->p1;
}

void pd_line_move_x(t_pd_line* l, int offset)
{
    l->p0.x += offset;
    l->p1.x += offset;
    pd_line_update_pos(l);
}

void pd_line_move_y(t_pd_line* l, int offset)
{
    l->p0.y += offset;
    l->p1.y += offset;
    pd_line_update_pos(l);
}

void pd_line_move(t_pd_line* l, int x, int y)
{
    l->p0.x += x;
    l->p1.x += x;
    l->p0.y += y;
    l->p1.y += y;
    pd_line_update_pos(l);
}

const char* pd_line_get_tag(t_pd_line* l)
{
    return l->tag;
}

void pd_line_set_tag(t_pd_line* l, const char* tag)
{
    strncpy(l->tag, tag, TAG_MAX_SIZE - 1);
}

t_pd_surface pd_line_get_surface(t_pd_line* l)
{
    return l->surface;
}

void pd_line_draw(t_pd_line* l)
{
    if (l->hidden == 1) {
        l->hidden = 0;

#if defined(__APPLE__)
        tcl_line_draw(l);
#else
        tcl_line_draw(l);
#endif
    }
}

void pd_line_erase(t_pd_line* l)
{
    if (l->hidden == 0) {
        l->hidden = 1;

#if defined(__APPLE__)
        tcl_line_erase(l);
#else
        tcl_line_erase(l);
#endif
    }
}

void pd_line_update_color(t_pd_line* l)
{
#if defined(__APPLE__)
    tcl_line_update_color(l);
#else
    tcl_line_update_color(l);
#endif
}

void pd_line_update_width(t_pd_line* l)
{
#if defined(__APPLE__)
    tcl_line_update_width(l);
#else
    tcl_line_update_width(l);
#endif
}

void pd_line_update_pos(t_pd_line* l)
{
#if defined(__APPLE__)
    tcl_line_update_pos(l);
#else
    tcl_line_update_pos(l);
#endif
}
