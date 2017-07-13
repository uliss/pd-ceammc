#include "mod_misc.h"
#include "misc_mikmod.h"

extern "C" void setup_live0x2ecapture_tilde();

void ceammc_misc_setup()
{
    setup_live0x2ecapture_tilde();
    setup_misc0x2emikmod_tilde();
}
