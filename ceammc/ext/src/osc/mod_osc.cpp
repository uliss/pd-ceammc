#include "mod_osc.h"

extern "C" void setup_osc0x2eimpulse_tilde();
extern "C" void setup_osc0x2epulse_tilde();

extern void setup_osc_blit();
extern void setup_osc_saw_tilde();
extern void setup_osc_saw4_tilde();
extern void setup_osc_sinfb_tilde();
extern void setup_osc_square_tilde();
extern void setup_osc_tri_tilde();

void ceammc_osc_setup()
{
    setup_osc0x2eimpulse_tilde();
    setup_osc0x2epulse_tilde();

    setup_osc_saw_tilde();
    setup_osc_saw4_tilde();
    setup_osc_sinfb_tilde();
    setup_osc_square_tilde();
    setup_osc_tri_tilde();
    setup_osc_blit();
}
