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
#include "g_style.h"

#include "g_canvas.h"
#include "m_imp.h"

int style_cord_width(t_glist* x, t_object* ob, int nout)
{
    return (obj_issignaloutlet(ob, nout) ? STYLE_CORD_AUDIO_WIDTH : STYLE_CORD_CONTROL_WIDTH) * x->gl_zoom;
}

int style_border_width(t_glist* glist)
{
    return glist->gl_zoom;
    //    return STYLE_BORDER_WIDTH;
}

int style_xlet_height(t_glist* glist)
{
    //    return 1 - 2*glist->gl_zoom;
    return 1 + 1; // EXTRAPIX;
}

int style_cord_width_by_type(int issignal)
{
    return issignal ? STYLE_CORD_AUDIO_WIDTH : STYLE_CORD_CONTROL_WIDTH;
}
