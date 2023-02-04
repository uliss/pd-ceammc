#include "mod_lang.h"

#ifdef WITH_LUA
#include "lang_luajit.h"
#endif

#ifndef FAUST_SEPARATE_EXTERNAL
#include "lang_faust_tilde.h"
#include "lang_faust_ui_tilde.h"
#endif

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
