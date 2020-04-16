#include "mod_path.h"

extern "C" void setup_path0x2ebasename();
extern "C" void setup_path0x2edirname();
void setup_path_lsdir();
void setup_path_exists();

void ceammc_path_setup()
{
    setup_path0x2ebasename();
    setup_path0x2edirname();
    setup_path_lsdir();
    setup_path_exists();
}
