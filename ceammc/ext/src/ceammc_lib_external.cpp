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
#include "ceammc_impl.h"
#include "ceammc_object_info.h"
#include "ceammc_pd.h"
#include "mod_init.h"

#include "m_pd.h"

#include <algorithm>
#include <iostream>
#include <iterator>

t_class* ceammc_class = nullptr;

namespace {

inline int int_version(int maj, int min, int fix)
{
    return (maj << 20) | (min << 10) | fix;
}

void ceammc_info_message()
{
    post("\nCEAMMC library\n"
         "       Centre for Electroacoustic Music Moscow Conservatory, © 2016-2021\n"
         "       authors: Serge Poltavsky and Alex Nadzharov\n"
         "       arch: %d-bit\n"
         "       precision: %s\n"
         "       path: %s\n"
         "       version: %s\n"
         "       url: %s\n"
#ifdef PD_INSTANCE
         "        pd instance: true\n"
#else
         "        pd instance: false\n"
#endif
         "       license: GPL-3\n"
         "       build date: '%s'\n",
        sizeof(void*) * 8,
        std::is_same<t_float, float>::value ? "float" : "double",
        ceammc_class ? class_gethelpdir(ceammc_class) : "?",
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

    pd_bind(&x->te_g.g_pd, gensym("ceammc"));

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

void ceammc_postscript(t_object* x, t_symbol* path)
{
    post("ceammc_postscript: %s", path->s_name);
    sys_vgui("::ceammc::postscript {%s}\n", path->s_name);
}

void ceammc_tcl(t_object* x, t_symbol* s, int argc, t_atom* argv)
{
    for (int i = 0; i < argc; i++) {
        char buf[MAXPDSTRING];
        atom_string(argv + i, buf, MAXPDSTRING);
        sys_vgui("%s ", buf);
    }

    sys_gui("\n");
}

void ceammc_cords(t_object* x, t_symbol* s)
{
    if (s == gensym("lower"))
        sys_vgui("[tkcanvas_name $::focused_window] lower cord\n");
}

void ceammc_tcl_path_init()
{
    auto extern_dir = class_gethelpdir(ceammc_class);
    if (extern_dir)
        sys_vgui("lappend ::auto_path {%s}/tcl\n", extern_dir);
}
}

extern "C" CEAMMC_EXTERN int ceammc_init_done()
{
    return ceammc_class == nullptr ? 0 : 1;
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
    class_addmethod(ceammc_class,
        reinterpret_cast<t_method>(ceammc_postscript), gensym("postscript"), A_DEFSYMBOL, 0);
    class_addmethod(ceammc_class,
        reinterpret_cast<t_method>(ceammc_cords), gensym("cords"), A_DEFSYMBOL, 0);

    auto tcl = getenv("CEAMMC_TCL");
    if (tcl && tcl[0] == '1') {
        class_addmethod(ceammc_class,
            reinterpret_cast<t_method>(ceammc_tcl), gensym("tcl"), A_GIMME, 0);
    }

    if (!ceammc_class) {
        pd_error(0, "[ceammc] can't create library class");
        return;
    }

    ceammc_info_message();
    ceammc_tcl_path_init();
    ceammc_new();
    ceammc_init();
}

extern "C" CEAMMC_EXTERN void ceammc_list_externals(int vanilla)
{
    using namespace std;

    if (vanilla) {
        std::set<std::string> all_ext;

        for (auto& x : ceammc::pd::currentListOfExternals())
            all_ext.insert(x);

        std::set<std::string> vanilla_ext;
        auto& ceammc_ext = ceammc_ext_list();

        std::set_difference(all_ext.begin(), all_ext.end(),
            ceammc_ext.begin(), ceammc_ext.end(),
            std::inserter(vanilla_ext, vanilla_ext.begin()));

        vanilla_ext.erase("ceammc");

        std::copy(vanilla_ext.begin(), vanilla_ext.end(),
            std::ostream_iterator<std::string>(std::cout, "\n"));
    } else {
        auto& ceammc_set = ceammc_ext_list();

        std::copy(ceammc_set.begin(), ceammc_set.end(),
            std::ostream_iterator<std::string>(std::cout, "\n"));
    }
}

static void print_alias(t_class* c)
{
    using Os = ceammc::ObjectInfoStorage;

    if (Os::instance().hasInfo(c)) {
        auto& aliases = Os::instance().info(c).aliases;
        if (aliases.empty())
            return;

        std::cout << class_getname(c);
        for (auto& alias : aliases)
            std::cout << ' ' << alias;

        std::cout << std::endl;
    }
}

extern "C" CEAMMC_EXTERN void ceammc_list_aliases()
{
    using namespace std;
    using Os = ceammc::ObjectInfoStorage;

    for (auto* c : Os::instance().baseSet())
        print_alias(c);

    for (auto* c : Os::instance().uiSet())
        print_alias(c);
}
