#include "mod_osc.h"

extern void setup_osc_blit_tilde();
extern void setup_osc_impulse_tilde();
extern void setup_osc_pulse_tilde();
extern void setup_osc_saw4_tilde();
extern void setup_osc_saw_tilde();
extern void setup_osc_sin_tilde();
extern void setup_osc_sinfb_tilde();
extern void setup_osc_square_tilde();
extern void setup_osc_tri_tilde();

void ceammc_osc_setup()
{
    setup_osc_blit_tilde();
    setup_osc_impulse_tilde();
    setup_osc_pulse_tilde();
    setup_osc_saw4_tilde();
    setup_osc_saw_tilde();
    setup_osc_sin_tilde();
    setup_osc_sinfb_tilde();
    setup_osc_square_tilde();
    setup_osc_tri_tilde();
}
