#include "mod_random.h"
#include "random_discrete.h"
#include "random_float.h"
#include "random_gauss.h"
#include "random_linear.h"
#include "random_pwconst.h"
#include "random_pwlin.h"

extern "C" void setup_random0x2eint();

void ceammc_random_setup()
{
    setup_random0x2efloat();
    setup_random0x2egauss();
    setup_random0x2eint();

    setup_random0x2ediscrete();
    setup_random0x2elinear();
    setup_random0x2epw_const();
    setup_random0x2epw_lin();
}
