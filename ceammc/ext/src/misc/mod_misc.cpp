#include "mod_misc.h"

#ifdef HAVE_MIKMOD
#include "misc_mikmod.h"
#endif

extern "C" void setup_live0x2ecapture_tilde();

void ceammc_misc_setup()
{
    setup_live0x2ecapture_tilde();

#ifdef HAVE_MIKMOD
    setup_misc0x2emikmod_tilde();
#endif
}
