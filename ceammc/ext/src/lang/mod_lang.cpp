#include "mod_lang.h"

#ifdef WITH_LUA
#include "lang_luajit.h"
#endif

void setup_lang_faust_non_external();
void setup_ui_faust_non_external();

void ceammc_lang_setup()
{
#ifdef WITH_LUA
    setup_lang_luajit();
#endif

#ifndef FAUST_SEPARATE_EXTERNAL
    setup_lang_faust_non_external();
    setup_ui_faust_non_external();
#endif
}
