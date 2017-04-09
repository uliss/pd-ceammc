#include "mod_tl.h"

extern "C" void setup_tl0x2ebang();
extern "C" void setup_tl0x2ecue();
extern "C" void setup_tl0x2etransport();

void ceammc_tl_setup() {
     setup_tl0x2ebang();
     setup_tl0x2ecue();
     setup_tl0x2etransport();
}
