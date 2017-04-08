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

#include "base/mod_base.h"
#include "conv/mod_conv.h"
#include "data/mod_data.h"
#include "dyn/mod_dyn.h"
#include "env/mod_env.h"
#include "flt/mod_flt.h"
#include "fx/mod_fx.h"
#include "lfo/mod_lfo.h"
#include "list/mod_list.h"
#include "math/mod_math.h"
#include "misc/mod_misc.h"
#include "noise/mod_noise.h"
#include "osc/mod_osc.h"
#include "path/mod_path.h"
#include "random/mod_random.h"
#include "symbol/mod_symbol.h"
#include "spat/mod_spat.h"
#include "system/mod_system.h"
#include "tl/mod_tl.h"
#include "ui/mod_ui.h"
#include "vector/mod_vector.h"

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

    ceammc_base_setup();
    ceammc_conv_setup();
    ceammc_data_setup();
    ceammc_dyn_setup();
    ceammc_env_setup();
    ceammc_flt_setup();
    ceammc_fx_setup();
    ceammc_lfo_setup();
    ceammc_list_setup();
    ceammc_math_setup();
    ceammc_misc_setup();
    ceammc_noise_setup();
    ceammc_path_setup();
    ceammc_osc_setup();
    ceammc_random_setup();
    ceammc_spat_setup();
    ceammc_symbol_setup();
    ceammc_system_setup();
    ceammc_tl_setup();
    ceammc_ui_setup();
    ceammc_vector_setup();
}

extern "C" int sys_zoomfontwidth(int fontsize, int, int)
{
    return sys_fontwidth(fontsize);
}

extern "C" int sys_zoomfontheight(int fontsize, int, int)
{
    return sys_fontheight(fontsize);
}
