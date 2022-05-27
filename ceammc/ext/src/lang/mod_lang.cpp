#include "mod_lang.h"

#ifdef WITH_LUA
#include "lang_luajit.h"
#endif

void ceammc_lang_setup()
{
#ifdef WITH_LUA
    setup_lang_luajit();
#endif
}
