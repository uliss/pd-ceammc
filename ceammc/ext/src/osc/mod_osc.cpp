#include "mod_osc.h"

extern "C" void setup_osc0x2eimpulse_tilde();
extern "C" void setup_osc0x2epulse_tilde();
extern "C" void setup_osc0x2esaw_tilde();
extern "C" void setup_osc0x2esinfb_tilde();
extern "C" void setup_osc0x2esquare_tilde();
extern "C" void setup_osc0x2etri_tilde();

void ceammc_osc_setup() {
     setup_osc0x2eimpulse_tilde();
     setup_osc0x2epulse_tilde();
     setup_osc0x2esaw_tilde();
     setup_osc0x2esinfb_tilde();
     setup_osc0x2esquare_tilde();
     setup_osc0x2etri_tilde();
}
