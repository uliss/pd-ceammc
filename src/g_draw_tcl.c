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

#include "g_draw_tcl.h"
#include "m_pd.h"

#include <math.h>

void tcl_line_draw(t_pd_line* l)
{
    t_pd_surface s = pd_line_get_surface(l);
    t_pd_point p0, p1;
    pd_line_get(l, &p0, &p1);
    t_pd_color color = pd_line_get_color(l);
    float width = roundf(pd_line_get_width(l));
    const char* tag = pd_line_get_tag(l);

    sys_vgui(".x%lx.c create line %d %d %d %d -width %d -fill %s -tags {%s} \n", s,
        p0.x, p0.y, p1.x, p1.y, (int)width,
        pd_color_to_hex(&color), tag);
}

void tcl_line_erase(t_pd_line* l)
{
    t_pd_surface s = pd_line_get_surface(l);
    const char* tag = pd_line_get_tag(l);

    sys_vgui(".x%lx.c delete {%s}\n", s, tag);
}

void tcl_line_update_pos(t_pd_line* l)
{
    t_pd_surface s = pd_line_get_surface(l);
    const char* tag = pd_line_get_tag(l);

    sys_vgui(".x%lx.c coords {%s} %d %d %d %d\n", s, tag);
}

void tcl_line_update_color(t_pd_line* l)
{
    t_pd_surface s = pd_line_get_surface(l);
    const char* tag = pd_line_get_tag(l);
    t_pd_color color = pd_line_get_color(l);

    sys_vgui(".x%lx.c itemconfigure l%lx -fill %s\n", s, tag, pd_color_to_hex(&color));
}

void tcl_line_update_width(t_pd_line* l)
{
    t_pd_surface s = pd_line_get_surface(l);
    const char* tag = pd_line_get_tag(l);
    float width = roundf(pd_line_get_width(l));

    sys_vgui(".x%lx.c itemconfigure l%lx -width %d\n", s, tag, (int)width);
}
