#include "mod_spat.h"

extern "C" void setup_spat0x2epan2_tilde();
extern "C" void setup_spat0x2epan4_tilde();

void ceammc_spat_setup() {
     setup_spat0x2epan2_tilde();
     setup_spat0x2epan4_tilde();
}
