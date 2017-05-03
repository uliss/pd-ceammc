#include "mod_math.h"

extern "C" void setup_math0x2eabs();
extern "C" void setup_math0x2eacos();
extern "C" void setup_math0x2eacosh();
extern "C" void setup_math0x2easin();
extern "C" void setup_math0x2easinh();
extern "C" void setup_math0x2eatan();
extern "C" void setup_math0x2eatanh();
extern "C" void setup_math0x2ecbrt();
extern "C" void setup_math0x2eceil();
extern "C" void setup_math0x2ecos();
extern "C" void setup_math0x2ecosh();
extern "C" void setup_math0x2ee();
extern "C" void setup_math0x2eexp();
extern "C" void setup_math0x2eexp2();
extern "C" void setup_math0x2efloor();
extern "C" void setup_math0x2einf();
extern "C" void setup_math0x2elog();
extern "C" void setup_math0x2elog10();
extern "C" void setup_math0x2elog2();
extern "C" void setup_math0x2enan();
extern "C" void setup_math0x2eneg();
extern "C" void setup_math0x2epi();
extern "C" void setup_math0x2ereciprocal();
extern "C" void setup_math0x2eround();
extern "C" void setup_math0x2esign();
extern "C" void setup_math0x2esin();
extern "C" void setup_math0x2esinh();
extern "C" void setup_math0x2esqrt();
extern "C" void setup_math0x2esquared();
extern "C" void setup_math0x2etan();
extern "C" void setup_math0x2etanh();
extern "C" void setup_math0x2etrunc();

extern "C" void setup_math0x2eand();
extern "C" void setup_math0x2eor();
extern "C" void setup_math0x2eaapprox();

void ceammc_math_setup()
{
    setup_math0x2eabs();
    setup_math0x2eacos();
    setup_math0x2eacosh();
    setup_math0x2easin();
    setup_math0x2easinh();
    setup_math0x2eatan();
    setup_math0x2eatanh();
    setup_math0x2ecbrt();
    setup_math0x2eceil();
    setup_math0x2ecos();
    setup_math0x2ecosh();
    setup_math0x2ee();
    setup_math0x2eexp();
    setup_math0x2eexp2();
    setup_math0x2efloor();
    setup_math0x2einf();
    setup_math0x2elog();
    setup_math0x2elog10();
    setup_math0x2elog2();
    setup_math0x2enan();
    setup_math0x2eneg();
    setup_math0x2epi();
    setup_math0x2ereciprocal();
    setup_math0x2eround();
    setup_math0x2esign();
    setup_math0x2esin();
    setup_math0x2esinh();
    setup_math0x2esqrt();
    setup_math0x2esquared();
    setup_math0x2etan();
    setup_math0x2etanh();
    setup_math0x2etrunc();

    setup_math0x2eand();
    setup_math0x2eor();
    setup_math0x2eaapprox();
}
