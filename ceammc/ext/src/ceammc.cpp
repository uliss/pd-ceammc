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

#include "list/mod_list.h"
#include "math/mod_math.h"
#include "noise/mod_noise.h"
#include "flt/mod_flt.h"

#include "m_pd.h"

static const char* ceammc_version = "0.1";
static t_class* ceammc_class;

static void* ceammc_new(t_symbol*)
{
    t_object* x = reinterpret_cast<t_object*>(pd_new(ceammc_class));
    if (x)
        post("CEAMMC extension library: %s. Build date: %s\n", ceammc_version, __DATE__);

    return x;
}

extern "C" void ceammc_setup()
{
    ceammc_class = class_new(gensym("ceammc"),
        reinterpret_cast<t_newmethod>(ceammc_new),
        reinterpret_cast<t_method>(pd_free),
        sizeof(t_object), CLASS_PD, A_NULL, 0);

    ceammc_new(NULL);

    ceammc_flt_setup();
    ceammc_list_setup();
    ceammc_math_setup();
    ceammc_noise_setup();
}

extern "C" int sys_zoomfontwidth(int fontsize, int, int)
{
    return sys_fontwidth(fontsize);
}

extern "C" int sys_zoomfontheight(int fontsize, int, int)
{
    return sys_fontheight(fontsize);
}
