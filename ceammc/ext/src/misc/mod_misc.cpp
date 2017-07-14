#include "mod_misc.h"

//#ifdef CEAMMC_HAVE_MODPLUG
#include "misc_mikmod.h"
//#endif

extern "C" void setup_live0x2ecapture_tilde();

void ceammc_misc_setup()
{
    setup_live0x2ecapture_tilde();

    //#ifdef CEAMMC_HAVE_MODPLUG
    setup_misc0x2emikmod_tilde();
    //#endif
}
