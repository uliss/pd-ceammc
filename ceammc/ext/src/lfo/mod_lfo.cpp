#include "mod_lfo.h"

extern "C" void setup_lfo0x2eimpulse_tilde();
extern "C" void setup_lfo0x2esquare_tilde();
extern "C" void setup_lfo0x2etri_tilde();

void ceammc_lfo_setup() {
     setup_lfo0x2eimpulse_tilde();
     setup_lfo0x2esquare_tilde();
     setup_lfo0x2etri_tilde();
}
