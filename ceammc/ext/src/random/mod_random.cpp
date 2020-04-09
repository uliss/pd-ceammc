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
    setup_random0x2egauss();
    setup_random0x2eint();
    setup_random_discrete();
    setup_random_float();
    setup_random_linear();
    setup_random_pw_const();
    setup_random_pw_lin();
}
