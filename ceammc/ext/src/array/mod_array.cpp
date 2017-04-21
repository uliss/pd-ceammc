#include "mod_array.h"

extern "C" void setup_array0x2erange();
extern "C" void setup_array0x2esample();
extern "C" void setup_array0x2ep2s();
extern "C" void setup_array0x2es2p();

void ceammc_array_setup()
{
    setup_array0x2ep2s();
    setup_array0x2es2p();
    setup_array0x2erange();
    setup_array0x2esample();
}
