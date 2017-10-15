#include "mod_env.h"

extern "C" void setup_env0x2eadsr_tilde();
extern "C" void setup_env0x2ear_tilde();
extern "C" void setup_env0x2efollow_tilde();

void ceammc_env_setup() {
     setup_env0x2eadsr_tilde();
     setup_env0x2ear_tilde();
     setup_env0x2efollow_tilde();
}
