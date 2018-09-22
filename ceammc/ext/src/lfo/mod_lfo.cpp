#include "mod_lfo.h"

extern "C" void setup_lfo0x2eimpulse_tilde();

extern void setup_lfo_pulse_tilde();
extern void setup_lfo_square_tilde();
extern void setup_lfo_square_pos_tilde();
extern void setup_lfo_tri_tilde();
extern void setup_lfo_tri_pos_tilde();

void ceammc_lfo_setup()
{
    setup_lfo0x2eimpulse_tilde();

    setup_lfo_pulse_tilde();
    setup_lfo_square_tilde();
    setup_lfo_square_pos_tilde();
    setup_lfo_tri_tilde();
    setup_lfo_tri_pos_tilde();
}
