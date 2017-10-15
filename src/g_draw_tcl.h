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
#ifndef G_DRAW_TCL_H
#define G_DRAW_TCL_H

#include "g_draw.h"

void tcl_line_draw(t_pd_line* l);
void tcl_line_erase(t_pd_line* l);
void tcl_line_update_color(t_pd_line* l);
void tcl_line_update_pos(t_pd_line* l);
void tcl_line_update_width(t_pd_line* l);

#endif // G_DRAW_TCL_H
