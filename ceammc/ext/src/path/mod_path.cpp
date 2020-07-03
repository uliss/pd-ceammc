#include "mod_path.h"

extern "C" void setup_path0x2ebasename();
extern "C" void setup_path0x2edirname();
void setup_path_lsdir();
void setup_path_exists();
void setup_path_is_dir();

void ceammc_path_setup()
{
    setup_path0x2ebasename();
    setup_path0x2edirname();
    setup_path_exists();
    setup_path_is_dir();
    setup_path_lsdir();
}
