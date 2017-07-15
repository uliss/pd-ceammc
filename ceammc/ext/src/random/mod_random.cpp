#include "mod_random.h"
#include "random_gauss.h"
#include "random_linear.h"

extern "C" void setup_random0x2efloat();
extern "C" void setup_random0x2eint();

void ceammc_random_setup()
{
    setup_random0x2efloat();
    setup_random0x2egauss();
    setup_random0x2eint();
    setup_random0x2elinear();
}
