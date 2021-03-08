#include "mod_lang.h"

void setup_lang_faust_tilde();
void setup_lang_faust_ui_tilde();

void ceammc_lang_setup()
{
#ifdef WITH_FAUST
    setup_lang_faust_tilde();
    setup_lang_faust_ui_tilde();
#endif
}
