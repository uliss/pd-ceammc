#include "mod_random.h"
#include "random_atom.h"
#include "random_discrete.h"
#include "random_float.h"
#include "random_gauss.h"
#include "random_int.h"
#include "random_linear.h"
#include "random_pwconst.h"
#include "random_pwlin.h"

void ceammc_random_setup()
{
    setup_random_atom();
    setup_random_discrete();
    setup_random_float();
    setup_random_gauss();
    setup_random_int();
    setup_random_linear();
    setup_random_pw_const();
    setup_random_pw_lin();
}
