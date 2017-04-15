#include "mod_fx.h"

extern "C" void setup_fx0x2edistortion_tilde();
extern "C" void setup_fx0x2eflanger_tilde();
extern "C" void setup_fx0x2efreeverb2_tilde();
extern "C" void setup_fx0x2efreeverb_tilde();
extern "C" void setup_fx0x2efreqshift_tilde();
extern "C" void setup_fx0x2epitchshift_tilde();
extern "C" void setup_fx0x2ezita_rev1_tilde();


void ceammc_fx_setup() {
     setup_fx0x2edistortion_tilde();
     setup_fx0x2eflanger_tilde();
     setup_fx0x2efreeverb2_tilde();
     setup_fx0x2efreeverb_tilde();
     setup_fx0x2efreqshift_tilde();
     setup_fx0x2epitchshift_tilde();
     setup_fx0x2ezita_rev1_tilde();
}
