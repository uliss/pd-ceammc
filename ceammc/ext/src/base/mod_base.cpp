#include "mod_base.h"

extern "C" void msg_setup();
extern "C" void setup_prop0x2eget();
extern "C" void setup_prop0x2eset();

void ceammc_base_setup() {
     msg_setup();
     setup_prop0x2eget();
     setup_prop0x2eset();
}
