/*****************************************************************************
 * Copyright 2021 Serge Poltavsky. All rights reserved.
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
#include "ext_load_lib.h"

#include <cstdio>

#include "m_pd.h"
extern "C" {
extern void pd_init();
#include "s_stuff.h"
}

constexpr const char* setup_sym = "ceammc_setup";
constexpr const char* sym_dump_json = "ceammc_dump_json";
constexpr const char* sym_list_all = "ceammc_list_externals";
constexpr const char* sym_list_methods = "ceammc_list_methods";
constexpr const char* sym_list_props = "ceammc_list_props";
constexpr const char* sym_list_aliases = "ceammc_list_aliases";

list_objects_fn list_objects = nullptr;
list_methods_fn list_methods = nullptr;
dump_json_fn dump_json = nullptr;
list_props_fn list_props = nullptr;
list_aliases_fn list_aliases = nullptr;

#ifdef _WIN32
#include <libgen.h>
#include <windows.h>

#ifndef MAXPDSTRING
constexpr size_t MAXPDSTRING = 1000;
#endif

bool load_ceammc()
{
    char dir[MAXPDSTRING] = CEAMMC_DLL;
    if (!SetDllDirectory(dirname(dir)))
        fprintf(stderr, "could not set '%s' as DllDirectory(), '%s' might not load.\n",
            dirname, basename);
    /* now load the DLL for the external */
    char base[MAXPDSTRING] = CEAMMC_DLL;
    auto ntdll = LoadLibrary(basename(base));
    if (!ntdll) {
        DWORD rc = GetLastError();
        LPSTR messageBuffer = NULL;
        size_t size = FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
            NULL, rc, MAKELANGID(LANG_ENGLISH, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL);

        fprintf(stderr, "%s: couldn't load - %s\n", CEAMMC_DLL, messageBuffer);
        //Free the buffer.
        LocalFree(messageBuffer);
        return false;
    }
    auto fn = (setup_fn)GetProcAddress(ntdll, setup_sym);
    SetDllDirectory(NULL); /* reset DLL dir to nothing */
    if (!fn)
        return false;

    fprintf(stderr, "%s\n", __FUNCTION__);
    (*fn)();
    fprintf(stderr, "%s 2\n", __FUNCTION__);
    list_objects = (list_objects_fn)GetProcAddress(ntdll, sym_list_all);
    if (!list_objects) {
        fprintf(stderr, "load_object: Symbol \"%s\" not found\n", sym_list_all);
        return false;
    }

    return true;
}
#else
#include <dlfcn.h>

bool load_ceammc()
{
    auto dlobj = dlopen(CEAMMC_DLL, RTLD_NOW | RTLD_GLOBAL);
    if (!dlobj) {
        fprintf(stderr, "%s: %s\n", CEAMMC_DLL, dlerror());
        return false;
    }

    auto fn = (setup_fn)dlsym(dlobj, setup_sym);
    if (!fn) {
        fprintf(stderr, "load_object: Symbol \"%s\" not found\n", setup_sym);
        return false;
    }

    (*fn)();

    list_objects = (list_objects_fn)dlsym(dlobj, sym_list_all);
    if (!list_objects) {
        fprintf(stderr, "load_object: Symbol \"%s\" not found\n", sym_list_all);
        return false;
    }

    list_methods = (list_methods_fn)dlsym(dlobj, sym_list_methods);
    if (!list_methods) {
        fprintf(stderr, "load_object: Symbol \"%s\" not found\n", sym_list_methods);
        return false;
    }

    dump_json = (dump_json_fn)dlsym(dlobj, sym_dump_json);
    if (!dump_json) {
        fprintf(stderr, "load_object: Symbol \"%s\" not found\n", sym_dump_json);
        return false;
    }

    list_props = (list_props_fn)dlsym(dlobj, sym_list_props);
    if (!list_props) {
        fprintf(stderr, "load_object: Symbol \"%s\" not found\n", sym_list_props);
        return false;
    }

    list_aliases = (list_aliases_fn)dlsym(dlobj, sym_list_aliases);
    if (!list_aliases) {
        fprintf(stderr, "load_object: Symbol \"%s\" not found\n", sym_list_aliases);
        return false;
    }

    if (!getenv("RAWWAVES"))
        setenv("RAWWAVES", STK_RAWWAVES, 1);

    return true;
}
#endif

bool init_pd(int sr)
{
    pd_init();
    sys_setchsr(1, 1, sr);
    return true;
}
