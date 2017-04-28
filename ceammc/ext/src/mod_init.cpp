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
#include "mod_init.h"
#include "lib/ceammc.hpp"
#include "s_stuff.h"

#include "array/mod_array.h"
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
#include "spat/mod_spat.h"
#include "symbol/mod_symbol.h"
#include "system/mod_system.h"
#include "tl/mod_tl.h"
#include "ui/mod_ui.h"
#include "vector/mod_vector.h"

#include <algorithm>
#include <set>
#include <string>
#include <vector>

std::set<std::string>& ceammc_ext_list()
{
    static std::set<std::string> instance_;
    return instance_;
}

static void setup_env_doc_path()
{
    if (!sys_libdir)
        return;

    std::string path(sys_libdir->s_name);
    path += "/doc";
    ceammc::set_env("DOC", path.c_str());
}

static void setup_env_ceammc_doc_path()
{
    if (!sys_libdir)
        return;

    std::string path(sys_libdir->s_name);
    path += "/extra/ceammc";
    ceammc::set_env("CEAMMC", path.c_str());
}

using namespace std;

void ceammc_init()
{
    // setup env variables
    setup_env_doc_path();
    setup_env_ceammc_doc_path();

    // save vanilla extension list
    vector<string> l = ceammc::currentExtensionList();
    set<string> vanilla_set(l.begin(), l.end());

    ceammc_array_setup();
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
    //    ceammc_spat_setup();
    ceammc_symbol_setup();
    ceammc_system_setup();
    ceammc_tl_setup();
    ceammc_ui_setup();
    ceammc_vector_setup();

    // get ceammc extension list
    l = ceammc::currentExtensionList();
    set<string> current_set(l.begin(), l.end());

    set_difference(current_set.begin(), current_set.end(),
        vanilla_set.begin(), vanilla_set.end(),
        inserter(ceammc_ext_list(), ceammc_ext_list().end()));
}
