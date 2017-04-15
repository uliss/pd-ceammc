#include "mod_fx.h"

extern "C" void setup_fx0x2edistortion_tilde();
extern "C" void setup_fx0x2efreeverb2_tilde();
extern "C" void setup_fx0x2efreeverb_tilde();
extern "C" void setup_fx0x2efreqshift_tilde();
extern "C" void setup_fx0x2epitchshift_tilde();


void ceammc_fx_setup() {
     setup_fx0x2edistortion_tilde();
     setup_fx0x2efreeverb2_tilde();
     setup_fx0x2efreeverb_tilde();
     setup_fx0x2efreqshift_tilde();
     setup_fx0x2epitchshift_tilde();
}
