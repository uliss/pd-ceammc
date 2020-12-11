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

#include "ceammc.h"
#include "ceammc_config.h"
#include "mod_init.h"

#include "m_pd.h"

t_class* ceammc_class = 0;

namespace {

inline int int_version(int maj, int min, int fix)
{
    return (maj << 20) | (min << 10) | fix;
}

void ceammc_info_message()
{
    post("\nCEAMMC library\n"
         "       Centre for Electroacoustic Music Moscow Conservatory, © 2016-2020\n"
         "       authors: Serge Poltavsky and Alex Nadzharov\n"
         "       version: %s\n"
         "       url: %s\n"
         "       license: GPL-3\n"
         "       build date: '%s'\n",
        CEAMMC_LIB_VERSION, CEAMMC_LIB_HOME, __DATE__);

    int major, minor, fix;
    sys_getversion(&major, &minor, &fix);

    const auto runtime_version = int_version(major, minor, fix);
    const int compiled_version = int_version(PD_MAJOR_VERSION, PD_MINOR_VERSION, PD_BUGFIX_VERSION);

    if (runtime_version < compiled_version) {
        pd_error(0, "[ceammc] WARNING: running on Pd version (%d.%d.%d) "
                    "that is older then ceammc library was compiled for (%d.%d.%d). "
                    "Not all features can be supported.",
            major, minor, fix, PD_MAJOR_VERSION, PD_MINOR_VERSION, PD_BUGFIX_VERSION);
    }
}

void* ceammc_new()
{
    t_object* x = reinterpret_cast<t_object*>(pd_new(ceammc_class));
    if (!x) {
        pd_error(0, "[ceammc] load error");
        return NULL;
    }

    return x;
}

void ceammc_bang(t_object* x)
{
    auto obj_list = ceammc_ext_list();
    startpost("[ceammc] objects: ");
    std::string str;
    for (auto& o : obj_list) {
        str += '[';
        str += o;
        str += ']';
        poststring(str.c_str());
        str.clear();
    }

    endpost();
}
}

extern "C" CEAMMC_EXTERN void ceammc_setup()
{
    if (ceammc_class) {
        post("%s", "already loaded");
        return;
    }

    ceammc_class = class_new(gensym("ceammc"),
        reinterpret_cast<t_newmethod>(ceammc_new), 0,
        sizeof(t_object), CLASS_DEFAULT, A_NULL);

    class_addbang(ceammc_class, reinterpret_cast<t_method>(ceammc_bang));

    if (!ceammc_class) {
        pd_error(0, "[ceammc] can't create library class");
        return;
    }

    ceammc_info_message();
    ceammc_new();
    ceammc_init();
}
