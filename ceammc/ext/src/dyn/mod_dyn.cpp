#include "mod_dyn.h"

extern "C" void setup_dyn0x2egate2_tilde();
extern "C" void setup_dyn0x2egate_tilde();
extern "C" void setup_dyn0x2elimit2_tilde();
extern "C" void setup_dyn0x2elimit_tilde();
extern "C" void setup_dyn0x2esoftclip_tilde();

extern void setup_dyn_comp2_tilde();
extern void setup_dyn_comp_tilde();

void ceammc_dyn_setup()
{
    setup_dyn0x2egate2_tilde();
    setup_dyn0x2egate_tilde();
    setup_dyn0x2elimit2_tilde();
    setup_dyn0x2elimit_tilde();
    setup_dyn0x2esoftclip_tilde();

    setup_dyn_comp_tilde();
    setup_dyn_comp2_tilde();
}
