#include "mod_array.h"

extern "C" void setup_array0x2ecopy();
extern "C" void setup_array0x2edo();
extern "C" void setup_array0x2eeach();
extern "C" void setup_array0x2efill();
extern "C" void setup_array0x2eminmax();
extern "C" void setup_array0x2ep2s();
extern "C" void setup_array0x2es2p();

void ceammc_array_setup()
{
    setup_array0x2ecopy();
    setup_array0x2edo();
    setup_array0x2eeach();
    setup_array0x2efill();
    setup_array0x2eminmax();
    setup_array0x2ep2s();
    setup_array0x2es2p();
}
