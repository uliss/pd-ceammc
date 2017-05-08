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
#include "ceammc_canvas.h"
#include "ceammc_object.h"

#include "m_pd.h"

extern "C" {
#include "g_canvas.h"
#include "m_imp.h"
void pd_init();
}

using namespace ceammc;

Canvas::Canvas(t_canvas* c)
    : canvas_(c)
{
}

ArrayPtr Canvas::createArray(const char* name, size_t n)
{
    ArrayPtr ptr;
    if (!canvas_)
        return ptr;

    t_symbol* sname = gensym(name);
    t_garray* a = graph_array(canvas_, sname, &s_float, n, 0);
    if (!a)
        return ptr;

    ptr.reset(new Array(name));
    return ptr;
}

bool Canvas::connect(t_object* src, size_t nout, t_object* dest, size_t ninl)
{
    if (!src || !dest)
        return false;

    t_outconnect* c = obj_connect(src, nout, dest, ninl);
    return c != 0;
}

bool Canvas::connect(const BaseObject& src, size_t nout, BaseObject& dest, size_t ninl)
{
    return connect(src.owner(), nout, dest.owner(), ninl);
}
