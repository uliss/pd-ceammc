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
#include "ceammc_log.h"
#include "ceammc_object_info.h"
#include "ceammc_pd.h"
#include "ceammc_platform.h"
#include "ceammc_shared_rs.hpp"
#include "mod_init.h"
#include "tcl/ceammc_tcl.h"

#include "m_pd.h"

#include <algorithm>
#include <iostream>
#include <iterator>

// ceammc external class pointer
t_class* ceammc_class = nullptr;

// ceammc extern object struct
struct ceammc_external {
    t_object x_obj;
    ceammc_config* config;
};

namespace {

inline int int_version(int maj, int min, int fix)
{
    return (maj << 20) | (min << 10) | fix;
}

void ceammc_info_message()
{
    post("\nCEAMMC library\n"
         "Centre for Electroacoustic Music Moscow Conservatory, © 2016-2026\n"
         "    authors:    Serge Poltavsky and Alex Nadzharov\n"
         "    arch:       %d-bit\n"
         "    precision:  %s\n"
         "    version:    %s\n"
         "    url:        %s\n"
         "    license:    GPL-3\n"
         "    build date: '%s'\n",
        sizeof(void*) * 8,
        std::is_same<t_float, float>::value ? "float" : "double",
        CEAMMC_LIB_VERSION, CEAMMC_LIB_HOME, __DATE__);

    logpost(nullptr, PD_DEBUG, "[ceammc] path: %s", ceammc_class ? class_gethelpdir(ceammc_class) : "?");
#ifdef PD_INSTANCE
    logpost(nullptr, PD_DEBUG, "[ceammc] pd instance: true");
#else
    logpost(nullptr, PD_DEBUG, "[ceammc] pd instance: false");
#endif

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

ceammc_external* ceammc_new()
{
    auto ext = reinterpret_cast<ceammc_external*>(pd_new(ceammc_class));
    if (!ext) {
        pd_error(0, "[ceammc] load error");
        return NULL;
    }

    pd_bind(&ext->x_obj.te_g.g_pd, gensym("ceammc"));

    // load config
    ext->config = ceammc_config_load();
    if (!ext->config)
        ceammc::pdError(ext, "can't load external config");
    else
        ceammc::pdDebug(ext, "config file loaded");

    return ext;
}

void ceammc_free(t_object* x)
{
    auto ext = reinterpret_cast<ceammc_external*>(x);
    if (!ext)
        return;

    pd_unbind(&ext->x_obj.te_g.g_pd, gensym("ceammc"));

    // store config on exit
    if (ext->config) {
        ceammc_config_store(ext->config);
        if (ceammc_config_free(ext->config))
            ext->config = nullptr;
    }
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

void ceammc_keypress(t_object* x, t_symbol* s, int argc, t_atom* argv)
{
    auto sym = gensym("#ceammc_keypress");
    if (sym->s_thing)
        pd_list(sym->s_thing, s, argc, argv);
}

void ceammc_keyrelease(t_object* x, t_symbol* s, int argc, t_atom* argv)
{
    auto sym = gensym("#ceammc_keyrelease");
    if (sym->s_thing)
        pd_list(sym->s_thing, s, argc, argv);
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

t_symbol* ceammc_gen_doc_path(ceammc_config_lang lang)
{
    switch (lang) {
    case ceammc_config_lang::RUSSIAN: {
        char buf[MAXPDSTRING] = { 0 };
        snprintf(buf, sizeof(buf), "help-ru/%s", class_getname(ceammc_class));
        return gensym(buf);
    }
    case ceammc_config_lang::DEFAULT:
    case ceammc_config_lang::ENGLISH:
    default: {
        return gensym(class_getname(ceammc_class));
    }
    }
}

void ceammc_set_doc_lang(ceammc_external* ext, ceammc_config_lang lang)
{
    using namespace ceammc;

    if (!ext)
        return;

    switch (lang) {
    case ceammc_config_lang::RUSSIAN: {
        pdDebug(ext, "set documentation language to Russian");
        ObjectInfoStorage::instance().setDocLanguage(ObjectInfoStorage::Russian);
    } break;
    case ceammc_config_lang::DEFAULT:
    case ceammc_config_lang::ENGLISH:
    default: {
        pdDebug(ext, "set documentation language to English");
        ObjectInfoStorage::instance().setDocLanguage(ObjectInfoStorage::English);
    } break;
    }

    /// function name in ceammc_settings-plugin.tcl
    constexpr const char* TCL_SETTING_PLUGIN_SET_DOC_LANG = "::ceammc_prefs::set_doc_lang";

    pdgui_vmess(TCL_SETTING_PLUGIN_SET_DOC_LANG, "i", static_cast<int>(lang));

    class_sethelpsymbol(ext->x_obj.te_g.g_pd, ceammc_gen_doc_path(lang));
    ext->config->doc_lang = lang;
}

void ceammc_doc_lang(t_object* x, t_symbol* s)
{
    auto ext = reinterpret_cast<ceammc_external*>(x);

    auto lang = ceammc_config_lang::DEFAULT;
    if (!ceammc_config_parse_lang(s->s_name, &lang))
        pd_error(ext, "[ceammc] unknown lang '%s', setting to default", s->s_name);

    ceammc_set_doc_lang(ext, lang);
}

void ceammc_on_quit(t_object* x, t_symbol* s)
{
    auto ext = reinterpret_cast<ceammc_external*>(x);
    if (!ext)
        return;

    // store config on quit
    if (ext->config) {
        if (!ceammc_config_store(ext->config))
            pd_error(x, "[ceammc] can't store external config");
    }
}

void ceammc_on_any(t_object* x, t_symbol* s, int argc, t_atom* argv)
{
    auto ext = reinterpret_cast<ceammc_external*>(x);
    if (!ext)
        return;

    pd_error(x, "message '%s' is not supported, supported messages are:", s->s_name);
    for (auto m : { "doc", "load", "store" }) {
        pd_error(nullptr, " - %s", m);
    }
}

void ceammc_tcl_path_init()
{
    auto extern_dir = class_gethelpdir(ceammc_class);
    if (extern_dir)
        sys_vgui("lappend ::auto_path {%s/tcl}\n", extern_dir);
}

void ceammc_sync_config(ceammc_external* ext)
{
    if (ext)
        ceammc_set_doc_lang(ext, ext->config->doc_lang);
}

} // namespace

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
        reinterpret_cast<t_newmethod>(ceammc_new),
        reinterpret_cast<t_method>(ceammc_free),
        sizeof(ceammc_external), CLASS_DEFAULT, A_NULL);

    class_addbang(ceammc_class, reinterpret_cast<t_method>(ceammc_bang));
    class_addmethod(ceammc_class,
        reinterpret_cast<t_method>(ceammc_keypress), gensym("keypress"), A_GIMME, 0);
    class_addmethod(ceammc_class,
        reinterpret_cast<t_method>(ceammc_keyrelease), gensym("keyrelease"), A_GIMME, 0);
    class_addmethod(ceammc_class,
        reinterpret_cast<t_method>(ceammc_postscript), gensym("postscript"), A_DEFSYMBOL, 0);
    class_addmethod(ceammc_class,
        reinterpret_cast<t_method>(ceammc_cords), gensym("cords"), A_DEFSYMBOL, 0);
    class_addmethod(ceammc_class,
        reinterpret_cast<t_method>(ceammc_doc_lang), gensym("doc"), A_DEFSYMBOL, 0);
    class_addmethod(ceammc_class,
        reinterpret_cast<t_method>(ceammc_on_quit), gensym("verifyquit"), A_DEFSYMBOL, 0);
    class_addanything(ceammc_class,
        reinterpret_cast<t_method>(ceammc_on_any));

    auto tcl = ceammc::platform::get_env("CEAMMC_TCL");
    if (!tcl.empty() && tcl[0] == '1') {
        class_addmethod(ceammc_class,
            reinterpret_cast<t_method>(ceammc_tcl), gensym("tcl"), A_GIMME, 0);
    }

    if (!ceammc_class) {
        pd_error(0, "[ceammc] can't create library class");
        return;
    }

    // print load message
    ceammc_info_message();

    // add ceammc external tcl paths
    ceammc_tcl_path_init();
    // init core tcl functions
    ceammc::ceammc_tcl_init();

    // create ceammc external superobject and load config settings
    auto ext = ceammc_new();

    // other settings and init library objects
    ceammc_init();

    // sync loaded config settings
    // this should be done after all objects are loaded
    // for example: help dir change should be done here
    ceammc_sync_config(ext);
}

extern "C" CEAMMC_EXTERN void ceammc_list_externals(int vanilla)
{
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
    using Os = ceammc::ObjectInfoStorage;

    for (auto* c : Os::instance().baseSet())
        print_alias(c);

    for (auto* c : Os::instance().uiSet())
        print_alias(c);
}
