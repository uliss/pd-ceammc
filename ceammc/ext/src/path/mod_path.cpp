#include "mod_path.h"

extern "C" void setup_path0x2ebasename();
extern "C" void setup_path0x2edirname();
extern "C" void setup_path0x2elsdir();
extern "C" void setup_path0x2eexists();

void ceammc_path_setup()
{
    setup_path0x2ebasename();
    setup_path0x2edirname();
    setup_path0x2elsdir();
    setup_path0x2eexists();
}
