#include "mod_vector.h"

extern "C" void setup_vector0x2edistance();
extern "C" void setup_vector0x2edot();
extern "C" void setup_vector0x2elength();
extern "C" void setup_vector0x2enormalize();

void ceammc_vector_setup() {
     setup_vector0x2edistance();
     setup_vector0x2edot();
     setup_vector0x2elength();
     setup_vector0x2enormalize();
}
