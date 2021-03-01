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

constexpr const char* setup_sym = "ceammc_setup";
constexpr const char* sym_dump_json = "ceammc_dump_json";
constexpr const char* sym_list_all = "ceammc_list_all";
constexpr const char* sym_list_methods = "ceammc_list_methods";
constexpr const char* sym_list_props = "ceammc_list_props";

fn_type list_objects = nullptr;
list_methods_fn list_methods = nullptr;
dump_json_fn dump_json = nullptr;
list_props_fn list_props = nullptr;

#ifdef _WIN32
#include <libgen.h>

bool load_ceammc()
{
    char dir[MAXPDSTRING] = CEAMMC_LIB;
    if (!SetDllDirectory(dirname(dir)))
        error("could not set '%s' as DllDirectory(), '%s' might not load.",
            dirname, basename);
    /* now load the DLL for the external */
    char base[MAXPDSTRING] = CEAMMC_LIB;
    auto ntdll = LoadLibrary(basename(base));
    if (!ntdll) {
        DWORD rc = GetLastError();
        LPSTR messageBuffer = NULL;
        size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
            NULL, rc, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL);

        error("%s: couldn't load - %s", CEAMMC_LIB, messageBuffer);
        //Free the buffer.
        LocalFree(messageBuffer);
        return false;
    }
    auto fn = (fn_type)GetProcAddress(ntdll, setup_sym);
    SetDllDirectory(NULL); /* reset DLL dir to nothing */
    if (!fn)
        return false;

    (*fn)();

    list_objects = (fn_type)GetProcAddress(ntdll, sym_list_all);
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

    auto fn = (fn_type)dlsym(dlobj, setup_sym);
    if (!fn) {
        fprintf(stderr, "load_object: Symbol \"%s\" not found\n", setup_sym);
        return false;
    }

    (*fn)();

    list_objects = (fn_type)dlsym(dlobj, sym_list_all);
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

    if (!getenv("RAWWAVES"))
        setenv("RAWWAVES", STK_RAWWAVES, 1);

    return true;
}
#endif
